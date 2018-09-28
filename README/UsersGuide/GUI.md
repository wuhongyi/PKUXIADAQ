<!-- GUI.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 5月 13 20:23:55 2018 (+0800)
;; Last-Updated: 五 9月 28 13:15:57 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 44
;; URL: http://wuhongyi.cn -->

# GUI

<!-- toc -->


配置好 **parset** 内参数文件
进入 GUI 目录，执行以下命令即可弹出主控制界面

```bash
./pku
```

## 主控制界面

![MainFrame](/img/mainframe.png)

主界面最上方是File、UV_Setup、Expert、Monitor、Offiline五个下拉栏。里面的子菜单如下所示：

- File
	- Exit
	- About
- UV_Setup
	- Base Setup
	- Trigger Filter
	- Energy
	- CFD
	- QDC
	- Decimation
	- Copy Pars
	- Save2File
- Expert
	- Module Variables
	- CSRA
	- Logic Set
- Monitor
	- Hist & XDT
	- Trace & Baseline
- Offiline
	- Adjust Par
	- Simulation(暂未实现)


开启获取主界面之后，选择 **Online Mode** 选项表示在线模式，需要连接机箱，该模式下可使用所有的功能（包括离线分析功能）。不选择**Online Mode** 选项则表示开启离线模式，可设置、修改获取参数，分析已采集波形。

选择、或者不选择 **Online Mode** 选项之后，按 **Boot** 按钮开启初始化过程，可看到最下方 *Information* 栏目中的状态变化。

系统初始化成功后，再次确认 *Setup* 栏中的获取文件存放路径、文件名、文件编号是否有问题，如果有问题则直接修改，确认之后按 **Complete**。

确认 *Setup* 栏中的信息之后， *Control* 栏中的主按钮 **LSRunStart** 则开启，此时点击该按钮，获取则开启，按钮状态更改为 **LSRunStop**，再次点击该按钮，获取结束，运行的 *Run Num* 号码自动加一。再次点击 **LSRunStart** 开启下轮获取。

当前，获取之前可通过最上方的下拉栏里面的子菜单来调节、修改参数。获取数据时请勿操作 *Control*栏之外的所有选项。

*Control* 栏内的 **Online Statistics** 选项开启则获取每 **3 s** 向 *OnineStatistics* 程序发送时时每路信号的输入率、输出率信息。

**点击 Update Energy Monitor 选项一次，则将所有采集卡内部寄存器中每路的一维能谱发送到 Online Statistics 程序，发送该信息会导致一定的死时间，请不要频繁点击该选项。** 


----

## File 下拉栏

本下拉栏内容没有实际用途。

### About

![About](/img/About.png)

软件开发者介绍。之后将会添加主程序基本操作说明。

----

## UV_Setup 下拉栏

本下拉栏中调节内容为基础内容，任何使用Pixie16获取系统的人员都应该熟悉并掌握其调节技巧。

### Base Setup

![Base Setup](/img/BaseSetup.png)

界面下方的 Status 显示为**绿色的 Ready** 时表示可操作该界面，否则需要等待。

界面中 Module 后面的参数用来选择调节的采集卡模块， **Load** 表示读取该采集卡的参数数值， **Apply** 表示将界面中的数值写入采集卡。

界面下方的 Select Channel 后面的参数表示选择用来将界面上该通道的参数复制给其它通道，点击后面 **Copy** 完成复制，然后需要 **Apply** 来将参数写入采集卡。


- 选项 *Gain* 为增益调节，用户可选择 Larger 或者 Small 档，具体每个采集卡这两档所对应的增益参数用户可自行测试或者咨询厂家。
- 选项 *Sign* 选择输入信号的极性，输入正信号选择 "+"，输入负信号则选择 "-"。
- 选项 *GC* 表示是否开启该通道，选择表示开启该通道，不选择表示不开启。
- 选项 *ECT* 选择表示开启CFD功能。

