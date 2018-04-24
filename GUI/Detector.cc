#include "Detector.hh"
#include "Global.hh"
#include "wuReadData.hh"

#include "pixie16app_defs.h"
#include "pixie16app_export.h"

#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <math.h>

using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Detector::Detector(int mode)
  :evtlen(0),shmfd(-1),fonline(0)
{
  OfflineMode = (unsigned short)mode;
  
  for(int i = 0; i < PRESET_MAX_MODULES; i++)
    {
      fsave[i] = NULL;
      buffid[i] = 0;
      FILESIZE[i] = 0;
    }

  PXISlotMap = new unsigned short[PRESET_MAX_MODULES];
  moduleslot = new std::vector<unsigned short>;
  modulesamplingrate = new std::vector<unsigned short>;
  modulebits = new std::vector<unsigned short>;
}

Detector::~Detector()
{
  if(moduleslot) delete moduleslot;
  if(modulesamplingrate) delete modulesamplingrate;
  if(modulebits) delete modulebits;
  
  cout<<"detector is deleted!"<<endl;
  ExitSystem();
}

bool Detector::ReadConfigFile(char *config)
{
  moduleslot->clear();
  modulesamplingrate->clear();
  modulebits->clear();
  int tempcount1 = wuReadData::ReadVector("ModuleSlot", config, moduleslot);
  int tempcount2 = wuReadData::ReadVector("ModuleSampingRate", config, modulesamplingrate);
  int tempcount3 = wuReadData::ReadVector("ModuleBits", config, modulebits);
  // std::cout<<tempcount1 <<"  "<<tempcount2<<std::endl;
  // std::cout<<moduleslot->size() <<"  "<<modulesamplingrate->size()<<std::endl;
								    

  File100M14bit_sys = wuReadData::ReadValue<std::string>("100M14bit_sys", config);
  File100M14bit_fip = wuReadData::ReadValue<std::string>("100M14bit_fip", config);
  File100M14bit_dspldr = wuReadData::ReadValue<std::string>("100M14bit_dspldr", config);
  File100M14bit_dsplst = wuReadData::ReadValue<std::string>("100M14bit_dsplst", config);
  File100M14bit_dspvar = wuReadData::ReadValue<std::string>("100M14bit_dspvar", config);
    
  File250M14bit_sys = wuReadData::ReadValue<std::string>("250M14bit_sys", config);
  File250M14bit_fip = wuReadData::ReadValue<std::string>("250M14bit_fip", config);
  File250M14bit_dspldr = wuReadData::ReadValue<std::string>("250M14bit_dspldr", config);
  File250M14bit_dsplst = wuReadData::ReadValue<std::string>("250M14bit_dsplst", config);
  File250M14bit_dspvar = wuReadData::ReadValue<std::string>("250M14bit_dspvar", config);
    
  FileSettingPars = wuReadData::ReadValue<std::string>("SettingPars", config);

    
  std::cout<<File100M14bit_sys<<std::endl;
  std::cout<<File100M14bit_fip<<std::endl;
  std::cout<<File100M14bit_dspldr<<std::endl;
  std::cout<<File100M14bit_dsplst<<std::endl;
  std::cout<<File100M14bit_dspvar<<std::endl;
  std::cout<<File250M14bit_sys<<std::endl;
  std::cout<<File250M14bit_fip<<std::endl;
  std::cout<<File250M14bit_dspldr<<std::endl;
  std::cout<<File250M14bit_dsplst<<std::endl;
  std::cout<<File250M14bit_dspvar<<std::endl;
  std::cout<<FileSettingPars<<std::endl;
    
  return true;
}

