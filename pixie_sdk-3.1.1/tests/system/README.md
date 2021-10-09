# PixieSDK - System Tests

This folder contains various system level tests for the PixieSDK. We define system tests as being
any test that requires hardware to be present. For example, we can't test whether or not we can
write to and read from the DSP memory without having the DSP connected to the system.

## test_direct_communication

This test program allows users direct access to the modules via the hardware level APIs. It doesn't
use any of the functionality provided at the User API level. Users can test the following features:

* booting modules
* writing to and reading from the
    * DSP
    * Main Memory (MCA)
* Checking the status of the External FIFO (list-mode data)
* Direct register access

The program has a dry-run mode activated with the `--dry_run` switch. This switch disables any
communication with the modules and starts the API in the `Offline` setting.

### Configuration File

For the moment, we're only allowing interaction with a single module type in the system. The
configuration file should take the following form:

```shell
1
5
/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/firmware/syspixie16_revfgeneral_adc250mhz_r33339.bin
/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/firmware/fippixie16_revfgeneral_16b250m_r36563.bin
FPGATrig
/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/dsp/Pixie16DSP_revfgeneral_16b250m_r35921.ldr
pixie.set
/usr/local/xia/pixie/firmware/revf_general_16b250m_r35921/dsp/Pixie16DSP_revfgeneral_16b250m_r35921.var
```

Where the first line is the number of modules in the system (only 1 supported) and 5 is the slot
number of that module. The remaining files are firmware for the FPGAs and DSP along with the DSP
settings file. Multiple modules can be added by adding their slots and updating the number of
modules in the system. **Note** This will only work if all modules have the same type!

```shell
2
5
10
...
```

### Test Data Patterns

The software recognizes a number of test data patterns. Each pattern may be useful to test different
aspects of the system.

* HI_LO
    * Even elements = `0xAAAA5555`
    * Odd elements = `0x5555AAAA`
* FLIP_FLOP
    * Even elements = `0xA0500A05`
    * Odd elements = `0x50A0050A`
* RAMP_UP - monotonic increase from `0` to `data_size - 1`
* RAMP_DOWN - monotonic decrease from `data_size` to `1`
* CONSTANT - All values equal to `0x50f750fa`
* EVEN_BITS - All elements equal to `0xA5A5A5A5`
* ODD_BITS - All elements equal to `0x5A5A5A5A`
* ZERO - All elements equal to `0x0`

### Usage

#### Booting

* Perform a full boot - 0x7F
  ```shell
  test_direct_communication boot cfg.txt
  ```
* Perform a partial boot - 0x70
  ```shell
  test_direct_communication boot cfg.txt -b 0x70
  ```

#### Direct Register Access

This type of access will only write a single 32-bit word to the provided memory register. The memory
address is referenced from the modules PLX Virtual Address. We use `Pixie_Register_IO`.

```shell
test_direct_communication raw cfg.txt -wr -d 0x1 -a 0x10
```

#### DSP Tests

All of the examples below perform a full boot of the system and test the DSP and test against memory
address 0x50000.

* Perform a write/read with constant data at memory address 0x50000
   ```shell
   test_direct_communication dsp cfg.txt -wr -p CONSTANT -a 0x50000
   ```
* Write data in a RAMP_UP pattern once, and then read three times.
   ```shell
   test_direct_communication dsp cfg.txt -wr -p RAMP_UP -a 0x50000 --one_write -l 3
   ```
* Write data in the EVEN_BITS pattern and read it back in a burst read.
   ```shell
   test_direct_communication dsp cfg.txt -wr -p EVEN_BITS -a 0x50000 --burst_read
   ```
* Write and Read Constant data three times
   ```shell
   test_direct_communication dsp cfg.txt -wr -p CONSTANT -a 0x50000 -l 3
   ```
* Don't actually perform any of the the module interaction, just print what we would have done.
   ```shell
   test_direct_communication dsp cfg.txt -p CONSTANT -a 0x50000 -w -r --dry_run
   ```

#### External FIFO

All of these examples perform a full boot of the system.

* Read the external FIFO data
    ```shell
    test_direct_communication external_fifo cfg.txt -r
    ```
* Check the number of words in the External FIFO
    ```shell
    test_direct_communication external_fifo cfg.txt -s
    ```

#### Main Memory (MCA)

All of these examples perform a full boot of the system.

* Clear the MCA memory
    ```shell
    test_direct_communication mca cfg.txt -c
    ```
* Read data from the MCA memory and compare it to the CONSTANT data pattern
    ```shell
    test_direct_communication mca cfg.txt -r -p CONSTANT
    ```
* Write CONSTANT data to the MCA memory and read it back
    ```shell
    test_direct_communication mca cfg.txt -wr -p CONSTANT
    ```
* Write CONSTANT data to the MCA memory
    ```shell
    test_direct_communication mca cfg.txt -w -p CONSTANT
    ```
