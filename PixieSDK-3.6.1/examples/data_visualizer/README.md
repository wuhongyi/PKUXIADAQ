# Data Visualizer
This script visualizes the CSV and list-mode data from our example codes.
## Usage
```shell
usage: data_visualizer.py [-h] [-b] [-c CHAN] -f FILE [--freq FREQ] [-l] [-m]
                          [-x XLIM] [--rev REV] [-s] [-a]

Optional app description

options:
  -h, --help            show this help message and exit
  -b, --baseline        Plots MCA spectra
  -c CHAN, --chan CHAN  The channel that you'd like to plot.
  -f FILE, --file FILE  The file containing the data to read.
  --freq FREQ           The sampling frequency used to collect list-mode data.
                        Ex. 250
  -l, --lmd             Tells the program that the file is list-mode data.
  -m, --mca             Plots MCA spectra
  -x XLIM, --xlim XLIM  Comma separated range for X-axis limits. Ex. 10,400
  --rev REV             The firmware used to collect list-mode data. Ex. 30474
  -s, --stats           Processes a module's CSV statistics file.
  -a, --adc             Plots traces
```