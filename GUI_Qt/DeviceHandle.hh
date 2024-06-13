// DeviceHandle.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 三 6月 22 09:55:01 2022 (+0800)
// Last-Updated: 三 1月 17 12:51:50 2024 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 69
// URL: http://wuhongyi.cn 

#ifndef _DEVICEHANDLE_H_
#define _DEVICEHANDLE_H_

#include "Global.hh"
#include "pixie16/pixie16.h"

// 调用 XIA API
// 所有交互由该类完成


	       // 确保 ConnectDialog 系统初始化成功之后，不可再打开
#include <string>
#include <cstring>
#include <vector>
// #if defined(_WIN64) || defined(_WIN32)
// #include <windows.h>
// #else
// #include <unistd.h>
// #endif
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// struct firmware_spec {
//   unsigned int version;
//   int revision;
//   int adc_msps;
//   int adc_bits;

//   firmware_spec() : version(0), revision(0), adc_msps(0), adc_bits(0) {}
// };

// struct mod_cfg : module_config {
//   firmware_spec fw;
//   std::string dsp_par;
//   fifo_worker_config worker_config;
//   bool has_worker_cfg;
// };

// typedef std::vector<mod_cfg> module_configs;

// struct configuration {
//   module_configs modules;
//   std::vector<unsigned short> slot_def;
//   unsigned short num_modules() const {
//     return static_cast<unsigned short>(modules.size());
//   }
// };


class DeviceHandle
{
public:
  DeviceHandle();
  virtual ~DeviceHandle();

  void SYNCH();
  void StartAcquisition();
  void StopAcquisition();
  bool GetAcqStatus();
  
  unsigned short GetNumberOfDevice();
  
  
  void ClearDevice();
  void AddDevice(unsigned short slot);


  void SetCrateID(unsigned short cid);
  unsigned short GetCrateID() {return crateid;}
  
  bool InitSystem(int online);
  void ExitSystem();
  bool ReadModuleInfo();
  bool GetModuleInfo(unsigned short num, unsigned short *rev, unsigned short *msps, unsigned short *bits);
  bool BootModule(std::string ComFPGAConfigFile, std::string SPFPGAConfigFile, std::string DSPCodeFile, std::string DSPParFile, std::string DSPVarFile, unsigned short ModNum);

  bool WriteSglChanParQDC(unsigned short mod, unsigned short ch, unsigned short q, double value);
  bool WriteSglChanParTRACEDELAY(unsigned short mod, unsigned short ch, double value);
  bool WriteSglChanParTRACELENGTH(unsigned short mod, unsigned short ch, double value);
  bool WriteSglChanParINTEGRATOR(unsigned short mod, unsigned short ch, double value);
  bool WriteSglChanParCHANNELCSRA(unsigned short mod, unsigned short ch, double value);
  bool WriteSglChanParTRIGGERRISETIME(unsigned short mod, unsigned short ch, double value);
  bool WriteSglChanParTRIGGERFLATTOP(unsigned short mod, unsigned short ch, double value);
  bool WriteSglChanParTRIGGERTHRESHOLD(unsigned short mod, unsigned short ch, double value);
  bool WriteSglChanParENERGYRISETIME(unsigned short mod, unsigned short ch, double value);
  bool WriteSglChanParENERGYFLATTOP(unsigned short mod, unsigned short ch, double value);
  bool WriteSglChanParTAU(unsigned short mod, unsigned short ch, double value);
  bool WriteSglChanParCFDDelay(unsigned short mod, unsigned short ch, double value);
  bool WriteSglChanParCFDScale(unsigned short mod, unsigned short ch, double value);
  bool WriteSglChanParCFDThresh(unsigned short mod, unsigned short ch, double value);
  bool WriteSglChanParResetDelay(unsigned short mod, unsigned short ch, double value);
  bool WriteSglChanParBASELINEPERCENT(unsigned short mod, unsigned short ch, double value);
  bool WriteSglChanParExtTrigStretch(unsigned short mod, unsigned short ch, double value);
  bool WriteSglChanParExternDelayLen(unsigned short mod, unsigned short ch, double value);
  bool WriteSglChanParFtrigoutDelay(unsigned short mod, unsigned short ch, double value);
  bool WriteSglChanParVetoStretch(unsigned short mod, unsigned short ch, double value);
  bool WriteSglChanParChanTrigStretch(unsigned short mod, unsigned short ch, double value);
  bool WriteSglChanParFASTTRIGBACKLEN(unsigned short mod, unsigned short ch, double value);
  bool WriteSglChanParMultiplicityMaskL(unsigned short mod, unsigned short ch, double value);
  bool WriteSglChanParMultiplicityMaskH(unsigned short mod, unsigned short ch, double value);
  bool WriteSglChanParEMIN(unsigned short mod, unsigned short ch, double value);
  bool WriteSglChanParBINFACTOR(unsigned short mod, unsigned short ch, double value);
  bool WriteSglChanParXDT(unsigned short mod, unsigned short ch, double value);


  
  
