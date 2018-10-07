<!-- BASESETUP_TriggerFilter.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 10月  7 09:35:24 2018 (+0800)
;; Last-Updated: 日 10月  7 09:35:43 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 2
;; URL: http://wuhongyi.cn -->

# Trigger Filter

![Trigger Filter](/img/TriggerFilter.png)

界面下方的 Status 显示为**绿色的 Ready** 时表示可操作该界面，否则需要等待。底下按钮的操作同上。

- 参数 *Rise Time* 。。。
- 参数 *Flat Top* 。。。
- 参数 *Thresh.* 表示阈值，该数值的设置是相对 fast filter 波形。


**TODO**

**这里需要补充一个示意图，还有计算公式。**

Assume the digitized waveform stream can be represented by data series Trace[i], i=0,1,2,... First the fast filter response of the digitized waveform is computed as follows:  
**TODO公式**  
Where FL is called the fast length and FG is called the fast gap of the digital trapezoidal filter.

General rules of thumb for the following important parameters are:

- A longer trigger filter rise time averages more samples and thus allows setting lower thresholds without triggering on noise.
- Typically the threshold should be set as low as possible, just above the noise level.
- A longer trigger filter flat top time makes it easier to detect slow rising pulses.


<!-- BASESETUP_TriggerFilter.md ends here -->
