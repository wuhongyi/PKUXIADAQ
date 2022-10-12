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

/** @file test_pixie16_module.cpp
 * @brief
 */

#include <doctest/doctest.h>

#include <pixie/error.hpp>
#include <pixie/log.hpp>

#include <pixie/pixie16/defs.hpp>
#include <pixie/pixie16/module.hpp>
#include <pixie/pixie16/sim.hpp>

using crate_error = xia::pixie::crate::error;

static const std::vector<std::string> module_def = {
    "device-number=0 slot=2 revision=15 eeprom-format=1 serial-num=1034 num-channels=16 "
    "adc-bits=14 adc-msps=500 adc-clk-div=5",
    "device-number=1 slot=6 revision=15 eeprom-format=1 serial-num=1035 num-channels=16 "
    "adc-bits=14 adc-msps=500 adc-clk-div=5",
    "device-number=2 slot=10 revision=17 eeprom-format=1 serial-num=2034 num-channels=32 "
    "adc-bits=14 adc-msps=500 adc-clk-div=5"};

static const size_t test_modules = module_def.size();

static void test_setup() {
    xia::logging::start("log", "stdout", false);
    xia::logging::set_level(xia::log::level::off);
    for (auto& def : module_def) {
        xia::pixie::sim::add_module_def(def);
    }
}

