.. OnlineStattics.rst --- 
.. 
.. Description: 
.. Author: Hongyi Wu(吴鸿毅)
.. Email: wuhongyi@qq.com 
.. Created: 三 7月  3 13:52:44 2019 (+0800)
.. Last-Updated: 四 2月 13 00:59:39 2020 (+0800)
..           By: Hongyi Wu(吴鸿毅)
..     Update #: 6
.. URL: http://wuhongyi.cn 

##################################################
在线统计
##################################################

============================================================
基于 ROOT 的图形界面
============================================================

通过执行以下命令，开启在线监视主界面：  

.. code:: bash

  ./online
	  
点击 **RunStart** 则开启在线监视，在线监视每 3 秒刷新一次。可时时监视每路的触发率、每路的实际事件输出率。


监视界面如下：

.. image:: /_static/img/OnlineMainFrame.png


在 “Alert” 界面用户可以设置每个通道触发率的上限以及下限，当触发率低于设置的下限或者高于设置的上限，则该通道的数值将会变成蓝色和红色。

在“EnergyMonitor”界面用户可以浏览每个通道的能谱。

============================================================
基于网页的图形界面
============================================================
	   
**使用本程序的前提是安装 MYSQL 数据库。关于如何安装及设置数据库，请查看相对应的章节。**

.. image:: /_static/img/webOnlineMainFrame.png

在 mysql 目录下有两个子目录，分别为 createdb 和 online。

**createdb**

修改 *mysql/createdb/UserDefine.hh* 文件中的数据库参数，该信息包括数据库的 IP，拥有创建数据库的用户名和密码。

.. code:: cpp

   #define HOST_NAME    "222.29.111.140"
   #define USER_NAME    "admin"
   #define USER_PASSWD  "xia17pxn"

然后运行以下命令编译程序：

.. code:: bash

   cd mysql/createdb
   make clean
   make 

编译完成将生成可执行文件 *createdb* ，执行该程序，将创建数据库以及相应的表。**需要注意的是，每次执行该程序，将会删除原有的数据库然后重新创建。**

**online**

修改 *mysql/online/UserDefine.hh* 文件中的数据库参数，该信息包括数据库的 IP，拥有能够对数据库进行写入、更新权限的用户名和密码。

.. code:: cpp
	  
   #define HOST_NAME    "222.29.111.140"
   #define USER_NAME    "data"
   #define USER_PASSWD  "xia17pxn"

然后运行以下命令编译程序：

.. code:: bash

   cd mysql/online
   make clean
   make 

编译完成将生成可执行文件 *onlinedb* ，执行该程序，将周期性读取共享内存中的数据，并写入到数据库中。**本程序需要始终运行。**


.. 
.. OnlineStattics.rst ends here
