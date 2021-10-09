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

/** @file util.hpp
 * @brief Defines utility functions and data structures useful throughout the SDK.
 */

#ifndef PIXIE16_UTIL_H
#define PIXIE16_UTIL_H

#include <atomic>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace xia {
/**
 * @brief Utilities to accomplish various needs in the SDK.
 */
namespace util {
/**
 * @brief Defines a type for a vector of strings.
 */
typedef std::vector<std::string> strings;

/**
 * @brief Split a string into a vector of strings.
 * @param[out] split_string The split string
 * @param[in] s String to split
 * @param[in] delimiter delimiter to split on. Default: ' '.
 * @param[in] count Number of split to make before returning. Default: 0.
 * @param[in] strip_whitespace Trim white space from the split strings. Default: True
 * @param[in] strip_quotes Remove quotes from string fragments. Default: True
 * @param[in] empty Include empty string fragments. Default: False.
 */
void split(strings& split_string, const std::string& s, const char delimiter = ' ',
           size_t count = 0, bool strip_whitespace = true, bool strip_quotes = true,
           bool empty = false);

/**
 * @brief Join the strings into a single string.
 * @tparam T Templated parameter for the stream type
 * @param ss stream containing the data to join.
 * @param c delimiter to join with
 * @return The joined string
 */
template<typename T>
std::string join(T& ss, const char c = ' ') {
    std::string ns;
    for (auto& s : ss) {
        if (!ss.empty()) {
            ns += c;
        }
        ns += s;
    }
    return ns;
}

/**
 * @brief Remove spaces from the front of the string.
 * @param[in,out] s the string that we'll trim.
 */
void ltrim(std::string& s);

/**
 * @brief Remove spaces from the end of the string
 * @param[in,out] s the string that we'll trim.
 */
void rtrim(std::string& s);

/**
 * @brief Trim spaces from both ends of a string
 * @param[in,out] s the string that we'll trim.
 */
void trim(std::string& s);

/**
 * @brief Dequote a string
 * @param[in,out] s the string that we'll trim.
 */
void dequote(std::string& s);

/**
 * @brief Humanize a number into standard size names. Ex. gigabyte (GB).
 * @tparam T A type that can be streamed into a ostringstream
 * @param[in] value The number that we'll humanize.
 * @param[in] suffix Any additional information to go after the unit.
 * @returns The string as a human-readable number
 */
template<typename T>
std::string humanize(T value, const std::string suffix = "") {
    const char* units = " KMGTPEZY";
    const char* unit = units;
    double num = static_cast<double>(value);
    while (*unit != '\0') {
        if (std::abs(num) < 1024.0) {
            break;
        }
        ++unit;
        num /= 1024.0;
    }
    if (*unit == '\0') {
        --unit;
    }
    std::ostringstream oss;
    oss << std::setprecision(3) << std::fixed << num << *unit << suffix;
    return oss.str();
}

/**
 * @brief Save and restore the output stream's settings.
 */
struct ostream_guard {
    std::ostream& o;
    std::ios_base::fmtflags flags;
    ostream_guard(std::ostream& o_) : o(o_), flags(o_.flags()) {}
    ~ostream_guard() {
        o.flags(flags);
    }
};

/**
 * @brief Timepoint measures a period of time between two points.
 */
struct timepoint {
    using marker = std::chrono::time_point<std::chrono::steady_clock>;

    std::atomic_bool active;
    std::atomic_bool suspended;
    std::atomic_bool captured;

    marker start_mark;
    marker end_mark;

    timepoint(bool autostart = false);
    ~timepoint();

    bool running() const {
        return active.load() && !suspended.load();
    }

    void reset();
    void start();
    void end();
    void stop() {
        end();
    }
    void restart();
    void pause();
    void resume();

    uint64_t secs();
    uint64_t msecs();
    uint64_t usecs();

    operator std::string();
    std::string output();

private:
    void lock();
    void unlock();

    std::atomic_bool locked;
};

/**
 * @brief Defines a type for the IEEE 754 floating point standard.
 */
struct ieee_float {
    using value_type = unsigned int;

    ieee_float();
    ieee_float(const ieee_float& ieee);
    ieee_float(value_type value_);
    ieee_float(double dec_num);

    ieee_float& operator=(const ieee_float& ieee);
    ieee_float& operator=(const double dec_num);

    bool operator==(const ieee_float& ieee) const;
    bool operator==(const double dec_num) const;

    operator double() const;
    operator value_type() const;

private:
    value_type in(const double dec_num) const;
    double out() const;

    value_type value;
};

/**
 * @brief Defines a function for a CRC32 checksum
 *
 * The polynomial is
 * `X^32+X^26+X^23+X^22+X^16+X^12+X^11+X^10+X^8+X^7+X^5+X^4+X^2+X^1+X^0`
 *
 * @see https://en.wikipedia.org/wiki/Cyclic_redundancy_check
 */
struct crc32 {
    using value_type = uint32_t;

    value_type value;

    template<typename T>
    void update(const T val) {
        update(static_cast<const unsigned char*>(&val), sizeof(T));
    }

    template<typename T>
    void update(const std::vector<T>& vals, const size_t start = 0) {
        const size_t so = ((vals.size() - start) * sizeof(typename std::vector<T>::value_type));
        update(static_cast<const unsigned char*>(&vals[start]), int(so));
    }

    template<typename T>
    crc32& operator<<(const T& val) {
        update(val);
        return *this;
    }

    crc32& operator<<(const std::string& val);

    operator const std::string() const;

    crc32();

    void clear();

private:
    void update(const unsigned char* data, int len);
    /**
     * A precomputed lookup table for the CRC32 bitwise algorithm
     */
    static const value_type table[256];
};

}  // namespace util
}  // namespace xia

/**
 * @brief Output stream operator for timepoint objects.
 */
std::ostream& operator<<(std::ostream& out, xia::util::timepoint& timepoint);

#endif  // PIXIE16_UTIL_H
