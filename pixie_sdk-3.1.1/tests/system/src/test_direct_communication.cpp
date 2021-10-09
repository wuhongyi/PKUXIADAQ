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

/** @file test_direct_communication.cpp
 * @brief Used to test direct communication with memory registers on the system.
 */

#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>

#include "args/args.hxx"
#include "pixie16app_export.h"
#include "pixie16sys_export.h"
#include "system_test_utilities.hpp"

#ifdef _WINDOWS
#include <windows.h>
#endif


/**
 * @brief A small configuration class to hold information useful to the legacy API.
 */
class Configuration {
public:
    unsigned short numModules;
    unsigned short* slot_map;
    std::string ComFPGAConfigFile;
    std::string SPFPGAConfigFile;
    std::string TrigFPGAConfigFile;
    std::string DSPCodeFile;
    std::string DSPParFile;
    std::string DSPVarFile;
};

/**
 * @brief Reads the provided configuration file into the Configuration class.
 * @param[in] config_file_name The name of the configuration file we'll open.
 * @return A populated instance of the Configuration class.
 */
Configuration read_configuration_file(const std::string& config_file_name) {
    std::ifstream input(config_file_name, std::ios::in);

    if (input.fail()) {
        std::stringstream errmsg;
        errmsg << "Could not open " << config_file_name << "!";
        throw std::invalid_argument(errmsg.str());
    }

    Configuration cfg;
    input >> cfg.numModules;
    cfg.slot_map = new unsigned short[cfg.numModules + 1];
    for (size_t i = 0; i < cfg.numModules; i++)
        input >> cfg.slot_map[i];

    input >> cfg.ComFPGAConfigFile >> cfg.SPFPGAConfigFile >> cfg.TrigFPGAConfigFile >>
        cfg.DSPCodeFile >> cfg.DSPParFile >> cfg.DSPVarFile;

    input.close();
    return cfg;
}

