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
PIXIE-16 API
=================================

It from **Pixie-16 Programmer’s Manual** *Version 3.06, September 13, 2019*

----

----------------------------------------------------------------------
XIA API
----------------------------------------------------------------------


:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16AcquireADCTrace
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Acquire ADC traces in single or multiple modules

.. code:: cpp

  // Use this function to acquire ADC traces from Pixie-16 modules. Specify the module using ModNum which starts counting at 0. If ModNum is set to be less than the total number of modules in the system, only the module specified by ModNum will have its ADC traces acquired. But if ModNum is equal to the total number of modules in the system, then all modules in the system will have their ADC traces acquired.
  // After the successful return of this function, the DSP’s internal memory will be filled with ADC trace data. A user’s application software should then call the function Pixie16ReadSglChanADCTrace to read the ADC trace data out to the host computer, channel by channel.
  int Pixie16AcquireADCTrace (
        unsigned short ModNum );// module number
   
**Return values description and error handling**

- 0 Success
   - None
- -1 Invalid Pixie-16 module number
   - Correct ModNum
- -2 Failed to start run
   - Reboot the modules
- -3 Acquiring ADC traces timed out
   - Reboot the modules

**Usage example**

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

Acquire baselines from a module

.. code:: cpp

  // Use this function to acquire baselines from Pixie-16 modules. Specify the module using ModNum which starts counting at 0. If ModNum is set to be less than the total number of modules in the system, only the module specified by ModNum will have its baselines acquired. But if ModNum is set to be equal to the total number of modules in the system, then all modules in the system will have their baselines acquired.
  // After the successful return of this function, the DSP’s internal memory will be filled with baselines data. Users should then call the function Pixie16ReadSglChanBaselines to read the baselines data out to the host computer, channel by channel.
  int Pixie16AcquireBaselines (
        unsigned short ModNum );// module number

**Return values description and error handling**

- 0 Success
   - None
- -1 Invalid Pixie-16 module number
   - Correct ModNum
- -2 Failed to start the GET_BASELINES run
   - Reboot the modules
- -3 GET_BASELINES run timed out
   - Reboot the modules

**Usage example**

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

Adjust DC-offsets in single or multiple modules

.. code:: cpp

  // Use this function to adjust the DC-offsets of Pixie-16 modules. Specify the module using ModNum which starts counting at 0. If ModNum is set to be less than the total number of modules in the system, only the module specified by ModNum will have its DC-offsets adjusted. But if ModNum is set to be equal to the total number of modules in the system, then all modules in the system will have their DC-offsets adjusted.
  // After the DC-offset levels have been adjusted, the baseline level of the digitized input signals will be determined by the DSP parameter BaselinePercent. For instance, if BaselinePercent is set to 10(%), the baseline level of the input signals will be ~409 on the 12-bit ADC scale (minimum: 0; maximum: 4095).
  // The main purpose of this function is to ensure the input signals fall within the voltage range of the ADCs so that all input signals can be digitized by the ADCs properly.
  int Pixie16AdjustOffsets (
        unsigned short ModNum ); // module number

**Return values description and error handling**

- 0 Success
   - None
- -1 Invalid Pixie-16 module number
   - Check if ModNum is valid
- -2 Failed to start the ADJUST_OFFSETS run
   - Reboot the module
- -3 ADJUST_OFFSETS run timed out
   - Check error message log file Pixie16msg.txt

**Usage example**

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

Compute new Baseline Cut values of a module

.. code:: cpp

  // Use this function to find the Baseline Cut value for one channel of a Pixie-16 module. The baseline cut value is then downloaded to the DSP, where baselines are captured and averaged over time. The cut value would prevent a bad baseline value from being used in the averaging process, i.e., if a baseline value is outside the baseline cut range, it will not be used for computing the baseline average. Averaging baselines over time improves energy resolution measurement.
  // ModNum is the module number which starts counting at 0. ChanNum is the channel number which starts counting at 0.
  int Pixie16BLcutFinder (
        unsigned short ModNum, // module number
        unsigned short ChanNum,// channel number
        unsigned int *BLcut ); // returned BLcut value
        
**Return values description and error handling**

- 0 Success
   - None
- -1 Invalid Pixie-16 module number
   - Check if ModNum is valid
- -2 Failed to collect baselines
   - Reboot the module
- -3 Failed to read baselines from the data memory
   - Check error message log file Pixie16msg.txt

**Usage example**

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

Boot modules so that they can be set up for data taking

.. code:: cpp

  // Use this function to boot Pixie-16 modules so that they can be set up for data taking. The function downloads to the Pixie-16 modules the communication (or system) FPGA configurations, signal processing FPGA configurations, trigger FPGA configurations (Revision A modules only), executable code for the digital signal processor (DSP), and DSP parameters.
  // The FPGA configurations consist of a fixed number of words depending on the hardware mounted on the modules; the DSP codes have a length which depends on the actual compiled code; and the set of DSP parameters always consists of 1280 32-bit words for each module. The host software has to make the names of those boot data files on the hard disk available to the boot function.
  // ModNum is the module number which starts counting at 0. If ModNum is set to be less than the total number of modules in the system, only the module specified by ModNum will be booted. But if ModNum is equal to the total number of modules in the system, e.g. there are 5 modules in the chassis and ModNum = 5, then all modules in the system will be booted.
  // The boot pattern is a bit mask (shown below) indicating which on-board chip will be booted. Under normal circumstances, all on-board chips should be booted, i.e. the boot pattern would be 0x7F. For Rev-B, C, D, F modules, bit 1, i.e., “Boot trigger FPGA”, will be ignored even if that bit is set to 1.
  // bit 0: Boot communication FPGA, All Pixie-16 Revisions
  // bit 1: Boot trigger FPGA, Pixie-16 Revision A only
  // bit 2: Boot signal processing FPGA, All Pixie-16 Revisions
  // bit 3: Boot digital signal processor (DSP), All Pixie-16 Revisions
  // bit 4: Download DSP I/O parameters, All Pixie-16 Revisions
  // bit 5: Program on-board FPGAs, All Pixie-16 Revisions
  // bit 6: Set on-board DACs, All Pixie-16 Revisions

  int Pixie16BootModule (
        char *ComFPGAConfigFile,     // name of ComFPGA configuration file
        char *SPFPGAConfigFile,	     // name of SPFPGA configuration file 
        char *TrigFPGAConfigFile,    // name of trigger FPGA file         
        char *DSPCodeFile,	     // name of DSP code file             
        char *DSPParFile,	     // name of DSP parameter file        
        char *DSPVarFile,	     // name of DSP variable names file   
        unsigned short ModNum,	     // pixie-16 module number            
        unsigned short BootPattern); // boot pattern bit mask             

