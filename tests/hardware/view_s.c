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

#if defined(TARGET_NANOS)

void h_review_button_left();
void h_review_button_right();
void view_review_show();
void view_sign_show_s();

ux_state_t ux;

const ux_menu_entry_t menu_main[] = {
    {NULL, NULL, 0, &C_icon_app, MENU_MAIN_APP_LINE1, MENU_MAIN_APP_LINE2, 33, 12},
    {NULL, NULL, 0, NULL, "v"APPVERSION, NULL, 0, 0},
    {NULL, os_sched_exit, 0, &C_icon_dashboard, "Quit", NULL, 50, 29},
    UX_MENU_END
};

static const bagl_element_t view_address[] = {
    UI_FillRectangle(0, 0, 0, UI_SCREEN_WIDTH, UI_SCREEN_HEIGHT, 0x000000, 0xFFFFFF),
    UI_Icon(0, 128 - 7, 0, 7, 7, BAGL_GLYPH_ICON_CHECK),
    UI_LabelLine(UIID_LABEL + 0, 0, 8, UI_SCREEN_WIDTH, UI_11PX, UI_WHITE, UI_BLACK, viewdata.key),
    UI_LabelLine(UIID_LABEL + 0, 0, 19, UI_SCREEN_WIDTH, UI_11PX, UI_WHITE, UI_BLACK, viewdata.value),
    UI_LabelLineScrolling(UIID_LABELSCROLL, 14, 30, 100, UI_11PX, UI_WHITE, UI_BLACK, viewdata.value2),
};

void h_review(unsigned int _) { UNUSED(_); view_sign_show_impl(); }

const ux_menu_entry_t menu_sign[] = {
    {NULL, h_review, 0, NULL, "View transaction", NULL, 0, 0},
    {NULL, h_sign_accept, 0, NULL, "Sign transaction", NULL, 0, 0},
    {NULL, h_sign_reject, 0, &C_icon_back, "Reject", NULL, 60, 40},
    UX_MENU_END
};

static const bagl_element_t view_review[] = {
    UI_BACKGROUND_LEFT_RIGHT_ICONS,
    UI_LabelLine(UIID_LABEL + 0, 0, 8, UI_SCREEN_WIDTH, UI_11PX, UI_WHITE, UI_BLACK, viewdata.key),
    UI_LabelLine(UIID_LABEL + 1, 0, 19, UI_SCREEN_WIDTH, UI_11PX, UI_WHITE, UI_BLACK, viewdata.value),
    UI_LabelLine(UIID_LABEL + 2, 0, 30, UI_SCREEN_WIDTH, UI_11PX, UI_WHITE, UI_BLACK, viewdata.value2),
};

static unsigned int view_address_button(unsigned int button_mask, unsigned int button_mask_counter) {
    switch (button_mask) {
        case BUTTON_EVT_RELEASED | BUTTON_LEFT | BUTTON_RIGHT:
        case BUTTON_EVT_RELEASED | BUTTON_LEFT:
            break;
        case BUTTON_EVT_RELEASED | BUTTON_RIGHT:
            h_address_accept(0);
            break;
    }
    return 0;
}

static unsigned int view_review_button(unsigned int button_mask, unsigned int button_mask_counter) {
    switch (button_mask) {
        case BUTTON_EVT_RELEASED | BUTTON_LEFT | BUTTON_RIGHT:
            // Press both left and right buttons to quit
            view_sign_show_s();
            break;
        case BUTTON_EVT_RELEASED | BUTTON_LEFT:
            // Press left to progress to the previous element
            h_review_button_left();
            break;

        case BUTTON_EVT_RELEASED | BUTTON_RIGHT:
            // Press right to progress to the next element
            h_review_button_right();
            break;
    }
    return 0;
}

const bagl_element_t *view_prepro(const bagl_element_t *element) {
    switch (element->component.userid) {
        case UIID_ICONLEFT:
        case UIID_ICONRIGHT:
            UX_CALLBACK_SET_INTERVAL(2000);
            break;
        case UIID_LABELSCROLL:
            UX_CALLBACK_SET_INTERVAL(
                MAX(3000, 1000 + bagl_label_roundtrip_duration_ms(element, 7))
            );
            break;
    }
    return element;
}

void h_review_button_left() {
    h_review_decrease();

    if (h_review_update_data() == TX_NO_MORE_DATA) {
        view_sign_show_s();
    } else {
        view_review_show();
    }
}

void h_review_button_right() {
    h_review_increase();

    if (h_review_update_data() == TX_NO_MORE_DATA) {
        view_sign_show_s();
    } else {
        view_review_show();
    }
}

//////////////////////////
//////////////////////////
//////////////////////////
//////////////////////////
//////////////////////////

void view_idle_show_impl() {
    UX_MENU_DISPLAY(0, menu_main, NULL);
}

void view_address_show_impl() {
    UX_DISPLAY(view_address, view_prepro);
}

void view_sign_show_impl() {
    h_review_init();
    ////
    h_review_update_data();
    view_review_show();
}

void view_sign_show_s(void){
    UX_MENU_DISPLAY(0, menu_sign, NULL);
}

void view_review_show() {
    UX_DISPLAY(view_review, view_prepro);
}

#endif
