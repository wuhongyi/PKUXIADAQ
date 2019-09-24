.. XIAAPI.rst --- 
.. 
.. Description: 
.. Author: Hongyi Wu(吴鸿毅)
.. Email: wuhongyi@qq.com 
.. Created: 三 7月  3 14:59:18 2019 (+0800)
.. Last-Updated: 二 9月 24 16:55:43 2019 (+0800)
..           By: Hongyi Wu(吴鸿毅)
..     Update #: 30
.. URL: http://wuhongyi.cn 

=================================   
XIA API
=================================

以下说明来源于 **Pixie-16 Programmer’s Manual** *Version 3.06, September 13, 2019*

----------------------------------------------------------------------
XIA API
----------------------------------------------------------------------


:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16AcquireADCTrace
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

在单个或多个模块中获取 ADC 波形

.. code:: cpp

  // 使用此功能可从 Pixie-16 模块获取 ADC 波形。 使用从 0 开始计数的 ModNum 指定模块。如果 ModNum 设置为小于系统中模块的总数，则仅 ModNum 指定的模块将获取其 ADC 波形。 但是，如果 ModNum 等于系统中模块的总数，则系统中的所有模块都将获取其 ADC 波形。
  // 成功返回此功能后，DSP 的内部存储器将充满 ADC 跟踪数据。 然后，用户的应用程序软件应调用 Pixie16ReadSglChanADCTrace 函数，以逐通道的方式将 ADC 跟踪数据读出到主机。
  int Pixie16AcquireADCTrace (
        unsigned short ModNum );// module number
   
**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 无效的 Pixie-16 模块编号
   - 使用正确的 ModNum
- -2 无法开始运行
   - 重新启动模块
- -3 获取 ADC 跟踪超时
   - 重新启动模块

**示例代码**

.. code:: cpp

  unsigned short ModNum;
  int retval;
  
  // assume we want to acquire ADC trace from module 0
  ModNum = 0;
  
  // acquire the trace
  retval = Pixie16AcquireADCTrace (ModNum);
  if(retval < 0)
  {
  // error handling
  }

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16AcquireBaselines
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

从模块获取基线

.. code:: cpp

  // 使用此功能可从 Pixie-16 模块获取基线。 使用从 0 开始计数的 ModNum 指定模块。如果 ModNum 设置为小于系统中模块的总数，则仅 ModNum 指定的模块将获取其基线。但是，如果将 ModNum 设置为等于系统中模块的总数，那么系统中的所有模块都将获取其基线。
  // 成功返回此功能后，DSP 的内部存储器将充满基线数据。然后，用户应调用函数 Pixie16ReadSglChanBaselines，以逐个通道的方式将基线数据读出到主机。
  int Pixie16AcquireBaselines (
        unsigned short ModNum );// module number

**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 无效的 Pixie-16 模块编号
   - 使用正确的 ModNum
- -2 无法启动 GET_BASELINES 运行
   - 重新启动模块
- -3 GET_BASELINES 运行超时
   - 重新启动模块

**示例代码**

.. code:: cpp
	
  unsigned short ModNum;
  int retval;
  
  // assume we want to acquire baselines for module 0
  ModNum = 0;
  
  // acquire the trace
  retval = Pixie16AcquireBaselines (ModNum);
  if(retval < 0)
  {
  // error handling
  }

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16AdjustOffsets
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

调整单个或多个模块中的直流偏置

.. code:: cpp

  // 使用此功能可以调整 Pixie-16 模块的 DC 偏置。 使用从 0 开始计数的 ModNum 指定模块。如果 ModNum 设置为小于系统中模块的总数，则仅 ModNum 指定的模块将调整其 DC 偏移量。 但是，如果 ModNum 设置为等于系统中模块的总数，则系统中的所有模块都将调整其 DC 偏移量。
  // 调整直流偏置电平后，数字化输入信号的基准电平将由 DSP 参数 BaselinePercent 确定。 例如，如果 BaselinePercent设置为 10（％），则输入信号的基线电平将在 12 位 ADC 比例上为 ~409（最小值：0；最大值：4095）。
  // 此功能的主要目的是确保输入信号落在 ADC 的电压范围内，以便所有输入信号都能被 ADC 正确数字化。
  int Pixie16AdjustOffsets (
        unsigned short ModNum ); // module number

**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 无效的 Pixie-16 模块编号
   - 检查 ModNum 是否有效
- -2 无法启动 ADJUST_OFFSETS 运行
   - 重新启动模块
- -3 ADJUST_OFFSETS 运行超时
   - 检查错误消息日志文件 Pixie16msg.txt

**示例代码**

.. code:: cpp

  int retval;
  unsigned short ModNum;
  
  // set to the first module
  ModNum = 0;
  
  // adjust dc-offsets
  retval = Pixie16AdjustOffsets(ModNum);
  if(retval < 0)
  {
  // error handling
  }

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16BLcutFinder
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

计算模块的新基线切割值

.. code:: cpp

  // 使用此功能可以找到 Pixie-16 模块一个通道的“基线切割”值。 然后将基线切割值下载到 DSP，在其中捕获基线并随时间平均。 削减值将防止在平均过程中使用错误的基线值，即，如果基线值超出基线削减范围，则不会将其用于计算基线平均值。 随时间推移平均基线可以改善能量分辨率的测量。
  // ModNum 是从 0 开始计数的模块号。ChanNum 是从 0 开始计数的通道号。
  int Pixie16BLcutFinder (
        unsigned short ModNum, // module number
        unsigned short ChanNum,// channel number
        unsigned int *BLcut ); // returned BLcut value
        
**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 无效的 Pixie-16 模块编号
   - 检查 ModNum 是否有效
- -2 收集基线失败
   - 重新启动模块
- -3 无法从数据存储器读取基线
   - 检查错误消息日志文件 Pixie16msg.txt

**示例代码**

.. code:: cpp

  int retval;
  unsigned short ModNum, ChanNum;
  unsigned int BLcut;
  
  // set to the first module
  ModNum = 0;
  
  // set to the first channel
  ChanNum = 0;
  
  // find the BLcut value
  retval = Pixie16BLcutFinder(ModNum, ChanNum, &BLcut);
  if(retval < 0)
  {
  // error handling
  }

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16BootModule
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

引导模块，以便可以设置它们以进行数据接收。

.. code:: cpp

  // 使用此功能引导 Pixie-16 模块，以便可以设置它们以进行数据采集。 该功能将通信 FPGA 配置，信号处理（或系统）FPGA 配置，触发 FPGA 配置（仅适用于版本A模块），数字信号处理器（DSP）的可执行代码和 DSP 参数下载到 Pixie-16 模块。
  // FPGA 配置由固定长度的字符组成，具体取决于模块上安装的硬件。 DSP 代码的长度取决于实际编译的代码； 每个模块的 DSP 参数集始终由 1280 个 32 位字组成。 主机软件必须使硬盘上那些引导数据文件的名称可用于引导功能。
  // ModNum 是从 0 开始计数的模块号。如果 ModNum 设置为小于系统中模块的总数，则仅引导由 ModNum 指定的模块。 但是如果 ModNum 等于系统中模块的总数，例如 机箱中有 5 个模块，ModNum = 5，则系统中的所有模块都将被引导。
  // 引导模式是一个位掩码（如下所示），指示将引导哪个板载芯片。 通常情况下，所有板载芯片都应被引导，即引导模式为 0x7F。 对于版本 B，C，D，F 模块，即使该位设置为 1，也将忽略其位 1，即 “启动触发 FPGA”。
  // bit 0: 引导通信 FPGA，所有 Pixie-16 版本
  // bit 1: 引导触发 FPGA，仅 Pixie-16 版本A
  // bit 2: 引导信号处理 FPGA，所有 Pixie-16 版本
  // bit 3: 引导数字信号处理器（DSP），所有 Pixie-16 版本
  // bit 4: 下载DSP I/O 参数，所有 Pixie-16 版本
  // bit 5: 编程板载 FPGA，所有 Pixie-16 版本
  // bit 6: 设置板载 DAC，所有 Pixie-16 版本
  int Pixie16BootModule (
        char *ComFPGAConfigFile,     // name of ComFPGA configuration file
        char *SPFPGAConfigFile,	     // name of SPFPGA configuration file 
        char *TrigFPGAConfigFile,    // name of trigger FPGA file         
        char *DSPCodeFile,	     // name of DSP code file             
        char *DSPParFile,	     // name of DSP parameter file        
        char *DSPVarFile,	     // name of DSP variable names file   
        unsigned short ModNum,	     // pixie-16 module number            
        unsigned short BootPattern); // boot pattern bit mask             
  
**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 无效的 Pixie-16 模块编号
   - 使用正确的 ModNum
- -2 ComFPGAConfigFile 的大小无效
   - 使用正确的 ComFPGAConfigFile
- -3 无法启动 Communication FPGA
   - 检查日志文件（Pixie16msg.txt）
- -4 无法分配内存以将数据存储在 ComFPGAConfigFile中
   - 关闭其他程序或重新启动计算机
- -5 无法打开 ComFPGAConfigFile
   - 使用正确的 ComFPGAConfigFile
- -10 SPFPGAConfigFile 的大小无效
   - 使用正确的 SPFPGAConfigFile
- -11 无法启动信号处理 FPGA
   - 检查日志文件（Pixie16msg.txt）
- -12 无法分配内存以将数据存储在 SPFPGAConfigFile中
   - 关闭其他程序或重新启动计算机
- -13 无法打开 SPFPGAConfigFile
   - 使用正确的 SPFPGAConfigFile
- -14 无法启动 DSP
   - 检查日志文件（Pixie16msg.txt）
- -15 无法分配内存来存储 DSP 可执行代码
   - 关闭其他程序或重新启动计算机
- -16 无法打开 DSPCodeFile
   - 使用正确的 DSPCodeFile
- -17 DSPParFile 的大小无效
   - 使用正确的 DSPParFile
- -18 无法打开 DSPParFile
   - 使用正确的 DSPParFile
- -19 无法初始化 DSP 变量索引
   - 使用正确的 DSPVarFile
- -20 无法复制 DSP 变量索引
   - 检查日志文件（Pixie16msg.txt）
- -21 无法在模块中编程 Fippi
   - 检查日志文件（Pixie16msg.txt）
- -22 无法在模块中设置 DAC
   - 检查日志文件（Pixie16msg.txt）
