/*******************************************************************************
*   (c) 2019 ZondaX GmbH
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

#pragma once

#include "base58.h"
#include <zxmacros.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BIP44_LEN_DEFAULT 5

extern uint32_t bip44Path[BIP44_LEN_DEFAULT];

uint8_t crc8(const uint8_t *data, size_t data_len);

void ethAddressFromPubKey(uint8_t *ethAddress, uint8_t *pubkey);

uint8_t manAddressFromEthAddr(char *manAddress, uint8_t *ethAddress);

void keccak(uint8_t *out, size_t out_len, uint8_t *in, size_t in_len);

#if defined(TARGET_NANOS) || defined(TARGET_NANOX)
void extractPublicKey(uint32_t bip44Path[5], uint8_t *pubKey);
#endif


#ifdef __cplusplus
}
#endif
