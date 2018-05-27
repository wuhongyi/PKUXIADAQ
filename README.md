<!-- README.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 六 6月 18 13:37:42 2016 (+0800)
;; Last-Updated: 日 5月 27 07:59:35 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 84
;; URL: http://wuhongyi.cn -->

# README

**最新版本号 Version:2018.05.27**  

程序下载请访问(*私有仓库，暂未开放下载*)
https://github.com/wuhongyi/PKUXIADAQ

网页版说明书请访问
http://wuhongyi.cn/PKUXIADAQ/

markdown 版本说明书请访问  README/  

离线网页版说明书请访问  docs/  

pdf版本说明书请访问  README.pdf  


- 对本获取程序有任何的意见及建议(功能添加及改进)，欢迎给吴鸿毅(wuhongyi@qq.com)发邮件。
- 我们将会尽快完善软件的中英文使用说明书，当前基本以操作演示讲解软件的使用为主。

----

**This manual applies only to XIA LLC Pixie-16**

- This program is developed by the **experimental nuclear physics group of Peking University**.  本程序由**北京大学实验核物理组**开发。
- The earliest graphical interface development of this program is based on NSCL DDAS Nscope(http://docs.nscl.msu.edu/daq/newsite/ddas-1.1/nscope.html).  最早的图形界面程序是基于 NSCL DDAS Nscope 开发。
- Thanks to Hui Tan's(XIA LLC) support for our development.  特别感谢 *谭辉(XIA LLC)* 对我们开发的支持。


Technical adviser 技术指导:
- Zhihuan Li  李智焕
- Hui Tan  谭辉(XIA LLC)

Software Developer 软件主要开发者:
- Hongyi Wu 吴鸿毅(wuhongyi@qq.com)
- Jing Li 李晶(lijinger02@126.com)

Principal author of the instruction 说明书主要撰写者:
- Hongyi Wu 吴鸿毅
- Xiang Wang 王翔
- Diwen Luo 罗迪雯



The development of this program is supported by the following 本程序的开发得到以下单位的支持：
- XIA LLC
- Institute of Modern Physics, Chinese Academy of Sciences 中国科学院近代物理研究所(IMP)
- China Institute of Atomic Energy 中国原子能科学研究院(CIAE)
- The University of Hong Kong  香港大学(HKU)
- Shandong University, Weihai 山东大学(威海)(SDU)
- ...

----


本程序适用于 XIA Pixie16 系列采集卡，支持 100/250/500 MHz 采集卡(具体支持型号可查看图形软件中的File->About)，最大支持 8 个机箱同步运行，即 1600+ 路信号同时采集。**本程序包要求使用 CERN ROOT6 版本。要求采用 1920x1080 及以上分辨率显示屏**。

本程序的设计兼容 100/250/500 MHz 采集卡的混合使用，只需在 cfgPixie16.txt 添加各类采样率采集卡的固件位置即可，程序在线能够自动识别采集卡类型并加载相应的固件。当前我们只有100/250 MHz 14 bit 的采集卡，因此默认可运行该类型的采集卡，如要支持其它类型，请联系 XIA LLC 获取对应固件或者联系吴鸿毅(wuhongyi@qq.com)。


----

用户使用程序包中包含以下文件/文件夹:

- Decode(将原始二进制数据转为 ROOT)
- docs(使用说明书，网页版)
- firmware(固件)
	- firmware/firmware.md(历史各版本固件说明)
- GUI(图形软件)
- MakeEvent(事件重构程序，可选)
- NOGUI(非图形软件。新版本升级中，暂时不可用)
- OnlineStattics(在线监视程序)
- parset(参数设置文件)
- PlxSdk.tar.gz(Plx9054 驱动)
- README(说明书 markdown 版)
- README.md(github首页介绍)
- README.pdf(pdf版本说明书)
- software(pixie16驱动API，非官方标准，已经被吴鸿毅修改)
- TestTool(开发者测试工具，用户不需要！！！)

----

## 升级计划：

- 当前基于 ROOT GUI 开发的主控制界面复杂度高，用户修改难度大。其它用户不容易基于其发展适合自己的版本。
- 我们也在开发基于网页控制的获取在线/离线分析程序:
	- FastCGI
	- JSROOT
	- web
	- ...


<!-- README.md ends here -->
