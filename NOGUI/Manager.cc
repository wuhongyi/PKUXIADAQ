// Manager.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 8月 15 22:19:02 2016 (+0800)
// Last-Updated: 四 8月 29 19:58:39 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 58
// URL: http://wuhongyi.cn 

#include "Manager.hh"
#include "wuReadData.hh"

#include "Global.hh"
#include "Detector.hh"
#include "pixie16sys_export.h"
#include "pixie16app_export.h"

#include <cstdio>
#include <cstdlib>

#include <unistd.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Manager::Manager()
{
  detector = 0; 
  runnum = 0;
  fonlinedata = true;
  frecorddata = true;
  fupdateenergyonline = false;
  fautorun = false;

  autoruntimes = wuReadData::ReadValue<int>("AutoRunModeTimes", "../parset/cfgPixie16.txt");

  
  // restore last run's file information
  std::ifstream in("../parset/Run.config");
  if(!in.is_open()) return;
  char tmp[200];
  in.getline(tmp,200);
  filepathtext = tmp;
  in.getline(tmp,200);
  filenametext = tmp;
  in.close();

  std::ifstream inrunnumber("../parset/RunNumber");
  if(!inrunnumber.is_open()) return;
  inrunnumber.getline(tmp,200);
  filerunnum = tmp;
  inrunnumber.close();
  
  std::stringstream ss;//sstream cstring
  ss.clear();//
  ss<<filerunnum;
  ss>>runnum;

  Quit = false;
  AcqRun = false;
}

Manager::~Manager()
{
  if(detector != 0) delete detector;
}

bool Manager::BootSystem()
{
  
  if(detector == 0) detector = new Detector();
  if(detector->BootSystem())
    {
      std::cout<<"Booted system"<<std::endl;
      return true;
    }
  else
    {
      std::cout<<"boot Failed..."<<std::endl;
      return false;
    }

}

void Manager::SetFileName()
{
  if(detector == 0)
    {
      std::cout<<"Modules not booted!"<<std::endl;
      return ;
    }

  char tempchardir[1024];
  sprintf(tempchardir,"%s%04d",filepathtext.c_str(),runnum);
  CreateDirectory(tempchardir);
  
 detector->SetRunNumber(runnum);
  for(int i = 0;i < detector->NumModules;i++)
    {
      sprintf(Filename[i],"%s%04d/%s_R%04d_M%02d.bin",filepathtext.c_str(),runnum,filenametext.c_str(),runnum,i);
      sprintf(Histogramname[i],"%s%04d/%s_histogram_R%04d_M%02d.bin",filepathtext.c_str(),runnum,filenametext.c_str(),runnum,i);
      // cout<<i<<" "<<Filename[i]<<endl;
    }

  sprintf(DSPParsFileName,"%s%04d/DSPPar.set",filepathtext.c_str(),runnum);
  sprintf(LogFileName,"%s%04d/run.log",filepathtext.c_str(),runnum);
    
  if(IsDirectoryExists(tempchardir))
    {
      std::ofstream out("../parset/Run.config");
      out<<filepathtext<<std::endl;
      out<<filenametext<<std::endl;
      out.close();
    }
  
  fstartdaq = 0;
}

void Manager::SetOnlineDataFlag()
{
  if(!fonlinedata)
    {
      fonlinedata = 1;
      detector->SetOnlineFlag(1);
      std::cout<<"DAQ will send online data!"<<std::endl;
    }
  else
    {
      fonlinedata = 0;
      detector->SetOnlineFlag(0);
      std::cout<<"DAQ wont send online data!"<<std::endl;
    }
}

void Manager::PreStartRun()
{
  SetFileName();
  detector->SetOnlineFlag(fonlinedata);

  for(int i = 0;i < detector->NumModules;i++)
    {
      std::cout<<"open: "<<Filename[i]<<std::endl;
      if(!detector->OpenSaveFile(i,Filename[i]))
	{
	  std::cout<<Filename[i]<<std::endl;
	  return;
	}
    }

  std::ofstream writelog;//fstream
  writelog.open(LogFileName,std::ios::app);//ios::bin ios::app
  if(!writelog.is_open())
    {
      std::cout<<"can't open Log file."<<std::endl;
    }
  time_t timep;
  time(&timep);
  char tmp[64];
  strftime(tmp, sizeof(tmp), "Start: %Y-%m-%d %H:%M:%S",localtime(&timep));
  writelog<<tmp<<std::endl;
  writelog.close();    

		
  if(detector->StartRun(0))
    {
      std::cout<<"CANNOT start the LSM Run!"<<std::endl;
      return;
    }
  usleep(100000); //delay for the DSP boot 
  // sleep(2);// wait 2 seconds for modules to be ready
  // start a new run, not resume

  AcqRun = true;

  PrintRunStatus();
}

