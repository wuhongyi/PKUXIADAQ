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
    log(log::info) << "pool create: num=" << number_ << " size=" << size_;
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
        log(log::info) << "pool destroy";
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

queue::queue() : size_(0), count_(0) {}

void queue::push(handle buf) {
    if (buf->size() > 0) {
        lock_guard guard(lock);
        buffers.push_back(buf);
        size_ += buf->size();
        ++count_;
    }
}

handle queue::pop() {
    lock_guard guard(lock);
    handle buf = buffers.front();
    buffers.pop_front();
    size_ -= buf->size();
    --count_;
    return buf;
}

void queue::copy(buffer& to) {
    lock_guard guard(lock);
    /*
     * If the `to` size is 0 copy all the available data
     */
    size_t count = to.size();
    if (count == 0) {
        count = size_.load();
        to.resize(count);
    }
    copy_unprotected(to.data(), count);
}

void queue::copy(buffer_value_ptr to, const size_t count) {
    lock_guard guard(lock);
    copy_unprotected(to, count);
}

void queue::copy_unprotected(buffer_value_ptr to, const size_t count) {
    if (count > size_.load()) {
        throw error(error::code::buffer_pool_not_enough, "not enough data in queue");
    }
    auto to_move = count;
    auto from_bi = buffers.begin();
    while (to_move > 0 && from_bi != buffers.end()) {
        auto from = *from_bi;
        if (to_move >= from->size()) {
            std::memcpy(to, from->data(), from->size() * sizeof(*to));
            to += from->size();
            to_move -= from->size();
            size_ -= from->size();
            count_--;
            from->clear();
        } else {
            std::memcpy(to, from->data(), to_move);
            std::move(from->begin() + to_move, from->end(), from->begin());
            from->resize(from->size() - to_move);
            to += from->size();
            to_move = 0;
            size_ -= to_move;
        }
        from_bi++;
    }
    if (from_bi != buffers.begin()) {
        buffers.erase(buffers.begin(), from_bi);
    }
}

void queue::compact() {
    lock_guard guard(lock);

    /*
     * Erasing elements from the queue's container invalidates the
     * iterators. After moving one or more buffers into another buffer and
     * removing them we start again, so we have valid iterators.
     */
    bool rerun = true;
    while (rerun) {
        rerun = false;
        auto to_bi = buffers.begin();
        while (to_bi != buffers.end()) {
            auto& to = *to_bi;
            if (to->capacity() - to->size() > 0) {
                auto erase_from = buffers.end();
                auto erase_to = buffers.end();
                auto to_move = to->capacity() - to->size();
                auto from_bi = to_bi + 1;
                while (to_move > 0 && from_bi != buffers.end()) {
                    auto from = *from_bi;
                    if (to_move >= from->size()) {
                        to->insert(to->end(), from->begin(), from->end());
                        to_move -= from->size();
                        if (erase_from == buffers.end()) {
                            erase_from = from_bi;
                        }
                        from_bi++;
                        erase_to = from_bi;
                        count_--;
                        from->clear();
                    } else {
                        to->insert(to->end(), from->begin(), from->begin() + to_move);
                        from->erase(from->begin(), from->begin() + to_move);
                        to_move = 0;
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
}

void queue::flush() {
    lock_guard guard(lock);
    buffers.clear();
}

void queue::output(std::ostream& out) {
    out << "count=" << count() << " size=" << size();
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
