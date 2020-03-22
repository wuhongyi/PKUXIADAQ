.. README.rst --- 
.. 
.. Description: 
.. Author: Hongyi Wu(吴鸿毅)
.. Email: wuhongyi@qq.com 
.. Created: 二 7月  2 20:19:34 2019 (+0800)
.. Last-Updated: 日 3月 22 19:08:00 2020 (+0800)
..           By: Hongyi Wu(吴鸿毅)
..     Update #: 30
.. URL: http://wuhongyi.cn 

=================================   
Introduction
=================================

.. image:: /_static/img/pkulogo100.jpg

---------------------------------
Version
---------------------------------

**We recommend users to download a stable version**

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Stable Version
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**Stable Version Version:2020.03.20**  

Download the latest version, please click:  `PKUXIADAQ stable <https://github.com/pkuNucExp/PKUXIADAQ>`_ 

For web page manual, please click:  `English/简体中文 <https://pkunucexp.github.io/PKUXIADAQ/>`_  

- manual version in reStructuredText: README/  
- manual version in offline web: docs/  
- manual version in pdf: README_en.pdf  README_ch.pdf  


^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Pre-alpha
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**Pre-alpha Version:2020.03.20**  

Download the latest version:  `PKUXIADAQ <https://github.com/wuhongyi/PKUXIADAQ>`_ 

manual version in web page:  `简体中文/English <http://wuhongyi.cn/PKUXIADAQ/>`_ 


- If you have any comments or suggestions for this project(function addition or improvement), please send an email to Hongyi Wu(wuhongyi@qq.com). 
- We will improve the Chinese/English manuals of the software as soon as possible. Currently, we mainly explain the use of the software through our demonstration. 


----

---------------------------------  
About
---------------------------------

**This manual applies only to XIA LLC Pixie-16**

- This program is developed by the `Group of Experimental Nuclear Physics, Peking University <https://github.com/pkuNucExp>`_ .
- The earliest graphical interface development of this program is based on `NSCL DDAS Nscope <http://docs.nscl.msu.edu/daq/newsite/ddas-1.1/nscope.html>`_ . 
- Thanks to *Hui Tan's(XIA LLC)* support for our development.

Technical adviser:

- `Zhihuan Li 李智焕 <https://github.com/zhihuanli>`_ 
- Hui Tan  谭辉(XIA LLC)
- Wolfgang Hennig(XIA LLC)

Software Developer:

- 2015 - 2016
	- Jing Li 李晶(lijinger02@126.com) 
- 2016 - now
	- Hongyi Wu 吴鸿毅(wuhongyi@qq.com) 

Principal author of the instruction:

- `Diwen Luo 罗迪雯 <https://github.com/luodiwen>`_ 
- `Hongyi Wu 吴鸿毅 <https://github.com/wuhongyi>`_
- `Xiang Wang 王翔 <https://github.com/wangxianggit>`_ 

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

---------------------------------
File contents
---------------------------------

The following files/folders are included in the user's use package:

- Decode(Converting raw binary data to ROOT file)
- docs(Web page manual)
- firmware
	- firmware/firmware.md(History of firmware)
- GUI
- MakeEvent(event builder，optional)
- NOGUI(Non-graphics software)
- OnlineStattics(Online monitoring program)
- parset(parameter setting file)
- PlxSdk.tar.gz(Plx9054 driver)
- README(manual version in markdown)
- README.md(introduction for home page)
- README.pdf(manual version in pdf)
- software(non-standard pixie16 driver API revised by Hongyi Wu)
- TestTool(testing tool for developer, not necessary for users)


----

---------------------------------  
Update plan
---------------------------------  

- The main control interface development based on the ROOT GUI is highly complex, which is difficult for users to modify now. It is not easy for users to develop their own version based on this program.
- We are also developing acquisition software  for online/offline analysis based on web control:
	- Django
	- ZeroMQ
	- JSROOT
	- ...

---------------------------------  
License
---------------------------------

..
   This project is licensed under the MIT License - see the LICENSE.md file for details
	  

.. 
.. README.rst ends here
