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

/** @file module.hpp
 * @brief Defines the functions and data structures to work with a Pixie-16 module.
 */

#ifndef PIXIE_MODULE_H
#define PIXIE_MODULE_H

#include <atomic>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include <pixie/buffer.hpp>
#include <pixie/eeprom.hpp>
#include <pixie/error.hpp>
#include <pixie/fw.hpp>
#include <pixie/log.hpp>
#include <pixie/param.hpp>
#include <pixie/stats.hpp>

#include <pixie/pixie16/channel.hpp>
#include <pixie/pixie16/hw.hpp>
#include <pixie/pixie16/run.hpp>

namespace xia {
namespace pixie {
/**
 * @brief Contains everything needed to work with a composite Pixie-16 module.
 */
namespace module {
/**
 * @brief Defines an error structure for module specific errors.
 */
struct error : public pixie::error::error {
    typedef pixie::error::code code;
    explicit error(const int num, const int slot, const code type, const std::ostringstream& what);
    explicit error(const int num, const int slot, const code type, const std::string& what);
    explicit error(const int num, const int slot, const code type, const char* what);
    virtual void output(std::ostream& out);

private:
    std::string make_what(const int num, const int slot, const char* what);
};

/**
 * PCI bus handle is opaque. No direct access as it is
 * specific to the PCI drivers.
 */
struct pci_bus_handle;
/**
 * @brief Defines a vector of unique pci_bus_handles to ensure thread safety.
 */
typedef std::unique_ptr<pci_bus_handle> bus_handle;

/**
 * @brief Defines a Pixie-16 Module
 *
 * A module can only be a single specific instance, and it is designed to
 * live in a container of modules in a crate. There are limitations on
 * the type of things you can do with a module object. It contains a
 * unique pointer to the opaque bus handle and there can only ever be
 * one instance of a bus handle. If the handle in a module is initialised
 * the handle will be closed when the module destructs. If an instance of
 * a module could be copied and that instance destructs the handle would
 * close the module's device.
 */
class module {
    /*
     * Module lock
     */
    typedef std::recursive_mutex lock_type;
    typedef std::lock_guard<lock_type> lock_guard;

    /*
     * Bus lock
     */
    typedef std::mutex bus_lock_type;
    typedef std::lock_guard<bus_lock_type> bus_lock_guard;

public:
    /**
     * @brief Provides a guard to prevent concurrent module access.
     */
    class guard {
        lock_type& lock_;
        lock_guard guard_;

    public:
        guard(module& mod);
        ~guard() = default;
        void lock();
        void unlock();
    };

    /**
     * @brief Bus lock guard to prevent concurrent requests to the bus.
     */
    class bus_guard {
        bus_lock_type& lock_;
        bus_lock_guard guard_;

    public:
        bus_guard(module& mod);
        ~bus_guard() = default;
        void lock();
        void unlock();
    };

    /**
     * @brief Test mode
     */
    enum struct test { off = 0, lm_fifo };

    /*
     * Defaults
     */
    static const size_t default_fifo_buffers = 100;
    static const size_t default_fifo_run_wait_usec = 5000;
    static const size_t default_fifo_idle_wait_usec = 150000;
    static const size_t default_fifo_hold_usec = 100000;

    /**
     * Slot in the crate.
     */
    int slot;

    /**
     * Logical module mapping for this instance of the SDK.
     */
    int number;

    /**
     * Serial number.
     */
    int serial_num;

    /**
     * Revision of the board
     */
    int revision;

    /**
     * Number of channels
     */
    size_t num_channels;

    /**
     * Maximum umber of channels
     */
    size_t max_channels;

    /**
     * Module's register VM address.
     */
    void* vmaddr;

    /**
     * Channel configs
     */
    hw::configs configs;

    /**
     * EEPROM
     */
    eeprom::eeprom eeprom;
    int eeprom_format;

    /*
     * Module parameters
     */
    param::module_var_descs module_var_descriptors;
    param::module_variables module_vars;

    /*
     * Channel parameters, a set per channel.
     */
    param::channel_var_descs channel_var_descriptors;
    channel::channels channels;