红色的*TC*，*EQS*，*ERB*用来选择输出哪些原始数据：
- 选项 *TC* 选择表示记录波形，此时*TraceDelay*、*TraceLength*生效，不选择则表示不记录波形。
- 选项 *EQS* 选择表示记录八个QDC的积分，不选择则不记录。
- 选项 *ERB* 选择表示记录能量梯形的三部分面积积分及梯形计算的基线数值。


绿色的*TraceDelay*、*TraceLength*为输出数据的点数，**该参数除以采集卡的标称采样率即为波形实际输出数据点数**：
- *TraceDelay* 表示触发前的采集波形长度。
- *TraceLength* 表示整个波形采集长度。
需要特别说明的是采用降频模式时，实际波形长度为 TraceDelay x 2^N / TraceLength x 2^N（N 为降频参数）

蓝色的 *Baseline* 用来调节基线位置，通过电压的补偿将基线调节到用户预期的位置：

- *Baseline* 可调节范围为 0 - 100，表示波形基线落在满量程的位置百分比。例如垂直精度 14 bit采集卡，该参数设置为10意味着降基线补偿调节到满量程 16384 道的 10% 左右即 1638 附近。

紫色的 *DCOffset*、*BLcut* 用户不需要修改，采用自动调节参数即可。本子菜单中修改了 *Baseline*、*Gain*、*Sign* 之后，需要按最下方的 **AdjustOffset**，之后再按**BLcutFinder** 来自动调节这两个参数。


- Good channel. Only channels marked as good will have their events recorded.
This setting has no bearing on the channel's capability to issue a trigger. There
can be a triggering channel whose data are discarded. Channels not marked as
good will be excluded from the automatic offset adjustment.
- Capture trace. When this box is checked, trace will be captured and recorded
for each event, along with other list mode information, e.g. timestamp, energy,
etc.
- Capture QDC sums. When this box is checked, eight QDC sums will be
recorded for each event. QDC sums are consecutive sums of the list mode
trace.
- Enable CFD trigger. Check this box to enable this channel’s CFD trigger.
Otherwise, regular trapezoidal fast trigger will be used.
- Capture raw energy sums and baseline. Check this box to record raw energy
sums and baseline value for each event.



The **Base Setup** page controls the analog gain, offset and polarity for each channel. It is useful to click on **Trace & Baseline** in the top control **Monitor** bar to view the signal read from the ADCs while adjusting these parameters. The display shows one or all 16 channels of a module; you can set the sampling interval for each block to capture a longer time frame in **Hist & XDT** page. Click **Draw** to update the graph.

Pulses from the detector should fall in the range from 0 to 16383(14 bit), with the baseline at ~1638 to allow for drifts and/or undershoots and no clipping at the upper limit. If there is clipping, adjust the Gain and Offset or click on the *AdjustOffset* button to let the software set the DC offsets to proper levels.**TODO 这段还需要进一步修改**

Since the trigger/filter circuits in the FPGA only act on rising pulses, negative pulses are inverted at the input of the FPGA, and the waveforms shown in the ADC trace display include this optional inversion. Thus set the channel’s Polarity such that pulses from the detector appear with positive amplitude (rising edge).


In the **Base Setup**  tab, you can set the total trace length and the pre-trigger trace delay for the waveforms to be acquired in list mode runs.

The trace delay cannot be longer than the trace length, and for each Pixie-16 variant, there is also a limit for the maximum value of trace delay and trace length.


> **[info] trace length in 500 MHz**
>
> For the 500 MHz Pixie-16 modules, the ADCs are running at 500 MHz, but the traces are recorded with 100 MHz clocks in the FPGA with 5 ADC samples captured in each 10 ns interval. In addition, the data packing from the FPGA to the onboard External FIFO is two sets of 5 ADC samples in one transfer. So the trace length should be multiples of 20 ns, i.e., 20 ns, 40 ns, ... for instance, a trace length of 500 ns and a trace delay of 200 ns.





