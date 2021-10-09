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

/** @file pcf8574.cpp
 * @brief Implements data structures to read the Pixie-16's PCF 8574 chip.
 */

#include <pixie/pixie16/module.hpp>
#include <pixie/pixie16/pcf8574.hpp>

namespace xia {
namespace pixie {
namespace hw {
namespace i2c {
pcf8574::pcf8574(module::module& module, int reg, uint32_t SDA, uint32_t SCL, uint32_t CTRL,
                 bool trace)
    : bitbash(module, reg, SDA, SCL, CTRL, trace) {}

uint8_t pcf8574::read_a_byte() {
    module::module::bus_guard guard(module);

    start();
    write_ack(0x43, "pcf8574::read_a_byte: no ACK after DevSel");

    uint8_t value = read_ack();

    stop();

    return value;
}
};  // namespace i2c
};  // namespace hw
};  // namespace pixie
};  // namespace xia
