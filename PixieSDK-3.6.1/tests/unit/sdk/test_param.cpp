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

/** @file test_param.cpp
 * @brief
 */

#include <fstream>
#include <sstream>

#include <doctest/doctest.h>

#include <pixie/error.hpp>
#include <pixie/log.hpp>
#include <pixie/param.hpp>

xia::pixie::param::channel_variables source;
constexpr size_t source_default = 0xDEFACE;

xia::pixie::param::channel_variables destination;
constexpr size_t destination_default = 0xFACADE;

void initialize_vars() {
    for (auto& var : source)
        for (auto& item : var.value)
            item.value = source_default;
    for (auto& var : destination)
        for (auto& item : var.value)
            item.value = destination_default;
}

static void test_setup() {
    xia::logging::start("log", "stdout", false);
    xia::logging::set_level(xia::log::level::debug);
    for (auto& var : xia::pixie::param::get_channel_var_descriptors()) {
        source.push_back(var);
        destination.push_back(var);
    }
}

static void test_filter_mask(const int& mask) {
    initialize_vars();
    xia::pixie::param::copy_parameters(mask, source, destination);
}

TEST_SUITE("xia::pixie::param") {
    TEST_CASE("SETUP") {
        test_setup();
    }

    TEST_CASE("copy_parameters with copy filter") {
        auto var = xia::pixie::param::channel_var::PreampTau;
        auto var_idx = static_cast<int>(var);
        xia::pixie::param::copy_filter filter = {var};

        SUBCASE("Mis-matched sizes.") {
            try {
                xia::pixie::param::channel_variables empty;
                xia::pixie::param::copy_parameters(filter, source, empty);
            } catch (xia::pixie::error::error& err) {
                CHECK(err.type == xia::pixie::error::code::device_copy_failure);
            }
        }

        SUBCASE("Happy Path") {
            initialize_vars();
            xia::pixie::param::copy_parameters(filter, source, destination);
            CHECK(source[var_idx].value.front().value == destination[var_idx].value.front().value);
        }
    }

    TEST_CASE("copy_parameters with filter mask") {
        SUBCASE("Everything") {
            test_filter_mask(4095);
            for (auto& var : xia::pixie::param::get_channel_var_descriptors()) {
                auto idx = static_cast<int>(var.par);
                CHECK(source[idx].value.front().value == destination[idx].value.front().value);
            }
        }
        SUBCASE("Nothing") {
            test_filter_mask(0);
            for (auto& var : xia::pixie::param::get_channel_var_descriptors()) {
                auto idx = static_cast<int>(var.par);
                CHECK(destination[idx].value.front().value == destination_default);
            }
        }
        SUBCASE("Energy and Trigger Filters") {
            test_filter_mask(1);
            for (auto& var : xia::pixie::param::get_channel_var_descriptors()) {
                auto idx = static_cast<int>(var.par);
                if (var.par != xia::pixie::param::channel_var::BLcut &&
                    var.par != xia::pixie::param::channel_var::PAFlength &&
                    var.par != xia::pixie::param::channel_var::PeakSample &&
                    var.par != xia::pixie::param::channel_var::PeakSep &&
                    var.par != xia::pixie::param::channel_var::SlowLength &&
                    var.par != xia::pixie::param::channel_var::SlowGap &&
                    var.par != xia::pixie::param::channel_var::TriggerDelay &&
                    var.par != xia::pixie::param::channel_var::FastGap &&
                    var.par != xia::pixie::param::channel_var::FastLength &&
                    var.par != xia::pixie::param::channel_var::FastThresh) {
                    CHECK(destination[idx].value.front().value == destination_default);
                } else {
                    CHECK(source[idx].value.front().value == destination[idx].value.front().value);
                }
            }
        }
        SUBCASE("Analog Signal Conditioning") {
            source[static_cast<int>(xia::pixie::param::channel_var::ChanCSRa)].value.front().value =
                0xDEBAEE;
            test_filter_mask(2);
            for (auto& var : xia::pixie::param::get_channel_var_descriptors()) {
                auto idx = static_cast<int>(var.par);
                if (var.par != xia::pixie::param::channel_var::OffsetDAC) {
                    CHECK(destination[idx].value.front().value == destination_default);
                } else if (var.par == xia::pixie::param::channel_var::ChanCSRa) {
                    CHECK(0xFA8AFE == destination[idx].value.front().value);
                } else {
                    CHECK(source[idx].value.front().value == destination[idx].value.front().value);
                }
            }
        }
        SUBCASE("Histogram Control") {
            test_filter_mask(4);
            for (auto& var : xia::pixie::param::get_channel_var_descriptors()) {
                auto idx = static_cast<int>(var.par);
                if (var.par != xia::pixie::param::channel_var::EnergyLow &&
                    var.par != xia::pixie::param::channel_var::Log2Ebin) {
                    CHECK(destination[idx].value.front().value == destination_default);
                } else {
                    CHECK(source[idx].value.front().value == destination[idx].value.front().value);
                }
            }
        }
        SUBCASE("Decay Time") {
            test_filter_mask(8);
            for (auto& var : xia::pixie::param::get_channel_var_descriptors()) {
                auto idx = static_cast<int>(var.par);
                if (var.par != xia::pixie::param::channel_var::PreampTau) {
                    CHECK(destination[idx].value.front().value == destination_default);
                } else {
                    CHECK(source[idx].value.front().value == destination[idx].value.front().value);
                }
            }
        }
        SUBCASE("Pulse Shape Analysis") {
            test_filter_mask(16);
            for (auto& var : xia::pixie::param::get_channel_var_descriptors()) {
                auto idx = static_cast<int>(var.par);
                if (var.par != xia::pixie::param::channel_var::TraceLength &&
                    var.par != xia::pixie::param::channel_var::TriggerDelay &&
                    var.par != xia::pixie::param::channel_var::PAFlength) {
                    CHECK(destination[idx].value.front().value == destination_default);
                } else {
                    CHECK(source[idx].value.front().value == destination[idx].value.front().value);
                }
            }
        }
        SUBCASE("Baseline Control") {
            test_filter_mask(32);
            for (auto& var : xia::pixie::param::get_channel_var_descriptors()) {
                auto idx = static_cast<int>(var.par);
                if (var.par != xia::pixie::param::channel_var::BLcut &&
                    var.par != xia::pixie::param::channel_var::BaselinePercent &&
                    var.par != xia::pixie::param::channel_var::Log2Bweight) {
                    CHECK(destination[idx].value.front().value == destination_default);
                } else {
                    CHECK(source[idx].value.front().value == destination[idx].value.front().value);
                }
            }
        }
        SUBCASE("Channel CSRA") {
            test_filter_mask(128);
            for (auto& var : xia::pixie::param::get_channel_var_descriptors()) {
                auto idx = static_cast<int>(var.par);
                if (var.par != xia::pixie::param::channel_var::ChanCSRa) {
                    CHECK(destination[idx].value.front().value == destination_default);
                } else {
                    CHECK(source[idx].value.front().value == destination[idx].value.front().value);
                }
            }
        }
        SUBCASE("CFD Triggering") {
            test_filter_mask(256);
            for (auto& var : xia::pixie::param::get_channel_var_descriptors()) {
                auto idx = static_cast<int>(var.par);
                if (var.par != xia::pixie::param::channel_var::CFDDelay &&
                    var.par != xia::pixie::param::channel_var::CFDScale &&
                    var.par != xia::pixie::param::channel_var::CFDThresh) {
                    CHECK(destination[idx].value.front().value == destination_default);
                } else {
                    CHECK(source[idx].value.front().value == destination[idx].value.front().value);
                }
            }
        }
        SUBCASE("Trigger Stretch Length") {
            test_filter_mask(512);
            for (auto& var : xia::pixie::param::get_channel_var_descriptors()) {
                auto idx = static_cast<int>(var.par);
                if (var.par != xia::pixie::param::channel_var::ExtTrigStretch &&
                    var.par != xia::pixie::param::channel_var::ChanTrigStretch &&
                    var.par != xia::pixie::param::channel_var::VetoStretch &&
                    var.par != xia::pixie::param::channel_var::FastTrigBackLen) {
                    CHECK(destination[idx].value.front().value == destination_default);
                } else {
                    CHECK(source[idx].value.front().value == destination[idx].value.front().value);
                }
            }
        }
        SUBCASE("FIFO Delays") {
            test_filter_mask(1024);
            for (auto& var : xia::pixie::param::get_channel_var_descriptors()) {
                auto idx = static_cast<int>(var.par);
                if (var.par != xia::pixie::param::channel_var::ExternDelayLen &&
                    var.par != xia::pixie::param::channel_var::FtrigoutDelay) {
                    CHECK(destination[idx].value.front().value == destination_default);
                } else {
                    CHECK(source[idx].value.front().value == destination[idx].value.front().value);
                }
            }
        }
        SUBCASE("Multiplicity Mask") {
            test_filter_mask(2048);
            for (auto& var : xia::pixie::param::get_channel_var_descriptors()) {
                auto idx = static_cast<int>(var.par);
                if (var.par != xia::pixie::param::channel_var::MultiplicityMaskL &&
                    var.par != xia::pixie::param::channel_var::MultiplicityMaskH) {
                    CHECK(destination[idx].value.front().value == destination_default);
                } else {
                    CHECK(source[idx].value.front().value == destination[idx].value.front().value);
                }
            }
        }
        SUBCASE("QDC") {
            test_filter_mask(4096);
            for (auto& var : xia::pixie::param::get_channel_var_descriptors()) {
                auto idx = static_cast<int>(var.par);
                if (var.par != xia::pixie::param::channel_var::QDCLen0 &&
                    var.par != xia::pixie::param::channel_var::QDCLen1 &&
                    var.par != xia::pixie::param::channel_var::QDCLen2 &&
                    var.par != xia::pixie::param::channel_var::QDCLen3 &&
                    var.par != xia::pixie::param::channel_var::QDCLen4 &&
                    var.par != xia::pixie::param::channel_var::QDCLen5 &&
                    var.par != xia::pixie::param::channel_var::QDCLen6 &&
                    var.par != xia::pixie::param::channel_var::QDCLen7) {
                    CHECK(destination[idx].value.front().value == destination_default);
                } else {
                    CHECK(source[idx].value.front().value == destination[idx].value.front().value);
                }
            }
        }
        SUBCASE("Multiplicity Mask and FIFO Delays") {
            test_filter_mask(3072);
            for (auto& var : xia::pixie::param::get_channel_var_descriptors()) {
                auto idx = static_cast<int>(var.par);
                if (var.par != xia::pixie::param::channel_var::MultiplicityMaskL &&
                    var.par != xia::pixie::param::channel_var::MultiplicityMaskH &&
                    var.par != xia::pixie::param::channel_var::ExternDelayLen &&
                    var.par != xia::pixie::param::channel_var::FtrigoutDelay) {
                    CHECK(destination[idx].value.front().value == destination_default);
                } else {
                    CHECK(source[idx].value.front().value == destination[idx].value.front().value);
                }
            }
        }
    }
    TEST_CASE("TEARDOWN") {
        xia::logging::stop("log");
    }
}
