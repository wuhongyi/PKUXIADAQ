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

/** @file xia_common.h
 * @brief Defines commonly used macros in various XIA LLC software.
 * @note This file will be deprecated July 31, 2023
 */

#ifndef XIA_COMMON_H
#define XIA_COMMON_H

#include <math.h>
#include <string.h>

/** Constants **/

/** Typedefs **/
typedef unsigned char byte_t;
typedef unsigned char boolean_t;
typedef unsigned short parameter_t;
typedef unsigned short flag_t;

/** MACROS **/
#define TRUE_ (1 == 1)
#define FALSE_ (1 == 0)

#define UNUSED(x) ((x) = (x))
#define STREQ(x, y) (strcmp((x), (y)) == 0)
#define STRNEQ(x, y) (strncmp((x), (y), strlen(y)) == 0)
#define ROUND(x) ((x) < 0.0 ? ceil((x) -0.5) : floor((x) + 0.5))
#define PRINT_NON_NULL(x) ((x) == NULL ? "NULL" : (x))
#define BYTE_TO_WORD(lo, hi) (unsigned short) (((unsigned short) (hi) << 8) | (lo))
#define LO_BYTE(word) ((word) &0xFF)
#define HI_BYTE(word) (((word) >> 8) & 0xFF)
#define MAKE_LOWER_CASE(s, i)                                                                      \
    for ((i) = 0; (i) < strlen((s)); (i)++)                                                        \
    (s)[i] = (char) tolower((s)[i])
#define N_ELEMS(x) (sizeof(x) / sizeof((x)[0]))


/* These macros already exist on Linux, so they need to be protected */

#ifndef MIN
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#endif /* MIN */

#ifndef MAX
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif /* MAX */

#endif /* Endif for XIA_COMMON_H */
