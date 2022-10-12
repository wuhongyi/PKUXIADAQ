//! [PixieGetModuleInfo]
unsigned short mod_num = 0;
module_config cfg;
int retval = PixieGetModuleInfo(mod_num, &cfg);
if (retval < 0) {
    // error handling
}
//! [PixieGetModuleInfo]

//! [Pixie16AcquireADCTrace]
unsigned short module_number = 0;
int retval = Pixie16AcquireADCTrace(module_number);
if (retval < 0) {
    // error handling
}
//! [Pixie16AcquireADCTrace]

//! [Pixie16AcquireBaselines]
unsigned short ModNum = 0;
int retval = Pixie16AcquireBaselines(ModNum);
if (retval < 0) {
    // Error handling
}
//! [Pixie16AcquireBaselines]

//! [Pixie16AdjustOffsets]
unsigned short ModNum = 0;
int retval = Pixie16AdjustOffsets(ModNum);
if (retval < 0) {
    // Error handling
}
//! [Pixie16AdjustOffsets]

//! [Pixie16BLcutFinder]
unsigned short ModNum = 0;
unsigned short ChanNum = 0;
unsigned int BLcut;
int retval = Pixie16BLcutFinder(ModNum, ChanNum, &BLcut);
if (retval < 0) {
    // Error handling
}
//! [Pixie16BLcutFinder]

//! [Pixie16BootModule]
/*
 * We will assume that there are a total of 5 modules in the system and that
 * they are all the same type. Modules can be booted individually by calling the
 * module's number.
 */
unsigned short ModNum = 5;
// We assume that this is the first time the modules were booted after power-up.
unsigned short BootPattern = 0x7F;
int retval = Pixie16BootModule(
    "C:\\XIA\\Pixie16\\Firmware\\syspixie16.bin", "C:\\XIA\\Pixie16\\Firmware\\fippixie16.bin",
    null, "C:\\XIA\\Pixie16\\DSP\\Pixie16DSP.ldr", "C:\\XIA\\Pixie16\\Configuration\\default.set",
    "C:\\XIA\\Pixie16\\DSP\\Pixie16DSP.var", ModNum, BootPattern);
if (retval < 0) {
    // Error handling
}
//! [Pixie16BootModule]

//! [Pixie16CheckExternalFIFOStatus]
unsigned int nFIFOWords;
unsigned short ModNum = 0;
int retval = Pixie16CheckExternalFIFOStatus(&nFIFOWords, ModNum);
if (retval < 0) {
    // Error handling
}
//! [Pixie16CheckExternalFIFOStatus]

//! [Pixie16CheckRunStatus]
unsigned short ModNum = 0;
int retval = Pixie16CheckRunStatus(ModNum);
if (retval < 0) {
    // Error handling
} else if (retval == 1) {
    // run is still in progress
} else if (retval == 0) {
    // run has finished
}
//! [Pixie16CheckRunStatus]

//! [Pixie16ComputeInputCountRate]
std::vector<unsigned int> stats(Pixie16GetStatisticsSize(), 0);
unsigned short ModNum = 0;
// First call Pixie16ReadStatisticsFromModule to get the statistics data
if (Pixie16ReadStatisticsFromModule(stats.data(), ModNum) < 0) {
    // Error handling
}

unsigned short ChanNum = 0;
double icr = Pixie16ComputeInputCountRate(stats.data(), ModNum, ChanNum);
//! [Pixie16ComputeInputCountRate]

//! [Pixie16ComputeLiveTime]
std::vector<unsigned int> stats(Pixie16GetStatisticsSize(), 0);
unsigned short ModNum = 0;
// First call Pixie16ReadStatisticsFromModule to get the statistics data
if (Pixie16ReadStatisticsFromModule(stats.data(), ModNum) < 0) {
    // Error handling
}

