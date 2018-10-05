<!-- INSTALL.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 3月 25 20:23:51 2018 (+0800)
;; Last-Updated: 五 10月  5 08:14:09 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 14
;; URL: http://wuhongyi.cn -->

# Installation of Software/程序安装

<!-- toc -->

Installation for this software is requrired by (本程序安装要求)

- CERN ROOT 6
	- GCC >= 4.8
- FFTW3

本程序测试过的系统包括Scientific Linux 7.2/7.3/7.4




## The steps for Installation/软件安装步骤

- Delete the old version of the PKUXIADAQ folder in your personal directory
- Extract this package into your personal directory ($HOME)
- Set up environment variables
- Compile Plx9054 driver
- Compile pixie16 driver API (this API has been modified by Wu Hongyi, driven by unofficial standards)
- Compile graphical acquisition software
- Compile online monitor program
- Compile data converter program
- Compile event reconstruction program (optional)

- 删除个人目录下的老版本PKUXIADAQ文件夹
- 将本程序包解压缩到个人目录中($HOME)
- 设置环境变量
- 编译Plx9054驱动
- 编译pixie16驱动API(该API被吴鸿毅修改过，非官方标准驱动)
- 编译图形化获取软件
- 编译在线监视程序
- 编译数据转换程序
- 编译事件重构程序(可选)


```bash
## Set up environment variables
##设置环境变量

# Add the follwing content into .bashrc file
#在 .bashrc 文件中添加
export PLX_SDK_DIR=$HOME/PKUXIADAQ/PlxSdk

# put PKUXIADAQ.tar.gz(or PKUXIADAQ-master.tar.gz) in the personal directory /home, the position ~/ 
# 将 PKUXIADAQ.tar.gz(或者PKUXIADAQ-master.tar.gz) 放置到 /home 下的个人目录下，即 ~/ 位置
tar -zxvf PKUXIADAQ.tar.gz  #解压缩
or 或者
tar -zxvf PKUXIADAQ-master.tar.gz
mv PKUXIADAQ-master  PKUXIADAQ

# Acquire PKUXIADAQ directory
#得到 PKUXIADAQ 目录
```

```bash
## Compile Plx9054 driver
##编译Plx9054驱动

# Open a new terminal
#打开新终端
cd ~
cd PKUXIADAQ/
#Delete the undeleted driver that may exist. If there is no such directory, you do not need to execute the command.
rm -rf PlxSdk #删除可能存在的未删除驱动，如果没有该目录则不用执行该行命令
tar -zxvf PlxSdk.tar.gz
cd PlxSdk/PlxApi/
make clean
make 
# If it succeeds,you will see Library "Library/PlxApi.a" built successfully
#成功后你将会看到 Library "Library/PlxApi.a" built successfully

cd ../Samples/ApiTest/
make clean
make
#if it succeeds,you will see Application "App/ApiTest" built successfully
#成功后你将会看到 Application "App/ApiTest" built successfully

cd ../../Driver/
./builddriver 9054

# If it succeeds,you will see Driver "Plx9054/Plx9054.ko" built sucessfully
#成功后你将会看到 Driver "Plx9054/Plx9054.ko" built sucessfully
```


```bash
## Compile pixie16
##编译pixie16

cd ~
cd PKUXIADAQ/software/
make clean
make 

# As long as no error is reported, the libPixie16App.a libPixie16Sys.a will be generated in the folder
#只要没报错，并且该文件夹内生成libPixie16App.a libPixie16Sys.a
```


```bash
## Compile graphical acquisition software
##编译图形化获取软件

# Modify settings parameters
#修改设置参数
cd ~
cd PKUXIADAQ/parset/


# Modify cfgPixie16.txt file。
# The value after CrateID indicates the chassis number, and the value is allowed to be 0-15. If there is only a chassis, the parameter is set freely (usually the default 0 is used). If multiple chassis are running synchronously, make sure that the number of each chassis is set to a different value.
# SettingPars Following is the parameter setting file and write the parameter configuration file to be used.
# ModuleSlot The first value number indicates the number of plugins, and if there are 3 plugins, it is 3. The following numbers are for each plug-in in the slot position of the chassis (the slot position is counted from 2), and there are three plugins followed by 2 3 4 respectively.
# Parameter ModuleSampingRate and ModuleBits only take effect in offline mode. When the main interface is initialized in Offline mode, this parameter is read.
#修改cfgPixie16.txt文件。
#其中CrateID 后面的数值表示机箱编号，该值允许0-15。如果单机箱则随意设置(一般就采用默认的0)，如果多个机箱同步运行务必让每个机箱的该编号设置为不同的数值。
#SettingPars 后面为参数设置文件，写入要采用的参数配置文件即可。
#ModuleSlot 后面第一个数值表示插件个数，如果有3个插件则为3。之后的数字未为每个插件在机箱的插槽位置（插槽位置从2开始计数），有三个插件则之后分别为2 3 4。
#参数 ModuleSampingRate与ModuleBits 只对离线模式生效，当主界面采用Offline模式初始化时则读取该参数。

# Modify the Run.config file, the first line in the file is the original data storage path, and the second is the file name.
# Modify the RunNumber file, the value in this file is the run number of the actual run.
#修改Run.config文件，该文件中第一行为原始数据存放路径，第二行为文件名。
#修改RunNumber文件，该文件中的数值为运行的run number。

cd ~
cd PKUXIADAQ/GUI/
make clean
make 
```


