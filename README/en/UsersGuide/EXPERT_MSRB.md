<!-- EXPERT_MSRB.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 10月  7 09:17:02 2018 (+0800)
;; Last-Updated: 日 5月 19 21:52:40 2019 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 3
;; URL: http://wuhongyi.cn -->

# Module Variables

![Module Variables](/img/ModuleVariables.png)


In addition to distributing the global clock signal, the Pixie-16 rear I/O trigger module can also share global triggers and run synchronization signals. The global trigger signals include the global validation trigger and global fast trigger, plus the Pixie-16 FPGA data storage buffers’ full flag signal. The run synchronization signals include synchronous run start and stop signals that can be shared among multiple crates.

In order to enable the distribution of such global triggers and run synchronization signals, certain Pixie-16 parameters have to be set properly. The parameter that controls the trigger distribution and run synchronization is the Module Control Register B (ModCSRB).

ModCSRB is a 32-bit parameter with each of 32 bits controlling different operation modes of the Pixie-16 module.

> **[info] Trigger Distribution and Run Synchronization**
>
> For the System Director module that is installed in the Master crate, bits 0, 4, 6 and 11 of ModCSRB should be set to 1 (checked & enabled).
>
> For the Crate Master module that is installed in the Slave crate, bits 0, 6 and 11 of ModCSRB should be set to 1 (checked & enabled).
>
> For the General modules that are installed in both the Slave crate and Master crate, bit 11 of ModCSRB should be set to 1 (checked & enabled).



## Register definition

Module Control Register B affecting the module as a whole.

- bit 0 - MODCSRB_CPLDPULLUP
	- Enable pullups for PXI trigger lines on the backplane through an onboard CPLD. 
	- With the pullups, those PXI trigger lines default to logic high state. 
	- Only when one module actively pulls a line to logic low state will such a line be in the low state. 
	- Therefore signals transmitted over those PXI trigger lines are actively low signals.
	- **Note: enable this bit only for one module per crate (e.g. the crate master module)**
- bit 4 - MODCSRB_DIRMOD
	- Set this module as the Director module so that it can send triggers, trace and header DPM full signal and run synchronization signal to all crates through the rear I/O trigger modules. 
	- Here triggers include fast trigger and validation trigger
	- **Note: enable this bit only for one module among all crates (e.g. the system director module in multi-crate configuration)**
- bit 6 - MODCSRB_CHASSISMASTER
	- Set this module as the chassis master module so that it can send triggers, trace and header DPM full signal and run synchronization signal to the backplane of the local crate. 
	- Here triggers include fast trigger and validation trigger
	- **Note: enable this bit only for one module per crate(e.g. the crate master module)**
- bit 7 - MODCSRB_GFTSEL
	- Select external fast trigger source(=1: external validation trigger, =0: external fast trigger, in case these two signals are swapped at the Pixie-16 front panel input connectors)
- bit 8 - MODCSRB_ETSEL
	- Select external validation trigger source(=1: external fast trigger,
=0: external validation trigger, in case these two signals are swapped at the Pixie-16 front panel input connectors)
- bit 10 - MODCSRB_INHIBITENA
	- Enable(=1) or disable(=0) the use of external INHIBIT signal.
	- When enabled, the external INHIBIT signal in the logic high state will prevent the run from starting until this external INHIBIT signal goes to logic low state.
- bit 11 - MODCSRB_MULTCRATES
	- Set this module to run in the multi-crate mode(=1) or in the local-crate mode(=0). 
	- If the module is running in multi-crate mode, it will use the trace and header DPM full signal and run synchronization signal that are generated and distributed among multiple crates. 
	- If the module is running in local-crate mode, it will use the trace and header DPM full signal and run synchronization signal generated in the local crate.
- bit 12 - MODCSRB_SORTEVENTS
	- Sort(=1) or do not sort(=0) events from all 16 channels of a Pixie-16 module based on the timestamps of the events, before storing the events in the external FIFO.
	- Note: all 16 channels must have the same DAQ parameters setting to use this feature
- bit 13 - MODCSRB_BKPLFASTTRIG
	- Enable(=1) or disable(=0) the sending of 16 local fast triggers to the 16 lines on the backplane of the crate.
	- **Note: only one module can enable this option in each PCI bus segment of a crate(not limited to the crate master module, e.g. any module in each PCI bus segment)**


<!-- EXPERT_MSRB.md ends here -->
