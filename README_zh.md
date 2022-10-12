<!-- README.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 六 6月 18 13:37:42 2016 (+0800)
;; Last-Updated: 三 10月 12 15:01:35 2022 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 178
;; URL: http://wuhongyi.cn -->

# A general-purpose digital data acquisition system(GDDAQ)

<!-- toc -->

![Yi logo](Yilogo100.png)

[English](https://github.com/wuhongyi/PKUXIADAQ/blob/master/README.md) | 简体中文

<!-- [English](https://github.com/pkuNucExp/PKUXIADAQ/blob/master/README.md) | 简体中文 -->

## 版本

- **GUI Version: 2022.10.12**  
- **DecodeAndSortAll Version: 2022.04.17**
- **EventBuilder Version: 2022.05.01**

程序下载请访问:  [PKUXIADAQ](https://github.com/wuhongyi/PKUXIADAQ)

网页版说明书请访问:  [简体中文/English](http://wuhongyi.cn/PKUXIADAQ/)

MZTIO 网页版说明书请访问:  [简体中文/English](http://wuhongyi.cn/MZTIO/)


- 对本获取程序有任何的意见及建议(功能添加及改进)，欢迎给吴鸿毅(wuhongyi@qq.com)发邮件。
- 我们将会尽快完善软件的中英文使用说明书。


----

## 关于

**本说明书仅适用于 XIA LLC 的 Pixie-16 系列采集卡**

- 本程序由[北京大学实验核物理组](https://github.com/pkuNucExp)开发。
- 最早的图形界面程序是基于 [NSCL DDAS Nscope](http://docs.nscl.msu.edu/daq/newsite/ddas-1.1/nscope.html) 开发。



技术指导:
- [Zhihuan Li 李智焕](https://github.com/zhihuanli)
- Hui Tan  谭辉(XIA LLC)
- Wolfgang Hennig(XIA LLC)

软件主要开发者:
- 2015 - 2016
	- Jing Li 李晶(lijinger02@126.com) 
- 2016 - now
	- Hongyi Wu 吴鸿毅(wuhongyi@qq.com) 

说明书主要撰写者:
- [Diwen Luo 罗迪雯](https://github.com/luodiwen)
- [Hongyi Wu 吴鸿毅](https://github.com/wuhongyi)


美术指导：
- Yi Song 宋祎


本程序的开发得到以下单位的支持：
- XIA LLC
- 中国科学院近代物理研究所(IMP)
- 中国原子能科学研究院(CIAE)
- 香港大学(HKU)
- 山东大学(威海)(SDU)
- ...

----

本程序适用于 XIA Pixie16 系列采集卡，支持 100/250/500 MHz 采集卡(具体支持型号可查看图形软件中的File->About)，最大支持 8 个机箱同步运行，即 1600+ 路信号同时采集。**本程序包要求使用 CERN ROOT6 版本。要求采用 1920x1080 及以上分辨率显示屏**。

本程序的设计兼容 100/250/500 MHz 采集卡的混合使用，只需在 cfgPixie16.txt 添加相应采样率采集卡的固件位置即可，程序在线能够自动识别采集卡类型并加载相应的固件。当前我们测试了绝大多数类型的采集卡，因此默认可运行我们测试过类型的采集卡，如要支持其它类型，请联系 XIA LLC 获取对应固件或者联系吴鸿毅(wuhongyi@qq.com)。


----

## 文件目录

用户使用程序包中包含以下文件/文件夹:

- DecodeAndSortAll(将原始二进制数据转为 ROOT)
- docs(使用说明书，网页版)
- EventBuilder(事件重构程序，可选)
- firmware(固件)
	- firmware/firmware.md(历史各版本固件说明)
- GUI(图形软件)
- icons(logo)
- mysql
	- createdb
	- onlline
	- www
- NOGUI(非图形软件)
- OnlineStattics(在线监视程序)
- papers
- parset(参数设置文件)
- PlxSdk823.tar(Plx9054 驱动)
- README(markdown 版本说明书)
- RealDecoder
- README.md(主页介绍)
- README.pdf(pdf 版本说明书)
- software(非标准驱动，吴鸿毅修改)
- TestTool(开发者测试工具，用户不需要！！！)

----

## 升级计划

- 当前基于 ROOT GUI 开发的主控制界面复杂度高，用户修改难度大。其它用户不容易基于其发展适合自己的版本。
- 我们也在开发基于网页控制的获取在线/离线分析程序:
	- Qt
	- ZeroMQ
	- JSROOT
	- ...

## 版权声明

如果您使用本软件或者基于本软件开发的任何软件，请引用以下文献：

- [A general-purpose digital data acquisition system (GDDAQ) at Peking University](https://doi.org/10.1016/j.nima.2020.164200)
- [Performance of digital data acquisition system in gamma-ray spectroscopy](https://doi.org/10.1007/s41365-021-00917-8)
- [基于数字化的通用获取系统及波形分析算法](https://doi.org/10.1360/TB-2021-0552)
 
使用包含 pku 的固件，请发邮件联系吴鸿毅（wuhongyi@qq.com/wuhongyi@pku.edu.cn）获取授权。  
 

<!-- README.md ends here -->
