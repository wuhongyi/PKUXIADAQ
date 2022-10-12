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

/** @file test_pixie16.cpp
 * @brief
 */

#include <fstream>
#include <sstream>

#include <doctest/doctest.h>

#include <pixie/error.hpp>
#include <pixie/log.hpp>

#include <pixie/pixie16/channel.hpp>
#include <pixie/pixie16/sim.hpp>
#include <pixie16/pixie16.h>

TEST_SUITE("Pixie16Api: Bit manipulation functions.") {
    TEST_CASE("APP16_ClrBit") {
        CHECK(10 == APP16_ClrBit(10, 10));
        CHECK(8 == APP16_ClrBit(1, 10));
        CHECK(8 == APP16_ClrBit(64, 8));
        CHECK(8 == APP16_ClrBit(32, 8));
        CHECK(8 == APP16_ClrBit(-4, 8));
    }
    TEST_CASE("APP16_SetBit") {
        CHECK(8 == APP16_SetBit(3, 8));
        CHECK(10 == APP16_SetBit(1, 8));
        CHECK(8 == APP16_SetBit(400, 8));
        CHECK(8 == APP16_SetBit(-4, 8));
    }
    TEST_CASE("APP16_TstBit") {
        CHECK(APP16_TstBit(3, 8));
        CHECK_FALSE(APP16_TstBit(10, 8));
        CHECK_FALSE(APP16_TstBit(17, 65535));
    }
    TEST_CASE("APP32_ClrBit") {
        CHECK(8 == APP32_ClrBit(1, 8));
        CHECK(8 == APP32_ClrBit(400, 8));
        CHECK(8 == APP32_ClrBit(-4, 8));
    }
    TEST_CASE("APP32_SetBit") {
        CHECK(524290 == APP32_SetBit(1, 524288));
        CHECK(8 == APP32_SetBit(400, 8));
        CHECK(8 == APP32_SetBit(-4, 8));
    }
    TEST_CASE("APP32_TstBit") {
        CHECK(APP32_TstBit(19, 1 << 19));
    }
}
