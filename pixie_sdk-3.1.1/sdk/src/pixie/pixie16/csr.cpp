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

/** @file csr.cpp
 * @brief Implements how we handle the Pixie-16 CSR.
 */

#include <pixie/pixie16/csr.hpp>
#include <pixie/pixie16/defs.hpp>
#include <pixie/pixie16/module.hpp>

namespace xia {
namespace pixie {
namespace hw {
namespace csr {
void reset(module::module& module) {
    clear(module, (1 << hw::bit::RUNENA) | (1 << hw::bit::DSPDOWNLOAD) | (1 << hw::bit::PCIACTIVE));
}

word read(module::module& module) {
    return module.read_word(hw::device::CSR);
}

void write(module::module& module, word value) {
    module.write_word(hw::device::CSR, value);
}

void set(module::module& module, word mask) {
    write(module, read(module) | mask);
}

void clear(module::module& module, word mask) {
    write(module, read(module) & ~mask);
}

set_clear::set_clear(module::module& module_, word mask_) : module(module_), mask(mask_) {
    set(module, mask);
}

set_clear::~set_clear() {
    clear(module, mask);
}

void fifo_ready_wait(module::module& module, const size_t polls) {
    size_t count = 0;
    while (count++ < polls) {
        if ((read(module) & (1 << hw::bit::EXTFIFO_WML)) != 0) {
            return;
        }
    }
    throw error(error::code::device_dma_busy, "csr: EXT FIFO failed to get ready for read");
}
}  // namespace csr
}  // namespace hw
}  // namespace pixie
}  // namespace xia
