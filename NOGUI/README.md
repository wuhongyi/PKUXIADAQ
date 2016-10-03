<!-- README.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 四 9月  8 16:09:02 2016 (+0800)
;; Last-Updated: 四 9月  8 17:28:09 2016 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 4
;; URL: http://wuhongyi.github.io -->

# 代码简单描述

运行程序之前需要先修改以下文件：

- 文件**PixieLSMRun.config**第一行为输出文件路径， 第二行为文件名，第三行为run number。
- 文件**cfgPixie16.txt**中参数意义与官网提供示例代码相同。

测试只有两个插件。

测试程序包含以下代码文件：

- main.cc
- Global.hh
- Global.cc
- Manager.hh
- Manager.cc
- Detector.hh
- Detector.cc

# 程序运行

```
  [q]   Quit
  [s]   Start/Stop acquisition
--------------------------------------------------------------------------
```

执行编译好的可执行文件后会有以上运行提示，键盘输入 q **退出程序**，输入 s **启动获取**，再输入一次 s **停止获取**。


# 程序逻辑

主程序 main.cc 初始化运行管理类 Manager 并进入键盘交互状态。

Detector类主要函数功能描述：

```cpp
//执行函数 Pixie16InitSystem Pixie16BootModule
bool BootSystem();

//执行函数 Pixie16ExitSystem
int ExitSystem();

//Pixie16WriteSglModPar - SYNCH_WAIT
//Pixie16WriteSglModPar - IN_SYNCH
//执行函数 Pixie16StartListModeRun
int StartLSMRun(int continue_run);

//执行函数  Pixie16EndRun
int StopLSMRun();

//执行函数 Pixie16CheckExternalFIFOStatus Pixie16ReadDataFromExternalFIFO
//该函数中 if(fonline) StatisticsForModule(); 每3秒会执行函数 Pixie16ReadStatisticsFromModule 读取统计信息
int ReadDataFromModules(int thres=0,unsigned short endofrun=0);

//执行函数 Pixie16ReadStatisticsFromModule 读取统计参数并通过共享内存发送
int UpdateSharedMemory();

// 每隔3秒调用一次UpdateSharedMemory()函数
void StatisticsForModule();

```

# 输入信号

测试信号源为脉冲发生器，频率 1 kHz，幅值 1V。

<!-- README.md ends here -->