    /**
     * Parameter configuration.
     */
    param::address_map param_addresses;

    /**
     * Firmware
     */
    firmware::module firmware;

    /*
     * Run and control task states.
     */
    std::atomic<hw::run::run_task> run_task;
    std::atomic<hw::run::control_task> control_task;

    /**
     * Number of buffers in the FIFO pool. The buffers are fixed to the
     * maximum DMA block size and allocated at the start of a run.
     */
    size_t fifo_buffers;

    /**
     * FIFO run wait poll period. This setting needs to be less than the
     * period of time it takes to full the FIFO device at the maxiumum data
     * rate. It is used when a run using the FIFO starts or data is detected
     * in the FIFO.
     */
    std::atomic_size_t fifo_run_wait_usecs;

    /**
     * FIFO idle wait poll period. This setting is a back ground poll period
     * used when there is not run active using the FIFO. When a run
     * finishes the poll period increases by the power 2 every hold period
     * until this value is reached.
     */
    std::atomic_size_t fifo_idle_wait_usecs;

    /**
     * FIFO hold time is the period data is held in the FIFO before being
     * read into a buffer. Slow data and long poll periods by the user can
     * use all the buffers. If buffers run low the queue is compacted.
     */
    std::atomic_size_t fifo_hold_usecs;

    /**
     * Maximum bandwidth the module is to use. The value is Mbytes/secs. If
     * set to 0 there is no throttling of the bandwidth.
     */
    std::atomic_size_t fifo_bandwidth;

    /*
     * Dataflow stats
     */
    std::atomic_size_t data_dma_in; /* DMA data in */
    std::atomic_size_t data_fifo_in; /* Data into the fifo queue */
    std::atomic_size_t data_fifo_out; /* Data read from the fifo queue */

    /*
     * Run stats
     */
    util::timepoint run_interval; /* Period of the run */
    std::atomic_size_t data_fifo_run_in; /* DMA data into the fifo queue */
    std::atomic_size_t data_fifo_run_out; /* Data read from the fifo queue */

    /**
     * Crate revision
     */
    int crate_revision;

    /**
     * Board revision
     */
    int board_revision;

    /**
     * Diagnostics
     */
    bool reg_trace;

    /**
     * Modules are created by the crate.
     */
    module();
    module(module&& m);
    virtual ~module();
    module& operator=(module&& mod);

    /**
     * If the module present?
     */
    bool present() const;

    /**
     * Has the module been booted and is online?
     */
    bool online() const;

    /**
     * Open the module and find the device on the bus.
     */
    virtual void open(size_t device_number);
    virtual void close();

    /**
     * Force offline.
     */
    void force_offline();

    /**
     * Range check the channel number.
     */
    template<typename T>
    void check_channel_num(T number);

    /**
     * Probe the board to see what is running.
     */
    virtual void probe();

    /**
     * Boot the module. If successful it will be online.
     */
    virtual void boot(bool boot_comms = true, bool boot_fippi = true, bool boot_dsp = true);

    /**
     * Initialise the module ready for use.
     */
    virtual void initialize();

    /**
     * Add or get the firmware.
     */
    void add(firmware::module& fw);
    firmware::firmware_ref get(const std::string device);

    /**
     * Range checking operator to index channels based on various index
     * types.
     */
    template<typename T>
    channel::channel& operator[](T number) {
        size_t number_ = static_cast<size_t>(number);
        channel_check(number_);
        return channels[number_];
    }

    /*
     * Read a parameter.
     */
    param::value_type read(const std::string& par);
    param::value_type read(param::module_param par);
    double read(const std::string& par, size_t channel);
    double read(param::channel_param par, size_t channel);

    /*
     * Write a parameter.
     */
    bool write(const std::string& var, param::value_type value);
    bool write(param::module_param var, param::value_type value);
    void write(const std::string& var, size_t channel, double value);
    void write(param::channel_param par, size_t channel, double value);

