<!-- README.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 六 6月 18 13:37:42 2016 (+0800)
;; Last-Updated: 五 10月 12 10:47:22 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 100
;; URL: http://wuhongyi.cn -->

# README

<!-- toc -->

![PKU logo](pkulogo100.jpg)

## Version/版本

**We recommend users to download a stable version(我们建议用户下载稳定版本)**

### Stable Version/稳定版本

**Stable Version（稳定版本） Version:2018.05.27**  

Download the latest version, please click(下载最新版本，请点击):  
https://github.com/pkuNucExp/PKUXIADAQ

Web page manual, please click(网页版说明书请访问):  
https://pkunucexp.github.io/PKUXIADAQ/

markdown version manual(md版说明书)  README/  

Offline web version manual(离线网页版说明书)  docs/  

pdf version manual(pdf版本说明书)  README.pdf  


### Pre-alpha/准预览版本

Download the latest version(程序下载请访问):  
https://github.com/wuhongyi/PKUXIADAQ

Web page manual(网页版说明书请访问):  
http://wuhongyi.cn/PKUXIADAQ/



- Have any comments or suggestions for this project(function addition or improvement), please send an email to Hongyi Wu(wuhongyi@qq.com). 对本获取程序有任何的意见及建议(功能添加及改进)，欢迎给吴鸿毅(wuhongyi@qq.com)发邮件。
- We will improve the Chinese/English manuals of the software as soon as possible. Currently, we mainly use the software to explain the use of the software. 我们将会尽快完善软件的中英文使用说明书，当前主要以操作演示来讲解软件的使用。


----

## About/关于

**This manual applies only to XIA LLC Pixie-16**

- This program is developed by the **group of experimental nuclear physics of Peking University**.  本程序由**北京大学实验核物理组**开发。
- The earliest graphical interface development of this program is based on NSCL DDAS Nscope(http://docs.nscl.msu.edu/daq/newsite/ddas-1.1/nscope.html).  最早的图形界面程序是基于 NSCL DDAS Nscope 开发。
- Thanks to Hui Tan's(XIA LLC) support for our development.  特别感谢 *谭辉(XIA LLC)* 对我们开发的支持。


Technical adviser 技术指导:
- Zhihuan Li  李智焕
- Hui Tan  谭辉(XIA LLC)
- Wolfgang Hennig(XIA LLC)

Software Developer 软件主要开发者:
- Hongyi Wu 吴鸿毅(wuhongyi@qq.com)
- Jing Li 李晶(lijinger02@126.com)

Principal author of the instruction 说明书主要撰写者:
- Diwen Luo 罗迪雯
- Hongyi Wu 吴鸿毅
- Xiang Wang 王翔


The development of this program is supported by the following 本程序的开发得到以下单位的支持：
- XIA LLC
- Institute of Modern Physics, Chinese Academy of Sciences 中国科学院近代物理研究所(IMP)
- China Institute of Atomic Energy 中国原子能科学研究院(CIAE)
- The University of Hong Kong  香港大学(HKU)
- Shandong University, Weihai 山东大学(威海)(SDU)
- ...

----

This program is applicable to XIA Pixie16 module, supports 100/250/500 MHz(specifically, the model can be found in File->About in the graphics software), and supports up to 8 chassis synchronous operation, that is, 1600+ signals are simultaneously collected. **This package requires the CERN ROOT6. Requires a 1920x1080 and above resolution display**.

本程序适用于 XIA Pixie16 系列采集卡，支持 100/250/500 MHz 采集卡(具体支持型号可查看图形软件中的File->About)，最大支持 8 个机箱同步运行，即 1600+ 路信号同时采集。**本程序包要求使用 CERN ROOT6 版本。要求采用 1920x1080 及以上分辨率显示屏**。

The program is designed to be compatible with the 100/250/500 MHz module. Simply add the firmware location of the corresponding sample rate capture card to cfgPixie16.txt. The program can automatically identify the capture card type and load the corresponding firmware. Currently we only have a 100/250 MHz 14 bit capture card, so this type of capture card can be run by default. To support other types, please contact XIA LLC to obtain the corresponding firmware or contact Wu Hongyi (wuhongyi@qq.com).

本程序的设计兼容 100/250/500 MHz 采集卡的混合使用，只需在 cfgPixie16.txt 添加相应采样率采集卡的固件位置即可，程序在线能够自动识别采集卡类型并加载相应的固件。当前我们只有100/250 MHz 14 bit 的采集卡，因此默认可运行该类型的采集卡，如要支持其它类型，请联系 XIA LLC 获取对应固件或者联系吴鸿毅(wuhongyi@qq.com)。


----

## contents / 目录

The following files/folders are included in the user's use package 用户使用程序包中包含以下文件/文件夹:

- Decode(Convert raw binary data to ROOT file / 将原始二进制数据转为 ROOT)
- docs(Web page manual / 使用说明书，网页版)
- firmware(固件)
	- firmware/firmware.md(History of firmware / 历史各版本固件说明)
- GUI(图形软件)
- MakeEvent(事件重构程序，可选)
- NOGUI(Non-graphics software. New version upgrade, temporarily unavailable / 非图形软件。新版本升级中，暂时不可用)
- OnlineStattics(Online monitoring program / 在线监视程序)
- parset(parameter setting file / 参数设置文件)
- PlxSdk.tar.gz(Plx9054 driver)
- README(markdown version manual)
- README.md(home page introduction)
- README.pdf(pdf version manual)
- software(unofficial standards pixie16 driver API, have been revised by Hongyi Wu)
- TestTool(Developer testing tool, users don't need / 开发者测试工具，用户不需要！！！)

----

## Update/升级计划

- The main control interface developed based on the ROOT GUI is highly complex and difficult for users to modify now. Users do not easy to develop their own version based on this program. 当前基于 ROOT GUI 开发的主控制界面复杂度高，用户修改难度大。其它用户不容易基于其发展适合自己的版本。
- We are also developing a web control for online/offline analysis 我们也在开发基于网页控制的获取在线/离线分析程序:
	- ZeroMQ
	- FastCGI
	- JSROOT
	- web
	- ...


<!-- README.md ends here -->
