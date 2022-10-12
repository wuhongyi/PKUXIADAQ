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

/** @file fpga_comms.hpp
 * @brief Defines how we interact with the Pixie-16 communication FPGA.
 */

#ifndef PIXIE_HW_FPGA_COMMS_H
#define PIXIE_HW_FPGA_COMMS_H

#include <pixie/pixie16/fpga.hpp>

namespace xia {
namespace pixie {
namespace hw {
namespace fpga {
/**
 * @brief Data structure used to communicate with the module's System FPGA
 *
 * This FPGA is also called the communications FPGA.
 */
struct comms {
    control ctrl;

    comms(module::module& module, bool trace = false);

    void boot(const firmware::image& image, int retries = 10);

    bool done();
};
}  // namespace fpga
}  // namespace hw
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_HW_FPGA_COMMS_H
