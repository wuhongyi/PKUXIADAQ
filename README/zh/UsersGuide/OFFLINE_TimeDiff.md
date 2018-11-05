<!-- OFFLINE_TimeDiff.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 10月  7 09:00:25 2018 (+0800)
;; Last-Updated: 一 10月 22 18:47:37 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 3
;; URL: http://wuhongyi.cn -->

# Time Diff

![Time Diff](/img/TimeDiff.png)

This interface is used to quickly view the time resolution of the two signals. The user can choose to view the time difference between the two signals of the zero crossing of the CFD algorithm or the time difference between two signals of the fast filter over threshold. **Xbin** represents the bin number of the abscissa, **Xmin** represents the minimum value of the abscissa, and **Xmax** represents the maximum value of the abscissa. With Ch A, Ch B we can select the two channels we want to view. Then press the *Draw* button.

该界面用于快速查看两路信号的时间分辨。用户可以选择查看 CFD 算法过零点的时间差或者 fast filter 过阈值的时间差。Xbin 表示横坐标分 bin 数，Xmin 表示横坐标的最小值，Xmax 表示横坐标的最大值。通过 Ch A、Ch B 来选择想要查看的两个通道。然后按 Draw 按钮即可。

The option **Limits** selection turns on the energy range constraint. After selecting this option, the following four parameters, AL, AR, BL, and BR, take effect, which respectively represent the left and right ranges of the Ch A/B energy address, and only events with energy falling in this interval are filled into the histogram. The **Orig Energy** page allows the user to select the appropriate energy channel interval.

选项 Limits 选择则开启能量范围约束。选择该选项后，之后的四个参数 AL、AR、BL、BR才生效，其分别表示 Ch A/B 能量道址的左右范围，只有能量落在这个区间的事件才填充到直方图中。用户可通过 Orig Energy 页面来选择合适的能量道址区间。

<!-- OFFLINE_TimeDiff.md ends here -->