- -23 无法启动 RESET_ADC 在模块中运行
   - 检查日志文件（Pixie16msg.txt）
- -24 RESET_ADC 运行在模块中超时
   - 检查日志文件（Pixie16msg.txt）
     
**示例代码**

.. code:: cpp

  int retval;
  char ComFPGAConfigFile[256], SPFPGAConfigFile[256];
  char TrigFPGAConfigFile[256], DSPCodeFile[256], DSPParFile[256];
  char DSPVarFile[256];
  unsigned short ModNum, BootPattern;
   
  // first, specify file names and paths for all boot data files
  sprintf(ComFPGAConfigFile, "C:\\XIA\\Pixie16\\Firmware\\syspixie16.bin");
  sprintf(TrigFPGAConfigFile, " ");
  sprintf(SPFPGAConfigFile, "C:\\XIA\\Pixie16\\Firmware\\fippixie16.bin");
  sprintf(DSPCodeFile, "C:\\XIA\\Pixie16\\DSP\\Pixie16DSP.ldr");
  sprintf(DSPParFile, "C:\\XIA\\Pixie16\\Configuration\\default.set");
  sprintf(DSPVarFile, "C:\\XIA\\Pixie16\\DSP\\Pixie16DSP.var");
   
  // second, select the Pixie-16 module to boot. All modules in the system
  // can be booted either one-by-one or all at once. The simplest way to
  // boot all modules at once is to set ModNum to be equal to the total
  // number of modules in the system. Here we assume there are total 5
  // Pixie-16 modules.
  ModNum = 5;
   
  // third, specify the boot pattern. Normally, it should be 0x7F, i.e.
  // all bits of the bit mask are selected.
  BootPattern = 0x7F;
   
  // finally, boot the selected modules
  retval = Pixie16BootModule (ComFPGAConfigFile, SPFPGAConfigFile,
  TrigFPGAConfigFile, DSPCodeFile, DSPParFile, DSPVarFile, ModNum,
  BootPattern);
  if(retval < 0)
  {
  // error handling
  }

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16CheckExternalFIFOStatus
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Check status of external FIFO of a module

.. code:: cpp

  // 在进行列表模式数据采集运行时，使用此功能检查 Pixie-16 模块的外部 FIFO 的状态。 该函数返回外部 FIFO 当前具有的字数（32位）。 如果字数大于用户设置的阈值，则可以使用函数 Pixie16ReadDataFromExternalFIFO 从外部 FIFO 读取数据。
  // 用户可以设置阈值，以最大程度地减少读取开销或尽快从 FIFO 中读取数据。 Pixie-16 API（pixie16app_defs.h）为外部FIFO 读取（EXTFIFO_READ_THRESH）定义了一个值为 1024 的阈值。
  // *nFIFOWords 返回外部 FIFO 当前具有的 32 位数据的数量。
  // ModNum是从 0 开始计数的模块号。
  int Pixie16CheckExternalFIFOStatus (
        unsigned int *nFIFOWords,  // returned number of words in the FIFO
        unsigned short ModNum );   // module number
	
**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 无效的 Pixie-16 模块编号
   - 使用正确的 ModNum

**示例代码**

.. code:: cpp

  int retval;
  unsigned int nFIFOWords;
  unsigned short ModNum;
   
  ModNum = 0;
  // the first module
  retval = Pixie16CheckExternalFIFOStatus (&nFIFOWords, ModNum);

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16CheckRunStatus
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

检查数据采集运行的状态。

.. code:: cpp

  // 在进行列表模式数据采集运行时，使用此功能检查 Pixie-16 模块的运行状态。 如果运行仍在进行，请继续轮询。
  // 如果此函数的返回码指示运行已完成，则外部 FIFO（版本 B，C，D，F 模块）中仍可能需要将某些数据读出到主机。 此外，最终运行统计数据和直方图数据也可用于读取。
  // 在MCA直方图运行模式下，也可以调用此函数以检查运行是否仍在进行中，即使它通常是自终止的。
  // ModNum 是从 0 开始计数的模块号。
  int Pixie16CheckRunStatus (
        unsigned short ModNum ); // module number	  

**返回值描述及错误处理方法**

- 0 没有运行中
   - 没有
- 1 运行仍在进行中
   - 没有
- -1 无效的 Pixie-16 模块编号
   - 使用正确的 ModNum

**示例代码**

.. code:: cpp

  int retval;
  unsigned short ModNum;
  
  ModNum = 0;
  // the first module
  retval = Pixie16CheckRunStatus(ModNum);
  if(retval < 0)
  {
  // invalid module number
  }
  else if(retval == 1)
  {
  // run is still in progress
  }
  else if(retval == 0)
  {
  // run has finished
  }

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16ComputeFastFiltersOffline
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

计算快速滤波响应以进行离线分析

.. code:: cpp

  // 使用此功能可以计算列表模式数据文件中每个事件的快速触发响应，以进行离线分析。 在此离线分析功能中实现的算法与在 Pixie-16 硬件中实现的算法等效。 因此，该功能可用于分析在硬件中实现的前沿快速触发滤波和 CFD 滤波如何响应记录的波形形状。 通过分析这些滤波的响应，可以通过离线调整快速滤波和 CFD 参数来优化前沿触发或 CFD 触发的性能。然后可以将此类优化的参数保存到设置文件中，以用于在线数据采集。
  int Pixie16ComputeFastFiltersOffline (
        char *FileName, // the list mode data file name (with complete path)
        unsigned short ModuleNumber, // the module to be analyzed
        unsigned short ChannelNumber, // the channel to be analyzed
        unsigned int FileLocation, // the location of the trace in the file
        unsigned short RcdTraceLength,// recorded trace length
        unsigned short *RcdTrace, // recorded trace
        double *fastfilter, // fast filter response
        double *cfd ); // cfd response

**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 空指针 RcdTrace
   - 确保已为 RcdTrace 分配了足够的内存（最大长度为 32768，但可以更短以匹配实际的波形长度）
- -2 空指针  fastfilter
   - 确保为 fastfilter 分配了足够的内存（最大长度为 32768，但可以更短以匹配实际的波形长度）
- -3 空指针  cfd
   - 确保已为 cfd 分配了足够的内存（最大长度为 32768，但可以更短以匹配实际的波形长度）
- -4 目标模块编号无效
   - 使用正确的 ModuleNumber
- -5 波形长度太短
   - 对于使用快速滤波长度（快速滤波上升时间和平坦顶部）的离线分析，记录的波形的长度太短。 或者必须使用具有较短快速滤波长度的其他设置文件，或者必须获取具有较长波形长度的新波形
- -6 无法打开列表模式数据文件
   - 检查列表模式文件名及其路径是否正确

**示例代码**

.. code:: cpp

  int retval;
  unsigned short ModuleNumber, ChannelNumber;
  unsigned int FileLocation;
  unsigned short RcdTraceLength;
  unsigned short RcdTrace[1000];
  double fastfilter[1000];
  double cfd[1000];
   
  char *FileName = {"C:\\XIA\\Pixie16\\PulseShape\\listmodedata.bin"};
  ModuleNumber = 0;      //the first module
  ChannelNumber = 0;     //the first channel 
  FileLocation = 16;     //the starting address of the trace in the list mode data file (32-bit word address)
  RcdTraceLength = 1000; //the length of the recorded trace 
   
  retval = Pixie16ComputeFastFiltersOffline (FileName, ModuleNumber,
  ChannelNumber, FileLocation, RcdTrace, fastfilter, cfd);
  if(retval < 0)
  {
  // error handling
  }

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16ComputeInputCountRate
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

计算通道的输入计数率

.. code:: cpp

  // 使用此功能可在 Pixie-16 模块的一个通道上计算输入计数率。 此功能无法与 Pixie-16 模块通信。 在调用此函数之前，应调用另一个函数 Pixie16ReadStatisticsFromModule 从模块读取统计数据。
  // *Statistics 是指向数组的指针，该数组的大小正好是 448个 无符号整数字（32位）。 调用函数 Pixie16ReadStatisticsFromModule 后，*Statistics 数组将填充来自 Pixie-16 模块的数据。 ModNum 是从 0 开始计数的模块号。ChanNum 是从 0 开始计数的通道号。
  double Pixie16ComputeInputCountRate (
        unsigned int *Statistics,// run statistics data
        unsigned short ModNum,	 // module number      
        unsigned short ChanNum );// channel number 

**返回值描述及错误处理方法**

返回值是输入计数率，以每秒计数为单位。

**示例代码**

.. code:: cpp

  double icr;
  unsigned int Statistics[448];
  unsigned short ModNum, ChanNum;
  int retval;
   
  ModNum = 0;  // the first module 
  ChanNum = 0; // the first channel
   
  // first call Pixie16ReadStatisticsFromModule to get the statistics data
  retval = Pixie16ReadStatisticsFromModule (Statistics, ModNum);
  if(retval < 0)
  {
  // error handling
  }
   
  // compute input count rate
  icr = Pixie16ComputeInputCountRate (Statistics, ModNum, ChanNum);
  
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16ComputeLiveTime
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

计算通道在运行中累积的实时时间

.. code:: cpp

  // 使用此功能可以计算 Pixie-16 模块的一个通道在数据采集上花费的实时时间。此功能无法与 Pixie-16 模块通信。 在调用此函数之前，应调用另一个函数 Pixie16ReadStatisticsFromModule 从模块读取统计数据。
  // *Statistics 是指向数组的指针，该数组的大小正好是 448 个无符号整数字（32位）。 调用函数 Pixie16ReadStatisticsFromModule 后，*Statistics 数组将填充来自 Pixie-16 模块的数据。 ModNum 是从 0 开始计数的模块号。ChanNum 是从 0 开始计数的通道号。
  double Pixie16ComputeLiveTime (
        unsigned int *Statistics, // run statistics data
        unsigned short ModNum,	  // module number      
        unsigned short ChanNum ); // channel number     

**返回值描述及错误处理方法**

返回值为以秒为单位的实时时间。

**示例代码**

.. code:: cpp

  double livetime;
  unsigned int Statistics[448];
  unsigned short ModNum, ChanNum;
  int retval;
   
  ModNum = 0; // the first module 
  ChanNum = 0;// the first channel
   
  // first call Pixie16ReadStatisticsFromModule to get the statistics data
  retval = Pixie16ReadStatisticsFromModule (Statistics, ModNum);
  if(retval < 0)
  {
  // error handling
  }
   
  // compute live time
  livetime = Pixie16ComputeLiveTime (Statistics, ModNum, ChanNum);

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16ComputeOutputCountRate
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

