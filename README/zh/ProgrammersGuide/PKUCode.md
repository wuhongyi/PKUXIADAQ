<!-- PKUCode.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 三 5月 23 08:18:41 2018 (+0800)
;; Last-Updated: 三 5月 23 09:06:34 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 3
;; URL: http://wuhongyi.cn -->

# PKU Code

本节介绍程序的主要思路。


**DOTO  需要补充框图帮助理解程序！！！**


## Decode

- decoder.cc
- decoder.hh
	- 读取二进制文件
- main.cc
	- 主程序
- Makefile
- r2root.cc
- r2root.hh
	- 保存ROOT文件
- UserDefine.hh
	- 用户定义参数

## GUI

- Base.cc 
- Base.hh
	- 子界面，基线、极性、增益、波形长度、数据记录等参数调节
- Cfd.cc 
- Cfd.hh
	- 子界面，CFD参数调节
- CopyPars.cc
- CopyPars.hh
	- 子界面，参数复制
- Csra.cc
- Csra.hh
	- 子界面，方便快速调节每通道的控制寄存器
- Decimation.cc
- Decimation.hh
	- 子界面，降频参数设置
- Detector.cc 
- Detector.hh
	- 数据采集循环主体
- Energy.cc
- Energy.hh
	- 子界面，梯形参数调节界面
- ExpertMod.cc
- ExpertMod.hh
	- 子界面，采集卡模块参数设置
- Global.cc
- Global.hh
	- 全局函数
- HistXDT.cc
- HistXDT.hh
	- 子界面，设置记录的一维能谱的最小值、bin宽及DSP抓波形时的部长
- LogicTrigger.cc
- LogicTrigger.hh
	- 子界面，逻辑参数调节
- main.cc
- MainFrame.cc
- MainFrame.hh
	- 主控制界面
- MainLinkdef.h
- Makefile
- Offline.cc
- Offline.hh
	- 离线分析主界面，离线分析功能代码
- OfflineData.cc
- OfflineData.hh
	- 离线分析读取文件数据
- pkuFFTW.cc
- pkuFFTW.hh
	- 基于FFTW3封装类
- Qdc.cc
- Qdc.hh
	- 子界面，用于 QDC 积分门窗的调节
- ReadChanStatus.cc
- ReadChanStatus.hh
	- 子界面，查看DSP中抓取的波形及baseline
- Simulation.cc
- Simulation.hh
	- 未实现
- Table.cc
- Table.hh
	- 基类，用于参数调节界面
- TriggerFilter.cc
- TriggerFilter.hh
	- 子界面，fast filter 参数调节界面
- wuReadData.hh
	- 模版函数，用来读取输入卡


## MakeEvent


- main.cc
	- 主程序
- Makefile
- sort.cc
- sort.hh
	- 事件组装
- UserDefine.hh
	- 用户定义参数



## OnlineStattics


- Linkdef.hh
- main.cc
	- 主程序
- Makefile
- Online.cc
- Online.hh
	- 在线监视界面
- PixieOnline.config










<!-- PKUCode.md ends here -->