The Pixie-16 constantly takes baseline measurements when no pulse is detected and keeps a baseline average to be subtracted from the energy filter output during pulse height reconstruction. Baseline measurements that differ from the average by more than the BaselineCut value will be rejected as they are likely contaminated with small pulses below the trigger threshold.

A series of baseline measurements for each channel can be viewed in **Trace & Baseline** page, and in the BASELINE panel a histogram of baselines can be built to verify that the Baseline Cut does not reject measurements falling into the main (ideally Gaussian) peak in the baseline distribution.

Usually, it is sufficient to keep Baseline Cut at its default value.


Note: Since the baseline computation takes into account the exponential decay, no pulses should be noticeable in the baseline display if  
- a) the decay time is set correctly and 
- b) the detector pulses are truly exponential.

Baseline Percent is a parameter used for automatic offset adjustment; by clicking on the *AdjustOffses* button, offsets will be set such that the baseline seen in the ADC trace display falls at the Baseline Percent fraction of the full ADC range (e.g. for a 12-bit ADC and Baseline Percent = 10% the baseline falls at ADC step 409 out of 4096 total).





### Trigger Filter

![Trigger Filter](/img/TriggerFilter.png)

界面下方的 Status 显示为**绿色的 Ready** 时表示可操作该界面，否则需要等待。底下按钮的操作同上。

- 参数 *Rise Time* 。。。
- 参数 *Flat Top* 。。。
- 参数 *Thresh.* 表示阈值，该数值的设置是相对 fast filter 波形。


**TODO   这里需要补充一个示意图，还有计算公式。**

Assume the digitized waveform stream can be represented by data series Trace[i], i=0,1,2,... First the fast filter response of the digitized waveform is computed as follows:  
**TODO公式**  
Where FL is called the fast length and FG is called the fast gap of the digital trapezoidal filter.

General rules of thumb for the following important parameters are:

- A longer trigger filter rise time averages more samples and thus allows setting lower thresholds without triggering on noise.
- Typically the threshold should be set as low as possible, just above the noise level.
- A longer trigger filter flat top time makes it easier to detect slow rising pulses.




### Energy

![Energy](/img/Energy.png)

界面下方的 Status 显示为**绿色的 Ready** 时表示可操作该界面，否则需要等待。底下按钮的操作同上。

- 参数 *Rise Time* 。。。
- 参数 *Flat Top* 。。。
- 参数 *Tau* 。。。


**TODO**


The most critical parameter for the energy computation is the signal decay time Tau. It is used to compensate for the falling edge of a previous pulse in the computation of the energy. You can either enter Tau directly for each channel, or enter an approximate value in the right control, select a channel, and click Find it to let the software determine the decay time automatically.
Click Accept it to apply the found value to the channel. (If the approximate value is unchanged,the software could not find a better value.)  


At high count rates, pulses overlap with each other at higher frequency. In order to compute the energy or pulse height of those pulses accurately without the need to wait until they decay back to baseline level completely, the pulse height computation algorithm implemented in the Pixie-16 uses the decay time to compute and remove the contribution from the exponentially decaying tail of the overlapping prior pulse when computing the pulse height of the current pulse.

> **[danger] single exponential decay constant**
>
> It is assumed the pulses have only a single exponential decay constant. If pulses have multiple decay constants, it might be possible to use the decay constant that dominates the decay of the pulse, but the accuracy of pulse height computation will be degraded.





General rules of thumb for the following important parameters are:

- The energy filter flat top time should be larger than the longest pulse rise time.
- The energy filter rise time can be varied to balance the resolution and throughput.
- In general, energy resolution improves with the increase of energy filter rise time, up to an optimum when longer filters only add more noise into the measurement.
- The energy filter dead time TD is about $$2×(T_{rise}+T_{flat})$$, and the maximum throughput for Poisson statistics is 1/(TD*e). For HPGe detectors, a rise time of 4-6us and a flat top of 1us are usually appropriate.
- Choose the smallest energy filter range that allows setting the optimum energy filter rise time. Larger filter ranges allow longer filter sums, but increase the granularity of possible values for the energy filter rise time and flat top time and increase the jitter of latching the energy filter output relative to the rising edge of the pulse. This is usually only important for very fast pulses.


