<!-- GUI.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 5月 13 20:23:55 2018 (+0800)
;; Last-Updated: 三 5月 16 16:29:12 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 10
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

系统初始化成功后，再次确认 *Setup* 栏中从的获取文件存放路径、文件名、文件编号是否有问题，如果有问题则直接修改，确认之后按 **Complete**。

之后 *Control* 栏中的主按钮 **LSRunStart** 则开启，此时点击该按钮，获取则开启，按钮状态更改为 **LSRunStop**，再次点击该按钮，获取结束，运行的 *Run Num* 号码自动加一。再次点击 **LSRunStart** 开启下轮获取。

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
- 选项 *Sign* 选择输入信号的极性，输入正信号选择 +，输入负信号则选择 -。
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


### Trigger Filter

![Trigger Filter](/img/TriggerFilter.png)

界面下方的 Status 显示为**绿色的 Ready** 时表示可操作该界面，否则需要等待。底下按钮的操作同上。

- 参数 *TPeaking* 。。。
- 参数 *TGap* 。。。
- 参数 *Thresh.* 表示阈值，该数值的设置是相对 fast filter 波形。


**TODO   这里需要补充一个示意图，还有计算公式。**


### Energy

![Energy](/img/Energy.png)

界面下方的 Status 显示为**绿色的 Ready** 时表示可操作该界面，否则需要等待。底下按钮的操作同上。

。。。。


### CFD

![CFD](/img/CFDPars.png)

### QDC

![QDC](/img/QDCPars.png)

### Decimation

![Decimation](/img/Decimation.png)

### Copy Pars

![Copy Pars](/img/CopyPars.png)

### Save2File

![Save2File](/img/Save2File.png)

----

## Expert 下拉栏

本下拉栏中调节内容为高阶内容，需要对获取逻辑有一定基础的人学习掌握。

### Module Variables

![Module Variables](/img/ModuleVariables.png)

### CSRA

![CSRA](/img/CSRA.png)

### Logic Set

![Logic Set](/img/LogicTrigger.png)

----

## Monitor 下拉栏

本下拉栏中调节内容为监视波形噪声水平、基线分布等。

### Hist & XDT

![Hist & XDT](/img/HistXDT.png)

### Trace & Baseline


![Trace & Baseline](/img/Trace.png)


![Trace & Baseline](/img/BaselineHist.png)


![Trace & Baseline](/img/BaselineTimestamps.png)

----

## Offiline 下拉栏

本下拉栏中为离线参数优化调节。

### Adjust Par

![Adjust Par](/img/AdjustPar.png)

### Simulation(暂未实现)

通过模型产生不同类型探测器的波形，辅助使用者学习参数优化调节的。




<!-- GUI.md ends here -->
