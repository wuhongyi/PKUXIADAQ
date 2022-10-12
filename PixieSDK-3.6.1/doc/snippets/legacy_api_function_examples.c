//! [Pixie16AcquireADCTrace]
unsigned short module_number = 0;
int retval = Pixie16AcquireADCTrace(module_number);
if (retval < 0) {
    // error handling
}
//! [Pixie16AcquireADCTrace]

//! [Pixie16AcquireBaselines]
unsigned short ModNum;
int retval;
// assume we want to acquire baselines for module 0
ModNum = 0;
// acquire the trace
retval = Pixie16AcquireBaselines(ModNum);
if (retval < 0) {
    //Error handling
}
//! [Pixie16AcquireBaselines]

//! [Pixie16AdjustOffsets]
int retval;
unsigned short ModNum;
// set to the first module
ModNum = 0;
// adjust dc-offsets
retval = Pixie16AdjustOffsets(ModNum);
if (retval < 0) {
    //Error handling
}
//! [Pixie16AdjustOffsets]

//! [Pixie16BLcutFinder]
int retval;
unsigned short ModNum, ChanNum;
unsigned int BLcut;
// set to the first module
ModNum = 0;
// set to the first channel
ChanNum = 0;
// find the BLcut value
retval = Pixie16BLcutFinder(ModNum, ChanNum, &BLcut);
if (retval < 0) {
    //Error handling
}
//! [Pixie16BLcutFinder]

//! [Pixie16BootModule]
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
retval = Pixie16BootModule(ComFPGAConfigFile, SPFPGAConfigFile, TrigFPGAConfigFile, DSPCodeFile,
                           DSPParFile, DSPVarFile, ModNum, BootPattern);
if (retval < 0) {
    //Error handling
}
//! [Pixie16BootModule]

//! [Pixie16CheckExternalFIFOStatus]
int retval;
unsigned int nFIFOWords;
unsigned short ModNum;
ModNum = 0;  // the first module
retval = Pixie16CheckExternalFIFOStatus(&nFIFOWords, ModNum);
//! [Pixie16CheckExternalFIFOStatus]

//! [Pixie16CheckRunStatus]
int retval;
unsigned short ModNum;
ModNum = 0;  // the first module
retval = Pixie16CheckRunStatus(ModNum);
if (retval < 0) {
    // invalid module number
} else if (retval == 1) {
    // run is still in progress
} else if (retval == 0) {
    // run has finished
}
//! [Pixie16CheckRunStatus]

//! [Pixie16ComputeFastFiltersOffline]
int retval;
unsigned short ModuleNumber, ChannelNumber;
unsigned int FileLocation;
unsigned short RcdTraceLength;
unsigned short RcdTrace[1000];
double fastfilter[1000];
double cfd[1000];
char* FileName = {"C:\\XIA\\Pixie16\\PulseShape\\listmodedata.bin"};
ModuleNumber = 0;  // the first module
ChannelNumber = 0;  // the first channel
FileLocation = 16;  // the starting address of the trace in the
// list mode data file (32-bit word address)
RcdTraceLength = 1000;  // the length of the recorded trace
retval = Pixie16ComputeFastFiltersOffline(FileName, ModuleNumber, ChannelNumber, FileLocation,
                                          RcdTrace, fastfilter, cfd);
if (retval < 0) {
    //Error handling
}
//! [Pixie16ComputeFastFiltersOffline]

//! [Pixie16ComputeInputCountRate]
double icr;
unsigned int Statistics[448];
unsigned short ModNum, ChanNum;
int retval;
ModNum = 0;  // the first module
ChanNum = 0;  // the first channel
// first call Pixie16ReadStatisticsFromModule to get the statistics data
retval = Pixie16ReadStatisticsFromModule(Statistics, ModNum);
if (retval < 0) {
    //Error handling
}
// compute input count rate
icr = Pixie16ComputeInputCountRate(Statistics, ModNum, ChanNum);
//! [Pixie16ComputeInputCountRate]

