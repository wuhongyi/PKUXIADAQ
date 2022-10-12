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

/**
 * @brief A C++ Tool for Pixie16
 */

#include <algorithm>
#include <cstring>
#include <fstream>
#include <future>
#include <iostream>
#include <map>
#include <numeric>
#include <regex>
#include <sstream>

#include <pixie/config.hpp>
#include <pixie/log.hpp>
#include <pixie/util.hpp>

#include <pixie/pixie16/crate.hpp>
#include <pixie/pixie16/legacy.hpp>
#include <pixie/pixie16/memory.hpp>
#include <pixie/pixie16/sim.hpp>

#include <args/args.hxx>
#include <nolhmann/json.hpp>

#include <sys/types.h>
#include <dirent.h>

/*
 * Localize the log and error
 */
using error = xia::pixie::error::error;

/*
 * JSON support
 */
using json = nlohmann::json;

/*
 * Module threading worker base
 */
struct module_thread_worker {
    int number;
    int slot;
    int pci_bus;
    int pci_slot;

    std::atomic_bool running;
    bool has_error;
    xia::util::timepoint period;
    size_t total;
    size_t last_total;

    module_thread_worker();

    virtual void worker(xia::pixie::module::module& module) = 0;
};

using slot_range = std::vector<int>;
using module_range = std::vector<size_t>;
using files = std::vector<std::string>;

struct firmware {
    std::string filename;
    std::string date;
    int version;
    int mod_revision;
    int mod_adc_msps;
    int mod_adc_bits;
    std::string device;

    firmware(
        const std::string& name, const std::string& dev,
        const std::string& fname);

    std::string spec() const;
};

using firmwares = std::vector<firmware>;

struct process_command_options {
    int num_modules;
    slot_range slots;
    std::string firmware_host_path;
    files firmware_crate_files;
    files firmware_files;
    bool reg_trace;
    bool verbose;
};

/*
 * Command processor.
 */
using args_parser = args::ArgumentParser;
using args_group = args::Group;
using args_help_flag = args::HelpFlag;
using args_flag = args::Flag;
using args_int_flag = args::ValueFlag<int>;
using args_size_flag = args::ValueFlag<size_t>;
using args_string_flag = args::ValueFlag<std::string>;
using args_strings_flag = args::ValueFlagList<std::string>;
using args_command = std::string;
using args_positional_list = args::PositionalList<args_command>;
using args_commands = std::vector<args_command>;
using args_commands_iter = args_commands::iterator;

struct command {
    using alias = std::vector<std::string>;
    using handler = void (*)(
        xia::pixie::crate::crate& crate,
        args_commands_iter& ci, args_commands_iter& ce,
        bool verbose);

    std::string name;
    handler call;
    alias aliases;
    std::string boot;
    std::string help;
    std::string help_cmd;

    command() = default;
};

#define command_handler_decl(_name) \
    static void _name(\
       xia::pixie::crate::crate& crate, \
       args_commands_iter& ci, args_commands_iter& ce, bool verbose)

/*
 * The commands are sorted alphabetical
 */

command_handler_decl(adc_acq);
static const command adc_acq_cmd = {
    "adc-acq", adc_acq,
    {},
    "init,probe",
    "Acquire a module's ADC trace",
    "adc-acq modules(s)"
};

command_handler_decl(adc_save);
static const command adc_save_cmd = {
    "adc-save", adc_save,
    {},
    "init,probe",
    "Save a module's ADC trace to a file",
    "adc-save modules(s) [channel(s) [length]]"
};

command_handler_decl(adj_off);
static const command adj_off_cmd = {
    "adj-off", adj_off,
    {},
    "init,probe",
    "Adjust the module's offsets",
    "adj-off modules(s)"
};

command_handler_decl(bl_acq);
static const command bl_acq_cmd = {
    "bl-acq", bl_acq,
    {},
    "init,probe",
    "Acquire module baselines",
    "bl-acq module(s)"
};

command_handler_decl(bl_save);
static const command bl_save_cmd = {
    "bl-save", bl_save,
    {},
    "init,probe",
    "Save the module's baselines",
    "bl-save module(s) [channel(s)]"
};

command_handler_decl(boot);
static const command boot_cmd = {
    "boot", boot,
    {"b"},
    "init,probe",
    "Boots the module(s)",
    "boot"
};

command_handler_decl(crate_report);
static const command crate_cmd = {
    "crate", crate_report,
    {},
    "init,probe",
    "Report the crate",
    "crate"
};

command_handler_decl(export_);
static const command export_cmd = {
    "export", export_,
    {},
    "init,probe",
    "Export a configuration to a JSON file",
    "export file"
};

command_handler_decl(help);
static const command help_cmd = {
    "help", help,
    {},
    "none",
    "Command specific help. Add '-l' to list all commands",
    "help [command]"
};

command_handler_decl(hist_resume);
static const command hist_resume_cmd = {
    "hist-resume", hist_resume,
    {"hr"},
    "init,probe",
    "Resume module histograms",
    "hist-resume module(s)"
};

command_handler_decl(hist_save);
static const command hist_save_cmd = {
    "hist-save", hist_save,
    {"hv"},
    "init,probe",
    "Save a module's histogram to a file",
    "hist-save [-b bins] module(s) [channel(s)]"
};

command_handler_decl(hist_start);
static const command hist_start_cmd = {
    "hist-start", hist_start,
    {"hs"},
    "init,probe",
    "Start module histograms",
    "hist-start module(s)"
};

command_handler_decl(import);
static const command import_cmd = {
    "import", import,
    {},
    "init,probe",
    "Import a JSON configuration file",
    "import file"
};

command_handler_decl(list_mode);
static const command list_mode_cmd = {
    "list-mode", list_mode,
    {"lm"},
    "init,probe",
    "Run list mode saving the data to a file",
    "list-mode module(s) secs file"
};

command_handler_decl(list_resume);
static const command list_resume_cmd = {
    "list-resume", list_resume,
    {"lr"},
    "init,probe",
    "Resume module list mode",
    "list-resume module(s)"
};

command_handler_decl(list_save);
static const command list_save_cmd = {
    "list-save", list_save,
    {"ls"},
    "init,probe",
    "Save a module's list-mode data to a file",
    "list-save module(s) secs file"
};

command_handler_decl(list_start);
static const command list_start_cmd = {
    "list-start", list_start,
    {},
    "init,probe",
    "Start module list mode",
    "list-start module(s)"
};

command_handler_decl(lset_import);
static const command lset_import_cmd = {
    "lset-import", lset_import,
    {"lsi"},
    "init,probe",
    "Import a legacy settings file to a module",
    "lset-import module(s) file [flush/sync]"
};

command_handler_decl(lset_load);
static const command lset_load_cmd = {
    "lset-load", lset_load,
    {"lsl"},
    "init,probe",
    "Load a legacy settings file to a module's DSP memory",
    "lset-load module(s) file [flush/sync]"
};

command_handler_decl(lset_report);
static const command lset_report_cmd = {
    "lset-report", lset_report,
    {"lsr"},
    "init,probe",
    "Output a legacy settings file in a readable format",
    "lset-report module(s) file"
};

command_handler_decl(par_read);
static const command par_read_cmd = {
    "par-read", par_read,
    {"pr"},
    "init,probe",
    "Read module/channel parameter",
    "par-read module(s) param [channel(s)]"
};

command_handler_decl(par_write);
static const command par_write_cmd = {
    "par-write", par_write,
    {},
    "init,probe",
    "Write module/channel parameter",
    "par-write module(s) param [channel(s)] value"
};

command_handler_decl(report);
static const command report_cmd = {
    "report", report,
    {"re"},
    "init,probe",
    "Report the crate's configuration",
    "report file"
};

command_handler_decl(reg_read);
static const command reg_read_cmd = {
    "reg-read", reg_read,
    {"rr"},
    "init",
    "Read a register from a module or slot memory address",
    "reg-read [-s] [-x] module/slot [address] [name] [memory:name]"
};

command_handler_decl(run_active);
static const command run_active_cmd = {
    "run-active", run_active,
    {"ra"},
    "init,probe",
    "Does the module have an active run?",
    "run-active module(s)"
};

command_handler_decl(run_end);
static const command run_end_cmd = {
    "run-end", run_end,
    {"re"},
    "init,probe",
    "End module run's",
    "run-end module(s)"
};

command_handler_decl(set_dacs);
static const command set_dacs_cmd = {
    "set-dacs", set_dacs,
    {},
    "init,probe",
    "Set the module's DACs",
    "set-dacs modules(s)"
};

command_handler_decl(stats);
static const command stats_cmd = {
    "stats", stats,
    {"st"},
    "init,probe",
    "Module/channel stats",
    "stats [-s stat (pe/ocr/rt/lt)] module(s) [channel(s)]"
};

command_handler_decl(test);
static const command test_cmd = {
    "test", test,
    {},
    "init,probe",
    "Test control, default mode is 'off'",
    "test [-m mode (off/lmfifo)] module(s)"
};

command_handler_decl(var_read);
static const command var_read_cmd = {
    "var-read", var_read,
    {},
    "init,probe",
    "Read module/channel variable",
    "var-read module(s) param [channel(s) [offset(s)]]"
};

command_handler_decl(var_write);
static const command var_write_cmd = {
    "var-write", var_write,
    {},
    "init,probe",
    "Write module/channel variable",
    "var-write module(s) param [channel(s) [offset(s)]] value"
};

command_handler_decl(wait);
static const command wait_cmd = {
    "wait", wait,
    {},
    "none",
    "wait a number of msecs",
    "wait msecs"
};

