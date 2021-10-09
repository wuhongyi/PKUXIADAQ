Test API
========

A simple command line test interface to exercise the C++ API.

Crate
-----

The firmware for a crate is supplied on the command line using a text file where each line in the
crate firmware file is a firmware file.

The format of a line is:

`<field>,<field>,<field>..<field>`

The fields in order they appear in a line is:

`version` : string

`revision` : int

`device` : string, can be `sys`, `fippi`, or `dsp`

`filename` : string


Module Definition
-----------------

The module definition for crate is required to be supplied on the command line if the
simulation (`-S`) option is provided. The definition is a text file where each line defines a module
in the crate.

The fields are specified as a label/value pair and can appear in any order. It is recommended all
fields are provide, the `var-defaults` is optional.

The format of a line is:

`label=value,label=value...label=value`

The fields are:

`device=number` : `size_t`, the device number order modules are opened

`slot` : `int`, the slot the module occupies

`revision` : `int`, the revision of the module

`eeprom-format` : `int`, set to `1`

`serial-num` : `int`, serial number of the module

`num-channels` : number of channels

`adc-bits` : `int`, number of ADC bits

`adc-msps` : `int`, ADC mega-samples per second

`adc-clk-div` : `int`, the divider of the ADC clock used to clock the FPGA

`var-defaults` : `string`, path to a file of default values

Commands
--------

The test command supports the following command:

boot

~~~~

`boot`

Arguments:

none

Description:

Boot the module using the firmware set passed in on the command line using the
`-C` option.

`par-read`
~~~~~~~~~~

`par-read <mod> [channel] <param>`

Arguments:

`mod` : module number

`channel` : optionally provide a channel number

`param` : parameter to read and print or `all` for all parameters

Description:

Read a parameter or all parameters. If a channel is provided channel parameters are read.

`par-write`

~~~~~~~~~~~

`par-write <mod> [channel] <param> <value>`

Arguments:

`mod` : module number

`channel` : optionally provide a channel number

`param` : parameter to write to the value too

`value` : value to write to the parameter

Description:

Write a parameter with a value. If a channel is provided a channel parameter is
written too.

`var-read`
~~~~~~~~~~

`var-read <mod> [channel] <var>`

Arguments:

`mod` : module number

`channel` : optionally provide a channel number

`var` : DSP variable to read and print or `all` for all variables

Description:

Read a DSP variable or all variables. If a channel is provided channel
variables are read.

`var-write`
~~~~~~~~~~~

`var-write <mod> [channel] <var> <value>`

Arguments:

`mod` : module number

`channel` : optionally provide a channel number

`var` : variable to write to the value too

`value` : value to write to the variable

Description:

Write a value with a value. If a channel is provided a channel variable is written too. The variable
needs to be enabled and writable.
