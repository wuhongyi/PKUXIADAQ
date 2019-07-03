.. OnlineStattics.rst --- 
.. 
.. Description: 
.. Author: Hongyi Wu(吴鸿毅)
.. Email: wuhongyi@qq.com 
.. Created: 三 7月  3 13:52:44 2019 (+0800)
.. Last-Updated: 三 7月  3 13:54:16 2019 (+0800)
..           By: Hongyi Wu(吴鸿毅)
..     Update #: 1
.. URL: http://wuhongyi.cn 

=================================   
在线统计
=================================

---------------------------------
控制界面
---------------------------------

修改 **OnlineStattics** 中的文件 **PixieOnline.config**，其中第一行为原始二进制文件存放路径，第二行为文件名。通过该两行参数来监视每个文件时时大小及硬盘占用量。


通过执行以下命令，开启在线监视主界面：  

.. code:: bash

  ./online
	  
检查二进制文件路径、文件名是否有问题，如果没问题则点击按钮 **Complete**，之后点击 **RunStart** 则开启在线监视，在线监视每 3 秒刷新一次。可时时监视每路的触发率、每路的实际事件输出率。


监视界面如下：

.. image:: /_static/img/OnlineMainFrame.png

.. 
.. OnlineStattics.rst ends here
