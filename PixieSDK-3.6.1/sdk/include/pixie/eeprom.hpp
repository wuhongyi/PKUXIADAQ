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
    major_revision, /*!< major revision string */
    minor_revision, /*!< minor revision string */
    mod_strike, /*!< hardware modifications */
    /*
     * Parameters
     */
    index = 30, /*!< number, index, slot, id */
    size, /*!< number of items, channels, modules */
    format_ver, /*!< format version */
    /*
     * Sub-assemblies
     */
    db = 40, /*!< AFE Daughter board */
    /*
     * Analog Front End (AFE)
     */
    adc_msps = 100, /*!< The ADC's sampling frequency */
    adc_bits, /*!< The ADC's bit resolution */
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
 * @brief A daughter board assembly
 */
struct db_assemble {
    std::string label;
    int index;  /**< Daughter board definition table index *. */
    int position; /**< Position on the main board */

    db_assemble();
};

using db_assembles = std::vector<db_assemble>;

/**
 * @brief A data structure holding the data decoded from a module's EEPROM.
 *
 * ### EEPROM Format
 *
 * Read the first 128 bytes from the beginning of the I2C serial EEPROM.
 *
 * #### Version 1
 *   First three words of EEPROM stores serial number and revision number
 *
 * #### Version 2
 *   If the CRC in the header validates it is a version 2 or higher format.
 *
 *   Header: [CRC32 (little endian, 4 bytes)] [CTRL (1 byte)] [RESERVED (1 byte)]
 *   Data: [[ID (1 byte)] [DATA (variable length)]
 *
 * **Supported Ids**
 *   - SERIAL_NUM = 11, little endian number, size = 4
 *   - REVISION = 12, little endian number, size = 2
 *   - END = 255
 *
 * @note The @ref version is the module version and the revision may be
 * referred to the major revision.
 */
struct eeprom {
    using tag = xia::pixie::eeprom::tag;
    using element_type = xia::pixie::eeprom::element_type;
    using contents = xia::pixie::eeprom::contents;
    using db_assemble = xia::pixie::eeprom::db_assemble;
    using db_assembles = xia::pixie::eeprom::db_assembles;

    contents data;
    util::crc32 crc;

    header hdr;

    int format;

    std::string model;
    int serial_num;
    int revision;
    int major_revision;
    int minor_revision;
    int mod_strike;

    int num_channels;
    int max_channels;

    hw::configs configs;

    db_assembles dbs;

    eeprom();

    void clear();
    void clear_data();

    /**
     * Is there a valid EEPROM configuration.
     */
    bool valid() const;

    /**
     * Process the loaded data
     */
    void process();

    /**
     * Get the daughter boards
     */
    void process_dbs();

    /**
     * Get the DB table index given a module channel number.
     *
     * @param channel The channel to look up
     * @returns int Return -1 if the channel does not have a DB
     */
    int db_find(const int channel) const;
    template<typename T> int db_find(const T channel) const {
        return db_find(static_cast<int>(channel));
    }

    /**
     * Get the DB index for a given DB label. @note the index is not the index
     * into this table of DBs, it is the index identifier for the DB.
     */
    int db_find_index(const std::string label) const;

    /**
     * Get the DB label given a DB index.
     */
    std::string db_find_label(const int index) const;

    /**
     * Get the DB channel base.
     *
     * @param number The DB number
     * @returns int Return -1 if the number of out of range
     */
    int db_channel_base(const int number) const;

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
    size_t find(const tag key, size_t count = 0, bool throw_error = true) const;

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
