# example_pixie16api

This sample C++ code can be used to test basic functionality to Pixie modules. This single code
creates both

* example_pixie16_sdk and
* example_pixie16_legacy.

`example_pixie16_legacy` compiles and links against the Legacy C implementation. Most users will be
familiar with this implementation. `example_pixie16_sdk` compiles and links against the new library
`Pixie16Api.so`. This library provides users a legacy compatible C wrapper to the `PixieSDK`.

Users will note that the difference between using the Legacy library and the Pixie16Api is simply the
linked library and a change to the included header file. This strikes a balance between maintaining
backward compatibility, while also allowing users access to enhanced functionality.

## Configuration file format
The configuration file format is a JSON file. The file contains a single array element, where each
element represents a module in the configuration. The software will automatically determine the
number of modules in the system according to the number of objects in the file. The order of the
objects defines the slot to module number mapping. In the example below, Slot 2 will map to Module 0.
```
[
  {
    "slot": <slot number>,
    "dsp": {
      "ldr": "<absolute or relative path to DSP LDR file>",
      "par": "<absolute or relative path to binary settings file>",
      "var": "<absolute or relative path to DSP VAR file>"
    },
    "fpga": {
      "fippi": "<absolute or relative path to FPGA FIPPI settings file>",
      "sys": "<absolute or relative path to FPGA SYS settings file>",
    }
  },
  <repeat above object with next module configruation>
]
```

### Example config

```
[
  {
    "slot": 2,
    "dsp": {
      "ldr": "/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/dsp/Pixie16DSP_revfgeneral_16b250m_r35921.ldr",
      "par": "pixie.set",
      "var": "/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/dsp/Pixie16DSP_revfgeneral_16b250m_r35921.var"
    },
    "fpga": {
      "fippi": "/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/firmware/fippixie16_revfgeneral_16b250m_r36563.bin",
      "sys": "/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin",
    }
  }
]
```

## Usage Instructions

These instructions assume that you've built and installed PixieSDK into the default location.

### Linux
1. Create the directory
   ```shell script
   mkdir ~/pixie_sdk_example
   cd ~/pixie_sdk_example
   ```
2. Copy the sample configuration file from the repo
   ```shell script
   cp /usr/local/xia/PixieSdk/share/config/example_config.json .
   ```
3. Update the configuration file. You can name it whatever you want.
4. Execute the program to boot the modules
   ```shell script
   LD_LIBRARY_PATH=/usr/local/xia/PixieSDK/lib/ /usr/local/xia/PixieSDK/bin/pixie_sdk_example boot <name of config file>
   ```

### Windows
Windows will work similarly to Linux. You'll need to add the PixieSDK library and bin paths to your `Path` variable.