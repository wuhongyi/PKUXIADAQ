<!-- DropDown4.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 10月  7 08:50:48 2018 (+0800)
;; Last-Updated: 六 10月 20 19:18:09 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 2
;; URL: http://wuhongyi.cn -->

# Offiline

The contents in this drop-down column are the offline parameter optimization adjustment.  
本下拉栏中为离线参数优化调节。

## InitData

![Adjust Par](/img/AdjustPar.png)

- *Run*: Select the file run number to be read, *Mod*: selects to read which module, and the button *Read* loads the file main information (channel address, energy, waveform position, etc.) into the memory.
- Additional analysis: Among the three options, the selection indicates that the information is included when reading the file data into memory. Some analysis methods can only be enabled if this data is read. But the premise is that this information needs to be recorded during data collection.

- Run 选择要读取的文件运行编号，Mod 选择要读取第几个采集卡，按钮 Read 将文件主要信息(道址、能量、波形位置等)载入内存。
- Additional analysis: 三个选项中，选择表示读取该文件数据到内存中时包括该信息。只有读取了该数据，才能启用一些分析方法。但是前提是数据采集时候需要记录该信息。


- InitData: This page. Read the binary file.
- Adjust Par:  Waveform/fast filter/energy filter/cfd/threshould of the select channel.
- Wave-16:  Waveform/fast filter/energy filter/cfd filter/threshould of the 16 channels.
- Energy-16:  The original energy spectrum of the 16 channel..
- Orig Energy:  The original energy spectrum of the select channel.
- Calc Energy:  Recalculation of energy spectrum by waveforms.
- FF/CFD Thre:   Accumulation of a large number of fast filter/cfd filter waveforms to determine the threshold.
- Energy-FF:  The two dimensional relationship between energy and fast filter first/second peak height.
- QDC:  It will be finished soon.
- FFT:  Fourier transformation of a single waveform. User can choose XIA/CAEN/FFTW3 functions.
- Time Diff:  Time difference of two signal.

<!-- DropDown4.md ends here -->