//! [Pixie16ComputeLiveTime]
double livetime;
unsigned int Statistics[448];
unsigned short ModNum, ChanNum;
int retval;
ModNum = 0;  // the first module
ChanNum = 0;  // the first channel
// first call Pixie16ReadStatisticsFromModule to get the statistics data
retval = Pixie16ReadStatisticsFromModule(Statistics, ModNum);
if (retval < 0) {
    //Error handling
}
// compute live time
livetime = Pixie16ComputeLiveTime(Statistics, ModNum, ChanNum);
//! [Pixie16ComputeLiveTime]

//! [Pixie16ComputeOutputCountRate]
double ocr;
unsigned int Statistics[448];
unsigned short ModNum, ChanNum;
int retval;
ModNum = 0;  // the first module
ChanNum = 0;  // the first channel
// first call Pixie16ReadStatisticsFromModule to get the statistics data
retval = Pixie16ReadStatisticsFromModule(Statistics, ModNum);
if (retval < 0) {
    //Error handling
}
// compute output count rate
ocr = Pixie16ComputeOutputCountRate(Statistics, ModNum, ChanNum);
//! [Pixie16ComputeOutputCountRate]

//! [Pixie16ComputeProcessedEvents]
double NumEvents;
unsigned long Statistics[448];
unsigned short ModNum;
int retval;
ModNum = 0;  // the first module
// first call Pixie16ReadStatisticsFromModule to get the statistics data
retval = Pixie16ReadStatisticsFromModule(Statistics, ModNum);
if (retval < 0) {
    //Error handling
}
// compute number of processed events
NumEvents = Pixie16ComputeProcessedEvents(Statistics, ModNum);
//! [Pixie16ComputeProcessedEvents]

//! [Pixie16ComputeRealTime]
double realtime;
unsigned int Statistics[448];
unsigned short ModNum;
int retval;
ModNum = 0;  // the first module
// first call Pixie16ReadStatisticsFromModule to get the statistics data
retval = Pixie16ReadStatisticsFromModule(Statistics, ModNum);
if (retval < 0) {
    //Error handling
}
// compute real time
realtime = Pixie16ComputeRealTime(Statistics, ModNum);
//! [Pixie16ComputeRealTime]

//! [Pixie16ComputeSlowFiltersOffline]
int retval;
unsigned short ModuleNumber, ChannelNumber;
unsigned int FileLocation;
unsigned short RcdTraceLength;
unsigned short RcdTrace[1000];
double slowfilter[1000];
char* FileName = {"C:\\XIA\\Pixie16\\PulseShape\\listmodedata.bin"};
ModuleNumber = 0;  // the first module
ChannelNumber = 0;  // the first channel
FileLocation = 16;  // the starting address of the trace in the
// list mode data file (32-bit word address)
RcdTraceLength = 1000;  // the length of the recorded trace
retval = Pixie16ComputeSlowFiltersOffline(FileName, ModuleNumber, ChannelNumber, FileLocation,
                                          RcdTrace, slowfilter);
if (retval < 0) {
    //Error handling
}
//! [Pixie16ComputeSlowFiltersOffline]

//! [Pixie16ControlTaskRun]
int retval;
unsigned short ControlTask, ModNum;
unsigned int Max_Poll;
ModNum = 0;  // this is the first module
ControlTask = SET_DACS;  // constant SET_DACS defined in header file
Max_Poll = 10000;  // give the run 10 seconds to finish
retval = Pixie16ControlTaskRun(ControlTask, ModNum, Max_Poll);
if (retval < 0) {
    //Error handling
}
//! [Pixie16ControlTaskRun]