**Return values description and error handling**
  
- 0 Success
   - None
- -1 Invalid Pixie-16 module number
   - Correct ModNum
- -2 Size of ComFPGAConfigFile is invalid
   - Correct ComFPGAConfigFile
- -3 Failed to boot Communication FPGA
   - Check log file (Pixie16msg.txt)
- -4 Failed to allocate memory to store data in ComFPGAConfigFile
   - Close other programs or reboot the computer
- -5 Failed to open ComFPGAConfigFile
   - Correct ComFPGAConfigFile
- -10 Size of SPFPGAConfigFile is invalid
   - Correct SPFPGAConfigFile
- -11 Failed to boot signal processing FPGA
   - Check log file (Pixie16msg.txt)
- -12 Failed to allocate memory to store data in SPFPGAConfigFile
   - Close other programs or reboot the computer
- -13 Failed to open SPFPGAConfigFile
   - Correct SPFPGAConfigFile
- -14 Failed to boot DSP
   - Check log file (Pixie16msg.txt)
- -15 Failed to allocate memory to store DSP executable code
   - Close other programs or reboot the computer
- -16 Failed to open DSPCodeFile
   - Correct DSPCodeFile
- -17 Size of DSPParFile is invalid
   - Correct DSPParFile
- -18 Failed to open DSPParFile
   - Correct DSPParFile
- -19 Can't initialize DSP variable indices
   - Correct DSPVarFile
- -20 Can't copy DSP variable indices
   - Check log file (Pixie16msg.txt)
- -21 Failed to program Fippi in a module
   - Check log file (Pixie16msg.txt)
- -22 Failed to set DACs in a module
   - Check log file (Pixie16msg.txt)
- -23 Failed to start RESET_ADC run in a module
   - Check log file (Pixie16msg.txt)
- -24 RESET_ADC run timed out in a module
   - Check log file (Pixie16msg.txt)
     
**Usage example**

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

  // Use this function to check the status of the external FIFO of a Pixie-16 module while a list mode data acquisition run is in progress. The function returns the number of words (32-bit) that the external FIFO currently has. If the number of words is greater than a user-set threshold, function Pixie16ReadDataFromExternalFIFO can then be used to read the data from the external FIFO.
  // The threshold can be set by the user to either minimize reading overhead or to read data out of the FIFO as quickly as possible. The Pixie-16 API (pixie16app_defs.h) has defined a threshold with value of 1024 for external FIFO read out (EXTFIFO_READ_THRESH).
  // *nFIFOWords returns the number of 32-bit words that the external FIFO currently has.
  // ModNum is the module number which starts counting at 0.
  int Pixie16CheckExternalFIFOStatus (
        unsigned int *nFIFOWords,  // returned number of words in the FIFO
        unsigned short ModNum );   // module number

**Return values description and error handling**
  
- 0 Success
   - None
- -1 Invalid Pixie-16 module number
   - Correct ModNum

**Usage example**

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

Check status of a data acquisition run

.. code:: cpp

  // Use this function to check the run status of a Pixie-16 module while a list mode data acquisition run is in progress. If the run is still in progress continue polling.
  // If the return code of this function indicates the run has finished, there might still be some data in the external FIFO (Rev-B, C, D, F modules) that need to be read out to the host computer. In addition, final run statistics and histogram data are available for reading out too.
  // In MCA histogram run mode, this function can also be called to check if the run is still in progress even though it is normally self-terminating.
  // ModNum is the module number which starts counting at 0.
  int Pixie16CheckRunStatus (
        unsigned short ModNum ); // module number	  

**Return values description and error handling**

- 0 No run is in progress
   - None
- 1 Run is still in progress
   - None
- -1 Invalid Pixie-16 module number
   - Correct ModNum

**Usage example**

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

Compute fast filter response for offline analysis

.. code:: cpp

  // Use this function to compute fast filter responses of each event in the list mode data file for offline analysis. The algorithm implemented in this offline analysis function is equivalent to the one implemented in the Pixie-16 hardware. So this function can be used to analyze how the leading edge fast trigger filter and the CFD filter implemented in the hardware respond to the shape of recorded traces. By analyzing the response of these filters, it is possible to optimize the performance of the leading edge trigger or CFD trigger by adjusting the fast filter and CFD parameters offline. Such optimized parameters can then be saved to a settings file to be used for online data acquisition.
  int Pixie16ComputeFastFiltersOffline (
        char *FileName, // the list mode data file name (with complete path)
        unsigned short ModuleNumber, // the module to be analyzed
        unsigned short ChannelNumber, // the channel to be analyzed
        unsigned int FileLocation, // the location of the trace in the file
        unsigned short RcdTraceLength,// recorded trace length
        unsigned short *RcdTrace, // recorded trace
        double *fastfilter, // fast filter response
        double *cfd ); // cfd response

**Return values description and error handling**

- 0 Success
   - None
- -1 Null pointer RcdTrace
   - Make sure RcdTrace has been allocated enough memory (maximum possible length is 32768, but can be shorter to match the actual trace length)
- -2 Null pointer fastfilter
   - Make sure fastfilter has been allocated enough memory (maximum possible length is 32768, but can be shorter to match the actual trace length)
- -3 Null pointer cfd
   - Make sure cfd has been allocated enough memory (maximum possible length is 32768, but can be shorter to match the actual trace length)
- -4 Target module number is invalid
   - Correct ModuleNumber
- -5 Trace length is too short
   - The length of recorded trace is too short for the offline analysis using the fast filter length (fast filter rise time and flat top). Either a different settings file with shorter fast filter length has to be used, or new traces with longer trace length have to be acquired
- -6 Failed to open list mode data file
   - Check the list mode file name and its path are correct

**Usage example**

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

Compute input count rate of a channel

