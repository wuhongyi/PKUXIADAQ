/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2021 XIA LLC, All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/** @file pixie16sys_common.h
 * @brief Declares internal prototypes and common functions.
 * @note This file will be deprecated July 31, 2023
 */

#ifndef XIA_PIXIE16SYS_COMMON_H
#define XIA_PIXIE16SYS_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "pixie16sys_defs.h"

PIXIE16SYS_EXPORT unsigned short PIXIE16SYS_API SYS16_SetBit(unsigned short bit,
                                                             unsigned short value);
PIXIE16SYS_EXPORT unsigned short PIXIE16SYS_API SYS16_ClrBit(unsigned short bit,
                                                             unsigned short value);
PIXIE16SYS_EXPORT unsigned short PIXIE16SYS_API SYS16_TstBit(unsigned short bit,
                                                             unsigned short value);
PIXIE16SYS_EXPORT unsigned int PIXIE16SYS_API SYS32_SetBit(unsigned short bit, unsigned int value);
PIXIE16SYS_EXPORT unsigned int PIXIE16SYS_API SYS32_ClrBit(unsigned short bit, unsigned int value);
PIXIE16SYS_EXPORT unsigned int PIXIE16SYS_API SYS32_TstBit(unsigned short bit, unsigned int value);

PIXIE16SYS_EXPORT int PIXIE16SYS_API get_ns_per_cycle(double* ns_per_cycle);
PIXIE16SYS_EXPORT void PIXIE16SYS_API wait_for_a_short_time(int cycles);

int Pixie_DSP_Memory_Burst_Read(unsigned int* dsp_data,  // DSP data for the I/O
                                unsigned int dsp_address,  // DSP data memory address
                                unsigned int nWords,  // Number of DSP data words for the I/O
                                unsigned short ModNum);  // The Pixie module for the I/O

int I2CM24C64_start(unsigned short ModNum);
int I2CM24C64_stop(unsigned short ModNum);
int I2CM24C64_byte_write(unsigned short ModNum, char ByteToSend);
int I2CM24C64_byte_read(unsigned short ModNum, char* ByteToReceive);
char I2CM24C64_getACK(unsigned short ModNum);
char I2CM24C64_sendACK(unsigned short ModNum);


int PCF8574_start(unsigned short ModNum);
int PCF8574_stop(unsigned short ModNum);
int PCF8574_byte_write(unsigned short ModNum, char ByteToSend);
int PCF8574_byte_read(unsigned short ModNum, char* ByteToReceive);
char PCF8574_getACK(unsigned short ModNum);
char PCF8574_sendACK(unsigned short ModNum);

#ifdef __cplusplus
}
#endif  // End of notice for C++ compilers

#endif  // End of pixie16sys_common.h
