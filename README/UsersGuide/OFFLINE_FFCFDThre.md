<!-- OFFLINE_FFCFDThre.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 10月  7 09:01:44 2018 (+0800)
;; Last-Updated: 日 10月  7 09:01:54 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 1
;; URL: http://wuhongyi.cn -->

# FF/CFD Thre

![FFCFDThre](/img/FFCFDThre.png)

该界面用于 fast filter波形、cfd filter波形的累加。用户选择查看通道，然后按 Draw 按钮则开始进入计算，左上角可时时监视进度，也可按 Stop 按钮提前终止计算。计算结束得到如上图所示。

上方按钮 FFProjectY、CFDProjectY、OriginalCFD、CalculateCFD 分别可弹出子画板。

![FFCFDThreFFProjectY](/img/FFCFDThreFFProjectY.png)

点击按钮 FFProjectY，则开启查看fast filter投影图，再次点击则关闭该功能。开启功能时，将鼠标放在二维图上，左右移动鼠标， Fast Filter ShowProjectY 子画板则显示鼠标指向的该位置的投影分布。触发前的该分布，也表征噪声的水平。

![FFCFDThreCFDProjectY](/img/FFCFDThreCFDProjectY.png)

同理，按钮 CFDProjectY 功能如上图所示。

![FFCFDThreCFD](/img/FFCFDThreCFD.png)

点击按钮 OriginalCFD，则展示左图中原始数据中 CFD 数值的分布。点击按钮 CalculateCFD，则展示右图中通过离线波形计算的结果，计算所用参数为当前的参数。对于一个合适的 CFD 参数设置，该CFD分布该是平均分布的。 

<!-- OFFLINE_FFCFDThre.md ends here -->
