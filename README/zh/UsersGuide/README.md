<!-- README.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 5月 13 15:47:48 2018 (+0800)
;; Last-Updated: 一 11月  5 15:22:28 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 17
;; URL: http://wuhongyi.cn -->

# Guide

<!-- toc -->

**User's Manual** *Digital Gamma Finder (DGF) PIXIE-16*  **Version 1.40, October 2009**  
**Pixie-16 User Manual**  **Version 3.00**  **August 21, 2018**

----


> **[info] IMPORTANT**
>
> The Pixie-16 is designed for single exponentially decaying signals. 
>
> Step pulses or short non-exponential pulses can be accommodated with specific parameter settings. 
>
> Staircase type signals from reset preamplifiers generally need to be AC coupled.

> **[danger] IMPORTANT**
>
> The amplitude of the detector output signals is not recommended to exceed +/-3.5V if 50Ohm input termination jumper is installed and the 1:4 attenuation is not used.
>
> Do Not Hot-Swap!
>
> To avoid personal injury, and/or damage to the DGF-Pixie-16, always turn off crate power before removing the DGF-Pixie-16 from the crate!



----



![pixie16](/img/pixie16.jpg)

The DGF Pixie-16 is a 16-channel all-digital waveform acquisition and spectrometer card based on the CompactPCI/PXI standard for fast data readout to the host. It combines spectroscopy with waveform digitizing and the option of on-line pulse shape analysis. The Pixie-16 accepts signals from virtually any radiation detector. Incoming signals are digitized by 12/14/16-bit 100/250/500 MSPS ADCs. Waveforms of up to 163.8 μs in length for each channel can be stored in a FIFO.

The waveforms are available for onboard pulse shape analysis, which can be customized by adding user functions to the core processing software. Waveforms, timestamps, and the results of the pulse shape analysis can be read out by the host system for further off-line processing. Pulse heights are calculated to 16-bit precision and can be binned into spectra with up to 32K channels. The Pixie-16 supports coincidence spectroscopy and can recognize complex hit patterns.


Data readout rates through the CompactPCI/PXI backplane to the host computer can be up to 109 Mbyte/s. The standard PXI backplane, as well as additional custom backplane connections are used to distribute clocks and trigger signals between several Pixie-16 modules for group operation. A complete data acquisition and processing systems can be built by combining Pixie-16 modules with commercially available CompactPCI/PXI processor, controller or I/O modules in the same chassis.


The Pixie-16 is an instrument for waveform acquisition and MCA histogramming for arrays of gamma ray or other radiation detectors such as

- 100 MSPS
	- Segmented HPGe detectors.
	- Scintillator/PMT combinations: NaI, CsI, BGO and many others.
	- Gas detector.
	- Silicon strip detectors.
- 250 MSPS
	- Scintillator
	- LaBr3
- 500 MSPS
	- Scintillator
	- LaBr3





The Pixie-16 modules must be operated in a custom 6U CompactPCI/PXI chassis providing high currents at specific voltages not included in the CompactPCI/PXI standard 1 . Currently XIA provides a 14-slot chassis. Put the host computer(or remote PXI controller) in the system slot (slot 1) of your chassis. Put the Pixie-16 modules into any free peripheral slot (slot 2-14) with the chassis still powered down. After modules are installed, power up the chassis (Pixie-16 modules are not hot swappable). If using a remote controller, be sure to boot the host computer after powering up the chassis.



----

## Output Data Structures


![Header 100M](/img/data_100M.PNG)
![Header 250M](/img/data_250M.PNG)
![Header 500M](/img/data_500M.PNG)

*以下为可选择输出*


<!-- 
![External 1](/img/data_ex1.PNG)
![External 2](/img/data_ex2.PNG)
![External 3](/img/data_ex3.PNG)
![External 4](/img/data_ex4.PNG)
![External 5](/img/data_ex5.PNG)
![External 6](/img/data_ex6.PNG)
![External 7](/img/data_ex7.PNG)
![External 8](/img/data_ex8.PNG)
 -->
 
