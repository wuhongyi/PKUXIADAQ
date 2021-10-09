# Glossary of Terms

## Analog to Digital Converter (ADC)

This chip samples the analog signal and digitizes it. The digitized signal is then sent to the FIPPI
for processing. In some cases module’s serial number can be used to determine information about the
ADC.

### ADC Information based on Serial Number

> For modules with serial number greater than 1034, the ADC’s information is stored in the modules I2C serial EEPROM starting at address 99 and taking up 3 bytes.

| Serial Number | Rev | ADC Bit Resolution | ADC Sampling Frequency (MSPS) |
|---|---|---|---|
| 0 - 274 | A - D | 12 | 100 |
| 1000 - 1034 | F | 12 | 250 |

<hr>

## Bit Resolution

This refers to the bit resolution of the module’s ADC. This value determines how precise the ADC can
sample the incoming signal. It provides a limit on the system’s energy resolution. Typical bit
resolutions are 12, 14, and 16.

<hr>

## Crate

This refers to the chassis that holds the modules. The crate provides several critical functions:

1. it interfaces with the host computer via the crate controller,
2. it provides channels for the modules to communicate with each other,
3. and it delivers power to the modules.

The first slot in a crate is reserved for the crate controller. The remaining slots (2+) are
available for Pixie-16 modules.

| Revision | Crate Manufacturer | Connection Type | Available Slots |
|---|---|---|---|
| A | ELMA | PCI | 8 |
| B | ELMA | PCI | 8 |
| C | ELMA | PCI | 14 |
| D | Wiener | PCI | 14 |
| E | Wiener | PCIe | 14 |

<hr>

## Crate Controller

This is a National Instruments device that allows users to interact with the modules stored in a
crate. It’s always installed in the first slot of a crate. There are various types of crate
controllers. Some of these come with integrated computers making the crate a self-contained unit.

<hr>

## Digital Signal Processor (DSP)

The device on the module that handles some calculations, run control, etc.

<hr>

## DSP Variables

A variable is any value defined in the DSP VAR file. Variables can be used for both input and
output. They are used at both the module and channel level. These values are not validated or
transformed before being written to the DSP.

Examples include `MultiplicityMaskL`, `LiveTimeA`, `ChanCSRa`, `CoincPattern`, `HardwareID`.

<hr>

## Electrically erasable programmable read-only memory (EEPROM)

An on-board chip that holds information such as the module’s Serial Number, Revision, and ADC
information.

<hr>

## External FIFO (ExtFIFO) or List-Mode FIFO

A chip on-board the module that handles information related to list-mode data runs. It’s accessible
via the System FPGA.

<hr>

## Filter Integrator and Pulse Pileup Inspector (FIPPI)

Refers to the pulse processing FPGA code, and sometimes it refers to an FPGA running the pulse
processing code. See Signal-Processing FPGA for more information

<hr>

## Firmware

The files that are loaded onto the module’s on-board DSP and FPGAs. Firmware varies depending on the
module’s ADC’s bit resolution and sampling frequency. It’s critical that firmware matches with the
module’s FPGAs. Firmware can be matched to the HW using the ADC information stored in the module’s
EEPROM.

### DSP

The DSP firmware has 3 files: the image (.ldr), the listing (.lst), and the variable file (.var).
The image file contains the actual code that we upload to the DSP while booting the module. The
variable file contains a list of variables and their memory addresses. The listing file is primarily
used for debugging purposes. It contains a full address list of the DSP’s memory, including internal
variables.

For example, the `Pixie16_revfgeneral_12b250m_firmware_release_05182019.zip` release has the
following files:

* Pixie16DSP_revfgeneral_12b250m_r41847.ldr
* Pixie16DSP_revfgeneral_12b250m_r41847.lst
* Pixie16DSP_revfgeneral_12b250m_r41847.var

### FPGA

There are two types of FPGA firmware files: SYS and FIPPI. The SYS files get loaded onto the System
FPGA. The FIP file gets loaded onto the FIPPI FPGA.

<hr>

## Host Bus Request (HBR)

A procedure by which the System FPGA takes control of the bus connecting the FIPPI FPGAs from the
DSP.

<hr>

## Load

### Software

Refers to the act of moving data from the host system to the module. This includes putting the FIPPI
firmware onto the chip or writing variables into the DSP memory.

<hr>

## Main Memory

A chip on-board the module that houses spectra from histogram (MCA) runs.

<hr>

## Module / Board

This refers to one unit of the Pixie-16 hardware that gets shipped to end users. Colloquially the
boards are called by their alphabetical revision (A-H). A module is uniquely identified by its
Serial Number.

<hr>

## Parameter

Parameters are the user facing names of DSP Variables. These are used for inputting user values to
the system. They are used at both the module and channel level. The value of the parameter undergoes
validation and, in some cases transformation before being written to the hardware.

For example, users set the parameter `VOFFSET` in units of Volts. The SDK then transforms the value
using the following transformation.

