<!-- backup.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 5月 13 15:49:27 2018 (+0800)
;; Last-Updated: 日 5月 13 15:49:31 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 1
;; URL: http://wuhongyi.cn -->

环境设置
```bash
export PLX_SDK_DIR=$HOME/XIADAQ/PlxSdk
```


Install the driver

The PLX software should work on all Linux platform, primarily Fedora. Make sure your system is Little Endian.

```
[lijing@localhost Samples]$ lscpu 
Architecture:          x86_64
CPU op-mode(s):        32-bit, 64-bit
Byte Order:            Little Endian
CPU(s):                8
....
```

The environment variable PLX_SDK_DIR should be set to the root location of the PlxSdk folder.

```bash
 export PLX_SDK_DIR=$HOME/PlxSdk
```

Then compile the source code

```bash
cd PlxSdk/PlxApi
make
cd PlxSdk/Samples/ApiTest
make // verify the API works
```

Our PCI card named Plx9054, so go to the folder Driver

```bash
cd PlxSdk/Driver 
./builddriver 9054
```

Load the driver

```
cd PlxSdk/Bin
sh Plx_load 9054 // need super user permission 
sh Plx_load 9054 d // debug mode, show more information
ls -l /dev/plx 
total 0
crwxrwxrwx. 1 root root 246, 255 Jan 12 12:02 Plx9054
crwxrwxrwx. 1 root root 246,   0 Jan 12 12:02 Plx9054-0
crwxrwxrwx. 1 root root 246,   1 Jan 12 12:02 Plx9054-1
crwxrwxrwx. 1 root root 246,  10 Jan 12 12:02 Plx9054-10
crwxrwxrwx. 1 root root 246,  11 Jan 12 12:02 Plx9054-11
crwxrwxrwx. 1 root root 246,  12 Jan 12 12:02 Plx9054-12
crwxrwxrwx. 1 root root 246,  13 Jan 12 12:02 Plx9054-13
crwxrwxrwx. 1 root root 246,  14 Jan 12 12:02 Plx9054-14
crwxrwxrwx. 1 root root 246,  15 Jan 12 12:02 Plx9054-15
crwxrwxrwx. 1 root root 246,  16 Jan 12 12:02 Plx9054-16
crwxrwxrwx. 1 root root 246,   2 Jan 12 12:02 Plx9054-2
crwxrwxrwx. 1 root root 246,   3 Jan 12 12:02 Plx9054-3
crwxrwxrwx. 1 root root 246,   4 Jan 12 12:02 Plx9054-4
crwxrwxrwx. 1 root root 246,   5 Jan 12 12:02 Plx9054-5
crwxrwxrwx. 1 root root 246,   6 Jan 12 12:02 Plx9054-6
crwxrwxrwx. 1 root root 246,   7 Jan 12 12:02 Plx9054-7
crwxrwxrwx. 1 root root 246,   8 Jan 12 12:02 Plx9054-8
crwxrwxrwx. 1 root root 246,   9 Jan 12 12:02 Plx9054-9
```

monitor the Device traffic

```bash
cd PlxSdk/Samples/PlxCm
make
cd App
./Plxcm

    # Bs Sl Fn Pt Dev  Ven  Chip Rv I2C  Description
   ---------------------------------------------------------------------------
==> 0 06 0E 00 -- 9054 10B5 9054 AC  --  PLX PCI <==> Local Bus bridge
    1 06 0F 00 -- 9054 10B5 9054 AC  --  PLX PCI <==> Local Bus bridge

PLX Console Monitor, v2.60 [Jan 12 2016]
Copyright (c) PLX Technology, Inc.
```




----

说明书将包括这几部分：
软件的安装，插件的跳线的设置，软件的使用，软件参数的意义。

一级trigger，二级trigger




----

第一步：下官网下载PLX_SDK
     
第二步：解压，打开Documentation\PLX_Linux_Release_Notes.htm，查看具体的安装步骤和要求。

