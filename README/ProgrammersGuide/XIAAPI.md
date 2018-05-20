<!-- XIAAPI.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 六 4月 28 18:50:41 2018 (+0800)
;; Last-Updated: 六 5月 19 02:23:21 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 7
;; URL: http://wuhongyi.cn -->

# XIA API




```cpp
// Configure modules for communication in PXI chassis
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16InitSystem (
	unsigned short NumModules,    // total number of Pixie16 modules in the system
	unsigned short *PXISlotMap,   // an array containing the PXI slot number for each pixie16 module
	unsigned short OfflineMode ); // specify if the system is in offline mode

// Release user virtual addressees assigned to modules
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ExitSystem (
	unsigned short ModNum );      // module number

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadModuleInfo (
	unsigned short ModNum,        // module number
	unsigned short *ModRev,       // returned module revision
	unsigned int   *ModSerNum,    // returned module serial number
	unsigned short *ModADCBits,   // returned module ADC bits
	unsigned short *ModADCMSPS ); // returned module ADC sampling rate

// Boot modules so that they can be set up for data taking
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16BootModule (
	char *ComFPGAConfigFile,      // name of communications FPGA configuration file
	char *SPFPGAConfigFile,       // name of signal processing FPGA configuration file
	char *TrigFPGAConfigFile,     // name of trigger FPGA configuration file
	char *DSPCodeFile,            // name of executable code file for digital signal processor (DSP)
	char *DSPParFile,             // name of DSP parameter file
	char *DSPVarFile,             // name of DSP variable names file
	unsigned short ModNum,        // pixie module number
	unsigned short BootPattern ); // boot pattern bit mask

// Acquire ADC traces in single or multiple modules
// Use this function to acquire ADC traces from Pixie-16 modules. Specify the module using ModNum. If ModNum is set to be less than the total number of modules in the system, only the module specified by ModNum will have its ADC traces acquired. But if ModNum is equal to the total number of modules in the system, then all modules in the system will have their ADC traces acquired.
// After the successful return of this function, the DSP's internal memory will be filled with ADC trace data. A user's application software should then call another function Pixie16ReadSglChanADCTrace to read the ADC trace data out to the host computer, channel by channel.
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16AcquireADCTrace (
	unsigned short ModNum );      // module number

// Read ADC trace data from a channel in a module
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadSglChanADCTrace (
	unsigned short *Trace_Buffer, // trace data
	unsigned int Trace_Length,    // trace length
	unsigned short ModNum,        // module number
	unsigned short ChanNum );     // channel number

// Transfer data between host and DSP internal memory
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16IMbufferIO (
	unsigned int *Buffer,         // buffer data
	unsigned int NumWords,        // number of buffer words to read or write
	unsigned int Address,         // buffer address
	unsigned short Direction,     // I/O direction
	unsigned short ModNum );      // module number

// Transfer data between host and DSP external memory
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16EMbufferIO (
	unsigned int *Buffer,         // buffer data
	unsigned int NumWords,        // number of buffer words to read or write
	unsigned int Address,         // buffer address
	unsigned short Direction,     // I/O direction
	unsigned short ModNum );      // module number

// Start a list mode data acquisition run
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16StartListModeRun (
	unsigned short ModNum,        // module number
	unsigned short RunType,       // run type
	unsigned short mode );        // run mode

// Start a MCA histogram mode data acquisition run
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16StartHistogramRun (
	unsigned short ModNum,        // module number
	unsigned short mode );        // run mode

// Check status of a data acquisition run
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16CheckRunStatus (
	unsigned short ModNum );      // Pixie module number  

// Stop a data acquisition run
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16EndRun (
	unsigned short ModNum );      // Pixie module number  

// Compute input count rate
PIXIE16APP_EXPORT double PIXIE16APP_API Pixie16ComputeInputCountRate (
	unsigned int   *Statistics,
	unsigned short ModNum,
	unsigned short ChanNum );

// Compute output count rate of a channel
PIXIE16APP_EXPORT double PIXIE16APP_API Pixie16ComputeOutputCountRate (
	unsigned int   *Statistics,
	unsigned short ModNum,
	unsigned short ChanNum );

// Compute live time that a channel accumulated in a run
PIXIE16APP_EXPORT double PIXIE16APP_API Pixie16ComputeLiveTime (
	unsigned int   *Statistics,
	unsigned short ModNum,
	unsigned short ChanNum );

// Compute number of events processed by a channel
PIXIE16APP_EXPORT double PIXIE16APP_API Pixie16ComputeProcessedEvents (
	unsigned int   *Statistics,
	unsigned short ModNum );

// Compute real time that a channel accumulated in a run
PIXIE16APP_EXPORT double PIXIE16APP_API Pixie16ComputeRealTime (
	unsigned int   *Statistics,
	unsigned short ModNum );

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16complexFFT (
	double *data,
	unsigned int length );

PIXIE16APP_EXPORT unsigned short PIXIE16APP_API APP16_TstBit (
	unsigned short bit,
	unsigned short value );

PIXIE16APP_EXPORT unsigned short PIXIE16APP_API APP16_SetBit (
	unsigned short bit,
	unsigned short value );

PIXIE16APP_EXPORT unsigned short PIXIE16APP_API APP16_ClrBit (
	unsigned short bit,
	unsigned short value );

PIXIE16APP_EXPORT unsigned int PIXIE16APP_API APP32_SetBit ( 
	unsigned short bit,
	unsigned int   value );

PIXIE16APP_EXPORT unsigned int PIXIE16APP_API APP32_ClrBit (
	unsigned short bit,
	unsigned int   value );

PIXIE16APP_EXPORT unsigned int PIXIE16APP_API APP32_TstBit (
	unsigned short bit,
	unsigned int   value );

// Program on-board DACs
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16SetDACs (
	unsigned short ModNum );

// Program on-board signal processing FPGAs
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ProgramFippi (
	unsigned short ModNum );


// Adjust DC-offsets in single or multiple modules
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16AdjustOffsets (
	unsigned short ModNum );

// Acquire baselines from a module
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16AcquireBaselines (
	unsigned short ModNum );      // module number

// Read baselines from a channel in a module
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadSglChanBaselines (
	double *Baselines,            // returned baselines values
	double *TimeStamps,           // time stamp for each baseline value
	unsigned short NumBases,      // number of baseline values to read
	unsigned short ModNum,        // module number
	unsigned short ChanNum );     // channel number

// Ramp Offset DACs of a module and record the baselines
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16RampOffsetDACs (
	double *DCValues,             // returned DC offset values
	unsigned short NumDCVals,     // number of DC values to read
	unsigned short ModNum );

// Execute special control tasks
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ControlTaskRun (
	unsigned short ModNum,        // Pixie module number
	unsigned short ControlTask,   // Control task number
	unsigned int   Max_Poll );    // Timeout control in unit of ms for control task run

// Find the Baseline Cut values of a module
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16BLcutFinder (
	unsigned short ModNum,        // Pixie module number
	unsigned short ChanNum,       // Pixie channel number
	unsigned int   *BLcut );      // BLcut return value
	
// Find the exponential decay time of a channel	
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16TauFinder (
	unsigned short ModNum,        // Pixie module number
	double         *Tau );        // 16 returned Tau values, in 祍

// Write a MODULE level parameter to a module
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16WriteSglModPar (
	char *ModParName,             // the name of the module parameter
	unsigned int   ModParData,    // the module parameter value to be written to the module
	unsigned short ModNum );      // module number

// Read a MODULE level parameter from a module
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadSglModPar (
	char *ModParName,             // the name of the module parameter
	unsigned int   *ModParData,   // the module parameter value to be read from the module
	unsigned short ModNum );      // module number

// Write a CHANNEL level parameter to a module
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16WriteSglChanPar (
	char *ChanParName,            // the name of the channel parameter
	double ChanParData,           // the channel parameter value to be written to the module
	unsigned short ModNum,        // module number
	unsigned short ChanNum );     // channel number

// Read a CHANNEL level parameter from a module
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadSglChanPar (
	char *ChanParName,            // the name of the channel parameter
	double *ChanParData,          // the channel parameter value to be read from the module
	unsigned short ModNum,        // module number
	unsigned short ChanNum );     // channel number

// Read histogram data from a module
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadHistogramFromModule (
	unsigned int   *Histogram,    // histogram data
	unsigned int   NumWords,      // number of words to be read out
	unsigned short ModNum,        // module number
	unsigned short ChanNum);      // channel number

// Read run statistics data from a module
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadStatisticsFromModule (
	unsigned int   *Statistics,   // run statistics data
	unsigned short ModNum );      // module number

// Read histogram data from a module and save to a file
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16SaveHistogramToFile (
	char *FileName,               // histogram data file name
	unsigned short ModNum);       // module number

// Parse a list mode data file to get events information
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16GetModuleEvents (
	char *FileName,               // the list mode data file name (with complete path)
	unsigned int *ModuleEvents ); // receives number of events for each module

// Get detailed events information from a data file
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16GetEventsInfo (
	char *FileName,                   // the list mode data file name (with complete path)
	unsigned int   *EventInformation, // to hold event information
	unsigned short ModuleNumber);     // the module whose events are to be retrieved

// Read trace data from a list mode data file
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadListModeTrace (
	char *FileName,               // list mode data file name
	unsigned short *Trace_Data,   // list mode trace data (16-bit words)
	unsigned short NumWords,      // number of 16-bit words to be read out
	unsigned int   FileLocation); // the location of the trace in the file

// Read histogram data from a histogram data file
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadHistogramFromFile (
	char  *FileName,              // the histogram data file name (with complete path)
	unsigned int *Histogram,      // histogram data
	unsigned int NumWords,        // number of words to be read out
	unsigned short ModNum,        // module number
	unsigned short ChanNum);      // channel number

// Read DSP parameters from modules and save to a file
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16SaveDSPParametersToFile (
	char  *FileName );            // the DSP parameters file name (with complete path)

// Load DSP parameters to modules from a file
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16LoadDSPParametersFromFile (
	char  *FileName );            // the DSP parameters file name (with complete path)

// Copy DSP parameters from a module to others
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16CopyDSPParameters (
	unsigned short BitMask,             // copy items bit mask
	unsigned short SourceModule,        // source module
	unsigned short SourceChannel,       // source channel
	unsigned short *DestinationMask );  // the destination module and channel bit mask

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadMSGFile (
	char *ReturnMsgStr );

PIXIE16APP_EXPORT unsigned int PIXIE16APP_API Decimal2IEEEFloating(double DecimalNumber);

PIXIE16APP_EXPORT double PIXIE16APP_API IEEEFloating2Decimal(unsigned int IEEEFloatingNumber);


// Read data from external FIFO and save to a file
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16SaveExternalFIFODataToFile (
	char *FileName,               // list mode data file name
	unsigned int   *nFIFOWords,   // number of words read from external FIFO
	unsigned short ModNum,        // module number
	unsigned short EndOfRunRead); // indicator whether this is the end of run read
	
// Read from or write to registers on a module
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16RegisterIO (
	unsigned short ModNum,        // the Pixie module to communicate to
	unsigned int   address,       // register address
	unsigned short direction,     // either MOD_READ or MOD_WRITE
	unsigned int   *value );      // holds or receives the data
	
// Read Control & Status Register value from a module
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadCSR (
	unsigned short ModNum,
	unsigned int   *CSR );
	
// Write to Control & Status Register in a module
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16WriteCSR (
	unsigned short ModNum,
	unsigned int   CSR );

// Check status of external FIFO of a module
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16CheckExternalFIFOStatus (
	unsigned int   *nFIFOWords,
	unsigned short ModNum );
	
// Read data from external FIFO of a module
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadDataFromExternalFIFO (
	unsigned int   *ExtFIFO_Data, // To receive the external FIFO data
	unsigned int   nFIFOWords,    // number of words to read from external FIFO
	unsigned short ModNum );      // module number

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ComputeFastFiltersOffline (
	char           *FileName,          // the list mode data file name (with complete path)
	unsigned short ModuleNumber,       // the module whose events are to be analyzed
	unsigned short ChannelNumber,      // the channel whose events are to be analyzed
	unsigned int   FileLocation,       // the location of the trace in the file
	unsigned short RcdTraceLength,     // recorded trace length
	unsigned short *RcdTrace,          // recorded trace
	double         *fastfilter,        // fast filter response
	double         *cfd );             // cfd response

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ComputeSlowFiltersOffline (
	char           *FileName,          // the list mode data file name (with complete path)
	unsigned short ModuleNumber,       // the module whose events are to be analyzed
	unsigned short ChannelNumber,      // the channel whose events are to be analyzed
	unsigned int   FileLocation,       // the location of the trace in the file
	unsigned short RcdTraceLength,     // recorded trace length
	unsigned short *RcdTrace,          // recorded trace
	double         *slowfilter );      // slow filter response

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
