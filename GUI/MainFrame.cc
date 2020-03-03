// MainFrame.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 3月  9 13:01:33 2018 (+0800)
// Last-Updated: 二 3月  3 14:09:34 2020 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 403
// URL: http://wuhongyi.cn 

#include "MainFrame.hh"
#include "Csra.hh"
#include "Detector.hh"
#include "Global.hh"

#include "pixie16app_export.h"
#include "pixie16sys_export.h"

#include "TGResourcePool.h"
#include "TGGC.h"

#include <fstream>
#include <unistd.h>
#include <sys/stat.h>//stat(const char *file_name,struct stat *buf)
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ClassImp(MainFrame)

const char *filetypes[] =
  { "Set Files", "*.set", "all files", "*.*", 0, 0 };

MainFrame::MainFrame(const TGWindow * p)
{
  ///initialise variables///////////////

  detector = NULL; 
  runnum = 0;

  SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  // gClient->GetHilite(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  // gClient->GetShadow(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  // gClient->GetResourcePool()->GetFrameHiliteGC();
  // gClient->GetResourcePool()->GetFrameBckgndGC();
  // gClient->GetResourcePool()->GetFrameShadowGC();
  // gClient->GetResourcePool()->GetFocusHiliteGC();
  // gClient->GetResourcePool()->GetSelectedGC();
  // gClient->GetResourcePool()->GetSelectedBckgndGC();
    
  CreateMenuBar();
  ControlPanel(this);

  
  SetWindowName("GDDAQ");
  MapSubwindows();
  MapWindow();
  Resize(INITIAL_WIDTH, INITIAL_HIGHT);

  // AppendPad(); //foarte important

  flagonlinemode = 0;
}

MainFrame::~MainFrame()
{
  std::cout<<"destructor of MainFrame is called!"<<std::endl;
}

