// algorithm.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 7月 22 21:08:06 2016 (+0800)
// Last-Updated: 二 11月 22 21:03:28 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 40
// URL: http://wuhongyi.cn 

#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#define N_DSP_PAR                   1280      // number of DSP parameters (32-bit word)
#define MAX_PAR_NAME_LENGTH     65  // Maximum length of parameter names
#define DATA_MEMORY_ADDRESS      0x4A000      // DSP data memory address

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/*-----------------------------------------------------------------
    module specifications
  -----------------------------------------------------------------*/

#define PRESET_MAX_MODULES            24      // Preset maximum number of Pixie modules
#define NUMBER_OF_CHANNELS            16

#define DSP_CLOCK_MHZ                100      // DSP clock frequency in MHz
#define SYSTEM_CLOCK_MHZ             100      // System FPGA clock frequency in MHz (used for real time counting)

#define DAC_VOLTAGE_RANGE            3.0      // Pixie-16 DAC range is -1.5 V to +1.5 V

#define MAX_ADC_TRACE_LEN           8192      // Maximum ADC trace length for a channel
  
/*------------------------------------- 
	Length limits for certain DSP parameters
 --------------------------------------*/ 

#define FASTFILTER_MAX_LEN 127
#define FAST_THRESHOLD_MAX 65535
#define MIN_FASTLENGTH_LEN 2

#define SLOWFILTER_MAX_LEN 127
#define MIN_SLOWLENGTH_LEN 2
#define MIN_SLOWGAP_LEN 3

#define FASTFILTERRANGE_MAX 0
#define FASTFILTERRANGE_MIN 0

#define SLOWFILTERRANGE_MAX 6
#define SLOWFILTERRANGE_MIN 1

#define FASTTRIGBACKLEN_MAX 4095
#define FASTTRIGBACKLEN_MIN_100MHZFIPCLK 1
#define FASTTRIGBACKLEN_MIN_125MHZFIPCLK 2

#define CFDDELAY_MAX 63
#define CFDDELAY_MIN 1

#define CFDSCALE_MAX 7

#define CFDTHRESH_MAX 65535
#define CFDTHRESH_MIN 1

#define EXTTRIGSTRETCH_MAX 4095
#define EXTTRIGSTRETCH_MIN 1

#define VETOSTRETCH_MAX 4095
#define VETOSTRETCH_MIN 1

#define EXTDELAYLEN_MAX_REVBCD 255
#define EXTDELAYLEN_MAX_REVF 511
#define EXTDELAYLEN_MIN 0

#define FASTTRIGBACKDELAY_MAX_REVBCD 255
#define FASTTRIGBACKDELAY_MAX_REVF 511
#define FASTTRIGBACKDELAY_MIN 0

#define QDCLEN_MAX 32767
#define QDCLEN_MIN 1

#define TRACELEN_MIN_500MHZADC		10
#define TRACELEN_MIN_250OR100MHZADC	4

#define TRACEDELAY_MAX 1023

#define CHANTRIGSTRETCH_MAX 4095
#define CHANTRIGSTRETCH_MIN 1



  
/*------------------------------------- 
  MODCSRB bits definitions
  --------------------------------------*/ 

