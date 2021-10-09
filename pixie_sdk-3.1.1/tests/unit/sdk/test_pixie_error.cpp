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

/** @file test_pixie_error.cpp
 * @brief Provides test coverage for error namespace.
 */

#include <doctest/doctest.h>
#include <pixie/error.hpp>

TEST_SUITE("xia::pixie::error") {
    TEST_CASE("Result Generation") {
        SUBCASE("Valid Error Code") {
            CHECK(xia::pixie::error::api_result_text(xia::pixie::error::code::unknown_error) ==
                  "unknown error");
            CHECK(xia::pixie::error::api_result(xia::pixie::error::code::unknown_error) == 900);
        }
        SUBCASE("Invalid Error Code") {
            CHECK(xia::pixie::error::api_result_text(xia::pixie::error::code::last) ==
                  "bad error code");
            CHECK(xia::pixie::error::api_result(xia::pixie::error::code::last) == 990);
        }
    }
    TEST_CASE("Result_codes size matches code::last") {
        CHECK(xia::pixie::error::check_code_match());
    }
}