To install the Linux support files, use the following steps:
1.    Copy the PLX Linux TAR file to the desired working directory, e.g. the user’s home directory or/usr/linux/src.
2.    If the PLX file contains a .gz extension, the file must first be unzipped, as follows: gunzip PlxSdk.tar.gz
3.    Use a TAR file extractor or open a terminal window and type the following command: tar -xvf  PlxSdk.tar
4.    This will create a folder called PlxSdk with all PLX supplied files and folders inside.
5.    In order to build the files supplied, the shell environment variable PLX_SDK_DIR must be set.  This should be set to the root location of where thePlxSdk folder is created.  This can be set with the export command and may be placed in the shell initialization script (e.g. “.bashrc”) so that it is automatically set when a new terminal session is started.

   Examples:  export PLX_SDK_DIR=$HOME/PlxSdk
              export PLX_SDK_DIR=/opt/PlxSdk

配置PLX_SDK_DIR环境变量，并且导出。这里我导出的步骤是：

emacs /etc/enviroment 
```
export PLX_SDK_DIR=/opt/PlxSdk  在文件末尾输入这一行导出命令
export PLX_CPU_BITS=64 #配置CPU位数，如果是64位，就必须配置PLX_CPU_BITS环境变量。
```
 
第四步： 编译库函数和测试程序

                     PLX_SDK提供了一组API函数供开发者使用，同样的给出了一些测试程序用来测试API函数的正确性。下面编译：
                   编译API函数库:

                     #cd /opt/PlxSdk/PlxApi

                     跳转到PLX_SDK的安装目录

                    #make PLX_DEBUG=1

                    使用默认的makefile编译即可。加入了PLX_DEBUG参数，意思是编译调试版API函数。这里是为了后面调试驱动方便，调试驱动必须是编译有调试版的API函数。

                    不想调试也可以去掉PLX_DEBUG参数，直接make即可。

                    编译测试程序：

                    #cd /usr/src/PlxSdk/Linux/Samples/ApiTest

                    #make PLX_DEBUG=1

                     同样加入PLX_DEBUG参数，编译为调试版的测试程序。这样可以方便的使用gdb调试代码的执行流程以及是否有错误。

       第五步： 编译驱动程序

                   PLX_SDK中提供了自动编译的工具，即builddriver工具（其实是一个脚本，后面会分析具体代码）。利用builddriver工具可以方便的选择需要编译的驱动程序并进行编译。

                    #cd /usr/src/PlxSdk/Linux/Driver

                     #./builddriver 9054 d

                    9054指定了芯片号，d则是说明编译为可调试的驱动程序。



----

当前完成情况：实验前期获取参数的调节、实验中数据采集、离线数据转换。

- 参数调节：运行程序先获取部分波形 -> 保存数据文件 -> 离线文件调节参数 -> 保存参数文件
- 数据获取：运行程序（读取调节好的参数） -> 正常获取数据
- 数据转换：将原始数据转ROOT文件


当前时时在线。Trigger Rate监视通过不断I/O从机箱获取。

ROOT文件读取参数文件调用算法查看波形、能量、堆积等信息

----


# XIA 与 CAEN 的区别



# XIA 的灵活性及其引起的困难


## 从数据结构及FIFO讲起

CAEN 插件在采集波形时，一个插件所有通道每个事件的采样长度只能设置成相同的。
而 XIA 插件可以每个通道单独设置，例如：ch-0 设置采3000个点，ch-1 设置采2000个点，ch-2 设置采5000个点，ch-3 设置成不采集波形……。

在读取数据时候，CAEN 插件每次拿到的数据都是完整事件的数据。
而 XIA 插件 FIFO 的读指针、写指针是独立的、同时进行的，所以每次我们要取数据时候都需要先查询下当前 FIFO 有多少数据，然后再指定该次读取多少数据。因为FIFO读、写是独立的，因而查询到的数据量是该时刻拥有的数据量，这个数据量基本都不是完整事件的数据量（还有部分是当前事件正在写入的）。每个事件除了有 4 words 的 header 及后面紧跟着的数据外并没有事件标记符，当然这样设计最大的好处就是速度快。

## 灵活性带来的困难

基于以上事实，两个公司的产品对用户写时时监视获取率、在线监视程序等的实现方式存在较大差异。