If trace recording is enabled, trace data will immediately follow the last word of the event header. Since raw ADC data points are 14-bit number, two 14-bit numbers are packed into one 32-bit word,as shown below. Since the event header could have variable length(4,6,8,10,12,14 ,16 or 18 words) depending on the selection of various output data options, the header length, event length and trace length that are recorded in the first 4 words of the event header should be used to navigate through the output data stream.



- CAEN 插件在采集波形时，一个插件所有通道每个事件波形采集长度只能设置成相同的。
- XIA 插件可以每个通道单独设置数据长度。例如：ch-0 设置采3000个点，ch-1 设置采2000个点，ch-2 设置采5000个点，ch-3 设置成不采集波形……。另外，还可每个通道选择是否记录baseline sum、QDC、external timestamps。
- XIA还可设置对pile up事件的处理。例如pile up事件记录波形，其它事件则不记录波形。意味着一个channel 不同事件其数据长度也不一定相等。
- 在读取数据时候，CAEN 插件每次拿到的数据都是完整事件的数据。 而 XIA 插件 FIFO 的读指针、写指针是独立的、同时进行的，所以每次我们要取数据时候都需要先查询下当前 FIFO 有多少数据，然后再指定该次读取多少数据。因为FIFO读、写是独立的，因而查询到的数据量是该时刻拥有的数据量，这个数据量基本都不是完整事件的数据量（还有部分是当前事件正在写入的）。每个事件除了有 4 words 的 header 及后面紧跟着的数据外并没有事件标记符，当然这样设计最大的好处就是速度快。



基于以上事实，两个公司的产品对用户写时时监视获取率、在线监视程序等的实现方式存在较大差异。
- CAEN： 由于 CAEN 一个插件上每个通道的采样数相同，因而他们每个事件的长度是一致的。在拿到数据时即可存文件或者 decode 出每个事件的信息用来统计时时获取速率，也可将数据发送到在线程序。
- XIA： 由于其上每个通道数据长度可单独设置，这是 XIA 插件的灵活性。又 FIFO 查询到的数据量是当前时刻的数据量，这样每次取到的数据都不是完整事件的数据。
- 对于多个插件的系统，获取时候一直循环读取每一个插件上的数据，如果将所有插件的数据存在同一个文件内，将造成数据的错乱。如果每个插件的数据单独存在一个文件内，那么下一次读取的该插件的数据中开头的不完整事件的数据刚好接上一次最后一个不完整事件的数据，可解决这个问题。
- 由于每次拿到的不是完整数据，因此也不好 decode 当前数据来获得每个事件的信息（从开始就decode记录每次数据的不完整事件的情况也可解决该问题，但是这样decode需要额外的时间）。但是 XIA 内部 DSP上统计着信号输入量、获取接收量、活时等信息。可频繁调用函数读取 DSP 上的统计信息来监视时时获取速率。但是频繁调用该函数会对获取的 I/O 有一点影响。几秒钟调用一次该函数，这个影响基本可以忽略。
- 由于每次拿到的不是完整数据，因此共享内存的在线监视也不容易实现，如若要实现，也不是没办法（从开始就decode记录每次数据的不完整事件的情况也可解决该问题，但是这样decode需要额外的时间）。
- 下面给出一些困难的解决思路： 牺牲XIA插件的灵活性，对一个插件设置成每个通道采集波形长度相同（这样就与CAEN一样了），因为每个通道的每个事件数据的长度相等，读取数据时候查询到当前的数据量之后，然后只读取事件长度整数倍的最大数据，这样每次拿到的都是完整的事件数据了，可参照CAEN的实现方式。
- 对采波形时候，可采用“伪”在线监视获取情况，即获取文件每隔一段时间保存一次，用上一轮的数据作为“在线”数据。


<!-- README.md ends here -->
