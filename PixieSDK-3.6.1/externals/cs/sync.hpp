/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2022 Chris Johns, All rights reserved.
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

/** @file sync.hpp
 * @brief Defines sychronization variables
 */

#ifndef CS_SYNC_H
#define CS_SYNC_H

#include <condition_variable>
#include <mutex>

namespace cs {
namespace sync {
/**
 * @brief A synchronisation variable
 */
struct variable {
    using lock_type = std::mutex;
    using lock_guard = std::lock_guard<lock_type>;
    using variable_type = std::condition_variable;
    static constexpr size_t wait_for_ever = 0;
    lock_type& lock;
    variable_type cv;
    variable(lock_type& lock_) : lock(lock_) {}
    /**
     * Wait for the sychronisation notification. Make sure you hold
     * the lock when waiting as the lock is adopted.
     * @retval true The wait has timed out
     * @retval false The waiter has been notified
     */
    bool wait(size_t timeout_usec = wait_for_ever);
    void notify();
};

inline bool variable::wait(size_t timeout_usec) {
    std::unique_lock<lock_type> sync_lock(lock, std::adopt_lock);
    bool timedout = false;
    if (timeout_usec != wait_for_ever) {
        using namespace std::chrono_literals;
        auto period = timeout_usec * 1us;
        timedout = cv.wait_for(sync_lock, period) == std::cv_status::timeout;
    } else {
        cv.wait(sync_lock);
    }
    sync_lock.release();
    return timedout;
}

inline void variable::notify() {
    cv.notify_one();
}

}  // namespace sync
}  // namespace cs

#endif /* CS_SYNC_H */