![filter range](/img/filterrange_format.PNG)


### CFD

![CFD](/img/CFDPars.png)

#### 100 MHz and 250 MHz modules

The following CFD algorithm is implemented in the signal processing FPGA of the 100 MHz(Rev. B, C, D and F) and 250 MHz(Rev. F) Pixie-16 modules. 

Assume the digitized waveform can be represented by data series Trace[i], i = 0, 1, 2, ... First the fast filter response(FF) of the digitized waveform is computed as follows:

$$FF[i]=\sum_{j=i-(FL-1)}^{i}Trace[j]-\sum_{j=i-(2\timesFL+FG-1)}^{i-(FL+FG)}Trace[j]$$

Where FL is called the fast length and FG is called the fast gap of the digital trapezoidal filter. Then the CFD is computed as follows:

$$CFD[i+D]=FF[i+D]\times(1-w/8)-FF[i]$$

Where D is called the CFD delay length and w is called the CFD scaling factor(w=0, 1,..., 7).

The CFD zero crossing point(ZCP) is then determined when $$CFD[i]\leq0$$ and $$CFD[i+1]<0$$. The timestamp is latched at Trace point $$i$$, and the fraction time $$f$$ is given by the ratio of the two CFD response amplitudes right before and after the ZCP.

$$f=\frac{CFDout1}{CFDout1-CFDout2}$$

Where CFDout1 is the CFD response amplitude right before the ZCP, and CFDout2 is the CFD response amplitude right after the ZCP(subtraction is used in the denominator since CFDout2 is negative). The Pixie-16 DSP computes the CFD final value as follows and stores it in the output data stream for online or offline analysis.

$$CFD=\frac{CFDout1}{CFDout1-CFDout2} \times N$$

Where N is scaling factor, which equals to 32768 for 100 MHz modules and 16384 for 250 MHz modules, respectively.

![WAVEFORM](/img/adcsamples_cfd.png)  
![FASTFILTER](/img/fastfilter_cfd.png)  
![CFDFILTER](/img/cfdfilter_cfd.png)

Figure shows a sample ADC trace, its fast filter response and its CFD response, respectively. 

The top figure shows a raw ADC trace. After computing the fast filter response on the raw ADC trace using Equation $$FF[i]$$, the fast filter response is compared against the fast filter threshold as shown in the middle figure. The ADC sample where the fast filter response crosses the fast filter threshold is called the fast trigger point, which also starts the search for the CFD zero crossing point. 

The CFD response is computed using Equation $$CFD[i+D]$$ and is shown in the bottom figure(for actual implementation in the firmware, the fast filter response FF is delayed slightly before being used for computing the CFD response so that there are sufficient number of CFD response points to look for the zero crossing point after the fast trigger). To prevent premature CFD trigger as a result of the noise in the CFD response before the actual trigger, a DSP parameter called CFDThresh is used to suppress those noise-caused zero crossing. However, if a zero crossing point cannot be found within a certain period after the fast trigger (typically 32 clock cycles), e.g., due to unnecessarily high CFDThresh, a forced CFD Trigger will be issued and a flag will be set in an event header word to indicate that the recorded CFD time for this event is invalid.

However, the event will still have a valid timestamp which is latched by the fast filter trigger when fast filter crosses over the trigger threshold. The aforementioned CFD parameters correspond to the following DSP parameters.

![Corresponding DSP Parameters for the CFD Parameters](/img/correspondingdspparametersforthecfdparmeters.png)