    /*
     * Read a variable.
     *
     * Note, the variable string version is a convenience function
     * for test tools only. The channel is ignored if the string is
     * for a module variable.
     *
     * `io` true reads the value from the DSP, false returns the
     * module's copy.
     */
    param::value_type read_var(const std::string& var, size_t channel, size_t offset = 0,
                               bool io = true);
    param::value_type read_var(param::module_var var, size_t offset = 0, bool io = true);
    param::value_type read_var(param::channel_var, size_t channel, size_t offset = 0,
                               bool io = true);

    /*
     * Write a variable.
     *
     * Note, the variable string version is a convenience function
     * for test tools only. The channel is ignored if the string is
     * for a module variable.
     */
    void write_var(const std::string& var, param::value_type value, size_t channel,
                   size_t offset = 0, bool io = true);
    void write_var(param::module_var var, param::value_type value, size_t offset = 0,
                   bool io = true);
    void write_var(param::channel_var, param::value_type value, size_t channel, size_t offset = 0,
                   bool io = true);

    /**
     * Synchronize dirty variables with the hardware and then sync the
     * hardware state.
     */
    void sync_vars();

    /**
     * Sync the hardware after the variables have been sync'ed.
     */
    void sync_hw();

    /*
     * Run control and status
     */
    void run_end();
    bool run_active();

    /*
     * Control tasks
     */
    void acquire_baselines();
    void adjust_offsets();
    void get_traces();
    void set_dacs();

    /*
     * Run tasks
     */
    void start_histograms(hw::run::run_mode mode);
    void start_listmode(hw::run::run_mode mode);

    /**
     * @brief Reads an ADC trace from the specified channel.
     * @param[in] channel The channel that we'd like to read the ADC trace from.
     * @param[out] buffer A buffer large enough to hold the ADC trace.
     * @param[in] size The size of the buffer to hold the trace.
     * @param[in] run If true, then we execute the control task to collect the
     *                  ADC traces. If false, then we assume the control task has
     *                  already been run.
     */
    void read_adc(size_t channel, hw::adc_word* buffer, size_t size, bool run = true);
    /**
     * @brief Reads an ADC trace from the specified channel.
     * @param[in] channel The channel that we'd like to read the ADC trace from.
     * @param[out] buffer An adc_trace vector that will hold the resultant data.
     * @param[in] run If true, then we execute the control task to collect the
     *                  ADC traces. If false, then we assume the control task has
     *                  already been run.
     */
    void read_adc(size_t channel, hw::adc_trace& buffer, bool run = true);

    /*
     * Find the baseline cut for the range of channels. Return the
     * baselines.
     */
    void bl_find_cut(channel::range& channels, param::values& cuts);
    void bl_get(channel::range& channels_, channel::baseline::channels_values& values,
                bool run = true);

    /*
     * Read a channel's histogram.
     */
    void read_histogram(size_t channel, hw::words& values);
    void read_histogram(size_t channel, hw::word_ptr values, const size_t size);

    /*
     * Read the module's list mode
     */
    size_t read_list_mode_level();
    void read_list_mode(hw::words& words);
    void read_list_mode(hw::word_ptr values, const size_t size);

    /**
     * Read the stats
     */
    void read_stats(stats::stats& stats);

    /*
     * Output the module details.
     */
    void output(std::ostream& out) const;
    char revision_label() const;

    /**
     * Read a word.
     */
    hw::word read_word(int reg);

    /**
     * Write a word
     */
    void write_word(int reg, const hw::word value);

    /*
     * DMA block read.
     */
    virtual void dma_read(const hw::address source, hw::words& values);
    virtual void dma_read(const hw::address source, hw::word_ptr values, const size_t size);

    /*
     * Revision tag operators to make comparisons of a version simpler to
     * code.
     */
    bool operator==(const hw::rev_tag rev) const;
    bool operator!=(const hw::rev_tag rev) const;
    bool operator>=(const hw::rev_tag rev) const;
    bool operator<=(const hw::rev_tag rev) const;
    bool operator<(const hw::rev_tag rev) const;
    bool operator>(const hw::rev_tag rev) const;

    /*
     * Checks, throws errors.
     */
    void online_check() const;
    void channel_check(const size_t channel) const;

    /*
     * PCI bus.
     */
    int pci_bus();
    int pci_slot();