.. code:: cpp

  // Use this function to calculate the input count rate on one channel of a Pixie-16 module. This function does not communicate with Pixie-16 modules. Before calling this function, another function, Pixie16ReadStatisticsFromModule, should be called to read statistics data from the module.
  // *Statistics is a pointer to an array whose size is exactly 448 unsigned integer words (32-bit). The *Statistics array is filled with data from a Pixie-16 module after calling function Pixie16ReadStatisticsFromModule. ModNum is the module number which starts counting at 0. ChanNum is the channel number which starts counting at 0.
  double Pixie16ComputeInputCountRate (
        unsigned int *Statistics,// run statistics data
        unsigned short ModNum,	 // module number      
        unsigned short ChanNum );// channel number     


**Return values description and error handling**

The return value is the input count rate in counts per second.

**Usage example**

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

Compute live time that a channel accumulated in a run

.. code:: cpp

  // Use this function to calculate the live time that one channel of a Pixie-16 module has spent on data acquisition. This function does not communicate with Pixie-16 modules. Before calling this function, another function, Pixie16ReadStatisticsFromModule, should be called to read statistics data from the module.
  // *Statistics is a pointer to an array whose size is exactly 448 unsigned integer words (32-bit). The *Statistics array is filled with data from a Pixie-16 module after calling function Pixie16ReadStatisticsFromModule. ModNum is the module number which starts counting at 0. ChanNum is the channel number which starts counting at 0.
  double Pixie16ComputeLiveTime (
        unsigned int *Statistics, // run statistics data
        unsigned short ModNum,	  // module number      
        unsigned short ChanNum ); // channel number     

**Return values description and error handling**

The return value is the live time in seconds.

**Usage example**

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

Compute output count rate of a channel

.. code:: cpp

  // Use this function to calculate the output count rate on one channel of a Pixie-16 module. This function does not communicate with Pixie-16 modules. Before calling this function, another function, Pixie16ReadStatisticsFromModule, should be called to read statistics data from the module.
  // *Statistics is a pointer to an array whose size is exactly 448 unsigned integer words (32-bit). The *Statistics array is filled with data from a Pixie-16 module after calling function Pixie16ReadStatisticsFromModule. ModNum is the module number which starts counting at 0. ChanNum is the channel number which starts counting at 0.
  double Pixie16ComputeOutputCountRate (
        unsigned int *Statistics, // run statistics data
        unsigned short ModNum,	  // module number      
        unsigned short ChanNum ); // channel number     

**Return values description and error handling**

The return value is the output count rate in counts per second.

**Usage example**

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

Compute number of events processed by a module

.. code:: cpp

  // Use this function to calculate the number of events that have been processed by a Pixie-16 module during a data acquisition run. This function is only used by Rev-A modules. This function does not communicate with Pixie-16 modules. Before calling this function, another function, Pixie16ReadStatisticsFromModule, should be called to read statistics data from the module first.
  // *Statistics is a pointer to an array whose size is exactly 448 unsigned integer words (32-bit). The *Statistics array is filled with data from a Pixie-16 module after calling function Pixie16ReadStatisticsFromModule. ModNum is the module number which starts counting at 0. ChanNum is the channel number which starts counting at 0.
  double Pixie16ComputeProcessedEvents (
        unsigned long *Statistics,// run statistics data
        unsigned short ModNum );  // module number      

**Return values description and error handling**

The return value is the number of processed events.

**Usage example**

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

Compute real time that a module accumulated in a run

.. code:: cpp

  // Use this function to calculate the real time that a Pixie-16 module has spent on data acquisition. This function does not communicate with Pixie-16 modules. Before calling this function, another function, Pixie16ReadStatisticsFromModule, should be called to read statistics data from the module.
  // *Statistics is a pointer to an array whose size is exactly 448 unsigned integer words (32-bit). The *Statistics array is filled with data from a Pixie-16 module after calling function Pixie16ReadStatisticsFromModule. ModNum is the module number which starts counting at 0. ChanNum is the channel number which starts counting at 0.
  double Pixie16ComputeRealTime (
        unsigned int *Statistics,// run statistics data
        unsigned short ModNum ); // module number      

**Return values description and error handling**

The return value is the real time in seconds.

**Usage example**

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

Compute slow filter response for offline analysis

.. code:: cpp

  // Use this function to compute slow filter responses of each event in the list mode data file for offline analysis. The algorithm implemented in this offline analysis function is equivalent to the one implemented in the Pixie-16 hardware. So this function can be used to analyze how the slow filter implemented in the hardware for computing event energy responds to the shape of recorded traces. By analyzing the responses of these filters, it is possible to optimize the performance of the slow filter by adjusting its parameters offline. Such optimized parameters can then be saved to a settings file to be used for online data acquisition.
  int Pixie16ComputeSlowFiltersOffline (
        char *FileName, // the list mode data file name (with complete path)
        unsigned short ModuleNumber, // the module to be analyzed
        unsigned short ChannelNumber, // the channel to be analyzed
        unsigned int FileLocation, // the location of the trace in the file
        unsigned short RcdTraceLength,// recorded trace length
        unsigned short *RcdTrace,// recorded trace
        double *slowfilter);// slow filter response

**Return values description and error handling**

- 0 Success
   - None
- -1 Null pointer RcdTrace
   - Make sure RcdTrace has been allocated enough memory (maximum possible length is 32768, but can be shorter to match the actual trace length)
- -2 Null pointer slowfilter
   - Make sure slowfilter has been allocated enough memory (maximum possible length is 32768, but can be shorter to match the actual trace length)
- -3 Target module number is invalid
   - Correct ModuleNumber
- -4 Trace length is too short
   - The length of recorded trace is too short for the offline analysis using the slow filter length (slow filter rise time and flat top). Either a different settings file with shorter slow filter length has to be used, or new traces with longer trace length have to be acquired
- -5 Failed to open list mode data file
   - Check the list mode file name and its path are correct

**Usage example**

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

Execute special control tasks

.. code:: cpp

  // Use this function to execute special control tasks. This may include programming the Fippi or setting the DACs after downloading DSP parameters. 
  // ModNum is the module number which starts counting at 0. ChanNum is the channel number which starts counting at 0.	  
  int Pixie16ControlTaskRun (
        unsigned short ModNum,     // module number                
        unsigned short ControlTask,// control task run type        
        unsigned int Max_Poll );   // Timeout control in unit of ms
  
**Return values description and error handling**

- 0 Success
   - None
- -1 Invalid Pixie-16 module number
   - Correct ModNum
- -2 The control task run failed
   - Reboot the module

**Usage example**

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

Copy DSP parameters from a module to others

