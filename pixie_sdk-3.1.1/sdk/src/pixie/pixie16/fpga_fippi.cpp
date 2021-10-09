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

/** @file fpga_fippi.cpp
 * @brief Implements data structures and functions for using Pixie-16 FIPPI (Signal Processing) FPGAs
 */

#include <pixie/pixie16/defs.hpp>
#include <pixie/pixie16/fpga_fippi.hpp>
#include <pixie/pixie16/module.hpp>

namespace xia {
namespace pixie {
namespace hw {
namespace fpga {
fippi::fippi(module::module& module_, bool trace)
    : module(module_),
      ctrl_1_2(module, "fippi-1-2", control::controls(0xfffff000, 0x000000f2, 0x0a3),
               control::controls(0xfffff000, 0x00000052, 0x053),
               control::regs(hw::device::CFG_DATACS, hw::device::CFG_CTRLCS, hw::device::CFG_RDCS),
               trace),
      ctrl_3_4(module, "fippi-3-4", control::controls(0xfffff000, 0x00000fa2, 0xaa3),
               control::controls(0xfffff000, 0x000005a2, 0x5a3),
               control::regs(hw::device::CFG_DATACS, hw::device::CFG_CTRLCS, hw::device::CFG_RDCS),
               trace) {}

void fippi::boot(const firmware::image& image, int retries) {
    module::module::bus_guard guard(module);
    ctrl_1_2.load(image, retries);
    ctrl_3_4.load(image, retries);
    wait(10000);
    module.write_word(hw::device::CFG_DCMRST, 0);
}

bool fippi::done() {
    module::module::bus_guard guard(module);
    return ctrl_1_2.done() && ctrl_3_4.done();
}
};  // namespace fpga
};  // namespace hw
};  // namespace pixie
};  // namespace xia
