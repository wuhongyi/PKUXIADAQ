<!-- OFFLINE_Calc Energy.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 10月  7 08:58:08 2018 (+0800)
;; Last-Updated: 六 10月 20 19:27:15 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 2
;; URL: http://wuhongyi.cn -->

# Calc Energy

![CalcEnergy](/img/CalcEnergy.png)

This page recalculates the energy using the acquired waveform. Like the **Adjust Par** page, the SF BL algorithm can choose either the Calculate algorithm or the Old Baseline algorithm.

该页面利用采集的波形重新计算能量。同 Adjust Par 页面一样，SF BL 算法可选择Calculate算法或者 Old Baseline 算法。

The fast filter and energy filter parameters used to calculate the energy adopt the setting parameters of the module. The user needs to select the number of bins that the energy about 65536 channels are divided into. You can select 1024/2048/4096/8192/16384/32768/65536. Select the calculated channel and then press the button *Draw* to start the calculation, the upper left corner will display the progress of the calculation, or press the button *Stop* to terminate the calculation early. When the calculation is terminated, the spectrum will be displayed on the artboard.

计算能量采用的fast filter、energy filter参数采用采集卡的设置参数，用户需要选择能量 0-65536 分成多少个 bin，可选择 1024/2048/4096/8192/16384/32768/65536，选择计算的通道，然后按按钮 Draw即开始计算，左上角将会显示计算的进度，也可以按按钮 Stop 提前终止计算。当计算终止时，画板上将显示能谱。


<!-- OFFLINE_Calc Energy.md ends here -->
