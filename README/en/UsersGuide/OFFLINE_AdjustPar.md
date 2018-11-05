<!-- OFFLINE_AdjustPar.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 10月  7 08:54:43 2018 (+0800)
;; Last-Updated: 一 11月  5 17:02:06 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 3
;; URL: http://wuhongyi.cn -->

# Adjust Par

![offlineapi](/img/OfflineAPI.png)

To calculate the fast filter and slow filter cfd curves offline from the acquired waveform, the following requirements are imposed on the acquired waveform. In the above figure, the calculated energy is the difference between the energy of the algorithm and the baseline of the algorithm. To get the correct trapezoid, the premise is that there is a long enough point in front to calculate the baseline.


In the figure, EFRT stands for Energy Filter Rise Time and EFFT stands for Energy Filter Flat Top.


To compute energy filter response offline, the ideal settings are:
- Total trance length > 2 * ( 2 * EFRT + EFFT)
- Pre-trigger trace (Trace-delay) length > ( 3 * EFRT + EFFT )

Of course, this is just one way to calculate the trapezoid. If we record the baseline of the energy trapezoid for each event and use the average of the pre-trigger partial points as the infinite extension to the left of the waveform, then this is not limited by this condition Pre-trigger trace length > ( 3 x EFRT + EFFT ) . In the following pages, when using the Old Baseline method to calculate the energy trapezoid, there is a premise that the pre-trigger trace length requires at least 200 points because the left side of the waveform is averaged by the first 200 points.


![Adjustpars3](/img/Adjustpars3.png)

When the acquired waveform pre-trigger trace length > 3 x EFRT + EFFT, the pre-trigger trace provides enough points to calculate the baseline, and the SF BL algorithm can choose Calculate, otherwise the Old Baseline algorithm needs to be selected. When recording data with the premise of the Old Baseline algorithm, select to turn on the baseline for the trapezoid and the raw E sums/baseline option for the InitData page. When the Old Baseline algorithm is selected, the next four option parameters take effect, which are the parameters of the energy trapezoid used in the data acquisition.

The green curve in the above figure is a typical condition that does not satisfy the pre-trigger trace length > 3 x EFRT + EFFT when the Calculate algorithm is used. The figure shows a pre-trigger trace length of 10 us, an EFRT of 5.04 us, and an EFFT of 1.60 us.

In this case, you should use the Old Baseline algorithm shown below.

![Adjustpars1](/img/Adjustpars1.png)

The user can choose to view the channel of the waveform. The button *Load* can read and display the current parameter setting. When modifying the above parameters, you need to press the *Apply* button to make it effective. The button *Draw* is used to display the event waveform of the next channel.

Users can choose to display multiple waveforms in **Wave / Slow Filter / Fast filter / Thres / CFD / CFD Thres** or select the drawing style of the curve.


![Adjustpars2](/img/Adjustpars2.png)

The figure above shows four waveforms，fast filter, Thres, CFD, and CFD Thres. The pattern displays in dots. The lowest level horizontal bar can be dragged at both ends, and the user can pull to control the display range of the waveform abscissa, as shown in the figure of 800 - 1300 points. In this case, clicking the Draw button will hold the specified coordinate range.


<!-- OFFLINE_AdjustPar.md ends here -->
