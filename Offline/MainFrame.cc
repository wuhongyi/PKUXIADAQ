// MainFrame.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 10月 23 15:43:08 2016 (+0800)
// Last-Updated: 三 11月 23 14:03:42 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 155
// URL: http://wuhongyi.cn 

#include "MainFrame.hh"

#include "TString.h"
#include "TTree.h"
#include "TChain.h"
#include "TSystem.h"
#include "TApplication.h"
#include "TRootHelpDialog.h"

#include <sys/stat.h>//stat(const char *file_name,struct stat *buf)
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ClassImp(MainFrame)

MainFrame::MainFrame(const TGWindow * p, UInt_t w, UInt_t h)
: TGMainFrame(p, w, h)
{
  SetCleanup(kDeepCleanup);

  xia = NULL;
  energyTH1 = NULL;
  energyTH1_0 = NULL;

  // rawdata = NULL;
  // threshdata = NULL;
  // cfddata = NULL;
  // sfilterdata = NULL;
  // ffilterdata = NULL;
  // RcdTrace = NULL;
  // doublethresh = NULL;
  // doublesample = NULL;
  // doublercdtrace = NULL;
  // doublefastfilter = NULL;
  // doublecfd = NULL;
  // doubleslowfilter = NULL;
  // // adjustCanvas = new TCanvas();
  // offlinemultigraph = new TMultiGraph();
   
  InitMenu();
  TGTab *TabPanel = new TGTab(this);
  this->AddFrame(TabPanel, new TGLayoutHints(kLHintsBottom | kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
  TGCompositeFrame *Tab1 = TabPanel->AddTab("Init");
  MakeFoldPanelInit(Tab1);

  TGCompositeFrame *Tab2 = TabPanel->AddTab("Energy");
  MakeFoldPanelEnergy(Tab2);
  
  
  SetWindowName("PKU Pixie16-RevF Offline");
  MapSubwindows();
  Resize(GetDefaultSize());
  MapWindow();
  
  // AppendPad(); //foarte important
}

MainFrame::~MainFrame()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

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

      // ==================================
      
    case kC_COMMAND:
      switch (test2 = GET_SUBMSG(msg))
	{
	case kCM_MENU:
	  switch (parm1)
	    {




	    default:
	      break;
	    }
	  break;

	case kCM_BUTTON:
	  if(parm1 >= 100 && parm1 <=113)
	    {
	      if(parm1 == 113)
		{
		  for (int i = 0; i < modnum->GetSelected(); i++)
		    {
		      if(!modfilesetdone[i]->GetState())
			{
			  ReadModule(i);
			  sprintf(tempchar,"%d",xia->OfflineModuleEventsCount[i]);
			  ModCountStatusLabels[i]->SetText(tempchar);
			  if(xia->OfflineModuleEventsCount[i] > 0)
			    {
			      sprintf(tempchar,"%02d",i);
			      energymod->AddEntry(tempchar,i);
			    }
			  gSystem->ProcessEvents();
			}
		    }
		}
	      else
		{
		  ReadModule(parm1-100);

		  sprintf(tempchar,"%d",xia->OfflineModuleEventsCount[int(parm1-100)]);
		  ModCountStatusLabels[int(parm1-100)]->SetText(tempchar);
		  if(xia->OfflineModuleEventsCount[int(parm1-100)] > 0)
		  {
		    sprintf(tempchar,"%02d",int(parm1-100));
		    energymod->AddEntry(tempchar,int(parm1-100));
		  }
		}
	    }
	  else
	    {
	      switch (parm1)
		{


		default:
		  break;
		}
	    }
	      
	}
      break;

      // ==================================
      
    case kC_TEXTENTRY:
      switch (parm1)
	{
	// case MODULE_NUMBER:
	//   switch (GET_SUBMSG(msg))
	//     {
	//     case kTE_ENTER:

	//       break;
	//     default:
	//       break;
	//     }
	// case CHANNEL_NUMBER:
	//   switch (GET_SUBMSG(msg))
	//     {
	//     case kTE_ENTER:

	//       break;
	      
	//     default:
	//       break;
	}
      break;
         
    }

  return kTRUE;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MainFrame::InitMenu()
{
   fMenuBarItemLayout = new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 4, 0, 0);
   TGMenuBar *fMenuBar = new TGMenuBar(this, 1, 1, kHorizontalFrame);
   AddFrame(fMenuBar, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 5));
   
   fMenuFile = new TGPopupMenu(gClient->GetRoot());
   fMenuFile->AddEntry(new TGHotString("&Open"), M_FILE_OPEN, 0, gClient->GetPicture("bld_open.png"));
   fMenuFile->AddSeparator();
   fMenuFile->AddEntry(new TGHotString("E&xit"), M_FILE_EXIT, 0, gClient->GetPicture("bld_exit.png"));

   fMenuHelp = new TGPopupMenu(gClient->GetRoot());
   fMenuHelp->AddEntry(new TGHotString("&Contents"), M_HELP_CONTENTS);
   fMenuHelp->AddSeparator();
   fMenuHelp->AddEntry(new TGHotString("&About"), M_HELP_ABOUT);

   fMenuHelp->DisableEntry(M_HELP_CONTENTS);
   // fMenuHelp->DisableEntry(M_HELP_ABOUT);

   // menu message are handled by the class' HandleMenu() method
   fMenuFile->Connect("Activated(Int_t)", "MainFrame", this, "HandleMenu(Int_t)");
   fMenuHelp->Connect("Activated(Int_t)", "MainFrame", this, "HandleMenu(Int_t)");

   fMenuBar->AddPopup(new TGHotString("&File"), fMenuFile, fMenuBarItemLayout);
   fMenuBar->AddPopup(new TGHotString("&Help"), fMenuHelp, fMenuBarItemLayout);

}