bool Detector::BootSystem()
{
  ReadConfigFile();
  
  if(OfflineMode == 0)
    {
      NumModules = moduleslot->size();
      cout<<"---------- Init System Mode: Online ----------"<<endl;
      std::cout<<"Module Number: "<< NumModules<<std::endl;
      for (unsigned int i = 0; i < moduleslot->size(); ++i)
	{
	  std::cout<<"module "<<i<<" in slot "<<moduleslot->at(i)<<std::endl;
	}
    }
  else
    {
      NumModules = modulesamplingrate->size();
      cout<<"---------- Init System Mode: Offline ----------"<<endl;
      std::cout<<"Module Number: "<< NumModules <<std::endl;
      for (unsigned int i = 0; i < modulesamplingrate->size(); ++i)
	{
	  std::cout<<"module "<<i<<"  sampling rate "<<modulesamplingrate->at(i)<<"  bits "<<modulebits->at(i)<<std::endl;
	}
    }

  for(unsigned short k = 0; k < NumModules; k++)
    {
      PXISlotMap[k] = moduleslot->at(k);
      ModuleInformation[k].Module_OfflineVariant = OfflineMode;
    }

  
  int retval = 0;
  retval = Pixie16InitSystem(NumModules, PXISlotMap, OfflineMode);
  if (retval != 0)
    {
      ErrorInfo("Detector.cc", "BootSystem()", "Pixie16InitSystem", retval);
      cout << "PCI Pixie init has failed: " << retval << endl;
      return false;
    }


  for(unsigned short k = 0; k < NumModules; k++)
    {
      retval = Pixie16ReadModuleInfo(k, &ModuleInformation[k].Module_Rev, &ModuleInformation[k].Module_SerNum, &ModuleInformation[k].Module_ADCBits, &ModuleInformation[k].Module_ADCMSPS);
      if(retval != 0)
	{
	  ErrorInfo("Detector.cc", "BootSystem()", "Pixie16ReadModuleInfo", retval);
	  printf("Pixie16ReadModuleInfo: failed to read module information in module %d, retval=%d", k, retval);
	}
      else
	{
	  if(OfflineMode != 0)
	    {
	      ModuleInformation[k].Module_ADCBits = modulebits->at(k);
	      ModuleInformation[k].Module_ADCMSPS = modulesamplingrate->at(k);
	    }
	  cout<<ModuleInformation[k].Module_Rev<<"  "<<ModuleInformation[k].Module_SerNum<<"  "<<ModuleInformation[k].Module_ADCBits<<"  "<<ModuleInformation[k].Module_ADCMSPS<<endl;
	}
    }


  for(unsigned short k = 0; k < NumModules; k++)
    {
      // 旧版固件0x7F 启动有问题，11032016新版没问题
      strcpy(DSPParFile, FileSettingPars.c_str());
      if(ModuleInformation[k].Module_ADCMSPS == 100 && ModuleInformation[k].Module_ADCBits == 14)
  	{
  	  strcpy(ComFPGAConfigFile, File100M14bit_sys.c_str());
  	  strcpy(SPFPGAConfigFile, File100M14bit_fip.c_str());
  	  strcpy(TrigFPGAConfigFile, File100M14bit_dsplst.c_str());
  	  strcpy(DSPCodeFile, File100M14bit_dspldr.c_str());
  	  strcpy(DSPVarFile, File100M14bit_dspvar.c_str());
  	}
      else if(ModuleInformation[k].Module_ADCMSPS == 250 && ModuleInformation[k].Module_ADCBits == 14)
  	{
  	  strcpy(ComFPGAConfigFile, File250M14bit_sys.c_str());
  	  strcpy(SPFPGAConfigFile, File250M14bit_fip.c_str());
  	  strcpy(TrigFPGAConfigFile, File250M14bit_dsplst.c_str());
  	  strcpy(DSPCodeFile, File250M14bit_dspldr.c_str());
  	  strcpy(DSPVarFile, File250M14bit_dspvar.c_str());
  	}
      else
  	{
  	  std::cout<<"There is no Samping Rate =>"<<ModuleInformation[k].Module_ADCMSPS<<" and Bits =>"<<ModuleInformation[k].Module_ADCBits<<" firmware. "<<std::endl;
  	  std::cout<<"Please contact Hongyi Wu(wuhongyi@qq.com)"<<std::endl;
  	  return false;
  	}

      cout<<"AA"<<ComFPGAConfigFile<<"AA"<<endl;
      cout<<"AA"<<SPFPGAConfigFile<<"AA"<<endl;
      cout<<"AA"<<TrigFPGAConfigFile<<"AA"<<endl;
      cout<<"AA"<<DSPCodeFile<<"AA"<<endl;
      cout<<"AA"<<DSPParFile<<"AA"<<endl;
      cout<<"AA"<<DSPVarFile<<"AA"<<endl;
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
  	  ErrorInfo("Detector.cc", "BootSystem()", "Pixie16BootModule", retval);
  	  cout << "cards booting has failed !  Module NUMBER "<< k <<endl;;
  	  return false;
  	}

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

  if(OfflineMode == 0)
    {
      for(unsigned short k = 0; k < NumModules; k++)
	{
	  unsigned int CSR;
	  retval = Pixie16ReadCSR(k, &CSR);
	  if(retval != 0)
	    {
	      ErrorInfo("Detector.cc", "BootSystem()", "Pixie16ReadCSR", retval);
	      printf("Pixie16ReadCSR: Invalid Pixie module number in module %d, retval=%d", k, retval);
	    }
	  else
	    {
	      cout<<"host Control & Status Register =>  mod:"<<k<<"  value:0x"<<hex<<CSR<<dec<<endl;
	    }
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
  if(!SetEvtl()) return 1;
  int retval = 0;
  // All modules start acuqire and Stop acquire simultaneously
  retval = Pixie16WriteSglModPar((char*)"SYNCH_WAIT", 1, 0);
  if (retval < 0)
    {
      ErrorInfo("Detector.cc", "StartLSMRun(...)", "Pixie16WriteSglModPar/SYNCH_WAIT", retval);
      fprintf(stderr, "Failed to write SYNCH_WAIT\n");
      return retval;
    }
   
  if (continue_run == 0)
    {
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

  for(int i = 0;i < NumModules;i++)
    if(evtlen[i] <= 0) return 1; // confirm the evtlengt

  return 0;
}

int Detector::ReadDataFromModules(int thres,unsigned short  endofrun)
{ 
  // when evnts' number exceeds thres, data will be read out from FIFO
  if(endofrun == 0)
    {
      if(thres <= EXTFIFO_READ_THRESH) thres = EXTFIFO_READ_THRESH; // 1024 words
      if(thres > 3*EXTFIFO_READ_THRESH) thres = 3*EXTFIFO_READ_THRESH;
    }
  else
    {
      thres = 2;
    }
  // if(fsave==NULL) {
  //   cout<<"No date file has been specified "<<endl;
  //   return 0;
  // }

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
  int sum=0;
  
  for(int i = 0;i < NumModules;i++)
    {
      int ret = 0;
      ret = Pixie16CheckRunStatus(i);
      cout<<"Run status: "<<i <<" sta:"<<ret<<endl;
      sum = sum + ret;
    }
  return sum;
}

/*
int
Detector::Write2FileLSM (char *name)
{
  int *ret=new int[NumModules];
  int sum =0 ;
  for(int i=0;i<NumModules;i++)
    {    
      ret[i]=0;
//      ret[i]=Pixie16SaveListModeDataToFile (name, i);
      if (ret[i] < 0)
	std::cout << "failed to save to file block from mod "<<i<<" !\n";
      sum=sum+ret[i];
    }

  return sum;
}
*/

int Detector::AcquireADCTrace(unsigned short *trace, unsigned long size, unsigned short module, unsigned short ChanNum)
{ 
  int result;
  if(module < NumModules)
    {
      	result = Pixie16AcquireADCTrace(module);
	if (result < 0)
	  {
	    ErrorInfo("Detector.cc", "AcquireADCTrace(...)", "Pixie16AcquireADCTrace", result);
	    return result;
	  }
	
        result = Pixie16ReadSglChanADCTrace(trace,	// trace data
					    size,	// trace length
					    module,	// module number
					    ChanNum);
	if (result < 0) ErrorInfo("Detector.cc", "AcquireADCTrace(...)", "Pixie16ReadSglChanADCTrace", result);
    }
  else
    {
      std::cout<<"wrong module number: "<<module<<std::endl; 
      return -1000;
    }
  
  FILE* datafil;
  datafil=fopen("trace.dat","w");

  for(unsigned int i = 0;i < size;i++)
    {
      fprintf(datafil,"%d %d\n",i,trace[i]);
    }
  fclose(datafil);

  return result;
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
  cout<<"saving file ..."<<endl;
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
  cout<<"FILE: "<<nFile<<" SIZE: "<<FILESIZE[nFile]<<endl;
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
  
  int counter = 0;
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
  for(unsigned short i = 0;i < NumModules;i++)
    {
      SavetoFile(i);
    }
  CloseFile();

  cout<<"Real Run Time:"<<StopTime-StartTime<<endl;
  return 0;
}

int Detector::OpenSharedMemory(){
   int flag = 0;
   if((shmsem=sem_open("sempixie16pkuxiadaq",O_CREAT,0666,1)) == SEM_FAILED)
     {
       cout<<"Cannot create seamphore!"<<endl;
       flag++;
     }

   if((shmfd=shm_open("shmpixie16pkuxiadaq",O_CREAT|O_RDWR,0666)) < 0)
     {
       cout<<"Cannot create shared memory"<<endl;
       flag++;
     }

   if(ftruncate(shmfd,(off_t)(PRESET_MAX_MODULES*4*SHAREDMEMORYDATASTATISTICS+SHAREDMEMORYDATAOFFSET+PRESET_MAX_MODULES*4*SHAREDMEMORYDATAENERGYLENGTH*SHAREDMEMORYDATAMAXCHANNEL)) < 0)
     {

       cout<<"Cannot alloc memory for shared memory!"<<endl;
       flag++;
     }

   if((shmptr = (unsigned char*) mmap(NULL,(PRESET_MAX_MODULES*SHAREDMEMORYDATASTATISTICS*4)+SHAREDMEMORYDATAOFFSET+PRESET_MAX_MODULES*4*SHAREDMEMORYDATAENERGYLENGTH*SHAREDMEMORYDATAMAXCHANNEL, PROT_READ|PROT_WRITE,MAP_SHARED,shmfd,0)) == MAP_FAILED)
     {
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
  if(rc == -1 && errno != EAGAIN)
    {
      cout<<"sem_wait error!"<<endl;
      return 1;
    }
  else if(rc == -1) return 1; // this indicates the shm is under use
  
  static unsigned int tmp = 0;
  tmp++;
  memcpy(shmptr,&tmp,sizeof(unsigned int));
  memcpy(shmptr+4,&NumModules,sizeof(unsigned short));
  memcpy(shmptr+6,&runnumber,sizeof(unsigned int));
  int retval = 0;
  unsigned int Statistics[SHAREDMEMORYDATASTATISTICS];
  for(unsigned short i = 0; i < NumModules; i++)
    {
      retval = Pixie16ReadStatisticsFromModule(Statistics, i);
      if(retval < 0)
	{
	  ErrorInfo("Detector.cc", "UpdateSharedMemory()", "Pixie16ReadStatisticsFromModule", retval);
	  cout<<"error in get statistics info"<<endl;
	}
      memcpy(shmptr+SHAREDMEMORYDATAOFFSET+SHAREDMEMORYDATASTATISTICS*4*i,Statistics,sizeof(unsigned int)*SHAREDMEMORYDATASTATISTICS);
    }

  if(sem_post(shmsem) == -1){
    cout<<"sem_post error!"<<endl;
    return 1;
  }
  cout<<"SHM updated!"<<endl;
  return 0;
}

void Detector::UpdateEnergySpectrumForModule()
{
  int retval = 0;
  
  // unsigned int Statistics[SHAREDMEMORYDATAENERGYLENGTH];
  // for(unsigned short i = 0; i < NumModules; i++)
  //   for(unsigned short j = 0; j < SHAREDMEMORYDATAMAXCHANNEL; j++)
  //     {
  // 	retval = Pixie16ReadHistogramFromModule(Statistics,SHAREDMEMORYDATAENERGYLENGTH,i,j);//channel by channel
  // 	if(retval < 0)
  // 	  {
  // 	    ErrorInfo("Detector.cc", "UpdateEnergySpectrumForModule()", "Pixie16ReadHistogramFromModule", retval);
  // 	    cout<<"Invalid Pixie module/channel number OR Failed to get the histogram data"<<endl;
  // 	  }
  // 	memcpy(shmptr+SHAREDMEMORYDATAOFFSET+PRESET_MAX_MODULES*4*SHAREDMEMORYDATASTATISTICS+i*4*SHAREDMEMORYDATAENERGYLENGTH*SHAREDMEMORYDATAMAXCHANNEL+j*4*SHAREDMEMORYDATAENERGYLENGTH,Statistics,sizeof(unsigned int)*SHAREDMEMORYDATAENERGYLENGTH);
  //     }


  unsigned int Statistics[SHAREDMEMORYDATAENERGYLENGTH*SHAREDMEMORYDATAMAXCHANNEL];
  for(unsigned short i = 0; i < NumModules; i++)
      {
	retval = Pixie16EMbufferIO(Statistics,SHAREDMEMORYDATAENERGYLENGTH*SHAREDMEMORYDATAMAXCHANNEL,0x0,1,i);
	if(retval < 0)
	  {
	    ErrorInfo("Detector.cc", "UpdateEnergySpectrumForModule()", "Pixie16EMbufferIO", retval);
	    cout<<"null pointer for buffer data / number of buffer words exceeds the limit / invalid DSP external memory address / invalid I/O direction / invalid Pixie module number / I/O Failure"<<endl;
	  }
	memcpy(shmptr+SHAREDMEMORYDATAOFFSET+PRESET_MAX_MODULES*4*SHAREDMEMORYDATASTATISTICS+i*4*SHAREDMEMORYDATAENERGYLENGTH*SHAREDMEMORYDATAMAXCHANNEL,Statistics,sizeof(unsigned int)*SHAREDMEMORYDATAENERGYLENGTH*SHAREDMEMORYDATAMAXCHANNEL);
      }


  
  cout<<"Updated Monitor Energy Spectrum. "<<endl;
  cout<<"Please don't update it too often. Frequent updates will affect DAQ efficiency."<<endl;
}


int Detector::SetOnlineF(bool flag)
{
  fonline = flag;
  return 1;
}

bool Detector::SetEvtl()
{
  evtlen = new unsigned int[NumModules];
  for(unsigned short i = 0;i < NumModules;i++){
    double mpar = -1;
    evtlen[i] = 4; // header
    int retval = Pixie16ReadSglChanPar((char*)"CHANNEL_CSRA",&mpar,i,0);
    if(retval<0){
      ErrorInfo("Detector.cc", "SetEvtl()", "Pixie16ReadSglChanPar/CHANNEL_CSRA", retval);
      return 0;
    }
    if(APP16_TstBit(12,mpar)) evtlen[i] += 4; // esum and baseline enabled
    if(APP16_TstBit(9,mpar)) evtlen[i] += 8; // qsum enabled
    if(APP16_TstBit(8,mpar))
      {
	double tracelen = -1;
	retval = Pixie16ReadSglChanPar((char*)"TRACE_LENGTH",&tracelen,i,0);
	if(retval < 0)
	  {
	    ErrorInfo("Detector.cc", "SetEvtl()", "Pixie16ReadSglChanPar/TRACE_LENGTH", retval);
	    return 0;
	  }
	evtlen[i] += tracelen*50;
      }
    cout<<"evtlen ch: "<<i<<" len: "<<evtlen[i]<<endl; 
  }
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

int Detector::SaveHistogram(char *fileN , int mod)
{
  
  int retval ;

  retval = Pixie16SaveHistogramToFile(fileN,mod);
  if(retval <0 ) {
    ErrorInfo("Detector.cc", "SaveHistogram(...)", "Pixie16SaveHistogramToFile", retval);
  }

  return 0;
}

void Detector::SetRunNumber(int r)
{
  runnumber = r;
}
