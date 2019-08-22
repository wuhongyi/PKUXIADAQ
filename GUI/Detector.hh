#ifndef DETECTOR_HH_
#define DETECTOR_HH_

#include "../software/app/pixie16app_export.h"

#include <fcntl.h>
#include <semaphore.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <string>

#define FIRMWARE100M12BIT
#define FIRMWARE100M14BIT
#define FIRMWARE250M12BIT
#define FIRMWARE250M14BIT
#define FIRMWARE250M16BIT
// #define FIRMWARE500M12BIT
#define FIRMWARE500M14BIT

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#define BLEN (500*2516) // size of 1 buffer
#define BUFFLENGTH (500*2516) // 4.8MB Buffer *2 (DBUFF)

#define SHAREDMEMORYDATAOFFSET 10 //BYTE
// 1st 4 bytes IDcode for event shared memory
// 2nd 2 bytes number of valid Num Modules in shared memory
// 3rd 4 bytes Run Number
#define SHAREDMEMORYDATASTATISTICS 448
#define SHAREDMEMORYDATAENERGYLENGTH 32768
#define SHAREDMEMORYDATAMAXCHANNEL 16

// 共享内存数据定义 10bytes header + 插件采样率 + 数据统计 + 能谱
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class Detector
{
public:
  Detector(int mode);
  virtual ~Detector();
  
  int Syncronise();
  int ExitSystem();
  int StartLSMRun(int continue_run);
  int OpenSaveFile(int n,const char *FileN);
  int SavetoFile(int nFile);
  int RunStatus();
  int ReadDataFromModules(int thres = 0,unsigned short endofrun = 0);
  int StopLSMRun();
  int CloseFile();
  int SetOnlineF(bool flag);
  void SetRecordFlag(bool flag);
  int SaveHistogram(char *fileN,int mod);
  int SaveDSPPars(char *file);
  int OpenSharedMemory();
  int UpdateSharedMemory();
  void SetRunNumber(int r);
  unsigned int GetFileSize(int n); // in MB
  // bool SetEvtl();
  //  	int Write2FileLSM(char *name);
  int AcquireADCTrace(unsigned short *trace, unsigned long size, unsigned short module, unsigned short ChanNum);

  bool BootSystem();

  void StatisticsForModule();
  void UpdateEnergySpectrumForModule();

  unsigned short GetModuleADCMSPS(int mod) {return ModuleInformation[mod].Module_ADCMSPS;}
  
  struct Module_Info
  {
    // Module information
    unsigned short Module_Rev;
    unsigned int   Module_SerNum;
    unsigned short Module_ADCBits;
    unsigned short Module_ADCMSPS;
    unsigned short Module_OfflineVariant;
  };

  
public:
  unsigned short NumModules;
  
private:
  unsigned short *PXISlotMap;
  unsigned short OfflineMode;

  unsigned int crateid;
  std::vector<unsigned short> *moduleslot;
  std::vector<unsigned short> *modulesamplingrate;
  std::vector<unsigned short> *modulebits;

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
  
  bool ReadConfigFile(char *config = (char*)"../parset/cfgPixie16.txt");
  
  FILE *fsave[PRESET_MAX_MODULES];
  unsigned int buff[PRESET_MAX_MODULES][BUFFLENGTH];
  int buffid[PRESET_MAX_MODULES];
  unsigned long FILESIZE[PRESET_MAX_MODULES];
  // unsigned int  *evtlen;// TODO  in dan't fixed length

  struct Module_Info ModuleInformation[PRESET_MAX_MODULES];
  
  // variables for shared memory
  sem_t *shmsem; // shared memory semaphore
  int    shmfd;  // shared memory id
  unsigned char *shmptr;// pointer to shm
  bool   fonline;
  bool   frecord;
  int runnumber;
  //  	bool BootSystem (unsigned short NumModules, unsigned short *PXISlotMap);

  uint64_t CurrentTime;
  uint64_t PrevRateTime;
  uint64_t ElapsedTime;
  uint64_t StartTime;
  uint64_t StopTime;
  long get_time();
  
};

#endif /*DETECTOR_HH_*/
