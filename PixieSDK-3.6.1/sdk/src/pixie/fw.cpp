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

/** @file fw.cpp
 * @brief Implements data structures and functions to handle firmware.
 */

#include <algorithm>
#include <cctype>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <utility>

#include <pixie/error.hpp>
#include <pixie/fw.hpp>
#include <pixie/log.hpp>
#include <pixie/util.hpp>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#if defined(_WIN64) || defined(_WIN32)
#include <io.h>
#else
#include <unistd.h>
#endif

namespace xia {
namespace pixie {
namespace firmware {
/*
 * Param errors
 */
typedef pixie::error::error error;

/*
 * Amount of memory being used.
 */
static std::atomic_size_t total_image_size;

reader::reader(const image& img_, const size_t default_word_size_)
    : img(img_), default_word_size(default_word_size_), offset(0) {}

image_value_type reader::get(size_t word_size) {
    return read(word_size, true);
}

image_value_type reader::peek(size_t word_size) {
    return read(word_size, false);
}

image_value_type reader::read(size_t word_size, bool inc) {
    if (word_size > sizeof(image_value_type)) {
        std::ostringstream oss;
        oss << "firmware: word-size out of range: word-size=" << word_size;
        throw error(error::code::device_image_failure, oss.str());
    }

    if (word_size == 0) {
        word_size = default_word_size;
    }

    if (offset > (img.size() - word_size)) {
        std::ostringstream oss;
        oss << "firmware: image get out of range: offset=" << offset << " word-size=" << word_size
            << " size=" << img.size();
        throw error(error::code::device_image_failure, oss.str());
    }

    image_value_type value = 0;

    for (size_t w = 0; w < word_size; ++w) {
        value = (value << 8) | img[offset + w];
    }

    if (inc) {
        offset += word_size;
    }

    return value;
}

firmware::firmware(const std::string version_, const int mod_revision_, const int mod_adc_msps_,
                   const int mod_adc_bits_, const std::string device_)
    : tag(pixie::firmware::tag(mod_revision_, mod_adc_msps_, mod_adc_bits_)), version(version_),
      mod_revision(mod_revision_), mod_adc_msps(mod_adc_msps_), mod_adc_bits(mod_adc_bits_),
      device(device_) {}

firmware::firmware(const firmware& orig) :
    tag(orig.tag), filename(orig.filename), version(orig.version), mod_revision(orig.mod_revision),
    mod_adc_msps(orig.mod_adc_msps), mod_adc_bits(orig.mod_adc_bits), device(orig.device),
    slot(orig.slot) {
}

firmware::firmware(firmware&& from)
    : tag(from.tag), filename(from.filename), version(from.version), mod_revision(from.mod_revision),
      mod_adc_msps(from.mod_adc_msps), mod_adc_bits(from.mod_adc_bits), device(from.device),
      slot(from.slot) {
    lock_guard guard(from.lock);
    from.filename.clear();
    from.data.clear();
}

std::string firmware::basename() const {
    auto pos = filename.find_last_of('/');
    if (pos == std::string::npos) {
        pos = filename.find_last_of('\\');
    }
    if (pos == std::string::npos) {
        pos = 0;
    } else {
        ++pos;
    }
    return filename.substr(pos);
}

void firmware::load() {
    lock_guard guard(lock);
    if (data.empty()) {
        util::timepoint load_time(true);
        /*
         * Use C and the standard file system interfaces. They are faster
         * than the C++ stream interface
         */
        int fd = -1;
        try {
            fd = ::open(filename.c_str(), O_RDONLY);
            if (fd < 0) {
                throw error(error::code::file_not_found,
                            "firmware: image open: " + tag + ": " + std::strerror(errno));
            }
            struct stat sb;
            int r = ::fstat(fd, &sb);
            if (r < 0) {
                throw error(error::code::file_not_found,
                            "firmware: image stat: " + tag + ": " + std::strerror(errno));
            }
            size_t size = size_t(sb.st_size);
            data.resize(size);
            r = ::read(fd, data.data(), static_cast<unsigned int>(size));
            if (r < 0) {
                throw error(error::code::file_not_found,
                            "firmware: image read: " + tag + ": " + std::strerror(errno));
            }
            ::close(fd);
            total_image_size += size;
        } catch (...) {
            if (fd >= 0) {
                ::close(fd);
            }
            throw;
        }
        xia_log(log::debug) << "firmware: load: tag=" << tag << " time=" << load_time
                            << " total=" << total_image_size.load();
    }
}

void firmware::clear() {
    lock_guard guard(lock);
    if (!data.empty()) {
        total_image_size -= data.size();
        data.clear();
        data.shrink_to_fit();
    }
}

size_t firmware::words() {
    lock_guard guard(lock);
    /*
     * Size of the vector should be uint32_t aligned so rounding there
     * should be redundant, but it does not harm so lets keep it.
     */
    return ((size_t(data.size()) - 1) / sizeof(image_value_type)) + 1;
}

bool firmware::operator==(const firmware& fw) const {
    return fw.version == version && fw.mod_revision == mod_revision &&
           fw.mod_adc_msps == mod_adc_msps && fw.mod_adc_bits == mod_adc_bits &&
           fw.device == device;
}

template<typename T>
void firmware::output(T& out) const {
    out << tag << ": ver:" << version << " mod-rev:" << mod_revision
        << " mod-adc-msps:" << mod_adc_msps << " mod-adc-bits:" << mod_adc_bits << " dev:" << device
        << " slots:";
    if (slot.size() > 0) {
        out << slot.size() << ':';
        const char* delimiter = "";
        for (auto s : slot) {
            out << delimiter << s;
            delimiter = ",";
        }
    } else {
        out << "default";
    }
    out << " size:" << data.size();
}

std::string tag(const int revision, const int adc_msps, const int adc_bits) {
    return std::to_string(revision) + '-' + std::to_string(adc_msps) + '-' +
           std::to_string(adc_bits);
}

void add(crate& firmwares, firmware& fw) {
    auto mi = firmwares.find(fw.tag);
    if (mi == std::end(firmwares)) {
        firmwares[fw.tag] = module();
    }
    auto& module_fw = firmwares[fw.tag];
    for (auto mfw : module_fw) {
        if (mfw->filename == fw.filename) {
            if (fw.slot.size() != 0) {
                mfw->slot.insert(mfw->slot.end(), fw.slot.begin(), fw.slot.end());
                auto last = std::unique(mfw->slot.begin(), mfw->slot.end());
                mfw->slot.erase(last, mfw->slot.end());
                return;
            }
        }
    }
    module_fw.push_back(std::make_shared<firmware>(fw));
}

bool check(const crate& firmwares, const firmware& fw) {
    auto mod_fw = firmwares.find(fw.tag);
    if (mod_fw != firmwares.end()) {
        for (auto fwr : std::get<1>(*mod_fw)) {
            if (*fwr == fw) {
                return true;
            }
        }
    }
    return false;
}

firmware_ref find(module& firmwares, const std::string device, const int slot) {
    /*
     * First check if a slot assigned firmware exists for this
     * device. If not, then see if a default is available.
     */
    for (auto& fwr : firmwares) {
        if (fwr->device == device && !fwr->slot.empty()) {
            auto found = std::find(fwr->slot.begin(), fwr->slot.end(), slot);
            if (found != fwr->slot.end()) {
                return fwr;
            }
        }
    }
    for (auto& fwr : firmwares) {
        if (fwr->slot.empty() && fwr->device == device && fwr) {
            return fwr;
        }
    }
    throw error(error::code::device_image_failure, "firmware: device not found: device=" + device);
}

void load(crate& crate_fw) {
    for (auto& mod_fw : crate_fw) {
        load(mod_fw.second);
    }
}

void clear(crate& crate_fw) {
    for (auto& mod_fw : crate_fw) {
        clear(mod_fw.second);
    }
}

void load(module& mod_fw) {
    for (auto fwr : mod_fw) {
        fwr->load();
    }
}

void clear(module& mod_fw) {
    for (auto fwr : mod_fw) {
        fwr->clear();
    }
}

firmware parse(const std::string fw_desc, const char delimiter) {
    std::string version;
    int mod_revision = 0;
    int mod_adc_msps = 0;
    int mod_adc_bits = 0;
    std::string device;
    std::string filename;

    util::strings fields;
    util::split(fields, fw_desc, delimiter);

    for (auto field : fields) {
        util::strings label_value;
        util::split(label_value, field, '=');
        if (label_value.size() != 2) {
            throw error(error::code::invalid_value, "invalid firmware field: " + field);
        }
        if (label_value[0] == "version") {
            version = label_value[1];
        } else if (label_value[0] == "revision") {
            try {
                mod_revision = std::stoi(label_value[1]);
            } catch (std::invalid_argument&) {
                throw error(error::code::device_image_failure,
                            "firmware: module revision not a number: " + field);
            }
        } else if (label_value[0] == "adc-msps") {
            try {
                mod_adc_msps = std::stoi(label_value[1]);
            } catch (std::invalid_argument&) {
                throw error(error::code::device_image_failure,
                            "firmware: module ADC MSPS not a number: " + field);
            }
        } else if (label_value[0] == "adc-bits") {
            try {
                mod_adc_bits = std::stoi(label_value[1]);
            } catch (std::invalid_argument&) {
                throw error(error::code::device_image_failure,
                            "firmware: module ADC BITS not a number: " + field);
            }
        } else if (label_value[0] == "device") {
            device = label_value[1];
        } else if (label_value[0] == "file") {
            filename = label_value[1];
        }
    }

    if (version.empty()) {
        throw error(error::code::device_image_failure, "firmware version mandatory: " + fw_desc);
    }
    if (mod_revision == 0) {
        throw error(error::code::device_image_failure, "firmware revision mandatory: " + fw_desc);
    }
    if (mod_adc_msps == 0) {
        throw error(error::code::device_image_failure, "firmware ADC MSPS mandatory: " + fw_desc);
    }
    if (mod_adc_bits == 0) {
        throw error(error::code::device_image_failure, "firmware ADC_bits mandatory: " + fw_desc);
    }
    if (device.empty()) {
        throw error(error::code::device_image_failure, "firmware device mandatory: " + fw_desc);
    }
    if (filename.empty()) {
        throw error(error::code::device_image_failure, "firmware file name mandatory: " + fw_desc);
    }

    firmware fw(version, mod_revision, mod_adc_msps, mod_adc_bits, device);
    fw.filename = filename;

    return fw;
}
};  // namespace firmware
};  // namespace pixie
};  // namespace xia

std::ostringstream& operator<<(std::ostringstream& out, const xia::pixie::firmware::firmware& fw) {
    fw.output(out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const xia::pixie::firmware::firmware& fw) {
    fw.output(out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const xia::pixie::firmware::module& mod_fw) {
    for (auto& mod : mod_fw) {
        out << *mod;
    }
    return out;
}
