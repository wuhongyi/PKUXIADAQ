<!-- OFFLINE_Energy-FF.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 10月  7 09:01:21 2018 (+0800)
;; Last-Updated: 六 10月 20 19:31:30 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 2
;; URL: http://wuhongyi.cn -->

# Energy-FF

![EnergyFFGraph](/img/EnergyFFGraph.png)

This interface is a two-dimensional map of the energy and fast filter peak heights used to determine the appropriate threshold. The left picture is a two-dimensional correlation between energy and fast filter, which should have a good linear relationship. The right picture shows the two-dimensional correlation between the energy and the remaining maximum value of the part of the fast filer where the trapezoid is thrown away, which characterizes the noise level. And the energy should be uncorrelated with this value.

该界面是能量与 fast filter 峰高的二维关联图。用于确定合适的阈值。左图是能量与 fast filter的二维关联，它们应该有个较好的线性关系，右图为能量与 fast filer 中抛除梯形部分剩余中最大值的二维关联，抛除梯形部分剩余分布的最大值表征噪声水平，能量跟该值应该是没有关联的。


Firstly, for **Draw Style** we choose **Graph**, which is the 2D scatter plot mode. Select the channel you want to view, then press the *Draw* button to start the calculation, in the top left corner the progress from time to time can be monitored, or you can press the *Stop* button to terminate the calculation early. The calculation ends as shown in the figure above.

首先 Draw Style 选择 Graph，即二维散点图模式。选择查看通道，然后按 Draw 按钮则开始进入计算，左上角可时时监视进度，也可按 Stop 按钮提前终止计算。计算结束得到如上图所示。

![EnergyFFHist](/img/EnergyFFHist.png)

The 2D scatter plot does not visually show the density distribution of the displayed data points, so for **Draw Style** we select the **Hist mode**. Select the bins or ranges of the X and Y axis, and then press the *Draw* button to start the calculation. The result is shown in the figure above, and the right figure reflects the level of noise.

二维散点图并不能很直观显示展示数据点的密度分布，因此 Draw Style 选择 Hist 模式，选择 X、Y轴的分 bin 数即范围，然后同样按 Draw 按钮开始计算。结果如上图所示，右图反映了噪声的水平。



<!-- OFFLINE_Energy-FF.md ends here -->
