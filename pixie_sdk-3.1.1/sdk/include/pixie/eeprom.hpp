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

/** @file eeprom.hpp
 * @brief Defines data structures and functions for working with EEPROMs
 */

#ifndef PIXIE_EEPROM_H
#define PIXIE_EEPROM_H

#include <array>
#include <cstdint>
#include <vector>

#include <pixie/error.hpp>
#include <pixie/util.hpp>

#include <pixie/pixie16/hw.hpp>

namespace xia {
namespace pixie {
/**
 * @brief Namespace for working with hardware EEPROM chips.
 */
namespace eeprom {
/*
 * Local error
 */
typedef pixie::error::error error;

/**
 * @brief Supported tags. Gaps allow additions in each section.
 */
enum struct tag {
    null = 0, /*!< invalid */
    /*
     * Build state
     */
    model = 10, /*!< model, part number */
    serial_num, /*!< serial number */
    revision, /*!< revision string */
    mod_strike, /*!< hardware modifications */
    /*
     * Parameters
     */
    index = 30, /*!< number, index, slot, id */
    size, /*!< number of items, channels, modules */
    format_ver, /*!< format version */
    /*
     * Analog Front End (AFE)
     */
    adc_msps = 100, /*!< The ADC's sampling frequency*/
    adc_bits, /*!< The ADC's bit resolution */
    adc_clk_div, /*!< The ADC's clock division, defined as the adc_msps / fpga_clk. */
    fpga_clk, /*!< The Signal processing FPGA's clock frequency.*/
    end = 0xff /*!< Last maps to erase EEPROM value */
};

/**
 * @brief Defines the data types that values may take.
 */
enum element_type { null, string, num8, num16, num32, ieee_float, mac, ipv4 };

using mac_addr = std::array<uint8_t, 6>;
using ipv4_addr = std::array<uint8_t, 4>;

/**
 * @brief Defines the EEPROM element descriptor.
 */
struct element_decs {
    tag key;
    std::string name;
    element_type type;
    size_t size;
    bool mandatory;
    bool depreciated;
};

/**
 * @brief The EEPROM header that's used to validate the EEPROM contents.
 */
struct header {
    static const size_t size = 4 + 1 + 1;

    uint32_t crc;
    uint8_t control;

    header();

    void clear();

    int version() const;
};

using contents = std::vector<uint8_t>;

/**
 * @brief A data structure holding the data decoded from a module's EEPROM.
 */
struct eeprom {
    contents data;
    util::crc32 crc;

    header hdr;

    int format;

    std::string model;
    int serial_num;
    int revision;
    int mod_strike;

    int num_channels;
    int max_channels;

    hw::configs configs;

    eeprom();

    void clear();

    /**
     * Process the loaded data
     */
    void process();

    /**
     * Is there a valid EEPROM configuration.
     */
    bool valid() const;

    /*
     * Tag queries.
     */
    std::string get_string(const tag key, size_t count = 1);
    int get_number(const tag key, size_t count = 1);
    double get_float(const tag key, size_t count = 1);
    mac_addr get_mac(const tag key, size_t count = 1);
    ipv4_addr get_ipv4(const tag key, size_t count = 1);

    /*
     * Little endian reads of values from the data.
     */
    template<typename T>
    T get32(const size_t offset) const;
    template<typename T>
    T get16(const size_t offset) const;
    template<typename T>
    T get8(const size_t offset) const;

    /**
     * Find an instance of a tag in the data
     */
    size_t find(const tag key, size_t count = 0) const;

    /**
     * Return the tag's descriptor
     */
    const element_decs& lookup(const tag key) const;

protected:
    void throw_bad_tag_type(const tag key);
};

template<typename T>
inline T eeprom::get32(const size_t offset) const {
    return static_cast<T>((data[offset + 3] << 24) | (data[offset + 2] << 16) |
                          (data[offset + 1] << 8) | data[offset + 0]);
}

template<typename T>
inline T eeprom::get16(const size_t offset) const {
    return static_cast<T>((data[offset + 1] << 8) | data[offset + 0]);
}

template<typename T>
inline T eeprom::get8(const size_t offset) const {
    return static_cast<T>(data[offset + 0]);
}

}  // namespace eeprom
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_EEPROM_H