using command_map = std::map<std::string, const command&>;

static const command_map commands = {
    {"adc-acq", adc_acq_cmd},
    {"adc-save", adc_save_cmd},
    {"adj-off", adj_off_cmd},
    {"bl-acq", bl_acq_cmd},
    {"bl-save", bl_save_cmd},
    {"boot", boot_cmd},
    {"crate", crate_cmd},
    {"export", export_cmd},
    {"help", help_cmd},
    {"hist-resume", hist_resume_cmd},
    {"hist-save", hist_save_cmd},
    {"hist-start", hist_start_cmd},
    {"import", import_cmd},
    {"list-mode", list_mode_cmd},
    {"list-resume", list_resume_cmd},
    {"list-save", list_save_cmd},
    {"list-start", list_start_cmd},
    {"lset-import", lset_import_cmd},
    {"lset-load", lset_load_cmd},
    {"lset-report", lset_report_cmd},
    {"par-read", par_read_cmd},
    {"par-write", par_write_cmd},
    {"reg-read", reg_read_cmd},
    {"report", report_cmd},
    {"run-active", run_active_cmd},
    {"run-end", run_end_cmd},
    {"set-dacs", set_dacs_cmd},
    {"stats", stats_cmd},
    {"test", test_cmd},
    {"var-read", var_read_cmd},
    {"var-write", var_write_cmd},
    {"wait", wait_cmd}
};

static std::string adc_prefix = "pixie16-omnitool-adc";
static std::string histogram_prefix = "pixie16-omnitool-mca";
static std::string baseline_prefix = "pixie16-omnitool-baseline";
static std::string list_mode_prefix = "pixie16-omnitool-list-mode";

static const json hardware = {
    {"memory",
     {{"sys",
       {{"cfg_datacs", 0x00},
        {"cfg_ctrlcs", 0x04},
        {"cfg_rdcs", 0x08},
        {"i2cm24c64", 0x10},
        {"pcf8574", 0x10},
        {"cfg_dcmrst", 0x14},
        {"cfg_dac", 0x18},
        {"csr", 0x48},
        {"request_hbr", 0x80},
        {"hbr_done", 0x84},
        {"wrt_ext_mem", 0xc0},
        {"ext_mem_test", 0xc4},
        {"wrt_dsp_mma", 0xc8},
        {"set_ext_fifo", 0xcc},
        {"wrt_dsp_dmac11", 0xd0},
        {"wrt_dsp_ii11", 0xd4},
        {"wrt_clr_extmem", 0xd8},
        {"wrt_dsp_c11", 0xdc},
        {"wrt_dsp_im11", 0xe0},
        {"wrt_dsp_ec11", 0xe4},
        {"rd_wrt_fifo_wml", 0xe8},
        {"set_exmem_fifo", 0xec},
        {"set_int_fifo", 0xf0},
        {"wrt_reset_rfcnt", 0xf4}}},
      {"fippi",
       {{"csrin", 0x00800400},
        {"coincpattern", 0x00800402},
        {"adcctrl", 0x00800404},
        {"hostclr", 0x00800408},
        {"adcspi", 0x0080040a},
        {"bitslip", 0x0080040c},
        {"hdr_ids", 0x00800412},
        {"adcfifoctrl", 0x00800418}}},
      {"dsp", {}},
      {"mca", {}},
      {"fifo", {}}}},
    {"bits",
     {{"hbr",
       {{"hdr_hbr", 0},
        {"hbr_dsp_ncs", 1},
        {"hbr_auto_inc", 2},
        {"hbr_read_again_read", 3}}},
      {"csr",
       {{"runena", 0},
        {"dspdownload", 1},
        {"pciactive", 2},
        {"pullup_ctrl", 3},
        {"dspreset", 4},
        {"extfifo_wml", 6},
        {"runactive", 13},
        {"clrextmem_active", 15},
        {"modcsrb_cpldpullup", 0},
        {"modcsrb_dirmod", 4},
        {"modcsrb_chassismaster", 6},
        {"cpldcsr_bpconnect", 12},
        {"cpldcsr_pullup", 13}}},
      {"cgancsra",
       {{"ccsra_ftrigsel", 0},
        {"ccsra_exttrigsel", 1},
        {"ccsra_good", 2},
        {"ccsra_chantrigsel", 3},
        {"ccsra_syncdataacq", 4},
        {"ccsra_polarity", 5},
        {"ccsra_vetoena", 6},
        {"ccsra_histoe", 7},
        {"ccsra_traceena", 8},
        {"ccsra_qdcena", 9},
        {"ccsra_cfdmode", 10},
        {"ccsra_globtrig", 11},
        {"ccsra_esumsena", 12},
        {"ccsra_chantrig", 13},
        {"ccsra_enarelay", 14}}}}}
};

static bool starts_with(const std::string& s1, const std::string& s2) {
    return s2.size () <= s1.size () && s1.compare (0, s2.size (), s2) == 0;
}

static void string_replace(
    std::string& target, const char find, const char replace) {
    while (true) {
        auto pos = target.find_first_of(find);
        if (pos == std::string::npos) {
            break;
        }
        target.replace(pos, 1, 1, replace);
    }
}

static void tolower(std::string& s) {
    std::transform(
        s.cbegin(), s.cend(), s.begin(),
        [](unsigned char c) { return std::tolower(c); });
}

static bool check_number(const std::string& opt) {
    return std::regex_match(
        opt, std::regex(("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?")));
}

template<typename T>
static T get_value(const std::string& opt) {
    T value = 0;
    try {
        value = std::stoul(opt, nullptr, 0);
    } catch (std::invalid_argument e) {
        throw std::runtime_error("invalid number: " + opt);
    } catch (std::out_of_range e) {
        throw std::runtime_error("number out of range: " + opt);
    }
    return value;
}

template<typename T>
static std::vector<T> get_values(
    const std::string& opt, const size_t max_count = 0, bool no_error = false) {
    std::vector<T> values;
    if (opt == "all") {
        if (max_count == 0) {
            throw std::runtime_error("range `all` invalid, max count is unknown");
        }
        values.resize(max_count);
        std::iota(values.begin(), values.end(), 0);
    } else {
        xia::util::strings sc;
        xia::util::split(sc, opt, ',');
        for (auto& slots : sc) {
            xia::util::strings sd;
            xia::util::split(sd, slots, '-');
            if (sd.size() == 1) {
                values.push_back(get_value<T>(sd[0]));
            } else if (sd.size() == 2) {
                auto start = get_value<T>(sd[0]);
                auto end = get_value<T>(sd[1]);
                if (start > end) {
                    values.clear();
                    break;
                }
                for (T s = start; s <= end; ++s) {
                    values.push_back(s);
                }
            } else {
                if (!no_error) {
                    throw std::runtime_error("invalid range: " + opt);
                }
                values.clear();
                break;
            }
        }
    }
    return values;
}

static void next(args_commands_iter& ci, args_commands_iter& ce) {
    if (ci != ce) {
        ++ci;
    }
}

static const args_command& get_and_next(args_commands_iter& ci, args_commands_iter& ce) {
    if (ci == ce) {
        throw std::runtime_error("not enough arguments");
    }
    const args_command& opt = *ci;
    next(ci, ce);
    return opt;
}
static size_t args_count(args_commands_iter& ci, args_commands_iter& ce) {
    return std::distance(ci, ce);
}

static command_map::const_iterator find_command(const args_command& opt) {
    for (command_map::const_iterator ci = commands.begin(); ci != commands.end(); ++ci) {
        const command& cmd = std::get<1>(*ci);
        if (cmd.name == opt) {
            return ci;
        }
        for (auto& alias : cmd.aliases) {
            if (alias == opt) {
                return ci;
            }
        }
    }
    return commands.end();
}

static bool valid_command(command_map::const_iterator ci) {
    return ci != commands.end();
}

static bool valid_option(
    args_commands_iter& ci, args_commands_iter& ce, size_t count) {
    bool valid = true;
    if (args_count(ci, ce) < count) {
        valid = false;
    } else {
        for (int i = 0; valid && i < count; ++i) {
            valid = !valid_command(find_command(*(ci + i)));
        }
    }
    return valid;
}

static args_command switch_option(
    const std::string& opt_switch, args_commands_iter& ci, args_commands_iter& ce,
    bool has_opt = true) {
    args_command sopt;
    if (ci != ce) {
        auto opt = *ci;
        if (!valid_command(find_command(opt))) {
            /*
             * Check the start of the option for the option switch
             */
            if (starts_with(opt, opt_switch)) {
                ++ci;
                /*
                 * Get next opt if separate else remove the switch
                 */
                if (opt == opt_switch) {
                    if (has_opt) {
                        if (ci == ce) {
                            throw std::runtime_error(
                                "no option with switch: " + opt_switch);
                        }
                        sopt = *ci++;
                    }
                } else {
                    sopt = opt.substr(opt_switch.length());
                    if (has_opt && sopt.empty()) {
                        throw std::runtime_error(
                            "no option with switch: " + opt_switch);
                    }
                }
                if (!has_opt && sopt.empty()) {
                    sopt = "true";
                }
            }
        }
    }
    return sopt;
}

static void channels_option(
    xia::pixie::channel::range& channels, const args_command& opt, size_t num_channels) {
    if (opt.empty()) {
        channels.resize(num_channels);
        xia::pixie::channel::range_set(channels);
    } else {
        auto chans = get_values<size_t>(opt, num_channels);
        for (auto c : chans) {
            channels.push_back(c);
        }
    }
}