计算通道的输出计数率

.. code:: cpp

  // 使用此功能可以计算 Pixie-16 模块一个通道上的输出计数率。 此功能无法与 Pixie-16 模块通信。 在调用此函数之前，应调用另一个函数 Pixie16ReadStatisticsFromModule 从模块读取统计数据。
  // *Statistics 是指向数组的指针，该数组的大小正好是 448 个无符号整数字（32位）。 调用函数 Pixie16ReadStatisticsFromModule 后，*Statistics 数组将填充来自 Pixie-16 模块的数据。 ModNum 是从 0 开始计数的模块号。ChanNum 是从 0 开始计数的通道号。
  double Pixie16ComputeOutputCountRate (
        unsigned int *Statistics, // run statistics data
        unsigned short ModNum,	  // module number      
        unsigned short ChanNum ); // channel number
	
**返回值描述及错误处理方法**

返回值是输出计数率，以每秒计数为单位。

**示例代码**

.. code:: cpp

  double ocr;
  unsigned int Statistics[448];
  unsigned short ModNum, ChanNum;
  int retval;
   
  ModNum = 0;  // the first module 
  ChanNum = 0; // the first channel
   
  // first call Pixie16ReadStatisticsFromModule to get the statistics data
  retval = Pixie16ReadStatisticsFromModule(Statistics, ModNum);
  if(retval < 0)
  {
  // error handling
  }
   
  // compute output count rate
  ocr = Pixie16ComputeOutputCountRate(Statistics, ModNum, ChanNum);

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16ComputeProcessedEvents
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

计算模块处理的事件数

.. code:: cpp

  // 使用此功能可以计算数据采集运行期间 Pixie-16 模块已处理的事件数。 此功能仅由 版本A 模块使用。 此功能无法与 Pixie-16 模块通信。 在调用此函数之前，应先调用另一个函数 Pixie16ReadStatisticsFromModule，以首先从模块读取统计数据。
  // *Statistics 是指向数组的指针，该数组的大小正好是 448 个无符号整数字（32位）。 调用函数 Pixie16ReadStatisticsFromModule 后，*Statistics 数组将填充来自 Pixie-16 模块的数据。 ModNum 是从 0 开始计数的模块号。ChanNum 是从 0 开始计数的通道号。
  double Pixie16ComputeProcessedEvents (
        unsigned long *Statistics,// run statistics data
        unsigned short ModNum );  // module number   

**返回值描述及错误处理方法**

返回值是已处理事件的数量。

**示例代码**

.. code:: cpp

  double NumEvents;
  unsigned long Statistics[448];
  unsigned short ModNum;
  int retval;
   
  ModNum = 0;// the first module
   
  // first call Pixie16ReadStatisticsFromModule to get the statistics data
  retval = Pixie16ReadStatisticsFromModule(Statistics, ModNum);
  if(retval < 0)
  {
  // error handling
  }
   
  // compute number of processed events
  NumEvents = Pixie16ComputeProcessedEvents(Statistics, ModNum);

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16ComputeRealTime
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

计算模块在运行中累积的实时时间

.. code:: cpp

  // 使用此功能可以计算 Pixie-16 模块在数据采集上花费的实时时间。 此功能无法与 Pixie-16 模块通信。 在调用此函数之前，应调用另一个函数 Pixie16ReadStatisticsFromModule 从模块读取统计数据。
  // *Statistics 是指向数组的指针，该数组的大小正好是 448 个无符号整数字（32位）。 调用函数 Pixie16ReadStatisticsFromModule 后，*Statistics 数组将填充来自 Pixie-16 模块的数据。 ModNum 是从 0 开始计数的模块号。ChanNum 是从 0 开始计数的通道号。
  double Pixie16ComputeRealTime (
        unsigned int *Statistics,// run statistics data
        unsigned short ModNum ); // module number      

**返回值描述及错误处理方法**

返回值为以秒为单位的实时时间。

**示例代码**

.. code:: cpp

  double realtime;
  unsigned int Statistics[448];
  unsigned short ModNum;
  int retval;
  ModNum = 0; // the first module
   
  // first call Pixie16ReadStatisticsFromModule to get the statistics data
  retval = Pixie16ReadStatisticsFromModule(Statistics, ModNum);
  if(retval < 0)
  {
  // error handling
  }
   
  // compute real time
  realtime = Pixie16ComputeRealTime(Statistics, ModNum);

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16ComputeSlowFiltersOffline
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

计算慢速滤波响应以进行离线分析

.. code:: cpp

  // 使用此功能可以计算列表模式数据文件中每个事件的慢速滤波响应，以进行离线分析。 在此离线分析功能中实现的算法与在 Pixie-16 硬件中实现的算法等效。 因此，该功能可用于分析在硬件中实现的用于计算事件能量的慢速滤波如何响应记录的波形形状。 通过分析这些滤波的响应，可以通过离线调整慢速滤波的参数来优化其性能。然后可以将此类优化的参数保存到设置文件中，以用于在线数据采集。
  int Pixie16ComputeSlowFiltersOffline (
        char *FileName, // the list mode data file name (with complete path)
        unsigned short ModuleNumber, // the module to be analyzed
        unsigned short ChannelNumber, // the channel to be analyzed
        unsigned int FileLocation, // the location of the trace in the file
        unsigned short RcdTraceLength,// recorded trace length
        unsigned short *RcdTrace,// recorded trace
        double *slowfilter);// slow filter response

**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 空指针 RcdTrace
   - 确保已为 RcdTrace 分配了足够的内存（最大长度为 32768，但可以更短以匹配实际的波形长度）
- -2 空指针 slowfilter
   - 确保为 slowfilter 分配了足够的内存（最大长度为 32768，但可以更短以匹配实际的波形长度）
- -3 目标模块编号无效
   - 使用正确的 ModuleNumber
- -4 波形长度太短
   - 对于使用slow filter长度（较慢的滤波上升时间和平坦的顶部）的离线分析，记录的波形长度太短。要么必须使用具有较短的滤波长度的其他设置文件，要么必须获取具有较长波形长度的新波形
- -5 无法打开列表模式数据文件
   - 检查列表模式文件名及其路径是否正确
     
**示例代码**

.. code:: cpp

  int retval;
  unsigned short ModuleNumber, ChannelNumber;
  unsigned int FileLocation;
  unsigned short RcdTraceLength;
  unsigned short RcdTrace[1000];
  double slowfilter[1000];
   
  char *FileName = {"C:\\XIA\\Pixie16\\PulseShape\\listmodedata.bin"};
   
  ModuleNumber = 0;     // the first module 
  ChannelNumber = 0;    // the first channel
  FileLocation = 16;    // the starting address of the trace in the list mode data file (32-bit word address)
  RcdTraceLength = 1000;// the length of the recorded trace
   
  retval = Pixie16ComputeSlowFiltersOffline (FileName, ModuleNumber,
  ChannelNumber, FileLocation, RcdTrace, slowfilter);
  if(retval < 0)
  {
  // error handling
  }	  

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16ControlTaskRun
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

执行特殊控制任务

.. code:: cpp

  // 使用此功能可以执行特殊的控制任务。 这可能包括在下载 DSP 参数后对 Fippi 进行编程或设置 DAC。
  // ModNum 是从 0 开始计数的模块号。ChanNum 是从 0 开始计数的通道号。
  int Pixie16ControlTaskRun (
        unsigned short ModNum,     // module number                
        unsigned short ControlTask,// control task run type        
        unsigned int Max_Poll );   // Timeout control in unit of ms
  
**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 无效的 Pixie-16 模块编号
   - 使用正确的 ModNum
- -2 控制任务运行失败
   - 重新启动模块
     
**示例代码**

.. code:: cpp

  int retval;
  unsigned short ControlTask, ModNum;
  unsigned int Max_Poll;
   
  ModNum = 0;            // this is the first module                
  ControlTask = SET_DACS;// constant SET_DACS defined in header file
  Max_Poll = 10000;      // give the run 10 seconds to finish       
   
  retval = Pixie16ControlTaskRun (ControlTask, ModNum, Max_Poll);
  if(retval < 0)
  {
  // Error handling
  }
  
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16CopyDSPParameters
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

将DSP参数复制到其他模块

.. code:: cpp

  // 使用此功能可将 DSP 参数从一个模块复制到系统中其它模块并使之生效。
  // BitMask 是位模式，它指定应将哪些项目从源模块复制到目标模块。
  // Bit 0: Filter (energy and trigger)
  // Bit 1: Analog signal conditioning (polarity, dc-offset, gain/attenuation)
  // Bit 2: Histogram control (minimum energy, binning factor)
  // Bit 3: Decay time
  // Bit 4: Pulse shape analysis (trace length and trace delay)
  // Bit 5: Baseline control (baseline cut, baseline percentage)
  // Bit 7: Channel CSRA register (good channel, trigger enabled, etc.)
  // Bit 8: CFD trigger (CFD delay, scaling factor)
  // Bit 9: Trigger stretch lengths (veto, external trigger, etc.)
  // Bit 10: FIFO delays (analog input delay, fast trigger output delay, etc.)
  // Bit 11: Multiplicity (bit masks, thresholds, etc.)
  // Bit 12: QDC (QDC sum lengths)
  // SourceModule 和 SourceChannel 是要复制到其它模块和通道的 DSP 参数的模块和通道号。
  // DestinationMask 是一个数组，指示要从源通道和模块复制其设置的通道和模块。 例如，如果系统中有 5 个模块（总共 80 个通道），则 DestinationMask 将被定义为 DestinationMask[80]，其中 DestinationMask[0] 至 DestinationMask[15] 将选择模块 0 的通道 0 至 15，DestinationMask[16] 到 DestinationMask[31] 将选择模块 1 的通道 0 到 15，依此类推。 如果要复制给定的通道 i，则 DestinationMask[i] 应设置为 1，否则应将其设置为 0。
  int Pixie16CopyDSPParameters (
        unsigned short BitMask,           // copy items bit mask 
        unsigned short SourceModule,	  // source module       
        unsigned short SourceChannel,	  // source channel      
        unsigned short *DestinationMask );// destination bit mask

**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 无法在模块中编程 Fippi
   - 重新启动模块
