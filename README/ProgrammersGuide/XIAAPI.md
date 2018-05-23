<!-- XIAAPI.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 六 4月 28 18:50:41 2018 (+0800)
;; Last-Updated: 三 5月 23 06:45:45 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 12
;; URL: http://wuhongyi.cn -->

# XIA API

It from **Programmer's Manual Digital Gamma Finder (DGF) PIXIE-16** *Version 1.40, October 2009*


-----

```cpp
// Configure modules for communication in PXI chassis
// Use this function to configure the Pixie-16 modules in the PXI chassis.
// NumModules is the total number of Pixie-16 modules installed in the system. PXISlotMap is the pointer to an array that must have at least as many entries as there are Pixie-16 modules in the chassis.
// PXISlotMap serves as a simple mapping of the logical module number and the physical slot number that the modules reside in. The logical module number runs from 0. For instance, in a system with 5 Pixie-16 modules, these 5 modules may occupy slots 3 through 7. The user must fill PXISlotMap as follows: PXISlotMap = {3, 4, 5, 6, 7 ...} since module number 0 resides in slot number 3, etc. To find out in which slot a module is located, any piece of subsequent code can use the expression PXISlotMap[ModNum], where ModNum is the logic module number.
// OfflineMode is used to indicate to the API whether the system is running in OFFLINE mode (1) or ONLINE mode (0). OFFLINE mode is useful for situations where no Pixie-16 modules are present but users can still test their calls to the API functions in their application software.
// This function must be called as the first step in the boot process. It makes the modules known to the system and “opens” each module for communication.
// The function relies on an initialization file (pxisys.ini) that contains information about the Host PC’s PCI buses, including the slot enumeration scheme. XIA’s software distribution normally puts this file under the same folder as Pixie-16 software installation folder. However, the user has the flexibility of putting it in other folders by simply changing the definition of the string PCISysIniFile_Windows or PCISysIniFile_Linux in the header part of the file pixie16sys.c, depending on which operating system is being used.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16InitSystem (
	unsigned short NumModules,    // total number of Pixie16 modules in the system
	unsigned short *PXISlotMap,   // an array containing the PXI slot number for each pixie16 module
	unsigned short OfflineMode ); // specify if the system is in offline mode
```

```cpp
// Release user virtual addressees assigned to modules
// Use this function to release the user virtual addressees that are assigned to Pixie-16 modules when these modules are initialized by function Pixie16InitSystem. This function should be called before a user’s application exits.
// If ModNum is set to less than the total number of modules in the system, only the module specified by ModNum will be closed. But if ModNum is equal to the total number of modules in the system, e.g. there are 5 modules in the chassis and ModNum = 5, then all modules in the system will be closed altogether. Note that the modules are counted starting at 0.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ExitSystem (
	unsigned short ModNum );      // module number
```

```cpp
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadModuleInfo (
	unsigned short ModNum,        // module number
	unsigned short *ModRev,       // returned module revision
	unsigned int   *ModSerNum,    // returned module serial number
	unsigned short *ModADCBits,   // returned module ADC bits
	unsigned short *ModADCMSPS ); // returned module ADC sampling rate
```

```cpp
// Boot modules so that they can be set up for data taking
// Use this function to boot Pixie-16 modules so that they can be set up for data taking. The function downloads to the Pixie-16 modules the communication FPGA configurations, signal processing FPGA configurations, trigger FPGA configurations (Revision A modules only), executable code for the digital signal processor (DSP), and DSP parameters.
// The FPGA configurations consist of a fixed number of words depending on the hardware mounted on the modules; the DSP codes have a length which depends on the actual compiled code; and the set of DSP parameters always consists of 1280 32-bit words for each module. The host software has to make the names of those boot data files on the hard disk available to the boot function.
// If ModNum is set to be less than the total number of modules in the system, only the module specified by ModNum will be booted. But if ModNum is equal to the total number of modules in the system, e.g. there are 5 modules in the chassis and ModNum = 5, then all modules in the system will be booted.
// The boot pattern is a bit mask (shown below) indicating which on-board chip will be booted. Under normal circumstances, all on-board chips should be booted, i.e. the boot pattern would be 0x7F. For Rev-B, C, D modules, bit 1, i.e., “Boot trigger FPGA”, will be ignored even if that bit is set to 1.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16BootModule (
	char *ComFPGAConfigFile,      // name of communications FPGA configuration file
	char *SPFPGAConfigFile,       // name of signal processing FPGA configuration file
	char *TrigFPGAConfigFile,     // name of trigger FPGA configuration file
	char *DSPCodeFile,            // name of executable code file for digital signal processor (DSP)
	char *DSPParFile,             // name of DSP parameter file
	char *DSPVarFile,             // name of DSP variable names file
	unsigned short ModNum,        // pixie module number
	unsigned short BootPattern ); // boot pattern bit mask
```

