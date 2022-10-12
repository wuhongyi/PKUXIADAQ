""" SPDX-License-Identifier: Apache-2.0 """
from argparse import ArgumentParser
from io import BytesIO
import logging
import math
import os
import sys

from dolosse.hardware.xia.pixie16.list_mode_data_mask import ListModeDataMask
from dolosse.hardware.xia.pixie16.list_mode_data_decoder import decode_listmode_data
import pandas as pd
import matplotlib.pyplot as plt

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

""" data_visualizer.py visualizes CSV and list-mode energies from the example software. 

"""

logging.basicConfig(stream=sys.stdout, datefmt="%Y-%m-%dT%H:%M:%S.000Z", level=logging.INFO,
                    format='%(asctime)s - %(levelname)s - %(message)s')


def calculate_subplot_dims(num_elements):
    """
    Calculates the subplot dimensions based on the number of channels
    that we're going to be plotting.
    :param num_elements: The number of channels that need plotted.
    :return: A dictionary containing the calculated subplot dimensions
    """
    square_dim = int(math.sqrt(math.pow(math.ceil(math.sqrt(num_elements)), 2)))
    pad = square_dim ** 2 - num_elements
    if pad == 0:
        rows = square_dim
    else:
        pad = square_dim - pad
        rows = round((num_elements + pad) / square_dim)
    return {
        "rows": rows,
        "cols": square_dim,
        "pad": pad
    }


def process_list_mode_data_file(args):
    """
    Processes an entire list-mode data file written by the example software.
    The file must contain nothing but data buffers from the Pixie-16's
    external FIFO.

    :param args: The arguments provided on the command line.
    :return: A pandas data frame containing the events.
    """
    logging.info(f"Started to process {args.file}")
    results = decode_listmode_data(BytesIO(open(args.file, 'rb').read()),
                                   ListModeDataMask(args.freq, args.rev))
    logging.info(f"Finished processing {args.file}")
    return pd.DataFrame(results)


def plot_lmd(df, args):
    """
    Plots the energy from each channel present in a data frame containing
    decoded list-mode data.
    :param df: The data frame containing the data, which much have both
               a channel and energy column.
    :param args: The arguments obtained from the argument parser
    :return: None
    """
    logging.info(f"Starting to plot list-mode energy histograms.")

    ax = df.hist('energy', by='channel', sharey=True, sharex=True)
    ax.item(0).get_figure().supxlabel("Energy(arb)")
    ax.item(0).get_figure().supylabel("Energy(arb) / bin")
    plt.xlim(args.xlim)
    plt.suptitle("List-Mode Energy Histograms")
    plt.tight_layout()
    plt.savefig(f"{os.path.splitext(args.file)[0]}.png", format='png')
    plt.show()


def plot_csv(plot_type, data, args):
    """
    Plots the various CSV data produced by the example software.
    :param plot_type: One of mca, adc, baseline, or stats.
    :param data: The data frame containing the data that we need to plot
    :param args: The arguments parsed by the argument parser.
    :return: None
    """
    logging.info(f"Starting to plot {plot_type} data.")
    title = "Untitled"
    xlabel = "Sample"
    ylabel = "ADC (arb) / Sample"

    if plot_type == 'mca':
        xlabel = "Bin"
        ylabel = "Energy (arb) / Bin"
        title = "MCA Spectrum"

        logging.info("MCA Sums")
        print(df.sum())
    if plot_type == 'adc':
        title = "ADC Traces"
    if plot_type == 'baseline':
        title = "Baselines"

    if plot_type == 'stats':
        data.plot.bar(x="channel", y=["input_count_rate", "output_count_rate"],
                      title="Rate Statistics", xlabel="Channel", ylabel="Counts / second",
                      label=['Input', "Output"])
    else:
        if args.chan is not None:
            data[f'Chan{args.chan}'].plot(title=title, xlabel=xlabel, ylabel=ylabel)
            plt.legend()
        else:
            subplot_cfg = calculate_subplot_dims(len(df.columns))
            data.plot(subplots=True, layout=(subplot_cfg['rows'], subplot_cfg['cols']), title=title,
                      xlabel=xlabel, ylabel=ylabel, figsize=(11, 8.5), sharey=True)
            plt.tight_layout()

    if args.xlim:
        plt.xlim(args.xlim)
        if any(val > 1000 for val in args.xlim):
            plt.subplots_adjust(wspace=0.2)

    plt.savefig(f"{os.path.splitext(args.file)[0]}.png", format='png')
    plt.show()


if __name__ == '__main__':
    PARSER = ArgumentParser(description='Optional app description')
    PARSER.add_argument('-b', '--baseline', action='store_true', help='Plots MCA spectra')
    PARSER.add_argument('-c', '--chan', type=int, dest='chan',
                        help="The channel that you'd like to plot.")
    PARSER.add_argument('-f', '--file', type=str, dest="file", required=True,
                        help="The file containing the data to read.")
    PARSER.add_argument('--freq', type=int, dest='freq',
                        help="The sampling frequency used to collect list-mode data. Ex. 250")
    PARSER.add_argument('-l', '--lmd', action='store_true',
                        help="Tells the program that the file is list-mode data.")
    PARSER.add_argument('-m', '--mca', action='store_true', help='Plots MCA spectra')
    PARSER.add_argument('-x', '--xlim', dest='xlim',
                        type=lambda r: [int(x) for x in r.split(",")],
                        help="Comma separated range for X-axis limits. Ex. 10,400")
    PARSER.add_argument('--rev', type=int, dest='rev',
                        help="The firmware used to collect list-mode data. Ex. 30474")
    PARSER.add_argument('-s', '--stats', action='store_true',
                        help="Processes a module's CSV statistics file.")
    PARSER.add_argument('-a', '--adc', action='store_true', help='Plots traces')
    ARGS = PARSER.parse_args()

    if ARGS.xlim:
        if len(ARGS.xlim) != 2:
            PARSER.error("The xlim flag expects two and ONLY two values.")
        if ARGS.xlim[1] < ARGS.xlim[0]:
            ARGS.xlim.sort()

    logging.info("Processing data file.")
    if ARGS.lmd:
        if not ARGS.freq or not ARGS.rev:
            PARSER.error("When requesting list-mode data you must provide us with the "
                         "hardware's sampling frequency and firmware revision.")
        plot_lmd(process_list_mode_data_file(ARGS), ARGS)
    else:
        if not ARGS.stats:
            df = pd.read_csv(ARGS.file, index_col='bin', keep_default_na=False)
            if ARGS.adc:
                plot_csv('adc', df, ARGS)
            if ARGS.mca:
                plot_csv('mca', df, ARGS)
            if ARGS.baseline:
                df.pop("timestamp")
                plot_csv('baseline', df, ARGS)
        else:
            plot_csv('stats', pd.read_csv(ARGS.file, keep_default_na=False), ARGS)
