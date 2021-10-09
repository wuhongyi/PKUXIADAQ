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

/** @file i2cm24c64.cpp
 * @brief Implements communication with the Pixie-16's I2C Serial EEPROM
 */

#include <pixie/pixie16/i2cm24c64.hpp>
#include <pixie/pixie16/module.hpp>

namespace xia {
namespace pixie {
namespace hw {
namespace i2c {
i2cm24c64::i2cm24c64(module::module& module, int reg, uint32_t SDA, uint32_t SCL, uint32_t CTRL)
    : bitbash(module, reg, SDA, SCL, CTRL) {}

void i2cm24c64::read(int address, size_t length, eeprom::contents& data) {
    module::module::bus_guard guard(module);
    data.clear();
    data.reserve(length);

    start();

    write_ack(0xA0, "i2cm24c64::sequential_read: no ACK after DevSel");
    write_ack(address >> 8, "i2cm24c64::sequential_read: no ACK after addr (MSB)");
    write_ack(address & 0xff, "i2cm24c64::sequential_read: no ACK after addr (LSB)");

    start();
    write_ack(0xA1, "i2cm24c64::sequential_read: no ACK after DevSel");
    for (size_t k = 0; k < length - 1; k++) {
        data.push_back(read_ack());
    }
    data.push_back(read_ack(false));

    stop();
}

void i2cm24c64::read(eeprom::contents& data) {
    read(0, size, data);
}
};  // namespace i2c
};  // namespace hw
};  // namespace pixie
};  // namespace xia