CAEN：
由于 CAEN 一个插件上每个通道的采样数相同，因而他们每个事件的长度是一致的。在拿到数据时即可存文件或者 decode 出每个事件的信息用来统计时时获取速率，也可将数据发送到在线程序。

XIA：
由于其上每个通道数据长度可单独设置，这是 XIA 插件的灵活性。又 FIFO 查询到的数据量是当前时刻的数据量，这样每次取到的数据都不是完整事件的数据。

对于多个插件的系统，获取时候一直循环读取每一个插件上的数据，如果将所有插件的数据存在同一个文件内，将造成数据的错乱。如果每个插件的数据单独存在一个文件内，那么下一次读取的该插件的数据中开头的不完整事件的数据刚好接上一次最后一个不完整事件的数据，可解决这个问题。

由于每次拿到的不是完整数据，因此也不好 decode 当前数据来获得每个事件的信息（从开始就decode记录每次数据的不完整事件的情况也可解决该问题，但是这样decode需要额外的时间）。但是 XIA 内部 DSP上统计着信号输入量、获取接收量、活时等信息。可频繁调用函数读取 DSP 上的统计信息来监视时时获取速率。但是频繁调用该函数会对获取的 I/O 有一点影响。

由于每次拿到的不是完整数据，因此共享内存的在线监视也不容易实现，如若要实现，也不是没办法（从开始就decode记录每次数据的不完整事件的情况也可解决该问题，但是这样decode需要额外的时间）。

下面给出一些困难的解决思路：
牺牲XIA插件的灵活性，对一个插件设置成每个通道采集波形长度相同（这样就与CAEN一样了），因为每个通道的每个事件数据的长度相等，读取数据时候查询到当前的数据量之后，然后只读取事件长度整数倍的最大数据，这样每次拿到的都是完整的事件数据了，可参照CAEN的实现方式。

对采波形时候，可采用“伪”在线监视获取情况，即获取文件每隔一段时间保存一次，用上一轮的数据作为“在线”数据。






# 参数的设置


----

## 建议操作流程

- 先用放射源
- 初步调节参数
- 采集波形
- 离线调节参数







# 软件使用

基线调节有以下两种方式：

- 先到 **Baseline Setup**设置基线百分比，然后到**Analog Signal Setup**按自动调节偏置**AdjustOffset**
- 直接到**Analog Signal Setup**调节**DCOffset**

基线调节的本质就是调节偏置（DCOffset）


**Analog Signal Setup**中的*Gain、Sign*功能可由**CSRA Register**中的选项*TP、EIR*代替。

获取时候不可按**Read WF**，不可调节参数，否则获取将自动停止。

BLCut 可手动/自动调节。自动调节到**Analog Signal Setup**按自动调节偏置**AdjustOffset**。手动调节到**Baseline Setup**中的*BLcut*。



----

<!-- 以下参数等新版说明书再整理 -->

# User Parameters

以下参数为 Pixie-16 API 的变量。在大多数情况下，Pixie-16 API 将会调整输入参数到对应于一个有效DSP参数最接近的数值。

## System Parameters

- NUMBER_MODULES
	- 指定该系统中的插件数量，默认范围是 1 - 32。如若超过32个插件，则需要修改pixie16sys_defs.h定义的最大数量。
- OFFLINE_ANALYSIS
	- 1表示离线模式，0表示在线模式。
	- 可选参数：0、1
- C_LIBRARY_RELEASE
	- 不详
- C_LIBRARY_BUILD
	- 输出表示Pixle-16 API 的版本信息。
- PXI_SLOT_MAP
	- 用来指定每个插件在机箱的插槽编号。
	- 可选参数： 2 - 14

## Module Parameters(Input)

- MODULE_NUMBER
	- 在初始化的为每个插件分配的编号。该编号将写在输出的data header中。
	- 参数范围： 0 - N-1
- MODULE_CSRA
	- 控制寄存器，32bit。 <!-- GOTO -->
- MODULE_CSRB
	- 控制寄存器，32bit。 <!-- GOTO -->
- MODULE_FORMATE
	- 当前无用。
- RUN_TYPE
	- (0) - slow control run, 0x100 - list mode runs, 0x301 - MCA run
