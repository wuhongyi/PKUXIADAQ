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
 * @brief Defines functions and data structures for creating threaded data buffers
 */

#ifndef PIXIE_BUFFER_H
#define PIXIE_BUFFER_H

#include <atomic>
#include <cstdint>
#include <deque>
#include <forward_list>
#include <iostream>
#include <memory>
#include <mutex>
#include <vector>

#include <pixie/error.hpp>

#include <pixie/pixie16/hw.hpp>

namespace xia {
/**
 * @brief Handles threaded data buffers to read data from the External FIFO.
 */
namespace buffer {
/*
 * Local error
 */
typedef pixie::error::error error;

/**
 * @brief Defines the Buffer lock type
 */
typedef std::mutex lock_type;
/**
 * @brief A vector of lock_types is a lock_guard.
 */
typedef std::lock_guard<lock_type> lock_guard;

/**
 * @brief A buffer value is still based on the hardware words.
 * A generic container is more work than it is worth at this point in time.
 */
typedef pixie::hw::word buffer_value;
/**
 * @brief Defines a type for pointers to buffer_values
 */
typedef pixie::hw::word_ptr buffer_value_ptr;
/**
 * @brief Defines a type for a vector of buffer words.
*/
typedef pixie::hw::words buffer;
/**
 * @brief Defines a pointer to a vector of buffer words.
*/
typedef buffer* buffer_ptr;
/**
 * @brief Defines a shared pointer of buffer objects to share between threads.
*/
typedef std::shared_ptr<buffer> handle;

/**
 * @brief The buffer pool to manage the buffer workers.
 */
struct pool {
    pool();
    ~pool();

    void create(const size_t number, const size_t size);
    void destroy();

    handle request();

    bool valid() const {
        return number != 0;
    }

    bool empty() const {
        return count_.load() == 0;
    }

    bool full() const {
        return number != 0 && count_.load() == number;
    }

    size_t count() const {
        return count_.load();
    }

    size_t number;
    size_t size;

    void output(std::ostream& out);

private:
    struct releaser;
    void release(buffer_ptr buf);

    std::atomic_size_t count_;

    std::forward_list<buffer_ptr> buffers;

    lock_type lock;
};

/**
 * @brief Buffer queue for allocating work to the workers.
 */
struct queue {
    typedef std::deque<handle> handles;

    queue();

    void push(handle buf);
    handle pop();

    void copy(buffer& to);
    void copy(buffer_value_ptr to, const size_t count);

    void compact();

    bool empty() const {
        return count_.load() == 0;
    }

    size_t size() const {
        return size_.load();
    }
    size_t count() const {
        return count_.load();
    }

    void flush();

    void output(std::ostream& out);

private:
    void copy_unprotected(buffer_value_ptr to, const size_t count);

    handles buffers;
    lock_type lock;
    std::atomic_size_t size_;
    std::atomic_size_t count_;
};

}  // namespace buffer
}  // namespace xia

std::ostream& operator<<(std::ostream& out, xia::buffer::pool& pool);
std::ostream& operator<<(std::ostream& out, xia::buffer::queue& queue);

#endif  // PIXIE_BUFFER_H
