// algorithm.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 7月 22 21:08:18 2016 (+0800)
// Last-Updated: 三 10月 26 20:15:58 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 58
// URL: http://wuhongyi.github.io 

#include "algorithm.hh"

#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <string>
#include <cmath>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

algorithm::algorithm()
{

}

algorithm::~algorithm()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// IEEEFloating2Decimal:
// Convert a IEEE 754 standrad floating point number (1-bit sign, 8-bit exponent, and 23-bit mantissa) to a decimal fractional number.
// Return Value:
// the converted decimal fractional number
double algorithm::IEEEFloating2Decimal(unsigned int IEEEFloatingNumber)
{
  short signbit, exponent;
  double mantissa, DecimalNumber;
	
  signbit = (short)(IEEEFloatingNumber >> 31);
  exponent = (short)((IEEEFloatingNumber & 0x7F800000) >> 23) - 127;
  mantissa = 1.0 + (double)(IEEEFloatingNumber & 0x7FFFFF) / pow(2.0, 23.0);
  if(signbit == 0)
    {
      DecimalNumber = mantissa * pow(2.0, (double)exponent);
    }
  else
    {
      DecimalNumber = - mantissa * pow(2.0, (double)exponent);
    }
	
  return(DecimalNumber);
	
}


// Decimal2IEEEFloating:
// Convert a decimal fractional number to a IEEE 754 standrad floating point number (1-bit sign, 8-bit exponent, and 23-bit mantissa).
// Return Value:
// the converted IEEE floating point number
unsigned int algorithm::Decimal2IEEEFloating(double DecimalNumber)
{
  unsigned int IEEEFloatingNum, IntPart, IntPartHex, FractPartHex;
  double AbsDecNum, FractPart;
  unsigned short k, m, exponent;
  short n;
  char signbit;
	
  // Check signbit
  if(DecimalNumber > 0)
    {
      signbit = 0;
    }
  else if(DecimalNumber < 0)
    {
      signbit = 1;
    }
  else	// DecimalNumber = 0
    {
      return(0);
    }
	
  // Get the absolute value
  AbsDecNum = fabs(DecimalNumber);
	
  // Get the integer part
  IntPart = (unsigned int)floor(AbsDecNum);
	
  // Convert the integer part
  IntPartHex = 0;
  k = 0;
  do
    {
      if((IntPart % 2) == 1)
	{
	  IntPartHex = SetBit_32(k, IntPartHex);
	}
      else
	{
	  IntPartHex = ClrBit_32(k, IntPartHex);
	}
		
      k ++;
		
      IntPart /= 2;
		
    } while((IntPart > 0) && (k < 32));
	
  // Get the fractional part
  FractPart = AbsDecNum - floor(AbsDecNum);
	
  // Convert the fractional part
  FractPartHex = 0;
  m = 0;
  do
    {
      if((FractPart * 2.0) >= 1.0)
	{
	  FractPartHex = SetBit_32((unsigned short)(31 - m), FractPartHex);
	  FractPart = FractPart * 2.0 - 1.0;
	}
      else
	{
	  FractPartHex = ClrBit_32((unsigned short)(31 - m), FractPartHex);
	  FractPart = FractPart * 2.0;
	}
		
      m ++;
		
    } while((FractPart > 0) && (m < 32));
	
  // Combine the integer and fractional part
  // First, find the exponent
  if(IntPartHex > 0)
    {
      exponent = 127 + k - 1;	// 127 is the exponent bias
		
      // Add sign bit first
      IEEEFloatingNum = signbit * (unsigned int)pow(2.0, 31.0);
		
      // Add the exponent
      IEEEFloatingNum += exponent * (unsigned int)pow(2.0, 23.0);
		
      // Add the mantissa
      IntPartHex = ClrBit_32((unsigned short)(k - 1), IntPartHex);
      IEEEFloatingNum += IntPartHex * (unsigned int)pow(2.0, (double)(22 - (k - 2)));
      IEEEFloatingNum += FractPartHex >> (32 - (23 - (k - 1)));
    }
  else	// IntPartHex = 0
    {
      // Search the fist non-zero bit in FractPartHex
      for(n = 31; n >= 0; n --)
	{
	  if(TstBit_32(n, FractPartHex) == 1)
	    {
	      break;
	    }
	}
		
      exponent = 127 - (32 - n);	// 127 is the exponent bias
		
      // Add sign bit first
      IEEEFloatingNum = signbit * (unsigned int)pow(2.0, 31.0);
		
      // Add the exponent
      IEEEFloatingNum += exponent * (unsigned int)pow(2.0, 23.0);
		
      // Add the mantissa
      FractPartHex = ClrBit_32(n, FractPartHex);
      if(n >= 23)
	{
	  IEEEFloatingNum += FractPartHex >> (n - 23);
	}
      else
	{
	  IEEEFloatingNum += FractPartHex << (23 - n);
	}
    }
	
  return(IEEEFloatingNum);
												
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// LoadDSPParametersFromFile:
// Load DSP parameters from a settings file.

// Return Value:
// 0 - Success
// -1 - Size of DSPParFile is invalid
// -2 - Failed to open the DSP parameters file

int algorithm::LoadDSPParametersFromFile(char  *FileName )// the DSP parameters file name (with complete path)
{
  FILE *DSPSettingsFile = NULL;

  unsigned int TotalWords;
  unsigned short k;
  
  // Open DSP parameters file
  if( (DSPSettingsFile = fopen(FileName, "rb")) != NULL )  // Make sure DSPSettingsFile is opened successfully
    {
      // Check if file size is consistent with predefined length (N_DSP_PAR * PRESET_MAX_MODULES)
      fseek(DSPSettingsFile, 0, SEEK_END);
      TotalWords = (ftell(DSPSettingsFile) + 1) / 4;
      if(TotalWords != (N_DSP_PAR * PRESET_MAX_MODULES))
	{
	  printf("*ERROR* : size of DSPParFile is invalid. Check DSPParFile name %s\n", FileName);
	  // close opened files
	  fclose(DSPSettingsFile);
	  return(-1);
	}

      // Point configfil to the beginning of file
      fseek(DSPSettingsFile, 0, SEEK_SET);
		
      // Read DSP parameters
      for(k=0; k<PRESET_MAX_MODULES; k++)
	{
	  fread(&Pixie_Devices[k].DSP_Parameter_Values[0], sizeof(unsigned int), N_DSP_PAR, DSPSettingsFile);
			
	  // Force correct module number
	  Pixie_Devices[k].DSP_Parameter_Values[0] = k;
	}

    }
  else
    {
      printf("*ERROR* : failed to open DSP parameters file %s\n", FileName);
      return(-2);
    }

  return 0;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// ComputeFastFiltersOffline:
// Compute fast filter responses of each event in the list mode data file for offline analysis
// Return Value:
//  0 - Success
// -1 - Null pointer *RcdTrace
// -2 - Null pointer *fastfilter
// -3 - Null pointer *cfd
// -4 - Target module number is invalid
// -5 - Trace length is too short
int algorithm::ComputeFastFiltersOffline(
					 unsigned short ModuleNumber,       // the module whose events are to be analyzed
					 unsigned short ChannelNumber,      // the channel whose events are to be analyzed
					 unsigned short RcdTraceLength,     // recorded trace length
					 unsigned short *RcdTrace,          // recorded trace
					 double         *fastfilter,        // fast filter response
					 double         *cfd )              // cfd response
{

  unsigned int FastLen, FastGap, FastFilterRange, CFD_Delay, CFD_W;
  unsigned int fsum0[32768], fsum1[32768];
  unsigned int offset, x, y;
  double cfdscale;

  // Check if RcdTrace is valid
  if(RcdTrace == NULL)
    {
      printf("*Error* : Null pointer *RcdTrace\n");
      return(-1);
    }
	
  // Check if fastfilter is valid
  if(fastfilter == NULL)
    {
      printf("*Error* : Null pointer *fastfilter\n");
      return(-2);
    }
	
  // Check if cfd is valid
  if(cfd == NULL)
    {
      printf("*Error* : Null pointer *cfd\n");
      return(-3);
    }

  if(ModuleNumber >= PRESET_MAX_MODULES)
    {
      printf("*ERROR* : Target module number is invalid %d\n", ModuleNumber);
      return(-4);
    }

  // Retrieve channel parameters
  FastFilterRange = Pixie_Devices[ModuleNumber].DSP_Parameter_Values[FastFilterRange_Address[ModuleNumber] - DATA_MEMORY_ADDRESS];
  FastLen = Pixie_Devices[ModuleNumber].DSP_Parameter_Values[FastLength_Address[ModuleNumber] + ChannelNumber - DATA_MEMORY_ADDRESS] * (unsigned int)pow(2.0, (double)FastFilterRange);
  FastGap = Pixie_Devices[ModuleNumber].DSP_Parameter_Values[FastGap_Address[ModuleNumber] + ChannelNumber - DATA_MEMORY_ADDRESS] * (unsigned int)pow(2.0, (double)FastFilterRange);
  CFD_Delay = Pixie_Devices[ModuleNumber].DSP_Parameter_Values[CFDDelay_Address[ModuleNumber] + ChannelNumber - DATA_MEMORY_ADDRESS];
  CFD_W = Pixie_Devices[ModuleNumber].DSP_Parameter_Values[CFDScale_Address[ModuleNumber] + ChannelNumber - DATA_MEMORY_ADDRESS];

  // Check if trace length is sufficiently long
  if(RcdTraceLength < ((2*FastLen + FastGap)*2))
    {
      printf("*Error* : the length of recorded trace is too short\n");
      return(-5);
    }

  // Compute fast filter response
  offset = 2*FastLen + FastGap - 1;
  for(x=offset; x<RcdTraceLength; x++)
    {
      fsum0[x] = 0;
      for(y=(x-offset); y<(x-offset+FastLen); y++)
	{
	  fsum0[x] += RcdTrace[y];
	}
      fsum1[x] = 0;
      for(y=(x-offset+FastLen+FastGap); y<(x-offset+2*FastLen+FastGap); y++)
	{
	  fsum1[x] += RcdTrace[y];
	}
      fastfilter[x] = ((double)fsum1[x] - (double)fsum0[x])/(double)FastLen;
    }

  // Extend the value of fastfilter[offset] to all non-computed ones from index 0 to offset-1
  for(x=0; x<offset; x++)
    {
      fastfilter[x] = fastfilter[offset];
    }

  // Decide CFD Scale value
  cfdscale = 1.0 - (double)CFD_W * 0.125;

  // Compute CFD
  for(x=CFD_Delay; x<RcdTraceLength; x++)
    {
      cfd[x] = -fastfilter[x-CFD_Delay] + fastfilter[x] * cfdscale;

    }

  // Extend the value of cfd[CFD_Delay] to all non-computed ones from index 0 to CFD_Delay-1
  for(x=0; x<CFD_Delay; x++)
    {
      cfd[x] = cfd[CFD_Delay];
    }

	
  return(0);
	
}

// ComputeSlowFiltersOffline:
// Compute slow filter responses of each event in the list mode data file for offline analysis
// Return Value:
// 0 - Success
// -1 - Null pointer *RcdTrace
// -2 - Null pointer *slowfilter
// -3 - Target module number is invalid
// -4 - Trace length is too short
int algorithm::ComputeSlowFiltersOffline(
					 unsigned short ModuleNumber,       // the module whose events are to be analyzed
					 unsigned short ChannelNumber,      // the channel whose events are to be analyzed
					 unsigned short RcdTraceLength,     // recorded trace length
					 unsigned short *RcdTrace,          // recorded trace
					 double         *slowfilter )       // slow filter response
{

  unsigned int SlowLen, SlowGap, SlowFilterRange, PreampTau_IEEE;
  unsigned int esum0[32768], esum1[32768], esum2[32768];
  unsigned int offset, x, y;
  double preamptau, deltaT;
  double b1, c0, c1, c2;
  unsigned int bsum0, bsum1, bsum2;
  double baseline;

  // Check if RcdTrace is valid
  if(RcdTrace == NULL)
    {
      printf("*Error* : Null pointer *RcdTrace\n");
      return(-1);
    }
	
  // Check if slowfilter is valid
  if(slowfilter == NULL)
    {
      printf("*Error* : Null pointer *slowfilter\n");
      return(-2);
    }
	
  if(ModuleNumber >= PRESET_MAX_MODULES)
    {
      printf("*ERROR* : Target module number is invalid %d\n", ModuleNumber);
      return(-3);
    }

  // Retrieve channel parameters

  SlowFilterRange = Pixie_Devices[ModuleNumber].DSP_Parameter_Values[SlowFilterRange_Address[ModuleNumber] - DATA_MEMORY_ADDRESS];
  SlowLen = Pixie_Devices[ModuleNumber].DSP_Parameter_Values[SlowLength_Address[ModuleNumber] + ChannelNumber - DATA_MEMORY_ADDRESS] * (unsigned int)pow(2.0, (double)SlowFilterRange);
  SlowGap = Pixie_Devices[ModuleNumber].DSP_Parameter_Values[SlowGap_Address[ModuleNumber] + ChannelNumber - DATA_MEMORY_ADDRESS] * (unsigned int)pow(2.0, (double)SlowFilterRange);
  PreampTau_IEEE = Pixie_Devices[ModuleNumber].DSP_Parameter_Values[PreampTau_Address[ModuleNumber] + ChannelNumber - DATA_MEMORY_ADDRESS];
  preamptau = IEEEFloating2Decimal(PreampTau_IEEE);

  // Check if trace length is sufficiently long
  if(RcdTraceLength < ((2*SlowLen + SlowGap)*2))
    {
      printf("*Error* : the length of recorded trace is too short\n");
      return(-4);
    }
		
  // Compute slow filter coefficients
  deltaT = 1.0/((double)Module_Information[ModuleNumber].Module_ADCMSPS);
  b1 = exp(-1.0 * deltaT / preamptau);
  c0 = -(1.0 - b1) * pow(b1, (double)SlowLen) * 4.0 / (1.0 - pow(b1, (double)SlowLen));
  c1 = (1.0 - b1) * 4.0;
  c2 = (1.0 - b1) * 4.0 / (1.0 - pow(b1, (double)SlowLen));

  // Compute baseline
  bsum0 = 0;
  for(y=0; y<SlowLen; y++)
    {
      bsum0 += RcdTrace[y];
    }
  bsum1 = 0;
  for(y=SlowLen; y<SlowLen+SlowGap; y++)
    {
      bsum1 += RcdTrace[y];
    }
  bsum2 = 0;
  for(y=(SlowLen+SlowGap); y<(2*SlowLen+SlowGap); y++)
    {
      bsum2 += RcdTrace[y];
    }
  baseline = c0 * (double)bsum0 + c1 * (double)bsum1 + c2 * (double)bsum2;

  // Compute slow filter response
  offset = 2*SlowLen + SlowGap - 1;
  for(x=offset; x<RcdTraceLength; x++)
    {
      esum0[x] = 0;
      for(y=(x-offset); y<(x-offset+SlowLen); y++)
	{
	  esum0[x] += RcdTrace[y];
	}
      esum1[x] = 0;
      for(y=(x-offset+SlowLen); y<(x-offset+SlowLen+SlowGap); y++)
	{
	  esum1[x] += RcdTrace[y];
	}
      esum2[x] = 0;
      for(y=(x-offset+SlowLen+SlowGap); y<(x-offset+2*SlowLen+SlowGap); y++)
	{
	  esum2[x] += RcdTrace[y];
	}
      slowfilter[x] = c0 * (double)esum0[x] + c1 * (double)esum1[x] + c2 * (double)esum2[x] - baseline;
    }

  // Extend the value of slowfilter[offset] to all non-computed ones from index 0 to offset-1
  for(x=0; x<offset; x++)
    {
      slowfilter[x] = slowfilter[offset];
    }
	
  return(0);
	
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Receive DSP I/O parameter name addressee from the DSP .var file.
// Return Value:
// 0 - load successful
// -1 - premature EOF encountered
// -2 - DSP variable name not found
// -3 - DSP .var file not found
int algorithm::Init_DSPVarAddress(char *DSPVarFile, unsigned short ModNum)
{
  char DSP_Parameter_Names[N_DSP_PAR][MAX_PAR_NAME_LENGTH];
  char *DSP_Parameter_AddrStr;
  unsigned int DSP_Parameter_Addr[N_DSP_PAR];
  char DSPParaName[MAX_PAR_NAME_LENGTH], str[MAX_PAR_NAME_LENGTH];
  unsigned short k;
  FILE* namesFile = NULL;
	
  // Open DSP .var file
  namesFile = fopen(DSPVarFile, "r");
  if(namesFile != NULL)
    {
      // Clear all names
      for(k=0; k<N_DSP_PAR; k++)
	{
	  strcpy(DSP_Parameter_Names[k],"");
	}
		
      // DSP_Parameter_AddrStr is 11 characters long: 0x + 8 hex characters + NULL terminator
      DSP_Parameter_AddrStr = (char *)malloc(sizeof(char) * 10 + 1);
		
      // Read names
      for(k=0; k<N_DSP_PAR; k++)
	{
	  if( fgets(DSPParaName, MAX_PAR_NAME_LENGTH, namesFile) != NULL )  // Read one line
	    {
	      sscanf(DSPParaName, "%s %s", DSP_Parameter_AddrStr, DSP_Parameter_Names[k]);
	      DSP_Parameter_Addr[k] = strtol(DSP_Parameter_AddrStr, NULL, 0);
	    }
	  else  // Reached end of file
	    {
	      break;
	    }
	}
		
      // Release memory
      free(DSP_Parameter_AddrStr);
		
      // Initialize addressee
		
      //--------------------
      //	Module parameters
      //--------------------
		
      ModNum_Address[ModNum] = 0;
      ModCSRA_Address[ModNum] = 0;
      ModCSRB_Address[ModNum] = 0;
      ModFormat_Address[ModNum] = 0;
      RunTask_Address[ModNum] = 0;
      ControlTask_Address[ModNum] = 0;
      MaxEvents_Address[ModNum] = 0;
      CoincPattern_Address[ModNum] = 0;
      CoincWait_Address[ModNum] = 0;
      SynchWait_Address[ModNum] = 0;
      InSynch_Address[ModNum] = 0;
      Resume_Address[ModNum] = 0;
      SlowFilterRange_Address[ModNum] = 0;
      FastFilterRange_Address[ModNum] = 0;
      ChanNum_Address[ModNum] = 0;
      HostIO_Address[ModNum] = 0;
      UserIn_Address[ModNum] = 0;
      U00_Address[ModNum] = 0;
      FastTrigBackplaneEna_Address[ModNum] = 0;
      CrateID_Address             [ModNum] = 0;
      SlotID_Address              [ModNum] = 0;
      ModID_Address               [ModNum] = 0;
      TrigConfig_Address          [ModNum] = 0;
      HRTP_Address[ModNum] = 0;
		
      //--------------------
      //	Channel parameters
      //--------------------
		
      ChanCSRa_Address[ModNum] = 0;
      ChanCSRb_Address[ModNum] = 0;
      GainDAC_Address[ModNum] = 0;
      OffsetDAC_Address[ModNum] = 0;
      DigGain_Address[ModNum] = 0;
      SlowLength_Address[ModNum] = 0;
      SlowGap_Address[ModNum] = 0;
      FastLength_Address[ModNum] = 0;
      FastGap_Address[ModNum] = 0;
      PeakSample_Address[ModNum] = 0;
      PeakSep_Address[ModNum] = 0;
      CFDThresh_Address[ModNum] = 0;
      FastThresh_Address[ModNum] = 0;
      ThreshWidth_Address[ModNum] = 0;
      PAFlength_Address[ModNum] = 0;
      TriggerDelay_Address[ModNum] = 0;
      ResetDelay_Address[ModNum] = 0;
      ChanTrigStretch_Address[ModNum] = 0;
      TraceLength_Address[ModNum] = 0;
      TrigOutLen_Address[ModNum] = 0;
      EnergyLow_Address[ModNum] = 0;
      Log2Ebin_Address[ModNum] = 0;
      Log2Bweight_Address[ModNum] = 0;
      MultiplicityMaskL_Address[ModNum] = 0;
      PSAoffset_Address[ModNum] = 0;
      PSAlength_Address[ModNum] = 0;
      Integrator_Address[ModNum] = 0;
      BLcut_Address[ModNum] = 0;
      BaselinePercent_Address[ModNum] = 0;
      FtrigoutDelay_Address[ModNum] = 0;
      Log2Bweight_Address[ModNum] = 0;
      PreampTau_Address[ModNum] = 0;
      MultiplicityMaskH_Address[ModNum] = 0;
      FastTrigBackLen_Address[ModNum] = 0;
      Xwait_Address[ModNum] = 0;
      CFDDelay_Address            [ModNum] = 0;
      CFDScale_Address            [ModNum] = 0;
      ExternDelayLen_Address      [ModNum] = 0;
      ExtTrigStretch_Address      [ModNum] = 0;
      VetoStretch_Address         [ModNum] = 0;
      QDCLen0_Address             [ModNum] = 0;
      QDCLen1_Address             [ModNum] = 0;
      QDCLen2_Address             [ModNum] = 0;
      QDCLen3_Address             [ModNum] = 0;
      QDCLen4_Address             [ModNum] = 0;
      QDCLen5_Address             [ModNum] = 0;
      QDCLen6_Address             [ModNum] = 0;
      QDCLen7_Address             [ModNum] = 0;
      
      //--------------------
      //	Results parameters
      //--------------------
		
      RealTimeA_Address[ModNum] = 0;
      RealTimeB_Address[ModNum] = 0;
      RunTimeA_Address[ModNum] = 0;
      RunTimeB_Address[ModNum] = 0;
      GSLTtime_Address[ModNum] = 0;
      NumEventsA_Address[ModNum] = 0;
      NumEventsB_Address[ModNum] = 0;
      DSPerror_Address[ModNum] = 0;
      SynchDone_Address[ModNum] = 0;
      BufHeadLen_Address[ModNum] = 0;
      EventHeadLen_Address[ModNum] = 0;
      ChanHeadLen_Address[ModNum] = 0;
      UserOut_Address[ModNum] = 0;
      AOutBuffer_Address[ModNum] = 0;
      LOutBuffer_Address[ModNum] = 0;
      AECorr_Address[ModNum] = 0;
      LECorr_Address[ModNum] = 0;
      HardwareID_Address[ModNum] = 0;
      HardVariant_Address[ModNum] = 0;
      FIFOLength_Address[ModNum] = 0;
      FippiID_Address[ModNum] = 0;
      FippiVariant_Address[ModNum] = 0;
      DSPrelease_Address[ModNum] = 0;
      DSPbuild_Address[ModNum] = 0;
      DSPVariant_Address[ModNum] = 0;
      U20_Address[ModNum] = 0;
      LiveTimeA_Address[ModNum] = 0;
      LiveTimeB_Address[ModNum] = 0;
      FastPeaksA_Address[ModNum] = 0;
      FastPeaksB_Address[ModNum] = 0;
      OverflowA_Address[ModNum] = 0;
      OverflowB_Address[ModNum] = 0;
      InSpecA_Address[ModNum] = 0;
      InSpecB_Address[ModNum] = 0;
      UnderflowA_Address[ModNum] = 0;
      UnderflowB_Address[ModNum] = 0;
      ChanEventsA_Address[ModNum] = 0;
      ChanEventsB_Address[ModNum] = 0;
      AutoTau_Address[ModNum] = 0;
      U30_Address[ModNum] = 0;
		
      // Find Address
      for(k=0; k<N_DSP_PAR; k++)
	{
	  strcpy(str, DSP_Parameter_Names[k]);
	  
	  //--------------------
	  //	Module parameters
	  //--------------------
			
	  if(strcmp(str, "ModNum") == 0)
	    ModNum_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "ModCSRA") == 0)
	    ModCSRA_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "ModCSRB") == 0)
	    ModCSRB_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "ModFormat") == 0)
	    ModFormat_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "RunTask") == 0)
	    RunTask_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "ControlTask") == 0)
	    ControlTask_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "MaxEvents") == 0)
	    MaxEvents_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "CoincPattern") == 0)
	    CoincPattern_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "CoincWait") == 0)
	    CoincWait_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "SynchWait") == 0)
	    SynchWait_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "InSynch") == 0)
	    InSynch_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "Resume") == 0)
	    Resume_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "SlowFilterRange") == 0)
	    SlowFilterRange_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "FastFilterRange") == 0)
	    FastFilterRange_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "ChanNum") == 0)
	    ChanNum_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "HostIO") == 0)
	    HostIO_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "UserIn") == 0)
	    UserIn_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "U00") == 0)
	    U00_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "FastTrigBackplaneEna") == 0)
	    FastTrigBackplaneEna_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "CrateID") == 0)
	    CrateID_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "SlotID") == 0)
	    SlotID_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "ModID") == 0)
	    ModID_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "TrigConfig") == 0)
	    TrigConfig_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "HostRunTimePreset") == 0)
	    HRTP_Address[ModNum] = DSP_Parameter_Addr[k];

	  //--------------------
	  //	Channel parameters
	  //--------------------
			
	  else if(strcmp(str, "ChanCSRa") == 0)
	    ChanCSRa_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "ChanCSRb") == 0)
	    ChanCSRb_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "GainDAC") == 0)
	    GainDAC_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "OffsetDAC") == 0)
	    OffsetDAC_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "DigGain") == 0)
	    DigGain_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "SlowLength") == 0)
	    SlowLength_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "SlowGap") == 0)
	    SlowGap_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "FastLength") == 0)
	    FastLength_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "FastGap") == 0)
	    FastGap_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "PeakSample") == 0)
	    PeakSample_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "PeakSep") == 0)
	    PeakSep_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "CFDThresh") == 0)
	    CFDThresh_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "FastThresh") == 0)
	    FastThresh_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "ThreshWidth") == 0)
	    ThreshWidth_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "PAFlength") == 0)
	    PAFlength_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "TriggerDelay") == 0)
	    TriggerDelay_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "ResetDelay") == 0)
	    ResetDelay_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "ChanTrigStretch") == 0)
	    ChanTrigStretch_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "TraceLength") == 0)
	    TraceLength_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "TrigOutLen") == 0)
	    TrigOutLen_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "EnergyLow") == 0)
	    EnergyLow_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "Log2Ebin") == 0)
	    Log2Ebin_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "Log2Bweight") == 0)
	    Log2Bweight_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "MultiplicityMaskL") == 0)
	    MultiplicityMaskL_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "PSAoffset") == 0)
	    PSAoffset_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "PSAlength") == 0)
	    PSAlength_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "Integrator") == 0)
	    Integrator_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "BLcut") == 0)
	    BLcut_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "BaselinePercent") == 0)
	    BaselinePercent_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "FtrigoutDelay") == 0)
	    FtrigoutDelay_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "Log2Bweight") == 0)
	    Log2Bweight_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "PreampTau") == 0)
	    PreampTau_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "MultiplicityMaskH") == 0)
	    MultiplicityMaskH_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "FastTrigBackLen") == 0)
	    FastTrigBackLen_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "Xwait") == 0)
	    Xwait_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "CFDDelay") == 0)
	    CFDDelay_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "CFDScale") == 0)
	    CFDScale_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "ExternDelayLen") == 0)
	    ExternDelayLen_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "ExtTrigStretch") == 0)
	    ExtTrigStretch_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "VetoStretch") == 0)
	    VetoStretch_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "QDCLen0") == 0)
	    QDCLen0_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "QDCLen1") == 0)
	    QDCLen1_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "QDCLen2") == 0)
	    QDCLen2_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "QDCLen3") == 0)
	    QDCLen3_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "QDCLen4") == 0)
	    QDCLen4_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "QDCLen5") == 0)
	    QDCLen5_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "QDCLen6") == 0)
	    QDCLen6_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "QDCLen7") == 0)
	    QDCLen7_Address[ModNum] = DSP_Parameter_Addr[k];
			
	  //--------------------
	  //	Results parameters
	  //--------------------
			
	  else if(strcmp(str, "RealTimeA") == 0)
	    RealTimeA_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "RealTimeB") == 0)
	    RealTimeB_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "RunTimeA") == 0)
	    RunTimeA_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "RunTimeB") == 0)
	    RunTimeB_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "GSLTtime") == 0)
	    GSLTtime_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "NumEventsA") == 0)
	    NumEventsA_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "NumEventsB") == 0)
	    NumEventsB_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "DSPerror") == 0)
	    DSPerror_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "SynchDone") == 0)
	    SynchDone_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "BufHeadLen") == 0)
	    BufHeadLen_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "EventHeadLen") == 0)
	    EventHeadLen_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "ChanHeadLen") == 0)
	    ChanHeadLen_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "UserOut") == 0)
	    UserOut_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "AOutBuffer") == 0)
	    AOutBuffer_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "LOutBuffer") == 0)
	    LOutBuffer_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "AECorr") == 0)
	    AECorr_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "LECorr") == 0)
	    LECorr_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "HardwareID") == 0)
	    HardwareID_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "HardVariant") == 0)
	    HardVariant_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "FIFOLength") == 0)
	    FIFOLength_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "FippiID") == 0)
	    FippiID_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "FippiVariant") == 0)
	    FippiVariant_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "DSPrelease") == 0)
	    DSPrelease_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "DSPbuild") == 0)
	    DSPbuild_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "DSPVariant") == 0)
	    DSPVariant_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "U20") == 0)
	    U20_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "LiveTimeA") == 0)
	    LiveTimeA_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "LiveTimeB") == 0)
	    LiveTimeB_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "FastPeaksA") == 0)
	    FastPeaksA_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "FastPeaksB") == 0)
	    FastPeaksB_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "OverflowA") == 0)
	    OverflowA_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "OverflowB") == 0)
	    OverflowB_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "InSpecA") == 0)
	    InSpecA_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "InSpecB") == 0)
	    InSpecB_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "UnderflowA") == 0)
	    UnderflowA_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "UnderflowB") == 0)
	    UnderflowB_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "ChanEventsA") == 0)
	    ChanEventsA_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "ChanEventsB") == 0)
	    ChanEventsB_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "AutoTau") == 0)
	    AutoTau_Address[ModNum] = DSP_Parameter_Addr[k];
	  else if(strcmp(str, "U30") == 0)
	    U30_Address[ModNum] = DSP_Parameter_Addr[k];
	}
		
      // Check if all the DSP variable names have been found in the file
		
      //--------------------
      //	Module parameters
      //--------------------
		
      if(ModNum_Address[ModNum] == 0)
	{
	  printf("*ERROR* : ModNum was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(ModCSRA_Address[ModNum] == 0)
	{
	  printf("*ERROR* : ModCSRA was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(ModCSRB_Address[ModNum] == 0)
	{
	  printf("*ERROR* : ModCSRB was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(ModFormat_Address[ModNum] == 0)
	{
	  printf("*ERROR* : ModFormat was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(RunTask_Address[ModNum] == 0)
	{
	  printf("*ERROR* : RunTask was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(ControlTask_Address[ModNum] == 0)
	{
	  printf("*ERROR* : ControlTask was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(MaxEvents_Address[ModNum] == 0)
	{
	  printf("*ERROR* : MaxEvents was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(CoincPattern_Address[ModNum] == 0)
	{
	  printf("*ERROR* : CoincPattern was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(CoincWait_Address[ModNum] == 0)
	{
	  printf("*ERROR* : CoincWait was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(SynchWait_Address[ModNum] == 0)
	{
	  printf("*ERROR* : SynchWait was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(InSynch_Address[ModNum] == 0)
	{
	  printf("*ERROR* : InSynch was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(Resume_Address[ModNum] == 0)
	{
	  printf("*ERROR* : Resume was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(SlowFilterRange_Address[ModNum] == 0)
	{
	  printf("*ERROR* : SlowFilterRange was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(FastFilterRange_Address[ModNum] == 0)
	{
	  printf("*ERROR* : FastFilterRange was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(ChanNum_Address[ModNum] == 0)
	{
	  printf("*ERROR* : ChanNum was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(HostIO_Address[ModNum] == 0)
	{
	  printf("*ERROR* : HostIO was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(UserIn_Address[ModNum] == 0)
	{
	  printf("*ERROR* : UserIn was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(U00_Address[ModNum] == 0)
	{
	  printf("*ERROR* : U00 was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}	
      if(FastTrigBackplaneEna_Address[ModNum] == 0)
	{
	  printf("*ERROR* : FastTrigBackplaneEna was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(CrateID_Address[ModNum] == 0)
	{
	  printf("*ERROR* : CrateID was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(SlotID_Address[ModNum] == 0)
	{
	  printf("*ERROR* : SlotID was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(ModID_Address[ModNum] == 0)
	{
	  printf("*ERROR* : ModID was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(TrigConfig_Address[ModNum] == 0)
	{
	  printf("*ERROR* : TrigConfig was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(HRTP_Address[ModNum] == 0)
	{
	  printf("*ERROR* : HostRunTimePreset was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
		
      //--------------------
      //	Channel parameters
      //--------------------
		
      if(ChanCSRa_Address[ModNum] == 0)
	{
	  printf("*ERROR* : ChanCSRa was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(ChanCSRb_Address[ModNum] == 0)
	{
	  printf("*ERROR* : ChanCSRb was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(GainDAC_Address[ModNum] == 0)
	{
	  printf("*ERROR* : GainDAC was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(OffsetDAC_Address[ModNum] == 0)
	{
	  printf("*ERROR* : OffsetDAC was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(DigGain_Address[ModNum] == 0)
	{
	  printf("*ERROR* : DigGain was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(SlowLength_Address[ModNum] == 0)
	{
	  printf("*ERROR* : SlowLength was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(SlowGap_Address[ModNum] == 0)
	{
	  printf("*ERROR* : SlowGap was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(FastLength_Address[ModNum] == 0)
	{
	  printf("*ERROR* : FastLength was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(FastGap_Address[ModNum] == 0)
	{
	  printf("*ERROR* : FastGap was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(PeakSample_Address[ModNum] == 0)
	{
	  printf("*ERROR* : PeakSample was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(PeakSep_Address[ModNum] == 0)
	{
	  printf("*ERROR* : PeakSep was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(CFDThresh_Address[ModNum] == 0)
	{
	  printf("*ERROR* : CFDThresh was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(FastThresh_Address[ModNum] == 0)
	{
	  printf("*ERROR* : FastThresh was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(ThreshWidth_Address[ModNum] == 0)
	{
	  printf("*ERROR* : ThreshWidth was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(PAFlength_Address[ModNum] == 0)
	{
	  printf("*ERROR* : PAFlength was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(TriggerDelay_Address[ModNum] == 0)
	{
	  printf("*ERROR* : TriggerDelay was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(ResetDelay_Address[ModNum] == 0)
	{
	  printf("*ERROR* : ResetDelay was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(ChanTrigStretch_Address[ModNum] == 0)
	{
	  printf("*ERROR* : ChanTrigStretch was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(TraceLength_Address[ModNum] == 0)
	{
	  printf("*ERROR* : TraceLength was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(TrigOutLen_Address[ModNum] == 0)
	{
	  printf("*ERROR* : TrigOutLen was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(EnergyLow_Address[ModNum] == 0)
	{
	  printf("*ERROR* : EnergyLow was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(Log2Ebin_Address[ModNum] == 0)
	{
	  printf("*ERROR* : Log2Ebin was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(Log2Bweight_Address[ModNum] == 0)
	{
	  printf("*ERROR* : Log2Bweight was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(EnergyLow_Address[ModNum] == 0)
	{
	  printf("*ERROR* : EnergyLow was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(MultiplicityMaskL_Address[ModNum] == 0)
	{
	  printf("*ERROR* : MultiplicityMaskL was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(PSAoffset_Address[ModNum] == 0)
	{
	  printf("*ERROR* : PSAoffset was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(PSAlength_Address[ModNum] == 0)
	{
	  printf("*ERROR* : PSAlength was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(Integrator_Address[ModNum] == 0)
	{
	  printf("*ERROR* : Integrator was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(BLcut_Address[ModNum] == 0)
	{
	  printf("*ERROR* : BLcut was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(BaselinePercent_Address[ModNum] == 0)
	{
	  printf("*ERROR* : BaselinePercent was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(FtrigoutDelay_Address[ModNum] == 0)
	{
	  printf("*ERROR* : FtrigoutDelay was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(Log2Bweight_Address[ModNum] == 0)
	{
	  printf("*ERROR* : Log2Bweight was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(PreampTau_Address[ModNum] == 0)
	{
	  printf("*ERROR* : PreampTau was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(MultiplicityMaskH_Address[ModNum] == 0)
	{
	  printf("*ERROR* : MultiplicityMaskH was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(FastTrigBackLen_Address[ModNum] == 0)
	{
	  printf("*ERROR* : FastTrigBackLen was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(Xwait_Address[ModNum] == 0)
	{
	  printf("*ERROR* : Xait was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(CFDDelay_Address[ModNum] == 0)
	{
	  printf("*ERROR* : CFDDelay was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(CFDScale_Address[ModNum] == 0)
	{
	  printf("*ERROR* : CFDScale was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(ExternDelayLen_Address[ModNum] == 0)
	{
	  printf("*ERROR* : ExternDelayLen was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(ExtTrigStretch_Address[ModNum] == 0)
	{
	  printf("*ERROR* : ExtTrigStretch was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(VetoStretch_Address[ModNum] == 0)
	{
	  printf("*ERROR* : VetoStretch was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(QDCLen0_Address[ModNum] == 0)
	{
	  printf("*ERROR* : QDCLen0 was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(QDCLen1_Address[ModNum] == 0)
	{
	  printf("*ERROR* : QDCLen1 was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(QDCLen2_Address[ModNum] == 0)
	{
	  printf("*ERROR* : QDCLen2 was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(QDCLen3_Address[ModNum] == 0)
	{
	  printf("*ERROR* : QDCLen3 was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(QDCLen4_Address[ModNum] == 0)
	{
	  printf("*ERROR* : QDCLen4 was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(QDCLen5_Address[ModNum] == 0)
	{
	  printf("*ERROR* : QDCLen5 was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(QDCLen6_Address[ModNum] == 0)
	{
	  printf("*ERROR* : QDCLen6 was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(QDCLen7_Address[ModNum] == 0)
	{
	  printf("*ERROR* : QDCLen7 was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
		
      //--------------------
      //	Results parameters
      //--------------------
		
		
      if(RealTimeA_Address[ModNum] == 0)
	{
	  printf("*ERROR* : RealTimeA was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(RealTimeB_Address[ModNum] == 0)
	{
	  printf("*ERROR* : RealTimeB was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(RunTimeA_Address[ModNum] == 0)
	{
	  printf("*ERROR* : RunTimeA was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(RunTimeB_Address[ModNum] == 0)
	{
	  printf("*ERROR* : RunTimeB was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(GSLTtime_Address[ModNum] == 0)
	{
	  printf("*ERROR* : GSLTtime was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(NumEventsA_Address[ModNum] == 0)
	{
	  printf("*ERROR* : NumEventsA was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(NumEventsB_Address[ModNum] == 0)
	{
	  printf("*ERROR* : NumEventsB was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(DSPerror_Address[ModNum] == 0)
	{
	  printf("*ERROR* : DSPerror was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(NumEventsB_Address[ModNum] == 0)
	{
	  printf("*ERROR* : NumEventsB was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(SynchDone_Address[ModNum] == 0)
	{
	  printf("*ERROR* : SynchDone was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(BufHeadLen_Address[ModNum] == 0)
	{
	  printf("*ERROR* : BufHeadLen was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(EventHeadLen_Address[ModNum] == 0)
	{
	  printf("*ERROR* : EventHeadLen was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(ChanHeadLen_Address[ModNum] == 0)
	{
	  printf("*ERROR* : ChanHeadLen was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(UserOut_Address[ModNum] == 0)
	{
	  printf("*ERROR* : UserOut was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(AOutBuffer_Address[ModNum] == 0)
	{
	  printf("*ERROR* : AOutBuffer was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(UserOut_Address[ModNum] == 0)
	{
	  printf("*ERROR* : UserOut was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(LOutBuffer_Address[ModNum] == 0)
	{
	  printf("*ERROR* : LOutBuffer was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(AECorr_Address[ModNum] == 0)
	{
	  printf("*ERROR* : AECorr was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(LECorr_Address[ModNum] == 0)
	{
	  printf("*ERROR* : LECorr was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(HardwareID_Address[ModNum] == 0)
	{
	  printf("*ERROR* : HardwareID was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(HardVariant_Address[ModNum] == 0)
	{
	  printf("*ERROR* : HardVariant was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(FIFOLength_Address[ModNum] == 0)
	{
	  printf("*ERROR* : FIFOLength was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(FippiID_Address[ModNum] == 0)
	{
	  printf("*ERROR* : FippiID was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(FippiVariant_Address[ModNum] == 0)
	{
	  printf("*ERROR* : FippiVariant was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(FippiID_Address[ModNum] == 0)
	{
	  printf("*ERROR* : FippiID was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(DSPrelease_Address[ModNum] == 0)
	{
	  printf("*ERROR* : DSPrelease was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(DSPbuild_Address[ModNum] == 0)
	{
	  printf("*ERROR* : DSPbuild was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(DSPVariant_Address[ModNum] == 0)
	{
	  printf("*ERROR* : DSPVariant was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(U20_Address[ModNum] == 0)
	{
	  printf("*ERROR* : U20 was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(LiveTimeA_Address[ModNum] == 0)
	{
	  printf("*ERROR* : LiveTimeA was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(LiveTimeB_Address[ModNum] == 0)
	{
	  printf("*ERROR* : LiveTimeB was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(FastPeaksA_Address[ModNum] == 0)
	{
	  printf("*ERROR* : FastPeaksA was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(FastPeaksB_Address[ModNum] == 0)
	{
	  printf("*ERROR* : FastPeaksB was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(OverflowA_Address[ModNum] == 0)
	{
	  printf("*ERROR* : OverflowA was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(OverflowB_Address[ModNum] == 0)
	{
	  printf("*ERROR* : OverflowB was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(InSpecA_Address[ModNum] == 0)
	{
	  printf("*ERROR* : InSpecA was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(InSpecB_Address[ModNum] == 0)
	{
	  printf("*ERROR* : InSpecB was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(UnderflowA_Address[ModNum] == 0)
	{
	  printf("*ERROR* : UnderflowA was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(UnderflowB_Address[ModNum] == 0)
	{
	  printf("*ERROR* : UnderflowB was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(ChanEventsA_Address[ModNum] == 0)
	{
	  printf("*ERROR* : ChanEventsA was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(ChanEventsB_Address[ModNum] == 0)
	{
	  printf("*ERROR* : ChanEventsB was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(AutoTau_Address[ModNum] == 0)
	{
	  printf("*ERROR* : AutoTau was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
      if(U30_Address[ModNum] == 0)
	{
	  printf("*ERROR* : U30 was not found in the DSP .var file %s\n", DSPVarFile);
	  return(-2);
	}
    }
  else
    {
      printf("*ERROR* : DSP .var file %s can't be opened\n", DSPVarFile);
      return(-3);
    }
	
  return(0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Copy DSP variable address between modules.
// Return Value:
// 0 - success
// -1 - Invalid source module number
// -2 - Invalid destination module number
int algorithm::Copy_DSPVarAddress(unsigned short SourceModNum, unsigned short DestinationModNum)
{
  // Copy Address
	
  //--------------------
  //	Module parameters
  //--------------------
  ModNum_Address[DestinationModNum] = ModNum_Address[SourceModNum];
  ModCSRA_Address[DestinationModNum] = ModCSRA_Address[SourceModNum];
  ModCSRB_Address[DestinationModNum] = ModCSRB_Address[SourceModNum];
  ModFormat_Address[DestinationModNum] = ModFormat_Address[SourceModNum];
  RunTask_Address[DestinationModNum] = RunTask_Address[SourceModNum];
  ControlTask_Address[DestinationModNum] = ControlTask_Address[SourceModNum];
  MaxEvents_Address[DestinationModNum] = MaxEvents_Address[SourceModNum];
  CoincPattern_Address[DestinationModNum] = CoincPattern_Address[SourceModNum];
  CoincWait_Address[DestinationModNum] = CoincWait_Address[SourceModNum];
  SynchWait_Address[DestinationModNum] = SynchWait_Address[SourceModNum];
  InSynch_Address[DestinationModNum] = InSynch_Address[SourceModNum];
  Resume_Address[DestinationModNum] = Resume_Address[SourceModNum];
  SlowFilterRange_Address[DestinationModNum] = SlowFilterRange_Address[SourceModNum];
  FastFilterRange_Address[DestinationModNum] = FastFilterRange_Address[SourceModNum];
  ChanNum_Address[DestinationModNum] = ChanNum_Address[SourceModNum];
  HostIO_Address[DestinationModNum] = HostIO_Address[SourceModNum];
  UserIn_Address[DestinationModNum] = UserIn_Address[SourceModNum];
  U00_Address[DestinationModNum] = U00_Address[SourceModNum];
  FastTrigBackplaneEna_Address[DestinationModNum] = FastTrigBackplaneEna_Address[SourceModNum];
  CrateID_Address             [DestinationModNum] = CrateID_Address             [SourceModNum];
  SlotID_Address              [DestinationModNum] = SlotID_Address              [SourceModNum];
  ModID_Address               [DestinationModNum] = ModID_Address               [SourceModNum];
  TrigConfig_Address          [DestinationModNum] = TrigConfig_Address          [SourceModNum];
  HRTP_Address[DestinationModNum] = HRTP_Address[SourceModNum];

  //--------------------
  //	Channel parameters
  //--------------------
	
  ChanCSRa_Address[DestinationModNum] = ChanCSRa_Address[SourceModNum];
  ChanCSRb_Address[DestinationModNum] = ChanCSRb_Address[SourceModNum];
  GainDAC_Address[DestinationModNum] = GainDAC_Address[SourceModNum];
  OffsetDAC_Address[DestinationModNum] = OffsetDAC_Address[SourceModNum];
  DigGain_Address[DestinationModNum] = DigGain_Address[SourceModNum];
  SlowLength_Address[DestinationModNum] = SlowLength_Address[SourceModNum];
  SlowGap_Address[DestinationModNum] = SlowGap_Address[SourceModNum];
  FastLength_Address[DestinationModNum] = FastLength_Address[SourceModNum];
  FastGap_Address[DestinationModNum] = FastGap_Address[SourceModNum];
  PeakSample_Address[DestinationModNum] = PeakSample_Address[SourceModNum];
  PeakSep_Address[DestinationModNum] = PeakSep_Address[SourceModNum];
  CFDThresh_Address[DestinationModNum] = CFDThresh_Address[SourceModNum];
  FastThresh_Address[DestinationModNum] = FastThresh_Address[SourceModNum];
  ThreshWidth_Address[DestinationModNum] = ThreshWidth_Address[SourceModNum];
  PAFlength_Address[DestinationModNum] = PAFlength_Address[SourceModNum];
  TriggerDelay_Address[DestinationModNum] = TriggerDelay_Address[SourceModNum];
  ResetDelay_Address[DestinationModNum] = ResetDelay_Address[SourceModNum];
  ChanTrigStretch_Address[DestinationModNum] = ChanTrigStretch_Address[SourceModNum];
  TraceLength_Address[DestinationModNum] = TraceLength_Address[SourceModNum];
  TrigOutLen_Address[DestinationModNum] = TrigOutLen_Address[SourceModNum];
  EnergyLow_Address[DestinationModNum] = EnergyLow_Address[SourceModNum];
  Log2Ebin_Address[DestinationModNum] = Log2Ebin_Address[SourceModNum];
  Log2Bweight_Address[DestinationModNum] = Log2Bweight_Address[SourceModNum];
  
  MultiplicityMaskL_Address[DestinationModNum] = MultiplicityMaskL_Address[SourceModNum];
  
  PSAoffset_Address[DestinationModNum] = PSAoffset_Address[SourceModNum];
  PSAlength_Address[DestinationModNum] = PSAlength_Address[SourceModNum];
  Integrator_Address[DestinationModNum] = Integrator_Address[SourceModNum];
  
  BLcut_Address[DestinationModNum] = BLcut_Address[SourceModNum];
  BaselinePercent_Address[DestinationModNum] = BaselinePercent_Address[SourceModNum];
  
  FtrigoutDelay_Address[DestinationModNum] = FtrigoutDelay_Address[SourceModNum];
  
  Log2Bweight_Address[DestinationModNum] = Log2Bweight_Address[SourceModNum];
  PreampTau_Address[DestinationModNum] = PreampTau_Address[SourceModNum];
  
  MultiplicityMaskH_Address[DestinationModNum] = MultiplicityMaskH_Address[SourceModNum];

  FastTrigBackLen_Address[DestinationModNum] = FastTrigBackLen_Address[SourceModNum];
	
  Xwait_Address[DestinationModNum] = Xwait_Address[SourceModNum];
  
  CFDDelay_Address            [DestinationModNum] = CFDDelay_Address            [SourceModNum];
  CFDScale_Address            [DestinationModNum] = CFDScale_Address            [SourceModNum];
  ExternDelayLen_Address      [DestinationModNum] = ExternDelayLen_Address      [SourceModNum];
  ExtTrigStretch_Address      [DestinationModNum] = ExtTrigStretch_Address      [SourceModNum];
  VetoStretch_Address         [DestinationModNum] = VetoStretch_Address         [SourceModNum];
  QDCLen0_Address             [DestinationModNum] = QDCLen0_Address             [SourceModNum];
  QDCLen1_Address             [DestinationModNum] = QDCLen1_Address             [SourceModNum];
  QDCLen2_Address             [DestinationModNum] = QDCLen2_Address             [SourceModNum];
  QDCLen3_Address             [DestinationModNum] = QDCLen3_Address             [SourceModNum];
  QDCLen4_Address             [DestinationModNum] = QDCLen4_Address             [SourceModNum];
  QDCLen5_Address             [DestinationModNum] = QDCLen5_Address             [SourceModNum];
  QDCLen6_Address             [DestinationModNum] = QDCLen6_Address             [SourceModNum];
  QDCLen7_Address             [DestinationModNum] = QDCLen7_Address             [SourceModNum];
	
  //--------------------
  //	Results parameters
  //--------------------
	
  RealTimeA_Address[DestinationModNum] = RealTimeA_Address[SourceModNum];
  RealTimeB_Address[DestinationModNum] = RealTimeB_Address[SourceModNum];
  RunTimeA_Address[DestinationModNum] = RunTimeA_Address[SourceModNum];
  RunTimeB_Address[DestinationModNum] = RunTimeB_Address[SourceModNum];
  GSLTtime_Address[DestinationModNum] = GSLTtime_Address[SourceModNum];
  NumEventsA_Address[DestinationModNum] = NumEventsA_Address[SourceModNum];
  NumEventsB_Address[DestinationModNum] = NumEventsB_Address[SourceModNum];
  DSPerror_Address[DestinationModNum] = DSPerror_Address[SourceModNum];
  SynchDone_Address[DestinationModNum] = SynchDone_Address[SourceModNum];
  BufHeadLen_Address[DestinationModNum] = BufHeadLen_Address[SourceModNum];
  EventHeadLen_Address[DestinationModNum] = EventHeadLen_Address[SourceModNum];
  ChanHeadLen_Address[DestinationModNum] = ChanHeadLen_Address[SourceModNum];
  UserOut_Address[DestinationModNum] = UserOut_Address[SourceModNum];
  AOutBuffer_Address[DestinationModNum] = AOutBuffer_Address[SourceModNum];
  LOutBuffer_Address[DestinationModNum] = LOutBuffer_Address[SourceModNum];
  AECorr_Address[DestinationModNum] = AECorr_Address[SourceModNum];
  LECorr_Address[DestinationModNum] = LECorr_Address[SourceModNum];
  HardwareID_Address[DestinationModNum] = HardwareID_Address[SourceModNum];
  HardVariant_Address[DestinationModNum] = HardVariant_Address[SourceModNum];
  FIFOLength_Address[DestinationModNum] = FIFOLength_Address[SourceModNum];
  FippiID_Address[DestinationModNum] = FippiID_Address[SourceModNum];
  FippiVariant_Address[DestinationModNum] = FippiVariant_Address[SourceModNum];
  DSPrelease_Address[DestinationModNum] = DSPrelease_Address[SourceModNum];
  DSPbuild_Address[DestinationModNum] = DSPbuild_Address[SourceModNum];
  DSPVariant_Address[DestinationModNum] = DSPVariant_Address[SourceModNum];
  U20_Address[DestinationModNum] = U20_Address[SourceModNum];
  LiveTimeA_Address[DestinationModNum] = LiveTimeA_Address[SourceModNum];
  LiveTimeB_Address[DestinationModNum] = LiveTimeB_Address[SourceModNum];
  FastPeaksA_Address[DestinationModNum] = FastPeaksA_Address[SourceModNum];
  FastPeaksB_Address[DestinationModNum] = FastPeaksB_Address[SourceModNum];
  OverflowA_Address[DestinationModNum] = OverflowA_Address[SourceModNum];
  OverflowB_Address[DestinationModNum] = OverflowB_Address[SourceModNum];
  InSpecA_Address[DestinationModNum] = InSpecA_Address[SourceModNum];
  InSpecB_Address[DestinationModNum] = InSpecB_Address[SourceModNum];
  UnderflowA_Address[DestinationModNum] = UnderflowA_Address[SourceModNum];
  UnderflowB_Address[DestinationModNum] = UnderflowB_Address[SourceModNum];
  ChanEventsA_Address[DestinationModNum] = ChanEventsA_Address[SourceModNum];
  ChanEventsB_Address[DestinationModNum] = ChanEventsB_Address[SourceModNum];
  AutoTau_Address[DestinationModNum] = AutoTau_Address[SourceModNum];
  U30_Address[DestinationModNum] = U30_Address[SourceModNum];
	
  return(0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Test a bit in a 16-bit unsigned integer.
unsigned short  algorithm::TstBit_16(unsigned short bit,unsigned short value)//获得value第bit位数值
{
  return(((value & (unsigned short)(std::pow(2.0, (double)bit))) >> bit));
}

// Set a bit in a 16-bit unsigned integer.
unsigned short algorithm::SetBit_16(unsigned short bit,unsigned short value)//将value第bit位设为1
{
  return(value | (unsigned short)(std::pow(2.0, (double)bit)));
}

// Clear a bit in a 16-bit unsigned integer.
unsigned short algorithm::ClrBit_16(unsigned short bit,unsigned short value)//将value第bit位设为0
{
  value = SetBit_16(bit, value);
  return(value ^ (unsigned short)(std::pow(2.0, (double)bit)));
}

// Set a bit in a 32-bit unsigned integer.
unsigned int algorithm::SetBit_32(unsigned short bit,unsigned int value)//将value第bit位设为1
{
  return(value | (unsigned int)(std::pow(2.0, (double)bit)));
}

// Clear a bit in a 32-bit unsigned integer.
unsigned int algorithm::ClrBit_32(unsigned short bit,unsigned int value)//将value第bit位设为0
{
  value = SetBit_32(bit, value);
  return(value ^ (unsigned int)(std::pow(2.0, (double)bit)));
}

// Test a bit in a 32-bit unsigned integer.
unsigned int algorithm::TstBit_32(unsigned short bit,unsigned int value )//获得value第bit位数值
{
  return(((value & (unsigned int)(std::pow(2.0, (double)bit))) >> bit));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

unsigned int algorithm::GetModuleEvents(char *FileName)
{
  int ModuleEvents = 0;
  unsigned int eventdata, eventlength;
  long TotalWords = 0;
  long TotalSkippedWords;
  FILE *ListModeFile = NULL;

  // Open the list mode file
  ListModeFile = fopen(FileName, "rb");
  if(ListModeFile != NULL)
    {	
      // Get file length
      fseek(ListModeFile, 0, SEEK_END);
      TotalWords = (ftell(ListModeFile) + 1) / 4;
      
      
      // Point ListModeFile to the beginning of file
      fseek(ListModeFile, 0, SEEK_SET);
		
      // Initialize indicator and counter
      TotalSkippedWords = 0;
		
      do
	{
	  fread(&eventdata, 4, 1, ListModeFile);
	  eventlength = (eventdata & 0x7FFE0000) >> 17;
	  TotalSkippedWords += eventlength;
	  fseek(ListModeFile, (eventlength-1)*4, SEEK_CUR);
	  ModuleEvents ++;
	}while( TotalSkippedWords < TotalWords );
		
      fclose(ListModeFile);
    }
  
  if(TotalWords == 0) ModuleEvents = 0;
  return ModuleEvents;
  
}

void algorithm::GetEventsInfo(char *FileName, unsigned int *EventInformation)
{
  unsigned int eventdata, headerlength, eventlength;
  unsigned int  NumEvents;
  long TotalWords, TotalSkippedWords;
  FILE *ListModeFile = NULL;

  // Open the list mode file
  ListModeFile = fopen(FileName, "rb");
  if(ListModeFile != NULL)
    {	
      // Get file length
      fseek(ListModeFile, 0, SEEK_END);
      TotalWords = (ftell(ListModeFile) + 1) / 4;
      // Point ListModeFile to the beginning of file
      fseek(ListModeFile, 0, SEEK_SET);
		
      // Initialize indicator and counter
      TotalSkippedWords = 0;
      NumEvents = 0;
		
      do
	{
	  fread(&eventdata, 4, 1, ListModeFile);		
	  // Event #
	  EventInformation[12 * NumEvents]     = NumEvents;
	  // Channel #
	  EventInformation[12 * NumEvents + 1] = (eventdata & 0xF);	
	  // Slot #
	  EventInformation[12 * NumEvents + 2] = (eventdata & 0xF0) >> 4;	
	  // Crate #
	  EventInformation[12 * NumEvents + 3] = (eventdata & 0xF00) >> 8;	
	  // Header length
	  headerlength = (eventdata & 0x1F000) >> 12;
	  EventInformation[12 * NumEvents + 4] = (eventdata & 0x1F000) >> 12;	
	  // Event length
	  eventlength = (eventdata & 0x7FFE0000) >> 17;
	  EventInformation[12 * NumEvents + 5] = (eventdata & 0x7FFE0000) >> 17;	
	  // Finish code
	  EventInformation[12 * NumEvents + 6] = (eventdata & 0x80000000) >> 31;	
	  fread(&eventdata, 4, 1, ListModeFile);
	  // EventTime_Low
	  EventInformation[12 * NumEvents + 7] = eventdata;	
	  fread(&eventdata, 4, 1, ListModeFile);
	  // EventTime_High
	  EventInformation[12 * NumEvents + 8] = (eventdata & 0xFFFF);
	  fread(&eventdata, 4, 1, ListModeFile);
	  // Event Energy
	  EventInformation[12 * NumEvents + 9] = (eventdata & 0xFFFF);
	  // Trace Length
	  EventInformation[12 * NumEvents + 10] = (eventdata & 0x7FFF0000) >> 16;
	  // Trace location
	  EventInformation[12 * NumEvents + 11] = TotalSkippedWords + headerlength;

	  // if(eventlength != headerlength + EventInformation[12 * NumEvents + 10]/2)
	  //   std::cout<<"Data error..."<<std::endl;
	  
	  TotalSkippedWords += eventlength;
	  NumEvents ++;
	  fseek(ListModeFile, (eventlength - 4) * 4, SEEK_CUR);
	}while( TotalSkippedWords < TotalWords );
		
      fclose(ListModeFile);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


// WriteSglModPar:
// Write a module parameter to a Pixie module.
// Return Value:
// 0 - Success
// -1 - Invalid Pixie module number
// -2 - Invalid module parameter name
// -3 - Failed to program Fippi after downloading module parameter
// -4 - Failed to find BLcut after downloading module parameter
int algorithm::WriteSglModPar(
			      char *ModParName,          // the name of the module parameter
			      unsigned int   ModParData, // the module parameter value to be written to the module
			      unsigned short ModNum )    // module number
{
  unsigned short k;
  unsigned int paflength, triggerdelay, tracedelay, fastfilterrange, LastFastFilterRange, baselinecut;
  unsigned int CSR, slowfilterrange;
  unsigned int CPLD_CSR;
	
  // Check if ModNum is valid
  if( ModNum > Number_Modules )
    {
      printf("*ERROR* (WriteSglModPar): invalid Pixie module number %d", ModNum);
      return(-1);
    }
	
  // Write module parameter
	
  if(strcmp(ModParName,"MODULE_CSRA") == 0)
    {
      // Update the DSP parameter ModCSRA
      Pixie_Devices[ModNum].DSP_Parameter_Values[ModCSRA_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;	
    }
  else if(strcmp(ModParName,"MODULE_CSRB") == 0)
    {
      // Update the DSP parameter ModCSRB
      Pixie_Devices[ModNum].DSP_Parameter_Values[ModCSRB_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;

      // //**************************************************
      // // Set up Pull-up resistors
      // //**************************************************
		
      // CPLD_CSR = 0xAAA;

      // // Pull-up
      // if( TstBit_32(MODCSRB_CPLDPULLUP, ModParData) )
      // 	{
      // 	  CPLD_CSR = SetBit_32(CPLDCSR_PULLUP, CPLD_CSR);
      // 	}
      // else
      // 	{
      // 	  CPLD_CSR = ClrBit_32(CPLDCSR_PULLUP, CPLD_CSR);
      // 	}

      // Pixie_Register_IO(ModNum, CFG_CTRLCS, MOD_WRITE, &CPLD_CSR);

      // //**************************************************
      // // Set pullups for the SYNCH lines on the backplane
      // //**************************************************
	
      // Pixie_ReadCSR(ModNum, &CSR);
      // if( TstBit_32(MODCSRB_CHASSISMASTER, ModParData) )
      // 	{
      // 	  CSR = SetBit_32(PULLUP_CTRL, CSR);
      // 	}
      // else
      // 	{
      // 	  CSR = ClrBit_32(PULLUP_CTRL, CSR);
      // 	}
      // Pixie_WrtCSR(ModNum, CSR);

    }
  else if(strcmp(ModParName,"MODULE_FORMAT") == 0)
    {
      // Update the DSP parameter ModFormat
      Pixie_Devices[ModNum].DSP_Parameter_Values[ModFormat_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;
    }
  else if(strcmp(ModParName,"MAX_EVENTS") == 0)
    {
      // Update the DSP parameter MaxEvents
      Pixie_Devices[ModNum].DSP_Parameter_Values[MaxEvents_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;
    }
  else if(strcmp(ModParName,"SYNCH_WAIT") == 0)
    {
      // Update the DSP parameter SynchWait
      Pixie_Devices[ModNum].DSP_Parameter_Values[SynchWait_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;
		
      // // When changing SynchWait in one Pixie module, we need to broadcast it to all other modules as well
      // Pixie_Broadcast("SYNCH_WAIT", ModNum);
    }
  else if(strcmp(ModParName,"IN_SYNCH") == 0)
    {
      // Update the DSP parameter InSynch
      Pixie_Devices[ModNum].DSP_Parameter_Values[InSynch_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;
	
      // // When changing InSynch in one Pixie module, we need to broadcast it to all other modules as well
      // Pixie_Broadcast("IN_SYNCH", ModNum);
    }
  else if(strcmp(ModParName,"SLOW_FILTER_RANGE") == 0)
    {
      // Check slow filter range limit
      slowfilterrange = ModParData;

      if(slowfilterrange > SLOWFILTERRANGE_MAX)
	{
	  slowfilterrange = SLOWFILTERRANGE_MAX;

	  printf("*WARNING* (WriteSglModPar): Maximum SlowFilterRange that is currently supported is %d", slowfilterrange);
	}
      if(slowfilterrange < SLOWFILTERRANGE_MIN)
	{
	  slowfilterrange = SLOWFILTERRANGE_MIN;

	  printf("*WARNING* (WriteSglModPar): Minimum SlowFilterRange that is currently supported is %d", slowfilterrange);
	}
		
      // // Update the DSP parameter SlowFilterRange
      Pixie_Devices[ModNum].DSP_Parameter_Values[SlowFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS] = slowfilterrange;
		
      /*******************************************/
      //	We need to recompute FIFO settings
      /*******************************************/
      fastfilterrange = Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];
      for(k=0; k<NUMBER_OF_CHANNELS; k++)
	{
	  // Get the current TraceDelay
	  paflength = Pixie_Devices[ModNum].DSP_Parameter_Values[PAFlength_Address[ModNum] + k - DATA_MEMORY_ADDRESS];
	  triggerdelay = Pixie_Devices[ModNum].DSP_Parameter_Values[TriggerDelay_Address[ModNum] + k - DATA_MEMORY_ADDRESS];
	  tracedelay = paflength - (unsigned int)((double)triggerdelay / pow(2.0, (double)fastfilterrange));
			
	  // Update FIFO settings (TriggerDelay and PAFLength)
	  ComputeFIFO(tracedelay, ModNum, k);
	}
		
		
      // /*******************************************/
      // //	We also need to update baseline cut value
      // /*******************************************/
      // for(k=0; k<NUMBER_OF_CHANNELS; k++)
      // 	{
      // 	  retval = Pixie16BLcutFinder(ModNum, k, &baselinecut);
      // 	  if(retval < 0)
      // 	    {
      // 	      sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglModPar): BLcutFinder failed in module %d after downloading SlowFilterRange, retval=%d", k, retval);
      // 	      Pixie_Print_MSG(ErrMSG);
      // 	      return(-4);
      // 	    }
      // 	}
    }
  else if(strcmp(ModParName,"FAST_FILTER_RANGE") == 0)
    {
      // Get the last fast filter range
      LastFastFilterRange = Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];
		
      // Check fast filter range limit
      fastfilterrange = ModParData;

      if(fastfilterrange > FASTFILTERRANGE_MAX)
	{
	  fastfilterrange = FASTFILTERRANGE_MAX;

	  printf("*WARNING* (WriteSglModPar): Maximum FastFilterRange that is currently supported is %d", fastfilterrange);
	}
      if(fastfilterrange < FASTFILTERRANGE_MIN)
	{
	  fastfilterrange = FASTFILTERRANGE_MIN;

	  printf("*WARNING* (Pixie16WriteSglModPar): Minimum FastFilterRange that is currently supported is %d", fastfilterrange);
	}
		
      // Update the new DSP parameter FastFilterRange
      Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS] = fastfilterrange;
		
      /*******************************************/
      //	We need to recompute FIFO settings
      /*******************************************/
      for(k=0; k<NUMBER_OF_CHANNELS; k++)
	{
	  // Get the current TraceDelay
	  paflength = Pixie_Devices[ModNum].DSP_Parameter_Values[PAFlength_Address[ModNum] + k - DATA_MEMORY_ADDRESS];
	  triggerdelay = Pixie_Devices[ModNum].DSP_Parameter_Values[TriggerDelay_Address[ModNum] + k - DATA_MEMORY_ADDRESS];
	  tracedelay = paflength - (unsigned int)((double)triggerdelay / pow(2.0, (double)LastFastFilterRange));
			
	  // Update FIFO settings (TriggerDelay and PAFLength)
	  ComputeFIFO(tracedelay, ModNum, k);
	}
    }
  else if(strcmp(ModParName,"FastTrigBackplaneEna") == 0)
    {		
      // Update the new DSP parameter FastTrigBackplaneEna
      Pixie_Devices[ModNum].DSP_Parameter_Values[FastTrigBackplaneEna_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;
    }
  else if(strcmp(ModParName,"CrateID") == 0)
    {		
      // Update the new DSP parameter CrateID
      Pixie_Devices[ModNum].DSP_Parameter_Values[CrateID_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;
    }	
  else if(strcmp(ModParName,"SlotID") == 0)
    {		
      // Update the new DSP parameter SlotID
      Pixie_Devices[ModNum].DSP_Parameter_Values[SlotID_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;
    }	
  else if(strcmp(ModParName,"ModID") == 0)
    {		
      // Update the new DSP parameter ModID
      Pixie_Devices[ModNum].DSP_Parameter_Values[ModID_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;
    }
  else if(strcmp(ModParName,"TrigConfig0") == 0)
    {		
      // Update the new DSP parameter TrigConfig0
      Pixie_Devices[ModNum].DSP_Parameter_Values[TrigConfig_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;
    }
  else if(strcmp(ModParName,"TrigConfig1") == 0)
    {		
      // Update the new DSP parameter TrigConfig1
      Pixie_Devices[ModNum].DSP_Parameter_Values[TrigConfig_Address[ModNum] + 1 - DATA_MEMORY_ADDRESS] = ModParData;
    }
  else if(strcmp(ModParName,"TrigConfig2") == 0)
    {		
      // Update the new DSP parameter TrigConfig2
      Pixie_Devices[ModNum].DSP_Parameter_Values[TrigConfig_Address[ModNum] + 2 - DATA_MEMORY_ADDRESS] = ModParData; 
    }
  else if(strcmp(ModParName,"TrigConfig3") == 0)
    {		
      // Update the new DSP parameter TrigConfig3
      Pixie_Devices[ModNum].DSP_Parameter_Values[TrigConfig_Address[ModNum] + 3 - DATA_MEMORY_ADDRESS] = ModParData;     
    }
  else if(strcmp(ModParName,"HOST_RT_PRESET") == 0)
    {		
      // Update the new DSP parameter HRTP
      Pixie_Devices[ModNum].DSP_Parameter_Values[HRTP_Address[ModNum] - DATA_MEMORY_ADDRESS] = ModParData;

      // // When changing HostRunTimePreset in one Pixie module, we need to broadcast it to all other modules as well
      // Pixie_Broadcast("HOST_RT_PRESET", ModNum);
    }
  else
    {
      printf("*ERROR* (WriteSglModPar): invalid module parameter name %s", ModParName);
      return(-2);
    }
	
  return(0);
}

// ReadSglModPar:
// Read a module parameter from a Pixie module.
// Return Value:
// 0 - Success
// -1 - Invalid Pixie module number
// -2 - Invalid module parameter name
int algorithm::ReadSglModPar(
			     char *ModParName,           // the name of the module parameter
			     unsigned int   *ModParData, // the module parameter value to be read from the module
			     unsigned short ModNum )     // module number
{
  // Check if ModNum is valid
  if( ModNum >= Number_Modules )
    {
      printf("*ERROR* (ReadSglModPar): invalid Pixie module number %d", ModNum);
      return(-1);
    }
	
  // Read module parameter
	
  if(strcmp(ModParName,"MODULE_NUMBER") == 0)
    {
      // Update module parameter ModNum
      *ModParData = Pixie_Devices[ModNum].DSP_Parameter_Values[ModNum_Address[ModNum] - DATA_MEMORY_ADDRESS];
    }
  else if(strcmp(ModParName,"MODULE_CSRA") == 0)
    {
      // Update module parameter ModCSRA
      *ModParData = Pixie_Devices[ModNum].DSP_Parameter_Values[ModCSRA_Address[ModNum] - DATA_MEMORY_ADDRESS];
    }
  else if(strcmp(ModParName,"MODULE_CSRB") == 0)
    {	
      // Update module parameter ModCSRB
      *ModParData = Pixie_Devices[ModNum].DSP_Parameter_Values[ModCSRB_Address[ModNum] - DATA_MEMORY_ADDRESS];
    }
  else if(strcmp(ModParName,"MODULE_FORMAT") == 0)
    {		
      // Update module parameter ModFormat
      *ModParData = Pixie_Devices[ModNum].DSP_Parameter_Values[ModFormat_Address[ModNum] - DATA_MEMORY_ADDRESS];
    }
  else if(strcmp(ModParName,"MAX_EVENTS") == 0)
    {
      // Update module parameter MaxEvents
      *ModParData = Pixie_Devices[ModNum].DSP_Parameter_Values[MaxEvents_Address[ModNum] - DATA_MEMORY_ADDRESS];
    }
  else if(strcmp(ModParName,"SYNCH_WAIT") == 0)
    {		
      // Update module parameter SynchWait
      *ModParData = Pixie_Devices[ModNum].DSP_Parameter_Values[SynchWait_Address[ModNum] - DATA_MEMORY_ADDRESS];
    }
  else if(strcmp(ModParName,"IN_SYNCH") == 0)
    {		
      // Update module parameter InSynch
      *ModParData = Pixie_Devices[ModNum].DSP_Parameter_Values[InSynch_Address[ModNum] - DATA_MEMORY_ADDRESS];
    }
  else if(strcmp(ModParName,"SLOW_FILTER_RANGE") == 0)
    {		
      // Update module parameter SlowFilterRange
      *ModParData = Pixie_Devices[ModNum].DSP_Parameter_Values[SlowFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];
    }
  else if(strcmp(ModParName,"FAST_FILTER_RANGE") == 0)
    {	
      // Update module parameter FastFilterRange
      *ModParData = Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];
    }
  else if(strcmp(ModParName,"FastTrigBackplaneEna") == 0)
    {	
      // Update module parameter FastTrigBackplaneEna
      *ModParData = Pixie_Devices[ModNum].DSP_Parameter_Values[FastTrigBackplaneEna_Address[ModNum] - DATA_MEMORY_ADDRESS];
    }	
  else if(strcmp(ModParName,"CrateID") == 0)
    {	
      // Update module parameter CrateID
      *ModParData = Pixie_Devices[ModNum].DSP_Parameter_Values[CrateID_Address[ModNum] - DATA_MEMORY_ADDRESS];
    }	
  else if(strcmp(ModParName,"SlotID") == 0)
    {	
      // Update module parameter SlotID
      *ModParData = Pixie_Devices[ModNum].DSP_Parameter_Values[SlotID_Address[ModNum] - DATA_MEMORY_ADDRESS];
    }	
  else if(strcmp(ModParName,"ModID") == 0)
    {
      // Update module parameter ModID
      *ModParData = Pixie_Devices[ModNum].DSP_Parameter_Values[ModID_Address[ModNum] - DATA_MEMORY_ADDRESS];
    }
  else if(strcmp(ModParName,"TrigConfig0") == 0)
    {
      // Update module parameter TrigConfig0
      *ModParData = Pixie_Devices[ModNum].DSP_Parameter_Values[TrigConfig_Address[ModNum] - DATA_MEMORY_ADDRESS];
    }	
  else if(strcmp(ModParName,"TrigConfig1") == 0)
    {
      // Update module parameter TrigConfig1
      *ModParData = Pixie_Devices[ModNum].DSP_Parameter_Values[TrigConfig_Address[ModNum] + 1 - DATA_MEMORY_ADDRESS];
    }
  else if(strcmp(ModParName,"TrigConfig2") == 0)
    {
      // Update module parameter TrigConfig2
      *ModParData = Pixie_Devices[ModNum].DSP_Parameter_Values[TrigConfig_Address[ModNum] + 2 - DATA_MEMORY_ADDRESS];
    }
  else if(strcmp(ModParName,"TrigConfig3") == 0)
    {	
      // Update module parameter TrigConfig3
      *ModParData = Pixie_Devices[ModNum].DSP_Parameter_Values[TrigConfig_Address[ModNum] + 3 - DATA_MEMORY_ADDRESS];
    }
  else if(strcmp(ModParName,"HOST_RT_PRESET") == 0)
    {	
      // Update module parameter HostRTPreset
      *ModParData = Pixie_Devices[ModNum].DSP_Parameter_Values[HRTP_Address[ModNum] - DATA_MEMORY_ADDRESS];
    }
  else
    {
      printf("*ERROR* (ReadSglModPar): invalid module parameter name %s", ModParName);
      return(-2);
    }
  return(0);
}

// WriteSglChanPar:
// Write a channel parameter to a Pixie module.
// Return Value:
// 0 - Success
// -1 - Invalid Pixie module number
// -2 - Invalid Pixie channel number
// -3 - Invalid channel parameter name
// -4 - Programing Fippi failed downloading channel parameter
// -5 - BLcutFinder failed downloading channel parameter
// -6 - SetDACs failed downloading channel parameter
int algorithm::WriteSglChanPar(
			       char *ChanParName,         // the name of the channel parameter
			       double ChanParData,        // the channel parameter value to be written to the module
			       unsigned short ModNum,     // module number
			       unsigned short ChanNum )   // channel number
{
  unsigned int FL, FG, SL, SG, FastFilterRange, SlowFilterRange, FifoLength;
  unsigned int fastthresh, peaksample, peaksep, preamptau, tracelength, tracedelay;
  unsigned int paflength, triggerdelay, offsetdac;
  unsigned int xwait, lastxwait;
  unsigned int baselinepercent, energylow, log2ebin, newchancsra, oldchancsra, chancsrb;
  unsigned int baselinecut, fasttrigbacklen, baselineaverage;
  unsigned int cfddelay, cfdscale, qdclen, exttrigstretch, vetostretch, externdelaylen, multiplicitymaskl, multiplicitymaskh, ftrigoutdelay;
  unsigned int chantrigstretch, cfdthresh;

  // Check if ModNum is valid
  if( ModNum > Number_Modules )
    {
      printf("*ERROR* (WriteSglChanPar): invalid Pixie module number %d", ModNum);
      return(-1);
    }
  // Check if ChanNum is valid
  if( ChanNum >= NUMBER_OF_CHANNELS )
    {
      printf("*ERROR* (WriteSglChanPar): invalid Pixie channel number %d", ChanNum);
      return(-2);
    }
	
  // Write channel parameter
	
  if(strcmp(ChanParName,"TRIGGER_RISETIME") == 0)
    {
		
      // Calculate fast length
      FastFilterRange = Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];
      if(Module_Information[ModNum].Module_ADCMSPS == 100)
	FL = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS / pow(2.0, (double)FastFilterRange));
      else if(Module_Information[ModNum].Module_ADCMSPS == 250)
	FL = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 2) / pow(2.0, (double)FastFilterRange));
      else if(Module_Information[ModNum].Module_ADCMSPS == 500)
	FL = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5) / pow(2.0, (double)FastFilterRange));

      // Check fast length limit
      FG = Pixie_Devices[ModNum].DSP_Parameter_Values[FastGap_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
      if( (FL + FG) > FASTFILTER_MAX_LEN )
	{
	  FL = FASTFILTER_MAX_LEN - FG;
	}
      if( FL < MIN_FASTLENGTH_LEN )
	{
	  FL = MIN_FASTLENGTH_LEN;
	  if( (FL + FG) > FASTFILTER_MAX_LEN )
	    {
	      FG = FASTFILTER_MAX_LEN - MIN_FASTLENGTH_LEN;
	    }
	}
		
      // Update DSP parameter FastLength
      Pixie_Devices[ModNum].DSP_Parameter_Values[FastLength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = FL;

      // Update DSP parameter FastGap
      Pixie_Devices[ModNum].DSP_Parameter_Values[FastGap_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = FG;
    }
  else if(strcmp(ChanParName,"TRIGGER_FLATTOP") == 0)
    {
		
      // Calculate fast gap
      FastFilterRange = Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];
      if(Module_Information[ModNum].Module_ADCMSPS == 100)
	FG = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS / pow(2.0, (double)FastFilterRange));
      else if(Module_Information[ModNum].Module_ADCMSPS == 250)
	FG = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 2) / pow(2.0, (double)FastFilterRange));
      else if(Module_Information[ModNum].Module_ADCMSPS == 500)
	FG = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5) / pow(2.0, (double)FastFilterRange));
		
      // Check fast gap limit
      FL = Pixie_Devices[ModNum].DSP_Parameter_Values[FastLength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
      if( (FL + FG) > FASTFILTER_MAX_LEN )
	{
	  FG = FASTFILTER_MAX_LEN - FL;
	}
		
      // Update DSP parameter FastGap
      Pixie_Devices[ModNum].DSP_Parameter_Values[FastGap_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = FG;
    }
  else if(strcmp(ChanParName,"TRIGGER_THRESHOLD") == 0)
    {
		
      // Calculate FastThresh
      FL = Pixie_Devices[ModNum].DSP_Parameter_Values[FastLength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
      if(Module_Information[ModNum].Module_ADCMSPS == 100)
	fastthresh = (unsigned int)(ChanParData * (double)FL);
      else if(Module_Information[ModNum].Module_ADCMSPS == 250)
	fastthresh = (unsigned int)(ChanParData * (double)FL * 2.0);
      else if(Module_Information[ModNum].Module_ADCMSPS == 500)
	fastthresh = (unsigned int)(ChanParData * (double)FL * 5.0);

      // Check FastThresh limit
      if( fastthresh >= FAST_THRESHOLD_MAX )
	{
	  fastthresh = (unsigned int)(((double)FAST_THRESHOLD_MAX / (double)FL - 0.5) * (double)FL);	// in ADC counts
	}
		
      // Update DSP parameter FastThresh
      Pixie_Devices[ModNum].DSP_Parameter_Values[FastThresh_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = fastthresh;
    }
  else if((strcmp(ChanParName,"ENERGY_RISETIME") == 0) || (strcmp(ChanParName,"ENERGY_FLATTOP") == 0))
    {
      // Get the current TraceDelay
      FastFilterRange = Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];
      paflength = Pixie_Devices[ModNum].DSP_Parameter_Values[PAFlength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
      triggerdelay = Pixie_Devices[ModNum].DSP_Parameter_Values[TriggerDelay_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
      tracedelay = paflength - (unsigned int)((double)triggerdelay / pow(2.0, (double)FastFilterRange));
		
      // Get the current SlowFilterRange
      SlowFilterRange = Pixie_Devices[ModNum].DSP_Parameter_Values[SlowFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];
		
      if(strcmp(ChanParName,"ENERGY_RISETIME") == 0)
	{
			
	  // Calculate slow length
	  if(Module_Information[ModNum].Module_ADCMSPS == 100)
	    SL = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS / pow(2.0, (double)SlowFilterRange));
	  else if(Module_Information[ModNum].Module_ADCMSPS == 250)
	    SL = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 2) / pow(2.0, (double)SlowFilterRange));
	  else if(Module_Information[ModNum].Module_ADCMSPS == 500)
	    SL = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5) / pow(2.0, (double)SlowFilterRange));

	  // Check slow length limit
	  SG = Pixie_Devices[ModNum].DSP_Parameter_Values[SlowGap_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
	  if( (SL + SG) > SLOWFILTER_MAX_LEN)
	    {
	      SL = SLOWFILTER_MAX_LEN - SG;
	    }
	  if( SL < MIN_SLOWLENGTH_LEN)
	    {
	      SL = MIN_SLOWLENGTH_LEN;
	      if((SL + SG) > SLOWFILTER_MAX_LEN)
		{
		  SG = SLOWFILTER_MAX_LEN - MIN_SLOWLENGTH_LEN;
		}
	    }
	}
      else if(strcmp(ChanParName,"ENERGY_FLATTOP") == 0)
	{
			
	  // Calculate slow gap
	  if(Module_Information[ModNum].Module_ADCMSPS == 100)
	    SG = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS / pow(2.0, (double)SlowFilterRange));
	  else if(Module_Information[ModNum].Module_ADCMSPS == 250)
	    SG = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 2) / pow(2.0, (double)SlowFilterRange));
	  else if(Module_Information[ModNum].Module_ADCMSPS == 500)
	    SG = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5) / pow(2.0, (double)SlowFilterRange));
			
	  // Check slow gap limit
	  SL = Pixie_Devices[ModNum].DSP_Parameter_Values[SlowLength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
	  if( (SL + SG) > SLOWFILTER_MAX_LEN)
	    {
	      SG = SLOWFILTER_MAX_LEN - SL;
	    }
	  if( SG < MIN_SLOWGAP_LEN)
	    {
	      SG = MIN_SLOWGAP_LEN;
	      if((SL + SG) > SLOWFILTER_MAX_LEN)
		{
		  SL = SLOWFILTER_MAX_LEN - MIN_SLOWGAP_LEN;
		}
	    }
	}
		
      // Update DSP parameter SlowLength
      Pixie_Devices[ModNum].DSP_Parameter_Values[SlowLength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = SL;
		
      // Update DSP parameter SlowGap
      Pixie_Devices[ModNum].DSP_Parameter_Values[SlowGap_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = SG;
		
      // Re-calculate PeakSample and PeakSep
      switch (SlowFilterRange)
	{
	case 1:
	  peaksample = SL + SG - 3;
	  break;
	case 2:
	  peaksample = SL + SG - 2;
	  break;
	case 3:
	  peaksample = SL + SG - 2;
	  break;
	case 4:
	  peaksample = SL + SG - 1;
	  break;
	case 5:
	  peaksample = SL + SG;
	  break;
	case 6:
	  peaksample = SL + SG + 1;
	  break;
	default:
	  peaksample = SL + SG - 2;
	  break;
	}

      peaksep = SL + SG;

      // Update DSP parameter PeakSample
      Pixie_Devices[ModNum].DSP_Parameter_Values[PeakSample_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = peaksample;
		
      // Update DSP parameter PeakSep
      Pixie_Devices[ModNum].DSP_Parameter_Values[PeakSep_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = peaksep;
		
      // Update FIFO settings (TriggerDelay and PAFLength)
      ComputeFIFO(tracedelay, ModNum, ChanNum);	
		
      // // Update baseline cut value
      // retval = Pixie16BLcutFinder(ModNum, ChanNum, &baselinecut);
      // if(retval < 0)
      // 	{
      // 	  sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): BLcutFinder failed in module %d channel %d after downloading EnergyRiseTime or EnergyFlatTop, retval=%d", ModNum, ChanNum, retval);
      // 	  Pixie_Print_MSG(ErrMSG);
      // 	  return(-5);
      // 	}
    }
  else if(strcmp(ChanParName,"TAU") == 0)
    {
		
      // Calculate PreampTau
      preamptau = Decimal2IEEEFloating(ChanParData);
		
      // Update DSP parameter PreampTau
      Pixie_Devices[ModNum].DSP_Parameter_Values[PreampTau_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = preamptau;
		
      // // Update baseline cut value
      // retval = Pixie16BLcutFinder(ModNum, ChanNum, &baselinecut);
      // if(retval < 0)
      // 	{
      // 	  sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): BLcutFinder failed in module %d channel %d after downloading Tau, retval=%d", ModNum, ChanNum, retval);
      // 	  Pixie_Print_MSG(ErrMSG);
      // 	  return(-5);
      // 	}
    }
  else if(strcmp(ChanParName,"TRACE_LENGTH") == 0)
    {
		
      // Get the current FastFilterRange
      FastFilterRange = Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];
		
      // Compute the requested TraceLength
      tracelength = (unsigned int)(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS / pow(2.0, (double)FastFilterRange));

      if(Module_Information[ModNum].Module_ADCMSPS == 500)
	{
	  // Ensure TraceLength is multiple of 10 to fit 10-to-5*2 packing in the FPGA
	  tracelength = ((unsigned int)(tracelength / 10)) * 10;
	  if(tracelength < TRACELEN_MIN_500MHZADC)
	    {
	      // Enforce a minimum value
	      tracelength = TRACELEN_MIN_500MHZADC;
	    }
	}
      else if((Module_Information[ModNum].Module_ADCMSPS == 250) || (Module_Information[ModNum].Module_ADCMSPS == 100))
	{
	  // Ensure TraceLength is an even number to fit 2-to-1 packing in the FPGA
	  tracelength = ClrBit_32(0, tracelength);
	  if(tracelength < TRACELEN_MIN_250OR100MHZADC)
	    {
	      // Enforce a minimum value
	      tracelength = TRACELEN_MIN_250OR100MHZADC;
	    }
	}

      // Check if TraceLength exceeds FifoLength
      FifoLength = Pixie_Devices[ModNum].DSP_Parameter_Values[FIFOLength_Address[ModNum] - DATA_MEMORY_ADDRESS];
      if(tracelength > FifoLength)
	{
	  tracelength = FifoLength;
	}
		
      // Update DSP parameter TraceLength
      Pixie_Devices[ModNum].DSP_Parameter_Values[TraceLength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = tracelength;
    }
  else if(strcmp(ChanParName,"TRACE_DELAY") == 0)
    {
		
      // Get the current FastFilterRange
      FastFilterRange = Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];
		
      // Check if TraceDelay exceeds TraceLength
      if(Module_Information[ModNum].Module_ADCMSPS == 100)
	tracedelay = (unsigned int)(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS / pow(2.0, (double)FastFilterRange));
      else if(Module_Information[ModNum].Module_ADCMSPS == 250)
	tracedelay = (unsigned int)(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 2) / pow(2.0, (double)FastFilterRange));
      else if(Module_Information[ModNum].Module_ADCMSPS == 500)
	tracedelay = (unsigned int)(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5) / pow(2.0, (double)FastFilterRange));

      tracelength = Pixie_Devices[ModNum].DSP_Parameter_Values[TraceLength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
      if(tracedelay > tracelength)
	{
	  tracedelay = (unsigned int)((double)tracelength / 2.0);
	}
		
      // Check if TraceDelay exceeds TRACEDELAY_MAX
      if(tracedelay > TRACEDELAY_MAX)
	{
	  tracedelay = TRACEDELAY_MAX;
	}

      // Update FIFO settings (TriggerDelay and PAFLength)
      ComputeFIFO(tracedelay, ModNum, ChanNum);
    }
  else if(strcmp(ChanParName,"VOFFSET") == 0)
    {
		
      offsetdac = (unsigned int)(65536.0 * (ChanParData / DAC_VOLTAGE_RANGE  + 1.0 / 2.0));
		
      // Check limit
      if(offsetdac > 65535)
	{
	  offsetdac = 65535;	// a 16-bit DAC
	}
		
      // Update DSP parameter OffsetDAC
      Pixie_Devices[ModNum].DSP_Parameter_Values[OffsetDAC_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = offsetdac;
		
      // // Set DACs to apply the new DAC settings
      // retval = Pixie16SetDACs(ModNum);
      // if(retval < 0)
      // 	{
      // 	  sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): SetDACs failed in module %d channel %d after downloading OffsetDACs, retval=%d", ModNum, ChanNum, retval);
      // 	  Pixie_Print_MSG(ErrMSG);
      // 	  return(-6);
      // 	}
    }
  else if(strcmp(ChanParName,"XDT") == 0)
    {
      // Get the last Xwait
      lastxwait = Pixie_Devices[ModNum].DSP_Parameter_Values[Xwait_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
      // Compute the new Xwait
      xwait = (unsigned int)ROUND(ChanParData * (double)DSP_CLOCK_MHZ);
      if((Module_Information[ModNum].Module_ADCMSPS == 100) || (Module_Information[ModNum].Module_ADCMSPS == 500))
	{
	  // For 100 MSPS RevB/C/D or RevF, or 500 MSPS RevF, xwait should be multiples of 6

	  if(xwait < 6)	// xwait should be at least 6
	    {
	      xwait = 6;
	    }
			
	  if(xwait > lastxwait) // increase Xwait
	    {
	      xwait = (unsigned int)(ceil((double)xwait / 6.0) * 6.0);
	    }
	  else	// decrease Xwait
	    {
	      xwait = (unsigned int)(floor((double)xwait / 6.0) * 6.0);
	    }
	}
      else
	{
	  // For 250 RevF, xwait should be multiples of 8

	  if(xwait < 8)	// xwait should be at least 8
	    {
	      xwait = 8;
	    }
			
	  if(xwait > lastxwait) // increase Xwait
	    {
	      xwait = (unsigned int)(ceil((double)xwait / 8.0) * 8.0);
	    }
	  else	// decrease Xwait
	    {
	      xwait = (unsigned int)(floor((double)xwait / 8.0) * 8.0);
	    }
	}

      // Update DSP parameter Xwait
      Pixie_Devices[ModNum].DSP_Parameter_Values[Xwait_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = xwait;		
    }
  else if(strcmp(ChanParName,"BASELINE_PERCENT") == 0)
    {
		
      // Get the new BaselinePercent
      baselinepercent = (unsigned int)ChanParData;
		
      // Check limit
      if(baselinepercent < 1)
	{
	  baselinepercent = 1;
	}
      else if(baselinepercent > 99)
	{
	  baselinepercent = 99;
	}
		
      // Update DSP parameter BaselinePercent
      Pixie_Devices[ModNum].DSP_Parameter_Values[BaselinePercent_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = baselinepercent;
		
    }
  else if(strcmp(ChanParName,"EMIN") == 0)
    {
		
      // Get the new EnergyLow
      energylow = (unsigned int)ChanParData;
		
      // Update DSP parameter EnergyLow
      Pixie_Devices[ModNum].DSP_Parameter_Values[EnergyLow_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = energylow;
		
    }
  else if(strcmp(ChanParName,"BINFACTOR") == 0)
    {
		
      // Get the new Log2Ebin
      log2ebin = (unsigned int)ChanParData;
		
      // Check limit
      if(log2ebin < 1)
	{
	  log2ebin = 1;
	}
      if(log2ebin > 6)
	{
	  log2ebin = 6;
	}
		
      // Convert to a signed negative number
      log2ebin = (unsigned int)(pow(2.0, 32.0) - (double)log2ebin);
		
      // Update DSP parameter Log2Ebin
      Pixie_Devices[ModNum].DSP_Parameter_Values[Log2Ebin_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = log2ebin;
		
    }
  else if(strcmp(ChanParName,"BASELINE_AVERAGE") == 0)
    {
		
      // Get the new baselineaverage
      baselineaverage = (unsigned int)ChanParData;

      // Check limit
      if(baselineaverage > 16)
	{
	  baselineaverage = 16;
	}
		
      // Convert to a signed negative number
      if(baselineaverage > 0)
	{
	  baselineaverage = (unsigned int)(pow(2.0, 32.0) - (double)baselineaverage);
	}
		
      // Update DSP parameter Log2Bweight
      Pixie_Devices[ModNum].DSP_Parameter_Values[Log2Bweight_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = baselineaverage;
	
    }
  else if(strcmp(ChanParName,"CHANNEL_CSRA") == 0)
    {
		
      // Get the new ChanCSRa
      newchancsra = (unsigned int)ChanParData;
      oldchancsra = Pixie_Devices[ModNum].DSP_Parameter_Values[ChanCSRa_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      // Update DSP parameter ChanCSRa
      Pixie_Devices[ModNum].DSP_Parameter_Values[ChanCSRa_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = newchancsra;
		
      // // Set DACs to apply the new DAC settings
      // retval = Pixie16SetDACs(ModNum);
      // if(retval < 0)
      // 	{
      // 	  sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): SetDACs failed in module %d channel %d after downloading ChanCSRA, retval=%d", ModNum, ChanNum, retval);
      // 	  Pixie_Print_MSG(ErrMSG);
      // 	  return(-6);
      // 	}
	
      // // Check if we need to update baseline cut value (only needed if Vgain changed)
      // if(TstBit_32(CCSRA_ENARELAY, newchancsra) != TstBit_32(CCSRA_ENARELAY, oldchancsra))
      // 	{
      // 	  retval = Pixie16BLcutFinder(ModNum, ChanNum, &baselinecut);
      // 	  if(retval < 0)
      // 	    {
      // 	      sprintf(ErrMSG, "*ERROR* (Pixie16WriteSglChanPar): BLcutFinder failed in module %d channel %d after downloading ChanCSRA, retval=%d", ModNum, ChanNum, retval);
      // 	      Pixie_Print_MSG(ErrMSG);
      // 	      return(-5);
      // 	    }
      // 	}
    }
  else if(strcmp(ChanParName,"CHANNEL_CSRB") == 0)
    {
		
      // Get the new ChanCSRb
      chancsrb = (unsigned int)ChanParData;
		
      // Update DSP parameter ChanCSRb
      Pixie_Devices[ModNum].DSP_Parameter_Values[ChanCSRb_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = chancsrb;
		
    }
  else if(strcmp(ChanParName,"BLCUT") == 0)
    {
		
      // Get the new BLcut
      baselinecut = (unsigned int)ChanParData;
		
      // Update DSP parameter BLcut
      Pixie_Devices[ModNum].DSP_Parameter_Values[BLcut_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = baselinecut;
		
    }
  else if(strcmp(ChanParName,"FASTTRIGBACKLEN") == 0)
    {
		
      // Get the new FastTrigBackLen
      if(Module_Information[ModNum].Module_ADCMSPS == 100)
	fasttrigbacklen = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS);
      else if(Module_Information[ModNum].Module_ADCMSPS == 250)
	fasttrigbacklen = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 2));
      else if(Module_Information[ModNum].Module_ADCMSPS == 500)
	fasttrigbacklen = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5));

      // Range check for FastTrigBackLen
      if((Module_Information[ModNum].Module_ADCMSPS == 100) || (Module_Information[ModNum].Module_ADCMSPS == 500))
	{
	  if(fasttrigbacklen < FASTTRIGBACKLEN_MIN_100MHZFIPCLK)
	    {
	      fasttrigbacklen = FASTTRIGBACKLEN_MIN_100MHZFIPCLK;
	    }
	}
      else if(Module_Information[ModNum].Module_ADCMSPS == 250)
	{
	  if(fasttrigbacklen < FASTTRIGBACKLEN_MIN_125MHZFIPCLK)
	    {
	      fasttrigbacklen = FASTTRIGBACKLEN_MIN_125MHZFIPCLK;
	    }
	}

      if(fasttrigbacklen > FASTTRIGBACKLEN_MAX)
	{
	  fasttrigbacklen = FASTTRIGBACKLEN_MAX;
	}
		
      // Update DSP parameter FastTrigBackLen
      Pixie_Devices[ModNum].DSP_Parameter_Values[FastTrigBackLen_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = fasttrigbacklen;
    }
  else if(strcmp(ChanParName,"CFDDelay") == 0)
    {
      // Get the new CFDDelay
      if(Module_Information[ModNum].Module_ADCMSPS == 100)
	cfddelay = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS);
      else if(Module_Information[ModNum].Module_ADCMSPS == 250)
	cfddelay = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 2));
      else if(Module_Information[ModNum].Module_ADCMSPS == 500)
	cfddelay = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5));

      // Range check for CFDDelay
      if(cfddelay < CFDDELAY_MIN)
	{
	  cfddelay = CFDDELAY_MIN;
	}
      if(cfddelay > CFDDELAY_MAX)
	{
	  cfddelay = CFDDELAY_MAX;
	}

      // Update DSP parameter CFDDelay
      Pixie_Devices[ModNum].DSP_Parameter_Values[CFDDelay_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = cfddelay;
    }
  else if(strcmp(ChanParName,"CFDScale") == 0)
    {
      // Get the new CFDScale
      cfdscale = (unsigned int)ChanParData;

      // Range check for the value of CFDScale
      if(cfdscale > CFDSCALE_MAX)
	{
	  cfdscale = CFDSCALE_MAX;
	}

      // Update DSP parameter CFDScale
      Pixie_Devices[ModNum].DSP_Parameter_Values[CFDScale_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = cfdscale;
    }
  else if(strcmp(ChanParName,"CFDThresh") == 0)
    {
      // Get the new CFDThresh
      cfdthresh = (unsigned int)ChanParData;

      // Range check for the value of CFDThresh
      if( (cfdthresh >= CFDTHRESH_MIN) && (cfdthresh <= CFDTHRESH_MAX) )
	{
	  // Update DSP parameter CFDThresh
	  Pixie_Devices[ModNum].DSP_Parameter_Values[CFDThresh_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = cfdthresh;
	}
      else
	{
	  printf("*ERROR* (WriteSglChanPar): Value of CFDThresh being set was out of its valid range");
	}
    }
  else if(strcmp(ChanParName,"QDCLen0") == 0)
    {
      // Get the new QDCLen0
      if(Module_Information[ModNum].Module_ADCMSPS == 500)
	qdclen = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5));
      else
	qdclen = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS);

      // Range check for QDC length
      if(qdclen < QDCLEN_MIN)
	{
	  qdclen = QDCLEN_MIN;
	}
      if(qdclen > QDCLEN_MAX)
	{
	  qdclen = QDCLEN_MAX;
	}

      // Update DSP parameter QDCLen0
      Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen0_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;
    }
  else if(strcmp(ChanParName,"QDCLen1") == 0)
    {
      // Get the new QDCLen1
      if(Module_Information[ModNum].Module_ADCMSPS == 500)
	qdclen = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5));
      else
	qdclen = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS);

      // Range check for QDC length
      if(qdclen < QDCLEN_MIN)
	{
	  qdclen = QDCLEN_MIN;
	}
      if(qdclen > QDCLEN_MAX)
	{
	  qdclen = QDCLEN_MAX;
	}

      // Update DSP parameter QDCLen1
      Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen1_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;
    }
  else if(strcmp(ChanParName,"QDCLen2") == 0)
    {
      // Get the new QDCLen2
      if(Module_Information[ModNum].Module_ADCMSPS == 500)
	qdclen = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5));
      else
	qdclen = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS);

      // Range check for QDC length
      if(qdclen < QDCLEN_MIN)
	{
	  qdclen = QDCLEN_MIN;
	}
      if(qdclen > QDCLEN_MAX)
	{
	  qdclen = QDCLEN_MAX;
	}

      // Update DSP parameter QDCLen2
      Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen2_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;
    }
  else if(strcmp(ChanParName,"QDCLen3") == 0)
    {
      // Get the new QDCLen3
      if(Module_Information[ModNum].Module_ADCMSPS == 500)
	qdclen = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5));
      else
	qdclen = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS);

      // Range check for QDC length
      if(qdclen < QDCLEN_MIN)
	{
	  qdclen = QDCLEN_MIN;
	}
      if(qdclen > QDCLEN_MAX)
	{
	  qdclen = QDCLEN_MAX;
	}

      // Update DSP parameter QDCLen3
      Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen3_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;
    }
  else if(strcmp(ChanParName,"QDCLen4") == 0)
    {
      // Get the new QDCLen4
      if(Module_Information[ModNum].Module_ADCMSPS == 500)
	qdclen = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5));
      else
	qdclen = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS);

      // Range check for QDC length
      if(qdclen < QDCLEN_MIN)
	{
	  qdclen = QDCLEN_MIN;
	}
      if(qdclen > QDCLEN_MAX)
	{
	  qdclen = QDCLEN_MAX;
	}

      // Update DSP parameter QDCLen4
      Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen4_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;
    }
  else if(strcmp(ChanParName,"QDCLen5") == 0)
    {
      // Get the new QDCLen5
      if(Module_Information[ModNum].Module_ADCMSPS == 500)
	qdclen = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5));
      else
	qdclen = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS);

      // Range check for QDC length
      if(qdclen < QDCLEN_MIN)
	{
	  qdclen = QDCLEN_MIN;
	}
      if(qdclen > QDCLEN_MAX)
	{
	  qdclen = QDCLEN_MAX;
	}

      // Update DSP parameter QDCLen5
      Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen5_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;
    }
  else if(strcmp(ChanParName,"QDCLen6") == 0)
    {
      // Get the new QDCLen6
      if(Module_Information[ModNum].Module_ADCMSPS == 500)
	qdclen = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5));
      else
	qdclen = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS);

      // Range check for QDC length
      if(qdclen < QDCLEN_MIN)
	{
	  qdclen = QDCLEN_MIN;
	}
      if(qdclen > QDCLEN_MAX)
	{
	  qdclen = QDCLEN_MAX;
	}

      // Update DSP parameter QDCLen6
      Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen6_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;
    }
  else if(strcmp(ChanParName,"QDCLen7") == 0)
    {
      // Get the new QDCLen7
      if(Module_Information[ModNum].Module_ADCMSPS == 500)
	qdclen = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5));
      else
	qdclen = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS);

      // Range check for QDC length
      if(qdclen < QDCLEN_MIN)
	{
	  qdclen = QDCLEN_MIN;
	}
      if(qdclen > QDCLEN_MAX)
	{
	  qdclen = QDCLEN_MAX;
	}

      // Update DSP parameter QDCLen7
      Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen7_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = qdclen;
    }
  else if(strcmp(ChanParName,"ExtTrigStretch") == 0)
    {
      // Get the new ExtTrigStretch
      if(Module_Information[ModNum].Module_ADCMSPS == 100)
	exttrigstretch = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS);
      else if(Module_Information[ModNum].Module_ADCMSPS == 250)
	exttrigstretch = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 2));
      else if(Module_Information[ModNum].Module_ADCMSPS == 500)
	exttrigstretch = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5));

      // Range check for ExtTrigStretch
      if(exttrigstretch < EXTTRIGSTRETCH_MIN)
	{
	  exttrigstretch = EXTTRIGSTRETCH_MIN;
	}
      if(exttrigstretch > EXTTRIGSTRETCH_MAX)
	{
	  exttrigstretch = EXTTRIGSTRETCH_MAX;
	}

      // Update DSP parameter ExtTrigStretch
      Pixie_Devices[ModNum].DSP_Parameter_Values[ExtTrigStretch_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = exttrigstretch;
    }
  else if(strcmp(ChanParName,"VetoStretch") == 0)
    {
      // Get the new VetoStretch
      if(Module_Information[ModNum].Module_ADCMSPS == 100)
	vetostretch = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS);
      else if(Module_Information[ModNum].Module_ADCMSPS == 250)
	vetostretch = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 2));
      else if(Module_Information[ModNum].Module_ADCMSPS == 500)
	vetostretch = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5));

      // Range check for VetoStretch
      if(vetostretch < VETOSTRETCH_MIN)
	{
	  vetostretch = VETOSTRETCH_MIN;
	}
      if(vetostretch > VETOSTRETCH_MAX)
	{
	  vetostretch = VETOSTRETCH_MAX;
	}

      // Update DSP parameter VetoStretch
      Pixie_Devices[ModNum].DSP_Parameter_Values[VetoStretch_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = vetostretch;
    }
  else if(strcmp(ChanParName,"MultiplicityMaskL") == 0)
    {
      // Get the new MultiplicityMaskL
      multiplicitymaskl = (unsigned int)ChanParData;

      // Update DSP parameter MultiplicityMaskL
      Pixie_Devices[ModNum].DSP_Parameter_Values[MultiplicityMaskL_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = multiplicitymaskl;
    }
  else if(strcmp(ChanParName,"MultiplicityMaskH") == 0)
    {
      // Get the new MultiplicityMaskH
      multiplicitymaskh = (unsigned int)ChanParData;

      // Update DSP parameter MultiplicityMaskH
      Pixie_Devices[ModNum].DSP_Parameter_Values[MultiplicityMaskH_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = multiplicitymaskh;
    }
  else if(strcmp(ChanParName,"ExternDelayLen") == 0)
    {
      // Get the new ExternDelayLen
      if(Module_Information[ModNum].Module_ADCMSPS == 100)
	externdelaylen = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS);
      else if(Module_Information[ModNum].Module_ADCMSPS == 250)
	externdelaylen = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 2));
      else if(Module_Information[ModNum].Module_ADCMSPS == 500)
	externdelaylen = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5));

      // Range check for ExternDelayLen
      if(externdelaylen < EXTDELAYLEN_MIN)
	{
	  externdelaylen = EXTDELAYLEN_MIN;
	}

      if ((Module_Information[ModNum].Module_Rev == 0xB) || (Module_Information[ModNum].Module_Rev == 0xC) || (Module_Information[ModNum].Module_Rev == 0xD))
	{
	  if(externdelaylen > EXTDELAYLEN_MAX_REVBCD)
	    {
	      externdelaylen = EXTDELAYLEN_MAX_REVBCD;
	    }

	}
      else if (Module_Information[ModNum].Module_Rev == 0xF)
	{
	  if(externdelaylen > EXTDELAYLEN_MAX_REVF)
	    {
	      externdelaylen = EXTDELAYLEN_MAX_REVF;
	    }
	}

      // Update DSP parameter ExternDelayLen
      Pixie_Devices[ModNum].DSP_Parameter_Values[ExternDelayLen_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = externdelaylen;
    }
  else if(strcmp(ChanParName,"FtrigoutDelay") == 0)
    {
      // Get the new FtrigoutDelay
      if(Module_Information[ModNum].Module_ADCMSPS == 100)
	ftrigoutdelay = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS);
      else if(Module_Information[ModNum].Module_ADCMSPS == 250)
	ftrigoutdelay = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 2));
      else if(Module_Information[ModNum].Module_ADCMSPS == 500)
	ftrigoutdelay = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5));

      // Range check for FtrigoutDelay
      if(ftrigoutdelay < FASTTRIGBACKDELAY_MIN)
	{
	  ftrigoutdelay = FASTTRIGBACKDELAY_MIN;
	}
		
      if ((Module_Information[ModNum].Module_Rev == 0xB) || (Module_Information[ModNum].Module_Rev == 0xC) || (Module_Information[ModNum].Module_Rev == 0xD))
	{
	  if(ftrigoutdelay > FASTTRIGBACKDELAY_MAX_REVBCD)
	    {
	      ftrigoutdelay = FASTTRIGBACKDELAY_MAX_REVBCD;
	    }
	}
      else if (Module_Information[ModNum].Module_Rev == 0xF)
	{
	  if(ftrigoutdelay > FASTTRIGBACKDELAY_MAX_REVF)
	    {
	      ftrigoutdelay = FASTTRIGBACKDELAY_MAX_REVF;
	    }
	}

      // Update DSP parameter FtrigoutDelay
      Pixie_Devices[ModNum].DSP_Parameter_Values[FtrigoutDelay_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = ftrigoutdelay;
    }
  else if(strcmp(ChanParName,"ChanTrigStretch") == 0)
    {
      // Get the new ChanTrigStretch
      if(Module_Information[ModNum].Module_ADCMSPS == 100)
	chantrigstretch = (unsigned int)ROUND(ChanParData * (double)Module_Information[ModNum].Module_ADCMSPS);
      else if(Module_Information[ModNum].Module_ADCMSPS == 250)
	chantrigstretch = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 2));
      else if(Module_Information[ModNum].Module_ADCMSPS == 500)
	chantrigstretch = (unsigned int)ROUND(ChanParData * (double)(Module_Information[ModNum].Module_ADCMSPS / 5));

      // Range check for ChanTrigStretch
      if(chantrigstretch < CHANTRIGSTRETCH_MIN)
	{
	  chantrigstretch = CHANTRIGSTRETCH_MIN;
	}
      if(chantrigstretch > CHANTRIGSTRETCH_MAX)
	{
	  chantrigstretch = CHANTRIGSTRETCH_MAX;
	}

      // Update DSP parameter ChanTrigStretch
      Pixie_Devices[ModNum].DSP_Parameter_Values[ChanTrigStretch_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = chantrigstretch;
    }
  else
    {
      printf("*ERROR* (WriteSglChanPar): invalid channel parameter name %s", ChanParName);
      return(-3);
    }
	
  return(0);
}

// ReadSglChanPar:
// Read a channel parameter from a Pixie module.
// Return Value:
// 0 - Success
// -1 - Invalid Pixie module number
// -2 - Invalid Pixie channel number
// -3 - Invalid channel parameter name
int algorithm::ReadSglChanPar(
			      char *ChanParName,         // the name of the channel parameter
			      double *ChanParData,       // the channel parameter value to be read from the module
			      unsigned short ModNum,     // module number
			      unsigned short ChanNum )   // channel number
{
  unsigned int FL, FG, SL, SG, FastFilterRange, SlowFilterRange;
  unsigned int fastthresh, preamptau, tracelength;
  unsigned int paflength, triggerdelay, offsetdac;
  unsigned int xwait;
  unsigned int baselinepercent, energylow, log2ebin, chancsra, chancsrb;
  unsigned int baselinecut, fasttrigbacklen, baselineaverage;
  unsigned int cfddelay, cfdscale, qdclen, exttrigstretch, vetostretch, externdelaylen, multiplicitymaskl, multiplicitymaskh, ftrigoutdelay;
  unsigned int chantrigstretch, cfdthresh;

  // Check if ModNum is valid
  if( ModNum >= Number_Modules )
    {
      printf("*ERROR* (ReadSglChanPar): invalid Pixie module number %d", ModNum);
      return(-1);
    }
  // Check if ChanNum is valid
  if( ChanNum >= NUMBER_OF_CHANNELS )
    {
      printf("*ERROR* (ReadSglChanPar): invalid Pixie channel number %d", ChanNum);
      return(-2);
    }
	
  // Read channel parameter
	
  if(strcmp(ChanParName,"TRIGGER_RISETIME") == 0)
    {
      FL = Pixie_Devices[ModNum].DSP_Parameter_Values[FastLength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      FastFilterRange = Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];
		
      // Update channel parameter TriggerRiseTime
      if(Module_Information[ModNum].Module_ADCMSPS == 100)
	*ChanParData = (double)FL * pow(2.0, (double)FastFilterRange) / (double)Module_Information[ModNum].Module_ADCMSPS;
      else if(Module_Information[ModNum].Module_ADCMSPS == 250)
	*ChanParData = (double)FL * pow(2.0, (double)FastFilterRange) / (double)(Module_Information[ModNum].Module_ADCMSPS / 2);
      else if(Module_Information[ModNum].Module_ADCMSPS == 500)
	*ChanParData = (double)FL * pow(2.0, (double)FastFilterRange) / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
	
    }
  else if(strcmp(ChanParName,"TRIGGER_FLATTOP") == 0)
    {
      FG = Pixie_Devices[ModNum].DSP_Parameter_Values[FastGap_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      FastFilterRange = Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];
		
      // Update channel parameter TriggerFlatTop
      if(Module_Information[ModNum].Module_ADCMSPS == 100)
	*ChanParData = (double)FG * pow(2.0, (double)FastFilterRange) / (double)Module_Information[ModNum].Module_ADCMSPS;
      else if(Module_Information[ModNum].Module_ADCMSPS == 250)
	*ChanParData = (double)FG * pow(2.0, (double)FastFilterRange) / (double)(Module_Information[ModNum].Module_ADCMSPS / 2);
      else if(Module_Information[ModNum].Module_ADCMSPS == 500)
	*ChanParData = (double)FG * pow(2.0, (double)FastFilterRange) / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
		
    }
  else if(strcmp(ChanParName,"TRIGGER_THRESHOLD") == 0)
    {
      fastthresh = Pixie_Devices[ModNum].DSP_Parameter_Values[FastThresh_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      FL = Pixie_Devices[ModNum].DSP_Parameter_Values[FastLength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      // Update channel parameter TriggerThreshold
      if(Module_Information[ModNum].Module_ADCMSPS == 100)
	*ChanParData = (double)fastthresh / (double)FL;
      else if(Module_Information[ModNum].Module_ADCMSPS == 250)
	*ChanParData = (double)fastthresh / ((double)FL * 2.0);
      else if(Module_Information[ModNum].Module_ADCMSPS == 500)
	*ChanParData = (double)fastthresh / ((double)FL * 5.0);

    }
  else if(strcmp(ChanParName,"ENERGY_RISETIME") == 0)
    {
      SL = Pixie_Devices[ModNum].DSP_Parameter_Values[SlowLength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      SlowFilterRange = Pixie_Devices[ModNum].DSP_Parameter_Values[SlowFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];
		
      // Update channel parameter EnergyRiseTime
      if(Module_Information[ModNum].Module_ADCMSPS == 100)
	*ChanParData = (double)SL * pow(2.0, (double)SlowFilterRange) / (double)Module_Information[ModNum].Module_ADCMSPS;
      else if(Module_Information[ModNum].Module_ADCMSPS == 250)
	*ChanParData = (double)SL * pow(2.0, (double)SlowFilterRange) / (double)(Module_Information[ModNum].Module_ADCMSPS / 2);
      else if(Module_Information[ModNum].Module_ADCMSPS == 500)
	*ChanParData = (double)SL * pow(2.0, (double)SlowFilterRange) / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
		
    }
  else if(strcmp(ChanParName,"ENERGY_FLATTOP") == 0)
    {
      SG = Pixie_Devices[ModNum].DSP_Parameter_Values[SlowGap_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];

      SlowFilterRange = Pixie_Devices[ModNum].DSP_Parameter_Values[SlowFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];
		
      // Update channel parameter EnergyFlatTop
      if(Module_Information[ModNum].Module_ADCMSPS == 100)
	*ChanParData = (double)SG * pow(2.0, (double)SlowFilterRange) / (double)Module_Information[ModNum].Module_ADCMSPS;
      else if(Module_Information[ModNum].Module_ADCMSPS == 250)
	*ChanParData = (double)SG * pow(2.0, (double)SlowFilterRange) / (double)(Module_Information[ModNum].Module_ADCMSPS / 2);
      else if(Module_Information[ModNum].Module_ADCMSPS == 500)
	*ChanParData = (double)SG * pow(2.0, (double)SlowFilterRange) / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);

    }
  else if(strcmp(ChanParName,"TAU") == 0)
    {
      preamptau = Pixie_Devices[ModNum].DSP_Parameter_Values[PreampTau_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      // Update channel parameter PreampTau
      *ChanParData = IEEEFloating2Decimal(preamptau);
    }
  else if(strcmp(ChanParName,"TRACE_LENGTH") == 0)
    {
      tracelength = Pixie_Devices[ModNum].DSP_Parameter_Values[TraceLength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];

      FastFilterRange = Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];
		
      // Update channel parameter TraceLength
      *ChanParData = (double)tracelength / (double)Module_Information[ModNum].Module_ADCMSPS * pow(2.0, (double)FastFilterRange);
    }
  else if(strcmp(ChanParName,"TRACE_DELAY") == 0)
    {
      paflength =Pixie_Devices[ModNum].DSP_Parameter_Values[PAFlength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];

      triggerdelay = Pixie_Devices[ModNum].DSP_Parameter_Values[TriggerDelay_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];

      FastFilterRange = Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];
		
      // Update channel parameter TraceDelay
      if(Module_Information[ModNum].Module_ADCMSPS == 100)
	*ChanParData = (double)(paflength - (unsigned int)((double)triggerdelay / pow(2.0, (double)FastFilterRange))) / (double)Module_Information[ModNum].Module_ADCMSPS * pow(2.0, (double)FastFilterRange);
      else if(Module_Information[ModNum].Module_ADCMSPS == 250)
	*ChanParData = (double)(paflength - (unsigned int)((double)triggerdelay / pow(2.0, (double)FastFilterRange))) / (double)(Module_Information[ModNum].Module_ADCMSPS / 2) * pow(2.0, (double)FastFilterRange);
      else if(Module_Information[ModNum].Module_ADCMSPS == 500)
	*ChanParData = (double)(paflength - (unsigned int)((double)triggerdelay / pow(2.0, (double)FastFilterRange))) / (double)(Module_Information[ModNum].Module_ADCMSPS / 5) * pow(2.0, (double)FastFilterRange);
		
    }
  else if(strcmp(ChanParName,"VOFFSET") == 0)
    {
      offsetdac = Pixie_Devices[ModNum].DSP_Parameter_Values[OffsetDAC_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      // Update channel parameter DCoffset
      *ChanParData = (double)offsetdac / 65536.0 * DAC_VOLTAGE_RANGE - DAC_VOLTAGE_RANGE / 2.0;	
    }
  else if(strcmp(ChanParName,"XDT") == 0)
    {
      xwait = Pixie_Devices[ModNum].DSP_Parameter_Values[Xwait_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      // Update channel parameter Xdt
      *ChanParData = (double)xwait / (double)DSP_CLOCK_MHZ;		
    }
  else if(strcmp(ChanParName,"BASELINE_PERCENT") == 0)
    {
      baselinepercent = Pixie_Devices[ModNum].DSP_Parameter_Values[BaselinePercent_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      // Update channel parameter BaselinePercent
      *ChanParData = (double)baselinepercent;
		
    }
  else if(strcmp(ChanParName,"EMIN") == 0)
    {
      energylow = Pixie_Devices[ModNum].DSP_Parameter_Values[EnergyLow_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      // Update channel parameter EnergyLow
      *ChanParData = (double)energylow;	
    }
  else if(strcmp(ChanParName,"BINFACTOR") == 0)
    {
      log2ebin = Pixie_Devices[ModNum].DSP_Parameter_Values[Log2Ebin_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      // Update channel parameter BinFactor
      *ChanParData = (double)(pow(2.0, 32.0) - log2ebin);	
    }
  else if(strcmp(ChanParName,"BASELINE_AVERAGE") == 0)
    {
      baselineaverage = Pixie_Devices[ModNum].DSP_Parameter_Values[Log2Bweight_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      // Update channel parameter Baseline Average
      if(baselineaverage == 0)
	{
	  *ChanParData = 0.0;
	}
      else
	{
	  *ChanParData = (double)(pow(2.0, 32.0) - baselineaverage);
	}
		
    }
  else if(strcmp(ChanParName,"CHANNEL_CSRA") == 0)
    {
      chancsra = Pixie_Devices[ModNum].DSP_Parameter_Values[ChanCSRa_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      // Update channel parameter ChanCSRA
      *ChanParData = (double)chancsra;	
    }
  else if(strcmp(ChanParName,"CHANNEL_CSRB") == 0)
    {
      chancsrb = Pixie_Devices[ModNum].DSP_Parameter_Values[ChanCSRb_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      // Update channel parameter ChanCSRB
      *ChanParData = (double)chancsrb;	
    }
  else if(strcmp(ChanParName,"BLCUT") == 0)
    {
      baselinecut = Pixie_Devices[ModNum].DSP_Parameter_Values[BLcut_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      // Update channel parameter BaselineCut
      *ChanParData = (double)baselinecut;	
    }
  else if(strcmp(ChanParName,"FASTTRIGBACKLEN") == 0)
    {
      fasttrigbacklen = Pixie_Devices[ModNum].DSP_Parameter_Values[FastTrigBackLen_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      // Update channel parameter FastTrigBackLen
      if(Module_Information[ModNum].Module_ADCMSPS == 100)
	*ChanParData = (double)fasttrigbacklen / (double)Module_Information[ModNum].Module_ADCMSPS;
      else if(Module_Information[ModNum].Module_ADCMSPS == 250)
	*ChanParData = (double)fasttrigbacklen / (double)(Module_Information[ModNum].Module_ADCMSPS / 2);
      else if(Module_Information[ModNum].Module_ADCMSPS == 500)
	*ChanParData = (double)fasttrigbacklen / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
		
    }
  else if(strcmp(ChanParName,"CFDDelay") == 0)
    {
      cfddelay = Pixie_Devices[ModNum].DSP_Parameter_Values[CFDDelay_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      // Update channel parameter CFDDelay
      if(Module_Information[ModNum].Module_ADCMSPS == 100)
	*ChanParData = (double)cfddelay / (double)Module_Information[ModNum].Module_ADCMSPS;
      else if(Module_Information[ModNum].Module_ADCMSPS == 250)
	*ChanParData = (double)cfddelay / (double)(Module_Information[ModNum].Module_ADCMSPS / 2);
      else if(Module_Information[ModNum].Module_ADCMSPS == 500)
	*ChanParData = (double)cfddelay / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
    }
  else if(strcmp(ChanParName,"CFDScale") == 0)
    {
      cfdscale = Pixie_Devices[ModNum].DSP_Parameter_Values[CFDScale_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      // Update channel parameter CFDScale
      *ChanParData = (double)cfdscale;
    }
  else if(strcmp(ChanParName,"CFDThresh") == 0)
    {
      cfdthresh = Pixie_Devices[ModNum].DSP_Parameter_Values[CFDThresh_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      // Update channel parameter CFDThresh
      *ChanParData = (double)cfdthresh;
    }
  else if(strcmp(ChanParName,"QDCLen0") == 0)
    {
      qdclen = Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen0_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      // Update channel parameter QDCLen0
      if(Module_Information[ModNum].Module_ADCMSPS == 500)
	*ChanParData = (double)qdclen / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
      else
	*ChanParData = (double)qdclen / (double)Module_Information[ModNum].Module_ADCMSPS;
    }
  else if(strcmp(ChanParName,"QDCLen1") == 0)
    {
      qdclen = Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen1_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      if(Module_Information[ModNum].Module_ADCMSPS == 500)
	*ChanParData = (double)qdclen / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
      else
	*ChanParData = (double)qdclen / (double)Module_Information[ModNum].Module_ADCMSPS;
    }
  else if(strcmp(ChanParName,"QDCLen2") == 0)
    {
      qdclen = Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen2_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      if(Module_Information[ModNum].Module_ADCMSPS == 500)
	*ChanParData = (double)qdclen / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
      else
	*ChanParData = (double)qdclen / (double)Module_Information[ModNum].Module_ADCMSPS;
    }
  else if(strcmp(ChanParName,"QDCLen3") == 0)
    {
      qdclen = Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen3_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      if(Module_Information[ModNum].Module_ADCMSPS == 500)
	*ChanParData = (double)qdclen / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
      else
	*ChanParData = (double)qdclen / (double)Module_Information[ModNum].Module_ADCMSPS;
    }
  else if(strcmp(ChanParName,"QDCLen4") == 0)
    {
      qdclen = Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen4_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      if(Module_Information[ModNum].Module_ADCMSPS == 500)
	*ChanParData = (double)qdclen / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
      else
	*ChanParData = (double)qdclen / (double)Module_Information[ModNum].Module_ADCMSPS;
    }
  else if(strcmp(ChanParName,"QDCLen5") == 0)
    {
      qdclen = Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen5_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      if(Module_Information[ModNum].Module_ADCMSPS == 500)
	*ChanParData = (double)qdclen / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
      else
	*ChanParData = (double)qdclen / (double)Module_Information[ModNum].Module_ADCMSPS;
    }
  else if(strcmp(ChanParName,"QDCLen6") == 0)
    {
      qdclen = Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen6_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      if(Module_Information[ModNum].Module_ADCMSPS == 500)
	*ChanParData = (double)qdclen / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
      else
	*ChanParData = (double)qdclen / (double)Module_Information[ModNum].Module_ADCMSPS;
    }
  else if(strcmp(ChanParName,"QDCLen7") == 0)
    {
      qdclen = Pixie_Devices[ModNum].DSP_Parameter_Values[QDCLen7_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      if(Module_Information[ModNum].Module_ADCMSPS == 500)
	*ChanParData = (double)qdclen / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
      else
	*ChanParData = (double)qdclen / (double)Module_Information[ModNum].Module_ADCMSPS;
    }
  else if(strcmp(ChanParName,"ExtTrigStretch") == 0)
    {
      exttrigstretch = Pixie_Devices[ModNum].DSP_Parameter_Values[ExtTrigStretch_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      // Update channel parameter ExtTrigStretch
      if(Module_Information[ModNum].Module_ADCMSPS == 100)
	*ChanParData = (double)exttrigstretch / (double)Module_Information[ModNum].Module_ADCMSPS;
      else if(Module_Information[ModNum].Module_ADCMSPS == 250)
	*ChanParData = (double)exttrigstretch / (double)(Module_Information[ModNum].Module_ADCMSPS / 2);
      else if(Module_Information[ModNum].Module_ADCMSPS == 500)
	*ChanParData = (double)exttrigstretch / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
    }
  else if(strcmp(ChanParName,"VetoStretch") == 0)
    {
      vetostretch = Pixie_Devices[ModNum].DSP_Parameter_Values[VetoStretch_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      // Update channel parameter VetoStretch
      if(Module_Information[ModNum].Module_ADCMSPS == 100)
	*ChanParData = (double)vetostretch / (double)Module_Information[ModNum].Module_ADCMSPS;
      else if(Module_Information[ModNum].Module_ADCMSPS == 250)
	*ChanParData = (double)vetostretch / (double)(Module_Information[ModNum].Module_ADCMSPS / 2);
      else if(Module_Information[ModNum].Module_ADCMSPS == 500)
	*ChanParData = (double)vetostretch / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
    }
  else if(strcmp(ChanParName,"MultiplicityMaskL") == 0)
    {
      multiplicitymaskl = Pixie_Devices[ModNum].DSP_Parameter_Values[MultiplicityMaskL_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      // Update channel parameter MultiplicityMaskL
      *ChanParData = (double)multiplicitymaskl;
    }
  else if(strcmp(ChanParName,"MultiplicityMaskH") == 0)
    {
      multiplicitymaskh = Pixie_Devices[ModNum].DSP_Parameter_Values[MultiplicityMaskH_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      // Update channel parameter MultiplicityMaskH
      *ChanParData = (double)multiplicitymaskh;
    }
  else if(strcmp(ChanParName,"ExternDelayLen") == 0)
    {
      externdelaylen = Pixie_Devices[ModNum].DSP_Parameter_Values[ExternDelayLen_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      // Update channel parameter ExternDelayLen
      if(Module_Information[ModNum].Module_ADCMSPS == 100)
	*ChanParData = (double)externdelaylen / (double)Module_Information[ModNum].Module_ADCMSPS;
      else if(Module_Information[ModNum].Module_ADCMSPS == 250)
	*ChanParData = (double)externdelaylen / (double)(Module_Information[ModNum].Module_ADCMSPS / 2);
      else if(Module_Information[ModNum].Module_ADCMSPS == 500)
	*ChanParData = (double)externdelaylen / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
    }
  else if(strcmp(ChanParName,"FtrigoutDelay") == 0)
    {
      ftrigoutdelay = Pixie_Devices[ModNum].DSP_Parameter_Values[FtrigoutDelay_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      // Update channel parameter FtrigoutDelay
      if(Module_Information[ModNum].Module_ADCMSPS == 100)
	*ChanParData = (double)ftrigoutdelay / (double)Module_Information[ModNum].Module_ADCMSPS;
      else if(Module_Information[ModNum].Module_ADCMSPS == 250)
	*ChanParData = (double)ftrigoutdelay / (double)(Module_Information[ModNum].Module_ADCMSPS / 2);
      else if(Module_Information[ModNum].Module_ADCMSPS == 500)
	*ChanParData = (double)ftrigoutdelay / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
    }
  else if(strcmp(ChanParName,"ChanTrigStretch") == 0)
    {
      chantrigstretch = Pixie_Devices[ModNum].DSP_Parameter_Values[ChanTrigStretch_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
		
      // Update channel parameter ChanTrigStretch
      if(Module_Information[ModNum].Module_ADCMSPS == 100)
	*ChanParData = (double)chantrigstretch / (double)Module_Information[ModNum].Module_ADCMSPS;
      else if(Module_Information[ModNum].Module_ADCMSPS == 250)
	*ChanParData = (double)chantrigstretch / (double)(Module_Information[ModNum].Module_ADCMSPS / 2);
      else if(Module_Information[ModNum].Module_ADCMSPS == 500)
	*ChanParData = (double)chantrigstretch / (double)(Module_Information[ModNum].Module_ADCMSPS / 5);
    }
  else
    {
      printf("*ERROR* (ReadSglChanPar): invalid channel parameter name %s", ChanParName);
      return(-3);
    }
	
  return(0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


// InitSystem:
// Return Value:
// 0 - Success
// -1 - Invalid total number of Pixie16 modules
// -2 - Null pointer *PXISlotMap
// -3 - Failed to initialize system
int algorithm::InitSystem(
			  unsigned short NumModules,    // total number of Pixie16 modules in the system
			  unsigned short OfflineMode )  // specify if the system is in offline mode
{
  int retval;    // return value
  unsigned short k;
	
  // Check the validity of total number of Pixie16 modules
  if( (NumModules < 1) || (NumModules > PRESET_MAX_MODULES) )  // At least one module is necessary
    {
      printf("*ERROR* (InitSystem): invalid total number (%d) of Pixie16 modules", NumModules);
      return(-1);
    }
  else
    {
      Number_Modules = NumModules;  // Number_Modules is a global variable
    }

  // Assign module variant if running in offline mode
  for(k = 0; k < Number_Modules; k ++)
    {
      Module_Information[k].Module_OfflineVariant = OfflineMode;
    }

		
  // Read and store module information
  for(k = 0; k < Number_Modules; k ++)
    {
      retval = ReadModuleInfo(k, &Module_Information[k].Module_Rev, &Module_Information[k].Module_SerNum, &Module_Information[k].Module_ADCBits, &Module_Information[k].Module_ADCMSPS);
      if(retval < 0)
	{
	  printf("*ERROR* (InitSystem): failed to read module information in module %d, retval=%d", k, retval);
	  return(-3);
	}
    }

  return(0);

}

// ReadModuleInfo:
// Read information stored on each module, including its revision, serial number, ADC characteristics.
// Return Value:
// 0 - Success
// -1 - Invalid Pixie module number
// -2 - Failed to read from I2C serial EEPROM

int algorithm::ReadModuleInfo(
			      unsigned short ModNum,			// module number
			      unsigned short *ModRev,			// returned module revision
			      unsigned int   *ModSerNum,		// returned module serial number
			      unsigned short *ModADCBits,		// returned module ADC bits
			      unsigned short *ModADCMSPS )	// returned module ADC sampling rate
{	
  // Check if ModNum is valid
  if( ModNum > Number_Modules )
    {
      printf("*ERROR* (ReadModuleInfo): invalid Pixie module number %d", ModNum);
      return(-1);
    }

  *ModRev = 0xF;
  *ModSerNum = 1000;	// dummy serial number

  if(Module_Information[ModNum].Module_OfflineVariant == 1)
    {
      *ModRev = 0xD;
      *ModSerNum = 275;	// dummy serial number
      *ModADCBits = 12;
      *ModADCMSPS = 100;
    }
  else if(Module_Information[ModNum].Module_OfflineVariant == 2)
    {
      *ModADCBits = 14;
      *ModADCMSPS = 100;
    }
  else if(Module_Information[ModNum].Module_OfflineVariant == 3)
    {
      *ModADCBits = 12;
      *ModADCMSPS = 250;
    }
  else if(Module_Information[ModNum].Module_OfflineVariant == 4)
    {
      *ModADCBits = 14;
      *ModADCMSPS = 250;
    }
  else if(Module_Information[ModNum].Module_OfflineVariant == 5)
    {
      *ModADCBits = 12;
      *ModADCMSPS = 500;
    }
  else if(Module_Information[ModNum].Module_OfflineVariant == 6)
    {
      *ModADCBits = 14;
      *ModADCMSPS = 500;
    }

  return(0);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


// ComputeFIFO:
// Update FIFO settings (TriggerDelay and PAFLength) when the following DSP parameters are changed: SlowLength, SlowGap, SlowFilterRange
// Return Value:
// 0 - success
int algorithm::ComputeFIFO(
		unsigned int   TraceDelay,	// current trace dealy value
		unsigned short ModNum,		// Pixie module number
		unsigned short ChanNum )	// Pixie channel number
{
	
  unsigned int PAFLength, TriggerDelay, FifoLength;
  unsigned int SlowFilterRange, FastFilterRange, PeakSep;
	
  // Get the DSP parameter SlowFilterRange
  SlowFilterRange = Pixie_Devices[ModNum].DSP_Parameter_Values[SlowFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];
  FastFilterRange = Pixie_Devices[ModNum].DSP_Parameter_Values[FastFilterRange_Address[ModNum] - DATA_MEMORY_ADDRESS];
	
  // Get the DSP parameter PeakSep
  PeakSep = Pixie_Devices[ModNum].DSP_Parameter_Values[PeakSep_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS];
	
  // Re-calculate TriggerDelay
  TriggerDelay = (unsigned int)(((double)PeakSep - 1.0) * pow(2.0, (double)SlowFilterRange));
	
  // Re-calculate PAFLength
  PAFLength = (unsigned int)((double)TriggerDelay / pow(2.0, (double)FastFilterRange))  + TraceDelay;
	
  // Get the current FIFO Length
  FifoLength = Pixie_Devices[ModNum].DSP_Parameter_Values[FIFOLength_Address[ModNum] - DATA_MEMORY_ADDRESS];
	
  if(PAFLength > FifoLength)  // PAFLength must be not larger than FifoLength
    {
      PAFLength = FifoLength - 1; // Keep TraceDelay while reducing TriggerDelay
      TriggerDelay = (PAFLength - TraceDelay) * (unsigned int)pow(2.0, (double)FastFilterRange);
    }
	
  // Update the DSP parameter TriggerDelay
  Pixie_Devices[ModNum].DSP_Parameter_Values[TriggerDelay_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = TriggerDelay;
	
  // Update the DSP parameter PAFLength
  Pixie_Devices[ModNum].DSP_Parameter_Values[PAFlength_Address[ModNum] + ChanNum - DATA_MEMORY_ADDRESS] = PAFLength;
	
  return(0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


// SaveDSPParametersToFile:
// Save DSP parameters to a settings file.
// Return Value:
// 0 - Success
// -1 - Failed to read DSP parameter values from the Pixie-16 modules
// -2 - Failed to open the DSP parameters file
int algorithm::SaveDSPParametersToFile(char  *FileName ) // the DSP parameters file name (with complete path)
{
  unsigned short ModNum;
  FILE *DSPSettingsFile = NULL;
  int retval;
	
  // Open the DSP parameters file
  DSPSettingsFile = fopen(FileName, "wb");
  if(DSPSettingsFile != NULL)
    {	
      // Write DSP parameter values to the settings file
      for(ModNum = 0; ModNum < PRESET_MAX_MODULES; ModNum ++)
	{
	  fwrite(Pixie_Devices[ModNum].DSP_Parameter_Values, sizeof(unsigned int), N_DSP_PAR, DSPSettingsFile);
	}
		
      // Close the file
      fclose(DSPSettingsFile);
		
      return(0);
    }
  else
    {
      printf("*ERROR* (SaveDSPParametersToFile): failed to open DSP parameters file %s", FileName);
      return(-2);
    }
}




// 
// algorithm.cc ends here
