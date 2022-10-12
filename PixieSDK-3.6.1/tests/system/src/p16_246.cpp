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

/** @file p16_246.cpp
 * @brief Tests the system when we allow the external FIFO to fill up.
 *
 * For now, we've hard coded the pathing for our Linux test machine. This test
 * isn't really intended to be run anywhere else for the moment. We'll clean up
 * the pathing at a later date.
 */
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include <pixie/pixie16/crate.hpp>
#include <pixie/pixie16/memory.hpp>

int main() {
    xia::logging::start("log", "stdout", false);
    xia::logging::set_level(xia::log::level::info);

    xia::pixie::crate::crate crate;

    static const std::vector<std::string> firmware_list = {
        "version=r33341, revision=15, adc-msps=500, adc-bits=14, device=sys, file=/usr/local/xia/pixie/firmware/revf_general_14b500m_r35207/firmware/syspixie16_revfgeneral_adc500mhz_r33341.bin",
        "version=r34687, revision=15, adc-msps=500, adc-bits=14, device=fippi, file=/usr/local/xia/pixie/firmware/revf_general_14b500m_r35207/firmware/fippixie16_revfgeneral_14b500m_r34687.bin",
        "version=r35207, revision=15, adc-msps=500, adc-bits=14, device=dsp, file=/usr/local/xia/pixie/firmware/revf_general_14b500m_r35207/dsp/Pixie16DSP_revfgeneral_14b500m_r35207.ldr",
        "version=r35207, revision=15, adc-msps=500, adc-bits=14, device=var, file=/usr/local/xia/pixie/firmware/revf_general_14b500m_r35207/dsp/Pixie16DSP_revfgeneral_14b500m_r35207.var",
        "version=r33339, revision=15, adc-msps=250, adc-bits=14, device=sys, file=/usr/local/xia/pixie/firmware/revf_general_14b250m_r33356/firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin",
        "version=r33332, revision=15, adc-msps=250, adc-bits=14, device=fippi, file=/usr/local/xia/pixie/firmware/revf_general_14b250m_r33356/firmware/fippixie16_revfgeneral_14b250m_r33332.bin",
        "version=r33356, revision=15, adc-msps=250, adc-bits=14, device=dsp, file=/usr/local/xia/pixie/firmware/revf_general_14b250m_r33356/dsp/Pixie16DSP_revfgeneral_14b250m_r33356.ldr",
        "version=r33356, revision=15, adc-msps=250, adc-bits=14, device=var, file=/usr/local/xia/pixie/firmware/revf_general_14b250m_r33356/dsp/Pixie16DSP_revfgeneral_14b250m_r33356.var"};

    for (auto& firmware : firmware_list) {
        auto fw = xia::pixie::firmware::parse(firmware, ',');
        if (xia::pixie::firmware::check(crate.firmware, fw)) {
            std::string what("duplicate firmware: ");
            what += firmware;
            throw std::runtime_error(what);
        }
        xia::pixie::firmware::add(crate.firmware, fw);
    }

    try {
        crate.initialize(false);
        crate.set_firmware();
        crate.probe();
        crate.boot();

        xia::pixie::module::number_slots modules;
        crate.import_config("pixie.json", modules);

        auto module = crate.modules.front();

        module->write_var(xia::pixie::param::module_var::SynchWait, 0);
        module->write_var(xia::pixie::param::module_var::InSynch, 1);
        module->start_listmode(xia::pixie::hw::run::run_mode::new_run);

        module->fifo_buffers = 1;
        module->fifo_hold_usecs = (unsigned long)1e7;
        std::this_thread::sleep_for(std::chrono::seconds(2));

        xia::pixie::hw::memory::fifo fifo(*module);

        size_t last_fifo_val = 0;
        size_t fifo_change_timeout = 10;
        xia::util::timepoint fifo_change_time;
        while (fifo_change_time.secs() < fifo_change_timeout) {
            auto level = fifo.level();
            xia_log(xia::log::level::info) << "On-board FIFO level: " << level;
            xia_log(xia::log::level::info)
                << "Worker FIFO level: " << module->read_list_mode_level();

            if (level == last_fifo_val)
                fifo_change_time.start();

            last_fifo_val = level;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        fifo_change_time.stop();

        xia_log(xia::log::level::info) << "On-board FIFO level: " << fifo.level();
        xia_log(xia::log::level::info) << "Worker FIFO level: " << module->read_list_mode_level();
        xia_log(xia::log::level::info) << "On-board FIFO level hasn't changed in " << fifo_change_time << ". It's filled up!";

        static const size_t wait_in_seconds = 120;
        xia::util::timepoint timepoint;
        timepoint.start();
        while (timepoint.secs() < wait_in_seconds) {
            xia_log(xia::log::level::info)
                << "Sleeping for " << wait_in_seconds - timepoint.secs() << " more seconds.";
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
        timepoint.end();
        module->run_end();
    } catch (xia::pixie::error::error& error) {
        std::cerr << error.what() << std::endl;
        return EXIT_FAILURE;
    }

    //    unsigned int fifo_words = 0;
    //    unsigned int size_of_buffer_queue = 6488520;
    //    while (fifo_words <= size_of_buffer_queue) {
    //        Pixie16CheckExternalFIFOStatus(&fifo_words, 0);
    //        std::cout << "Module 0 has " << fifo_words << std::endl;
    //        std::this_thread::sleep_for(std::chrono::seconds(1));
    //    }
    //
    //    std::cout << "Module 0's FIFO workers have been filled!" << std::endl
    //              << "Keeping the data run going for another 120 seconds." << std::endl;
    //
    //    std::this_thread::sleep_for(std::chrono::seconds(120));
    //
    //    if (Pixie16EndRun(number_of_modules) != 0) {
    //        std::cerr << "Module 0 failed to end its run properly." << std::endl;
    //        return EXIT_FAILURE;
    //    }
    //
    //    Pixie16ExitSystem(number_of_modules);
}
