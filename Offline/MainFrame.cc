// MainFrame.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 10月 23 15:43:08 2016 (+0800)
// Last-Updated: 四 10月 27 21:53:39 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 36
// URL: http://wuhongyi.github.io 

#include "MainFrame.hh"

#include "TString.h"
#include "TTree.h"
#include "TChain.h"
#include "TSystem.h"
#include "TApplication.h"
#include "TRootHelpDialog.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ClassImp(MainFrame)

MainFrame::MainFrame(const TGWindow * p, UInt_t w, UInt_t h)
: TGMainFrame(p, w, h)
{
  SetCleanup(kDeepCleanup);
  
  rawdata = NULL;
  threshdata = NULL;
  cfddata = NULL;
  sfilterdata = NULL;
  ffilterdata = NULL;
  RcdTrace = NULL;
  doublethresh = NULL;
  doublesample = NULL;
  doublercdtrace = NULL;
  doublefastfilter = NULL;
  doublecfd = NULL;
  doubleslowfilter = NULL;
  // adjustCanvas = new TCanvas();
  offlinemultigraph = new TMultiGraph();

  

  xia = new algorithm();
  xia->InitSystem(3,2);

  char  *FileName = (char *)"20161007.set";
  if(xia->LoadDSPParametersFromFile(FileName ) == 0)
    
  if(xia->Init_DSPVarAddress((char *)"Pixie16DSP_revfgeneral_14b100m_r33354.var", 0) == 0)
    printf("zzzzzzzzz\n");
  xia->Init_DSPVarAddress((char *)"Pixie16DSP_revfgeneral_14b100m_r33354.var", 1);
  xia->Init_DSPVarAddress((char *)"Pixie16DSP_revfgeneral_14b100m_r33354.var", 2);
   
  double ENERGY_RISETIME;
  double ENERGY_FLATTOP;
  unsigned int SLOW_FILTER_RANGE;
  xia->ReadSglChanPar((char *)"ENERGY_RISETIME",&ENERGY_RISETIME,0,2);
  xia->ReadSglChanPar((char *)"ENERGY_FLATTOP",&ENERGY_FLATTOP,0,2);
  xia->ReadSglModPar((char *)"SLOW_FILTER_RANGE",&SLOW_FILTER_RANGE,0);
  std::cout<<SLOW_FILTER_RANGE<<"  "<<ENERGY_RISETIME<<"  "<<ENERGY_FLATTOP<<std::endl;

  SLOW_FILTER_RANGE = 1;
  xia->WriteSglModPar((char *)"SLOW_FILTER_RANGE",SLOW_FILTER_RANGE,0);
  xia->ReadSglChanPar((char *)"ENERGY_RISETIME",&ENERGY_RISETIME,0,2);
  xia->ReadSglChanPar((char *)"ENERGY_FLATTOP",&ENERGY_FLATTOP,0,2);
  xia->ReadSglModPar((char *)"SLOW_FILTER_RANGE",&SLOW_FILTER_RANGE,0);
  std::cout<<SLOW_FILTER_RANGE<<"  "<<ENERGY_RISETIME<<"  "<<ENERGY_FLATTOP<<std::endl; 

  InitMenu();
  TGTab *TabPanel = new TGTab(this);
  this->AddFrame(TabPanel, new TGLayoutHints(kLHintsBottom | kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
  TGCompositeFrame *Tab1 = TabPanel->AddTab("Init");
  MakeFoldPanelInit(Tab1);
  
  SetWindowName("PKU Pixie16 Offline");
  MapSubwindows();
  Resize(GetDefaultSize());
  // Resize(600, 500);
  MapWindow();

  
  // AppendPad(); //foarte important


  // DrawEntry(500);
}

MainFrame::~MainFrame()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Bool_t MainFrame::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{

  return kTRUE;
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
  filesetdone->Connect("Pressed()","MainFrame",this,"SetFileName()");
  filenamef->AddFrame(filesetdone,new TGLayoutHints(kLHintsLeft|kLHintsTop,0,3,4,0));
  filesetgroup->AddFrame(filenamef,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
  
  TabPanel->AddFrame(filesetgroup,new TGLayoutHints(kLHintsExpandX|kLHintsTop));


  
}

void MainFrame::SetFileName()
{

}



// 
// MainFrame.cc ends here