> **[info] 250 MHz**
>
> In the 250 MHz Pixie-16 modules, the event timestamp is counted with 125 MHz clock ticks, i.e., 8 ns intervals, and two consecutive 250 MHz ADC samples are captured in one 8 ns interval as well. 
>
> The CFD trigger also runs at 125 MHz, but the CFD zero crossing point is still reported as a fractional time between two neighboring 250 MHz ADC samples, which are processed by the FPGA in one 125 MHz clock cycle. 
>
> However, the CFD zero crossing point could be in either the odd or even clock cycle of the captured 250 MHz ADC waveforms. 
>
> Therefore, the firmware outputs a "CFD trigger source" bit in the output data stream to indicate whether the CFD zero crossing point is in the odd or even clock cycle of the captured 250 MHz ADC waveforms.

> **[info] 100 MHz**
>
> In the 100 MHz Pixie-16 modules, event timestamp, CFD trigger, and ADC waveform capture are all carried out with the same 100 MHz clock. So there is no need to report "CFD trigger source" for the 100 MHz Pixie-16 modules.



#### 500 MHz modules

The CFD algorithm discussed in the previous section for the 100 MHz and 250 MHz Pixie-16 modules can also be written in the following format:  
$$CFD(k)=w\cdot\left(\sum_{i=k}^{k+L}a(i)-\sum_{i=k-B}^{k-B+L}a(i)\right)-\left(\sum_{i=k-D}^{k-D+L}a(i)-\sum_{i=k-D-B}^{k-D-B+L}a(i)\right)$$

Where a(i) is the ADC trace data, k is the index, and w, B, D, and L are CFD parameters.

The CFD algorithm implemented in the 500 MHz Pixie-16 modules is special when compared to the one implemented in the 100 MHz and 250 MHz Pixie-16 modules in terms of the ability to adjust parameters w, B, D, and L. 

The reason for this is that in the 500 MHz Pixie-16 modules, ADC data that come into the FPGA at the speed of 500 MHz is first slowed down with a ratio of 1:5, in other words, the FPGA captures 5 ADC samples at the rate of 100 MHz, i.e., every 10 ns. The FPGA then tries to find the CFD trigger point between any two adjacent 2-ns ADC samples within that 10 ns by first building sums of ADC samples and then calculating differences between delayed and non-delayed sums until the zero crossing point is found. However, in the 500 MHz Pixie-16 modules, the FPGA does not have enough resources to build sums for 5 ADC samples in parallel with variable delays. Therefore, the CFD algorithm for the 500 MHz modules was implemented using a set of fixed CFD parameters as shown in Table *Fixed CFD Parameter Values for 500 MHz Pixie-16 Modules*. Tests show these fixed parameters give best performance for LaBr3(Ce) detectors.

![Fixed CFD Parameter Values for 500 MHz Pixie-16 Modules](/img/fixedcfdparametervaluesfor500mhzpixie16modules.png)

The CFD time given by the 500 MHz Pixie-16 modules consists of two parts: a shift within the 5 ADC samples and a fractional time between two ADC samples where the CFD zero crossing occurred. The shift within the 5 ADC samples is reported as the 3-bit CFD trigger source[2:0] is defined as follows.

![Meanings of the CFD Trigger Source for 500 MHz Pixie-16 Modules](/img/meaningofthecfdtriggersourcefor500mhzpixie16modules.png)

The CFD fractional time is given as follows:

$$CFD=\frac{CFDout1}{CFDout1-CFDout2}\times8192$$


### QDC

![QDC](/img/QDCPars.png)


Eight QDC sums, each of which can have different lengths, are computed in the Signal Processing FPGA of a Pixie-16 module for each channel and the sums are written to the list mode output data stream if the user requests so. 

The recording of QDC sums starts at the waveform point which is *Pre-trigger Trace Length* or *Trace Delay* earlier than the trigger point, which is either the CFD trigger or channel fast trigger depending on whether or not CFD trigger mode is enabled. 

The eight QDC sums are computed one by one continuously, but they are not overlapping. The recording of QDC sums ends when the eight intervals have all passed.

![The 8 QDC sums of a triggered event](/img/the8qdcsumsofatriggeredevent.png)


### Decimation

