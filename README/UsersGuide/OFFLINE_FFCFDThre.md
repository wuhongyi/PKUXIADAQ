<!-- OFFLINE_FFCFDThre.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 10月  7 09:01:44 2018 (+0800)
;; Last-Updated: 六 10月 20 19:29:38 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 2
;; URL: http://wuhongyi.cn -->

# FF/CFD Thre

![FFCFDThre](/img/FFCFDThre.png)

This interface is used for the accumulation of fast filter waveforms and cfd filter waveforms. The user selects the channel to view and presses the *Draw* button to start the calculation. The top left corner of the page can monitor the progress from time to time, or press the *Stop* button to terminate the calculation early. The calculation ends as shown in the figure above.

该界面用于 fast filter波形、cfd filter波形的累加。用户选择查看通道，然后按 Draw 按钮则开始进入计算，左上角可时时监视进度，也可按 Stop 按钮提前终止计算。计算结束得到如上图所示。

The upper buttons FFProjectY, CFDProjectY, OriginalCFD, and CalculateCFD respectively pop up the sub-painters.

上方按钮 FFProjectY、CFDProjectY、OriginalCFD、CalculateCFD 分别可弹出子画板。

![FFCFDThreFFProjectY](/img/FFCFDThreFFProjectY.png)

Click the button *FFProjectY* to open the view of the fast filter projection. Click again to close the function. When the function is turned on, place the mouse on the 2D map and move the mouse left and right. The Fast Filter ShowProjectY sub-panel displays the projection distribution of the position pointed by the mouse. This distribution before triggering also characterizes the level of noise.

点击按钮 FFProjectY，则开启查看fast filter投影图，再次点击则关闭该功能。开启功能时，将鼠标放在二维图上，左右移动鼠标， Fast Filter ShowProjectY 子画板则显示鼠标指向的该位置的投影分布。触发前的该分布，也表征噪声的水平。

![FFCFDThreCFDProjectY](/img/FFCFDThreCFDProjectY.png)

Similarly, the button CFDProjectY function is as shown in the figure above.

同理，按钮 CFDProjectY 功能如上图所示。

![FFCFDThreCFD](/img/FFCFDThreCFD.png)

Click the button *OriginalCFD* to display the distribution of CFD values in the raw data on the left. Click the button *CalculateCFD* to display the result of the offline waveform calculation on the right, and the parameters used for the calculation are the current ones. For a suitable CFD parameter setting, the CFD should be evenly distributed.

点击按钮 OriginalCFD，则展示左图中原始数据中 CFD 数值的分布。点击按钮 CalculateCFD，则展示右图中通过离线波形计算的结果，计算所用参数为当前的参数。对于一个合适的 CFD 参数设置，该CFD分布该是平均分布的。 

<!-- OFFLINE_FFCFDThre.md ends here -->
