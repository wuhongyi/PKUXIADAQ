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

/** @file buffer.hpp
 * @brief Implements functions and data structures for creating threaded data buffers
 */

#include <cstring>
#include <iomanip>
#include <iostream>

#include <pixie/buffer.hpp>
#include <pixie/error.hpp>
#include <pixie/log.hpp>

namespace xia {
namespace buffer {
static constexpr bool queue_trace = false;

struct pool::releaser {
    pool& pool_;
    releaser(pool& pool_);
    void operator()(buffer* buf) const;
};

pool::releaser::releaser(pool& pool__) : pool_(pool__) {}

void pool::releaser::operator()(buffer* buf) const {
    pool_.release(buf);
}

pool::pool() : number(0), size(0), count_(0) {}

pool::~pool() {
    try {
        destroy();
    } catch (...) {
        /* any error will be logged */
    }
}

void pool::create(const size_t number_, const size_t size_) {
    xia_log(log::info) << "pool create: num=" << number_ << " size=" << size_;
    lock_guard guard(lock);
    if (valid()) {
        throw error(error::code::buffer_pool_not_empty, "pool is already created");
    }
    number = number_;
    size = size_;
    for (size_t n = 0; n < number; ++n) {
        buffer_ptr buf = new buffer;
        buf->reserve(size);
        buffers.push_front(buf);
    }
    count_ = number;
}

void pool::destroy() {
    lock_guard guard(lock);
    if (number > 0) {
        xia_log(log::info) << "pool destroy";
        if (count_.load() != number) {
            throw error(error::code::buffer_pool_busy, "pool destroy made while busy");
        }
        while (!buffers.empty()) {
            buffer_ptr buf = buffers.front();
            delete buf;
            buffers.pop_front();
        }
        number = 0;
        size = 0;
        count_ = 0;
    }
}

handle pool::request() {
    lock_guard guard(lock);
    if (empty()) {
        throw error(error::code::buffer_pool_empty, "no buffers available");
    }
    count_--;
    buffer_ptr buf = buffers.front();
    buffers.pop_front();
    return handle(buf, releaser(*this));
}

void pool::release(buffer_ptr buf) {
    buf->clear();
    lock_guard guard(lock);
    buffers.push_front(buf);
    count_++;
}

void pool::output(std::ostream& out) {
    out << "count=" << count_.load() << " num=" << number << " size=" << size;
}

queue::queue() : size_(0) {}

void queue::push(handle buf) {
    if (buf->size() > 0) {
        lock_guard guard(lock);
        buffers.push_back(buf);
        size_ += buf->size();
        if (queue_trace) {
            xia_log(log::debug) << "queue::push: buffers=" << buffers.size()
                                << " buf=" << buf->size()
                                << " size=" << size_;
            check("push");
        }
    }
}

handle queue::pop() {
    lock_guard guard(lock);
    handle buf = buffers.front();
    buffers.pop_front();
    size_ -= buf->size();
    if (queue_trace) {
        xia_log(log::debug) << "queue::pop: buffers=" << buffers.size()
                            << " buf=" << buf->size()
                            << " size=" << size_;
        check("pop");
    }
    return buf;
}

size_t queue::copy(buffer& to) {
    lock_guard guard(lock);
    /*
     * If the `to` size is 0 copy all the available data
     */
    size_t to_move = to.size();
    if (to_move == 0) {
        to_move = size_;
        to.resize(to_move);
    }
    return copy_unprotected(to.data(), to_move);
}

size_t queue::copy(buffer_value_ptr to, const size_t to_move) {
    lock_guard guard(lock);
    return copy_unprotected(to, to_move);
}

size_t queue::copy_unprotected(buffer_value_ptr to, size_t to_move) {
    if (to_move > size_) {
        throw error(error::code::buffer_pool_not_enough, "not enough data in queue");
    }
    if (queue_trace) {
        xia_log(log::debug) << "queue::copy: buffers=" << buffers.size()
                            << " size_=" << size_
                            << " to-move=" << to_move;
        check("copy start");
    }
    auto copied = to_move;
    auto to_in = to;
    auto from_bi = buffers.begin();
    while (to_move > 0 && from_bi != buffers.end()) {
        auto from = *from_bi;
        ++from_bi;
        if (to_move >= from->size()) {
            if (queue_trace) {
                xia_log(log::debug) << "queue::copy: from-all: to_move=" << to_move
                                    << " from=" << from->size()
                                    << " size_=" << size_;
            }
            std::memcpy(to, from->data(), from->size() * sizeof(*to));
            to += from->size();
            to_move -= from->size();
            size_ -= from->size();
        } else {
            auto remaining = from->size() - to_move;
            if (queue_trace) {
                xia_log(log::debug) << "queue::copy: from-some: to_move=" << to_move
                                    << " from=" << from->size()
                                    << " remaining=" << remaining
                                    << " size_=" << size_;
            }
            std::memcpy(to, from->data(), to_move * sizeof(*to));
            std::memmove(
                from->data(), from->data() + to_move, remaining * sizeof(*to));
            from->resize(remaining);
            to += to_move;
            size_ -= to_move;
            to_move = 0;
            --from_bi;
        }
    }
    copied -= to_move;
    if (from_bi != buffers.begin()) {
        buffers.erase(buffers.begin(), from_bi);
    }
    if (queue_trace) {
        check("copy end");
        xia_log(log::debug) << "queue::copy: finished: buffers=" << buffers.size()
                            << " copied=" << copied
                            << " size_=" << size_;
    }
    return copied;
}

void queue::compact() {
    lock_guard guard(lock);
    if (queue_trace) {
        check("compact start");
    }
    /*
     * Erasing elements from the queue's container invalidates the
     * iterators. After moving one or more buffers into another buffer and
     * removing them we start again, so we have valid iterators.
     */
    bool rerun = true;
    while (rerun) {
        rerun = false;
        if (queue_trace) {
            xia_log(log::debug) << "compact: buffers=" << buffers.size();
        }
        auto to_bi = buffers.begin();
        while (to_bi != buffers.end()) {
            auto& to = *to_bi;
            auto to_move = to->capacity() - to->size();
            auto from_bi = to_bi + 1;
            if (to_move > 0 && from_bi != buffers.end()) {
                auto erase_from = buffers.end();
                auto erase_to = buffers.end();
                while (to_move > 0 && from_bi != buffers.end()) {
                    auto from = *from_bi;
                    if (queue_trace) {
                        xia_log(log::debug) << "compact: move=" << to_move
                                            << " to=" << to->data() << '/' << to->size()
                                            << " from=" << from->data() << '/' << from->size();
                    }
                    if (to_move >= from->size()) {
                        to->insert(to->end(), from->begin(), from->end());
                        to_move -= from->size();
                        if (erase_from == buffers.end()) {
                            erase_from = from_bi;
                        }
                        from_bi++;
                        erase_to = from_bi;
                        from->clear();
                    } else {
                        to->insert(to->end(), from->begin(), from->begin() + to_move);
                        from->erase(from->begin(), from->begin() + to_move);
                        to_move = 0;
                    }
                    if (queue_trace) {
                        xia_log(log::debug) << "compact: moved:    "
                                            << " to=" << to->data() << '/' << to->size()
                                            << " from=" << from->data() << '/' << from->size();
                    }
                }
                if (erase_from != buffers.end()) {
                    buffers.erase(erase_from, erase_to);
                    rerun = true;
                    break;
                }
            }
            to_bi++;
        }
    }
    if (queue_trace) {
        check("compact start");
    }
}

bool queue::empty() {
    lock_guard guard(lock);
    return buffers.empty();
}

size_t queue::size() {
    lock_guard guard(lock);
    return size_;
    }

size_t queue::count() {
    lock_guard guard(lock);
    return buffers.size();
}

void queue::flush() {
    lock_guard guard(lock);
    buffers.clear();
    size_ = 0;
}

void queue::output(std::ostream& out) {
    out << "count=" << count() << " size=" << size();
}

void queue::check(const char* label) {
    size_t csize = 0;
    size_t zero_pairs = 0;
    buffer_value prev = 1;
    for (auto& buf : buffers) {
        csize += buf->size();
        auto* ptr = buf->data();
        for (size_t i = 0; i < buf->size(); ++i) {
            if (ptr[i] == 0 && prev == 0) {
                ++zero_pairs;
            }
            prev = ptr[i];
        }
    }
    xia_log(log::debug) << "queue::check: " << label << ": found=" << csize << " has=" << size_
                        << " buffers=" << buffers.size() << " zero-pairs=" << zero_pairs;
}
}  // namespace buffer
}  // namespace xia

std::ostream& operator<<(std::ostream& out, xia::buffer::pool& pool) {
    pool.output(out);
    return out;
}

std::ostream& operator<<(std::ostream& out, xia::buffer::queue& queue) {
    queue.output(out);
    return out;
}