static void modules_option(
    module_range& modules, const args_command& opt, size_t num_modules) {
    if (opt.empty()) {
        modules.resize(num_modules);
        std::iota(modules.begin(), modules.end(), 0);
    } else {
        modules = get_values<size_t>(opt, num_modules);
    }
}

static const std::string basename(const std::string& name) {
    size_t b = name.find_last_of('/');
    if (b != std::string::npos) {
        return name.substr(b + 1);
    }
    return name;
}

static const std::string dirname(const std::string& name) {
    size_t b = name.find_last_of('/');
    if (b != std::string::npos) {
        return name.substr(0, b);
    }
    return name;
}

static void find_files(
    const std::string path, files& files_, const std::string& ext, size_t depth = 0) {
    if (depth > 100) {
        throw std::runtime_error("file find path too deep: " + path);
    }
    if (depth == 0) {
        files_.clear();
    }
    DIR* dir = nullptr;
    try {
        dir = ::opendir(path.c_str());
        if (dir == nullptr) {
            throw std::runtime_error(
              "file find path: " + path + ": " + std::strerror(errno));
        }
        while (true) {
            struct dirent* ent = ::readdir(dir);
            if (ent == nullptr) {
                break;
            }
            std::string name = ent->d_name;
            if (ent->d_type == DT_REG) {
                if (name.size() > ext.size() && name.compare(
                        name.size() - ext.size(), ext.size(), ext) == 0) {
                    files_.push_back(path + '/' + name);
                }
            } else if (ent->d_type == DT_DIR && name != "." && name != "..") {
                std::string child = path + '/' + name;
                find_files(child, files_, ext, depth + 1);
            }
        }
        ::closedir(dir);
    } catch (...) {
        if (dir != nullptr) {
            ::closedir(dir);
        }
        throw;
    }
}

firmware::firmware(
    const std::string& name, const std::string& dev, const std::string& fname) {
    xia::util::strings ns;
    xia::util::split(ns, name, '_');
    if (ns.size() != 6) {
        throw std::runtime_error("invalid firmware name: " + name);
    }
    filename = fname;
    date = ns[5];
    auto mr = ns[1][3];
    if (mr >= 'a' && mr <= 'z') {
        mr -= 'a' - 10;
    }
    version = std::atoi(ns[4].c_str());
    mod_revision = int(mr);
    auto b = ns[3].find_first_of('b');
    mod_adc_msps = std::atoi(ns[3].substr(b + 1, ns[3].size() - b - 1).c_str());
    mod_adc_bits = std::atoi(ns[3].substr(0, b).c_str());
    device = dev;
}

std::string firmware::spec() const {
    std::ostringstream oss;
    oss << "version=" << version << ", "
        << "revision=" << mod_revision << ", "
        << "adc-msps=" << mod_adc_msps << ", "
        << "adc-bits=" << mod_adc_bits << ", "
        << "device=" << device << ", "
        << "file=" << filename;
    return oss.str();
}

static void find_firmwares(const std::string basepath, firmwares& fws) {
    files files_;
    find_files(basepath, files_, ".yaml");
    for (auto& f : files_) {
        std::string bname = basename(f);
        std::string dir = dirname(f);
        auto name = bname.substr(0, bname.size() - 5);
        files rev_files;
        find_files(dir, rev_files, ".ldr");
        if (rev_files.size() != 1) {
            throw std::runtime_error("fimrware has too many LDR files: " + name);
        }
        fws.emplace_back(name, "dsp", rev_files[0]);
        find_files(dir, rev_files, ".var");
        if (rev_files.size() != 1) {
            throw std::runtime_error("fimrware has too many VAR files: " + name);
        }
        fws.emplace_back(name, "var", rev_files[0]);
        find_files(dir, rev_files, ".bin");
        if (rev_files.size() != 2) {
            throw std::runtime_error("fimrware has too many BIN files: " + name);
        }
        for (auto bin : rev_files) {
            if (bin.find("fip") != std::string::npos) {
                fws.emplace_back(name, "fippi", bin);
            } else if (bin.find("sys") != std::string::npos) {
                fws.emplace_back(name, "sys", bin);
            }
        }
    }
}

static void load_host_firmwares(
    const std::string& basepath, xia::pixie::firmware::crate& fws) {
    firmwares all;
    firmwares latest;
    find_firmwares(basepath, all);
    /*
     * Sort the firmwares found into the latest for each type of
     * module.
     */
    for (auto& fw : all) {
        auto mod_match = [&fw](auto& b) {
            return
                fw.device == b.device &&
                fw.mod_revision == b.mod_revision &&
                fw.mod_adc_msps == b.mod_adc_msps &&
                fw.mod_adc_bits == b.mod_adc_bits;
        };
        auto lfi =
            std::find_if(std::begin(latest), std::end(latest), mod_match);
        if (lfi == std::end(latest)) {
            latest.push_back(fw);
        } else {
            auto& lfw = *lfi;
            bool swap = false;
            if (lfw.date == fw.date) {
                swap = lfw.version < fw.version;
            } else {
                swap = lfw.date < fw.date;
            }
            if (swap) {
                *lfi = fw;
            }
        }
    }
    for (auto& lfw : latest) {
        auto fw = xia::pixie::firmware::parse(lfw.spec(), ',');
        if (!xia::pixie::firmware::check(fws, fw)) {
            xia::pixie::firmware::add(fws, fw);
        }
    }
}

void load_crate_firmwares(
    const std::string& file, xia::pixie::firmware::crate& fws) {
    std::ifstream input(file, std::ios::in | std::ios::binary);
    if (!input) {
        throw std::runtime_error(std::string("crate firmware file open: ") + file + ": " +
                                 std::strerror(errno));
    }
    for (std::string line; std::getline(input, line);) {
        if (!line.empty()) {
            auto fw = xia::pixie::firmware::parse(line, ',');
            if (xia::pixie::firmware::check(fws, fw)) {
                std::string what("duplicate firmware option: ");
                what += line;
                throw std::runtime_error(what);
            }
            xia::pixie::firmware::add(fws, fw);
        }
    }
}

static void firmware_load(
    xia::pixie::crate::crate& crate, process_command_options& process_opts) {
    /*
     * Load command line first, then crate files and then fill in any
     * gaps with the host files.
     */
    for (auto& fwfile : process_opts.firmware_files) {
        auto fw = xia::pixie::firmware::parse(fwfile, ':');
        if (xia::pixie::firmware::check(crate.firmware, fw)) {
            std::string what("duplicate firmware on command line: ");
            what += fwfile;
            throw std::runtime_error(what);
        }
        xia::pixie::firmware::add(crate.firmware, fw);
    }
    for (auto& fwfile : process_opts.firmware_crate_files) {
        load_crate_firmwares(fwfile, crate.firmware);
    }
    load_host_firmwares(process_opts.firmware_host_path, crate.firmware);
}

static void initialize(
    xia::pixie::crate::crate& crate, process_command_options& process_opts) {
    xia::util::timepoint tp;
    if (process_opts.verbose) {
        std::cout << "crate: initialize" << std::endl;
        tp.start();
    }
    crate.initialize(process_opts.reg_trace);
    if (process_opts.verbose) {
        tp.end();
        std::cout << "modules: detected=" << crate.modules.size()
                  << " time=" << tp << std::endl;
    }
    if (process_opts.num_modules != 0 &&
        crate.num_modules != process_opts.num_modules) {
        throw std::runtime_error("invalid number of modules detected: "
                                 "found " +
                                 std::to_string(crate.num_modules));
    }
    if (!process_opts.slots.empty()) {
        xia::pixie::module::number_slots slot_map;
        for (auto s : process_opts.slots) {
            slot_map.emplace_back(std::make_pair(int(slot_map.size()), s));
        }
        crate.assign(slot_map);
    }
}

static void probe(
    xia::pixie::crate::crate& crate, process_command_options& process_opts) {
    if (process_opts.verbose) {
        std::cout << "modules: online=" << crate.modules.size()
                  << " offline=" << crate.offline.size() << std::endl;
    }
    crate.set_firmware();
    crate.probe();
}

static void process_commands(
    xia::pixie::crate::crate& crate, process_command_options& process_opts,
    args_commands& opts) {
    bool init_done = false;
    bool probe_done = false;
    auto ci = opts.begin();
    auto ce = opts.end();
    while (ci != ce) {
        auto& opt = get_and_next(ci, ce);
        auto fci = find_command(opt);
        if (valid_command(fci)) {
            const command& cmd = std::get<1>(*fci);
            if (!init_done && cmd.boot.find("init") != std::string::npos) {
                initialize(crate, process_opts);
                init_done = true;
            }
            if (!probe_done && cmd.boot.find("probe") != std::string::npos) {
                firmware_load(crate, process_opts);
                probe(crate, process_opts);
                probe_done = true;
            }
            cmd.call(crate, ci, ce, process_opts.verbose);
        } else {
            throw std::runtime_error("invalid command: " + opt);
        }
    }
}

static void load_commands(const std::string& name, args_commands& cmds) {
    std::ifstream in(name);
    if (!in) {
        throw std::runtime_error(
            std::string("command file open: ") + name + ": " +
            std::strerror(errno));
    }
    std::string line;
    while (getline(in, line)) {
        string_replace(line, '\r', ' ');
        string_replace(line, '\t', ' ');
        xia::util::strings sc;
        xia::util::split(sc, line);
        if (!sc.empty()) {
            cmds.insert(std::end(cmds), std::begin(sc), std::end(sc));
        }
    }
}

