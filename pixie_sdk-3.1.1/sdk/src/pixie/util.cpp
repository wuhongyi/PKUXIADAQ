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

/** @file util.cpp
 * @brief Implements utility functions and data structures useful throughout the SDK.
 */

#include <algorithm>
#include <bitset>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>

#include <pixie/util.hpp>

namespace xia {
namespace util {
void dequote(std::string& s) {
    if (!s.empty()) {
        char front = s[0];
        char back = s[s.length() - 1];
        if ((front == '"') || (front == '\'')) {
            if (front != back)
                throw std::runtime_error("invalid quoting: " + s);
            s = s.substr(1, s.length() - (1 + 1));
        }
    }
}

void split(strings& split_string, const std::string& s, const char delimiter, size_t count,
           bool strip_whitespace, bool strip_quotes, bool empty) {
    std::stringstream ss(s);
    std::string e;
    split_string.clear();
    while (std::getline(ss, e, delimiter)) {
        if (strip_whitespace)
            trim(e);
        if (strip_quotes)
            dequote(e);
        if (empty || !e.empty()) {
            split_string.push_back(e);
            if (count != 0 && split_string.size() >= count) {
                break;
            }
        }
    }
}

void ltrim(std::string& s) {
    s.erase(s.begin(),
            std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
}

void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); })
                .base(),
            s.end());
}

void trim(std::string& s) {
    ltrim(s);
    rtrim(s);
}

timepoint::timepoint(bool autostart)
    : active(false), suspended(false), captured(false), locked(false) {
    if (autostart) {
        start();
    }
}

timepoint::~timepoint() {
    lock();
}

void timepoint::reset() {
    lock();
    active = false;
    suspended = false;
    captured = false;
    unlock();
}

void timepoint::start() {
    lock();
    if (!active.load() && !captured.load()) {
        active = true;
        start_mark = std::chrono::steady_clock::now();
    }
    unlock();
}

void timepoint::end() {
    lock();
    if (!active.load() && !captured.load()) {
        active = false;
        suspended = false;
        captured = true;
        end_mark = std::chrono::steady_clock::now();
    }
    unlock();
}

void timepoint::pause() {
    if (active.load()) {
        suspended = true;
    }
}

void timepoint::resume() {
    suspended = false;
}

void timepoint::restart() {
    lock();
    active = true;
    suspended = false;
    captured = false;
    start_mark = std::chrono::steady_clock::now();
    unlock();
}

uint64_t timepoint::secs() {
    return msecs() / 1000;
}

uint64_t timepoint::msecs() {
    lock();
    if (active.load() && !suspended.load() && !captured.load()) {
        end_mark = std::chrono::steady_clock::now();
    }
    uint64_t period = 0;
    if (active.load() || captured.load()) {
        using namespace std::literals;
        period = (end_mark - start_mark) / 1ms;
    }
    unlock();
    return period;
}

uint64_t timepoint::usecs() {
    lock();
    if (active && !suspended.load() && !captured.load()) {
        end_mark = std::chrono::steady_clock::now();
    }
    uint64_t period = 0;
    if (active.load() || captured.load()) {
        using namespace std::literals;
        period = (end_mark - start_mark) / 1us;
    }
    unlock();
    return period;
}


timepoint::operator std::string() {
    return output();
}

std::string timepoint::output() {
    lock();
    if (active.load() && !captured.load()) {
        end_mark = std::chrono::steady_clock::now();
    }
    if (!active.load() && !captured.load()) {
        unlock();
        return std::string("n/a");
    }
    const auto duration = end_mark - start_mark;
    unlock();
    using namespace std::literals;
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << duration / 1h << ':' << std::setw(2)
        << (duration / 1min) % 60 << ':' << std::setw(2) << (duration / 1s) % 60 << '.'
        << std::setw(6) << (duration / 1us) % 1000000;
    return oss.str();
}

void timepoint::lock() {
    while (true) {
        bool is_locked = locked.exchange(true);
        if (!is_locked) {
            return;
        }
    }
}

void timepoint::unlock() {
    locked = false;
}

ieee_float::ieee_float() : value(0) {}

ieee_float::ieee_float(const ieee_float& ieee) : value(ieee.value) {}

ieee_float::ieee_float(value_type value_) : value(value_) {}

ieee_float::ieee_float(const double dec_num) : value(in(dec_num)) {}

ieee_float& ieee_float::operator=(const ieee_float& ieee) {
    value = ieee.value;
    return *this;
}

ieee_float& ieee_float::operator=(const double dec_num) {
    value = in(dec_num);
    return *this;
}

bool ieee_float::operator==(const ieee_float& ieee) const {
    return value == ieee.value;
}

bool ieee_float::operator==(const double dec_num) const {
    return value == in(dec_num);
}

ieee_float::operator double() const {
    return out();
}

ieee_float::operator value_type() const {
    return value;
}

