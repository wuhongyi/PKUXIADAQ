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

/** @file test_list_mode.cpp
 * @brief Tests related to the list_mode namespace
 */

#include <doctest/doctest.h>

#include <pixie/data/list_mode.hpp>
#include <pixie/error.hpp>

using namespace xia::pixie::data::list_mode;

TEST_SUITE("xia::pixie::list_mode") {
    TEST_CASE("record") {
        static const std::string json_str = "{"
                                            "\"cfd_forced_trigger\":false,"
                                            "\"cfd_fractional_time\":0.0,"
                                            "\"cfd_trigger_source\":0,"
                                            "\"channel_number\":1,"
                                            "\"crate_id\":1,"
                                            "\"energy\":1235.0,"
                                            "\"energy_sums\":[],"
                                            "\"event_length\":7,"
                                            "\"external_time\":4771708667090.0,"
                                            "\"filter_baseline\":0.0,"
                                            "\"filter_time\":0.0,"
                                            "\"finish_code\":false,"
                                            "\"header_length\":6,"
                                            "\"qdc\":[],"
                                            "\"slot_id\":1,"
                                            "\"time\":4771708666967.0,"
                                            "\"trace\":[196080109],"
                                            "\"trace_out_of_range\":false}";

        record evt;
        evt.energy = 1235;
        evt.slot_id = 1;
        evt.channel_number = 1;
        evt.crate_id = 1;
        evt.event_length = 7;
        evt.external_time = record::time_type(4771708667090);
        evt.time = record::time_type(4771708666967);
        evt.header_length = 6;
        evt.trace = std::vector<size_t>(1, 0xBAFF1ED);

        SUBCASE("Comparison operators") {
            record eventB = evt;
            CHECK(evt == eventB);

            eventB.energy = 1234;
            CHECK(evt != eventB);

            eventB.time = record::time_type(4771708666968);
            CHECK(evt < eventB);
            CHECK(eventB > evt);
        }

        SUBCASE("Verify JSON output and input") {
            record rec;
            CHECK_THROWS_AS(json_to_record("", rec), xia::pixie::error::error);
            CHECK_THROWS_AS(json_to_record("{}", rec), xia::pixie::error::error);

            CHECK(record_to_json(evt) == json_str);
            json_to_record(json_str, rec);
            CHECK(rec == evt);
        }

        SUBCASE("Verify streamed output") {
            std::ostringstream buf;
            buf << evt;
            CHECK(buf.str() == "crate: 1 slot: 1 channel: 1 time: 4771708666967 energy: 1235");
        }
    }

    TEST_CASE("Data Validity") {
        records recs;
        buffer leftover;

        SUBCASE("Verify data pointer") {
            CHECK_THROWS_WITH_AS(decode_data_block(nullptr, 0, 30474, 250, recs, leftover),
                                 "buffer pointed to an invalid location", xia::pixie::error::error);
        }

        std::vector<uint32_t> empty(4, 0);
        SUBCASE("Verify minimum buffer size") {
            CHECK_THROWS_WITH_AS(decode_data_block(empty.data(), 0, 30474, 250, recs, leftover),
                                 "minimum data buffer size is 1", xia::pixie::error::error);
        }
        SUBCASE("Verify valid frequency") {
            CHECK_THROWS_WITH_AS(
                decode_data_block(empty.data(), empty.size(), 29000, 189, recs, leftover),
                "invalid frequency: 189", xia::pixie::error::error);
        }
        SUBCASE("Verify minimum 100 MSPS revision") {
            CHECK_THROWS_WITH_AS(
                decode_data_block(empty.data(), empty.size(), 1, 250, recs, leftover),
                "minimum supported firmware rev is 17562", xia::pixie::error::error);
        }
        SUBCASE("Verify minimum 250 MSPS revision") {
            CHECK_THROWS_WITH_AS(
                decode_data_block(empty.data(), empty.size(), 20000, 250, recs, leftover),
                "minimum supported firmware rev is 20466", xia::pixie::error::error);
        }
        SUBCASE("Verify minimum 500 MSPS revision") {
            CHECK_THROWS_WITH_AS(
                decode_data_block(empty.data(), empty.size(), 29000, 500, recs, leftover),
                "minimum supported firmware rev is 29432", xia::pixie::error::error);
        }
    }
    /*
     * Testing data decoding and encoding requires tons of different data combinations.
     * We're going to define most of the building blocks below. These will then be used
     * to construct all combinations that we'll need to perform our tests.
     */
    record init_event(size_t freq, double cfd_scale, size_t event_len, size_t header_len,
                      size_t trig_source, bool cfd_force, bool ets, bool esum, bool qdc, bool trc) {
        record expected;
        double cfd_frac_time = 3085.0;

        if (cfd_force) {
            cfd_frac_time = 0;
        }

        double filter_conv;
        switch (freq) {
            case 250:
                filter_conv = 8e-9;
                expected.cfd_fractional_time = record::time_type(
                    ((cfd_frac_time / cfd_scale) - static_cast<double>(trig_source)) * 4e-9);
                break;
            case 500:
                filter_conv = 10e-9;
                expected.cfd_fractional_time = record::time_type(
                    ((cfd_frac_time / cfd_scale) + static_cast<double>(trig_source) - 1) * 2e-9);
                break;
            default:
                filter_conv = 10e-9;
                expected.cfd_fractional_time =
                    record::time_type((cfd_frac_time / cfd_scale) * 10e-9);
                break;
        }

        expected.cfd_forced_trigger = cfd_force;
        expected.cfd_trigger_source = trig_source;
        expected.channel_number = 10;
        expected.crate_id = 0;
        expected.energy = 480.0;
        if (esum) {
            expected.energy_sums = {123, 456, 789};
            expected.filter_baseline = 159.357;
        }
        expected.event_length = event_len;
        expected.filter_time = record::time_type(17514317598928 * filter_conv);
        expected.finish_code = true;
        expected.header_length = header_len;
        if (qdc) {
            expected.qdc = {147, 258, 369, 963, 852, 741, 159, 357};
        }
        expected.slot_id = 2;
        expected.time = record::time_type(expected.cfd_fractional_time + expected.filter_time);
        if (trc) {
            expected.trace = {29,  31,  28, 30, 27, 30, 30, 31, 28, 29, 29, 30, 36, 86, 270,
                              426, 268, 80, 34, 29, 27, 32, 28, 29, 31, 32, 27, 33, 28, 29};
        }
        expected.trace_length = expected.trace.size();
        expected.trace_out_of_range = true;
        if (ets) {
            expected.external_time = record::time_type(145445310572568);
        }
        return expected;
    }

    buffer generate_data(uint32_t word0, uint32_t word1, uint32_t word2, uint32_t word3, bool ets,
                         bool esum, bool qdc, bool trc) {
        static const record::trace_type packed_trace = {
            2031645, 1966108, 1966107, 2031646, 1900572, 1966109, 5636132, 27918606,
            5243148, 1900578, 2097179, 1900572, 2097183, 2162715, 1900572};
        static const record::energy_sums_type energy_sums = {123, 456, 789, 1126128484};
        static const record::qdc_type qdcs = {147, 258, 369, 963, 852, 741, 159, 357};

        buffer data = {word0, word1, word2, word3};
        if (esum) {
            data.insert(data.end(), energy_sums.begin(), energy_sums.end());
        }
        if (qdc) {
            data.insert(data.end(), qdcs.begin(), qdcs.end());
        }
        if (ets) {
            buffer vals = {538060824, 33864};
            data.insert(data.end(), vals.begin(), vals.end());
        }
        if (trc) {
            data.insert(data.end(), packed_trace.begin(), packed_trace.end());
        }
        return data;
    }

    void check_decoded_data(const record& result, const record& expected) {
        CHECK(result.cfd_forced_trigger == expected.cfd_forced_trigger);
        CHECK(result.cfd_fractional_time == expected.cfd_fractional_time);
        CHECK(result.cfd_trigger_source == expected.cfd_trigger_source);
        CHECK(result.channel_number == expected.channel_number);
        CHECK(result.crate_id == expected.crate_id);
        CHECK(result.energy == expected.energy);
        CHECK(result.energy_sums == expected.energy_sums);
        CHECK(result.event_length == expected.event_length);
        CHECK(result.external_time == expected.external_time);
        CHECK(result.filter_time == expected.filter_time);
        CHECK(doctest::Approx(result.filter_baseline).epsilon(0.00001) == expected.filter_baseline);
        CHECK(result.finish_code == expected.finish_code);
        CHECK(result.header_length == expected.header_length);
        CHECK(result.qdc == expected.qdc);
        CHECK(result.slot_id == expected.slot_id);
        CHECK(result.time == expected.time);
        CHECK(result.trace == expected.trace);
        CHECK(result.trace_length == expected.trace_length);
        CHECK(result.trace_out_of_range == expected.trace_out_of_range);
    }

    TEST_CASE("Decoding Failure Modes") {
        records recs;
        buffer leftover;
        SUBCASE("Event Length = 0") {
            buffer data = {3221241898, 123456789, 202182637, 1966560};
            CHECK_THROWS_WITH_AS(
                decode_data_block(data, 17562, 100, recs, leftover),
                "bad event length: 0", xia::pixie::error::error);
        }
        SUBCASE("Random header") {
            buffer data = {3533011249, 3957994, 292194509, 84778};
            CHECK_THROWS_WITH_AS(
                decode_data_block(data, 17562, 100, recs, leftover),
                "unknown header length: 23", xia::pixie::error::error);
        }
        SUBCASE("event length != header length + 0.5*trace length") {
            auto data =
                generate_data(3223257130, 123456789, 202182637, 1966560, false, true, true, true);
            CHECK_THROWS_WITH_AS(
                decode_data_block(data, 17562, 100, recs, leftover),
                "event does not match header and trace: crate=0, slot=0, chan=0, event_length=15, header_length=16, trace_length=0",
                xia::pixie::error::error);
        }
        SUBCASE("Invalid Header Length") {
            auto data =
                generate_data(3224678442, 123456789, 202182637, 1966560, false, true, true, true);
            CHECK_THROWS_WITH_AS(
                decode_data_block(data, 17562, 100, recs, leftover),
                "unknown header length: 11", xia::pixie::error::error);
        }
        SUBCASE("Invalid slot - too small") {
            buffer data = {3221766154, 123456789, 202182637, 480};
            CHECK_THROWS_WITH_AS(
                decode_data_block(data, 17562, 100, recs, leftover),
                "bad slot id: 0", xia::pixie::error::error);
        }
        SUBCASE("Invalid slot - too big") {
            buffer data = {3221766394, 123456789, 202182637, 480};
            CHECK_THROWS_WITH_AS(
                decode_data_block(data, 17562, 100, recs, leftover),
                "bad slot id: 15", xia::pixie::error::error);
        }
        SUBCASE("Invalid revision for external timestamp") {
            auto data = generate_data(2151882794, 123456789, 2349666285, 2149450208, true, true,
                                      true, true);
            CHECK_THROWS_WITH_AS(
                decode_data_block(data, 17562, 100, recs, leftover),
                "external timestamps not introduced until revision 30980",
                xia::pixie::error::error);
        }
        SUBCASE("CFD forced but still had a time") {
            auto data =
                generate_data(2148024362, 123456789, 2164195328, 480, false, false, false, false);
            CHECK_THROWS_WITH_AS(
                decode_data_block(data, 30474, 250, recs, leftover),
                "data corruption: cfd was forced but still recorded a time",
                xia::pixie::error::error);
        }
    }

    TEST_CASE("Leftover handling") {
        records recs;
        buffer leftover;
        SUBCASE("Missing core header words") {
            buffer data = {2149990442, 3735933136};
            decode_data_block(data, 34688, 250, recs, leftover);
            CHECK(recs.size() == 0);
            CHECK(leftover.size() == 2);
            CHECK(leftover == data);
        }
        SUBCASE("Missing trace") {
            buffer data = {2149990442, 3735933136, 1275924461, 2149450208};
            decode_data_block(data, 34688, 250, recs, leftover);
            CHECK(recs.size() == 0);
            CHECK(leftover.size() == 4);
            CHECK(leftover == data);
        }
        SUBCASE("A full record, with a partial") {
            auto evt = init_event(250, 16384, 19, 4, 1, false, false, false, false, true);
            auto data = generate_data(2149990442, 3735933136, 1275924461, 2149450208, false, false,
                                      false, true);
            std::vector<unsigned int> extra_data = {2149990442, 3735933136, 1275924461, 2149450208};
            data.insert(data.end(), extra_data.begin(), extra_data.end());

            decode_data_block(data, 34688, 250, recs, leftover);

            CHECK(recs.size() == 1);
            check_decoded_data(recs[0], evt);
            CHECK(leftover.size() == 4);
            CHECK(leftover == extra_data);
        }
    }

    TEST_CASE("17562-100") {
        records recs;
        buffer leftover;
        auto evt = init_event(100, 65536, 31, 16, 0, false, false, true, true, true);
        auto data =
            generate_data(3225354282, 3735933136, 202182637, 1966560, false, true, true, true);
        decode_data_block(data, 17562, 100, recs, leftover);
        check_decoded_data(recs[0], evt);
    }
    TEST_CASE("29432-100") {
        records recs;
        buffer leftover;
        auto evt = init_event(100, 65536, 31, 16, 0, false, false, true, true, true);
        auto data =
            generate_data(2151612458, 3735933136, 202182637, 1999328, false, true, true, true);
        decode_data_block(data, 29432, 100, recs, leftover);
        check_decoded_data(recs[0], evt);
    }
    TEST_CASE("30474-100") {
        records recs;
        buffer leftover;
        auto evt = init_event(100, 32768, 31, 16, 0, false, false, true, true, true);
        auto data =
            generate_data(2151612458, 3735933136, 202182637, 1999328, false, true, true, true);
        decode_data_block(data, 30474, 100, recs, leftover);
        check_decoded_data(recs[0], evt);
    }
    TEST_CASE("34688-100") {
        records recs;
        buffer leftover;
        auto evt = init_event(100, 32768, 33, 18, 0, false, true, true, true, true);
        auto data =
            generate_data(2151882794, 3735933136, 202182637, 2149450208, true, true, true, true);
        decode_data_block(data, 34688, 100, recs, leftover);
        check_decoded_data(recs[0], evt);
    }
    TEST_CASE("20466-250") {
        records recs;
        buffer leftover;
        auto evt = init_event(250, 65536, 31, 16, 0, false, false, true, true, true);
        auto data =
            generate_data(3225354282, 3735933136, 202182637, 1966560, false, true, true, true);
        decode_data_block(data, 20466, 250, recs, leftover);
        check_decoded_data(recs[0], evt);
    }
    TEST_CASE("27361-250") {
        records recs;
        buffer leftover;
        auto evt = init_event(250, 32768, 31, 16, 1, false, false, true, true, true);
        auto data =
            generate_data(3225354282, 3735933136, 2349666285, 1966560, false, true, true, true);
        decode_data_block(data, 27361, 250, recs, leftover);
        check_decoded_data(recs[0], evt);
    }
    TEST_CASE("29432-250") {
        records recs;
        buffer leftover;
        auto evt = init_event(250, 32768, 31, 16, 1, false, false, true, true, true);
        auto data =
            generate_data(2151612458, 3735933136, 2349666285, 1999328, false, true, true, true);
        decode_data_block(data, 29432, 250, recs, leftover);
        check_decoded_data(recs[0], evt);
    }
    TEST_CASE("30474-250") {
        records recs;
        buffer leftover;
        auto evt = init_event(250, 16384, 31, 16, 1, false, false, true, true, true);
        auto data =
            generate_data(2151612458, 3735933136, 1275924461, 1999328, false, true, true, true);
        decode_data_block(data, 30474, 250, recs, leftover);
        check_decoded_data(recs[0], evt);
    }
    TEST_CASE("34688-250") {
        records recs;
        buffer leftover;
        SUBCASE("header") {
            auto evt = init_event(250, 16384, 4, 4, 1, false, false, false, false, false);
            auto data = generate_data(2148024362, 3735933136, 1275924461, 2147484128, false, false,
                                      false, false);
            decode_data_block(data, 34688, 250, recs, leftover);
            check_decoded_data(recs[0], evt);
        }
        SUBCASE("header - forced cfd") {
            auto evt = init_event(250, 16384, 19, 4, 1, true, false, false, false, true);
            auto data = generate_data(2149990442, 3735933136, 3221229549, 2149450208, false, false,
                                      false, true);
            decode_data_block(data, 34688, 250, recs, leftover);
            check_decoded_data(recs[0], evt);
        }
        SUBCASE("header_ets") {
            auto evt = init_event(250, 16384, 21, 6, 1, false, true, false, false, true);
            auto data = generate_data(2150260778, 3735933136, 1275924461, 2149450208, true, false,
                                      false, true);
            decode_data_block(data, 34688, 250, recs, leftover);
            check_decoded_data(recs[0], evt);
        }
        SUBCASE("header_esum") {
            auto evt = init_event(250, 16384, 23, 8, 1, false, false, true, false, true);
            auto data = generate_data(2150531114, 3735933136, 1275924461, 2149450208, false, true,
                                      false, true);
            decode_data_block(data, 34688, 250, recs, leftover);
            check_decoded_data(recs[0], evt);
        }
        SUBCASE("header_esum_ets") {
            auto evt = init_event(250, 16384, 25, 10, 1, false, true, true, false, true);
            auto data = generate_data(2150801450, 3735933136, 1275924461, 2149450208, true, true,
                                      false, true);
            decode_data_block(data, 34688, 250, recs, leftover);
            check_decoded_data(recs[0], evt);
        }
        SUBCASE("header_qdc") {
            auto evt = init_event(250, 16384, 27, 12, 1, false, false, false, true, true);
            auto data = generate_data(2151071786, 3735933136, 1275924461, 2149450208, false, false,
                                      true, true);
            decode_data_block(data, 34688, 250, recs, leftover);
            check_decoded_data(recs[0], evt);
        }
        SUBCASE("header_qdc_ets") {
            auto evt = init_event(250, 16384, 29, 14, 1, false, true, false, true, true);
            auto data = generate_data(2151342122, 3735933136, 1275924461, 2149450208, true, false,
                                      true, true);
            decode_data_block(data, 34688, 250, recs, leftover);
            check_decoded_data(recs[0], evt);
        }
        SUBCASE("header_esum_qdc_ets") {
            auto evt = init_event(250, 16384, 33, 18, 1, false, true, true, true, true);
            auto data = generate_data(2151882794, 3735933136, 1275924461, 2149450208, true, true,
                                      true, true);
            decode_data_block(data, 34688, 250, recs, leftover);
            check_decoded_data(recs[0], evt);
        }
    }
    TEST_CASE("46540-250") {
        records recs;
        buffer leftover;
        auto evt = init_event(250, 16384, 33, 18, 1, false, true, true, true, true);
        auto data =
            generate_data(2151882890, 3735933136, 1275924461, 2149450208, true, true, true, true);
        decode_data_block(data, 46540, 250, recs, leftover);
        check_decoded_data(recs[0], evt);
    }
    TEST_CASE("29432-500") {
        records recs;
        buffer leftover;
        auto evt = init_event(500, 8192, 31, 16, 6, false, false, true, true, true);
        auto data =
            generate_data(2151612458, 3735933136, 3423408109, 1999328, false, true, true, true);
        decode_data_block(data, 29432, 500, recs, leftover);
        check_decoded_data(recs[0], evt);
    }
    TEST_CASE("34688-500") {
        records recs;
        buffer leftover;
        auto evt = init_event(500, 8192, 33, 18, 6, false, true, true, true, true);
        auto data =
            generate_data(2151882794, 3735933136, 3423408109, 2149450208, true, true, true, true);
        decode_data_block(data, 34688, 500, recs, leftover);
        check_decoded_data(recs[0], evt);
    }
}