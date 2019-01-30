// Manager.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 8月 15 22:17:54 2016 (+0800)
// Last-Updated: 三 1月 30 10:49:03 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 17
// URL: http://wuhongyi.cn 

#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "pixie16app_export.h"
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>
#include <cmath>

class Detector;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class Manager
{
public:
  Manager();
  virtual ~Manager();


  bool BootSystem();
 
  // void StartLSRun();

  void RunManager();
  
protected:
  void CheckKeyboard();
  void SetLSFileName();
  // void LSRunReadData();
  void SetLSonlinedataf();

  bool IsDirectoryExists(const char *path);//判断文件夹是否存在
  bool CreateDirectory(const char *path);//创建文件夹
  
  void PrintRunStatus();

  void PreStartRun();
  void PostStopRun();
private:
  char  LogFileName[1024];
  char  DSPParsFileName[1024];  
  char Filename[PRESET_MAX_MODULES][1024];
  char  Histogramname[PRESET_MAX_MODULES][1024];
  
  std::string filepathtext;
  std::string filenametext;
  std::string filerunnum;
  int runnum;

  bool fupdateenergyonline;
  bool fonlinedata;
  bool frecorddata;
  bool fautorun;
  bool fstartdaq;
  bool fstopdaq;
  
  Detector *detector;
  int autoruntimes;//s
  
  //run manager
  bool Quit;//true:exit  false:run
  bool AcqRun;//true:start  false:stop
  char Key;//
  bool WriteFlag;//true:write  false:not write
};

#endif /* _MANAGER_H_ */
// 
// Manager.hh ends here