//! [Pixie16CopyDSPParameters]
int retval;
unsigned short BitMask;  // copy items bit mask
unsigned short SourceModule;  // source module
unsigned short SourceChannel;  // source channel
unsigned short DestinationMask[384];  // destination bit mask
unsigned short k;
BitMask = 0x1FF;  // copy everything
SourceModule = 0;  // the first module
SourceChannel = 0;  // the first channel
// assume there are 6 Pixie-16 modules in the system
for (k = 0; k < (6 * 16); k++) {
    DestinationMask[k] = 1;  // copy to all channels of all modules
}
retval = Pixie16CopyDSPParameters(BitMask, SourceModule, SourceChannel, DestinationMask);
if (retval < 0) {
    //Error handling
}
//! [Pixie16CopyDSPParameters]

//! [Pixie16EMbufferIO]
int retval;
unsigned short Direction, ModNum;
unsigned int MCAData[32768], NumWords, Address;
NumWords = 32768;  // to read out the MCA data from channel 0
ModNum = 0;  // the first module in the system
Address = 0x0;  // the starting memory address
Direction = 1;  // I/O direction is read
// read out the MCA data for Channel 0
retval = Pixie16EMbufferIO(MCAData, NumWords, Address, Direction, ModNum);
if (retval != 0) {
    //Error handling
}
//! [Pixie16EMbufferIO]

//! [Pixie16EndRun]
int retval;
unsigned short ModNum;
ModNum = 0;  // the first module
retval = Pixie16EndRun(ModNum);
if (retval < 0) {
    //Error handling
}
//! [Pixie16EndRun]

//! [Pixie16ExitSystem]
int retval;
unsigned short ModNum, k;
// assume there are totally 5 modules in the system
// so close all 5 modules
ModNum = 5;
retval = Pixie16ExitSystem(ModNum);
if (retval < 0) {
    //Error handling
}
// or module by module
for (k = 0; k < 5; k++) {
    retval = Pixie16ExitSystem(k);
    if (retval < 0) {
        //Error handling
    }
}
//! [Pixie16ExitSystem]

//! [Pixie16GetEventsInfo]
int retval;
char* FileName = {"C:\\XIA\\Pixie16\\PulseShape\\listmodedata.bin"};
unsigned short ModuleNumber;
unsigned int* EventInformation;
unsigned int ModuleEvents[7];  // assume maximum number of modules is 7
retval = Pixie16GetModuleEvents(FileName, ModuleEvents);
if (retval < 0) {
    //Error handling
}
ModuleNumber = 0;  // the first module
EventInformation = (unsigned int*) malloc(sizeof(unsigned int) * ModuleEvents[ModuleNumber] * 68);
retval = Pixie16GetEventsInfo(FileName, EventInformation, ModuleNumber);
if (retval < 0) {
    //Error handling
}
//! [Pixie16GetEventsInfo]

//! [Pixie16GetModuleEvents]
int retval;
char* FileName = {"C:\\XIA\\Pixie16\\PulseShape\\listmodedata.bin"};
unsigned int ModuleEvents[7];  // assume maximum number of modules is 7
retval = Pixie16GetModuleEvents(FileName, ModuleEvents);
if (retval < 0) {
    //Error handling
}
//! [Pixie16GetModuleEvents]

//! [Pixie16IMbufferIO]
int retval;
unsigned short Direction, ModNum;
unsigned int DSPMemBlock1[65536], NumWords, Address;
NumWords = 65536;  // to read out block 1 of the DSP internal memory
ModNum = 0;  // the first module in the system
Address = 0x50000;  // the starting address for block 1
Direction = 1;  // I/O direction is read
// read out the whole block 1 of the DSP internal memory
retval = Pixie16IMbufferIO(DSPMemBlock1, NumWords, Address, Direction, ModNum);
if (retval != 0) {
    //Error handling
}
//! [Pixie16IMbufferIO]

//! [Pixie16InitSystem]
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
retval = Pixie16InitSystem(NumModules, PXISlotMap, OfflineMode);
if (retval < 0) {
    //Error handling
}
//! [Pixie16InitSystem]

//! [Pixie16LoadDSPParametersFromFile]
int retval;
char* FileName = {"C:\\XIA\\Pixie16\\Configuration\\test.set"};
retval = Pixie16LoadDSPParametersFromFile(FileName);
if (retval < 0) {
    //Error handling
}
//! [Pixie16LoadDSPParametersFromFile]

