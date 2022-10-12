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

/** @file test_pixie_log.cpp
 * @brief Provides test coverage for our logging implementation
 */


#include <iostream>

#include <doctest/doctest.h>
#include <pixie/error.hpp>
#include <pixie/log.hpp>

static const std::string test_message = "Curiouser and curiouser!";

TEST_SUITE("xia::log") {
    TEST_CASE("off") {
        std::stringstream test_stream;
        std::streambuf* old = std::cout.rdbuf();
        std::cout.rdbuf(test_stream.rdbuf());
        xia::logging::start("off", "", false);
        xia::logging::set_level(xia::log::level::off);
        std::cout.rdbuf(old);

        xia::log(xia::log::level::off) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::log(xia::log::level::error) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::log(xia::log::level::warning) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::log(xia::log::level::info) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::log(xia::log::level::debug) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::log(xia::log::level::max_level) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::logging::stop("off");
    }
    TEST_CASE("error") {
        std::stringstream test_stream;
        std::streambuf* old = std::cout.rdbuf();
        std::cout.rdbuf(test_stream.rdbuf());
        xia::logging::start("error", "", false);
        xia::logging::set_level(xia::log::level::error);
        xia::logging::set_datetime_stamp("error", false);
        std::cout.rdbuf(old);

        xia::log(xia::log::level::off) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::log(xia::log::level::error) << test_message;
        CHECK(test_stream.str() == "[ERROR] " + test_message + "\n");
        test_stream.str("");

        xia::log(xia::log::level::warning) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::log(xia::log::level::info) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::log(xia::log::level::debug) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::logging::stop("error");
    }
    TEST_CASE("warning") {
        std::stringstream test_stream;
        std::streambuf* old = std::cout.rdbuf();
        std::cout.rdbuf(test_stream.rdbuf());
        xia::logging::start("warning", "", false);
        xia::logging::set_level(xia::log::level::warning);
        xia::logging::set_datetime_stamp("warning", false);
        std::cout.rdbuf(old);

        xia::log(xia::log::level::off) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::log(xia::log::level::error) << test_message;
        CHECK(test_stream.str() == "[ERROR] " + test_message + "\n");
        test_stream.str("");

        xia::log(xia::log::level::warning) << test_message;
        CHECK(test_stream.str() == "[WARN ] " + test_message + "\n");
        test_stream.str("");

        xia::log(xia::log::level::info) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::log(xia::log::level::debug) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::logging::stop("warning");
    }
    TEST_CASE("info") {
        std::stringstream test_stream;
        std::streambuf* old = std::cout.rdbuf();
        std::cout.rdbuf(test_stream.rdbuf());
        xia::logging::start("info", "", false);
        xia::logging::set_level(xia::log::level::info);
        xia::logging::set_datetime_stamp("info", false);
        std::cout.rdbuf(old);

        xia::log(xia::log::level::off) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::log(xia::log::level::error) << test_message;
        CHECK(test_stream.str() == "[ERROR] " + test_message + "\n");
        test_stream.str("");

        xia::log(xia::log::level::warning) << test_message;
        CHECK(test_stream.str() == "[WARN ] " + test_message + "\n");
        test_stream.str("");

        xia::log(xia::log::level::info) << test_message;
        CHECK(test_stream.str() == "[INFO ] " + test_message + "\n");
        test_stream.str("");

        xia::log(xia::log::level::debug) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::logging::stop("info");
    }
    TEST_CASE("debug") {
        std::stringstream test_stream;
        std::streambuf* old = std::cout.rdbuf();
        std::cout.rdbuf(test_stream.rdbuf());
        xia::logging::start("debug", "", false);
        xia::logging::set_level(xia::log::level::debug);
        xia::logging::set_datetime_stamp("debug", false);
        std::cout.rdbuf(old);

        xia::log(xia::log::level::off) << test_message;
        CHECK(test_stream.str() == "");
        test_stream.str("");

        xia::log(xia::log::level::error) << test_message;
        CHECK(test_stream.str() == "[ERROR] " + test_message + "\n");
        test_stream.str("");

        xia::log(xia::log::level::warning) << test_message;
        CHECK(test_stream.str() == "[WARN ] " + test_message + "\n");
        test_stream.str("");

        xia::log(xia::log::level::info) << test_message;
        CHECK(test_stream.str() == "[INFO ] " + test_message + "\n");
        test_stream.str("");

        xia::log(xia::log::level::debug) << test_message;
        CHECK(test_stream.str() == "[DEBUG] " + test_message + "\n");
        test_stream.str("");

        xia::logging::stop("debug");
    }
}

