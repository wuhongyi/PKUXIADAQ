#define MADE "v. 1.0,---> 2016.10.07"

#include <unistd.h>
#include <sys/stat.h>//stat(const char *file_name,struct stat *buf)
#include "Main.h"
#include "PulseShape.h"
#include "Detector.h"
#include "EnergyFilter.h"
#include "Csra.h"
#include "TriggerFilter.h"
#include "pixie16app_export.h"
#include "pixie16sys_export.h"
#include "Popup.h"
#include "Global.h"
ClassImp(Main)
const char *spec_types[] =
{ "Radware spectrum", "*.spe", "all files", "*.*", 0, 0 };

const char *filetypes[] =
  { "Set Files", "*.set", "all files", "*.*", 0, 0 };

Main::Main(const TGWindow * p)
{
  ///initialise variables///////////////
  moduleNr = 0;
  channelNr = 0;
  size = 8192; //scope trace size
  detector = 0; 
  runnum=0;
  ///////////////////////////////////////
  CreateMenuBar();
  //	SetWindowName("Pixie16 Oscilloscope");
  SetWindowName("Pixie16 DAQ");
  MapSubwindows();
  MapWindow();
  Resize(INITIAL_WIDTH, INITIAL_HIGHT);
  //hpx_once_wave=false;
  fHpx_wave = NULL;
  trace = NULL;
  trace_float = NULL;
  //mca_float = NULL;
  //MCA_RUN = false;

  wave_once = false;
  //MCAALL = true;
  
  AppendPad(); //foarte important
  //fMca = NULL;
  xmin = 0;
  xmax = 8192;
  ymin = 0;
  ymax = 4096;
  NUMBERofTRACES = 500; //number of traces to search for waveforms
  range = 480;
  separation = 480;
  fraction = 0.0002;
}

Main::~Main()
{
  cout<<"destructor of Main is called!"<<endl;
}

