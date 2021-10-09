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

/** @file test_utilities.cpp
 * @brief Defines tests for functions defined in utilities.c
 */

#include "pixie16app_export.h"

#include "doctest/doctest.h"

TEST_SUITE("app/utilities.c") {
    TEST_CASE("Decimal2IEEEFloating") {
        CHECK((unsigned) 0 == Decimal2IEEEFloating(0));
        CHECK((unsigned) 1230546000 == doctest::Approx(Decimal2IEEEFloating(887365)));
        CHECK((unsigned) 3378029648 == doctest::Approx(Decimal2IEEEFloating(-887365)));
        CHECK((unsigned) 1118927127 == doctest::Approx(Decimal2IEEEFloating(88.7365)));
        CHECK((unsigned) 3266410775 == doctest::Approx(Decimal2IEEEFloating(-88.7365)));
    }
    TEST_CASE("IEEEFloating2Decimal") {
        CHECK(doctest::Approx(0) == IEEEFloating2Decimal(0));
        CHECK(doctest::Approx(887365) == IEEEFloating2Decimal(1230546000));
        CHECK(doctest::Approx(-887365) == IEEEFloating2Decimal(3378029648));
        CHECK(doctest::Approx(88.7365) == IEEEFloating2Decimal(1118927127));
        CHECK(doctest::Approx(-88.7365) == IEEEFloating2Decimal(3266410775));
    }
}