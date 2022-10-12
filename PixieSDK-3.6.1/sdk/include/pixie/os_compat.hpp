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

/** @file os_compat.hpp
 * @brief Defines OS specific compatibility flags and defines
 */

#ifndef XIA_OS_COMPAT_HPP
#define XIA_OS_COMPAT_HPP

#if defined(_WIN64) || defined(_WIN32)
#define _USE_MATH_DEFINES

#ifndef NOMINMAX
#define NOMINMAX
#endif

#define PIXIE_EXPORT __declspec(dllexport)
#define PIXIE_API _stdcall
#else
#define PIXIE_EXPORT
#define PIXIE_API
#endif

#endif /* XIA_OS_COMPAT_HPP */