![Decimation](/img/Decimation.png)

**。。TODO。。**

### Copy Pars

![Copy Pars](/img/CopyPars.png)

**。。TODO。。**

### Save2File

![Save2File](/img/Save2File.png)

**。。TODO。。**

----

## Expert 下拉栏

本下拉栏中调节内容为高阶内容，需要对获取逻辑有一定基础的人学习掌握。

**。。TODO。。**

### Module Variables

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



**。。TODO。。**

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




### CSRA

![CSRA](/img/CSRA.png)

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








### Logic Set

![Logic Set](/img/LogicTrigger.png)

**TODO**


![stretch length](/img/stretchlength.PNG)


----

## Monitor 下拉栏

本下拉栏中调节内容为监视波形噪声水平、基线分布等。

### Hist & XDT

![Hist & XDT](/img/HistXDT.png)

The binning factor controls the number of MCA bins in the spectrum. Energies are computed as 16 bit numbers, allowing in principle 64K MCA bins.

However, spectrum memory for each channel is limited to 32K bins, so computed energy values are divided by $2^{binning factor}$$ before building the histogram. Binning Factor is usually set to 1, but for low count rates and wide peaks, it might be useful to set it to a larger value to obtain a spectrum with fewer bins, but more counts per bin.

$$E_{min}$$ is reserved for a future function to subtract a constant “minimum energy” from the computed energy value before binning to essentially cut off the lower end of the spectrum.



### Trace & Baseline


![Trace & Baseline](/img/Trace.png)


![Trace & Baseline](/img/BaselineHist.png)


![Trace & Baseline](/img/BaselineTimestamps.png)


。。TODO。。

----

## Offiline 下拉栏

本下拉栏中为离线参数优化调节。

### Adjust Par

#### InitData

![Adjust Par](/img/AdjustPar.png)

- Run 选择要读取的文件运行编号，Mod 选择要读取第几个采集卡，按钮 Read 将文件主要信息(道址、能量、波形位置等)载入内存。
- Additional analysis: 三个选项中，选择表示读取该文件数据到内存中时包括该信息。只有读取了该数据，才能启用一些分析方法。但是前提是数据采集时候需要记录该信息。


- InitData: This page. Read the binary file.
- Adjust Par:  Waveform/fast filter/energy filter/cfd/threshould of the select channel.
- Wave-16:  Waveform/fast filter/energy filter/cfd filter/threshould of the 16 channels.
- Energy-16:  The original energy spectrum of the 16 channel..
- Orig Energy:  The original energy spectrum of the select channel.
- Calc Energy:  Recalculation of energy spectrum by waveforms.
- FF/CFD Thre:   Accumulation of a large number of fast filter/cfd filter waveforms to determine the threshold.
- Energy-FF:  The two dimensional relationship between energy and fast filter first/second peak height.
- QDC:  It will be finished soon.
- FFT:  Fourier transformation of a single waveform. User can choose XIA/CAEN/FFTW3 functions.
- Time Diff:  Time difference of two signal.

-----

#### Adjust Par

![offlineapi](/img/OfflineAPI.png)

要通过采集的波形离线计算fast filter、slow filter cfd 曲线，对采集的波形有以下要求。如上图中，计算的能量是算法的能量与算法的基线的差，要得到正确的梯形，那么前提是前面有足够长的点来计算基线。

图中 EFRT 表示 *Energy Filter Rise Time*，EFFT 表示 *Energy Filter Flat Top*。

To compute energy filter response offline, the ideal settings are:
- Total trance length > 2 * ( 2 * EFRT + EFFT)
- Pre-trigger trace (Trace-delay) length > ( 3 * EFRT + EFFT )

当然，这只是计算梯形的一个方法，如果我们记录了每个事件的能量梯形的基线，并且采用pre-trigger部分点的平均值作为波形左侧的无限延伸，那么就不受 Pre-trigger trace length > ( 3 x EFRT + EFFT ) 条件的限制了。下面的页面中，当采用 Old Baseline 方法来计算能量梯形时，有个前提是 pre-trigger trace length 至少需要有 200 个点，因为波形左侧延伸采用前 200 个点来平均。

