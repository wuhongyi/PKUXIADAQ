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

/** @file fixture.hpp
 * @brief Defines a hardware channel fixture
 */

#ifndef PIXIE_FIXTURE_H
#define PIXIE_FIXTURE_H

#include <pixie/pixie16/hw.hpp>

namespace xia {
namespace pixie {
namespace channel {
struct channel;
};
namespace module {
class module;
};
/**
 * @brief Collects Pixie-16 specific fixtures information.
 */
namespace fixture {
/**
 * @brief The object representing the channel fixture.
 *
 * Channels can be part of the motherboard of a module or thet can be on a
 * separate PCB and the module is an assemble of boards or fixtures.
 *
 * The nature of the supported fixtures can vary.
 */
struct channel {
    /**
     * The module's channel that exists on this type of fixture.
     */
    pixie::channel::channel& module_channel;

    std::string label;

    /**
     * The hardware configuration.
     */
    hw::config config;

    channel(pixie::channel::channel& module_channel, const hw::config& config);
    virtual ~channel();

    /**
     * Open the channel fixture. Configure and set up.
     */
    virtual void open();

    /**
     * Close the channel fixture. Called by the default destructor.
     */
    virtual void close();

    /**
     * Per channel DAC set up.
     */
    virtual void set_dac(param::value_type value);

    /**
     * Per channel offsetdac adjust
     */
    virtual void adjust_offsetdac();

    /**
     * Per channel ADC acquire.
     */
    virtual void acquire_adc();

    /**
     * Per channel ADC acquire.
     */
    virtual void read_adc(hw::adc_word* buffer, size_t size);

    /**
     * Set a value.
     *
     * Overload the types required
     */
    virtual void set(const std::string item, bool value);
    virtual void set(const std::string item, int value);
    virtual void set(const std::string item, double value);
    virtual void set(const std::string item, hw::word value);

    /**
     * Get a value.
     *
     * Overload the types required
     */
    virtual void get(const std::string item, bool& value);
    virtual void get(const std::string item, int& value);
    virtual void get(const std::string item, double& value);
    virtual void get(const std::string item, hw::word& value);

    /**
     * Report the fixture's details
     */
    virtual void report(std::ostream& out) const;

    /**
     * Get the module.
     */
    module::module& get_module();
};

using channel_ptr = std::shared_ptr<channel>;

/**
 * @brief The object to manage a module's fixtures.
 *
 * Fixtures may require module level management. The @ref module struct lets
 * you hook various module events to handle fixture specific activities.
 *
 * The things you can do in each event handler will depend on the state of the
 * module. For example you cannot use the DSP services in the @ref dsp_loaded
 * handler because the module is still not formally online.
 */
struct module {
    /**
     * The module that has the fixtures.
     */
    pixie::module::module& module_;

    std::string label;

    module(pixie::module::module& module_);
    virtual ~module();

    /**
     * Open the fxitures. Called once the module is opened. Do not touch the
     * hardware. There are no channels and no variables.
     */
    virtual void open();

    /**
     * Close the fxitures. Called before the module is forced offline and
     * closed.
     */
    virtual void close();

    /**
     * Initialize the module.
     */
    virtual void initialize();

    /**
     * The module is online.
     */
    virtual void online();

    /**
     * The module has been forced offline.
     */
    virtual void forced_offline();

    /**
     * The COMMS FPGA has been loaded. Only this FPGA is loaded.
     */
    virtual void fgpa_comms_loaded();

    /**
     * The FIPPI FPGAs have been loaded. Only assume the COMMS and FIPPI FPGAs
     * are loaded.
     */
    virtual void fgpa_fippi_loaded();

    /**
     * The DSP has been loaded and can be accessed. The COMMS and FIPPI FPGAs
     * are also loaded.
     */
    virtual void dsp_loaded();

    /**
     * The module have been booted and is online.
     */
    virtual void boot();

    /**
     * The module values will be erased
     */
    virtual void erase_values();

    /**
     * The module values have been initialized
     */
    virtual void init_values();

    /**
     * The module channels will be erased
     */
    virtual void erase_channels();

    /**
     * The module channels have been initialized
     */
    virtual void init_channels();

    /**
     * Hardware synchronisation
     */
    virtual void sync_hw();

    /**
     * Variable sychronisation
     */
    virtual void sync_vars();

    /*
     * Module fixture control task support
     */

    /**
     * Set DACs.
     */
    virtual void set_dacs();

    /**
     * Get ADC traces.
     */
    virtual void get_traces();

    /**
     * Adjust offsets
     */
    virtual void adjust_offsets();

    /**
     * Tau Finder
     */
    virtual void tau_finder();
};

using module_ptr = std::shared_ptr<module>;

/**
 * Make a channel fixture given a hardware configuration
 */
channel_ptr make(pixie::channel::channel& module_channel, const hw::config& config);

/**
 * Make a module fixtures given a hardware configuration
 */
module_ptr make(pixie::module::module& module_);

}  // namespace fixture
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_FIXTURE_H