TEST_SUITE("xia::logging") {
    TEST_CASE("level_logging") {
        std::string name = "level_logging";
        xia::logging::start(name, "", false);
        xia::logging::set_level(xia::log::level::off);
        SUBCASE("off") {
            CHECK(xia::logging::level_logging(xia::log::level::off));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::error));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::warning));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::info));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::debug));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::max_level));
        }
        SUBCASE("error") {
            xia::logging::set_level(xia::log::level::error);
            CHECK(xia::logging::level_logging(xia::log::level::off));
            CHECK(xia::logging::level_logging(xia::log::level::error));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::warning));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::info));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::debug));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::max_level));
        }
        SUBCASE("warning") {
            xia::logging::set_level(xia::log::level::warning);
            CHECK(xia::logging::level_logging(xia::log::level::off));
            CHECK(xia::logging::level_logging(xia::log::level::error));
            CHECK(xia::logging::level_logging(xia::log::level::warning));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::info));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::debug));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::max_level));
        }
        SUBCASE("info") {
            xia::logging::set_level(xia::log::level::info);
            CHECK(xia::logging::level_logging(xia::log::level::off));
            CHECK(xia::logging::level_logging(xia::log::level::error));
            CHECK(xia::logging::level_logging(xia::log::level::warning));
            CHECK(xia::logging::level_logging(xia::log::level::info));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::debug));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::max_level));
        }
        SUBCASE("debug") {
            xia::logging::set_level(xia::log::level::debug);
            CHECK(xia::logging::level_logging(xia::log::level::off));
            CHECK(xia::logging::level_logging(xia::log::level::error));
            CHECK(xia::logging::level_logging(xia::log::level::warning));
            CHECK(xia::logging::level_logging(xia::log::level::info));
            CHECK(xia::logging::level_logging(xia::log::level::debug));
            CHECK_FALSE(xia::logging::level_logging(xia::log::level::max_level));
        }
        SUBCASE("max_level") {
            xia::logging::set_level(xia::log::level::max_level);
            CHECK(xia::logging::level_logging(xia::log::level::off));
            CHECK(xia::logging::level_logging(xia::log::level::error));
            CHECK(xia::logging::level_logging(xia::log::level::warning));
            CHECK(xia::logging::level_logging(xia::log::level::info));
            CHECK(xia::logging::level_logging(xia::log::level::debug));
            CHECK(xia::logging::level_logging(xia::log::level::max_level));
        }
        xia::logging::stop("level_logging");
    }
    TEST_CASE("set_datetime_stamp") {
        xia::logging::start("set_datetime_stamp", "", false);
        xia::logging::set_level(xia::log::level::off);
        CHECK_THROWS_WITH_AS(xia::logging::set_datetime_stamp("unit_test", false),
                             "invalid log output name in set datetime stamp",
                             xia::pixie::error::error);
        xia::logging::stop("set_datetime_stamp");
    }
    TEST_CASE("set_line_numbers") {
        xia::logging::start("set_line_numbers", "", false);
        xia::logging::set_level(xia::log::level::off);
        CHECK_THROWS_WITH_AS(xia::logging::set_line_numbers("unit_test", false),
                             "invalid log output name in set line numbers",
                             xia::pixie::error::error);
        xia::logging::stop("set_line_numbers");
    }
    TEST_CASE("set_level_stamp") {
        xia::logging::start("set_level_stamp", "", false);
        xia::logging::set_level(xia::log::level::off);
        CHECK_THROWS_WITH_AS(xia::logging::set_level_stamp("unit_test", false),
                             "invalid log output name in set level stamp",
                             xia::pixie::error::error);
        xia::logging::stop("set_level_stamp");
    }
}
