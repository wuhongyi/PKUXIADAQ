// MainFrame.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 10月 23 15:43:08 2016 (+0800)
// Last-Updated: 三 10月 26 20:22:19 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 17
// URL: http://wuhongyi.github.io 

#include "MainFrame.hh"

#include "TString.h"
#include "TTree.h"
#include "TChain.h"
#include "TSystem.h"
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

  SetWindowName("PKU Pixie16 Offline");
  MapSubwindows();
  Resize(GetDefaultSize());
  MapWindow();
  // Resize(600, 500);
  
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
   // fMenuBarItemLayout = new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 4, 0, 0);

   // fMenuFile = new TGPopupMenu(gClient->GetRoot());
   // fMenuFile->AddEntry(new TGHotString("&New Window"), M_FILE_NEW);
   // fMenuFile->AddEntry(new TGHotString("&Close Window"), M_FILE_CLOSE);
   // fMenuFile->AddSeparator();
   // fMenuFile->AddEntry(new TGHotString("E&xit"), M_FILE_EXIT);

   // fMenuWindow = new TGPopupMenu(gClient->GetRoot());
   // fMenuWindow->AddEntry(new TGHotString("Tile &Horizontally"), M_WINDOW_HOR);
   // fMenuWindow->AddEntry(new TGHotString("Tile &Vertically"), M_WINDOW_VERT);
   // fMenuWindow->AddEntry(new TGHotString("&Cascade"), M_WINDOW_CASCADE);
   // fMenuWindow->AddSeparator();
   // fMenuWindow->AddPopup(new TGHotString("&Windows"), fMainFrame->GetWinListMenu());
   // fMenuWindow->AddSeparator();
   // fMenuWindow->AddEntry(new TGHotString("&Arrange icons"), M_WINDOW_ARRANGE);
   // fMenuWindow->AddSeparator();
   // fMenuWindow->AddEntry(new TGHotString("&Opaque resize"), M_WINDOW_OPAQUE);

   // fMenuWindow->CheckEntry(M_WINDOW_OPAQUE);

   // fMenuHelp = new TGPopupMenu(gClient->GetRoot());
   // fMenuHelp->AddEntry(new TGHotString("&Contents"), M_HELP_CONTENTS);
   // fMenuHelp->AddSeparator();
   // fMenuHelp->AddEntry(new TGHotString("&About"), M_HELP_ABOUT);

   // fMenuHelp->DisableEntry(M_HELP_CONTENTS);
   // fMenuHelp->DisableEntry(M_HELP_ABOUT);

   // // menu message are handled by the class' HandleMenu() method
   // fMenuFile->Connect("Activated(Int_t)", "TGAppMainFrame", this,
   //                    "HandleMenu(Int_t)");
   // fMenuWindow->Connect("Activated(Int_t)", "TGAppMainFrame", this,
   //                      "HandleMenu(Int_t)");
   // fMenuHelp->Connect("Activated(Int_t)", "TGAppMainFrame", this,
   //                    "HandleMenu(Int_t)");

   // fMenuBar->AddPopup(new TGHotString("&File"), fMenuFile, fMenuBarItemLayout);
   // fMenuBar->AddPopup(new TGHotString("&Windows"),fMenuWindow,fMenuBarItemLayout);
   // fMenuBar->AddPopup(new TGHotString("&Help"), fMenuHelp, fMenuBarItemLayout);

}


void MainFrame::HandleMenu(Int_t id)
{
   // Handle menu items.

   // switch (id) {
   //    case M_FILE_NEW:
   //       new TGMdiFrame(fMainFrame, 200, 100);
   //       break;

   //    case M_FILE_CLOSE:
   //       fMainFrame->Close(fMainFrame->GetCurrent());
   //       break;

   //    case M_FILE_EXIT:
   //       CloseWindow();
   //       break;

   //    case M_WINDOW_HOR:
   //       fMainFrame->TileHorizontal();
   //       break;

   //    case M_WINDOW_VERT:
   //       fMainFrame->TileVertical();
   //       break;

   //    case M_WINDOW_CASCADE:
   //       fMainFrame->Cascade();
   //       break;

   //    case M_WINDOW_ARRANGE:
   //       fMainFrame->ArrangeMinimized();
   //       break;

   //    case M_WINDOW_OPAQUE:
   //       if (fMenuWindow->IsEntryChecked(M_WINDOW_OPAQUE)) {
   //          fMenuWindow->UnCheckEntry(M_WINDOW_OPAQUE);
   //          fMainFrame->SetResizeMode(kMdiNonOpaque);
   //       } else {
   //          fMenuWindow->CheckEntry(M_WINDOW_OPAQUE);
   //          fMainFrame->SetResizeMode(kMdiOpaque);
   //       }
   //       break;

   //    default:
   //       fMainFrame->SetCurrent(id);
   //       break;
   // }
}


// 
// MainFrame.cc ends here