TEST_SUITE("Crate: modules") {
    TEST_CASE("SETUP") {
        test_setup();
    }
    TEST_CASE("none") {
        using namespace xia::pixie;
        sim::crate crate;
        CHECK(crate.num_modules == 0);
        CHECK(crate.revision == -1);
        CHECK(crate.modules.size() == 0);
        CHECK(crate.offline.size() == 0);
        CHECK(crate.busy() == false);
        CHECK(crate.users() == 0);
        CHECK_THROWS_WITH_AS(crate.probe(), "crate is not ready", crate_error);
        CHECK_THROWS_WITH_AS(crate.ready(), "crate is not ready", crate_error);
        CHECK_THROWS_WITH_AS(crate.probe(), "crate is not ready", crate_error);
        CHECK_THROWS_WITH_AS(crate[0], "module number out of range: 0", crate_error);
        CHECK_THROWS_WITH_AS(crate.find(0), "module slot not found", crate_error);
    }
    TEST_CASE("initialize") {
        using namespace xia::pixie;
        sim::crate crate;
        CHECK_NOTHROW(crate.initialize());
        CHECK_NOTHROW(crate.probe());
        SUBCASE("Finding and offline") {
            CHECK_NOTHROW(crate.find(2));
            CHECK_THROWS_WITH_AS(crate[-1], "module number out of range: -1", crate_error);
            CHECK_THROWS_WITH_AS(crate[20], "module number out of range: 20", crate_error);
            CHECK_THROWS_WITH_AS(crate.find(7), "module slot not found", crate_error);
            CHECK(crate.num_modules == test_modules);
            CHECK(crate.modules.size() == test_modules);
            CHECK(crate.offline.size() == hw::max_slots - test_modules);
            CHECK(crate.backplane.sync_waiters.size() == crate.num_modules);
            CHECK_THROWS_WITH_AS(crate.set_offline(7), "module number out of range: 7",
                                 crate_error);
            CHECK_NOTHROW(crate.set_offline(1));
            CHECK(crate.num_modules == test_modules - 1);
            CHECK(crate.modules.size() == test_modules - 1);
            CHECK(crate.offline.size() == hw::max_slots - test_modules + 1);
            CHECK(crate.backplane.sync_waiters.size() == crate.num_modules);
        }
        using namespace xia::pixie;
        SUBCASE("FIFO defaults") {
            CHECK(crate[0].fifo_buffers == module::module::default_fifo_buffers);
            CHECK(crate[0].fifo_run_wait_usecs == module::module::default_fifo_run_wait_usec);
            CHECK(crate[0].fifo_idle_wait_usecs == module::module::default_fifo_idle_wait_usec);
            CHECK(crate[0].fifo_hold_usecs == module::module::default_fifo_hold_usec);
            CHECK(crate[0].fifo_dma_trigger_level ==
                  module::module::default_fifo_dma_trigger_level);
            CHECK(crate[0].fifo_bandwidth == 0);
        }
        SUBCASE("FIFO bandwidth") {
            CHECK_NOTHROW(crate[0].set_fifo_buffers(50));
            CHECK(crate[0].fifo_buffers == 50);
        }
        SUBCASE("FIFO run wait") {
            CHECK_NOTHROW(crate[0].set_fifo_run_wait(module::module::min_fifo_run_wait_usec));
            CHECK(crate[0].fifo_run_wait_usecs == module::module::min_fifo_run_wait_usec);
            CHECK_NOTHROW(crate[0].set_fifo_run_wait(module::module::max_fifo_run_wait_usec));
            CHECK(crate[0].fifo_run_wait_usecs == module::module::max_fifo_run_wait_usec);
            CHECK_THROWS_WITH_AS(
                crate[0].set_fifo_run_wait(module::module::min_fifo_run_wait_usec - 1),
                "module: num=0,slot=2: fifo: run wait value out of range", crate_error);
            CHECK_THROWS_WITH_AS(
                crate[0].set_fifo_run_wait(module::module::max_fifo_run_wait_usec + 1),
                "module: num=0,slot=2: fifo: run wait value out of range", crate_error);
        }
        SUBCASE("FIFO idle wait") {
            CHECK_NOTHROW(crate[0].set_fifo_idle_wait(module::module::min_fifo_idle_wait_usec));
            CHECK(crate[0].fifo_idle_wait_usecs == module::module::min_fifo_idle_wait_usec);
            CHECK_NOTHROW(crate[0].set_fifo_idle_wait(module::module::max_fifo_idle_wait_usec));
            CHECK(crate[0].fifo_idle_wait_usecs == module::module::max_fifo_idle_wait_usec);
            CHECK_THROWS_WITH_AS(
                crate[0].set_fifo_idle_wait(module::module::min_fifo_idle_wait_usec - 1),
                "module: num=0,slot=2: fifo: idle wait value out of range", crate_error);
            CHECK_THROWS_WITH_AS(
                crate[0].set_fifo_idle_wait(module::module::max_fifo_idle_wait_usec + 1),
                "module: num=0,slot=2: fifo: idle wait value out of range", crate_error);
        }
        SUBCASE("FIFO hold") {
            CHECK_NOTHROW(crate[0].set_fifo_hold(module::module::min_fifo_hold_usec));
            CHECK(crate[0].fifo_hold_usecs == module::module::min_fifo_hold_usec);
            CHECK_NOTHROW(crate[0].set_fifo_hold(module::module::max_fifo_hold_usec));
            CHECK(crate[0].fifo_hold_usecs == module::module::max_fifo_hold_usec);
            CHECK_THROWS_WITH_AS(crate[0].set_fifo_hold(module::module::min_fifo_hold_usec - 1),
                                 "module: num=0,slot=2: fifo: hold value out of range",
                                 crate_error);
            CHECK_THROWS_WITH_AS(crate[0].set_fifo_hold(module::module::max_fifo_hold_usec + 1),
                                 "module: num=0,slot=2: fifo: hold value out of range",
                                 crate_error);
        }
        SUBCASE("FIFO DMA trigger level") {
            CHECK_NOTHROW(
                crate[0].set_fifo_dma_trigger_level(module::module::min_fifo_dma_trigger_level));
            CHECK(crate[0].fifo_dma_trigger_level == module::module::min_fifo_dma_trigger_level);
            CHECK_NOTHROW(
                crate[0].set_fifo_dma_trigger_level(module::module::max_fifo_dma_trigger_level));
            CHECK(crate[0].fifo_dma_trigger_level == module::module::max_fifo_dma_trigger_level);
            CHECK_THROWS_WITH_AS(
                crate[0].set_fifo_dma_trigger_level(module::module::min_fifo_dma_trigger_level - 1),
                "module: num=0,slot=2: fifo: dma trigger level value out of range", crate_error);
            CHECK_THROWS_WITH_AS(
                crate[0].set_fifo_dma_trigger_level(module::module::max_fifo_dma_trigger_level + 1),
                "module: num=0,slot=2: fifo: dma trigger level value out of range", crate_error);
        }
        SUBCASE("FIFO bandwidth") {
            CHECK_NOTHROW(crate[0].set_fifo_bandwidth(0));
            CHECK(crate[0].fifo_bandwidth == 0);
            CHECK_NOTHROW(crate[0].set_fifo_bandwidth(100));
            CHECK(crate[0].fifo_bandwidth == 100);
            CHECK_THROWS_WITH_AS(crate[0].set_fifo_bandwidth(101),
                                 "module: num=0,slot=2: fifo: bandwidth value out of range",
                                 crate_error);
        }
    }
    TEST_CASE("assign slots") {
        using namespace xia::pixie;
        sim::crate crate;
        CHECK_NOTHROW(crate.initialize());
        CHECK_NOTHROW(crate.probe());
        CHECK_NOTHROW(crate.assign(module::number_slots{{0, 2}, {1, 6}, {2, 10}}));
        CHECK(crate[0].slot == 2);
        CHECK(crate[1].slot == 6);
        CHECK(crate[2].slot == 10);
        CHECK_NOTHROW(crate.assign(module::number_slots{{2, 2}, {1, 6}, {0, 10}}));
        CHECK(crate[0].slot == 10);
        CHECK(crate[1].slot == 6);
        CHECK(crate[2].slot == 2);
        CHECK_NOTHROW(crate.assign(module::number_slots{{1, 2}, {0, 6}, {2, 10}}));
        CHECK(crate[0].slot == 6);
        CHECK(crate[1].slot == 2);
        CHECK(crate[2].slot == 10);
        CHECK_NOTHROW(crate.assign(module::number_slots{{1, 2}, {0, 6}}, false));
        CHECK(crate.num_modules == test_modules - 1);
        CHECK(crate.offline.size() == hw::max_slots - test_modules + 1);
        CHECK(crate[0].slot == 6);
        CHECK(crate[1].slot == 2);
        CHECK_NOTHROW(crate.assign(module::number_slots{{0, 6}}));
        CHECK(crate.num_modules == test_modules - 2);
        CHECK(crate.offline.size() == hw::max_slots - test_modules + 1);
        CHECK(crate[0].slot == 6);
    }
    TEST_CASE("run") {
        using namespace xia::pixie;
        sim::crate crate;
        CHECK_NOTHROW(crate.initialize());
        CHECK_NOTHROW(crate.probe());
        CHECK_NOTHROW(crate[0].start_histograms(hw::run::run_mode::new_run));
        CHECK_NOTHROW(crate[1].start_histograms(hw::run::run_mode::new_run));
        CHECK_NOTHROW(crate[2].start_histograms(hw::run::run_mode::new_run));
        CHECK_NOTHROW(crate[0].run_end());
        CHECK_NOTHROW(crate[2].run_end());
        CHECK_NOTHROW(crate[1].run_end());
    }
    TEST_CASE("backplane") {
        using namespace xia::pixie;
        using namespace xia::pixie::param;
        sim::crate crate;
        CHECK_NOTHROW(crate.initialize());
        CHECK_NOTHROW(crate.probe());
        SUBCASE("wired-or trigger pullup") {
            CHECK_NOTHROW(
                crate[0].write(module_param::module_csrb, 1 << hw::bit::MODCSRB_CPLDPULLUP));
            CHECK_THROWS_WITH_AS(
                crate[2].write(module_param::module_csrb, 1 << hw::bit::MODCSRB_CPLDPULLUP),
                "module: num=2,slot=10: wired-or trigger pullups leader role already taken: 0",
                crate_error);
            CHECK_NOTHROW(crate[0].write(module_param::module_csrb, 0));
            CHECK_NOTHROW(
                crate[2].write(module_param::module_csrb, 1 << hw::bit::MODCSRB_CPLDPULLUP));
            CHECK_NOTHROW(crate[2].write(module_param::module_csrb, 0));
        }
        SUBCASE("director mode") {
            CHECK_NOTHROW(crate[0].write(module_param::module_csrb, 1 << hw::bit::MODCSRB_DIRMOD));
            CHECK_THROWS_WITH_AS(
                crate[2].write(module_param::module_csrb, 1 << hw::bit::MODCSRB_DIRMOD),
                "module: num=2,slot=10: director leader role already taken: 0", crate_error);
            CHECK_NOTHROW(crate[0].write(module_param::module_csrb, 0));
            CHECK_NOTHROW(crate[2].write(module_param::module_csrb, 1 << hw::bit::MODCSRB_DIRMOD));
            CHECK_NOTHROW(crate[2].write(module_param::module_csrb, 0));
        }
        SUBCASE("run leader") {
            CHECK_NOTHROW(
                crate[0].write(module_param::module_csrb, 1 << hw::bit::MODCSRB_CHASSISMASTER));
            CHECK_THROWS_WITH_AS(
                crate[2].write(module_param::module_csrb, 1 << hw::bit::MODCSRB_CHASSISMASTER),
                "module: num=2,slot=10: run leader role already taken: 0", crate_error);
            CHECK_NOTHROW(crate[0].write(module_param::module_csrb, 0));
            CHECK_NOTHROW(
                crate[2].write(module_param::module_csrb, 1 << hw::bit::MODCSRB_CHASSISMASTER));
            CHECK_NOTHROW(crate[2].write(module_param::module_csrb, 0));
        }
        SUBCASE("no synch wait") {
            CHECK(crate.backplane.sync_waits == 0);
            CHECK(crate.backplane.sync_waiters[0] == false);
            CHECK(crate.backplane.sync_waiters[1] == false);
            CHECK(crate.backplane.sync_waiters[2] == false);
            CHECK_NOTHROW(crate[0].write(module_param::synch_wait, 0));
            CHECK(crate.backplane.sync_waits == 0);
            CHECK(crate.backplane.sync_waiters[0] == false);
            CHECK(crate.backplane.sync_waiters[1] == false);
            CHECK(crate.backplane.sync_waiters[2] == false);
            CHECK_NOTHROW(crate[0].start_histograms(hw::run::run_mode::new_run));
            CHECK_NOTHROW(crate[0].run_end());
        }
        SUBCASE("incomplete synch wait") {
            CHECK_NOTHROW(crate[0].write(module_param::synch_wait, 1));
            CHECK(crate.backplane.sync_waits == 1);
            CHECK(crate.backplane.sync_waiters[0] == true);
            CHECK(crate.backplane.sync_waiters[1] == false);
            CHECK(crate.backplane.sync_waiters[2] == false);
            CHECK_THROWS_WITH_AS(
                crate[0].start_histograms(hw::run::run_mode::new_run),
                "module: num=0,slot=2: sync wait mode enabled and not all modules in sync wait state",
                crate_error);
            CHECK_NOTHROW(crate[0].run_end());
        };
        SUBCASE("complete synch wait") {
            CHECK_NOTHROW(crate[0].write(module_param::synch_wait, 1));
            CHECK_NOTHROW(crate[1].write(module_param::synch_wait, 1));
            CHECK_NOTHROW(crate[2].write(module_param::synch_wait, 1));
            CHECK(crate.backplane.sync_waits == test_modules);
            CHECK(crate.backplane.sync_waiters[0] == true);
            CHECK(crate.backplane.sync_waiters[1] == true);
            CHECK(crate.backplane.sync_waiters[2] == true);
            CHECK_NOTHROW(crate[0].start_histograms(hw::run::run_mode::new_run));
            CHECK_NOTHROW(crate[0].run_end());
        }
    }
    TEST_CASE("TEARDOWN") {
        xia::logging::stop("log");
    }
}