void MainFrame::CreateMenuBar()
{

  MenuFile = new TGPopupMenu(fClient->GetRoot());
  MenuFile->AddEntry("E&xit", FILE_EXIT,0,gClient->GetPicture("bld_exit.png"));
  MenuFile->AddSeparator();
  MenuFile->AddEntry("&About", ABOUT,0,gClient->GetPicture("ed_help.png"));
  MenuFile->Associate(this);
  MenuFile->SetBackgroundColor(TColor::RGB2Pixel(0,128,0));


  MenuSetup = new TGPopupMenu(fClient->GetRoot());
  MenuSetup->AddEntry("&Base Setup", BASE);
  MenuSetup->AddEntry("&Trigger Filter", TFILTER);
  MenuSetup->AddEntry("&Energy", ENERGY);
  MenuSetup->AddEntry("&CFD", CFDP);
  MenuSetup->AddEntry("&QDC", QDCP);
  MenuSetup->AddEntry("&Decimation", DECIMATIONP);
  MenuSetup->AddSeparator();
  MenuSetup->AddEntry("&Copy Pars", COPYPARS);
  MenuSetup->AddSeparator();
  MenuSetup->AddEntry("Save2File", FILE_SAVE,0,gClient->GetPicture("save.xpm"));
  MenuSetup->Associate(this);
  MenuSetup->SetBackgroundColor(TColor::RGB2Pixel(0,128,0));
  
  MenuExpert = new TGPopupMenu(fClient->GetRoot());
  MenuExpert->AddEntry("Module Variables", MODVAR);
  MenuExpert->AddEntry("&CSRA", CSRA);
  MenuExpert->AddEntry("&Logic Set", LOGIC);
  MenuExpert->Associate(this);
  MenuExpert->SetBackgroundColor(TColor::RGB2Pixel(0,128,0));

  MenuMonitor = new TGPopupMenu(fClient->GetRoot());
  MenuMonitor->AddEntry("&Hist & XDT", HISTXDT);
  MenuMonitor->AddEntry("&Trace & Baseline", READCHANSTATUS);
  MenuMonitor->Associate(this);
  MenuMonitor->SetBackgroundColor(TColor::RGB2Pixel(0,128,0));

  MenuOffline = new TGPopupMenu(fClient->GetRoot());
  MenuOffline->AddEntry("Pars optimization", OFFLINEADJUSTPAR);
  MenuOffline->AddEntry("Simulation", SIMULATION);
  MenuOffline->Associate(this);
  MenuOffline->SetBackgroundColor(TColor::RGB2Pixel(0,128,0));


  // MenuBar->GetTitles()->Print();
  
  // (TGMenuTitle*)(MenuBar->GetTitles()->FindObject("Offline"))->Print();

  
  // ((TGMenuTitle*)MenuBar->GetTitles()->At(1))->SetTextColor(TColor::RGB2Pixel(TOPBAR_TEXT_R,TOPBAR_TEXT_G,TOPBAR_TEXT_B));
  // ((TGMenuTitle*)MenuBar->GetTitles()->At(2))->SetTextColor(TColor::RGB2Pixel(TOPBAR_TEXT_R,TOPBAR_TEXT_G,TOPBAR_TEXT_B));
  // ((TGMenuTitle*)MenuBar->GetTitles()->At(3))->SetTextColor(TColor::RGB2Pixel(TOPBAR_TEXT_R,TOPBAR_TEXT_G,TOPBAR_TEXT_B));
  // ((TGMenuTitle*)MenuBar->GetTitles()->At(4))->SetTextColor(TColor::RGB2Pixel(TOPBAR_TEXT_R,TOPBAR_TEXT_G,TOPBAR_TEXT_B));


  TGMenuBar *MenuBar = new TGMenuBar(this, INITIAL_WIDTH, 24, kHorizontalFrame);
  AddFrame(MenuBar, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX, 0, 0, 0, 0));
  MenuBar->ChangeBackground(TColor::RGB2Pixel(TOPBAR_BG_R,TOPBAR_BG_G,TOPBAR_BG_B));
  // MenuBar->SetTextColor(TColor::RGB2Pixel(TOPBAR_TEXT_R,TOPBAR_TEXT_G,TOPBAR_TEXT_B),false);
  MenuBar->AddPopup(" &File  ", MenuFile, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  MenuBar->AddPopup(" &Base  ", MenuSetup, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  MenuBar->AddPopup("&Expert ", MenuExpert, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  MenuBar->AddPopup(" &Debug ", MenuMonitor, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  MenuBar->AddPopup("&Offline", MenuOffline, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  // std::cout<<MenuBar->GetTitles()->GetSize()<<std::endl;
  // for (int i = 0; i < MenuBar->GetTitles()->GetSize(); ++i)
  //   {
  //     ((TGMenuTitle*)MenuBar->GetTitles()->At(i))->SetTextColor(TColor::RGB2Pixel(TOPBAR_TEXT_R,TOPBAR_TEXT_G,TOPBAR_TEXT_B));
  //     ((TGMenuTitle*)MenuBar->GetTitles()->At(i))->ChangeBackground(TColor::RGB2Pixel(7,7,7));
  //     ((TGMenuTitle*)MenuBar->GetTitles()->At(i))->Print();
  //   }

  // TList * list = MenuBar->GetTitles();
  // TGMenuTitle* point = (TGMenuTitle*)list->First();
  // for (int i = 0; i < list->GetSize(); ++i)
  //   {
  //     point->SetTextColor(TColor::RGB2Pixel(TOPBAR_TEXT_R,TOPBAR_TEXT_G,TOPBAR_TEXT_B));
  //     point->ChangeBackground(TColor::RGB2Pixel(7,7,7));
  //     point = (TGMenuTitle*)list->After(point);
  //   }

  // TGMenuTitle *idcur = (TGMenuTitle*)MenuBar->GetTitles()->At(0);
  // while (idcur) {
  //   idcur->Print();
  //   idcur->SetTextColor(TColor::RGB2Pixel(TOPBAR_TEXT_R,TOPBAR_TEXT_G,TOPBAR_TEXT_B));
  //   idcur = (TGMenuTitle*)MenuBar->GetTitles()->After(idcur);
  // }

  
  
  SetMenuStatus(false,flagonlinemode);

}

void MainFrame::CloseWindow()
{
  if(detector != NULL) delete detector;
  gApplication->Terminate(0);
}

Bool_t MainFrame::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
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

	    case ABOUT:
	     about =  new TRootHelpDialog(this, "About GDDAQ", 550, 250);
	     about->SetText(gAbout);
	     about->Popup();
	      break;
	    case BASE:
	      base = new Base(fClient->GetRoot(), this, (char*)"Base Setup", 6, 16, detector);
	      base->load_info(0);
	      break;
	    case CFDP:
	      cfd = new Cfd(fClient->GetRoot(),this,(char*)"Cfd Par.", 4, 16, detector);
	      cfd->load_info(0);
	      break;
	    case QDCP:
	      qdc=new Qdc(fClient->GetRoot(),this,(char*)"Qdc Par.", 9, 16, detector);
	      qdc->load_info(0);
	      break;
	    case DECIMATIONP:
	      decimation = new Decimation(fClient->GetRoot(),this,(char*)"Waveform Decimation", 2, 16, detector);
	      decimation->load_info(0);
	      break;
	    case ENERGY:
	      energy = new Energy(fClient->GetRoot(), this, (char*)"Energy", 5, 16, detector);
	      energy->load_info(0);
	      break;
	    case CSRA:
	      csra = new Csra(fClient->GetRoot(), this,detector);
	      csra->load_info(0);
	      break;
	    case TFILTER:
	      triggerfilter = new TriggerFilter(fClient->GetRoot (), this, (char*)"Trigger Filter", 4, 16, detector);
	      triggerfilter->load_info(0);
	      break;
	    case MODVAR:
	      expertmod = new ExpertMod(fClient->GetRoot(), this, (char*)"Expert MOD", detector);
	      expertmod->load_info(0);
	      break;
	    case LOGIC:
	      logictrigger = new LogicTrigger(fClient->GetRoot(),this,(char*)"Logic Trigger", 16/*7*/, 16, detector);
	      logictrigger->load_info(0);
	      break;
	    case HISTXDT:
	      histxdt = new HistXDT(fClient->GetRoot(), this, (char*)"Hist & XDT", 4, 16, detector);
	      histxdt->load_info(0);
	      break;
	    case OFFLINEADJUSTPAR:
	      popupoffline = new Offline(fClient->GetRoot(), this, detector,filepathtext,filenametext);
	      break;
	    case COPYPARS:
	      copypars = new CopyPars(fClient->GetRoot(), this, detector);
	      break;
	    case READCHANSTATUS:
	      readchanstatus = new ReadChanStatus(fClient->GetRoot(), this, detector);
	      break;
	    case SIMULATION:
	      simulation = new Simulation(fClient->GetRoot(), this/*, detector,filepathtext,filenametext*/); //TODO
	      break;
	      
	    case FILE_SAVE:
	      {
	    	static TString dir2(".");
	    	TGFileInfo fInput2;
	    	fInput2.fFileTypes = filetypes;
	    	fInput2.fIniDir = StrDup(dir2);
	    	new TGFileDialog(fClient->GetRoot(), this, kFDSave, &fInput2);
	    	save_setup(fInput2.fFilename);
	    	//    cout<<"bingo\n\n\n";
	      }
	      break;

	    default:
	      break;
	    }
	case kCM_BUTTON:
	  switch (parm1)
	    {	      
	    case BOOT_BUTTON:
	      bootB->SetEnabled(0);
	      filesetdone->SetEnabled(0);
	      StateMsgFold1->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
	      StateMsgFold1->SetText("Booting... Wait a moment");
	      gSystem->ProcessEvents();
	      // gPad->SetCursor(kWatch);
	      if(detector != 0) delete detector;
	      detector = new Detector(flagonlinemode);
	      detector->SetRecordFlag(true);
	      recordchk->SetState(kButtonDown);
#ifdef DECODERONLINE	      
	      detector->SetDecoterFlag(false);
	      decoderchk->SetState(kButtonUp);
#endif
	      detector->SetRunFlag(true);
	      if(detector->BootSystem())
		{
		  StateMsgFold1->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
		  StateMsgFold1->SetText("Booted System");
		  // gPad->SetCursor(kPointer);

		  SetMenuStatus(true,flagonlinemode);
		  filesetdone->SetEnabled(1);
		}
	      else
		{
		  StateMsgFold1->SetTextColor(TColor::RGB2Pixel(COLOR_BLUE_R,COLOR_BLUE_G,COLOR_BLUE_B), false);
		  StateMsgFold1->SetText("Boot Failed...");
		  bootB->SetEnabled(1);
		}
	      detector->SetRunFlag(false);
	      break;

	    default:
	      break;
	    }
	}
    case kC_TEXTENTRY:
      switch (parm1)
	{

	default:
	  break;
	}
        
    default:
      // selected = gPad->GetSelected();
      break;
    }
  return kTRUE;
}