```cpp
// Acquire ADC traces in single or multiple modules
// Use this function to acquire ADC traces from Pixie-16 modules. Specify the module using ModNum. If ModNum is set to be less than the total number of modules in the system, only the module specified by ModNum will have its ADC traces acquired. But if ModNum is equal to the total number of modules in the system, then all modules in the system will have their ADC traces acquired.
// After the successful return of this function, the DSP's internal memory will be filled with ADC trace data. A user's application software should then call another function Pixie16ReadSglChanADCTrace to read the ADC trace data out to the host computer, channel by channel.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16AcquireADCTrace (
	unsigned short ModNum );      // module number
```

```cpp
// Read ADC trace data from a channel in a module
// Use this function to read ADC trace data from a Pixie-16 module. Before calling this function, another function Pixie16AcquireADCTrace should be called to fill the DSP internal memory first. Also, the host code should allocate appropriate amount of memory to store the trace data. The ADC trace data length for each channel is 8192. Since the trace data are 16-bit unsigned integers (actually only the lower 14-bit contains real data due to the on-board 14-bit ADC), two consecutive 16-bit words are packed into one 32-bit word in the DSP internal memory. So for each channel, 4096 32-bit words are read out first from the DSP, and then each 32-bit word is unpacked to form two 16-bit words.
// Specify the module using ModNum and the channel on the module using ChanNum. Note that both the modules and channels are counted starting at 0.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadSglChanADCTrace (
	unsigned short *Trace_Buffer, // trace data
	unsigned int Trace_Length,    // trace length
	unsigned short ModNum,        // module number
	unsigned short ChanNum );     // channel number
```

```cpp
// Transfer data between host and DSP internal memory
// Use this function to directly transfer data between the host and the DSP internal memory of a Pixie-16 module. The DSP internal memory is split into two blocks with address range 0x40000 to 0x4FFFF for the first block and address range 0x50000 to 0x5FFFF for the second block. Within the first block, address range 0x40000 to 0x49FFF is reserved for program memory and shouldn’t be accessed directly by the host. Address range 0x4A000 to 0x4A4FF is used by the DSP I/O parameters which are stored in the configuration files (.set files) in the host. Within this range, 0x4A000 to 0x4A33F can be both read and written, but 0x4A340 to 0x4A4FF can only be read but not written. The remaining address range (0x4A500 to 4FFFF) in the first block and the entire second block (0x50000 to 0x5FFFF) should only be read but not written by the host. Use Direction = 1 for read and Direction = 0 for write.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16IMbufferIO (
	unsigned int *Buffer,         // buffer data
	unsigned int NumWords,        // number of buffer words to read or write
	unsigned int Address,         // buffer address
	unsigned short Direction,     // I/O direction
	unsigned short ModNum );      // module number
```
	
```cpp
// Transfer data between host and DSP external memory
// Use this function to directly read data from or write data to the on-board external memory of a Pixie-16 module. The valid memory address is from 0x0 to 0x7FFFF (32-bit wide). Use Direction = 1 for read and Direction = 0 for write.
// The external memory is used to store the histogram data accumulated for each of the 16 channels of a Pixie-16 module. Each channel has a fixed histogram length of 32768 words(32-bit wide), and the placement of the histogram data in the memory is in the same order of the channel number, i.e. channel 0 occupies memory address 0x0 to 0x7FFF, channel 1 occupies 0x8000 to 0xFFFF, and so on.
// NOTE: another function Pixie16ReadHistogramFromModule can also be used to read out the histograms except that it needs to be called channel by channel.
// In Rev-A modules, part of the external memory is also used to store the list mode data in ping-pong buffering mode. This function can be used to read list mode data from the buffers.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16EMbufferIO (
	unsigned int *Buffer,         // buffer data
	unsigned int NumWords,        // number of buffer words to read or write
	unsigned int Address,         // buffer address
	unsigned short Direction,     // I/O direction
	unsigned short ModNum );      // module number
```