  bool ReadSglChanParQDC(unsigned short mod, unsigned short ch, unsigned short q, double *value);
  bool ReadSglChanParTRACEDELAY(unsigned short mod, unsigned short ch, double *value);
  bool ReadSglChanParTRACELENGTH(unsigned short mod, unsigned short ch, double *value);
  bool ReadSglChanParINTEGRATOR(unsigned short mod, unsigned short ch, double *value);
  bool ReadSglChanParCHANNELCSRA(unsigned short mod, unsigned short ch, double *value);
  bool ReadSglChanParTRIGGERRISETIME(unsigned short mod, unsigned short ch, double *value);
  bool ReadSglChanParTRIGGERFLATTOP(unsigned short mod, unsigned short ch, double *value);
  bool ReadSglChanParTRIGGERTHRESHOLD(unsigned short mod, unsigned short ch, double *value);
  bool ReadSglChanParENERGYRISETIME(unsigned short mod, unsigned short ch, double *value);
  bool ReadSglChanParENERGYFLATTOP(unsigned short mod, unsigned short ch, double *value);
  bool ReadSglChanParTAU(unsigned short mod, unsigned short ch, double *value);
  bool ReadSglChanParCFDDelay(unsigned short mod, unsigned short ch, double *value);
  bool ReadSglChanParCFDScale(unsigned short mod, unsigned short ch, double *value);
  bool ReadSglChanParCFDThresh(unsigned short mod, unsigned short ch, double *value);
  bool ReadSglChanParResetDelay(unsigned short mod, unsigned short ch, double *value);
  bool ReadSglChanParBASELINEPERCENT(unsigned short mod, unsigned short ch, double *value);
  bool ReadSglChanParExtTrigStretch(unsigned short mod, unsigned short ch, double *value);
  bool ReadSglChanParExternDelayLen(unsigned short mod, unsigned short ch, double *value);
  bool ReadSglChanParFtrigoutDelay(unsigned short mod, unsigned short ch, double *value);
  bool ReadSglChanParVetoStretch(unsigned short mod, unsigned short ch, double *value);
  bool ReadSglChanParChanTrigStretch(unsigned short mod, unsigned short ch, double *value);
  bool ReadSglChanParFASTTRIGBACKLEN(unsigned short mod, unsigned short ch, double *value);
  bool ReadSglChanParMultiplicityMaskL(unsigned short mod, unsigned short ch, double *value);
  bool ReadSglChanParMultiplicityMaskH(unsigned short mod, unsigned short ch, double *value);
  bool ReadSglChanParEMIN(unsigned short mod, unsigned short ch, double *value);
  bool ReadSglChanParBINFACTOR(unsigned short mod, unsigned short ch, double *value);
  bool ReadSglChanParXDT(unsigned short mod, unsigned short ch, double *value);

  

  
  bool ReadSglChanParVOFFSET(unsigned short mod, unsigned short ch, double *value);
  bool ReadSglChanParBLCUT(unsigned short mod, unsigned short ch, double *value);


  