void MainFrame::HandleMenu(Int_t id)
{
   // Handle menu items.

  TRootHelpDialog *hd;
  // static TString dir(".");
  // TGFileInfo fi;
  // fi.fFileTypes = dnd_types;
  // fi.fIniDir    = StrDup(dir);

  switch (id)
    {
  case M_FILE_OPEN:
    // new TGFileDialog(gClient->GetRoot(), this, kFDOpen, &fi);
    // dir = fi.fIniDir;
    // // doesn't do much, but can be used to open a root file...
    break;



  case M_FILE_EXIT:
    CloseWindow();
    break;
	 
  case M_HELP_CONTENTS:
     
    break;
	 
 
  case M_HELP_ABOUT:
    hd = new TRootHelpDialog(this, "About PKU Pixie16 Offline", 550, 250);
    hd->SetText(gHelpAbout);
    hd->Popup();
    break;


  }
}

void MainFrame::CloseWindow()
{
  gApplication->Terminate(0);
}


void MainFrame::MakeFoldPanelInit(TGCompositeFrame *TabPanel)
{
  // Set up for file store parametrs
  TGGroupFrame *filesetgroup = new TGGroupFrame(TabPanel,"Setup");
  TabPanel->AddFrame(filesetgroup,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
  
  // *** FILE path frame
  TGHorizontalFrame *filepathandname = new TGHorizontalFrame(filesetgroup);
  filesetgroup->AddFrame(filepathandname,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
  
  TGLabel *filepathlabel = new TGLabel(filepathandname,"File Path: ");
  filepathandname->AddFrame(filepathlabel,new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 3, 4, 0));
  filepathtext = new TGTextEntry(filepathandname,new TGTextBuffer(100));
  filepathandname->AddFrame(filepathtext,new TGLayoutHints(kLHintsLeft |kLHintsTop, 10 ,3,4,0));//kLHintsExpandX

  TGLabel *filenamelabel = new TGLabel(filepathandname,"File Name: ");
  filepathandname->AddFrame(filenamelabel,new TGLayoutHints(kLHintsLeft | kLHintsTop, 10 ,3,4,0));
  filenametext = new TGTextEntry(filepathandname, new TGTextBuffer(20));
  filepathandname->AddFrame(filenametext,new TGLayoutHints(kLHintsLeft| kLHintsTop,10,3,4,0));

  filesetdone = new TGTextButton(filepathandname,"Complete");
  filesetdone->Connect("Pressed()","MainFrame",this,"SetFileName()");
  filepathandname->AddFrame(filesetdone,new TGLayoutHints(kLHintsLeft|kLHintsTop,0,3,4,0));
  


  // Control of list mode run
  TGGroupFrame *controlgroup = new TGGroupFrame(TabPanel,"Control");
  TabPanel->AddFrame(controlgroup,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
  
  TGHorizontalFrame *readparfile = new TGHorizontalFrame(controlgroup);
  controlgroup->AddFrame(readparfile,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
  TGLabel *filedspparlabel = new TGLabel(readparfile,"DSP Par: ");
  readparfile->AddFrame(filedspparlabel,new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 3, 4, 0));
  filenamedsppar = new TGTextEntry(readparfile,new TGTextBuffer(30));
  readparfile->AddFrame(filenamedsppar,new TGLayoutHints(kLHintsLeft|kLHintsTop, 10 ,3,4,0));

  TGLabel *filedspvarlabel = new TGLabel(readparfile,"DSP Var: ");
  readparfile->AddFrame(filedspvarlabel,new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 3, 4, 0));
  filenamedspvar = new TGTextEntry(readparfile,new TGTextBuffer(60));
  readparfile->AddFrame(filenamedspvar,new TGLayoutHints(kLHintsLeft | kLHintsTop, 10 ,3,4,0));//kLHintsExpandX

  TGLabel *modnumlabel = new TGLabel(readparfile,"Total Mod: ");
  readparfile->AddFrame(modnumlabel,new TGLayoutHints(kLHintsLeft| kLHintsTop,10,3,5,0));

  modnum = new TGComboBox(readparfile);
  readparfile->AddFrame(modnum,new TGLayoutHints(kLHintsLeft|kLHintsTop,10,3,5,0));
  modnum->Resize(50, 20);
  for (int i = 1; i <= 13; ++i)
    {
      sprintf(tempchar,"%d",i);
      modnum->AddEntry(tempchar,i);
    }
  modnum->Select(1);
  
  initsetdone = new TGTextButton(readparfile,"Init");
  initsetdone->Connect("Pressed()","MainFrame",this,"InitSet()");
  readparfile->AddFrame(initsetdone,new TGLayoutHints(kLHintsLeft|kLHintsTop,10,3,5,0));
  

  TGHorizontalFrame *selectrunnumber = new TGHorizontalFrame(controlgroup);
  controlgroup->AddFrame(selectrunnumber,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
  
  TGLabel *filerunlabel = new TGLabel(selectrunnumber,"Run Num: ");
  selectrunnumber->AddFrame(filerunlabel,new TGLayoutHints(kLHintsLeft| kLHintsTop,10,3,5,0));

  filerunnum = new TGNumberEntry(selectrunnumber,0,5,999,TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative);
  filerunnum->SetButtonToNum(1);
  selectrunnumber->AddFrame(filerunnum,new TGLayoutHints(kLHintsLeft|kLHintsTop,10,3,5,0));
  filerunnum->Associate(this);

  fileinitsetdone = new TGTextButton(selectrunnumber,"InitFile");
  fileinitsetdone->Connect("Pressed()","MainFrame",this,"FileInitSet()");
  selectrunnumber->AddFrame(fileinitsetdone,new TGLayoutHints(kLHintsLeft|kLHintsTop,10,3,5,0));



  TGGroupFrame *filestatus = new TGGroupFrame(TabPanel,"Status");
  TabPanel->AddFrame(filestatus,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
  
  TGHorizontalFrame *filestatushf = new TGHorizontalFrame(filestatus);
  filestatus->AddFrame(filestatushf,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
  
  TGVerticalFrame *column1, *column2, *column3;
  column1 = new TGVerticalFrame(filestatushf, 200, 300);
  column2 = new TGVerticalFrame(filestatushf, 200, 300);
  column3 = new TGVerticalFrame(filestatushf, 200, 300);
  filestatushf->AddFrame(column1, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  filestatushf->AddFrame(column2, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  filestatushf->AddFrame(column3, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  TGTextEntry *te = new TGTextEntry(column1, new TGTextBuffer(100), 10000,
				    te->GetDefaultGC ()(),
				    te->GetDefaultFontStruct (),
				    kRaisedFrame | kDoubleBorder,
				    GetWhitePixel ());
  te->SetText("Mod #");
  te->Resize(35, 20);
  te->SetEnabled(kFALSE);
  te->SetFrameDrawn(kTRUE);
  column1->AddFrame(te, new TGLayoutHints(kLHintsCenterX, 0, 0, 10, 0));

  
  for (int i = 0; i < 13; i++)
    {
      ModStatusLabels[i] = new TGTextEntry(column1, new TGTextBuffer(100), 10000,
				  ModStatusLabels[i]->GetDefaultGC()(),
				  ModStatusLabels[i]->GetDefaultFontStruct(),
				  kRaisedFrame | kDoubleBorder,
				  GetWhitePixel());
      ModStatusLabels[i]->SetText("");
      ModStatusLabels[i]->Resize(35, 20);
      ModStatusLabels[i]->SetEnabled(kFALSE);
      ModStatusLabels[i]->SetFrameDrawn(kTRUE);

      column1->AddFrame(ModStatusLabels[i], new TGLayoutHints(kLHintsCenterX, 0, 3, 0, 0));
    }


  TGTextEntry *tee = new TGTextEntry(column2, new TGTextBuffer(100), 10000,
				    tee->GetDefaultGC ()(),
				    tee->GetDefaultFontStruct (),
				    kRaisedFrame | kDoubleBorder,
				    GetWhitePixel ());
  tee->SetText("status");
  tee->Resize(55, 20);
  tee->SetEnabled(kFALSE);
  tee->SetFrameDrawn(kTRUE);
  column2->AddFrame(tee, new TGLayoutHints(kLHintsCenterX, 0, 0, 10, 0));


  for (int i = 0; i < 14; i++)
    {
      if(i == 13)
	{
	  modfilesetdone[i] = new TGTextButton(column2,"Read  All",100+i);
	}
      else
	{
	  modfilesetdone[i] = new TGTextButton(column2,"Read File",100+i);
	}
      modfilesetdone[i]->Associate(this);
      modfilesetdone[i]->SetEnabled(0);
      column2->AddFrame(modfilesetdone[i],new TGLayoutHints(kLHintsLeft|kLHintsTop,0,3,0,0));
    }


  TGTextEntry *teee = new TGTextEntry(column3, new TGTextBuffer(100), 10000,
				    teee->GetDefaultGC()(),
				    teee->GetDefaultFontStruct(),
				    kRaisedFrame | kDoubleBorder,
				    GetWhitePixel());
  teee->SetText("Count");
  teee->Resize(100, 20);
  teee->SetEnabled(kFALSE);
  teee->SetFrameDrawn(kTRUE);
  column3->AddFrame(teee, new TGLayoutHints(kLHintsCenterX, 0, 0, 10, 0));

  
  for (int i = 0; i < 13; i++)
    {
      ModCountStatusLabels[i] = new TGTextEntry(column3, new TGTextBuffer(100), 10000,
				  ModCountStatusLabels[i]->GetDefaultGC()(),
				  ModCountStatusLabels[i]->GetDefaultFontStruct(),
				  kRaisedFrame | kDoubleBorder,
				  GetWhitePixel());
      ModCountStatusLabels[i]->SetText("");
      ModCountStatusLabels[i]->Resize(100, 20);
      ModCountStatusLabels[i]->SetEnabled(kFALSE);
      ModCountStatusLabels[i]->SetFrameDrawn(kTRUE);
      
      column3->AddFrame(ModCountStatusLabels[i], new TGLayoutHints(kLHintsCenterX, 0, 3, 0, 0));
    }
  

  
  
  std::ifstream in1("Run.config");
  if(!in1.is_open()) std::cout<<"Can't open file Run.config"<<std::endl;
  in1.getline(tempchar,sizeof(tempchar));
  filepathtext->Insert(tempchar);
  in1.getline(tempchar,sizeof(tempchar));
  filenametext->Insert(tempchar);
  in1.close();

  std::ifstream in2("DSP.config");
  if(!in2.is_open()) std::cout<<"Can't open file DSP.config"<<std::endl;
  in2.getline(tempchar,sizeof(tempchar));
  filenamedspvar->Insert(tempchar);
  in2.getline(tempchar,sizeof(tempchar));
  filenamedsppar->Insert(tempchar);
  in2.close();
  
}


void MainFrame::MakeFoldPanelEnergy(TGCompositeFrame *TabPanel)
{

  TGCompositeFrame *CanvasFrame = new TGCompositeFrame(TabPanel, 60, 60, kHorizontalFrame);
  TabPanel->AddFrame(CanvasFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 1, 1, 1, 1));
  

  TRootEmbeddedCanvas *EnergyCanvas = new TRootEmbeddedCanvas("Energyc1", CanvasFrame, 100, 100);
  CanvasFrame->AddFrame(EnergyCanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 1, 1, 1, 1));
  
  energyCanvas = EnergyCanvas->GetCanvas();
  energyCanvas->SetBorderMode(0); //no red frame


  



  //make the buttons frame        
  TGCompositeFrame *filterFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);
  TabPanel->AddFrame(filterFrame, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));


  // mod
  TGLabel *mod = new TGLabel(filterFrame, "Mod:"); 
  filterFrame->AddFrame(mod, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 0, 0));
  
  energymod = new TGComboBox(filterFrame);
  filterFrame->AddFrame(energymod, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 5, 0, 0));
  energymod->Resize(50, 20);
  

  // ch
  TGLabel *ch = new TGLabel(filterFrame, "Ch:"); 
  filterFrame->AddFrame(ch, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 0, 0));
  
  energych = new TGComboBox(filterFrame);
  filterFrame->AddFrame(energych, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 15, 0, 0));
  energych->Resize(50, 20);
  for (int i = 0; i < 16; ++i)
    {
      sprintf(tempchar,"%02d",i);
      energych->AddEntry(tempchar,i);
    }
  energych->Select(0);





  
  // slowlength
  TGLabel *lowlength = new TGLabel( filterFrame, "SLen:");
  filterFrame->AddFrame(lowlength, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 0, 0));
  energyfilter[0] = new TGNumberEntryField(filterFrame, -1, 0, TGNumberFormat::kNESReal);
  filterFrame->AddFrame(energyfilter[0], new TGLayoutHints( kLHintsExpandX | kLHintsTop, 1, 0, 0, 0));

  // slowgap
  TGLabel *lowgap = new TGLabel( filterFrame, "SGap:");
  filterFrame->AddFrame(lowgap, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 0, 0));
  energyfilter[1] = new TGNumberEntryField(filterFrame, -1, 0, TGNumberFormat::kNESReal);
  filterFrame->AddFrame(energyfilter[1], new TGLayoutHints( kLHintsExpandX | kLHintsTop, 1, 0, 0, 0));

  // preamptau
  TGLabel *preamptau = new TGLabel( filterFrame, "Tau:");
  filterFrame->AddFrame(preamptau, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 0, 0));
  energyfilter[2] = new TGNumberEntryField(filterFrame, -1, 0, TGNumberFormat::kNESReal);
  filterFrame->AddFrame(energyfilter[2], new TGLayoutHints( kLHintsExpandX | kLHintsTop, 1, 0, 0, 0));

  // slowrange
  TGLabel *slowrange = new TGLabel( filterFrame, "Range:");
  filterFrame->AddFrame(slowrange, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 0, 0));
  energyfilter[3] = new TGNumberEntryField(filterFrame, -1, 2, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 1, 6);
  filterFrame->AddFrame(energyfilter[3], new TGLayoutHints( kLHintsExpandX | kLHintsTop, 1, 0, 0, 0));



  
  energyLoad = new TGTextButton(filterFrame, "&Load");
  filterFrame->AddFrame(energyLoad, new TGLayoutHints(kLHintsLeft | kLHintsTop, 200, 0, 0, 0));
  energyLoad->Connect("Pressed()","MainFrame",this,"LoadPar_Energy()");

  energyApply = new TGTextButton(filterFrame, "&Apply");
  filterFrame->AddFrame(energyApply, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 0, 0, 0));
  energyApply->Connect("Pressed()","MainFrame",this,"ApplyPar_Energy()");

  energyDraw = new TGTextButton(filterFrame, "&Draw");
  filterFrame->AddFrame(energyDraw, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 0, 0, 0));
  energyDraw->Connect("Pressed()","MainFrame",this,"Draw_Energy()");
  

}