.. code:: cpp

  // Use this function to copy DSP parameters from one module to the others that are installed in the system.
  // BitMask is bit pattern which designates which items should be copied from the source module to the destination module(s).
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
  // SourceModule and SourceChannel is the module and channel number of the source of the DSP parameters which are to be copied to other modules and channels.
  // DestinationMask is an array which indicates the channel and module whose settings will be copied from the source channel and module. For instance, if there are 5 modules (total 80 channels) in the system, DestinationMask would be defined as DestinationMask[80], where DestinationMask[0] to DestinationMask[15] would select channel 0 to 15 of module 0, DestinationMask[16] to DestinationMask[31] would select channel 0 to 15 of module 1, and so on. If a given channel i is to be copied, then DestinationMask[i] should be set to 1, otherwise, it should be set to 0.
  int Pixie16CopyDSPParameters (
        unsigned short BitMask,           // copy items bit mask 
        unsigned short SourceModule,	  // source module       
        unsigned short SourceChannel,	  // source channel      
        unsigned short *DestinationMask );// destination bit mask

**Return values description and error handling**

- 0 Success
   - None
- -1 Failed to program Fippi in a module
   - Reboot the modules
- -2 Failed to set DACs in a module
   - Reboot the modules

**Usage example**

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

Transfer data between host and DSP external memory

.. code:: cpp

  // Use this function to directly read data from or write data to the on-board external memory of a Pixie-16 module. The valid memory address is from 0x0 to 0x7FFFF (32-bit wide). Use Direction = 1 for read and Direction = 0 for write.
  // The external memory is used to store the histogram data accumulated for each of the 16 channels of a Pixie-16 module. Each channel has a fixed histogram length of 32768 words (32-bit wide), and the placement of the histogram data in the memory is in the same order of the channel number, i.e. channel 0 occupies memory address 0x0 to 0x7FFF, channel 1 occupies 0x8000 to 0xFFFF, and so on.
  // NOTE: another function Pixie16ReadHistogramFromModule can also be used to read out the histograms except that it needs to be called channel by channel.
  // ModNum is the module number which starts counting at 0.
  int Pixie16EMbufferIO (
        unsigned int *Buffer,// buffer data                            
        unsigned int NumWords, // number of buffer words to read or write
        unsigned int Address,// buffer address                         
        unsigned short Direction, // I/O direction                       
        unsigned short ModNum ); //module number                          

**Return values description and error handling**

- 0 Success
   - None
- -1 null pointer for buffer data
   - Correct pointer Buffer
- -2 number of buffer words exceeds the limit
   - Reduce the number of buffer words
- -3 invalid DSP external memory address
   - Use the valid address
- -4 invalid I/O direction
   - Use the valid direction
- -5 invalid Pixie-16 module number
   - Correct the ModNum
- -6 I/O Failure
   - Reboot the module

**Usage example**

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

Stop a data acquisition run

.. code:: cpp

  // Use this function to end a histogram run, or to force the end of a list mode run. In a multi-module system, if all modules are running synchronously, only one module needs to be addressed this way. It will immediately stop the run in all other module in the system. ModNum is the module number which starts counting at 0.
  int Pixie16EndRun (
        unsigned short ModNum );// module number
  
**Return values description and error handling**

- 0 Success
   - None
- -1 Invalid Pixie-16 module number
   - Correct ModNum
- -2 Failed to stop the run
   - Reboot the module

**Usage example**

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

Release user virtual addressees assigned to modules

.. code:: cpp

  // Use this function to release the user virtual addressees that are assigned to Pixie-16 modules when these modules are initialized by function Pixie16InitSystem. This function should be called before a user’s application exits.
  // If ModNum is set to less than the total number of modules in the system, only the module specified by ModNum will be closed. But if ModNum is equal to the total number of modules in the system, e.g. there are 5 modules in the chassis and ModNum = 5, then all modules in the system will be closed altogether. Note that the modules are counted starting at 0.
  int Pixie16ExitSystem (
        unsigned short ModNum ); // Pixie-16 module number
	  
**Return values description and error handling**

- 0 Success
   - None
- -1 Invalid Pixie-16 module number
   - Correct ModNum (it should not be greater than the total number of modules in the system)
- -2 Failed to close Pixie-16 modules
   - Check error message log file Pixie16msg.txt

**Usage example**

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

Get detailed events information from a data file

.. code:: cpp

  // Use this function to retrieve the detailed information (except waveforms) of each event in the list mode data file for the designated module. ModNum is the module number which starts counting at 0. ChanNum is the channel number which starts counting at 0.
  // Before calling this function to get the individual events information, another function Pixie16GetModuleEvents should be called first to determine the number of events that have been recorded for each module. If the number of events for a given module is nEvents, a memory block *EventInformation should be allocated with a length of (nEvents*68):
  // EventInformation = (unsigned int *)malloc(sizeof(unsigned int) * nEvents * 68);
  // where 68 is the length of the information records of each event (energy, timestamps, etc.) and has the following structure.
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
  
**Return values description and error handling**

- 0 Success
   - None
- -1 Null pointer EventInformation
   - Correct EventInformation
- -2 Invalid Pixie-16 module number
   - Correct ModuleNumber
- -3 Failed to open list mode data file
   - Correct file name and path

**Usage example**

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

Parse a list mode data file to get events information

.. code:: cpp

  // Use this function to parse the list mode events in the list mode data file. The number of events for each module will be reported.
  int Pixie16GetModuleEvents (
        char *FileName, // the list mode data file name (with complete path)
        unsigned int *ModuleEvents ); // receives number of events for modules

**Return values description and error handling**

- 0 Success
   - None
- -1 Null pointer ModuleEvents
   - Correct ModuleEvents
- -2 Failed to open list mode data file
   - Correct file name and path

**Usage example**

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

Transfer data between host and DSP internal memory