static void help_output(std::ostream& out) {
    out << "  COMMANDS:" << std::endl;
    const command& cmd = std::get<1>(*find_command("help"));
    out << "      " << cmd.name << " - " << cmd.help << std::endl
        << "        eg '-- help -l'" << std::endl
        << std::endl;
}

static void module_check(xia::pixie::crate::crate& crate, std::vector<size_t> mod_nums) {
    for (auto mod_num : mod_nums) {
        if (mod_num > crate.num_modules) {
            throw std::runtime_error(
                std::string("invalid module number: " + std::to_string(mod_num)));
        }
        if (!crate.modules[mod_num]->online()) {
            throw std::runtime_error(std::string("module offline: " + std::to_string(mod_num)));
        }
    }
}

template<typename V>
static void output_value(const std::string& name, V value) {
    xia::util::ostream_guard oguard(std::cout);
    std::cout << name << " = " << value;
    if (!std::is_same<V, double>::value) {
        std::cout << std::hex << " (0x" << value << ')';
    }
    std::cout << std::endl;
}

template<typename W>
void module_threads(xia::pixie::crate::crate& crate, std::vector<size_t>& mod_nums,
                    std::vector<W>& workers, std::string error_message,
                    bool show_performance = true) {
    if (workers.size() != mod_nums.size()) {
        throw std::runtime_error("workers and modules counts mismatch");
    }
    using promise_error = std::promise<error::code>;
    using future_error = std::future<error::code>;
    std::vector<promise_error> promises(mod_nums.size());
    std::vector<future_error> futures;
    std::vector<std::thread> threads;
    for (size_t m = 0; m < mod_nums.size(); ++m) {
        auto module = crate.modules[mod_nums[m]];
        auto& worker = workers[m];
        futures.push_back(future_error(promises[m].get_future()));
        threads.push_back(std::thread([m, &promises, module, &worker] {
            try {
                worker.running = true;
                worker.worker(*module);
                promises[m].set_value(error::code::success);
            } catch (xia::pixie::error::error& e) {
                promises[m].set_value(e.type);
            } catch (...) {
                try {
                    promises[m].set_exception(std::current_exception());
                } catch (...) {
                }
            }
            worker.running = false;
        }));
    }
    error::code first_error = error::code::success;
    size_t finished = 0;
    size_t show_secs = 5;
    xia::util::timepoint duration(true);
    xia::util::timepoint interval(true);
    while (finished != threads.size()) {
        finished = threads.size();
        for (size_t t = 0; t < threads.size(); ++t) {
            auto& future = futures[t];
            if (future.valid()) {
                auto zero = std::chrono::seconds(0);
                if (future.wait_for(zero) == std::future_status::ready) {
                    workers[t].period.stop();
                    error::code e = future.get();
                    if (e != error::code::success) {
                        std::cout << "module " << t
                                  << ": error: " << xia::pixie::error::api_result_text(e)
                                  << std::endl;
                    }
                    if (first_error == error::code::success) {
                        first_error = e;
                    }
                    threads[t].join();
                } else {
                    --finished;
                }
            }
        }
        xia::pixie::hw::wait(20 * 1000);
        if (show_performance && interval.secs() > show_secs) {
            auto secs = interval.secs();
            interval.restart();
            std::cout << "running: " << threads.size() - finished << std::endl;
            size_t all_total = 0;
            for (auto& w : workers) {
                if (w.period.secs() > 0) {
                    auto total = w.total;
                    auto bytes = (total - w.last_total) * sizeof(xia::pixie::hw::word);
                    auto rate = double(bytes) / secs;
                    all_total += total;
                    bytes = total * sizeof(xia::pixie::hw::word);
                    char active = w.running.load() ? '>' : ' ';
                    w.last_total = total;
                    std::ostringstream oss;
                    oss << ' ' << active << std::setw(2) << w.number << ": total: " << std::setw(8)
                        << xia::util::humanize(bytes) << " rate: " << std::setw(8)
                        << xia::util::humanize(rate) << " bytes/sec pci: bus=" << w.pci_bus
                        << " slot=" << w.pci_slot;
                    std::cout << oss.str() << std::endl;
                    xia_log(xia::log::info) << oss.str();
                } else {
                    std::cout << ' ' << std::setw(2) << w.number << ": not running" << std::endl;
                }
            }
            all_total *= sizeof(xia::pixie::hw::word);
            std::ostringstream oss;
            oss << " all: total: " << std::setw(8) << xia::util::humanize(all_total)
                << " rate: " << std::setw(8)
                << xia::util::humanize(double(all_total) / duration.secs()) << " bytes/sec";
            std::cout << oss.str() << std::endl;
            xia_log(xia::log::info) << oss.str();
        }
    }
    if (first_error != error::code::success) {
        throw error(first_error, error_message);
    }
}

module_thread_worker::module_thread_worker()
    : number(-1), slot(-1), pci_bus(-1), pci_slot(-1), running(false), has_error(false), total(0),
      last_total(0) {}

template<typename W>
void set_num_slot(xia::pixie::crate::crate& crate, std::vector<size_t>& mod_nums,
                  std::vector<W>& workers) {
    for (size_t m = 0; m < mod_nums.size(); ++m) {
        auto module = crate.modules[mod_nums[m]];
        auto& worker = workers[m];
        worker.number = module->number;
        worker.slot = module->slot;
        worker.pci_bus = module->pci_bus();
        worker.pci_slot = module->pci_slot();
    }
}

template<typename W>
void performance_stats(std::vector<W>& workers, bool show_workers = false) {
    size_t total = 0;
    size_t secs = 0;
    for (auto& w : workers) {
        if (w.period.secs() > secs) {
            secs = size_t(w.period.secs());
        }
        total += w.total;
        if (show_workers) {
            if (w.has_error) {
                std::stringstream he_oss;
                he_oss << "module: num:" << std::setw(2) << w.number << " slot:" << std::setw(2)
                       << w.slot << ": has an error; check the log";
                std::cout << he_oss.str() << std::endl;
                xia_log(xia::log::info) << he_oss.str();
            }
            std::stringstream dr_oss;
            auto bytes = w.total * sizeof(xia::pixie::hw::word);
            auto rate = double(bytes) / w.period.secs();
            dr_oss << "module: num:" << std::setw(2) << w.number << " slot:" << std::setw(2)
                   << w.slot << ": data received: " << std::setw(8) << xia::util::humanize(bytes)
                   << " bytes (" << std::setw(9) << bytes << "), rate: " << std::setw(8)
                   << xia::util::humanize(rate) << " bytes/sec pci: bus=" << w.pci_bus
                   << " slot=" << w.pci_slot;
            std::cout << dr_oss.str() << std::endl;
            xia_log(xia::log::info) << dr_oss.str();
        }
    }
    total *= sizeof(xia::pixie::hw::word);
    std::stringstream oss;
    oss << "data received: " << xia::util::humanize(total) << " bytes (" << total
        << "), rate: " << xia::util::humanize(double(total) / secs, " bytes/sec");
    std::cout << oss.str() << std::endl;
    xia_log(xia::log::info) << oss.str();
}

static void adc_acq(
    xia::pixie::crate::crate& crate, args_commands_iter& ci, args_commands_iter& ce,
    bool verbose) {
    if (!valid_option(ci, ce, 1)) {
        throw std::runtime_error("adj-acq: not enough options");
    }
    auto mod_nums_opt = get_and_next(ci, ce);
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        crate[mod_num].get_traces();
    }
}

static void adc_save(
    xia::pixie::crate::crate& crate, args_commands_iter& ci, args_commands_iter& ce,
    bool verbose) {
    if (!valid_option(ci, ce, 1)) {
        throw std::runtime_error("adj-save: not enough options");
    }
    auto mod_nums_opt = get_and_next(ci, ce);
    args_command chans_opt;
    args_command len_opt;
    if (valid_option(ci, ce, 1)) {
        chans_opt = get_and_next(ci, ce);
    }
    if (valid_option(ci, ce, 1)) {
        len_opt = get_and_next(ci, ce);
    }
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    size_t length = xia::pixie::hw::max_adc_trace_length;
    if (!len_opt.empty()) {
        length = get_value<size_t>(len_opt);
    }
    for (auto mod_num : mod_nums) {
        xia::pixie::channel::range channels;
        channels_option(channels, chans_opt, crate[mod_num].num_channels);
        std::vector<xia::pixie::hw::adc_trace> traces;
        for (auto channel : channels) {
            xia::pixie::hw::adc_trace adc_trace(length);
            crate[mod_num].read_adc(channel, adc_trace, false);
            traces.push_back(adc_trace);
        }
        std::ostringstream name;
        name << std::setfill('0') << adc_prefix
             << '-' << std::setw(2) << mod_num << ".csv";
        std::ofstream out(name.str());
        out << "bin,";
        for (size_t idx = 0; idx < channels.size(); idx++) {
            if (idx != channels.size() - 1)
                out << "Chan" << channels[idx] << ",";
            else
                out << "Chan" << channels[idx] << std::endl;
        }
        for (unsigned int bin = 0; bin < length; bin++) {
            out << bin << ",";
            for (size_t idx = 0; idx < traces.size(); idx++) {
                if (idx != traces.size() - 1)
                    out << traces[idx][bin] << ",";
                else
                    out << traces[idx][bin] << std::endl;
            }
        }
    }
}

static void adj_off(
    xia::pixie::crate::crate& crate, args_commands_iter& ci, args_commands_iter& ce,
    bool verbose) {
    if (!valid_option(ci, ce, 1)) {
        throw std::runtime_error("adj-off: not enough options");
    }
    auto mod_nums_opt = get_and_next(ci, ce);
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        crate[mod_num].adjust_offsets();
    }
}

