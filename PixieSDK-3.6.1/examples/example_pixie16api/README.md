# example_pixie16api

`example_pixie16api` compiles and links against `Pixie16Api.so`. This library provides users a C
wrapper to the `PixieSDK`. We also provide a python example that uses the same configuration file
and library.

## Configuration file

### File Format

The configuration file uses [JSON](https://www.json.org/json-en.html). This file format is ideal as
it provides us a semi-structured format that allows expansion without impacting older versions.

### Slot Definition Order

The software will automatically determine the number of requested modules in the system according to
the number of objects in the array. Note that this is separate from the process that the SDK uses to
identify the PLX 9054 devices present. The order of the objects defines the slot to module number
mapping. For example, Slot 2 will map to Module 0.

### Module definitions

Each requested module must have a JSON array element. These elements contain the information
necessary for the SDK to fully initialize and boot the hardware.

| Name | Description | Elements | Required? |
|---|---|---|---|
| slot | Defines the physical slot this module occupies | N/A | Yes |
| dsp | Defines the DSP firmware and settings file to load to the hardware. | ldr, var, par | Yes |
| fpga | Defines the FPGA firmware to load to the hardware. | fippi, sys | Yes |
| fw | Used during parallel booting to associated a registered firmware with the module | version, revision, adc_msps, adc_bits | Yes, if using parallel boot functionality or python example. |
| worker | Used to setup the module's list-mode worker FIFO configuration | bandwidth_mb_per_sec, buffers, dma_trigger_level_bytes, hold_usecs, idel_wait_usecs, run_wait_usecs | No |

### Example config

The same configuration file is used on both Linux and Windows systems. Please note that on Windows
that `\` characters in paths need to be escaped. For example

```
"C:\\Program Files (x86)\\XIA\\Pixie16_VB 2.3.1\\DSP\\Pixie16DSP_revfgeneral_16b250m_r35921.ldr"
```

#### Base format

```json
[
  {
    "slot": "<slot number>",
    "dsp": {
      "ldr": "<absolute or relative path to DSP LDR file>",
      "par": "<absolute or relative path to binary settings file>",
      "var": "<absolute or relative path to DSP VAR file>"
    },
    "fpga": {
      "fippi": "<absolute or relative path to FPGA FIPPI settings file>",
      "sys": "<absolute or relative path to FPGA SYS settings file>"
    }
  }
]
```

#### Single module configuration

```json
[
  {
    "slot": 2,
    "dsp": {
      "ldr": "/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/dsp/Pixie16DSP_revfgeneral_16b250m_r35921.ldr",
      "par": "pixie.json",
      "var": "/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/dsp/Pixie16DSP_revfgeneral_16b250m_r35921.var"
    },
    "fpga": {
      "fippi": "/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/firmware/fippixie16_revfgeneral_16b250m_r36563.bin",
      "sys": "/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin"
    }
  }
]
```

#### Multi-module configuration w/ parallel boot

```json
[
  {
    "slot": 2,
    "dsp": {
      "ldr": "/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/dsp/Pixie16DSP_revfgeneral_16b250m_r35921.ldr",
      "par": "pixie.json",
      "var": "/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/dsp/Pixie16DSP_revfgeneral_16b250m_r35921.var"
    },
    "fpga": {
      "fippi": "/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/firmware/fippixie16_revfgeneral_16b250m_r36563.bin",
      "sys": "/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin"
    },
    "fw": {
      "version": 35921,
      "revision": 15,
      "adc_msps": 250,
      "adc_bits": 16
    }
  },
  {
    "slot": 4,
    "dsp": {
      "ldr": "/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/dsp/Pixie16DSP_revfgeneral_16b250m_r35921.ldr",
      "par": "pixie.json",
      "var": "/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/dsp/Pixie16DSP_revfgeneral_16b250m_r35921.var"
    },
    "fpga": {
      "fippi": "/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/firmware/fippixie16_revfgeneral_16b250m_r36563.bin",
      "sys": "/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin"
    },
    "fw": {
      "version": 35921,
      "revision": 15,
      "adc_msps": 250,
      "adc_bits": 16
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
   cp /usr/local/xia/PixieSDK/share/config/example_config.json .
   ```
3. Update the configuration file. You can name it whatever you want.
4. Execute the program to boot the modules
   ```shell script
   LD_LIBRARY_PATH=/usr/local/xia/PixieSDK/lib/ /usr/local/xia/PixieSDK/bin/example_pixie16api boot <name of config file>
   ```

### Windows

Windows will work similarly to Linux. You'll need to add the PixieSDK library and bin paths to
your `Path` variable.