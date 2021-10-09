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

/** @file test_pixie16app.cpp
 * @brief Defines unit tests for some functions defined in pixie16app.c
 */

#include "doctest/doctest.h"
#include "pixie16app_export.h"
#include "pixie16app_globals.h"

#include <complex>
#include <fstream>
#include <vector>

TEST_SUITE("app/pixie16app.c") {
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
    TEST_CASE("Pixie16complexFFT") {
        static const size_t length = 256;
        double data[length] = {
            0.0,     0, 0.00394, 0, 0.00787, 0, 0.01181, 0, 0.01575, 0, 0.01968, 0, 0.02362, 0,
            0.02756, 0, 0.03149, 0, 0.03543, 0, 0.03936, 0, 0.04329, 0, 0.04723, 0, 0.05116, 0,
            0.05509, 0, 0.05902, 0, 0.06295, 0, 0.06688, 0, 0.07081, 0, 0.07473, 0, 0.07866, 0,
            0.08258, 0, 0.08651, 0, 0.09043, 0, 0.09435, 0, 0.09827, 0, 0.10218, 0, 0.1061,  0,
            0.11001, 0, 0.11393, 0, 0.11784, 0, 0.12174, 0, 0.12565, 0, 0.12956, 0, 0.13346, 0,
            0.13736, 0, 0.14126, 0, 0.14515, 0, 0.14905, 0, 0.15294, 0, 0.15683, 0, 0.16072, 0,
            0.1646,  0, 0.16848, 0, 0.17236, 0, 0.17624, 0, 0.18011, 0, 0.18399, 0, 0.18785, 0,
            0.19172, 0, 0.19558, 0, 0.19944, 0, 0.2033,  0, 0.20715, 0, 0.211,   0, 0.21485, 0,
            0.21869, 0, 0.22253, 0, 0.22637, 0, 0.2302,  0, 0.23403, 0, 0.23786, 0, 0.24168, 0,
            0.2455,  0, 0.24931, 0, 0.25312, 0, 0.25693, 0, 0.26073, 0, 0.26453, 0, 0.26832, 0,
            0.27212, 0, 0.2759,  0, 0.27968, 0, 0.28346, 0, 0.28723, 0, 0.291,   0, 0.29477, 0,
            0.29853, 0, 0.30228, 0, 0.30603, 0, 0.30978, 0, 0.31352, 0, 0.31726, 0, 0.32099, 0,
            0.32471, 0, 0.32843, 0, 0.33215, 0, 0.33586, 0, 0.33957, 0, 0.34327, 0, 0.34696, 0,
            0.35065, 0, 0.35434, 0, 0.35802, 0, 0.36169, 0, 0.36536, 0, 0.36902, 0, 0.37267, 0,
            0.37633, 0, 0.37997, 0, 0.38361, 0, 0.38724, 0, 0.39087, 0, 0.39449, 0, 0.3981,  0,
            0.40171, 0, 0.40531, 0, 0.40891, 0, 0.4125,  0, 0.41608, 0, 0.41966, 0, 0.42323, 0,
            0.42679, 0, 0.43035, 0, 0.4339,  0, 0.43745, 0, 0.44098, 0, 0.44451, 0, 0.44804, 0,
            0.45155, 0, 0.45506, 0, 0.45856, 0, 0.46206, 0, 0.46555, 0, 0.46903, 0, 0.4725,  0,
            0.47597, 0, 0.47943, 0};

        CHECK(Pixie16complexFFT(&data[0], (unsigned int) (length * 0.5)) == 0);
        std::vector<double> result;
        for (unsigned int i = 0; i < length - 1; i += 2)
            result.push_back(std::abs(std::complex<double>(data[i], data[i + 1])));

        std::vector<double> expected_result_from_numpy = {
            31.33370189, 9.90883821, 4.93118791, 3.28607334, 2.46549191, 1.97388292, 1.64658857,
            1.41313889,  1.23832497, 1.10259511, 0.9942218,  0.90574373, 0.83218754, 0.77011063,
            0.71705435,  0.67121593, 0.63124348, 0.59610335, 0.56499177, 0.53727427, 0.51244345,
            0.49008863,  0.46987393, 0.451522,   0.43480182, 0.41951945, 0.40551087, 0.39263649,
            0.38077676,  0.36982873, 0.35970329, 0.35032291, 0.34161991, 0.33353488, 0.32601554,
            0.31901569,  0.3124944,  0.30641526, 0.30074583, 0.29545713, 0.29052321, 0.28592076,
            0.28162886,  0.27762867, 0.27390321, 0.27043715, 0.26721666, 0.26422927, 0.26146369,
            0.25890975,  0.25655826, 0.25440097, 0.25243045, 0.25064003, 0.24902378, 0.24757641,
            0.24629328,  0.2451703,  0.24420395, 0.24339122, 0.2427296,  0.24221709, 0.24185211,
            0.24163356,  0.24156079, 0.24163356, 0.24185211, 0.24221709, 0.2427296,  0.24339122,
            0.24420395,  0.2451703,  0.24629328, 0.24757641, 0.24902378, 0.25064003, 0.25243045,
            0.25440097,  0.25655826, 0.25890975, 0.26146369, 0.26422927, 0.26721666, 0.27043715,
            0.27390321,  0.27762867, 0.28162886, 0.28592076, 0.29052321, 0.29545713, 0.30074583,
            0.30641526,  0.3124944,  0.31901569, 0.32601554, 0.33353488, 0.34161991, 0.35032291,
            0.35970329,  0.36982873, 0.38077676, 0.39263649, 0.40551087, 0.41951945, 0.43480182,
            0.451522,    0.46987393, 0.49008863, 0.51244345, 0.53727427, 0.56499177, 0.59610335,
            0.63124348,  0.67121593, 0.71705435, 0.77011063, 0.83218754, 0.90574373, 0.9942218,
            1.10259511,  1.23832497, 1.41313889, 1.64658857, 1.97388292, 2.46549191, 3.28607334,
            4.93118791,  9.90883821};
        for (unsigned int i = 0; i < result.size(); i++)
            CHECK(expected_result_from_numpy[i] == doctest::Approx(result[i]).epsilon(1e-4));
    }
    TEST_CASE("Pixie16ComputeFastFiltersOffline") {
        unsigned short module_number = 0, channel_number = 0;

        FastFilterRange_Address[module_number] = 0x0004a00d;
        Pixie_Devices[module_number]
            .DSP_Parameter_Values[FastFilterRange_Address[module_number] - DATA_MEMORY_ADDRESS] = 0;

        FastLength_Address[module_number] = 0x0004a0b0;
        Pixie_Devices[module_number].DSP_Parameter_Values[FastLength_Address[module_number] +
                                                          channel_number - DATA_MEMORY_ADDRESS] =
            15;

        FastGap_Address[module_number] = 0x0004a0c0;
        Pixie_Devices[module_number].DSP_Parameter_Values[FastGap_Address[module_number] +
                                                          channel_number - DATA_MEMORY_ADDRESS] = 0;

        CFDDelay_Address[module_number] = 0x0004a270;
        Pixie_Devices[module_number].DSP_Parameter_Values[CFDDelay_Address[module_number] +
                                                          channel_number - DATA_MEMORY_ADDRESS] = 8;

        CFDScale_Address[module_number] = 0x0004a280;
        Pixie_Devices[module_number].DSP_Parameter_Values[CFDScale_Address[module_number] +
                                                          channel_number - DATA_MEMORY_ADDRESS] = 0;

        Module_Information[module_number].Module_ADCMSPS = 250;
        Module_Information[module_number].Module_ADCBits = 16;

        static std::vector<unsigned short> trc = {
            437, 436, 434, 434,  437,  437,  438,  435,  434,  438,  439,  437,  438,  434,
            435, 439, 438, 434,  434,  435,  437,  440,  439,  435,  437,  439,  438,  435,
            436, 436, 437, 439,  435,  433,  434,  436,  439,  441,  436,  437,  439,  438,
            438, 435, 434, 434,  438,  438,  434,  434,  437,  440,  439,  438,  434,  436,
            439, 439, 437, 436,  434,  436,  438,  437,  436,  437,  440,  440,  439,  436,
            435, 437, 501, 1122, 2358, 3509, 3816, 3467, 2921, 2376, 1914, 1538, 1252, 1043,
            877, 750, 667, 619,  591,  563,  526,  458,  395,  403,  452,  478,  492,  498,
            494, 477, 460, 459,  462,  461,  460,  456,  452,  452,  455,  453,  446,  441,
            440, 444, 456, 459,  451,  450,  447,  445,  449,  456,  456,  455};
        auto trace_length = static_cast<unsigned short>(trc.size());

        std::string filename = std::tmpnam(nullptr);
        std::ofstream outfile(filename, std::ios::binary | std::ios::out);
        for (uint16_t val : trc)
            outfile.write(reinterpret_cast<const char*>(&val), sizeof(val));
        outfile.close();

        SUBCASE("Check trace is valid") {
            CHECK(Pixie16ComputeFastFiltersOffline(nullptr, module_number, channel_number, 0,
                                                   trace_length, nullptr, nullptr, nullptr) == -1);
        }

        SUBCASE("Check if results array is valid") {
            CHECK(Pixie16ComputeFastFiltersOffline(nullptr, module_number, channel_number, 0,
                                                   trace_length, trc.data(), nullptr,
                                                   nullptr) == -2);
        };

        std::vector<double> result(trace_length, 1337);
        SUBCASE("Check if CFD array is valid") {
            CHECK(Pixie16ComputeFastFiltersOffline(nullptr, module_number, channel_number, 0,
                                                   trace_length, trc.data(), result.data(),
                                                   nullptr) == -3);
        }

        std::vector<double> cfd(trace_length, 1337);
        SUBCASE("Check if Module number is valid") {
            /// NOTE: This value depends on the global variable PRESET_MAX_MODULES, which has a value of 24 (e.g. 2 crates)
            CHECK(Pixie16ComputeFastFiltersOffline(nullptr, 40, channel_number, 0, trace_length,
                                                   trc.data(), result.data(), cfd.data()) == -4);
        }

        SUBCASE("Verify that the trace length is long enough") {
            CHECK(Pixie16ComputeFastFiltersOffline(nullptr, module_number, channel_number, 0, 1,
                                                   trc.data(), result.data(), cfd.data()) == -5);
        }

        SUBCASE("Verify the binary can be opened") {
            CHECK(Pixie16ComputeFastFiltersOffline("/tmp/notavalidfile.bin", module_number,
                                                   channel_number, 0, trace_length, trc.data(),
                                                   result.data(), cfd.data()) == -6);
        }

        SUBCASE("Verify the fast filter is correct") {
            CHECK(Pixie16ComputeFastFiltersOffline(filename.c_str(), module_number, channel_number,
                                                   0, trace_length, trc.data(), result.data(),
                                                   cfd.data()) == 0);

            std::vector<double> expected_fast_filter = {
                0.3,      0.3,      0.3,      0.3,      0.3,      0.3,      0.3,       0.3,
                0.3,      0.3,      0.3,      0.3,      0.3,      0.3,      0.3,       0.3,
                0.3,      0.3,      0.3,      0.3,      0.3,      0.3,      0.3,       0.3,
                0.3,      0.3,      0.3,      0.3,      0.3,      0.3,      0.3,       0.3,
                0.3,      0.3,      0.3,      0.3,      0.3,      0.3,      0.3,       0.3,
                0.3,      0.3,      0.3,      0.3,      0.3,      0.3,      0.3,       0.3,
                0.3,      0.3,      0.3,      0.3,      0.3,      0.3,      0.3,       0.3,
                0.3,      0.3,      0.3,      0.3,      0.2,      0,        0.0666667, 0.233333,
                0.4,      0.466667, 0.466667, 0.233333, 0.266667, 0.266667, 0.133333,  0.0666667,
                2.16667,  25.0333,  89.2,     191.867,  304.467,  405.3,    488.2,     552.967,
                602.2,    638.8,    665.9,    685.967,  700.833,  711.4,    718.967,   724.833,
                729.933,  734.167,  737.333,  738.167,  736.633,  735.367,  735.833,   737.167,
                738.867,  740.833,  742.567,  743.967,  744.933,  745.7,    742.3,     697.367,
                569.967,  365.7,    140.967,  -60.5,    -225.6,   -354.433, -452.6,    -525.767,
                -579.933, -620.067, -648.867, -669.033, -683.833, -695.467, -705.4,    -713.567,
                -719.2,   -720,     -716.533, -713.667};

            for (unsigned int i = 0; i < trace_length; i++)
                CHECK(expected_fast_filter[i] == doctest::Approx(result[i]).epsilon(0.01));
        }

        SUBCASE("Verify the CFD results") {
            CHECK(Pixie16ComputeFastFiltersOffline(filename.c_str(), module_number, channel_number,
                                                   0, trace_length, trc.data(), result.data(),
                                                   cfd.data()) == 0);
            std::vector<double> expected_cfd = {
                0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
                0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
                0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
                0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
                0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
                0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
                -3,     -9,     -7,     -2,     3,      5,      5,      -2,     -1,     -1,
                -5,     -7,     56,     742,    2667,   5747,   9128,   12159,  14644,  16582,
                18054,  19150,  19963,  20572,  21017,  21334,  21565,  21743,  21833,  21274,
                19444,  16389,  12965,  9902,   7429,   5526,   4100,   3061,   2300,   1740,
                1323,   1029,   700,    -824,   -4799,  -11054, -17891, -23960, -28867, -32694,
                -35653, -37888, -39564, -40827, -41743, -42390, -42863, -43235, -43431, -42328,
                -38675, -32571, -25725, -19595};
            for (unsigned int i = 0; i < trace_length; i++)
                CHECK(expected_cfd[i] == doctest::Approx(cfd[i]).epsilon(0.01));
        }

        SUBCASE("Sets an invalid module variant") {
            Module_Information[module_number].Module_ADCMSPS = 1000;
            CHECK(Pixie16ComputeFastFiltersOffline(filename.c_str(), module_number, channel_number,
                                                   0, trace_length, trc.data(), result.data(),
                                                   cfd.data()) == 0);
            for (auto i : cfd)
                CHECK(0.0 == i);
        }
    }
    TEST_CASE("Statistics Computations") {
        uint32_t stats[N_DSP_PAR - DSP_IO_BORDER] = {
            0,          1899999978, 0,          1899999930, 0,          0,          100,
            0,          1,          6,          3,          9,          2159411345, 2159411345,
            2159411345, 2159411345, 2159411345, 2159411345, 2159411345, 2159411345, 2159411345,
            2159411345, 2159411345, 2159411345, 2159411345, 2159411345, 2159411345, 2159411345,
            327680,     65536,      0,          0,          2306,       1,          16380,
            0,          0,          512,        802,        0,          1418,       0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          2362135268, 2374999609, 2374999626, 2374999644, 2374999661,
            2374999679, 2374999696, 2374999714, 2374999732, 2374999748, 2374999766, 2374999783,
            2374999801, 2374999818, 2374999836, 2374999853, 0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          396141,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          197961,     0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          0,
            0,          0,          0,          0,          0,          0,          201795124};

        unsigned short module_number = 0;
        Module_Information[module_number].Module_ADCMSPS = 250;

        SUBCASE("Pixie16ComputeLiveTime") {
            LiveTimeA_Address[module_number] = 0x0004a37f;
            LiveTimeB_Address[module_number] = 0x0004a38f;
            CHECK(18.9 ==
                  doctest::Approx(Pixie16ComputeLiveTime(stats, module_number, 0)).epsilon(0.1));
        }

        SUBCASE("Pixie16ComputeInputCountRate") {
            /// NOTE: This function uses LiveTime to compute its values.
            FastPeaksA_Address[module_number] = 0x0004a39f;
            FastPeaksB_Address[module_number] = 0x0004a3af;
            CHECK(
                20963 ==
                doctest::Approx(Pixie16ComputeInputCountRate(stats, module_number, 0)).epsilon(1));
        }

        SUBCASE("Pixie16ComputeRealTime") {
            RunTimeA_Address[module_number] = 0x0004a342;
            RunTimeB_Address[module_number] = 0x0004a343;
            CHECK(18.9 ==
                  doctest::Approx(Pixie16ComputeRealTime(stats, module_number)).epsilon(0.1));
        }

        SUBCASE("Pixie16ComputeOutputCountRate") {
            /// NOTE: This function uses RunTime.
            ChanEventsA_Address[module_number] = 0x0004a41f;
            ChanEventsB_Address[module_number] = 0x0004a42f;
            CHECK(
                10419 ==
                doctest::Approx(Pixie16ComputeOutputCountRate(stats, module_number, 0)).epsilon(1));
        }

        SUBCASE("Pixie16ComputeProcessedEvents") {
            NumEventsA_Address[module_number] = 0x0004a345;
            NumEventsB_Address[module_number] = 0x0004a346;
            CHECK(100 == Pixie16ComputeProcessedEvents(stats, module_number));
        }
    }

    TEST_CASE("Pixie16ComputeSlowFiltersOffline") {
        /// We're limited on how much testing we can do here because this function needs to open a list mode data file.
        unsigned short module_number = 0, channel_number = 0;
        SlowFilterRange_Address[module_number] = 0x0004a00c;
        Pixie_Devices[module_number]
            .DSP_Parameter_Values[SlowFilterRange_Address[module_number] - DATA_MEMORY_ADDRESS] = 1;

        SlowLength_Address[module_number] = 0x0004a090;
        Pixie_Devices[module_number].DSP_Parameter_Values[SlowLength_Address[module_number] +
                                                          channel_number - DATA_MEMORY_ADDRESS] = 6;

        SlowGap_Address[module_number] = 0x0004a0a0;
        Pixie_Devices[module_number].DSP_Parameter_Values[SlowGap_Address[module_number] +
                                                          channel_number - DATA_MEMORY_ADDRESS] = 2;

        PreampTau_Address[module_number] = 0x0004a230;
        Pixie_Devices[module_number].DSP_Parameter_Values[PreampTau_Address[module_number] +
                                                          channel_number - DATA_MEMORY_ADDRESS] =
            1008981770;

        Module_Information[module_number].Module_ADCMSPS = 250;
        Module_Information[module_number].Module_ADCBits = 16;

        SUBCASE("Verify the trace is valid") {
            CHECK(Pixie16ComputeSlowFiltersOffline(nullptr, module_number, channel_number, 0, 0,
                                                   nullptr, nullptr) == -1);
        }

        static const unsigned short trace_length = 124;
        unsigned short trc[trace_length] = {
            437, 436, 434, 434,  437,  437,  438,  435,  434,  438,  439,  437,  438,  434,
            435, 439, 438, 434,  434,  435,  437,  440,  439,  435,  437,  439,  438,  435,
            436, 436, 437, 439,  435,  433,  434,  436,  439,  441,  436,  437,  439,  438,
            438, 435, 434, 434,  438,  438,  434,  434,  437,  440,  439,  438,  434,  436,
            439, 439, 437, 436,  434,  436,  438,  437,  436,  437,  440,  440,  439,  436,
            435, 437, 501, 1122, 2358, 3509, 3816, 3467, 2921, 2376, 1914, 1538, 1252, 1043,
            877, 750, 667, 619,  591,  563,  526,  458,  395,  403,  452,  478,  492,  498,
            494, 477, 460, 459,  462,  461,  460,  456,  452,  452,  455,  453,  446,  441,
            440, 444, 456, 459,  451,  450,  447,  445,  449,  456,  456,  455};
        SUBCASE("Verify the results array is valid") {
            CHECK(Pixie16ComputeSlowFiltersOffline(nullptr, module_number, channel_number, 0,
                                                   trace_length, trc, nullptr) == -2);
        }

        double result[trace_length] = {0.};
        SUBCASE("Verify the module number is valid") {
            CHECK(Pixie16ComputeSlowFiltersOffline(nullptr, 400, channel_number, 0, trace_length,
                                                   trc, result) == -3);
        }

        SUBCASE("Verify the trace is long enough") {
            CHECK(Pixie16ComputeSlowFiltersOffline(nullptr, module_number, channel_number, 0, 0,
                                                   trc, result) == -4);
        }

        SUBCASE("Verify that the file is valid") {
            CHECK(Pixie16ComputeSlowFiltersOffline("/tmp/trace-elfje.bin", module_number,
                                                   channel_number, 0, trace_length, trc,
                                                   result) == -5);
        }

        SUBCASE("Verify results") {
            std::string filename = std::tmpnam(nullptr);
            std::ofstream outfile(filename, std::ios::binary | std::ios::out);
            for (uint16_t val : trc)
                outfile.write(reinterpret_cast<const char*>(&val), sizeof(val));
            outfile.close();
            Pixie16ComputeSlowFiltersOffline(filename.c_str(), module_number, channel_number, 0,
                                             trace_length, trc, result);

            std::vector<double> expected = {0,        0,         0,        0,         0,
                                            0,        0,         0,        0,         0,
                                            0,        0,         0,        0,         0,
                                            0,        0,         0,        0,         0,
                                            0,        0,         0,        0,         0,
                                            0,        0,         0,        0,         0,
                                            0,        0,         0,        0,         0,
                                            0,        0,         0,        0,         0,
                                            0,        0,         0,        0,         0,
                                            0,        0,         0,        0,         0,
                                            0,        0,         0,        0,         0,
                                            0,        0.659449,  0.659516, 0.329814,  0.659472,
                                            2.23E-05, -6.70E-05, 0.32968,  -0.329769, -0.000178643,
                                            1.31863,  3.29687,   4.61572,  4.61581,   2.9673,
                                            2.63753,  2.63755,   23.0792,  248.596,   881.624,
                                            1895.13,  3010.18,   4010.17,  4828.82,   5467.78,
                                            5955.74,  6319.73,   6588.44,  6787.25,   6931.66,
                                            7034.53,  7111.35,   7171.68,  7221.8,    7262.68,
                                            7292.03,  7299.28,   7286.42,  7275.54,   7280.16,
                                            7293.67,  7312.14,   7332.23,  7349.99,   7362.12,
                                            7368.97,  7376.49,   7385.33,  7393.2,    7379.65,
                                            7160.05,  6531.62,   5523.7,   4415.56,   3421.83,
                                            2605.81,  1967.84,   1481.85,  1121.16,   858.71,
                                            666.164,  525.711,   426.801,  354.267,   296.897,
                                            250.078,  214.799,   191.719,  190.729};
            for (auto i = 0; i < trace_length; i++)
                CHECK(expected[i] == doctest::Approx(result[i]).epsilon(0.01));
        }
    }
    TEST_CASE("Pixie16InitSystem") {
        SUBCASE("Tests around module number checking") {
            CHECK(Pixie16InitSystem(0, nullptr, 1) == -1);
            CHECK(Pixie16InitSystem(PRESET_MAX_MODULES + 1, nullptr, 1) == -1);
        }

        SUBCASE("Tests related to PXISlotMap verification") {
            CHECK(Pixie16InitSystem(1, nullptr, 1) == -2);
        }

        unsigned short slot_map[1] = {5};
        SUBCASE("Tests related to Initialization ") {
            CHECK(Pixie16InitSystem(1, slot_map, 1) == 0);
        }
    }
    TEST_CASE("Pixie16ReadSglChanADCTrace") {
        unsigned short dummy;
        unsigned short slot_map[1] = {5};

        CHECK(Pixie16ReadSglChanADCTrace(&dummy, 0, 10, 0) == -1);
        Pixie16InitSystem(1, slot_map, 1);
        CHECK(Pixie16ReadSglChanADCTrace(&dummy, 0, 0, 50) == -2);
        CHECK(Pixie16ReadSglChanADCTrace(&dummy, MAX_ADC_TRACE_LEN + 1, 0, 15) == -3);
    }
}