.. code:: cpp

  // Use this function to directly transfer data between the host and the DSP internal memory of a Pixie-16 module. ModNum is the module number which starts counting at 0.
  // The DSP internal memory is split into two blocks with address range 0x40000 to 0x4FFFF for the first block and address range 0x50000 to 0x5FFFF for the second block. Within the first block, address range 0x40000 to 0x49FFF is reserved for program memory and shouldn’t be accessed directly by the host computer. Address range 0x4A000 to 0x4A4FF is used by the DSP I/O parameters which are stored in the configuration files (.set files) in the host. Within this range, 0x4A000 to 0x4A33F can be both read and written, but 0x4A340 to 0x4A4FF can only be read but not written. The remaining address range (0x4A500 to 4FFFF) in the first block and the entire second block (0x50000 to 0x5FFFF) should only be read but not written by the host. Use Direction = 1 for read and Direction = 0 for write.
  int Pixie16IMbufferIO (
        unsigned int *Buffer,    //buffer data                       
        unsigned int NumWords,	 //number of buffer words to transfer
        unsigned int Address,	 //buffer address                    
        unsigned short Direction,//I/O direction                     
        unsigned short ModNum ); //module number                     

**Return values description and error handling**

- 0 Success
   - None
- -1 Null pointer for buffer data
   - Correct pointer Buffer
- -2 Number of buffer words exceeds the limit
   - Reduce the number of buffer words
- -3 Invalid DSP internal memory address
   - Use the valid address
- -4 Invalid I/O direction
   - Use the valid direction
- -5 Invalid Pixie-16 module number
   - Correct the ModNum
- -6 I/O Failure
   - Reboot the module

**Usage example**

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

Configure modules for communication in PXI chassis

.. code:: cpp

  // Use this function to configure the Pixie-16 modules in the PXI chassis.
  // NumModules is the total number of Pixie-16 modules installed in the system. PXISlotMap is the pointer to an array that must have at least as many entries as there are Pixie-16 modules in the chassis.
  // PXISlotMap serves as a simple mapping of the logical module number and the physical slot number that the modules reside in. The logical module number runs from 0. For instance, in a system with 5 Pixie-16 modules, these 5 modules may occupy slots 3 through 7. The user must fill PXISlotMap as follows: PXISlotMap = {3, 4, 5, 6, 7 ...} since module number 0 resides in slot number 3, etc. To find out in which slot a module is located, any piece of subsequent code can use the expression PXISlotMap[ModNum], where ModNum is the logic module number.
  // OfflineMode is used to indicate to the API whether the system is running in OFFLINE mode (1) or ONLINE mode (0). OFFLINE mode is useful for situations where no Pixie-16 modules are present but users can still test their calls to the API functions in their application software.
  // This function must be called as the first step in the boot process. It makes the modules known to the system and “opens” each module for communication.
  // The function relies on an initialization file (pxisys.ini) that contains information about the Host PC’s PCI buses, including the slot enumeration scheme. XIA’s software distribution normally puts this file under the same folder as Pixie-16 software installation folder. However, the user has the flexibility of putting it in other folders by simply changing the definition of the string PCISysIniFile in the header part of the file pixie16sys.c.
  int Pixie16InitSystem (
        unsigned short NumModules,   //total number of Pixie-16 modules                           
        unsigned short *PXISlotMap,  //an array containing the slot number of each Pixie-16 module
        unsigned short OfflineMode );//specify if using offline mode                              

**Return values description and error handling**

- 0 Success
   - None
- -1 Invalid total number of Pixie-16 modules
   - Check if NumModules <= PRESET_MAX_MODULES(defined in pixie16app_defs.h)
- -2 Null pointer PXISlotMap
   - Correct PXISlotMap
- -3 Failed to initialize system
   - Check error message log file Pixie16msg.txt

**Usage example**

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

Load DSP parameters to modules from a file

.. code:: cpp

  // Use this function to read DSP parameters from a settings file and then download the settings to Pixie-16 modules that are installed in the system. Each module has exactly 1280 DSP parameter values (32-bit unsigned integers), and depending on the value of PRESET_MAX_MODULES (defined in pixie16app_defs.h), the settings file should have exactly (1280 * PRESET_MAX_MODULES * 4) bytes when stored on the computer hard drive.
  int Pixie16LoadDSPParametersFromFile (
        char *FileName ); // DSP parameters file name (with complete path)	  

**Return values description and error handling**

- 0 Success
   - None
- -1 Size of DSPParFile is invalid
   - Correct DSPParFile
- -2 Failed to program Fippi in a module
   - Reboot the modules
- -3 Failed to set DACs in a module
   - Reboot the modules
- -4 Failed to open the DSP parameters file
   - Correct the DSP parameters file name

**Usage example**

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

Program on-board signal processing FPGAs

.. code:: cpp

  // Use this function to program the on-board signal processing FPGAs of the Pixie-16 modules. After the host computer has written the DSP parameters to the DSP memory, the DSP needs to write some of these parameters to the FPGAs. This function makes the DSP perform that action. ModNum is the module number which starts counting at 0.
  int Pixie16ProgramFippi (
        unsigned short ModNum);// module number
	  
**Return values description and error handling**

- 0 Success
   - None
- -1 Invalid Pixie-16 module number
   - Correct ModNum
- -2 Failed to start the PROGRAM_FIPPI run
   - Reboot the module
- -3 PROGRAM_FIPPI run timed out
   - Reboot the module

**Usage example**

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

Read Control & Status Register value from a module

.. code:: cpp

  // Use this function to read the host Control & Status Register (CSR) value. This register is unrelated to the DSP parameters ModCSRA/B, ChanCSRA/B. It is used to control the operation of the module and read directly by the host. Direct reading or writing by the host is not recommended, for example use functions like Pixie16CheckRunStatus to poll the active bit.
  int Pixie16ReadCSR (
        unsigned short ModNum,// module number     
        unsigned int *CSR );  // returned CSR value

**Return values description and error handling**

- 0 Success
   - None
- -1 Invalid Pixie-16 module number
   - Correct ModNum

**Usage example**

.. code:: cpp

  unsigned short ModNum;
  unsigned int CSR;
  ModNum = 0;// the first module
  Pixie16ReadCSR (ModNum, &CSR);
	  
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
Pixie16ReadDataFromExternalFIFO
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Read data from external FIFO of a module

.. code:: cpp

  // Use this function to read data from the external FIFO of a module.
  // This function reads list mode data from the external FIFO of a Pixie-16 module. The data are 32-bit unsigned integers. Normally, function Pixie16CheckExternalFIFOStatus is called first to see how many words the external FIFO currently has, and then this function is called to read the data from the FIFO. ModNum is the module number which starts counting at 0.
  int Pixie16ReadDataFromExternalFIFO (
        unsigned int *ExtFIFO_Data,// to receive the external FIFO data
        unsigned int nFIFOWords,// number of words to read from FIFO
        unsigned short ModNum );// module number	  