- COINCIDENCE_PATTERN
	- 32bit。位模式，用来指定所需通道之间的符合。
- SYNCH_WAIT
	- 用来控制不同插件是否同时开始获取数据。
	- 1 - 同时；0 - 不同时。
- IN_SYNCH
	- 用来控制是否做时间同步。
	- 0 - 同步；1 - 不同步。
- SLOW_FILTER_RANGE
	- <!-- GOTO -->
	- 参数范围：1 - 6
- FAST_FILTER_RANGE
	- <!-- GOTO -->
	- 指定参数：0
- HOST_RT_PRESET
	- 预设采集时间。仅适用于 MCA mode run。

## Module Parameters(Output)

- RUN_TIME
	- 数据获取时间，单位：s
- NUMBER_EVENTS
	- 上一轮获取事件数。
- BOARD_VERSION
	- 不详。
- SERIAL_NUMBER
	- 不详。


## Channel Parameter(Input) 

- CHANNEL_CSRA
	- 控制寄存器，32bit。 <!-- GOTO -->
- CHANNEL_CSRB
	- 控制寄存器，32bit。 <!-- GOTO -->
- CFD_THRESHOLD
	- 不详。
- PSA_START
	- 不详。
- PSA_END
	- 不详。
- INTEGRATOR
	- 不详。
- XDT
	- 示波器监视下的ADC采样之间的时间间隔。

以下是当前可设置参数：

- VOFFSET[Analog Signal Setup: DCOffset]
	- 输入信号偏置
	- 参数范围： -1.5V - 1.5V
- [Analog Signal Setup: Gain Sign]
	- 同CHANCSRA Bit 14
- ENERGY_RISETIME[Energy Filter: TPeaking]
	- T形算法的上升时间
	- 参数范围： $2\cdot 2^{SLOWFILTERRANGE}\cdot 10ns$ -  $124*2^{SLOWFILTERRANGE}*10ns$
- ENERGY_FLATTOP[Energy Filter: TGap]
	- T形算法的平台时间
	- 参数范围： $3*2^{SLOWFILTERRANGE}*10ns$ -  $125*2^{SLOWFILTERRANGE}*10ns$
- TRIGGER_RISETIME[Triger Filter: TPeaking]
	- T形算法的上升时间
	- 参数范围： 10ns - 63*10ns
- TRIGGER_FLATTOP[Triger Filter: TGap]
	- T形算法的平台时间
	- 参数范围： 0 - 63*10ns
- TRIGGER_THRESHOLD[Triger Filter: Thresh]
	- 该数值为trigger filter的阈值。当trigger filter输出大于该数值时，信号便被触发。
	- 参数范围： 0 - 16383 
- TRACE_LENGTH[Pulse Shape: Trace Length]
	- 获取波形的总长度
	- 参数范围： 0 - 81.92us，10ns一个采样点
- TRACE_DELAY[Pulse Shape: Trace Delay]	
	- 获取波形在触发前的长度
	- 参数范围： 0 - 10.24us，10ns一个采样点
- BLCUT[Baseline Setup： BL Cut]*该参数没理解*
	- 该变量用来设置基线的截断阈值在基线测量中。如果该数值不为0，DSP将会持续地检查计算出的每个基线数值是否超出设定的BLCUT限制。如果基线数值在限制范围内，他将用来计算平均值；反之，它将被抛弃。将BLCUT设为0将不检查基线，这样可减少处理时间。
- BASELINE_PERCENT[Baseline Setup：Baseline]
	- 该参数设置数字信号的偏置，让其自动调整到满量程的百分比处。例如满量程是16383，该数值设为10，那么信号的基线将自动调整到1638附近。
	- 参数范围： 0 - 100
- TAU[Decay Time： Tau]
	- 该数值为前放的指数衰减时间。它是DSP计算能量的参数之一，因此其输入的准确性将影响计算出能量的准确性。
	- 该参数单位：us


下面两个只适用于MCA mode

- EMIN[Histograming： EMin]
	- 能量值为16位的精度。
	- MCA的最小能量，计算出来的能量都会减去该数值再填充到谱上。因此 bin 0 即为 EMIN。 
	- 参数范围： 0 - 65535
