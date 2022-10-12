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
    TEST_CASE("Error class") {
        SUBCASE("Test constructor taking char*") {
            xia::pixie::error::error err(xia::pixie::error::code::device_dma_busy, "test char");
            CHECK(err.result_text() == "device dma busy");
            CHECK(err.result() == 507);
            CHECK(err.return_code() == -507);
            std::ostringstream ostringstream;
            err.output(ostringstream);
            CHECK(ostringstream.str() == "error: code:507 : test char");
        }
        SUBCASE("Test constructor taking string") {
            xia::pixie::error::error err(xia::pixie::error::code::device_dma_busy,
                                         std::string("test str"));
            CHECK(err.result_text() == "device dma busy");
            CHECK(err.result() == 507);
            CHECK(err.return_code() == -507);
            std::ostringstream ostringstream;
            err.output(ostringstream);
            CHECK(ostringstream.str() == "error: code:507 : test str");
        }
        SUBCASE("Test constructor taking ostringstream") {
            std::ostringstream msg;
            msg << "test stream";
            xia::pixie::error::error err(xia::pixie::error::code::device_dma_busy, msg);
            CHECK(err.result_text() == "device dma busy");
            CHECK(err.result() == 507);
            CHECK(err.return_code() == -507);
            std::ostringstream ostringstream;
            err.output(ostringstream);
            CHECK(ostringstream.str() == "error: code:507 : test stream");
        }
    }
    TEST_CASE("Output Streams") {
        SUBCASE("ostringstreams") {
            std::ostringstream ostringstream;
            xia::pixie::error::error err(xia::pixie::error::code::device_dma_busy, "test what");
            ostringstream << err;
            CHECK(ostringstream.str() == "result=507, test what");
        }
        SUBCASE("ostreams") {
            std::stringbuf stringbuf;
            xia::pixie::error::error err(xia::pixie::error::code::device_dma_busy, "test what");
            std::ostream os(&stringbuf);
            os << err;
            CHECK(stringbuf.str() == "error: code:507 : test what");
        }
    }
    TEST_CASE("Error text generation") {
        CHECK(xia::pixie::error::api_result_text(xia::pixie::error::code::unknown_error) ==
              "unknown error");
        CHECK(xia::pixie::error::api_result_text(xia::pixie::error::code::last) ==
              "bad error code");
        CHECK(xia::pixie::error::api_result_text(1234) == "bad error code");
        CHECK(xia::pixie::error::api_result_text(-900) == "unknown error");
    }
    TEST_CASE("Result Generation") {
        CHECK(xia::pixie::error::api_result(xia::pixie::error::code::unknown_error) == 900);
        CHECK(xia::pixie::error::api_result(xia::pixie::error::code::last) == 990);
    }
    TEST_CASE("Check return_code wrappers") {
        SUBCASE("Generate Return code") {
            CHECK(xia::pixie::error::return_code(1234) == -1234);
        }
        SUBCASE("Bad Alloc Return Code") {
            CHECK(xia::pixie::error::return_code_bad_alloc_error() == -902);
        }
        SUBCASE("Unknown Error Return Code") {
            CHECK(xia::pixie::error::return_code_unknown_error() == -900);
        }
        SUBCASE("Not Supported Return Code") {
            CHECK(xia::pixie::error::return_code_not_supported() == -803);
        }
    }
    TEST_CASE("Result_codes size matches code::last") {
        CHECK(xia::pixie::error::check_code_match());
    }
}
