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

/** @file pixie16sys_globals.h
 * @brief Declares internal global variables and data structures.
 * @note This file will be deprecated July 31, 2023
 */

#ifndef XIA_PIXIE16SYS_GLOBALS_H
#define XIA_PIXIE16SYS_GLOBALS_H

#include <PlxApi.h>

#ifdef __cplusplus
extern "C" {
#endif

PLX_UINT_PTR VAddr[SYS_MAX_NUM_MODULES];  // PCI device virutal address
PLX_DEVICE_OBJECT SYS_hDevice[SYS_MAX_NUM_MODULES];  // PCI device handle
unsigned short SYS_Number_Modules;  // Total number of modules in the crate
unsigned short SYS_Offline;  // SYS_Offline = 1: offline mode; SYS_Offline = 0: Online mode

/// The number of nanoseconds that a CPU cycle takes. Used to ensure that we wait for consistent times.
double Ns_Per_Cycle;

#ifdef __cplusplus
}
#endif  // End of notice for C++ compilers

#endif  // End of pixie16sys_globals.h
