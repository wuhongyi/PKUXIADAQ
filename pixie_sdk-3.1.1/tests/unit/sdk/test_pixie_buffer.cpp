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

/** @file test_pixie_buffer.cpp
 * @brief Defines tests for the threaded FIFO buffer readout.
 */

#include <cstring>

#include <doctest/doctest.h>
#include <pixie/buffer.hpp>

TEST_SUITE("xia::buffer") {
    TEST_CASE("pool create/destroy") {
        SUBCASE("no actions") {
            xia::buffer::pool pool;
            CHECK(pool.empty());
            CHECK(!pool.full());
            CHECK(pool.count() == 0);
            CHECK(pool.number == 0);
            CHECK(pool.size == 0);
            pool.create(100, 8 * 1024);
            CHECK(!pool.empty());
            CHECK(pool.full());
            CHECK(pool.count() == 100);
            CHECK(pool.number == 100);
            CHECK(pool.size == 8 * 1024);
            pool.destroy();
            CHECK(pool.empty());
            CHECK(!pool.full());
            CHECK(pool.count() == 0);
            CHECK(pool.number == 0);
            CHECK(pool.size == 0);
        }
        SUBCASE("buffer held") {
            xia::buffer::pool pool;
            pool.create(100, 8 * 1024);
            CHECK(!pool.empty());
            {
                xia::buffer::handle buf = pool.request();
                CHECK_THROWS_WITH_AS(pool.destroy(), "pool destroy made while busy",
                                     xia::buffer::error);
            }
            pool.destroy();
        }
    }
    TEST_CASE("pool create/destroy reuse") {
        xia::buffer::pool pool;
        SUBCASE("one") {
            pool.create(100, 8 * 1024);
            CHECK_THROWS_WITH_AS(pool.create(1, 1), "pool is already created", xia::buffer::error);
            pool.destroy();
        }
        SUBCASE("two") {
            pool.create(200, 4 * 1024);
            pool.destroy();
        }
    }
    TEST_CASE("pool request/release") {
        xia::buffer::pool pool;
        pool.create(100, 8 * 1024);
        SUBCASE("single") {
            xia::buffer::handle buf = pool.request();
            CHECK(buf->size() == 0);
            CHECK((*buf).size() == 0);
            CHECK(buf->capacity() == pool.size);
            CHECK(pool.count() == pool.number - 1);
        }
        SUBCASE("all") {
            std::vector<xia::buffer::handle> handles;
            for (size_t b = 0; b < pool.number; ++b) {
                handles.push_back(pool.request());
            }
            CHECK_THROWS_WITH_AS(pool.request(), "no buffers available", xia::buffer::error);
            CHECK(pool.empty());
            CHECK(!pool.full());
            CHECK(pool.count() == 0);
            handles.pop_back();
            CHECK(!pool.empty());
            CHECK(!pool.full());
            CHECK(pool.count() == 1);
            handles.erase(handles.begin() + 5);
            CHECK(!pool.empty());
            CHECK(!pool.full());
            CHECK(pool.count() == 2);
            handles.clear();
            CHECK(!pool.empty());
            CHECK(pool.full());
            CHECK(pool.count() == pool.number);
        }
        SUBCASE("variable") {
            std::vector<xia::buffer::handle> handles[3];
            for (size_t h = 0; h < 3; ++h) {
                for (size_t b = 0; b < 10; ++b) {
                    handles[h].push_back(pool.request());
                }
            }
            CHECK(pool.count() == pool.number - 3 * 10);
            handles[1].erase(handles[1].begin() + 5);
            handles[1].erase(handles[1].begin() + 6);
            handles[0].pop_back();
            handles[2].erase(handles[2].begin() + 3);
            CHECK(pool.count() == pool.number - 3 * 10 + 4);
            handles[0].push_back(pool.request());
            CHECK(pool.count() == pool.number - 3 * 10 + 3);
            std::vector<xia::buffer::handle> remaining;
            while (!pool.empty()) {
                remaining.push_back(pool.request());
            }
        }
        pool.destroy();
    }
    TEST_CASE("queue") {
        xia::buffer::pool pool;
        pool.create(100, 8 * 1024);
        SUBCASE("create/destroy") {
            xia::buffer::queue queue;
            CHECK(queue.size() == 0);
            CHECK(queue.count() == 0);
        }
        SUBCASE("push and pop") {
            xia::buffer::queue queue;
            CHECK(queue.size() == 0);
            CHECK(queue.count() == 0);
            /*
             * Empty buffers are not queued but released.
             */
            queue.push(pool.request());
            CHECK(queue.size() == 0);
            CHECK(queue.count() == 0);
            CHECK(pool.full());
            {
                /*
                 * hold in the block to check the handle's ref counting.
                 */
                xia::buffer::handle buf = pool.request();
                buf->resize(100);
                queue.push(buf);
                CHECK(queue.size() == 100);
                CHECK(queue.count() == 1);
                CHECK(pool.count() == pool.number - 1);
            }
            CHECK(queue.size() == 100);
            CHECK(queue.count() == 1);
            CHECK(pool.count() == pool.number - 1);
            queue.pop();
            CHECK(queue.size() == 0);
            CHECK(queue.count() == 0);
            CHECK(pool.full());
            /*
             * Hold the buffer in a handle and the queue and then let them
             * destruct. The pool destroy will catch any leaks.
             */
            xia::buffer::handle buf = pool.request();
            buf->resize(100);
            queue.push(buf);
        }
        SUBCASE("size and count") {
            xia::buffer::queue queue;
            size_t total = 0;
            for (size_t b = 0; b < 10; ++b) {
                xia::buffer::handle buf = pool.request();
                CHECK(buf->size() == 0);
                CHECK(buf->capacity() == pool.size);
                size_t size = b * 100 + 10;
                buf->resize(size);
                total += size;
                queue.push(buf);
            }
            CHECK(queue.size() == total);
            CHECK(queue.count() == 10);
            for (size_t b = 0; b < 5; ++b) {
                size_t size = b * 100 + 10;
                total -= size;
                xia::buffer::handle buf = queue.pop();
                CHECK(buf->size() == size);
            }
            CHECK(queue.size() == total);
            CHECK(queue.count() == 5);
        }
        SUBCASE("compact to one") {
            xia::buffer::queue queue;
            size_t total = 0;
            for (size_t b = 0; b < 10; ++b) {
                xia::buffer::handle buf = pool.request();
                CHECK(buf->size() == 0);
                CHECK(buf->capacity() == pool.size);
                size_t size = b * 100 + 10;
                buf->resize(size);
                total += size;
                queue.push(buf);
            }
            CHECK(queue.count() == 10);
            CHECK(queue.size() == total);
            queue.compact();
            CHECK(queue.count() == 1);
            CHECK(queue.size() == total);
        }
        SUBCASE("compact various sizes") {
            xia::buffer::queue queue;
            size_t total = 0;
            for (size_t b = 0; b < 5; ++b) {
                xia::buffer::handle buf = pool.request();
                CHECK(buf->size() == 0);
                CHECK(buf->capacity() == pool.size);
                size_t size = b * 100 + 10;
                buf->resize(size);
                total += size;
                queue.push(buf);
            }
            for (size_t b = 0; b < 15; ++b) {
                xia::buffer::handle buf = pool.request();
                CHECK(buf->size() == 0);
                CHECK(buf->capacity() == pool.size);
                size_t size = buf->capacity() / 4 + b + 1;
                buf->resize(size);
                total += size;
                queue.push(buf);
            }
            for (size_t b = 0; b < 5; ++b) {
                xia::buffer::handle buf = pool.request();
                size_t size = buf->capacity() - b * 100;
                buf->resize(size);
                total += size;
                queue.push(buf);
            }
            CHECK(queue.count() == 25);
            CHECK(queue.size() == total);
            queue.compact();
            CHECK(queue.count() == 9);
            CHECK(queue.size() == total);
        }
        SUBCASE("copy") {
            xia::buffer::queue queue;
            size_t total = 0;
            const xia::buffer::buffer_value inc_by = 0x11111111;
            xia::buffer::buffer_value value = 0x11111111;
            for (size_t b = 0; b < 5; ++b) {
                xia::buffer::handle buf = pool.request();
                CHECK(buf->size() == 0);
                CHECK(buf->capacity() == pool.size);
                size_t size = 100;
                buf->resize(size, value);
                value += inc_by;
                total += size;
                queue.push(buf);
            }
            for (size_t b = 0; b < 5; ++b) {
                xia::buffer::handle buf = pool.request();
                size_t size = 200;
                buf->resize(size, value);
                value += inc_by;
                total += size;
                queue.push(buf);
            }
            for (size_t b = 0; b < 5; ++b) {
                xia::buffer::handle buf = pool.request();
                CHECK(buf->size() == 0);
                CHECK(buf->capacity() == pool.size);
                size_t size = 300;
                buf->resize(size, value);
                value += inc_by;
                total += size;
                queue.push(buf);
            }
            CHECK(queue.count() == 15);
            CHECK(queue.size() == total);
            xia::buffer::buffer buffer;
            queue.copy(buffer);
            CHECK(buffer.size() == total);
            CHECK(queue.count() == 0);
            CHECK(queue.size() == 0);
            value = 0x11111111;
            size_t v = 0;
            bool matched = true;
            for (size_t b = 0; b < 5; ++b) {
                for (size_t c = 0; c < 100; ++c, ++v) {
                    if (buffer[v] != value) {
                        matched = false;
                    }
                }
                value += inc_by;
            }
            CHECK(matched);
            matched = true;
            for (size_t b = 0; b < 5; ++b) {
                for (size_t c = 0; c < 200; ++c, ++v) {
                    if (buffer[v] != value) {
                        matched = false;
                    }
                }
                value += inc_by;
            }
            CHECK(matched);
            matched = true;
            for (size_t b = 0; b < 5; ++b) {
                for (size_t c = 0; c < 300; ++c, ++v) {
                    if (buffer[v] != value) {
                        matched = false;
                    }
                }
                value += inc_by;
            }
            CHECK(matched);
            total = 0;
            for (size_t b = 0; b < 10; ++b) {
                xia::buffer::handle buf = pool.request();
                CHECK(buf->size() == 0);
                CHECK(buf->capacity() == pool.size);
                size_t size = pool.size;
                buf->resize(size, queue.count() + 1);
                total += size;
                queue.push(buf);
            }
            for (size_t b = 0; b < 10; ++b) {
                xia::buffer::handle buf = pool.request();
                CHECK(buf->size() == 0);
                CHECK(buf->capacity() == pool.size);
                size_t size = pool.size / 4;
                buf->resize(size, queue.count() + 1);
                total += size;
                queue.push(buf);
            }
            buffer.clear();
            xia::buffer::buffer_value_ptr bufp = new xia::buffer::buffer_value[total];
            CHECK(bufp != nullptr);
            queue.compact();
            CHECK_THROWS_WITH_AS(queue.copy(bufp, total + 1), "not enough data in queue",
                                 xia::buffer::error);
            queue.copy(bufp, total);
            v = 0;
            value = 1;
            matched = true;
            for (size_t b = 0; b < 10; ++b) {
                for (size_t c = 0; c < pool.size; ++c, ++v) {
                    if (bufp[v] != value) {
                        matched = false;
                    }
                }
                ++value;
            }
            CHECK(matched);
            matched = true;
            for (size_t b = 0; b < 10; ++b) {
                for (size_t c = 0; c < pool.size / 4; ++c, ++v) {
                    if (bufp[v] != value) {
                        matched = false;
                    }
                }
                ++value;
            }
            CHECK(matched);
            delete[] bufp;
        }
        pool.destroy();
    }
}
