/*******************************************************************************
*   (c) 2018 ZondaX GmbH
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

#include <stdint.h>
#include <cstddef>

uint8_t hex2dec(char c);

size_t parseHexString(const char *s, uint8_t *out);

#ifdef __cplusplus
extern "C" {
#endif

void keccak(uint8_t *out, size_t out_len, uint8_t *in, size_t in_len);

#ifdef __cplusplus
}
#endif