- -2 无法在模块中设置 DAC
   - 重新启动模块
     
**示例代码**

.. code:: cpp

  int retval;
  unsigned short BitMask;             //copy items bit mask 
  unsigned short SourceModule;        //source module       
  unsigned short SourceChannel;       //source channel      
  unsigned short DestinationMask[384];//destination bit mask
  unsigned short k;                   
   
  BitMask = 0x1FF;  // copy everything  
  SourceModule = 0; // the first module 
  SourceChannel = 0;// the first channel
   
  // assume there are 6 Pixie-16 modules in the system
  for(k = 0; k < (6 * 16); k ++)
  {
  DestinationMask[k] = 1; // copy to all channels of all modules
  }
   
  retval = Pixie16CopyDSPParameters (BitMask, SourceModule, SourceChannel,
  DestinationMask);
  if(retval < 0)
  {
  // error handling
  }

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16EMbufferIO
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

在主机和 DSP 外部存储器之间传输数据

.. code:: cpp

  // 使用此功能可直接从 Pixie-16 模块的板载外部存储器读取数据或将数据写入其中。有效的存储器地址是从 0x0 到 0x7FFFF（32位宽）。 将 Direction = 1 用于读取，将 Direction = 0 用于写入。
  // 外部存储器用于存储 Pixie-16 模块 16 个通道中每个通道累积的直方图数据。每个通道的固定直方图长度为 32768 字（32位宽），直方图数据在存储器中的位置与通道号的顺序相同，即通道 0 占用内存地址 0x0 至 0x7FFF，通道 1 占用 0x8000 到 0xFFFF，依此类推。
  // 注意：另一个函数 Pixie16ReadHistogramFromModule 也可以用于读出直方图，只是需要逐个通道调用它。
  // ModNum 是从 0 开始计数的模块号。
  int Pixie16EMbufferIO (
        unsigned int *Buffer,// buffer data                            
        unsigned int NumWords, // number of buffer words to read or write
        unsigned int Address,// buffer address                         
        unsigned short Direction, // I/O direction                       
        unsigned short ModNum ); //module number                          

**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 空指针 buffer
   - 使用正确的指针 Buffer
- -2 buffer 字数超过限制
   - 减少 buffer 字数
- -3 无效的 DSP 外部存储器地址
   - 使用有效地址
- -4 无效的 I/O 方向
   - 使用有效方向
- -5 无效的 Pixie-16 模块编号
   - 使用正确的 ModNum
- -6 I/O 失败
   - 重新启动模块
     
**示例代码**

.. code:: cpp

  int retval;
  unsigned short Direction, ModNum;
  unsigned int MCAData[32768], NumWords, Address;
  NumWords = 32768;//to read out the MCA data from channel 0
  ModNum = 0;	 //the first module in the system         
  Address = 0x0;	 //the starting memory address            
  Direction = 1;	 //I/O direction is read                  
   
  // read out the MCA data for Channel 0
  retval = Pixie16EMbufferIO (MCAData, NumWords, Address, Direction, ModNum);
  if(retval != 0)
  {
  // Error handling
  }
  
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16EndRun
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

停止数据采集运行

.. code:: cpp

  // 使用此功能可结束直方图运行，或强制结束列表模式运行。在多模块系统中，如果所有模块都同步运行，则只需要以这种方式寻址一个模块。它将立即停止系统中所有其他模块的运行。ModNum 是从 0 开始计数的模块号。
  int Pixie16EndRun (
        unsigned short ModNum );// module number
	
**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 无效的 Pixie-16 模块编号
   - 使用正确的 ModNum
- -2 停止运行失败
   - 重新启动模块
     
**示例代码**

.. code:: cpp

  int retval;
  unsigned short ModNum;
  ModNum = 0;
  // the first module
  retval = Pixie16EndRun (ModNum);
  if(retval < 0)
  {
  // error handling
  }

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16ExitSystem
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

释放分配给模块的用户虚拟地址

.. code:: cpp

  // 当通过功能 Pixie16InitSystem 初始化这些模块时，使用此功能可释放分配给 Pixie-16 模块的用户虚拟地址。 应该在用户的应用程序退出之前调用此函数。
  // 如果将 ModNum 设置为小于系统中模块的总数，则将仅关闭 ModNum 指定的模块。 但是如果 ModNum 等于系统中模块的总数，例如 机箱中有 5 个模块，ModNum = 5，则系统中的所有模块将完全关闭。 请注意，模块从 0 开始计数。
  int Pixie16ExitSystem (
        unsigned short ModNum ); // Pixie-16 module number

**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 无效的 Pixie-16 模块编号
   - 使用正确的 ModNum（不应大于系统中模块的总数）
- -2 无法关闭 Pixie-16 模块
   - 检查错误消息日志文件 Pixie16msg.txt
     
**示例代码**

.. code:: cpp

  int retval;
  unsigned short ModNum, k;
   
  // assume there are totally 5 modules in the system
  // so close all 5 modules
  ModNum = 5;
  retval = Pixie16ExitSystem (ModNum);
  if(retval < 0)
  {
  // error handling
  }
   
  // or module by module
  for(k=0; k<5; k++)
  {
    retval = Pixie16ExitSystem (k);
    if(retval < 0)
    {
    // error handling
    }
  }
  
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16GetEventsInfo
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

从数据文件获取详细的事件信息

.. code:: cpp

  // 使用此功能可在指定模块的列表模式数据文件中检索每个事件的详细信息（波形除外）。 ModNum 是从 0 开始计数的模块号。ChanNum 是从 0 开始计数的通道号。
  // 在调用此函数以获取单个事件信息之前，应首先调用另一个函数 Pixie16GetModuleEvents 以确定每个模块已记录的事件数。 如果给定模块的事件数为 nEvents，则应为内存块 *EventInformation 分配长度（nEvents * 68）：
  // EventInformation = (unsigned int *)malloc(sizeof(unsigned int) * nEvents * 68);
  // 其中 68 是每个事件（能量，时间戳等）的信息记录的长度，并具有以下结构。
  // EventInformation [0] Event number
  // EventInformation [1] Channel number
  // EventInformation [2] Slot number
  // EventInformation [3] Crate number
  // EventInformation [4] Header length
  // EventInformation [5] Event length
  // EventInformation [6] Finish code
  // EventInformation [7] Event timestamp (lower 32-bit)
  // EventInformation [8] Event timestamp (upper 16-bit)
  // EventInformation [9] Event energy
  // EventInformation [10] Trace length
  // EventInformation [11] Trace location
  // EventInformation [67:12] Not used
  int Pixie16GetEventsInfo (
        char *FileName,// the list mode data file name (with complete path)
        unsigned int *EventsInformation,// to hold event information
        unsigned short ModuleNumber );// module to get events from
  
**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 空指针 EventInformation
   - 使用正确的 EventInformation
- -2 无效的 Pixie-16 模块编号
   - 使用正确的 ModuleNumber
- -3 无法打开列表模式数据文件
   - 正确的文件名和路径

**示例代码**

.. code:: cpp

  int retval;
  char *FileName = {"C:\\XIA\\Pixie16\\PulseShape\\listmodedata.bin"};
  unsigned short ModuleNumber;
  unsigned int *EventInformation;
  unsigned int ModuleEvents[7]; // assume maximum number of modules is 7
   
  retval = Pixie16GetModuleEvents (FileName, ModuleEvents);
  if(retval < 0)
  {
  // error handling
  }
   
  ModuleNumber = 0;// the first module
  EventInformation = (unsigned int *)malloc(sizeof(unsigned int) *
                     ModuleEvents[ModuleNumber] * 68);
  retval = Pixie16GetEventsInfo(FileName, EventInformation, ModuleNumber);
  if(retval < 0)
  {
  // error handling
  }

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16GetModuleEvents
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

解析列表模式数据文件以获取事件信息

.. code:: cpp

  // 使用此功能可以解析列表模式数据文件中的列表模式事件。 将报告每个模块的事件数。
  int Pixie16GetModuleEvents (
        char *FileName, // the list mode data file name (with complete path)
        unsigned int *ModuleEvents ); // receives number of events for modules

**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 无效指针 ModuleEvents
   - 使用正确的 ModuleEvents
- -2 无法打开列表模式数据文件
   - 使用正确的文件名和路径

**示例代码**

.. code:: cpp

  int retval;
  char *FileName = {"C:\\XIA\\Pixie16\\PulseShape\\listmodedata.bin"};
  unsigned int ModuleEvents[7];// assume maximum number of modules is 7
   
  retval = Pixie16GetModuleEvents (FileName, ModuleEvents);
  if(retval < 0)
  {
  // error handling
  }	  

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16IMbufferIO
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

在主机和 DSP 内部存储器之间传输数据

.. code:: cpp

  // 使用此功能可直接在主机与 Pixie-16 模块的 DSP 内部存储器之间传输数据。 ModNum 是从 0 开始计数的模块号。
  // DSP 内部存储器分为两个块，第一个块的地址范围为 0x40000 至 0x4FFFF，第二个块的地址范围为 0x50000 至 0x5FFFF。 在第一个块中，地址范围 0x40000 到 0x49FFF 被保留用于程序存储器，并且不应由主机直接访问。 DSP I/O 参数使用地址范围 0x4A000 至 0x4A4FF，这些参数存储在主机的配置文件（.set文件）中。 在此范围内，可以读取和写入 0x4A000 至 0x4A33F，但是只能读取但不能写入 0x4A340 至 0x4A4FF。 第一块中的剩余地址范围（0x4A500 至 4FFFF）和整个第二块中的其余地址范围（0x50000 至 0x5FFFF）只能由主机读取，而不能写入。 将 Direction = 1 用于读取，将 Direction = 0 用于写入。
  int Pixie16IMbufferIO (
        unsigned int *Buffer,    //buffer data                       
        unsigned int NumWords,	 //number of buffer words to transfer
        unsigned int Address,	 //buffer address                    
        unsigned short Direction,//I/O direction                     
        unsigned short ModNum ); //module number   

**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 无效的指针 Buffer
   - 使用正确的指针 Buffer
- -2 Buffer 字数超过限制
   - 减少 Buffer 字长
- -3 无效的 DSP 内部存储器地址
   - 使用有效地址
- -4 无效的 I/O 方向
   - 使用有效方向
- -5 无效的 Pixie-16 模块编号
   - 使用正确的 ModNum
- -6 I/O 失败
   - 重新启动模块