void MainFrame::save_setup(char *name)
{
  int retval;
  retval = Pixie16SaveDSPParametersToFile(name);
  if(retval < 0)
    ErrorInfo("MainFrame.cc", "save_setup(...)", "Pixie16SaveDSPParametersToFile", retval);
  std::cout << "saving setup to file: " << name << std::endl;
}

void MainFrame::ControlPanel(TGCompositeFrame *TabPanel)
{
  TabPanel->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGCompositeFrame *TitleFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);
  TabPanel->AddFrame(TitleFrame, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  TitleFrame->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));


  TGTextEntry *titleinfor = new TGTextEntry(TitleFrame,new TGTextBuffer(300), 10000);
  TitleFrame->AddFrame(titleinfor, new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 0, 0, 0));
  titleinfor->SetAlignment(kTextCenterX);
  titleinfor->SetFont(TITLE_FONT, false);
  titleinfor->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
  titleinfor->SetText("List Mode");
  titleinfor->Resize(INITIAL_WIDTH, TITLE_LISTMODE_HIGHT);
  titleinfor->SetEnabled(kFALSE);
  titleinfor->SetFrameDrawn(kFALSE);
  titleinfor->ChangeBackground(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGGroupFrame *bootframe = new TGGroupFrame(TabPanel,"");
  TabPanel->AddFrame(bootframe,new TGLayoutHints(kLHintsExpandX|kLHintsTop,INITIAL_SIDE_WIDTH,INITIAL_SIDE_WIDTH,0,0));
  bootframe->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  // bootframe->SetTextColor(TColor::RGB2Pixel(0,0,0));
  // bootframe->DrawBorder();



  TGCompositeFrame *LogoFrame = new TGCompositeFrame(bootframe, 0, 0, kHorizontalFrame);
  bootframe->AddFrame(LogoFrame, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 10, 10));
  LogoFrame->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  TGImageMap* fImagePKU = new TGImageMap(LogoFrame, "../icons/logo1.png");
  LogoFrame->AddFrame(fImagePKU,new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 0, 0, 0));
  fImagePKU->ChangeOptions(fImagePKU->GetOptions() ^ kRaisedFrame);
  fImagePKU->Resize(100,100);
  fImagePKU->ChangeOptions(fImagePKU->GetOptions() | kFixedSize);
  fImagePKU->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  TGImageMap* fImageWHY = new TGImageMap(LogoFrame, "../icons/logo2.png");
  LogoFrame->AddFrame(fImageWHY,new TGLayoutHints(kLHintsTop | kLHintsRight, 70, 0, 0, 0));
  fImageWHY->ChangeOptions(fImageWHY->GetOptions() ^ kRaisedFrame);
  fImageWHY->Resize(100,100);
  fImageWHY->ChangeOptions(fImageWHY->GetOptions() | kFixedSize);
  fImageWHY->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));


  
  //make the buttons frame        
  TGCompositeFrame *ButtonFrame = new TGCompositeFrame(bootframe, 0, 0, kHorizontalFrame);
  bootframe->AddFrame(ButtonFrame, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 0, 0, 7, 7));  
  ButtonFrame->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  // Online mode
  onlinemode = new TGCheckButton(ButtonFrame,"Online Mode");
  onlinemode->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  onlinemode->SetTextColor(TColor::RGB2Pixel(CHECKBUTTON_TEXT_R,CHECKBUTTON_TEXT_G,CHECKBUTTON_TEXT_B));
  onlinemode->SetFont(CHECKBUTTON_FONT, false);
  onlinemode->SetState(kButtonDown);
  onlinemode->Connect("Clicked()","MainFrame",this,"SetOnlineMode()");
  ButtonFrame->AddFrame(onlinemode,new TGLayoutHints(kLHintsLeft|kLHintsTop,0,0,0,0));
  
  // BOOT button//////////////////////////////////////////////////////////////    
  bootB = new TGTextButton(ButtonFrame, "  Boot  ", BOOT_BUTTON);
  ButtonFrame->AddFrame(bootB, new TGLayoutHints(kLHintsRight | kLHintsTop, 0,0,0,0));
  bootB->ChangeOptions(bootB->GetOptions() ^ kRaisedFrame);
  bootB->SetFont(TEXTBUTTON_FONT, false);
  bootB->Resize(75,24);
  bootB->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  bootB->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));
  bootB->Associate(this);


  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  // TGHorizontalFrame *splitline1 = new TGHorizontalFrame(TabPanel,INITIAL_WIDTH, 30,kFixedHeight);
  // TabPanel->AddFrame(splitline1, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 20, 20));
  // splitline1->SetBackgroundColor(TColor::RGB2Pixel(0,0,0));
  TGHSplitter *hsplitter1 = new TGHSplitter(TabPanel,INITIAL_WIDTH,2);
  // hsplitter->SetFrame(bootframe, kTRUE);
  TabPanel->AddFrame(hsplitter1, new TGLayoutHints(kLHintsTop | kLHintsExpandX,0,0,10,10));
  hsplitter1->SetBackgroundColor(TColor::RGB2Pixel(0,0,0));
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  
  // Set up for file store parametrs
  TGCompositeFrame *filesetgroup = new TGCompositeFrame(TabPanel,0,0);
 TabPanel->AddFrame(filesetgroup,new TGLayoutHints(kLHintsExpandX|kLHintsTop,INITIAL_SIDE_WIDTH,INITIAL_SIDE_WIDTH,0,0));
  filesetgroup->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  TGTextEntry *labelsetup = new TGTextEntry(filesetgroup,new TGTextBuffer(300), 10000);
  filesetgroup->AddFrame(labelsetup, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 0, TITLE_TOP_HIGHT, TITLE_BOTTON_HIGHT));
  labelsetup->SetFont(TITLE_FONT, false);
  labelsetup->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
  labelsetup->SetText("Setup");
  labelsetup->SetEnabled(kFALSE);
  labelsetup->SetFrameDrawn(kFALSE);
  labelsetup->ChangeBackground(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));


  TGLabel *filepathlabel = new TGLabel(filesetgroup,"File Path: ");
  filesetgroup->AddFrame(filepathlabel,new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 0, 0, 0));
  filepathlabel->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  filepathlabel->SetTextColor(TColor::RGB2Pixel(LABEL_TEXT_R,LABEL_TEXT_G,LABEL_TEXT_B),false);
  filepathlabel->SetTextFont(LABEL_FONT,false);

  filepathtext = new TGTextEntry(filesetgroup,new TGTextBuffer(20));
  filesetgroup->AddFrame(filepathtext,new TGLayoutHints(kLHintsExpandX|kLHintsTop, 0 ,0,4,0));
  filepathtext->ChangeBackground(TColor::RGB2Pixel(TEXTENTRY_BG_R,TEXTENTRY_BG_G,TEXTENTRY_BG_B));
  filepathtext->SetFrameDrawn(kFALSE);
  filepathtext->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
  filepathtext->SetFont(TEXTENTRY_FONT,false);
  filepathtext->Resize(INITIAL_CONTANT_WIDTH,28);
  

  TGLabel *filenamelabel = new TGLabel(filesetgroup,"File Name: ");
  filesetgroup->AddFrame(filenamelabel,new TGLayoutHints(kLHintsLeft | kLHintsTop, 0 ,0,10,0));
  filenamelabel->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  filenamelabel->SetTextColor(TColor::RGB2Pixel(LABEL_TEXT_R,LABEL_TEXT_G,LABEL_TEXT_B),false);
  filenamelabel->SetTextFont(LABEL_FONT,false);

  filenametext = new TGTextEntry(filesetgroup, new TGTextBuffer(20));
  filesetgroup->AddFrame(filenametext,new TGLayoutHints(kLHintsExpandX| kLHintsTop,0,0,4,0));
  filenametext->ChangeBackground(TColor::RGB2Pixel(TEXTENTRY_BG_R,TEXTENTRY_BG_G,TEXTENTRY_BG_B));
  filenametext->SetFrameDrawn(kFALSE);
  filenametext->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
  filenametext->SetFont(TEXTENTRY_FONT,false);
  filenametext->Resize(INITIAL_CONTANT_WIDTH,28);
  

  TGLabel *filerunlabel = new TGLabel(filesetgroup,"Run Num: ");
  filesetgroup->AddFrame(filerunlabel,new TGLayoutHints(kLHintsLeft| kLHintsTop,0,0,10,0));
  filerunlabel->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  filerunlabel->SetTextColor(TColor::RGB2Pixel(LABEL_TEXT_R,LABEL_TEXT_G,LABEL_TEXT_B),false);
  filerunlabel->SetTextFont(LABEL_FONT,false);
  
  // Run Num frame 
  TGHorizontalFrame *runnumf = new TGHorizontalFrame(filesetgroup);
  filesetgroup->AddFrame(runnumf,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
  runnumf->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  

  filerunnum = new TGNumberEntry(runnumf,0,5,999,TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative);
  filerunnum->SetButtonToNum(true);
  runnumf->AddFrame(filerunnum,new TGLayoutHints(kLHintsLeft|kLHintsTop,0,0,4,0));
  filerunnum->Associate(this);
  filerunnum->Resize(70,28);
  filerunnum->GetNumberEntry()->ChangeOptions(filerunnum->GetNumberEntry()->GetOptions() ^ kRaisedFrame);
  filerunnum->GetNumberEntry()->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
  filerunnum->GetButtonUp()->ChangeOptions(filerunnum->GetButtonUp()->GetOptions() ^ kRaisedFrame);
  filerunnum->GetButtonDown()->ChangeOptions(filerunnum->GetButtonDown()->GetOptions() ^ kRaisedFrame);
  filerunnum->ChangeSubframesBackground(TColor::RGB2Pixel(TEXTENTRY_BG_R,TEXTENTRY_BG_G,TEXTENTRY_BG_B));

  
  filesetdone = new TGTextButton(runnumf,"Complete");
  filesetdone->Connect("Pressed()","MainFrame",this,"ConfigFileInfo()");
  runnumf->AddFrame(filesetdone,new TGLayoutHints(kLHintsRight|kLHintsTop,0,0,4,0));
  filesetdone->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  filesetdone->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));
  filesetdone->SetFont(TEXTBUTTON_FONT, false);
  filesetdone->Resize(100,24);
  filesetdone->ChangeOptions(filesetdone->GetOptions() ^ kRaisedFrame);
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  TGHSplitter *hsplitter2 = new TGHSplitter(TabPanel,INITIAL_WIDTH,2);
  TabPanel->AddFrame(hsplitter2, new TGLayoutHints(kLHintsTop | kLHintsExpandX,0,0,10,10));
  hsplitter2->SetBackgroundColor(TColor::RGB2Pixel(0,0,0));

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  
  // Control of list mode run
  TGCompositeFrame *controlgroup = new TGCompositeFrame(TabPanel,0,0);
  TabPanel->AddFrame(controlgroup,new TGLayoutHints(kLHintsExpandX|kLHintsTop,INITIAL_SIDE_WIDTH,INITIAL_SIDE_WIDTH,0,0));
  controlgroup->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  TGTextEntry *labelcontrol = new TGTextEntry(controlgroup,new TGTextBuffer(300), 10000);
  controlgroup->AddFrame(labelcontrol, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 0, TITLE_TOP_HIGHT, TITLE_BOTTON_HIGHT));
  labelcontrol->SetFont(TITLE_FONT, false);
  labelcontrol->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
  labelcontrol->SetText("Control");
  labelcontrol->SetEnabled(kFALSE);
  labelcontrol->SetFrameDrawn(kFALSE);
  labelcontrol->ChangeBackground(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));




  
  TGHorizontalFrame *cgrouphframe0 = new TGHorizontalFrame(controlgroup);
  controlgroup->AddFrame(cgrouphframe0,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
  cgrouphframe0->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  // send/not send online data stream box
  onlinechk = new TGCheckButton(cgrouphframe0,"Online Statistics");
  onlinechk->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  onlinechk->SetTextColor(TColor::RGB2Pixel(CHECKBUTTON_TEXT_R,CHECKBUTTON_TEXT_G,CHECKBUTTON_TEXT_B));
  onlinechk->SetFont(CHECKBUTTON_FONT, false);
  onlinechk->SetState(kButtonDown);
  fonlinedata = 1;
  onlinechk->Connect("Clicked()","MainFrame",this,"SetOnlineDataFlag()");
  cgrouphframe0->AddFrame(onlinechk,new TGLayoutHints(kLHintsLeft|kLHintsTop,0,0,5,10));
  // record/not record raw data
  recordchk = new TGCheckButton(cgrouphframe0,"Record");
  recordchk->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  recordchk->SetTextColor(TColor::RGB2Pixel(CHECKBUTTON_TEXT_R,CHECKBUTTON_TEXT_G,CHECKBUTTON_TEXT_B));
  recordchk->SetFont(CHECKBUTTON_FONT, false);
  recordchk->SetState(kButtonDown);
  frecorddata = 1;
  recordchk->Connect("Clicked()","MainFrame",this,"SetRecordDataFlag()");
  cgrouphframe0->AddFrame(recordchk,new TGLayoutHints(kLHintsRight|kLHintsTop,0,0,5,10));
  

  
  TGHorizontalFrame *cgrouphframe1 = new TGHorizontalFrame(controlgroup);
  controlgroup->AddFrame(cgrouphframe1,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
  cgrouphframe1->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  // start/stop LSM run button
  startdaq = new TGTextButton(cgrouphframe1,"Run Start");
  startdaq->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  startdaq->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));
  startdaq->SetFont(STARTSTOP_TEXTBUTTON_FONT, false);
  startdaq->Connect("Pressed()","MainFrame",this,"StartRun()");
  startdaq->SetEnabled(0);
  startdaq->Resize(INITIAL_CONTANT_WIDTH,46);
  startdaq->ChangeOptions(startdaq->GetOptions() | kFixedSize);
  startdaq->ChangeOptions(startdaq->GetOptions() ^ kRaisedFrame);
  cgrouphframe1->AddFrame(startdaq,new TGLayoutHints(kLHintsCenterX|kLHintsTop));
  


  
  TGHorizontalFrame *cgrouphframe2 = new TGHorizontalFrame(controlgroup);
  controlgroup->AddFrame(cgrouphframe2,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
  cgrouphframe2->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  // update once time energy
  updateenergyonline = new TGCheckButton(cgrouphframe2,"Update Energy Monitor");
  updateenergyonline->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  updateenergyonline->SetTextColor(TColor::RGB2Pixel(CHECKBUTTON_TEXT_R,CHECKBUTTON_TEXT_G,CHECKBUTTON_TEXT_B));
  updateenergyonline->SetFont(CHECKBUTTON_FONT, false);
  updateenergyonline->SetOn(kFALSE);
  cgrouphframe2->AddFrame(updateenergyonline,new TGLayoutHints(kLHintsLeft|kLHintsTop,0,0,10,3));

#ifdef DECODERONLINE
  // record/not record raw data
  decoderchk = new TGCheckButton(cgrouphframe2,"Decoder");
  decoderchk->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  decoderchk->SetTextColor(TColor::RGB2Pixel(CHECKBUTTON_TEXT_R,CHECKBUTTON_TEXT_G,CHECKBUTTON_TEXT_B));
  decoderchk->SetFont(CHECKBUTTON_FONT, false);
  decoderchk->SetState(kButtonUp);
  decoderchk->Connect("Clicked()","MainFrame",this,"SetDecoderDataFlag()");
  cgrouphframe2->AddFrame(decoderchk,new TGLayoutHints(kLHintsRight|kLHintsTop,0,0,10,3));
#endif
  

  
  // restore last run's file information
  char tmp[200];
  ifstream in("../parset/Run.config");
  if(!in.is_open()) return;
  in.getline(tmp,200);
  filepathtext->Insert(tmp);
  in.getline(tmp,200);
  filenametext->Insert(tmp);
  in.close();

  ifstream inrunnumber("../parset/RunNumber");
  if(!inrunnumber.is_open()) return;
  inrunnumber.getline(tmp,200);
  filerunnum->SetText(tmp);
  inrunnumber.close();

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  TGHSplitter *hsplitter3 = new TGHSplitter(TabPanel,INITIAL_WIDTH,2);
  TabPanel->AddFrame(hsplitter3, new TGLayoutHints(kLHintsTop | kLHintsExpandX,0,0,10,10));
  hsplitter3->SetBackgroundColor(TColor::RGB2Pixel(0,0,0));

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  // run information
  TGCompositeFrame *informationgroup = new TGCompositeFrame(TabPanel,0,0);
  TabPanel->AddFrame(informationgroup,new TGLayoutHints(kLHintsExpandX|kLHintsTop,INITIAL_SIDE_WIDTH,INITIAL_SIDE_WIDTH,0,0));
  informationgroup->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));


  TGTextEntry *labelinformation = new TGTextEntry(informationgroup,new TGTextBuffer(300), 10000);
  informationgroup->AddFrame(labelinformation, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 0, TITLE_TOP_HIGHT, TITLE_BOTTON_HIGHT));
  labelinformation->SetFont(TITLE_FONT, false);
  labelinformation->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
  labelinformation->SetText("Information");
  labelinformation->SetEnabled(kFALSE);
  labelinformation->SetFrameDrawn(kFALSE);
  labelinformation->ChangeBackground(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));




  
  TGHorizontalFrame *versionstatusframe = new TGHorizontalFrame(informationgroup);
  informationgroup->AddFrame(versionstatusframe,new TGLayoutHints(kLHintsLeft|kLHintsTop,0,0,0,0));
  versionstatusframe->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  TGTextEntry *versiontextinfor = new TGTextEntry(versionstatusframe,new TGTextBuffer(30), 10000);
  versionstatusframe->AddFrame(versiontextinfor, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 0, 6, 5));
  versiontextinfor->SetFont(INFORMATION_FONT, false);
  versiontextinfor->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
  versiontextinfor->SetText(gVERSION);
  versiontextinfor->SetEnabled(kFALSE);
  versiontextinfor->SetFrameDrawn(kFALSE);
  versiontextinfor->ChangeBackground(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  TGHorizontalFrame *StateMsgFrame = new TGHorizontalFrame(informationgroup);
  informationgroup->AddFrame(StateMsgFrame,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
  StateMsgFrame->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  StateMsgFold1 = new TGTextEntry(StateMsgFrame,
				  new TGTextBuffer(30), 10000,
				  StateMsgFold1->GetDefaultGC()(),
				  StateMsgFold1->GetDefaultFontStruct(),
				  kRaisedFrame | kDoubleBorder,
				  GetWhitePixel());
  StateMsgFrame->AddFrame(StateMsgFold1, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 5, 5));
  StateMsgFold1->SetFont(INFORMATION_FONT, false);
  StateMsgFold1->SetTextColor(TColor::RGB2Pixel(COLOR_BLUE_R,COLOR_BLUE_G,COLOR_BLUE_B), false);
  StateMsgFold1->SetText("System not booted");
  StateMsgFold1->SetEnabled(kFALSE);
  StateMsgFold1->SetFrameDrawn(kFALSE);
  StateMsgFold1->ChangeBackground(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  
  TGHorizontalFrame *lastruninfor = new TGHorizontalFrame(informationgroup);
  informationgroup->AddFrame(lastruninfor,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
  lastruninfor->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  lastruntextinfor = new TGTextEntry(lastruninfor,new TGTextBuffer(30), 10000);
  lastruninfor->AddFrame(lastruntextinfor, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 0, 5, 0));
  lastruntextinfor->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  lastruntextinfor-> SetFont(INFORMATION_FONT, false);
  lastruntextinfor->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B), false);
  lastruntextinfor->SetText(TString::Format("Last run number: %d",int(filerunnum->GetIntNumber())-1).Data());
  lastruntextinfor->SetEnabled(kFALSE);
  lastruntextinfor->SetFrameDrawn(kFALSE);
  lastruntextinfor->ChangeBackground(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
}