static void bl_acq(
    xia::pixie::crate::crate& crate, args_commands_iter& ci, args_commands_iter& ce,
    bool verbose) {
    if (!valid_option(ci, ce, 1)) {
        throw std::runtime_error("bl-acq: not enough options");
    }
    auto mod_nums_opt = get_and_next(ci, ce);
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        crate[mod_num].acquire_baselines();
    }
}

static void bl_save(
    xia::pixie::crate::crate& crate, args_commands_iter& ci, args_commands_iter& ce,
    bool verbose) {
    if (!valid_option(ci, ce, 1)) {
        throw std::runtime_error("bl-save: not enough options");
    }
    auto mod_nums_opt = get_and_next(ci, ce);
    args_command chans_opt;
    if (valid_option(ci, ce, 1)) {
        chans_opt = get_and_next(ci, ce);
    }
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        xia::pixie::channel::range channels;
        channels_option(channels, chans_opt, crate[mod_num].num_channels);
        xia::pixie::channel::baseline::channels_values baselines(crate[mod_num].num_channels);
        crate[mod_num].bl_get(channels, baselines, false);

        std::ostringstream name;
        name << std::setfill('0') << baseline_prefix << '-' << std::setw(2) << mod_num << ".csv";
        std::ofstream out(name.str());
        out << "sample, time,";

        for (size_t idx = 0; idx < channels.size(); idx++) {
            if (idx != channels.size() - 1)
                out << "Chan" << channels[idx] << ",";
            else
                out << "Chan" << channels[idx] << std::endl;
        }

        for (unsigned int sample = 0; sample < baselines.front().size(); sample++) {
            out << sample << "," << std::get<0>(baselines.front()[sample]) << ",";
            for (unsigned int idx = 0; idx < channels.size(); idx++) {
                if (idx != channels.size() - 1)
                    out << std::get<1>(baselines[channels[idx]][sample]) << ",";
                else
                    out << std::get<1>(baselines[channels[idx]][sample]) << std::endl;
            }
        }
    }
}

static void boot(
    xia::pixie::crate::crate& crate, args_commands_iter& , args_commands_iter& , bool ) {
    xia::util::timepoint tp;
    std::cout << "booting crate" << std::endl;
    tp.start();
    crate.boot();
    tp.end();
    std::cout << "boot time=" << tp << std::endl;
}

static void crate_report(
    xia::pixie::crate::crate& crate, args_commands_iter& , args_commands_iter& , bool ) {
    std::cout << crate << std::endl;
}

static void export_(
    xia::pixie::crate::crate& crate, args_commands_iter& ci, args_commands_iter& ce,
    bool verbose) {
    if (!valid_option(ci, ce, 1)) {
        throw std::runtime_error("export: not enough options");
    }
    auto file_opt = get_and_next(ci, ce);
    xia::util::timepoint tp;
    if (verbose) {
        tp.start();
    }
    crate.export_config(file_opt);
    if (verbose) {
        tp.end();
        std::cout << "Modules export time=" << tp << std::endl;
    }
}

static void help(
    xia::pixie::crate::crate& , args_commands_iter& ci, args_commands_iter& ce,
    bool verbose) {
    auto long_opt = switch_option("-l", ci, ce, false);
    std::cout << "Command help:" << std::endl
              << " 'modules(s)' and 'channel(s)' can be a number or series:" << std::endl
              << "   eg '0' or '3,4,6' or '3,4-5,10,20-22'" << std::endl;
    args_command help_opt;
    if (args_count(ci, ce) >= 1) {
        help_opt = get_and_next(ci, ce);
    }
    auto mi = std::max_element(
        commands.begin(), commands.end(), [](auto& a, auto& b) {
            return (std::get<0>(a).size() < std::get<0>(b).size());
        });
    auto max = std::get<0>(*mi).size();
    auto cmds = std::vector<args_command>();
    if (!help_opt.empty()) {
        cmds.push_back(help_opt);
    } else {
        for (auto& c : commands) {
            cmds.push_back(std::get<0>(c));
        }
    }
    for (auto& c : cmds) {
        auto& cmd = std::get<1>(*find_command(c));
        if (long_opt == "true") {
            std::cout << std::left << cmd.name << " : ";
            for (auto& a : cmd.aliases) {
                std::cout << a << ' ';
            }
            std::cout << std::endl
                      << " " << cmd.help << std::endl
                      << "  # " << cmd.help_cmd << std::endl;
        } else {
            std::cout << std::left << std::setw(max + 1) << cmd.name
                      << " - " << cmd.help
                      << std::endl;
        }
    }
}

static void hist_resume(
    xia::pixie::crate::crate& crate, args_commands_iter& ci, args_commands_iter& ce,
    bool verbose) {
    if (!valid_option(ci, ce, 1)) {
        throw std::runtime_error("hist-resume: not enough options");
    }
    auto mod_nums_opt = get_and_next(ci, ce);
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        using namespace xia::pixie::hw::run;
        crate[mod_num].start_histograms(run_mode::resume);
    }
}

static void hist_save(
    xia::pixie::crate::crate& crate, args_commands_iter& ci, args_commands_iter& ce,
    bool verbose) {
    auto bins_opt = switch_option("-b", ci, ce);
    if (!valid_option(ci, ce, 1)) {
        throw std::runtime_error("hist-save: not enough options");
    }
    auto mod_nums_opt = get_and_next(ci, ce);
    args_command chans_opt;
    if (valid_option(ci, ce, 1)) {
        chans_opt = get_and_next(ci, ce);
    }
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        using namespace xia::pixie::hw::run;
        xia::pixie::channel::range channels;
        channels_option(channels, chans_opt, crate[mod_num].num_channels);
        size_t length = crate[mod_num].num_channels;
        if (!bins_opt.empty()) {
            length = get_value<size_t>(bins_opt);
        }
        std::vector<xia::pixie::hw::words> histos;
        for (auto channel : channels) {
            xia::pixie::hw::words histogram(length);
            crate[mod_num].read_histogram(channel, histogram);
            histos.push_back(histogram);
        }
        std::ostringstream name;
        name << std::setfill('0') << histogram_prefix << '-' << std::setw(2) << mod_num << ".csv";
        std::ofstream out(name.str());
        out << "bin,";
        for (size_t idx = 0; idx < channels.size(); idx++) {
            if (idx != channels.size() - 1)
                out << "Chan" << channels[idx] << ",";
            else
                out << "Chan" << channels[idx] << std::endl;
        }
        for (unsigned int bin = 0; bin < length; bin++) {
            out << bin << ",";
            for (size_t idx = 0; idx < histos.size(); idx++) {
                if (idx != histos.size() - 1)
                    out << histos[idx][bin] << ",";
                else
                    out << histos[idx][bin] << std::endl;
            }
        }
    }
}

static void hist_start(
    xia::pixie::crate::crate& crate, args_commands_iter& ci, args_commands_iter& ce,
    bool verbose) {
    if (!valid_option(ci, ce, 1)) {
        throw std::runtime_error("hist-start: not enough options");
    }
    auto mod_nums_opt = get_and_next(ci, ce);
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        using namespace xia::pixie::hw::run;
        crate[mod_num].start_histograms(run_mode::new_run);
    }
}

static void import(
    xia::pixie::crate::crate& crate, args_commands_iter& ci, args_commands_iter& ce,
    bool verbose) {
    if (!valid_option(ci, ce, 1)) {
        throw std::runtime_error("import: not enough options");
    }
    auto path_opt = get_and_next(ci, ce);
    xia::util::timepoint tp;
    xia::pixie::module::number_slots modules;
    tp.start();
    crate.import_config(path_opt, modules);
    crate.initialize_afe();
    tp.end();
    std::cout << "Modules imported: " << modules.size()
              << " time=" << tp << std::endl;
}

struct list_save_worker : public module_thread_worker {
    std::string name;
    size_t seconds;
    bool run_task;

    list_save_worker();
    void worker(xia::pixie::module::module& module);
};

list_save_worker::list_save_worker() : seconds(0) {}

void list_save_worker::worker(xia::pixie::module::module& module) {
    name += '-' + std::to_string(module.number) + ".lmd";
    std::ofstream out(name, std::ios::binary);
    if (!out) {
        throw std::runtime_error(
            std::string("list mode file open: ") + name + ": " +
            std::strerror(errno));
    }
    using namespace xia::pixie::hw::run;
    if (run_task) {
        module.start_listmode(run_mode::new_run);
    }
    xia::pixie::hw::words lm;
    const size_t poll_period_usecs = 100 * 1000;
    total = 0;
    period.start();
    while (period.secs() < seconds) {
        lm.clear();
        if (module.read_list_mode(lm) > 0) {
            total += lm.size();
            out.write(
                reinterpret_cast<char*>(lm.data()),
                lm.size() * sizeof(xia::pixie::hw::word));
        } else {
            xia::pixie::hw::wait(poll_period_usecs);
        }
    }
    if (run_task) {
        module.run_end();
        lm.clear();
        if (module.read_list_mode(lm) > 0) {
            total += lm.size();
            out.write(
                reinterpret_cast<char*>(lm.data()),
                lm.size() * sizeof(xia::pixie::hw::word));
        }
        std::cout << "list-mode: " << module.number
                  << ": " << module.run_stats.output() << std::endl;
        if (module.run_stats.hw_overflows != 0) {
            throw std::runtime_error(
                "list mode: EXT FIFO overflow (check workflow config)");
        }
        if (module.run_stats.overflows != 0) {
            throw std::runtime_error(
                "list mode: data FIFO overflow (check buffer sizes)");
        }
        if (module.run_stats.in != module.run_stats.out) {
            throw std::runtime_error(
                "list mode: data left in data FIFO");
        }
    }
    period.end();
}