**示例代码**

.. code:: cpp

  int retval;
  unsigned short Direction, ModNum;
  unsigned int DSPMemBlock1[65536], NumWords, Address;
   
  NumWords = 65536; //to read out block 1 of the DSP internal memory
  ModNum = 0;	  //the first module in the system                
  Address = 0x50000;//the starting address for block 1              
  Direction = 1;	  //I/O direction is read                         
   
  // read out the whole block 1 of the DSP internal memory
  retval = Pixie16IMbufferIO (DSPMemBlock1, NumWords, Address, Direction, ModNum);
  if(retval != 0)
  {
  // Error handling
  }

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16InitSystem
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

在 PXI 机箱中配置用于通信的模块

.. code:: cpp

  // 使用此功能可在 PXI 机箱中配置 Pixie-16 模块。
  // NumModules 是系统中安装的 Pixie-16 模块的总数。 PXISlotMap 是指向数组的指针，该数组必须至少具有与机箱中 Pixie-16 模块一样多的条目。
  // PXISlotMap 充当逻辑模块号和模块所驻留的物理插槽号的简单映射。逻辑模块号从 0 开始运行。例如，在具有 5 个 Pixie-16 模块的系统中，这 5 个模块可能占用插槽 3 到 7。用户必须按如下方式填充 PXISlotMap：PXISlotMap = {3、4、5、6、7 ...}，因为模块号 0 驻留在插槽号 3 中，依此类推。要找出模块位于哪个插槽中， 任何后续代码都可以使用表达式 PXISlotMap [ModNum]，其中 ModNum 是逻辑模块号。
  // OfflineMode 用于向 API 指示系统是在离线模式（1）还是在线模式（0）下运行。 离线模式对于没有 Pixie-16 模块但用户仍然可以在其应用程序软件中测试，对 API 函数的调用的情况很有用。
  // 在启动过程中，必须将此功能称为第一步。 它使系统知道模块并 “打开” 每个模块进行通讯。
  // 该功能依赖于一个初始化文件（pxisys.ini），该文件包含有关主机 PC 的 PCI 总线的信息，包括插槽枚举方案。 XIA 的软件发行版通常将此文件放在与 Pixie-16 软件安装文件夹相同的文件夹下。 但是，用户可以通过简单地在文件 pixie16sys.c 的顶部中更改字符串 PCISysIniFile 的定义来灵活地将其放置在其他文件夹中。
  int Pixie16InitSystem (
        unsigned short NumModules,   //total number of Pixie-16 modules                           
        unsigned short *PXISlotMap,  //an array containing the slot number of each Pixie-16 module
        unsigned short OfflineMode );//specify if using offline mode 

**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 Pixie-16 模块总数无效
   - 检查是否满足 NumModules <= PRESET_MAX_MODULES(在 pixie16app_defs.h 中定义)
- -2 空指针 PXISlotMap
   - 使用正确的 PXISlotMap
- -3 初始化系统失败
   - 检查错误消息日志文件 Pixie16msg.txt
     
**示例代码**

.. code:: cpp

  int retval;
  unsigned short NumModules, PXISlotMap[8], OfflineMode;
   
  // there are 5 modules in the system
  NumModules = 5;
   
  // specify the slot number for each module
  PXISlotMap[0] = 2;
  PXISlotMap[1] = 3;
  PXISlotMap[2] = 4;
  PXISlotMap[3] = 5;
  PXISlotMap[4] = 6;
   
  // running in online mode
  OfflineMode = 0;
   
  // configure the PXI slots in the chassis
  retval = Pixie16InitSystem (NumModules, PXISlotMap, OfflineMode);
  if(retval < 0)
  {
  // error handling
  }

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16LoadDSPParametersFromFile
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

从文件将DSP参数加载到模块

.. code:: cpp

  // 使用此功能可从设置文件读取 DSP 参数，然后将设置下载到系统中安装的 Pixie-16 模块。 每个模块具有正好 1280 个 DSP 参数值（32位无符号整数），并且根据 PRESET_MAX_MODULES 的值（在 pixie16app_defs.h 中定义），设置文件存储在计算机上时应正好具有（1280 * PRESET_MAX_MODULES * 4）个字节。
  int Pixie16LoadDSPParametersFromFile (
        char *FileName ); // DSP parameters file name (with complete path)

**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 DSPParFile 的大小无效
   - 使用正确的 DSPParFile
- -2 无法在模块中编程 Fippi
   - 重新启动模块
- -3 无法在模块中设置 DAC
   - 重新启动模块
- -4 打开 DSP 参数文件失败
   - 更正 DSP 参数文件名
     
**示例代码**

.. code:: cpp

  int retval;
  char *FileName = {"C:\\XIA\\Pixie16\\Configuration\\test.set"};
   
  retval = Pixie16LoadDSPParametersFromFile (FileName);
  if(retval < 0)
  {
  // error handling
  }

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16ProgramFippi
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

编程板载信号处理FPGA

.. code:: cpp

  // 使用此功能可以对 Pixie-16 模块的板载信号处理 FPGA 进行编程。 主机将 DSP 参数写入 DSP 存储器后，DSP 需要将其中一些参数写入 FPGA。 此功能使 DSP 执行该操作。 ModNum 是从 0 开始计数的模块号。
  int Pixie16ProgramFippi (
        unsigned short ModNum);// module number

**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 无效的 Pixie-16 模块编号
   - 使用正确的 ModNum
- -2 无法启动 PROGRAM_FIPPI 运行
   - 重新启动模块
- -3 PROGRAM_FIPPI 运行超时
   - 重新启动模块

**示例代码**

.. code:: cpp

  int retval;
  unsigned short ModNum;
  ModNum = 0;// the first module
   
  retval = Pixie16ProgramFippi (ModNum);
  if(retval < 0)
  {
  // error handling
  }

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16ReadCSR
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

从模块读取控制和状态寄存器值

.. code:: cpp

  // 使用此功能读取主机控制和状态寄存器（CSR）值。 该寄存器与 DSP 参数 ModCSRA/B，ChanCSRA/B 无关。 它用于控制模块的操作并由主机直接读取。不建议主机直接读取或写入，例如，使用 Pixie16CheckRunStatus 之类的功能来轮询活动位。
  int Pixie16ReadCSR (
        unsigned short ModNum,// module number     
        unsigned int *CSR );  // returned CSR value

**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 无效的 Pixie-16 模块编号
   - 使用正确的 ModNum

**示例代码**

.. code:: cpp

  unsigned short ModNum;
  unsigned int CSR;
  ModNum = 0;// the first module
  Pixie16ReadCSR (ModNum, &CSR);

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16ReadDataFromExternalFIFO
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

从模块的外部 FIFO 读取数据

.. code:: cpp

  // 使用此功能从模块的外部 FIFO 读取数据。
  // 此功能从 Pixie-16 模块的外部 FIFO 读取列表模式数据。 数据是 32 位无符号整数。通常，首先调用函数 Pixie16CheckExternalFIFOStatus 来查看外部 FIFO 当前有多少个字，然后调用此函数从 FIFO 中读取数据。 ModNum 是从 0 开始计数的模块号。
  int Pixie16ReadDataFromExternalFIFO (
        unsigned int *ExtFIFO_Data,// to receive the external FIFO data
        unsigned int nFIFOWords,// number of words to read from FIFO
        unsigned short ModNum );// module number

**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 无效的 Pixie-16 模块编号
   - 检查 ModNum
- -2 无法从外部 FIFO 读取数据
   - 检查错误消息日志文件 Pixie16msg.txt

**示例代码**

.. code:: cpp

  int retval;
  unsigned int nFIFOWords, *ExtFIFO_Data;
  unsigned short ModNum;
   
  ModNum = 0;// the first module in the system
  retval = Pixie16CheckExternalFIFOStatus (&nFIFOWords, ModNum);
  if(retval < 0)
  {
  // Error handling
  }
   
  if(nFIFOWords > 0) // Check if there is data in the external FIFO
  {
    ExtFIFO_Data =
    (unsigned int *)malloc(sizeof(unsigned int) * nFIFOWords);
    retval = Pixie16ReadDataFromExternalFIFO(ExtFIFO_Data,nFIFOWords,ModNum);
    if(retval != 0)
    {
    // Error handling
    }
  }

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16ReadHistogramFromFile
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

从直方图数据文件中读取直方图数据

.. code:: cpp

  // 使用此功能可以从直方图数据文件中读取直方图数据。在调用此函数之前，主机代码应分配适当的内存量以存储直方图数据。 默认直方图长度为 32768。直方图数据是 32 位无符号整数。
  // 使用 ModNum 指定模块，并使用 ChanNum 指定模块上的通道。请注意，模块和通道均从 0 开始计数。
  int Pixie16ReadHistogramFromFile (
        char *FileName,// histogram file name (with complete path)
        unsigned int *Histogram,// histogram data
        unsigned int NumWords,// number of words to be read out
        unsigned short ModNum,// module number
        unsigned short ChanNum);// channel number

**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 无法打开直方图数据文件
   - 更正直方图数据文件名
- -2 该通道没有直方图数据
   - 更改 ModNum 和 ChanNum

**示例代码**

.. code:: cpp

  int retval;
  char *FileName = {"C:\\XIA\\Pixie16\\MCA\\histogramdata.bin"};
  unsigned short ModNum, ChanNum;
  unsigned int NumWords, Histogram[32768];
  ModNum = 0;  // the first module  
  ChanNum = 0; // the first channel 
  NumWords = 32768;
   
  retval = Pixie16ReadHistogramFromFile (FileName, Histogram, NumWords,
  ModNum, ChanNum);
  if(retval < 0)
  {
  // error handling
  }

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16ReadHistogramFromModule
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

从模块读取直方图数据

.. code:: cpp

  // 使用此功能可以从 Pixie-16 模块的直方图存储器中读取直方图数据。在调用此函数之前，主机代码应分配适当的内存量以存储直方图数据。默认直方图长度为 32768。直方图数据是 32 位无符号整数。
  // 使用 ModNum 指定模块，并使用 ChanNum 指定模块上的通道。请注意，模块和通道均从 0 开始计数。	  
  int Pixie16ReadHistogramFromModule(
        unsigned int *Histogram,//histogram data
        unsigned int NumWords,//number of words to be read out
        unsigned short ModNum,//module number
        unsigned short ChanNum);//channel number

**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 无效的 Pixie-16 模块编号
   - 使用正确的 ModNum
