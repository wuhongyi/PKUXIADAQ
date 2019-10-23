// Online.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 10月  3 10:42:50 2016 (+0800)
// Last-Updated: 三 10月 23 14:04:04 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 293
// URL: http://wuhongyi.cn 

#include "Online.hh"

#include "TColor.h"
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
  ModNum = 0;
  number = UINT_MAX;
  flagrunnumber = false;

  SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  buf = new unsigned char[(PRESET_MAX_MODULES*SHAREDMEMORYDATASTATISTICS*4)+PRESET_MAX_MODULES*2+SHAREDMEMORYDATAOFFSET];
  buf_new = new unsigned char[(PRESET_MAX_MODULES*SHAREDMEMORYDATASTATISTICS*4)+PRESET_MAX_MODULES*2+SHAREDMEMORYDATAOFFSET];

  for (int i = 0; i < 16; ++i)
    {
      onlineth1i3[i] = NULL;
    }
  
  CreateMenuBar();
  
  SetWindowName("GDDAQ Online");
  MapSubwindows();
  MapWindow();
  Resize(INITIAL_WIDTH, INITIAL_HIGHT);

  gStyle->SetOptStat(0);//不显示统计框


  Init();
}

Online::~Online()
{
  delete []buf;
  delete []buf_new;

  for (int i = 0; i < 16; ++i)
    {
      if(onlineth1i3[i] != NULL) delete onlineth1i3[i];
    }
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Online::Init()
{
  bool flag = true;

  // 检查文件目录是否存在
  // if(!IsDirectoryExists(filepathtext->GetText()))
  //   {
  //     printf("The data file directory does not exist.\n");
  //     flag = false;
  //   }
  // else
  //   {
  //     printf("The data file directory exist.\n");
  //   }
  
  shm_id=shm_open("shmpixie16pkuxiadaq",O_RDWR,0);/*第一步：打开共享内存区*/
  if (shm_id == -1)
    {
      printf( "open shared memory error.errno=%d,desc=%s.\n", errno, strerror(errno));
      flag = false;
    }
  else
    {
      printf( "open shared memory ok.\n");
    }

  sem = sem_open("sempixie16pkuxiadaq",1);/*打开信号量*/
  if (sem == SEM_FAILED)
    {
      printf( "open semaphore error.errno=%d,desc=%s.\n", errno, strerror(errno));
      flag = false;
    }
  else
    {
      printf( "open semaphore ok.\n");
    }

  ftruncate(shm_id,(off_t)(SHAREDMEMORYDATAOFFSET+PRESET_MAX_MODULES*2+PRESET_MAX_MODULES*4*SHAREDMEMORYDATASTATISTICS+PRESET_MAX_MODULES*4*SHAREDMEMORYDATAENERGYLENGTH*SHAREDMEMORYDATAMAXCHANNEL));
  
  ptr = (unsigned char*)mmap(NULL,SHAREDMEMORYDATAOFFSET+PRESET_MAX_MODULES*2+(PRESET_MAX_MODULES*SHAREDMEMORYDATASTATISTICS*4)+PRESET_MAX_MODULES*4*SHAREDMEMORYDATAENERGYLENGTH*SHAREDMEMORYDATAMAXCHANNEL,PROT_READ|PROT_WRITE,MAP_SHARED,shm_id,0);/*连接共享内存区*/

  if(flag)
    {
      OnlineDrawButton3->SetEnabled(1);
	
      startloop->SetEnabled(1);
      fstartloop = false;
    }
  else
    {
      OnlineDrawButton3->SetEnabled(0);
	
      startloop->SetEnabled(0);
      fstartloop = true;
    }
}

void Online::AlertRead()
{
  std::ifstream readalert;//fstream
  readalert.open(alertfilenametext->GetText());
  if(!readalert.is_open())
    {
      std::cout<<"can't open  "<<alertfilenametext->GetText()<<std::endl;
      return;
    }

  double low,high;
  for (int i = 0; i < 208; ++i)
    {
      readalert>>low>>high;
      LowR[i]->SetNumber(low);
      HighR[i]->SetNumber(high);	
    }
  
  readalert.close();
}

void Online::AlertSave()
{
  std::ofstream writealert;//fstream
  writealert.open(alertfilenametext->GetText());//ios::bin ios::app
  if(!writealert.is_open())
    {
      std::cout<<"can't open  "<<alertfilenametext->GetText()<<std::endl;
      return;
    }

  for (int i = 0; i < 208; ++i)
    {
      writealert<<LowR[i]->GetNumber()<<" "<<HighR[i]->GetNumber()<<std::endl;
    }
 
  writealert.close();
}

void Online::AlertDefault()
{
  for (int i = 0; i < 224; ++i)
    {
      LowR[i]->SetTextColor(TColor::RGB2Pixel(COLOR_BLACK_R,COLOR_BLACK_G,COLOR_BLACK_B), false);
      LowR[i]->SetText("0");
      
      HighR[i]->SetTextColor(TColor::RGB2Pixel(COLOR_BLACK_R,COLOR_BLACK_G,COLOR_BLACK_B), false);
      HighR[i]->SetText("0");     
    }
}



void Online::CreateMenuBar()
{
  TGMenuBar *MenuBar = new TGMenuBar(this, 1, 1, kHorizontalFrame);
  AddFrame(MenuBar, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX, 0, 0, 0, 0));
  MenuBar->ChangeBackground(TColor::RGB2Pixel(TOPBAR_BG_R,TOPBAR_BG_G,TOPBAR_BG_B));
  
  TGPopupMenu *MenuFile = new TGPopupMenu(fClient->GetRoot());
  MenuFile->AddEntry("E&xit", FILE_EXIT);
  MenuFile->AddSeparator();
  MenuFile->Associate(this);
  
  MenuBar->AddPopup("&File", MenuFile, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGTab *TabPanel = new TGTab(this);
  this->AddFrame(TabPanel, new TGLayoutHints(kLHintsBottom | kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
  TabPanel->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  TGCompositeFrame *Tab1 = TabPanel->AddTab("CountRate");
  TabPanel->GetTabTab("CountRate")->ChangeBackground(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B));
  MakeFold1Panel(Tab1);
  Tab1->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  
  TGCompositeFrame *Tab2 = TabPanel->AddTab("Alert");
  TabPanel->GetTabTab("Alert")->ChangeBackground(TColor::RGB2Pixel(COLOR_YELLOW_R,COLOR_YELLOW_G,COLOR_YELLOW_B));
  MakeFold2Panel(Tab2);
  Tab2->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  
  TGCompositeFrame *Tab3 = TabPanel->AddTab("EnergyMonitor");
  TabPanel->GetTabTab("EnergyMonitor")->ChangeBackground(TColor::RGB2Pixel(COLOR_BLUE_R,COLOR_BLUE_G,COLOR_BLUE_B));
  MakeFold3Panel(Tab3);
  Tab3->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
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
	  break;

	case kCM_BUTTON:
	  switch (parm1)
	    {
	    case ONLINEDRAW3:
	      Panel3Draw();
	      break;
	    case ONLINEMODNUM3:
	      if (parm2 == 0)
		{
		  if (modNumber3 != ModNum-1)
		    {
		      ++modNumber3;
		      onlinemodnum3->SetIntNumber(modNumber3);
		    }
		}
	      else
		{
		  if (modNumber3 != 0)
		    {
		      if (--modNumber3 == 0)
			modNumber3 = 0;
		      onlinemodnum3->SetIntNumber(modNumber3);
		    }
		}
	      break;
	    case ONLINECHNUM3:
	      if (parm2 == 0)
		{
		  if (chanNumber3 != 15)
		    {
		      ++chanNumber3;
		      onlinechnum3->SetIntNumber(chanNumber3);
		    }
		}
	      else
		{
		  if (chanNumber3 != 0)
		    {
		      if (--chanNumber3 == 0)
			chanNumber3 = 0;
		      onlinechnum3->SetIntNumber(chanNumber3);
		    }
		}
	      break;


	    default:
	      break;
	    }
	  break;

	  
	// case kCM_COMBOBOX:
	//   switch (parm1)
	//     {
	//       std::cout<<"XXX: "<<parm1-100<<std::endl;

	//       break;

	//     default:
	//       break;
	//     }

	}

    case kC_TEXTENTRY:
      switch (parm1)
	{
	case ONLINEMODNUM3:
	  switch (GET_SUBMSG(msg))
	    {
	    case kTE_ENTER:
	      modNumber3 = onlinemodnum3->GetIntNumber();
	      if(modNumber3 > ModNum-1) modNumber3 = ModNum-1;
	      if(modNumber3 < 0) modNumber3 = 0;
	      onlinemodnum3->SetIntNumber(modNumber3);
	      break;
	      
	    default:
	      break;
	    }
	  break;
	case ONLINECHNUM3:
	  switch (GET_SUBMSG(msg))
	    {
	    case kTE_ENTER:
	      chanNumber3 = onlinechnum3->GetIntNumber();
	      if(chanNumber3 > 15) chanNumber3 = 15;
	      if(chanNumber3 < 0) chanNumber3 = 0;
	      onlinechnum3->SetIntNumber(chanNumber3);
	      break;
	      
	    default:
	      break;
	    }
	  break;

	}
      break;

      
    default:
      break;
    }
  return kTRUE;
}

