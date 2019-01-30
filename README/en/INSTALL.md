<!-- INSTALL.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 3月 25 20:23:51 2018 (+0800)
;; Last-Updated: 三 1月 30 20:05:39 2019 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 17
;; URL: http://wuhongyi.cn -->

# Installation of Software

<!-- toc -->

Installation for this software is requrired by

- CERN ROOT 6
	- GCC >= 4.8
- FFTW3

The operating system tested by this program includes Scientific Linux 7.2/7.3/7.4

> **[danger] For danger**
>
> Graphical interface programs and non-graphical interface programs cannot run at the same time!

## The steps for Installation

- Delete the old version of the PKUXIADAQ folder in your personal directory
- Extract this package into your personal directory ($HOME)
- Set up environment variables
- Compile Plx9054 driver
- Compile pixie16 driver API (this API has been modified by Wu Hongyi, driven by unofficial standards)
- Compile graphical acquisition software
- Compile non-graphical acquisition software
- Compile online monitor program
- Compile data converter program
- Compile event reconstruction program (optional)



```bash
## Set up environment variables


# Add the follwing content into .bashrc file
export PLX_SDK_DIR=$HOME/PKUXIADAQ/PlxSdk

# put PKUXIADAQ.tar.gz(or PKUXIADAQ-master.tar.gz) in the personal directory /home, the position ~/ 

tar -zxvf PKUXIADAQ.tar.gz  
or 
tar -zxvf PKUXIADAQ-master.tar.gz
mv PKUXIADAQ-master  PKUXIADAQ

# Acquire PKUXIADAQ directory
```

```bash
## Compile Plx9054 driver

# Open a new terminal
cd ~
cd PKUXIADAQ/
#Delete the undeleted driver that may exist. If there is no such directory, you do not need to execute the command.
rm -rf PlxSdk
tar -zxvf PlxSdk.tar.gz
cd PlxSdk/PlxApi/
make clean
make 
# If it succeeds,you will see Library "Library/PlxApi.a" built successfully


cd ../Samples/ApiTest/
make clean
make
#if it succeeds,you will see Application "App/ApiTest" built successfully


cd ../../Driver/
./builddriver 9054

# If it succeeds,you will see Driver "Plx9054/Plx9054.ko" built sucessfully
```


```bash
## Compile pixie16

cd ~
cd PKUXIADAQ/software/
make clean
make 

# As long as no error is reported, the libPixie16App.a libPixie16Sys.a will be generated in the folder
```

----

```bash
# Modify settings parameters
cd ~
cd PKUXIADAQ/parset/


# Modify cfgPixie16.txt file。
# The value after CrateID indicates the chassis number, and the value is allowed to be 0-15. If there is only a chassis, the parameter is set freely (usually the default 0 is used). If multiple chassis are running synchronously, make sure that the number of each chassis is set to a different value.
# SettingPars Following is the parameter setting file and write the parameter configuration file to be used.
# ModuleSlot The first value number indicates the number of plugins, and if there are 3 plugins, it is 3. The following numbers are for each plug-in in the slot position of the chassis (the slot position is counted from 2), and there are three plugins followed by 2 3 4 respectively.
#AutoRunModeTimes The following values are the time for automatic switching in automatic operation mode.
# Parameter ModuleSampingRate and ModuleBits only take effect in offline mode. When the main interface is initialized in Offline mode, this parameter is read.


# Modify the Run.config file, the first line in the file is the original data storage path, and the second is the file name.
# Modify the RunNumber file, the value in this file is the run number of the actual run.
```

```bash
## Compile graphical acquisition software

cd ~
cd PKUXIADAQ/GUI/
make clean
make 
```

```bash
## Compile non-graphical acquisition software

cd ~
cd PKUXIADAQ/NOGUI/
make clean
make 
```

```bash
## Compile online monitor program

cd ~
cd PKUXIADAQ/OnlineStattics/

# modify the parameters in the PixieOnline.config file
# the content of the first line is the storage path of acquired data files.
# the content of the second line is the names of acquired data files.

make clean
make 
```

```bash
## Compile data converter program

cd ~
cd PKUXIADAQ/Decode/

# Modify UserDefine.hh according to the instructions in the program.

make clean
make 
```


```bash
## Compile event reconstruction program

cd ~
cd PKUXIADAQ/MakeEvent/

# Modify UserDefine.hh according to the instructions in the program.

make clean
make
```


## Instruction for use

-  Restart the computer after booting the chassis (the computer must be open later than the chassis)
-  Load Plx9054 driver under ROOT permission after opening the chassis
-  Normal acquisition


```bash
## Load Plx9054 driver under ROOT permission


cd ~
cd PKUXIADAQ/PlxSdk/Bin/
su #input ROOT password   
./Plx_load 9054

# You Will see a prompt to load successfully

exit  #Exit ROOT permission  退出ROOT权限
```



----



```bash
## Start the graphical interface program

cd ~
cd ~/PKUXIADAQ/GUI
./pku

# The graphical interface will pop up.
# You can choose Online/Offline Mode then press Boot to initialize.
# After waiting for initialization, you can modify the output data file path, file name, and run number. Press the Complete button to confirm.
# The LSRunStart button becomes operational at this time. You can start pressing Start and then press Stop for the second time.
# Online Statistics option selections mean sending online statistics
# Update Energy Monitor: Each time you select it, the energy spectrum information is read from the plug-in and sent to the online program (frequent select
```


```bash
## Start the non-graphical interface program

cd ~
cd ~/PKUXIADAQ/NOGUI
./pku
```


```bash
## Start online monitor program


cd ~
cd PKUXIADAQ/OnlineStattics/
./online

# The graphical interface will pop up.
# Check the path to the original data folder above and the file name is correct. Confirm by pressing Complete.
# Press RunStart to start monitoring and update the input rate and output rate of each channel every 3 seconds. (The first time you enable the program after opening the chassis, you need to enable it after the acquisition is turned on)
# In the lower right corner of the monitoring interface, there is monitoring of the amount of hard disk usage for writing data.



# EnergyMonitor page is used to view the spectrum. Due to the internal register size limitations of the plug-in, this energy spectrum differs from the actual spectrum in channel range.

```


```bash
## execute data converter program


cd ~
cd PKUXIADAQ/Decode/

# After the last run of acquisition, we can convert the previous run of data to ROOT file.

./decode xxx
#xxx indicates Run Number
```


<!-- INSTALL.md ends here -->