//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void MainFrame::SetFileName()
{
  if(IsDirectoryExists(filepathtext->GetText()))
    {
      std::ofstream out("Run.config");
      out<<filepathtext->GetText()<<std::endl;
      out<<filenametext->GetText()<<std::endl;
      out.close();
    }
  else//error
    {
      std::cout<<"Raw data directory "<<filepathtext->GetText() <<"  didn't exist."<<std::endl;
    }
  
}

void MainFrame::InitSet()
{
  if(xia != NULL) delete xia;
  xia = new algorithm();
  xia->InitSystem(modnum->GetSelected(),2);// 2 表示 RevF 100 MHz

  for (int i = 0; i < 13; i++)
    {
      if(i < modnum->GetSelected())
	{
	  sprintf(tempchar, "%d", i);
	  ModStatusLabels[i]->SetText(tempchar);
	}
      else
	{
	  ModStatusLabels[i]->SetText("");
	}
    }
  
  sprintf(tempchar,"%s",filenamedsppar->GetText());
  if(xia->LoadDSPParametersFromFile(tempchar) != 0)//error
    {

    }

  sprintf(tempchar,"%s",filenamedspvar->GetText());
  for (int i = 0; i < modnum->GetSelected(); ++i)
    {
      if(xia->Init_DSPVarAddress(tempchar, i) != 0)//error
	{
	  
	}
    }

  std::ofstream out("DSP.config");
  out<<filenamedspvar->GetText()<<std::endl;
  out<<filenamedsppar->GetText()<<std::endl;
  out.close();

  
}