```cpp
// Start a list mode data acquisition run
// Use this function to start a list mode data acquisition run in Pixie-16 modules. List mode run is used to collect data on an event-by-event basis, gathering energies, timestamps, pulse shape analysis values, and waveforms, for each event. Runs will continue until a preset number of events are reached or the user terminates the run by calling function Pixie16EndRun. Once the run is progress, if the run is set to terminate after a given number of events have been accumulated, another function, Pixie16CheckRunStatus, should be called to check if the run has finished. To start the data acquisition this function has to be called for every Pixie-16 module in the system. If all modules are to run synchronously, The last module addressed will release all others and the acquisition starts then. The first module to end the run will immediately stop the run in all other modules.
// Use mode=NEW_RUN (=1) to erase histograms and statistics information before launching the new run. Note that this will cause a start up delay of up to 1 millisecond. Use mode=RESUME_RUN (=0) to resume an earlier run. This mode has a start up delay of only a few microseconds.
// For Rev-A modules, currently there are 4 list mode run types supported. They are 0x100 (general purpose run), 0x101 (without waveforms), 0x102 (without auxiliary data) and 0x103 (energy and timestamp only).
// For Rev-B, C, D modules, there are only one list mode run type supported, that is, 0x100. However, different output data options can be chosen by enabling or disabling different CHANCSRA bits.
// Histograms and statistics data are updated incrementally from run to run provided RESUME_RUN mode is used.
// ModNum is the module number which starts counting at 0. If ModNum is set to be less than the total number of modules in the system, only the module specified by ModNum will have its list mode run started. But if ModNum is set to equal to the total number of modules in the system, then all modules in the system will have their runs started together.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16StartListModeRun (
	unsigned short ModNum,        // module number
	unsigned short RunType,       // run type
	unsigned short mode );        // run mode
```

```cpp
// Start a MCA histogram mode data acquisition run
// Use this function to begin a data acquisition run that accumulates energy histograms, one for each channel. It launches a data acquisition run in which only energy information is preserved and histogrammed locally to each channel.
// Call this function for each Pixie-16 module in the system. The last module addressed will allow the actual data acquisition to begin. Histogram run can be self-terminating when the elapsed run time exceeds the preset run time, or the user can prematurely terminate the run by calling Pixie16EndRun. On completion, final histogram and statistics data will be available.
// Use mode=NEW_RUN (=1) to erase histograms and statistics information before launching the new run. Use mode=RESUME_RUN (=0) to resume an earlier run.
// ModNum is the module number which starts counting at 0. If ModNum is set to be less than the total number of modules in the system, only the module specified by ModNum will have its histogram run started. But if ModNum is set to be equal to the total number of modules in the system, then all modules in the system will have their runs started together.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16StartHistogramRun (
	unsigned short ModNum,        // module number
	unsigned short mode );        // run mode
```

```cpp
// Check status of a data acquisition run
// Use this function to check the run status of a Pixie-16 module while a list mode data acquisition run is in progress. If the run is still in progress continue polling.
// If the return code of this function indicates the run has finished, there might still be some data in the external memory (Rev-A modules) or external FIFO (Rev-B, C, D modules) that need to be read out to the host. In addition, final run statistics and histogram data are available for reading out too.
// In MCA histogram run mode, this function can also be called to check if the run is still in progress even though it is normally self-terminating.
// ModNum is the module number which starts counting at 0.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16CheckRunStatus (
	unsigned short ModNum );      // Pixie module number  
```

```cpp
// Stop a data acquisition run
// Use this function to end a histogram run, or to force the end of a list mode run. In a multi-module system, if all modules are running synchronously, only one module needs to be addressed this way. It will immediately stop the run in all other module in the system.
// ModNum is the module number which starts counting at 0.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16EndRun (
	unsigned short ModNum );      // Pixie module number  
```

```cpp
// Compute input count rate
// Use this function to calculate the input count rate on one channel of a Pixie-16 module. This function does not communicate with Pixie-16 modules. Before calling this function, another function, Pixie16ReadStatisticsFromModule, should be called to read statistics data from the module first.
// *Statistics is a pointer to an array whose size is exactly 448 unsigned integer words (32-bit). The *Statistics array is filled with data from a Pixie-16 module after calling function Pixie16ReadStatisticsFromModule. ModNum is the module number which starts counting at 0. ChanNum is the channel number which starts counting at 0.
PIXIE16APP_EXPORT double PIXIE16APP_API Pixie16ComputeInputCountRate (
	unsigned int   *Statistics,
	unsigned short ModNum,
	unsigned short ChanNum );
```

