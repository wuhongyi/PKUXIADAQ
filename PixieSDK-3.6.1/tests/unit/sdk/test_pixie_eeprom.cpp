/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2022 XIA LLC, All rights reserved.
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

/** @file test_pixie_epprom.cpp
 * @brief Provides test coverage for eeprom namespace.
 */

#include <algorithm>
#include <map>

#include <doctest/doctest.h>
#include <pixie/eeprom.hpp>

using eeprom_error = xia::pixie::eeprom::error;

namespace eetest {
using eeprom_addr = unsigned long;
using eeprom_data = std::vector<std::string>;
    using rev_eeprom_data = std::map<std::string, const eeprom_data&>;

/*
 * Lets handle the EEPROM data as found the log. The less we touch it the
 * better the results we get in this unit test.
 */
static const eeprom_data blank = {
    "00000000 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000010 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000020 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000030 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000040 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000050 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000060 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000070 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................"
};

static const eeprom_data rev_d = {
    "00000000 ac 0c 00 28 01 00 09 02-00 28 01 00 09 02 00 28 ................",
    "00000010 01 00 09 02 00 28 01 00-09 02 00 28 01 00 09 02 ................",
    "00000020 00 28 01 00 09 02 00 28-01 00 09 02 00 28 01 00 ................",
    "00000030 09 02 00 28 01 00 09 02-00 28 01 00 09 02 00 28 ................",
    "00000040 01 00 09 02 00 28 01 00-09 02 00 28 01 00 09 02 ................",
    "00000050 00 28 01 00 09 02 00 28-01 00 09 02 00 28 01 00 ................",
    "00000060 09 02 ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000070 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
};

static const eeprom_data rev_f = {
    "00000000 d8 04 0f 00 19 01 00 06-02 00 19 01 00 06 02 00 ................",
    "00000010 19 01 00 06 02 00 19 01-00 06 02 00 19 01 00 06 ................",
    "00000020 02 00 19 01 00 06 02 00-19 01 00 06 02 00 19 01 ................",
    "00000030 00 06 02 00 19 01 00 06-02 00 19 01 00 06 02 00 ................",
    "00000040 19 01 00 06 02 00 19 01-00 06 02 00 19 01 00 06 ................",
    "00000050 02 00 19 01 00 06 02 00-19 01 00 06 02 00 19 01 ................",
    "00000060 00 06 02 10 fa 00 ff ff-ff ff ff ff ff ff ff ff ................",
    "00000070 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................"
};

static const eeprom_data rev_h = {
    "00000000 61 96 ec e3 f2 ff 0b e1-07 00 00 0c 11 0d 42 0e ................",
    "00000010 02 0f 00 28 04 00 ff ff-ff ff ff ff ff ff ff ff ................",
    "00000020 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000030 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000040 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000050 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000060 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000070 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
};

static const eeprom_data rev_h_bad_crc = {
    "00000000 61 96 ec e3 f2 ff 4b e1-07 00 00 0c 11 0d 42 0e ................",
    "00000010 02 0f 00 28 04 00 ff ff-ff ff ff ff ff ff ff ff ................",
    "00000020 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000030 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000040 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000050 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000060 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
    "00000070 ff ff ff ff ff ff ff ff-ff ff ff ff ff ff ff ff ................",
};

static const rev_eeprom_data eeproms = {
    { "blank", blank },
    { "D", rev_d },
    { "F", rev_f },
    { "H", rev_h },
    { "H:bad-crc", rev_h_bad_crc },
};

template<typename T>
static T get_value(const std::string& opt, int base = 9) {
    T value = 0;
    try {
        value = std::stoul(opt, nullptr, base);
    } catch (std::invalid_argument e) {
        throw std::runtime_error("invalid number: " + opt);
    } catch (std::out_of_range e) {
        throw std::runtime_error("number out of range: " + opt);
    }
    return value;
}

static void load(const std::string rev, xia::pixie::eeprom::eeprom& eeprom) {
    auto edi = eeproms.find(rev);
    if (edi != std::end(eeproms)) {
        eeprom.clear();
        eeprom.data.resize(xia::pixie::hw::eeprom_block_size);
        for (auto& s : std::get<1>(*edi)) {
            xia::util::strings ss;
            xia::util::split(ss, s);
            if (ss.size() != 17) {
                throw std::runtime_error("invalid EEPROM format: rev=" + rev);
            }
            eeprom_addr a = get_value<eeprom_addr>(ss[0], 16);
            if ((a + 16) > eeprom.data.size()) {
                throw std::runtime_error("invalid EEPROM address: " + ss[0]);
            }
            for (int b = 0; b < 7; ++b) {
                eeprom.data[a + b] = get_value<uint8_t>(ss[b + 1], 16);
            }
            xia::util::strings ss7_8;
            xia::util::split(ss7_8, ss[8], '-');
            if (ss7_8.size() != 2) {
                throw std::runtime_error("invalid EEPROM format: rev=" + rev);
            }
            eeprom.data[a + 7] = get_value<uint8_t>(ss7_8[0], 16);
            eeprom.data[a + 8] = get_value<uint8_t>(ss7_8[1], 16);
            for (int b = 9; b < 16; ++b) {
                eeprom.data[a + b] = get_value<uint8_t>(ss[b], 16);
            }
        }
    }
}
} // namespace eetest

