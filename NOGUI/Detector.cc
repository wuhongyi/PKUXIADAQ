// Detector.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 8月 15 16:52:00 2016 (+0800)
// Last-Updated: 六 11月  5 21:34:31 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 28
// URL: http://wuhongyi.cn 

#include "Detector.hh"

#include "Global.hh"

#include "pixie16app_defs.h"
#include "pixie16app_export.h"
#include "pixie16sys_export.h"


#include <errno.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <signal.h>
#include <cstring>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctime>

using namespace std;
Detector::Detector()
  :shmfd(-1),fonline(0)
{
  PXISlotMap = NULL;
  for(int i = 0; i < PRESET_MAX_MODULES;i++)
    {
      fsave[i] = NULL;
      buffid[i] = 0;
      FILESIZE[i] = 0;
    }
  ReadConfigFile();
}

Detector::~Detector()
{
  if(PXISlotMap != NULL) delete[]PXISlotMap;
  cout<<"detector is deleted!"<<endl;
  ExitSystem();
}

bool Detector::ReadConfigFile(char *config)
{
  ifstream input;
  char *temp = new char[80];
  input.open(config, ios::in);

  if (input.fail ())
    {
      cout << "can't open the config file ! " << config << endl << flush;
      if(temp != 0) delete[]temp;
      return false;
    }

  input >> NumModules;
  cout << "\n\n" << NumModules << " modules, in slots:";
  input.getline (temp, 80);
  PXISlotMap = new unsigned short[NumModules+1];
  for (int i = 0; i < NumModules; i++)
    {
      input >> PXISlotMap[i];
      input.getline(temp, 80);
      cout << PXISlotMap[i] << " ";
    }

  //==== This code is necessary if modules are installed in two crates ====//
  //input >> PXISlotMap[NumModules];
  //input.getline (temp, 80);
  //cout << PXISlotMap[NumModules] << " ";

  cout << endl << "Firmware files: \n";
  input >> ComFPGAConfigFile;
  input.getline (temp, 80);
  cout << "ComFPGAConfigFile:  " << ComFPGAConfigFile << endl;
  input >> SPFPGAConfigFile;
  input.getline (temp, 80);
  cout << "SPFPGAConfigFile:   " << SPFPGAConfigFile << endl;
  input >> TrigFPGAConfigFile;
  input.getline (temp, 80);
  cout << "TrigFPGAConfigFile: " << TrigFPGAConfigFile << endl;
  input >> DSPCodeFile;
  input.getline (temp, 80);
  cout << "DSPCodeFile:        " << DSPCodeFile << endl;
  input >> DSPParFile;
  input.getline (temp, 80);
  cout << "DSPParFile:         " << DSPParFile << endl;
  input >> DSPVarFile;
  input.getline (temp, 80);
  cout << "DSPVarFile:         " << DSPVarFile << endl;
  cout << "--------------------------------------------------------\n\n";

  input.close();
  input.clear();
  
  if(temp != 0) delete[]temp;
  return true;
}

bool Detector::BootSystem()
{
  int retval = 0;
  retval = Pixie16InitSystem(NumModules, PXISlotMap, 0);

  if (retval != 0)
    {
      ErrorInfo("Detector.cc", "BootSystem()", "Pixie16InitSystem", retval);
      cout << "PCI Pixie init has failed: " << retval << endl;
      return false;
    }

  Sleep(1000);
  retval = Pixie16BootModule(ComFPGAConfigFile, SPFPGAConfigFile,
			     TrigFPGAConfigFile, DSPCodeFile, DSPParFile,
			     DSPVarFile, NumModules, 0x7F);
  
  if (retval != 0)
    {
      ErrorInfo("Detector.cc", "BootSystem()", "Pixie16BootModule", retval);
      cout << "cards booting has failed !\n";
      return false;
    }

  // Adjust DC-Offsets
  for(int k = 0; k < NumModules; k++)
    {		
      retval = Pixie16AdjustOffsets(k);
      if (retval < 0)
	{
	  ErrorInfo("Detector.cc", "BootSystem()", "Pixie16AdjustOffsets", retval);
	  printf("Pixie16AdjustOffsets in module %d failed, retval = %d", k, retval);
	  return false;
	}
    }
  
  return true;

}