- BINFACTOR[Histograming： Bin Factor]
	- 该数值用来控制直方图分bin个数。每个bin对应的能量为$MCAbin=(Energy-EMIN)\cdot 2^{-BINFACTOR}$
	- 参数范围： 1 - 16

## Channel Parameters(Output)

- LIVE_TIME
	- 该通道的活时
- FAST_PEAKS
	- 该通道的 fast trigger 事件数
- INPUT_COUNT_RATE
	- 该通道的输入计数率
- OUTPUT_COUNT_RATE
	- 该通道的输出计数率
- EVENT_RATE
	- 该插件的事件处理速率

----

# DSP Parmeters

## Module input parameters

- MODNUM
	- 见 API 中的MODULE_NUMBER。
- MODCSRA **重要**
	- Bit 0 : If set,connect module trigger lines to backplane.Triggers will be shared with other modules.If not set,triggers are distributed only between channels of this module,depending on channel settings.
	- Bit 1-31 : 预留。
- MODCSRB **重要**
	- Bit 0 : If set,wired-OR trigger lines on the backplane connect to a pullup resistor.This bit should be set for only one module in the backplane segment.    Enable pullups for backplane bus lines(Enable for only module per chassis)
	- Bit 1 : If set,trigger lines in the J2 connector connect to the backplane.
	- Bit 2 : If set,the module is a trigger master that issues and receives triggers from the backplane.Several modules can be set as trigger masters.If not set,the module only receives triggers from the backplane.
	- Bit 4 : Send trigger to trigger card and local backplane(enable for only one module among all chassis)
	- Bit 6 : Chassis master module(Enable for only one module per chassis)
	- Bit 7 : Select global fast trigger source (either external validation trigger (checked) or external fast trigger (unchecked), in case they are swapped at the front panel)
	- Bit 8 : Select external validation trigger source (either external fast trigger (checked) or external validation trigger(unchecked), in case they are swapped at the front panel)
	- Bit 10 : Enable run inhibit signal input (enable for only one module among all chassis)
	- Bit 11 : Synchronize runs and distribute triggers among multiple chassis
	- Bit 12 : Sort events based on timestamps (all channels should have same setting to use this feature)
	- Bit 13 : Enable sending fast triggers to backplane(only one module can enable this option in each PCI bus segment of a chassis)
- MODFORMAT
	- 见 API 中的MODULE_FORMATE。
- CHANNUM
	- 通道编号。
- RUNTASK
	- 运行模式。当前支持三种模式，见 API 中的RUN_TYPE。
- CONTROLTASK
	- Use this variable to select a control task.Consult the control tasks section of this manual for detailed information.The control task will be launched when you issue a run start command with RUNTASK=0.
- MAXEVENTS
	- 如果该插件获取事件数到达该数值，则停止获取。将MaxEvents设置为0则关闭该功能。
- COINCPATTERN
	- The user can request that certain coincidence/anticoincidence patterns are found for the event to be accepted.
- COINCWAIT
	- Duration of the cioncidence time window in clock ticks(each clock tick spans 10ns)
- SYNCHWAIT
	- 见 API 中的SYNCH_WAIT。
- INSYNCH
	- 见 API 中的IN_SYNCH。
- HOSTIO
	- A 4 word data block that is used to specify command options.
- RESUME
	- Set this variable to 1 to resume a data run;otherwise,set it to 0.
- SLOWFILTERRANGE
	- The energy filter range downloaded from the host to the DSP and FPGA.It sets the number of ADC samples($2^{SLOWFILTERRANGE}$)to be averaged before entering the energy filtering logic.The currently supported filter range in the signal processing FPGA includes 1,2,3,4,5,6.
- FASTFILTERRANGE
	- The trigger filter range downloaded from the host to the DSP and FPGA.It sets the number of ADC samples($2^{FASTFILTERRANGE}$)to be averaged before entering the trigger filtering logic.The currently supported filter range in the signal processing FPGA is only 0.
- U00
	- 预留。
- USERIN
	- A block of 16 input variables used by user-written DSP code.

## Channel input parameters 