void Manager::PostStopRun()
{
  std::cout<<"done!!!!!!"<<std::endl;
  int counter = 0;
  while(detector->StopRun())
    {
      // failed to stop run 
      sleep(1); // wait 1s then try again
      counter++;
      if(counter > 10) break;
    }

  detector->SaveDSPPars(DSPParsFileName);
  for(int i = 0;i < detector->NumModules;i++)
    {
      detector->SaveHistogram(Histogramname[i],i);
    }
		
  std::cout<<"finish!"<<std::endl;
		
  AcqRun = false;

  runnum++;
  std::ofstream out("../parset/RunNumber");
  out<<runnum;
  out.close();

  std::ofstream writelog;//fstream
  writelog.open(LogFileName,std::ios::app);//ios::bin ios::app
  if(!writelog.is_open())
    {
      std::cout<<"can't open Log file."<<std::endl;
    }
  time_t timep;
  time(&timep);
  char tmp[64];
  strftime(tmp, sizeof(tmp), "Stop : %Y-%m-%d %H:%M:%S",localtime(&timep));
  writelog<<tmp<<std::endl;
  writelog.close();

  PrintRunStatus();
}

void Manager::CheckKeyboard()
{
  // Check keyboard
  if(kbhit())
    {
      Key = getch();
      std::cout<<Key<<std::endl;
      
      switch(Key)
	{
	case 'q' :
	  {
	    if(AcqRun) 
	      {
		printf("Please enter [s] to stop and enter [q] to quit.\n");
		break;
	      }
	    Quit = true;
	    break;
	  }

	case 's' :
	  {
	    if(AcqRun)
	      {//running,do stop
		PostStopRun();
	      }
	    else
	      {//stop,do run
		PreStartRun();
	      }
	    break;
	  }
	case 'c':
	  {
	    PrintInterface();
	    break;
	  }
	case 'w' :
	  {
	    if(AcqRun)
	      {
		printf("Please enter [s] to stop acquisition first.\n");
		break;
	      }
	    if(frecorddata)
	      {//writting, to does not write mode
	        frecorddata = false;
		detector->SetRecordFlag(0);
	      }
	    else
	      {//not write, to write mode
	        frecorddata = true;
		detector->SetRecordFlag(1);
	      }	
	    break;
	  }

	case 'o' :
	  {
	    SetOnlineDataFlag();	    
	    break;
	  }

	case 'e':
	  {
	    fupdateenergyonline = true;
	    break;
	  }

	case 'a':
	  {
	    if(fautorun)
	      {
		fautorun = false;
		detector->SetAutoRunFlag(0);
	      }
	    else
	      {
		fautorun = true;
		detector->SetAutoRunFlag(1);
		detector->SetTimesPerRun(autoruntimes);
		std::cout<<"The times for each run: "<<autoruntimes<<" s"<<std::endl;
	      }
	    break;
	  }
	  
	case 'h':
	  {
	    PrintInterface();
	    PrintRunStatus();
	    break;
	  }
	  
	default: break;
	}
    }
}

void Manager::PrintRunStatus()
{
  if(AcqRun)
    std::cout<<"Running No. "<<runnum<<std::endl;
  else
    std::cout<<"Last run No. "<<runnum-1<<std::endl;

  if(fautorun)
    std::cout<<"Auto Run Mode: OPEN - "<<autoruntimes<<" s per run"<<std::endl;
  else
    std::cout<<"Auto Run Mode: CLOSE"<<std::endl;
  
  if(frecorddata)
    std::cout<<"Send Shared Memory Online: OPEN"<<std::endl;
  else
    std::cout<<"Send Shared Memory Online: CLOSE"<<std::endl;

  if(fonlinedata)
    std::cout<<"Write Data To Harddisk: OPEN"<<std::endl;
  else
    std::cout<<"Write Data To Harddisk: CLOSE"<<std::endl;
  
}

void Manager::RunManager()
{
  // Readout Loop
  PrintInterface();

  while(!Quit)
    {
      CheckKeyboard();

      if (!AcqRun) 
  	{
  	  Sleep(10);
  	  continue;
  	}

      if(AcqRun)
	{
	  // std::cout<<"Main:: read loop.."<<std::endl;
	  if(detector->ReadDataFromModules(0,0) == 1898)
	    {
	      PostStopRun();

	      PreStartRun();
	    }
	  
	  if(fupdateenergyonline)
	    {
	      detector->UpdateEnergySpectrumForModule();
	      fupdateenergyonline = false;
	    }
	}
    }
}


bool Manager::IsDirectoryExists(const char *path)
{
  struct stat fileStat;
  if ((stat(path, &fileStat) == 0) && S_ISDIR(fileStat.st_mode))
    return true;
  else
    return false;
}

bool Manager::CreateDirectory(const char *path)
{
  if(mkdir(path,0777)==0)//
    {
      printf("created the directory %s.\n",path);
      return true;
    }
  else
    {   	  
      printf("cant't creat the directory %s.\n",path);
      printf("errno：%d\n",errno);
      printf("ERR  ：%s\n",strerror(errno));	
      return false;
    }
}

// 
// Manager.cc ends here
