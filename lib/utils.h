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

#include <zxmacros.h>

#ifdef __cplusplus
extern "C" {
#endif

#define UTILS_NOERROR 0
#define UTILS_NOT_ENOUGH_DATA -1

// Converts data bytes into a hexstring !! IN PLACE !!
uint8_t convertToHexstringInPlace(uint8_t *data, uint16_t dataLen, uint16_t dataLenMax);

void printTime(char *out, uint16_t outLen, uint64_t t);

#ifdef __cplusplus
}
#endif
