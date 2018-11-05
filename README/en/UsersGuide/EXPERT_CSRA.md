<!-- EXPERT_CSRA.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 10月  7 09:18:51 2018 (+0800)
;; Last-Updated: 一 11月  5 16:57:49 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 3
;; URL: http://wuhongyi.cn -->

# CSRA

![CSRA](/img/CSRA.png)

- The yellow FTS and GTS combinations are used to select the channel fast trigger:
    - When both are not selected, it is local fast trigger.
    - When selecting FTS, it is a latched module fast trigger.
    - FTS is not selected, GTS is selected as a latched channel validation trigger.
- Blue MSE, CSE, MVT, CVT are used to select module/channel validation trigger:
    - MVT is the module validation trigger.
    - CVT is the channel validation trigger.
    - MSE select module validation trigger depends on System FPGA or front panel module GATE.
    - CSE selection channel validation trigger depends on System FPGA or front panel channel GATE.
-  The pink NPR, IPR combination is used to select the treatment of the pileup event:
   - All events are logged when both are not selected, and the accumulated event energy value is invalid.
   - NPR selection does not record stacking events when IPR is not selected.
   - When the NPR is not selected, the event record waveform is accumulated, and the waveform is not recorded when it is not stacked.
   - When both NPR and IPR are selected, only the stacked events are recorded.
- Green CTV, CVS, MVS are used to select module/channel veto:
     - MVS select module veto depending on the front panel module GATE or module validation trigger.
     - CVS select channel veto depending on the front panel channel GATE or channel validation trigger.
     - CTV is whether to enable channel trigger veto.
- The Red contents are the basic settings.
    - Black NTL is to keep the waveform out of range.
    - Black ETS is to record the data of the external clock.
- The remaining BDA does not need to be selected, HE does not matter.





## Register definition

Channel Control Register A affecting each channel individually 

- bit 0 - CCSRA_FTRIGSEL
	- Channel fast trigger selection(=1: module fast trigger from the System FPGA; =0: the selection depends on the value of another bit CCSRA_GROUPTRIGSEL – if CCSRA_GROUPTRIGSEL = 1, select the channel validation trigger from the System FPGA, and if CCSRA_GROUPTRIGSEL = 0, select this channel’s local fast trigger)
- bit 1 - CCSRA_EXTTRIGSEL
	- Module validation trigger selection(=1: module gate input from the Pixie-16 front panel Module Gate LVDS connector; =0: module validation trigger from the System FPGA)
- bit 2 - CCSRA_GOOD
	- Set this channel as a Good channel(=1) or a not Good channel(=0).
	- **When a channel is set to be a not Good channel, it still generates local fast triggers, which could be used in multiplicity computation, etc., but this channel will not record list mode data or MCA data, and will not update its baseline value**
- bit 3 - CCSRA_CHANTRIGSEL
	- Channel validation signal selection(=1: channel gate input from the Pixie-16 front panel Channel Gate LVDS connector; =0:channel validation trigger from the System FPGA)
- bit 4 - CCSRA_SYNCDATAACQ
	- Choose the level of synchronous data acquisition for this channel(=1: stops taking data when the trace or header DPM for any channel of any Pixie-16 module in the system is full; =0: stops taking data only when the trace or header DPM for this channel of this Pixie-16 module is full)
- bit 5 - CCSRA_POLARITY
	- Choose this channel’s input signal polarity(=1: invert input signal’s polarity; =0: do not invert input signal’s polarity).
	- **Please note in Pixie-16, signal processing requires positive rising input signal. So if input signal has a negative falling edge, it should be inverted by setting this CCSRA_POLARITY bit to 1**
- bit 6 - CCSRA_VETOENA
	- Enable(=1) or disable(=0) this channel’s veto. 
	- If veto is enabled, this channel’s fast trigger will be vetoed by either the module veto signal(see bit 20 CCSRA_MODVETOSEL below) or channel veto signal(see bit 19 CCSRA_CHANVETOSEL below). 
	- But if veto is disabled, this channel’s fast trigger will not be vetoed by either veto signal, even if either veto signal is present
