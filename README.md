<!-- README.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 六 6月 18 13:37:42 2016 (+0800)
;; Last-Updated: 二 7月 19 21:02:23 2022 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 177
;; URL: http://wuhongyi.cn -->

# A general-purpose digital data acquisition system(GDDAQ)

<!-- toc -->

![PKU logo](pkulogo100.jpg)

English | [简体中文](https://github.com/wuhongyi/PKUXIADAQ/blob/master/README_zh.md)

<!-- English | [简体中文](https://github.com/pkuNucExp/PKUXIADAQ/blob/master/README_zh.md) -->

A general-purpose digital data acquisition system (GDDAQ) has been developed at Peking University. This GDDAQ, composed of 16-channel Digital Pulse Processor Pixie-16 modules from XIA LLC, is a versatile, flexible, and easily expandable data acquisition system for nuclear physics research in China. The software used by this GDDAQ is based on the CERN ROOT framework and developed and tested in CentOS 7 LINUX operating platform. A flexible trigger system has also been developed to accommodate different experimental settings. A user-friendly software GUI helps users monitor and debug the detection system in real timer or offline. Many offline analysis tools have been developed to help users quickly optimize parameters for various types of detectors without the need for time-consuming tests and measurements. This GDDAQ has been successfully implemented in several nuclear physics experiments and its versatility and high efficiency have been demonstrated.

## Version

- **GUI Version: 2022.07.19**  
- **DecodeAndSortAll Version: 2022.04.17**
- **EventBuilder Version: 2022.05.01**


Download the latest version:  [PKUXIADAQ](https://github.com/wuhongyi/PKUXIADAQ)

manual version in web page:  [简体中文/English](http://wuhongyi.cn/PKUXIADAQ/)

MZTIO manual version in web page:  [简体中文/English](http://wuhongyi.cn/MZTIO/)


- If you have any comments or suggestions for this project(function addition or improvement), please send an email to Hongyi Wu(wuhongyi@qq.com). 
- We will improve the Chinese/English manuals of the software as soon as possible. Currently, we mainly explain the use of the software through our demonstration. 


----

## About

**This manual applies only to XIA LLC Pixie-16**

- This program is developed by the [Group of Experimental Nuclear Physics, Peking University](https://github.com/pkuNucExp).
- The earliest graphical interface development of this program is based on [NSCL DDAS Nscope](http://docs.nscl.msu.edu/daq/newsite/ddas-1.1/nscope.html). 
- Thanks to Hui Tan's(XIA LLC) support for our development.


Technical adviser:
- [Zhihuan Li 李智焕](https://github.com/zhihuanli)
- Hui Tan 谭辉(XIA LLC)
- Wolfgang Hennig(XIA LLC)

Software Developer:
- 2015 - 2016
	- Jing Li 李晶(lijinger02@126.com) 
- 2016 - now
	- Hongyi Wu 吴鸿毅(wuhongyi@qq.com) 

Principal author of the instruction:
- [Diwen Luo 罗迪雯](https://github.com/luodiwen)
- [Hongyi Wu 吴鸿毅](https://github.com/wuhongyi)


Art director:
- Yi Song 宋祎

The development of this program is supported by the following：
- XIA LLC
- Institute of Modern Physics, Chinese Academy of Sciences(IMP)
- China Institute of Atomic Energy(CIAE)
- The University of Hong Kong(HKU)
- Shandong University, Weihai(SDU)
- ...

----

This program is applicable to XIA Pixie16 module, which supports the 100/250/500 MHz sampling rates(specifically, information about the module can be found in File->About in the graphics software) and supports up to 8-chassis synchronous operation, that maeans, at least 1600-channel signals are simultaneously collected. **This package requires the CERN ROOT6 version and the resolution display above 1920x1080**.

The program is designed to be compatible with the 100/250/500 MHz modules. Simply add the firmware location of the corresponding sample rate modules to cfgPixie16.txt. The program can automatically identify the module's type and load the corresponding firmware.

Currently we have tested most types of modules, so you can run the type of modules of our tested by default. To support other types, please contact XIA LLC to obtain the corresponding firmware or contact Hongyi Wu(wuhongyi@qq.com).

----

## contents

The following files/folders are included in the user's use package:

- DecodeAndSortAll(Converting raw binary data to ROOT file)
- docs(Web page manual)
- EventBuilder(event builder，optional)
- firmware
	- firmware/firmware.md(History of firmware)
- GUI
- icons(logo)
- mysql
	- createdb
	- onlline
	- www
- NOGUI(Non-graphics software)
- OnlineStattics(Online monitoring program)
- papers
- parset(parameter setting file)
- PlxSdk823.tar(Plx9054 driver)
- README(manual version in markdown)
- RealDecoder
- README.md(introduction for home page)
- README.pdf(manual version in pdf)
- software(non-standard pixie16 driver API revised by Hongyi Wu)
- TestTool(testing tool for developer, not necessary for users)

----

## Update

- The main control interface development based on the ROOT GUI is highly complex, which is difficult for users to modify now. It is not easy for users to develop their own version based on this program. 
- We are also developing acquisition software for online/offline analysis based on web control:
	- Qt
	- ZeroMQ
	- JSROOT
	- ...

## License

If you use part of this software, please cite the following paper:

- [A general-purpose digital data acquisition system (GDDAQ) at Peking University](https://doi.org/10.1016/j.nima.2020.164200)
- [Performance of digital data acquisition system in gamma-ray spectroscopy](https://doi.org/10.1007/s41365-021-00917-8)
- [基于数字化的通用获取系统及波形分析算法](https://doi.org/10.1360/TB-2021-0552)

<!-- README.md ends here -->
