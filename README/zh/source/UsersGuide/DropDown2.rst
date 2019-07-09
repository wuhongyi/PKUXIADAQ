.. DropDown2.rst --- 
.. 
.. Description: 
.. Author: Hongyi Wu(吴鸿毅)
.. Email: wuhongyi@qq.com 
.. Created: 三 7月  3 10:49:52 2019 (+0800)
.. Last-Updated: 四 7月  4 15:51:35 2019 (+0800)
..           By: Hongyi Wu(吴鸿毅)
..     Update #: 9
.. URL: http://wuhongyi.cn 

---------------------------------
Expert
---------------------------------

本下拉栏中调节内容为高阶内容，需要对获取逻辑有一定基础的人学习掌握。


^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Module Variables
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

"""""""""""""""""""""""""""""""""
控制界面
"""""""""""""""""""""""""""""""""

.. image:: /_static/img/ModuleVariables.png

In addition to distributing the global clock signal, the Pixie-16 rear I/O trigger module can also share global triggers and run synchronization signals. The global trigger signals include the global validation trigger and global fast trigger, plus the Pixie-16 FPGA data storage buffers’ full flag signal. The run synchronization signals include synchronous run start and stop signals that can be shared among multiple crates.

In order to enable the distribution of such global triggers and run synchronization signals, certain Pixie-16 parameters have to be set properly. The parameter that controls the trigger distribution and run synchronization is the Module Control Register B (ModCSRB).

ModCSRB is a 32-bit parameter with each of 32 bits controlling different operation modes of the Pixie-16 module.


.. NOTE:: **Trigger Distribution and Run Synchronization**

  For the System Director module that is installed in the Master crate, bits 0, 4, 6 and 11 of ModCSRB should be set to 1 (checked & enabled).
 
  For the Crate Master module that is installed in the Slave crate, bits 0, 6 and 11 of ModCSRB should be set to 1 (checked & enabled).
 
  For the General modules that are installed in both the Slave crate and Master crate, bit 11 of ModCSRB should be set to 1 (checked & enabled).

  
"""""""""""""""""""""""""""""""""
寄存器定义
"""""""""""""""""""""""""""""""""

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
	- Select external validation trigger source(=1: external fast trigger, =0: external validation trigger, in case these two signals are swapped at the Pixie-16 front panel input connectors)
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

----

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
CSRA
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

"""""""""""""""""""""""""""""""""
控制界面
"""""""""""""""""""""""""""""""""

.. image:: /_static/img/CSRA.png

- 黄色 FTS、GTS 组合来选择 channel fast trigger：
	- 两个均不选时为 local fast trigger
	- 选择 FTS 时为 latched module fast trigger
	- FTS 不选、GTS 选上时为 latched channel validation trigger
- 蓝色 MSE、CSE、MVT、CVT 用来选择 module/channel validation trigger：
	- MVT 为是否开启 module validation trigger
	- CVT 为是否开启 channel validation trigger
	- MSE选择 module validation trigger 来源于 System FPGA 还是前面板 module GATE
	- CSE选择 channel validation trigger 来源于 System FPGA 还是前面板 channel GATE
- 粉红色 NPR、IPR 组合选择 pileup 事件的处理：
	- 两个均不选时则记录所有事件，堆积事件能量值为无效
	- NPR 选择 IPR 不选时不记录堆积事件
	- NPR 不选 IPR 选择时堆积事件记录波形、不堆积时候不记录波形
	- 两个均选择时只记录堆积事件
- 绿色CTV、CVS、MVS用来选择module/channel veto：
	- MVS 选择 module veto 来源于前面板 module GATE 还是 module validation trigger
	- CVS 选择 channel veto 来源于前面板 channel GATE 还是 channel validation trigger
	- CTV 为是否开启 channel trigger veto
- 红色为基础设置
	- 黑色 NTL 是否保留超出量程的波形
	- 黑色 ETS 是否记录外部时钟的数据
- 剩余的BDA不选，HE不管
	   


"""""""""""""""""""""""""""""""""
寄存器定义
"""""""""""""""""""""""""""""""""

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

----

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Logic Set
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

"""""""""""""""""""""""""""""""""
控制界面
"""""""""""""""""""""""""""""""""

.. image:: /_static/img/LogicTrigger.png

	   
**TODO**

.. image:: /_static/img/stretchlength.PNG


	   

.. 
.. DropDown2.rst ends here