unsigned short ChanNum = 0;
double livetime = Pixie16ComputeLiveTime(stats.data(), ModNum, ChanNum);
//! [Pixie16ComputeLiveTime]

//! [Pixie16ComputeOutputCountRate]
std::vector<unsigned int> stats(Pixie16GetStatisticsSize(), 0);
unsigned short ModNum = 0;
// First call Pixie16ReadStatisticsFromModule to get the statistics data
if (Pixie16ReadStatisticsFromModule(stats.data(), ModNum) < 0) {
    // Error handling
}

unsigned short ChanNum = 0;
double ocr = Pixie16ComputeOutputCountRate(stats.data(), ModNum, ChanNum);
//! [Pixie16ComputeOutputCountRate]

//! [Pixie16ComputeProcessedEvents]
std::vector<unsigned int> stats(Pixie16GetStatisticsSize(), 0);
unsigned short ModNum = 0;
// First call Pixie16ReadStatisticsFromModule to get the statistics data
if (Pixie16ReadStatisticsFromModule(stats.data(), ModNum) < 0) {
    // Error handling
}

unsigned short ChanNum = 0;
// compute number of processed events
double NumEvents = Pixie16ComputeProcessedEvents(stats.data(), ModNum);
//! [Pixie16ComputeProcessedEvents]

//! [Pixie16ComputeRawInputCount]
std::vector<unsigned int> stats(Pixie16GetStatisticsSize(), 0);
unsigned short ModNum = 0;
// First call Pixie16ReadStatisticsFromModule to get the statistics data
if (Pixie16ReadStatisticsFromModule(stats.data(), ModNum) < 0) {
    // Error handling
}

unsigned short ChanNum = 0;
// compute number of processed events
double raw_input_count = Pixie16ComputeRawInputCount(stats.data(), ModNum);
//! [Pixie16ComputeRawInputCount]

//! [Pixie16ComputeRawOutputCount]
std::vector<unsigned int> stats(Pixie16GetStatisticsSize(), 0);
unsigned short ModNum = 0;
// First call Pixie16ReadStatisticsFromModule to get the statistics data
if (Pixie16ReadStatisticsFromModule(stats.data(), ModNum) < 0) {
    // Error handling
}

unsigned short ChanNum = 0;
// compute number of processed events
double raw_output_count = Pixie16ComputeRawOutputCount(stats.data(), ModNum);
//! [Pixie16ComputeRawOutputCount]

//! [Pixie16ComputeRealTime]
std::vector<unsigned int> stats(Pixie16GetStatisticsSize(), 0);
unsigned short ModNum = 0;
// First call Pixie16ReadStatisticsFromModule to get the statistics data
if (Pixie16ReadStatisticsFromModule(stats.data(), ModNum) < 0) {
    // Error handling
}

unsigned short ChanNum = 0;
// compute real time
double realtime = Pixie16ComputeRealTime(stats.data(), ModNum);
//! [Pixie16ComputeRealTime]

//! [Pixie16CopyDSPParameters]
//Copies everything
unsigned short BitMask = 0x1FF;
unsigned short SourceModule = 0;
unsigned short SourceChannel = 0;
unsigned short DestinationMask[384];

// Assume there are 6 Pixie-16 modules in the system, and we copy to all of them
for (unsigned short k = 0; k < (6 * NUMBER_OF_CHANNELS); k++)
    DestinationMask[k] = 1;

int retval = Pixie16CopyDSPParameters(BitMask, SourceModule, SourceChannel, DestinationMask);
if (retval < 0) {
    // Error handling
}
//! [Pixie16CopyDSPParameters]

//! [Pixie16EndRun]
unsigned short ModNum = 0;
if (Pixie16EndRun(ModNum) < 0) {
    // Error handling
}
//! [Pixie16EndRun]

//! [Pixie16ExitSystem - single module]
unsigned short ModNum = 0;
if (Pixie16ExitSystem(ModNum) < 0) {
    // Error handling
}
//! [Pixie16ExitSystem - single module]