**Return values description and error handling**

- 0 Success
   - None
- -1 Invalid Pixie-16 module number
   - Check ModNum
- -2 Failed to read data from the external FIFO
   - Check error message log file Pixie16msg.txt

**Usage example**

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

Read histogram data from a histogram data file

.. code:: cpp

  // Use this function to read histogram data from a histogram data file. Before calling this function, the host code should allocate appropriate amount of memory to store the histogram data. The default histogram length is 32768. Histogram data are 32-bit unsigned integers.
  // Specify the module using ModNum and the channel on the module using ChanNum. Note that both the modules and channels are counted starting at 0.
  int Pixie16ReadHistogramFromFile (
        char *FileName,// histogram file name (with complete path)
        unsigned int *Histogram,// histogram data
        unsigned int NumWords,// number of words to be read out
        unsigned short ModNum,// module number
        unsigned short ChanNum);// channel number

**Return values description and error handling**

- 0 Success
   - None
- -1 Failed to open the histogram data file
   - Correct the histogram data file name
- -2 No histogram data is available for this channel
   - Change the ModNum and ChanNum

**Usage example**

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

Read histogram data from a module

.. code:: cpp

  // Use this function to read out the histogram data from a Pixie-16 module’s histogram memory. Before calling this function, the host code should allocate appropriate amount of memory to store the histogram data. The default histogram length is 32768. Histogram data are 32-bit unsigned integers.
  // Specify the module using ModNum and the channel on the module using ChanNum. Note that both the modules and channels are counted starting at 0.	  
  int Pixie16ReadHistogramFromModule(
        unsigned int *Histogram,//histogram data
        unsigned int NumWords,//number of words to be read out
        unsigned short ModNum,//module number
        unsigned short ChanNum);//channel number
  
**Return values description and error handling**

- 0 Success
   - None
- -1 Invalid Pixie-16 module number
   - Correct ModNum
- -2 Invalid Pixie-16 channel number
   - Correct ChanNum
- -3 Failed to get the histogram data
   - Reboot the module

**Usage example**

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

Read trace data from a list mode data file

.. code:: cpp

  // Use this function to retrieve list mode trace from a list mode data file. It uses the trace length and file location information obtained from function Pixie16GetEventsInfo for the selected event.
  int Pixie16ReadListModeTrace (
        char *FileName,            //list mode data file name             
        unsigned short *Trace_Data,//list mode trace data (16-bit words)  
        unsigned short NumWords,   //number of 16-bit words to be read out
        unsigned int FileLocation);//the location of the trace in the file

**Return values description and error handling**

- 0 Success
   - None
- -1 Failed to open list mode data file
   - Correct file name and path

**Usage example**

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

Read information about a module stored in the EEPROM

.. code:: cpp

  // Use this function to read information stored on each module, including its revision, serial number, ADC bits and sampling rate. This should be done after initializing the PCI communication. Information from the module can be used to select the appropriate firmware, DSP, and configuration parameters files before booting the module. ModNum is the module number which starts counting at 0.
  int Pixie16ReadModuleInfo (
        unsigned short ModNum,       //module number                    
        unsigned short *ModRev,	     //returned module revision         
        unsigned int *ModSerNum,     //returned module serial number    
        unsigned short *ModADCBits,  //returned module ADC bits         
        unsigned short *ModADCMSPS );//returned module ADC sampling rate

**Return values description and error handling**

- 0 Success
   - None
- -1 Invalid Pixie module number
   - Check ModNum
- -2 Failed to read from I2C serial EEPROM
   - Check error message log file Pixie16msg.txt

**Usage example**

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

Read ADC trace data from a channel in a module

.. code:: cpp

  // Use this function to read ADC trace data from a Pixie-16 module. Before calling this function, another function Pixie16AcquireADCTrace should be called to fill the DSP internal memory first. Also, the host code should allocate appropriate amount of memory to store the trace data. The ADC trace data length for each channel is 8192. Since the trace data are 16-bit unsigned integers (for hardware variants with less than 16-bit ADCs only the lower 12-bit or 14-bit contain real data), two consecutive 16-bit words are packed into one 32-bit word in the DSP internal memory. So for each channel, 4096 32-bit words are read out first from the DSP, and then each 32-bit word is unpacked to form two 16-bit words.
  // Specify the module using ModNum and the channel on the module using ChanNum. Note that both the modules and channels are counted starting at 0.
  int Pixie16ReadSglChanADCTrace (
        unsigned short *Trace_Buffer, //trace data
        unsigned int Trace_Length,//number of trace data words to read
        unsigned short ModNum,//module number
        unsigned short ChanNum );//channel number

**Return values description and error handling**

- 0 Success
   - None
- -1 Invalid Pixie-16 module number
   - Correct ModNum
- -2 Invalid Pixie-16 channel number
   - Correct ChanNum
- -3 Invalid trace length
   - Correct Trace_Length
- -4 Failed to allocate memory to store ADC traces
   - Close other programs or reboot the computer
- -5 Failed to read ADC traces
   - Reboot the module

**Usage example**

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

Read baselines from a channel in a module

.. code:: cpp

  // Use this function to read baseline data from a Pixie-16 module. Before calling this function, another function Pixie16AcquireBaselines should be called to fill the DSP internal memory first. Also, the host code should allocate appropriate amount of memory to store the baseline data. The number of baselines for each channel is 3640. In the DSP internal memory, each baseline is a 32-bit IEEE floating point number. After being read out to the host, this function will convert each baseline data to a decimal number. In addition to baseline values, timestamps corresponding to each baseline are also returned after this function call.
  // Specify the module using ModNum and the channel on the module using ChanNum. Note that the modules and channels are counted starting at 0.
  int Pixie16ReadSglChanBaselines(
        double *Baselines,       //returned baseline values             
        double *TimeStamps,	 //timestamps for each baseline value   
        unsigned short NumBases, //number of baseline data words to read
        unsigned short ModNum,	 //module number                        
        unsigned short ChanNum );//channel number                       

**Return values description and error handling**

- 0 Success
   - None
- -1 Invalid Pixie-16 module number
   - Correct ModNum
- -2 Requested number of baselines exceeded the limit
   - Correct NumBases
- -3 Failed to allocate memory to store baselines
   - Close other programs or reboot computer
- -4 Failed to read baselines
   - Reboot the module