void Main::CreateMenuBar()
{
  TGMenuBar *MenuBar = new TGMenuBar(this, 1, 1, kHorizontalFrame);
  TGPopupMenu *MenuFile = new TGPopupMenu(fClient->GetRoot());
  MenuFile->AddEntry("E&xit", FILE_EXIT);
  MenuFile->AddSeparator();
  MenuFile->AddEntry("&About", ABOUT);
  MenuFile->Associate(this);
  MenuBar->AddPopup("&File", MenuFile, new TGLayoutHints (kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  AddFrame(MenuBar, new TGLayoutHints (kLHintsTop | kLHintsLeft | kLHintsExpandX, 0, 0, 0, 0));

  TGPopupMenu *MenuSetup = new TGPopupMenu(fClient->GetRoot ());
  MenuSetup->AddEntry("A&nalog Signal Conditioning", ASG);
  MenuSetup->AddEntry("Ba&selines Setup", BASELINE);
  MenuSetup->AddEntry("&Energy Filter", EFILTER);
  MenuSetup->AddEntry("&Trigger Filter", TFILTER);
  MenuSetup->AddEntry("&CSRA", CSRA);
  MenuSetup->AddEntry("&Pulse Shape", PULSE);
  MenuSetup->AddEntry("C&FD", CFDP);
  MenuSetup->AddEntry("&QDC", QDCP);
  MenuSetup->AddEntry("&Decay Time", DECAY);
  MenuSetup->AddEntry("&Histogramming", HISTOGRAM);
  MenuSetup->AddSeparator();
  MenuSetup->AddEntry("Save2File", FILE_SAVE);
  MenuSetup->Associate(this);
  MenuBar->AddPopup("&UV_Setup", MenuSetup, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  TGPopupMenu *MenuExpert = new TGPopupMenu(fClient->GetRoot());
  MenuExpert->AddEntry("Module Variables", MODVAR);
  MenuExpert->AddEntry("Logic Set", LOGIC);
  MenuExpert->AddEntry("Front Outputs", FRONTPANELOUTPUTS);
  MenuExpert->AddEntry("Multiplicity", MULTIPLICITYMASK);
  MenuExpert->Associate(this);
  MenuBar->AddPopup("&Expert", MenuExpert, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  TGPopupMenu *MenuScope = new TGPopupMenu(fClient->GetRoot());
  // MenuScope->AddEntry("xy maxmin", MAXMIN);
  MenuScope->AddEntry("dT", SCOPEDT);
  MenuScope->Associate(this);
  MenuBar->AddPopup("&Scope", MenuScope, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  TGPopupMenu *MenuOffline = new TGPopupMenu(fClient->GetRoot ());
  MenuOffline->AddEntry("Adjust Par", OFFLINEADJUSTPAR);
  MenuOffline->Associate(this);
  MenuBar->AddPopup("&Offline", MenuOffline, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
      
  TGTab *TabPanel = new TGTab(this);
  this->AddFrame(TabPanel, new TGLayoutHints(kLHintsBottom | kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
  TGCompositeFrame *Tab1 = TabPanel->AddTab("Oscilloscope");
  MakeFold1Panel(Tab1);

  TGCompositeFrame *Tab2 = TabPanel->AddTab("List Mode Run");
  MakeFold2Panel(Tab2);

}

void Main::CloseWindow()
{
  if(detector !=0) delete detector;
  gApplication->Terminate(0);
}

Bool_t Main::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
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
	      about=new Popup(fClient->GetRoot(),this,(char*)MADE);
	      break;

	    case PULSE:
	      pulseshape = new PulseShape(fClient->GetRoot (), this, (char*)"Pulse Shape", 3, 16, detector->NumModules);
	      pulseshape->load_info(0);
	      break;
	    case CFDP:
	      cfd=new Cfd(fClient->GetRoot(),this,(char*)"Cfd Par.", 4, 16, detector->NumModules);
	      cfd->load_info(0);
	      break;
	    case QDCP:
	      qdc=new Qdc(fClient->GetRoot(),this,(char*)"Qdc Par.", 9, 16, detector->NumModules);
	      qdc->load_info(0);
	      break;
	    case EFILTER:
	      energyfilter = new EnergyFilter(fClient->GetRoot (), this, (char*)"Energy Filter", 3, 16, detector->NumModules);
	      energyfilter->load_info(0);
	      break;
	    case ASG:
	      analogsignal = new AnalogSignal(fClient->GetRoot (), this, (char*)"Analog Signal Setup", 3, 16, detector->NumModules);
	      analogsignal->load_info(0);
	      break;
	    case CSRA:
	      csra = new Csra(fClient->GetRoot(), this,detector->NumModules);
	      csra->load_info(0);
	      break;
	    case TFILTER:
	      triggerfilter = new TriggerFilter(fClient->GetRoot (), this, (char*)"Trigger Filter", 4, 16, detector->NumModules);
	      triggerfilter->load_info(0);
	      break;

	    case BASELINE:
	      baseline = new Baseline(fClient->GetRoot (), this, (char*)"Baseline Setup", 3, 16, detector->NumModules);
	      baseline->load_info(0);
	      break;
	    case DECAY:
	      tau = new Tau(fClient->GetRoot(), NULL, (char*)"Decay Time", 3, 16, detector->NumModules);
	      tau->load_info(0);
	      break;
	    case MODVAR:
	      expertmod = new ExpertMod(fClient->GetRoot(), this, (char*)"Expert MOD", detector->NumModules);
	      expertmod->load_info(0);
	      break;
	    case LOGIC:
	      logictrigger = new LogicTrigger(fClient->GetRoot(),this,(char*)"Logic Trigger", 6, 16, detector->NumModules);
	      logictrigger->load_info(0);
	      break;
	    case MULTIPLICITYMASK:
	      multiplicitymask = new MultiplicityMask(fClient->GetRoot(),this,(char*)"Multiplicity Mark", 7, 16, detector->NumModules);
		multiplicitymask->load_info(0);
	      break;
	    case FRONTPANELOUTPUTS:
	      fpoutputs = new FPOutputs(fClient->GetRoot(), this, detector->NumModules);
	      fpoutputs->load_info(0);
	      break;
	    case HISTOGRAM:
	      histogram = new Histogram(fClient->GetRoot(), this, (char*)"Histogramming", 3, 16, detector->NumModules);
	      histogram->load_info(0);
	      break;
	    case SCOPEDT:
	      scopedt = new ScopedT(fClient->GetRoot(), this, (char*)"dT", 2, 16, detector->NumModules);
	      scopedt->load_info(0);
	      break;
	    case OFFLINEADJUSTPAR:
	      popupoffline = new Offline(fClient->GetRoot(), this, detector,filepathtext,filenametext);
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
	      fClient->GetColorByName("red", color);
	      StateMsgFold1->SetTextColor(color, false);
	      StateMsgFold1->SetText("booting ...");
	      gPad->SetCursor (kWatch);
	      if(detector==0) detector = new Detector();
	      if(detector->BootSystem()){
		fClient->GetColorByName("green", color);
		StateMsgFold1->SetTextColor(color, false);
		StateMsgFold1->SetText("Booted system");
		gPad->SetCursor (kPointer);
	      }else {
		fClient->GetColorByName("blue", color);
		StateMsgFold1->SetTextColor(color, false);
		StateMsgFold1->SetText("boot Failed...");
	      }
	      break;
	    case READ_WF:
	      {
		NewTrace(size, moduleNr, channelNr);
	      }
	      break;
	    case SAVE_SEC:
	      {
		cout << "Saving to file\n" << flush;
		static TString dir1(".");
		TGFileInfo fInput1;
		fInput1.fFilename = NULL;
		fInput1.fFileTypes = spec_types;
		fInput1.fIniDir = StrDup(dir1);
		new
		  TGFileDialog(fClient->GetRoot (), this, kFDSave, &fInput1);
		if (fInput1.fFilename != NULL)
		  {
		    if (trace_float != NULL)
		      delete trace_float;
		    trace_float = new float[size];
		    for (int i = 0; i < size; i++)
		      trace_float[i] = (float) trace[i];
		    writeSpe(fInput1.fFilename, trace_float, size);
		  }
		else
		  {
		    cout << "no file name entered !\n" << flush;
		  }
	      }
	      break;
	    case MODULE_NUMBER:
	      if (parm2 == 0)
		{
		  if (moduleNr != detector->NumModules-1)
		    {
		      ++moduleNr;
		      numericMod->SetIntNumber(moduleNr);
		    }
		}
	      else
		{
		  if (moduleNr != 0)
		    {
		      if (--moduleNr == 0)
			moduleNr = 0;
		      numericMod->SetIntNumber(moduleNr);
		    }
		}
	      break;
	    case CHANNEL_NUMBER:
	      if (parm2 == 0)
		{
		  if (channelNr != 15)
		    {
		      ++channelNr;
		      numericCh->SetIntNumber(channelNr);
		    }
		}
	      else
		{
		  if (channelNr != 0)
		    {
		      if (--channelNr == 0)
			channelNr = 0;
		      numericCh->SetIntNumber(channelNr);
		    }
		}
	      break;
	      
	    default:
	      break;
	    }
	}
    case kC_TEXTENTRY:
      switch (parm1)
	{
	case MODULE_NUMBER:
	  switch (GET_SUBMSG(msg))
	    {
	    case kTE_ENTER:
	      moduleNr = numericMod->GetIntNumber();
	      numericMod->SetIntNumber(moduleNr);
	      break;
	    default:
	      break;
	    }
	case CHANNEL_NUMBER:
	  switch (GET_SUBMSG(msg))
	    {
	    case kTE_ENTER:
	      channelNr = numericCh->GetIntNumber();
	      numericCh->SetIntNumber(channelNr);
	      break;
	    default:
	      break;
	    }
	  break;

	default:
	  break;
	}

      /////////////////default for the most inclusive switch//////////          
    default:
      selected = gPad->GetSelected();
      break;
    }
  return kTRUE;
}

void Main::MakeFold1Panel(TGCompositeFrame * TabPanel)
{
  //make the buttons frame        
  TGCompositeFrame *ButtonFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);
  // BOOT button//////////////////////////////////////////////////////////////    
  TGTextButton *bootB = new TGTextButton(ButtonFrame, "Boot", BOOT_BUTTON);
  bootB->SetFont("-adobe-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1", false);
  fClient->GetColorByName("blue", color);
  bootB->SetTextColor(color, false);
  bootB->Associate(this);
  ButtonFrame->AddFrame(bootB, new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 10,10, 0));

  TabPanel->AddFrame(ButtonFrame, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  // status frame and status TGTextEntry holder//////////////////////////////////
  TGGroupFrame *StateMsgFrame = new TGGroupFrame(ButtonFrame, "Status", kVerticalFrame);

  StateMsgFold1 = new TGTextEntry(StateMsgFrame,
				  new TGTextBuffer(30), 10000,
				  StateMsgFold1->GetDefaultGC()(),
				  StateMsgFold1->GetDefaultFontStruct (),
				  kRaisedFrame | kDoubleBorder,
				  GetWhitePixel());
  StateMsgFold1->SetFont("-adobe-helvetica-bold-r-*-*-10-*-*-*-*-*-iso8859-1", false);

  fClient->GetColorByName("red", color);
  StateMsgFold1->SetTextColor(color, false);
  StateMsgFold1->SetText("System not booted");
  StateMsgFold1->Resize(100, 12);
  StateMsgFold1->SetEnabled(kFALSE);
  StateMsgFold1->SetFrameDrawn(kFALSE);

  /////////////
  StateMsgFrame->AddFrame(StateMsgFold1, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  ButtonFrame->AddFrame(StateMsgFrame, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 5, 1, 5));

  //////////////////////////////////////////////////////////////////////////
  TGTextButton *acquireB = new TGTextButton(ButtonFrame, "Read WF", READ_WF);
  fClient->GetColorByName("blue", color);
  acquireB->SetTextColor(color, false);
  acquireB->Associate(this);
  ButtonFrame->AddFrame(acquireB, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 10, 10, 0));
  //////////////////////////////////////////////////////////////////////////////

  TGTextButton *saveB = new TGTextButton(ButtonFrame, "  Save  ", SAVE_SEC);
  saveB->Associate(this);
  fClient->GetColorByName("blue", color);
  saveB->SetTextColor(color, false);
  saveB->SetToolTipText("Save waveform to radware .sec file", 0);
  ButtonFrame->AddFrame(saveB, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 10, 10, 0));
  ///Labels///////////////////////////////////////////////////////////////////////////
  TGVerticalFrame *fModChLabels = new TGVerticalFrame(ButtonFrame);

  TGLabel *mod = new TGLabel(fModChLabels, "Module  nr:");
  fModChLabels->AddFrame(mod, new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 3, 4, 0));
  TGLabel *ch = new TGLabel(fModChLabels, "Channel nr:");
  fModChLabels->AddFrame(ch, new TGLayoutHints(kLHintsLeft, 10, 3, 4, 0));
  ButtonFrame->AddFrame(fModChLabels, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  ///Number entries/////////////////////////////////////////////////////////////////////////
  TGVerticalFrame *fModCh = new TGVerticalFrame(ButtonFrame);
  numericMod = new TGNumberEntry(fModCh, 0, 4, MODULE_NUMBER, //module number entry
				 (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3/*kNELLimitMinMax*/, 0, PRESET_MAX_MODULES-1);
  numericMod->SetButtonToNum(0);
  fModCh->AddFrame(numericMod, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 3, 0, 0));
  numericMod->Associate(this);

  numericCh = new TGNumberEntry(fModCh, 0, 4, CHANNEL_NUMBER, //channel number entry
				(TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3/*kNELLimitMinMax*/, 0, 15);
  numericCh->SetButtonToNum(0);

  fModCh->AddFrame(numericCh, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 3, 0, 0));

  numericCh->Associate(this);
  ButtonFrame->AddFrame(fModCh, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  
  ////////////////////Canvas///////////////////////////////////////////////////                                  
  TGCompositeFrame *CanvasFrame = new TGCompositeFrame(TabPanel, 60, 60, kHorizontalFrame);
  TGLayoutHints *Hint = new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 1, 1, 1, 1);

  TRootEmbeddedCanvas *eCanvasF1 = new TRootEmbeddedCanvas("ec1", CanvasFrame, 100, 100);

  dCanvasF1 = eCanvasF1->GetCanvas();

  dCanvasF1->SetBorderMode(0); //no red frame
  CanvasFrame->AddFrame(eCanvasF1, Hint);
  TabPanel->AddFrame(CanvasFrame, Hint);

  test = new TGStatusBar(TabPanel, 10, 10);

  parts[0]=33;
  parts[1]=10;
  parts[2]=10;
  parts[3]=47;

  test->SetParts(parts, 4);

  TabPanel->AddFrame(test, new TGLayoutHints(kLHintsBottom | kLHintsLeft | kLHintsExpandX, 2, 2, 1, 1));

}

void Main::NewTrace(unsigned long size, unsigned short module, unsigned short ChanNum)
{
  if (trace == NULL)
    trace = new unsigned short[size];

  memset(trace, 0, size * sizeof(unsigned short));

  if (fHpx_wave == NULL)
    fHpx_wave = new TH1S("hpx_wave", "Live trace", size, 0,size);
  if(fHpx_wave == NULL) cout<<"Error!"<<endl;
  detector->AcquireADCTrace(trace, size, module, ChanNum);

  gStyle->SetOptStat(0);

  fHpx_wave->Reset();
  for (int i = 0; i < (int)size; i++){
    //		cout<<"T:"<< i <<" : " <<trace[i]<<endl;
    fHpx_wave->Fill(i, trace[i]);
    //		fHpx_wave->Fill(i, (Double_t)i);
  }

  //	if (!wave_once)
  //	{
  //		wave_once = true;
  //		fHpx_wave->SetMaximum(ymax);
  //		fHpx_wave->SetMinimum(ymin);
  //	}
  dCanvasF1->cd();
  fHpx_wave->DrawCopy("hist");
  dCanvasF1->Modified();
  dCanvasF1->Update();

  gSystem->ProcessEvents();

}

void Main::writeSpe(const char *filename, float buf[], int dim)
{
  struct spePrefix
  {
    int reclA; /* 24 */
    unsigned titleA;
    unsigned titleB;
    int dim;
    int a1; /*  1 */
    int a2; /*  1 */
    int a3; /*  1 */
    int reclB; /* 24 */
  } x = { 24, 0, 0, 0, 1, 1, 1, 24 };
  int recl;

  //de revenit -> trebe puse sigurante !
  // char buffer[100];
  ofstream out(filename, ios::out | ios::binary);
  x.dim = dim;
  recl = sizeof(float) * dim;
  out.write(reinterpret_cast < char *>(&x), sizeof(struct spePrefix));
  out.write(reinterpret_cast < char *>(&recl), sizeof (recl));
  out.write(reinterpret_cast < char *>(buf), sizeof(float) * dim);
  out.write(reinterpret_cast < char *>(&recl), sizeof (recl));
  out.close();
}

void Main::save_setup(char *name)
{
  int retval;
  retval = Pixie16SaveDSPParametersToFile(name);
  if(retval < 0)
    ErrorInfo("Main.cpp", "save_setup(...)", "Pixie16SaveDSPParametersToFile", retval);
  cout << "saving setup to file: " << name << endl;
}

void Main::MakeFold2Panel(TGCompositeFrame *TabPanel){
 
  // Set up for file store parametrs
  TGGroupFrame *filesetgroup = new TGGroupFrame(TabPanel,"Setup");
 
  // *** FILE path frame
  TGHorizontalFrame *filepath = new TGHorizontalFrame(filesetgroup);
  TGLabel *filepathlabel = new TGLabel(filepath,"File Path: ");
  filepath->AddFrame(filepathlabel,new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 3, 4, 0));
  filepathtext = new TGTextEntry(filepath,new TGTextBuffer(100));
  filepath->AddFrame(filepathtext,new TGLayoutHints(kLHintsExpandX|kLHintsTop, 10 ,3,4,0));
  
  filesetgroup->AddFrame(filepath,new TGLayoutHints(kLHintsExpandX|kLHintsTop));

  // *** File Name frame 
  TGHorizontalFrame *filenamef = new TGHorizontalFrame(filesetgroup);

  TGLabel *filenamelabel = new TGLabel(filenamef,"File Name: ");
  filenamef->AddFrame(filenamelabel,new TGLayoutHints(kLHintsLeft | kLHintsTop, 10 ,3,4,0));
  filenametext = new TGTextEntry(filenamef, new TGTextBuffer(20));
  filenamef->AddFrame(filenametext,new TGLayoutHints(kLHintsLeft| kLHintsTop,10,3,4,0));

  TGLabel *filerunlabel = new TGLabel(filenamef,"Run Num: ");
  filenamef->AddFrame(filerunlabel,new TGLayoutHints(kLHintsLeft| kLHintsTop,0,3,5,0));

  filerunnum = new TGNumberEntry(filenamef,0,5,999,TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative);
  filerunnum->SetButtonToNum(1);
  filenamef->AddFrame(filerunnum,new TGLayoutHints(kLHintsLeft|kLHintsTop,0,3,4,0));
  filerunnum->Associate(this);

  filesetdone = new TGTextButton(filenamef,"Complete");
  filesetdone->Connect("Pressed()","Main",this,"SetLSFileName()");
  filenamef->AddFrame(filesetdone,new TGLayoutHints(kLHintsLeft|kLHintsTop,0,3,4,0));
  filesetgroup->AddFrame(filenamef,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
  
  TabPanel->AddFrame(filesetgroup,new TGLayoutHints(kLHintsExpandX|kLHintsTop));

  // Control of list mode run
  TGGroupFrame *controlgroup = new TGGroupFrame(TabPanel,"Control");

  // start/stop LSM run button
  TGHorizontalFrame *cgrouphframe=new TGHorizontalFrame(controlgroup);
  startdaq = new TGTextButton(cgrouphframe,"LSRunStart");
  startdaq->Connect("Pressed()","Main",this,"StartLSRun()");
  startdaq->SetEnabled(0);
  cgrouphframe->AddFrame(startdaq,new TGLayoutHints(kLHintsLeft|kLHintsTop));
  // send/not send online data stream box
  onlinechk=new TGCheckButton(cgrouphframe,"&Online data");
  fClient->GetColorByName("black", color);
  onlinechk->SetTextColor(color);
  onlinechk->SetState(kButtonDown);
  fonlinedata = 1;
  onlinechk->Connect("Clicked()","Main",this,"SetLSonlinedataf()");
  cgrouphframe->AddFrame(onlinechk,new TGLayoutHints(kLHintsLeft|kLHintsTop,10,4,3,3));

  // save hitogram


  controlgroup->AddFrame(cgrouphframe,new TGLayoutHints(kLHintsExpandX|kLHintsTop));

  TabPanel->AddFrame(controlgroup,new TGLayoutHints(kLHintsExpandX|kLHintsTop));

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
}


void Main::SetLSFileName()
{
  if(detector == 0){
    cout<<"Modules not booted!"<<endl;
    return ;
  }
  const char *path=filepathtext->GetText();
  const char *filen=filenametext->GetText();
  runnum=(int)filerunnum->GetIntNumber();

  detector->SetRunNumber(runnum);
  for(int i = 0;i < detector->NumModules;i++)
    {
      sprintf(Filename[i],"%s%s_R%04d_M%02d.bin",path,filen,runnum,i);
      cout<<i<<" "<<Filename[i]<<endl;
    }

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
      cout<<"The output file directory does not exist"<<endl;
    }
}