![Adjustpars3](/img/Adjustpars3.png)

当采集的波形 pre-trigger trace length > 3 x EFRT + EFFT 时，pre-trigger trace 提供足够多的点来计算基线，SF BL 算法可选择 Calculate，否则需要选择 Old Baseline 算法。选择Old Baseline 算法的前提时记录数据的时候， 选择开启记录 梯形的baseline，并且 InitData 页面的 raw E sums/baseline 选项开启。当选择 Old Baseline 算法时，之后的四个选项参数生效，该四个参数为该数据采集时候所用的能量梯形的参数。

上图中绿色曲线为典型的不满足 pre-trigger trace length > 3 x EFRT + EFFT 时，采用的 Calculate 算法造成的结果。图中显示 pre-trigger trace length 为 10 us，EFRT 为 5.04 us，EFFT 为 1.60 us。

此时，应该采用下图所示的  Old Baseline 算法。

![Adjustpars1](/img/Adjustpars1.png)

用户可选择查看波形的通道，按钮 Load 可读取并显示当前的参数设置情况，当修改以上的参数时候，需要按 Apply 按钮使之生效。按钮 Draw 用来显示下一个该通道的事件波形。

用户可选择同时显示 Wave / Slow Filter/ Fast filter / Thres / CFD / CFD Thres 中的多个波形。或者选择曲线的绘画样式。


![Adjustpars2](/img/Adjustpars2.png)

上图展示了显示 fast filter、Thres、CFD、CFD Thres 四个波形，图样采用点显示方式。最低端的水平条两端可以拖动，用户可拉动来控制波形横坐标的显示范围，如图中显示800 - 1300 的点。该情况下，点击 Draw 按钮，将会保持该指定的坐标范围。

-----

#### Wave-16

![Waveform16](/img/Waveform16.png)

该页面用于同时查看16通道的原始波形、filter 波形，阈值等。用户可选择同时显示 Wave / Slow Filter/ Fast filter / Thres / CFD / CFD Thres 中的多个波形。

用户可通过该页面，快速查看该采集卡所有通道的波形是否正常，参数设置是否合理。点击按钮 Draw 一次，则显示所有通道下一个波形。

*需要注意的一点，本页面的 Slow Filter 波形需要在采集的波形 pre-trigger trace 长度大于 3 x EFRT + EFFT 时才是正确的。*

-----

#### Energy-16

![Energy16](/img/Energy16.png)

该界面用于同时查看 16 通道的一维能谱。点击右上角的按钮 Draw 即可。

-----

#### Orig Energy

![OrigEnergy](/img/OrigEnergy.png)

该页面用来快速查看某通道的能谱。用户选择能谱的分 Bin 数，该数值表示将 0 - 65536 道分成多少分。选择查看通道。然后按 Draw 按钮即可。

![OrigEnergyFit](/img/OrigEnergyFit.png)

左上角的 Open Fit 按钮用来快速高斯拟合看能量分辨。点击按钮，开启拟合模式，再次点击按钮则关闭该功能。将鼠标移动到直方图的蓝线上，鼠标十字将会变成三角箭头。三角箭头的鼠标点击直方图中的两个位置，两点所在区间即为拟合区间，则可查看能量分辨。

-----

#### Calc Energy

![CalcEnergy](/img/CalcEnergy.png)

该页面利用采集的波形重新计算能量。同 Adjust Par 页面一样，SF BL 算法可选择Calculate算法或者 Old Baseline 算法。

计算能量采用的fast filter、energy filter参数采用采集卡的设置参数，用户需要选择能量 0-65536 分成多少个 bin，可选择 1024/2048/4096/8192/16384/32768/65536，选择计算的通道，然后按按钮 Draw即开始计算，左上角将会显示计算的进度，也可以按按钮 Stop 提前终止计算。当计算终止时，画板上将显示能谱。

