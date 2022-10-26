// DeviceHandle.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 三 6月 22 09:55:17 2022 (+0800)
// Last-Updated: 三 10月 26 22:19:36 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 44
// URL: http://wuhongyi.cn 

#include "DeviceHandle.hh"
#include <iostream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DeviceHandle::DeviceHandle()
{
  crateid= 0;
  runstatus = false;

  mNumberOfDevice = 0;
  slot_def.clear();

}

DeviceHandle::~DeviceHandle()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

unsigned short DeviceHandle::GetNumberOfDevice()
{
  return static_cast<unsigned short>(slot_def.size());
}

// void DeviceHandle::SetNumberOfDevice(int num)
// {
//   mNumberOfDevice = num;
// }

void DeviceHandle::StartAcquisition()
{
  // 时间同步，获取启动

  int retval = 0;
  // All modules start acuqire and Stop acquire simultaneously
  // retval = Pixie16WriteSglModPar((char*)"SYNCH_WAIT", 1, 0);
  // if (retval < 0)
  //   {
  //     ErrorInfo("Detector.cc", "StartRun(...)", "Pixie16WriteSglModPar/SYNCH_WAIT", retval);
  //     fprintf(stderr, "Failed to write SYNCH_WAIT\n");
  //     return retval;
  //   }

  // Reset clock counters to 0
  // retval = Pixie16WriteSglModPar((char*)"IN_SYNCH", 0, 0);
  // if (retval < 0)
  //   {
  //     ErrorInfo("Detector.cc", "StartRun(...)", "Pixie16WriteSglModPar/IN_SYNCH", retval);
  //     fprintf(stderr, "In Sync problem\n");
  //     return retval;
  //   }

  // retval = Pixie16StartListModeRun(NumModules,0x100, NEW_RUN);
  // if (retval < 0)
  //   {
  //     ErrorInfo("Detector.cc", "StartRun(...)", "Pixie16StartListModeRun", retval);
  //     fprintf(stderr, "Failed to start ListMode run in module");
  //     return retval;
  //   }

  //usleep(100000); //delay for the DSP boot 
  // sleep(2);// wait 2 seconds for modules to be ready
  // start a new run, not resume
  
  
  runstatus = true;
}

void DeviceHandle::StopAcquisition()  
{
  //获取结束
  std::cout << "done!!!!!!" << std::endl;
  int counter = 0;

  std::cout << "STOP RUN!" << std::endl;
  // unsigned short ModNum = 0;
  // int retval = Pixie16EndRun(ModNum);
  // if(retval < 0) {
  //   ErrorInfo("Detector.cc", "StopRun(...)", "Pixie16EndRun", retval);
  //   cout<<"FAILED TO END THE RUN!!!"<<endl;
  //   return 1;
  // }
  

  
  runstatus = false;
}

