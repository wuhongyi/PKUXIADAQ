.. INSTALLQT.rst --- 
.. 
.. Description: 
.. Author: Hongyi Wu(吴鸿毅)
.. Email: wuhongyi@qq.com 
.. Created: 五 11月  8 23:26:13 2024 (+0800)
.. Last-Updated: 日 5月 11 22:41:58 2025 (+0800)
..           By: Hongyi Wu(吴鸿毅)
..     Update #: 3
.. URL: http://wuhongyi.cn 

=================================   
Qt 程序安装
=================================   

本程序安装要求依赖 OpenSSL、Qt5、ROOT6 等第三方库。以下版本经过测试。

* OS
    - Ubuntu 20.04
    - CentOS 8

**本程序测试过的系统包含 Ubuntu20.04、CentOS8。 支持各种 LINUX 操作系统编译。如果您的操作系统编译不通过请联系吴鸿毅。**



.. code-block:: bash

  # Ubuntu20.04 依赖安装
  sudo apt -y install libssl-dev openssl qt5-default qtcreator libqt5charts5-dev
  # CentOS 8  依赖安装
  sudo dnf -y install openssl openssl-devel qt5-qtcharts qt5-qtcharts-devel qt5-qtbase qt5-qtbase-devel
  # ROOT 6 推荐 6.26.16

  
---------------------------------
软件安装步骤
---------------------------------


.. code:: bash
	  
  tar -zxvf PixieSDK-5.0.0.tar.gz
  cd PixieSDK-5.0.0
  mkdir build
  cd build
  cmake  -DCMAKE_INSTALL_PREFIX=../../pixiesdk ..
  make
  make install


**编译 GUI_Qt 软件**

.. code-block:: bash
  
  cd GUI_Qt
  chmod +x makefile.sh.x
  ./makefile.sh.x
  # 等待编译结束，编译通过之后，文件夹内将生成可执行文件 gddaq
  # 检查是否生成可执行文件 gddaq, 如有则编译成功。如果没有，请联系吴鸿毅。



  
   
.. 
.. INSTALLQT.rst ends here
