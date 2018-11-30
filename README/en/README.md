<!-- README.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 六 6月 18 13:37:42 2016 (+0800)
;; Last-Updated: 五 11月 30 21:16:47 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 113
;; URL: http://wuhongyi.cn -->

# README

<!-- toc -->

![PKU logo](pkulogo100.jpg)

English | [简体中文](https://github.com/wuhongyi/PKUXIADAQ/blob/master/README_zh.md)

<!-- English | [简体中文](https://github.com/pkuNucExp/PKUXIADAQ/blob/master/README_zh.md) -->

## Version

**We recommend users to download a stable version**

### Stable Version

**Stable Version Version:2018.05.27**  

Download the latest version, please click:  [PKUXIADAQ stable](https://github.com/pkuNucExp/PKUXIADAQ)

Web page manual, please click:  [English](https://pkunucexp.github.io/PKUXIADAQ/en/)    [简体中文](https://pkunucexp.github.io/PKUXIADAQ/zh/)

- markdown version manual: README/  
- Offline web version manual: docs/  
- pdf version manual: README_en.pdf  README_ch.pdf  


### Pre-alpha

**Pre-alpha Version:2018.11.30**  

Download the latest version:  [PKUXIADAQ](https://github.com/wuhongyi/PKUXIADAQ)

Web page manual:  [English](http://wuhongyi.cn/PKUXIADAQ/en/)   [简体中文](http://wuhongyi.cn/PKUXIADAQ/zh/)



- Have any comments or suggestions for this project(function addition or improvement), please send an email to Hongyi Wu(wuhongyi@qq.com). 
- We will improve the Chinese/English manuals of the software as soon as possible. Currently, we mainly use the software to explain the use of the software. 


----

## About

**This manual applies only to XIA LLC Pixie-16**

- This program is developed by the **group of experimental nuclear physics of Peking University**.  
- The earliest graphical interface development of this program is based on NSCL DDAS Nscope(http://docs.nscl.msu.edu/daq/newsite/ddas-1.1/nscope.html). 
- Thanks to Hui Tan's(XIA LLC) support for our development.


Technical adviser:
- Zhihuan Li  李智焕
- Hui Tan  谭辉(XIA LLC)
- Wolfgang Hennig(XIA LLC)

Software Developer:
- 2015 - 2016
	- Jing Li 李晶(lijinger02@126.com) 
- 2016 - now
	- Hongyi Wu 吴鸿毅(wuhongyi@qq.com) 

Principal author of the instruction:
- Diwen Luo 罗迪雯
- Hongyi Wu 吴鸿毅
- Xiang Wang 王翔


The development of this program is supported by the following：
- XIA LLC
- Institute of Modern Physics, Chinese Academy of Sciences(IMP)
- China Institute of Atomic Energy(CIAE)
- The University of Hong Kong(HKU)
- Shandong University, Weihai(SDU)
- ...

----

This program is applicable to XIA Pixie16 module, supports 100/250/500 MHz(specifically, the model can be found in File->About in the graphics software), and supports up to 8 chassis synchronous operation, that is, 1600+ signals are simultaneously collected. **This package requires the CERN ROOT6. Requires a 1920x1080 and above resolution display**.



The program is designed to be compatible with the 100/250/500 MHz modules. Simply add the firmware location of the corresponding sample rate modules to cfgPixie16.txt. The program can automatically identify the modules type and load the corresponding firmware. Currently we only have a 100/250 MHz 14 bit modules, so this type of modules can be run by default. To support other types, please contact XIA LLC to obtain the corresponding firmware or contact Hongyi Wu(wuhongyi@qq.com).

----

## contents

The following files/folders are included in the user's use package:

- Decode(Convert raw binary data to ROOT file)
- docs(Web page manual)
- firmware
	- firmware/firmware.md(History of firmware)
- GUI
- MakeEvent(event builder，optional)
- NOGUI(Non-graphics software. New version upgrade, temporarily unavailable)
- OnlineStattics(Online monitoring program)
- parset(parameter setting file)
- PlxSdk.tar.gz(Plx9054 driver)
- README(markdown version manual)
- README.md(home page introduction)
- README.pdf(pdf version manual)
- software(unofficial standards pixie16 driver API, have been revised by Hongyi Wu)
- TestTool(Developer testing tool, users don't need)

----

## Update

- The main control interface developed based on the ROOT GUI is highly complex and difficult for users to modify now. Users do not easy to develop their own version based on this program. 
- We are also developing a web control for online/offline analysis:
	- ZeroMQ
	- FastCGI
	- JSROOT
	- web
	- ...


<!-- README.md ends here -->