//! [Pixie16ExitSystem - all modules]
// Assumes that there are 5 modules in the system.
unsigned short ModNum = 5;
if (Pixie16ExitSystem(ModNum) < 0) {
    // Error handling
}
//! [Pixie16ExitSystem - all modules]

//! [Pixie16InitSystem]
std::vector<unsigned short> PXISlotMap = {2, 3, 4, 5, 6};
unsigned short OfflineMode = 0;
if (Pixie16InitSystem(PXISlotMap.size(), PXISlotMap, OfflineMode) < 0) {
    // Error handling
}
//! [Pixie16InitSystem]

//! [Pixie16ReadDataFromExternalFIFO]
unsigned short ModNum = 0;
unsigned int nFIFOWords;
// First, check how many words need to be read out of the FIFO.
if (Pixie16CheckExternalFIFOStatus(&nFIFOWords, ModNum) < 0) {
    // Error handling
}

// Second, initialize a storage location large enough to hold all the data.
std::vector<uint32_t> data(nFIFOWords, 0);
// Finally, read the data from the external FIFO.
if (nFIFOWords > 0) {
    if (Pixie16ReadDataFromExternalFIFO(data.data(), nFIFOWords, ModNum) < 0) {
        // Error handling
    }
}
//! [Pixie16ReadDataFromExternalFIFO]

//! [Pixie16LoadDSPParametersFromFile]
if (Pixie16LoadDSPParametersFromFile("C:\\XIA\\Pixie16\\Configuration\\test.json") < 0) {
    // Error handling
}
//! [Pixie16LoadDSPParametersFromFile]

//! [Pixie16ReadHistogramFromModule]
unsigned short ModNum = 0;
unsigend short ChanNum = 0;
std::vector<unsigned int> Histogram(MAX_HISTOGRAM_LENGTH, 0);
int retval =
    Pixie16ReadHistogramFromModule(Histogram.data(), MAX_HISTOGRAM_LENGTH, ModNum, ChanNum);
if (retval < 0) {
    // Error handling
}
//! [Pixie16ReadHistogramFromModule]

//! [Pixie16ReadModuleInfo]
unsigned short ModuleNumber;
unsigned short ModRev;
unsigned int ModSerNum;
unsigned short ModADCBits;
unsigned short ModADCMSPS;
if (Pixie16ReadModuleInfo(ModuleNumber, &ModRev, &ModSerNum, &ModADCBits, &ModADCMSPS) < 0) {
    // Error handling
}
//! [Pixie16ReadModuleInfo]

//! [Pixie16ReadSglChanADCTrace]
unsigned short ModNum = 0;
unsigned short ChanNum = 0;

// First, acquire the ADC trace from the hardware
if (Pixie16AcquireADCTrace(ModNum) < 0) {
    // Error handling
}

// Second, allocate a storage object to hold the resulting trace.
std::vector<unsigned short> ADCTrace(MAX_ADC_TRACE_LEN, 0);

// Finally, read the trace from the module object.
if (Pixie16ReadSglChanADCTrace(ADCTrace, MAX_ADC_TRACE_LEN, ModNum, ChanNum) < 0) {
    // Error handling
}
//! [Pixie16ReadSglChanADCTrace]

//! [Pixie16ReadSglChanBaselines]
unsigned short ModNum = 0;

// First, capture the baseline measurements from the hardware.
if (Pixie16AcquireBaselines(ModNum) < 0) {
    // Error handling
}

unsigned short ChanNum = 0;
std::vector<double> baselines(MAX_NUM_BASELINES);
std::vector<double> timestamps(MAX_NUM_BASELINES);

// Finally, read a single channel's baseline from the local memory.
if (Pixie16ReadSglChanBaselines(baselines, timestamps, MAX_NUM_BASELINES, ModNum, ChanNum) < 0) {
    // Error handling
}
//! [Pixie16ReadSglChanBaselines]

