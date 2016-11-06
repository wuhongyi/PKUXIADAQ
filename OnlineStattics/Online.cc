// Online.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 10月  3 10:42:50 2016 (+0800)
// Last-Updated: 日 11月  6 10:36:11 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 136
// URL: http://wuhongyi.cn 

#include "Online.hh"
#include <climits>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
ClassImp(Online)
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Online::Online(const TGWindow * p)
{
  buf = new unsigned char[(PRESET_MAX_MODULES*448*4)+10];
  buf_new = new unsigned char[(PRESET_MAX_MODULES*448*4)+10];
  number = UINT_MAX;
  flagrunnumber = false;
  
  CreateMenuBar();
  
  SetWindowName("PKU Pixie16-RevF DAQ Online");
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
  TGCompositeFrame *Tab2 = TabPanel->AddTab("Alert");
  MakeFold2Panel(Tab2);
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
	// case kCM_COMBOBOX:
	//   switch (parm1)
	//     {
	//       std::cout<<"XXX: "<<parm1-100<<std::endl;

	//       break;

	//     default:
	//       break;
	//     }

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
  filepathtext = new TGTextEntry(filepath,new TGTextBuffer(20));
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
  StateMsg->SetFont("-adobe-helvetica-bold-r-*-*-11-*-*-*-*-*-iso8859-1", false);
  fClient->GetColorByName("green", color);
  StateMsg->SetTextColor(color, false);
  StateMsg->SetText("Rxxxx     Mxx");
  StateMsg->SetEnabled(kFALSE);
  StateMsg->SetFrameDrawn(kFALSE);

  filepath->AddFrame(StateMsg,new TGLayoutHints(kLHintsLeft|kLHintsTop,10,3,4,0));
  
  TabPanel->AddFrame(setgroup,new TGLayoutHints(kLHintsExpandX|kLHintsTop));

  // =================
  cl0 = new TGTextEntry *[14];
  LabelsI = new TGTextEntry *[14];
  LabelsO = new TGTextEntry *[14];
  ICR = new TGTextEntry *[224];
  OCR = new TGTextEntry *[224];
  Labels = new TGTextEntry *[224];
  SampleRate = new TGComboBox *[13];
  
  char nnn[10];

  TGGroupFrame *monitorgroup = new TGGroupFrame(TabPanel,"Monitor");

  TGHorizontalFrame *horizontal1 = new TGHorizontalFrame(monitorgroup);
  monitorgroup->AddFrame(horizontal1,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
  TGHorizontalFrame *horizontal2 = new TGHorizontalFrame(monitorgroup);
  monitorgroup->AddFrame(horizontal2,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
  
  Column1 = new TGVerticalFrame *[42];
  for (int i = 0; i < 13; i++)
    {
      if(i < 7)
	{
	  Column1[3*i] = new TGVerticalFrame(horizontal1, 200, 300);
	  horizontal1->AddFrame(Column1[3*i], new TGLayoutHints(kLHintsTop | kLHintsExpandX, 10, 0, 0, 0));
	  Column1[3*i+1] = new TGVerticalFrame(horizontal1, 200, 300);
	  horizontal1->AddFrame(Column1[3*i+1], new TGLayoutHints(kLHintsTop | kLHintsExpandX, 0, 0, 0, 0));
	  Column1[3*i+2] = new TGVerticalFrame(horizontal1, 200, 300);
	  horizontal1->AddFrame(Column1[3*i+2], new TGLayoutHints(kLHintsTop | kLHintsExpandX, 0, 0, 0, 0));
	}
      else
	{
	  Column1[3*i] = new TGVerticalFrame(horizontal2, 200, 300);
	  horizontal2->AddFrame(Column1[3*i], new TGLayoutHints(kLHintsTop | kLHintsExpandX, 10, 0, 0, 0));
	  Column1[3*i+1] = new TGVerticalFrame(horizontal2, 200, 300);
	  horizontal2->AddFrame(Column1[3*i+1], new TGLayoutHints(kLHintsTop | kLHintsExpandX, 0, 0, 0, 0));
	  Column1[3*i+2] = new TGVerticalFrame(horizontal2, 200, 300);
	  horizontal2->AddFrame(Column1[3*i+2], new TGLayoutHints(kLHintsTop | kLHintsExpandX, 0, 0, 0, 0));
	}
      
      cl0[i] = new TGTextEntry(Column1[3*i], new TGTextBuffer(100), 10000,
			       cl0[i]->GetDefaultGC()(),
			       cl0[i]->GetDefaultFontStruct(),
			       kRaisedFrame | kDoubleBorder, GetWhitePixel());
      cl0[i]->SetFont("-adobe-helvetica-bold-r-*-*-11-*-*-*-*-*-iso8859-1", false);
      cl0[i]->SetText ("ch #");
      cl0[i]->Resize(35, 20);
      cl0[i]->SetEnabled(kFALSE);
      cl0[i]->SetFrameDrawn(kTRUE);
      Column1[3*i]->AddFrame(cl0[i], new TGLayoutHints(kLHintsCenterX, 0, 0, 10, 0));

      LabelsI[i] = new TGTextEntry(Column1[3*i+1], new TGTextBuffer(100), 10000,
				   LabelsI[i]->GetDefaultGC()(),
				   LabelsI[i]->GetDefaultFontStruct(),
				   kRaisedFrame | kDoubleBorder, GetWhitePixel());
      LabelsI[i]->SetFont("-adobe-helvetica-bold-r-*-*-10-*-*-*-*-*-iso8859-1", false);
      LabelsI[i]->SetText("InputRate/s");
      LabelsI[i]->SetAlignment(kTextCenterX);
      LabelsI[i]->Resize(90, 20);
      LabelsI[i]->SetEnabled(kFALSE);
      LabelsI[i]->SetFrameDrawn(kTRUE);
      Column1[3*i+1]->AddFrame(LabelsI[i], new TGLayoutHints(kLHintsCenterX, 0, 0, 10, 0));


      LabelsO[i] = new TGTextEntry(Column1[3*i+2], new TGTextBuffer(100), 10000,
				   LabelsO[i]->GetDefaultGC()(),
				   LabelsO[i]->GetDefaultFontStruct(),
				   kRaisedFrame | kDoubleBorder, GetWhitePixel());
      LabelsO[i]->SetFont("-adobe-helvetica-bold-r-*-*-10-*-*-*-*-*-iso8859-1", false);
      LabelsO[i]->SetText("OutputRate/s");
      LabelsO[i]->SetAlignment(kTextCenterX);
      LabelsO[i]->Resize(90, 20);
      LabelsO[i]->SetEnabled(kFALSE);
      LabelsO[i]->SetFrameDrawn(kTRUE);
      Column1[3*i+2]->AddFrame(LabelsO[i], new TGLayoutHints(kLHintsCenterX, 0, 0, 10, 0));
      
      for (int j = 0; j < 16; j++)
	{
	  Labels[16*i+j] = new TGTextEntry(Column1[3*i], new TGTextBuffer(100), 10000,
				      Labels[16*i+j]->GetDefaultGC()(),
				      Labels[16*i+j]->GetDefaultFontStruct(),
				      kRaisedFrame | kDoubleBorder,
				      GetWhitePixel());
	  sprintf(nnn, "%02d", j);
	  Labels[16*i+j]->SetText(nnn);
	  Labels[16*i+j]->Resize(35, 20);
	  Labels[16*i+j]->SetEnabled(kFALSE);
	  Labels[16*i+j]->SetFrameDrawn(kTRUE);
	  Column1[3*i]->AddFrame(Labels[16*i+j], new TGLayoutHints (kLHintsCenterX, 0, 3, 0, 0));

	  
	  ICR[16*i+j] = new TGTextEntry(Column1[3*i+1], new TGTextBuffer(100), 10000,
				      ICR[16*i+j]->GetDefaultGC()(),
				      ICR[16*i+j]->GetDefaultFontStruct(),
				      kRaisedFrame | kDoubleBorder, GetWhitePixel());
	  ICR[16*i+j]->SetAlignment(kTextCenterX);
	  ICR[16*i+j]->SetText("");
	  ICR[16*i+j]->Resize(35, 20);
	  ICR[16*i+j]->SetEnabled(kFALSE);
	  // ICR[16*i+j]->SetFrameDrawn(kFALSE);
	  Column1[3*i+1]->AddFrame(ICR[16*i+j], new TGLayoutHints (kLHintsExpandX, 0, 0, 0, 0));//kLHintsCenterX


	  OCR[16*i+j] = new TGTextEntry(Column1[3*i+2], new TGTextBuffer(100), 10000,
					OCR[16*i+j]->GetDefaultGC()(),
					OCR[16*i+j]->GetDefaultFontStruct(),
					kRaisedFrame | kDoubleBorder, GetWhitePixel());
	  OCR[16*i+j]->SetAlignment(kTextCenterX);
	  OCR[16*i+j]->SetText("");
	  OCR[16*i+j]->Resize(35, 20);
	  OCR[16*i+j]->SetEnabled(kFALSE);
	  // OCR[16*i+j]->SetFrameDrawn(kFALSE);
	  Column1[3*i+2]->AddFrame(OCR[16*i+j], new TGLayoutHints (kLHintsExpandX, 0, 0, 0, 0));
	}
      
    }

  // ========
  
  Column1[39] = new TGVerticalFrame(horizontal2, 200, 300);
  horizontal2->AddFrame(Column1[39], new TGLayoutHints(kLHintsTop | kLHintsLeft, 10, 0, 0, 0));
  Column1[40] = new TGVerticalFrame(horizontal2, 200, 300);
  horizontal2->AddFrame(Column1[40], new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  Column1[41] = new TGVerticalFrame(horizontal2, 200, 300);
  horizontal2->AddFrame(Column1[41], new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  cl0[13] = new TGTextEntry(Column1[39], new TGTextBuffer(100), 10000,
			   cl0[13]->GetDefaultGC()(),
			   cl0[13]->GetDefaultFontStruct(),
			   kRaisedFrame | kDoubleBorder, GetWhitePixel());
  cl0[13]->SetFont("-adobe-helvetica-bold-r-*-*-11-*-*-*-*-*-iso8859-1", false);
  cl0[13]->SetText ("File");
  cl0[13]->Resize(35, 20);
  cl0[13]->SetEnabled(kFALSE);
  cl0[13]->SetFrameDrawn(kTRUE);
  Column1[39]->AddFrame(cl0[13], new TGLayoutHints(kLHintsCenterX, 0, 0, 10, 0));

  LabelsI[13] = new TGTextEntry(Column1[40], new TGTextBuffer(100), 10000,
			       LabelsI[13]->GetDefaultGC()(),
			       LabelsI[13]->GetDefaultFontStruct(),
			       kRaisedFrame | kDoubleBorder, GetWhitePixel());
  LabelsI[13]->SetFont("-adobe-helvetica-bold-r-*-*-10-*-*-*-*-*-iso8859-1", false);
  LabelsI[13]->SetText("Size/MB");
  LabelsI[13]->SetAlignment(kTextCenterX);
  LabelsI[13]->Resize(90, 20);
  LabelsI[13]->SetEnabled(kFALSE);
  LabelsI[13]->SetFrameDrawn(kTRUE);
  Column1[40]->AddFrame(LabelsI[13], new TGLayoutHints(kLHintsCenterX, 0, 0, 10, 0));

  for (int j = 0; j < 13; j++)
    {
      Labels[208+j] = new TGTextEntry(Column1[39], new TGTextBuffer(100), 10000,
				       Labels[208+j]->GetDefaultGC()(),
				       Labels[208+j]->GetDefaultFontStruct(),
				       kRaisedFrame | kDoubleBorder,
				       GetWhitePixel());
      sprintf(nnn, "M%02d", j);
      Labels[208+j]->SetText(nnn);
      Labels[208+j]->Resize(35, 20);
      Labels[208+j]->SetEnabled(kFALSE);
      Labels[208+j]->SetFrameDrawn(kTRUE);
      Column1[39]->AddFrame(Labels[208+j], new TGLayoutHints(kLHintsCenterX, 0, 3, 0, 0));
      
      ICR[208+j] = new TGTextEntry(Column1[40], new TGTextBuffer(100), 10000,
				   ICR[208+j]->GetDefaultGC()(),
				   ICR[208+j]->GetDefaultFontStruct(),
				   kRaisedFrame | kDoubleBorder, GetWhitePixel());
      ICR[208+j]->SetAlignment(kTextCenterX);
      ICR[208+j]->SetText("");
      ICR[208+j]->Resize(35, 20);
      ICR[208+j]->SetEnabled(kFALSE);
      // ICR[208+j]->SetFrameDrawn(kFALSE);
      Column1[40]->AddFrame(ICR[208+j], new TGLayoutHints(kLHintsExpandX, 0, 0, 0, 0));
    }



  LabelsO[13] = new TGTextEntry(Column1[41], new TGTextBuffer(100), 10000,
			       LabelsO[13]->GetDefaultGC()(),
			       LabelsO[13]->GetDefaultFontStruct(),
			       kRaisedFrame | kDoubleBorder, GetWhitePixel());
  LabelsO[13]->SetFont("-adobe-helvetica-bold-r-*-*-10-*-*-*-*-*-iso8859-1", false);
  LabelsO[13]->SetText("Sample/MHz");
  LabelsO[13]->SetAlignment(kTextCenterX);
  LabelsO[13]->Resize(90, 20);
  LabelsO[13]->SetEnabled(kFALSE);
  LabelsO[13]->SetFrameDrawn(kTRUE);
  Column1[41]->AddFrame(LabelsO[13], new TGLayoutHints(kLHintsCenterX, 0, 0, 10, 0));

  for (int j = 0; j < 13; j++)
    {
      SampleRate[j] = new TGComboBox(Column1[41]);
      Column1[41]->AddFrame(SampleRate[j], new TGLayoutHints(kLHintsExpandX, 0, 0, 0, 0));
      SampleRate[j]->Associate(this);
      SampleRate[j]->AddEntry("100", 1);
      SampleRate[j]->AddEntry("250", 2);
      SampleRate[j]->AddEntry("500", 3);
      SampleRate[j]->Select(1);
      SampleRate[j]->Resize(35, 20);
    }
  
  TabPanel->AddFrame(monitorgroup,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  std::ifstream in("PixieOnline.config");
  if(!in.is_open()) return;
  char tmp[200];
  in.getline(tmp,200);
  filepathtext->Insert(tmp);
  in.getline(tmp,200);
  filenametext->Insert(tmp);
  in.close();

}

void Online::MakeFold2Panel(TGCompositeFrame *TabPanel)
{

}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

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
      PrevTime = get_time();
      LoopRun();
    }
}

void Online::LoopRun()
{
  double filesize;
  int tempinputcountrate,tempoutputcountrate;// double
  std::string tempstring;
  
  std::stringstream ss;//sstream cstring
  ss.clear();//重复使用前一定要清空

  PrevProtectionTime = get_time();
  
  while(fstartloop)
    {
      
      CurrentTime = get_time();
      ElapsedTime = CurrentTime - PrevTime; /* milliseconds */
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
		      filesize = GetFileSizeMB(Filename[i]);
		      ss.clear();//重复使用前一定要清空
		      ss<<filesize;
		      ss>>tempstring;
		      ICR[208+i]->SetText(tempstring.c_str());
		    }
		}
	    }
	  PrevTime = CurrentTime;
	}
      
      // if(sem_wait(sem) == -1) continue;
      // sem_post(sem);
      sem_getvalue(sem, &val);
      printf("sem: %d\n",val);
      if(val > 0)
	{
	  memcpy(&tempN,ptr,4);
	  if(number != tempN)
	    {
	      PrevProtectionTime = get_time();
	      std::cout<<"get time 1"<<std::endl;
	      
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
	      // printf("RunNumber: %d -- Num: %d\n",RunNumber,number);
     
	      for(int i = 0; i < ModNum;i++)
		{
		  // SYSTEM_CLOCK_MHZ = 100;
		  if(SampleRate[i]->GetSelected() == 1) SYSTEM_CLOCK_MHZ = 100;
		  if(SampleRate[i]->GetSelected() == 2) SYSTEM_CLOCK_MHZ = 250;
		  if(SampleRate[i]->GetSelected() == 3) SYSTEM_CLOCK_MHZ = 500;
		  // std::cout<<"Mod: "<<i<<"  SYSTEM_CLOCK_MHZ: "<<SYSTEM_CLOCK_MHZ << "  " << SampleRate[i]->GetSelected() <<std::endl;
		  
		  memcpy(Statistics,buf+10+4*448*i,448*4);
		  memcpy(Statistics_new,buf_new+10+4*448*i,448*4);

		  RealTime = (double)Statistics[2] * pow(2.0, 32.0);
		  RealTime += (double)Statistics[3];
		  RealTime *= 1.0e-6 / (double)SYSTEM_CLOCK_MHZ;

		  RealTime_new = (double)Statistics_new[2] * pow(2.0, 32.0);
		  RealTime_new += (double)Statistics_new[3];
		  RealTime_new *= 1.0e-6 / (double)SYSTEM_CLOCK_MHZ;
		  
		  // std::cout<<"======================="<<std::endl;
		  // std::cout<<"Mod: "<<i<<std::endl;
		  // std::cout<<"Real Time: "<<RealTime_new<<std::endl;
		  // std::cout<<"Cha:  LiveTime:   InputCountRate:  OutputCountRate:"<<std::endl;
		  for(int j=0;j<16;j++)
		    {
		      LiveTime[j] = (double)Statistics[63+j] * pow(2.0, 32.0);
		      LiveTime[j] += (double)Statistics[79+j];
		      LiveTime[j] *= 1.0e-6 / (double)SYSTEM_CLOCK_MHZ;
		      FastPeaks[j] = (double)Statistics[95+j] * pow(2.0, 32.0);
		      FastPeaks[j] += (double)Statistics[111+j];
		      ChanEvents[j] = (double)Statistics[223+j] * pow(2.0, 32.0);
		      ChanEvents[j] += (double)Statistics[239+j];


		      LiveTime_new[j] = (double)Statistics_new[63+j] * pow(2.0, 32.0);
		      LiveTime_new[j] += (double)Statistics_new[79+j];
		      LiveTime_new[j] *= 1.0e-6 / (double)SYSTEM_CLOCK_MHZ;
		      FastPeaks_new[j] = (double)Statistics_new[95+j] * pow(2.0, 32.0);
		      FastPeaks_new[j] += (double)Statistics_new[111+j];
		      ChanEvents_new[j] = (double)Statistics_new[223+j] * pow(2.0, 32.0);
		      ChanEvents_new[j] += (double)Statistics_new[239+j];

		      // std::cout<<j<<"  "<<LiveTime_new[j] <<"  "<<(FastPeaks_new[j]-FastPeaks[j])/(LiveTime_new[j]-LiveTime[j])<<"  "<<(ChanEvents_new[j]-ChanEvents[j])/(RealTime_new-RealTime)<<std::endl;
		      tempinputcountrate = (FastPeaks_new[j]-FastPeaks[j])/(LiveTime_new[j]-LiveTime[j]);
		      tempoutputcountrate = (ChanEvents_new[j]-ChanEvents[j])/(RealTime_new-RealTime);

		      ss.clear();//重复使用前一定要清空
		      ss<<tempinputcountrate;
		      ss>>tempstring;
		      ICR[16*i+j]->SetText(tempstring.c_str());
		      ss.clear();//重复使用前一定要清空
		      ss<<tempoutputcountrate;
		      ss>>tempstring;
		      OCR[16*i+j]->SetText(tempstring.c_str());
		    }


		}

	      memcpy(buf,buf_new,(PRESET_MAX_MODULES*448*4)+10);

	      sprintf(charrunstate,"R%04d     M%02d",RunNumber,ModNum);
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

	}
      else
	{
	  std::cout<<"get time 2"<<std::endl;
	  CurrentProtectionTime = get_time();
	  ElapsedProtectionTime = CurrentProtectionTime - PrevProtectionTime;
	  if(ElapsedProtectionTime > 5000)
	    {
	      StateMsg->SetText("NOT SHARE DATA");
	      fClient->GetColorByName("yellow", color);
	      StateMsg->SetTextColor(color, false);
	    }
	  
	}

      gSystem->ProcessEvents();

      usleep(400000);//sleep 400us
    }
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
