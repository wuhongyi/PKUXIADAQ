<!-- OFFLINE_Energy-FF.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 10月  7 09:01:21 2018 (+0800)
;; Last-Updated: 一 11月  5 17:07:38 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 3
;; URL: http://wuhongyi.cn -->

# Energy-FF

![EnergyFFGraph](/img/EnergyFFGraph.png)

This interface is a two-dimensional map of the energy and fast filter peak heights used to determine the appropriate threshold. The left picture is a two-dimensional correlation between energy and fast filter, which should have a good linear relationship. The right picture shows the two-dimensional correlation between the energy and the remaining maximum value of the part of the fast filer where the trapezoid is thrown away, which characterizes the noise level. And the energy should be uncorrelated with this value.


Firstly, for **Draw Style** we choose **Graph**, which is the 2D scatter plot mode. Select the channel you want to view, then press the *Draw* button to start the calculation, in the top left corner the progress from time to time can be monitored, or you can press the *Stop* button to terminate the calculation early. The calculation ends as shown in the figure above.

![EnergyFFHist](/img/EnergyFFHist.png)

The 2D scatter plot does not visually show the density distribution of the displayed data points, so for **Draw Style** we select the **Hist mode**. Select the bins or ranges of the X and Y axis, and then press the *Draw* button to start the calculation. The result is shown in the figure above, and the right figure reflects the level of noise.



<!-- OFFLINE_Energy-FF.md ends here -->
