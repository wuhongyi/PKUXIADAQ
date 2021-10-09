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

/** @file crate.cpp
 * @brief Implements functions and data structures related to handling a Pixie-16 crate.
 */

#include <algorithm>
#include <future>
#include <iomanip>
#include <sstream>

#include <pixie/config.hpp>
#include <pixie/log.hpp>

#include <pixie/pixie16/crate.hpp>

namespace xia {
namespace pixie {
namespace crate {
crate::guard::guard(crate& crate_) : lock_(crate_.lock_), guard_(lock_) {}

crate::user::user(crate& crate__) : crate_(crate__) {
    ++crate_.users_;
}

crate::user::~user() {
    --crate_.users_;
}

crate::crate() : num_modules(0), revision(-1), ready_(false) {}

crate::~crate() {}

void crate::ready() {
    if (!ready_.load()) {
        throw error(pixie::error::code::crate_not_ready, "crate is not ready");
    }
}

bool crate::busy() const {
    return users_.load() > 0;
}

int crate::users() const {
    return users_.load();
}

void crate::initialize(bool reg_trace) {
    log(log::info) << "crate: initialise";

    /*
         * Set ready to true and if there is an issue return it to false.
         */
    if (ready_.exchange(true)) {
        throw error(pixie::error::code::crate_already_open, "create already initialised");
    }

    lock_guard guard(lock_);

    try {
        for (size_t device_number = 0; device_number < hw::max_slots; ++device_number) {

            add_module();

            auto& module_ptr = modules.back();
            module::module& module = *module_ptr;
            bool last_module_found = false;

            try {
                module.module_var_descriptors =
                    param::module_var_descs(param::get_module_var_descriptors());
                module.channel_var_descriptors =
                    param::channel_var_descs(param::get_channel_var_descriptors());
                module.reg_trace = reg_trace;
                module.open(device_number);
            } catch (pixie::error::error& e) {
                last_module_found = e.type == module::error::code::not_supported;
                log(log::error) << "module: device " << device_number << ": error: " << e.what();
            }

            if (module.present()) {
                log(log::info) << "module: device " << device_number << ": slot:" << module.slot
                               << " serial-number:" << module.serial_num
                               << " revision:" << module.revision_label();
            } else {
                if (last_module_found) {
                    modules.pop_back();
                    break;
                }
                log(log::info) << "module offline: device " << device_number;
                std::move(std::prev(modules.end()), modules.end(), std::back_inserter(offline));
                modules.pop_back();
            }
        }

        num_modules = modules.size();

        check_revision();
        check_slots();

        module::set_number_by_slot(modules);
        order_by_slot(modules);
    } catch (...) {
        ready_ = false;
        throw;
    }
}

void crate::set_offline(module::module_ptr module) {
    log(log::info) << "crate: set offline: slot=" << module->slot;
    lock_guard guard(lock_);
    for (auto mi = modules.begin(); mi != modules.end(); ++mi) {
        if (module == *mi) {
            module->force_offline();
            std::move(mi, std::next(mi), std::back_inserter(offline));
            modules.erase(mi);
            num_modules = modules.size();
            return;
        }
    }
    throw error(error::code::module_not_found, "module not seen as online");
}

void crate::probe() {
    log(log::info) << "crate: probe";
    ready();
    lock_guard guard(lock_);
    firmware::load(firmware);
    for (auto& module : modules) {
        module->probe();
    }
    firmware::clear(firmware);
}

void crate::boot() {
    log(log::info) << "crate: boot";

    ready();
    lock_guard guard(lock_);
    firmware::load(firmware);

    typedef std::promise<error::code> promise_error;
    typedef std::future<error::code> future_error;

    std::vector<promise_error> promises(modules.size());
    std::vector<future_error> futures;
    std::vector<std::thread> threads;

    for (size_t m = 0; m < modules.size(); ++m) {
        auto module = modules[m];
        if (module->revision == 0) {
            continue;
        }
        futures.push_back(future_error(promises[m].get_future()));
        threads.push_back(std::thread([m, &promises, module] {
            try {
                module->boot();
                promises[m].set_value(error::code::success);
            } catch (pixie::error::error& e) {
                promises[m].set_value(e.type);
            } catch (...) {
                try {
                    promises[m].set_exception(std::current_exception());
                } catch (...) {
                }
            }
        }));
    }

    error::code first_error = error::code::success;

    for (size_t t = 0; t < threads.size(); ++t) {
        error::code e = futures[t].get();
        if (first_error == error::code::success) {
            first_error = e;
        }
        threads[t].join();
    }

    firmware::clear(firmware);

    if (first_error != error::code::success) {
        throw error(first_error, "crate boot error; see log");
    }
}

void crate::set_firmware() {
    log(log::info) << "crate: set firmware";
    ready();
    lock_guard guard(lock_);
    for (auto& module : modules) {
        for (auto& config : module->configs) {
            auto tag = firmware::tag(module->revision, config.adc_msps, config.adc_bits);
            auto mod_fw = firmware.find(tag);
            if (mod_fw != firmware.end()) {
                module->add(firmware[tag]);
            } else {
                log(log::warning) << module::module_label(*module)
                                  << "crate: module firmware not found: " << tag;
            }
        }
        if (module->firmware.size() == 0) {
            log(log::warning) << module::module_label(*module) << "no firmware set";
        }
    }
}

void crate::import_config(const std::string json_file, module::number_slots& loaded) {
    log(log::info) << "crate: import configuration";
    ready();
    lock_guard guard(lock_);
    loaded.clear();
    config::import_json(json_file, *this, loaded);
    for (auto& module : modules) {
        if (module->online()) {
            module->sync_vars();
            module->sync_hw();
        }
    }
}

void crate::export_config(const std::string json_file) {
    log(log::info) << "crate: export configuration";
    lock_guard guard(lock_);
    config::export_json(json_file, *this);
}

void crate::move_offlines() {
    /*
     * Move any modules in the online list that are offline to the offline
     * list.
     */
    log(log::info) << "crate: move offline modules";
    lock_guard guard(lock_);
    bool have_moved = true;
    while (have_moved) {
        have_moved = false;
        for (auto mi = modules.begin(); mi != modules.end(); ++mi) {
            auto& module = *mi;
            if (!module->online()) {
                std::move(mi, std::next(mi), std::back_inserter(offline));
                modules.erase(mi);
                have_moved = true;
                break;
            }
        }
    }
    num_modules = modules.size();
}

void crate::output(std::ostream& out) const {
    if (!ready_.load()) {
        out << "not initialized";
        return;
    }
    out << "fw: tags: " << firmware.size() << std::endl;
    int c = 0;
    for (auto fw_rev : firmware) {
        for (auto& fw : std::get<1>(fw_rev)) {
            out << ' ' << std::setw(3) << ++c << ". " << ' ' << *fw << std::endl;
        }
    }
    out << "modules:" << modules.size() << std::endl;
    bool first = true;
    for (auto& mod : modules) {
        if (first) {
            first = false;
        } else {
            out << std::endl;
        }
        out << ' ' << *mod;
    }
}

void crate::assign(const module::number_slots& numbers, bool force_offline) {
    ready();
    lock_guard guard(lock_);
    /*
     * Any errors result in the crate being an unknown state.
     */
    try {
        module::assign(modules, numbers);
        /*
         * Force offline any module not in the map. The loop resets the
         * iterator after any changes to modules.
         */
        while (force_offline) {
            force_offline = false;
            for (auto mod : modules) {
                if (mod->number == -1) {
                    set_offline(mod);
                    force_offline = true;
                    break;
                }
            }
        }
        num_modules = modules.size();
        module::order_by_number(modules);
    } catch (...) {
        num_modules = modules.size();
        module::set_number_by_slot(modules);
        throw;
    }
}

void crate::add_module() {
    modules.push_back(std::make_unique<module::module>());
}


void crate::check_slots() {
    using duplicate = std::pair<module::module_ptr, module::module_ptr>;
    using duplicates = std::vector<duplicate>;

    struct find_dups {
        duplicates dups;

        void check(module::module_ptr& mod, module::modules& mods) {
            if (mod->present()) {
                for (auto compare : mods) {
                    if (compare->present() && mod != compare) {
                        if (mod->slot == compare->slot) {
                            auto di = std::find_if(dups.begin(), dups.end(),
                                                   [&mod](const duplicate& dup) {
                                                       auto dmod = std::get<0>(dup);
                                                       return mod->slot == dmod->slot;
                                                   });
                            if (di == dups.end()) {
                                dups.push_back(duplicate(mod, compare));
                            }
                        }
                    }
                }
            }
        }
    };

    find_dups dups;

    for (auto module : modules) {
        dups.check(module, modules);
        dups.check(module, offline);
    }

    for (auto module : offline) {
        dups.check(module, modules);
        dups.check(module, offline);
    }

    for (auto& dup : dups.dups) {
        auto mod1 = std::get<0>(dup);
        auto mod2 = std::get<1>(dup);
        if (revision == 2) {
            if (mod1->slot == 5 && mod1->pci_bus() == mod2->pci_bus()) {
                if (mod1->pci_slot() == 13) {
                    mod1->slot = 4;
                } else if (mod2->pci_slot() == 13) {
                    mod2->slot = 4;
                }
            }
        }
        if (mod1->slot == mod2->slot) {
            log(log::error) << "crate: duplicate slot: " << mod1->slot
                            << " 1:pci=" << mod1->pci_bus() << ':' << mod1->pci_slot()
                            << " 2:pci=" << mod2->pci_bus() << ':' << mod2->pci_slot();
        }
    }
}

void crate::check_revision() {
    revision = -1;
    for (auto module : modules) {
        if (module->present()) {
            if (revision < 0) {
                revision = module->crate_revision;
                log(log::info) << "crate: crate revision: " << revision;
            } else if (revision != module->crate_revision) {
                log(log::warning) << "crate: crate revision mismatch: " << module->crate_revision
                                  << " module slot=" << module->slot;
            }
        }
    }
    for (auto module : offline) {
        if (module->present()) {
            if (revision < 0) {
                revision = module->crate_revision;
                log(log::info) << "crate: crate revision: " << revision;
            } else if (revision != module->crate_revision) {
                log(log::warning) << "crate: crate revision mismatch: " << module->crate_revision
                                  << " module slot=" << module->slot;
            }
        }
    }
}

module_handle::module_handle(crate& crate_, size_t number)
    : handle(crate_[number]), user(crate_), guard(handle) {
    crate_.ready();
    if (!handle.online()) {
        throw error(pixie::error::code::module_offline, "module not online");
    }
}

module_handle::module_handle(crate& crate_, unsigned short number)
    : handle(crate_[number]), user(crate_), guard(handle) {
    crate_.ready();
    if (!handle.online()) {
        throw error(pixie::error::code::module_offline, "module not online");
    }
}
};  // namespace crate
};  // namespace pixie
};  // namespace xia

std::ostream& operator<<(std::ostream& out, const xia::pixie::crate::crate& crate) {
    crate.output(out);
    return out;
}
