<!-- GUI.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 5月 13 20:23:55 2018 (+0800)
;; Last-Updated: 一 5月 28 05:40:57 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 30
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

- 参数 *Rise Time* 。。。
- 参数 *Flat Top* 。。。
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

- 参数 *Rise Time* 。。。
- 参数 *Flat Top* 。。。
- 参数 *Tau* 。。。


**TODO**


The most critical parameter for the energy computation is the signal decay time Tau. It is used
to compensate for the falling edge of a previous pulse in the computation of the energy. You can
either enter Tau directly for each channel, or enter an approximate value in the right control,
select a channel, and click Find it to let the software determine the decay time automatically.
Click Accept it to apply the found value to the channel. (If the approximate value is unchanged,
the software could not find a better value.)  **TODO 这段还需要进一步修改**


- The energy filter flat top time should be about the same as the pulse rise time.
- The energy filter rise time can be varied to balance resolution and throughput. Typically, energy resolution increases with the length of the filter rise time, up to an optimum when longer filters only add more noise into the measurement. The filter dead time is about TD = 2 × (T rise + T flat ), and the maximum throughput for Poisson statistics is 1/(TD*e). For HPGe detectors, a rise time of 4-6μs is usually appropriate.
- Choose the smallest energy filter range that allows setting the optimum energy filter rise time. Larger filter ranges allow longer filter sums, but increase the granularity of possible values for the energy filter rise time and flat top time and increase the jitter of latching the energy filter output relative to the rising edge of the pulse. This is usually only important for very fast pulses.


![filter range](/img/filterrange_format.PNG)


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
![cfd scale](/img/cfdscale_format.PNG)


### QDC

![QDC](/img/QDCPars.png)

**。。TODO。。**

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

**。。TODO。。**

Module Control Register B affecting the module as a whole.

- a) Enable pullups for backplane bus lines. This should be enabled for only one module in the crate.
- b) Connect trigger signals to backplane. You can set the module to share triggers over the backplane with other modules, unless you run each module (or channel) independently.
- c) Accept external trigger and run inhibit signals. Enable this option to let this module accept external trigger and run inhibit signals and then put the signals on the backplane so that all modules can see the same signals. This should be enabled for only one module in the crate.
- d) Crate master module (multiple crates only). This option is only used when multiple Pixie-16 crates communicate with each other. By enabling this option,the mater module in each crate is responsible for sending synchronization or trigger signals to certain backplane lines. This should be enabled for only one module in the crate.
- e) Enable run inhibit signal input. This option is only applicable to the module which has the “Accept external trigger and run inhibit signals” option enabled. This should be enabled for only one module in the crate.
- f) Multiple crates. This option is only used when multiple Pixie-16 crates communicate with each other.




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

**。。TODO。。**

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

**TODO**


![stretch length](/img/stretchlength.PNG)


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
