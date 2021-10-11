.. MYSQL.rst --- 
.. 
.. Description: 
.. Author: Hongyi Wu(吴鸿毅)
.. Email: wuhongyi@qq.com 
.. Created: 日 2月  9 14:20:45 2020 (+0800)
.. Last-Updated: 五 9月 17 22:09:32 2021 (+0800)
..           By: Hongyi Wu(吴鸿毅)
..     Update #: 17
.. URL: http://wuhongyi.cn 

##################################################
MYSQL
##################################################

============================================================
MYSQL安装步骤
============================================================

----------------------------------------------------------------------
CentOS 7
----------------------------------------------------------------------

.. code:: bash

   # CentOS 7
   yum remove akonadi-mysql.x86_64
    	  
   #MYSQL 依赖 libaio，所以先要安装 libaio
   yum search libaio  # 检索相关信息
   yum install libaio # 安装依赖包
    
   #检查 MySQL 是否已安装
   yum list installed | grep mysql


.. code:: bash
	  
   #下载 MySQL Yum Repository
   wget http://dev.mysql.com/get/mysql-community-release-el7-5.noarch.rpm
    
   #添加 MySQL Yum Repository
   yum localinstall mysql-community-release-el7-5.noarch.rpm
    
   #验证是否添加成功
   yum repolist enabled | grep "mysql.*-community.*"
    
   ######选择要启用 MySQL 版本
    
   #查看 MySQL 版本，执行
   yum repolist all | grep mysql
   #可以看到 5.5， 5.7 版本是默认禁用的，因为现在最新的稳定版是 5.6
    
   #可以通过类似下面的语句来启动某些版本
   yum-config-manager --disable mysql56-community
   yum-config-manager --enable mysql57-community-dmr
    
   #查看当前启动的 MySQL 版本
   yum repolist enabled | grep mysql
    
   #通过 Yum 来安装 MySQL
   yum install mysql-community-server
    
   #查看 MySQL 的安装目录
   whereis mysql



**启动和关闭 MySQL Server**

.. code:: bash

   #启动 MySQL Server
   systemctl start  mysqld
    
   #查看 MySQL Server 状态
   systemctl status  mysqld
    
   #关闭 MySQL Server，如果需要的时候
   systemctl stop mysqld


**开机自启动**

.. code:: bash

   #查看 MySQL 服务是否开机启动
   systemctl is-enabled mysql.service;echo $?
    
   #如果是 enabled 则说明是开机自动，如果不是，执行
   chkconfig --levels 235 mysqld on
   

**防火墙设置 (firewall-cmd)**

远程访问 MySQL， 需开放默认端口号 3306。

.. code:: bash

   #开启防火墙，没有任何提示即开启成功
   systemctl start firewalld
	  
   #执行开放了相应的端口	  
   firewall-cmd --permanent --zone=public --add-port=3306/tcp
   firewall-cmd --permanent --zone=public --add-port=3306/udp
       
   #执行使最新的防火墙设置规则生效
   firewall-cmd --reload 





----------------------------------------------------------------------
Ubuntu 18.04LTS
----------------------------------------------------------------------

.. code:: bash

   #安装默认的 MySQL 软件包
   apt install mysql-server
   apt install libmysqlclient-dev
   #上面的命令将安装 MySQL 服务和所有必需的依赖项。安装完成后，MySQL 服务将自动启动。

   #验证 MySQL 服务是否正在运行
   service mysql status

   vi /etc/mysql/mysql.conf.d/mysqld.cnf
   #查找下面的行，并使用 # 字符将其注释掉：
   # bind-address = 127.0.0.1
   #保存文件并退出，然后像下面这样重新启动mysql服务：
   service mysql restart


MySQL 作为系统服务安装。这意味着您可以在 Ubuntu上 使用 **systemctl** 或 **service** 等标准系统命令进行管理。


.. code:: bash

   #To start the MySQL service
   service mysql start
    
   #To check the current status of the MySQL service
   service mysql status
    
   #Stop the service anytime by typing:
   service mysql stop
    
   #If you want to restart the service
   service mysql restart
    
   #To reload the MySQL service
   service mysql reload
    
   #By default, MySQL is configured to start automatically when the server starts. You can disable the automatic startup by typing:
   systemctl disable mysql
    
   #If you want to enable the automatic startup again, just type:
   systemctl enable mysql




============================================================
MySQL 基本设置
============================================================

**MySQL 安全设置**

在安装过程中，安装程序不会提示您选择 root 用户的密码或确认其它任何配置更改。由于这会使 MySQL 安装不安全，因此您需要手动运行随附的安全脚本来设置密码并更改其它安全默认值。

.. code:: bash

   #在终端中执行
   mysql_secure_installation

此时输入 root 原始密码（初始化安装的话为空），接下来，为了安全，MySQL 会提示你重置 root 密码，移除其他用户账号，禁用 root 远程登录，移除 test 数据库，重新加载 privilege 表格等，你只需输入 **y** 继续执行即可。至此，整个 MySQL 安装完成
... Remove anonymous users?  Disallow root login remotely? Remove test database and access to it? Reload privilege tables now? 



**创建指定用户** 

.. code:: bash

   #在终端中执行
   mysql -u root -p
   #然后输入之前设置的密码
   
.. code:: mysql

   #创建一个普通用户 data ，密码是 xia17pxn
   CREATE USER 'data'@'%' IDENTIFIED BY 'xia17pxn';
    
   #给这个用户授予 SELECT,INSERT,UPDATE,DELETE 的远程访问的权限，这个账号一般用于提供给实施的系统访问
   GRANT SELECT,INSERT,UPDATE,DELETE  ON *.* TO 'data'@'%';
    
   #创建一个管理员用户 admin 账号 ，密码是 xia17pxn
   CREATE USER 'admin'@'%' IDENTIFIED BY 'xia17pxn';
    
   #给这个用户授予所有的远程访问的权限。这个用户主要用于管理整个数据库、备份、还原等操作。
   GRANT ALL  ON *.* TO 'admin'@'%';
    
   #使授权立刻生效
   flush privileges;

   #退出
   exit


   

   
============================================================
数据库基本操作命令
============================================================

.. code:: bash

   mysql  -h 127.0.0.1 -u admin -p
   mysql  -h 127.0.0.1 -u data -p
   #这里的 127.0.0.1 表示查看本机,如果从其它电脑远程查询,则更改为相应电脑的 IP,然后输入密码


现在数据库名称为 GDDAQ，数据表 statistics 存储在线监视的数据，数据表 fileinfo 存储原始文件大小信息。

.. code:: mysql

   # 查看所有数据库
   SHOW DATABASES;
    
   #进入我们要操作的数据库
   use GDDAQ;

   #展示数据表中的元素
   desc statistics;
   
   
.. 
.. MYSQL.rst ends here
