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

/** @file pixie_api_select.hpp
 * @brief Provides a consistent interface between the Legacy and updated SDK.
 */

#ifndef PIXIE_API_SELECT_HPP
#define PIXIE_API_SELECT_HPP

#include <map>
#include <memory>
#include <string>

#include <stdint.h>

namespace xia {
namespace pixie {
namespace api {
struct calls {
    typedef std::map<const std::string, const std::string> labels;

    std::string name;

    calls(std::string name);
    virtual ~calls() = default;

    const std::string label(const std::string call) const;

    virtual int acquire_adc_trace(unsigned short module);
    virtual int acquire_baselines(unsigned short module);
    virtual int adjust_offsets(unsigned short module);
    virtual int boot_module(const char* com_fpga_config, const char* sp_fpga_config,
                            const char* trig_fpga_config, const char* dsp_code, const char* dsp_par,
                            const char* dsp_var, unsigned short module,
                            unsigned short boot_pattern);
    virtual int check_run_status(unsigned short module);
    virtual int end_run(unsigned short module);
    virtual int exit_system(unsigned short module);
    virtual int init_system(unsigned short num_modules, unsigned short* pxi_slot_map,
                            unsigned short offline);
    virtual int read_ext_fifo_status(unsigned int* words, unsigned short module);
    virtual int read_sgl_chan_adc_trace(unsigned short* buffer, unsigned int length,
                                        unsigned short module, unsigned short channel);
    virtual int read_sgl_chan_baselines(double* baselines, double* timestamps,
                                        unsigned short num_bases, unsigned short module,
                                        unsigned short channel);
    virtual int read_sgl_chan_par(const char* name, double* data, unsigned short module,
                                  unsigned short channel);
    virtual int read_sgl_mod_par(const char* name, unsigned int* data, unsigned short module);
    virtual int save_dsp_parameters_to_file(const char* file_name);
    virtual int save_external_fifo_data_to_file(const char* file_name, unsigned int* words,
                                                unsigned short module,
                                                unsigned short end_of_run_read);
    virtual int save_histogram_to_file(const char* file_name, unsigned short module);
    virtual int start_list_mode_run(unsigned short module, unsigned short run_type,
                                    unsigned short mode);
    virtual int write_sgl_chan_par(const char* name, double data, unsigned short module,
                                   unsigned short channel);
    virtual int write_sgl_mod_par(const char* name, unsigned int data, unsigned short module);

    virtual const labels& get_labels() const = 0;
};

/*
 * A calls handle.
 */
typedef std::unique_ptr<calls> calls_handle;

/*
 * Make a call handle for the selected API.
 *
 *      call_handle handle = make_pixi16();
 */
calls_handle make_pixie16_api();
calls_handle make_pixie_api();
}  // namespace api
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_API_SELECT_HPP