    /*
     * Test modes
     *
     * The following provide module test modes. These are use to check the
     * hardware and not the signal or signal analysis quality.
     *
     * Please consult with XIA support services if you are considering
     * using these calls.
     */
    void start_test(const test mode);
    void end_test();

protected:
    /*
     * Locks
     */
    void lock() {
        lock_.lock();
    }

    void unlock() {
        lock_.unlock();
    }

    /*
     * Load the variable address map.
     */
    virtual void load_vars();

    /*
     * Initialise the values.
     */
    virtual void erase_values();
    virtual void init_values();

    /*
     * Module parameter handlers.
     */
    void module_csrb(param::value_type value, size_t offset = 0, bool io = true);
    void slow_filter_range(param::value_type value, size_t offset = 0, bool io = true);
    void fast_filter_range(param::value_type value, size_t offset = 0, bool io = true);

    /*
     * Check if the FPGA devices are programmed and start the FIFO services
     * if they are.
     */
    void start_fifo_services();
    void stop_fifo_services();

    /*
     * FIFO worker
     */
    void start_fifo_worker();
    void stop_fifo_worker();
    void fifo_worker();

    std::thread fifo_thread;

    std::atomic_bool fifo_worker_running;
    std::atomic_bool fifo_worker_finished;

    buffer::pool fifo_pool;
    buffer::queue fifo_data;

    /*
     * Module lock
     */
    lock_type lock_;

    /*
     * Bus lock
     */
    bus_lock_type bus_lock_;

    /*
     * In use counter.
     */
    size_t in_use;

    /*
     * Present in the rack.
     */
    std::atomic_bool present_;

    /*
     * Online and ready to use.
     */
    std::atomic_bool online_;

    /*
     * Forced offline by the user.
     */
    std::atomic_bool forced_offline_;

    /*
     * Pause the FIFO worker.
     */
    std::atomic_bool pause_fifo_worker;

    /*
     * System, FIPPI and DSP online.
     */
    bool comms_fpga;
    bool fippi_fpga;
    bool dsp_online;

    /*
     * Have hardware?
     */
    bool have_hardware;

    /*
     * Vars loaded?
     */
    bool vars_loaded;

    /*
     * PCI bus. The type is opaque.
     */
    bus_handle device;

    /*
     * Current test mode.
     */
    std::atomic<test> test_mode;
};

inline hw::word module::read_word(int reg) {
    hw::word value;
    if (have_hardware) {
        value = hw::read_word(vmaddr, reg);
    } else {
        value = 0;
    }
    if (reg_trace) {
        log(log::debug) << "M r " << std::setfill('0') << std::hex << vmaddr << ':' << std::setw(2)
                        << reg << " => " << std::setw(8) << value;
    }
    return value;
}

inline void module::write_word(int reg, const hw::word value) {
    if (reg_trace) {
        log(log::debug) << "M w " << std::setfill('0') << std::hex << vmaddr << ':' << std::setw(2)
                        << reg << " <= " << std::setw(8) << value;
    }
    if (have_hardware) {
        hw::write_word(vmaddr, reg, value);
    }
}

/**
 * Make a label from the module
 */
std::string module_label(const module& mod, const char* label = "module");

/*
 * A list of numbers that can be assigned to modules by slots
 */
typedef std::pair<int, int> number_slot;
typedef std::vector<number_slot> number_slots;

/**
 * A module pointer.
 */
typedef std::shared_ptr<module> module_ptr;

/**
 * A container of modules.
 */
typedef std::vector<module_ptr> modules;

/**
 * Assign the number to the slots in the rack.
 */
void assign(modules& mods, const number_slots& numbers);

/**
 * Sort the modules by index.
 */
void order_by_number(modules& mods);

/**
 * Sort the modules by slot.
 */
void order_by_slot(modules& mods);

/**
 * Set the module numbers to the slot order.
 */
void set_number_by_slot(modules& mods);
}  // namespace module
}  // namespace pixie
}  // namespace xia

/**
 * @brief Output stream operator for modules
 */
std::ostream& operator<<(std::ostream& out, const xia::pixie::module::module& module);

#endif  // PIXIE_MODULE_H
