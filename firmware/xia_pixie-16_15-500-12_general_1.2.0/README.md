# Pixie-16 15-500-12 Firmware 1.2.0

## Change Log

### New Features

* added chantrig_sel

### Updates

* expanded `ext_fasttrig_in` and `ext_validtrig_in`
* changed `front_ctrl_i0` and `front_ctrl_i1` to 0

### Bug Fixes

* Corrected `trigconfig*` assignment for test signals.
    * `trigconfig0` bits [7:4] - Select source for external fast trigger
    * `trigconfig0` bits [25:24] - Select source for module fast trigger
    * `trigconfig0` bits [27:26] - Select source for module validation trigger
    * `trigconfig0` bits [31:28] - Select source for external validation trigger
    * `trigconfig2` bits [31:28] - Select source for channel trigger from one of 16 local channels