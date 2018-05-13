<!-- GUI.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 5月 13 20:23:55 2018 (+0800)
;; Last-Updated: 日 5月 13 21:18:45 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 5
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

### Trigger Filter

![Trigger Filter](/img/TriggerFilter.png)

### Energy

![Energy](/img/Energy.png)

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

待补充
<!-- ![Trace & Baseline](/img/) -->

----

## Offiline 下拉栏

本下拉栏中为离线参数优化调节。

### Adjust Par

![Adjust Par](/img/AdjustPar.png)

### Simulation(暂未实现)

通过模型产生不同类型探测器的波形，辅助使用者学习参数优化调节的。




<!-- GUI.md ends here -->