static void list_mode_command(
    xia::pixie::crate::crate& crate, args_commands_iter& ci, args_commands_iter& ce, bool run_task) {
    if (!valid_option(ci, ce, 3)) {
        throw std::runtime_error("list-[save,mode]: not enough options");
    }
    auto mod_nums_opt = get_and_next(ci, ce);
    auto secs_opt = get_and_next(ci, ce);
    auto name_opt = get_and_next(ci, ce);
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    auto secs = get_value<size_t>(secs_opt);
    auto name = name_opt;
    module_check(crate, mod_nums);
    if (secs == 0) {
        throw std::runtime_error(std::string("list mode run/save period is 0"));
    }
    auto saves = std::vector<list_save_worker>(mod_nums.size());
    set_num_slot(crate, mod_nums, saves);
    for (auto& s : saves) {
        s.name = name;
        s.seconds = secs;
        s.run_task = run_task;
    };
    module_threads(crate, mod_nums, saves, "list mode command error; see log");
    performance_stats(saves);
}

static void list_mode(
    xia::pixie::crate::crate& crate, args_commands_iter& ci, args_commands_iter& ce,
    bool verbose) {
    list_mode_command(crate, ci, ce, true);
}

static void list_resume(
    xia::pixie::crate::crate& crate, args_commands_iter& ci, args_commands_iter& ce,
    bool verbose) {
    if (!valid_option(ci, ce, 1)) {
        throw std::runtime_error("list-resume: not enough options");
    }
    auto mod_nums_opt = get_and_next(ci, ce);
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        using namespace xia::pixie::hw::run;
        crate[mod_num].start_listmode(run_mode::resume);
    }
}

static void list_save(
    xia::pixie::crate::crate& crate, args_commands_iter& ci, args_commands_iter& ce,
    bool verbose) {
    list_mode_command(crate, ci, ce, false);
}

static void list_start(
    xia::pixie::crate::crate& crate, args_commands_iter& ci, args_commands_iter& ce,
    bool verbose) {
    if (!valid_option(ci, ce, 1)) {
        throw std::runtime_error("list-start: not enough options");
    }
    auto mod_nums_opt = get_and_next(ci, ce);
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        using namespace xia::pixie::hw::run;
        crate[mod_num].start_listmode(run_mode::new_run);
    }
}

static void lset_import(
    xia::pixie::crate::crate& crate, args_commands_iter& ci, args_commands_iter& ce,
    bool verbose) {
    if (!valid_option(ci, ce, 2)) {
        throw std::runtime_error("lset-import: not enough options");
    }
    auto mod_nums_opt = get_and_next(ci, ce);
    auto settings_opt = get_and_next(ci, ce);
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    std::string action;
    if (valid_option(ci, ce, 1)) {
        auto action_opt = get_and_next(ci, ce);
        if (action_opt == "flush" || action_opt == "sync") {
            action = action_opt;
        } else {
            throw std::runtime_error(
                std::string("invalid post settings import operation: " + action_opt));
        }
    }
    for (auto mod_num : mod_nums) {
        xia::pixie::module::module& module = crate[mod_num];
        xia::pixie::legacy::settings settings(module);
        settings.load(settings_opt);
        settings.import(module);
        if (module.online() && !action.empty()) {
            if (action == "flush") {
                module.sync_vars();
            } else if (action == "sync") {
                module.sync_vars();
                module.sync_hw();
            }
        }
    }
}

static void lset_load(
    xia::pixie::crate::crate& crate, args_commands_iter& ci, args_commands_iter& ce,
    bool verbose) {
    if (!valid_option(ci, ce, 2)) {
        throw std::runtime_error("lset-load: not enough options");
    }
    auto mod_nums_opt = get_and_next(ci, ce);
    auto settings_opt = get_and_next(ci, ce);
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    std::string action;
    if (valid_option(ci, ce, 1)) {
        auto action_opt = get_and_next(ci, ce);
        if (action_opt == "flush" || action_opt == "sync") {
            action = action_opt;
        } else {
            throw std::runtime_error(
                std::string("invalid post settings import operation: " +
                            action_opt));
        }
    }
    for (auto mod_num : mod_nums) {
        xia::pixie::module::module& module = crate[mod_num];
        xia::pixie::legacy::settings settings(module);
        settings.load(settings_opt);
        settings.write(module);
        if (module.online() && !action.empty()) {
            if (action == "flush") {
                module.sync_vars();
            } else if (action == "sync") {
                module.sync_vars();
                module.sync_hw();
            }
        }
    }
}

static void lset_report(
    xia::pixie::crate::crate& crate, args_commands_iter& ci, args_commands_iter& ce,
    bool verbose) {
    if (!valid_option(ci, ce, 2)) {
        throw std::runtime_error("lset-report: not enough options");
    }
    auto mod_nums_opt = get_and_next(ci, ce);
    auto settings_opt = get_and_next(ci, ce);
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        xia::pixie::legacy::settings settings(crate[mod_num]);
        settings.load(settings_opt);
        std::cout << settings;
    }
}

static void get_memory_reg(
    const std::string& mem_reg, std::string& mem, std::string& reg,
    const std::string& label) {
    reg = mem_reg;
    tolower(reg);
    auto has_mem_field = mem_reg.find(':') != std::string::npos;
    if (has_mem_field) {
        xia::util::strings mr;
        xia::util::split(mr, mem_reg, ':');
        if (mr.size() != 2) {
            throw std::runtime_error(
                label + ": invalid reg format: " + mem_reg);
        }
        const auto& mems = hardware["memory"];
        if (!mems.contains(mr[0])) {
            throw std::runtime_error(
                label + ": invalid memory: " + mr[0]);
        }
        mem = mr[0];
        reg = mr[1];
    } else {
        mem = "sys";
    }
}

static xia::pixie::hw::address get_address(
    const std::string& mem, const std::string& reg, const std::string& label) {
    xia::pixie::hw::address address = 0;
    bool number = true;
    try {
        address = std::stoul(reg, nullptr, 0);
    } catch (std::invalid_argument e) {
        number = false;
    } catch (std::out_of_range e) {
        throw std::runtime_error(label + ": reg value of range: " + reg);
    }
    const auto& mems = hardware["memory"];
    if (!mems.contains(mem)) {
        throw std::runtime_error(
            label + ": invalid memory type: " + mem);
    }
    if (!number) {
        const auto& mem_regs = mems[mem];
        if (!mem_regs.contains(reg)) {
            throw std::runtime_error(label + ": " + mem + " reg not found: " + reg);
        }
        address = mem_regs[reg];
    }
    return address;
}

static void reg_read(
    xia::pixie::crate::crate& crate, args_commands_iter& ci, args_commands_iter& ce,
    bool verbose) {
    const std::string label = "reg-read";
    auto slot_opt = switch_option("-s", ci, ce, false);
    auto hex_opt = switch_option("-x", ci, ce, false);
    if (!valid_option(ci, ce, 2)) {
        throw std::runtime_error("reg-read: not enough options");
    }
    auto mod_slot_opt = get_and_next(ci, ce);
    auto reg_opt = get_and_next(ci, ce);
    size_t mod_slot = get_value<size_t>(mod_slot_opt);
    std::string mem;
    std::string reg;
    get_memory_reg(reg_opt, mem, reg, label);
    xia::pixie::hw::address address = get_address(mem, reg, label);
    if (slot_opt == "true") {
        auto mod = crate.find(mod_slot);
        mod_slot = mod->number;
    }
    auto& mod = crate[mod_slot];
    xia::pixie::hw::word value;
    if (mem == "sys") {
        value =  mod.read_word(address);
    } else if (mem == "fippi") {
        auto fippi = xia::pixie::hw::memory::fippi(mod);
        value = fippi.read(address);
    } else if (mem == "dsp") {
        auto dsp = xia::pixie::hw::memory::dsp(mod);
        value = dsp.read(address);
    }
    if (hex_opt == "true") {
        std::cout << std::hex << "0x";
    }
    std::cout << value << std::endl;
    if (hex_opt == "true") {
        std::cout << std::dec;
    }
}

static void par_read(
    xia::pixie::crate::crate& crate, args_commands_iter& ci, args_commands_iter& ce,
    bool verbose) {
    if (!valid_option(ci, ce, 2)) {
        throw std::runtime_error("par-write: not enough options");
    }
    auto mod_nums_opt = get_and_next(ci, ce);
    auto param_opt = get_and_next(ci, ce);
    args_command chans_opt;
    if (valid_option(ci, ce, 1)) {
        chans_opt = get_and_next(ci, ce);
    }
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        if (chans_opt.empty()) {
            std::cout << "# module param read: " << mod_num
                      << ": " << param_opt << std::endl;
            if (param_opt == "all") {
                for (auto& par : xia::pixie::param::get_module_param_map()) {
                    try {
                        output_value(
                            par.first, crate[mod_num].read(par.second));
                    } catch (error& e) {
                        if (e.type != error::code::module_param_disabled &&
                            e.type != error::code::module_param_writeonly) {
                            throw;
                        }
                    }
                }
            } else {
                output_value(param_opt, crate[mod_num].read(param_opt));
            }
        } else {
            xia::pixie::channel::range channels;
            channels_option(channels, chans_opt, crate[mod_num].num_channels);
            for (auto channel_num : channels) {
                std::cout << "# channel param read: " << mod_num
                          << ':' << channel_num << ": "
                          << param_opt << std::endl;
                if (param_opt == "all") {
                    for (auto& par : xia::pixie::param::get_channel_param_map()) {
                        try {
                            output_value(
                                par.first, crate[mod_num].read(
                                    par.second, channel_num));
                        } catch (error& e) {
                            if (e.type != error::code::channel_param_disabled &&
                                e.type != error::code::channel_param_writeonly) {
                                throw;
                            }
                        }
                    }
                } else {
                    output_value(
                        param_opt, crate[mod_num].read(param_opt, channel_num));
                }
            }
        }
    }
}