//! [Pixie16ProgramFippi]
int retval;
unsigned short ModNum;
ModNum = 0;  // the first module
retval = Pixie16ProgramFippi(ModNum);
if (retval < 0) {
    //Error handling
}
//! [Pixie16ProgramFippi]

//! [Pixie16RampOffsetDACs]
int retval;
unsigned short ModNum;
ModNum = 0;  // the first module
retval = Pixie16RampOffsetDACs(ModNum);
if (retval < 0) {
    //Error handling
}
//! [Pixie16RampOffsetDACs]

//! [Pixie16ReadCSR]
unsigned short ModNum;
unsigned int CSR;
ModNum = 0;  // the first module
Pixie16ReadCSR(ModNum, &CSR);
//! [Pixie16ReadCSR]

//! [Pixie16ReadDataFromExternalFIFO]
int retval;
unsigned int nFIFOWords, *ExtFIFO_Data;
unsigned short ModNum;
ModNum = 0;  // the first module in the system
retval = Pixie16CheckExternalFIFOStatus(&nFIFOWords, ModNum);
if (retval < 0) {
    //Error handling
}
if (nFIFOWords > 0)  // Check if there is data in the external FIFO
{
    ExtFIFO_Data = (unsigned int*) malloc(sizeof(unsigned int) * nFIFOWords);
    retval = Pixie16ReadDataFromExternalFIFO(ExtFIFO_Data, nFIFOWords, ModNum);
    if (retval != 0) {
        //Error handling
    }
}
//! [Pixie16ReadDataFromExternalFIFO]

//! [Pixie16ReadHistogramFromFile]
int retval;
char* FileName = {"C:\\XIA\\Pixie16\\MCA\\histogramdata.bin"};
unsigned short ModNum, ChanNum;
unsigned int NumWords, Histogram[32768];
ModNum = 0;  // the first module
ChanNum = 0;  // the first channel
NumWords = 32768;
retval = Pixie16ReadHistogramFromFile(FileName, Histogram, NumWords, ModNum, ChanNum);
if (retval < 0) {
    //Error handling
}
//! [Pixie16ReadHistogramFromFile]

//! [Pixie16ReadHistogramFromModule]
int retval;
unsigned short ModNum, ChanNum;
unsigned int NumWords, Histogram[32768];
ModNum = 0;  // the first module
ChanNum = 0;  // the first channel
NumWords = 32768;
retval = Pixie16ReadHistogramFromModule(Histogram, NumWords, ModNum, ChanNum);
if (retval < 0) {
    //Error handling
}
//! [Pixie16ReadHistogramFromModule]

//! [Pixie16ReadListModeTrace]
int retval;
char* FileName = {"C:\\XIA\\Pixie16\\PulseShape\\listmodedata.bin"};
unsigned short ModuleNumber, ChannelNumber;
unsigned int *EventInformation, FileLocation, EventNumber;
unsigned short *Trace_Data, NumWords;
unsigned int ModuleEvents[7];  // assume maximum number of modules is 7
retval = Pixie16GetModuleEvents(FileName, ModuleEvents);
if (retval < 0) {
    //Error handling
}
ModuleNumber = 0;  // the first module
EventInformation = (unsigned int*) malloc(sizeof(unsigned int) * ModuleEvents[ModuleNumber] * 68);
retval = Pixie16GetEventsInfo(FileName, EventInformation, ModuleNumber);
if (retval < 0) {
    //Error handling
}
ChannelNumber = 0;  // the first channel
EventNumber = 0;  // the first event
NumWords = (unsigned short) EventInformation[EventNumber * 68 + 10] * 2;
FileLocation = EventInformation[EventNumber * 68 + 11];
Trace_Data = (unsigned short*) malloc(sizeof(unsigned short) * NumWords);
retval = Pixie16ReadListModeTrace(FileName, Trace_Data, NumWords, FileLocation);
if (retval < 0) {
    //Error handling
}
//! [Pixie16ReadListModeTrace]

