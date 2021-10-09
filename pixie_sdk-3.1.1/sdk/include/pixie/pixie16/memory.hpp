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

/** @file memory.hpp
 * @brief Defines data and functions used to access Pixie-16 memory registers.
 */

#ifndef PIXIE_HW_MEMORY_H
#define PIXIE_HW_MEMORY_H

#include <cstdint>

#include <pixie/error.hpp>
#include <pixie/fw.hpp>

#include <pixie/pixie16/hw.hpp>
#include <pixie/pixie16/module.hpp>

namespace xia {
namespace pixie {
namespace module {
class module;
}
namespace hw {
/**
 * @brief Pixie-16 memory management
 */
namespace memory {
/*
 * Bus addresses
 */
static const address IO_BUFFER_ADDR = 0x00050000; /* R */
static const address FIFO_MEM_DMA = 0x00200000; /* R */
static const address DSP_MEM_DMA = 0x00300000; /* R */
static const address MCA_MEM_DMA = 0x00400000; /* R */
static const address MCA_MEM_DATA = 0x00400000; /* R W */

/*
 * MCA addresses
 */
static const address HISTOGRAM_MEMORY = 0x00000000;

/**
 * @brief Defines a memory bus for low level communication with the hardware.
 */
struct bus {
    module::module& module;

    bus(module::module& module);

protected:
    /*
     * Low level access.
     */
    void bus_write(int reg, uint32_t data) {
        module.write_word(reg, data);
    }
    uint32_t bus_read(int reg) {
        return module.read_word(reg);
    }
};

/**
 * @brief Defines the communications channel for the DSP.
 */
struct dsp : public bus {
    dsp(module::module& module);

    /*
     * Memory read.
     */
    word read(const address addr);
    word read(const size_t offset, const address addr);
    word read(const size_t channel, const size_t offset, const address addr);

    /*
     * Memory block read.
     *
     * If length is not 0 only the length of data will be read.
     */
    template<class B>
    void read(const address addr, B& values, const size_t length = 0);
    void read(const address addr, word_ptr buffer, const size_t length);

    /*
     * Memory write.
     */
    void write(const address addr, const word value);
    void write(const size_t offset, const address addr, const word value);
    void write(const size_t channel, const size_t offset, const address addr, const word value);

    /*
     * Memory block write.
     */
    void write(const address addr, const words& values);
    void write(const size_t channel, const address addr, const words& values);

private:
    /*
     * DMA set up
     */
    void dma_read(const address addr, word_ptr buffer, const size_t length);
};

template<class B>
inline void dsp::read(const address addr, B& values, const size_t length) {
    if (length > values.size()) {
        throw error(error::code::invalid_value, "dsp: read length greater than buffer size");
    }
    read(addr, values.data(), length != 0 ? length : values.size());
}

/**
 * @brief Defines the memory bus access for the MCA data.
 */
struct mca : public bus {
    mca(module::module& module);

    /*
     * Block read and write.
     */
    void read(const address addr, words& values);
    void read(const address addr, word_ptr values, size_t size);
    void write(const address addr, const words& values);
};

/**
 * @brief Defines the memory bus for interacting with the External FIFO.
 */
struct fifo : public bus {
    fifo(module::module& module);

    /**
     * Level of data currently in the FIFO in words
     */
    size_t level();

    /*
     * Memory block read.
     *
     * If length is not 0 only the length of data will be read.
     */
    template<typename B>
    void read(B& values, const size_t length = 0);
    void read(word_ptr buffer, const size_t length);
};

template<class B>
inline void fifo::read(B& values, const size_t length) {
    if (length > values.size()) {
        throw error(error::code::invalid_value, "fifo: read length greater than buffer size");
    }
    read(values.data(), length != 0 ? length : values.size());
}
}  // namespace memory
}  // namespace hw
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_HW_MEMORY_H