static void par_write(
    xia::pixie::crate::crate& crate, args_commands_iter& ci, args_commands_iter& ce,
    bool verbose) {
    if (!valid_option(ci, ce, 3)) {
        throw std::runtime_error("par-write: not enough options");
    }
    auto mod_nums_opt = get_and_next(ci, ce);
    auto param_opt = get_and_next(ci, ce);
    auto value_opt = get_and_next(ci, ce);
    args_command chans_opt;
    if (valid_option(ci, ce, 1)) {
        chans_opt = value_opt;
        value_opt = get_and_next(ci, ce);
    }
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    auto value = get_value<xia::pixie::param::value_type>(value_opt);
    for (auto mod_num : mod_nums) {
        if (chans_opt.empty()) {
            crate[mod_num].write(param_opt, value);
        } else {
            xia::pixie::channel::range channels;
            channels_option(channels, chans_opt, crate[mod_num].num_channels);
            for (auto channel_num : channels) {
                crate[mod_num].write(param_opt, channel_num, value);
            }
        }
    }
}

static void report(
    xia::pixie::crate::crate& crate, args_commands_iter& ci, args_commands_iter& ce,
    bool verbose) {
    if (!valid_option(ci, ce, 1)) {
        throw std::runtime_error("report: not enough options");
    }
    auto file_opt = get_and_next(ci, ce);
    std::ofstream output_file(file_opt);
    if (!output_file) {
        throw std::runtime_error(
            std::string("opening report: " + file_opt + ": " + std::strerror(errno)));
    }
    crate.report(output_file);
}

static void run_active(
    xia::pixie::crate::crate& crate, args_commands_iter& ci, args_commands_iter& ce,
    bool verbose) {
    if (!valid_option(ci, ce, 1)) {
        throw std::runtime_error("run-active: not enough options");
    }
    auto mod_nums_opt = get_and_next(ci, ce);
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        std::cout << "module=" << mod_num << " run-active=" << std::boolalpha
                  << crate[mod_num].run_active() << std::endl;
    }
}

static void run_end(
    xia::pixie::crate::crate& crate, args_commands_iter& ci, args_commands_iter& ce,
    bool verbose) {
    if (!valid_option(ci, ce, 1)) {
        throw std::runtime_error("run-end: not enough options");
    }
    auto mod_nums_opt = get_and_next(ci, ce);
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        crate[mod_num].run_end();
    }
}

static void set_dacs(
    xia::pixie::crate::crate& crate, args_commands_iter& ci, args_commands_iter& ce,
    bool verbose) {
    if (!valid_option(ci, ce, 1)) {
        throw std::runtime_error("set-dacs: not enough options");
    }
    auto mod_nums_opt = get_and_next(ci, ce);
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        crate[mod_num].set_dacs();
    }
}

static void stats(
    xia::pixie::crate::crate& crate, args_commands_iter& ci, args_commands_iter& ce,
    bool verbose) {
    auto stat_opt = switch_option("-s", ci, ce);
    if (!valid_option(ci, ce, 1)) {
        throw std::runtime_error("stats: not enough options");
    }
    auto mod_nums_opt = get_and_next(ci, ce);
    args_command chans_opt;
    if (valid_option(ci, ce, 1)) {
        mod_nums_opt = get_and_next(ci, ce);
    }
    if (valid_option(ci, ce, 1)) {
        chans_opt = get_and_next(ci, ce);
    }
    std::string stat = "all";
    if (!stat_opt.empty()) {
        if (stat_opt == "all" ||
            stat_opt == "pe" || stat_opt == "ocr" ||
            stat_opt == "rt" || stat_opt == "lt") {
            stat = stat_opt;
        } else {
            throw std::runtime_error("invalid stat type: " + stat_opt);
        }
    }
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        xia::pixie::channel::range channels;
        channels_option(channels, chans_opt, crate[mod_num].num_channels);
        xia::pixie::stats::stats stats(crate[mod_num]);
        crate[mod_num].read_stats(stats);
        if (stat == "all" || stat == "pe") {
            std::cout << "module " << mod_num
                      << ": processed-events="
                      << stats.mod.processed_events() << std::endl;
        }
        if (stat == "all" || stat == "icr") {
            for (auto channel : channels) {
                std::cout << "module " << mod_num << " chan " << channel
                          << ": input-count-rate="
                          << stats.chans[channel].input_count_rate()
                          << std::endl;
            }
        }
        if (stat == "all" || stat == "ocr") {
            for (auto channel : channels) {
                std::cout << "module " << mod_num << " chan " << channel
                          << ": output-count-rate="
                          << stats.chans[channel].output_count_rate()
                          << std::endl;
            }
        }
        if (stat == "all" || stat == "rt") {
            std::cout << "module " << mod_num
                      << ": real-time=" << stats.mod.real_time()
                      << std::endl;
        }
        if (stat == "all" || stat == "lt") {
            for (auto channel : channels) {
                std::cout << "module " << mod_num << " chan " << channel
                          << ": live-time="
                          << stats.chans[channel].live_time() << std::endl;
            }
        }
    }
}

struct test_fifo_worker : public module_thread_worker {
    size_t length;

    test_fifo_worker();
    void worker(xia::pixie::module::module& module);
};

test_fifo_worker::test_fifo_worker() : length(0) {}

void test_fifo_worker::worker(xia::pixie::module::module& module) {
    try {
        std::ofstream out(list_mode_prefix + "-control-task-11.lmd", std::ios::out | std::ios::binary);
        module.start_test(xia::pixie::module::module::test::lm_fifo);
        const size_t poll_period_usecs = 10 * 1000;
        period.start();
        while (total < length) {
            size_t data_available = module.read_list_mode_level();
            if (data_available > 0) {
                xia::pixie::hw::words lm;
                module.read_list_mode(lm);
                total += lm.size();
                out.write(reinterpret_cast<char*>(lm.data()), lm.size() * sizeof(uint32_t));
            }
            if (data_available == 0) {
                xia::pixie::hw::wait(poll_period_usecs);
            }
        }
        period.end();
        module.end_test();
        out.close();
    } catch (...) {
        period.end();
        has_error = true;
        module.end_test();
        throw;
    }
}

static void test(
    xia::pixie::crate::crate& crate, args_commands_iter& ci, args_commands_iter& ce,
    bool verbose) {
    auto mode_opt = switch_option("-m", ci, ce);
    if (!valid_option(ci, ce, 1)) {
        throw std::runtime_error("test: not enough options");
    }
    auto mod_nums_opt = get_and_next(ci, ce);
    xia::pixie::module::module::test mode = xia::pixie::module::module::test::off;
    if (!mode_opt.empty()) {
        if (mode_opt == "lmfifo") {
            mode = xia::pixie::module::module::test::lm_fifo;
        } else if (mode_opt != "off") {
            throw std::runtime_error(std::string("invalid test mode: " + mode_opt));
        }
    }
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    size_t bytes = 500 * 1024 * 1000;
    auto tests = std::vector<test_fifo_worker>(mod_nums.size());
    set_num_slot(crate, mod_nums, tests);
    for (auto& t : tests) {
        t.length = (bytes) / sizeof(xia::pixie::hw::word);
    }
    std::cout << "Test: " << mode_opt << " length=" << xia::util::humanize(bytes) << std::endl;
    module_threads(crate, mod_nums, tests, "fifo test error; see log");
    performance_stats(tests, true);
}

static void var_read(
    xia::pixie::crate::crate& crate, args_commands_iter& ci, args_commands_iter& ce,
    bool verbose) {
    if (!valid_option(ci, ce, 2)) {
        throw std::runtime_error("var-read: not enough options");
    }
    auto mod_nums_opt = get_and_next(ci, ce);
    auto param_opt = get_and_next(ci, ce);
    args_command chans_opt;
    args_command offsets_opt = "0";
    if (valid_option(ci, ce, 1)) {
        chans_opt = get_and_next(ci, ce);
    }
    if (valid_option(ci, ce, 1)) {
        offsets_opt = get_and_next(ci, ce);
    }
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    auto offsets = get_values<size_t>(offsets_opt);
    for (auto mod_num : mod_nums) {
        if (chans_opt.empty()) {
            if (param_opt == "all") {
                std::cout << "# module var read: " << mod_num << ": " << param_opt << std::endl;
                for (auto& var : crate[mod_num].module_var_descriptors) {
                    try {
                        output_value(var.name, crate[mod_num].read_var(var.par));
                    } catch (error& e) {
                        if (e.type != error::code::module_param_disabled &&
                            e.type != error::code::module_param_writeonly) {
                            throw;
                        }
                    }
                }
            } else {
                output_value(param_opt, crate[mod_num].read_var(param_opt, 0));
            }
        } else {
            xia::pixie::channel::range channels;
            channels_option(channels, chans_opt, crate[mod_num].num_channels);
            if (param_opt == "all") {
                for (auto channel : channels) {
                    std::cout << "# channel var read: " << mod_num << ':' << channel << ": "
                              << param_opt << std::endl;
                    for (auto& var : crate[mod_num].channel_var_descriptors) {
                        for (auto offset : offsets) {
                            try {
                                output_value(
                                    var.name, crate[mod_num].read_var(
                                        var.par, channel, offset));
                            } catch (error& e) {
                                if (e.type != error::code::channel_param_disabled &&
                                    e.type != error::code::channel_param_writeonly) {
                                    throw;
                                }
                            }
                        }
                    }
                }
            } else {
                for (auto channel : channels) {
                    for (auto offset : offsets) {
                        output_value(
                            param_opt, crate[mod_num].read_var(
                                param_opt, channel, offset));
                    }
                }
            }
        }
    }
}