```cpp
// Compute output count rate of a channel
// Use this function to calculate the output count rate on one channel of a Pixie-16 module. This function does not communicate with Pixie-16 modules. Before calling this function, another function, Pixie16ReadStatisticsFromModule, should be called to read statistics data from the module first.
// *Statistics is a pointer to an array whose size is exactly 448 unsigned integer words (32-bit). The *Statistics array is filled with data from a Pixie-16 module after calling function Pixie16ReadStatisticsFromModule. ModNum is the module number which starts counting at 0. ChanNum is the channel number which starts counting at 0.
PIXIE16APP_EXPORT double PIXIE16APP_API Pixie16ComputeOutputCountRate (
	unsigned int   *Statistics,
	unsigned short ModNum,
	unsigned short ChanNum );
```

```cpp
// Compute live time that a channel accumulated in a run
// Use this function to calculate the live time that one channel of a Pixie-16 module has spent on data acquisition. This function does not communicate with Pixie-16 modules. Before calling this function, another function, Pixie16ReadStatisticsFromModule, should be called to read statistics data from the module first.
// *Statistics is a pointer to an array whose size is exactly 448 unsigned integer words (32-bit). The *Statistics array is filled with data from a Pixie-16 module after calling function Pixie16ReadStatisticsFromModule. ModNum is the module number which starts counting at 0. ChanNum is the channel number which starts counting at 0.
PIXIE16APP_EXPORT double PIXIE16APP_API Pixie16ComputeLiveTime (
	unsigned int   *Statistics,
	unsigned short ModNum,
	unsigned short ChanNum );
```

```cpp
// Compute number of events processed by a channel
// Use this function to calculate the number of events that have been processed by a Pixie-16 module during a data acquisition run. This function is only used by Rev-A modules. This function does not communicate with Pixie-16 modules. Before calling this function, another function, Pixie16ReadStatisticsFromModule, should be called to read statistics data from the module first.
// *Statistics is a pointer to an array whose size is exactly 448 unsigned integer words (32-bit). The *Statistics array is filled with data from a Pixie-16 module after calling function Pixie16ReadStatisticsFromModule. ModNum is the module number which starts counting at 0. ChanNum is the channel number which starts counting at 0.
PIXIE16APP_EXPORT double PIXIE16APP_API Pixie16ComputeProcessedEvents (
	unsigned int   *Statistics,
	unsigned short ModNum );
```

```cpp
// Compute real time that a channel accumulated in a run
// Use this function to calculate the real time that a Pixie-16 module has spent on data acquisition. This function does not communicate with Pixie-16 modules. Before calling this function, another function, Pixie16ReadStatisticsFromModule, should be called to read statistics data from the module first.
// *Statistics is a pointer to an array whose size is exactly 448 unsigned integer words (32-bit). The *Statistics array is filled with data from a Pixie-16 module after calling function Pixie16ReadStatisticsFromModule. ModNum is the module number which starts counting at 0. ChanNum is the channel number which starts counting at 0.
PIXIE16APP_EXPORT double PIXIE16APP_API Pixie16ComputeRealTime (
	unsigned int   *Statistics,
	unsigned short ModNum );
```

```cpp
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16complexFFT (
	double *data,
	unsigned int length );

// Test one bit of a 16-bit unsigned integer
PIXIE16APP_EXPORT unsigned short PIXIE16APP_API APP16_TstBit (
	unsigned short bit,
	unsigned short value );

// Set one bit of a 16-bit unsigned integer
PIXIE16APP_EXPORT unsigned short PIXIE16APP_API APP16_SetBit (
	unsigned short bit,
	unsigned short value );

// Clear one bit of a 16-bit unsigned integer
PIXIE16APP_EXPORT unsigned short PIXIE16APP_API APP16_ClrBit (
	unsigned short bit,
	unsigned short value );

// Set one bit of a 32-bit unsigned integer
PIXIE16APP_EXPORT unsigned int PIXIE16APP_API APP32_SetBit ( 
	unsigned short bit,
	unsigned int   value );

// Clear one bit of a 32-bit unsigned integer
PIXIE16APP_EXPORT unsigned int PIXIE16APP_API APP32_ClrBit (
	unsigned short bit,
	unsigned int   value );

// Test one bit of a 32-bit unsigned integer
PIXIE16APP_EXPORT unsigned int PIXIE16APP_API APP32_TstBit (
	unsigned short bit,
	unsigned int   value );
```