ieee_float::value_type ieee_float::in(const double dec_num) const {
    if (dec_num == 0) {
        return 0;
    }

    /*
     * Get the absolute value then convert the integer part.
     */
    double abs_dec_num = std::fabs(dec_num);
    value_type int_part = static_cast<value_type>(std::floor(abs_dec_num));
    value_type int_part_hex = 0;
    size_t k;
    for (k = 0; k < 32 && int_part > 0; ++k, int_part >>= 1) {
        if ((int_part & 1) == 1) {
            int_part_hex |= 1 << k;
        } else {
            int_part_hex &= ~(1 << k);
        }
    }

    /*
     * Get the fractional part then convert it.
     */
    double frac_part = abs_dec_num - std::floor(abs_dec_num);
    value_type frac_part_hex = 0;
    size_t m;
    for (m = 0; m < 32 && frac_part > 0; ++m) {
        if ((frac_part * 2.0) >= 1.0) {
            frac_part_hex |= 1 << (31 - m);
            frac_part *= 2.0;
            frac_part -= 1.0;
        } else {
            frac_part_hex &= ~(1 << (31 - m));
            frac_part *= 2.0;
        }
    }

    value_type result;

    if (dec_num > 0) {
        result = 0;
    } else {
        result = 1 << 31;
    }

    /*
     * Combine the integer and fractional part. First, find the exponent
     */
    if (int_part_hex > 0) {
        /*
         * 127 is the exponent bias
         */
        value_type exponent = value_type(127 + k - 1);

        result += exponent * (1 << 23);

        /*
         * Add the mantissa
         */
        int_part_hex &= ~(1 << (k - 1));
        result += int_part_hex * (1 << (22 - (k - 2)));
        result += frac_part_hex >> (32 - (23 - (k - 1)));
    } else {
        /*
         * Search the fist non-zero bit in fract_part_hex
         */
        int n;
        for (n = 31; n > 0; --n) {
            if ((frac_part_hex & (1 << n)) != 0) {
                break;
            }
        }

        /*
         * 127 is the exponent bias
         */
        value_type exponent = 127 - (32 - n);

        result += exponent * (1 << 23);

        /*
         * Add the mantissa
         */
        frac_part_hex &= ~(1 << n);
        if (n >= 23) {
            result += frac_part_hex >> (n - 23);
        } else {
            result += frac_part_hex << (32 - n);
        }
    }

    return result;
}

double ieee_float::out() const {
    auto signbit = static_cast<short>(value >> 31);
    auto exponent = static_cast<short>(((value & 0x7F800000) >> 23) - 127);
    double mantissa = 1.0 + ((double(value & 0x7FFFFF) / double(1 << 23)));
    double result = mantissa * std::pow(2.0, static_cast<double>(exponent));
    if (signbit != 0)
        result *= -1.0;
    return result;
}

crc32::crc32() {
    clear();
}

void crc32::clear() {
    value = 0;
}

crc32& crc32::operator<<(const std::string& val) {
    update(reinterpret_cast<const unsigned char*>(val.c_str()), int(val.length()));
    return *this;
}

crc32::operator const std::string() const {
    std::ostringstream oss;
    oss << std::hex << value;
    return oss.str();
}

void crc32::update(const unsigned char* data, int len) {
    value = ~value;
    while (len-- != 0) {
        value = table[(value ^ *data++) & 0xff] ^ (value >> 8);
    }
    value = ~value;
}

const crc32::value_type crc32::table[256] = {
    0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L, 0x706af48fL, 0xe963a535L,
    0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L, 0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL,
    0xe7b82d07L, 0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL, 0x1adad47dL,
    0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L, 0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL,
    0x14015c4fL, 0x63066cd9L, 0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
    0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL, 0x35b5a8faL, 0x42b2986cL,
    0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L, 0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL,
    0x51de003aL, 0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L, 0xb8bda50fL,
    0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L, 0x2f6f7c87L, 0x58684c11L, 0xc1611dabL,
    0xb6662d3dL, 0x76dc4190L, 0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
    0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL, 0xe10e9818L, 0x7f6a0dbbL,
    0x086d3d2dL, 0x91646c97L, 0xe6635c01L, 0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL,
    0x6c0695edL, 0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L, 0x8bbeb8eaL,
    0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L, 0xfbd44c65L, 0x4db26158L, 0x3ab551ceL,
    0xa3bc0074L, 0xd4bb30e2L, 0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
    0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L, 0xaa0a4c5fL, 0xdd0d7cc9L,
    0x5005713cL, 0x270241aaL, 0xbe0b1010L, 0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L,
    0xce61e49fL, 0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L, 0x2eb40d81L,
    0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L, 0x03b6e20cL, 0x74b1d29aL, 0xead54739L,
    0x9dd277afL, 0x04db2615L, 0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
    0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L, 0x8708a3d2L, 0x1e01f268L,
    0x6906c2feL, 0xf762575dL, 0x806567cbL, 0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L,
    0x10da7a5aL, 0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L, 0xd6d6a3e8L,
    0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L, 0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL,
    0xd80d2bdaL, 0xaf0a1b4cL, 0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
    0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L, 0xcc0c7795L, 0xbb0b4703L,
    0x220216b9L, 0x5505262fL, 0xc5ba3bbeL, 0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L,
    0xb5d0cf31L, 0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL, 0x026d930aL,
    0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L, 0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL,
    0x0cb61b38L, 0x92d28e9bL, 0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
    0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L, 0x18b74777L, 0x88085ae6L,
    0xff0f6a70L, 0x66063bcaL, 0x11010b5cL, 0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L,
    0xa00ae278L, 0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L, 0x4969474dL,
    0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L, 0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L,
    0x47b2cf7fL, 0x30b5ffe9L, 0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
    0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L, 0x5d681b02L, 0x2a6f2b94L,
    0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL, 0x2d02ef8dUL};

}  // namespace util
}  // namespace xia

std::ostream& operator<<(std::ostream& out, xia::util::timepoint& timepoint) {
    out << timepoint.output();
    return out;
}