void MainFrame::ConfigFileInfo()
{
  if(IsDirectoryExists(filepathtext->GetText()))
    {
      ofstream out("../parset/Run.config");
      out<<filepathtext->GetText()<<endl;
      out<<filenametext->GetText()<<endl;
      out.close();
      
      startdaq->SetEnabled(1);
      fstartdaq = 0;
      detector->SetRunFlag(fstartdaq);

      if(flagonlinemode == 0)
      	{
      	  const char *path = filepathtext->GetText();
      	  const char *filen = filenametext->GetText();
      	  detector->UpdateFilePathAndNameInSharedMemory(path,filen);
      	}
    }
  else
    {
      startdaq->SetEnabled(0);
      std::cout<<"The output file directory does not exist"<<std::endl;
    }
}

void MainFrame::SetFileName()
{
  if(detector == NULL)
    {
      std::cout<<"Modules not booted!"<<std::endl;
      return ;
    }
  const char *path = filepathtext->GetText();
  const char *filen = filenametext->GetText();
  runnum = (int)filerunnum->GetIntNumber();

  CreateDirectory(TString::Format("%s%04d",path,runnum).Data());
  
  detector->SetRunNumber(runnum);
  for(int i = 0; i < detector->NumModules; i++)
    {
      sprintf(Filename[i],"%s%04d/%s_R%04d_M%02d.bin",path,runnum,filen,runnum,i);
      sprintf(Histogramname[i],"%s%04d/%s_histogram_R%04d_M%02d.bin",path,runnum,filen,runnum,i);
      // cout<<i<<" "<<Filename[i]<<endl;
    }

  sprintf(DSPParsFileName,"%s%04d/DSPPar.set",path,runnum);
  sprintf(LogFileName,"%s%04d/run.log",path,runnum);
  
  if(IsDirectoryExists(TString::Format("%s%04d",path,runnum).Data()))
    {
      ofstream out("../parset/Run.config");
      out<<filepathtext->GetText()<<endl;
      out<<filenametext->GetText()<<endl;
      out.close();
      
      startdaq->SetEnabled(1);
      fstartdaq = 0;
      detector->SetRunFlag(fstartdaq);
    }
  else
    {
      startdaq->SetEnabled(0);
      std::cout<<"The output file directory does not exist."<<std::endl;
    }
}

