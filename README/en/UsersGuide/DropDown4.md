<!-- DropDown4.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 10月  7 08:50:48 2018 (+0800)
;; Last-Updated: 一 11月  5 17:00:03 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 3
;; URL: http://wuhongyi.cn -->

# Offiline

The contents in this drop-down column are the offline parameter optimization adjustment.  


## InitData

![Adjust Par](/img/AdjustPar.png)

- *Run*: Select the file run number to be read, *Mod*: selects to read which module, and the button *Read* loads the file main information (channel address, energy, waveform position, etc.) into the memory.
- Additional analysis: Among the three options, the selection indicates that the information is included when reading the file data into memory. Some analysis methods can only be enabled if this data is read. But the premise is that this information needs to be recorded during data collection.



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