//! [Pixie16ReadModuleInfo]
int retval;
unsigned short ModuleNumber;
unsigned short ModRev;
unsigned int ModSerNum;
unsigned short ModADCBits;
unsigned short ModADCMSPS;
retval = Pixie16ReadModuleInfo(ModuleNumber, &ModRev, &ModSerNum, &ModADCBits, &ModADCMSPS);
if (retval < 0) {
    //Error handling
}
//! [Pixie16ReadModuleInfo]

//! [Pixie16ReadSglChanADCTrace]
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
if (retval < 0) {
    //Error handling
}
// read out the trace
retval = Pixie16ReadSglChanADCTrace(ADCTrace, NumWords, ModNum, ChanNum);
if (retval < 0) {
    //Error handling
}
//! [Pixie16ReadSglChanADCTrace]

//! [Pixie16ReadSglChanBaselines]
unsigned short NumWords, ModNum, ChanNum;
int retval;
double Baselines[3640], TimeStamps[3640];
// assume we want to acquire baselines for channel 0 of module 0
ModNum = 0;
ChanNum = 0;
// number of baseline words is 3640
NumWords = 3640;
// acquire the baselines
retval = Pixie16AcquireBaselines(ModNum);
if (retval < 0) {
    //Error handling
}
// read out the baselines
retval = Pixie16ReadSglChanBaselines(Baselines, TimeStamps, NumWords, ModNum, ChanNum);
if (retval < 0) {
    //Error handling
}
//! [Pixie16ReadSglChanBaselines]

//! [Pixie16ReadSglChanPar]
int retval;
unsigned short ModNum, ChanNum;
double ChanParData;
// read energy filter rise time from module 0 channel 0
ModNum = 0;  // this is the first module
ChanNum = 0;  // the first channel
retval = Pixie16ReadSglChanPar("ENERGY_RISETIME", &ChanParData, ModNum, ChanNum);
if (retval < 0) {
    //Error handling
}
//! [Pixie16ReadSglChanPar]

//! [Pixie16ReadSglModPar]
int retval;
unsigned short ModNum;
unsigned int ModParData;
// Read SlowFilterRange in module 0
ModNum = 0;  // this is the first module
retval = Pixie16ReadSglModPar("SLOW_FILTER_RANGE", &ModParData, ModNum);
if (retval < 0) {
    //Error handling
}
//! [Pixie16ReadSglModPar]

//! [Pixie16ReadStatisticsFromModule]
int retval;
unsigned short ModNum, ChanNum;
unsigned int Statistics[448];
ModNum = 0;  // the first module
ChanNum = 0;  // the first channel
retval = Pixie16ReadStatisticsFromModule(Statistics, ModNum);
if (retval < 0) {
    //Error handling
}
//! [Pixie16ReadStatisticsFromModule]

//! [Pixie16RegisterIO]
int retval;
unsigned short ModNum;
unsigned int address, value;
ModNum = 0;  // the first module
address = PCI_STOPRUN_REGADDR;  // address of the register for ending run
value = 0;
retval = Pixie16RegisterIO(ModNum, address, MOD_WRITE, &value);
if (retval < 0) {
    //Error handling
}
//! [Pixie16RegisterIO]

//! [Pixie16SaveDSPParametersToFile]
int retval;
char* FileName = {"C:\\XIA\\Pixie16\\Configuration\\test.set"};
retval = Pixie16SaveDSPParametersToFile(FileName);
if (retval < 0) {
    //Error handling
}
//! [Pixie16SaveDSPParametersToFile]

//! [Pixie16SaveExternalFIFODataToFile]
int retval;
unsigned int nFIFOWords;
unsigned short ModNum, EndOfRunRead;
ModNum = 0;  // the first module in the system
EndOfRunRead = 0;  // this is a read during the run
retval =
    Pixie16SaveExternalFIFODataToFile("listmodedata_mod0.bin", &nFIFOWords, ModNum, EndOfRunRead);