-----

#### FF/CFD Thre

![FFCFDThre](/img/FFCFDThre.png)

该界面用于 fast filter波形、cfd filter波形的累加。用户选择查看通道，然后按 Draw 按钮则开始进入计算，左上角可时时监视进度，也可按 Stop 按钮提前终止计算。计算结束得到如上图所示。

上方按钮 FFProjectY、CFDProjectY、OriginalCFD、CalculateCFD 分别可弹出子画板。

![FFCFDThreFFProjectY](/img/FFCFDThreFFProjectY.png)

点击按钮 FFProjectY，则开启查看fast filter投影图，再次点击则关闭该功能。开启功能时，将鼠标放在二维图上，左右移动鼠标， Fast Filter ShowProjectY 子画板则显示鼠标指向的该位置的投影分布。触发前的该分布，也表征噪声的水平。

![FFCFDThreCFDProjectY](/img/FFCFDThreCFDProjectY.png)

同理，按钮 CFDProjectY 功能如上图所示。

![FFCFDThreCFD](/img/FFCFDThreCFD.png)

点击按钮 OriginalCFD，则展示左图中原始数据中 CFD 数值的分布。点击按钮 CalculateCFD，则展示右图中通过离线波形计算的结果，计算所用参数为当前的参数。对于一个合适的 CFD 参数设置，该CFD分布该是平均分布的。 

-----

#### Energy-FF

![EnergyFFGraph](/img/EnergyFFGraph.png)

该界面是能量与 fast filter 峰高的二维关联图。用于确定合适的阈值。左图是能量与 fast filter的二维关联，它们应该有个较好的线性关系，右图为能量与 fast filer 中抛除梯形部分剩余中最大值的二维关联，抛除梯形部分剩余分布的最大值表征噪声水平，能量跟该值应该是没有关联的。

首先 Draw Style 选择 Graph，即二维散点图模式。选择查看通道，然后按 Draw 按钮则开始进入计算，左上角可时时监视进度，也可按 Stop 按钮提前终止计算。计算结束得到如上图所示。

![EnergyFFHist](/img/EnergyFFHist.png)

二维散点图并不能很直观显示展示数据点的密度分布，因此 Draw Style 选择 Hist 模式，选择 X、Y轴的分 bin 数即范围，然后同样按 Draw 按钮开始计算。结果如上图所示，右图反映了噪声的水平。

-----

#### QDC

**QDC  TODO   功能未完成**

-----

#### FFT

![FFT](/img/FFT.png)

该界面用于快速查看波形的傅立叶变换。用户可以选择不同的算法，例如 XIA、fftw3、CAEN(HANNING)、CAEN(HAMMING)、CAEN(BLACKMAN)、CAEN(RECT)。选择查看通道。然后按 Draw 按钮即可，每点击一次该按钮，则显示下一个结果。

the ADC trace display also includes the option to view a FFT of the acquired trace. This is useful to diagnose noise contributions.

-----

#### Time Diff

![Time Diff](/img/TimeDiff.png)

该界面用于快速查看两路信号的时间分辨。用户可以选择查看 CFD 算法过零点的时间差或者 fast filter 过阈值的时间差。Xbin 表示横坐标分 bin 数，Xmin 表示横坐标的最小值，Xmax 表示横坐标的最大值。通过 Ch A、Ch B 来选择想要查看的两个通道。然后按 Draw 按钮即可。

选项 Limits 选择则开启能量范围约束。选择该选项后，之后的四个参数 AL、AR、BL、BR才生效，其分别表示 Ch A/B 能量道址的左右范围，只有能量落在这个区间的事件才填充到直方图中。用户可通过 Orig Energy 页面来选择合适的能量道址区间。


----

### Simulation(暂未实现)

通过模型产生不同类型探测、不同信噪比的波形，辅助使用者学习参数优化调节的。




<!-- GUI.md ends here -->
