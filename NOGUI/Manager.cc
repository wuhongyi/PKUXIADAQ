// Manager.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 8月 15 22:19:02 2016 (+0800)
// Last-Updated: 三 10月 26 19:32:47 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 31
// URL: http://wuhongyi.cn 

#include "Manager.hh"

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
  fonlinedata = 0;
  
  // restore last run's file information
  std::ifstream in("../parset/Run.config");
  if(!in.is_open())
    {
      std::cout<<"can't open file PixieLSMRun.config"<<std::endl;
      exit(1);
    }
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
  ss.clear();//重复使用前一定要清空
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

void Manager::SetLSFileName()
{
  if(detector == 0){
    std::cout<<"Modules not booted!"<<std::endl;
    return ;
  }
  
 detector->SetRunNumber(runnum);
  for(int i = 0;i < detector->NumModules;i++)
    {
      sprintf(Filename[i],"%s%s_R%04d_M%02d.bin",filepathtext.c_str(),filenametext.c_str(),runnum,i);
      std::cout<<i<<" "<<Filename[i]<<std::endl;
    }
  fstartdaq = 0;
}

void Manager::SetLSonlinedataf()
{
  if(!fonlinedata){
    fonlinedata = 1;
    detector->SetOnlineF(1);
    std::cout<<"DAQ will send online data!"<<std::endl;
  }else{
    fonlinedata = 0;
    detector->SetOnlineF(0);
    std::cout<<"DAQ wont send online data!"<<std::endl;
  }
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
		std::cout<<"done!!!!!!"<<std::endl;
		int counter = 0;
		while(detector->StopLSMRun()){
		  // failed to stop run 
		  sleep(1); // wait 1s then try again
		  counter++;
		  if(counter > 10) break;
		}
		std::cout<<"finish!"<<std::endl;
		
	        AcqRun = false;

		runnum++;
		std::ofstream out("../parset/RunNumber");
		out<<runnum;
		out.close();
	      }
	    else
	      {//stop,do run
		SetLSFileName();
		detector->SetOnlineF(fonlinedata);

		for(int i = 0;i < detector->NumModules;i++)
		  {
		    std::cout<<"open: "<<Filename[i]<<std::endl;
		    if(!detector->OpenSaveFile(i,Filename[i]))
		      {
			std::cout<<Filename[i]<<std::endl;
			return;
		      }
		  }
      
		if(detector->StartLSMRun(0))
		  {
		    std::cout<<"CANNOT start the LSM Run!"<<std::endl;
		    return;
		  }
		usleep(100000); //delay for the DSP boot 
		// sleep(2);// wait 2 seconds for modules to be ready
		// start a new run, not resume

	        AcqRun = true;
	      }
	    break;
	  }
	// case 'w' :
	//   {
	//     if(!AcqRun)
	//       {
	// 	printf("Please enter [s] start.\n");
	// 	break;
	//       }
	//     if(WriteFlag)
	//       {//writting, do stop
	//         WriteFlag = false;

	//       }
	//     else
	//       {//not write, do start
	//         WriteFlag = true;


	//       }	
	//     break;
	//   }

	case 'o' :
	  {
	    detector->SetOnlineF(true);
	    
	    break;
	  }

	  
	default: break;
	}
    }
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
	  detector->ReadDataFromModules(0,0); // during the run
	}
    }
}

// 
// Manager.cc ends here