void MainFrame::FileInitSet()
{
  for (int i = 0; i < 14; i++)
    {
      modfilesetdone[i]->SetEnabled(0);
    }
  for (int i = 0; i < 13; i++)
    {
      ModCountStatusLabels[i]->SetText("");
      xia->OfflineModuleEventsCount[i] = 0;
    }
  
  for (int i = 0; i < modnum->GetSelected(); i++)
    {
      sprintf(xia->offlinefilename[i],"%s%s_R%04d_M%02d.bin",filepathtext->GetText(),filenametext->GetText(),(int)filerunnum->GetIntNumber(),i);
      // std::cout<<i<<" "<<xia->offlinefilename[i]<<std::endl;

      if(IsFileExists(xia->offlinefilename[i]))
	{
	  modfilesetdone[i]->SetEnabled(1);
	}
    }
  modfilesetdone[13]->SetEnabled(1);

  energymod->RemoveAll();
}

 void MainFrame::ReadModule(int i)
{
  xia->OfflineModuleEventsCount[i] = xia->GetModuleEvents(xia->offlinefilename[i]);

  if(xia->OfflineModuleEventsCount[i] > 0)
    {
      if(xia->OfflineEventInformation[i] != NULL) delete xia->OfflineEventInformation[i];
      xia->OfflineEventInformation[i] = new unsigned int[12*xia->OfflineModuleEventsCount[i]];
      xia->GetEventsInfo(xia->offlinefilename[i],xia->OfflineEventInformation[i]);
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

bool MainFrame::IsFileExists(const char *name)
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


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MainFrame::LoadPar_Energy()
{
  double ChanParData = -1;
  unsigned int OfflinefRange;
  int retval; 
  char text[20];

  retval = xia->ReadSglChanPar((char*)"ENERGY_RISETIME", &ChanParData, energymod->GetSelected(), energych->GetSelected());
  sprintf(text, "%1.2f", ChanParData);
  energyfilter[0]->SetText(text);

  retval = xia->ReadSglChanPar((char*)"ENERGY_FLATTOP", &ChanParData, energymod->GetSelected(), energych->GetSelected());
  sprintf(text, "%1.2f", ChanParData);
  energyfilter[1]->SetText(text);

  
  retval = xia->ReadSglChanPar((char*)"TAU", &ChanParData, energymod->GetSelected(), energych->GetSelected());
  sprintf(text, "%1.2f", ChanParData);
  energyfilter[2]->SetText(text);

  
  retval = xia->ReadSglModPar((char*)"SLOW_FILTER_RANGE", &OfflinefRange, energymod->GetSelected());
  energyfilter[3]->SetIntNumber(OfflinefRange);
}

void MainFrame::ApplyPar_Energy()
{
  double ChanParData = -1;
  unsigned int OfflinefRange;
  int retval; 
  char text[20];
  
  OfflinefRange = (unsigned int)energyfilter[3]->GetIntNumber();
  retval = xia->WriteSglModPar((char*)"SLOW_FILTER_RANGE", OfflinefRange, energymod->GetSelected());

  ChanParData = energyfilter[0]->GetNumber();
  retval = xia->WriteSglChanPar((char*)"ENERGY_RISETIME", ChanParData, energymod->GetSelected(), energych->GetSelected());

  ChanParData = energyfilter[1]->GetNumber();
  retval = xia->WriteSglChanPar((char*)"ENERGY_FLATTOP", ChanParData, energymod->GetSelected(), energych->GetSelected());

  ChanParData = energyfilter[2]->GetNumber();
  retval = xia->WriteSglChanPar((char*)"TAU", ChanParData, energymod->GetSelected(), energych->GetSelected());

  LoadPar_Energy();
}
    
void MainFrame::Draw_Energy()
{
  if(energyTH1 != NULL) delete energyTH1;
  if(energyTH1_0 != NULL) delete energyTH1_0;
  energyTH1_0 = new TH1D("energy_read","",8192,0,16384);
  energyTH1_0->SetLineColor(1);
  energyTH1 = new TH1D("energy_compute","",8192,0,16384);

  // energymod->GetSelected(), energych->GetSelected()

  unsigned short RcdTrace[10000];
  
  FILE *ListModeFile = NULL;
  ListModeFile = fopen(xia->offlinefilename[energymod->GetSelected()], "rb");

  
  for (unsigned int i = 0; i < xia->OfflineModuleEventsCount[energymod->GetSelected()]; ++i)
    {
      if((unsigned int)energych->GetSelected() != xia->OfflineEventInformation[energymod->GetSelected()][12*i+1]) continue;

      // Position ListModeFile to the requested trace location
      fseek(ListModeFile, xia->OfflineEventInformation[energymod->GetSelected()][12*i+11]*4, SEEK_SET);
				
      // Read trace
      fread(RcdTrace, 2, xia->OfflineEventInformation[energymod->GetSelected()][12*i+10], ListModeFile);
		
      energyTH1->Fill(xia->ComputeEnergyOffline(energymod->GetSelected(), energych->GetSelected(),xia->OfflineEventInformation[energymod->GetSelected()][12*i+10],RcdTrace));
      energyTH1_0->Fill(xia->OfflineEventInformation[energymod->GetSelected()][12*i+9]);
    }
    
  // Close file
  fclose(ListModeFile);

  energyCanvas->cd();
  energyCanvas->Clear();

  energyTH1_0->Draw();
  energyTH1->Draw("same");
  
  energyCanvas->Modified();
  energyCanvas->Update();
  gSystem->ProcessEvents();
}






//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Bool_t MainFrame::DrawEntry(Long64_t entry)
// {
//   // b_sid->GetEntry(entry);
//   // b_ch->GetEntry(entry);
//   // b_ltra->GetEntry(entry);
//   // b_data->GetEntry(entry);
//   t->GetEntry(entry);
//   // b_dt->GetEntry(entry);

//   if(rawdata != NULL)
//     {
//       delete rawdata;
//       rawdata = NULL;
//     }
//   if(threshdata != NULL)
//     {
//       delete threshdata;
//       threshdata = NULL;
//     }
//   if(cfddata != NULL)
//     {
//       delete cfddata;
//       cfddata = NULL;
//     }
//   if(sfilterdata != NULL)
//     {
//       delete sfilterdata;
//       sfilterdata  = NULL;
//     }
//   if(ffilterdata != NULL)
//     {
//       delete ffilterdata;
//       ffilterdata = NULL;
//     }
//   if(RcdTrace != NULL)
//     {
//       delete []RcdTrace;
//       RcdTrace = NULL;
//     }
//   if(doublethresh != NULL)
//     {
//       delete []doublethresh;
//       doublethresh = NULL;
//     }
//   if(doublesample != NULL)
//     {
//       delete []doublesample;
//       doublesample = NULL;
//     }	      
//   if(doublercdtrace != NULL)
//     {
//       delete []doublercdtrace;
//       doublercdtrace = NULL;
//     }
//   if(doublefastfilter != NULL)
//     {
//       delete []doublefastfilter;
//       doublefastfilter = NULL;
//     }
//   if(doublecfd != NULL)
//     {
//       delete []doublecfd;
//       doublecfd = NULL;
//     }
//   if(doubleslowfilter != NULL)
//     {
//       delete []doubleslowfilter;
//       doubleslowfilter = NULL;
//     }

//   RcdTrace = new unsigned short[ltra];
//   doublethresh = new double[ltra];
//   doublesample = new double[ltra];
//   doublercdtrace = new double[ltra];
//   doublefastfilter = new double[ltra];
//   doublecfd = new double[ltra];
//   doubleslowfilter = new double[ltra];	      

//   for (int i = 0; i < ltra; ++i)
//     {
//       RcdTrace[i] = data[i];
//     }
	      
//   xia->ComputeFastFiltersOffline((unsigned short)(sid-2), (unsigned short) ch,(unsigned short) ltra, RcdTrace, doublefastfilter, doublecfd );
//   xia->ComputeSlowFiltersOffline((unsigned short)(sid-2), (unsigned short) ch, (unsigned short)ltra, RcdTrace,doubleslowfilter );

//   for (int i = 0; i < ltra; ++i)
//     {
//       doublesample[i] = i;
//       doublercdtrace[i] = (double)data[i];
//       double TRIGGER_THRESHOLD;
//       xia->ReadSglChanPar((char *)"TRIGGER_THRESHOLD",&TRIGGER_THRESHOLD,(unsigned short)(sid-2),(unsigned short) ch);
//       doublethresh[i] = TRIGGER_THRESHOLD;
//     }

//   rawdata = new TGraph(ltra,doublesample,doublercdtrace);
//   threshdata = new TGraph(ltra,doublesample,doublethresh);
//   cfddata = new TGraph(ltra,doublesample,doublecfd);
//   sfilterdata = new TGraph(ltra,doublesample,doubleslowfilter);
//   ffilterdata  = new TGraph(ltra,doublesample,doublefastfilter);

//   adjustCanvas->cd();
//   adjustCanvas->Clear();
//   cfddata->SetLineColor(2);
//   sfilterdata->SetLineColor(3);
//   ffilterdata->SetLineColor(4);
//   threshdata->SetLineColor(5);
//   // rawdata->SetLineWidth(3);
//   // cfddata->SetLineWidth(3);
//   // sfilterdata->SetLineWidth(3);
//   // cfddata->SetLineWidth(3);
//   offlinemultigraph->Clear();
//   offlinemultigraph->Add(rawdata);
//   offlinemultigraph->Add(threshdata);
//   offlinemultigraph->Add(cfddata);
//   offlinemultigraph->Add(sfilterdata);
//   offlinemultigraph->Add(ffilterdata);
//   offlinemultigraph->Draw("AL");
//   adjustCanvas->Modified();
//   adjustCanvas->Update();
//   // gSystem->ProcessEvents();	 
	      
//   return true; 
// }



// 
// MainFrame.cc ends here
