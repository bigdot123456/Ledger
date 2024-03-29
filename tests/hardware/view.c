/*******************************************************************************
*   (c) 2018, 2019 ZondaX GmbH
*   (c) 2016 Ledger
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
********************************************************************************/

#include "view.h"
#include "view_internal.h"

#include "actions.h"
#include "apdu_codes.h"
#include "glyphs.h"
#include "bagl.h"
#include "zxmacros.h"
#include "view_templates.h"
#include "transaction.h"

#include <string.h>
#include <stdio.h>

view_t viewdata;

void h_address_accept(unsigned int _) {
    UNUSED(_);
    app_reply_address();

    view_idle_show(0);
}

void h_sign_accept(unsigned int _) {
    UNUSED(_);

    const uint8_t replyLen = app_sign();

    view_idle_show(0);

    set_code(G_io_apdu_buffer, replyLen, APDU_CODE_OK);
    io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, replyLen + 2);
}

void h_sign_reject(unsigned int _) {
    UNUSED(_);

    set_code(G_io_apdu_buffer, 0, APDU_CODE_COMMAND_NOT_ALLOWED);
    io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, 2);

    view_idle_show(0);
}

void h_review_init() {
    viewdata.idx = 0;
    viewdata.pageIdx = 0;
    viewdata.pageCount = 1;
}

void h_review_increase() {
    viewdata.pageIdx++;
    if (viewdata.pageIdx >= viewdata.pageCount) {
        viewdata.idx++;
        viewdata.pageIdx = 0;
    }
}

void h_review_decrease() {
    viewdata.pageIdx--;
    if (viewdata.pageIdx < 0) {
        viewdata.idx--;
        viewdata.pageIdx = 0;
    }
}


inline void splitValueField() {
#if defined(TARGET_NANOS)
    print_value2("");
    uint16_t vlen = strlen(viewdata.value);
    if (vlen > MAX_CHARS_PER_VALUE2_LINE - 1) {
        strcpy(viewdata.value2, viewdata.value + MAX_CHARS_PER_VALUE_LINE);
        viewdata.value[MAX_CHARS_PER_VALUE_LINE] = 0;
    }
#endif
}

int8_t h_review_update_data() {
    int8_t err = TX_NO_ERROR;

    do {
        err = transaction_getItem(viewdata.idx,
                                  viewdata.key, MAX_CHARS_PER_KEY_LINE,
                                  viewdata.value, MAX_CHARS_PER_VALUE1_LINE,
                                  viewdata.pageIdx, &viewdata.pageCount);

        if (err == TX_NO_MORE_DATA) {
            return TX_NO_MORE_DATA;
        }

        if (viewdata.pageCount == 0) {
            h_review_increase();
        }
    } while(viewdata.pageCount == 0);

    if (err != TX_NO_ERROR) {
        print_key("Error");
        print_value("%d", err);
        return err;
    }

    splitValueField();
    return TX_NO_ERROR;
}

void io_seproxyhal_display(const bagl_element_t *element) {
    io_seproxyhal_display_default((bagl_element_t *) element);
}

void view_init(void) {
    UX_INIT();
}

void view_idle_show(unsigned int ignored) {
    view_idle_show_impl();
}

void view_address_show() {
    // Address has been placed in the output buffer
    char *const manAddress = (char *) (G_io_apdu_buffer + 65);
    snprintf(viewdata.key, MAX_CHARS_PER_KEY_LINE, "Confirm address");
    snprintf(viewdata.value, MAX_CHARS_PER_VALUE1_LINE, "%s", manAddress);
    splitValueField();
    view_address_show_impl();
}

void view_sign_show() {
    view_sign_show_impl();
}