void Online::MakeFold1Panel(TGCompositeFrame * TabPanel)
{
  TGGroupFrame *setgroup = new TGGroupFrame(TabPanel,"Setup");
  TabPanel->AddFrame(setgroup,new TGLayoutHints(kLHintsExpandX|kLHintsTop,4,4,0,0));
  setgroup->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  
  TGHorizontalFrame *filepath = new TGHorizontalFrame(setgroup);
  setgroup->AddFrame(filepath,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
  filepath->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  TGLabel *filepathlabel = new TGLabel(filepath,"File Path: ");
  filepath->AddFrame(filepathlabel,new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 3, 4, 0));
  filepathlabel->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  filepathtext = new TGTextEntry(filepath,new TGTextBuffer(20));
  filepath->AddFrame(filepathtext,new TGLayoutHints(kLHintsExpandX|kLHintsTop, 10 ,3,4,0));
  filepathtext->SetEnabled(kFALSE);
  filepathtext->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  TGLabel *filenamelabel = new TGLabel(filepath,"File Name: ");
  filepath->AddFrame(filenamelabel,new TGLayoutHints(kLHintsLeft | kLHintsTop, 10 ,3,4,0));
  filenamelabel->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  filenametext = new TGTextEntry(filepath, new TGTextBuffer(20));
  filepath->AddFrame(filenametext,new TGLayoutHints(kLHintsLeft| kLHintsTop,10,3,4,0));
  filenametext->SetEnabled(kFALSE);
  filenametext->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  

  StateMsg = new TGTextEntry(filepath,new TGTextBuffer(20));
  filepath->AddFrame(StateMsg,new TGLayoutHints(kLHintsLeft|kLHintsTop,10,3,4,0));
  StateMsg->SetFont("-adobe-helvetica-bold-r-*-*-11-*-*-*-*-*-iso8859-1", false);
  StateMsg->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  StateMsg->SetText("Rxxxx     Mxx");
  StateMsg->SetEnabled(kFALSE);
  StateMsg->SetFrameDrawn(kFALSE);
  StateMsg->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  

  startloop = new TGTextButton(filepath,"RunStart");
  filepath->AddFrame(startloop,new TGLayoutHints(kLHintsRight|kLHintsTop,10,3,4,0));
  startloop->Connect("Pressed()","Online",this,"StartStop()");
  startloop->SetEnabled(0);
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGTextEntry **cl0 = new TGTextEntry *[14];
  TGTextEntry **LabelsI = new TGTextEntry *[14];
  TGTextEntry **LabelsO = new TGTextEntry *[14];
  TGTextEntry **Labels = new TGTextEntry *[224];
  ICR = new TGTextEntry *[224];//[0-207] Input rate   >=208 File size
  OCR = new TGTextEntry *[224];//[0-207] Output rate  >=208 not used

  TGGroupFrame *monitorgroup = new TGGroupFrame(TabPanel,"Monitor");
  TabPanel->AddFrame(monitorgroup,new TGLayoutHints(kLHintsExpandX|kLHintsTop,4,4,0,0));
  monitorgroup->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  TGHorizontalFrame *horizontal1 = new TGHorizontalFrame(monitorgroup);
  monitorgroup->AddFrame(horizontal1,new TGLayoutHints(kLHintsExpandX|kLHintsTop,0,0,0,0));
  horizontal1->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  TGHorizontalFrame *horizontal2 = new TGHorizontalFrame(monitorgroup);
  monitorgroup->AddFrame(horizontal2,new TGLayoutHints(kLHintsExpandX|kLHintsTop,0,0,0,0));
  horizontal2->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  
  TGVerticalFrame **Column1 = new TGVerticalFrame *[42];
  for (int i = 0; i < 14; i++)
    {
      if(i < 7)
	{
	  Column1[3*i] = new TGVerticalFrame(horizontal1, 200,300);
	  horizontal1->AddFrame(Column1[3*i], new TGLayoutHints(kLHintsTop | kLHintsExpandX, 0, 0, 0, 0));
	  Column1[3*i]->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

	  Column1[3*i+1] = new TGVerticalFrame(horizontal1, 200, 300);
	  horizontal1->AddFrame(Column1[3*i+1], new TGLayoutHints(kLHintsTop | kLHintsExpandX, 0, 0, 0, 0));
	  Column1[3*i+1]->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

	  Column1[3*i+2] = new TGVerticalFrame(horizontal1, 200, 300);
	  horizontal1->AddFrame(Column1[3*i+2], new TGLayoutHints(kLHintsTop | kLHintsExpandX, 0, 0, 0, 0));
	  Column1[3*i+2]->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
	}
      else
	{
	  Column1[3*i] = new TGVerticalFrame(horizontal2, 200, 300);
	  horizontal2->AddFrame(Column1[3*i], new TGLayoutHints(kLHintsTop | kLHintsExpandX, 0, 0, 0, 0));
	  Column1[3*i]->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
	  
	  Column1[3*i+1] = new TGVerticalFrame(horizontal2, 200, 300);
	  horizontal2->AddFrame(Column1[3*i+1], new TGLayoutHints(kLHintsTop | kLHintsExpandX, 0, 0, 0, 0));
	  Column1[3*i+1]->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
	  
	  Column1[3*i+2] = new TGVerticalFrame(horizontal2, 200, 300);
	  horizontal2->AddFrame(Column1[3*i+2], new TGLayoutHints(kLHintsTop | kLHintsExpandX, 0, 0, 0, 0));
	  Column1[3*i+2]->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
	}
      
      cl0[i] = new TGTextEntry(Column1[3*i], new TGTextBuffer(100), 10000,
			       cl0[i]->GetDefaultGC()(),
			       cl0[i]->GetDefaultFontStruct(),
			       kRaisedFrame | kDoubleBorder, GetWhitePixel());
      Column1[3*i]->AddFrame(cl0[i], new TGLayoutHints(kLHintsRight | kLHintsTop, 0, 0, 10, 0));
      cl0[i]->SetFont("-adobe-helvetica-bold-r-*-*-11-*-*-*-*-*-iso8859-1", false);
      if(i == 13)
	cl0[i]->SetText("File");
      else
	cl0[i]->SetText("ch #");
      // cl0[i]->SetTextColor(TColor::RGB2Pixel(TEXTENTRY_TEXT_R,TEXTENTRY_TEXT_G,TEXTENTRY_TEXT_B), false);
      cl0[i]->Resize(30, 20);
      cl0[i]->SetEnabled(kFALSE);
      cl0[i]->SetFrameDrawn(kFALSE);
      // cl0[i]->SetAlignment(kTextRight);
      cl0[i]->ChangeBackground(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
      
      LabelsI[i] = new TGTextEntry(Column1[3*i+1], new TGTextBuffer(100), 10000,
				   LabelsI[i]->GetDefaultGC()(),
				   LabelsI[i]->GetDefaultFontStruct(),
				   kRaisedFrame | kDoubleBorder, GetWhitePixel());
      LabelsI[i]->SetFont("-adobe-helvetica-bold-r-*-*-10-*-*-*-*-*-iso8859-1", false);
      if(i == 13)
        LabelsI[i]->SetText("Size/MB");
      else
	LabelsI[i]->SetText("InRate/s");
      // LabelsI[i]->SetAlignment(kTextCenterX);
      LabelsI[i]->Resize(90, 20);
      LabelsI[i]->SetEnabled(kFALSE);
      LabelsI[i]->SetFrameDrawn(kTRUE);
      Column1[3*i+1]->AddFrame(LabelsI[i], new TGLayoutHints(kLHintsCenterX, 0, 0, 10, 0));


      LabelsO[i] = new TGTextEntry(Column1[3*i+2], new TGTextBuffer(100), 10000,
				   LabelsO[i]->GetDefaultGC()(),
				   LabelsO[i]->GetDefaultFontStruct(),
				   kRaisedFrame | kDoubleBorder, GetWhitePixel());
      LabelsO[i]->SetFont("-adobe-helvetica-bold-r-*-*-10-*-*-*-*-*-iso8859-1", false);
      if(i == 13)
        LabelsO[i]->SetText("ADC/MHz");
      else
	LabelsO[i]->SetText("OutRate/s");
      // LabelsO[i]->SetAlignment(kTextCenterX);
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
	  Column1[3*i]->AddFrame(Labels[16*i+j], new TGLayoutHints(kLHintsRight | kLHintsTop, 0, 0, 0, 0));
	  if(i == 13)
	    {
	      if(j < 13)
		Labels[16*i+j]->SetText(TString::Format("M%02d",j).Data());
	      else
		Labels[16*i+j]->SetText("");
	    }
	  else
	    {
	      Labels[16*i+j]->SetText(TString::Format("%02d",j).Data());
	    }
	  Labels[16*i+j]->Resize(30, 20);
	  Labels[16*i+j]->SetEnabled(kFALSE);
	  Labels[16*i+j]->SetFrameDrawn(kFALSE);
	  Labels[16*i+j]->ChangeBackground(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
	  
	  ICR[16*i+j] = new TGTextEntry(Column1[3*i+1], new TGTextBuffer(100), 10000,
				      ICR[16*i+j]->GetDefaultGC()(),
				      ICR[16*i+j]->GetDefaultFontStruct(),
				      kRaisedFrame | kDoubleBorder, GetWhitePixel());
	  ICR[16*i+j]->SetAlignment(kTextCenterX);
	  ICR[16*i+j]->SetText("");
	  // ICR[16*i+j]->Resize(35, 20);
	  ICR[16*i+j]->SetEnabled(kFALSE);
	  // ICR[16*i+j]->SetFrameDrawn(kFALSE);
	  Column1[3*i+1]->AddFrame(ICR[16*i+j], new TGLayoutHints(kLHintsExpandX, 0, 0, 0, 0));//kLHintsCenterX


	  OCR[16*i+j] = new TGTextEntry(Column1[3*i+2], new TGTextBuffer(100), 10000,
					OCR[16*i+j]->GetDefaultGC()(),
					OCR[16*i+j]->GetDefaultFontStruct(),
					kRaisedFrame | kDoubleBorder, GetWhitePixel());
	  OCR[16*i+j]->SetAlignment(kTextCenterX);
	  OCR[16*i+j]->SetText("");
	  OCR[16*i+j]->Resize(35, 20);
	  OCR[16*i+j]->SetEnabled(kFALSE);
	  // OCR[16*i+j]->SetFrameDrawn(kFALSE);
	  Column1[3*i+2]->AddFrame(OCR[16*i+j], new TGLayoutHints(kLHintsExpandX, 0, 0, 0, 0));
	}
      
    }

  ICR[221]->SetFont("-adobe-helvetica-bold-r-*-*-11-*-*-*-*-*-iso8859-1", false);
  ICR[222]->SetFont("-adobe-helvetica-bold-r-*-*-11-*-*-*-*-*-iso8859-1", false);
  ICR[223]->SetFont("-adobe-helvetica-bold-r-*-*-11-*-*-*-*-*-iso8859-1", false);
  ICR[221]->SetText("Used");
  ICR[222]->SetText("Available");
  ICR[223]->SetText("Use%");
  
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  // std::ifstream in("PixieOnline.config");
  // if(!in.is_open()) return;
  // char tmp[200];
  // in.getline(tmp,200);
  // filepathtext->Insert(tmp);
  // in.getline(tmp,200);
  // filenametext->Insert(tmp);
  // in.close();

}

void Online::MakeFold2Panel(TGCompositeFrame *TabPanel)
{
  TGGroupFrame *setgroup = new TGGroupFrame(TabPanel,"Setup");
  TabPanel->AddFrame(setgroup,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
  setgroup->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  TGHorizontalFrame *filepath = new TGHorizontalFrame(setgroup);
  setgroup->AddFrame(filepath,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
    
  TGLabel *filenamelabel = new TGLabel(filepath,"File Name: ");
  filepath->AddFrame(filenamelabel,new TGLayoutHints(kLHintsLeft | kLHintsTop, 10 ,3,4,0));

  alertfilenametext = new TGTextEntry(filepath, new TGTextBuffer(20));
  filepath->AddFrame(alertfilenametext,new TGLayoutHints(kLHintsLeft| kLHintsTop,10,3,4,0));
  alertfilenametext->SetText("alert.dat");

  alertread = new TGTextButton(filepath,"Read");
  alertread->Connect("Pressed()","Online",this,"AlertRead()");
  filepath->AddFrame(alertread,new TGLayoutHints(kLHintsLeft|kLHintsTop,10,3,4,4));

  alertsave = new TGTextButton(filepath,"Save");
  alertsave->Connect("Pressed()","Online",this,"AlertSave()");
  filepath->AddFrame(alertsave,new TGLayoutHints(kLHintsLeft|kLHintsTop,10,3,4,4));

  alertdefault = new TGTextButton(filepath,"Default");
  alertdefault->Connect("Pressed()","Online",this,"AlertDefault()");
  filepath->AddFrame(alertdefault,new TGLayoutHints(kLHintsLeft|kLHintsTop,10,3,4,4));


  
  
  // =================

  TGTextEntry **cl0 = new TGTextEntry *[14];
  TGTextEntry **LabelsLow = new TGTextEntry *[14];
  TGTextEntry **LabelsHigh = new TGTextEntry *[14];
  TGTextEntry **Labels = new TGTextEntry *[224];
  LowR = new TGNumberEntryField *[224];
  HighR = new TGNumberEntryField *[224];

  
  TGGroupFrame *limitgroup = new TGGroupFrame(TabPanel,"Count rate limit");
  TabPanel->AddFrame(limitgroup,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
  limitgroup->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  TGHorizontalFrame *horizontal1 = new TGHorizontalFrame(limitgroup);
  limitgroup->AddFrame(horizontal1,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
  TGHorizontalFrame *horizontal2 = new TGHorizontalFrame(limitgroup);
  limitgroup->AddFrame(horizontal2,new TGLayoutHints(kLHintsExpandX|kLHintsTop));

  TGVerticalFrame **Column2 = new TGVerticalFrame *[42];
  for (int i = 0; i < 14; i++)
    {
      if(i < 7)
	{
	  Column2[3*i] = new TGVerticalFrame(horizontal1, 200, 300);
	  horizontal1->AddFrame(Column2[3*i], new TGLayoutHints(kLHintsTop | kLHintsExpandX, 0, 0, 0, 0));
	  Column2[3*i+1] = new TGVerticalFrame(horizontal1, 200, 300);
	  horizontal1->AddFrame(Column2[3*i+1], new TGLayoutHints(kLHintsTop | kLHintsExpandX, 0, 0, 0, 0));
	  Column2[3*i+2] = new TGVerticalFrame(horizontal1, 200, 300);
	  horizontal1->AddFrame(Column2[3*i+2], new TGLayoutHints(kLHintsTop | kLHintsExpandX, 0, 0, 0, 0));
	}
      else
	{
	  Column2[3*i] = new TGVerticalFrame(horizontal2, 200, 300);
	  horizontal2->AddFrame(Column2[3*i], new TGLayoutHints(kLHintsTop | kLHintsExpandX, 0, 0, 0, 0));
	  Column2[3*i+1] = new TGVerticalFrame(horizontal2, 200, 300);
	  horizontal2->AddFrame(Column2[3*i+1], new TGLayoutHints(kLHintsTop | kLHintsExpandX, 0, 0, 0, 0));
	  Column2[3*i+2] = new TGVerticalFrame(horizontal2, 200, 300);
	  horizontal2->AddFrame(Column2[3*i+2], new TGLayoutHints(kLHintsTop | kLHintsExpandX, 0, 0, 0, 0));
	}

      cl0[i] = new TGTextEntry(Column2[3*i], new TGTextBuffer(100), 10000,
			       cl0[i]->GetDefaultGC()(),
			       cl0[i]->GetDefaultFontStruct(),
			       kRaisedFrame | kDoubleBorder, GetWhitePixel());
      cl0[i]->SetFont("-adobe-helvetica-bold-r-*-*-11-*-*-*-*-*-iso8859-1", false);
      if(i == 13)
	cl0[i]->SetText("File");
      else
	cl0[i]->SetText("ch #");
      cl0[i]->Resize(35, 20);
      cl0[i]->SetEnabled(kFALSE);
      cl0[i]->SetFrameDrawn(kTRUE);
      Column2[3*i]->AddFrame(cl0[i], new TGLayoutHints(kLHintsCenterX, 0, 0, 10, 0));


      LabelsLow[i] = new TGTextEntry(Column2[3*i+1], new TGTextBuffer(100), 10000,
      				   LabelsLow[i]->GetDefaultGC()(),
      				   LabelsLow[i]->GetDefaultFontStruct(),
      				   kRaisedFrame | kDoubleBorder, GetWhitePixel());
      LabelsLow[i]->SetFont("-adobe-helvetica-bold-r-*-*-10-*-*-*-*-*-iso8859-1", false);
      if(i == 13)
        LabelsLow[i]->SetText("Size/MB");
      else
	LabelsLow[i]->SetText("Low/s");
      // LabelsLow[i]->SetAlignment(kTextCenterX);
      LabelsLow[i]->Resize(90, 20);
      LabelsLow[i]->SetEnabled(kFALSE);
      LabelsLow[i]->SetFrameDrawn(kTRUE);
      Column2[3*i+1]->AddFrame(LabelsLow[i], new TGLayoutHints(kLHintsCenterX, 0, 0, 10, 0));


      LabelsHigh[i] = new TGTextEntry(Column2[3*i+2], new TGTextBuffer(100), 10000,
				   LabelsHigh[i]->GetDefaultGC()(),
				   LabelsHigh[i]->GetDefaultFontStruct(),
				   kRaisedFrame | kDoubleBorder, GetWhitePixel());
      LabelsHigh[i]->SetFont("-adobe-helvetica-bold-r-*-*-10-*-*-*-*-*-iso8859-1", false);
      if(i == 13)
        LabelsHigh[i]->SetText("");
      else
	LabelsHigh[i]->SetText("High/s");
      // LabelsHigh[i]->SetAlignment(kTextCenterX);
      LabelsHigh[i]->Resize(90, 20);
      LabelsHigh[i]->SetEnabled(kFALSE);
      LabelsHigh[i]->SetFrameDrawn(kTRUE);
      Column2[3*i+2]->AddFrame(LabelsHigh[i], new TGLayoutHints(kLHintsCenterX, 0, 0, 10, 0));

      

      for (int j = 0; j < 16; j++)
	{
	  Labels[16*i+j] = new TGTextEntry(Column2[3*i], new TGTextBuffer(100), 10000,
					   Labels[16*i+j]->GetDefaultGC()(),
					   Labels[16*i+j]->GetDefaultFontStruct(),
					   kRaisedFrame | kDoubleBorder,
					   GetWhitePixel());
	  if(i == 13)
	    {
	      if(j < 13)
		Labels[16*i+j]->SetText(TString::Format("M%02d",j).Data());
	      else
		Labels[16*i+j]->SetText("");
	    }
	  else
	    {
	      Labels[16*i+j]->SetText(TString::Format("%02d",j).Data());
	    }
	  Labels[16*i+j]->Resize(35, 20);
	  Labels[16*i+j]->SetEnabled(kFALSE);
	  Labels[16*i+j]->SetFrameDrawn(kTRUE);
	  Column2[3*i]->AddFrame(Labels[16*i+j], new TGLayoutHints(kLHintsCenterX, 0, 0, 0, 0));


	  LowR[16*i+j] = new TGNumberEntryField(Column2[3*i+1], -1, 0, LowR[16*i+j]->GetDefaultGC()(),LowR[16*i+j]->GetDefaultFontStruct(),kRaisedFrame | kDoubleBorder, GetWhitePixel());
	  LowR[16*i+j]->SetAlignment(kTextCenterX);
	  LowR[16*i+j]->SetText("0");
	  LowR[16*i+j]->Resize(35, 20);
	  // LowR[16*i+j]->SetEnabled(kFALSE);
	  // LowR[16*i+j]->SetFrameDrawn(kFALSE);
	  Column2[3*i+1]->AddFrame(LowR[16*i+j], new TGLayoutHints(kLHintsExpandX, 0, 0, 0, 0));//kLHintsCenterX


	  HighR[16*i+j] = new TGNumberEntryField(Column2[3*i+2], -1, 0, HighR[16*i+j]->GetDefaultGC()(),HighR[16*i+j]->GetDefaultFontStruct(),kRaisedFrame | kDoubleBorder, GetWhitePixel());
	  HighR[16*i+j]->SetAlignment(kTextCenterX);
	  HighR[16*i+j]->SetText("0");
	  HighR[16*i+j]->Resize(35, 20);
	  // HighR[16*i+j]->SetEnabled(kFALSE);
	  // HighR[16*i+j]->SetFrameDrawn(kFALSE);
	  Column2[3*i+2]->AddFrame(HighR[16*i+j], new TGLayoutHints(kLHintsExpandX, 0, 0, 0, 0));
	}
      
    }


  
}


void Online::MakeFold3Panel(TGCompositeFrame *TabPanel)
{
  TGCompositeFrame *parFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);
  TabPanel->AddFrame(parFrame, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 2, 2, 1, 1));
  parFrame->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  // draw
  OnlineDrawButton3 = new TGTextButton(parFrame, "&Draw", ONLINEDRAW3);
  // OnlineDrawButton3->SetEnabled(0);
  OnlineDrawButton3->Associate(this);
  parFrame->AddFrame(OnlineDrawButton3, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 30, 0, 0));

  // ch
  onlinechnum3 = new TGNumberEntry (parFrame, 0, 2, ONLINECHNUM3, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, 15);
  parFrame->AddFrame(onlinechnum3, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 10, 0, 0));
  onlinechnum3->SetButtonToNum(0);
  onlinechnum3->Associate(this);
  
  TGLabel *ch = new TGLabel(parFrame, "Ch:"); 
  parFrame->AddFrame(ch, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 2, 3, 0));
  ch->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  onlinemodnum3 = new TGNumberEntry (parFrame, 0, 2, ONLINEMODNUM3, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, PRESET_MAX_MODULES-1);
  parFrame->AddFrame(onlinemodnum3, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 10, 0, 0));
  onlinemodnum3->SetButtonToNum(0);
  onlinemodnum3->Associate(this);
  
  TGLabel *mod = new TGLabel(parFrame, "Mod:"); 
  parFrame->AddFrame(mod, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 2, 3, 0));
  mod->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));


  chooseenergycanvasmode3 = new TGComboBox(parFrame);
  parFrame->AddFrame(chooseenergycanvasmode3, new TGLayoutHints(kLHintsRight, 0, 10, 0, 0));
  chooseenergycanvasmode3->Resize(150, 20);
  chooseenergycanvasmode3->AddEntry("Single Channel Mode", 0);
  chooseenergycanvasmode3->AddEntry("Multi Channel Mode", 1);
  chooseenergycanvasmode3->Select(0);
  
  TGLabel *choosemode = new TGLabel(parFrame, "Mode:"); 
  parFrame->AddFrame(choosemode, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 2, 3, 0));
  choosemode->SetTextColor(TColor::RGB2Pixel(COLOR_BLUE_R,COLOR_BLUE_G,COLOR_BLUE_B), false);
  choosemode->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGCompositeFrame *adCanvasFrame = new TGCompositeFrame(TabPanel, 800, 800, kHorizontalFrame);
  TGLayoutHints *Hint = new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 1, 1, 1, 1);

  TRootEmbeddedCanvas *adjCanvas = new TRootEmbeddedCanvas("canvas3", adCanvasFrame, 100, 100);

  canvas3 = adjCanvas->GetCanvas();
  adCanvasFrame->AddFrame(adjCanvas, Hint);
  TabPanel->AddFrame(adCanvasFrame, Hint);


  
}

