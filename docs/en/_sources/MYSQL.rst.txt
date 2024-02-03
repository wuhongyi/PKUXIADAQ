.. MYSQL.rst --- 
.. 
.. Description: 
.. Author: Hongyi Wu(吴鸿毅)
.. Email: wuhongyi@qq.com 
.. Created: 二 2月 11 13:18:52 2020 (+0800)
.. Last-Updated: 二 2月 11 23:13:54 2020 (+0800)
..           By: Hongyi Wu(吴鸿毅)
..     Update #: 9
.. URL: http://wuhongyi.cn 

##################################################
MYSQL
##################################################

============================================================
MYSQL installation steps
============================================================

----------------------------------------------------------------------
CentOS 7
----------------------------------------------------------------------

.. code:: bash

   # CentOS 7
   yum remove akonadi-mysql.x86_64
    	  
   #MySQL depends on libaio, so first install libaio
   yum search libaio  
   yum install libaio
    
   #Check if MySQL is installed
   yum list installed | grep mysql


.. code:: bash
	  
   #Download MySQL Yum Repository
   wget http://dev.mysql.com/get/mysql-community-release-el7-5.noarch.rpm
    
   #Add MySQL Yum Repository
   yum localinstall mysql-community-release-el7-5.noarch.rpm
    
   #Verify that it was added successfully
   yum repolist enabled | grep "mysql.*-community.*"
    
   ######Select the MySQL version you want to enable
    
   #View MySQL version, execute
   yum repolist all | grep mysql
   #You can see 5.5, 5.7 is disabled by default, because the latest stable version is 5.6
    
   #Some versions can be started with statements like
   yum-config-manager --disable mysql56-community
   yum-config-manager --enable mysql57-community-dmr
    
   #View the currently enabled MySQL version
   yum repolist enabled | grep mysql
    
   #Install MySQL via Yum
   yum install mysql-community-server
    
   #View the MySQL installation directory
   whereis mysql


**Start and stop MySQL Server**

.. code:: bash

   #start MySQL Server
   systemctl start  mysqld
    
   #check MySQL Server status
   systemctl status  mysqld
    
   #stop MySQL Server, If needed
   systemctl stop mysqld


**Self-starting**

.. code:: bash

   #Check if MySQL service starts automatically
   systemctl is-enabled mysql.service;echo $?
    
   #If it is enabled, it means it is on automatically, if not, execute
   chkconfig --levels 235 mysqld on
   

**Firewall settings (firewall-cmd)**

To access MySQL remotely, open the default port 3306.

.. code:: bash

   #Turn on the firewall, it will open successfully without any prompts
   systemctl start firewalld
	  	  
   #Open the corresponding port	  
   firewall-cmd --permanent --zone=public --add-port=3306/tcp
   firewall-cmd --permanent --zone=public --add-port=3306/udp
       
   #Make the latest firewall settings rules take effect
   firewall-cmd --reload 




----------------------------------------------------------------------
Ubuntu 18.04LTS
----------------------------------------------------------------------

.. code:: bash

   #install the default MySQL package
   apt install mysql-server
   apt install libmysqlclient-dev
   #The above command will install the MySQL server and all required dependencies. Once the installation is completed, the MySQL service will start automatically. 

   #verify whether the MySQL service is running
   service mysql status

   vi /etc/mysql/mysql.conf.d/mysqld.cnf
   #Look for the line below and comment it out using the # character:
   # bind-address = 127.0.0.1 
   #Save the file and exit, afterwards restart the mysql service like so:
   service mysql restart



MySQL is installed as a systemd service. It means that you can manage it using standard systemd commands like **systemctl** or **service** on Ubuntu.

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
MySQL Basic Settings
============================================================

**MySQL security settings**

During the installation, the installer will not prompt you to choose a password for the root user or confirm any other configuration changes. Since this leaves the MySQL installation insecure, you need to manually run the included security script to set a password and change other security defaults.

.. code:: bash

   #Execute in terminal
   mysql_secure_installation

First, enter MySQL the root user password (it is empty for initial installation). Next, for security, MySQL will prompt you to reset the root password, remove other user accounts, disable root remote login, remove the test database, and reload the privilege table , You just enter **y** to continue execution. At this point, the entire MySQL installation is complete.


**Create specified user**

.. code:: bash

   #Execute in terminal
   mysql -u root -p
   #Then enter the password you set previously
   
.. code:: mysql

   #Create a normal user 'data' with a password of 'xia17pxn'
   CREATE USER 'data'@'%' IDENTIFIED BY 'xia17pxn';
    
   #Give this user remote access permissions of SELECT, INSERT, UPDATE, DELETE. This account is generally used to provide system access for implementation.
   GRANT SELECT,INSERT,UPDATE,DELETE  ON *.* TO 'data'@'%';
    
   #Create an administrator user 'admin' with password 'xia17pxn'
   CREATE USER 'admin'@'%' IDENTIFIED BY 'xia17pxn';
    
   #Grant this user all remote access permissions. This user is mainly used to manage the entire database, backup, restore and other operations.
   GRANT ALL  ON *.* TO 'admin'@'%';
    
   #Make the authorization effective immediately
   flush privileges;

   #Exit
   exit


============================================================
Basic database operation commands
============================================================

.. code:: bash

   mysql  -h 127.0.0.1 -u admin -p
   mysql  -h 127.0.0.1 -u data -p
   #Here 127.0.0.1 means to view the machine, if remotely query from other computers, change to the IP of the corresponding computer, and then enter the password



   

   
.. 
.. MYSQL.rst ends here
