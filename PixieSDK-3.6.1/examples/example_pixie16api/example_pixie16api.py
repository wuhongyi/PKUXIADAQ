""" SPDX-License-Identifier: Apache-2.0 """
from argparse import ArgumentParser
import csv
import ctypes
import enum
from datetime import datetime
import itertools
import json
import logging
import struct
import time

"""
Copyright 2021 XIA LLC, All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

""" example_pixie16api.py demonstrates how to use the SDK in a Python script.

"""

logging.basicConfig(format=('[%(asctime)s] %(levelname)s-%(funcName)s-%(message)s'),
                    level=logging.INFO)


class BootCodes(enum.Enum):
    PIXIE_BOOT_PROBE = 0
    PIXIE_BOOT_RESET_LOAD = 1
    PIXIE_BOOT_SETTINGS_LOAD = 2


def adc(cfg, sdk):
    """
    Collects ADC traces from all modules in the configuration file and outputs them to CSV.
    :param cfg: The configuration file containing the information necessary to collect the data.
    :param sdk: The SDK instance to call the API functions.
    :return: None
    """
    logging.info("Beginning to collect ADC traces from all modules in the config.")
    for mod in range(len(cfg)):
        rc = sdk.Pixie16AcquireADCTrace(mod)
        check_return_code(sdk, rc, "Pixie16AcquireADCTrace")

        traces = list()
        for ch in range(cfg[mod]['info']['num_channels']):
            trc_length = ctypes.c_uint(0)
            rc = sdk.PixieGetTraceLength(mod, ch, ctypes.byref(trc_length))
            check_return_code(sdk, rc, "PixieGetTraceLength")

            trace = (ctypes.c_ushort * trc_length.value)()
            rc = sdk.Pixie16ReadSglChanADCTrace(
                ctypes.cast(trace, ctypes.POINTER(ctypes.c_ushort * 1)), trc_length, mod, ch)
            check_return_code(sdk, rc, "Pixie16ReadSglChanADCTrace")

            traces.append([trace[i] for i in range(trc_length.value)])

        export_csv(mod, "adc", ['bin'] + [f"Chan{x}" for x in range(len(traces))], traces)

    logging.info("Finished collecting ADC traces.")


def adjust_offsets(cfg, sdk):
    """
    Executes the adjust_offsets control task for all modules in the configuration file.
    :param cfg: configuration containing the list of modules
    :param sdk: instance of the library
    :return: nothing
    """
    logging.info("Staring to adjust offsets for all modules.")
    for mod in range(len(cfg)):
        rc = sdk.Pixie16AdjustOffsets(ctypes.c_uint(int(mod)))
        check_return_code(sdk, rc, "Pixie16AdjustOffsets")
    logging.info("Finished adjusting offsets for all modules.")
    export_settings(cfg, sdk)


def baseline(cfg, sdk):
    """
    Collects baseline data from all modules in the system.
    :param cfg: configuration containing the list of modules
    :param sdk: instance of the library
    :return: nothing
    """
    for mod in range(len(cfg)):
        logging.info(f"Starting baseline capture for Module {mod}.")
        rc = sdk.Pixie16AcquireBaselines(mod)
        check_return_code(sdk, rc, "Pixie16AcquireBaselines")

        baselines = list()
        timestamps = list()
        max_num_baselines = ctypes.c_uint(0)
        for ch in range(cfg[mod]['info']['num_channels']):
            rc = sdk.PixieGetMaxNumBaselines(mod, ch, ctypes.byref(max_num_baselines))
            check_return_code(sdk, rc, "PixieGetMaxNumBaselines")

            bl = (ctypes.c_double * max_num_baselines.value)()
            ts = (ctypes.c_double * max_num_baselines.value)()

            rc = sdk.Pixie16ReadSglChanBaselines(
                ctypes.cast(bl, ctypes.POINTER(ctypes.c_double * 1)),
                ctypes.cast(ts, ctypes.POINTER(ctypes.c_double * 1)), max_num_baselines, mod,
                ch)
            check_return_code(sdk, rc, "Pixie16ReadSglChanBaselines")

            baselines.append([bl[i] for i in range(max_num_baselines.value)])
            timestamps.append([ts[i] for i in range(max_num_baselines.value)])

        export_csv(mod, "baselines",
                   ['bin', 'timestamp'] + [f"Chan{x}" for x in range(len(baselines))],
                   (timestamps[0], *baselines))


def blcut(cfg, sdk):
    """
    Executes the control task to find an appropriate baseline cuts for each module in the config.
    The BL cuts are then exported as a CSV to disk.
    :param cfg: configuration containing the list of modules
    :param sdk: instance of the library
    :return: nothing
    """
    for mod in range(len(cfg)):
        logging.info(f"Starting to get BL cuts for Module {mod}.")
        vals = list()
        for ch in range(cfg[mod]['info']['num_channels']):
            val = ctypes.c_uint32(0)
            rc = sdk.Pixie16BLcutFinder(mod, ch, ctypes.byref(val))
            check_return_code(sdk, rc, "Pixie16BLcutFinder")
            vals.append(val.value)
        export_csv(mod, 'blcuts', ['channel', 'blcut'], [vals])
        logging.info(f"Finished getting BL cuts for Module {mod}.")


def cbuf_to_string(cbuf):
    """
    Converts a ctype buffer object to a string.
    :param cbuf: The C style buffer to convert to a string.
    :return: The string representation of the provided buffer.
    """
    return ''.join([x.decode() for x in cbuf if x]).strip()


def check_return_code(sdk, return_code, function):
    """
    Checks that the API call's return code didn't return an error.
    :param sdk: The SDK object so that we can get the return code's message.
    :param return_code: The return code that we want to validate.
    :param function: The name of the function that produced the return code.
    :return: Nothing!
    """
    if return_code < 0:
        msg = ctypes.create_string_buffer(1024, 1024)
        sdk.PixieGetReturnCodeText(return_code, msg, 1024)
        raise RuntimeError(
            f"{function} failed with Error Code {return_code} : "
            f"{cbuf_to_string(msg)}.")


def data_runs(args, cfg, sdk, func):
    """
    Executes N many data runs based on the number requested by the user.
    :param args: The command line arguments
    :param cfg: configuration containing the list of modules
    :param sdk: instance of the library
    :param func: the function that we want to execute multiple times.
    :return: nothing
    """
    for run in range(args.num):
        logging.info(f"Starting run number {run}.")
        func(run, args, cfg, sdk)
        logging.info(f'Finished run number {run}.')
        time.sleep(2)


def export_csv(mod, desc, header, data):
    """
    Exports a CSV file containing the provided data with the provided header.
    :param mod: The module number for formatting the file name.
    :param desc: the file description e.g. "adc"
    :param header: The header row for the CSV data as a list of names.
    :param data: The actual CSV data. The data must be in an M x N list.
        Where M is the number of columns minus 1, and N is the number of
        data elements. If each column doesn't have the same number of elements,
        then we append 0s to the end of the data.
    :return: Nothing
    """
    if len(header) - 1 != len(data):
        raise RuntimeError("The provided CSV header doesn't match with the number of data columns.")

    with open(generate_filename(mod, desc, "csv"), 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(header)
        for idx, row in enumerate(itertools.zip_longest(*data, fillvalue="0")):
            writer.writerow((idx, *row))


def export_stats(args, cfg, sdk, desc):
    """
    Exports statistics data from an MCA or List-mode data run to CSV.
    :param args: The command line arguments
    :param cfg: The configuration containing the module list.
    :param sdk: The SDK library with the functions we need.
    :param desc: The description of the data file.
    :return: Nothing
    """
    bin_stats = (ctypes.c_uint * sdk.Pixie16GetStatisticsSize())()
    rc = sdk.Pixie16ReadStatisticsFromModule(
        ctypes.cast(bin_stats, ctypes.POINTER(ctypes.c_uint * 1)), args.mod)
    check_return_code(sdk, rc, "Pixie16ReadStatisticsFromModule")

    stats_labels = ("real_time", "live_time", "input_counts", "input_count_rate",
                    "output_counts", "output_count_rate")
    stats = [[0] * cfg[args.mod]['info']['num_channels'] for x in range(len(stats_labels))]

    for ch in range(cfg[args.mod]['info']['num_channels']):
        stats[0][ch] = sdk.Pixie16ComputeRealTime(bin_stats, args.mod)
        stats[1][ch] = sdk.Pixie16ComputeLiveTime(bin_stats, args.mod, ch)
        stats[2][ch] = sdk.Pixie16ComputeRawInputCount(bin_stats, args.mod, ch)
        stats[3][ch] = sdk.Pixie16ComputeInputCountRate(bin_stats, args.mod, ch)
        stats[4][ch] = sdk.Pixie16ComputeRawOutputCount(bin_stats, args.mod, ch)
        stats[5][ch] = sdk.Pixie16ComputeOutputCountRate(bin_stats, args.mod, ch)
        logging.info({"module": args.mod, "channel": ch, "real_time": stats[0][ch],
                      "live_time": stats[1][ch], "input_counts": stats[2][ch],
                      "input_count_rate": stats[3][ch],
                      "output_counts": stats[4][ch], "output_count_rate": stats[5][ch]})

    export_csv(args.mod, desc, ("channel", *stats_labels), stats)


def export_settings(cfg, sdk):
    """
    Saves the DSP settings to the one provided in the configuration file.
    :param cfg: The configuration containing the module list.
    :param sdk: The SDK library with the functions we need.
    :return: Nothing
    """
    file_name = cfg[0]['dsp']['par']
    logging.info(f"Saving DSP settings to {file_name}.")
    rc = sdk.Pixie16SaveDSPParametersToFile(ctypes.c_char_p(file_name.encode()))
    check_return_code(sdk, rc, "Pixie16SaveDSPParametersToFile")


def export_list_mode_data(cfg, sdk, output):
    """
    Serially processes list-mode data from the Worker FIFO and outputs it into the data
    file.
    :param cfg: The configuration containing the module list.
    :param sdk: The SDK library with the functions we need.
    :param output: A list of open file handles that store the data we're exporting.
    :return: Nothing
    """
    for mod in range(len(cfg)):
        num_words = ctypes.c_uint32(0)
        check_return_code(sdk,
                          sdk.Pixie16CheckExternalFIFOStatus(ctypes.byref(num_words),
                                                             mod),
                          "Pixie16CheckExternalFifoStatus")
        if num_words.value > 0:
            logging.debug(f"Reading {num_words.value} words from Module {mod}'s FIFO.")
            buf = (ctypes.c_uint32 * num_words.value)()
            sdk.Pixie16ReadDataFromExternalFIFO(
                ctypes.cast(buf, ctypes.POINTER(ctypes.c_ushort * 1)), num_words, mod)
            for chunk in buf:
                output[mod].write(struct.pack("I", chunk))


def generate_filename(module, description, extension):
    """
    Generates a file name for the module with the provided description and extension
    :param module: The module number that the data represents.
    :param description: The description of the file e.g. adc for trace data, mca for histograms
    :param extension: The file extension, e.g. csv
    :return: The compiled file extension.
    """
    return f'pypixie16api-module{module}-{description}.{extension}'


def list_mode(run_num, args, cfg, sdk):
    """
    Executes a list-mode data run. Depending on the value of synch_wait the run may execute in
    all modules independently (0) or synchronously (1). Unlike MCA runs, list-mode data runs
    must be explicitly terminated, or they will run indefinitely.
    :param run_num: The run number for this execution of the function
    :param args: The command line arguments
    :param cfg: The configuration containing the settings file to write.
    :param sdk: The SDK object that has the functions we need.
    :return: None
    """
    logging.info(f"Starting list-mode data run for {args.time} s.")

    logging.info(f'Writing SYNCH_WAIT = {args.synch_wait}')
    check_return_code(sdk,
                      sdk.Pixie16WriteSglModPar(ctypes.c_char_p(b"SYNCH_WAIT"), args.synch_wait, 0),
                      "Pixie16WriteSglModPar")

    logging.info(f'Writing IN_SYNCH = {args.in_synch}')
    check_return_code(sdk,
                      sdk.Pixie16WriteSglModPar(ctypes.c_char_p(b"IN_SYNCH"), args.in_synch, 0),
                      "Pixie16WriteSglModPar")

    logging.info('Starting list-mode run in individual modules.')
    check_return_code(sdk, sdk.Pixie16StartListModeRun(len(cfg), 0x100, 1),
                      "Pixie16StartListModeRun")

    output = [open(generate_filename(mod, f"list-mode-run{run_num}", "lmd"), 'wb') for mod in
              range(len(cfg))]

    run_start_time = datetime.now()
    check_time = 0
    run_status = sdk.Pixie16CheckRunStatus(0)
    while run_status != 0:
        current_run_time = (datetime.now() - run_start_time).total_seconds()

        if current_run_time >= args.time:
            if args.synch_wait == 0:
                logging.info("Stopping list-mode run individually")
                check_return_code(sdk, sdk.Pixie16EndRun(len(cfg)), "Pixie16EndRun")
            else:
                logging.info("Stopping list-mode run in director module.")
                check_return_code(sdk, sdk.Pixie16EndRun(0), "Pixie16EndRun")
            break

        if current_run_time - check_time > 1:
            if current_run_time < args.time:
                logging.info(f"Remaining run time: {round(args.time - current_run_time)} s.")
            check_time = current_run_time

        """
        The PixieSDK now uses threaded list-mode FIFO workers that live on the host machine. These
        workers perform execute in parallel. They'll read the data from each module as needed to
        ensure that the EXTERNAL_FIFO_LENGTH isn't exceeded. When calling
        `Pixie16CheckExternalFIFOStatus`, you're actually checking the status of the FIFO workers 
        for that module. The Worker FIFOs do not have any limitations on how often you can poll 
        them for data.
        """
        export_list_mode_data(cfg, sdk, output)

        """
        Check the run status of the Director module (num=0) to see if the run has been stopped.
        This is possible in a multi-chassis system where modules in one chassis can stop the run
        in all chassis.
        """
        run_status = sdk.Pixie16CheckRunStatus(0)
        time.sleep(0.1)

    logging.info("Checking that the run finalized in all the modules.")
    finalized_mods = [False] * len(cfg)
    for attempt in range(50):
        for mod in range(len(cfg)):
            if sdk.Pixie16CheckRunStatus(mod) != 1:
                finalized_mods[mod] = True
        if all(finalized_mods):
            break
        time.sleep(0.1)

    if all(finalized_mods):
        logging.info("List-mode run finished in all modules.")
    else:
        raise RuntimeError(f"Modules {[i for i, j in enumerate(finalized_mods) if j]} "
                           f"failed to finalize their runs properly.")

    logging.info("Reading the final words from the External FIFO and getting run stats.")
    export_list_mode_data(cfg, sdk, output)
    export_stats(args, cfg, sdk, f"list-mode-stats-run{run_num}")


def load_sdk(path):
    """
    Loads the SDK into the module and sets the return values for some functions.
    :param path: The path to the Pixie16Api.so that we should load.
    :return: The loaded library
    """
    logging.info(f"Loading Pixie16API from {path}")
    sdk = ctypes.cdll.LoadLibrary(path)
    for func in [sdk.Pixie16ComputeRealTime, sdk.Pixie16ComputeInputCountRate,
                 sdk.Pixie16ComputeLiveTime, sdk.Pixie16ComputeOutputCountRate,
                 sdk.Pixie16ComputeProcessedEvents, sdk.Pixie16ComputeRawInputCount,
                 sdk.Pixie16ComputeRawOutputCount]:
        func.restype = ctypes.c_double
    logging.info(f"Finished loading Pixie16API.")
    return sdk


def mca(run_num, args, cfg, sdk):
    """
    Executes an MCA data run and outputs the histograms and statistics data to CSV
    :param run_num: The run number for this execution of the function
    :param args: The command line arguments
    :param cfg: The configuration containing the settings file to write.
    :param sdk: The SDK object that has the functions we need.
    :return: None
    """
    ieee754_time = struct.unpack("I", struct.pack('f', args.time))[0]
    logging.info(f"Writing HOST_RT_PRESET to {args.time} ({ieee754_time}) in Module {args.mod}.")
    rc = sdk.Pixie16WriteSglModPar(ctypes.c_char_p("HOST_RT_PRESET".encode()), ieee754_time,
                                   args.mod)
    check_return_code(sdk, rc, "Pixie16WriteSglModPar")

    logging.info(f"Writing SYNCH_WAIT to {args.synch_wait} in Module {args.mod}.")
    rc = sdk.Pixie16WriteSglModPar(ctypes.c_char_p("SYNCH_WAIT".encode()), args.synch_wait,
                                   args.mod)
    check_return_code(sdk, rc, "Pixie16WriteSglModPar")

    logging.info(f"Writing IN_SYNCH to {args.in_synch} in Module {args.mod}.")
    rc = sdk.Pixie16WriteSglModPar(ctypes.c_char_p("IN_SYNCH".encode()), args.in_synch, args.mod)
    check_return_code(sdk, rc, "Pixie16WriteSglModPar")

    rc = sdk.Pixie16StartHistogramRun(args.mod, 1)
    check_return_code(sdk, rc, "Pixie16StartHistogramRun")

    start_time = datetime.now()
    running_for = 0
    check_time = 0
    run_status = sdk.Pixie16CheckRunStatus(args.mod)
    forced_end = False

    time.sleep(args.time)

    while run_status != 0:
        running_for = (datetime.now() - start_time).total_seconds()

        if running_for - check_time > 1:
            if running_for < args.time:
                logging.info(f"Remaining run time: {round(args.time - running_for)} s.")
            check_time = running_for
        if running_for > args.time + 5:
            logging.warning(f"Forcing end of run in Module {args.mod}.")
            rc = sdk.Pixie16EndRun(args.mod)
            check_return_code(sdk, rc, "Pixie16EndRun")
            forced_end = True

        run_status = sdk.Pixie16CheckRunStatus(args.mod)
        time.sleep(0.1)

    if running_for < args.time:
        logging.error(f'run exited prematurely in Module {args.mod}! Check SDK log for errors.')
    if not forced_end:
        # TODO: We need to temporarily execute a manual end run until P16-440 is complete.
        rc = sdk.Pixie16EndRun(args.mod)
        check_return_code(sdk, rc, "Pixie16EndRun")
        logging.info(f"finished in Module {args.mod}.")

    hists = list()
    for ch in range(cfg[args.mod]['info']['num_channels']):
        hist_len = ctypes.c_uint(0)
        rc = sdk.PixieGetHistogramLength(args.mod, ch, ctypes.byref(hist_len))
        check_return_code(sdk, rc, "PixieGetHistogramLength")

        hist = (ctypes.c_uint32 * hist_len.value)()
        rc = sdk.Pixie16ReadHistogramFromModule(
            ctypes.cast(hist, ctypes.POINTER(ctypes.c_uint32 * 1)), hist_len, args.mod, ch)
        check_return_code(sdk, rc, "Pixie16ReadHistogramFromModule")

        hists.append([hist[i] for i in range(hist_len.value)])

    export_csv(args.mod, f"mca-run{run_num}", ['bin'] + [f"Chan{x}" for x in range(len(hists))],
               hists)
    export_stats(args, cfg, sdk, f"mca-stats-run{run_num}")


def parse_firmware(cfg, sdk):
    """
    Parses the firmware from the configuration file and then registers the firmware with the SDK.
    :param cfg: The configuration containing the information we need.
    :param sdk: The SDK object that contains our functions.
    :return: The API result code.
    """
    logging.info("Beginning firmware registration.")
    for mod in cfg:
        objects = {**mod['dsp'], **mod['fpga']}
        for device, path in [(k, v) for k, v in objects.items() if k != 'par']:
            if device == "ldr":
                device = "dsp"
            rc = sdk.PixieRegisterFirmware(mod['fw']['version'], mod['fw']['revision'],
                                           mod['fw']['adc_msps'], mod['fw']['adc_bits'],
                                           ctypes.c_char_p(device.encode()),
                                           ctypes.c_char_p(path.encode()), cfg.index(mod))
            if rc < 0:
                raise RuntimeError(f"Firmware registration failed with Error Code {rc}.")
    logging.info("Finished firmware registration.")


def read(args, sdk):
    """
    Reads the requested parameter from the hardware and outputs the information to the log.
    :param args: The command line arguments
    :param sdk: The SDK object that has the functions we need.
    :return:
    """
    if args.ch:
        logging.info(
            f'Pixie16ReadSglChanPar reading {args.name} from Module {args.mod} Channel {args.ch}.')
        val = ctypes.c_double()
        rc = sdk.Pixie16ReadSglChanPar(ctypes.c_char_p(args.name.encode()), ctypes.byref(val),
                                       args.mod, args.ch)
        check_return_code(sdk, rc, "Pixie16ReadSglChanPar")
    else:
        logging.info(
            f'Pixie16ReadSglModPar reading {args.name} from Module {args.mod}.')
        val = ctypes.c_uint()
        rc = sdk.Pixie16ReadSglModPar(ctypes.c_char_p(args.name.encode()), ctypes.byref(val),
                                      args.mod)
        check_return_code(sdk, rc, "Pixie16ReadSglModPar")

    logging.info(json.dumps(
        {'module': args.mod, 'channel': args.ch, 'name': args.name, 'value': val.value}))


def find_tau(cfg, sdk):
    """
    Executes the Tau finder control task for each module defined in the configuration.
    :param cfg: The configuration containing the settings file to write.
    :param sdk: The SDK object that has the functions we need.
    :return: None
    """
    for mod in range(len(cfg)):
        logging.info(f"Starting to find taus for Module {mod}.")
        taus = (ctypes.c_double * cfg[mod]['info']['num_channels'])()
        rc = sdk.Pixie16TauFinder(mod, ctypes.cast(taus, ctypes.POINTER(ctypes.c_double * 1)))
        check_return_code(sdk, rc, "Pixie16BLcutFinder")
        export_csv(mod, 'taus', ['channel', 'taus'], [taus])

        logging.info(f"Finished finding taus for Module {mod}.")


def set_dacs(cfg, sdk):
    """
    Executes the Tau finder control task for each module defined in the configuration.
    :param cfg: The configuration containing the settings file to write.
    :param sdk: The SDK object that has the functions we need.
    :return: None
    """
    for mod in range(len(cfg)):
        logging.info(f"Starting to set DACs for Module {mod}.")
        rc = sdk.Pixie16SetDACs(mod)
        check_return_code(sdk, rc, "Pixie16SetDACs")
        logging.info(f"Finished setting DACs for Module {mod}.")


def write(args, cfg, sdk):
    """
    Writes a value to a parameter on the hardware and updates the settings file.
    :param args: The command line arguments
    :param cfg: The configuration containing the settings file to write.
    :param sdk: The SDK object that has the functions we need.
    :return: None
    """
    read(args, sdk)

    if args.ch:
        logging.info(f'Pixie16WriteSglChanPar writing {args.name} to {args.value} in '
                     f'Module {args.mod} Channel {args.ch}.')
        rc = sdk.Pixie16WriteSglChanPar(ctypes.c_char_p(args.name.encode()),
                                        ctypes.c_double(args.value), args.mod, args.ch)
        check_return_code(sdk, rc, "Pixie16WriteSglChanPar")
    else:
        logging.info(
            f'Pixie16WriteSglModPar writing {args.name} to {args.value} in Module {args.mod}.')
        rc = sdk.Pixie16WriteSglModPar(ctypes.c_char_p(args.name.encode()),
                                       ctypes.c_uint(int(args.value)), args.mod)
        check_return_code(sdk, rc, "Pixie16WriteSglModPar")

    read(args, sdk)
    export_settings(cfg, sdk)


def check_node(level, nodes, expected, errors):
    """
    Checks a list of nodes against the expected set of entries. Updates
    the error dictionary with the problematic entries.
    :param level: The node level that we're currently working with.
    :param nodes: The set of nodes found at this level
    :param expected: The expected set of nodes to be at this level
    :param errors: The dictionary containing the found errors.
    :return: None
    """
    missing = expected.difference(nodes)
    if missing:
        errors[level] = f"Missing node(s): {', '.join(missing)}."


def validate_config(cfg):
    """
    Validates the configuration file to make sure that we have all the expected nodes before
    proceeding.
    :param cfg: The list of dictionary objects to validate.
    :return: None
    """
    cfg_errors = {}
    expected_nodes = {
        'root': {'slot', 'dsp', 'fpga', 'fw'},
        'dsp': {'ldr', 'par', 'var'},
        'fpga': {'fippi', 'sys'},
        'fw': {'version', 'revision', 'adc_msps', 'adc_bits'},
        'worker': {"bandwidth_mb_per_sec", "buffers", "dma_trigger_level_bytes", "hold_usecs",
                   "idle_wait_usecs", "run_wait_usecs"}
    }
    for idx, item in enumerate(cfg):
        node_errors = dict()
        check_node("root", item.keys(), expected_nodes['root'], node_errors)

        for key in [x for x in item.keys() if x != 'slot']:
            check_node(key, item[key].keys(), expected_nodes[key], node_errors)

        if node_errors:
            cfg_errors[f"Element {idx}"] = node_errors

    if cfg_errors:
        raise ValueError(cfg_errors)


def example_pixie16api(args, cfg):
    """
    The entry point for all work with the API.
    :param args: arguments parsed from the command line or via an argument parser.
    :param cfg: A dictionary containing the information from the JSON based configuration file.
    :return: Nothing!
    """
    pixie16_api = load_sdk(args.lib)

    logging.info(f"Initializing the system.")
    rc = pixie16_api.Pixie16InitSystem(ctypes.c_short(len(cfg)),
                                       ctypes.cast(
                                           (ctypes.c_ushort * len(cfg))(*[x['slot'] for x in cfg]),
                                           ctypes.POINTER(ctypes.c_short * 1)), 0)
    check_return_code(pixie16_api, rc, "Pixie16InitSystem")
    logging.info(f"Finished initialization.")

    if args.cmd == 'mca' and args.mod > len(cfg):
        raise RuntimeError(f'You requested Module {args.mod} but we only have {len(cfg)} modules.')

    for mod in range(len(cfg)):
        sn = ctypes.c_uint()
        rev = ctypes.c_ushort()
        adc_bits = ctypes.c_ushort()
        adc_msps = ctypes.c_ushort()
        num_channels = ctypes.c_ushort()

        rc = pixie16_api.Pixie16ReadModuleInfo(mod, ctypes.byref(rev), ctypes.byref(sn),
                                               ctypes.byref(adc_bits), ctypes.byref(adc_msps),
                                               ctypes.byref(num_channels))
        check_return_code(pixie16_api, rc, "Pixie16ReadModuleInfo")
        logging.info(f"Begin Module {mod} information.")
        cfg[mod]['info'] = {"number": mod, "sn": sn.value, "rev": rev.value,
                            "adc_msps": adc_msps.value, "adc_bits": adc_bits.value,
                            "num_channels": num_channels.value}
        logging.info(cfg[mod]['info'])
        logging.info(f"End Module {mod} information.")

    if args.cmd == 'init':
        return

    parse_firmware(cfg, pixie16_api)

    logging.info(f"Booting the crate with boot code {hex(args.boot_code)}")

    if args.boot_code == 0x00:
        boot_code = 0
    elif args.boot_code == 0x70:
        boot_code = 2
    else:
        boot_code = 1

    rc = pixie16_api.PixieBootCrate(ctypes.c_char_p(cfg[0]['dsp']['par'].encode()), boot_code)
    check_return_code(pixie16_api, rc, "Pixie16BootCrate")
    logging.info("Finished booting the crate.")

    if args.cmd == 'boot':
        return

    if args.cmd == 'adc':
        adc(cfg, pixie16_api)

    if args.cmd == 'adjust-offsets':
        adjust_offsets(cfg, pixie16_api)

    if args.cmd == 'baseline':
        baseline(cfg, pixie16_api)

    if args.cmd == 'blcut':
        blcut(cfg, pixie16_api)

    if args.cmd == 'set-dacs':
        set_dacs(cfg, pixie16_api)

    if args.cmd == 'find-tau':
        find_tau(cfg, pixie16_api)

    if args.cmd == 'lmd':
        data_runs(args, cfg, pixie16_api, list_mode)

    if args.cmd == 'mca':
        data_runs(args, cfg, pixie16_api, mca)

    if args.cmd == 'read':
        read(args, pixie16_api)

    if args.cmd == 'write':
        write(args, cfg, pixie16_api)

    pixie16_api.Pixie16ExitSystem(len(cfg))


if __name__ == '__main__':
    try:
        PARSER = ArgumentParser(description='A python example using the Pixie16 API library.')

        parent_parser = ArgumentParser(add_help=True)
        parent_parser.add_argument('-c', '--cfg', type=str, required=True,
                                   help='The YAML configuration file')
        parent_parser.add_argument('-l', '--lib', type=str, required=True,
                                   help='Path to the Pixie16Api.so.')

        subparsers = PARSER.add_subparsers(title="commands", help='Commands', dest='cmd')

        mod_parser = ArgumentParser(add_help=False)
        mod_parser.add_argument('-m', '--mod', type=int, default=0,
                                help="The module number that we'll execute against. "
                                     "If greater than the number of modules in the config, then"
                                     "we'll execute against all modules. Default: %(default)s")

        init_parser = subparsers.add_parser("init", add_help=False, parents=[parent_parser],
                                            help="Initializes the SDK and prints module information")

        boot_parser = subparsers.add_parser("boot", add_help=False, parents=[parent_parser],
                                            help="Boots the modules found in the config.")
        boot_parser.add_argument('-b', '--boot_code', type=int, default=0x7F,
                                 help='The boot code passed to the boot function. '
                                      'Default: %(default)s')

        ao_parser = subparsers.add_parser("adjust-offsets", add_help=False,
                                          parents=[boot_parser],
                                          help="Executes the adjust_offsets control task.")

        read_parser = subparsers.add_parser("read", add_help=False,
                                            parents=[boot_parser, mod_parser],
                                            help="Reads a parameter from the hardware.")
        read_parser.add_argument('--ch', type=int, default=None,
                                 help="The channel number that we'll execute against. If greater"
                                      "than the number of channels in a module, then we'll "
                                      "execute against all channels. Default: %(default)s")
        read_parser.add_argument('-n', '--name', type=str,
                                 help="The name of the parameter to work with.")

        write_parser = subparsers.add_parser('write', add_help=False, parents=[read_parser],
                                             help="Writes a parameter to the hardware.")
        write_parser.add_argument('-v', '--value', type=float,
                                  help="The value of the parameter to write.")

        adc_parser = subparsers.add_parser("adc", add_help=False, parents=[boot_parser],
                                           help="Collects ADC traces from all modules.")

        blcut_parser = subparsers.add_parser("blcut", add_help=False, parents=[boot_parser],
                                             help="Executes BL cut control task for all modules.")

        tau_parser = subparsers.add_parser("find-tau", add_help=False, parents=[boot_parser],
                                           help="Executes tau finder control task for all modules.")

        dac_parser = subparsers.add_parser("set-dacs", add_help=False, parents=[boot_parser],
                                           help="Executes set DACs control task for all modules.")

        baseline_parser = subparsers.add_parser("baseline", add_help=False, parents=[boot_parser],
                                                help="Collects baselines from all modules.")

        mca_parser = subparsers.add_parser("mca", add_help=False, parents=[adc_parser, mod_parser],
                                           help="Executes an MCA run for the provided module.")
        mca_parser.add_argument('-n', '--num', type=int, default=1,
                                help='The number of data runs to execute. Default: %(default)s')
        mca_parser.add_argument('-t', type=int, default=10, dest='time',
                                help='Run time for a data run. Default: %(default)s')
        mca_parser.add_argument('--synch_wait', type=int, default=0,
                                help="Value of SynchWait for data runs. Default: %(default)s")
        mca_parser.add_argument('--in_synch', type=int, default=1,
                                help="Value of InSynch for data runs. Default: %(default)s")

        lmd_parser = subparsers.add_parser('lmd', add_help=False, parents=[mca_parser],
                                           help="Perform a list-mode data run.")

        ARGS = PARSER.parse_args()
        with open(ARGS.cfg) as f:
            CFG = json.load(f)

        validate_config(CFG)
        example_pixie16api(ARGS, CFG)
    except (LookupError, RuntimeError, ValueError) as err:
        logging.error(err)
    except KeyboardInterrupt:
        logging.info("Received interrupt!")
    finally:
        logging.info("Finished.")