int main(int argc, char* argv[]) {
    args::ArgumentParser parser("Tests direct communication with the system.");
    parser.helpParams.addDefault = true;
    parser.helpParams.addChoices = true;

    args::Group commands(parser, "commands");
    args::Command boot(commands, "boot", "Just boots the system");
    args::Command dsp(commands, "dsp", "Tests related to the DSP.");
    args::Command mca(commands, "mca", "Tests related to the external memory.");
    args::Command external_fifo(commands, "external_fifo",
                                "Reads external FIFO status and returns the number of 32-bit"
                                " words it contains.");
    args::Command raw(commands, "raw", "Access the raw register IO on the system");
    args::Command csr(commands, "csr", "Access the CSR");

    args::Group arguments(parser, "arguments", args::Group::Validators::AtLeastOne,
                          args::Options::Global);
    args::Positional<std::string> configuration(arguments, "cfg", "The configuration file to load.",
                                                args::Options::Required);

    args::ValueFlag<unsigned int> module_number_flag(
        arguments, "module_number", "The module number to work with.", {'m', "module"}, 0);
    args::ValueFlag<std::string> address_flag(arguments, "address",
                                              "The memory address to operate on in hex.",
                                              {'a', "address"}, "0x10073D");
    args::ValueFlag<std::string> boot_pattern_flag(arguments, "boot_pattern",
                                                   "The boot pattern used for booting.",
                                                   {'b', "boot_pattern"}, "0x7F");
    args::Flag clear(mca, "clear", "Clears the main memory", {'c', "clear"});
    args::Flag is_dry_run(arguments, "dry_run", "Control command execution.", {"dry_run"});
    args::HelpFlag help_flag(arguments, "help", "Displays this message", {'h', "help"});
    args::ValueFlag<unsigned int> loop_flag(arguments, "num_loops",
                                            "How many times we perform write/read test.",
                                            {'l', "num_loops"}, 1);
    args::Flag status(arguments, "status", "Provides the status of the specified component",
                      {'s', "status"});
    args::Flag read(arguments, "read", "Perform a read procedure", {'r', "read"});
    args::Flag write(arguments, "write", "Perform a write procedure", {'w', "write"});
    args::Flag one_write(arguments, "one_write",
                         "Include this flag if you want to write the data only one time.",
                         {"one_write"});
    args::Flag burst_read(arguments, "burst_read", "Performs a burst read", {"burst_read"});
    args::Flag verbose(arguments, "verbose", "Control verbosity", {'v', "verbose"});

    args::Group data_arguments(parser, "data_arguments", args::Group::Validators::DontCare,
                               args::Options::Global);
    args::ValueFlag<std::string> data_flag(data_arguments, "data",
                                           "The data that we want to write to the register.",
                                           {'d', "data"}, "0x70FFE3");
    args::MapFlag<std::string, DATA_PATTERN> data_pattern_flag(
        data_arguments, "test_data_pattern",
        "The type of test data to generate as 32-bit words."
        "\nDefault: CONSTANT",
        {'p', "pattern"},
        {{"HI_LO", DATA_PATTERN::HI_LO},
         {"FLIP_FLOP", DATA_PATTERN::FLIP_FLOP},
         {"RAMP_UP", DATA_PATTERN::RAMP_UP},
         {"RAMP_DOWN", DATA_PATTERN::RAMP_DOWN},
         {"CONSTANT", DATA_PATTERN::CONSTANT},
         {"EVEN_BITS", DATA_PATTERN::EVEN_BITS},
         {"ODD_BITS", DATA_PATTERN::ODD_BITS},
         {"ZERO", DATA_PATTERN::ZERO}},
        DATA_PATTERN::CONSTANT);
    args::ValueFlag<unsigned int> data_size_flag(
        data_arguments, "data_size", "The number of 32-bit words to put into the buffer.",
        {'s', "data_size"}, 32768);

    try {
        parser.ParseCLI(argc, argv);
    } catch (args::Help& help) {
        std::cout << LOG("INFO") << help.what() << std::endl;
        std::cout << parser;
        return EXIT_SUCCESS;
    } catch (args::Error& e) {
        std::cout << LOG("ERROR") << e.what() << std::endl;
        std::cout << parser;
        return EXIT_FAILURE;
    }

    if (is_dry_run)
        std::cout << LOG("INFO") << "Performing a dry run, none of these commands actually execute."
                  << std::endl;

    unsigned int address;
    if (args::get(address_flag) == "0x10073D" && !csr && !external_fifo && !boot && !mca) {
        std::cout << LOG("ERROR") << " You must provide us with a memory address!" << std::endl;
        return EXIT_FAILURE;
    } else
        address = stoul(args::get(address_flag), nullptr, 0);

    Configuration cfg;
    try {
        read_configuration_file(args::get(configuration));
    } catch (std::invalid_argument& invalidArgument) {
        std::cout << LOG("ERROR") << invalidArgument.what() << std::endl;
        return EXIT_FAILURE;
    }

    int offline_mode = 0;
    if (is_dry_run)
        offline_mode = 1;

    std::cout << LOG("INFO") << "Calling Pixie16InitSystem." << std::endl;
    if (!verify_api_return_value(Pixie16InitSystem(cfg.numModules, cfg.slot_map, offline_mode),
                                 "Pixie16InitSystem"))
        return EXIT_FAILURE;

    unsigned int boot_pattern = stoul(args::get(boot_pattern_flag), nullptr, 0);
    if (is_dry_run || external_fifo) {
        std::cout << LOG("INFO") << "Will not boot the module!" << std::endl;
    } else {
        std::cout << LOG("INFO") << "Calling Pixie16BootModule with boot pattern: " << std::showbase
                  << std::hex << boot_pattern << std::dec << std::endl;

        if (!verify_api_return_value(
                Pixie16BootModule(cfg.ComFPGAConfigFile.c_str(), cfg.SPFPGAConfigFile.c_str(), NULL,
                                  cfg.DSPCodeFile.c_str(), cfg.DSPParFile.c_str(),
                                  cfg.DSPVarFile.c_str(), cfg.numModules, boot_pattern),
                "Pixie16BootModule", "INFO - Finished booting!"))
            return EXIT_FAILURE;
        if (boot)
            return EXIT_SUCCESS;
    }

    bool has_written_data = false;
    std::cout << LOG("INFO") << "User requested that we perform our tests " << args::get(loop_flag)
              << " time(s)." << std::endl;
    for (unsigned int test_number = 0; test_number < args::get(loop_flag); test_number++) {
        std::cout << LOG("INFO") << "Starting test number " << test_number << std::endl;
        if (dsp) {
            std::cout << LOG("INFO") << "Starting DSP test" << std::endl;
            auto data =
                prepare_data_to_write(args::get(data_pattern_flag), args::get(data_size_flag));

            if (write && !has_written_data) {
                std::cout << LOG("INFO") << "Performing a write to memory address "
                          << args::get(address_flag) << " with a size of "
                          << args::get(data_size_flag) << " on Module "
                          << args::get(module_number_flag) << std::endl;
                if (!is_dry_run)
                    Pixie_DSP_Memory_IO(
                        data.data(), address, args::get(data_size_flag),
                        static_cast<std::underlying_type<DATA_IO>::type>(DATA_IO::WRITE),
                        args::get(module_number_flag));
                if (one_write)
                    has_written_data = true;
            }

            if (read) {
                std::cout << LOG("INFO") << "Performing a read from memory address "
                          << args::get(address_flag) << " with a size of "
                          << args::get(data_size_flag) << " on Module "
                          << args::get(module_number_flag) << std::endl;
                if (!is_dry_run) {
                    std::vector<unsigned int> read_data(args::get(data_size_flag), 0);
                    if (burst_read) {
                        std::cout << LOG("INFO") << "Performing the burst read from memory address "
                                  << args::get(address_flag) << std::endl;
                        Pixie_DSP_Memory_IO(
                            read_data.data(), address, args::get(data_size_flag),
                            static_cast<std::underlying_type<DATA_IO>::type>(DATA_IO::READ),
                            args::get(module_number_flag));
                    } else {
                        std::cout << LOG("INFO")
                                  << "Performing a word-by-word read from memory address "
                                  << args::get(address_flag) << std::endl;
                        for (unsigned int word = 0; word < args::get(data_size_flag); word++) {
                            Pixie_DSP_Memory_IO(
                                &read_data[word], address + word, 1,
                                static_cast<std::underlying_type<DATA_IO>::type>(DATA_IO::READ),
                                args::get(module_number_flag));
                        }
                    }

                    auto error_count =
                        verify_data_read(data.data(), read_data.data(),
                                         args::get(module_number_flag), args::get(data_size_flag));
                    if (error_count == 0)
                        std::cout << LOG("INFO") << "Data read was the same as data written!"
                                  << std::endl;
                    if (verbose) {
                        std::cout << LOG("INFO")
                                  << "Outputting read data to terminal:" << std::endl;
                        std::cout << std::hex;
                        for (const auto& it : read_data) {
                            std::cout << it;
                            std::cout << std::dec;
                        }
                    }
                }
            }
        }

        if (mca) {
            std::cout << LOG("INFO") << "Performing a test with the MCA" << std::endl;

            auto data = prepare_data_to_write(args::get(data_pattern_flag), 32768);

            if (write && !has_written_data) {
                if (!is_dry_run) {
                    for (unsigned int channel = 0; channel < NUMBER_OF_CHANNELS; channel++) {
                        std::cout << LOG("INFO") << "Writing Channel " << channel
                                  << "'s MCA spectrum on Module " << args::get(module_number_flag)
                                  << "." << std::endl;
                        if (!verify_api_return_value(
                                Pixie_Main_Memory_IO(
                                    data.data(), data.size() * channel, data.size(),
                                    static_cast<std::underlying_type<DATA_IO>::type>(
                                        DATA_IO::WRITE),
                                    args::get(module_number_flag)),
                                "Pixie_Main_Memory_IO", "OK")) {
                            std::cout << LOG("ERROR")
                                      << " Had a problem writing the MCA spectrum for Channel "
                                      << channel << "! Aborting!" << std::endl;
                            return EXIT_FAILURE;
                        }
                    }
                }
                if (one_write)
                    has_written_data = true;
            }

            if (read) {
                std::cout << LOG("INFO") << "Reading MCA spectrum from Module "
                          << args::get(module_number_flag) << std::endl;
                if (!is_dry_run) {
                    for (unsigned int channel = 0; channel < NUMBER_OF_CHANNELS; channel++) {
                        std::cout << LOG("INFO") << "Reading Channel " << channel
                                  << "'s MCA spectrum on Module " << args::get(module_number_flag)
                                  << "." << std::endl;

                        std::vector<unsigned int> read_data(data.size(), 0x1a1a1a1a);

                        if (!verify_api_return_value(
                                Pixie_Main_Memory_IO(
                                    read_data.data(), address, data.size(),
                                    static_cast<std::underlying_type<DATA_IO>::type>(DATA_IO::READ),
                                    args::get(module_number_flag)),
                                "Pixie_Main_Memory_IO", "OK")) {
                            std::cout << LOG("ERROR")
                                      << " Had a problem reading the MCA spectrum for Channel "
                                      << channel << "! Aborting!" << std::endl;
                            return EXIT_FAILURE;
                        }

                        auto error_count =
                            verify_data_read(data.data(), read_data.data(),
                                             args::get(module_number_flag), data.size());
                        if (error_count == 0)
                            std::cout << LOG("INFO") << "Data read was the same as data written!"
                                      << std::endl;
                        if (verbose) {
                            std::cout << LOG("INFO")
                                      << "Outputting read data to terminal:" << std::endl;
                            std::cout << std::hex;
                            for (const auto& it : read_data)
                                std::cout << it << std::dec;
                        }
                    }
                }
            }
            if (!is_dry_run && clear) {
                std::cout << LOG("INFO") << "Clearing MCA memory" << std::endl;
                if (!verify_api_return_value(Pixie_Clear_Main_Memory(0, 32768 * NUMBER_OF_CHANNELS,
                                                                     args::get(module_number_flag)),
                                             "Pixie Clear Main Memory")) {
                    std::cout << LOG("ERROR") << "Couldn't clear the main memory in Module "
                              << args::get(module_number_flag) << std::endl;
                    return EXIT_FAILURE;
                }
            }
        }

        if (raw) {
            std::cout << LOG("INFO") << "Performing a test with raw memory access." << std::endl;
            if (write) {
                if (!data_flag) {
                    std::cout << LOG("ERROR") << " Must define the data that you'd like to write!"
                              << std::endl;
                    return EXIT_FAILURE;
                }
                unsigned int data = stoul(args::get(data_flag), nullptr, 0);

                std::cout << LOG("INFO") << "Writing " << args::get(data_flag) << " to "
                          << args::get(address_flag) << " in Module "
                          << args::get(module_number_flag) << std::endl;
                if (!is_dry_run)
                    Pixie_Register_IO(
                        args::get(module_number_flag), address,
                        static_cast<std::underlying_type<DATA_IO>::type>(DATA_IO::WRITE), &data);
            }

            if (read) {
                unsigned int data;
                std::cout << LOG("INFO") << "Reading from " << args::get(address_flag)
                          << " in Module " << args::get(module_number_flag) << std::endl;
                if (!is_dry_run) {
                    Pixie_Register_IO(
                        args::get(module_number_flag), address,
                        static_cast<std::underlying_type<DATA_IO>::type>(DATA_IO::READ), &data);
                    std::cout << LOG("INFO") << "Read " << std::showbase << std::hex << data
                              << " from " << args::get(address_flag) << " in Module " << std::dec
                              << args::get(module_number_flag) << std::endl;
                } else {
                    std::cout << LOG("INFO") << "Dry run active didn't perform the read!"
                              << std::endl;
                }
            }
        }

        if (external_fifo) {
            std::cout << LOG("INFO") << "Performing a test with the External FIFO" << std::endl;
            unsigned int number_of_words_in_fifo = 0;
            Pixie_Read_ExtFIFOStatus(&number_of_words_in_fifo, args::get(module_number_flag));
            std::cout << LOG("INFO") << "Number of 32-bit words in the external FIFO of Module"
                      << args::get(module_number_flag) << ": " << number_of_words_in_fifo
                      << std::endl;
            if (read && number_of_words_in_fifo > 0) {
                std::vector<unsigned int> data = {number_of_words_in_fifo, 0};
                if (!verify_api_return_value(Pixie_ExtFIFO_Read(data.data(),
                                                                number_of_words_in_fifo,
                                                                args::get(module_number_flag)),
                                             "Pixie_ExtFIFO_Read", ""))
                    return EXIT_FAILURE;
                std::cout << LOG("INFO") << "Read " << number_of_words_in_fifo
                          << " 32-bit words from the External FIFO." << std::endl;
            } else {
                std::cout << LOG("INFO") << "External FIFO doesn't have anything to read!"
                          << std::endl;
            }
        }

        if (csr) {
            std::cout << LOG("INFO") << "CSR on Module " << args::get(module_number_flag) << "."
                      << std::endl;
            if (write) {
                if (args::get(data_flag) == "0x70FFE3") {
                    std::cout << LOG("ERROR") << " Must define the data that you'd like to write!"
                              << std::endl;
                    return EXIT_FAILURE;
                }
                unsigned int data = stoul(args::get(data_flag), nullptr, 0);
                std::cout << LOG("INFO") << "Writing " << args::get(data_flag)
                          << " to CSR in Module " << args::get(module_number_flag) << std::endl;
                Pixie_WrtCSR(args::get(module_number_flag), data);
            }
            if (read) {
                unsigned int data;
                std::cout << LOG("INFO") << "Reading from to CSR in Module "
                          << args::get(module_number_flag) << std::endl;
                Pixie_ReadCSR(args::get(module_number_flag), &data);
                std::cout << LOG("INFO") << "Read " << data << " from Module "
                          << args::get(module_number_flag) << " CSR." << std::endl;
            }
        }
        std::cout << LOG("INFO") << "Completed test number " << test_number << std::endl;
        if (args::get(loop_flag) > 1 && test_number != args::get(loop_flag) - 1) {
            std::cout << LOG("INFO") << "Will now sleep for 1 second before starting next test."
                      << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}