```c++
auto offset_dac = param::value_type (65536 * (value / DAC_VOLTAGE_RANGE + 0.5));

// 16-bit DAC, range check.
if (offset_dac > 65535)
    offset_dac = 65535;

```

The final value is then written to the DSP variable `OffsetDAC`.

Examples include `TAU`, `XDT`, `MultiplicityMaskH`, `TRACE_DELAY`.

<hr> 

## Parameter Configuration

Typically done by the user to setup the module for their specific requirements. Users often maintain
their own configuration files in the form of bash scripts. These scripts make repeated calls to the
parameter write API functions to configure the module.

A configuration file is specifically a file that deals with parameters.

<hr>

## PCI/PCIe Bridge

A device installed in a host computer that allows a hard line connection to a crate controller.
There are various types of bridges

<hr>

## PLX (Broadcom) Driver

The driver installed on a host computer that facilitates communication between the host and the
Pixie Module. Each Pixie-16 module contains a PLX 9054 chip. They show up under the host’s PCI bus
as PLX 9054 devices. The latest PLX Driver can be downloaded directly from Broadcom.

```shell
0b:0c.0 Bridge: PLX Technology, Inc. PCI9054 32-bit 33MHz PCI <-> IOBus Bridge (rev 0b)
```

> Broadcom acquired PLX Technologies and changed the name of the driver package.

<hr>

## Read

### Software

Read refers to the act of moving data from the host system’s disk into host memory.

<hr>

## Revision

Revision Refers to the Pixie-16 model number. Revisions are letter based starting with A and
proceeding to F. This value is stored in the third byte of the module's I2C serial EEPROM. For
revisions B-F this value took the hex value associated with the revision (0xB - 0xF).

<hr>

## Sampling Frequency

Refers to how quickly the ADC can sample the incoming signal. Typical sampling frequencies are 100,
250, and 500 Mega Samples per Second (MSPS). These values correspond to samples being taken every
10, 4, and 2 nanoseconds (ns), respectively.

<hr>

## Serial Number

A number assigned to a module that uniquely identifies it within the company. Starting with Serial
number 256 this value is stored in the first two bytes of the module’s I2C serial EEPROM. The serial
number can be used to determine the module’s ADC sampling frequency and bit resolution in some
cases.

<hr>

## Settings (Set) File

### Legacy Binary Settings file

The Pixie-16 settings file (.set) contains the values of DSP parameters that are downloaded to the
onboard DSP when a Pixie-16 is booted up. In addition, at the end of each run, either MCA run or
list mode run, a settings file will be automatically generated together with either the MCA spectra
file (.mca and .asc) or the list mode data file (.bin) and stored in either the MCA data folder or
list mode data folder. Such a settings file also contains the run statistics data for the run that
is just finished.

Each settings file contains settings for 24 Pixie-16 modules. And for each module, there are 1280
entries (32-bit for each entry). So the file size should be exactly 122,880 bytes. In the file,
settings of module #0 are stored first, followed by module #1, #2, … and so on.

See Section 4.2.4 of the Pixie-16 User’s Manual for more information.

### PixieSDK JSON Settings File

We found that customers often want to view the contents of their binary settings files. They would
often write software that allowed them to decode the file when coupled with a DSP VAR file. This
causes issues since users may not recall which DSP VAR file produced the binary settings file. In
addition, they may not have the information related to specifics of the FPGA firmware used.

To remove these issues, we now use a JSON settings file to setup and configure the DSP variables.
This settings file is plain text, and contains a metadata field. This now provides users all of the
information necessary to understand the contents.

We will no longer export an entire 2 crates worth of module configurations to this file. The SDK
only exports configurations for active modules in the system. This file does not contain any of the
read-only variables in the system. Users should access statistics data using the statistics
functions.

<hr>

## Signal-Processing or FIPPI FPGA

The on-board FPGA responsible for calculating filters trigger conditions and other information about
the digitized signal.

<hr>

## System FPGA

The System FPGA links the PCI slave with the DSP and the on-board memory, both the MCA memory and
the list mode External FIFO memory. The host can read out the memory without interrupting the
operation of the DSP. This allows updates of the MCA spectrum while a run is in progress and readout
of list mode data from the External FIFO while the FPGA internal FIFO is being filled with events
data.

The System FPGA also acts as an interface between the DSP, Signal Processing FPGAs and the
backplane. It also has general purpose I/O connections available on the front panel. It can be
configured to distribute trigger and hit pattern information from the Signal Processing FPGAs, and
over the backplane to other Pixie-16 modules. In this way, coincidence or multiplicity decisions can
be made to accept or not accept an event.

<hr>

## Variable

A variable names a DSP memory address defined in the DSP’s VAR file. The variable addresses and
names may change over time. It’s internal facing.

Examples include:  `ModCSRA`, `SlowLength`, `PreampTau`

<hr>

## Variable Initialization

The process by which the SDK reads a settings file into host memory, and then loads those variables
into the module.