```cpp
// Program on-board DACs
// Use this function to reprogram the on-board digital to analog converters (DAC) of the Pixie-16 modules. In this operation the DSP uses data from the DSP parameters that were previously downloaded.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16SetDACs (
	unsigned short ModNum );
```

```cpp
// Program on-board signal processing FPGAs
// Use this function to program the on-board signal processing FPGAs of the Pixie-16 modules. After the host computer has written the DSP parameters to the DSP memory, the DSP needs to write some of these parameters to the FPGAs. This function makes the DSP perform that action.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ProgramFippi (
	unsigned short ModNum );
```

```cpp
// Adjust DC-offsets in single or multiple modules
// Use this function to adjust the DC-offsets of Pixie-16 modules. Specify the module using ModNum. If ModNum is set to be less than the total number of modules in the system, only the module specified by ModNum will have its DC-offsets adjusted. But if ModNum is set to be equal to the total number of modules in the system, then all modules in the system will have their DC-offsets adjusted.
// After the DC-offset levels have been adjusted, the baseline level of the digitized input signals will be determined by the DSP parameter BaselinePercent. For instance, if BaselinePercent is set to 10(%), the baseline level of the input signals will be ~ 1638 on the 14-bit ADC scale (minimum: 0; maximum: 16383).
// The main purpose of this function is to ensure the input signals fall within the voltage range of the ADCs to ensure all input signals can be digitized by the ADCs properly.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16AdjustOffsets (
	unsigned short ModNum );
```

```cpp
// Acquire baselines from a module
// Use this function to acquire baselines from Pixie-16 modules. Specify the module using ModNum. If ModNum is set to be less than the total number of modules in the system, only the module specified by ModNum will have its baselines acquired. But if ModNum is set to be equal to the total number of modules in the system, then all modules in the system will have their baselines acquired.
// After the successful return of this function, the DSP’s internal memory will be filled with baselines data. Users should then call another function Pixie16ReadSglChanBaselines to read the baselines data out to the host computer, channel by channel.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16AcquireBaselines (
	unsigned short ModNum );      // module number
```

```cpp
// Read baselines from a channel in a module
// Use this function to read baselines data from a Pixie-16 module. Before calling this function, another function Pixie16AcquireBaselines should be called to fill the DSP internal memory first. Also, the host code should allocate appropriate amount of memory to store the baseline data. The baselines data length for each channel is 3640. In the DSP internal memory, each baseline data is a 32-bit IEEE floating point number. After being read out to the host, this function will convert each baseline data to a decimal number. In addition to baseline values, timestamps corresponding to each baseline were also returned after this function call.
// Specify the module using ModNum and the channel on the module using ChanNum. Note that the modules and channels are counted starting at 0.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadSglChanBaselines (
	double *Baselines,            // returned baselines values
	double *TimeStamps,           // time stamp for each baseline value
	unsigned short NumBases,      // number of baseline values to read
	unsigned short ModNum,        // module number
	unsigned short ChanNum );     // channel number
```

```cpp
// Ramp Offset DACs of a module and record the baselines
// Use this function to execute the RAMP_OFFSETDACS control task run. Each Offset DAC has 65536 steps, and the RAMP_OFFSETDACS control task ramps the DAC from 0 to 65335 with a step size of 64, i.e., a total of 1024 steps. At each DAC step, the control task computes the baseline value as the representation of the signal baseline and stores it in the DSP memory. After the control task is finished, the stored baseline values are read out to the host computer and saved to a binary file called “rampdacs.bin” in the form of IEEE 32-bit floating point numbers. Users can then plot the baseline values vs. DAC steps to determine the appropriate DAC value to be set in the DSP in order to bring the input signals into the voltage range of the ADCs. However, this function is no longer needed due to the introduction of function Pixie16AdjustOffsets.
// If ModNum is set to less than the total number of modules in the system, only the module specified by ModNum will start the RAMP_OFFSETDACS control task run. But if ModNum is equal to the total number of modules in the system, e.g. there are 5 modules in the chassis and ModNum = 5, then all modules in the system will start the RAMP_OFFSETDACS control task run. Note that the modules are counted starting at 0.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16RampOffsetDACs (
	double *DCValues,             // returned DC offset values
	unsigned short NumDCVals,     // number of DC values to read
	unsigned short ModNum );
```

