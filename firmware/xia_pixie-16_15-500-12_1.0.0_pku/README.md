# Pixie-16 Revision F 500 MSPS 12 bit Firmware
This firmware is only for release to PKU.

## System FPGA

### New Features

* Output 4-ch debug signals of the front panel A to the chassis backplane's TriggerAll bits [28:31] using TrigConfig3[0] control enable/disable
* Multiplicity results output from RJ45 port regardless of MultiplicityMaskHigh[31]=0 or 1
* Added option to select external clock timestamp (clock and clear) signals from the backplane.
	* TriggerAll27 - EXT_TS_CLK
	* TriggerAll26 - EXT_TS_CLR
* Added option to select inhibit from the backplane
	* TriggerAll25 - INHIBIT
* The switch to select from the backplane or front panel is TrigConfig3[1]

## DSP 

### New Features

* The event energy value is set to 0 when the calculated energy is negative
* The pileup event energy is not set to 0; value is calculated in the same way as non-pileup events
* Added a new feature "Discard a list mode event if computed event energy is smaller than EMIN (EnergyLow)"
	* This feature can be enabled or disabled by setting the bit 22 of ChanCSRA: 1: enable; 0: disable
	* If bit 22 of ChanCSRA is set to 1, then if computed event energy is smaller than EMIN (EnergyLow), the event will not be recored. Otherwise, it will be recored.
	