void Online::Panel3Draw()
{
  for (int i = 0; i < 16; ++i)
    {
      if(onlineth1i3[i] == NULL)
	{
	  char th1iname[16];
	  sprintf(th1iname,"ch%02d",i);
	  onlineth1i3[i] = new TH1I(th1iname,"",32768,0,32768);
	  onlineth1i3[i]->SetTitle(TString::Format("Ch: %d",i).Data());
	  onlineth1i3[i]->GetXaxis()->SetTitle("ch");
	  onlineth1i3[i]->GetXaxis()->SetLabelSize(0.04);
	  onlineth1i3[i]->GetYaxis()->SetLabelSize(0.04);
	}
      else
	{
	  onlineth1i3[i]->Reset("ICES");
	}
    }
  
  
  canvas3->cd();
  canvas3->Clear();

  memcpy(EnergySpec,ptr+SHAREDMEMORYDATAOFFSET+PRESET_MAX_MODULES*2+4*SHAREDMEMORYDATASTATISTICS*PRESET_MAX_MODULES+onlinemodnum3->GetIntNumber()*4*SHAREDMEMORYDATAENERGYLENGTH*SHAREDMEMORYDATAMAXCHANNEL,4*SHAREDMEMORYDATAENERGYLENGTH*SHAREDMEMORYDATAMAXCHANNEL);
  
  switch(chooseenergycanvasmode3->GetSelected())
    {
    case 0://single
      for (int j = 0; j < SHAREDMEMORYDATAENERGYLENGTH; ++j)
	{
	  onlineth1i3[onlinechnum3->GetIntNumber()]->SetBinContent(j+1, Double_t(EnergySpec[onlinechnum3->GetIntNumber()*SHAREDMEMORYDATAENERGYLENGTH+j]));
	}
      onlineth1i3[onlinechnum3->GetIntNumber()]->Draw();
      break;

    case 1://multi
      canvas3->Divide(4,4);

      for (int i = 0; i < 16; ++i)
	{
	  canvas3->cd(i+1);
	  for (int j = 0; j < SHAREDMEMORYDATAENERGYLENGTH; ++j)
	    {
	      onlineth1i3[i]->SetBinContent(j+1, Double_t(EnergySpec[i*SHAREDMEMORYDATAENERGYLENGTH+j]));
	    }
	  onlineth1i3[i]->Draw();
	}
      break;


    default:
      break;
    }

  canvas3->Modified();
  canvas3->Update();
  gSystem->ProcessEvents();
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
  int OldRunNUmber = -1;
  std::stringstream ss;//sstream cstring
  ss.clear();//

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
		  sprintf(Filename[i],"%s%04d/%s_R%04d_M%02d.bin",filepathtext->GetText(),RunNumber,filenametext->GetText(),RunNumber,i);
		  if(IsFileExists(Filename[i]))
		    {
		      filesize = GetFileSizeMB(Filename[i]);
		      if(LowR[208+i]->GetNumber() > 0 && filesize > LowR[208+i]->GetNumber())
			{
			  ICR[208+i]->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
			}
		      else
			{
			  ICR[208+i]->SetTextColor(TColor::RGB2Pixel(COLOR_BLACK_R,COLOR_BLACK_G,COLOR_BLACK_B), false);
			}

		      ss.clear();
		      ss<<filesize;
		      ss>>tempstring;
		      ICR[208+i]->SetText(tempstring.c_str());
		    }
		}
	      OCR[221]->SetText(gSystem->GetFromPipe(TString::Format("df -hl %s | awk 'NR>1{print $3}'",filepathtext->GetText()).Data()).Data());
	      OCR[222]->SetText(gSystem->GetFromPipe(TString::Format("df -hl %s | awk 'NR>1{print $4}'",filepathtext->GetText()).Data()).Data());
	      OCR[223]->SetText(gSystem->GetFromPipe(TString::Format("df -hl %s | awk 'NR>1{print $5}'",filepathtext->GetText()).Data()).Data());
	    }
	  PrevTime = CurrentTime;
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
	      PrevProtectionTime = get_time();
	      // std::cout<<"get time 1"<<std::endl;
		  
	      memcpy(buf_new,ptr,(PRESET_MAX_MODULES*SHAREDMEMORYDATASTATISTICS*4)+PRESET_MAX_MODULES*2+SHAREDMEMORYDATAOFFSET);
	      if(number == UINT_MAX) 
		{
		  number = tempN;
		  memcpy(buf,buf_new,(PRESET_MAX_MODULES*SHAREDMEMORYDATASTATISTICS*4)+PRESET_MAX_MODULES*2+SHAREDMEMORYDATAOFFSET);
		  continue;
		}
	      number = tempN;

	      char filepath[1024];
	      char filename[128];
	      memcpy(filename,buf_new+14,128);
	      memcpy(filepath,buf_new+142,1024);
	      filepathtext->SetText(filepath);
	      filenametext->SetText(filename);

	      // =======
	      memcpy(&ModNum,buf_new+4,2);
	      memcpy(&RunNumber,buf_new+6,4);
	      // printf("RunNumber: %d -- Num: %d\n",RunNumber,number);
	      if(OldRunNUmber != RunNumber)
		{
		  for (int i = 0; i < 208; ++i)
		    {
		      ICR[i]->SetText("0");
		      OCR[i]->SetText("0");
		    }
		  OldRunNUmber = RunNumber;
		}
		
	      for(int i = 0; i < ModNum;i++)
		{
		  memcpy(&tempsampingrate,buf_new+SHAREDMEMORYDATAOFFSET+2*i,2);
		  ss.clear();
		  ss<<tempsampingrate;
		  ss>>tempstring;
		  OCR[208+i]->SetText(tempstring.c_str());
		  
		  memcpy(Statistics,buf+SHAREDMEMORYDATAOFFSET+PRESET_MAX_MODULES*2+4*SHAREDMEMORYDATASTATISTICS*i,SHAREDMEMORYDATASTATISTICS*4);
		  memcpy(Statistics_new,buf_new+SHAREDMEMORYDATAOFFSET+PRESET_MAX_MODULES*2+4*SHAREDMEMORYDATASTATISTICS*i,SHAREDMEMORYDATASTATISTICS*4);

		  RealTime = (double)Statistics[2] * pow(2.0, 32.0);
		  RealTime += (double)Statistics[3];
		  RealTime *= 1.0e-6 / (double)SYSTEM_CLOCK_MHZ;

		  RealTime_new = (double)Statistics_new[2] * pow(2.0, 32.0);
		  RealTime_new += (double)Statistics_new[3];
		  RealTime_new *= 1.0e-6 / (double)SYSTEM_CLOCK_MHZ;
		  
		  for(int j=0;j<16;j++)
		    {
		      LiveTime[j] = (double)Statistics[63+j] * pow(2.0, 32.0);
		      LiveTime[j] += (double)Statistics[79+j];
		      
		      FastPeaks[j] = (double)Statistics[95+j] * pow(2.0, 32.0);
		      FastPeaks[j] += (double)Statistics[111+j];
		      ChanEvents[j] = (double)Statistics[223+j] * pow(2.0, 32.0);
		      ChanEvents[j] += (double)Statistics[239+j];

		      LiveTime_new[j] = (double)Statistics_new[63+j] * pow(2.0, 32.0);
		      LiveTime_new[j] += (double)Statistics_new[79+j];

		      if(tempsampingrate == 100)
			{
			  LiveTime[j] *= 1.0e-6 / (double)tempsampingrate;
			  LiveTime_new[j] *= 1.0e-6 / (double)tempsampingrate;
			}
		      else if(tempsampingrate == 250)
			{
			  LiveTime[j] *= 2.0 * 1.0e-6 / (double)tempsampingrate;
			  LiveTime_new[j] *= 2.0 * 1.0e-6 / (double)tempsampingrate;
			}
		      else if(tempsampingrate == 500)
			{
			  LiveTime[j] *= 5.0 * 1.0e-6 / (double)tempsampingrate;
			  LiveTime_new[j] *= 5.0 * 1.0e-6 / (double)tempsampingrate;
			}
		      
		      FastPeaks_new[j] = (double)Statistics_new[95+j] * pow(2.0, 32.0);
		      FastPeaks_new[j] += (double)Statistics_new[111+j];
		      ChanEvents_new[j] = (double)Statistics_new[223+j] * pow(2.0, 32.0);
		      ChanEvents_new[j] += (double)Statistics_new[239+j];

		      // std::cout<<j<<"  "<<LiveTime_new[j] <<"  "<<(FastPeaks_new[j]-FastPeaks[j])/(LiveTime_new[j]-LiveTime[j])<<"  "<<(ChanEvents_new[j]-ChanEvents[j])/(RealTime_new-RealTime)<<std::endl;
		      tempinputcountrate = int((FastPeaks_new[j]-FastPeaks[j])/(LiveTime_new[j]-LiveTime[j]) + 0.5);
		      tempoutputcountrate = int((ChanEvents_new[j]-ChanEvents[j])/(RealTime_new-RealTime) +0.5);

		      if(LowR[16*i+j]->GetNumber() > 0 && tempinputcountrate < LowR[16*i+j]->GetNumber())
			{
			  ICR[16*i+j]->SetTextColor(TColor::RGB2Pixel(COLOR_BLUE_R,COLOR_BLUE_G,COLOR_BLUE_B), false);
			}
		      else if(HighR[16*i+j]->GetNumber() > 0 && tempinputcountrate > HighR[16*i+j]->GetNumber())
			{
			  ICR[16*i+j]->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
			}
		      else
			{
			  ICR[16*i+j]->SetTextColor(TColor::RGB2Pixel(COLOR_BLACK_R,COLOR_BLACK_G,COLOR_BLACK_B), false);
			}

		      ss.clear();
		      ss<<tempinputcountrate;
		      ss>>tempstring;
		      ICR[16*i+j]->SetText(tempstring.c_str());
		      
		      ss.clear();
		      ss<<tempoutputcountrate;
		      ss>>tempstring;
		      OCR[16*i+j]->SetText(tempstring.c_str());
		    }
		}

	      memcpy(buf,buf_new,(PRESET_MAX_MODULES*SHAREDMEMORYDATASTATISTICS*4)+PRESET_MAX_MODULES*2+SHAREDMEMORYDATAOFFSET);

	      sprintf(charrunstate,"R%04d     M%02d",RunNumber,ModNum);
	      StateMsg->SetText(charrunstate);
	      if(flagrunnumber)
		{
		  StateMsg->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
		  flagrunnumber = false;
		}
	      else
		{
		  StateMsg->SetTextColor(TColor::RGB2Pixel(COLOR_BLUE_R,COLOR_BLUE_G,COLOR_BLUE_B), false);
		  flagrunnumber = true;
		}
	    }
	}
      else
	{
	  // std::cout<<"get time 2"<<std::endl;
	  CurrentProtectionTime = get_time();
	  ElapsedProtectionTime = CurrentProtectionTime - PrevProtectionTime;
	  if(ElapsedProtectionTime > 5000)
	    {
	      StateMsg->SetText("NOT SHARE DATA");
	      StateMsg->SetTextColor(TColor::RGB2Pixel(COLOR_YELLOW_R,COLOR_YELLOW_G,COLOR_YELLOW_B), false);
	    }
	  
	}

      gSystem->ProcessEvents();

      usleep(400000);//sleep 400us
    }
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

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