```cpp
// Execute special control tasks
// Use this function to call special control tasks. This may include programming the Fippi or setting the DACs after downloading DSP parameters.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ControlTaskRun (
	unsigned short ModNum,        // Pixie module number
	unsigned short ControlTask,   // Control task number
	unsigned int   Max_Poll );    // Timeout control in unit of ms for control task run
```

```cpp
// Find the Baseline Cut values of a module
// Use this function to find the Baseline Cut value for one channel of a Pixie-16 module. The baseline cut value is then downloaded to the DSP, where baselines are captured and averaged over time. The cut value would prevent a bad baseline value from being used in the averaging process, i.e., if a baseline value is outside the baseline cut range, it will not be used for computing the baseline average. Averaging baselines over time improves energy resolution measurement.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16BLcutFinder (
	unsigned short ModNum,        // Pixie module number
	unsigned short ChanNum,       // Pixie channel number
	unsigned int   *BLcut );      // BLcut return value
```	

```cpp
// Find the exponential decay time of a channel
// Use this function to find the exponential decay time constant (Tau value) of the detector or preamplifier signal that is connected to one channel of a Pixie-16 module. The found Tau value is returned via pointer *Tau.	
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16TauFinder (
	unsigned short ModNum,        // Pixie module number
	double         *Tau );        // 16 returned Tau values, in 祍
```

```cpp
// Write a MODULE level parameter to a module
// Use this function to write a module parameter to a Pixie-16 module.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16WriteSglModPar (
	char *ModParName,             // the name of the module parameter
	unsigned int   ModParData,    // the module parameter value to be written to the module
	unsigned short ModNum );      // module number
```

```cpp
// Read a MODULE level parameter from a module
// Use this function to read a module parameter from a Pixie-16 module.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadSglModPar (
	char *ModParName,             // the name of the module parameter
	unsigned int   *ModParData,   // the module parameter value to be read from the module
	unsigned short ModNum );      // module number
```

```cpp
// Write a CHANNEL level parameter to a module
// Use this function to write a channel parameter to a Pixie-16 module.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16WriteSglChanPar (
	char *ChanParName,            // the name of the channel parameter
	double ChanParData,           // the channel parameter value to be written to the module
	unsigned short ModNum,        // module number
	unsigned short ChanNum );     // channel number
```

```cpp
// Read a CHANNEL level parameter from a module
// Use this function to read a channel parameter from a Pixie-16 module.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadSglChanPar (
	char *ChanParName,            // the name of the channel parameter
	double *ChanParData,          // the channel parameter value to be read from the module
	unsigned short ModNum,        // module number
	unsigned short ChanNum );     // channel number
```

```cpp
// Read histogram data from a module
// Use this function to read out the histogram data from a Pixie-16 module’s histogram memory. Before calling this function, the host code should allocate appropriate amount of memory to store the histogram data. The default histogram length is 32768. Histogram data are 32-bit unsigned integers.
// Specify the module using ModNum and the channel on the module using ChanNum. Note that both the modules and channels are counted starting at 0.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadHistogramFromModule (
	unsigned int   *Histogram,    // histogram data
	unsigned int   NumWords,      // number of words to be read out
	unsigned short ModNum,        // module number
	unsigned short ChanNum);      // channel number
```

```cpp
// Read run statistics data from a module
// Use this function to read out statistics data from a Pixie-16 module. Before calling this function, the host code should allocate appropriate amount of memory to store the statistics data. The number of statistics data for each module is fixed at 448. Statistics data are 32-bit unsigned integers.
// Specify the module using ModNum. Note that the modules are counted starting at 0.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadStatisticsFromModule (
	unsigned int   *Statistics,   // run statistics data
	unsigned short ModNum );      // module number
```

```cpp
// Read histogram data from a module and save to a file
// Use this function to read histogram data from a Pixie-16 module and save the data to a file. New data will be appended to the end of the file. So the same file name can be used for multiple modules and the data from each module will be stored in the order that this function is called.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16SaveHistogramToFile (
	char *FileName,               // histogram data file name
	unsigned short ModNum);       // module number
```

```cpp
// Parse a list mode data file to get events information
// Use this function to parse the list mode events in the list mode data file. The number of events for each module will be reported.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16GetModuleEvents (
	char *FileName,               // the list mode data file name (with complete path)
	unsigned int *ModuleEvents ); // receives number of events for each module
```

