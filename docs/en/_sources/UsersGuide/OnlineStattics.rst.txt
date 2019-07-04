.. OnlineStattics.rst --- 
.. 
.. Description: 
.. Author: Hongyi Wu(吴鸿毅)
.. Email: wuhongyi@qq.com 
.. Created: 三 7月  3 13:52:44 2019 (+0800)
.. Last-Updated: 三 7月  3 17:24:48 2019 (+0800)
..           By: Hongyi Wu(吴鸿毅)
..     Update #: 2
.. URL: http://wuhongyi.cn 

=================================   
Online Stattics
=================================

---------------------------------
Interface
---------------------------------

Modify the file **PixieOnline.config** in **OnlineStattics**, where the first line is the original binary file storage path and the second line is the file name. The two-line parameters is file name. There are used to monitor the real-time size and disk usage of each file.

Open the online monitoring main interface by executing the following command:

.. code:: bash

  ./online
	  
Check the binary file path and file name for any problems. If there is no problem, click the button **Complete**, then click **RunStart** to enable online monitoring, and online monitoring will be refreshed every 3 seconds. The trigger rate of each channel and the actual event output rate of each channel can be monitored in real time.

The monitoring interface is as follows:

.. image:: /_static/img/OnlineMainFrame.png

.. 
.. OnlineStattics.rst ends here
