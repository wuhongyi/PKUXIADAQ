// Detector.hh --- 
// 
// Description: copy from nscope
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 8月 15 16:51:04 2016 (+0800)
// Last-Updated: 一 10月  3 10:12:43 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 4
// URL: http://wuhongyi.github.io 

#ifndef _DETECTOR_H_
#define _DETECTOR_H_

#include <iostream>
#include <fstream>
#include <cstdio>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include "pixie16app_defs.h"
#include "pixie16app_export.h"

#include <sys/time.h>
#include <stdint.h>

#define BLEN (500*2516) // size of 1 buffer
#define BUFFLENGTH (500*2516) // 4.8MB Buffer *2 (DBUFF)


class Detector
{
public:
  Detector();
  virtual ~Detector();
  
  int Syncronise();
  int ExitSystem();
  int StartLSMRun(int continue_run);
  int OpenSaveFile(int n,const char *FileN);
  int SavetoFile(int nFile);
  int RunStatus();
  int ReadDataFromModules(int thres=0,unsigned short endofrun=0);
  int StopLSMRun();
  int CloseFile();
  int SetOnlineF(bool flag);
  int OpenSharedMemory();
  int UpdateSharedMemory();
  void SetRunNumber(int r);
  unsigned int GetFileSize(int n); // in MB
  bool SetEvtl();

  bool BootSystem();
  void StatisticsForModule();

public:
  unsigned short NumModules;
  
private:
  unsigned short *PXISlotMap;
  unsigned short OfflineMode;
  char ComFPGAConfigFile[80];
  char SPFPGAConfigFile[80];
  char TrigFPGAConfigFile[80];
  char DSPCodeFile[80];
  char DSPParFile[80];
  char DSPVarFile[80];
  bool ReadConfigFile(char *config = (char*)"cfgPixie16.txt");
	
  FILE *fsave[PRESET_MAX_MODULES];
  unsigned int buff[PRESET_MAX_MODULES][BUFFLENGTH];
  int buffid[PRESET_MAX_MODULES];
  unsigned long FILESIZE[PRESET_MAX_MODULES];

  // variables for shared memory
  sem_t *shmsem; // shared memory semaphore
  int    shmfd;  // shared memory id
  unsigned char *shmptr;// pointer to shm
  bool   fonline;
  int runnumber;
  
  uint64_t CurrentTime;
  uint64_t PrevRateTime;
  uint64_t ElapsedTime;
  uint64_t StartTime;
  uint64_t StopTime;
  long get_time();
};

#endif /* _DETECTOR_H_ */
// 
// Detector.hh ends here