int Detector::Syncronise()
{
  int retval = 0;
  retval = Pixie16WriteSglModPar((char*)"IN_SYNCH", 0, 0);
  if (retval < 0)
    {
      ErrorInfo("Detector.cc", "Syncronise()", "Pixie16WriteSglModPar/IN_SYNCH", retval);
      fprintf (stderr, "Failed to write IN_SYNCH");
    }
  return retval;
}

int Detector::StartLSMRun(int continue_run)
{
  cout<<"RUN START"<<endl;
  // if(fonline&&shmfd<0) OpenSharedMemory();
  if(shmfd<0) OpenSharedMemory();//避免启动获取时候没开启，中途开启造成的bug问题。

  int retval = 0;
  // All modules start acuqire and Stop acquire simultaneously
  retval = Pixie16WriteSglModPar((char*)"SYNCH_WAIT", 1, 0);
  if (retval < 0)
    {
      ErrorInfo("Detector.cc", "StartLSMRun(...)", "Pixie16WriteSglModPar/SYNCH_WAIT", retval);
      fprintf(stderr, "Failed to write SYNCH_WAIT\n");
      return retval;
    }
   
  if (continue_run == 0){
    // Reset clock counters to 0
    retval = Pixie16WriteSglModPar((char*)"IN_SYNCH", 0, 0);
    if (retval < 0)
      {
	ErrorInfo("Detector.cc", "StartLSMRun(...)", "Pixie16WriteSglModPar/IN_SYNCH", retval);
        fprintf(stderr, "In Sync problem\n");
        return retval;
      }
  }

  PrevRateTime = get_time();// used in statistics 
  StartTime = get_time();

  if (continue_run == 0)
    retval = Pixie16StartListModeRun(NumModules,0x100, NEW_RUN);
  else
    retval = Pixie16StartListModeRun(NumModules, 0x100,RESUME_RUN);
  if (retval < 0)
    {
      ErrorInfo("Detector.cc", "StartLSMRun(...)", "Pixie16StartListModeRun", retval);
      fprintf(stderr, "Failed to start ListMode run in module");
      return retval;
    }

  return 0;
}

int Detector::ReadDataFromModules(int thres,unsigned short  endofrun){ 
  // when evnts' number exceeds thres, data will be read out from FIFO
  if(endofrun == 0){
    if(thres <= EXTFIFO_READ_THRESH) thres = EXTFIFO_READ_THRESH; // 1024 words
    if(thres > 3*EXTFIFO_READ_THRESH) thres = 3*EXTFIFO_READ_THRESH;
  } else {
    thres = 2;
  }

  // 共享内存输出统计
  if(fonline) StatisticsForModule();
  
  int retval=0;
  for(unsigned short i = 0;i < NumModules;i++)
    {
      unsigned int nwords;
      retval = Pixie16CheckExternalFIFOStatus(&nwords,i);
      if(retval<0) 
	{
	  ErrorInfo("Detector.cc", "ReadDataFromModules(...)", "Pixie16CheckExternalFIFOStatus", retval);
	  cout<<"Invalid modnum!"<<endl;
	  return 0;
	}

      if(nwords < (unsigned int)thres) continue;

      if(buffid[i]+nwords >= BLEN)
	{
	  SavetoFile(i);
	}

      retval = Pixie16ReadDataFromExternalFIFO( (unsigned int *) (&buff[i][buffid[i]]),nwords,i);
      if(retval < 0){
	ErrorInfo("Detector.cc", "ReadDataFromModules(...)", "Pixie16ReadDataFromExternalFIFO", retval);
	cout<<"CheckExternalFIFOWords: "<<nwords<<endl;
      }
      buffid[i]=buffid[i]+nwords;
      //    cout<<"nwords: "<<nwords<<endl;
    }
  return 1;
}