```bash
## Compile online monitor program
##编译在线监视程序

cd ~
cd PKUXIADAQ/OnlineStattics/

# modify the parameters in the PixieOnline.config file
# the content of the first line is the storage path of acquired data files.
# the content of the second line is the names of acquired data files.
#修改 PixieOnline.config 文件中的参数
#第一行为获取数据文件存放路径
#第二行为获取文件名

make clean
make 
```

```bash
## Compile data converter program
## 编译数据转换程序

cd ~
cd PKUXIADAQ/Decode/

# Modify UserDefine.hh according to the instructions in the program.
#修改 UserDefine.hh，按照程序中的说明修改即可

make clean
make 
```


```bash
## Compile event reconstruction program
## 编译事件重构程序

cd ~
cd PKUXIADAQ/MakeEvent/

# Modify UserDefine.hh according to the instructions in the program.
#修改 UserDefine.hh，按照程序中的说明修改即可

make clean
make
```


## Instruction for use/程序使用说明

-  Restart the computer after booting the chassis (the computer must be open later than the chassis)
-  Load Plx9054 driver under ROOT permission after opening the chassis
-  Normal acquisition

- 开机机箱后重启电脑(电脑必须晚于机箱开启)
- 开启机箱后ROOT权限下加载Plx9054驱动
- 正常获取


```bash
## Load Plx9054 driver under ROOT permission
## ROOT权限下加载Plx9054驱动

cd ~
cd PKUXIADAQ/PlxSdk/Bin/
su #input ROOT password   输入ROOT密码
./Plx_load 9054
You Will see a prompt to load successfully
#将会看到加载成功的提示
exit  #Exit ROOT permission  退出ROOT权限
```

----

```bash
## Start the graphical interface program
##启动图形界面程序

cd ~
cd ~/PKUXIADAQ/GUI
./pku

# The graphical interface will pop up.
# You can choose Online/Offline Mode then press Boot to initialize.
# After waiting for initialization, you can modify the output data file path, file name, and run number. Press the Complete button to confirm.
# The LSRunStart button becomes operational at this time. You can start pressing Start and then press Stop for the second time.
# Online Statistics option selections mean sending online statistics
# Update Energy Monitor: Each time you select it, the energy spectrum information is read from the plug-in and sent to the online program (frequent selection will affect the acquisition)
#将会弹出图形化界面
#可选择 Online/Offline Mode 然后按 Boot 初始化
#等待初始化成功后，可修改输出数据文件路径，文件名，run number。按 Complete 按钮确认。
#此时 LSRunStart 按钮变为可操作。即可开始按Start，之后第二次按即为Stop。
#Online Statistics选项选择表示发送在线统计
#Update Energy Monitor每选择一次则从插件内部读取一次能谱信息并发送给在线程序（频繁选择会影响获取）
```


```bash
## Start online monitor program
##启动在线监视程序

cd ~
cd PKUXIADAQ/OnlineStattics/
./online

# The graphical interface will pop up.
# Check the path to the original data folder above and the file name is correct. Confirm by pressing Complete.
# Press RunStart to start monitoring and update the input rate and output rate of each channel every 3 seconds. (The first time you enable the program after opening the chassis, you need to enable it after the acquisition is turned on)
# In the lower right corner of the monitoring interface, there is monitoring of the amount of hard disk usage for writing data.
# EnergyMonitor page is used to view the spectrum. Due to the internal register size limitations of the plug-in, this energy spectrum differs from the actual spectrum in channel range.

#将会弹出图形化界面
#查看上面的原始数据文件夹路径、文件名是否正确。按 Complete 确认。
#按 RunStart开始启动监视，每3秒更新一次每路的输入率、输出率。(开启机箱后第一次启用该程序需要在获取开启之后)
#监视界面右下角有对写入硬盘使用量的监视。

#EnergyMonitor页面用来查看能谱。由于插件内部寄存器大小限制，该能谱与实际能谱道址范围存在差别。
```


```bash
## execute data converter program
##执行数据转换程序

cd ~
cd PKUXIADAQ/Decode/

# After the last run of acquisition, we can convert the previous run of data to ROOT file.
#在上一轮获取结束之后，我们便可将上一轮数据转为ROOT文件
./decode xxx
#xxx indicates Run Number
```


<!-- INSTALL.md ends here -->