void Main::StartLSRun()
{
  if(fstartdaq == 0)
    {
      SetLSFileName();
      detector->SetOnlineF(fonlinedata);

      for(int i = 0;i < detector->NumModules;i++)
	{
	  cout<<"open: "<<Filename[i]<<endl;
	  if(!detector->OpenSaveFile(i,Filename[i]))
	    {
	      cout<<Filename[i]<<endl;
	      return;
	    }
	}
      if(detector->StartLSMRun(0))
	{
	  cout<<"CANNOT start the LSM Run!"<<endl;
	  return;
	}
      sleep(2);// wait 2 seconds for modules to be ready
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
    }
}

void Main::LSRunReadData()
{
  cout<<"Main:: read loop.."<<endl;
  while(fstartdaq){
    detector->ReadDataFromModules(0,0); // during the run
    gSystem->ProcessEvents();
  }
  cout<<"done!!!!!!"<<endl;
  int counter = 0;
  while(detector->StopLSMRun()){
    // failed to stop run 
    sleep(1); // wait 1s then try again
    counter++;
    if(counter>10) break;
  }
  cout<<"finish!"<<endl;
}

void Main::SetLSonlinedataf()
{
  if(onlinechk->IsOn()){
    fonlinedata = 1;
    detector->SetOnlineF(1);
    cout<<"DAQ will send online data!"<<endl;
  }else{
    fonlinedata = 0;
    detector->SetOnlineF(0);
    cout<<"DAQ wont send online data!"<<endl;
  }
}

bool Main::IsDirectoryExists(const char *path)
{
  struct stat fileStat;
  if ((stat(path, &fileStat) == 0) && S_ISDIR(fileStat.st_mode))
    return true;
  else
    return false;
}
