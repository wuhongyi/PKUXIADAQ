<!-- GUI.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 5月 13 20:23:55 2018 (+0800)
;; Last-Updated: 四 5月 24 11:58:04 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 19
;; URL: http://wuhongyi.cn -->

# GUI




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

Pulses from the detector should fall in the range from 0 to 12/14/16-bit -1, with the baseline at ~400 to allow for drifts and/or undershoots and no clipping at the upper limit. If there is clipping, adjust the Gain and Offset or click on the Adjust Offsets button to let the software set the DC offsets to proper levels.**TODO 这段还需要进一步修改**

Since the trigger/filter circuits in the FPGA only act on rising pulses, negative pulses are inverted at the input of the FPGA, and the waveforms shown in the ADC trace display include this optional inversion. Thus set the channel’s Polarity such that pulses from the detector appear with positive amplitude (rising edge).


In the **Base Setup**  tab, you can set the total trace length and the pre-trigger trace delay for the waveforms to be acquired in list mode runs.


The Pixie-16 constantly takes baseline measurements when no pulse is detected and keeps a
baseline average to be subtracted from the energy filter output during pulse height
reconstruction. Baseline measurements that differ from the average by more than Baseline Cut
will be rejected as they are likely contaminated with small pulses below the trigger threshold.
You can click on the Acquire Baseline button to view a series of baseline measurements for each
channel, and in the single channel view you can build a histogram of baselines to verify that the
Baseline Cut does not reject measurements falling into the main (ideally Gaussian) peak in the
baseline distribution. Usually, it is sufficient to keep Baseline Cut at its default value.


Note: Since the baseline computation takes into account the exponential decay, no pulses
should be noticeable in the baseline display if a) the decay time is set correctly and b) the
detector pulses are truly exponential.


Baseline Percent is a parameter used for automatic offset adjustment; by clicking on the
Adjust Offsets button, offsets will be set such that the baseline seen in the ADC trace display falls
at the Baseline Percent fraction of the full ADC range (e.g. for Baseline Percent = 10% the
baseline falls at ADC step 1638 out of 16384 total).





### Trigger Filter

![Trigger Filter](/img/TriggerFilter.png)

界面下方的 Status 显示为**绿色的 Ready** 时表示可操作该界面，否则需要等待。底下按钮的操作同上。

- 参数 *TPeaking* 。。。
- 参数 *TGap* 。。。
- 参数 *Thresh.* 表示阈值，该数值的设置是相对 fast filter 波形。


**TODO   这里需要补充一个示意图，还有计算公式。**

Assume the digitized waveform stream can be represented by data series Trace[i], i=0,1,2,... First the fast filter response of the digitized waveform is computed as follows:  
**TODO公式**  
Where FL is called the fast length and FG is called the fast gap of the digital trapezoidal filter.




- A longer trigger filter rise time averages more samples and thus allows setting lower thresholds without triggering on noise.
- Typically the threshold should be set as low as possible, just above the noise level.
- A longer trigger filter flat top time makes it easier to detect slow rising pulses.


### Energy

![Energy](/img/Energy.png)

界面下方的 Status 显示为**绿色的 Ready** 时表示可操作该界面，否则需要等待。底下按钮的操作同上。

。。TODO。。


The most critical parameter for the energy computation is the signal decay time Tau. It is used
to compensate for the falling edge of a previous pulse in the computation of the energy. You can
either enter Tau directly for each channel, or enter an approximate value in the right control,
select a channel, and click Find it to let the software determine the decay time automatically.
Click Accept it to apply the found value to the channel. (If the approximate value is unchanged,
the software could not find a better value.)  **TODO 这段还需要进一步修改**


- The energy filter flat top time should be about the same as the pulse rise time.
- The energy filter rise time can be varied to balance resolution and throughput. Typically, energy resolution increases with the length of the filter rise time, up to an optimum when longer filters only add more noise into the measurement. The filter dead time is about TD = 2 × (T rise + T flat ), and the maximum throughput for Poisson statistics is 1/(TD*e). For HPGe detectors, a rise time of 4-6μs is usually appropriate.
- Choose the smallest energy filter range that allows setting the optimum energy filter rise time. Larger filter ranges allow longer filter sums, but increase the granularity of possible values for the energy filter rise time and flat top time and increase the jitter of latching the energy filter output relative to the rising edge of the pulse. This is usually only important for very fast pulses.




### CFD

![CFD](/img/CFDPars.png)

。。TODO。。

Then the CFD is computed as follows:  
**TODO公式**  
Where D is called the CFD delay length and W is called the CFD scaling factor.

The CFD zero crossing point (ZCP) is then determined when CFD[i] > 0 and CFD[i+1] < 0.
The timestamp is latched at Trace point i, and the fraction time f is given by the ratio of the two
CFD response amplitudes right before and after the ZCP.

**TODO公式**  
Where CFDout1 is the CFD response amplitude right before the ZCP and CFDout2 is the CFD
response amplitude (absolute value since CFDout2 is negative) right after the ZCP. The Pixie-16
DSP computes the CFD final value as shown below and stored it in the output data stream for
online or offline analysis.

Valid CFD Scale values and corresponding CFD scaling factors are
**TODO  CFD参数对应表格**  


### QDC

![QDC](/img/QDCPars.png)

。。TODO。。

Eight QDC sums, each of which can have different length varying from 10 ns to 327.68 μs,
are computed in the signal processing FPGA of a Pixie-16 module for each channel and the sums
are written to the output data stream if the user requests so. The recording of QDC sums starts at
the waveform point which is Pre-trigger Trace Length earlier than the trigger point, which is
either the CFD trigger or local fast trigger depending on whether or not CFD trigger mode is
enabled. The eight QDC sums are computed one by one continuously, but they are not
overlapping. Both the QDC sum length and the Pre-trigger trace length are set by the user. The
recording of QDC sums ends when the eight intervals have passed.


