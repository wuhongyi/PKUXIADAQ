// DeviceHandle.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 三 6月 22 09:55:01 2022 (+0800)
// Last-Updated: 三 10月 26 22:17:03 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 27
// URL: http://wuhongyi.cn 

#ifndef _DEVICEHANDLE_H_
#define _DEVICEHANDLE_H_

#include <pixie16/pixie16.h>
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


  void StartAcquisition();
  void StopAcquisition();
  bool GetAcqStatus();

  unsigned short GetNumberOfDevice();
  void ClearDevice();
  void AddDevice(unsigned short slot);


  void SetCrateID(unsigned short cid);
  
  bool InitSystem();
  bool ReadModuleInfo();
  bool BootModule();

  // ReadDataFromModules
  bool CheckExternalFIFOStatus();
  bool ReadDataFromExternalFIFO();
  
private:
  

private:

  int mHandle;
  unsigned short mNumberOfDevice;
  std::vector<unsigned short> slot_def;
  unsigned short crateid;
  bool runstatus;
  unsigned short slotmap[13];


  struct ModuleInfo {
    // Module information
    unsigned short Module_Rev;
    unsigned int Module_SerNum;
    unsigned short Module_ADCBits;
    unsigned short Module_ADCMSPS;
    // unsigned short Module_OfflineVariant;
  };


  struct ModuleInfo ModuleInformation[13];
  
  // configuration cfg;
  
  // QList<DeviceModule *> mDeviceModules;
  
};

#endif /* _DEVICEHANDLE_H_ */
// 
// DeviceHandle.hh ends here
