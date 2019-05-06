// MainFrame.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 3月  9 13:01:33 2018 (+0800)
// Last-Updated: 一 5月  6 20:53:27 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 40
// URL: http://wuhongyi.cn 

#include "MainFrame.hh"
#include "Csra.hh"
#include "Detector.hh"
#include "Global.hh"

#include "pixie16app_export.h"
#include "pixie16sys_export.h"

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

  CreateMenuBar();

  SetWindowName("PKU XIA Pixie-16 DAQ");
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
  TGMenuBar *MenuBar = new TGMenuBar(this, 1, 1, kHorizontalFrame);
  fClient->GetColorByName("pink", color);
  MenuBar->ChangeBackground(color);
  
  MenuFile = new TGPopupMenu(fClient->GetRoot());
  MenuFile->AddEntry("E&xit", FILE_EXIT,0,gClient->GetPicture("bld_exit.png"));
  MenuFile->AddSeparator();
  MenuFile->AddEntry("&About", ABOUT,0,gClient->GetPicture("ed_help.png"));
  MenuFile->Associate(this);
  MenuBar->AddPopup("&File", MenuFile, new TGLayoutHints (kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  AddFrame(MenuBar, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX, 0, 0, 0, 0));

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
  MenuBar->AddPopup("&UV_Setup", MenuSetup, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  MenuExpert = new TGPopupMenu(fClient->GetRoot());
  MenuExpert->AddEntry("Module Variables", MODVAR);
  MenuExpert->AddEntry("&CSRA", CSRA);
  MenuExpert->AddEntry("Logic Set", LOGIC);
  MenuExpert->Associate(this);
  MenuBar->AddPopup("&Expert", MenuExpert, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));


  MenuMonitor = new TGPopupMenu(fClient->GetRoot());
  MenuMonitor->AddEntry("&Hist & XDT", HISTXDT);
  MenuMonitor->AddEntry("&Trace & Baseline", READCHANSTATUS);
  MenuMonitor->Associate(this);
  MenuBar->AddPopup("&Monitor", MenuMonitor, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));


  MenuOffline = new TGPopupMenu(fClient->GetRoot());
  MenuOffline->AddEntry("Adjust Par", OFFLINEADJUSTPAR);
  MenuOffline->AddEntry("Simulation", SIMULATION);
  MenuOffline->Associate(this);
  MenuBar->AddPopup("&Offline", MenuOffline, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  SetMenuStatus(false,flagonlinemode);


  TGTab *TabPanel = new TGTab(this);
  this->AddFrame(TabPanel, new TGLayoutHints(kLHintsBottom | kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));

  TGCompositeFrame *Tab2 = TabPanel->AddTab("List Mode Run");
  MakeFold2Panel(Tab2);
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
	     about =  new TRootHelpDialog(this, "About PKU XIA Pixie-16 DAQ", 550, 250);
	     about->SetText(gAbout);
	     about->Popup();
	      break;
	    case BASE:
	      base = new Base(fClient->GetRoot(), this, (char*)"Base Setup", 6, 16, detector->NumModules);
	      base->load_info(0);
	      break;
	    case CFDP:
	      cfd = new Cfd(fClient->GetRoot(),this,(char*)"Cfd Par.", 4, 16, detector->NumModules);
	      cfd->load_info(0);
	      break;
	    case QDCP:
	      qdc=new Qdc(fClient->GetRoot(),this,(char*)"Qdc Par.", 9, 16, detector->NumModules);
	      qdc->load_info(0);
	      break;
	    case DECIMATIONP:
	      decimation = new Decimation(fClient->GetRoot(),this,(char*)"Waveform Decimation", 2, 16, detector->NumModules);
	      decimation->load_info(0);
	      break;
	    case ENERGY:
	      energy = new Energy(fClient->GetRoot(), this, (char*)"Energy", 5, 16, detector->NumModules);
	      energy->load_info(0);
	      break;
	    case CSRA:
	      csra = new Csra(fClient->GetRoot(), this,detector->NumModules);
	      csra->load_info(0);
	      break;
	    case TFILTER:
	      triggerfilter = new TriggerFilter(fClient->GetRoot (), this, (char*)"Trigger Filter", 4, 16, detector->NumModules);
	      triggerfilter->load_info(0);
	      break;
	    case MODVAR:
	      expertmod = new ExpertMod(fClient->GetRoot(), this, (char*)"Expert MOD", detector->NumModules);
	      expertmod->load_info(0);
	      break;
	    case LOGIC:
	      logictrigger = new LogicTrigger(fClient->GetRoot(),this,(char*)"Logic Trigger", 16/*7*/, 16, detector->NumModules);
	      logictrigger->load_info(0);
	      break;
	    case HISTXDT:
	      histxdt = new HistXDT(fClient->GetRoot(), this, (char*)"Hist & XDT", 4, 16, detector->NumModules);
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
	      fClient->GetColorByName("red", color);
	      StateMsgFold1->SetTextColor(color, false);
	      StateMsgFold1->SetText("booting...wait a moment");
	      gSystem->ProcessEvents();
	      // gPad->SetCursor(kWatch);
	      if(detector != 0) delete detector;
	      detector = new Detector(flagonlinemode);
	      detector->SetRecordFlag(true);
	      recordchk->SetState(kButtonDown);
	      if(detector->BootSystem())
		{
		  fClient->GetColorByName("green", color);
		  StateMsgFold1->SetTextColor(color, false);
		  StateMsgFold1->SetText("Booted system");
		  // gPad->SetCursor(kPointer);

		  SetMenuStatus(true,flagonlinemode);
		  filesetdone->SetEnabled(1);
		}
	      else
		{
		  fClient->GetColorByName("blue", color);
		  StateMsgFold1->SetTextColor(color, false);
		  StateMsgFold1->SetText("boot Failed...");
		  bootB->SetEnabled(1);
		}
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

void MainFrame::MakeFold2Panel(TGCompositeFrame *TabPanel)
{

  TGCompositeFrame *LogoFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);

  TGImageMap* fImagePKU = new TGImageMap(LogoFrame, "../icons/logo1.jpg");
  fImagePKU->Resize(100,100);
  fImagePKU->ChangeOptions(fImagePKU->GetOptions() | kFixedSize);
  LogoFrame->AddFrame(fImagePKU,new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 0, 0, 0));

  TGImageMap* fImageWHY = new TGImageMap(LogoFrame, "../icons/logo2.jpg");
  fImageWHY->Resize(100,100);
  fImageWHY->ChangeOptions(fImageWHY->GetOptions() | kFixedSize);
  LogoFrame->AddFrame(fImageWHY,new TGLayoutHints(kLHintsTop | kLHintsRight, 100, 0, 0, 0));

  TabPanel->AddFrame(LogoFrame, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  //make the buttons frame        
  TGCompositeFrame *ButtonFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);

  // Online mode
  onlinemode = new TGCheckButton(ButtonFrame,"&Online Mode");
  fClient->GetColorByName("red", color);
  onlinemode->SetTextColor(color);
  onlinemode->SetState(kButtonDown);
  onlinemode->Connect("Clicked()","MainFrame",this,"SetOnlineMode()");
  ButtonFrame->AddFrame(onlinemode,new TGLayoutHints(kLHintsLeft|kLHintsTop,5,10,15,0));
  
  // BOOT button//////////////////////////////////////////////////////////////    
  bootB = new TGTextButton(ButtonFrame, "  Boot  ", BOOT_BUTTON);
  bootB->SetFont("-adobe-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1", false);
  fClient->GetColorByName("blue", color);
  bootB->SetTextColor(color, false);
  bootB->Associate(this);
  ButtonFrame->AddFrame(bootB, new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 10,10, 0));

  TabPanel->AddFrame(ButtonFrame, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  // Set up for file store parametrs
  TGGroupFrame *filesetgroup = new TGGroupFrame(TabPanel,"Setup");
  TabPanel->AddFrame(filesetgroup,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
  
  // FILE path frame
  TGHorizontalFrame *filepath = new TGHorizontalFrame(filesetgroup);
  TGLabel *filepathlabel = new TGLabel(filepath,"File Path: ");
  filepath->AddFrame(filepathlabel,new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 3, 4, 0));
  filepathtext = new TGTextEntry(filepath,new TGTextBuffer(20));
  filepath->AddFrame(filepathtext,new TGLayoutHints(kLHintsExpandX|kLHintsTop, 10 ,3,4,0));
  
  filesetgroup->AddFrame(filepath,new TGLayoutHints(kLHintsExpandX|kLHintsTop));

  
  // File Name frame 
  TGHorizontalFrame *filenamef = new TGHorizontalFrame(filesetgroup);
  filesetgroup->AddFrame(filenamef,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
  
  TGLabel *filenamelabel = new TGLabel(filenamef,"File Name: ");
  filenamef->AddFrame(filenamelabel,new TGLayoutHints(kLHintsLeft | kLHintsTop, 10 ,3,4,0));
  filenametext = new TGTextEntry(filenamef, new TGTextBuffer(20));
  filenamef->AddFrame(filenametext,new TGLayoutHints(kLHintsExpandX| kLHintsTop,10,3,4,0));

  // Run Num frame 
  TGHorizontalFrame *runnumf = new TGHorizontalFrame(filesetgroup);
  filesetgroup->AddFrame(runnumf,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
  
  TGLabel *filerunlabel = new TGLabel(runnumf,"Run Num: ");
  runnumf->AddFrame(filerunlabel,new TGLayoutHints(kLHintsLeft| kLHintsTop,10,3,5,0));

  filerunnum = new TGNumberEntry(runnumf,0,5,999,TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative);
  filerunnum->SetButtonToNum(1);
  runnumf->AddFrame(filerunnum,new TGLayoutHints(kLHintsLeft|kLHintsTop,20,3,4,0));
  filerunnum->Associate(this);

  filesetdone = new TGTextButton(runnumf,"Complete");
  filesetdone->Connect("Pressed()","MainFrame",this,"ConfigFileInfo()");
  runnumf->AddFrame(filesetdone,new TGLayoutHints(kLHintsLeft|kLHintsTop,10,3,4,0));


  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  
  // Control of list mode run
  TGGroupFrame *controlgroup = new TGGroupFrame(TabPanel,"Control");
  TabPanel->AddFrame(controlgroup,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
  

  TGHorizontalFrame *cgrouphframe0 = new TGHorizontalFrame(controlgroup);
  controlgroup->AddFrame(cgrouphframe0,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
  // send/not send online data stream box
  onlinechk = new TGCheckButton(cgrouphframe0,"&Online Statistics");
  fClient->GetColorByName("red", color);
  onlinechk->SetTextColor(color);
  onlinechk->SetState(kButtonDown);
  fonlinedata = 1;
  onlinechk->Connect("Clicked()","MainFrame",this,"SetLSonlinedataf()");
  cgrouphframe0->AddFrame(onlinechk,new TGLayoutHints(kLHintsExpandX|kLHintsTop,4,4,5,10));
  // record/not record raw data
  recordchk = new TGCheckButton(cgrouphframe0,"&Record");
  fClient->GetColorByName("red", color);
  recordchk->SetTextColor(color);
  recordchk->SetState(kButtonDown);
  frecorddata = 1;
  recordchk->Connect("Clicked()","MainFrame",this,"SetRecordDataFlag()");
  cgrouphframe0->AddFrame(recordchk,new TGLayoutHints(kLHintsExpandX|kLHintsTop,4,4,5,10));
  

  
  TGHorizontalFrame *cgrouphframe1 = new TGHorizontalFrame(controlgroup);
  controlgroup->AddFrame(cgrouphframe1,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
  // start/stop LSM run button
  startdaq = new TGTextButton(cgrouphframe1,"LSRunStart");
  fClient->GetColorByName("red", color);
  startdaq->SetTextColor(color, false);
  startdaq->SetFont("-adobe-helvetica-medium-r-*-*-20-*-*-*-*-*-iso8859-1", false);
  startdaq->Connect("Pressed()","MainFrame",this,"StartLSRun()");
  startdaq->SetEnabled(0);
  startdaq->Resize(110,110);
  startdaq->ChangeOptions(startdaq->GetOptions() | kFixedSize);
  cgrouphframe1->AddFrame(startdaq,new TGLayoutHints(kLHintsCenterX|kLHintsTop));

  TGHorizontalFrame *cgrouphframe2 = new TGHorizontalFrame(controlgroup);
  controlgroup->AddFrame(cgrouphframe2,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
  // update once time energy
  updateenergyonline = new TGCheckButton(cgrouphframe2,"&Update Energy Monitor");
  fClient->GetColorByName("red", color);
  updateenergyonline->SetTextColor(color);
  updateenergyonline->SetOn(kFALSE);
  cgrouphframe2->AddFrame(updateenergyonline,new TGLayoutHints(kLHintsRight|kLHintsTop,10,4,10,3));


  
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

  
  // run information
  TGGroupFrame *informationgroup = new TGGroupFrame(TabPanel,"Information");
  TabPanel->AddFrame(informationgroup,new TGLayoutHints(kLHintsExpandX|kLHintsTop));


  TGHorizontalFrame *versionstatusframe = new TGHorizontalFrame(informationgroup);
  informationgroup->AddFrame(versionstatusframe,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
  TGTextEntry *versiontextinfor = new TGTextEntry(versionstatusframe,new TGTextBuffer(30), 10000);
  versionstatusframe->AddFrame(versiontextinfor, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 0, 6, 20));
  versiontextinfor-> SetFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1", false);
  fClient->GetColorByName("red", color);
  versiontextinfor->SetTextColor(color, false);
  versiontextinfor->SetText(gVERSION);
  versiontextinfor->Resize(150, 16);
  versiontextinfor->SetEnabled(kFALSE);
  versiontextinfor->SetFrameDrawn(kFALSE);

  
  TGHorizontalFrame *StateMsgFrame = new TGHorizontalFrame(informationgroup);
  informationgroup->AddFrame(StateMsgFrame,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
  
  StateMsgFold1 = new TGTextEntry(StateMsgFrame,
				  new TGTextBuffer(30), 10000,
				  StateMsgFold1->GetDefaultGC()(),
				  StateMsgFold1->GetDefaultFontStruct(),
				  kRaisedFrame | kDoubleBorder,
				  GetWhitePixel());
  StateMsgFrame->AddFrame(StateMsgFold1, new TGLayoutHints(kLHintsTop | kLHintsLeft, 10, 0, 5, 10));
  StateMsgFold1->SetFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1", false);
  fClient->GetColorByName("blue", color);
  StateMsgFold1->SetTextColor(color, false);
  StateMsgFold1->SetText("System not booted");
  StateMsgFold1->Resize(200, 16);
  StateMsgFold1->SetEnabled(kFALSE);
  StateMsgFold1->SetFrameDrawn(kFALSE);


  
  TGHorizontalFrame *lastruninfor = new TGHorizontalFrame(informationgroup);
  informationgroup->AddFrame(lastruninfor,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
  lastruntextinfor = new TGTextEntry(lastruninfor,new TGTextBuffer(30), 10000);
  lastruninfor->AddFrame(lastruntextinfor, new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 0, 6, 0));
  lastruntextinfor-> SetFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1", false);
  fClient->GetColorByName("blue", color);
  lastruntextinfor->SetTextColor(color, false);
  lastruntextinfor->SetText(TString::Format("Last run number: %d",int(filerunnum->GetIntNumber())-1).Data());
  lastruntextinfor->Resize(200, 16);
  lastruntextinfor->SetEnabled(kFALSE);
  lastruntextinfor->SetFrameDrawn(kFALSE);

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
    }
  else
    {
      startdaq->SetEnabled(0);
      std::cout<<"The output file directory does not exist"<<std::endl;
    }
}

void MainFrame::SetLSFileName()
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
    }
  else
    {
      startdaq->SetEnabled(0);
      std::cout<<"The output file directory does not exist"<<std::endl;
    }
}

void MainFrame::StartLSRun()
{
  if(fstartdaq == 0)
    {
      onlinemode->SetEnabled(0);
      filesetdone->SetEnabled(0);
      recordchk->SetEnabled(0);
      
      SetMenuStatus(false,flagonlinemode);
      
      SetLSFileName();
      detector->SetOnlineF(fonlinedata);

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
      
      if(detector->StartLSMRun(0))
	{
	  std::cout<<"CANNOT start the LSM Run!"<<std::endl;

	  SetMenuStatus(true,flagonlinemode);

	  onlinemode->SetEnabled(1);
	  filesetdone->SetEnabled(1);
	  recordchk->SetEnabled(1);
	  return;
	}


      
      usleep(100000); //delay for the DSP boot 
      // sleep(2);// wait 2 seconds for modules to be ready
      // start a new run, not resume
      fstartdaq = 1;
      fstopdaq = 0;
      startdaq->SetText("LSRunStop");
      LSRunReadData();
    }
  else
    {
      fstartdaq = 0;
      startdaq->SetText("LSRunStart");
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
    }
}

void MainFrame::LSRunReadData()
{
  std::cout<<"MainFrame:: read loop.."<<std::endl;
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
  while(detector->StopLSMRun())
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

  fClient->GetColorByName("blue", color);
  StateMsgFold1->SetTextColor(color, false);
  StateMsgFold1->SetText("System not booted");

  startdaq->SetEnabled(0);
}

void MainFrame::SetLSonlinedataf()
{
  if(onlinechk->IsOn())
    {
      fonlinedata = 1;
      detector->SetOnlineF(1);
      std::cout<<"DAQ will send online data!"<<std::endl;
    }
  else
    {
      fonlinedata = 0;
      detector->SetOnlineF(0);
      std::cout<<"DAQ wont send online data!"<<std::endl;
    }
}

void MainFrame::SetRecordDataFlag()
{
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
