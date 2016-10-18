// algorithm.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 7月 22 21:08:18 2016 (+0800)
// Last-Updated: 六 7月 23 20:44:51 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 27
// URL: http://wuhongyi.github.io 

#include "algorithm.hh"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

algorithm::algorithm()
{
  rawdata = NULL;
  threshdata = NULL;
  cfddata = NULL;
  sfilterdata = NULL;
  ffilterdata = NULL;
  RcdTrace = NULL;
  doublethresh = NULL;
  doublesample = NULL;
  doublercdtrace = NULL;
  doublefastfilter = NULL;
  doublecfd = NULL;
  doubleslowfilter = NULL;
  adjustCanvas = new TCanvas();
  offlinemultigraph = new TMultiGraph();
}

algorithm::~algorithm()
{

}

void algorithm::SetTTree(TTree *tree)
{
  t = tree;

  t->SetBranchAddress("ch", &ch, &b_ch);
  t->SetBranchAddress("sid", &sid, &b_sid);
  t->SetBranchAddress("cid", &cid, &b_cid);
  t->SetBranchAddress("pileup", &pileup, &b_pileup);
  t->SetBranchAddress("ts", &ts, &b_ts);
  t->SetBranchAddress("cfd", &cfd, &b_cfd);
  t->SetBranchAddress("evte", &evte, &b_evte);
  t->SetBranchAddress("ltra", &ltra, &b_ltra);
  t->SetBranchAddress("trae", &trae, &b_trae);
  t->SetBranchAddress("leae", &leae, &b_leae);
  t->SetBranchAddress("gape", &gape, &b_gape);
  t->SetBranchAddress("base", &base, &b_base);
  t->SetBranchAddress("qs", qs, &b_qs);
  t->SetBranchAddress("data", data, &b_data);
  t->SetBranchAddress("dt", dt, &b_dt);
  t->SetBranchAddress("nevt", &nevt, &b_nevt);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

bool algorithm::DrawEntry(Long64_t entry)
{
  // b_sid->GetEntry(entry);
  // b_ch->GetEntry(entry);
  // b_ltra->GetEntry(entry);
  // b_data->GetEntry(entry);
  t->GetEntry(entry);
  // b_dt->GetEntry(entry);

  if(rawdata != NULL)
    {
      delete rawdata;
      rawdata = NULL;
    }
  if(threshdata != NULL)
    {
      delete threshdata;
      threshdata = NULL;
    }
  if(cfddata != NULL)
    {
      delete cfddata;
      cfddata = NULL;
    }
  if(sfilterdata != NULL)
    {
      delete sfilterdata;
      sfilterdata  = NULL;
    }
  if(ffilterdata != NULL)
    {
      delete ffilterdata;
      ffilterdata = NULL;
    }
  if(RcdTrace != NULL)
    {
      delete []RcdTrace;
      RcdTrace = NULL;
    }
  if(doublethresh != NULL)
    {
      delete []doublethresh;
      doublethresh = NULL;
    }
  if(doublesample != NULL)
    {
      delete []doublesample;
      doublesample = NULL;
    }	      
  if(doublercdtrace != NULL)
    {
      delete []doublercdtrace;
      doublercdtrace = NULL;
    }
  if(doublefastfilter != NULL)
    {
      delete []doublefastfilter;
      doublefastfilter = NULL;
    }
  if(doublecfd != NULL)
    {
      delete []doublecfd;
      doublecfd = NULL;
    }
  if(doubleslowfilter != NULL)
    {
      delete []doubleslowfilter;
      doubleslowfilter = NULL;
    }

  RcdTrace = new unsigned short[ltra];
  doublethresh = new double[ltra];
  doublesample = new double[ltra];
  doublercdtrace = new double[ltra];
  doublefastfilter = new double[ltra];
  doublecfd = new double[ltra];
  doubleslowfilter = new double[ltra];	      

  for (int i = 0; i < ltra; ++i)
    {
      RcdTrace[i] = data[i];
    }
	      
  ComputeFastFiltersOffline((unsigned short)(sid-2), (unsigned short) ch,(unsigned short) ltra, RcdTrace, doublefastfilter, doublecfd );
  ComputeSlowFiltersOffline((unsigned short)(sid-2), (unsigned short) ch, (unsigned short)ltra, RcdTrace,doubleslowfilter );

  for (int i = 0; i < ltra; ++i)
    {
      doublesample[i] = i;
      doublercdtrace[i] = (double)data[i];
      doublethresh[i] = (double)Pixie_Devices[sid-2].DSP_Parameter_Values[FastThresh_Address[sid-2] + ch - DATA_MEMORY_ADDRESS]/(double)Pixie_Devices[sid-2].DSP_Parameter_Values[FastLength_Address[sid-2] + ch - DATA_MEMORY_ADDRESS];// wuhongyi
    }

  rawdata = new TGraph(ltra,doublesample,doublercdtrace);
  threshdata = new TGraph(ltra,doublesample,doublethresh);
  cfddata = new TGraph(ltra,doublesample,doublecfd);
  sfilterdata = new TGraph(ltra,doublesample,doubleslowfilter);
  ffilterdata  = new TGraph(ltra,doublesample,doublefastfilter);

  adjustCanvas->cd();
  adjustCanvas->Clear();
  cfddata->SetLineColor(2);
  sfilterdata->SetLineColor(3);
  ffilterdata->SetLineColor(4);
  threshdata->SetLineColor(5);
  // rawdata->SetLineWidth(3);
  // cfddata->SetLineWidth(3);
  // sfilterdata->SetLineWidth(3);
  // cfddata->SetLineWidth(3);
  offlinemultigraph->Clear();
  offlinemultigraph->Add(rawdata);
  offlinemultigraph->Add(threshdata);
  offlinemultigraph->Add(cfddata);
  offlinemultigraph->Add(sfilterdata);
  offlinemultigraph->Add(ffilterdata);
  offlinemultigraph->Draw("AL");
  adjustCanvas->Modified();
  adjustCanvas->Update();
  // gSystem->ProcessEvents();	 
	      
  return true; 
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
  // deltaT = 1.0/((double)Module_Information[ModuleNumber].Module_ADCMSPS);
  deltaT = 1.0/((double)100);// wuhongyi
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


// 
// algorithm.cc ends here
