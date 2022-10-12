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

/** @file p16_371.cpp
 * @brief Test code used to reproduce a race condition when stopping a run.
 *
 * For now, we've hard coded the pathing for our Linux test machine. This test
 * isn't really intended to be run anywhere else for the moment. We'll clean up
 * the pathing at a later date.
 */
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#ifdef LEGACY_SWITCH
#include <legacy/pixie16app_export.h>
#else
#include <pixie16/pixie16.h>
#endif

void check(int error)
{
    if (error < 0) {
        std::cerr << "error: code=" << error << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

int main() {
    static const unsigned short number_of_modules = 2;
    std::vector<unsigned short> slot_map = {2, 10};
    check(Pixie16InitSystem(number_of_modules, slot_map.data(), 0));

    check(Pixie16BootModule(
        "/usr/local/xia/pixie/firmware/revf_general_14b500m_r35207/firmware/syspixie16_revfgeneral_adc500mhz_r33341.bin",
        "/usr/local/xia/pixie/firmware/revf_general_14b500m_r35207/firmware/fippixie16_revfgeneral_14b500m_r34687.bin",
        nullptr,
        "/usr/local/xia/pixie/firmware/revf_general_14b500m_r35207/dsp/Pixie16DSP_revfgeneral_14b500m_r35207.ldr",
        "./p16_371.set",
        "/usr/local/xia/pixie/firmware/revf_general_14b500m_r35207/dsp/Pixie16DSP_revfgeneral_14b500m_r35207.var",
        0, 0x7F));

    check(Pixie16BootModule(
        "/usr/local/xia/pixie/firmware/revf_general_14b250m_r33356/firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin",
        "/usr/local/xia/pixie/firmware/revf_general_14b250m_r33356/firmware/fippixie16_revfgeneral_14b250m_r33332.bin",
        nullptr,
        "/usr/local/xia/pixie/firmware/revf_general_14b250m_r33356/dsp/Pixie16DSP_revfgeneral_14b250m_r33356.ldr",
        "./p16_371.set",
        "/usr/local/xia/pixie/firmware/revf_general_14b250m_r33356/dsp/Pixie16DSP_revfgeneral_14b250m_r33356.var",
        1, 0x7F));

    check(Pixie16WriteSglModPar("SYNCH_WAIT", 1, 0));
    check(Pixie16WriteSglModPar("SYNCH_WAIT", 1, 1));
    check(Pixie16WriteSglModPar("IN_SYNCH", 0, 0));
    check(Pixie16WriteSglModPar("IN_SYNCH", 0, 1));

    static const size_t max_attempts = 2;
    static const size_t run_time_in_seconds = 3;
    static const size_t seconds_between_attempts = 1;
    unsigned int fifo_words = 0;
    std::cout << "Expecting " << max_attempts << " data runs to happen." << std::endl;
    for (size_t attempt = 0; attempt < max_attempts; attempt++) {
        std::cout << "-----------------" << std::endl
                  << "Starting attempt number: " << attempt << std::endl
                  << "Starting list-mode data run for " << run_time_in_seconds << " seconds"
                  << std::endl;

        if (Pixie16StartListModeRun(2, 0x100, 1) != 0)
            break;

        std::this_thread::sleep_for(std::chrono::seconds(run_time_in_seconds));

        check(Pixie16CheckExternalFIFOStatus(&fifo_words, 0));
        std::cout << "Module 0 has " << fifo_words << std::endl;
        check(Pixie16CheckExternalFIFOStatus(&fifo_words, 1));
        std::cout << "Module 1 has " << fifo_words << std::endl;

        if (Pixie16EndRun(2) != 0) {
            std::cout << "Attempt number " << attempt << " failed!" << std::endl
                      << "-----------------" << std::endl;
            break;
        }

        std::cout << "Ending attempt number: " << attempt << std::endl
                  << "-----------------" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(seconds_between_attempts));
    }
    check(Pixie16ExitSystem(2));
}
