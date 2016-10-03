/*
 *  xia_common.h
 *
 * Copyright (c) 2004, X-ray Instrumentation Associates
 *               2005 - 2009, XIA LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, 
 * with or without modification, are permitted provided 
 * that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above 
 *     copyright notice, this list of conditions and the 
 *     following disclaimer.
 *   * Redistributions in binary form must reproduce the 
 *     above copyright notice, this list of conditions and the 
 *     following disclaimer in the documentation and/or other 
 *     materials provided with the distribution.
 *   * Neither the name of X-ray Instrumentation Associates 
 *     nor the names of its contributors may be used to endorse 
 *     or promote products derived from this software without 
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE 
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR 
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF 
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
 * SUCH DAMAGE.
 *  
 *
 * $Id: xia_common.h 1197 2005-02-23 00:45:53Z SYSTEM $
 *
 *    All of the useful typedefs and macros
 *    go in here. Two conditions must be met
 *    in order to qualify as an "xia_common"
 *    typedef or macro:
 *    1) Must not be defined anywhere else and
 *    2) Must be able to be used in any library/module.
 *
 */


#ifndef XIA_COMMON_H
#define XIA_COMMON_H

#include <string.h>
#include <math.h>

/** Constants **/

/** Typedefs **/
typedef unsigned char  byte_t;
typedef unsigned char  boolean_t;
typedef unsigned short parameter_t;
typedef unsigned short flag_t;

/** MACROS **/
#define TRUE_  (1 == 1)
#define FALSE_ (1 == 0)

#define UNUSED(x)   ((x) = (x))
#define STREQ(x, y) (strcmp((x), (y)) == 0)
#define STRNEQ(x, y) (strncmp((x), (y), strlen(y)) == 0)
#define ROUND(x)    ((x) < 0.0 ? ceil((x) - 0.5) : floor((x) + 0.5))
#define PRINT_NON_NULL(x) ((x) == NULL ? "NULL" : (x))
#define BYTE_TO_WORD(lo, hi) (unsigned short)(((unsigned short)(hi) << 8) | (lo)) 
#define LO_BYTE(word) ((word) & 0xFF)
#define HI_BYTE(word) (((word) >> 8) & 0xFF)
#define MAKE_LOWER_CASE(s, i) for ((i) = 0; (i) < strlen((s)); (i)++) \
                                 (s)[i] = (char)tolower((s)[i])
#define N_ELEMS(x) (sizeof(x) / sizeof((x)[0]))


/* These macros already exist on Linux, so they need to be protected */

#ifndef MIN
#define MIN(x, y)  ((x) < (y) ? (x) : (y))
#endif /* MIN */

#ifndef MAX
#define MAX(x, y)  ((x) > (y) ? (x) : (y))
#endif /* MAX */

#endif						/* Endif for XIA_COMMON_H */
