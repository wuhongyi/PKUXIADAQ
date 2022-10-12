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

/** @file i2cm24c64.hpp
 * @brief Defines how we communicate with the Pixie-16's I2C Serial EEPROM
 */

#ifndef PIXIE_HW_I2CM24C64_H
#define PIXIE_HW_I2CM24C64_H

#include <pixie/eeprom.hpp>

#include <pixie/pixie16/i2c_bitbash.hpp>

namespace xia {
namespace pixie {
namespace hw {
namespace i2c {
/**
 * @brief Defines the specifics of working with the I2CM24C64 chip.
 *
 * We use this chip to hold information about the module. For example, its
 * serial number and ADC configuration.
 */
struct i2cm24c64 : public bitbash {
    /**
     * Size of the device in bytes,
     */
    static const size_t size = 64 * 1024 / 8;

    i2cm24c64(module::module& module, int reg, uint32_t SDA, uint32_t SCL, uint32_t CTRL);

    /**
     * @brief Read the EEPROM.
     * @param[in] address The starting address that you'd like to read.
     * @param[in] length How much data to read
     * @param[out] data A data structure to hold the contents of the read
     */
    void read(int address, size_t length, eeprom::contents& data);

    /**
     * @brief Reads the entire EEPROM
     * @param[out] data  A data structure to hold the contents of the read
     */
    void read(eeprom::contents& data);
};
}  // namespace i2c
}  // namespace hw
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_HW_I2CM24C64_H
