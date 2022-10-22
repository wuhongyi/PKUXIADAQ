// DeviceHandle.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 三 6月 22 09:55:01 2022 (+0800)
// Last-Updated: 三 6月 22 10:35:50 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 5
// URL: http://wuhongyi.cn 

#ifndef _DEVICEHANDLE_H_
#define _DEVICEHANDLE_H_



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DeviceHandle
{
public:
  DeviceHandle();
  virtual ~DeviceHandle();


  void StartAcquisition();
  void StopAcquisition();
  // QString getAcqStatus();
  
private:

  int mHandle;
  int mNumberOfDevice;

  // QList<DeviceModule *> mDeviceModules;
  
};

#endif /* _DEVICEHANDLE_H_ */
// 
// DeviceHandle.hh ends here