void MainFrame::StartRun()
{
  if(fstartdaq == 0)
    {
      onlinemode->SetEnabled(0);
      filesetdone->SetEnabled(0);
      recordchk->SetEnabled(0);
#ifdef DECODERONLINE
      decoderchk->SetEnabled(0);
#endif
      
      SetMenuStatus(false,flagonlinemode);
      
      SetFileName();
      detector->SetOnlineFlag(fonlinedata);

      for(int i = 0;i < detector->NumModules;i++)
	{
	  std::cout<<"open: "<<Filename[i]<<std::endl;
	  if(!detector->OpenSaveFile(i,Filename[i]))
	    {
	      cout<<Filename[i]<<endl;
	      return;
	    }
	}

      
      std::ofstream writelog;//fstream
      writelog.open(LogFileName,ios::app);//ios::bin ios::app
      if(!writelog.is_open())
	{
	  std::cout<<"can't open Log file."<<std::endl;
	}
      time_t timep;
      time(&timep);
      char tmp[64];
      strftime(tmp, sizeof(tmp), "Start: %Y-%m-%d %H:%M:%S",localtime(&timep));
      writelog<<tmp<<endl;
      writelog.close();    
      
      if(detector->StartRun(0))
	{
	  std::cout<<"CANNOT start the LSM Run!"<<std::endl;

	  SetMenuStatus(true,flagonlinemode);

	  onlinemode->SetEnabled(1);
	  filesetdone->SetEnabled(1);
	  recordchk->SetEnabled(1);
	  return;
	}

      fstartdaq = 1;
      detector->SetRunFlag(fstartdaq);
      usleep(100000); //delay for the DSP boot 
      // sleep(2);// wait 2 seconds for modules to be ready
      // start a new run, not resume
      
      startdaq->SetText("Run Stop");
      RunReadData();
    }
  else
    {
      fstartdaq = 0;
      
      startdaq->SetText("Run Start");
      filerunnum->SetIntNumber((++runnum));

      ofstream outrunnumber("../parset/RunNumber");
      outrunnumber<<filerunnum->GetIntNumber();
      outrunnumber.close();

      lastruntextinfor->SetText(TString::Format("Last run number: %d",int(filerunnum->GetIntNumber())-1).Data());

      SetMenuStatus(true,flagonlinemode);

      std::ofstream writelog;//fstream
      writelog.open(LogFileName,ios::app);//ios::bin ios::app
      if(!writelog.is_open())
	{
	  std::cout<<"can't open Log file."<<std::endl;
	}
      time_t timep;
      time(&timep);
      char tmp[64];
      strftime(tmp, sizeof(tmp), "Stop : %Y-%m-%d %H:%M:%S",localtime(&timep));
      writelog<<tmp<<endl;
      writelog.close();    
      
      onlinemode->SetEnabled(1);
      filesetdone->SetEnabled(1);
      recordchk->SetEnabled(1);
#ifdef DECODERONLINE
      decoderchk->SetEnabled(1);
#endif
    }
}

