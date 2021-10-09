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

/** @file i2c_bitbash.hpp
 * @brief Defines I2C bit bash data structures and functions
 */

#ifndef PIXIE_HW_I2C_BITBASH_H
#define PIXIE_HW_I2C_BITBASH_H

#include <vector>

#include <stdint.h>

namespace xia {
namespace pixie {
namespace module {
class module;
}
namespace hw {
/**
 * @brief Root namespace for working with Pixie-16 I2C chips.
 */
namespace i2c {
/**
 * @brief Defines how we work with the bits in the I2C chips.
 */
struct bitbash {
    module::module& module;

    /**
     * The register offset in the module's address space.
     */
    const int reg;

    /*
     * Bit mask of the signals.
     */
    const uint32_t SDA;
    const uint32_t SCL;
    const uint32_t CTRL;

    const bool trace;

    bitbash(module::module& module, int reg, uint32_t SDA, uint32_t SCL, uint32_t CTRL,
            bool trace = false);
    virtual ~bitbash();

    /*
     * Bus control
     */
    void start();
    void stop();

    /*
     * Byte level writes and reads with ACKs
     */
    void write_ack(uint8_t data, const char* what);
    uint8_t read_ack(bool ack = true);

    /*
     * Low level byte wide access.
     */
    void write(uint8_t data);
    uint8_t read();

    /*
     * ACK control
     */
    bool get_ack();
    void send_ack();
    void send_nack();

    /*
     * Low level I2C access.
     */
    void bus_write(uint8_t data);
    uint8_t bus_read();
};
}  // namespace i2c
}  // namespace hw
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_HW_I2C_BITBASH_H