- -2 无效的 Pixie-16 通道号
   - 使用正确的 ChanNum
- -3 无法获取直方图数据
   - 重新启动模块

**示例代码**

.. code:: cpp

  int retval;
  unsigned short ModNum, ChanNum;
  unsigned int NumWords, Histogram[32768];
   
  ModNum = 0; // the first module  
  ChanNum = 0;// the first channel 
  NumWords = 32768;
   
  retval = Pixie16ReadHistogramFromModule (Histogram, NumWords, ModNum, ChanNum);
  if(retval < 0)
  {
  // error handling
  }
  
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16ReadListModeTrace
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

从列表模式数据文件中读取波形数据

.. code:: cpp

  // 使用此功能从列表模式数据文件中检索列表模式的波形。它使用从函数 Pixie16GetEventsInfo 获得的选定事件的波形长度和文件位置信息。
  int Pixie16ReadListModeTrace (
        char *FileName,            //list mode data file name             
        unsigned short *Trace_Data,//list mode trace data (16-bit words)  
        unsigned short NumWords,   //number of 16-bit words to be read out
        unsigned int FileLocation);//the location of the trace in the file

**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 无法打开列表模式数据文件
   - 正确的文件名和路径

**示例代码**

.. code:: cpp

  int retval;
  char *FileName = {"C:\\XIA\\Pixie16\\PulseShape\\listmodedata.bin"};
  unsigned short ModuleNumber, ChannelNumber;
  unsigned int *EventInformation, FileLocation, EventNumber;
  unsigned short *Trace_Data, NumWords;
  unsigned int ModuleEvents[7]; // assume maximum number of modules is 7
   
  retval = Pixie16GetModuleEvents (FileName, ModuleEvents);
  if(retval < 0)
  {
  // error handling
  }
  ModuleNumber = 0;// the first module
  EventInformation = (unsigned int *)malloc(sizeof(unsigned int) * ModuleEvents[ModuleNumber]*68);
   
  retval = Pixie16GetEventsInfo (FileName, EventInformation, ModuleNumber);
  if(retval < 0)
  {
  // error handling
  }
   
  ChannelNumber = 0;// the first channel 
  EventNumber = 0;  // the first event   
   
  NumWords = (unsigned short)EventInformation[EventNumber*68 + 10] * 2;
   
  FileLocation = EventInformation[EventNumber*68 + 11];
  Trace_Data = (unsigned short *)malloc(sizeof(unsigned short) * NumWords);
  retval = Pixie16ReadListModeTrace (FileName,Trace_Data,NumWords,FileLocation);
  if(retval < 0)
  {
  // error handling
  }
  
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16ReadModuleInfo
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

读取有关存储在 EEPROM 中的模块信息

.. code:: cpp

  // 使用此功能可以读取存储在每个模块上的信息，包括其版本，序列号，ADC 位和采样率。这应该在初始化 PCI 通信之后完成。在启动模块之前，可以使用来自模块的信息来选择适当的固件，DSP 和配置参数文件。 ModNum 是从 0 开始计数的模块号。
  int Pixie16ReadModuleInfo (
        unsigned short ModNum,       //module number                    
        unsigned short *ModRev,	     //returned module revision         
        unsigned int *ModSerNum,     //returned module serial number    
        unsigned short *ModADCBits,  //returned module ADC bits         
        unsigned short *ModADCMSPS );//returned module ADC sampling rate

**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 无效的 Pixie 模块编号
   - 使用正确的 ModNum 
- -2 无法从 I2C 串行 EEPROM 读取
   - 检查错误消息日志文件 Pixie16msg.txt

**示例代码**

.. code:: cpp

  int retval;
  unsigned short ModuleNumber;
  unsigned short ModRev;
  unsigned int ModSerNum;
  unsigned short ModADCBits;
  unsigned short ModADCMSPS;
   
  retval = Pixie16ReadModuleInfo (ModuleNumber, &ModRev, &ModSerNum,
  &ModADCBits, &ModADCMSPS);
  if(retval < 0)
  {
  // error handling
  }

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16ReadSglChanADCTrace
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

从模块中的通道读取 ADC 波形数据

.. code:: cpp

  // 使用此功能从 Pixie-16 模块读取 ADC 波形数据。在调用此函数之前，应先调用另一个函数 Pixie16AcquireADCTrace 来填充 DSP 内部存储器。同样，主机代码应分配适当的内存量以存储波形数据。 每个通道的 ADC 波形数据长度为 8192。由于波形数据是 16 位无符号整数（对于少于 16 位 ADC 的硬件型号，只有低 12 位或 14 位包含实数据），因此两个连续的 16 位字被打包到 DSP 内部存储器中的一个 32 位字中。 因此，对于每个通道，首先从 DSP 读取 4096 个 32 位字，然后将每个 32 位字解包以形成两个 16 位字。
  // 使用 ModNum 指定模块，并使用 ChanNum 指定模块上的通道。请注意，模块和通道均从 0 开始计数。
  int Pixie16ReadSglChanADCTrace (
        unsigned short *Trace_Buffer, //trace data
        unsigned int Trace_Length,//number of trace data words to read
        unsigned short ModNum,//module number
        unsigned short ChanNum );//channel number

**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 无效的 Pixie-16 模块编号
   - 使用正确的 ModNum
- -2 无效的 Pixie-16 通道号
   - 使用正确的 ChanNum
- -3 无效的波形长度
   - 使用正确的 Trace_Length
- -4 分配内存以存储 ADC 波形失败
   - 关闭其他程序或重新启动计算机
- -5 读取 ADC 波形失败
   - 重新启动模块

**示例代码**

.. code:: cpp

  unsigned short NumWords, ModNum, ChanNum;
  int retval;
  unsigned short ADCTrace[8192];
  // assume we want to acquire ADC trace from channel 0 of module 0
  ModNum = 0;
  ChanNum = 0;
   
  // number of ADC trace words is 8192
  NumWords = 8192;
   
  // acquire the trace
  retval = Pixie16AcquireADCTrace(ModNum);
  if(retval < 0)
  {
  // error handling
  }
   
  // read out the trace
  retval = Pixie16ReadSglChanADCTrace(ADCTrace, NumWords, ModNum, ChanNum);
  if(retval < 0)
  {
  // error handling
  }

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16ReadSglChanBaselines
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

读取模块中一通道的基线

.. code:: cpp

  // 使用此功能可从 Pixie-16 模块读取基线数据。在调用该函数之前，应先调用另一个函数 Pixie16AcquireBaselines 来填充 DSP 内部存储器。另外，主机代码应分配适当的内存量以存储基线数据。每个通道的基线数目是 3640。在 DSP 内部存储器中，每个基准是 32 位 IEEE 浮点数。 在读出给主机后，此功能会将每个基线数据转换为十进制数。除了基线值，此函数调用后还会返回与每个基线相对应的时间戳。
  // 使用 ModNum 指定模块，并使用 ChanNum 指定模块上的通道。请注意，模块和通道从 0 开始计数。
  int Pixie16ReadSglChanBaselines(
        double *Baselines,       //returned baseline values             
        double *TimeStamps,	 //timestamps for each baseline value   
        unsigned short NumBases, //number of baseline data words to read
        unsigned short ModNum,	 //module number                        
        unsigned short ChanNum );//channel number  

**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 无效的 Pixie-16 模块编号
   - 使用正确的 ModNum
- -2 请求的基线数量超出限制
   - 使用正确的 NumBases
- -3 无法分配内存来存储基线
   - 关闭其他程序或重新启动计算机
- -4 无法读取基线
   - 重新启动模块
     
**示例代码**

.. code:: cpp

  unsigned short NumWords, ModNum, ChanNum;
  int retval;
  double Baselines[3640], TimeStamps[3640];
   
  // assume we want to acquire baselines for channel 0 of module 0
  ModNum = 0;
  ChanNum = 0;
   
  // number of baseline words is 3640
  NumWords = 3640;
   
  // acquire the baselines
  retval = Pixie16AcquireBaselines (ModNum);
  if(retval < 0)
  {
  // error handling
  }
   
  // read out the baselines
  retval = Pixie16ReadSglChanBaselines (Baselines, TimeStamps, NumWords, ModNum, ChanNum);
  if(retval < 0)
  {
  // error handling
  }
  
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16ReadSglChanPar
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

从模块读取 CHANNEL 级别参数

.. code:: cpp

  // 使用此功能可从 Pixie-16 模块读取通道参数。ModNum 是从 0 开始计数的模块号。ChanNum 是从 0 开始计数的通道号。
  int Pixie16ReadSglChanPar (
        char *ChanParName,       // channel parameter name 
        double *ChanParData,	 // channel parameter value
        unsigned short ModNum,	 // channel number         
        unsigned short ChanNum );// module number   

**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 无效的 Pixie-16 模块编号
   - 使用有效的 ModNum
- -2 无效的 Pixie-16 通道号
   - 使用有效的 ChanNum
- -3 无效的通道参数名称
   - 使用有效的 ChanParName
     
**示例代码**