- bit 7 - CCSRA_HISTOE
	- Enable(=1) or disable(=0) the histogramming of pulse energy values in the onboard MCA memory. 
	- However, the current Pixie-16 firmware always histograms pulse energy values in the onboard MCA memory. 
	- So this CCSRA_HISTOE is essentially not in use at the moment
- bit 8 - CCSRA_TRACEENA
	- Enable(=1) or disable(=0) trace capture in the list mode run for this channel
- bit 9 - CCSRA_QDCENA
	- Enable(=1) or disable(=0) QDC sums recording in the list mode run for this channel. 
	- There are a total of 8 QDC sums for each event
- bit 10 - CCSRA_CFDMODE
	- Enable(=1) or disable(=0) CFD trigger in the list mode run for this channel. 
	- CFD trigger is used to latch sub-sample timing for the event time of arrival or timestamp
- bit 11 - CCSRA_GLOBTRIG
	- Enable(=1) or disable(=0) the requirement of module validation trigger for this channel. 
	- If enabled, only when module validation trigger overlaps the channel fast trigger will the events be recorded for this channel
- bit 12 - CCSRA_ESUMSENA
	- Enable(=1) or disable(=0) the recording of raw energy sums and baseline values in the list mode run for this channel. 
	- There are a total of three raw energy sums and one baseline value for each event. 
	- **Please note the baseline value is stored in the format of 32-bit IEEE float point(IEEE 754)**
- bit 13 - CCSRA_CHANTRIG
	- Enable(=1) or disable(=0) the requirement of channel validation trigger for this channel. 
	- If enabled, only when channel validation trigger overlaps the channel fast trigger will the events be recorded for this channel
- bit 14 - CCSRA_ENARELAY
	- Switch between two attenuations or gains for the input signal in this channel through an input relay(=1: close the input relay resulting in no input signal attenuation; =0: open the input relay resulting in a 1⁄4 input signal attenuation)
- bit 15/16 - CCSRA_PILEUPCTRL/CCSRA_INVERSEPILEUP
	- Control normal pileup rejection(bit 15) and inverse pileup rejection(bit 16) for list mode runs:
	- 1) Bits [16:15] = 00, record all events
	- 2) Bits [16:15] = 01, only record single events, i.e., reject piled up events
	- 3) Bits [16:15] = 10, record everything for piled up events, but will not record trace for single events even if trace recording is enabled, i.e., only record event header
	- 4) Bits [16:15] = 11, only record piled up events, i.e., reject single events
	- In all cases, if the event is piled up, no energy will be computed for such event
- bit 17 - CCSRA_ENAENERGYCUT
	- Enable(=1) or disable(=0) the “no traces for large pulses” feature. 
	- If enabled, trace will be not be recorded if the event energy is larger than the value set in DSP parameter EnergyLow
- bit 18 - CCSRA_GROUPTRIGSEL
	- Select channel fast trigger – this bit works together with the CCSRA_FTRIGSEL bit(bit 0): if CCSRA_FTRIGSEL=1, this CCSRA_GROUPTRIGSEL bit has no effect; if CCSRA_FTRIGSEL=0, then if CCSRA_GROUPTRIGSEL=1, select the channel validation trigger from the System FPGA, and if CCSRA_GROUPTRIGSEL=0, select this channel’s local fast trigger
- bit 19 - CCSRA_CHANVETOSEL
	- Channel veto signal selection(=1: channel validation trigger from the System FPGA; =0: channel gate input from the Pixie-16 front panel Channel Gate LVDS connector)
- bit 20 - CCSRA_MODVETOSEL
	- Module veto signal selection(=1: module validation trigger from the System FPGA; =0: module gate input from the Pixie-16 front panel Module Gate LVDS connector)
- bit 21 - CCSRA_EXTTSENA
	- Enable(=1) or disable(=0) the recording of the 48-bit external clock timestamp in the event header during list mode run for this channel


<!-- EXPERT_CSRA.md ends here -->