  bool WriteSglModParSLOWFILTERRANGE(unsigned short mod, unsigned int value);
  bool WriteSglModParFastTrigBackplaneEna(unsigned short mod, unsigned int value);
  bool WriteSglModParTrigConfig0(unsigned short mod, unsigned int value);
  bool WriteSglModParTrigConfig1(unsigned short mod, unsigned int value);
  bool WriteSglModParTrigConfig2(unsigned short mod, unsigned int value);
  bool WriteSglModParTrigConfig3(unsigned short mod, unsigned int value);
  bool WriteSglModParMODULECSRB(unsigned short mod, unsigned int value);
  
  
  bool ReadSglModParSLOWFILTERRANGE(unsigned short mod, unsigned int *value);
  bool ReadSglModParFastTrigBackplaneEna(unsigned short mod, unsigned int *value);
  bool ReadSglModParTrigConfig0(unsigned short mod, unsigned int *value);
  bool ReadSglModParTrigConfig1(unsigned short mod, unsigned int *value);
  bool ReadSglModParTrigConfig2(unsigned short mod, unsigned int *value);
  bool ReadSglModParTrigConfig3(unsigned short mod, unsigned int *value);
  bool ReadSglModParMODULECSRB(unsigned short mod, unsigned int *value);
  

  bool TauFinder(unsigned short mod, double *tau);
  bool BLcutFinder(unsigned short mod, unsigned short ch, unsigned int *cut);
  bool AdjustOffsets(unsigned short mod);


  bool GetTraceLength(unsigned short mod, unsigned short ch, unsigned int *value);
  bool GetMaxNumBaselines(unsigned short mod, unsigned short ch, unsigned int *value);
  bool AcquireADCTrace(unsigned short mod);
  bool AcquireBaselines(unsigned short mod);
  bool ReadSglChanADCTrace(unsigned short *trace, unsigned int len, unsigned short mod, unsigned short ch);
  bool ReadSglChanBaselines(double *bl, double *ts, unsigned short len, unsigned short mod, unsigned short ch);
  
  // not this function
  // void SaveHistogramToFile(std::string filename, unsigned short mod);
  void SaveDSPParametersToFile(std::string filename);//json

  bool AdjustOffsets();

  // after boot
  bool ReadModuleInfoAll();
  unsigned short GetNumberOfChannels(unsigned short num);
  unsigned short GetADCBitResolution(unsigned short num);
  unsigned short GetADCSamplingFrequency(unsigned short num);
  unsigned short GetRevision(unsigned short num);
  unsigned int GetSerialNumber(unsigned short num);
  unsigned short GetSlot(unsigned short num);

  void GetHistogramLength(unsigned short mod, unsigned short ch, unsigned int* len);
  void ReadHistogramFromModule(unsigned int *hist, unsigned int len, unsigned short mod, unsigned short ch);

  
  // ReadDataFromModules
  bool CheckExternalFIFOStatus(unsigned int* nwords, unsigned short mod);
  bool ReadDataFromExternalFIFO(unsigned int* data, unsigned int nwords, unsigned short mod);
  void ReadStatisticsFromModule(unsigned int* stat, unsigned short mod);


  double ComputeRealTime(unsigned int* stat, unsigned short mod);
  double ComputeLiveTime(unsigned int* stat, unsigned short mod, unsigned short ch);
  double ComputeRawOutputCount(unsigned int* stat, unsigned short mod, unsigned short ch);
  double ComputeRawInputCount(unsigned int* stat, unsigned short mod, unsigned short ch);


  
private:
  

private:

  int mHandle;
  unsigned short mNumberOfDevice;
  std::vector<unsigned short> slot_def;
  unsigned short crateid;
  bool runstatus;
  unsigned short slotmap[MAXMODULENUM];

  // Module information
  struct ModuleInfo {
    unsigned short Module_Rev;
    unsigned int Module_SerNum;
    unsigned short Module_ADCBits;
    unsigned short Module_ADCMSPS;
  };

  ModuleInfo ModuleInformation[MAXMODULENUM];
  
  // configuration cfg;
  
  // QList<DeviceModule *> mDeviceModules;


  module_config cfg[MAXMODULENUM];

  char CodeText[2048];
};

#endif /* _DEVICEHANDLE_H_ */
// 
// DeviceHandle.hh ends here
