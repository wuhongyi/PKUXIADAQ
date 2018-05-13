<!-- XIAAPI.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 六 4月 28 18:50:41 2018 (+0800)
;; Last-Updated: 日 5月 13 15:53:52 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 2
;; URL: http://wuhongyi.cn -->

# XIA API




```cpp
PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16InitSystem (
	unsigned short NumModules,    // total number of Pixie16 modules in the system
	unsigned short *PXISlotMap,   // an array containing the PXI slot number for each pixie16 module
	unsigned short OfflineMode ); // specify if the system is in offline mode

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ExitSystem (
	unsigned short ModNum );      // module number

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadModuleInfo (
	unsigned short ModNum,        // module number
	unsigned short *ModRev,       // returned module revision
	unsigned int   *ModSerNum,    // returned module serial number
	unsigned short *ModADCBits,   // returned module ADC bits
	unsigned short *ModADCMSPS ); // returned module ADC sampling rate

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16BootModule (
	char *ComFPGAConfigFile,      // name of communications FPGA configuration file
	char *SPFPGAConfigFile,       // name of signal processing FPGA configuration file
	char *TrigFPGAConfigFile,     // name of trigger FPGA configuration file
	char *DSPCodeFile,            // name of executable code file for digital signal processor (DSP)
	char *DSPParFile,             // name of DSP parameter file
	char *DSPVarFile,             // name of DSP variable names file
	unsigned short ModNum,        // pixie module number
	unsigned short BootPattern ); // boot pattern bit mask

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16AcquireADCTrace (
	unsigned short ModNum );      // module number

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadSglChanADCTrace (
	unsigned short *Trace_Buffer, // trace data
	unsigned int Trace_Length,    // trace length
	unsigned short ModNum,        // module number
	unsigned short ChanNum );     // channel number

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16IMbufferIO (
	unsigned int *Buffer,         // buffer data
	unsigned int NumWords,        // number of buffer words to read or write
	unsigned int Address,         // buffer address
	unsigned short Direction,     // I/O direction
	unsigned short ModNum );      // module number

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16EMbufferIO (
	unsigned int *Buffer,         // buffer data
	unsigned int NumWords,        // number of buffer words to read or write
	unsigned int Address,         // buffer address
	unsigned short Direction,     // I/O direction
	unsigned short ModNum );      // module number

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16StartListModeRun (
	unsigned short ModNum,        // module number
	unsigned short RunType,       // run type
	unsigned short mode );        // run mode

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16StartHistogramRun (
	unsigned short ModNum,        // module number
	unsigned short mode );        // run mode

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16CheckRunStatus (
	unsigned short ModNum );      // Pixie module number  

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16EndRun (
	unsigned short ModNum );      // Pixie module number  

PIXIE16APP_EXPORT double PIXIE16APP_API Pixie16ComputeInputCountRate (
	unsigned int   *Statistics,
	unsigned short ModNum,
	unsigned short ChanNum );

PIXIE16APP_EXPORT double PIXIE16APP_API Pixie16ComputeOutputCountRate (
	unsigned int   *Statistics,
	unsigned short ModNum,
	unsigned short ChanNum );

PIXIE16APP_EXPORT double PIXIE16APP_API Pixie16ComputeLiveTime (
	unsigned int   *Statistics,
	unsigned short ModNum,
	unsigned short ChanNum );

PIXIE16APP_EXPORT double PIXIE16APP_API Pixie16ComputeProcessedEvents (
	unsigned int   *Statistics,
	unsigned short ModNum );

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

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16SetDACs (
	unsigned short ModNum );

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ProgramFippi (
	unsigned short ModNum );

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16AdjustOffsets (
	unsigned short ModNum );

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16AcquireBaselines (
	unsigned short ModNum );      // module number

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadSglChanBaselines (
	double *Baselines,            // returned baselines values
	double *TimeStamps,           // time stamp for each baseline value
	unsigned short NumBases,      // number of baseline values to read
	unsigned short ModNum,        // module number
	unsigned short ChanNum );     // channel number

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16RampOffsetDACs (
	double *DCValues,             // returned DC offset values
	unsigned short NumDCVals,     // number of DC values to read
	unsigned short ModNum );

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ControlTaskRun (
	unsigned short ModNum,        // Pixie module number
	unsigned short ControlTask,   // Control task number
	unsigned int   Max_Poll );    // Timeout control in unit of ms for control task run

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16BLcutFinder (
	unsigned short ModNum,        // Pixie module number
	unsigned short ChanNum,       // Pixie channel number
	unsigned int   *BLcut );      // BLcut return value

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16TauFinder (
	unsigned short ModNum,        // Pixie module number
	double         *Tau );        // 16 returned Tau values, in 祍

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16WriteSglModPar (
	char *ModParName,             // the name of the module parameter
	unsigned int   ModParData,    // the module parameter value to be written to the module
	unsigned short ModNum );      // module number

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadSglModPar (
	char *ModParName,             // the name of the module parameter
	unsigned int   *ModParData,   // the module parameter value to be read from the module
	unsigned short ModNum );      // module number

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16WriteSglChanPar (
	char *ChanParName,            // the name of the channel parameter
	double ChanParData,           // the channel parameter value to be written to the module
	unsigned short ModNum,        // module number
	unsigned short ChanNum );     // channel number

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadSglChanPar (
	char *ChanParName,            // the name of the channel parameter
	double *ChanParData,          // the channel parameter value to be read from the module
	unsigned short ModNum,        // module number
	unsigned short ChanNum );     // channel number

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadHistogramFromModule (
	unsigned int   *Histogram,    // histogram data
	unsigned int   NumWords,      // number of words to be read out
	unsigned short ModNum,        // module number
	unsigned short ChanNum);      // channel number

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadStatisticsFromModule (
	unsigned int   *Statistics,   // run statistics data
	unsigned short ModNum );      // module number

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16SaveHistogramToFile (
	char *FileName,               // histogram data file name
	unsigned short ModNum);       // module number

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16GetModuleEvents (
	char *FileName,               // the list mode data file name (with complete path)
	unsigned int *ModuleEvents ); // receives number of events for each module

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16GetEventsInfo (
	char *FileName,                   // the list mode data file name (with complete path)
	unsigned int   *EventInformation, // to hold event information
	unsigned short ModuleNumber);     // the module whose events are to be retrieved

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadListModeTrace (
	char *FileName,               // list mode data file name
	unsigned short *Trace_Data,   // list mode trace data (16-bit words)
	unsigned short NumWords,      // number of 16-bit words to be read out
	unsigned int   FileLocation); // the location of the trace in the file

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadHistogramFromFile (
	char  *FileName,              // the histogram data file name (with complete path)
	unsigned int *Histogram,      // histogram data
	unsigned int NumWords,        // number of words to be read out
	unsigned short ModNum,        // module number
	unsigned short ChanNum);      // channel number

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16SaveDSPParametersToFile (
	char  *FileName );            // the DSP parameters file name (with complete path)

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16LoadDSPParametersFromFile (
	char  *FileName );            // the DSP parameters file name (with complete path)

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16CopyDSPParameters (
	unsigned short BitMask,             // copy items bit mask
	unsigned short SourceModule,        // source module
	unsigned short SourceChannel,       // source channel
	unsigned short *DestinationMask );  // the destination module and channel bit mask

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadMSGFile (
	char *ReturnMsgStr );

PIXIE16APP_EXPORT unsigned int PIXIE16APP_API Decimal2IEEEFloating(double DecimalNumber);

PIXIE16APP_EXPORT double PIXIE16APP_API IEEEFloating2Decimal(unsigned int IEEEFloatingNumber);

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16SaveExternalFIFODataToFile (
	char *FileName,               // list mode data file name
	unsigned int   *nFIFOWords,   // number of words read from external FIFO
	unsigned short ModNum,        // module number
	unsigned short EndOfRunRead); // indicator whether this is the end of run read

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16RegisterIO (
	unsigned short ModNum,        // the Pixie module to communicate to
	unsigned int   address,       // register address
	unsigned short direction,     // either MOD_READ or MOD_WRITE
	unsigned int   *value );      // holds or receives the data

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16ReadCSR (
	unsigned short ModNum,
	unsigned int   *CSR );

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16WriteCSR (
	unsigned short ModNum,
	unsigned int   CSR );

PIXIE16APP_EXPORT int PIXIE16APP_API Pixie16CheckExternalFIFOStatus (
	unsigned int   *nFIFOWords,
	unsigned short ModNum );

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
