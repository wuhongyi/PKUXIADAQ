<!-- Logic.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 六 5月 26 09:17:06 2018 (+0800)
;; Last-Updated: 一 9月 17 13:32:55 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 5
;; URL: http://wuhongyi.cn -->

# Logic

<!-- toc -->

对于某路信号的每个事件是否有效被记录取决于：
- Fast trigger select  (一级trigger)
- Control logic  (二级trigger)

Fast trigger select：
- Local fast filter
- Channel validation trigger
- Module fast trigger

Control logic：
- Module validation trigger
- Channel validation trigger
- Veto
- Pileup
- ...


![Signal Process FPGA](/img/signalprocessfpga.png)

**TODO 这里需要补充**

-----

## Module Fast Trigger (for trigger)

Module fast trigger 有以下四种来源可供选择：
- Ext_FastTrig_In(来源于本插件)
	- Ext_FastTrig_Sel(前面板 TTL 输入)
	- Int_FastTrig_Sgl(内部某路 FT)
	- FTIN_Or(内部 FT 的 OR)
	- LVDS_ValidTrig_FP(前面板网口输入)
	- ChanTrig_Sel(内部某路的 valid trigger)(与 module validation trigger 共用一个设置)
- FT_LocalCrate_BP(本机箱中指定插件送出的 trigger)
- FT_In_BP(多机箱中指定机箱上指定插件发送的 trigger)
- FT_WiredOr(本机箱中所有插件发送出 trigger 的 OR)



## Module Validation Trigger (for control logic)

Module validation trigger 有以下来源可供选择：
- Ext_ValidTrig_In(来源于本插件)
	- Ext_ValidTrig_Sel(前面板 TTL 输入)
	- Int_ValidTrig_Sgl(内部某路 FT)
	- FTIN_Or(内部 FT 的 OR)
	- LVDS_ValidTrig_FP(前面板网口输入)
	- ChanTrig_Sel(内部某路的 valid trigger) )(与 module fast trigger 共用一个设置)
- ET_LocalCrate_BP(本机箱中指定插件送出的 trigger)
- ET_In_BP(多机箱中指定机箱上指定插件发送的 trigger)
- ET_WiredOr(本机箱中所有插件发送出 trigger 的 OR)
- 前面板 module GATE 输入 LVDS 信号



## Channel Validation Trigger(for trigger/control logic)

Channel validation trigger来源有以下选择：
- 每路独立设置，来源于多重性选择
- 每路独立设置，来源于符合
- 每4路共用一个设置，来源于左、中、右插件某路的 FT
- 每4路共用一个设置，来源于自身 FT 与 Ext_FastTrig_In 的符合
- 每路独立设置，前面板 channel GATE 输入 LVDS 信号(与前面板 Veto 共用一个输入口)


## Veto


来源于 ModuleVeto 与 ChannelVeto 的 OR
- ModuleVeto 来源有两个选择：
	- Module Validation Trigger
	- 前面板 Module Gate
- ChannelVote 来源有两个选择：
	- Channel Validtion Trigger
	- 前面板 Gate input for channel (与前面板 Channel validation trigger 共用一个输入口)


## System FPGA （coincidence/multiplicity）


![System FPGA](/img/SystemFPGA.png)

Multiplicity：对设置的该channel来说，左邻插件、自身插件、右邻插件共48路，可以选择参与多重性选择的路数

Coincidence：对设置的该channel来说，左邻插件、自身插件、右邻插件，每个插件均满足设置的符合条件，才能给出符合

![fast trigger stretch/delay](/img/fasttrigger_stretch_delay.png)

其它插件的fast filter通过机箱背板传到该插件需要时间，大约100ns左右。因此通过调节门宽、延迟来保证符合、多重性选择。
- Fast trigger stretch length 设置 fast filter 门宽，
- fast trigger delay length 设置 fast filter 延迟。

*Control logic (module/channel validation trigger)*

![validation trigger](/img/validationtrigger.png)

特别需要注意信号经过背板传输大约需要时间 100 ns。





<!-- Logic.md ends here -->