- CHANCSRB
	- Bit 0-15 : 预留，全部设为0。
- GAINDAC
	- Reserved and not supported.
- OFFSETDAC
	- This DAC determines the DC-offset voltage.The offset can be calculated using the following formula: Offset[V] = 1.5*((32768-TRACKDAC)/32768)
- DIGGAIN
	- The digital gain factor for compensating the difference between the user desired voltage gain and the SGA gain.
- UNUSEDA0
	- 预留。
- UNUSEDA1
	- 预留。

以下是当前可设置参数：

- CHANCSRA
	- Bit 0 : Fast trigger Selection (external fast trigger(checked) or selected group trigger(unchecked))
	- Bit 1 : Module validation trigger selection (module veto input(checked) or module validation trigger(unchecked))
	- Bit 2 : Good channel
		- 此项控制是否开启该路获取。
	- Bit 3 : Channel validation trigger selection (channel veto input(checked) or channel validation trigger(unchecked))
	- Bit 4 : Block data acquisition if trace or header buffer are full
	- Bit 5 : Trigger positive
		- 选择此项表示正斜率触发，清除此项表示负斜率触发。trigger/filter FPGA只能处理正信号，对负信号将会先作反相处理之后再送入FPGA。
	- Bit 6 : Enable channel trigger veto
	- Bit 7 : Histogram energy
		- 此项控制是否在插件MCA内存中记录能量直方图。
	- Bit 8 : Enable trace capture
		- 此项控制是否记录波形。
		- 1-enable  0-disable
	- Bit 9 : Enable QDC sums capture
		- 此项控制是否记录QDC sums。
		- 1-enable  0-disable
	- Bit 10 : Enable CFD trigger mode
		- 此项控制是否开启CFD trigger mode。
		- 1-enable  0-disable
	- Bit 11 : Enable global trigger validation
		- 此项控制是否开启global trigger for events validation for this channel。
		- 1-enable  0-disable
	- Bit 12 : Enable capture raw energy sums and baselines
		- 此项控制是否记录raw energy sums和baselines
		- 1-enable  0-disable
	- Bit 13 : Enable channel trigger validation
		- 此项控制是否开启channel trigger for events validation for this channel。
		- 1-enable  0-disable
	- Bit 14 : Enable input relay
	- This bit controls the input relay of the Pixie-16,switching between high and low gain or connecting/disconnecting the input.The function depends on the input specifications for your particular bardware.
	- Bit 15 : Pileup rejection control
		- 此项控制是否开启堆积拒绝。
		- 1-enable  0-disable
	- Bit 16 : Inverse pileup rejection control
	- Bit 17 : Enable "no traces for large pulses" featrue
	- Bit 18 : Group trigger selection (external group trigger(checked) or local fast trigger(unchecked))
	- Bit 19 : Channel veto selection (channel validation trigger(checked) or front panel channel veto(unchecked))
	- Bit 20 : Module veto selection (module validation trigger(checked) or front panel module veto(unchecked))
	- Bit 21 : Enable (checked) or disable (unchecked) recording of external clock timestamps in event header



----

## coincidence/multiplicity trigger

前提是自身fast filter得过阈值

CSRA bit3=0 bit13=1
选择左、中、右插件参与判断路

设置 coincidence/multiplicity 阈值

FastTrigBackplane 将trigger信号发给左右插件

ChanTrigStretch 设置门宽

## channel trigger veto

CSRA bit6=1

然后从 CSRA bit19 选择veto trigger 来源，0 为 front panel，1 为 channel validation trigger

如果选择 channel validation trigger，channel validation trigger 设置与 符合等是一样的。

需要设置 VetoSttretch 门宽


## group trigger for channel validation trigger （可被动记录）

select other group trigger as the source of channel validation trigger
CSRA bit18 =1 select external group trigger from system FPGA
对某4路选择group trigger，选择trigger 来源（左插件、自身插件、右插件），选择左右插件需要设置FastTrigBackplane


<!-- | a | b | c | -->
<!-- | :--- | :--- | :--- | -->
<!-- | 1 | 2 | 3 | -->
<!-- | 4 | 5 | 6 | -->

<!-- backup.md ends here -->
