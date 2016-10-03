// Online.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 10月  3 10:42:50 2016 (+0800)
// Last-Updated: 一 10月  3 19:08:55 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 33
// URL: http://wuhongyi.github.io 

#include "Online.hh"
#include <climits>
ClassImp(Online)
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Online::Online(const TGWindow * p)
{
  buf = new unsigned char[(PRESET_MAX_MODULES*448*4)+10];
  buf_new = new unsigned char[(PRESET_MAX_MODULES*448*4)+10];
  number = UINT_MAX;
  flagrunnumber = false;
  
  CreateMenuBar();
  
  SetWindowName("Pixie16 DAQ Online");
  MapSubwindows();
  MapWindow();
  Resize(INITIAL_WIDTH, INITIAL_HIGHT);

}

Online::~Online()
{
  delete []buf;
  delete []buf_new;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Online::Init()
{
  bool flag = true;

  // 检查文件目录是否存在
  if(!IsDirectoryExists(filepathtext->GetText()))
    {
      printf("The data file directory does not exist.\n");
      flag =false;
    }
  else
    {
      printf("The data file directory exist.\n");
    }
  
  shm_id=shm_open("shmpixie16lee",O_RDWR,0);/*第一步：打开共享内存区*/
  if (shm_id == -1)
    {
      printf( "open shared memory error.errno=%d,desc=%s.\n", errno, strerror(errno));
      flag = false;
    }
  else
    {
      printf( "open shared memory ok.\n");
    }

  sem=sem_open("sempixie16lee",1);/*打开信号量*/
  if (sem==SEM_FAILED)
    {
      printf( "open semaphore error.errno=%d,desc=%s.\n", errno, strerror(errno));
      flag = false;
    }
  else
    {
      printf( "open semaphore ok.\n");
    }

  ptr=(unsigned char*)mmap(NULL,(PRESET_MAX_MODULES*448*4)+10,PROT_READ|PROT_WRITE,MAP_SHARED,shm_id,0);/*连接共享内存区*/

  if(flag)
    {
      startloop->SetEnabled(1);
      fstartloop = false;
    }
  else
    {
      startloop->SetEnabled(0);
      fstartloop = true;
    }
}

void Online::CreateMenuBar()
{
  TGMenuBar *MenuBar = new TGMenuBar(this, 1, 1, kHorizontalFrame);
  TGPopupMenu *MenuFile = new TGPopupMenu(fClient->GetRoot());
  MenuFile->AddEntry("E&xit", FILE_EXIT);
  MenuFile->AddSeparator();
  MenuFile->Associate(this);
  MenuBar->AddPopup("&File", MenuFile, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  AddFrame(MenuBar, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX, 0, 0, 0, 0));


  TGTab *TabPanel = new TGTab(this);
  this->AddFrame(TabPanel, new TGLayoutHints(kLHintsBottom | kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
  TGCompositeFrame *Tab1 = TabPanel->AddTab("CountRate");
  MakeFold1Panel(Tab1);
  
}

void Online::CloseWindow()
{
  gApplication->Terminate(0);
}

Bool_t Online::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
  int test1 = 0, test2 = 0;
  switch (test1 = GET_MSG(msg))
    {
    case  kC_HSLIDER:
      switch (test2 = GET_SUBMSG(msg))
	{
	case  kSL_POS:
	  switch(parm1)
	    {

	    }
	  break;

	default:
	  break;
	}
      break;
      
    case kC_COMMAND:
      switch (test2 = GET_SUBMSG(msg))
	{
	  ////////////////////menu popups/////////////////////////////            
	case kCM_MENU:
	  switch (parm1)
	    {
	    case FILE_EXIT:
	      CloseWindow();
	      break;

	    default:
	      break;
	    }

	}
	           
    default:
      break;
    }
  return kTRUE;
}

void Online::MakeFold1Panel(TGCompositeFrame * TabPanel)
{
  TGGroupFrame *setgroup = new TGGroupFrame(TabPanel,"Setup");
  
  TGHorizontalFrame *filepath = new TGHorizontalFrame(setgroup);
  TGLabel *filepathlabel = new TGLabel(filepath,"File Path: ");
  filepath->AddFrame(filepathlabel,new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 3, 4, 0));
  filepathtext = new TGTextEntry(filepath,new TGTextBuffer(50));
  filepath->AddFrame(filepathtext,new TGLayoutHints(kLHintsExpandX|kLHintsTop, 10 ,3,4,0));

  TGLabel *filenamelabel = new TGLabel(filepath,"File Name: ");
  filepath->AddFrame(filenamelabel,new TGLayoutHints(kLHintsLeft | kLHintsTop, 10 ,3,4,0));
  filenametext = new TGTextEntry(filepath, new TGTextBuffer(20));
  filepath->AddFrame(filenametext,new TGLayoutHints(kLHintsLeft| kLHintsTop,10,3,4,0));
  
  setgroup->AddFrame(filepath,new TGLayoutHints(kLHintsExpandX|kLHintsTop));

  
  filesetdone = new TGTextButton(filepath,"Complete");
  filesetdone->Connect("Pressed()","Online",this,"Init()");
  filepath->AddFrame(filesetdone,new TGLayoutHints(kLHintsLeft|kLHintsTop,10,3,4,4));

  startloop = new TGTextButton(filepath,"RunStart");
  startloop->Connect("Pressed()","Online",this,"StartStop()");
  startloop->SetEnabled(0);
  filepath->AddFrame(startloop,new TGLayoutHints(kLHintsLeft|kLHintsTop,10,3,4,0));

  StateMsg = new TGTextEntry(filepath,new TGTextBuffer(20));
  fClient->GetColorByName("green", color);
  StateMsg->SetTextColor(color, false);
  StateMsg->SetText("Rxxxx Mxx");
  StateMsg->SetEnabled(kFALSE);
  StateMsg->SetFrameDrawn(kFALSE);

  filepath->AddFrame(StateMsg,new TGLayoutHints(kLHintsLeft|kLHintsTop,10,3,4,0));
  
  TabPanel->AddFrame(setgroup,new TGLayoutHints(kLHintsExpandX|kLHintsTop));

  std::ifstream in("PixieOnline.config");
  if(!in.is_open()) return;
  char tmp[200];
  in.getline(tmp,200);
  filepathtext->Insert(tmp);
  in.getline(tmp,200);
  filenametext->Insert(tmp);
  in.close();
}

void Online::StartStop()
{
  if(fstartloop)
    {
      fstartloop = false;
      startloop->SetText("RunStart");
      std::ofstream out("PixieOnline.config");
      out<<filepathtext->GetText()<<std::endl;
      out<<filenametext->GetText()<<std::endl;
      out.close();
    }
  else
    {
      fstartloop = true;
      startloop->SetText("RunStop");
      PrevRateTime = get_time();
      LoopRun();
    }
}

void Online::LoopRun()
{  
  while(fstartloop)
    {
      
      CurrentTime = get_time();
      ElapsedTime = CurrentTime - PrevRateTime; /* milliseconds */
      if (ElapsedTime > 1000)
	{
	  // 应该换成检索目录中的最新文件 这样才能支持非共享内存模式
	  if(ModNum > 0 && number != UINT_MAX)
	    {
	      for(int i = 0;i < ModNum;i++)
		{
		  sprintf(Filename[i],"%s%s_R%04d_M%02d.bin",filepathtext->GetText(),filenametext->GetText(),RunNumber,i);
		  if(IsFileExists(Filename[i]))
		    {
		      std::cout<<Filename[i]<<"  Size: "<<GetFileSizeMB(Filename[i])<<std::endl;
		    }
		}
	    }
	  PrevRateTime = CurrentTime;
	}
      
      // if(sem_wait(sem) == -1) continue;
      // sem_post(sem);
      sem_getvalue(sem, &val);
      // printf("sem: %d\n",val);
      if(val > 0)
	{
	  memcpy(&tempN,ptr,4);
	  if(number != tempN)
	    {
	      memcpy(buf_new,ptr,(PRESET_MAX_MODULES*448*4)+10);
	      if(number == UINT_MAX) 
		{
		  number = tempN;
		  memcpy(buf,buf_new,(PRESET_MAX_MODULES*448*4)+10);
		  continue;
		}
	      number = tempN;
	      
	      // =======
	      memcpy(&ModNum,buf_new+4,2);
	      memcpy(&RunNumber,buf_new+6,4);
	      printf("RunNumber: %d -- Num: %d\n",RunNumber,number);

	      
	      for(int i = 0; i < ModNum;i++)
		{
		  memcpy(Statistics,buf+10+4*448*i,448*4);
		  memcpy(Statistics_new,buf_new+10+4*448*i,448*4);

		  RealTime = (double)Statistics[2] * pow(2.0, 32.0);
		  RealTime += (double)Statistics[3];
		  RealTime *= 1.0e-6 / (double)SYSTEM_CLOCK_MHZ;

		  RealTime_new = (double)Statistics_new[2] * pow(2.0, 32.0);
		  RealTime_new += (double)Statistics_new[3];
		  RealTime_new *= 1.0e-6 / (double)SYSTEM_CLOCK_MHZ;
		  
		  std::cout<<"======================="<<std::endl;
		  std::cout<<"Mod: "<<i<<std::endl;
		  std::cout<<"Real Time: "<<RealTime_new<<std::endl;
		  std::cout<<"Cha:  LiveTime:   InputCountRate:  OutputCountRate:"<<std::endl;
		  for(int j=0;j<16;j++)
		    {
		      LiveTime[j] = (double)Statistics[63+j] * pow(2.0, 32.0);
		      LiveTime[j] += (double)Statistics[79+j];
		      LiveTime[j] *= 1.0e-6 / (double)100;
		      FastPeaks[j] = (double)Statistics[95+j] * pow(2.0, 32.0);
		      FastPeaks[j] += (double)Statistics[111+j];
		      ChanEvents[j] = (double)Statistics[223+j] * pow(2.0, 32.0);
		      ChanEvents[j] += (double)Statistics[239+j];


		      LiveTime_new[j] = (double)Statistics_new[63+j] * pow(2.0, 32.0);
		      LiveTime_new[j] += (double)Statistics_new[79+j];
		      LiveTime_new[j] *= 1.0e-6 / (double)100;
		      FastPeaks_new[j] = (double)Statistics_new[95+j] * pow(2.0, 32.0);
		      FastPeaks_new[j] += (double)Statistics_new[111+j];
		      ChanEvents_new[j] = (double)Statistics_new[223+j] * pow(2.0, 32.0);
		      ChanEvents_new[j] += (double)Statistics_new[239+j];

		      std::cout<<j<<"  "<<LiveTime_new[j] <<"  "<<(FastPeaks_new[j]-FastPeaks[j])/(LiveTime_new[j]-LiveTime[j])<<"  "<<(ChanEvents_new[j]-ChanEvents[j])/(RealTime_new-RealTime)<<std::endl;
		    }


		}

	      memcpy(buf,buf_new,(PRESET_MAX_MODULES*448*4)+10);

	      sprintf(charrunstate,"R%04d M%02d",RunNumber,ModNum);
	      StateMsg->SetText(charrunstate);
	      if(flagrunnumber)
		{
		  fClient->GetColorByName("red", color);
		  StateMsg->SetTextColor(color, false);
		  flagrunnumber = false;
		}
	      else
		{
		  fClient->GetColorByName("blue", color);
		  StateMsg->SetTextColor(color, false);
		  flagrunnumber = true;
		}
	      
	    }
	  
	  // memcpy(&ModNum,ptr+4,2);
	 
	}

      gSystem->ProcessEvents();
    }
  // memcpy(buf,ptr,(PRESET_MAX_MODULES*448*4)+6);

}

double Online::GetFileSizeMB(const char *name)
{
  struct stat statbuff;  
  if(stat(name, &statbuff) < 0){  
    printf("errno：%d\n",errno);
    printf("ERR  ：%s\n",strerror(errno));
    return 0;  
  }else{  
    return statbuff.st_size/(1024.0*1024.0);
  }
}


bool Online::IsFileExists(const char *name)
{
  if((access(name,F_OK))!=-1)  
    {  
      return true;
    }  
  else  
    {  
      return false;
    } 
}

bool Online::IsDirectoryExists(const char *path)
{
  struct stat fileStat;
  if ((stat(path, &fileStat) == 0) && S_ISDIR(fileStat.st_mode))
    return true;
  else
    return false;
}

long Online::get_time()
{
  long time_ms;
  struct timeval t1;
  struct timezone tz;
  gettimeofday(&t1, &tz);
  time_ms = (t1.tv_sec) * 1000 + t1.tv_usec / 1000;
  return time_ms;
}

// 
// Online.cc ends here










