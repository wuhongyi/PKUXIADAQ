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

/** @file eeprom.cpp
 * @brief Implements data structures and functions for working with EEPROMs
 */

#include <algorithm>
#include <map>

#include <pixie/eeprom.hpp>
#include <pixie/log.hpp>
#include <pixie/util.hpp>

namespace xia {
namespace pixie {
namespace eeprom {
/*
 * Configuration handles the various format versions.
 */
struct eeprom_format {
    std::pair<int, int> revisions;
    int version;
};

static const std::vector<eeprom_format> formats = {{{0x0a, 0x0f}, 0},
                                                   {{0x0f, 0x0f}, 1},
                                                   {{0x48, 0xff}, 2}};

/*
 * The early Pixie module EEPROMs do not contain the ADC data. Provide a
 * table of configuration data.
 */
struct v1_config {
    std::pair<int, int> serial_num;
    int adc_bits;
    int adc_msps;
    int num_channels;
    int eeprom_format;
    std::vector<int> revisions;
};

static const std::vector<v1_config> v1_configs = {{{0, 255}, 12, 100, 16, 0, {0xA}},
                                                  {{256, 274}, 12, 100, 16, 1, {0xB, 0xC, 0xD}},
                                                  {{1000, 1034}, 12, 250, 16, 1, {0xF}}};

static const std::map<tag, element_decs> descriptors = {
    {tag::model, {tag::model, "model", string, 16, true, false}},
    {tag::serial_num, {tag::serial_num, "serial num", num32, 4, true, false}},
    {tag::revision, {tag::revision, "revision", num8, 1, true, false}},
    {tag::major_revision, {tag::major_revision, "major revision", num8, 1, true, false}},
    {tag::minor_revision, {tag::minor_revision, "minor revision", num8, 1, true, false}},
    {tag::mod_strike, {tag::mod_strike, "mod-strike", num8, 1, true, false}},
    {tag::index, {tag::index, "index", num16, 2, false, false}},
    {tag::size, {tag::size, "size", num16, 2, false, false}},
    {tag::format_ver, {tag::format_ver, "format ver", num8, 1, false, false}},
    {tag::db, {tag::db, "daughter board", num8, 2, false, false}},
    {tag::adc_bits, {tag::adc_bits, "adc bits", num8, 1, false, false}},
    {tag::adc_msps, {tag::adc_msps, "adc msps", num16, 2, false, false}},
    {tag::fpga_clk, {tag::fpga_clk, "fpga clk", num8, 2, false, false}},
    {tag::end, {tag::end, "end", null, 0, true, false}}};

struct v2_config {
    int index;
    int channels;
    hw::config config;
    std::string label;
};

static const std::vector<v2_config> v2_configs = {
    {0, 4, hw::config(hw::small_histogram_length, hw::max_adc_trace_length, hw::max_small_num_baselines, 14, 75, 2, 75 / 2), "DB01"},
    {1, 4, hw::config(hw::small_histogram_length, hw::max_adc_trace_length, hw::max_small_num_baselines, 14, 125, 2, 125 / 2), "DB01"},
    {2, 8, hw::config(hw::small_histogram_length, hw::max_adc_trace_length, hw::max_small_num_baselines, 12, 250, 2, 250 / 2), "DB02"},
    {4, 8, hw::config(hw::small_histogram_length, hw::max_adc_trace_length, hw::max_small_num_baselines, 14, 250, 2, 250 / 2), "DB04"},
    {6, 4, hw::config(hw::small_histogram_length, hw::max_adc_trace_length, hw::max_small_num_baselines, 16, 250, 2, 250 / 2), "DB06"},
    {7, 4, hw::config(hw::small_histogram_length, hw::max_adc_trace_length, hw::max_small_num_baselines, 14, 500, 5, 500 / 5), "DB07"}
};

static const std::map<hw::rev_tag, int> rev_max_channels = {
    {hw::rev_A, 16}, {hw::rev_B, 16}, {hw::rev_C, 16}, {hw::rev_D, 16},
    {hw::rev_E, 16}, {hw::rev_F, 16}, {hw::rev_G, 16}, {hw::rev_H, 32},
};

static const v2_config& find_v2_config(const int id) {
    for (auto& db : v2_configs) {
        if (id == db.index) {
            return db;
        }
    }
    throw error(error::code::device_eeprom_failure,
                "invalid V2 config id: " + std::to_string(id));
}

static const v2_config& find_v2_config(const std::string label) {
    for (auto& db : v2_configs) {
        if (label == db.label) {
            return db;
        }
    }
    throw error(error::code::device_eeprom_failure, "invalid DB label: " + label);
}

header::header() {
    clear();
}

void header::clear() {
    crc = 0;
    control = 0;
}

int header::version() const {
    return int(control & 0xf);
}

db_assemble::db_assemble() : index(-1), position(-1) {
}

eeprom::eeprom() {
    clear();
}

void eeprom::clear() {
    data.clear();
    hdr.clear();
    crc.clear();
    format = -1;
    model.clear();
    serial_num = 0;
    revision = 0;
    major_revision = 0;
    minor_revision = 0;
    mod_strike = 0;
    num_channels = 0;
    max_channels = 0;
    configs.clear();
    dbs.clear();
}

void eeprom::clear_data() {
    data.clear();
    hdr.clear();
    crc.clear();
}

bool eeprom::valid() const {
    return hdr.crc != 0 && hdr.crc == crc.value;
}

void eeprom::process() {
    hdr.crc = get32<uint32_t>(0);
    crc.update(data, sizeof(crc.value));

    /*
     * If no CRC then the format is version 1 if the fields validate. Any error
     * in a field invalidates all the fields.
     */
    if (crc.value != hdr.crc) {
        /*
         *  Starting with serial number 256, serial number is stored in the
         *  first two bytes of EEPROM, followed by revision number, which is at
         *  least 11 (i.e. Rev-B)
         */
        revision = get8<int>(2);

        if (revision >= hw::rev_B) {
            serial_num = get16<int>(0);
        } else {
            revision = get8<int>(1);
            serial_num = get8<int>(0);
        }

        if (serial_num == 0xFFFF) {
            throw error(error::code::device_eeprom_failure, "invalid serial number: blank EEPROM");
        }

        hw::config config;

        num_channels = 0;
        format = 0;

        if (serial_num > 1034) {
            if (revision <= hw::rev_F) {
                num_channels = 16;
            } else {
                num_channels = 16;
            }
            format = 1;
            config.adc_bits = get8<int>(99);
            config.adc_msps = get16<int>(99 + 1);
        } else {
            for (const auto& cfg : v1_configs) {
                if (serial_num >= std::get<0>(cfg.serial_num) &&
                    serial_num <= std::get<1>(cfg.serial_num)) {
                    num_channels = cfg.num_channels;
                    format = cfg.eeprom_format;
                    config.adc_bits = cfg.adc_bits;
                    config.adc_msps = cfg.adc_msps;
                    break;
                }
            }
        }

        if (config.adc_bits == 0) {
            std::ostringstream oss;
            oss << "serial number not supported, not format map: " << serial_num;
            throw error(error::code::device_eeprom_failure, oss.str());
        }

        /*
         * Set the FPGA ADC clock divider and the FPGA clock frequency.
         */
        switch (config.adc_msps) {
            case 100:
                config.adc_clk_div = 1;
                break;
            case 250:
                config.adc_clk_div = 2;
                break;
            case 500:
                config.adc_clk_div = 5;
                break;
            default:
                throw error(error::code::device_eeprom_failure,
                            "invalid ADC MSPS: " + std::to_string(config.adc_msps));
        };

        config.fpga_clk_mhz = config.adc_msps / config.adc_clk_div;

        configs.resize(num_channels, config);

        /*
         * V1 format channel indices to the variables are 1:1 mapped.
         */
        int index = 0;
        for (auto& cfg : configs) {
            cfg.index = index;
            ++index;
        }
    } else {
        hdr.control = get8<uint8_t>(4);
        format = hdr.version();
        serial_num = get_number(tag::serial_num);
        revision = get_number(tag::revision);
        major_revision = get_number(tag::major_revision);
        minor_revision = get_number(tag::minor_revision);
        mod_strike = get_number(tag::mod_strike);

        process_dbs();

        int index = 0;
        for (auto db : dbs) {
            auto db_config = find_v2_config(db.index);
            db_config.config.fixture = hw::get_module_fixture(db.label);
            for (int c = 0; c < db_config.channels; ++c) {
                configs.push_back(db_config.config);
                configs.back().index = index;
                ++index;
            }
        }

        num_channels = int(configs.size());
    }

    auto mc = rev_max_channels.find(hw::rev_tag(revision));
    if (mc == rev_max_channels.end()) {
        throw error(error::code::device_eeprom_failure,
                    "no rev to max channels mapping: " + std::to_string(revision));
    }

    max_channels = mc->second;

    if (num_channels > max_channels) {
        throw error(error::code::device_eeprom_failure,
                    "number of channels exceeds max channels: " + std::to_string(num_channels) +
                        " (max: " + std::to_string(max_channels));
    }

    /*
     * Log the configuration
     */
    xia_log(log::info) << "eeprom: format=" << format << " snum=" << serial_num
                       << " revision=" << revision << " major-revision=" << major_revision
                       << " minor-revision=" << minor_revision
                       << " mod-strike=" << mod_strike << " num-channels=" << num_channels;
    if (num_channels != 0) {
        int found_configs = 0;
        while (true) {
            ++found_configs;
            hw::config config = configs[0];
            if (found_configs > 1) {
                int to_match = found_configs;
                for (int c = 0; c < num_channels; ++c) {
                    if (config != configs[c]) {
                        if (--to_match == 0) {
                            break;
                        }
                        config = configs[c];
                    }
                }
                if (to_match != 0) {
                    break;
                }
            }
            int start = 0;
            bool in = config == configs[0];;
            std::string channels;
            for (int c = 0; c < num_channels; ++c) {
                if (config == configs[c]) {
                    if (!in) {
                        start = c;
                    }
                    in = true;
                } else {
                    if (in) {
                        const int end = config == configs[c] ? c : c - 1;
                        if (!channels.empty()) {
                            channels += ',';
                        }
                        channels += std::to_string(start) + '-' + std::to_string(end);
                    }
                    in = false;
                }
            }
            if (in) {
                const int end = num_channels - 1;
                if (!channels.empty()) {
                    channels += ',';
                }
                channels += std::to_string(start) + '-' + std::to_string(end);
            }
            xia_log(log::info) << "eeprom: channels: " << channels
                               << ": adc-bits=" << config.adc_bits
                               << " adc-msps=" << config.adc_msps
                               << " adc-clk-div=" << config.adc_clk_div
                               << " fpga-clk=" << config.fpga_clk_mhz << "Mhz";
        }
    }
}

void eeprom::process_dbs()
{
    /*
     * Loop looking for all daughter boards starting from `1` up to the
     * number declared in the EEPROM.
     *
     * There is no ordering in the EEPROM as the position field defines the
     * channel ordering not the location of the DB entry in the EEPROM.
     *
     * A `0` offset is returned when all the daughter boards have been
     * found. Once all DBs have been found sort them based on position to get
     * the channel offsets for the module.
     */
    while (true) {
        size_t offset = find(tag::db, dbs.size() + 1, false);
        if (offset == 0) {
            std::sort(dbs.begin(), dbs.end(),
                      [](const db_assemble& a, const db_assemble& b) {
                          return a.position < b.position;
                      });
            break;
        }
        db_assemble db;
        db.index = get8<int>(offset + 0);
        db.position = get8<int>(offset + 1);
        db.label = db_find_label(db.index);
        dbs.push_back(db);
    }
}

int eeprom::db_find(const int channel) const {
    int index = 0;
    for (unsigned int db = 0; db < dbs.size(); ++db) {
        auto db_config = find_v2_config(dbs[db].index);
        if (channel < (index + db_config.channels)) {
            return db;
        }
        index += db_config.channels;
    }
    return -1;
}

int eeprom::db_find_index(const std::string label) const {
    return find_v2_config(label).index;
}

std::string eeprom::db_find_label(const int index) const {
    return find_v2_config(index).label;
}

int eeprom::db_channel_base(const int index) const {
    int base = -1;
    if (index < static_cast<int>(dbs.size())) {
        int db = 0;
        base = 0;
        while (db < index) {
            auto db_config = find_v2_config(dbs[db].index);
            base += db_config.channels;
            ++db;
        }
    }
    return base;
}

std::string eeprom::get_string(const tag key, size_t count) {
    auto& desc = lookup(key);
    if (desc.type != string) {
        throw_bad_tag_type(key);
    }
    size_t offset = find(key, count);
    const char* dp = reinterpret_cast<const char*>(data.data()) + offset;
    return std::string(dp, desc.size);
}

int eeprom::get_number(const tag key, size_t count) {
    auto& desc = lookup(key);
    size_t offset = find(key, count);
    int value = 0;
    switch (desc.type) {
        case num32:
            value = get32<int>(offset);
            break;
        case num16:
            value = get16<int>(offset);
            break;
        case num8:
            value = get8<int>(offset);
            break;
        default:
            throw_bad_tag_type(key);
            break;
    }
    return value;
}

double eeprom::get_float(const tag key, size_t count) {
    auto& desc = lookup(key);
    if (desc.type != string) {
        throw_bad_tag_type(key);
    }
    size_t offset = find(key, count);
    return util::ieee_float(get32<util::ieee_float::value_type>(offset));
}

mac_addr eeprom::get_mac(const tag key, size_t count) {
    auto& desc = lookup(key);
    if (desc.type != string) {
        throw_bad_tag_type(key);
    }
    size_t offset = find(key, count);
    mac_addr mac;
    for (size_t o = 0; o < mac.size(); o++) {
        mac[o] = data[offset + o];
    }
    return mac;
}

ipv4_addr eeprom::get_ipv4(const tag key, size_t count) {
    auto& desc = lookup(key);
    if (desc.type != string) {
        throw_bad_tag_type(key);
    }
    size_t offset = find(key, count);
    ipv4_addr ip;
    for (size_t o = 0; o < ip.size(); o++) {
        ip[o] = data[offset + o];
    }
    return ip;
}

size_t eeprom::find(const tag key, size_t count, bool throw_error) const {
    if (format >= 2) {
        size_t offset = hdr.size;
        size_t found = 0;
        while (offset < data.size()) {
            tag okey = get8<tag>(offset);
            if (okey == tag::end) {
                break;
            }
            auto& desc = lookup(okey);
            ++offset;
            if (desc.key == key) {
                ++found;
                if (found == count) {
                    return offset;
                }
            }
            offset += desc.size;
        }
    }
    if (throw_error) {
        std::ostringstream oss;
        oss << "tag not found: " << std::hex << int(key) << " count: " << count;
        throw error(error::code::device_eeprom_not_found, oss.str());
    }
    return 0;
}

const element_decs& eeprom::lookup(const tag key) const {
    const auto di = descriptors.find(key);
    if (di == descriptors.end()) {
        std::ostringstream oss;
        oss << "invalid EEPROM tag: " << int(key);
        throw error(error::code::device_eeprom_failure, oss.str());
    }
    return std::get<1>(*di);
}

void eeprom::throw_bad_tag_type(const tag key) {
    std::ostringstream oss;
    oss << "invalid tag type: " << int(key);
    throw error(error::code::device_eeprom_bad_type, oss.str());
}

}  // namespace eeprom
}  // namespace pixie
}  // namespace xia
