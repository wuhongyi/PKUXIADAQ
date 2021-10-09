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

/** @file hw.hpp
 * @brief Defines hardware specific data for the Pixie-16 modules.
 */

#ifndef PIXIE_HW_H
#define PIXIE_HW_H

#include <array>
#include <stdexcept>
#include <vector>

#include <cstdint>

#include <pixie/error.hpp>

namespace xia {
namespace pixie {
/**
 * @brief Collects Pixie-16 specific hardware information.
 */
namespace hw {
/**
 * @brief Revision tags for the Pixie-16 hardware
 */
enum rev_tag {
    rev_A = 10,
    rev_B,
    rev_C,
    rev_D,
    rev_E,
    rev_F,
    rev_G,
    rev_H,
    rev_I,
    rev_J,
    rev_K,
    rev_L
};

/**
 * Maximum number of slots in a crate.
 */
static const int max_slots = 13;

/**
 * Maximum number of channels a module can have.
 */
static const int max_channels = 32;

/**
 * Maximum ADC trace size.
 */
static const size_t max_adc_trace_length = 8192;

/**
 * Maximum histogram size.
 */
static const size_t max_histogram_length = 32768;

/**
 * IO buffer length
 */
static const size_t io_buffer_length = 65536;

/**
 * System FPGA clock frequency in MHz
 */
static const size_t system_clock_mhz = 100;

/**
 * FIFO size in words
 */
static const size_t fifo_size_words = 131072;

/**
 * DMA block size.
 */
static const size_t max_dma_block_size = 8192;

/**
 * EEPROM block size
 */
static const size_t eeprom_block_size = 128;

/**
 * Baselines block length
 *
 * Length of each baseline (default: 2 timestamp words + 16 baselines)
 */
static const size_t baselines_block_len = 18;

/**
 * PCI Bus maximum data transfer rate in MBytes.
 */
static const size_t pci_bus_datarate = 100;

/**
 * @brief Defines a type for memory register addresses.
 */
typedef uint32_t address;

/**
 * @brief Defines a type for the basic unit of read/write communication
 */
typedef uint32_t word;

/**
 * @brief defines a type for a pointer to a word
 */
typedef word* word_ptr;

/**
 * @brief Defines a type for a vector of words. Used to handle blocks of memory.
 */
typedef std::vector<word> words;

/**
 * @brief Defines a type for ADC trace words.
 */
typedef uint16_t adc_word;

/**
 * @brief Defines a type used to handle IO data from the hardware.
 */
typedef std::array<word, io_buffer_length> io_buffer;

/**
 * @brief Defines a type for an ADC trace buffer, as read from the channel
 */
typedef std::array<word, max_adc_trace_length> adc_trace_buffer;

/**
 * @brief defines a type for ADC traces
 */
typedef std::vector<adc_word> adc_trace;

/**
 * @brief Defines a type for Hardware errors
 */
typedef error::error error;

/**
 * @brief The object representing the physical hardware configuration
 *
 * This includes information about the number of channels, and their
 * analog front-end hardware.
 */
struct config {
    /**
     * Channel index in variable array.
     */
    int index;

    /*
     * Analog front end (AFE)
     */
    int adc_bits;
    int adc_msps;
    int adc_clk_div;
    int fpga_clk_mhz;

    config(int adc_bits, int adc_msps, int adc_clk_div, int fpga_clk_mhz);
    config();

    bool operator==(const config& cfg);
    bool operator!=(const config& cfg);

    void clear();
};

typedef std::vector<config> configs;

/**
 * Convertor. Use with caution as this steps around the type system.
 */
template<typename I, typename O>
inline void convert(I vin, O& vout) {
    vout = static_cast<O>(vin);
}

/**
 * @brief Wait in microseconds.
 * @todo We need to check how well this works.
 * @param microseconds The number of microseconds we should wait.
 */
void wait(size_t microseconds);

/**
 * Bus interface calls.
 */
static inline word read_word(void* addr, const int offset) {
    volatile word* p = static_cast<volatile word*>(addr);
    return *(p + (offset / 4));
}

static inline void write_word(void* addr, const int offset, const word value) {
    volatile word* p = static_cast<volatile word*>(addr);
    *(p + (offset / 4)) = value;
}
}  // namespace hw
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_HW_H