bool DeviceHandle::GetAcqStatus()
{
  // int sum = 0;
  
  // for(int i = 0;i < NumModules;i++)
  //   {
  //     int ret = 0;
  //     ret = Pixie16CheckRunStatus(i);
  //     cout<<"Run status: "<<i <<" sta:"<<ret<<endl;
  //     sum = sum + ret;
  //   }
  // return sum;


  
  return runstatus;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DeviceHandle::ClearDevice()
{
  slot_def.clear();
}


void DeviceHandle::SetCrateID(unsigned short cid)
{
  crateid = cid;
}

void DeviceHandle::AddDevice(unsigned short slot)
{
  slot_def.push_back(slot);
}

bool DeviceHandle::InitSystem()
{
  std::cout << "........................" << std::endl;
  
  int offline_mode = 0;

  std::cout<< GetNumberOfDevice() << std::endl;
  for (unsigned short i = 0; i < GetNumberOfDevice(); ++i)
    {
      std::cout<< i << " " << slot_def[i] << std::endl;
      slotmap[i] = slot_def[i];
    }

  int retval = 0;
  // retval  =  Pixie16InitSystem(GetNumberOfDevice(), slotmap, offline_mode);
  if(retval != 0)
    {
      std::cout << "Pixie16InitSystem: PCI Pixie init has failed: " << retval << std::endl;
      return false;
    }
  return true;
}

bool DeviceHandle::ReadModuleInfo()
{
  int retval = 0;
  for (unsigned short i = 0; i < GetNumberOfDevice(); ++i)
    {
      // TEST
      ModuleInformation[i].Module_Rev = 15;
      ModuleInformation[i].Module_SerNum = 1234;
      ModuleInformation[i].Module_ADCBits = 100;
      ModuleInformation[i].Module_ADCMSPS = 14;
      
      // retval = Pixie16ReadModuleInfo(i, &ModuleInformation[i].Module_Rev, &ModuleInformation[i].Module_SerNum, &ModuleInformation[i].Module_ADCBits, &ModuleInformation[i].Module_ADCMSPS);
      if(retval != 0)
	{
	   std::cout << "Pixie16ReadModuleInfo: failed to read module information in module " << i << ", retval=" << retval <<std::endl;
      
	  return false;
	}
      else
	{
	  std::cout << "Rev:" << ModuleInformation[i].Module_Rev<< "  SerNum:" << ModuleInformation[i].Module_SerNum << "  ADCBits:" << ModuleInformation[i].Module_ADCBits << "  ADCMSPS:" << ModuleInformation[i].Module_ADCMSPS << std::endl;

	}
    }
  return true;
}

bool DeviceHandle::BootModule()
{
  int retval = 0;
  // for (unsigned short i = 0; i < GetNumberOfDevice(); ++i)
  //   {
  //     retval = Pixie16BootModule(ComFPGAConfigFile,	// name of communications FPGA configuration file
  // 				 SPFPGAConfigFile,	// name of signal processing FPGA configuration file
  // 				 TrigFPGAConfigFile,	// name of trigger FPGA configuration file
  // 				 DSPCodeFile,	// name of executable code file for digital signal processor (DSP)
  // 				 DSPParFile,	// name of DSP parameter file
  // 				 DSPVarFile,	// name of DSP variable names file
  // 				 i,	// pixie module number
  // 				 0x7F);	// boot pattern bit mask


  //     // close debug mode   TrigConfig3 [0]
  //     unsigned int ModParData;
  //     retval = Pixie16ReadSglModPar((char*)"TrigConfig3", &ModParData, i);
  //     if(retval < 0) ErrorInfo("Detector.cc", "BootSystem()", "Pixie16ReadSglModPar/TrigConfig3", retval);
  //     if(APP32_TstBit(0, ModParData))
  // 	{
  // 	  ModParData = APP32_ClrBit(0, ModParData);
  // 	  retval = Pixie16WriteSglModPar((char*)"TrigConfig3", ModParData, i);
  // 	  if(retval < 0) ErrorInfo("Detector.cc", "BootSystem()", "Pixie16WriteSglModPar/TrigConfig3", retval);
  // 	}


      
  //   }


  // for(unsigned short k = 0; k < NumModules; k++)
  //   {
  //     retval = Pixie16WriteSglModPar((char*)"CrateID", crateid, k);
  //     if (retval < 0)
  // 	{
  // 	  ErrorInfo("Detector.cc", "BootSystem()", "Pixie16WriteSglModPar/CrateID", retval);
  // 	  std::cout<<"Pixie16WriteSglModPar/CrateID in module "<<k<<" failed, retval = "<<retval<<std::endl;
  // 	  return false;
  // 	}
  //   }


  // Adjust DC-Offsets
  // for(unsigned short k = 0; k < NumModules; k++)
  //   {		
  //     retval = Pixie16AdjustOffsets(k);
  //     if (retval < 0)
  // 	{
  // 	  ErrorInfo("Detector.cc", "BootSystem()", "Pixie16AdjustOffsets", retval);
  // 	  std::cout<<"Pixie16AdjustOffsets in module "<<k<<" failed, retval = "<<retval<<std::endl;
  // 	  return false;
  // 	}

  //     unsigned int blcut;
  //     for (unsigned short kk = 0; kk < 16; ++kk)
  // 	{
  // 	  retval = Pixie16BLcutFinder(k,kk,&blcut);
  // 	  if(retval < 0)
  // 	    {
  // 	      ErrorInfo("Detector.cc", "BootSystem()", "Pixie16BLcutFinder", retval);
  // 	      return false;
  // 	    }
  // 	}
  //   }  

  
  
  return true;
}


bool DeviceHandle::CheckExternalFIFOStatus()
{

  return true;
}
bool DeviceHandle::ReadDataFromExternalFIFO()
{

  return true;
}



// 
// DeviceHandle.cc ends here
