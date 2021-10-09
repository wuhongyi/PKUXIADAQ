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

/** @file fpga_comms.cpp
 * @brief Implements how we interact with the Pixie-16 communication FPGA.
 */

#include <iomanip>
#include <iostream>

#include <pixie/pixie16/defs.hpp>
#include <pixie/pixie16/fpga_comms.hpp>
#include <pixie/pixie16/module.hpp>

namespace xia {
namespace pixie {
namespace hw {
namespace fpga {
comms::comms(module::module& module, bool trace)
    : ctrl(module, "comms", control::controls(0xfffff000, 0x00000553, 0x003),
           control::controls(0xfffff000, 0x00000551, 0x001),
           control::regs(hw::device::CFG_DATACS, hw::device::CFG_CTRLCS, hw::device::CFG_RDCS),
           trace) {}

void comms::boot(const firmware::image& image, int retries) {
    module::module::bus_guard guard(ctrl.module);
    ctrl.load(image, retries);
}

bool comms::done() {
    module::module::bus_guard guard(ctrl.module);
    return ctrl.done();
}
};  // namespace fpga
};  // namespace hw
};  // namespace pixie
};  // namespace xia