//! [Pixie16ReadSglChanPar]
unsigned short ModNum = 0;
unsigned short ChanNum = 0;
double ChanParData;
if (Pixie16ReadSglChanPar("ENERGY_RISETIME", &ChanParData, ModNum, ChanNum) < 0) {
    // Error handling
}
//! [Pixie16ReadSglChanPar]

//! [Pixie16ReadSglModPar]
unsigned short ModNum = 0;
unsigned int ModParData;
if (Pixie16ReadSglModPar("SLOW_FILTER_RANGE", &ModParData, ModNum) < 0) {
    // Error handling
}
//! [Pixie16ReadSglModPar]

//! [Pixie16ReadStatisticsFromModule]
std::vector<unsigned int> stats(Pixie16GetStatisticsSize(), 0);
unsigned short ModNum = 0;
// First call Pixie16ReadStatisticsFromModule to get the statistics data
if (Pixie16ReadStatisticsFromModule(stats.data(), ModNum) < 0) {
    // Error handling
}
//! [Pixie16ReadStatisticsFromModule]

//! [Pixie16SaveDSPParametersToFile]
if (Pixie16SaveDSPParametersToFile("C:\\XIA\\Pixie16\\Configuration\\test.json") < 0) {
    // Error handling
}
//! [Pixie16SaveDSPParametersToFile]

//! [Pixie16SaveHistogramToFile]
unsigned short ModNum = 0;
if (Pixie16SaveHistogramToFile("C:\\XIA\\Pixie16\\MCA\\histogramdata.bin", ModNum) < 0) {
    // Error handling
}
//! [Pixie16SaveHistogramToFile]

//! [Pixie16SetDACs]
unsigned short ModNum = 0;
if (Pixie16SetDACs(ModNum) < 0) {
    // Error handling
}
//! [Pixie16SetDACs]

//! [Pixie16StartHistogramRun]
// We will assume that there are 5 total modules in the system.
unsigned short mode = NEW_RUN;
unsigned short ModNum = 5;
double runtime_in_seconds = 10.0;

// First, write the run time to the hardware as an IEEE 754 formatted number.
if (Pixie16WriteSglModPar("HOST_RT_PRESET", Decimal2IEEEFloating(runtime_in_seconds), ModNum) < 0) {
    // Error handling
}

// Second, start the histogram data run.
if (Pixie16StartHistogramRun(ModNum, mode) < 0) {
    // Error handling
}
//! [Pixie16StartHistogramRun]

//! [Pixie16StartListModeRun]
// We will assume 5 modules in the system, and will start a run in all modules.
unsigned short mode = NEW_RUN;
unsigned short ModNum = 5;
unsigned short RunType = 0x100;
if (Pixie16StartListModeRun(ModNum, RunType, mode) < 0) {
    // Error handling
}
//! [Pixie16StartListModeRun]

//! [Pixie16TauFinder]
unsigned short ModNum = 0;
vector<double> Tau(NUMBER_OF_CHANNELS);
if (Pixie16TauFinder(ModNum, Tau) < 0) {
    // Error handling
}
//! [Pixie16TauFinder]

//! [Pixie16WriteSglChanPar]
unsigned short ModNum = 0;
unsigned short ChanNum = 0;
// This parameter should be set in units of microseconds.
double ChanParData = 3.0;
if (Pixie16WriteSglChanPar("ENERGY_RISETIME", ChanParData, ModNum, ChanNum) < 0) {
    // Error handling
}
//! [Pixie16WriteSglChanPar]

//! [Pixie16WriteSglModPar]
unsigned short ModNum = 0;
unsigned int ModParData = 4;
if (Pixie16WriteSglModPar("SLOW_FILTER_RANGE", ModParData, ModNum) < 0) {
    // Error handling
}
//! [Pixie16WriteSglModPar]