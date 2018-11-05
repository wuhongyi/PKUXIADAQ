<!-- OFFLINE_FFCFDThre.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 10月  7 09:01:44 2018 (+0800)
;; Last-Updated: 一 11月  5 17:07:06 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 3
;; URL: http://wuhongyi.cn -->

# FF/CFD Thre

![FFCFDThre](/img/FFCFDThre.png)

This interface is used for the accumulation of fast filter waveforms and cfd filter waveforms. The user selects the channel to view and presses the *Draw* button to start the calculation. The top left corner of the page can monitor the progress from time to time, or press the *Stop* button to terminate the calculation early. The calculation ends as shown in the figure above.


The upper buttons *FFProjectY*, *CFDProjectY*, *OriginalCFD*, and *CalculateCFD* respectively pop up the sub-painters.


![FFCFDThreFFProjectY](/img/FFCFDThreFFProjectY.png)

Click the button *FFProjectY* to open the view of the fast filter projection. Click again to close the function. When the function is turned on, place the mouse on the 2D map and move the mouse left and right. The Fast Filter ShowProjectY sub-panel displays the projection distribution of the position pointed by the mouse. This distribution before triggering also characterizes the level of noise.


![FFCFDThreCFDProjectY](/img/FFCFDThreCFDProjectY.png)

Similarly, the button *CFDProjectY* function is as shown in the figure above.


![FFCFDThreCFD](/img/FFCFDThreCFD.png)

Click the button *OriginalCFD* to display the distribution of CFD values in the raw data on the left. Click the button *CalculateCFD* to display the result of the offline waveform calculation on the right, and the parameters used for the calculation are the current ones. For a suitable CFD parameter setting, the CFD should be evenly distributed.



<!-- OFFLINE_FFCFDThre.md ends here -->