```cpp
// Get detailed events information from a data file
// Use this function to retrieve the detailed information of each event in the list mode data file for the designated module. Before calling this function to get the individual events information, another function Pixie16GetModuleEvents should be called first to determine the number of events that have been recorded for each module. If the number of events for a given module is nEvents, a memory block *EventInformation should be allocated with a length of (nEvents*68):
// EventInformation = (unsigned long *)malloc(sizeof(unsigned long) * nEvents * 68);
// where 68 is the length of the information records of each event (energy, timestamps, etc.) and has the following structure.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16GetEventsInfo (
	char *FileName,                   // the list mode data file name (with complete path)
	unsigned int   *EventInformation, // to hold event information
	unsigned short ModuleNumber);     // the module whose events are to be retrieved
```

```cpp
// Read trace data from a list mode data file
// Use this function to retrieve list mode trace from a list mode data file. It uses the trace length and file location information obtained from function Pixie16GetEventsInfo for the selected event.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadListModeTrace (
	char *FileName,               // list mode data file name
	unsigned short *Trace_Data,   // list mode trace data (16-bit words)
	unsigned short NumWords,      // number of 16-bit words to be read out
	unsigned int   FileLocation); // the location of the trace in the file
```

```cpp
// Read histogram data from a histogram data file
// Use this function to read histogram data from a histogram data file. Before calling this function, the host code should allocate appropriate amount of memory to store the histogram data. The default histogram length is 32768. Histogram data are 32-bit unsigned integers.
// Specify the module using ModNum and the channel on the module using ChanNum. Note that both the modules and channels are counted starting at 0.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadHistogramFromFile (
	char  *FileName,              // the histogram data file name (with complete path)
	unsigned int *Histogram,      // histogram data
	unsigned int NumWords,        // number of words to be read out
	unsigned short ModNum,        // module number
	unsigned short ChanNum);      // channel number
```

```cpp
// Read DSP parameters from modules and save to a file
// Use this function to save DSP parameters to a settings file. It will first read the values of DSP parameters on each Pixie-16 module and then write them to the settings file. Each module has exactly 1280 DSP parameter values (32-bit unsigned integers), and depending on the value of PRESET_MAX_MODULES (defined in pixie16app_defs.h), the settings file should have exactly (1280 * PRESET_MAX_MODULES * 4) bytes when stored on the computer hard drive.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16SaveDSPParametersToFile (
	char  *FileName );            // the DSP parameters file name (with complete path)
```

```cpp
// Load DSP parameters to modules from a file
// Use this function to read DSP parameters from a settings file and then download the settings to Pixie-16 modules that are installed in the system. Each module has exactly 1280 DSP parameter values (32-bit unsigned integers), and depending on the value of PRESET_MAX_MODULES (defined in pixie16app_defs.h), the settings file should have exactly (1280 * PRESET_MAX_MODULES * 4) bytes when stored on the computer hard drive.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16LoadDSPParametersFromFile (
	char  *FileName );            // the DSP parameters file name (with complete path)
```

```cpp
// Copy DSP parameters from a module to others
// Use this function to copy DSP parameters from one module to the others that are installed in the system.
// BitMask is bit pattern which designates which items should be copied from the source module to the destination module(s).
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16CopyDSPParameters (
	unsigned short BitMask,             // copy items bit mask
	unsigned short SourceModule,        // source module
	unsigned short SourceChannel,       // source channel
	unsigned short *DestinationMask );  // the destination module and channel bit mask
```

```cpp
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadMSGFile (
	char *ReturnMsgStr );
```

```cpp
// Convert a decimal into IEEE 32-bit floating point number
PIXIE16APP_EXPORT unsigned int PIXIE16APP_API Decimal2IEEEFloating(double DecimalNumber);

// Convert an IEEE 32-bit floating point number to a decimal
PIXIE16APP_EXPORT double PIXIE16APP_API IEEEFloating2Decimal(unsigned int IEEEFloatingNumber);
```

```cpp
// Read data from external FIFO and save to a file
// Use this function to read data from the external FIFO of a module. This function can only be used for Pixie-16 Revision-B, C, and D modules.
// This function first checks the status of the external FIFO of a Pixie-16 module, and if there are data in the external FIFO, this function then reads list mode data (32-bit unsigned integers) from the external FIFO. So this function essentially encapsulates both functions Pixie16CheckExternalFIFOStatus and Pixie16ReadDataFromExternalFIFO within one function. The number of words that are read from the external FIFO is recorded in variable*FIFOWords.
// The function also expects setting the value of a variable called “EndOfRunRead” to indicate whether this read is at the end of a run (1) or during the run (0). This is necessary since the external FIFO needs special treatment when the host reads the last few words from the external FIFO due to its pipelined structure.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16SaveExternalFIFODataToFile (
	char *FileName,               // list mode data file name
	unsigned int   *nFIFOWords,   // number of words read from external FIFO
	unsigned short ModNum,        // module number
	unsigned short EndOfRunRead); // indicator whether this is the end of run read
```