**Usage example**

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

Read a CHANNEL level parameter from a module

.. code:: cpp

  // Use this function to read a channel parameter from a Pixie-16 module. ModNum is the module number which starts counting at 0. ChanNum is the channel number which starts counting at 0.
  int Pixie16ReadSglChanPar (
        char *ChanParName,       // channel parameter name 
        double *ChanParData,	 // channel parameter value
        unsigned short ModNum,	 // channel number         
        unsigned short ChanNum );// module number          
	  
**Return values description and error handling**

- 0 Success
   - None
- -1 Invalid Pixie-16 module number
   - Correct ModNum
- -2 Invalid Pixie-16 channel number
   - Correct ChanNum
- -3 Invalid channel parameter name
   - Correct ChanParName

**Usage example**

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

Read a MODULE level parameter from a module

.. code:: cpp

  // Use this function to read a module parameter from a Pixie-16 module. ModNum is the module number which starts counting at 0.
  int Pixie16ReadSglModPar (
        char *ModParName,        // module parameter name 
        unsigned int *ModParData,// module parameter value
        unsigned short ModNum ); // module number         

**Return values description and error handling**

- 0 Success
   - None
- -1 Invalid Pixie-16 module number
   - Correct ModNum
- -2 Invalid module parameter name
   - Correct ModParName

**Usage example**

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

Read run statistics data from a module

.. code:: cpp

  // Use this function to read out statistics data from a Pixie-16 module. Before calling this function, the host code should allocate appropriate amount of memory to store the statistics data. The number of statistics data for each module is fixed at 448. Statistics data are 32-bit unsigned integers.
  // Specify the module using ModNum. Note that the modules are counted starting at 0.
  int Pixie16ReadStatisticsFromModule (
        unsigned int *Statistics,// statistics data
        unsigned short ModNum ); // module number  

**Return values description and error handling**

- 0 Success
   - None
- -1 Invalid Pixie-16 module number
   - Correct ModNum
- -2 Failed to get the statistics data
   - Reboot the module

**Usage example**

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

Read from or write to registers on a module

.. code:: cpp

  // Use this function to read data from or write data to a register in a Pixie-16 module.
  // Specify the module using ModNum. Note that the modules are counted starting at 0.
  int Pixie16RegisterIO (
        unsigned short ModNum,   //module number             
        unsigned int address,	 //register address          
        unsigned short direction,//read or write             
        unsigned int *value );	 //holds or receives the data

**Return values description and error handling**

- 0 Success
   - None
- -1 Invalid Pixie-16 module number
   - Correct ModNum

**Usage example**

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

Read DSP parameters from modules and save to a file

.. code:: cpp

  // Use this function to save DSP parameters to a settings file. It will first read the values of DSP parameters on each Pixie-16 module and then write them to the settings file. Each module has exactly 1280 DSP parameter values (32-bit unsigned integers), and depending on the value of PRESET_MAX_MODULES (defined in pixie16app_defs.h), the settings file should have exactly (1280 * PRESET_MAX_MODULES * 4) bytes when stored on the computer hard drive.
  int Pixie16SaveDSPParametersToFile (
        char *FileName ); // DSP parameters file name (with complete path)
  
**Return values description and error handling**

- 0 Success
   - None
- -1 Failed to read DSP parameter values from the Pixie-16 modules
   - Reboot the modules
- -2 Failed to open the DSP parameters file
   - Correct the DSP parameters file name

**Usage example**

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

Read data from external FIFO and save to a file

.. code:: cpp

  // Use this function to read data from the external FIFO of a module. ModNum is the module number which starts counting at 0.
  // This function first checks the status of the external FIFO of a Pixie-16 module, and if there are data in the external FIFO, this function then reads list mode data (32-bit unsigned integers) from the external FIFO. So this function essentially encapsulates both functions Pixie16CheckExternalFIFOStatus and Pixie16ReadDataFromExternalFIFO within one function. The number of words that are read from the external FIFO is recorded in variable *FIFOWords. The function also expects setting the value of a variable called “EndOfRunRead” to indicate whether this read is at the end of a run (1) or during the run (0). This is necessary since the external FIFO needs special treatment when the host reads the last few words from the external FIFO due to its pipelined structure.
  int Pixie16SaveExternalFIFODataToFile (
        char *FileName,// list mode data file name
        unsigned int *nFIFOWords,// number of words read from ext. FIFO
        unsigned short ModNum,// module number
        unsigned short EndOfRunRead ); // indicator if this is end of run read

**Return values description and error handling**

- 0 Success
   - None
- -1 Invalid Pixie-16 module number
   - Check ModNum
- -2 Failed to allocate memory to store list mode data
   - Check computer resources
- -3 Failed to open list mode data file
   - Check if file is protected
- -4 Failed to read external FIFO status
   - Check error message log file Pixie16msg.txt
- -5 Failed to read data from external FIFO
   - Check error message log file Pixie16msg.txt

**Usage example**

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

Read histogram data from a module and save to a file

.. code:: cpp

  // Use this function to read histogram data from a Pixie-16 module and save the histogram data to a file with file name specified by the user: First this function saves the histogram data to a binary file, and it then saves the histogram data to an ASCII file with run statistics data appended to the end of the ASCII file. Existing files will be overwritten. ModNum is the module number which starts counting at 0.
  int Pixie16SaveHistogramToFile (
        char *FileName,// histogram data file name (with complete path)
        unsigned short ModNum); // module number

**Return values description and error handling**

- 0 Success
   - None
- -1 Invalid Pixie-16 module number
   - Correct ModNum
- -2 Failed to get histogram data from module
   - Reboot the module
- -3 Failed to allocate memory to store histogram data
   - Close other programs or reboot computer
- -4 Failed to open histogram data file
   - Correct file name and path
- -5 Failed to open mca ascii output file
   - Correct file name and path
- -6 Failed to allocate memory to store histogram data for ascii text file
   - Close other programs or reboot computer
- -7 Failed to read histogram data from file
   - Check file name and path
- -8 Failed to read run statistics data from module
   - Reboot the module

**Usage example**

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

Program on-board DACs

.. code:: cpp

  // Use this function to reprogram the on-board digital to analog converters (DAC) of the Pixie-16 modules. In this operation the DSP uses data from the DSP parameters that were previously downloaded. ModNum is the module number which starts counting at 0.
  int Pixie16SetDACs (
        unsigned short ModNum);// module number

