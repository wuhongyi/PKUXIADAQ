// daq.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 12月  6 19:12:16 2018 (+0800)
// Last-Updated: 五 12月  7 19:20:00 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 21
// URL: http://wuhongyi.cn 

#include "../software/app/pixie16app_export.h"
#include "../software/app/pixie16app_export.h"

#include "wuReadData.hh"

#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <cmath>
#include <cstring>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include <vector>
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#define FIRMWARE100M12BIT
#define FIRMWARE100M14BIT
// #define FIRMWARE250M12BIT
#define FIRMWARE250M14BIT
// #define FIRMWARE250M16BIT
// #define FIRMWARE500M12BIT
// #define FIRMWARE500M14BIT

struct Module_Info
{
  // Module information
  unsigned short Module_Rev;
  unsigned int   Module_SerNum;
  unsigned short Module_ADCBits;
  unsigned short Module_ADCMSPS;
  unsigned short Module_OfflineVariant;
};


int main(int argc, char *argv[])
{
  char *config = (char*)"../parset/cfgPixie16.txt";
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  int retval = 0;
  
  unsigned short NumModules;
  unsigned short *PXISlotMap;

  std::vector<unsigned short> *moduleslot = NULL;
  std::vector<unsigned short> *modulesamplingrate = NULL;
  std::vector<unsigned short> *modulebits = NULL;

  PXISlotMap = new unsigned short[PRESET_MAX_MODULES];
  moduleslot = new std::vector<unsigned short>;
  modulesamplingrate = new std::vector<unsigned short>;
  modulebits = new std::vector<unsigned short>;

  
  std::string File100M12bit_sys;
  std::string File100M12bit_fip;
  std::string File100M12bit_dspldr;
  std::string File100M12bit_dsplst;
  std::string File100M12bit_dspvar;
  
  std::string File100M14bit_sys;//ComFPGAConfigFile
  std::string File100M14bit_fip;//SPFPGAConfigFile
  std::string File100M14bit_dsplst;//TrigFPGAConfigFile
  std::string File100M14bit_dspldr;//DSPCodeFile
  std::string File100M14bit_dspvar;//DSPVarFile

  std::string File250M12bit_sys;
  std::string File250M12bit_fip;
  std::string File250M12bit_dspldr;
  std::string File250M12bit_dsplst;
  std::string File250M12bit_dspvar;
  
  std::string File250M14bit_sys;
  std::string File250M14bit_fip;
  std::string File250M14bit_dspldr;
  std::string File250M14bit_dsplst;
  std::string File250M14bit_dspvar;

  std::string File250M16bit_sys;
  std::string File250M16bit_fip;
  std::string File250M16bit_dspldr;
  std::string File250M16bit_dsplst;
  std::string File250M16bit_dspvar;

  std::string File500M12bit_sys;
  std::string File500M12bit_fip;
  std::string File500M12bit_dspldr;
  std::string File500M12bit_dsplst;
  std::string File500M12bit_dspvar;
  
  std::string File500M14bit_sys;
  std::string File500M14bit_fip;
  std::string File500M14bit_dspldr;
  std::string File500M14bit_dsplst;
  std::string File500M14bit_dspvar;
  
  std::string FileSettingPars;//DSPParFile


  char ComFPGAConfigFile[1024];
  char SPFPGAConfigFile[1024];
  char TrigFPGAConfigFile[1024];
  char DSPCodeFile[1024];
  char DSPParFile[1024];
  char DSPVarFile[1024];

  struct Module_Info ModuleInformation[PRESET_MAX_MODULES];

  unsigned int nFIFOWords;
  unsigned short EndOfRunRead;
  char filnam[80];
  unsigned int mod_numwordsread = 0;
  int count;

  struct timeval tim;
  double t1 = 0, t2 = 0; 
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  unsigned int DataWords = wuReadData::ReadValue<unsigned int>("DataWords", "Pars.txt");
  int HPGeModule = wuReadData::ReadValue<int>("HPGeModule", "Pars.txt");
  int HPGeChannel = wuReadData::ReadValue<int>("HPGeChannel", "Pars.txt");
  int BGOModule = wuReadData::ReadValue<int>("BGOModule", "Pars.txt");  
  int BGOChannel = wuReadData::ReadValue<int>("BGOChannel", "Pars.txt");         
  std::vector<int> *BGOThreshold = NULL;
  BGOThreshold = new std::vector<int>;
  BGOThreshold->clear();
  wuReadData::ReadVector("BGOThreshold", "Pars.txt", BGOThreshold);


  moduleslot->clear();
  modulesamplingrate->clear();
  modulebits->clear();
  wuReadData::ReadVector("ModuleSlot", config, moduleslot);
  wuReadData::ReadVector("ModuleSampingRate", config, modulesamplingrate);
  wuReadData::ReadVector("ModuleBits", config, modulebits);

#ifdef FIRMWARE100M12BIT
  File100M12bit_sys = wuReadData::ReadValue<std::string>("100M12bit_sys", config);
  File100M12bit_fip = wuReadData::ReadValue<std::string>("100M12bit_fip", config);
  File100M12bit_dspldr = wuReadData::ReadValue<std::string>("100M12bit_dspldr", config);
  File100M12bit_dsplst = wuReadData::ReadValue<std::string>("100M12bit_dsplst", config);
  File100M12bit_dspvar = wuReadData::ReadValue<std::string>("100M12bit_dspvar", config);   
#endif
  
#ifdef FIRMWARE100M14BIT
  File100M14bit_sys = wuReadData::ReadValue<std::string>("100M14bit_sys", config);
  File100M14bit_fip = wuReadData::ReadValue<std::string>("100M14bit_fip", config);
  File100M14bit_dspldr = wuReadData::ReadValue<std::string>("100M14bit_dspldr", config);
  File100M14bit_dsplst = wuReadData::ReadValue<std::string>("100M14bit_dsplst", config);
  File100M14bit_dspvar = wuReadData::ReadValue<std::string>("100M14bit_dspvar", config);  
#endif
  
#ifdef FIRMWARE250M12BIT
  File250M12bit_sys = wuReadData::ReadValue<std::string>("250M12bit_sys", config);
  File250M12bit_fip = wuReadData::ReadValue<std::string>("250M12bit_fip", config);
  File250M12bit_dspldr = wuReadData::ReadValue<std::string>("250M12bit_dspldr", config);
  File250M12bit_dsplst = wuReadData::ReadValue<std::string>("250M12bit_dsplst", config);
  File250M12bit_dspvar = wuReadData::ReadValue<std::string>("250M12bit_dspvar", config);   
#endif
  
#ifdef FIRMWARE250M14BIT
  File250M14bit_sys = wuReadData::ReadValue<std::string>("250M14bit_sys", config);
  File250M14bit_fip = wuReadData::ReadValue<std::string>("250M14bit_fip", config);
  File250M14bit_dspldr = wuReadData::ReadValue<std::string>("250M14bit_dspldr", config);
  File250M14bit_dsplst = wuReadData::ReadValue<std::string>("250M14bit_dsplst", config);
  File250M14bit_dspvar = wuReadData::ReadValue<std::string>("250M14bit_dspvar", config);  
#endif

#ifdef FIRMWARE250M16BIT
  File250M16bit_sys = wuReadData::ReadValue<std::string>("250M16bit_sys", config);
  File250M16bit_fip = wuReadData::ReadValue<std::string>("250M16bit_fip", config);
  File250M16bit_dspldr = wuReadData::ReadValue<std::string>("250M16bit_dspldr", config);
  File250M16bit_dsplst = wuReadData::ReadValue<std::string>("250M16bit_dsplst", config);
  File250M16bit_dspvar = wuReadData::ReadValue<std::string>("250M16bit_dspvar", config);  
#endif
  
#ifdef FIRMWARE500M12BIT
  File500M12bit_sys = wuReadData::ReadValue<std::string>("500M12bit_sys", config);
  File500M12bit_fip = wuReadData::ReadValue<std::string>("500M12bit_fip", config);
  File500M12bit_dspldr = wuReadData::ReadValue<std::string>("500M12bit_dspldr", config);
  File500M12bit_dsplst = wuReadData::ReadValue<std::string>("500M12bit_dsplst", config);
  File500M12bit_dspvar = wuReadData::ReadValue<std::string>("500M12bit_dspvar", config);   
#endif
  
#ifdef FIRMWARE500M14BIT
  File500M14bit_sys = wuReadData::ReadValue<std::string>("500M14bit_sys", config);
  File500M14bit_fip = wuReadData::ReadValue<std::string>("500M14bit_fip", config);
  File500M14bit_dspldr = wuReadData::ReadValue<std::string>("500M14bit_dspldr", config);
  File500M14bit_dsplst = wuReadData::ReadValue<std::string>("500M14bit_dsplst", config);
  File500M14bit_dspvar = wuReadData::ReadValue<std::string>("500M14bit_dspvar", config);  
#endif

  
  FileSettingPars = wuReadData::ReadValue<std::string>("SettingPars", config);
  
  // std::cout<<File100M14bit_sys<<std::endl;
  // std::cout<<File100M14bit_fip<<std::endl;
  // std::cout<<File100M14bit_dspldr<<std::endl;
  // std::cout<<File100M14bit_dsplst<<std::endl;
  // std::cout<<File100M14bit_dspvar<<std::endl;
  // std::cout<<File250M14bit_sys<<std::endl;
  // std::cout<<File250M14bit_fip<<std::endl;
  // std::cout<<File250M14bit_dspldr<<std::endl;
  // std::cout<<File250M14bit_dsplst<<std::endl;
  // std::cout<<File250M14bit_dspvar<<std::endl;
  // std::cout<<FileSettingPars<<std::endl;


  NumModules = moduleslot->size();
  std::cout<<"---------- Init System Mode: Online ----------"<<std::endl;
  std::cout<<"Module Number: "<< NumModules<<std::endl;
  for (unsigned int i = 0; i < moduleslot->size(); ++i)
    {
      std::cout<<"Module "<<i<<" in slot "<<moduleslot->at(i)<<std::endl;
      PXISlotMap[i] = moduleslot->at(i);
    }


  retval = Pixie16InitSystem(NumModules, PXISlotMap, 0);
  if (retval < 0)
    {
      std::cout << "PCI Pixie init has failed: " << retval << std::endl;
      return -1;
    }
  else
    {
      cout<<"Init OK "<<endl;
    }


  for(unsigned short k = 0; k < NumModules; k++)
    {
      retval = Pixie16ReadModuleInfo(k, &ModuleInformation[k].Module_Rev, &ModuleInformation[k].Module_SerNum, &ModuleInformation[k].Module_ADCBits, &ModuleInformation[k].Module_ADCMSPS);
      if(retval != 0)
	{
	  std::cout<<"Pixie16ReadModuleInfo: failed to read module information in module " <<k<<", retval="<<retval<<std::endl;
	  return -1;
	}
      else
	{
	  std::cout<<"Rev:"<<ModuleInformation[k].Module_Rev<<"  SerNum:"<<ModuleInformation[k].Module_SerNum<<"  ADCBits:"<<ModuleInformation[k].Module_ADCBits<<"  ADCMSPS:"<<ModuleInformation[k].Module_ADCMSPS<<std::endl;
	}
    }  


  for(unsigned short k = 0; k < NumModules; k++)
    {
      // 旧版固件0x7F 启动有问题，11032016新版没问题
      strcpy(DSPParFile, FileSettingPars.c_str());

      if(ModuleInformation[k].Module_ADCMSPS == 100 && ModuleInformation[k].Module_ADCBits == 12)
	{
#ifdef FIRMWARE100M12BIT
  	  strcpy(ComFPGAConfigFile, File100M12bit_sys.c_str());
  	  strcpy(SPFPGAConfigFile, File100M12bit_fip.c_str());
  	  strcpy(TrigFPGAConfigFile, File100M12bit_dsplst.c_str());
  	  strcpy(DSPCodeFile, File100M12bit_dspldr.c_str());
  	  strcpy(DSPVarFile, File100M12bit_dspvar.c_str());
#else
	  std::cout<<"No 100M-12bit firmware was found!!!"<<std::endl;
	  return -1;
#endif	  
	}
      else if(ModuleInformation[k].Module_ADCMSPS == 100 && ModuleInformation[k].Module_ADCBits == 14)
  	{
#ifdef FIRMWARE100M14BIT
  	  strcpy(ComFPGAConfigFile, File100M14bit_sys.c_str());
  	  strcpy(SPFPGAConfigFile, File100M14bit_fip.c_str());
  	  strcpy(TrigFPGAConfigFile, File100M14bit_dsplst.c_str());
  	  strcpy(DSPCodeFile, File100M14bit_dspldr.c_str());
  	  strcpy(DSPVarFile, File100M14bit_dspvar.c_str());
#else
	  std::cout<<"No 100M-14bit firmware was found!!!"<<std::endl;
	  return -1;
#endif	  
  	}
      else if(ModuleInformation[k].Module_ADCMSPS == 250 && ModuleInformation[k].Module_ADCBits == 12)
	{
#ifdef FIRMWARE250M12BIT
  	  strcpy(ComFPGAConfigFile, File250M12bit_sys.c_str());
  	  strcpy(SPFPGAConfigFile, File250M12bit_fip.c_str());
  	  strcpy(TrigFPGAConfigFile, File250M12bit_dsplst.c_str());
  	  strcpy(DSPCodeFile, File250M12bit_dspldr.c_str());
  	  strcpy(DSPVarFile, File250M12bit_dspvar.c_str());
#else
	  std::cout<<"No 250M-14bit firmware was found!!!"<<std::endl;
	  return -1;
#endif	
	}
      else if(ModuleInformation[k].Module_ADCMSPS == 250 && ModuleInformation[k].Module_ADCBits == 14)
  	{
#ifdef FIRMWARE250M14BIT
  	  strcpy(ComFPGAConfigFile, File250M14bit_sys.c_str());
  	  strcpy(SPFPGAConfigFile, File250M14bit_fip.c_str());
  	  strcpy(TrigFPGAConfigFile, File250M14bit_dsplst.c_str());
  	  strcpy(DSPCodeFile, File250M14bit_dspldr.c_str());
  	  strcpy(DSPVarFile, File250M14bit_dspvar.c_str());
#else
	  std::cout<<"No 250M-14bit firmware was found!!!"<<std::endl;
	  return -1;
#endif	
  	}
      else if(ModuleInformation[k].Module_ADCMSPS == 250 && ModuleInformation[k].Module_ADCBits == 16)
  	{
#ifdef FIRMWARE250M16BIT
  	  strcpy(ComFPGAConfigFile, File250M16bit_sys.c_str());
  	  strcpy(SPFPGAConfigFile, File250M16bit_fip.c_str());
  	  strcpy(TrigFPGAConfigFile, File250M16bit_dsplst.c_str());
  	  strcpy(DSPCodeFile, File250M16bit_dspldr.c_str());
  	  strcpy(DSPVarFile, File250M16bit_dspvar.c_str());
#else
	  std::cout<<"No 250M-16bit firmware was found!!!"<<std::endl;
	  return -1;
#endif	
  	}
      else if(ModuleInformation[k].Module_ADCMSPS == 500 && ModuleInformation[k].Module_ADCBits == 12)
	{
#ifdef FIRMWARE500M12BIT
  	  strcpy(ComFPGAConfigFile, File500M12bit_sys.c_str());
  	  strcpy(SPFPGAConfigFile, File500M12bit_fip.c_str());
  	  strcpy(TrigFPGAConfigFile, File500M12bit_dsplst.c_str());
  	  strcpy(DSPCodeFile, File500M12bit_dspldr.c_str());
  	  strcpy(DSPVarFile, File500M12bit_dspvar.c_str());
#else
	  std::cout<<"No 500M-12bit firmware was found!!!"<<std::endl;
	  return -1;
#endif	  
	}
      else if(ModuleInformation[k].Module_ADCMSPS == 500 && ModuleInformation[k].Module_ADCBits == 14)
  	{
#ifdef FIRMWARE500M14BIT
  	  strcpy(ComFPGAConfigFile, File500M14bit_sys.c_str());
  	  strcpy(SPFPGAConfigFile, File500M14bit_fip.c_str());
  	  strcpy(TrigFPGAConfigFile, File500M14bit_dsplst.c_str());
  	  strcpy(DSPCodeFile, File500M14bit_dspldr.c_str());
  	  strcpy(DSPVarFile, File500M14bit_dspvar.c_str());
#else
	  std::cout<<"No 500M-14bit firmware was found!!!"<<std::endl;
	  return -1;
#endif	  
  	}
      else
  	{
  	  std::cout<<"There is no Samping Rate =>"<<ModuleInformation[k].Module_ADCMSPS<<" and Bits =>"<<ModuleInformation[k].Module_ADCBits<<" firmware. "<<std::endl;
  	  return -1;
  	}
      std::cout<<"----------------------------------------"<<std::endl;
      std::cout<<ComFPGAConfigFile<<std::endl;
      std::cout<<SPFPGAConfigFile<<std::endl;
      std::cout<<TrigFPGAConfigFile<<std::endl;
      std::cout<<DSPCodeFile<<std::endl;
      std::cout<<DSPParFile<<std::endl;
      std::cout<<DSPVarFile<<std::endl;

      retval = Pixie16BootModule(ComFPGAConfigFile,	// name of communications FPGA configuration file
  				 SPFPGAConfigFile,	// name of signal processing FPGA configuration file
  				 TrigFPGAConfigFile,	// name of trigger FPGA configuration file
  				 DSPCodeFile,	// name of executable code file for digital signal processor (DSP)
  				 DSPParFile,	// name of DSP parameter file
  				 DSPVarFile,	// name of DSP variable names file
  				 k,	// pixie module number
  				 0x7F);	// boot pattern bit mask
      
      if (retval != 0)
  	{
  	  std::cout << "cards booting has failed !  Module NUMBER "<< k <<std::endl;;
  	  return -1;
  	}
    }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  // Adjust DC-Offsets
  for(unsigned short k = 0; k < NumModules; k++)
    {		
      retval = Pixie16AdjustOffsets(k);
      if (retval < 0)
	{
	  std::cout<<"Pixie16AdjustOffsets in module "<<k<<" failed, retval = "<<retval<<std::endl;
	  return -1;
	}

      unsigned int blcut;
      for (unsigned short kk = 0; kk < 16; ++kk)
	{
	  retval = Pixie16BLcutFinder(k,kk,&blcut);
	  if(retval < 0)
	    {
	      return -1;
	    }
	}
    }

  std::cout<<"----------------------------------------"<<std::endl;

  for(unsigned short k = 0; k < NumModules; k++)
    {
      unsigned int CSR;
      retval = Pixie16ReadCSR(k, &CSR);
      if(retval != 0)
	{
	  std::cout<<"Pixie16ReadCSR: Invalid Pixie module number in module "<<k<<", retval="<<retval<<std::endl;
	}
      else
	{
	  std::cout<<"host Control & Status Register =>  mod:"<<k<<"  value:0x"<<hex<<CSR<<dec<<std::endl;
	}
    }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  for (unsigned int iii = 0; iii < BGOThreshold->size(); ++iii)
    {
      gettimeofday (&tim, NULL);
      t1 = tim.tv_sec + (tim.tv_usec / 1000000.0);
      std::cout<<"========================================"<<std::endl;
      std::cout<<iii<<"/"<<BGOThreshold->size()<<"     BGO Threshold: "<<BGOThreshold->at(iii)<<std::endl;

      double ChanParData = -1;
      for(int k=0; k<NumModules; k++)
	{
	  for (int i = 0; i < 16; ++i)
	    {
	      retval = Pixie16ReadSglChanPar((char*)"CHANNEL_CSRA", &ChanParData, k, i);
	      if(retval < 0) std::cout<<"Pixie16ReadSglChanPar/CHANNEL_CSRA failed."<<std::endl;
	      ChanParData = APP32_ClrBit(2,ChanParData);
	      retval = Pixie16WriteSglChanPar((char*)"CHANNEL_CSRA", ChanParData, k, i);
	      if(retval < 0) std::cout<<"Pixie16WriteSglChanPar/CHANNEL_CSRA failed."<<std::endl;
	    }
	}
      retval = Pixie16ReadSglChanPar((char*)"CHANNEL_CSRA", &ChanParData, HPGeModule, HPGeChannel);
      if(retval < 0) std::cout<<"Pixie16ReadSglChanPar/CHANNEL_CSRA failed."<<std::endl;
      ChanParData = APP32_SetBit(2,ChanParData);
      retval = Pixie16WriteSglChanPar((char*)"CHANNEL_CSRA", ChanParData, HPGeModule, HPGeChannel);
      if(retval < 0) std::cout<<"Pixie16WriteSglChanPar/CHANNEL_CSRA failed."<<std::endl;

      
      double thresh = BGOThreshold->at(iii);
      retval = Pixie16WriteSglChanPar((char*)"TRIGGER_THRESHOLD", thresh, BGOModule, BGOChannel);
      if(retval < 0) std::cout<<"Pixie16WriteSglChanPar/TRIGGER_THRESHOLD failed."<<std::endl;

      
      ///////////////////////////////////////////////////
      // Synchronize modules
      int modnum = 0;
      retval = Pixie16WriteSglModPar((char*)"SYNCH_WAIT", 1, modnum);
      if(retval<0)
	std::cout<<"Synch Wait problem "<<retval<<std::endl;
      else
	std::cout<<"Synch Wait OK "<<std::endl;

      retval = Pixie16WriteSglModPar((char*)"IN_SYNCH", 0, modnum);
      if(retval<0)
	std::cout<<"In Sync problem "<<retval<<std::endl;
      else
	std::cout<<"In Synch OK "<<std::endl;
      //////////////////////////////


      retval = Pixie16StartListModeRun(NumModules, 0x100, NEW_RUN);
      if (retval < 0)
	{
	  std::cout<<"Pixie16StartListModeRun failed in module , retval="<<retval<<std::endl;
	  return -3;
	}
      else
	std::cout<<"List Mode started OK "<<std::endl<<std::flush;

      usleep(100000); //delay for the DSP boot

      // Clear counters to 0 (counters keep track of how many words each module has read)
      nFIFOWords = 0;


      // Acquiring data: here we terminate the loop when the Director module accumulates more than a given number of words in its external FIFO.	
      while(1)
	{
	  for(int k=0; k<NumModules; k++)
	    {
	      EndOfRunRead = 0;
	      sprintf(filnam, "data/HPGe_%02d_%02d_BGO_%02d_%02d_%04d.bin",HPGeModule,HPGeChannel,BGOModule,BGOChannel,BGOThreshold->at(iii));

	      retval = Pixie16SaveExternalFIFODataToFile(filnam, &mod_numwordsread, k, EndOfRunRead);
	      if(retval<0)
		{
		  printf("Pixie16SaveExternalFIFODataToFile failed in module %d, retval = %d\n", k, retval);
		  return -5;
		}
	      nFIFOWords += mod_numwordsread;
	    }
	  if(nFIFOWords >= DataWords)
	    {
	      cout<<"Number of events reached - ending run"<<endl<<flush;
	      break;
	    }
		
	  // Check the run status of the Director module (module #0) to see if the run has been stopped.
	  // This is possible in a multi-chassis system where modules in one chassis can stop the run
	  // in all chassis.
	  retval = Pixie16CheckRunStatus(0);
	  if (retval == 0)
	    {
	      cout<<"Run was stopped but number of events are not reached yet"<<endl<<flush;
	      break;
	    }
	}

      // Stop run in the Director module (module #0) - a SYNC interrupt should be generated
      // to stop run in all modules simultaneously
      retval = Pixie16EndRun(0);
      

      // Make sure all modules indeed finish their run successfully.
      for(int k=0; k<NumModules; k++)
	{
	  count = 0;
	  do
	    {
	      retval = Pixie16CheckRunStatus(k);
	      if (retval != 0)
		{
		  EndOfRunRead = 1;
		  sprintf(filnam, "data/HPGe_%02d_%02d_BGO_%02d_%02d_%04d.bin",HPGeModule,HPGeChannel,BGOModule,BGOChannel,BGOThreshold->at(iii));
		  retval = Pixie16SaveExternalFIFODataToFile(filnam, &mod_numwordsread, k, EndOfRunRead);
		  if(retval<0)
		    {
		      printf("Pixie16SaveExternalFIFODataToFile failed in module %d, retval = %d\n", k, retval);
		      return -5;
		    }
		  nFIFOWords += mod_numwordsread;
		}
	      else
		{
		  break;
		}

	      count ++;
	    } while(count < 10);
	  if(count == 10)
	    {
	      cout<<"end run in module "<<k<<" failed"<<endl<<flush;
	    }
	}

      // All modules have their run stopped successfully. Now read out the possible last words from the external FIFO
      for(int k=0; k<NumModules; k++)
	{
	  EndOfRunRead = 1;
	  sprintf(filnam, "data/HPGe_%02d_%02d_BGO_%02d_%02d_%04d.bin",HPGeModule,HPGeChannel,BGOModule,BGOChannel,BGOThreshold->at(iii));
	  retval = Pixie16SaveExternalFIFODataToFile(filnam, &mod_numwordsread, k, EndOfRunRead);
	  if(retval<0)
	    {
	      printf("Pixie16SaveExternalFIFODataToFile failed in module %d, retval = %d", k, retval);
	      return -5;
	    }
	  nFIFOWords += mod_numwordsread;
	}

      cout<<"Run ended \n"<<flush;
      
      gettimeofday (&tim, NULL);
      t2 = tim.tv_sec + (tim.tv_usec / 1000000.0);

      cout<<"run time: "<<t2-t1<<endl;
    }//Loop for number of BGO Threshold


  if(moduleslot) delete moduleslot;
  if(modulesamplingrate) delete modulesamplingrate;
  if(modulebits) delete modulebits;
  return 0;
}

// 
// daq.cc ends here