```cpp
// Read from or write to registers on a module
// Use this function to read data from or write data to a register in a Pixie-16 module.
// Specify the module using ModNum. Note that the modules are counted starting at 0.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16RegisterIO (
	unsigned short ModNum,        // the Pixie module to communicate to
	unsigned int   address,       // register address
	unsigned short direction,     // either MOD_READ or MOD_WRITE
	unsigned int   *value );      // holds or receives the data
```	

```cpp
// Read Control & Status Register value from a module
// Use this function to read the host Control & Status Register (CSR) value.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadCSR (
	unsigned short ModNum,
	unsigned int   *CSR );
```

```cpp
// Write to Control & Status Register in a module
// Use this function to write a value to the host Control & Status Register (CSR).
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16WriteCSR (
	unsigned short ModNum,
	unsigned int   CSR );
```

```cpp
// Check status of external FIFO of a module
// Use this function to check the status of the external FIFO of a Pixie-16 module while a list mode data acquisition run is in progress. The function returns the number of words (32-bit) that the external FIFO currently has. If the number of words is greater than a user-set threshold, function Pixie16ReadDataFromExternalFIFO can then be used to read the data from the external FIFO. The threshold can be set by the user to either minimize reading overhead or to read data out of the FIFO as quickly as possible.
// *nFIFOWords returns the number of 32-bit words that the external FIFO currently has.
// ModNum is the module number which starts counting at 0.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16CheckExternalFIFOStatus (
	unsigned int   *nFIFOWords,
	unsigned short ModNum );
```

```cpp
// Read data from external FIFO of a module
// Use this function to read data from the external FIFO of a module. This function can only be used for Pixie-16 Revision-B, C, and D modules.
// This function reads list mode data from the external FIFO of a Pixie-16 module. The data are 32-bit unsigned integers. Normally, function Pixie16CheckExternalFIFOStatus is called first to see how many words the external FIFO currently has, then this function is called to read the data from the FIFO.	
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadDataFromExternalFIFO (
	unsigned int   *ExtFIFO_Data, // To receive the external FIFO data
	unsigned int   nFIFOWords,    // number of words to read from external FIFO
	unsigned short ModNum );      // module number
```

```cpp
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ComputeFastFiltersOffline (
	char           *FileName,          // the list mode data file name (with complete path)
	unsigned short ModuleNumber,       // the module whose events are to be analyzed
	unsigned short ChannelNumber,      // the channel whose events are to be analyzed
	unsigned int   FileLocation,       // the location of the trace in the file
	unsigned short RcdTraceLength,     // recorded trace length
	unsigned short *RcdTrace,          // recorded trace
	double         *fastfilter,        // fast filter response
	double         *cfd );             // cfd response
```

```cpp
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ComputeSlowFiltersOffline (
	char           *FileName,          // the list mode data file name (with complete path)
	unsigned short ModuleNumber,       // the module whose events are to be analyzed
	unsigned short ChannelNumber,      // the channel whose events are to be analyzed
	unsigned int   FileLocation,       // the location of the trace in the file
	unsigned short RcdTraceLength,     // recorded trace length
	unsigned short *RcdTrace,          // recorded trace
	double         *slowfilter );      // slow filter response
```

```cpp
// Add by Hongyi Wu
PIXIE16APP_EXPORT int PIXIE16APP_API HongyiWuPixie16ComputeSlowFiltersOffline (
	char           *FileName,          // the list mode data file name (with complete path)
	unsigned short ModuleNumber,       // the module whose events are to be analyzed
	unsigned short ChannelNumber,      // the channel whose events are to be analyzed
	unsigned int   FileLocation,       // the location of the trace in the file
	unsigned short RcdTraceLength,     // recorded trace length
	unsigned short *RcdTrace,          // recorded trace
	double         *slowfilter,        // slow filter response
	unsigned int   bl,
	double         sl,
	double         sg,
	double         tau,
	int            sfr,
	int            pointtobl );
```


<!-- XIAAPI.md ends here -->