long Detector::get_time()
{
  long time_ms;
  struct timeval t1;
  struct timezone tz;
  gettimeofday(&t1, &tz);
  time_ms = (t1.tv_sec) * 1000 + t1.tv_usec / 1000;
  return time_ms;
}

void Detector::StatisticsForModule()
{
  
  CurrentTime = get_time();
  ElapsedTime = CurrentTime - PrevRateTime; /* milliseconds */
  if (ElapsedTime > 3000)
    {
      UpdateSharedMemory();
      PrevRateTime = CurrentTime;
    }
}

int Detector::RunStatus()
{
  int sum = 0;
  
  for(int i = 0;i < NumModules;i++)
    {
      int ret = 0;
      ret = Pixie16CheckRunStatus(i);
      cout<<"Run status: "<<i <<" sta:"<<ret<<endl;
      sum = sum + ret;
    }
  return sum;
}


int Detector::OpenSaveFile(int n,const char *FileN)
{
  fsave[n]=fopen(FileN,"wb");
  if(fsave[n] == NULL){
    cout<<"Cannot open store file!"<<endl;
    return 0;
  }
  return 1;
}

int Detector::SavetoFile(int nFile)
{
  // cout<<"saving file ..."<<endl;
  if(fsave[nFile] == NULL) {
    cout<<"ERROR! No opened file found for store!"<<endl;
    cout<<"CAUTION! No data will be saved!"<<endl;
    buffid[nFile] = 0;
    return 1;
  }
  
  size_t n = fwrite(buff[nFile],4,buffid[nFile],fsave[nFile]);

  if(n != (size_t)buffid[nFile]){
    cout<<"Not All Data has been stored!"<<endl;
  }
  FILESIZE[nFile] += buffid[nFile];
  buffid[nFile] = 0;
  // cout<<"FILE: "<<nFile<<" SIZE: "<<FILESIZE[nFile]<<endl;
  return 0;
}

int Detector::CloseFile()
{
  for(unsigned short i = 0;i < NumModules;i++)
    {
      if(buffid[i] > 0) SavetoFile(i);
      FILESIZE[i] = 0;
      fclose(fsave[i]);
    }
  return 1;
}

unsigned int  Detector::GetFileSize(int n)
{
  return (unsigned int)FILESIZE[n]/1024/1024*4;
}

int Detector::StopLSMRun()
{
  cout<<"STOP RUN!"<<endl;
  unsigned short ModNum = 0;
  StopTime = get_time();
  int retval = Pixie16EndRun(ModNum);
  if(retval < 0) {
    ErrorInfo("Detector.cc", "StopLSMRun(...)", "Pixie16EndRun", retval);
    cout<<"FAILED TO END THE RUN!!!"<<endl;
    return 1;
  }
  
  int counter=0;
  while(RunStatus()){
    ReadDataFromModules(0);
    counter++;
    if(counter > 10*NumModules) break;
  }
  if(counter == 10*NumModules) {
    cout<<" ERROR! Some modules may not End Run correctly!"<<endl;
  }
  // Make sure all data has been read out
  ReadDataFromModules(0,1); // end of run
  for(unsigned short i=0;i<NumModules;i++)
    {
      SavetoFile(i);
    }
  CloseFile();

  unsigned int Statistics[448];
  for(unsigned short i = 0;i < NumModules;i++)
    {
      retval = Pixie16ReadStatisticsFromModule(Statistics, i);
      if(retval < 0)
	{
	  ErrorInfo("Detector.cc", "StopLSMRun(...)", "Pixie16ReadStatisticsFromModule", retval);
	  cout<<"error in get statistics info"<<endl;
	}
      else
	{
	  cout<<"Mod: "<<i<<endl;

	  cout<<"InputCountRate: ";
	  for(unsigned short j=0;j<16;j++)
	    {
	      cout<<Pixie16ComputeInputCountRate(Statistics, i, j)<<" ";
	    }
	  cout<<endl;
	      
	  cout<<"LiveTime: ";
	  for(unsigned short j=0;j<16;j++)
	    {
	      cout<<Pixie16ComputeLiveTime(Statistics, i, j)<<" ";
	    }
	  cout<<endl;

	  cout<<"OutputCountRate: ";
	  for(unsigned short j=0;j<16;j++)
	    {
	      cout<<Pixie16ComputeOutputCountRate(Statistics, i, j)<<" ";
	    }
	  cout<<endl;

	  cout<<"FastPeaks ChanEvents: ";
	  for(unsigned short j=0;j<16;j++)
	    {
	      cout<< ((unsigned long)(Statistics[95+j])<<32)+Statistics[111+j] <<" "<<((unsigned long)(Statistics[223+j])<<32)+Statistics[239+j]<<", ";
	    }
	  cout<<endl;
	  
	  cout<<"RealTime: ";
	  cout<<Pixie16ComputeRealTime(Statistics, i)<<" ";
	  cout<<endl<<endl;
	      
	}
    }

  cout<<"Real Run Time:"<<StopTime-StartTime<<endl;
  return 0;
}

