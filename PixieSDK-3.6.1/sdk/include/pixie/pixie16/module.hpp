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
#include <pixie/sync.hpp>

#include <pixie/pixie16/backplane.hpp>
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
     * @brief Variable synchronising direction with the DSP
     */
    enum sync_var_mode {
        sync_to_dsp,
        sync_from_dsp
    };

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
     * @brief Register Trace guard to restore the previous trace value.
     */
    class reg_trace_guard {
        bool& reg_trace;
        bool reg_trace_state;

    public:
        reg_trace_guard(module& mod);
        ~reg_trace_guard();
        void enable();
        void disable();
    };

    /*
     * Stats for the module or a run.
     */
    struct fifo_stats {
        static constexpr size_t bw_update_period = 100000; /* only update after usecs */

        std::atomic_size_t in; /* Data into the fifo queue, units hw::words */
        std::atomic_size_t out; /* Data read from the fifo queue, units hw::words */
        std::atomic_size_t dma_in; /* DMA data in, units hw::words */
        std::atomic_size_t overflows; /* Fifo queue overflows, units events */
        std::atomic_size_t dropped; /* Fifo queue data dropped, units events */
        std::atomic_size_t hw_overflows; /* Fifo HW overflows, units events */
        std::atomic<double> bandwidth; /* Current bandwidth in MB/s*/
        std::atomic<double> max_bandwidth; /* Maximum bandwidth in MB/s */
        std::atomic<double> min_bandwidth; /* Minimum bandwidth in MB/s */

        fifo_stats();
        fifo_stats(const fifo_stats& s);

        void start();
        void stop();
        void clear();

        fifo_stats& operator=(const fifo_stats& s);

        /*
         * Get the lengths in bytes
         */
        size_t get_in_bytes() const;
        size_t get_out_bytes() const;
        size_t get_dma_in_bytes() const;

        /*
         * Update the bandwidth. The update only happens
         * after the @ref bw_update_period of time. True is
         * returned when the update occurred.
         */
        bool update_bandwidth();

        std::string output() const;

    private:
        bool calc_bandwidth(bool update_min_max = true);

        util::timepoint interval;
        uint64_t last_update;
        size_t last_dma_in;
    };

    /**
     * @brief Test mode
     */
    enum struct test { off = 0, lm_fifo };

    /*
     * Defaults
     */
    static const size_t default_fifo_buffers;
    static const size_t default_fifo_run_wait_usec;
    static const size_t default_fifo_idle_wait_usec;
    static const size_t default_fifo_hold_usec;
    static const size_t default_fifo_dma_trigger_level;

    /*
     * Ranges
     */
    static const size_t min_fifo_buffers;
    static const size_t max_fifo_buffers;
    static const size_t min_fifo_run_wait_usec;
    static const size_t max_fifo_run_wait_usec;
    static const size_t min_fifo_idle_wait_usec;
    static const size_t max_fifo_idle_wait_usec;
    static const size_t min_fifo_hold_usec;
    static const size_t max_fifo_hold_usec;
    static const size_t min_fifo_dma_trigger_level;
    static const size_t max_fifo_dma_trigger_level;

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
    int major_revision;
    int minor_revision;

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
     * Crate's backplane. Shared by all the modules.
     */
    backplane::backplane& backplane;

    /**
     * EEPROM
     */
    eeprom::eeprom eeprom;
    int eeprom_format;

    /**
     * Module fixtures
     */
    fixture::module_ptr fixtures;

    /**
     * Module run configuration
     */
    hw::run::module_config run_config;

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
     *
     * Do not set this value directly, use @ref set_fifo_buffers.
     */
    size_t fifo_buffers;

    /**
     * FIFO run wait poll period. This setting needs to be less than the
     * period of time it takes to full the FIFO device at the maxiumum data
     * rate. It is used when a run using the FIFO starts or data is detected
     * in the FIFO. If data is read from the FIFO while present and once
     * empty this period is the wait time.
     *
     * Do not set this value directly, use @ref set_fifo_run_wait.
     */
    std::atomic_size_t fifo_run_wait_usecs;

    /**
     * FIFO idle wait poll period. This setting is a back ground poll
     * period used when no run active using the FIFO. When a run
     * finishes the poll period increases by the power 2 every hold
     * period until this value is reached.
     *
     * Do not set this value directly, use @ref set_fifo_idle_wait.
     */
    std::atomic_size_t fifo_idle_wait_usecs;

    /**
     * FIFO hold time is the period data is held in the FIFO before being
     * read into a buffer. Slow data and long poll periods by the user can
     * use all the buffers. If buffers run low the queue is compacted.
     *
     * Do not set this value directly, use @ref set_fifo_hold.
     */
    std::atomic_size_t fifo_hold_usecs;

    /**
     * FIFO DMA transfer trigger level. This value is in FIFO words. A
     * DMA transfer will happen when the FIFO level is higher than
     * this trigger level or the hold time has been exceeded. This
     * value effects the latency of data in the FIFO.
     *
     * Do not set this value directly, use @ref set_fifo_dma_trigger_level.
     */
    std::atomic_size_t fifo_dma_trigger_level;

    /**
     * Maximum bandwidth the module is to use. The value is Mbytes/secs. If
     * set to 0 there is no throttling of the bandwidth.
     */
    std::atomic_size_t fifo_bandwidth;

    /*
     * Run stats, only updated when a run is active
     */
    util::timepoint run_interval; /* Period of the run */
    fifo_stats run_stats;

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

    /*
     * Microsconds for a read
     */
    double bus_cycle_period;

    /**
     * Modules are created by the crate.
     */
    module(backplane::backplane& backplane);
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
    void sync_vars(const sync_var_mode sync_mode = sync_to_dsp);

    /**
     * Sync the hardware after the variables have been sync'ed with @ref sync_var and
     * the mode sync mode is @ref sync_to_dsp.
     */
    void sync_hw(const bool program_fippi = true, const bool program_dacs = true);

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
    void tau_finder();
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
    size_t read_list_mode(hw::words& words);
    size_t read_list_mode(hw::word_ptr values, const size_t size);

    /**
     * Read the stats
     */
    void read_stats(stats::stats& stats);

    /**
     * Read auto tau values
     */
    void read_autotau(hw::doubles& taus);

    /**
     * FIFO worker controls. These range values.
     */
    void set_fifo_buffers(const size_t buffers);
    void set_fifo_run_wait(const size_t run_wait);
    void set_fifo_idle_wait(const size_t idle_wait);
    void set_fifo_hold(const size_t hold);
    void set_fifo_dma_trigger_level(const size_t dma_trigger_level);
    void set_fifo_bandwidth(const size_t bandwidth);

    /**
     * Select the module's port
     */
    void select_port(const int port);

    /*
     * Output the module details.
     */
    void output(std::ostream& out) const;
    std::string version_label() const;
    char revision_label() const;

    /**
     * A report
     */
    void report(std::ostream& out) const;

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
    hw::rev_tag get_rev_tag() const;
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
     * Initialise abs rease the values.
     */
    virtual void erase_values();
    virtual void init_values();

    /*
     * Initialise and erase the channels.
     */
    virtual void erase_channels();
    virtual void init_channels();

    /*
     * Module parameter handlers.
     */
    void module_csrb(param::value_type value, size_t offset = 0, bool io = true);
    void slow_filter_range(param::value_type value, size_t offset = 0, bool io = true);
    void fast_filter_range(param::value_type value, size_t offset = 0, bool io = true);

    /*
     * Synchronization support.
     */
    void sync_csrb();

    /*
     * Backplane filters.
     */
    void backplane_csrb(const param::value_type csrb);

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

    /*
     * Calculate the bus speed
     */
    void calc_bus_speed();

    /*
     * Output stats
     */
    void log_stats(const char* label, const fifo_stats& stats);

    /*
     * Request the worker to run and wait for to respond it has
     * completed the run.
     */
    bool fifo_worker_run(size_t timeout_usecs);

    /*
     * Synchronous worker run
     */
    void sync_worker_run(bool forced = false);

    std::thread fifo_thread;

    std::atomic_bool fifo_worker_running;
    std::atomic_bool fifo_worker_finished;
    sync::variable::lock_type fifo_worker_working;
    sync::variable fifo_worker_req;
    sync::variable fifo_worker_resp;

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
     * Control CS shadow, it is a write-only register
     */
    hw::word cfg_ctrlcs;

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
        xia_log(log::debug) << "M r " << std::setfill('0') << std::hex << vmaddr << ':' << std::setw(2)
                            << reg << " => " << std::setw(8) << value;
    }
    return value;
}

inline void module::write_word(int reg, const hw::word value) {
    if (reg_trace) {
        xia_log(log::debug) << "M w " << std::setfill('0') << std::hex << vmaddr << ':' << std::setw(2)
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
