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

module::module()
    : slot(0), number(-1), serial_num(0), revision(0), num_channels(0), vmaddr(nullptr),
      eeprom_format(-1), run_task(hw::run::run_task::nop), control_task(hw::run::control_task::nop),
      fifo_buffers(default_fifo_buffers), fifo_run_wait_usecs(default_fifo_run_wait_usec),
      fifo_idle_wait_usecs(default_fifo_idle_wait_usec), fifo_hold_usecs(default_fifo_hold_usec),
      fifo_bandwidth(0), data_dma_in(0), data_fifo_in(0), data_fifo_out(0), data_fifo_run_in(0),
      data_fifo_run_out(0), crate_revision(-1), board_revision(-1), reg_trace(false),
      fifo_worker_running(false), fifo_worker_finished(false), in_use(0), present_(false),
      online_(false), forced_offline_(false), pause_fifo_worker(true), comms_fpga(false),
      fippi_fpga(false), have_hardware(false), vars_loaded(false),
      device(std::make_unique<pci_bus_handle>()), test_mode(test::off) {}

module::module(module&& m)
    : slot(m.slot), number(m.number), serial_num(m.serial_num), revision(m.revision),
      num_channels(m.num_channels), vmaddr(m.vmaddr), configs(m.configs), eeprom(m.eeprom),
      eeprom_format(m.eeprom_format), module_var_descriptors(std::move(m.module_var_descriptors)),
      module_vars(std::move(m.module_vars)),
      channel_var_descriptors(std::move(m.channel_var_descriptors)),
      channels(std::move(m.channels)), firmware(std::move(m.firmware)), run_task(m.run_task.load()),
      control_task(m.control_task.load()), fifo_buffers(m.fifo_buffers),
      fifo_run_wait_usecs(m.fifo_run_wait_usecs.load()),
      fifo_idle_wait_usecs(m.fifo_idle_wait_usecs.load()),
      fifo_hold_usecs(m.fifo_hold_usecs.load()), fifo_bandwidth(m.fifo_bandwidth.load()),
      data_dma_in(m.data_dma_in.load()), data_fifo_in(m.data_fifo_in.load()),
      data_fifo_out(m.data_fifo_out.load()), data_fifo_run_in(m.data_fifo_run_in.load()),
      data_fifo_run_out(m.data_fifo_run_out.load()), crate_revision(m.crate_revision),
      board_revision(m.board_revision), reg_trace(m.reg_trace), fifo_worker_running(false),
      fifo_worker_finished(false), in_use(0), present_(m.present_.load()),
      online_(m.online_.load()), forced_offline_(m.forced_offline_.load()),
      pause_fifo_worker(m.pause_fifo_worker.load()), comms_fpga(m.comms_fpga),
      fippi_fpga(m.fippi_fpga), have_hardware(false), vars_loaded(false),
      device(std::move(m.device)), test_mode(m.test_mode.load()) {
    m.slot = 0;
    m.number = -1;
    m.serial_num = 0;
    m.revision = 0;
    m.num_channels = 0;
    m.vmaddr = nullptr;
    m.configs.clear();
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
    m.fifo_bandwidth = 0;
    m.data_dma_in = 0;
    m.data_fifo_in = 0;
    m.data_fifo_out = 0;
    m.data_fifo_run_in = 0;
    m.data_fifo_run_out = 0;
    m.crate_revision = -1;
    m.board_revision = -1;
    m.reg_trace = false;
    m.present_ = false;
    m.online_ = false;
    m.forced_offline_ = false;
    m.pause_fifo_worker = true;
    m.comms_fpga = false;
    m.fippi_fpga = false;
    m.have_hardware = false;
    m.vars_loaded = false;
    m.test_mode = test::off;
}