TEST_SUITE("xia::pixie::eeprom") {
    xia::pixie::eeprom::eeprom eeprom;
    TEST_CASE("Blank") {
        SUBCASE("Load data") {
            CHECK_NOTHROW(eetest::load("blank", eeprom));
        }
        SUBCASE("Process") {
            CHECK_THROWS_WITH_AS(
                eeprom.process(), "invalid serial number: blank EEPROM", eeprom_error);
        }
    }
    TEST_CASE("Rev D") {
        SUBCASE("Load data") {
            CHECK_NOTHROW(eetest::load("D", eeprom));
        }
        SUBCASE("Process") {
            CHECK_NOTHROW(eeprom.process());
        }
        SUBCASE("Validate module") {
            CHECK(eeprom.format == 0);
            CHECK(eeprom.valid() == false);
            CHECK(eeprom.serial_num == 172);
            CHECK(eeprom.revision == 12);
            CHECK(eeprom.major_revision == 0);
            CHECK(eeprom.minor_revision == 0);
            CHECK(eeprom.mod_strike == 0);
            CHECK(eeprom.num_channels == 16);
            CHECK(eeprom.max_channels == 16);
        }
        SUBCASE("Fixtures") {
            CHECK(eeprom.dbs.size() == 0);
        }
        SUBCASE("Configs") {
            CHECK(eeprom.configs.size() == 16);
            int index = 0;
            for (auto& cfg : eeprom.configs) {
                CHECK(cfg.index == index);
                CHECK(cfg.fixture == xia::pixie::hw::module_fixture::mainboard);
                CHECK(cfg.adc_bits == 12);
                CHECK(cfg.adc_msps == 100);
                CHECK(cfg.adc_clk_div == 1);
                CHECK(cfg.fpga_clk_mhz == 100);
                CHECK(cfg.max_histogram_length == 32768);
                CHECK(cfg.max_adc_trace_length == 8192);
                CHECK(cfg.max_num_baselines == 3640);
                ++index;
            }
        }
    }
    TEST_CASE("Rev F") {
        SUBCASE("Load data") {
            CHECK_NOTHROW(eetest::load("F", eeprom));
        }
        SUBCASE("Process") {
            CHECK_NOTHROW(eeprom.process());
        }
        SUBCASE("Validate module") {
            CHECK(eeprom.format == 1);
            CHECK(eeprom.valid() == false);
            CHECK(eeprom.serial_num == 1240);
            CHECK(eeprom.revision == 15);
            CHECK(eeprom.major_revision == 0);
            CHECK(eeprom.minor_revision == 0);
            CHECK(eeprom.mod_strike == 0);
            CHECK(eeprom.num_channels == 16);
            CHECK(eeprom.max_channels == 16);
        }
        SUBCASE("Fixtures") {
            CHECK(eeprom.dbs.size() == 0);
        }
        SUBCASE("Configs") {
            CHECK(eeprom.configs.size() == 16);
            int index = 0;
            for (auto& cfg : eeprom.configs) {
                CHECK(cfg.index == index);
                CHECK(cfg.fixture == xia::pixie::hw::module_fixture::mainboard);
                CHECK(cfg.adc_bits == 16);
                CHECK(cfg.adc_msps == 250);
                CHECK(cfg.adc_clk_div == 2);
                CHECK(cfg.fpga_clk_mhz == 125);
                CHECK(cfg.max_histogram_length == 32768);
                CHECK(cfg.max_adc_trace_length == 8192);
                CHECK(cfg.max_num_baselines == 3640);
                ++index;
            }
        }
    }
    TEST_CASE("Rev H") {
        SUBCASE("Load data") {
            CHECK_NOTHROW(eetest::load("H", eeprom));
        }
        SUBCASE("Process") {
            CHECK_NOTHROW(eeprom.process());
        }
        SUBCASE("Validate module") {
            CHECK(eeprom.format == 2);
            CHECK(eeprom.valid());
            CHECK(eeprom.serial_num == 2017);
            CHECK(eeprom.revision == 17);
            CHECK(eeprom.major_revision == 66);
            CHECK(eeprom.minor_revision == 2);
            CHECK(eeprom.mod_strike == 0);
            CHECK(eeprom.num_channels == 8);
            CHECK(eeprom.max_channels == 32);
        }
        SUBCASE("Fixtures") {
            CHECK(eeprom.dbs.size() == 1);
            for (auto& db : eeprom.dbs) {
                CHECK(db.label == "DB04");
                CHECK(db.index == 4);
                CHECK(db.position == 0);
            }
        }
        SUBCASE("Configs") {
            CHECK(eeprom.configs.size() == 8);
            int index = 0;
            for (auto& cfg : eeprom.configs) {
                CHECK(cfg.index == index);
                CHECK(cfg.fixture == xia::pixie::hw::module_fixture::DB04);
                CHECK(cfg.adc_bits == 14);
                CHECK(cfg.adc_msps == 250);
                CHECK(cfg.adc_clk_div == 2);
                CHECK(cfg.fpga_clk_mhz == 125);
                CHECK(cfg.max_histogram_length == 16384);
                CHECK(cfg.max_adc_trace_length == 8192);
                CHECK(cfg.max_num_baselines == 1927);
                ++index;
            }
        }
    }
    TEST_CASE("Rev H - bad crc") {
        SUBCASE("Load data") {
            CHECK_NOTHROW(eetest::load("H:bad-crc", eeprom));
        }
        SUBCASE("Process") {
            CHECK_THROWS_WITH_AS(
                eeprom.process(), "invalid ADC MSPS: 65535", eeprom_error);
        }
    }
}
