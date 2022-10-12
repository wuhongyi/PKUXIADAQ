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

/** @file module.cpp
 * @brief Implements the functions and data structures to work with a Pixie-16 module.
 */

#include <algorithm>
#include <cctype>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <pixie/log.hpp>
#include <pixie/util.hpp>

#include <pixie/pixie16/channel.hpp>
#include <pixie/pixie16/csr.hpp>
#include <pixie/pixie16/defs.hpp>
#include <pixie/pixie16/dsp.hpp>
#include <pixie/pixie16/fpga_comms.hpp>
#include <pixie/pixie16/fpga_fippi.hpp>
#include <pixie/pixie16/i2cm24c64.hpp>
#include <pixie/pixie16/memory.hpp>
#include <pixie/pixie16/module.hpp>
#include <pixie/pixie16/pcf8574.hpp>
#include <pixie/pixie16/run.hpp>

#include <PlxApi.h>

namespace xia {
namespace pixie {
namespace module {
static std::string module_label(const int num, const int slot, const char* label = "module") {
    std::ostringstream oss;
    oss << label << ": num=" << num << ",slot=" << slot << ": ";
    return oss.str();
}

std::string module_label(const module& mod, const char* label) {
    return module_label(mod.number, mod.slot, label);
}

error::error(const int num, const int slot, const code type, const std::ostringstream& what)
    : pixie::error::error(type, make_what(num, slot, what.str().c_str())) {}

error::error(const int num, const int slot, const code type, const std::string& what)
    : pixie::error::error(type, make_what(num, slot, what.c_str())) {}

error::error(const int num, const int slot, const code type, const char* what)
    : pixie::error::error(type, make_what(num, slot, what)) {}

void error::output(std::ostream& out) {
    util::ostream_guard flags(out);
    out << std::setfill(' ') << "error: code=" << std::setw(2) << result() << ' ' << what();
}

std::string error::make_what(const int num, const int slot, const char* what_) {
    std::ostringstream oss;
    oss << module_label(num, slot) << what_;
    return oss.str();
}

/*
 * PLX PCI vendor and device id
 */
const int vendor_id = 0x10b5;
const int device_id = 0x9054;

#if PLX_SDK_VERSION_MAJOR >= 8
static const char* pci_error_labels[] = {"PLX_STATUS_OK",
                                         "PLX_STATUS_FAILED",
                                         "PLX_STATUS_NULL_PARAM",
                                         "PLX_STATUS_UNSUPPORTED",
                                         "PLX_STATUS_NO_DRIVER",
                                         "PLX_STATUS_INVALID_OBJECT",
                                         "PLX_STATUS_VER_MISMATCH",
                                         "PLX_STATUS_INVALID_OFFSET",
                                         "PLX_STATUS_INVALID_DATA",
                                         "PLX_STATUS_INVALID_SIZE",
                                         "PLX_STATUS_INVALID_ADDR",
                                         "PLX_STATUS_INVALID_ACCESS",
                                         "PLX_STATUS_INSUFFICIENT_RES",
                                         "PLX_STATUS_TIMEOUT",
                                         "PLX_STATUS_CANCELED",
                                         "PLX_STATUS_COMPLETE",
                                         "PLX_STATUS_PAUSED",
                                         "PLX_STATUS_IN_PROGRESS",
                                         "PLX_STATUS_PAGE_GET_ERROR",
                                         "PLX_STATUS_PAGE_LOCK_ERROR",
                                         "PLX_STATUS_LOW_POWER",
                                         "PLX_STATUS_IN_USE",
                                         "PLX_STATUS_DISABLED",
                                         "PLX_STATUS_PENDING",
                                         "PLX_STATUS_NOT_FOUND",
                                         "PLX_STATUS_INVALID_STATE",
                                         "PLX_STATUS_BUFF_TOO_SMALL"};
#else
#define PLX_STATUS_OK ApiSuccess
#define PLX_STATUS_NO_DRIVER ApiNoActiveDriver
#define PLX_STATUS_INVALID_OBJECT ApiInvalidDeviceInfo
#define PLX_STATUS_INVALID_ACCESS ApiDmaChannelUnavailable
#define PLX_STATUS_IN_PROGRESS ApiDmaInProgress
#define PLX_STATUS_RSVD_LAST_ERROR ApiLastError

static const char* pci_error_labels[] = {"ApiSuccess",
                                         "ApiFailed",
                                         "ApiNullParam",
                                         "ApiUnsupportedFunction",
                                         "ApiNoActiveDriver",
                                         "ApiConfigAccessFailed",
                                         "ApiInvalidDeviceInfo",
                                         "ApiInvalidDriverVersion",
                                         "ApiInvalidOffset",
                                         "ApiInvalidData",
                                         "ApiInvalidSize",
                                         "ApiInvalidAddress",
                                         "ApiInvalidAccessType",
                                         "ApiInvalidIndex",
                                         "ApiInvalidPowerState",
                                         "ApiInvalidIopSpace",
                                         "ApiInvalidHandle",
                                         "ApiInvalidPciSpace",
                                         "ApiInvalidBusIndex",
                                         "ApiInsufficientResources",
                                         "ApiWaitTimeout",
                                         "ApiWaitCanceled",
                                         "ApiDmaChannelUnavailable",
                                         "ApiDmaChannelInvalid",
                                         "ApiDmaDone",
                                         "ApiDmaPaused",
                                         "ApiDmaInProgress",
                                         "ApiDmaCommandInvalid",
                                         "ApiDmaInvalidChannelPriority",
                                         "ApiDmaSglPagesGetError",
                                         "ApiDmaSglPagesLockError",
                                         "ApiMuFifoEmpty",
                                         "ApiMuFifoFull",
                                         "ApiPowerDown",
                                         "ApiHSNotSupported",
                                         "ApiVPDNotSupported",
                                         "ApiDeviceInUse",
                                         "ApiDeviceDisabled",
                                         "ApiPending",
                                         "ApiObjectNotFound",
                                         "ApiInvalidState",
                                         "ApiBufferTooSmall",
                                         "ApiLastError"};
#endif

struct pci_bus_handle {
    int device_number;
    PLX_DEVICE_OBJECT handle;
    PLX_DEVICE_KEY key;
    PLX_DMA_PROP dma;
    pci_bus_handle();
    unsigned int domain() const;
    unsigned int bus() const;
    unsigned int slot() const;
};

pci_bus_handle::pci_bus_handle() : device_number(-1) {
    ::memset(&key, PCI_FIELD_IGNORE, sizeof(PLX_DEVICE_KEY));
    ::memset(&dma, 0, sizeof(PLX_DMA_PROP));
    key.VendorId = vendor_id;
    key.DeviceId = device_id;
}

unsigned int pci_bus_handle::domain() const {
    unsigned int val(key.domain);
    return val;
}

unsigned int pci_bus_handle::bus() const {
    unsigned int val(key.bus);
    return val;
}

unsigned int pci_bus_handle::slot() const {
    unsigned int val(key.slot);
    return val;
}

std::string pci_error_text(PLX_STATUS ps) {
    std::ostringstream oss;
    oss << "PLX (" << int(ps) << ") ";
    if (ps >= PLX_STATUS_OK && ps < PLX_STATUS_RSVD_LAST_ERROR) {
        oss << pci_error_labels[ps - PLX_STATUS_OK];
    } else {
        oss << "unknown error code";
    }
    return oss.str();
}

module::guard::guard(module& mod) : lock_(mod.lock_), guard_(lock_) {}

void module::guard::lock() {
    lock_.lock();
}

void module::guard::unlock() {
    lock_.unlock();
}

module::bus_guard::bus_guard(module& mod) : lock_(mod.bus_lock_), guard_(lock_) {}

void module::bus_guard::lock() {
    lock_.lock();
}

void module::bus_guard::unlock() {
    lock_.unlock();
}

module::reg_trace_guard::reg_trace_guard(module& mod)
    : reg_trace(mod.reg_trace), reg_trace_state(mod.reg_trace) {}

module::reg_trace_guard::~reg_trace_guard() {
    reg_trace = reg_trace_state;
}

void module::reg_trace_guard::enable() {
    reg_trace = true;
}

void module::reg_trace_guard::disable() {
    reg_trace = false;
}

module::fifo_stats::fifo_stats() {
    clear();
}

module::fifo_stats::fifo_stats(const module::fifo_stats& s)
    : in(s.in.load()), out(s.out.load()), dma_in(s.dma_in.load()),
      overflows(s.overflows.load()), dropped(s.dropped.load()),
      hw_overflows(s.hw_overflows.load()), bandwidth(s.bandwidth.load()),
      max_bandwidth(s.max_bandwidth.load()), min_bandwidth(s.min_bandwidth.load()),
      last_update(0), last_dma_in(0) {
}

void module::fifo_stats::start() {
    clear();
    interval.start();
}

void module::fifo_stats::stop() {
    /*
     * Capture the interval and then computer the average bandwidth
     * for the interval.
     */
    interval.stop();
    last_update = 0;
    last_dma_in = 0;
    calc_bandwidth(false);
}

void module::fifo_stats::clear() {
    in = 0;
    out = 0;
    dma_in = 0;
    overflows = 0;
    dropped = 0;
    hw_overflows = 0;
    bandwidth = 0;
    max_bandwidth = 0;
    min_bandwidth = 0;
    interval.reset();
    last_update = 0;
    last_dma_in = 0;
}

module::fifo_stats& module::fifo_stats::operator=(const module::fifo_stats& s) {
    interval = s.interval;
    last_update = s.last_update;
    last_dma_in = s.last_dma_in;
    in = s.in.load();
    out = s.out.load();
    dma_in = s.dma_in.load();
    overflows = s.overflows.load();
    dropped = s.dropped.load();
    hw_overflows = s.hw_overflows.load();
    bandwidth = s.bandwidth.load();
    max_bandwidth = s.max_bandwidth.load();
    min_bandwidth = s.min_bandwidth.load();
    return *this;
}

size_t module::fifo_stats::get_in_bytes() const {
    return in.load() * sizeof(hw::word);
}

size_t module::fifo_stats::get_out_bytes() const {
    return out.load() * sizeof(hw::word);
}

size_t module::fifo_stats::get_dma_in_bytes() const {
    return dma_in.load() * sizeof(hw::word);
}

bool module::fifo_stats::update_bandwidth() {
    if (!interval.running()) {
        return false;
    }
    return calc_bandwidth();
}

std::string module::fifo_stats::output() const {
    auto period = interval;
    std::ostringstream oss;
    oss << "period=" << period
        << " bw=" << bandwidth
        << "MB/s max-bw=" << max_bandwidth
        << "MB/s min-bw=" << min_bandwidth
        << "MB/s in=" << get_in_bytes()
        << " out=" << get_out_bytes()
        << " dma-in=" << get_dma_in_bytes()
        << " overflows=" << overflows.load() << " dropped=" << dropped.load()
        << " hw-overflows=" << hw_overflows.load();
    return oss.str();
}

bool module::fifo_stats::calc_bandwidth(bool update_min_max) {
    bool updated = false;
    auto period = interval.usecs();
    auto update_period = period - last_update;
    if (update_period >= bw_update_period) {
        auto this_dma_in = dma_in.load();
        double delta = (this_dma_in - last_dma_in) * sizeof(hw::word);
        double bw = delta / update_period;
        bandwidth = bw;
        if (update_min_max) {
            if (bw > max_bandwidth.load()) {
                max_bandwidth = bw;
            }
            if (min_bandwidth == 0 || bw < min_bandwidth.load()) {
                min_bandwidth = bw;
            }
        }
        last_dma_in = this_dma_in;
        last_update = period;
        updated = true;
    }
    return updated;
}

/*
 * FIFO Worker settings
 */
const size_t module::default_fifo_buffers = 100;
const size_t module::default_fifo_run_wait_usec = 5000;
const size_t module::default_fifo_idle_wait_usec = 150000;
const size_t module::default_fifo_hold_usec = 10000;
const size_t module::default_fifo_dma_trigger_level = 1024;
const size_t module::min_fifo_buffers = 10;
const size_t module::max_fifo_buffers = 10000000;
const size_t module::min_fifo_run_wait_usec = 500;
const size_t module::max_fifo_run_wait_usec = 200000;
const size_t module::min_fifo_idle_wait_usec = 10000;
const size_t module::max_fifo_idle_wait_usec = 1000000;
const size_t module::min_fifo_hold_usec = 1000;
const size_t module::max_fifo_hold_usec = 100000;
const size_t module::min_fifo_dma_trigger_level = 512;
const size_t module::max_fifo_dma_trigger_level = hw::max_dma_block_size;

module::module(backplane::backplane& backplane_)
    : slot(0), number(-1), serial_num(0), revision(0), major_revision(0), minor_revision(0),
      num_channels(0), vmaddr(nullptr), backplane(backplane_), eeprom_format(-1),
      run_task(hw::run::run_task::nop), control_task(hw::run::control_task::nop),
      fifo_buffers(default_fifo_buffers), fifo_run_wait_usecs(default_fifo_run_wait_usec),
      fifo_idle_wait_usecs(default_fifo_idle_wait_usec), fifo_hold_usecs(default_fifo_hold_usec),
      fifo_dma_trigger_level(default_fifo_dma_trigger_level), fifo_bandwidth(0),
      crate_revision(-1), board_revision(-1), reg_trace(false), bus_cycle_period(100),
      fifo_worker_running(false), fifo_worker_finished(false), fifo_worker_req(fifo_worker_working),
      fifo_worker_resp(fifo_worker_working), in_use(0), present_(false), online_(false),
      forced_offline_(false), pause_fifo_worker(true), comms_fpga(false), fippi_fpga(false),
      have_hardware(false), vars_loaded(false), cfg_ctrlcs(0xaaa),
      device(std::make_unique<pci_bus_handle>()), test_mode(test::off) {}

module::module(module&& m)
    : slot(m.slot), number(m.number), serial_num(m.serial_num), revision(m.revision),
      major_revision(0), minor_revision(0), num_channels(m.num_channels), vmaddr(m.vmaddr),
      backplane(m.backplane), eeprom(m.eeprom), eeprom_format(m.eeprom_format),
      module_var_descriptors(std::move(m.module_var_descriptors)),
      module_vars(std::move(m.module_vars)),
      channel_var_descriptors(std::move(m.channel_var_descriptors)),
      channels(std::move(m.channels)), firmware(std::move(m.firmware)), run_task(m.run_task.load()),
      control_task(m.control_task.load()), fifo_buffers(m.fifo_buffers),
      fifo_run_wait_usecs(m.fifo_run_wait_usecs.load()),
      fifo_idle_wait_usecs(m.fifo_idle_wait_usecs.load()),
      fifo_hold_usecs(m.fifo_hold_usecs.load()), fifo_bandwidth(m.fifo_bandwidth.load()),
      fifo_dma_trigger_level(m.fifo_dma_trigger_level.load()),
      run_stats(m.run_stats), crate_revision(m.crate_revision),
      board_revision(m.board_revision), reg_trace(m.reg_trace), bus_cycle_period(100),
      fifo_worker_running(false), fifo_worker_finished(false), fifo_worker_req(fifo_worker_working),
      fifo_worker_resp(fifo_worker_working), in_use(0), present_(m.present_.load()),
      online_(m.online_.load()), forced_offline_(m.forced_offline_.load()),
      pause_fifo_worker(m.pause_fifo_worker.load()), comms_fpga(m.comms_fpga),
      fippi_fpga(m.fippi_fpga), have_hardware(false), vars_loaded(false),
      cfg_ctrlcs(0xaaa), device(std::move(m.device)), test_mode(m.test_mode.load()) {
    m.slot = 0;
    m.number = -1;
    m.serial_num = 0;
    m.revision = 0;
    m.major_revision = 0;
    m.minor_revision = 0;
    m.num_channels = 0;
    m.vmaddr = nullptr;
    m.eeprom.clear();
    m.eeprom_format = -1;
    m.module_var_descriptors.clear();
    m.module_vars.clear();
    m.channel_var_descriptors.clear();
    m.channels.clear();
    m.run_task = hw::run::run_task::nop;
    m.control_task = hw::run::control_task::nop;
    m.fifo_buffers = default_fifo_buffers;
    m.fifo_run_wait_usecs = default_fifo_run_wait_usec;
    m.fifo_idle_wait_usecs = default_fifo_idle_wait_usec;
    m.fifo_hold_usecs = default_fifo_hold_usec;
    m.fifo_dma_trigger_level = default_fifo_dma_trigger_level;
    m.fifo_bandwidth = 0;
    m.run_stats.clear();
    m.crate_revision = -1;
    m.board_revision = -1;
    m.reg_trace = false;
    m.bus_cycle_period = 100;
    m.present_ = false;
    m.online_ = false;
    m.forced_offline_ = false;
    m.pause_fifo_worker = true;
    m.comms_fpga = false;
    m.fippi_fpga = false;
    m.have_hardware = false;
    m.vars_loaded = false;
    m.cfg_ctrlcs = 0xaaa;
    m.test_mode = test::off;
}

module::~module() {
    /*
     * Catch an error here and log.
     */
    try {
        close();
    } catch (pixie::error::error& e) {
        xia_log(log::error) << e;
    }
    device.release();
}

module& module::operator=(module&& m) {
    lock_guard guard(lock_);
    lock_guard guard_m(m.lock_);

    if (in_use != 0 || m.in_use != 0) {
        throw error(number, slot, error::code::internal_failure, "cannot move modules when in use");
    }

    slot = m.slot;
    number = m.number;
    serial_num = m.serial_num;
    revision = m.revision;
    major_revision = m.major_revision;
    minor_revision = m.minor_revision;
    num_channels = m.num_channels;
    vmaddr = m.vmaddr;
    eeprom = std::move(m.eeprom);
    eeprom_format = m.eeprom_format;
    module_var_descriptors = std::move(m.module_var_descriptors);
    channel_var_descriptors = std::move(m.channel_var_descriptors);
    module_vars = std::move(m.module_vars);
    channels = std::move(m.channels);
    run_task = m.run_task.load();
    control_task = m.control_task.load();
    fifo_buffers = m.fifo_buffers;
    fifo_run_wait_usecs = m.fifo_run_wait_usecs.load();
    fifo_idle_wait_usecs = m.fifo_idle_wait_usecs.load();
    fifo_hold_usecs = m.fifo_hold_usecs.load();
    fifo_dma_trigger_level = m.fifo_dma_trigger_level.load();
    fifo_bandwidth = m.fifo_bandwidth.load();
    run_stats = m.run_stats;
    crate_revision = m.crate_revision;
    board_revision = m.board_revision;
    reg_trace = m.reg_trace;
    bus_cycle_period = m.reg_trace;
    present_ = m.present_.load();
    online_ = m.online_.load();
    forced_offline_ = m.forced_offline_.load();
    pause_fifo_worker = m.pause_fifo_worker.load();
    comms_fpga = m.comms_fpga;
    fippi_fpga = m.fippi_fpga;
    have_hardware = m.have_hardware;
    vars_loaded = m.vars_loaded;
    cfg_ctrlcs = m.cfg_ctrlcs;
    test_mode = m.test_mode.load();

    device = std::move(m.device);

    m.slot = 0;
    m.number = -1;
    m.serial_num = 0;
    m.revision = 0;
    m.major_revision = 0;
    m.minor_revision = 0;
    m.num_channels = 0;
    m.vmaddr = nullptr;
    m.eeprom.clear();
    m.eeprom_format = -1;
    m.run_task = hw::run::run_task::nop;
    m.control_task = hw::run::control_task::nop;
    m.fifo_buffers = default_fifo_buffers;
    m.fifo_run_wait_usecs = default_fifo_run_wait_usec;
    m.fifo_idle_wait_usecs = default_fifo_idle_wait_usec;
    m.fifo_hold_usecs = default_fifo_hold_usec;
    m.fifo_dma_trigger_level = default_fifo_dma_trigger_level;
    m.fifo_bandwidth = 0;
    m.run_stats.clear();
    m.crate_revision = -1;
    m.board_revision = -1;
    m.reg_trace = false;
    m.bus_cycle_period = 100;
    m.present_ = false;
    m.online_ = false;
    m.forced_offline_ = false;
    m.pause_fifo_worker = true;
    m.comms_fpga = false;
    m.fippi_fpga = false;
    m.have_hardware = false;
    m.vars_loaded = false;
    m.cfg_ctrlcs = 0xaaa;
    m.test_mode = test::off;

    return *this;
}

bool module::present() const {
    return present_.load();
}

bool module::online() const {
    return online_.load() && !forced_offline_.load();
}

void module::open(size_t device_number) {
    xia_log(log::debug) << "module: open: device-number=" << device_number;

    lock_guard guard(lock_);

    if (module_var_descriptors.empty() || channel_var_descriptors.empty()) {
        throw error(number, slot, error::code::internal_failure,
                    "no module or channel variable descriptors");
    }

    if (present()) {
        throw error(number, slot, error::code::module_already_open, "module already open");
    }

    if (device->device_number < 0) {
        PLX_STATUS ps;

        ps = ::PlxPci_DeviceFind(&device->key, uint16_t(device_number));
        if (ps != PLX_STATUS_OK) {
            std::ostringstream oss;
            oss << "PCI find: device: " << device_number << ": " << pci_error_text(ps);
            error::code ec = error::code::module_initialize_failure;
            if (ps == PLX_STATUS_NO_DRIVER || ps == PLX_STATUS_INVALID_OBJECT) {
                ec = error::code::not_supported;
            }
            throw error(number, slot, ec, oss);
        }

        device->device_number = int(device_number);

        ps = ::PlxPci_DeviceOpen(&device->key, &device->handle);
        if (ps != PLX_STATUS_OK) {
            std::ostringstream oss;
            oss << "PCI open: device: " << device_number << ": " << pci_error_text(ps);
            throw error(number, slot, error::code::module_initialize_failure, oss);
        }

        U8 drv_major;
        U8 drv_minor;
        U8 drv_rev;

        ps = ::PlxPci_DriverVersion(&device->handle, &drv_major, &drv_minor, &drv_rev);
        if (ps != PLX_STATUS_OK) {
            std::ostringstream oss;
            oss << "driver version: device: " << device_number << ": " << pci_error_text(ps);
            throw error(number, slot, error::code::module_initialize_failure, oss);
        }

        xia_log(log::info) << "module: PLX: driver: version=" << int(drv_major) << '.' << int(drv_minor)
                           << '.' << int(drv_rev);

        U16 chip_type;
        U8 chip_rev;
        ps = ::PlxPci_ChipTypeGet(&device->handle, &chip_type, &chip_rev);
        if (ps != PLX_STATUS_OK) {
            std::ostringstream oss;
            oss << "get chip type: device: " << device_number << ": " << pci_error_text(ps);
            throw error(number, slot, error::code::module_initialize_failure, oss);
        }

        xia_log(log::info) << "module: PLX: device: type=" << std::hex << chip_type
                           << " rev=" << std::dec << int(chip_rev);

        /*
         * For PLX 9054, Space 0 is at PCI BAR 2.
         */
        ps = PlxPci_PciBarMap(&device->handle, 2, (VOID**) &vmaddr);
        if (ps != PLX_STATUS_OK) {
            std::ostringstream oss;
            oss << "PCI BAR map: device: " << device_number << ": " << pci_error_text(ps);
            throw error(number, slot, error::code::module_initialize_failure, oss);
        }

        xia_log(log::info) << "module: PLX: pci: device-number=" << device_number
                           << " domain=" << device->domain() << " bus=" << device->bus()
                           << " slot=" << device->slot();

        /*
         * DMA channel for block transfers.
         */
        device->dma.ReadyInput = 1;
        device->dma.Burst = 1;
        device->dma.BurstInfinite = 1;
#if PLX_SDK_VERSION_MAJOR < 6
        device->dma.LocalAddrConst = 1;
#else
        device->dma.ConstAddrLocal = 1;
#endif
        device->dma.LocalBusWidth = 2;
        ps = ::PlxPci_DmaChannelOpen(&device->handle, 0, &device->dma);
        if (ps != PLX_STATUS_OK) {
            if (ps == PLX_STATUS_INVALID_ACCESS) {
                throw error(number, slot, error::code::module_already_open,
                            "module in use by another process");
            }
            std::ostringstream oss;
            oss << "PCI DMA: device: " << device_number << ": " << pci_error_text(ps);
            throw error(number, slot, error::code::module_initialize_failure, oss);
        }

        have_hardware = true;

        /*
         * Determine the bus speed.
         */
        calc_bus_speed();

        /*
         * We can only touch specific registers at this early stage of
         * handling a module. We can read the slot and EEPROM and then
         * check if the FPGAs are loaded. If the FPGAs are loaded the DSP
         * can be reset and the FIFO worker started. Do not access any
         * other registers until the FPGAs are loaded.
         */
        const uint32_t i2c_CTRL = 1 << 2;
        const uint32_t i2c_SCL = 1 << 1;
        const uint32_t i2c_SDA = 1 << 0;

        hw::i2c::pcf8574 pio(*this, hw::device::PCF8574, i2c_SDA, i2c_SCL, i2c_CTRL, reg_trace);
        uint8_t pio_value = pio.read_a_byte();

        if (pio_value == 0) {
            have_hardware = false;
            std::ostringstream oss;
            oss << "pio: device: " << device_number << ": invalid value: 0";
            throw error(number, slot, error::code::module_info_failure, oss);
        }

        slot = (pio_value & 0xf8) >> 3;
        crate_revision = pio_value & 0x7;

        const uint32_t cfg_rdver = read_word(0xc);
        board_revision = cfg_rdver & 0xffff;

        xia_log(log::debug) << module_label(*this) << "crate version: " << crate_revision
                            << ", board version: " << std::hex << board_revision;

        hw::i2c::i2cm24c64 i2cm24c64(*this, hw::device::I2CM24C64, i2c_SDA, i2c_SCL, i2c_CTRL);
        i2cm24c64.read(0, hw::eeprom_block_size, eeprom.data);

        if (eeprom.data.size() != hw::eeprom_block_size) {
            have_hardware = false;
            std::ostringstream oss;
            oss << "eeprom read: device: " << device_number
                << ": invalid data length:" << eeprom.data.size();
            throw error(number, slot, error::code::module_info_failure, oss);
        }

        logging::memdump(log::debug, module_label(*this) + "EEPROM:", eeprom.data.data(),
                         eeprom.data.size());

        eeprom.process();

        num_channels = eeprom.num_channels;
        max_channels = eeprom.max_channels;
        serial_num = eeprom.serial_num;
        revision = eeprom.revision;
        major_revision = eeprom.major_revision;
        minor_revision = eeprom.minor_revision;
        eeprom_format = eeprom.format;

        present_ = true;

        erase_values();
        erase_channels();

        fixtures = fixture::make(*this);
        run_config = hw::run::make(*this);

        start_fifo_services();

        fixtures->open();
    }
}

void module::close() {
    lock_guard guard(lock_);

    if (device && device->device_number >= 0) {
        PLX_STATUS ps_dma;
        PLX_STATUS ps_unmap_bar = PLX_STATUS_OK;
        PLX_STATUS ps_close;

        xia_log(log::debug) << module_label(*this) << "close: device-number=" << device->device_number;

        if (fixtures) {
            fixtures->close();
            fixtures.reset();
        }

        force_offline();

        ps_dma = ::PlxPci_DmaChannelClose(&device->handle, 0);
        if (ps_dma != PLX_STATUS_OK) {
            xia_log(log::debug) << module_label(*this) << "DMA close: " << pci_error_text(ps_dma);
            if (ps_dma == PLX_STATUS_IN_PROGRESS) {
                ::PlxPci_DeviceReset(&device->handle);
                ::PlxPci_DmaChannelClose(&device->handle, 0);
            }
        }

        if (vmaddr != nullptr) {
            ps_unmap_bar = ::PlxPci_PciBarUnmap(&device->handle, &vmaddr);
            vmaddr = nullptr;
        }

        ps_close = ::PlxPci_DeviceClose(&device->handle);

        device->device_number = -1;
        online_ = false;
        forced_offline_ = false;
        have_hardware = false;
        present_ = false;

        if (ps_unmap_bar != PLX_STATUS_OK || ps_close != PLX_STATUS_OK) {
            std::ostringstream oss;
            oss << "Pixie PCI ";
            if (ps_unmap_bar != PLX_STATUS_OK) {
                oss << "BAR unmap";
            }
            if (ps_unmap_bar != PLX_STATUS_OK && ps_close != PLX_STATUS_OK) {
                oss << " and ";
            }
            if (ps_close != PLX_STATUS_OK) {
                oss << "close";
            }
            oss << ": device: " << device->device_number << ": " << pci_error_text(ps_unmap_bar)
                << ", " << pci_error_text(ps_close);
            throw error(number, slot, error::code::module_close_failure, oss.str());
        }
    }
}

void module::force_offline() {
    xia_log(log::info) << module_label(*this) << "set offline";
    lock_guard guard(lock_);
    if (present() && !forced_offline_.load()) {
        try {
            if (fixtures) {
                fixtures->forced_offline();
            }
            if (online()) {
                run_end();
            }
        } catch (pixie::error::error& e) {
            xia_log(log::error) << "force offline: " << e;
        }
        stop_fifo_services();
        forced_offline_ = true;
        backplane.offline(*this);
    }
}

template<typename T>
void module::check_channel_num(T num) {
    if (num >= num_channels || (std::is_signed<T>::value && num < 0)) {
        throw error(number, slot, error::code::channel_number_invalid,
                    "channel number out of range");
    }
}

void module::probe() {
    lock_guard guard(lock_);

    if (!present()) {
        throw error(number, slot, error::code::module_offline, "module not open");
    }

    online_ = dsp_online = fippi_fpga = comms_fpga = false;

    load_vars();
    erase_values();
    erase_channels();

    hw::fpga::comms comms(*this);
    comms_fpga = comms.done();

    if (comms_fpga) {
        hw::fpga::fippi fippi(*this);
        fippi_fpga = fippi.done();

        hw::dsp::dsp dsp(*this);
        dsp_online = dsp.init_done();
    }

    xia_log(log::info) << std::boolalpha << module_label(*this) << "probe: sys=" << comms_fpga
                       << " fippi=" << fippi_fpga << " dsp=" << dsp_online;

    if (fippi_fpga) {
        init_values();
        init_channels();
    }

    online_ = comms_fpga && fippi_fpga && dsp_online;

    if (online_) {
        fixtures->online();
    }
}

void module::boot(bool boot_comms, bool boot_fippi, bool boot_dsp) {
    lock_guard guard(lock_);

    if (!present()) {
        throw error(number, slot, error::code::module_offline, "module not open");
    }

    if (forced_offline_.load()) {
        xia_log(log::warning) << "module forced offline";
    }

    if (online()) {
        xia_log(log::warning) << "booting online module";
    }

    online_ = false;

    stop_fifo_services();

    load_vars();

    if (boot_comms) {
        if (comms_fpga) {
            xia_log(log::info) << module_label(*this) << "comms already loaded";
        }
        firmware::firmware_ref fw = get("sys");
        hw::fpga::comms comms(*this);
        comms_fpga = false;
        fw->load();
        comms.boot(fw->data);
        comms_fpga = comms.done();
        if (comms_fpga) {
            fixtures->fgpa_comms_loaded();
        }
    }

    if (boot_fippi) {
        if (fippi_fpga) {
            xia_log(log::info) << module_label(*this) << "fippi already loaded";
        }
        if (!comms_fpga) {
            throw error(number, slot, error::code::module_initialize_failure,
                        "fippi boot needs comms booted");
        }
        firmware::firmware_ref fw = get("fippi");
        hw::fpga::fippi fippi(*this);
        fippi_fpga = false;
        fw->load();
        fippi.boot(fw->data);
        fippi_fpga = fippi.done();
        if (fippi_fpga) {
            fixtures->fgpa_fippi_loaded();
        }
    }

    if (boot_dsp) {
        if (dsp_online) {
            xia_log(log::info) << module_label(*this) << "dsp already running";
        }
        if (!comms_fpga || !fippi_fpga) {
            throw error(number, slot, error::code::module_initialize_failure,
                        "dsp needs comms and fippi booted");
        }
        firmware::firmware_ref fw = get("dsp");
        hw::dsp::dsp dsp(*this);
        dsp_online = false;
        fw->load();
        dsp.boot(fw->data);
        dsp_online = dsp.init_done();
        if (dsp_online) {
            fixtures->dsp_loaded();
        }
    }

    if (fippi_fpga) {
        init_values();
        init_channels();
    }

    start_fifo_services();

    xia_log(log::info) << module_label(*this) << std::boolalpha << "boot: sys-fpga=" << comms_fpga
                       << " fippi-fpga=" << fippi_fpga << " dsp=" << dsp_online;

    online_ = comms_fpga && fippi_fpga && dsp_online;

    if (online_) {
        hw::run::control(*this, hw::run::control_task::program_fippi);
        fixtures->boot();
        fixtures->online();
    }
}

void module::initialize() {
    if (fixtures) {
        fixtures->initialize();
    }
}

void module::add(firmware::module& fw) {
    lock_guard guard(lock_);
    if (online()) {
        xia_log(log::warning) << module_label(*this)
                              << "module already online, do not need to add firmware";
    } else {
        for (auto fp : fw) {
            bool found = false;
            for (auto fm : firmware) {
                if (fm == fp) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                xia_log(log::debug) << module_label(*this) << "add module firmware: " << fp->tag
                                    << " device: " << fp->device;
                firmware.push_back(fp);
            }
        }
    }
}

firmware::firmware_ref module::get(const std::string device) {
    lock_guard guard(lock_);
    /*
     * First check if a slot assigned firmware exists for this
     * device. If not, then see if a default is available.
     */
    for (auto& fwr : firmware) {
        if (fwr->device == device) {
            return fwr;
        }
    }
    if (firmware.empty()) {
        throw error(
            number, slot, error::code::module_invalid_firmware,
            "crate has no firmware");
    }
    std::ostringstream oss;
    oss << "firmware not found: slot=" << slot << ": device=" << device
        << " firmwares=" << firmware.size();
    throw error(number, slot, error::code::module_invalid_firmware, oss.str());
}

param::value_type module::read(const std::string& par) {
    xia_log(log::info) << module_label(*this) << "read: par=" << par;
    return read(param::lookup_module_param(par));
}

param::value_type module::read(param::module_param par) {
    xia_log(log::debug) << module_label(*this) << "read: par=" << int(par);
    online_check();
    const param::module_var var = param::map_module_param(par);
    size_t offset;
    if (var == param::module_var::TrigConfig) {
        offset = size_t(par) - size_t(param::module_param::trigconfig0);
    } else {
        offset = 0;
    }
    return read_var(var, offset);
}

double module::read(const std::string& par, size_t channel) {
    xia_log(log::info) << module_label(*this) << "read: par=" << par;
    return read(param::lookup_channel_param(par), channel);
}

double module::read(param::channel_param par, size_t channel) {
    xia_log(log::debug) << module_label(*this) << "read: par=" << int(par);
    online_check();
    channel_check(channel);
    lock_guard guard(lock_);
    double value;
    switch (par) {
        case param::channel_param::trigger_risetime:
            value = channels[channel].trigger_risetime();
            break;
        case param::channel_param::trigger_flattop:
            value = channels[channel].trigger_flattop();
            break;
        case param::channel_param::trigger_threshold:
            value = channels[channel].trigger_threshold();
            break;
        case param::channel_param::energy_risetime:
            value = channels[channel].energy_risetime();
            break;
        case param::channel_param::energy_flattop:
            value = channels[channel].energy_flattop();
            break;
        case param::channel_param::tau:
            value = channels[channel].tau();
            break;
        case param::channel_param::trace_length:
            value = channels[channel].trace_length();
            break;
        case param::channel_param::trace_delay:
            value = channels[channel].trace_delay();
            break;
        case param::channel_param::voffset:
            value = channels[channel].voffset();
            break;
        case param::channel_param::xdt:
            value = channels[channel].xdt();
            break;
        case param::channel_param::baseline_percent:
            value = channels[channel].baseline_percent();
            break;
        case param::channel_param::emin:
            value = channels[channel].emin();
            break;
        case param::channel_param::binfactor:
            value = channels[channel].binfactor();
            break;
        case param::channel_param::baseline_average:
            value = channels[channel].baseline_average();
            break;
        case param::channel_param::channel_csra:
            value = channels[channel].csra();
            break;
        case param::channel_param::channel_csrb:
            value = channels[channel].csrb();
            break;
        case param::channel_param::blcut:
            value = channels[channel].bl_cut();
            break;
        case param::channel_param::integrator:
            value = channels[channel].integrator();
            break;
        case param::channel_param::fasttrigbacklen:
            value = channels[channel].fast_trig_backlen();
            break;
        case param::channel_param::cfddelay:
            value = channels[channel].cfd_delay();
            break;
        case param::channel_param::cfdscale:
            value = channels[channel].cfd_scale();
            break;
        case param::channel_param::cfdthresh:
            value = channels[channel].cfd_thresh();
            break;
        case param::channel_param::qdclen0:
        case param::channel_param::qdclen1:
        case param::channel_param::qdclen2:
        case param::channel_param::qdclen3:
        case param::channel_param::qdclen4:
        case param::channel_param::qdclen5:
        case param::channel_param::qdclen6:
        case param::channel_param::qdclen7:
            value = channels[channel].qdc_len(par);
            break;
        case param::channel_param::exttrigstretch:
            value = channels[channel].ext_trig_stretch();
            break;
        case param::channel_param::vetostretch:
            value = channels[channel].veto_stretch();
            break;
        case param::channel_param::multiplicitymaskl:
            value = channels[channel].multiplicity_mask_l();
            break;
        case param::channel_param::multiplicitymaskh:
            value = channels[channel].multiplicity_mask_h();
            break;
        case param::channel_param::externdelaylen:
            value = channels[channel].extern_delay_len();
            break;
        case param::channel_param::ftrigoutdelay:
            value = channels[channel].ftrig_out_delay();
            break;
        case param::channel_param::chantrigstretch:
            value = channels[channel].chan_trig_stretch();
            break;
        case param::channel_param::reset_delay:
            value = channels[channel].reset_delay();
            break;
        default:
            throw error(number, slot, error::code::channel_invalid_param, "invalid read parameter");
    }
    return value;
}

bool module::write(const std::string& par, param::value_type value) {
    xia_log(log::info) << module_label(*this) << "write: module par=" << par << " value=" << value;
    return write(param::lookup_module_param(par), value);
}

bool module::write(param::module_param par, param::value_type value) {
    xia_log(log::debug) << module_label(*this) << "write: module par=" << int(par)
                        << " value=" << value;
    online_check();
    std::ostringstream oss;
    size_t offset = 0;
    bool bcast = false;
    lock_guard guard(lock_);
    switch (par) {
        case param::module_param::module_csrb:
            module_csrb(value);
            break;
        case param::module_param::slow_filter_range:
            slow_filter_range(value);
            break;
        case param::module_param::fast_filter_range:
            fast_filter_range(value);
            break;
        case param::module_param::synch_wait:
            backplane.sync_wait(*this, value);
        case param::module_param::in_synch:
        case param::module_param::host_rt_preset:
            bcast = true;
            /* fall through */
        case param::module_param::module_format:
        case param::module_param::max_events:
            write_var(param::map_module_param(par), value, offset);
            break;
            /* fall through */
        case param::module_param::trigconfig0:
        case param::module_param::trigconfig1:
        case param::module_param::trigconfig2:
        case param::module_param::trigconfig3:
            offset = size_t(par) - size_t(param::module_param::trigconfig0);
            /* fall through */
        case param::module_param::module_csra:
        case param::module_param::fasttrigbackplaneena:
        case param::module_param::crateid:
        case param::module_param::slotid:
        case param::module_param::module_id:
            write_var(param::map_module_param(par), value, offset);
            hw::run::control(*this, hw::run::control_task::program_fippi);
            break;
        default:
            oss << "invalid module parameter: " << int(par);
            throw error(number, slot, error::code::module_invalid_param, oss.str());
            break;
    }
    return bcast;
}

void module::write(const std::string& par, size_t channel, double value) {
    xia_log(log::info) << module_label(*this) << "write: par=" << par << " channel=" << channel
                       << " value=" << value;
    write(param::lookup_channel_param(par), channel, value);
}

void module::write(param::channel_param par, size_t channel, double value) {
    xia_log(log::debug) << module_label(*this) << "write: par=" << int(par) << " channel=" << channel
                        << " value=" << value;
    online_check();
    channel_check(channel);
    std::ostringstream oss;
    lock_guard guard(lock_);
    switch (par) {
        case param::channel_param::trigger_risetime:
            channels[channel].trigger_risetime(value);
            break;
        case param::channel_param::trigger_flattop:
            channels[channel].trigger_flattop(value);
            break;
        case param::channel_param::trigger_threshold:
            channels[channel].trigger_threshold(value);
            break;
        case param::channel_param::energy_risetime:
        case param::channel_param::energy_flattop:
            channels[channel].energy_risetime_flattop(par, value);
            break;
        case param::channel_param::tau:
            channels[channel].tau(value);
            break;
        case param::channel_param::trace_length:
            channels[channel].trace_length(value);
            break;
        case param::channel_param::trace_delay:
            channels[channel].trace_delay(value);
            break;
        case param::channel_param::voffset:
            channels[channel].voffset(value);
            set_dacs();
            break;
        case param::channel_param::xdt:
            channels[channel].xdt(value);
            break;
        case param::channel_param::baseline_percent:
            channels[channel].baseline_percent(value);
            break;
        case param::channel_param::emin:
            channels[channel].emin(value);
            break;
        case param::channel_param::binfactor:
            channels[channel].binfactor(value);
            break;
        case param::channel_param::baseline_average:
            channels[channel].baseline_average(value);
            break;
        case param::channel_param::channel_csra:
            channels[channel].csra(value);
            break;
        case param::channel_param::channel_csrb:
            channels[channel].csrb(value);
            break;
        case param::channel_param::blcut:
            channels[channel].bl_cut(value);
            break;
        case param::channel_param::integrator:
            channels[channel].integrator(value);
            break;
        case param::channel_param::fasttrigbacklen:
            channels[channel].fast_trig_backlen(value);
            break;
        case param::channel_param::cfddelay:
            channels[channel].cfd_delay(value);
            break;
        case param::channel_param::cfdscale:
            channels[channel].cfd_scale(value);
            break;
        case param::channel_param::cfdthresh:
            channels[channel].cfd_thresh(value);
            break;
        case param::channel_param::qdclen0:
        case param::channel_param::qdclen1:
        case param::channel_param::qdclen2:
        case param::channel_param::qdclen3:
        case param::channel_param::qdclen4:
        case param::channel_param::qdclen5:
        case param::channel_param::qdclen6:
        case param::channel_param::qdclen7:
            channels[channel].qdc_len(par, value);
            break;
        case param::channel_param::exttrigstretch:
            channels[channel].ext_trig_stretch(value);
            break;
        case param::channel_param::vetostretch:
            channels[channel].veto_stretch(value);
            break;
        case param::channel_param::multiplicitymaskl:
            channels[channel].multiplicity_mask_l(value);
            break;
        case param::channel_param::multiplicitymaskh:
            channels[channel].multiplicity_mask_h(value);
            break;
        case param::channel_param::externdelaylen:
            channels[channel].extern_delay_len(value);
            break;
        case param::channel_param::ftrigoutdelay:
            channels[channel].ftrig_out_delay(value);
            break;
        case param::channel_param::chantrigstretch:
            channels[channel].chan_trig_stretch(value);
            break;
        case param::channel_param::reset_delay:
            channels[channel].reset_delay(value);
            break;
        default:
            break;
    }
}

param::value_type module::read_var(const std::string& var, size_t channel, size_t offset, bool io) {
    xia_log(log::info) << module_label(*this) << "read: var=" << var << " channel=" << channel
                       << " offset=" << offset << " io=" << io;
    try {
        return read_var(param::lookup_module_var(var), offset, io);
    } catch (pixie::error::error& e) {
        if (e.type != error::code::module_invalid_var) {
            throw;
        }
    }
    return read_var(param::lookup_channel_var(var), channel, offset, io);
}

param::value_type module::read_var(param::module_var var, size_t offset, bool io) {
    online_check();
    const size_t index = static_cast<size_t>(var);
    if (index >= module_var_descriptors.size()) {
        std::ostringstream oss;
        oss << "invalid module variable: " << index;
        throw error(number, slot, error::code::module_invalid_param, oss.str());
    }
    const auto& desc = module_var_descriptors[index];
    xia_log(log::debug) << module_label(*this) << "read_var: module var=" << desc.name
                        << " offset=" << offset;
    if (desc.state == param::disable) {
        throw error(number, slot, error::code::module_param_disabled,
                    "module variable disabled: " + desc.name);
    }
    if (desc.mode == param::wr) {
        throw error(number, slot, error::code::module_param_writeonly,
                    "module variable not readable: " + desc.name);
    }
    if (offset >= desc.size) {
        throw error(number, slot, error::code::channel_invalid_param,
                    "invalid module variable offset: " + desc.name);
    }
    param::value_type value;
    {
        lock_guard guard(lock_);
        if (have_hardware && io) {
            hw::memory::dsp dsp(*this);
            hw::word mem = dsp.read(offset, desc.address);
            hw::convert(mem, value);
            module_vars[index].value[offset].value = value;
            module_vars[index].value[offset].dirty = false;
        } else {
            value = module_vars[index].value[offset].value;
        }
    }
    xia_log(log::debug) << module_label(*this) << "read_var: module var=" << desc.name << " value["
                        << offset << "]=" << value << " (0x" << std::hex << value << ')';
    return value;
}

param::value_type module::read_var(param::channel_var var, size_t channel, size_t offset, bool io) {
    online_check();
    channel_check(channel);
    const size_t index = static_cast<size_t>(var);
    if (index >= channel_var_descriptors.size()) {
        std::ostringstream oss;
        oss << "invalid channel variable: " << index;
        throw error(number, slot, error::code::channel_invalid_param, oss.str());
    }
    const auto& desc = channel_var_descriptors[index];
    xia_log(log::debug) << module_label(*this) << "read_var: channel var=" << desc.name
                        << " channel=" << channel << " offset=" << offset << " io=" << io;
    if (desc.state == param::disable) {
        throw error(number, slot, error::code::channel_param_disabled,
                    "channel variable disabled: " + desc.name);
    }
    if (desc.mode == param::wr) {
        throw error(number, slot, error::code::channel_param_writeonly,
                    "channel variable not readable: " + desc.name);
    }
    if (offset >= desc.size) {
        throw error(number, slot, error::code::channel_invalid_param,
                    "invalid channel variable offset: " + desc.name);
    }
    param::value_type value;
    {
        lock_guard guard(lock_);
        if (have_hardware && io) {
            hw::memory::dsp dsp(*this);
            hw::convert(dsp.read(channel, offset, desc.address), value);
            channels[channel].vars[index].value[offset].value = value;
            channels[channel].vars[index].value[offset].dirty = false;
        } else {
            value = channels[channel].vars[index].value[offset].value;
        }
    }
    xia_log(log::debug) << module_label(*this) << "read_var: channel var=" << desc.name << " value["
                        << offset << "]=" << value << " (0x" << std::hex << value << ')';
    return value;
}

void module::write_var(const std::string& var, param::value_type value, size_t channel,
                       size_t offset, bool io) {
    xia_log(log::info) << module_label(*this) << "write: var=" << var << " channel=" << channel
                       << " value[" << offset << "]=" << value << " (0x" << std::hex << value << ')';
    try {
        write_var(param::lookup_module_var(var), value, offset, io);
    } catch (pixie::error::error& e) {
        if (e.type != error::code::module_invalid_var) {
            throw;
        }
        write_var(param::lookup_channel_var(var), value, channel, offset, io);
    }
}

void module::write_var(param::module_var var, param::value_type value, size_t offset, bool io) {
    online_check();
    const size_t index = static_cast<size_t>(var);
    if (index >= module_var_descriptors.size()) {
        std::ostringstream oss;
        oss << "invalid module variable: " << index;
        throw error(number, slot, error::code::module_invalid_param, oss.str());
    }
    const auto& desc = module_var_descriptors[index];
    xia_log(log::debug) << module_label(*this) << "write_var: module var=" << desc.name << " value["
                        << offset << "]=" << value << " (0x" << std::hex << value << ')';
    if (desc.state == param::disable) {
        throw error(number, slot, error::code::module_param_disabled,
                    "module variable disabled: " + desc.name);
    }
    if (desc.mode == param::ro) {
        throw error(number, slot, error::code::module_param_readonly,
                    "module variable not writeable: " + desc.name);
    }
    if (offset >= desc.size) {
        throw error(number, slot, error::code::channel_invalid_param,
                    "invalid module variable offset: " + desc.name);
    }
    lock_guard guard(lock_);
    module_vars[index].value[offset].value = value;
    module_vars[index].value[offset].dirty = true;
    if (have_hardware && io) {
        hw::word word;
        hw::convert(value, word);
        hw::memory::dsp dsp(*this);
        dsp.write(offset, desc.address, word);
        module_vars[index].value[offset].dirty = false;
    }
}

void module::write_var(param::channel_var var, param::value_type value, size_t channel,
                       size_t offset, bool io) {
    online_check();
    channel_check(channel);
    const size_t index = static_cast<size_t>(var);
    if (index >= channel_var_descriptors.size()) {
        std::ostringstream oss;
        oss << "invalid channel variable: " << index;
        throw error(number, slot, error::code::channel_invalid_param, oss.str());
    }
    const auto& desc = channel_var_descriptors[index];
    xia_log(log::debug) << module_label(*this) << "write_var: channel var=" << desc.name
                        << " channel=" << channel << " value[" << offset << "]=" << value << " (0x"
                        << std::hex << value << ')';
    if (desc.state == param::disable) {
        throw error(number, slot, error::code::channel_param_disabled,
                    "channel variable disabled: " + desc.name);
    }
    if (desc.mode == param::ro) {
        throw error(number, slot, error::code::channel_param_readonly,
                    "channel variable not writeable: " + desc.name);
    }
    if (offset >= desc.size) {
        throw error(number, slot, error::code::channel_invalid_param,
                    "invalid channel variable offset: " + desc.name);
    }
    lock_guard guard(lock_);
    channels[channel].vars[index].value[offset].value = value;
    channels[channel].vars[index].value[offset].dirty = true;
    if (have_hardware && io) {
        hw::word word;
        hw::convert(value, word);
        hw::memory::dsp dsp(*this);
        dsp.write(channel, offset, desc.address, word);
        channels[channel].vars[index].value[offset].dirty = false;
    }
}

void module::sync_vars(const sync_var_mode sync_mode) {
    online_check();
    xia_log(log::info) << module_label(*this) << "sync variables: mode: "
                       << (char*) (sync_mode == sync_to_dsp ? "to dsp" : "from dsp");
    if (!have_hardware) {
        return;
    }
    lock_guard guard(lock_);
    hw::memory::dsp dsp(*this);
    for (auto& var : module_vars) {
        const auto& desc = var.var;
        if (desc.state == param::enable && desc.mode != param::ro) {
            for (size_t v = 0; v < var.value.size(); ++v) {
                auto& value = var.value[v];
                if (sync_mode == sync_to_dsp) {
                    if (value.dirty) {
                        hw::word word;
                        hw::convert(value.value, word);
                        dsp.write(v, desc.address, word);
                    }
                } else {
                    hw::convert(dsp.read(v, desc.address), value.value);
                }
                value.dirty = false;
            }
        }
    }
    for (auto& channel : channels) {
        for (auto& var : channel.vars) {
            const auto& desc = var.var;
            if (desc.state == param::enable && desc.mode != param::ro) {
                for (size_t v = 0; v < var.value.size(); ++v) {
                    auto& value = var.value[v];
                    if (sync_mode == sync_to_dsp) {
                        if (value.dirty) {
                            hw::word word;
                            hw::convert(value.value, word);
                            dsp.write(channel.number, v, desc.address, word);
                        }
                    } else {
                        hw::convert(dsp.read(channel.number, v, desc.address), value.value);
                    }
                    value.dirty = false;
                }
            }
        }
    }
    fixtures->sync_vars();
}

void module::sync_hw(const bool program_fippi, const bool program_dacs) {
    online_check();
    xia_log(log::info) << module_label(*this) << std::boolalpha << "sync hardware: "
                       << "program_fippi=" << program_fippi << " program_dacs=" << program_dacs;

    lock_guard guard(lock_);

    if (program_fippi) {
        hw::run::control(*this, hw::run::control_task::program_fippi);
    }

    if (program_dacs) {
        set_dacs();
    }

    /*
     * Update the backplane with the module's current CSRb parameter value.
     */
    backplane_csrb(read(param::module_param::module_csrb));
    sync_csrb();

    if (*this == hw::rev_F) {
        hw::run::control(*this, hw::run::control_task::reset_adc);
    }

    fixtures->sync_hw();
}

void module::run_end() {
    online_check();
    lock_guard guard(lock_);
    bool running = true;
    xia_log(log::info) << module_label(*this) << "run_end: attempting to stop run";
    if (run_task == hw::run::run_task::nop) {
        xia_log(log::warning) << module_label(*this) << "run-end: no run active";
        running = false;
    }
    /*
     * End the run. The DSP will clear the run enable bit, signal the
     * FIPPIs to stop generating events then read any remaining data
     * from the FIPPIs pushing it to the external FIFO. Once all data
     * has been written to the FIFO by the DSP it will clear the
     * active bit signaling the run has ended.
     *
     * Once the DSP has ended the run force the worker to run
     * synchronously to read the remaining records into the data
     * buffers. This is the only place we run the worker synchronously
     * when in asynchronous mode.
     */
    hw::run::end(*this);
    run_interval.end();
    sync_worker_run(true);
    pause_fifo_worker = true;
    run_stats.stop();
    if (running) {
        log_stats("run", run_stats);
    }
}

bool module::run_active() {
    online_check();
    lock_guard guard(lock_);
    return hw::run::active(*this);
}

void module::acquire_baselines() {
    xia_log(log::info) << module_label(*this) << "acquire-baselines";
    online_check();
    lock_guard guard(lock_);
    hw::run::control(*this, hw::run::control_task::get_baselines);
}

void module::adjust_offsets() {
    xia_log(log::info) << module_label(*this) << "adjust-offsets";
    online_check();
    lock_guard guard(lock_);
    hw::run::control(*this, hw::run::control_task::adjust_offsets);
}

void module::tau_finder() {
    xia_log(log::info) << module_label(*this) << "tau-finder";
    online_check();
    lock_guard guard(lock_);
    hw::run::control(*this, hw::run::control_task::tau_finder);
}

void module::get_traces() {
    xia_log(log::info) << module_label(*this) << "get-traces";
    online_check();
    lock_guard guard(lock_);
    hw::run::control(*this, hw::run::control_task::get_traces);
}

void module::set_dacs() {
    xia_log(log::info) << module_label(*this) << "set-dacs";
    online_check();
    lock_guard guard(lock_);
    hw::run::control(*this, hw::run::control_task::set_dacs);
}

void module::start_histograms(hw::run::run_mode mode) {
    xia_log(log::info) << module_label(*this) << "start-histograms: mode=" << int(mode);
    online_check();
    lock_guard guard(lock_);
    if (run_task.load() != hw::run::run_task::nop) {
        throw error(number, slot, error::code::module_invalid_operation,
                    "module already running a task");
    }
    if (test_mode.load() != test::off) {
        throw error(number, slot, error::code::module_test_invalid,
                    "test running; cannot start a run task");
    }
    auto sync_waits = backplane.sync_waits.load();
    if (sync_waits != 0 && sync_waits != backplane.sync_waiters.size()) {
        throw error(number, slot, error::code::module_invalid_operation,
                    "sync wait mode enabled and not all modules in sync wait state");
    }
    backplane.sync_wait_valid();
    run_stats.start();
    hw::run::run(*this, mode, hw::run::run_task::histogram);
    run_interval.restart();
}

void module::start_listmode(hw::run::run_mode mode) {
    xia_log(log::info) << module_label(*this) << "start-list-mode: mode=" << int(mode);
    online_check();
    lock_guard guard(lock_);
    if (run_task != hw::run::run_task::nop) {
        throw error(number, slot, error::code::module_invalid_operation,
                    "module already running a task");
    }
    if (test_mode.load() != test::off) {
        throw error(number, slot, error::code::module_test_invalid,
                    "test running; cannot start a run task");
    }
    backplane.sync_wait_valid();
    run_stats.start();
    fifo_data.flush();
    pause_fifo_worker = false;
    hw::run::run(*this, mode, hw::run::run_task::list_mode);
    run_interval.restart();
}

void module::read_adc(size_t channel, hw::adc_word* buffer, size_t size, bool run) {
    xia_log(log::info) << module_label(*this) << "read-adc: channel=" << channel << " size=" << size
                       << " run=" << std::boolalpha << run;
    online_check();
    lock_guard guard(lock_);
    if (run) {
        get_traces();
    }
    if (control_task != hw::run::control_task::get_traces) {
        throw error(number, slot, error::code::module_invalid_operation,
                    "control task not `get_traces`");
    }
    channel::channel& chan = channels[channel];
    chan.read_adc(buffer, size);
}

void module::read_adc(size_t channel, hw::adc_trace& buffer, bool run) {
    if (buffer.empty()) {
        channel::channel& chan = channels[channel];
        buffer.resize(chan.fixture->config.max_adc_trace_length);
    }
    read_adc(channel, buffer.data(), buffer.size(), run);
}

void module::bl_find_cut(channel::range& channels_, param::values& cuts) {
    xia_log(log::info) << module_label(*this) << "bl-find-cut: channels=" << channels.size();
    cuts.clear();
    channel::baseline bl(*this, channels_);
    lock_guard guard(lock_);
    bl.find_cut();
    cuts = bl.cuts;
}

void module::bl_get(channel::range& channels_, channel::baseline::channels_values& values,
                    bool run) {
    xia_log(log::info) << module_label(*this) << "bl-get: channels=" << channels.size();
    channel::baseline bl(*this, channels_);
    lock_guard guard(lock_);
    if (control_task != hw::run::control_task::get_baselines) {
        throw error(number, slot, error::code::module_invalid_operation,
                    "control task `get_baseline` has not run");
    }
    bl.get(values, run);
}

void module::read_histogram(size_t channel, hw::words& values) {
    xia_log(log::info) << module_label(*this) << "read-histogram: channel=" << channel
                       << " length=" << values.size();
    online_check();
    lock_guard guard(lock_);
    channels[channel].read_histogram(values);
}

void module::read_histogram(size_t channel, hw::word_ptr values, const size_t size) {
    xia_log(log::info) << module_label(*this) << "read-histogram: channel=" << channel
                       << " length=" << size;
    online_check();
    lock_guard guard(lock_);
    channels[channel].read_histogram(values, size);
}

size_t module::read_list_mode_level() {
    xia_log(log::debug) << module_label(*this) << "read-list-mode-level";
    online_check();
    if (!fifo_worker_running.load()) {
        xia_log(log::debug) << module_label(*this) << "read-list-mode-level: FIFO worker not running";
    }
    lock_guard guard(lock_);
    auto size = fifo_data.size();
    if (fifo_run_wait_usecs.load() == 0) {
        hw::memory::fifo fifo(*this);
        size += fifo.level();
    }
    if (size > 0) {
        xia_log(log::debug) << module_label(*this) << "read-list-mode-level: FIFO = " << size;
    }
    return size;
}

size_t module::read_list_mode(hw::words& values) {
    xia_log(log::debug) << module_label(*this) << "read-list-mode: length=" << values.size()
                        << " fifo-size=" << fifo_data.size();
    online_check();
    if (!fifo_worker_running.load()) {
        xia_log(log::warning) << module_label(*this) << "read-list-mode: FIFO worker not running";
    }
    lock_guard guard(lock_);
    sync_worker_run();
    if (fifo_data.empty()) {
        return 0;
    }
    auto out = fifo_data.copy(values);
    run_stats.out += out;
    xia_log(log::debug) << module_label(*this) << "read-list-mode: values=" << values.size()
                        << " out=" << out << " fifo-size=" << fifo_data.size();
    return out;
}

size_t module::read_list_mode(hw::word_ptr values, const size_t size) {
    xia_log(log::info) << module_label(*this) << "read-list-mode: length=" << size
                       << " fifo-size=" << fifo_data.size();
    online_check();
    if (fifo_data.empty()) {
        return 0;
    }
    lock_guard guard(lock_);
    auto out = fifo_data.copy(values, size);
    run_stats.out += size;
    xia_log(log::debug) << module_label(*this) << "read-list-mode: values=" << size
                        << " out=" << out << " fifo-size=" << fifo_data.size();
    return out;
}

void module::read_stats(stats::stats& stats) {
    xia_log(log::info) << module_label(*this) << "read-stats: channels=" << channels.size();
    online_check();
    lock_guard guard(lock_);
    stats::read(*this, stats);
}

void module::read_autotau(hw::doubles& taus) {
    xia_log(log::info) << module_label(*this) << "read-autotau";
    online_check();
    lock_guard guard(lock_);
    taus.resize(num_channels);
    for (auto& chan : channels) {
        taus[chan.number] = chan.autotau();
    }
}

void module::set_fifo_buffers(const size_t buffers) {
    if (buffers < min_fifo_buffers || buffers > max_fifo_buffers) {
        throw error(number, slot, error::code::module_invalid_var,
                    "fifo: buffer value out of range");
    }
    xia_log(log::debug) << module_label(*this) << "fifo: buffers=" << buffers;
    fifo_buffers = buffers;
}

void module::set_fifo_run_wait(const size_t run_wait) {
    if ((run_wait != 0 && run_wait < min_fifo_run_wait_usec) ||
        run_wait > max_fifo_run_wait_usec) {
        throw error(number, slot, error::code::module_invalid_var,
                    "fifo: run wait value out of range");
    }
    xia_log(log::debug) << module_label(*this) << "fifo: run-wait=" << run_wait;
    if (run_wait == 0) {
        xia_log(log::warning) << module_label(*this)
                              << "fifo: setting run-wait to zero is not recommended, it may result in data loss";
        xia_log(log::warning) << module_label(*this)
                              << "fifo: setting run-wait to zero may be deprecated in future versions";
    }
    fifo_run_wait_usecs = run_wait;
}

void module::set_fifo_idle_wait(const size_t idle_wait) {
    if (idle_wait < min_fifo_idle_wait_usec || idle_wait > max_fifo_idle_wait_usec) {
        throw error(number, slot, error::code::module_invalid_var,
                    "fifo: idle wait value out of range");
    }
    xia_log(log::debug) << module_label(*this) << "fifo: idle-wait=" << idle_wait;
    fifo_idle_wait_usecs = idle_wait;
}

void module::set_fifo_hold(const size_t hold) {
    if (hold < min_fifo_hold_usec || hold > max_fifo_hold_usec) {
        throw error(number, slot, error::code::module_invalid_var, "fifo: hold value out of range");
    }
    xia_log(log::debug) << module_label(*this) << "fifo: hold=" << hold;
    fifo_hold_usecs = hold;
}

void module::set_fifo_dma_trigger_level(const size_t dma_trigger_level) {
    if (dma_trigger_level < min_fifo_dma_trigger_level ||
        dma_trigger_level > max_fifo_dma_trigger_level) {
        throw error(number, slot, error::code::module_invalid_var,
                    "fifo: dma trigger level value out of range");
    }
    xia_log(log::debug) << module_label(*this) << "fifo: dma-trigger-level=" << dma_trigger_level;
    fifo_dma_trigger_level = dma_trigger_level;
}

void module::set_fifo_bandwidth(const size_t bandwidth) {
    if (bandwidth > 100) {
        throw error(number, slot, error::code::module_invalid_var,
                    "fifo: bandwidth value out of range");
    }
    xia_log(log::debug) << module_label(*this) << "fifo: bandwidth=" << bandwidth;
    fifo_bandwidth = bandwidth;
}

void module::select_port(const int port) {
    bus_guard guard(*this);
    cfg_ctrlcs &= ~(7 << 19);
    cfg_ctrlcs |= (port & 7) << 19;
    xia_log(log::debug) << module_label(*this) << "write: cfg_ctrlcs=0x" << std::hex << cfg_ctrlcs;
    write_word(hw::device::CFG_CTRLCS, cfg_ctrlcs);
}

void module::output(std::ostream& out) const {
    util::ostream_guard flags(out);
    out << std::boolalpha << "number: " << std::setw(2) << number << " slot: " << std::setw(2)
        << slot << " present:" << present_.load() << " online:" << online_.load()
        << " forced-offline:" << forced_offline_.load() << " serial:" << serial_num
        << " rev:" << revision_label() << " (" << revision << ") vaddr:" << vmaddr
        << " fw: " << firmware.size();
}

std::string module::version_label() const {
    std::ostringstream oss;
    oss << static_cast<char>(revision + 55);
    if (revision_label() > 'F') {
        oss << '-' << static_cast<char>(major_revision) << '(' << minor_revision << ')';
    }
    return oss.str();
}

char module::revision_label() const {
    return static_cast<char>(revision + 55);
}

void module::report(std::ostream& out) const {
    util::ostream_guard flags(out);

    out << std::fixed << std::setprecision(3);

    std::ostringstream title;
    title << "Module " << number << " (slot " << slot << ')';

    out << title.str() << std::endl
        << std::string(title.str().length(), '=') << std::endl
        << std::endl
        << "Serial Number  : " << serial_num << std::endl
        << "Revision       : " << revision_label() << " (" << revision << ')' << std::endl
        << "Major Revision : " << major_revision << std::endl
        << "Minor Revision : " << minor_revision << std::endl
        << "Crate Revision : " << crate_revision << std::endl
        << "Board Revision : " << board_revision << std::endl
        << std::endl;
    if (device) {
        out << "PCI Bus        : " << device->bus() << std::endl
            << "PCI_Slot       : " << device->slot() << std::endl
            << std::endl;
    }
    out << "Num Channels   : " << num_channels << std::endl
        << "Max Channels   : " << max_channels << std::endl
        << std::endl
        << "EEPROM Format  : " << eeprom_format << std::endl
        << std::endl
        << "FIFO Buffers   : " << fifo_buffers << std::endl
        << "FIFO Run wait  : " << fifo_run_wait_usecs << " usecs" << std::endl
        << "FIFO Idle wait : " << fifo_idle_wait_usecs << " usecs" << std::endl
        << "FIFO Hold      : " << fifo_hold_usecs << " usecs" << std::endl
        << "FIFO DMA Trig  : " << fifo_dma_trigger_level << " words" << std::endl
        << "FIFO Bandwidth : ";
    if (fifo_bandwidth == 0) {
        out << "unlimited";
    } else {
        out << fifo_bandwidth << " Mbytes/sec";
    }
    out << std::endl
        << std::endl
        << "Bus cycle      : " << bus_cycle_period << " usecs" << std::endl
        << std::endl;

    if (online()) {
        out << "Address Map" << std::endl << "-----------" << std::endl;
        param_addresses.output(out, true);
        out << std::endl;

        out << "Module Variables" << std::endl << "----------------" << std::endl << std::endl;
        for (auto& var : module_vars) {
            std::ostringstream vartitle;
            vartitle << var.var.name;
            out << vartitle.str() << std::endl
                << std::string(vartitle.str().length(), '~') << std::endl
                << "Mode           : " << param::label(var.var.mode) << std::endl
                << "Access         : " << param::label(var.var.state) << std::endl
                << std::hex << std::setfill('0') << "Address        : 0x" << std::setw(8)
                << var.var.address << std::endl
                << std::dec << std::setfill(' ') << "Size           : " << var.var.size << std::endl
                << "Index          : " << int(var.var.par) << std::endl
                << std::endl;
        }

        out << "Channels" << std::endl << "--------" << std::endl << std::endl;
        for (int ch = 0; ch < static_cast<int>(num_channels); ++ch) {
            const channel::channel& channel = channels[ch];
            std::ostringstream channeltitle;
            channeltitle << "Channel " << channel.number;
            out << channeltitle.str() << std::endl
                << std::string(channeltitle.str().length(), '~') << std::endl;
            channel.report(out);
            if (ch < static_cast<int>(num_channels - 1)) {
                out << std::endl;
            }
        }
    }
}

void module::dma_read(const hw::address source, hw::words& values) {
    dma_read(source, values.data(), values.size());
}

void module::dma_read(const hw::address source, hw::word_ptr values, const size_t size) {
    xia_log(log::debug) << module_label(*this) << "dma read: addr=0x" << std::hex << source
                        << " length=" << std::dec << size;

    online_check();

    if (bus_lock_.try_lock()) {
        bus_lock_.unlock();
        throw error(number, slot, error::code::device_dma_failure, "bus lock not held");
    }

    util::timepoint tp;
    tp.start();

    PLX_DMA_PARAMS dma_params;

    memset(&dma_params, 0, sizeof(PLX_DMA_PARAMS));

#if PLX_SDK_VERSION_MAJOR < 6
    dma_params.u.UserVa = static_cast<PLX_UINT_PTR>(values);
    dma_params.LocalToPciDma = 1;
#else
    dma_params.UserVa = PLX_PTR_TO_INT(values);
    dma_params.Direction = PLX_DMA_LOC_TO_PCI;
#endif
    dma_params.LocalAddr = source;
    dma_params.ByteCount = U32(size * sizeof(hw::words::value_type));

    /*
     * Wait while reading. The call will block until the interrupt happens.
     */
    PLX_STATUS ps = ::PlxPci_DmaTransferUserBuffer(&device->handle, 0, &dma_params, 5 * 1000);
    if (ps != PLX_STATUS_OK) {
        std::ostringstream oss;
        oss << "DMA read: " << pci_error_text(ps);
        throw error(number, slot, error::code::device_dma_failure, oss.str());
    }

    tp.end();

    xia_log(log::debug) << module_label(*this) << "dma read: done, period=" << tp;
}

hw::rev_tag module::get_rev_tag() const {
    return static_cast<hw::rev_tag>(revision);
}

bool module::operator==(const hw::rev_tag rev) const {
    return revision == int(rev);
}

bool module::operator!=(const hw::rev_tag rev) const {
    return revision != int(rev);
}

bool module::operator>=(const hw::rev_tag rev) const {
    return revision >= int(rev);
}

bool module::operator<=(const hw::rev_tag rev) const {
    return revision <= int(rev);
}

bool module::operator<(const hw::rev_tag rev) const {
    return revision < int(rev);
}

bool module::operator>(const hw::rev_tag rev) const {
    return revision > int(rev);
}

void module::online_check() const {
    if (forced_offline_.load()) {
        throw error(number, slot, error::code::module_offline, "module is forced offline");
    }
    if (!online()) {
        throw error(number, slot, error::code::module_offline, "module is not online");
    }
}

void module::channel_check(const size_t channel) const {
    if (channel >= num_channels) {
        throw error(number, slot, error::code::channel_number_invalid,
                    "invalid channel number: " + std::to_string(number));
    }
}

int module::pci_bus() {
    lock_guard guard(lock_);
    if (device) {
        return device->bus();
    }
    return -1;
}

int module::pci_slot() {
    lock_guard guard(lock_);
    if (device) {
        return device->slot();
    }
    return -1;
}

void module::start_test(const test mode) {
    xia_log(log::info) << module_label(*this) << "start-test: mode=" << int(mode);
    online_check();
    lock_guard guard(lock_);
    if (test_mode.load() != test::off) {
        throw error(number, slot, error::code::module_test_invalid, "test already running");
    }
    if (run_task.load() != hw::run::run_task::nop) {
        throw error(number, slot, error::code::module_test_invalid,
                    "module run task active; cannot start a test");
    }
    switch (mode) {
        case test::lm_fifo:
            xia_log(log::debug) << "pause the FIFO worker";
            test_mode = mode;
            run_stats.start();
            hw::run::start(*this, hw::run::run_mode::new_run, hw::run::run_task::nop,
                           hw::run::control_task::fill_ext_fifo);
            xia_log(log::debug) << "unpause the FIFO worker";
            pause_fifo_worker = false;
            break;
        default:
            throw error(number, slot, error::code::invalid_value, "invalid start test mode");
    }
}

void module::end_test() {
    pause_fifo_worker = true;
    test_mode = test::off;
    log_stats("run", run_stats);
}

void module::load_vars() {
    if (!vars_loaded) {
        firmware::firmware_ref vars = get("var");
        vars->load();
        param::load(vars, module_var_descriptors, channel_var_descriptors);
        param_addresses.set(max_channels, module_var_descriptors, channel_var_descriptors);
        vars_loaded = true;
        xia_log(log::info) << module_label(*this) << "address map: " << param_addresses;
    }
}

void module::erase_values() {
    if (fixtures) {
        fixtures->erase_values();
    }
    module_vars.clear();
}

void module::erase_channels() {
    if (fixtures) {
        fixtures->erase_channels();
    }
    channels.clear();
}

void module::init_values() {
    if (num_channels == 0) {
        throw error(number, slot, error::code::internal_failure, "number of channels is 0");
    }
    if (eeprom.configs.size() != num_channels) {
        throw error(number, slot, error::code::internal_failure,
                    "invalid number of channels configurations");
    }
    erase_values();
    for (const auto& desc : module_var_descriptors) {
        module_vars.push_back(param::module_variable(desc));
    }
    if (fixtures) {
        fixtures->init_values();
    }
}

void module::init_channels() {
    if (num_channels == 0) {
        throw error(number, slot, error::code::internal_failure, "number of channels is 0");
    }
    if (eeprom.configs.size() != num_channels) {
        throw error(number, slot, error::code::internal_failure,
                    "invalid number of channels configurations");
    }
    erase_channels();
    channels.resize(num_channels, channel::channel(*this));
    for (size_t channel = 0; channel < num_channels; ++channel) {
        channels[channel].number = channel;
        for (const auto& desc : channel_var_descriptors) {
            channels[channel].vars.push_back(param::channel_variable(desc));
        }
    }
    if (fixtures) {
        fixtures->init_channels();
    }
}

void module::module_csrb(param::value_type value, size_t offset, bool io) {
    backplane_csrb(value);
    write_var(param::module_var::ModCSRB, value, offset, io);
    if (io) {
        hw::run::control(*this, hw::run::control_task::program_fippi);
        sync_csrb();
    }
}

void module::slow_filter_range(param::value_type value, size_t offset, bool io) {
    if (value < hw::limit::SLOWFILTERRANGE_MIN) {
        std::stringstream oss;
        oss << "slow filter value below min: " << value;
        throw error(number, slot, error::code::module_invalid_param, oss.str());
    }
    if (value > hw::limit::SLOWFILTERRANGE_MAX) {
        std::stringstream oss;
        oss << "slow filter value above max: " << value;
        throw error(number, slot, error::code::module_invalid_param, oss.str());
    }

    write_var(param::module_var::SlowFilterRange, value, offset, io);

    if (io) {
        value = 1 << read_var(param::module_var::FastFilterRange, 0, false);
        for (size_t channel = 0; channel < num_channels; ++channel) {
            param::value_type paf_length =
                read_var(param::channel_var::PAFlength, channel, 0, false);
            param::value_type trigger_delay =
                read_var(param::channel_var::TriggerDelay, channel, 0, false);
            channels[channel].update_fifo(paf_length - (trigger_delay / value));
        }

        hw::run::control(*this, hw::run::control_task::program_fippi);

        channel::range chans(num_channels);
        channel::range_set(chans);
        channel::baseline bl(*this, chans);
        bl.find_cut();
    }
}

void module::fast_filter_range(param::value_type value, size_t offset, bool io) {
    if (value > hw::limit::FASTFILTERRANGE_MAX) {
        value = hw::limit::FASTFILTERRANGE_MAX;
        xia_log(log::warning) << "setting FAST_FILTER_RANGE to max: " << hw::limit::FASTFILTERRANGE_MAX;
    }

    if (int(value) < int(hw::limit::FASTFILTERRANGE_MIN)) {
        value = hw::limit::FASTFILTERRANGE_MIN;
        xia_log(log::warning) << "setting FAST_FILTER_RANGE to min: " << hw::limit::FASTFILTERRANGE_MIN;
    }

    write_var(param::module_var::FastFilterRange, value, offset, io);

    if (io) {
        param::value_type last_ffr = 1 << read_var(param::module_var::FastFilterRange, 0, false);

        for (size_t channel = 0; channel < num_channels; ++channel) {
            param::value_type paf_length =
                read_var(param::channel_var::PAFlength, channel, 0, false);
            param::value_type trigger_delay =
                read_var(param::channel_var::TriggerDelay, channel, 0, false);
            channels[channel].update_fifo(paf_length - (trigger_delay / last_ffr));
        }

        hw::run::control(*this, hw::run::control_task::program_fippi);
    }
}

void module::sync_csrb() {
    bus_guard guard(*this);

    /*
     * Set pullups for the trigger lines on the backplane
     *
     * Also enable connections of PXI nearest neighbor lines (J2) onto
     * the backplane if the module is a Rev-B or C module
     */
    if (backplane.wired_or_triggers_pullup == *this) {
        cfg_ctrlcs |= (1 << hw::bit::CPLDCSR_PULLUP);
    } else {
        cfg_ctrlcs &= ~(1 << hw::bit::CPLDCSR_PULLUP);
    }

    /*
     * Enable connections of PXI nearest neighbor lines (J2) onto the
     * backplane if the module is a rev B or C module
     */
    if (*this == hw::rev_B || *this == hw::rev_C) {
        cfg_ctrlcs |= (1 << hw::bit::CPLDCSR_BPCONNECT);
    }

    write_word(hw::device::CFG_CTRLCS, cfg_ctrlcs);

    /*
     * Set pullups for the SYNCH lines on the backplane
     */
    hw::word csr = hw::csr::read(*this);

    if (backplane.run == *this) {
        csr |= (1 << hw::bit::PULLUP_CTRL);
    } else {
        csr &= ~(1 << hw::bit::PULLUP_CTRL);
    }

    hw::csr::write(*this, csr);
}

void module::backplane_csrb(const param::value_type csrb) {
    /*
     * Wired-or trigger pullup leader
     */
    if ((csrb & (1 << hw::bit::MODCSRB_CPLDPULLUP)) != 0) {
        if (!backplane.wired_or_triggers_pullup.request(*this)) {
            throw error(number, slot, error::code::module_invalid_param,
                        "wired-or trigger pullups leader role already taken: " +
                            std::to_string(backplane.wired_or_triggers_pullup.module()));
        }
    } else {
        backplane.wired_or_triggers_pullup.release(*this);
    }
    /*
     * Run leader
     */
    if ((csrb & (1 << hw::bit::MODCSRB_CHASSISMASTER)) != 0) {
        if (!backplane.run.request(*this)) {
            throw error(number, slot, error::code::module_invalid_param,
                        "run leader role already taken: " + std::to_string(backplane.run.module()));
        }
    } else {
        backplane.run.release(*this);
    }
    /*
     * Director leader
     */
    if ((csrb & (1 << hw::bit::MODCSRB_DIRMOD)) != 0) {
        if (!backplane.director.request(*this)) {
            throw error(number, slot, error::code::module_invalid_param,
                        "director leader role already taken: " +
                            std::to_string(backplane.director.module()));
        }
    } else {
        backplane.director.release(*this);
    }
}

void module::start_fifo_services() {
    hw::fpga::comms comms(*this);
    if (comms.done()) {
        hw::fpga::fippi fippi(*this);
        if (fippi.done()) {
            hw::csr::reset(*this);
            if (!fifo_pool.valid()) {
                fifo_pool.create(fifo_buffers, 64 * 1024);
                start_fifo_worker();
                hw::run::end(*this);
            }
        }
    }
}

void module::stop_fifo_services() {
    stop_fifo_worker();
    fifo_data.flush();
    fifo_pool.destroy();
}

void module::start_fifo_worker() {
    xia_log(log::debug) << module_label(*this) << std::boolalpha
                        << "FIFO worker: starting: running=" << fifo_worker_running.load();
    if (!fifo_worker_running.load()) {
        pause_fifo_worker = true;
        fifo_worker_finished = false;
        fifo_worker_running = true;
        fifo_thread = std::thread(&module::fifo_worker, this);
    }
}

void module::stop_fifo_worker() {
    xia_log(log::debug) << module_label(*this) << std::boolalpha
                        << "FIFO worker: stopping: running=" << fifo_worker_running.load();
    fifo_worker_running = false;
    {
        sync::variable::lock_guard guard(fifo_worker_working);
        fifo_worker_req.notify();
    }
    if (fifo_thread.joinable()) {
        fifo_thread.join();
    }
}

void module::fifo_worker() {
    hw::memory::fifo fifo(*this);

    size_t level = fifo.level();

    xia_log(log::info) << module_label(*this) << "FIFO worker: running, level=" << level;

    /*
     * The worker must not hold the module's lock. That lock is for
     * front facing user calls only. The worker can only use atomics
     * or other other locks, ie the buffer pool and FIFO data queue.
     */
    try {
        size_t wait_time = fifo_run_wait_usecs.load();
        size_t hold_time = 0;

        bool requester_waiting = false;

        bool pool_empty_logged = false;
        bool fifo_full_logged = false;
        size_t last_dma_in = 0;

        int requested_wait_loops = 0;

        sync::variable::lock_guard guard(fifo_worker_working);

        while (fifo_worker_running.load()) {
            if (!online()) {
                if (!fifo_worker_req.wait(100 * 1000)) {
                    fifo_worker_resp.notify();
                }
                continue;
            }

            hw::run::run_task this_run_tsk = run_task.load();

            /*
             * If the run wait time is set to the 0 the worker is
             * synchronous to the user's calls and there is no wait
             * time.
             *
             * If list mode is running the wait period is the currently
             * configured run period.
             *
             * If fill ext FIFO mode is running check the FIFO level
             * and refill when we drop below a threadhold.
             *
             * Any other mode will decay the wait period every hold
             * period until capped at the idle wait period.
             */
            auto run_wait = fifo_run_wait_usecs.load();
            /*
             * If the run wait time is 0 the user has requested we
             * operate in a fully synchronous mode.
             */
            bool mode_asynchronous = run_wait != 0;

            if (mode_asynchronous) {
                if (this_run_tsk == hw::run::run_task::list_mode) {
                    wait_time = run_wait;
                }
                if (test_mode.load() != test::off) {
                    wait_time = run_wait;
                } else {
                    const size_t idle_wait_time = fifo_idle_wait_usecs.load();
                    /*
                     * WHen the wait time is equal to the idle period nothing
                     * further happens. The hold time should also be 0 which
                     * means any new data will be held for the hold period.
                     */
                    if (wait_time > idle_wait_time) {
                        wait_time = idle_wait_time;
                    } else if (wait_time < idle_wait_time &&
                               hold_time >= fifo_hold_usecs.load()) {
                        wait_time <<= 1;
                        if (wait_time > idle_wait_time) {
                            wait_time = idle_wait_time;
                        }
                        hold_time = 0;
                    }
                }
            } else {
                /*
                 * The level is read only once when the mode is
                 * synchronous.
                 */
                level = fifo.level();
                xia_log(log::debug) << "fifo worker: fifo-level = " << level;
            }

            /*
             * Loop while there is data to read. Read the FIFO if the level
             * is more than the DMA block size or it has sat in the FIFO
             * for the hold time. The hold time is reset on every transfer
             * from the FIFO no matter the run task state. If we are idle
             * and reading data the hold time is reset so the wait time
             * only starts to decay once we do not see data.
             */
            while (fifo_worker_running.load() && !pause_fifo_worker.load()) {
                /*
                 * See if the task is still running? If not the module may
                 * have been directed to stop running by another module.
                 */
                this_run_tsk = run_task.load();
                if (this_run_tsk != hw::run::run_task::nop &&
                    this_run_tsk != hw::run::run_task::run_stopping && !hw::run::active(*this)) {
                    run_task = hw::run::run_task::nop;
                    xia_log(log::info) << module_label(*this) << "FIFO worker: run not active";
                }
                /*
                 * Read the level of the FIFO every loop when the mode
                 * is asynchronous.
                 */
                if (mode_asynchronous) {
                    level = fifo.level();
                    xia_log(log::debug) << "fifo worker: fifo-level = " << level;
                }
                if (level >= hw::fifo_size_words) {
                    if (!fifo_full_logged) {
                        fifo_full_logged = true;
                        xia_log(log::warning) << module_label(*this) << "FIFO worker: FIFO full";
                    }
                    run_stats.hw_overflows++;
                }
                /*
                 * Nothing further to do if:
                 *   - the FIFO level is 0
                 *  or
                 *   - the mode is asychronous AND
                 *   - there is no requester waiting AND
                 *   - we have not reached the wait hold time AND
                 *   - the FIFO level has not reached the trigger level
                 */
                if (level == 0 ||
                    (mode_asynchronous && !requester_waiting &&
                     hold_time < fifo_hold_usecs.load() &&
                     level < fifo_dma_trigger_level.load())) {
                    break;
                }
                if (level == std::numeric_limits<hw::word>::max()) {
                    auto level2 = fifo.level();
                    xia_log(log::debug) << module_label(*this) << "invalid FIFO level: " << level
                                        << " repeat read: " << level2;
                    break;
                }
                /*
                 * We do not bother compacting if we are less than 3
                 */
                const size_t fifo_pool_count = fifo_pool.count();
                if (fifo_pool_count < 4 && fifo_pool_count > 1) {
                    if (!pool_empty_logged) {
                        xia_log(log::warning) << module_label(*this) << "FIFO worker: pool empty,"
                                              << " compacting queue ...";
                    }
                    fifo_data.compact();
                }
                /*
                 * Queue the buffer if there is more than one
                 * remaining in the pool and the worker has not been
                 * paused.
                 */
                bool queue_buf = fifo_pool_count > 1 && !pause_fifo_worker.load();
                /*
                 * If the pool is still empty after compacting wait
                 * letting the FIFO fill.
                 */
                if (!fifo_pool.empty()) {
                    buffer::handle buf = fifo_pool.request();
                    auto read_words = level;
                    if (read_words > buf->capacity()) {
                        read_words = buf->capacity();
                    }
                    buf->resize(read_words);
                    fifo.read(*buf, read_words);
                    run_stats.dma_in += read_words;
                    if (queue_buf) {
                        run_stats.in += read_words;
                        fifo_data.push(buf);
                    } else {
                        run_stats.dropped += read_words;
                        xia_log(log::debug) << module_label(*this)
                                            << std::boolalpha
                                            << "buffer drop: fifo_pool_count="
                                            << (fifo_pool_count > 1)
                                            << " fifo-worker-paused="
                                            << pause_fifo_worker.load();
                    }
                    /*
                     * If the logging level is `debug` compute the CRC32 of the data
                     * queued. This can be used to verify the data received by the
                     * user API.
                     */
                    util::crc32 crc;
                    if (logging::level_logging(log::debug)) {
                        crc.update(*buf);
                    }
                    xia_log(log::debug) << module_label(*this)
                                        << "FIFO read, level=" << level
                                        << " read-words=" << read_words
                                        << " data-fifo-words=" << fifo_data.count()
                                        << " crc=0x" << std::hex << crc.value
                                        << std::boolalpha << " queue-buf=" << queue_buf;
                    hold_time = 0;
                    pool_empty_logged = false;
                    fifo_full_logged = false;
                    /*
                     * Update the level for synchronous mode.
                     */
                    level -= read_words;
                } else {
                    if (!pool_empty_logged) {
                        xia_log(log::warning) << module_label(*this)
                                              << "FIFO worker: pool empty";
                        pool_empty_logged = true;
                    }
                    break;
                }

                /*
                 * Bandwidth of run FIFO performance stats.
                 */
                if (run_stats.update_bandwidth()) {
                     xia_log(log::debug) << module_label(*this)
                                         << "FIFO stats:  run: " << run_stats.output();
                }

                if (mode_asynchronous) {
                    /*
                     * Allocated bandwidth used?
                     */
                    auto bandwidth = fifo_bandwidth.load();
                    if (bandwidth > 0) {
                        auto data_in_bw = run_stats.bandwidth.load();
                        if (data_in_bw >= bandwidth) {
                            size_t slice =
                                (100 - ((100 * bandwidth) / hw::pci_bus_datarate)) * 100;
                            if (wait_time < slice) {
                                wait_time = slice;
                            }
                            xia_log(log::debug) << "BW limiter: data in:: " << data_in_bw << "MB";
                            break;
                        }
                    }
                }
            }

            /*
             * Wait for a request to run. If run has been requested
             * respond so the requester is notified the work has been
             * completed. If the request time's out it is a normal
             * run.
             */
            if (requester_waiting) {
                if (requested_wait_loops == 0) {
                    xia_log(log::debug) << module_label(*this) << "FIFO worker: respond to request";
                    requester_waiting = false;
                    fifo_worker_resp.notify();
                } else {
                    --requested_wait_loops;
                }
            }

            if (!fifo_worker_req.wait(mode_asynchronous ? wait_time : 0)) {
                xia_log(log::debug) << module_label(*this) << "FIFO worker: run requested";
                requester_waiting = true;
                if (mode_asynchronous) {
                    requested_wait_loops = 5;
                } else {
                    requested_wait_loops = 0;
                }
            }

            /*
             * Bound the lower timeout so this thread does not spin.
             */
            if (hold_time < fifo_hold_usecs.load()) {
                hold_time += wait_time;
            }
        }

        /*
         * Flush the buffers from the queue back into the pool. Any user
         * active calls should be done in a few seconds.
         */
        fifo_data.flush();
        size_t wait_period = 5 * 1000 / 10;
        while (wait_period-- > 0) {
            if (fifo_pool.full()) {
                break;
            }
            hw::wait(10000);
        }
    } catch (pixie::error::error& e) {
        xia_log(log::error) << "FIFO worker: " << e;
    } catch (std::exception& e) {
        xia_log(log::error) << "FIFO worker: error: " << e.what();
    } catch (...) {
        xia_log(log::error) << "FIFO worker: unhandled exception";
    }

    level = fifo.level();
    xia_log(log::info) << module_label(*this) << "FIFO worker: finishing, level=" << level;

    fifo_worker_running = false;
    fifo_worker_finished = true;
}


void module::calc_bus_speed() {
    const size_t count = 5000;
    util::timepoint tp;
    {
        module::module::bus_guard guard(*this);
        size_t polls = count;
        tp.start();
        while (polls-- != 0) {
            volatile uint32_t tmp = read_word(hw::device::PCF8574);
            (void) tmp;
        }
        tp.stop();
    }
    double usecs = static_cast<double>(tp.usecs());
    bus_cycle_period = usecs / count;
    xia_log(log::debug) << "module: PCI bus-cycle-speed=" << bus_cycle_period
                        << "usec sample-period=" << usecs << "usec";
}

void module::log_stats(const char* label, const fifo_stats& stats) {
    xia_log(log::info) << module_label(*this) << label << ": " << stats.output();
}

bool module::fifo_worker_run(size_t timeout_usecs) {
    sync::variable::lock_guard guard(fifo_worker_working);
    fifo_worker_req.notify();
    return fifo_worker_resp.wait(timeout_usecs);
}

void module::sync_worker_run(bool forced) {
    /*
     * Run the worker if forced or the mode is synchronous.
     */
    if (forced || fifo_run_wait_usecs.load() == 0) {
        fifo_worker_run(250 * 1000);
    }
}

void assign(modules& modules_, const number_slots& numbers) {
    std::ostringstream oss;
    for (auto& number_slot : numbers) {
        oss << number_slot.second << "->" << number_slot.first << ' ';
    }
    xia_log(log::info) << "assign slot map: " << oss.str();
    for (auto& mod : modules_) {
        mod->number = -1;
    }
    for (auto& mod : modules_) {
        for (auto number_slot : numbers) {
            if (mod->slot == number_slot.second) {
                mod->number = number_slot.first;
                break;
            }
        }
    }
    for (auto& mod : modules_) {
        if (mod->number == -1) {
            xia_log(log::warning) << "module not found in slot map: " << mod->slot;
        }
    }
}

void order_by_number(modules& mods) {
    std::sort(mods.begin(), mods.end(), [](module_ptr& a, module_ptr& b) {
        return a->number != -1 && b->number != -1 && a->number < b->number;
    });
}

void order_by_slot(modules& mods) {
    std::sort(mods.begin(), mods.end(),
              [](module_ptr& a, module_ptr& b) { return a->slot < b->slot; });
}

void set_number_by_slot(modules& mods) {
    order_by_slot(mods);
    int number = 0;
    for (auto& mod : mods) {
        mod->number = number;
        number++;
    }
}
};  // namespace module
};  // namespace pixie
};  // namespace xia

std::ostream& operator<<(std::ostream& out, const xia::pixie::module::module& module) {
    module.output(out);
    return out;
}