.. code:: cpp

  int retval;
  unsigned short ModNum, ChanNum;
  double ChanParData;
   
  // read energy filter rise time from module 0 channel 0
  ModNum = 0;// this is the first module
  ChanNum = 0;// the first channel
   
  retval = Pixie16ReadSglChanPar ("ENERGY_RISETIME”, &ChanParData, ModNum, ChanNum);
  if(retval < 0)
  {
  // Error handling
  }
  
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16ReadSglModPar
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

从模块读取 MODULE 级别参数

.. code:: cpp

  // 使用此功能可从 Pixie-16 模块读取模块参数。ModNum 是从 0 开始计数的模块号。
  int Pixie16ReadSglModPar (
        char *ModParName,        // module parameter name 
        unsigned int *ModParData,// module parameter value
        unsigned short ModNum ); // module number      

**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 无效的 Pixie-16 模块编号
   - 使用正确的 ModNum
- -2 无效的模块参数名称
   - 使用正确的 ModParName

**示例代码**

.. code:: cpp

  int retval;
  unsigned short ModNum;
  unsigned int ModParData;
   
  // Read SlowFilterRange in module 0
  ModNum = 0;// this is the first module
   
  retval = Pixie16ReadSglModPar ("SLOW_FILTER_RANGE”, &ModParData, ModNum);
  if(retval < 0)
  {
  // Error handling
  }

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16ReadStatisticsFromModule
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

从模块读取运行统计数据

.. code:: cpp

  // 使用此功能可以从 Pixie-16 模块中读取统计数据。在调用此函数之前，主机代码应分配适当的内存量以存储统计数据。每个模块的统计数据数量固定为 448。统计数据是 32 位无符号整数。
  // 使用 ModNum 指定模块。 请注意，模块从 0 开始计数。
  int Pixie16ReadStatisticsFromModule (
        unsigned int *Statistics,// statistics data
        unsigned short ModNum ); // module number  

**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 无效的 Pixie-16 模块编号
   - 使用正确的 ModNum
- -2 无法获取统计数据
   - 重新启动模块
     
**示例代码**

.. code:: cpp

  int retval;
  unsigned short ModNum, ChanNum;
  unsigned int Statistics[448];
   
  ModNum = 0;// the first module
  ChanNum = 0;// the first channel
   
  retval = Pixie16ReadStatisticsFromModule (Statistics, ModNum);
  if(retval < 0)
  {
  // error handling
  }


:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16RegisterIO
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

读取或写入模块上的寄存器

.. code:: cpp

  // 使用此功能可从 Pixie-16 模块中的寄存器读取数据或将数据写入其中。
  // 使用 ModNum 指定模块。 请注意，模块从 0 开始计数。
  int Pixie16RegisterIO (
        unsigned short ModNum,   //module number             
        unsigned int address,	 //register address          
        unsigned short direction,//read or write             
        unsigned int *value );	 //holds or receives the data

**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 无效的 Pixie-16 模块编号
   - 使用正确的 ModNum

**示例代码**

.. code:: cpp

  int retval;
  unsigned short ModNum;
  unsigned int address, value;
  ModNum = 0;// the first module
  address = PCI_STOPRUN_REGADDR;// address of the register for ending run
  value = 0;
   
  retval = Pixie16RegisterIO(ModNum, address, MOD_WRITE, &value);
  if(retval < 0)
  {
  // error handling
  }
  
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16SaveDSPParametersToFile
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

从模块读取 DSP 参数并保存到文件

.. code:: cpp

  // 使用此功能可将 DSP 参数保存到设置文件中。它将首先读取每个 Pixie-16 模块上的 DSP 参数值，然后将它们写入设置文件。每个模块具有正好 1280 个 DSP 参数值（32位无符号整数），并且根据 PRESET_MAX_MODULES 的值（在 pixie16app_defs.h中定义），设置文件存储在计算机上时应正好具有（1280 * PRESET_MAX_MODULES * 4）个硬盘字节。
  int Pixie16SaveDSPParametersToFile (
        char *FileName ); // DSP parameters file name (with complete path)

**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 无法从 Pixie-16 模块读取 DSP 参数值
   - 重新启动模块
- -2 打开 DSP 参数文件失败
   - 更正 DSP 参数文件名

**示例代码**

.. code:: cpp

  int retval;
  char *FileName = {"C:\\XIA\\Pixie16\\Configuration\\test.set"};
   
  retval = Pixie16SaveDSPParametersToFile (FileName);
  if(retval < 0)
  {
  // error handling
  }

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16SaveExternalFIFODataToFile
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

从外部 FIFO 读取数据并保存到文件

.. code:: cpp

  // Use this function to read data from the external FIFO of a module. ModNum is the module number which starts counting at 0.
  // 此函数首先检查 Pixie-16 模块的外部 FIFO 的状态，如果外部 FIFO 中有数据，则此函数从外部 FIFO 读取列表模式数据（32位无符号整数）。因此，此函数实质上将两个函数 Pixie16CheckExternalFIFOStatus 和 Pixie16ReadDataFromExternalFIFO 封装在一个函数中。从外部 FIFO 读取的字数记录在变量 *FIFOWords 中。该函数还希望设置一个名为 “EndOfRunRead”的变量的值，以指示该读取是在运行结束时（1）还是在运行期间（0）。这是必要的，因为当主机由于其流水线结构而从外部 FIFO 读取最后几个字时，需要对外部 FIFO 进行特殊处理。
  int Pixie16SaveExternalFIFODataToFile (
        char *FileName,// list mode data file name
        unsigned int *nFIFOWords,// number of words read from ext. FIFO
        unsigned short ModNum,// module number
        unsigned short EndOfRunRead ); // indicator if this is end of run read

**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 无效的 Pixie-16 模块编号
   - 使用正确的 ModNum
- -2 分配内存以存储列表模式数据失败
   - 检查计算机资源
- -3 无法打开列表模式数据文件
   - 检查文件是否受保护
- -4 读取外部 FIFO 状态失败
   - 检查错误消息日志文件 Pixie16msg.txt
- -5 无法从外部 FIFO 读取数据
   - 检查错误消息日志文件 Pixie16msg.txt
     
**示例代码**

.. code:: cpp

  int retval;
  unsigned int nFIFOWords;
  unsigned short ModNum, EndOfRunRead;
  ModNum = 0;// the first module in the system
  EndOfRunRead = 0;// this is a read during the run
   
  retval = Pixie16SaveExternalFIFODataToFile("listmodedata_mod0.bin", &nFIFOWords, ModNum, EndOfRunRead);
  if(retval < 0)
  {
  // Error handling
  }

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16SaveHistogramToFile
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

从模块读取直方图数据并将其保存到文件

.. code:: cpp

  // 使用此功能可从 Pixie-16 模块读取直方图数据并将直方图数据保存到用户指定文件名的文件中：首先，此功能将直方图数据保存到二进制文件，然后将直方图数据保存到带有运行统计数据的 ASCII 文件附加到 ASCII 文件的末尾。现有文件将被覆盖。 ModNum 是从 0 开始计数的模块号。
  int Pixie16SaveHistogramToFile (
        char *FileName,// histogram data file name (with complete path)
        unsigned short ModNum); // module number

**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 无效的 Pixie-16 模块编号
   - 使用正确的 ModNum
- -2 无法从模块获取直方图数据
   - 重新启动模块
- -3 分配内存以存储直方图数据失败
   - 关闭其它程序或重新启动计算机
- -4 无法打开直方图数据文件
   - 使用正确的文件名和路径
- -5 无法打开 mca ascii 输出文件
   - 使用正确的文件名和路径
- -6 无法分配用于存储 ASCII 文本文件的直方图数据的内存
   - 关闭其他程序或重新启动计算机
- -7 无法从文件读取直方图数据
   - 检查文件名和路径
- -8 无法从模块读取运行统计数据
   - 重新启动模块

**示例代码**

.. code:: cpp

  int retval;
  char *FileName = {"C:\\XIA\\Pixie16\\MCA\\histogramdata.bin"};
  unsigned short ModNum;
  ModNum = 0;// the first module
   
  retval = Pixie16SaveHistogramToFile (FileName, ModNum);
  if(retval < 0)
  {
  // error handling
  }

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16SetDACs
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

编程板载 DAC

.. code:: cpp

  // 使用此功能可以对 Pixie-16 模块的板载数模转换器（DAC）进行重新编程。 在此操作中，DSP 使用来自先前下载的 DSP 参数的数据。 ModNum 是从 0 开始计数的模块号。
  int Pixie16SetDACs (
        unsigned short ModNum);// module number

**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 无效的 Pixie-16 模块编号
   - 使用正确的 ModNum
- -2 无法启动 SET_DACS 运行
   - 重新启动模块
- -3 SET_DACS run timed out
   - 重新启动模块

**示例代码**

.. code:: cpp

  int retval;
  unsigned short ModNum;
  ModNum = 0;// the first module
   
  retval = Pixie16SetDACs (ModNum);
  if(retval < 0)
  {
  // error handling
  }

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16StartHistogramRun
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

开始 MCA 直方图模式数据采集运行

.. code:: cpp

  // 使用此功能可以开始进行数据采集运行，每个通道一个累积能量直方图。它启动数据采集运行，其中仅保存能量信息，并在每个通道本地对直方图进行直方图处理。
  // 为系统中的每个 Pixie-16 模块调用此函数以初始化每个模块中的运行。当最后一个模块完成初始化后，实际的数据采集将在所有模块中同步开始（需要设置同步参数）。当经过的运行时间超过预设的运行时间时，直方图运行可以自行终止，或者用户可以通过调用Pixie16EndRun 提前终止运行。完成后，最终的直方图和统计数据将可用。
  // 在启动新运行之前，请使用 mode = NEW_RUN（= 1）删除直方图和统计信息。 使用 mode = RESUME_RUN（= 0）恢复较早的运行。
  // ModNum 是从 0 开始计数的模块号。如果 ModNum 设置为小于系统中模块的总数，则只有 ModNum 指定的模块才能启动其直方图。 但是，如果将 ModNum 设置为等于系统中模块的总数，则系统中的所有模块将一起开始运行。
  int Pixie16StartHistogramRun (
        unsigned short ModNum,// module number
        unsigned short mode );// run mode

**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 无效的 Pixie-16 模块编号
   - 使用正确的 ModNum
- -2 无效的 run mode
   - 使用正确的 run mode
- -3 无法开始直方图运行
   - 重新启动模块

**示例代码**

.. code:: cpp

  int retval;
  unsigned short mode, ModNum;
  double preset_run_time;
  unsigned int ieee_preset_run_time;
   
  mode = NEW_RUN;// to start a new run
  // Assume there are 5 modules in the system
  ModNum = 5;// start histogram run in all 5 modules
   
  // Assume preset run time is 10 seconds
  preset_run_time = 10.0;
   
  // Convert preset run time to IEEE 32-bit floating point number
  ieee_preset_run_time = Decimal2IEEEFloating (preset_run_time);
   
  // Download the preset run time to the DSP
  retval = Pixie16WriteSglModPar("HOST_RT_PRESET", ieee_preset_run_time, ModNum)
  if(retval < 0)
  {
  // Error handling
  }
   
  // Start the histogram run
  retval = Pixie16StartHistogramRun (ModNum, mode);
  if(retval < 0)
  {
  // Error handling
  }
  
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16StartListModeRun
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

开始列表模式数据采集运行

.. code:: cpp

  // 使用此功能可启动在 Pixie-16 模块中运行的列表模式数据采集。列表模式运行用于逐事件收集数据，收集每个事件的能量，时间戳，脉冲形状分析值和波形。运行将一直持续到用户通过调用函数 Pixie16EndRun 终止运行为止。要开始数据采集，必须为系统中的每个 Pixie-16 模块调用此功能。如果所有模块都将同步运行，则最后寻址的模块将释放所有其它模块，然后开始采集。如果已在所有其它模块中设置了运行同步，则第一个结束运行的模块将立即停止所有其他模块中的运行。
  // 在启动新运行之前，请使用 mode = NEW_RUN（= 1）删除直方图和统计信息。请注意，这将导致最多 1 毫秒的启动延迟。使用 mode = RESUME_RUN（= 0）恢复较早的运行。此模式的启动延迟仅为几微秒。
  // 仅支持一种列表模式运行类型，即 0x100。 但是，可以通过启用或禁用不同的 CHANCSRA 位来选择不同的输出数据选项。
  // ModNum 是从 0 开始计数的模块号。如果 ModNum 设置为小于系统中模块的总数，则只有 ModNum 指定的模块才能启动其列表模式运行。但是，如果 ModNum 设置为等于系统中模块的总数，则系统中的所有模块将一起开始运行。
  int Pixie16StartListModeRun (
        unsigned short ModNum, // module number
        unsigned short RunType,// run type     
        unsigned short mode ); // run mode     

**返回值描述及错误处理方法**

- 0 Success
   - None
- -1 Invalid Pixie-16 module number
   - Correct the ModNum
- -2 Invalid run mode
   - Correct the run mode
- -3 Failed to start list mode run
   - Reboot the module
- -4 Invalid run type
   - Correct RunType

**示例代码**

.. code:: cpp

  int retval;
  unsigned short mode, ModNum, RunType;
   
  mode = NEW_RUN;// to start a new run
  RunType = 0x100;// general purpose list mode run
   
  // Assume there are 5 modules in the system
  ModNum = 5;
   
  // start list mode run in all 5 modules
  retval = Pixie16StartListModeRun (ModNum, RunType, mode);
  if(retval < 0)
  {
  // Error handling
  }
  
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16TauFinder
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

找出通道输入信号的指数衰减时间

.. code:: cpp

  // 使用此功能可以找到与 Pixie-16 模块的 16 个通道中的每个通道相连的探测器或前置放大器信号的指数衰减时间常数（Tau 值）。 找到的 16 个 Tau 值通过指针 *Tau 返回。 通道的 “-1.0” Tau 值表示该通道的 Tau_Finder 不成功。 ModNum 是从 0 开始计数的模块号。
  void Pixie16TauFinder (
        unsigned short ModNum,// module number
        double *Tau );// Tau value

**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 无效的 Pixie-16 模块编号
   - 使用正确的 ModNum
- -2 无效的 Pixie-16 通道号
   - 使用正确的 ChanNum
- -3 无法获取 ADC 波形
   - 重新启动模块
- -4 读取 ADC 波形失败
   - 重新启动模块
- -5 找不到足够的脉冲数
   - 增加输入计数率

**示例代码**

.. code:: cpp

  int retval;
  unsigned short ModNum;
  double Tau[16];
  ModNum = 0;// the first module
   
  retval = Pixie16TauFinder(ModNum, Tau);
  if(retval < 0)
  {
  // Error handling
  }


:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16WriteCSR
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

写入模块中的控制和状态寄存器

.. code:: cpp

  // 使用此功能可将值写入主机控制和状态寄存器（CSR）。该寄存器与 DSP 参数 ModCSRA/B，ChanCSRA/B 无关。它用于控制模块的操作并由主机直接读取。不建议主机直接读取或写入，例如，使用 Pixie16CheckRunStatus 之类的功能来轮询活动位。ModNum是从 0 开始计数的模块号。
  void Pixie16WriteCSR (
        unsigned short ModNum,// module number
        unsigned int CSR );   // CSR value to write

**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 无效的 Pixie-16 模块编号
   - 使用正确的 ModNum
     
**示例代码**

.. code:: cpp

  int retval;
  unsigned short ModNum;
  unsigned int CSR;
   
  ModNum = 0;// the first module
  retval = Pixie16ReadCSR(ModNum, &CSR);
  if(retval < 0)
  {
  // Error handling
  }
   
  CSR = APP32_ClrBit(3, CSR);
  retval = Pixie16WriteCSR (ModNum, CSR);
  if(retval < 0)
  {
  // Error handling
  }
  
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16WriteSglChanPar
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

将CHANNEL级别参数写入模块

.. code:: cpp

  // 使用此功能可将通道参数写入 Pixie-16 模块。 ModNum 是从 0 开始计数的模块号。ChanNum 是从 0 开始计数的通道号。
  int Pixie16WriteSglChanPar (
        char *ChanParName,       //channel parameter name 
        double ChanParData,	 //channel parameter value
        unsigned short ModNum,	 //channel number         
        unsigned short ChanNum );//module number    

**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 无效的 Pixie-16 模块编号
   - 使用正确的 ModNum
- -2 无效的 Pixie-16 通道号
   - 使用正确的 ChanNum
- -3 无效的通道参数名称
   - 使用正确的 ChanParName
- -4 编程 Fippi 失败下载通道参数
   - 重新启动模块
- -5 下载通道参数后找不到 BLcut
   - 重新启动模块
- -6 SetDACs 下载通道参数失败
   - 重新启动模块
     
**示例代码**

.. code:: cpp

  int retval;
  unsigned short ModNum, ChanNum;
  double ChanParData;
  // Set energy filter rise time to 6.08 us for module 0 channel 0
  ModNum = 0;// this is the first module
  ChanNum = 0;// the first channel
  ChanParData = 6.08; // energy filter rise time = 6.08 us
   
  retval = Pixie16WriteSglChanPar ("ENERGY_RISETIME", ChanParData, ModNum, ChanNum);
  if(retval < 0)
  {
  // Error handling
  }

  
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16WriteSglModPar
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

将MODULE级别参数写入模块

.. code:: cpp

  // 使用此功能可将模块参数写入 Pixie-16 模块。 ModNum 是从 0 开始计数的模块号。
  int Pixie16WriteSglModPar (
        char *ModParName,       // module parameter name 
        unsigned int ModParData,// module parameter value
        unsigned short ModNum );// module number         

**返回值描述及错误处理方法**

- 0 成功
   - 没有
- -1 无效的 Pixie-16 模块编号
   - 使用正确的 ModNum
- -2 无效的模块参数名称
   - 使用正确的 ModParName
- -3 下载模块参数后无法编程 Fippi
   - 重新启动模块
- -4 下载模块参数后找不到 BLcut
   - 重新启动模块
     
**示例代码**

.. code:: cpp

  int retval;
  unsigned short ModNum;
  unsigned int ModParData;
  // Set SlowFilterRange in module 0 to 4
  ModNum = 0;// this is the first module
  ModParData = 4;// SlowFilterRange = 4
  
  retval = Pixie16WriteSglModPar ("SLOW_FILTER_RANGE", ModParData, ModNum);
  if(retval < 0)
  {
  // Error handling
  }
  
----------------------------------------------------------------------
PKU API
----------------------------------------------------------------------

Add by Hongyi Wu

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
HongyiWuPixie16ComputeFastFiltersOffline
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::  

.. code:: cpp
	
  int HongyiWuPixie16ComputeFastFiltersOffline (
	char           *FileName,          // the list mode data file name (with complete path)
	unsigned short ModuleNumber,       // the module whose events are to be analyzed
	unsigned short ChannelNumber,      // the channel whose events are to be analyzed
	unsigned int   FileLocation,       // the location of the trace in the file
	unsigned short RcdTraceLength,     // recorded trace length
	unsigned short *RcdTrace,          // recorded trace
	double         *fastfilter,        // fast filter response, the same scale as the trigger threshold
	double         *cfd,               // cfd response, the same scale as the CFD threshold
	double         *cfds );            // cfd response, the same scale as the fast filter


:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
HongyiWuPixie16ComputeSlowFiltersOffline
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.. code:: cpp
	  
  int HongyiWuPixie16ComputeSlowFiltersOffline (
	char           *FileName,          // the list mode data file name (with complete path)
	unsigned short ModuleNumber,       // the module whose events are to be analyzed
	unsigned short ChannelNumber,      // the channel whose events are to be analyzed
	unsigned int   FileLocation,       // the location of the trace in the file
	unsigned short RcdTraceLength,     // recorded trace length
	unsigned short *RcdTrace,          // recorded trace
	double         *slowfilter );      // slow filter response

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
HongyiWuPixie16ComputeSlowFiltersOfflineAverageBaseline
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	
.. code:: cpp
	  
  int  HongyiWuPixie16ComputeSlowFiltersOfflineAverageBaseline (
	char           *FileName,          // the list mode data file name (with complete path)
	unsigned short ModuleNumber,       // the module whose events are to be analyzed
	unsigned short ChannelNumber,      // the channel whose events are to be analyzed
	unsigned int   FileLocation,       // the location of the trace in the file
	unsigned short RcdTraceLength,     // recorded trace length
	unsigned short *RcdTrace,          // recorded trace
	double         *slowfilter,        // slow filter response
	int            pointtobl );        // Average number of estimated waveform baselines
	
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
HongyiWuPixie16ComputeSlowFiltersOfflineExtendBaseline
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.. code:: cpp
	    
  int HongyiWuPixie16ComputeSlowFiltersOfflineExtendBaseline (
	char           *FileName,          // the list mode data file name (with complete path)
	unsigned short ModuleNumber,       // the module whose events are to be analyzed
	unsigned short ChannelNumber,      // the channel whose events are to be analyzed
	unsigned int   FileLocation,       // the location of the trace in the file
	unsigned short RcdTraceLength,     // recorded trace length
	unsigned short *RcdTrace,          // recorded trace
	double         *slowfilter,        // slow filter response
	unsigned int   bl,                 // The baseline calculated in the firmware
	double         sl,                 // SL used to calculate the baseline in the firmware
	double         sg,                 // SG used to calculate the baseline in the firmware 
	double         tau,                // TAU used to calculate the baseline in the firmware
	int            sfr,                // SlowFilterRange used to calculate the baseline in the firmware
	int            pointtobl );        // Average number of estimated waveform baselines

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
HongyiWuPixie16SetOfflineVariant
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::  
  
.. code:: cpp
	  
  int HongyiWuPixie16SetOfflineVariant(unsigned short mod,unsigned short variant,unsigned short bits,unsigned short samplerate);  
  




.. 
.. XIAAPI.rst ends here