**Return values description and error handling**

- 0 Success
   - None
- -1 Invalid Pixie-16 module number
   - Correct ModNum
- -2 Failed to start the SET_DACS run
   - Reboot the module
- -3 SET_DACS run timed out
   - Reboot the module

**Usage example**

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

Start a MCA histogram mode data acquisition run

.. code:: cpp

  // Use this function to begin a data acquisition run that accumulates energy histograms, one for each channel. It launches a data acquisition run in which only energy information is preserved and histogrammed locally to each channel.
  // Call this function for each Pixie-16 module in the system to initialize the run in each module. Actual data acquisition will start synchronously in all modules when the last module finished the initialization(requires the synchronization parameter to be set). Histogram runs can be self-terminating when the elapsed run time exceeds the preset run time, or the user can prematurely terminate the run by calling Pixie16EndRun. On completion, final histogram and statistics data will be available.
  // Use mode=NEW_RUN (=1) to erase histograms and statistics information before launching the new run. Use mode=RESUME_RUN (=0) to resume an earlier run.
  // ModNum is the module number which starts counting at 0. If ModNum is set to be less than the total number of modules in the system, only the module specified by ModNum will have its histogram run started. But if ModNum is set to be equal to the total number of modules in the system, then all modules in the system will have their runs started together.
  int Pixie16StartHistogramRun (
        unsigned short ModNum,// module number
        unsigned short mode );// run mode

**Return values description and error handling**

- 0 Success
   - None
- -1 Invalid Pixie-16 module number
   - Correct the ModNum
- -2 Invalid run mode
   - Correct the run mode
- -3 Failed to start histogram run
   - Reboot the module

**Usage example**

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

Start a list mode data acquisition run

.. code:: cpp

  // Use this function to start a list mode data acquisition run in Pixie-16 modules. List mode runs are used to collect data on an event-by-event basis, gathering energies, timestamps, pulse shape analysis values, and waveforms for each event. Runs will continue until the user terminates the run by calling function Pixie16EndRun. To start the data acquisition this function has to be called for every Pixie-16 module in the system. If all modules are to run synchronously, the last module addressed will release all others and the acquisition starts then. The first module to end the run will immediately stop the run in all other modules if run synchronization has been set up among these modules.
  // Use mode=NEW_RUN (=1) to erase histograms and statistics information before launching the new run. Note that this will cause a startup delay of up to 1 millisecond. Use mode=RESUME_RUN (=0) to resume an earlier run. This mode has a startup delay of only a few microseconds.
  // There is only one list mode run type supported, that is, 0x100. However, different output data options can be chosen by enabling or disabling different CHANCSRA bits.
  // ModNum is the module number which starts counting at 0. If ModNum is set to be less than the total number of modules in the system, only the module specified by ModNum will have its list mode run started. But if ModNum is set to equal to the total number of modules in the system, then all modules in the system will have their runs started together.
  int Pixie16StartListModeRun (
        unsigned short ModNum, // module number
        unsigned short RunType,// run type     
        unsigned short mode ); // run mode     

**Return values description and error handling**

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

**Usage example**

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

Find the exponential decay time of a channel

.. code:: cpp

  // Use this function to find the exponential decay time constants (Tau value) of the detector or preamplifier signal that is connected to each of the 16 channels of a Pixie-16 module. The 16 found Tau values are returned via pointer *Tau. A '-1.0' Tau value for a channel means the Tau_Finder was not successful for such a channel. ModNum is the module number which starts counting at 0.
  void Pixie16TauFinder (
        unsigned short ModNum,// module number
        double *Tau );// Tau value

**Return values description and error handling**

- 0 Success
   - None
- -1 Invalid Pixie-16 module number
   - Correct the ModNum
- -2 Invalid Pixie-16 channel number
   - Correct the ChanNum
- -3 Failed to acquire ADC traces
   - Reboot the module
- -4 Failed to read ADC traces
   - Reboot the module
- -5 Failed to find sufficient number of pulses
   - Increase input count rate

**Usage example**

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

Write to Control & Status Register in a module

.. code:: cpp

  // Use this function to write a value to the host Control & Status Register (CSR). This register is unrelated to the DSP parameters ModCSRA/B, ChanCSRA/B. It is used to control the operation of the module and read directly by the host. Direct reading or writing by the host is not recommended, for example use functions like Pixie16CheckRunStatus to poll the active bit. ModNum is the module number which starts counting at 0.
  void Pixie16WriteCSR (
        unsigned short ModNum,// module number
        unsigned int CSR );   // CSR value to write

**Return values description and error handling**

- 0 Success
   - None
- -1 Invalid Pixie-16 module number
   - Correct the ModNum

**Usage example**

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

Write a CHANNEL level parameter to a module

.. code:: cpp

  // Use this function to write a channel parameter to a Pixie-16 module. ModNum is the module number which starts counting at 0. ChanNum is the channel number which starts counting at 0.
  int Pixie16WriteSglChanPar (
        char *ChanParName,       //channel parameter name 
        double ChanParData,	 //channel parameter value
        unsigned short ModNum,	 //channel number         
        unsigned short ChanNum );//module number          
  
**Return values description and error handling**

- 0 Success
   - None
- -1 Invalid Pixie-16 module number
   - Correct ModNum
- -2 Invalid Pixie-16 channel number
   - Correct ChanNum
- -3 Invalid channel parameter name
   - Correct ChanParName
- -4 Programing Fippi failed downloading channel parameter
   - Reboot the module
- -5 Failed to find BLcut after downloading channel parameter
   - Reboot the module
- -6 SetDACs failed downloading channel parameter
   - Reboot the module

**Usage example**

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

Write a MODULE level parameter to a module

.. code:: cpp

  // Use this function to write a module parameter to a Pixie-16 module. ModNum is the module number which starts counting at 0.
  int Pixie16WriteSglModPar (
        char *ModParName,       // module parameter name 
        unsigned int ModParData,// module parameter value
        unsigned short ModNum );// module number         

**Return values description and error handling**

- 0 Success
   - None
- -1 Invalid Pixie-16 module number
   - Correct ModNum
- -2 Invalid module parameter name
   - Correct ModParName
- -3 Failed to program Fippi after downloading module parameter
   - Reboot the module
- -4 Failed to find BLcut after downloading module parameter
   - Reboot the module

**Usage example**

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