module::~module() {
    /*
     * Catch an error here and log.
     */
    try {
        close();
    } catch (error& e) {
        log(log::error) << e;
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
    num_channels = m.num_channels;
    vmaddr = m.vmaddr;
    configs = m.configs;
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
    fifo_bandwidth = m.fifo_bandwidth.load();
    data_dma_in = m.data_dma_in.load();
    data_fifo_in = m.data_fifo_in.load();
    data_fifo_out = m.data_fifo_out.load();
    data_fifo_run_in = m.data_fifo_run_in.load();
    data_fifo_run_out = m.data_fifo_run_out.load();
    crate_revision = m.crate_revision;
    board_revision = m.board_revision;
    reg_trace = m.reg_trace;
    present_ = m.present_.load();
    online_ = m.online_.load();
    forced_offline_ = m.forced_offline_.load();
    pause_fifo_worker = m.pause_fifo_worker.load();
    comms_fpga = m.comms_fpga;
    fippi_fpga = m.fippi_fpga;
    have_hardware = m.have_hardware;
    vars_loaded = m.vars_loaded;
    test_mode = m.test_mode.load();

    device = std::move(m.device);

    m.slot = 0;
    m.number = -1;
    m.serial_num = 0;
    m.revision = 0;
    m.num_channels = 0;
    m.vmaddr = nullptr;
    m.configs.clear();
    m.eeprom.clear();
    m.eeprom_format = -1;
    m.run_task = hw::run::run_task::nop;
    m.control_task = hw::run::control_task::nop;
    m.fifo_buffers = default_fifo_buffers;
    m.fifo_run_wait_usecs = default_fifo_run_wait_usec;
    m.fifo_idle_wait_usecs = default_fifo_idle_wait_usec;
    m.fifo_hold_usecs = default_fifo_hold_usec;
    m.fifo_bandwidth = 0;
    m.data_dma_in = 0;
    m.data_fifo_in = 0;
    m.data_fifo_out = 0;
    m.data_fifo_run_in = 0;
    m.data_fifo_run_out = 0;
    m.crate_revision = -1;
    m.board_revision = -1;
    m.reg_trace = false;
    m.present_ = false;
    m.online_ = false;
    m.forced_offline_ = false;
    m.pause_fifo_worker = true;
    m.comms_fpga = false;
    m.fippi_fpga = false;
    m.have_hardware = false;
    m.vars_loaded = false;
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
    log(log::debug) << "module: open: device-number=" << device_number;

    lock_guard guard(lock_);

    if (module_var_descriptors.empty() || channel_var_descriptors.empty()) {
        throw error(number, slot, error::code::internal_failure,
                    "no module or channel variable descriptors");
    }

    if (online()) {
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

        log(log::info) << "module: PLX: driver: version=" << int(drv_major) << '.' << int(drv_minor)
                       << '.' << int(drv_rev);

        U16 chip_type;
        U8 chip_rev;
        ps = ::PlxPci_ChipTypeGet(&device->handle, &chip_type, &chip_rev);
        if (ps != PLX_STATUS_OK) {
            std::ostringstream oss;
            oss << "get chip type: device: " << device_number << ": " << pci_error_text(ps);
            throw error(number, slot, error::code::module_initialize_failure, oss);
        }

        log(log::info) << "module: PLX: device: type=" << std::hex << chip_type
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

        log(log::info) << "module: PLX: pci: device-number=" << device_number
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

        log(log::debug) << module_label(*this) << "crate version: " << crate_revision
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
        configs = eeprom.configs;

        present_ = true;

        start_fifo_services();
    }
}

void module::close() {
    lock_guard guard(lock_);

    if (device && device->device_number >= 0) {
        PLX_STATUS ps_dma;
        PLX_STATUS ps_unmap_bar = PLX_STATUS_OK;
        PLX_STATUS ps_close;

        log(log::debug) << module_label(*this) << "close: device-number=" << device->device_number;

        stop_fifo_services();

        ps_dma = ::PlxPci_DmaChannelClose(&device->handle, 0);
        if (ps_dma != PLX_STATUS_OK) {
            log(log::debug) << module_label(*this) << "DMA close: " << pci_error_text(ps_dma);
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
    log(log::info) << module_label(*this) << "set offline";
    lock_guard guard(lock_);
    if (!forced_offline_.load()) {
        stop_fifo_services();
        forced_offline_ = true;
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

    online_ = dsp_online = fippi_fpga = comms_fpga = false;

    load_vars();

    erase_values();

    hw::fpga::comms comms(*this);
    comms_fpga = comms.done();

    if (comms_fpga) {
        hw::fpga::fippi fippi(*this);
        fippi_fpga = fippi.done();

        if (fippi_fpga) {
            hw::dsp::dsp dsp(*this);
            dsp_online = dsp.init_done();
        }
    }

    log(log::info) << std::boolalpha << module_label(*this) << "probe: sys=" << comms_fpga
                   << " fippi=" << fippi_fpga << " dsp=" << dsp_online;

    if (fippi_fpga) {
        init_values();
    }

    online_ = comms_fpga && fippi_fpga && dsp_online;
}

void module::boot(bool boot_comms, bool boot_fippi, bool boot_dsp) {
    lock_guard guard(lock_);

    if (forced_offline_.load()) {
        log(log::warning) << "module forced offline";
    }

    if (online()) {
        log(log::warning) << "booting online module";
    }

    online_ = false;

    stop_fifo_services();

    load_vars();

    if (boot_comms) {
        if (comms_fpga) {
            log(log::info) << module_label(*this) << "comms already loaded";
        }
        firmware::firmware_ref fw = get("sys");
        hw::fpga::comms comms(*this);
        comms_fpga = false;
        comms.boot(fw->data);
        comms_fpga = comms.done();
    }

    if (boot_fippi) {
        if (fippi_fpga) {
            log(log::info) << module_label(*this) << "fippi already loaded";
        }
        if (!comms_fpga) {
            throw error(number, slot, error::code::module_initialize_failure,
                        "fippi boot needs comms booted");
        }
        firmware::firmware_ref fw = get("fippi");
        hw::fpga::fippi fippi(*this);
        fippi_fpga = false;
        fippi.boot(fw->data);
        fippi_fpga = fippi.done();
    }

    if (boot_dsp) {
        if (dsp_online) {
            log(log::info) << module_label(*this) << "dsp already running";
        }
        if (!comms_fpga || !fippi_fpga) {
            throw error(number, slot, error::code::module_initialize_failure,
                        "dsp needs comms and fippi booted");
        }
        firmware::firmware_ref fw = get("dsp");
        hw::dsp::dsp dsp(*this);
        dsp_online = false;
        dsp.boot(fw->data);
        dsp_online = dsp.init_done();
    }

    if (fippi_fpga) {
        init_values();
    }

    start_fifo_services();

    log(log::info) << module_label(*this) << std::boolalpha << "boot: sys-fpga=" << comms_fpga
                   << " fippi-fpga=" << boot_fippi << " dsp=" << dsp_online;

    online_ = comms_fpga && fippi_fpga && dsp_online;
}

void module::initialize() {}

void module::add(firmware::module& fw) {
    lock_guard guard(lock_);
    if (online()) {
        log(log::warning) << module_label(*this)
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
                log(log::debug) << module_label(*this) << "add module firmware: " << fp->tag
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
    std::ostringstream oss;
    oss << "firmware not found: slot=" << slot << ": device=" << device
        << " firmwares=" << firmware.size();
    throw error(number, slot, error::code::module_invalid_firmware, oss.str());
}

param::value_type module::read(const std::string& par) {
    log(log::info) << module_label(*this) << "read: par=" << par;
    return read(param::lookup_module_param(par));
}

param::value_type module::read(param::module_param par) {
    log(log::debug) << module_label(*this) << "read: par=" << int(par);
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
    log(log::info) << module_label(*this) << "read: par=" << par;
    return read(param::lookup_channel_param(par), channel);
}

double module::read(param::channel_param par, size_t channel) {
    log(log::debug) << module_label(*this) << "read: par=" << int(par);
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
        default:
            throw error(number, slot, error::code::channel_invalid_param, "invalid read parameter");
    }
    return value;
}

bool module::write(const std::string& par, param::value_type value) {
    log(log::info) << module_label(*this) << "write: par=" << par << " value=" << value;
    return write(param::lookup_module_param(par), value);
}

bool module::write(param::module_param par, param::value_type value) {
    log(log::debug) << module_label(*this) << "write: par=" << int(par) << " value=" << value;
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
    log(log::info) << module_label(*this) << "write: par=" << par << " channel=" << channel
                   << " value=" << value;
    write(param::lookup_channel_param(par), channel, value);
}

void module::write(param::channel_param par, size_t channel, double value) {
    log(log::debug) << module_label(*this) << "write: par=" << int(par) << " channel=" << channel
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
        default:
            break;
    }
}

param::value_type module::read_var(const std::string& var, size_t channel, size_t offset, bool io) {
    log(log::info) << module_label(*this) << "read: var=" << var << " channel=" << channel
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
    log(log::debug) << module_label(*this) << "read_var: module var=" << int(var)
                    << " offset=" << offset;
    online_check();
    const size_t index = static_cast<size_t>(var);
    if (index >= module_var_descriptors.size()) {
        std::ostringstream oss;
        oss << "invalid module variable: " << index;
        throw error(number, slot, error::code::module_invalid_param, oss.str());
    }
    const auto& desc = module_var_descriptors[index];
    log(log::debug) << "module: read: " << desc.name;
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
    lock_guard guard(lock_);
    param::value_type value;
    if (have_hardware && io) {
        hw::memory::dsp dsp(*this);
        hw::word mem = dsp.read(offset, desc.address);
        hw::convert(mem, value);
        module_vars[index].value[offset].value = value;
        module_vars[index].value[offset].dirty = false;
    } else {
        value = module_vars[index].value[offset].value;
    }
    return value;
}

param::value_type module::read_var(param::channel_var var, size_t channel, size_t offset, bool io) {
    log(log::debug) << module_label(*this) << "read_var: channel var=" << int(var)
                    << " channel=" << channel << " offset=" << offset << " io=" << io;
    online_check();
    channel_check(channel);
    const size_t index = static_cast<size_t>(var);
    if (index >= channel_var_descriptors.size()) {
        std::ostringstream oss;
        oss << "invalid channel variable: " << index;
        throw error(number, slot, error::code::channel_invalid_param, oss.str());
    }
    const auto& desc = channel_var_descriptors[index];
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
    lock_guard guard(lock_);
    param::value_type value;
    if (have_hardware && io) {
        hw::memory::dsp dsp(*this);
        hw::convert(dsp.read(channel, offset, desc.address), value);
        channels[channel].vars[index].value[offset].value = value;
        channels[channel].vars[index].value[offset].dirty = false;
    } else {
        value = channels[channel].vars[index].value[offset].value;
    }
    return value;
}

void module::write_var(const std::string& var, param::value_type value, size_t channel,
                       size_t offset, bool io) {
    log(log::info) << module_label(*this) << "write: var=" << var << " channel=" << channel
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
    log(log::debug) << module_label(*this) << "write_var: module var=" << int(var) << " value["
                    << offset << "]=" << value << " (0x" << std::hex << value << ')';
    online_check();
    const size_t index = static_cast<size_t>(var);
    if (index >= module_var_descriptors.size()) {
        std::ostringstream oss;
        oss << "invalid module variable: " << index;
        throw error(number, slot, error::code::module_invalid_param, oss.str());
    }
    const auto& desc = module_var_descriptors[index];
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
    log(log::debug) << module_label(*this) << "write_var: channel var=" << int(var)
                    << " channel=" << channel << " value[" << offset << "]=" << value << " (0x"
                    << std::hex << value << ')';
    online_check();
    channel_check(channel);
    const size_t index = static_cast<size_t>(var);
    if (index >= channel_var_descriptors.size()) {
        std::ostringstream oss;
        oss << "invalid channel variable: " << index;
        throw error(number, slot, error::code::channel_invalid_param, oss.str());
    }
    const auto& desc = channel_var_descriptors[index];
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

void module::sync_vars() {
    online_check();
    log(log::info) << module_label(*this) << "sync variables";
    lock_guard guard(lock_);
    hw::memory::dsp dsp(*this);
    for (auto& var : module_vars) {
        const auto& desc = var.var;
        if (desc.state == param::enable && desc.mode != param::ro) {
            for (size_t v = 0; v < var.value.size(); ++v) {
                auto& value = var.value[v];
                if (value.dirty) {
                    if (have_hardware) {
                        hw::word word;
                        hw::convert(value.value, word);
                        dsp.write(v, desc.address, word);
                    }
                    value.dirty = false;
                }
            }
        }
    }
    for (auto& channel : channels) {
        for (auto& var : channel.vars) {
            const auto& desc = var.var;
            if (desc.state == param::enable && desc.mode != param::ro) {
                for (size_t v = 0; v < var.value.size(); ++v) {
                    auto& value = var.value[v];
                    if (value.dirty) {
                        if (have_hardware) {
                            hw::word word;
                            hw::convert(value.value, word);
                            dsp.write(channel.number, v, desc.address, word);
                            value.dirty = false;
                        }
                    }
                }
            }
        }
    }
}

void module::sync_hw() {
    online_check();
    log(log::info) << module_label(*this) << "sync hardware";
    lock_guard guard(lock_);

    /*
     * Update the FIPPI with the values
     */
    hw::run::control(*this, hw::run::control_task::program_fippi);

    /*
     * Set the DAC
     */
    hw::run::control(*this, hw::run::control_task::set_dacs);

    /*
     * Get the current module CSRb parameter.
     */
    param::value_type csrb = read(param::module_param::module_csrb);

    {
        bus_guard guard(*this);

        /*
         * Set pullups for the trigger lines on the backplane
         *
         * Also enable connections of PXI nearest neighbor lines (J2) onto
         * the backplane if the module is a Rev-B or C module
         */
        hw::word cpld_csr = 0xaaa;

        if ((csrb & (1 << hw::bit::MODCSRB_CPLDPULLUP)) != 0) {
            cpld_csr |= (1 << hw::bit::CPLDCSR_PULLUP);
        } else {
            cpld_csr &= ~(1 << hw::bit::CPLDCSR_PULLUP);
        }

        /*
         * Enable connections of PXI nearest neighbor lines (J2) onto the
         * backplane if the module is a rev B or C module
         */
        if (*this == hw::rev_B || *this == hw::rev_C) {
            cpld_csr |= (1 << hw::bit::CPLDCSR_BPCONNECT);
        }

        write_word(hw::device::CFG_CTRLCS, cpld_csr);

        /*
         * Set pullups for the SYNCH lines on the backplane
         */
        hw::word csr = hw::csr::read(*this);

        if ((csrb & (1 << hw::bit::MODCSRB_CHASSISMASTER)) != 0) {
            csr |= (1 << hw::bit::PULLUP_CTRL);
        } else {
            csr &= ~(1 << hw::bit::PULLUP_CTRL);
        }

        hw::csr::write(*this, csr);
    }

    if (*this == hw::rev_F) {
        hw::run::control(*this, hw::run::control_task::reset_adc);
    }
}

void module::run_end() {
    online_check();
    lock_guard guard(lock_);
    if (run_task == hw::run::run_task::nop) {
        log(log::warning) << module_label(*this) << "run-end: no run active";
    }
    run_interval.end();
    pause_fifo_worker = true;
    hw::run::end(*this);
}

bool module::run_active() {
    online_check();
    lock_guard guard(lock_);
    return hw::run::active(*this);
}

void module::acquire_baselines() {
    log(log::info) << module_label(*this) << "acquire-baselines";
    online_check();
    lock_guard guard(lock_);
    hw::run::control(*this, hw::run::control_task::get_baselines);
}

void module::adjust_offsets() {
    log(log::info) << module_label(*this) << "adjust-offsets";
    online_check();
    lock_guard guard(lock_);
    hw::run::control(*this, hw::run::control_task::adjust_offsets);
}

void module::get_traces() {
    log(log::info) << module_label(*this) << "get-traces";
    online_check();
    lock_guard guard(lock_);
    hw::run::control(*this, hw::run::control_task::get_traces);
}

void module::set_dacs() {
    log(log::info) << module_label(*this) << "set-dacs";
    online_check();
    lock_guard guard(lock_);
    hw::run::control(*this, hw::run::control_task::set_dacs);
}

void module::start_histograms(hw::run::run_mode mode) {
    log(log::info) << module_label(*this) << "start-histograms: mode=" << int(mode);
    online_check();
    lock_guard guard(lock_);
    if (run_task.load() != hw::run::run_task::nop) {
        throw error(number, slot, error::code::module_invalid_operation,
                    "module already runinng a task");
    }
    if (test_mode.load() != test::off) {
        throw error(number, slot, error::code::module_test_invalid,
                    "test running; cannot start a run task");
    }
    hw::run::run(*this, mode, hw::run::run_task::histogram);
    run_interval.restart();
}

void module::start_listmode(hw::run::run_mode mode) {
    log(log::info) << module_label(*this) << "start-listmode: mode=" << int(mode);
    online_check();
    lock_guard guard(lock_);
    if (run_task != hw::run::run_task::nop) {
        throw error(number, slot, error::code::module_invalid_operation,
                    "module already runinng a task");
    }
    if (test_mode.load() != test::off) {
        throw error(number, slot, error::code::module_test_invalid,
                    "test running; cannot start a run task");
    }
    fifo_data.flush();
    hw::run::run(*this, mode, hw::run::run_task::list_mode);
    pause_fifo_worker = false;
    run_interval.restart();
}

void module::read_adc(size_t channel, hw::adc_word* buffer, size_t size, bool run) {
    log(log::info) << module_label(*this) << "read-adc: channel=" << channel << " size=" << size
                   << " run=" << std::boolalpha << run;
    online_check();
    lock_guard guard(lock_);
    channel::channel& chan = channels[channel];
    if (run) {
        get_traces();
    }
    if (control_task != hw::run::control_task::get_traces) {
        throw error(number, slot, error::code::module_invalid_operation,
                    "control task not `get_traces`");
    }
    chan.read_adc(buffer, size);
}

void module::read_adc(size_t channel, hw::adc_trace& buffer, bool run) {
    read_adc(channel, buffer.data(), buffer.size(), run);
}

void module::bl_find_cut(channel::range& channels_, param::values& cuts) {
    log(log::info) << module_label(*this) << "bl-find-cut: channels=" << channels.size();
    cuts.clear();
    channel::baseline bl(*this, channels_);
    lock_guard guard(lock_);
    bl.find_cut();
    cuts = bl.cuts;
}

void module::bl_get(channel::range& channels_, channel::baseline::channels_values& values,
                    bool run) {
    log(log::info) << module_label(*this) << "bl-get: channels=" << channels.size();
    channel::baseline bl(*this, channels_);
    lock_guard guard(lock_);
    if (control_task != hw::run::control_task::get_baselines) {
        throw error(number, slot, error::code::module_invalid_operation,
                    "control task `get_baseline` has not run");
    }
    bl.get(values, run);
}

void module::read_histogram(size_t channel, hw::words& values) {
    log(log::info) << module_label(*this) << "read-histogram: channel=" << channel
                   << " length=" << values.size();
    online_check();
    lock_guard guard(lock_);
    channels[channel].read_histogram(values);
}

void module::read_histogram(size_t channel, hw::word_ptr values, const size_t size) {
    log(log::info) << module_label(*this) << "read-histogram: channel=" << channel
                   << " length=" << size;
    online_check();
    lock_guard guard(lock_);
    channels[channel].read_histogram(values, size);
}

size_t module::read_list_mode_level() {
    log(log::info) << module_label(*this) << "read-list-mode-level";
    online_check();
    if (!fifo_worker_running.load()) {
        log(log::info) << module_label(*this) << "read-list-mode-level: FIFO worker not running";
    }
    return fifo_data.size();
}

void module::read_list_mode(hw::words& values) {
    log(log::info) << module_label(*this) << "read-list-mode: length=" << values.size()
                   << " fifo-size=" << fifo_data.size();
    online_check();
    if (!fifo_worker_running.load()) {
        log(log::warning) << module_label(*this) << "read-list-mode: FIFO worker not running";
    }
    if (!fifo_data.empty()) {
        lock_guard guard(lock_);
        fifo_data.copy(values);
        auto out = values.size();
        data_fifo_out += out;
        data_fifo_run_out += out;
        log(log::debug) << module_label(*this) << "read-list-mode: values=" << values.size()
                        << " fifo-size=" << fifo_data.size();
    }
}

void module::read_list_mode(hw::word_ptr values, const size_t size) {
    log(log::info) << module_label(*this) << "read-list-mode: length=" << size
                   << " fifo-size=" << fifo_data.size();
    online_check();
    if (!fifo_data.empty()) {
        lock_guard guard(lock_);
        fifo_data.copy(values, size);
    }
}

void module::read_stats(stats::stats& stats) {
    log(log::info) << module_label(*this) << "read-stats: channels=" << channels.size();
    online_check();
    lock_guard guard(lock_);
    stats::read(*this, stats);
}

void module::output(std::ostream& out) const {
    util::ostream_guard flags(out);
    out << std::boolalpha << "number: " << std::setw(2) << number << " slot: " << std::setw(2)
        << slot << " present:" << present_.load() << " online:" << online_.load()
        << " forced-offline:" << forced_offline_.load() << " serial:" << serial_num
        << " rev:" << revision_label() << " (" << revision << ") vaddr:" << vmaddr
        << " fw: " << firmware.size();
}

char module::revision_label() const {
    return static_cast<char>(revision + 55);
}

void module::dma_read(const hw::address source, hw::words& values) {
    dma_read(source, values.data(), values.size());
}

void module::dma_read(const hw::address source, hw::word_ptr values, const size_t size) {
    log(log::debug) << module_label(*this) << "dma read: addr=0x" << std::hex << source
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

    log(log::debug) << module_label(*this) << "dma read: done, period=" << tp;
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
    log(log::info) << module_label(*this) << "start-test: mode=" << int(mode);
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
            log(log::debug) << "pause the FIFO worker";
            test_mode = mode;
            data_fifo_run_in = 0;
            data_fifo_run_out = 0;
            hw::run::start(*this, hw::run::run_mode::new_run, hw::run::run_task::nop,
                           hw::run::control_task::fill_ext_fifo);
            log(log::debug) << "unpause the FIFO worker";
            pause_fifo_worker = false;
            break;
        default:
            throw error(number, slot, error::code::invalid_value, "invalid start test mode");
    }
}

void module::end_test() {
    pause_fifo_worker = true;
    test_mode = test::off;
    auto word_size = sizeof(hw::word);
    log(log::info) << module_label(*this) << "run: in=" << data_fifo_run_in.load() * word_size
                   << " out=" << data_fifo_run_out.load() * word_size
                   << " total: in=" << data_fifo_in.load() * word_size
                   << " out=" << data_fifo_out.load() * word_size
                   << " dma: in=" << data_dma_in.load() * word_size;
}

void module::load_vars() {
    if (!vars_loaded) {
        firmware::firmware_ref vars = get("var");
        param::load(vars, module_var_descriptors, channel_var_descriptors);
        param_addresses.set(max_channels, module_var_descriptors, channel_var_descriptors);
        vars_loaded = true;
        log(log::info) << module_label(*this) << "address map: " << param_addresses;
    }
}

void module::erase_values() {
    module_vars.clear();
    channels.clear();
}

void module::init_values() {
    if (num_channels == 0) {
        throw error(number, slot, error::code::internal_failure, "number of channels is 0");
    }
    if (configs.size() != num_channels) {
        throw error(number, slot, error::code::internal_failure,
                    "invalid number of channels configurations");
    }

    erase_values();
    for (const auto& desc : module_var_descriptors) {
        module_vars.push_back(param::module_variable(desc));
    }
    channels.resize(num_channels, channel::channel(*this));
    for (size_t channel = 0; channel < num_channels; ++channel) {
        channels[channel].number = channel;
        channels[channel].config = configs[channel];
        for (const auto& desc : channel_var_descriptors) {
            channels[channel].vars.push_back(param::channel_variable(desc));
        }
    }
}

void module::module_csrb(param::value_type value, size_t offset, bool io) {
    write_var(param::module_var::ModCSRB, value, offset, io);
    if (io) {
        hw::run::control(*this, hw::run::control_task::program_fippi);

        bus_guard guard(*this);

        param::value_type csr = 0xaaa;

        /*
         * Set up Pull-up resistors
         */
        if ((value & (1 << hw::bit::MODCSRB_CPLDPULLUP)) != 0) {
            csr |= 1 << hw::bit::MODCSRB_CPLDPULLUP;
        } else {
            csr &= ~(1 << hw::bit::MODCSRB_CPLDPULLUP);
        }

        /*
         * Enable connections of PXI nearest neighbor lines (J2) onto the
         * backplane if the module is a Rev-B or C module
         */
        if (*this == hw::rev_B || *this == hw::rev_C) {
            csr |= 1 << hw::bit::CPLDCSR_BPCONNECT;
        }

        write_word(hw::device::CFG_CTRLCS, csr);

        /*
         * Set pullups for the SYNCH lines on the backplane
         */
        csr = hw::csr::read(*this);

        if ((csr & (1 << hw::bit::MODCSRB_CHASSISMASTER)) != 0) {
            csr |= 1 << hw::bit::PULLUP_CTRL;
        } else {
            csr &= ~(1 << hw::bit::PULLUP_CTRL);
        }
        hw::csr::write(*this, csr);
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
        log(log::warning) << "setting FAST_FILTER_RANGE to max: " << hw::limit::FASTFILTERRANGE_MAX;
    }

    if (int(value) < int(hw::limit::FASTFILTERRANGE_MIN)) {
        value = hw::limit::FASTFILTERRANGE_MIN;
        log(log::warning) << "setting FAST_FILTER_RANGE to min: " << hw::limit::FASTFILTERRANGE_MIN;
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
    /*
     * @todo disable until LM FIFO burst support is added
     */
    if (*this == hw::rev_H) {
        return;
    }
    log(log::debug) << module_label(*this) << std::boolalpha
                    << "FIFO worker: starting: running=" << fifo_worker_running.load();
    if (!fifo_worker_running.load()) {
        pause_fifo_worker = true;
        fifo_worker_finished = false;
        fifo_worker_running = true;
        ;
        fifo_thread = std::thread(&module::fifo_worker, this);
    }
}

void module::stop_fifo_worker() {
    /*
     * @todo disable until LM FIFO burst support is added
     */
    if (*this == hw::rev_H) {
        return;
    }
    log(log::debug) << module_label(*this) << std::boolalpha
                    << "FIFO worker: stopping: running=" << fifo_worker_running.load();
    fifo_worker_running = false;
    if (fifo_thread.joinable()) {
        fifo_thread.join();
    }
}

void module::fifo_worker() {
    log(log::info) << module_label(*this) << "FIFO worker: running";

    hw::memory::fifo fifo(*this);

    try {
        size_t wait_time = fifo_run_wait_usecs.load();
        size_t hold_time = 0;

        bool pool_empty_logged = false;
        bool fifo_full_logged = false;

        (void) fifo.level();

        size_t last_dma_in = 0;
        util::timepoint bw_interval(true);

        while (fifo_worker_running.load()) {
            if (!online()) {
                hw::wait(250 * 1000);
                bw_interval.restart();
                continue;
            }

            const hw::run::run_task this_run_tsk = run_task.load();

            /*
             * If list mode is running the wait period is the currently
             * configured run period.
             *
             * If fill ext FIFO mode is running check the FIFO level
             * and refill when we drop below a threadhold.
             *
             * Any other mode will decay the wait period every hold period
             * until capped at the idle wait period.
             */
            if (this_run_tsk == hw::run::run_task::list_mode) {
                wait_time = fifo_run_wait_usecs.load();
                /*
                 * See if the task is still running? If not the module may
                 * have been directed to stop running by another module.
                 */
                if (!hw::run::active(*this)) {
                    fifo_worker_running = false;
                    run_task = hw::run::run_task::nop;
                    log(log::info) << module_label(*this) << "FIFO worker: run not active";
                }
            }
            if (test_mode.load() != test::off) {
                wait_time = fifo_run_wait_usecs.load();
            } else {
                const size_t idle_wait_time = fifo_idle_wait_usecs.load();
                /*
                 * WHen the wait time is equal to the idle period nothing
                 * further happens. The hold time should also be 0 which
                 * means any new data will be held for the hold period.
                 */
                if (wait_time > idle_wait_time) {
                    wait_time = idle_wait_time;
                } else if (wait_time < idle_wait_time && hold_time >= fifo_hold_usecs.load()) {
                    wait_time <<= 1;
                    if (wait_time > idle_wait_time) {
                        wait_time = idle_wait_time;
                    }
                    hold_time = 0;
                }
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
                size_t level = fifo.level();
                if (level == 0 ||
                    (hold_time < fifo_hold_usecs.load() && level < hw::max_dma_block_size)) {
                    break;
                }
                if (level == std::numeric_limits<hw::word>::max()) {
                    auto level2 = fifo.level();
                    log(log::debug) << module_label(*this) << "invalid FIFO level: " << level
                                    << " repeat read: " << level2;
                    break;
                }
                bool queue_buf =
                    this_run_tsk == hw::run::run_task::list_mode || test_mode.load() != test::off;
                buffer::handle buf;
                if (fifo_pool.empty()) {
                    if (!pool_empty_logged) {
                        log(log::warning) << module_label(*this) << "FIFO worker: pool empty,"
                                          << " compacting queue ...";
                    }
                    fifo_data.compact();
                }
                /*
                 * If the pool is still empty after compacting wait
                 * letting the FIFO fill.
                 */
                if (!fifo_pool.empty()) {
                    log(log::debug) << module_label(*this) << "FIFO read, level=" << level;
                    buf = fifo_pool.request();
                    if (level > buf->capacity()) {
                        level = buf->capacity();
                    }
                    buf->resize(level);
                    fifo.read(*buf, level);
                    data_dma_in += level;
                    if (queue_buf) {
                        data_fifo_in += level;
                        data_fifo_run_in += level;
                        fifo_data.push(buf);
                    }
                    hold_time = 0;
                    pool_empty_logged = false;
                    fifo_full_logged = false;
                } else {
                    if (!pool_empty_logged) {
                        log(log::warning) << module_label(*this) << "FIFO worker: pool empty";
                        pool_empty_logged = true;
                    }
                    if (!fifo_full_logged && level >= hw::fifo_size_words) {
                        fifo_full_logged = true;
                        log(log::warning) << module_label(*this) << "FIFO worker: FIFO full";
                    }
                    break;
                }

                /*
                 * Allocated bandwidth used?
                 */
                auto bandwidth = fifo_bandwidth.load();
                if (bandwidth > 0) {
                    auto dma_in = data_dma_in.load();
                    auto data_delta = (dma_in - last_dma_in) * sizeof(hw::word);
                    size_t data_in = data_delta / 1000000;
                    if (data_in >= bandwidth) {
                        last_dma_in = dma_in;
                        size_t slice = (1000 - ((1000 * bandwidth) / hw::pci_bus_datarate)) * 1000;
                        if (wait_time < slice) {
                            wait_time = slice;
                        }
                        log(log::debug) << "BW limiter: data in:: " << data_in << "MB";
                        break;
                    }
                }
            }

            hw::wait(wait_time);

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
    } catch (error& e) {
        log(log::error) << "FIFO worker: " << e;
    } catch (std::exception& e) {
        log(log::error) << "FIFO worker: error: " << e.what();
    } catch (...) {
        log(log::error) << "FIFO worker: unhandled exception";
    }

    log(log::info) << module_label(*this) << "FIFO worker: finishing";

    fifo_worker_running = false;
    fifo_worker_finished = true;
}

void assign(modules& modules_, const number_slots& numbers) {
    std::ostringstream oss;
    for (auto& number_slot : numbers) {
        oss << number_slot.second << "->" << number_slot.first << ' ';
    }
    log(log::info) << "assign slot map: " << oss.str();
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
            log(log::warning) << "module not found in slot map: " << mod->slot;
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