#define MODCSRB_CPLDPULLUP      0   // Control pullups for PXI trigger lines on the backplane through CPLD
#define MODCSRB_DIRMOD		4	// Set this module as the Director module (1) or non-Director module (0)
#define MODCSRB_CHASSISMASTER 	6	// Control chassis master module: 1: chassis master module; 0: chassis non-master module
#define MODCSRB_GFTSEL		7	// Select global fast trigger source
#define MODCSRB_ETSEL		8	// Select external trigger source
#define MODCSRB_INHIBITENA      10  // Control external INHIBIT signal: use INHIBIT (1) or don't use INHIBIT (0)
#define MODCSRB_MULTCRATES      11  // Distribute clock and triggers in multiple crates: multiple crates (1) or only single crate (0)
#define MODCSRB_SORTEVENTS      12  // Sort (1) or don't sort events based on their timestamps
#define MODCSRB_BKPLFASTTRIG    13  // Enable connection of fast triggers to backplane

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#define ROUND(x)    ((x) < 0.0 ? ceil((x) - 0.5) : floor((x) + 0.5))


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class algorithm
{
public:
  algorithm();
  virtual ~algorithm();

public:
  char offlinefilename[PRESET_MAX_MODULES][256];
  unsigned int OfflineModuleEventsCount[PRESET_MAX_MODULES];//这个表示每个插件的事件数
  unsigned int *OfflineEventInformation[PRESET_MAX_MODULES];// 每个事件信息长度 12 * unsigned int
  // 0  Event
  // 1  Channel 0-15
  // 2  Slot 2-14 表示机箱的第几个插槽 实际插件标号应该在该数值上减2    
  // 3  Crate 表示第几个机箱，我们只有一个机箱所以为0
  // 4  Header length
  // 5  Event length
  // 6  Finish code
  // 7  EventTime_Low  时间
  // 8  EventTime_High 时间
  // 9  Event Energy   能量
  // 10 Trace Length  波形长度
  // 11 Trace location 表示波形在文件中的位置

public:

  // 以下几个函数用户可能用到
  
  int ComputeFastFiltersOffline(
				unsigned short ModuleNumber,       // the module whose events are to be analyzed
				unsigned short ChannelNumber,      // the channel whose events are to be analyzed
				unsigned short RcdTraceLength,     // recorded trace length
				unsigned short *RcdTrace,          // recorded trace
				double         *fastfilter,        // fast filter response
				double         *cfd );             // cfd response
  int ComputeSlowFiltersOffline(
				unsigned short ModuleNumber,       // the module whose events are to be analyzed
				unsigned short ChannelNumber,      // the channel whose events are to be analyzed
				unsigned short RcdTraceLength,     // recorded trace length
				unsigned short *RcdTrace,          // recorded trace
				double         *slowfilter );      // slow filter response

  double ComputeEnergyOffline(
			   unsigned short ModuleNumber,       // the module whose events are to be analyzed
			   unsigned short ChannelNumber,      // the channel whose events are to be analyzed
			   unsigned short RcdTraceLength,     // recorded trace length
			   unsigned short *RcdTrace);          // recorded trace



  
  // double ENERGY_RISETIME;
  // double ENERGY_FLATTOP;
  // unsigned int SLOW_FILTER_RANGE;
  // xia->ReadSglChanPar((char *)"ENERGY_RISETIME",&ENERGY_RISETIME,0,2);
  // xia->ReadSglChanPar((char *)"ENERGY_FLATTOP",&ENERGY_FLATTOP,0,2);
  // xia->ReadSglModPar((char *)"SLOW_FILTER_RANGE",&SLOW_FILTER_RANGE,0);
  // std::cout<<SLOW_FILTER_RANGE<<"  "<<ENERGY_RISETIME<<"  "<<ENERGY_FLATTOP<<std::endl;

  // SLOW_FILTER_RANGE = 1;
  // xia->WriteSglModPar((char *)"SLOW_FILTER_RANGE",SLOW_FILTER_RANGE,0);
  // xia->ReadSglChanPar((char *)"ENERGY_RISETIME",&ENERGY_RISETIME,0,2);
  // xia->ReadSglChanPar((char *)"ENERGY_FLATTOP",&ENERGY_FLATTOP,0,2);
  // xia->ReadSglModPar((char *)"SLOW_FILTER_RANGE",&SLOW_FILTER_RANGE,0);
  // std::cout<<SLOW_FILTER_RANGE<<"  "<<ENERGY_RISETIME<<"  "<<ENERGY_FLATTOP<<std::endl; 

  // Mod Par
  // SLOW_FILTER_RANGE  1-6

  // ChanPar
  // ENERGY_RISETIME 能量梯形上升时间
  // ENERGY_FLATTOP  能量梯形平台时间
  // TAU             衰减时间
  // TRIGGER_THRESHOLD 阈值
  // CFDDelay
  // CFDScale
  
  int ReadSglModPar(
  		    char *ModParName,           // the name of the module parameter
  		    unsigned int   *ModParData, // the module parameter value to be read from the module
  		    unsigned short ModNum );     // module number

  int ReadSglChanPar(
  		     char *ChanParName,         // the name of the channel parameter
  		     double *ChanParData,       // the channel parameter value to be read from the module
  		     unsigned short ModNum,     // module number
  		     unsigned short ChanNum );   // channel number


  int WriteSglModPar(
		     char *ModParName,          // the name of the module parameter
		     unsigned int   ModParData, // the module parameter value to be written to the module
		     unsigned short ModNum );    // module number
  int WriteSglChanPar(
		      char *ChanParName,         // the name of the channel parameter
		      double ChanParData,        // the channel parameter value to be written to the module
		      unsigned short ModNum,     // module number
		      unsigned short ChanNum );   // channel number
  

  int SaveDSPParametersToFile(char  *FileName ); // the DSP parameters file name (with complete path)

  
  // Convert a IEEE 754 standrad floating point number (1-bit sign, 8-bit exponent, and 23-bit mantissa) to a decimal fractional number.
  double IEEEFloating2Decimal(unsigned int IEEEFloatingNumber);
  // Convert a decimal fractional number to a IEEE 754 standrad floating point number (1-bit sign, 8-bit exponent, and 23-bit mantissa).
  unsigned int Decimal2IEEEFloating(double DecimalNumber);
  
  // Test a bit in a 16-bit unsigned integer.
  unsigned short  TstBit_16(unsigned short bit,unsigned short value);
  // Set a bit in a 16-bit unsigned integer.
  unsigned short SetBit_16(unsigned short bit,unsigned short value);
  // Clear a bit in a 16-bit unsigned integer.
  unsigned short ClrBit_16(unsigned short bit,unsigned short value);
  // Set a bit in a 32-bit unsigned integer.
  unsigned int SetBit_32(unsigned short bit,unsigned int value);
  // Clear a bit in a 32-bit unsigned integer.
  unsigned int ClrBit_32(unsigned short bit,unsigned int value);
  // Test a bit in a 32-bit unsigned integer.
  unsigned int TstBit_32(unsigned short bit,unsigned int value );


  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 以下部分用户不需要
  
public:
  int InitSystem(
		 unsigned short NumModules,    // total number of Pixie16 modules in the system
		 unsigned short OfflineMode );  // specify if the system is in offline mode

  
  int LoadDSPParametersFromFile(char  *FileName );
  int Init_DSPVarAddress(char *DSPVarFile, unsigned short ModNum);
  int Copy_DSPVarAddress(unsigned short SourceModNum, unsigned short DestinationModNum);

  
  unsigned int GetModuleEvents(char *FileName);//return events
  void GetEventsInfo(char *FileName, unsigned int *EventInformation);

  

private:
  int ReadModuleInfo(
		     unsigned short ModNum,			// module number
		     unsigned short *ModRev,			// returned module revision
		     unsigned int   *ModSerNum,		// returned module serial number
		     unsigned short *ModADCBits,		// returned module ADC bits
		     unsigned short *ModADCMSPS );	// returned module ADC sampling rate


  int ComputeFIFO(
		  unsigned int   TraceDelay,	// current trace dealy value
		  unsigned short ModNum,		// Pixie module number
		  unsigned short ChanNum );	// Pixie channel number
  
  

  


  
  
private:
  struct Pixie_Configuration
  {	
    // DSP I/O parameter values
    unsigned int DSP_Parameter_Values[N_DSP_PAR];
  };

  // Define PRESET_MAX_MODULES Pixie devices
  struct Pixie_Configuration Pixie_Devices[PRESET_MAX_MODULES];

  struct Module_Info
  {
    // Module information
    unsigned short Module_Rev;
    unsigned int   Module_SerNum;
    unsigned short Module_ADCBits;
    unsigned short Module_ADCMSPS;
    unsigned short Module_OfflineVariant;
  };

  // Define PRESET_MAX_MODULES Pixie devices
  struct Module_Info Module_Information[PRESET_MAX_MODULES];
  

  unsigned int Number_Modules;//实际插件数量

  
private:

  // -----------------------------------------------------------------
  //   DSP I/O parameters address
  //   We initialize these addressee when we boot the modules, then
  //   we can use them when we need to change DSP parameters.
  //   -----------------------------------------------------------------

  //--------------------
  //	Module parameters
  //--------------------

  unsigned int ModNum_Address[PRESET_MAX_MODULES];         // ModNum
  unsigned int ModCSRA_Address[PRESET_MAX_MODULES];        // ModCSRA
  unsigned int ModCSRB_Address[PRESET_MAX_MODULES];        // ModCSRB
  unsigned int ModFormat_Address[PRESET_MAX_MODULES];      // ModFormat
  unsigned int RunTask_Address[PRESET_MAX_MODULES];        // RunTask
  unsigned int ControlTask_Address[PRESET_MAX_MODULES];    // ControlTask
  unsigned int MaxEvents_Address[PRESET_MAX_MODULES];      // MaxEvents
  unsigned int CoincPattern_Address[PRESET_MAX_MODULES];   // CoincPattern
  unsigned int CoincWait_Address[PRESET_MAX_MODULES];      // CoincWait
  unsigned int SynchWait_Address[PRESET_MAX_MODULES];      // SynchWait
  unsigned int InSynch_Address[PRESET_MAX_MODULES];        // InSynch
  unsigned int Resume_Address[PRESET_MAX_MODULES];         // Resume
  unsigned int SlowFilterRange_Address[PRESET_MAX_MODULES];// SlowFilterRange
  unsigned int FastFilterRange_Address[PRESET_MAX_MODULES];// FastFilterRange
  unsigned int ChanNum_Address[PRESET_MAX_MODULES];        // ChanNum
  unsigned int HostIO_Address[PRESET_MAX_MODULES];         // HostIO
  unsigned int UserIn_Address[PRESET_MAX_MODULES];         // UserIn
  unsigned int U00_Address[PRESET_MAX_MODULES];            // U00
  unsigned int FastTrigBackplaneEna_Address[PRESET_MAX_MODULES]; // Enabling sending fast trigger to backplane
  unsigned int CrateID_Address             [PRESET_MAX_MODULES]; // CrateID
  unsigned int SlotID_Address              [PRESET_MAX_MODULES]; // SlotID
  unsigned int ModID_Address               [PRESET_MAX_MODULES]; // ModID
  unsigned int TrigConfig_Address          [PRESET_MAX_MODULES]; // General configuration registers
  unsigned int HRTP_Address[PRESET_MAX_MODULES];           // HostRunTimePreset

  //--------------------
  //	Channel parameters
  //--------------------

  unsigned int ChanCSRa_Address[PRESET_MAX_MODULES];       // ChanCSRa
  unsigned int ChanCSRb_Address[PRESET_MAX_MODULES];       // ChanCSRb

  unsigned int GainDAC_Address[PRESET_MAX_MODULES];        // GainDAC

  unsigned int OffsetDAC_Address[PRESET_MAX_MODULES];      // OffsetDAC

  unsigned int DigGain_Address[PRESET_MAX_MODULES];        // DigGain

  unsigned int SlowLength_Address[PRESET_MAX_MODULES];     // SlowLength
  unsigned int SlowGap_Address[PRESET_MAX_MODULES];        // SlowGap
  unsigned int FastLength_Address[PRESET_MAX_MODULES];     // FastLength
  unsigned int FastGap_Address[PRESET_MAX_MODULES];        // FastGap
  unsigned int PeakSample_Address[PRESET_MAX_MODULES];     // PeakSample
  unsigned int PeakSep_Address[PRESET_MAX_MODULES];        // PeakSep

  unsigned int CFDThresh_Address[PRESET_MAX_MODULES];      // CFDThresh

  unsigned int FastThresh_Address[PRESET_MAX_MODULES];     // FastThresh
  unsigned int ThreshWidth_Address[PRESET_MAX_MODULES];    // ThreshWidth
  unsigned int PAFlength_Address[PRESET_MAX_MODULES];      // PAFlength
  unsigned int TriggerDelay_Address[PRESET_MAX_MODULES];   // TriggerDelay
  unsigned int ResetDelay_Address[PRESET_MAX_MODULES];     // ResetDelay
  unsigned int ChanTrigStretch_Address[PRESET_MAX_MODULES];// ChanTrigStretch
  unsigned int TraceLength_Address[PRESET_MAX_MODULES];    // TraceLength
  unsigned int TrigOutLen_Address[PRESET_MAX_MODULES];     // TrigOutLen
  unsigned int EnergyLow_Address[PRESET_MAX_MODULES];      // EnergyLow
  unsigned int Log2Ebin_Address[PRESET_MAX_MODULES];       // Log2Ebin

  unsigned int MultiplicityMaskL_Address[PRESET_MAX_MODULES]; // Multiplicity contribution mask - low 32-bit

  unsigned int PSAoffset_Address[PRESET_MAX_MODULES];      // PSAoffset
  unsigned int PSAlength_Address[PRESET_MAX_MODULES];      // PSAlength
  unsigned int Integrator_Address[PRESET_MAX_MODULES];     // Integrator

  unsigned int BLcut_Address[PRESET_MAX_MODULES];          // BLcut
  unsigned int BaselinePercent_Address[PRESET_MAX_MODULES];// BaselinePercent

  unsigned int FtrigoutDelay_Address[PRESET_MAX_MODULES];  // Fast trigger output delay for system synchronization

  unsigned int Log2Bweight_Address[PRESET_MAX_MODULES];    // Log2Bweight
  unsigned int PreampTau_Address[PRESET_MAX_MODULES];      // PreampTau

  unsigned int MultiplicityMaskH_Address[PRESET_MAX_MODULES]; // Multiplicity contribution mask - high 32-bit

  unsigned int FastTrigBackLen_Address[PRESET_MAX_MODULES];	// FastTrigBackLen

  unsigned int CFDDelay_Address      [PRESET_MAX_MODULES];	// CFD delay
  unsigned int CFDScale_Address      [PRESET_MAX_MODULES];	// CFD scale
  unsigned int ExternDelayLen_Address[PRESET_MAX_MODULES];	// delay length for each channel's input signal
  unsigned int ExtTrigStretch_Address[PRESET_MAX_MODULES];	// external trigger stretch
  unsigned int VetoStretch_Address   [PRESET_MAX_MODULES];	// veto stretch
  unsigned int QDCLen0_Address       [PRESET_MAX_MODULES];	// QDC #0 length
  unsigned int QDCLen1_Address       [PRESET_MAX_MODULES];	// QDC #1 length
  unsigned int QDCLen2_Address       [PRESET_MAX_MODULES];	// QDC #2 length
  unsigned int QDCLen3_Address       [PRESET_MAX_MODULES];	// QDC #3 length
  unsigned int QDCLen4_Address       [PRESET_MAX_MODULES];	// QDC #4 length
  unsigned int QDCLen5_Address       [PRESET_MAX_MODULES];	// QDC #5 length
  unsigned int QDCLen6_Address       [PRESET_MAX_MODULES];	// QDC #6 length
  unsigned int QDCLen7_Address       [PRESET_MAX_MODULES];	// QDC #7 length

  unsigned int Xwait_Address[PRESET_MAX_MODULES];				// Xwait


  //--------------------
  //	Results parameters
  //--------------------

  unsigned int RealTimeA_Address[PRESET_MAX_MODULES];    // RealTimeA
  unsigned int RealTimeB_Address[PRESET_MAX_MODULES];    // RealTimeB
  unsigned int RunTimeA_Address[PRESET_MAX_MODULES];     // RunTimeA
  unsigned int RunTimeB_Address[PRESET_MAX_MODULES];     // RunTimeB
  unsigned int GSLTtime_Address[PRESET_MAX_MODULES];     // GSLTtime
  unsigned int NumEventsA_Address[PRESET_MAX_MODULES];   // NumEventsA
  unsigned int NumEventsB_Address[PRESET_MAX_MODULES];   // NumEventsB
  unsigned int DSPerror_Address[PRESET_MAX_MODULES];     // DSPerror
  unsigned int SynchDone_Address[PRESET_MAX_MODULES];    // SynchDone
  unsigned int BufHeadLen_Address[PRESET_MAX_MODULES];   // BufHeadLen
  unsigned int EventHeadLen_Address[PRESET_MAX_MODULES]; // EventHeadLen
  unsigned int ChanHeadLen_Address[PRESET_MAX_MODULES];  // ChanHeadLen
  unsigned int UserOut_Address[PRESET_MAX_MODULES];      // UserOut
  unsigned int AOutBuffer_Address[PRESET_MAX_MODULES];   // AOutBuffer
  unsigned int LOutBuffer_Address[PRESET_MAX_MODULES];   // LOutBuffer
  unsigned int AECorr_Address[PRESET_MAX_MODULES];       // AECorr
  unsigned int LECorr_Address[PRESET_MAX_MODULES];       // LECorr
  unsigned int HardwareID_Address[PRESET_MAX_MODULES];   // HardwareID
  unsigned int HardVariant_Address[PRESET_MAX_MODULES];  // HardVariant
  unsigned int FIFOLength_Address[PRESET_MAX_MODULES];   // FIFOLength
  unsigned int FippiID_Address[PRESET_MAX_MODULES];      // FippiID
  unsigned int FippiVariant_Address[PRESET_MAX_MODULES]; // FippiVariant
  unsigned int DSPrelease_Address[PRESET_MAX_MODULES];   // DSPrelease
  unsigned int DSPbuild_Address[PRESET_MAX_MODULES];     // DSPbuild
  unsigned int DSPVariant_Address[PRESET_MAX_MODULES];   // DSPVariant
  unsigned int U20_Address[PRESET_MAX_MODULES];          // U20
  unsigned int LiveTimeA_Address[PRESET_MAX_MODULES];    // LiveTimeA
  unsigned int LiveTimeB_Address[PRESET_MAX_MODULES];    // LiveTimeB
  unsigned int FastPeaksA_Address[PRESET_MAX_MODULES];   // FastPeaksA
  unsigned int FastPeaksB_Address[PRESET_MAX_MODULES];   // FastPeaksB
  unsigned int OverflowA_Address[PRESET_MAX_MODULES];    // OverflowA
  unsigned int OverflowB_Address[PRESET_MAX_MODULES];    // OverflowB
  unsigned int InSpecA_Address[PRESET_MAX_MODULES];      // InSpecA
  unsigned int InSpecB_Address[PRESET_MAX_MODULES];      // InSpecB
  unsigned int UnderflowA_Address[PRESET_MAX_MODULES];   // UnderflowA
  unsigned int UnderflowB_Address[PRESET_MAX_MODULES];   // UnderflowB
  unsigned int ChanEventsA_Address[PRESET_MAX_MODULES];  // ChanEventsA
  unsigned int ChanEventsB_Address[PRESET_MAX_MODULES];  // ChanEventsB
  unsigned int AutoTau_Address[PRESET_MAX_MODULES];      // AutoTau
  unsigned int U30_Address[PRESET_MAX_MODULES];          // U30


  
};


#endif /* _ALGORITHM_H_ */
// 
// algorithm.hh ends here