### Decimation

![Decimation](/img/Decimation.png)

。。TODO。。

### Copy Pars

![Copy Pars](/img/CopyPars.png)

。。TODO。。

### Save2File

![Save2File](/img/Save2File.png)

。。TODO。。

----

## Expert 下拉栏

本下拉栏中调节内容为高阶内容，需要对获取逻辑有一定基础的人学习掌握。

。。TODO。。

### Module Variables

![Module Variables](/img/ModuleVariables.png)

。。TODO。。

Module Control Register B affecting the module as a whole.

- a) Enable pullups for backplane bus lines. This should be enabled for only one module in the crate.
- b) Connect trigger signals to backplane. You can set the module to share triggers over the backplane with other modules, unless you run each module (or channel) independently.
- c) Accept external trigger and run inhibit signals. Enable this option to let this module accept external trigger and run inhibit signals and then put the signals on the backplane so that all modules can see the same signals. This should be enabled for only one module in the crate.
- d) Crate master module (multiple crates only). This option is only used when multiple Pixie-16 crates communicate with each other. By enabling this option,the mater module in each crate is responsible for sending synchronization or trigger signals to certain backplane lines. This should be enabled for only one module in the crate.
- e) Enable run inhibit signal input. This option is only applicable to the module which has the “Accept external trigger and run inhibit signals” option enabled. This should be enabled for only one module in the crate.
- f) Multiple crates. This option is only used when multiple Pixie-16 crates communicate with each other.




### CSRA

![CSRA](/img/CSRA.png)

。。TODO。。

Channel Control Register A affecting each channel individually 

- Good channel. Only channels marked as good will have their events recorded.
This setting has no bearing on the channel's capability to issue a trigger. There
can be a triggering channel whose data are discarded. Channels not marked as
good will be excluded from the automatic offset adjustment.
- Histogram energies. When this box is checked, pulse height (energy) computed
for each event will be incremented to an energy histogram in the MCA
memory.
- Capture trace. When this box is checked, trace will be captured and recorded
for each event, along with other list mode information, e.g. timestamp, energy,
etc.
- Capture QDC sums. When this box is checked, eight QDC sums will be
recorded for each event. QDC sums are consecutive sums of the list mode
trace.
- Enable CFD trigger. Check this box to enable this channel’s CFD trigger.
Otherwise, regular trapezoidal fast trigger will be used.
- Require global external trigger for validation. Check this box to require global
external trigger to validate events for this channel.
- Capture raw energy sums and baseline. Check this box to record raw energy
sums and baseline value for each event.
- Require channel external trigger for validation. Check this box to require
channel external trigger to validate events for this channel.
- Enable pileup rejection. Check this box to enable pileup rejection for this
channel. Otherwise, pulses will still be recorded even if they are piled up.
**TODO 这段还需要进一步修改**


### Logic Set

![Logic Set](/img/LogicTrigger.png)

。。TODO。。

----

## Monitor 下拉栏

本下拉栏中调节内容为监视波形噪声水平、基线分布等。

### Hist & XDT

![Hist & XDT](/img/HistXDT.png)

。。TODO。。


The binning factor controls the number of MCA bins in the spectrum. Energies are computed as 16 bit numbers, allowing in principle 64K MCA bins.

However, spectrum memory for each channel is limited to 32K bins, so computed energy values are divided by 2 binning factor before building the histogram. Binning Factor is usually set to 1, but for low count rates and wide peaks, it might be useful to set it to a larger value to obtain a spectrum with fewer bins, but more counts per bin.

Emin is reserved for a future function to subtract a constant “minimum energy” from the computed energy value before binning to essentially cut off the lower end of the spectrum.



### Trace & Baseline


![Trace & Baseline](/img/Trace.png)


![Trace & Baseline](/img/BaselineHist.png)


![Trace & Baseline](/img/BaselineTimestamps.png)


。。TODO。。

----

## Offiline 下拉栏

本下拉栏中为离线参数优化调节。

### Adjust Par

![Adjust Par](/img/AdjustPar.png)

- InitData: This page. Read the binary file.
	- Run 选择要读取的文件运行编号，Mod 选择要读取第几个采集卡，按钮 Read 将文件主要信息(道址、能量、波形位置等)载入内存。
	- Additional analysis: 三个选项中，选择表示读取该文件数据到内存中时包括该信息。只有读取了该数据，才能启用一些分析方法。但是前提是数据采集时候需要记录该信息。
- Adjust Par:  Waveform/fast filter/energy filter/cfd/threshould of the select channel.
- Wave-16:  Waveform/fast filter/energy filter/cfd filter/threshould of the 16 channels.
- Energy-16:  The original energy spectrum of the 16 channel..
- Orig Energy:  The original energy spectrum of the select channel.
- Calc Energy:  Recalculation of energy spectrum by waveforms.
- FF/CFD Thre:   Accumulation of a large number of fast filter/cfd filter waveforms to determine the threshold.
- Energy-FF:  The two dimensional relationship between energy and fast filter first/second peak height.
- QDC:  It will be finished soon.
- FFT:  Fourier transformation of a single waveform. User can choose XIA/CAEN/FFTW3 functions.

**需要添加一个页面功能，选择两路快速看时间分辨。类似于TechnoAP界面中的功能。**

。。TODO。。这里需要较大篇幅来介绍。每个子页面的用法需要详细介绍。

the ADC trace display also includes the option to view a FFT of the acquired trace. This is useful to diagnose noise contributions.


### Simulation(暂未实现)

通过模型产生不同类型探测、不同信噪比的波形，辅助使用者学习参数优化调节的。




<!-- GUI.md ends here -->