static void var_write(
    xia::pixie::crate::crate& crate, args_commands_iter& ci, args_commands_iter& ce,
    bool verbose) {
    if (!valid_option(ci, ce, 3)) {
        throw std::runtime_error("var-write: not enough options");
    }
    auto mod_nums_opt = get_and_next(ci, ce);
    auto param_opt = get_and_next(ci, ce);
    auto value_opt = get_and_next(ci, ce);
    args_command chans_opt;
    args_command offsets_opt = "0";
    if (valid_option(ci, ce, 1)) {
        chans_opt = value_opt;
        value_opt = get_and_next(ci, ce);
    }
    if (valid_option(ci, ce, 1)) {
        offsets_opt = value_opt;
        value_opt = get_and_next(ci, ce);
    }
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    auto offsets = get_values<size_t>(offsets_opt);
    auto value = get_value<xia::pixie::param::value_type>(value_opt);
    for (auto mod_num : mod_nums) {
        if (chans_opt.empty()) {
            if (param_opt == "all") {
                for (auto& var : crate[mod_num].module_var_descriptors) {
                    try {
                        crate[mod_num].write_var(var.par, value);
                    } catch (error& e) {
                        if (e.type != error::code::module_param_disabled &&
                            e.type != error::code::module_param_readonly) {
                            throw;
                        }
                    }
                }
            } else {
                crate[mod_num].read_var(param_opt, value);
            }
        } else {
            xia::pixie::channel::range channels;
            channels_option(channels, chans_opt, crate[mod_num].num_channels);
            if (param_opt == "all") {
                for (auto channel : channels) {
                    for (auto& var : crate[mod_num].channel_var_descriptors) {
                        for (auto offset : offsets) {
                            try {
                                crate[mod_num].write_var(
                                    var.par, channel, offset, value);
                            } catch (error& e) {
                                if (e.type != error::code::channel_param_disabled &&
                                    e.type != error::code::channel_param_readonly) {
                                    throw;
                                }
                            }
                        }
                    }
                }
            } else {
                for (auto channel : channels) {
                    for (auto offset : offsets) {
                        crate[mod_num].write_var(
                            param_opt, channel, offset, value);
                    }
                }
            }
        }
    }
}

static void wait(
    xia::pixie::crate::crate&, args_commands_iter& ci, args_commands_iter& ce,
    bool verbose) {
    if (!valid_option(ci, ce, 1)) {
        throw std::runtime_error("wait: not enough options");
    }
    auto period_opt = get_and_next(ci, ce);
    auto msecs = get_value<size_t>(period_opt);
    if (verbose) {
        std::cout << "waiting " << msecs << " msecs" << std::endl;
    }
    xia::pixie::hw::wait(msecs * 1000);
}

int main(int argc, char* argv[]) {
    args_parser parser("Pixie-16 OmniTool ");

    parser.helpParams.addDefault = true;
    parser.helpParams.addChoices = true;

    args_group option_group(parser, "Options");
    args_help_flag help(option_group, "help", "Display this help menu", {'h', "help"});
    args_flag debug_flag(
        option_group, "debug_flag", "Enable debug log level", {'d', "debug"},
        false);
    args_flag quiet_flag(
        option_group, "quiet_flag", "Quiet output", {'q', "quiet"},
        false);
    args_flag throw_unhandled_flag(
        option_group, "throw_unhandled_flag",
        "Throw an unhandled exception, it will detail the exception",
        {'t', "throw-unhandled"}, false);
    args_flag reg_trace(
        option_group, "reg_trace", "Registers debugging traces in the API.",
        {'R', "reg-trace"});
    args_flag simulate(
        option_group, "simulate", "Simulate the crate and modules",
        {'S', "simulate"}, false);
    args_size_flag num_modules_flag(
        option_group, "num_modules_flag",
        "Number of modules to report", {'n', "num-modules"}, 0);
    args_string_flag module_defs(
        option_group, "module_file_flag",
        "Crate simulation module definition file to load. "
        "The file contains the module to simulate.",
        {'M', "modules"});
    args_string_flag fw_host_path_flag(
        option_group, "fw_host_path_flag",
        "Path to the host installed firmware.",
        {'H', "hostpath"});
    args_strings_flag fw_file_flag(
        option_group, "fw_file_flag",
        "Firmware file(s) to load. Can be repeated. "
        "Takes the form rev:mod-rev-num:type:name"
        "Ex. r33339:15:sys:syspixie16_revfgeneral_adc250mhz_r33339.bin",
        {'F', "firmware"});
    args_strings_flag crate_file_flag(
        option_group, "crate_file_flag",
        "Crate firmware file to load. "
        "The file contain the list of firmware files.",
        {'C', "crate"});
    args_string_flag log_file_flag(
        option_group, "log_file_flag",
        "Log file. Use `stdout` for the console.", {'l', "log"});
    args_strings_flag slot_map_flag(
        option_group, "slot_map_flag",
        "A list of slots used to define the slot to index mapping. "
        " Use this option to limit slots available to use.",
        {'s', "slot_map"});
    args_strings_flag cmd_file_flag(
        option_group, "cmd_file_flag",
        "File of commands to execute.", {'c', "cmd"});

    args_group command_group(parser, "Commands");
    args_positional_list args_cmds(
        command_group, "commands",
        "Commands to be performed in order. Prefix with '--'. "
        "The command 'help' lists the available commands.");

    try {
        parser.ParseCLI(argc, argv);
    } catch (args::Help& help) {
        std::cout << help.what() << std::endl;
        std::cout << parser;
        help_output(std::cout);
        return EXIT_SUCCESS;
    } catch (args::Error& e) {
        std::cout << e.what() << std::endl;
        std::cout << parser;
        help_output(std::cout);
        return EXIT_FAILURE;
    }

    try {
        xia::util::timepoint run;
        run.start();

        std::string log;
        if (log_file_flag) {
            log = args::get(log_file_flag);
        } else {
            log = "pixie16-omnitool.log";
        }

        auto log_level = xia::log::info;
        if (args::get(debug_flag)) {
            log_level = xia::log::debug;
        }
        xia::logging::start("log", log, false);
        xia::logging::set_level(log_level);

        process_command_options process_opts;

        process_opts.verbose = !args::get(quiet_flag);
        process_opts.reg_trace = args::get(reg_trace);
        process_opts.num_modules = args::get(num_modules_flag);

        xia::pixie::crate::crate crate_hw;
        xia::pixie::sim::crate crate_sim;

        xia::pixie::crate::crate* crate_selection = &crate_hw;

        if (simulate) {
            if (!module_defs) {
                throw std::runtime_error("simulation requires a module definition");
            }
            xia_log(xia::log::info) << "simulation: " << args::get(module_defs);
            xia::pixie::sim::load_module_defs(args::get(module_defs));
            crate_selection = &crate_sim;
        }

        xia::pixie::crate::crate& crate = *crate_selection;

        if (fw_file_flag) {
            for (const auto& fw : args::get(fw_file_flag)) {
                process_opts.firmware_files.push_back(fw);
            }
        }

        if (crate_file_flag) {
            for (const auto& fw : args::get(crate_file_flag)) {
                process_opts.firmware_crate_files.push_back(fw);
            }
        }

        if (slot_map_flag) {
            for (const auto& slot : args::get(slot_map_flag)) {
                slot_range ss = get_values<int>(slot, xia::pixie::hw::max_slots);
                process_opts.slots.insert(
                    std::end(process_opts.slots), std::begin(ss), std::end(ss));
            }
        }

        if (fw_host_path_flag) {
            process_opts.firmware_host_path = args::get(fw_host_path_flag);
        } else {
#if WIN32
            process_opts.firmware_host_path = "c:/xia/pixie/firmware";
#else
            process_opts.firmware_host_path = "/usr/local/xia/pixie/firmware";
#endif
        }

        args_commands cmds;
        for (auto& cmd : args_cmds) {
            cmds.push_back(cmd);
        }
        if (cmd_file_flag) {
            for (const auto& name : args::get(cmd_file_flag)) {
                load_commands(name, cmds);
            }
        }

        process_commands(crate, process_opts, cmds);

        run.end();
        if (process_opts.verbose) {
            std::cout << "run time=" << run << std::endl;
        }
    } catch (xia::pixie::error::error& e) {
        xia_log(xia::log::error) << e;
        std::cerr << e << std::endl;
        return e.return_code();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << e.what();
        std::cerr << "error: " << e.what() << std::endl;
        return xia::pixie::error::api_result_unknown_error();
    } catch (...) {
        if (args::get(throw_unhandled_flag)) {
            throw;
        }
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        std::cerr << "error: unknown error: unhandled exception" << std::endl;
        return xia::pixie::error::api_result_unknown_error();
    }

    return EXIT_SUCCESS;
}