int Detector::OpenSharedMemory(){
  int flag = 0;
  if((shmsem=sem_open("sempixie16lee",O_CREAT,0666,1)) == SEM_FAILED){
    cout<<"Cannot create seamphore!"<<endl;
    flag++;
  }

  if((shmfd=shm_open("shmpixie16lee",O_CREAT|O_RDWR,0666)) < 0){
    cout<<"Cannot create shared memory"<<endl;
    flag++;
  }

  if(ftruncate(shmfd,(off_t)(PRESET_MAX_MODULES*4*448+10)) < 0){
    // 1st 4 bytes IDcode for event shared memory
    // 2nd 2 bytes number of valid Num Modules in shared memory
    // 3rd 4 bytes Run Number
    cout<<"Cannot alloc memory for shared memory!"<<endl;
    flag++;
  }

  if((shmptr = (unsigned char*) mmap(NULL,(PRESET_MAX_MODULES*448*4)+10, PROT_READ|PROT_WRITE,MAP_SHARED,shmfd,0)) == MAP_FAILED){
    cout<<"Cannot mmap the shared memroy to process space"<<endl;
    flag++;
  }
  if(flag > 0) return 0;
  cout<<"SHM Opend!"<<endl;
  return 1;
}

int Detector::UpdateSharedMemory()
{
  int rc;
  rc = sem_trywait(shmsem);
  if(rc == -1 && errno != EAGAIN){
    cout<<"sem_wait error!"<<endl;
    return 1;
  }else if(rc == -1) return 1; // this indicates the shm is under use
  static unsigned int tmp = 0;
  tmp++;
  memcpy(shmptr,&tmp,sizeof(unsigned int));
  memcpy(shmptr+4,&NumModules,sizeof(unsigned short));
  memcpy(shmptr+6,&runnumber,sizeof(unsigned int));
  int retval = 0;
  unsigned int Statistics[448];
  for(unsigned short i = 0;i < NumModules;i++)
    {
      retval = Pixie16ReadStatisticsFromModule(Statistics, i);
      if(retval < 0)
	{
	  ErrorInfo("Detector.cc", "UpdateSharedMemory()", "Pixie16ReadStatisticsFromModule", retval);
	  cout<<"error in get statistics info"<<endl;
	}
      memcpy(shmptr+10+448*4*i,Statistics,sizeof(unsigned int)*448);
    }

  if(sem_post(shmsem) == -1){
    cout<<"sem_post error!"<<endl;
    return 1;
  }
  cout<<"SHM updated!"<<endl;
  return 0;
}

int Detector::SetOnlineF(bool flag)
{
  fonline = flag;
  return 1;
}


int Detector::ExitSystem()
{
  int retval = Pixie16ExitSystem(NumModules);
  if(retval<0){
    ErrorInfo("Detector.cc", "ExitSystem()", "Pixie16ExitSystem", retval);
    return 1;
  }
  return 0;
}

void Detector::SetRunNumber(int r)
{
  runnumber = r;
}

// 
// Detector.cc ends here