if (retval < 0) {
    //Error handling
}
//! [Pixie16SaveExternalFIFODataToFile]

//! [Pixie16SaveHistogramToFile]
int retval;
char* FileName = {"C:\\XIA\\Pixie16\\MCA\\histogramdata.bin"};
unsigned short ModNum;
ModNum = 0;  // the first module
retval = Pixie16SaveHistogramToFile(FileName, ModNum);
if (retval < 0) {
    //Error handling
}
//! [Pixie16SaveHistogramToFile]

//! [Pixie16SetDACs]
int retval;
unsigned short ModNum;
ModNum = 0;  // the first module
retval = Pixie16SetDACs(ModNum);
if (retval < 0) {
    //Error handling
}
//! [Pixie16SetDACs]

//! [Pixie16StartHistogramRun]
int retval;
unsigned short mode, ModNum;
double preset_run_time;
unsigned int ieee_preset_run_time;
mode = NEW_RUN;  // to start a new run
// Assume there are 5 modules in the system
ModNum = 5;  // start histogram run in all 5 modules
// Assume preset run time is 10 seconds
preset_run_time = 10.0;
// Convert preset run time to IEEE 32-bit floating point number
ieee_preset_run_time = Decimal2IEEEFloating(preset_run_time);
// Download the preset run time to the DSP
retval = Pixie16WriteSglModPar("HOST_RT_PRESET", ieee_preset_run_time, ModNum) if (retval < 0) {
    //Error handling
}
// Start the histogram run
retval = Pixie16StartHistogramRun(ModNum, mode);
if (retval < 0) {
    //Error handling
}
//! [Pixie16StartHistogramRun]

//! [Pixie16StartListModeRun]
int retval;
unsigned short mode, ModNum, RunType;
mode = NEW_RUN;  // to start a new run
RunType = 0x100;  // general purpose list mode run
// Assume there are 5 modules in the system
ModNum = 5;  // start list mode run in all 5 modules
retval = Pixie16StartListModeRun(ModNum, RunType, mode);
if (retval < 0) {
    //Error handling
}
//! [Pixie16StartListModeRun]

//! [Pixie16TauFinder]
int retval;
unsigned short ModNum;
double Tau[16];
ModNum = 0;  // the first module
retval = Pixie16TauFinder(ModNum, Tau);
if (retval < 0) {
    //Error handling
}
//! [Pixie16TauFinder]

//! [Pixie16WriteCSR]
int retval;
unsigned short ModNum;
unsigned int CSR;
ModNum = 0;  // the first module
retval = Pixie16ReadCSR(ModNum, &CSR);
if (retval < 0) {
    //Error handling
}
CSR = APP32_ClrBit(3, CSR);
retval = Pixie16WriteCSR(ModNum, CSR);
if (retval < 0) {
    //Error handling
}
//! [Pixie16WriteCSR]

//! [Pixie16WriteSglChanPar]
int retval;
unsigned short ModNum, ChanNum;
double ChanParData;
// Set energy filter rise time to 6.08 us for module 0 channel 0
ModNum = 0;  // this is the first module
ChanNum = 0;  // the first channel
ChanParData = 6.08;  // energy filter rise time = 6.08 us
retval = Pixie16WriteSglChanPar("ENERGY_RISETIME", ChanParData, ModNum, ChanNum);
if (retval < 0) {
    //Error handling
}
//! [Pixie16WriteSglChanPar]

//! [Pixie16WriteSglModPar]
int retval;
unsigned short ModNum;
unsigned int ModParData;
// Set SlowFilterRange in module 0 to 4
ModNum = 0;  // this is the first module
ModParData = 4;  // SlowFilterRange = 4
retval = Pixie16WriteSglModPar("SLOW_FILTER_RANGE", ModParData, ModNum);
if (retval < 0) {
    //Error handling
}
//! [Pixie16WriteSglModPar]