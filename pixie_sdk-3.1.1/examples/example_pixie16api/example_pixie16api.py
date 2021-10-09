""" SPDX-License-Identifier: Apache-2.0 """

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

from argparse import ArgumentParser
import ctypes

import yaml


def calculate_num_modules(slot_map):
    """
    Reads the slot map and counts the number of modules we have in total
    :param slot_map: The Slot map containing the number of modules.
    :return: The number of modules counted in the config.
    """
    return sum([len(v) for v in slot_map.values()])


def initialize_modules(cfg, sdk):
    """
    Initializes the modules according to the information provided in the configuration file.
    :param cfg: The configuration containing the information we need.
    :param sdk: The SDK object that contains our functions.
    :return: The API result code.
    """
    num_modules = calculate_num_modules(cfg['slot_map'])

    slot_map = (ctypes.c_short * num_modules)()
    for crate, module_map in cfg.get('slot_map').items():
        for slot, module in module_map.items():
            slot_map[module] = ctypes.c_short(slot)

    return sdk.Pixie16InitSystem(ctypes.c_short(num_modules),
                                 ctypes.cast(slot_map, ctypes.POINTER(ctypes.c_short * 1)),
                                 ctypes.c_short(cfg['offline_mode']))


def boot(cfg, sdk):
    """
    Boots the modules by loading the firmware and a settings file.
    :param cfg: The configuration containing all the information we need to boot the module
    :param sdk: The SDK object with the boot function
    :return: The status code returned from the procedure.
    """
    return sdk.Pixie16BootModule(ctypes.c_char_p(cfg['firmware']['sys'].encode()),
                                 ctypes.c_char_p(cfg['firmware']['fippi'].encode()),
                                 ctypes.c_char_p(cfg['firmware']['trig'].encode()),
                                 ctypes.c_char_p(cfg['dsp']['ldr'].encode()),
                                 ctypes.c_char_p(cfg['dsp']['par'].encode()),
                                 ctypes.c_char_p(cfg['dsp']['var'].encode()),
                                 ctypes.c_short(calculate_num_modules(cfg['slot_map'])),
                                 127)


if __name__ == '__main__':
    try:
        PARSER = ArgumentParser(description='Converts PLD files into Apache Parquet format.')
        PARSER.add_argument('cfg', type=str, default='config.yaml',
                            help='The YAML configuration file')
        ARGS = PARSER.parse_args()
        with open(ARGS.cfg) as f:
            CFG = yaml.safe_load(f)

        if not CFG.get('sdk', None):
            raise LookupError("Configuration file is missing the path to the SDK")

        PIXIE_SDK = ctypes.cdll.LoadLibrary(CFG['sdk'])

        INIT_CODE = initialize_modules(CFG, PIXIE_SDK)
        if INIT_CODE != 0:
            raise RuntimeError(f"Initialization failed with Error Code {INIT_CODE}")

        BOOT_CODE = boot(CFG, PIXIE_SDK)
        if BOOT_CODE != 0:
            raise RuntimeError(f"Boot failed with Error Code {BOOT_CODE}")

    except (LookupError, RuntimeError) as err:
        print(err)
    except KeyboardInterrupt:
        print("Received interrupt!")
    finally:
        print("Finalizing...")
