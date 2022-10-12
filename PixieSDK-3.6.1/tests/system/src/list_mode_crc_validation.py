""" SPDX-License-Identifier: Apache-2.0 """
import argparse
import logging
import os
import re
import sys
import zlib

"""
Copyright 2022 XIA LLC, All rights reserved.

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

""" list-mode-crc-validation.py validates the data contained within a 
list-mode binary data file produced with the PixieSDK examples 
against the DMA CRC-32 checksums in the Pixie16Msg.log

**NOTE**: We expect that the only data run within the log file is the one that
we are checking against! 
"""

logging.basicConfig(stream=sys.stdout, level=logging.INFO, datefmt="%Y-%m-%dT%H:%M:%S",
                    format='%(asctime)s.%(msecs)03d - %(levelname)s - %(message)s')

DEBUG_PATTERN = re.compile('^\[DEBUG\]')
DMA_PATTERN = re.compile('read-words=(\d+)')
CRC_PATTERN = re.compile('crc=(0x[A-Fa-f0-9]+)')


def process_log(file):
    log_data = []
    with open(file, mode='r', newline='') as logfile:
        for line in logfile.readlines():
            if re.match(DEBUG_PATTERN, line):
                dma = re.findall(DMA_PATTERN, line)
                crc = re.findall(CRC_PATTERN, line)
                if dma and crc:
                    log_data.append({
                        'len': int(dma[0], 10),
                        'crc': int(crc[0], 16)
                    })
    return log_data


def main(cfg):
    logging.info(f'Parsing log file: {cfg.log}')
    log_data = process_log(cfg.log)
    logging.info(f'Found {len(log_data)} DMA transfers and checksums.')
    logging.info(f'Finished parsing log file: {cfg.log}')

    logging.info(f"Starting to process {cfg.file}")
    file_size_words = os.path.getsize(cfg.file) / cfg.bpw
    total_dma_words = sum([x['len'] for x in log_data])
    logging.info(f"DMA Word Total: {total_dma_words}")
    logging.info(f"File Word Total: {file_size_words}")

    if file_size_words != total_dma_words:
        raise ValueError("The DMA word total does not match the File word total!")

    with open(cfg.file, "rb") as file:
        for idx, x in enumerate(log_data):
            log_crc = x['crc']
            file_crc = zlib.crc32(file.read(x['len'] * cfg.bpw))
            if log_crc != file_crc:
                logging.info(
                    f"Total DMA Words Processed: {sum([x['len'] for x in log_data[:idx]])}")
                raise ValueError(
                    f"CRC check failed! Segment={idx} Len={x['len']} DMA={log_crc} FILE={file_crc}")
    logging.info("All CRC checks passed!")


if __name__ == '__main__':
    try:
        parser = argparse.ArgumentParser(description='Compares DMA CRCs to Files written to disk.')
        parser.add_argument('-b', '--bytes-per-word', type=int, dest='bpw', default=4,
                            help="The number of bytes per data word for the list-mode file.")
        parser.add_argument('-l', '--log', type=str, dest="log",
                            required=True, help="The CSV file containing the read lengths and CRC.")
        parser.add_argument('-f', '--file', dest='file',
                            help="The binary data file containing the list-mode data.")
        cfg = parser.parse_args()

        main(cfg)
        logging.info(f"Finished processing {cfg.file}.")
    except ValueError as ve:
        logging.error(ve)
    except KeyboardInterrupt:
        logging.info("Received keyboard interrupt. Stopping execution.")
    except Exception as e:
        logging.error(e)
    finally:
        logging.info("Execution finished.")
