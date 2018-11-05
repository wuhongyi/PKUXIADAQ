<!-- OFFLINE_AdjustPar.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 10月  7 08:54:43 2018 (+0800)
;; Last-Updated: 六 10月 20 19:22:57 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 2
;; URL: http://wuhongyi.cn -->

# Adjust Par

![offlineapi](/img/OfflineAPI.png)

To calculate the fast filter and slow filter cfd curves offline from the acquired waveform, the following requirements are imposed on the acquired waveform. In the above figure, the calculated energy is the difference between the energy of the algorithm and the baseline of the algorithm. To get the correct trapezoid, the premise is that there is a long enough point in front to calculate the baseline.

要通过采集的波形离线计算fast filter、slow filter cfd 曲线，对采集的波形有以下要求。如上图中，计算的能量是算法的能量与算法的基线的差，要得到正确的梯形，那么前提是前面有足够长的点来计算基线。

In the figure, EFRT stands for Energy Filter Rise Time and EFFT stands for Energy Filter Flat Top.


To compute energy filter response offline, the ideal settings are:
- Total trance length > 2 * ( 2 * EFRT + EFFT)
- Pre-trigger trace (Trace-delay) length > ( 3 * EFRT + EFFT )

Of course, this is just one way to calculate the trapezoid. If we record the baseline of the energy trapezoid for each event and use the average of the pre-trigger partial points as the infinite extension to the left of the waveform, then this is not limited by this condition Pre-trigger trace length > ( 3 x EFRT + EFFT ) . In the following pages, when using the Old Baseline method to calculate the energy trapezoid, there is a premise that the pre-trigger trace length requires at least 200 points because the left side of the waveform is averaged by the first 200 points.

当然，这只是计算梯形的一个方法，如果我们记录了每个事件的能量梯形的基线，并且采用pre-trigger部分点的平均值作为波形左侧的无限延伸，那么就不受 Pre-trigger trace length > ( 3 x EFRT + EFFT ) 条件的限制了。下面的页面中，当采用 Old Baseline 方法来计算能量梯形时，有个前提是 pre-trigger trace length 至少需要有 200 个点，因为波形左侧延伸采用前 200 个点来平均。

![Adjustpars3](/img/Adjustpars3.png)

When the acquired waveform pre-trigger trace length > 3 x EFRT + EFFT, the pre-trigger trace provides enough points to calculate the baseline, and the SF BL algorithm can choose Calculate, otherwise the Old Baseline algorithm needs to be selected. When recording data with the premise of the Old Baseline algorithm, select to turn on the baseline for the trapezoid and the raw E sums/baseline option for the InitData page. When the Old Baseline algorithm is selected, the next four option parameters take effect, which are the parameters of the energy trapezoid used in the data acquisition.

当采集的波形 pre-trigger trace length > 3 x EFRT + EFFT 时，pre-trigger trace 提供足够多的点来计算基线，SF BL 算法可选择 Calculate，否则需要选择 Old Baseline 算法。选择Old Baseline 算法的前提时记录数据的时候， 选择开启记录 梯形的baseline，并且 InitData 页面的 raw E sums/baseline 选项开启。当选择 Old Baseline 算法时，之后的四个选项参数生效，该四个参数为该数据采集时候所用的能量梯形的参数。

The green curve in the above figure is a typical condition that does not satisfy the pre-trigger trace length > 3 x EFRT + EFFT when the Calculate algorithm is used. The figure shows a pre-trigger trace length of 10 us, an EFRT of 5.04 us, and an EFFT of 1.60 us.

上图中绿色曲线为典型的不满足 pre-trigger trace length > 3 x EFRT + EFFT 时，采用的 Calculate 算法造成的结果。图中显示 pre-trigger trace length 为 10 us，EFRT 为 5.04 us，EFFT 为 1.60 us。

In this case, you should use the Old Baseline algorithm shown below.

此时，应该采用下图所示的  Old Baseline 算法。

![Adjustpars1](/img/Adjustpars1.png)

The user can choose to view the channel of the waveform. The button *Load* can read and display the current parameter setting. When modifying the above parameters, you need to press the *Apply* button to make it effective. The button *Draw* is used to display the event waveform of the next channel.

用户可选择查看波形的通道，按钮 Load 可读取并显示当前的参数设置情况，当修改以上的参数时候，需要按 Apply 按钮使之生效。按钮 Draw 用来显示下一个该通道的事件波形。

Users can choose to display multiple waveforms in **Wave / Slow Filter / Fast filter / Thres / CFD / CFD Thres** or select the drawing style of the curve.

用户可选择同时显示 Wave / Slow Filter/ Fast filter / Thres / CFD / CFD Thres 中的多个波形。或者选择曲线的绘画样式。


![Adjustpars2](/img/Adjustpars2.png)

The figure above shows four waveforms，fast filter, Thres, CFD, and CFD Thres. The pattern displays in dots. The lowest level horizontal bar can be dragged at both ends, and the user can pull to control the display range of the waveform abscissa, as shown in the figure of 800 - 1300 points. In this case, clicking the Draw button will hold the specified coordinate range.

上图展示了显示 fast filter、Thres、CFD、CFD Thres 四个波形，图样采用点显示方式。最低端的水平条两端可以拖动，用户可拉动来控制波形横坐标的显示范围，如图中显示800 - 1300 的点。该情况下，点击 Draw 按钮，将会保持该指定的坐标范围。


<!-- OFFLINE_AdjustPar.md ends here -->
