// Manager.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 8月 15 22:17:54 2016 (+0800)
// Last-Updated: 四 8月 29 16:18:24 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 18
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
  void SetFileName();
  // void RunReadData();
  void SetOnlineDataFlag();
  
  bool IsDirectoryExists(const char *path);//Determine if the folder exists
  bool CreateDirectory(const char *path);//Create folder
  
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