void MainFrame::RunReadData()
{
  std::cout<<"Read loop ..."<<std::endl;
  while(fstartdaq)
    {
      detector->ReadDataFromModules(0,0); // during the run
      if(updateenergyonline->IsOn())
	{
	  detector->UpdateEnergySpectrumForModule();
	  updateenergyonline->SetOn(kFALSE);
	}
      gSystem->ProcessEvents();
    }
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
  for(int i = 0;i < detector->NumModules; i++)
    {
      detector->SaveHistogram(Histogramname[i],i);
    }
  
  std::cout<<"finish!"<<std::endl;
  detector->SetRunFlag(false);
}

void MainFrame::SetOnlineMode()
{
  if(onlinemode->IsOn())
    {
      flagonlinemode = 0;
    }
  else
    {
      flagonlinemode = 1;
    }

  SetMenuStatus(false,flagonlinemode);

  bootB->SetEnabled(1);

  StateMsgFold1->SetTextColor(TColor::RGB2Pixel(COLOR_BLUE_R,COLOR_BLUE_G,COLOR_BLUE_B), false);
  StateMsgFold1->SetText("System not booted");

  startdaq->SetEnabled(0);
}

void MainFrame::SetOnlineDataFlag()
{
   if(detector == NULL) return;
  if(onlinechk->IsOn())
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


#ifdef DECODERONLINE
void MainFrame::SetDecoderDataFlag()
{
  if(detector == NULL) return;
  if(decoderchk->IsOn())
    {
      detector->SetDecoterFlag(true);
    }
  else
    {
      detector->SetDecoterFlag(false);
    }
}
#endif

void MainFrame::SetRecordDataFlag()
{
  if(detector == NULL) return;
  if(recordchk->IsOn())
    {
      frecorddata = true;
      detector->SetRecordFlag(true);
      std::cout<<"DAQ will record data!"<<std::endl;
    }
  else
    {
      frecorddata = false;
      detector->SetRecordFlag(false);
      std::cout<<"DAQ wont record data!"<<std::endl;
    }
}


bool MainFrame::IsDirectoryExists(const char *path)
{
  struct stat fileStat;
  if ((stat(path, &fileStat) == 0) && S_ISDIR(fileStat.st_mode))
    return true;
  else
    return false;
}

bool MainFrame::CreateDirectory(const char *path)
{
  //创建文件夹
  if(mkdir(path,0777)==0)//第一个0表示这里是八进制数
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

void MainFrame::SetMenuStatus(bool flag,int flagonline)
{
  if(flag)
    {
      MenuFile->EnableEntry(FILE_EXIT);
      MenuSetup->EnableEntry(BASE);
      MenuSetup->EnableEntry(ENERGY);
      MenuSetup->EnableEntry(TFILTER);
      MenuSetup->EnableEntry(CFDP);
      MenuSetup->EnableEntry(QDCP);
      MenuSetup->EnableEntry(DECIMATIONP);
      MenuSetup->EnableEntry(COPYPARS);
      MenuSetup->EnableEntry(FILE_SAVE);
      MenuExpert->EnableEntry(MODVAR);
      MenuExpert->EnableEntry(CSRA);
      MenuExpert->EnableEntry(LOGIC);
      MenuMonitor->EnableEntry(HISTXDT);
      if(flagonline == 1)
      	MenuMonitor->DisableEntry(READCHANSTATUS);
      else
      	MenuMonitor->EnableEntry(READCHANSTATUS);
      MenuOffline->EnableEntry(OFFLINEADJUSTPAR);
      MenuOffline->EnableEntry(SIMULATION);
    }
  else
    {
      if(flagonline == 1)
	MenuFile->EnableEntry(FILE_EXIT);
      else
	MenuFile->DisableEntry(FILE_EXIT);
      MenuSetup->DisableEntry(BASE);
      MenuSetup->DisableEntry(ENERGY);
      MenuSetup->DisableEntry(TFILTER);
      MenuSetup->DisableEntry(CFDP);
      MenuSetup->DisableEntry(QDCP);
      MenuSetup->DisableEntry(DECIMATIONP);
      MenuSetup->DisableEntry(COPYPARS);
      MenuSetup->DisableEntry(FILE_SAVE);
      MenuExpert->DisableEntry(MODVAR);
      MenuExpert->DisableEntry(CSRA);
      MenuExpert->DisableEntry(LOGIC);
      MenuMonitor->DisableEntry(HISTXDT);
      MenuMonitor->DisableEntry(READCHANSTATUS);
      MenuOffline->DisableEntry(OFFLINEADJUSTPAR);
      MenuOffline->DisableEntry(SIMULATION);
    }

}

// 
// MainFrame.cc ends here
