// Offline.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 7月 29 20:39:43 2016 (+0800)
// Last-Updated: 四 8月 24 15:54:29 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 234
// URL: http://wuhongyi.cn 

#include "Offline.hh"
#include "Detector.hh"
#include "Global.hh"

#include "pixie16app_export.h"
#include "pixie16sys_export.h"

#include "TGTab.h"
#include "TString.h"

#include <iostream>
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Offline::Offline(const TGWindow * p, const TGWindow * main,Detector *det,TGTextEntry *filepath, TGTextEntry *filename)
{
  SetCleanup(kDeepCleanup);
  detector = det;
  filepathtext = filepath;
  filenametext = filename;

  modNumber = 0;
  chanNumber = 0;
  chanNumber4 = 0;
  chanNumber5 = 0;
  chanNumber6 = 0;
  fileRunNum = 0;
  
  OfflineEventInformation = NULL;
  OfflineCurrentCount = -1;
  rawdata = NULL;
  threshdata = NULL;
  cfdthreshdata = NULL;
  cfddata = NULL;
  sfilterdata = NULL;
  ffilterdata = NULL;
  RcdTrace = NULL;
  doublethresh = NULL;
  doublecfdthresh = NULL;
  doublesample = NULL;
  doublercdtrace = NULL;
  doublefastfilter = NULL;
  doublecfd = NULL;
  doubleslowfilter = NULL;
  offlinemultigraph = new TMultiGraph();

  for (int i = 0; i < 16; ++i)
    {
      offlinemultigraph2[i] = new TMultiGraph();
      OfflineCurrentCount2[i] = -1;
      
      rawdata2[i] = NULL;
      threshdata2[i] = NULL;
      cfdthreshdata2[i] = NULL;
      cfddata2[i] = NULL;
      sfilterdata2[i] = NULL;
      ffilterdata2[i] = NULL;
      RcdTrace2[i] = NULL;
      doublethresh2[i] = NULL;
      doublesample2[i] = NULL;
      doublercdtrace2[i] = NULL;
      doublefastfilter2[i] = NULL;
      doublecfd2[i] = NULL;
      doubleslowfilter2[i] = NULL;
    }

  for (int i = 0; i < 16; ++i)
    {
      offlineth1d3[i] = NULL;
    }

  offlineth1d4 = NULL;
  offlineth2d5_0 = NULL;
  offlineth2d5_1 = NULL;
  originalcfdth1d5 = NULL;
  calculatecfdth1d5 = NULL;
  RcdTrace5 = NULL;
  doublefastfilter5 = NULL;
  doublecfd5 = NULL;
  offlineth2d6 = NULL;

  
  TGTab *TabPanel = new TGTab(this);
  this->AddFrame(TabPanel, new TGLayoutHints(kLHintsBottom | kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
  TGCompositeFrame *Tab0 = TabPanel->AddTab("InitData");
  MakeFold0Panel(Tab0);
  
  TGCompositeFrame *Tab1 = TabPanel->AddTab("Adjust Par");
  MakeFold1Panel(Tab1);

  TGCompositeFrame *Tab2 = TabPanel->AddTab("Wave-16");
  MakeFold2Panel(Tab2);

  TGCompositeFrame *Tab3 = TabPanel->AddTab("Energy-16");
  MakeFold3Panel(Tab3);

  TGCompositeFrame *Tab4 = TabPanel->AddTab("Energy");
  MakeFold4Panel(Tab4);

  TGCompositeFrame *Tab5 = TabPanel->AddTab("FF/CFD Thre");
  MakeFold5Panel(Tab5);
  
  TGCompositeFrame *Tab6 = TabPanel->AddTab("test");
  MakeFold6Panel(Tab6);
  
  SetWindowName("Adjust Par");
  MapSubwindows();
  MapWindow();
  Resize();

  gStyle->SetOptStat(0);//不显示统计框
}

Offline::~Offline()
{
  if(rawdata != NULL)
    delete rawdata;
  if(threshdata != NULL)
    delete threshdata;
  if(cfddata != NULL)
    delete cfddata;
  if(cfdthreshdata != NULL)
    delete cfdthreshdata;
  if(sfilterdata != NULL)
    delete sfilterdata;
  if(ffilterdata != NULL)
    delete ffilterdata;
  if(RcdTrace != NULL)
    delete []RcdTrace;
  if(doublethresh != NULL)
    delete []doublethresh;
  if(doublecfdthresh != NULL)
    delete []doublecfdthresh;
  if(doublesample != NULL)
    delete []doublesample;
  if(doublercdtrace != NULL)
    delete []doublercdtrace;
  if(doublefastfilter != NULL)
    delete []doublefastfilter;
  if(doublecfd != NULL)
    delete []doublecfd;
  if(doubleslowfilter != NULL)
    delete []doubleslowfilter;

  if(RcdTrace5 != NULL)
    delete []RcdTrace5;
    if(doublefastfilter5 != NULL)
    delete []doublefastfilter5;
  if(doublecfd5 != NULL)
    delete []doublecfd5;
  
  for (int i = 0; i < 16; ++i)
    {
      if(rawdata2[i] != NULL)
  	delete rawdata2[i];
      if(threshdata2[i] != NULL)
  	delete threshdata2[i];
      if(cfdthreshdata2[i] != NULL)
  	delete cfdthreshdata2[i];
      if(cfddata2[i] != NULL)
  	delete cfddata2[i];
      if(sfilterdata2[i] != NULL)
  	delete sfilterdata2[i];
      if(ffilterdata2[i] != NULL)
  	delete ffilterdata2[i];
      if(RcdTrace2[i] != NULL)
  	delete []RcdTrace2[i];
      if(doublethresh2[i] != NULL)
  	delete []doublethresh2[i];
      if(doublesample2[i] != NULL)
  	delete []doublesample2[i];	      
      if(doublercdtrace2[i] != NULL)
  	delete []doublercdtrace2[i];
      if(doublefastfilter2[i] != NULL)
  	delete []doublefastfilter2[i];
      if(doublecfd2[i] != NULL)
  	delete []doublecfd2[i];
      if(doubleslowfilter2[i] != NULL)
  	delete []doubleslowfilter2[i];
    }

  if(OfflineEventInformation != NULL)
    delete []OfflineEventInformation;
     
  if(offlinemultigraph != NULL)
    delete offlinemultigraph;

  for (int i = 0; i < 16; ++i)
    {
      if(offlinemultigraph2[i] != NULL)
	delete offlinemultigraph2[i];
      
      if(offlineth1d3[i] != NULL)
	{
	  delete offlineth1d3[i];
	}
    }

  if(offlineth1d4 != NULL)
    delete offlineth1d4;
  if(offlineth2d5_0 != NULL)
    delete offlineth2d5_0;
  if(offlineth2d5_1 != NULL)
    delete offlineth2d5_1;
  if(originalcfdth1d5 != NULL)
    delete originalcfdth1d5;
  if(calculatecfdth1d5 != NULL)
    delete calculatecfdth1d5;
  
  if(offlineth2d6 != NULL)
    delete offlineth2d6;
}

void Offline::MakeFold0Panel(TGCompositeFrame *TabPanel)
{
  TGCompositeFrame *LogoFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);

  TGImageMap* fImagePKU = new TGImageMap(LogoFrame, "pkulogo100.jpg");
  fImagePKU->Resize(100,100);
  fImagePKU->ChangeOptions(fImagePKU->GetOptions() | kFixedSize);
  LogoFrame->AddFrame(fImagePKU,new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 0, 0, 0));

  TGImageMap* fImageWHY = new TGImageMap(LogoFrame, "why.jpg");
  fImageWHY->Resize(100,100);
  fImageWHY->ChangeOptions(fImageWHY->GetOptions() | kFixedSize);
  LogoFrame->AddFrame(fImageWHY,new TGLayoutHints(kLHintsTop | kLHintsRight, 100, 0, 0, 0));

  TabPanel->AddFrame(LogoFrame, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  TGCompositeFrame *parFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);

  // run
  TGLabel *run = new TGLabel( parFrame, "Run:");
  parFrame->AddFrame(run, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 3, 0));
  offlinefilerunnum = new TGNumberEntry(parFrame, 0, 4, OFFLINERUNNUM, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, 9999);
  offlinefilerunnum->SetButtonToNum(0);
  offlinefilerunnum->Associate(this);
  parFrame->AddFrame(offlinefilerunnum, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 0, 0));

  // mod
  TGLabel *mod = new TGLabel( parFrame, "Mod:"); 
  parFrame->AddFrame(mod, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 3, 0));
  offlinemodnum = new TGNumberEntry (parFrame, 0, 2, OFFLINEMODNUM, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, PRESET_MAX_MODULES-1);
  offlinemodnum->SetButtonToNum(0);
  offlinemodnum->Associate(this);
  parFrame->AddFrame(offlinemodnum, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 0, 0));

  // read
  OfflineReadFileButton = new TGTextButton( parFrame, "&Read", OFFLINEREAD);
  OfflineReadFileButton->Associate (this);
  parFrame->AddFrame(OfflineReadFileButton, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 0, 0));

  // status
  OfflineFileStatus = new TGTextEntry(parFrame,new TGTextBuffer(30), 10000);
  OfflineFileStatus-> SetFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1", false);
  fClient->GetColorByName("blue", color);
  OfflineFileStatus->SetTextColor(color, false);
  OfflineFileStatus->SetText("NOT READ");
  OfflineFileStatus->Resize(200, 12);
  OfflineFileStatus->SetEnabled(kFALSE);
  OfflineFileStatus->SetFrameDrawn(kFALSE);
  parFrame->AddFrame(OfflineFileStatus, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 6, 0));// 


  
  TabPanel->AddFrame(parFrame, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 2, 2, 20, 1));
}


void Offline::MakeFold1Panel(TGCompositeFrame *TabPanel)
{
  TGCompositeFrame *adCanvasFrame = new TGCompositeFrame(TabPanel, 800, 800, kHorizontalFrame);
  TGLayoutHints *Hint = new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 1, 1, 1, 1);

  TRootEmbeddedCanvas *adjCanvas = new TRootEmbeddedCanvas("offlineadcanvas", adCanvasFrame, 100, 100);

  adjustCanvas = adjCanvas->GetCanvas();
  adCanvasFrame->AddFrame(adjCanvas, Hint);
  TabPanel->AddFrame(adCanvasFrame, Hint);

  // ===
  TGCompositeFrame *filterFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);

  // fastlength
  TGLabel *fastlength = new TGLabel( filterFrame, "FLen:");
  fClient->GetColorByName("blue", color);
  fastlength->SetTextColor(color, false);
  filterFrame->AddFrame(fastlength, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 3, 0));
  offlinefilters[0] = new TGNumberEntryField(filterFrame, OFFLINEFASTLENGTH, 0, TGNumberFormat::kNESReal);
  filterFrame->AddFrame(offlinefilters[0], new TGLayoutHints( kLHintsExpandX | kLHintsTop, 1, 0, 0, 0));

  // fastgap
  TGLabel *fastgap = new TGLabel( filterFrame, "FGap:");
  fClient->GetColorByName("blue", color);
  fastgap->SetTextColor(color, false);
  filterFrame->AddFrame(fastgap, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 3, 0));
  offlinefilters[1] = new TGNumberEntryField(filterFrame, OFFLINEFASTGAP, 0, TGNumberFormat::kNESReal);
  filterFrame->AddFrame(offlinefilters[1], new TGLayoutHints( kLHintsExpandX | kLHintsTop, 1, 0, 0, 0));

  // fast filter thresh
  TGLabel *thresh = new TGLabel( filterFrame, "FFThr:");
  fClient->GetColorByName("blue", color);
  thresh->SetTextColor(color, false);
  filterFrame->AddFrame(thresh, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 3, 0));
  offlinefilters[7] = new TGNumberEntryField(filterFrame, OFFLINETHRESH, 0, TGNumberFormat::kNESReal);
  filterFrame->AddFrame(offlinefilters[7], new TGLayoutHints( kLHintsExpandX | kLHintsTop, 1, 0, 0, 0));


  // cfddelay
  TGLabel *cfddelay = new TGLabel( filterFrame, "CFDDe:");
  fClient->GetColorByName("red", color);
  cfddelay->SetTextColor(color, false);
  filterFrame->AddFrame(cfddelay, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 3, 0));
  offlinefilters[5] = new TGNumberEntryField(filterFrame, OFFLINECFDDELAY, 0, TGNumberFormat::kNESReal);
  filterFrame->AddFrame(offlinefilters[5], new TGLayoutHints( kLHintsExpandX | kLHintsTop, 1, 0, 0, 0));

  // cfdscale
  TGLabel *cfdscale = new TGLabel( filterFrame, "CFDSc:");
  fClient->GetColorByName("red", color);
  cfdscale->SetTextColor(color, false);
  filterFrame->AddFrame(cfdscale, new TGLayoutHints (kLHintsLeft | kLHintsTop, 1, 2, 3, 0));
  offlinefilters[6] = new TGNumberEntryField(filterFrame, OFFLINECFDSCALE, 0, TGNumberFormat::kNESReal);
  filterFrame->AddFrame(offlinefilters[6], new TGLayoutHints( kLHintsExpandX | kLHintsTop, 1, 0, 0, 0));

  // cfd thresh
  TGLabel *cfdthresh = new TGLabel( filterFrame, "CFDThr:");
  fClient->GetColorByName("red", color);
  cfdthresh->SetTextColor(color, false);
  filterFrame->AddFrame(cfdthresh, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 3, 0));
  offlinefilters[8] = new TGNumberEntryField(filterFrame, OFFLINECFDTHRESH, 0, TGNumberFormat::kNESReal);
  filterFrame->AddFrame(offlinefilters[8], new TGLayoutHints( kLHintsExpandX | kLHintsTop, 1, 0, 0, 0));
  
  // slowlength
  TGLabel *lowlength = new TGLabel( filterFrame, "SLen:");
  fClient->GetColorByName("green", color);
  lowlength->SetTextColor(color, false);
  filterFrame->AddFrame(lowlength, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 3, 0));
  offlinefilters[2] = new TGNumberEntryField(filterFrame, OFFLINESLOWLENGTH, 0, TGNumberFormat::kNESReal);
  filterFrame->AddFrame(offlinefilters[2], new TGLayoutHints( kLHintsExpandX | kLHintsTop, 1, 0, 0, 0));

  // slowgap
  TGLabel *lowgap = new TGLabel( filterFrame, "SGap:");
  fClient->GetColorByName("green", color);
  lowgap->SetTextColor(color, false);
  filterFrame->AddFrame(lowgap, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 3, 0));
  offlinefilters[3] = new TGNumberEntryField(filterFrame, OFFLINESLOWGAP, 0, TGNumberFormat::kNESReal);
  filterFrame->AddFrame(offlinefilters[3], new TGLayoutHints( kLHintsExpandX | kLHintsTop, 1, 0, 0, 0));

  // preamptau
  TGLabel *preamptau = new TGLabel( filterFrame, "Tau:");
  fClient->GetColorByName("green", color);
  preamptau->SetTextColor(color, false);
  filterFrame->AddFrame(preamptau, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 3, 0));
  offlinefilters[4] = new TGNumberEntryField(filterFrame, OFFLINEPREAMPTAU, 0, TGNumberFormat::kNESReal);
  filterFrame->AddFrame(offlinefilters[4], new TGLayoutHints( kLHintsExpandX | kLHintsTop, 1, 0, 0, 0));


  // slowfilterrange
  TGLabel *slowfilterrange = new TGLabel(filterFrame, "FilRan:");
  fClient->GetColorByName("purple", color);
  slowfilterrange->SetTextColor(color, false);
  filterFrame->AddFrame(slowfilterrange, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 3, 0));
  offlinefilterrange = new TGNumberEntry(filterFrame, 0, 2, OFFLINEFILTERRANGE, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 1, 6);
  filterFrame->AddFrame(offlinefilterrange, new TGLayoutHints( kLHintsExpandX | kLHintsTop, 1, 0, 0, 0));
  offlinefilterrange->SetButtonToNum(0);
  offlinefilterrange->Associate(this);

  
  TabPanel->AddFrame(filterFrame, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 2, 2, 1, 1));

  // ===

  TGCompositeFrame *parFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);

  // current count
  OfflineCurrentCountText = new TGTextEntry(parFrame,new TGTextBuffer(30), 10000);
  OfflineCurrentCountText-> SetFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1", false);
  fClient->GetColorByName("blue", color);
  OfflineCurrentCountText->SetTextColor(color, false);
  OfflineCurrentCountText->SetText("-1");
  OfflineCurrentCountText->Resize(100, 12);
  OfflineCurrentCountText->SetEnabled(kFALSE);
  OfflineCurrentCountText->SetFrameDrawn(kFALSE);
  parFrame->AddFrame(OfflineCurrentCountText, new TGLayoutHints(kLHintsRight | kLHintsTop, 10, 0, 6, 0));

  // draw
  OfflineDrawButton = new TGTextButton( parFrame, "&Draw", OFFLINEDRAW);
  OfflineDrawButton->SetEnabled(0);
  OfflineDrawButton->Associate(this);
  parFrame->AddFrame(OfflineDrawButton, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 10, 0, 0));

  // apply
  OfflineApplyButton = new TGTextButton( parFrame, "&Apply", OFFLINEAPPLY);
  OfflineApplyButton->Associate(this);
  parFrame->AddFrame( OfflineApplyButton , new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 5, 0, 0));

  // load
  OfflineLoadButton = new TGTextButton( parFrame, "&Load", OFFLINELOAD);
  OfflineLoadButton->Associate(this);
  parFrame->AddFrame( OfflineLoadButton, new TGLayoutHints(kLHintsRight | kLHintsTop, 20, 5, 0, 0));

  // ch
  offlinechnum = new TGNumberEntry (parFrame, 0, 2, OFFLINECHNUM, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, 15);
  parFrame->AddFrame(offlinechnum, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 0, 0, 0));
  offlinechnum->SetButtonToNum(0);
  offlinechnum->Associate(this);
  TGLabel *ch = new TGLabel( parFrame, "Ch:"); 
  parFrame->AddFrame(ch, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 2, 3, 0));


  
  TabPanel->AddFrame(parFrame, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 2, 2, 1, 1));


  // ===
  dslider = new TGDoubleHSlider(TabPanel,4, kDoubleScaleBoth,ADJUSTPARSLIDER);
  dslider->Associate(this);
  dslider->SetRange(0,5000);
  dslider->SetPosition(0,5000);
  TabPanel->AddFrame(dslider, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 2, 2, 1, 1));
  // GOTO
  // connect to picture


  // ===
  sbfold3 = new TGStatusBar(TabPanel,10,10);
  int temp[4];
  temp[0] = 33;
  temp[1] = 10;
  temp[2] = 10;
  temp[3] = 47;
  sbfold3->SetParts(temp,4);
  //GOTO 
  // connect to canvas
  TabPanel->AddFrame(sbfold3, new TGLayoutHints(kLHintsBottom | kLHintsLeft | kLHintsExpandX, 2, 2, 1, 1));
}

void Offline::MakeFold2Panel(TGCompositeFrame *TabPanel)
{
  TGCompositeFrame *parFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);
  
  // draw
  OfflineDrawButton2 = new TGTextButton( parFrame, "&Draw", OFFLINEDRAW2);
  OfflineDrawButton2->SetEnabled(0);
  OfflineDrawButton2->Associate(this);
  parFrame->AddFrame(OfflineDrawButton2, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 30, 0, 0));

  TabPanel->AddFrame(parFrame, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 2, 2, 1, 1));
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGCompositeFrame *adCanvasFrame = new TGCompositeFrame(TabPanel, 800, 800, kHorizontalFrame);
  TGLayoutHints *Hint = new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 1, 1, 1, 1);

  TRootEmbeddedCanvas *adjCanvas = new TRootEmbeddedCanvas("canvas2", adCanvasFrame, 100, 100);

  canvas2 = adjCanvas->GetCanvas();
  canvas2->Divide(4,4);
  adCanvasFrame->AddFrame(adjCanvas, Hint);
  TabPanel->AddFrame(adCanvasFrame, Hint);
}

void Offline::MakeFold3Panel(TGCompositeFrame *TabPanel)
{
  TGCompositeFrame *parFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);

  // draw
  OfflineDrawButton3 = new TGTextButton( parFrame, "&Draw", OFFLINEDRAW3);
  OfflineDrawButton3->SetEnabled(0);
  OfflineDrawButton3->Associate(this);
  parFrame->AddFrame(OfflineDrawButton3, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 30, 0, 0));

  TabPanel->AddFrame(parFrame, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 2, 2, 1, 1));

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGCompositeFrame *adCanvasFrame = new TGCompositeFrame(TabPanel, 800, 800, kHorizontalFrame);
  TGLayoutHints *Hint = new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 1, 1, 1, 1);

  TRootEmbeddedCanvas *adjCanvas = new TRootEmbeddedCanvas("canvas3", adCanvasFrame, 100, 100);

  canvas3 = adjCanvas->GetCanvas();
  adCanvasFrame->AddFrame(adjCanvas, Hint);
  TabPanel->AddFrame(adCanvasFrame, Hint);

}

void Offline::MakeFold4Panel(TGCompositeFrame *TabPanel)
{
  TGCompositeFrame *parFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);

  // draw
  OfflineDrawButton4 = new TGTextButton( parFrame, "&Draw", OFFLINEDRAW4);
  OfflineDrawButton4->SetEnabled(0);
  OfflineDrawButton4->Associate(this);
  parFrame->AddFrame(OfflineDrawButton4, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 30, 0, 0));

  // ch
  offlinechnum4 = new TGNumberEntry (parFrame, 0, 2, OFFLINECHNUM4, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, 15);
  parFrame->AddFrame(offlinechnum4, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 10, 0, 0));
  offlinechnum4->SetButtonToNum(0);
  offlinechnum4->Associate(this);
  TGLabel *ch = new TGLabel( parFrame, "Ch:"); 
  parFrame->AddFrame(ch, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 2, 3, 0));
  

  
  // bin
  chooseth1dbin4 = new TGComboBox(parFrame);
  parFrame->AddFrame(chooseth1dbin4, new TGLayoutHints(kLHintsRight, 5, 25, 2, 2));
  chooseth1dbin4->Resize(80, 20);
  chooseth1dbin4->AddEntry("65536", 1);
  chooseth1dbin4->AddEntry("32768", 2);
  chooseth1dbin4->AddEntry("16384", 3);
  chooseth1dbin4->AddEntry("8192", 4);
  chooseth1dbin4->AddEntry("4096", 5);
  chooseth1dbin4->AddEntry("2048", 6);
  chooseth1dbin4->AddEntry("1024", 7);
  chooseth1dbin4->Select(1);
  TGTextEntry *LabelBinNumber = new TGTextEntry(parFrame,new TGTextBuffer(30));
  LabelBinNumber->SetText("Bin:");
  LabelBinNumber->Resize(30,15);
  LabelBinNumber->SetEnabled(kFALSE);
  LabelBinNumber->SetToolTipText("Choose TH1D bin number.");
  parFrame->AddFrame(LabelBinNumber, new TGLayoutHints(kLHintsRight | kLHintsExpandY, 0, 0, 3, 0));


  TabPanel->AddFrame(parFrame, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 2, 2, 1, 1));


  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  
  TGCompositeFrame *adCanvasFrame = new TGCompositeFrame(TabPanel, 800, 800, kHorizontalFrame);
  TGLayoutHints *Hint = new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 1, 1, 1, 1);

  TRootEmbeddedCanvas *adjCanvas = new TRootEmbeddedCanvas("canvas4", adCanvasFrame, 100, 100);

  canvas4 = adjCanvas->GetCanvas();
  adCanvasFrame->AddFrame(adjCanvas, Hint);
  TabPanel->AddFrame(adCanvasFrame, Hint);

  
}

void Offline::MakeFold5Panel(TGCompositeFrame *TabPanel)
{
  TGCompositeFrame *parFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);

  // text
  printtextinfor5 = new TGTextEntry(parFrame,new TGTextBuffer(30), 10000);
  printtextinfor5-> SetFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1", false);
  fClient->GetColorByName("blue", color);
  printtextinfor5->SetTextColor(color, false);
  printtextinfor5->SetText("Choose 'Ch' then enter button 'Draw'.");
  printtextinfor5->Resize(400, 12);
  printtextinfor5->SetEnabled(kFALSE);
  printtextinfor5->SetFrameDrawn(kFALSE);
  parFrame->AddFrame(printtextinfor5, new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 0, 6, 0));


  // FFProjectY
  showprojectyFF5 = new TGTextButton(parFrame, "&FFProjectY", OFFLINEPROJECTYFF5);
  showprojectyFF5->SetEnabled(0);
  showprojectyFF5->Associate(this);
  parFrame->AddFrame(showprojectyFF5, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 30, 0, 0));

  // CFDProjectY
  showprojectyCFD5 = new TGTextButton(parFrame, "&CFDProjectY", OFFLINEPROJECTYCFD5);
  showprojectyCFD5->SetEnabled(0);
  showprojectyCFD5->Associate(this);
  parFrame->AddFrame(showprojectyCFD5, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 30, 0, 0));


  // OriginalCFD
  originalcfd5 = new TGTextButton(parFrame, "&OriginalCFD", OFFLINEORIGINALCFD5);
  originalcfd5->SetEnabled(0);
  originalcfd5->Associate(this);
  parFrame->AddFrame(originalcfd5, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 30, 0, 0));

  // CalculateCFD
  calculatecfd5 = new TGTextButton(parFrame, "&CalculateCFD", OFFLINECALCULATECFD5);
  calculatecfd5->SetEnabled(0);
  calculatecfd5->Associate(this);
  parFrame->AddFrame(calculatecfd5, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 30, 0, 0));



  
  
  // draw
  OfflineDrawButton5 = new TGTextButton( parFrame, "&Draw", OFFLINEDRAW5);
  OfflineDrawButton5->SetEnabled(0);
  OfflineDrawButton5->Associate(this);
  parFrame->AddFrame(OfflineDrawButton5, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 30, 0, 0));

  // ch
  offlinechnum5 = new TGNumberEntry (parFrame, 0, 2, OFFLINECHNUM5, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, 15);
  parFrame->AddFrame(offlinechnum5, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 10, 0, 0));
  offlinechnum5->SetButtonToNum(0);
  offlinechnum5->Associate(this);
  TGLabel *ch = new TGLabel( parFrame, "Ch:"); 
  parFrame->AddFrame(ch, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 2, 3, 0));
  
  

  TabPanel->AddFrame(parFrame, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 2, 2, 1, 1));

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGCompositeFrame *adCanvasFrame = new TGCompositeFrame(TabPanel, 800, 800, kHorizontalFrame);
  TGLayoutHints *Hint = new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 1, 1, 1, 1);

  TRootEmbeddedCanvas *adjCanvas = new TRootEmbeddedCanvas("canvas5", adCanvasFrame, 100, 100);

  canvas5 = adjCanvas->GetCanvas();
  adCanvasFrame->AddFrame(adjCanvas, Hint);
  TabPanel->AddFrame(adCanvasFrame, Hint);


  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  OriginalCFDcanvas5 = new TCanvas("","",600,400);
  CalculateCFDcanvas5 = new TCanvas("","",600,400);
}

void Offline::MakeFold6Panel(TGCompositeFrame *TabPanel)
{
  TGCompositeFrame *parFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);

  // draw
  OfflineDrawButton6 = new TGTextButton( parFrame, "&Draw", OFFLINEDRAW6);
  OfflineDrawButton6->SetEnabled(0);
  OfflineDrawButton6->Associate(this);
  parFrame->AddFrame(OfflineDrawButton6, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 30, 0, 0));

    // ch
  offlinechnum6 = new TGNumberEntry (parFrame, 0, 2, OFFLINECHNUM6, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, 15);
  parFrame->AddFrame(offlinechnum6, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 10, 0, 0));
  offlinechnum6->SetButtonToNum(0);
  offlinechnum6->Associate(this);
  TGLabel *ch = new TGLabel( parFrame, "Ch:"); 
  parFrame->AddFrame(ch, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 2, 3, 0));
  

  TabPanel->AddFrame(parFrame, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 2, 2, 1, 1));

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGCompositeFrame *adCanvasFrame = new TGCompositeFrame(TabPanel, 800, 800, kHorizontalFrame);
  TGLayoutHints *Hint = new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 1, 1, 1, 1);

  TRootEmbeddedCanvas *adjCanvas = new TRootEmbeddedCanvas("canvas6", adCanvasFrame, 100, 100);

  canvas6 = adjCanvas->GetCanvas();
  adCanvasFrame->AddFrame(adjCanvas, Hint);
  TabPanel->AddFrame(adCanvasFrame, Hint);
}






Bool_t Offline::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
  int test1 = 0, test2 = 0;
  int retval;
  
  switch (test1 = GET_MSG(msg))
    {
    case  kC_HSLIDER:
      switch (test2 = GET_SUBMSG(msg))
	{
	case  kSL_POS:
	  switch(parm1)
	    {
	    case ADJUSTPARSLIDER:
	      offlinemultigraph->GetXaxis()->SetRangeUser(dslider->GetMinPosition(),dslider->GetMaxPosition());
	      
	      // std::cout<<"Slider Pos -  min:"<<dslider->GetMinPosition()<<" max:"<<dslider->GetMaxPosition()<<std::endl;
	      
	      adjustCanvas->Modified();
	      adjustCanvas->Update();
	      break;
	    default:
	      break;
	    }
	  break;

	default:
	  break;
	}
      break;
    case kC_COMMAND:
      switch (test2 = GET_SUBMSG(msg))
	{
	case kCM_BUTTON:
	  switch (parm1)
	    {
	    case OFFLINEREAD:
	      Panel0ReadFile();
	      for (unsigned int i = 0; i < OfflineModuleEventsCount; ++i)//将默认的ch横坐标范围与sample对应
		{
		  if(offlinechnum->GetIntNumber() == OfflineEventInformation[12*i+1])
		    {
		      dslider->SetRange(0,OfflineEventInformation[12*i+10]);
		      dslider->SetPosition(0,OfflineEventInformation[12*i+10]);
		      break;
		    }
		}
	      break;
	    case OFFLINELOAD:
	      OfflineLoadValues(offlinemodnum->GetIntNumber(),offlinechnum->GetIntNumber());
	      break;
	    case OFFLINEAPPLY:
	      OfflineChangeValues(offlinemodnum->GetIntNumber(),offlinechnum->GetIntNumber());
	      break;
	    case OFFLINEDRAW:
	      Panel1Draw();
	      break;
	    case OFFLINEDRAW2:
	      Panel2Draw();
	      break;
	    case OFFLINEDRAW3:
	      Panel3Draw();
	      break;
	    case OFFLINEDRAW4:
	      Panel4Draw();
	      break;
	    case OFFLINEDRAW5:
	      Panel5Draw();
	      break;
	    case OFFLINEDRAW6:
	      Panel6Draw();
	      break;

	      
	    case OFFLINEPROJECTYFF5:
	      FFShowProjectY5();
	      break;	      
	    case OFFLINEPROJECTYCFD5:
	      CFDShowProjectY5();
	      break;	  
	    case OFFLINEORIGINALCFD5:
	      OriginalCFDShow5();
	      break;	  
	    case OFFLINECALCULATECFD5:
	      CalculateCFDShow5();
	      break;	
	      
	    case OFFLINEFILTERRANGE:
	      if (parm2 == 0)
	      	{
	      	  if (OfflinefRange != 6)
	      	    {
	      	      ++OfflinefRange;
	      	      offlinefilterrange->SetIntNumber(OfflinefRange);
		      OfflineDrawButton->SetEnabled(0);
		      gSystem->ProcessEvents();
	      	      retval = Pixie16WriteSglModPar((char*)"SLOW_FILTER_RANGE", OfflinefRange, offlinemodnum->GetIntNumber());
		      if(retval < 0) ErrorInfo("Offline.cc", "ProcessMessage(...)", "Pixie16WriteSglModPar/SLOW_FILTER_RANGE", retval);
		      OfflineDrawButton->SetEnabled(1);
	      	    }
	      	}
	      else
	      	{
	      	  if (OfflinefRange != 1)
	      	    {
	      	      if (--OfflinefRange == 1)
	      		OfflinefRange = 1;
	      	      offlinefilterrange->SetIntNumber(OfflinefRange);
		      OfflineDrawButton->SetEnabled(0);
		      gSystem->ProcessEvents();
	      	      retval = Pixie16WriteSglModPar((char*)"SLOW_FILTER_RANGE", OfflinefRange, offlinemodnum->GetIntNumber());
		      if(retval < 0) ErrorInfo("Offline.cc", "ProcessMessage(...)", "Pixie16WriteSglModPar/SLOW_FILTER_RANGE", retval);
		      OfflineDrawButton->SetEnabled(1);
	      	    }
	      	}
	      break;  
	    case OFFLINERUNNUM:
	      OfflineDrawButton->SetEnabled(0);
	      OfflineDrawButton2->SetEnabled(0);
	      OfflineDrawButton3->SetEnabled(0);
	      OfflineDrawButton4->SetEnabled(0);
	      OfflineDrawButton5->SetEnabled(0);
	      OfflineDrawButton6->SetEnabled(0);
	      if (parm2 == 0)
		{
		  ++fileRunNum;
		  offlinefilerunnum->SetIntNumber(fileRunNum);
		}
	      else
		{
		  if(fileRunNum != 0)
		    {
		      if(--fileRunNum == 0)
			fileRunNum =0;
		      offlinefilerunnum->SetIntNumber(fileRunNum);
		    }
		}
	      break;
	    case OFFLINEMODNUM:
	      OfflineDrawButton->SetEnabled(0);
	      OfflineDrawButton2->SetEnabled(0);
	      OfflineDrawButton3->SetEnabled(0);
	      OfflineDrawButton4->SetEnabled(0);
	      OfflineDrawButton5->SetEnabled(0);
	      OfflineDrawButton6->SetEnabled(0);
	      if (parm2 == 0)
		{
		  if (modNumber != detector->NumModules-1)
		    {
		      ++modNumber;
		      offlinemodnum->SetIntNumber(modNumber);
		    }
		}
	      else
		{
		  if (modNumber != 0)
		    {
		      if (--modNumber == 0)
			modNumber = 0;
		      offlinemodnum->SetIntNumber(modNumber);
		    }
		}
	      break;
	    case OFFLINECHNUM:
	      if (parm2 == 0)
		{
		  if (chanNumber != 15)
		    {
		      ++chanNumber;
		      offlinechnum->SetIntNumber(chanNumber);
		    }
		}
	      else
		{
		  if (chanNumber != 0)
		    {
		      if (--chanNumber == 0)
			chanNumber = 0;
		      offlinechnum->SetIntNumber(chanNumber);
		    }
		}
	      for (unsigned int i = 0; i < OfflineModuleEventsCount; ++i)//每次改变channel，横坐标范围都回到全部sample
		{
		  if(offlinechnum->GetIntNumber() == OfflineEventInformation[12*i+1])
		    {
		      dslider->SetRange(0,OfflineEventInformation[12*i+10]);
		      dslider->SetPosition(0,OfflineEventInformation[12*i+10]);
		      break;
		    }
		}
	      break;
	    case OFFLINECHNUM4:
	      if (parm2 == 0)
		{
		  if (chanNumber4 != 15)
		    {
		      ++chanNumber4;
		      offlinechnum4->SetIntNumber(chanNumber4);
		    }
		}
	      else
		{
		  if (chanNumber4 != 0)
		    {
		      if (--chanNumber4 == 0)
			chanNumber4 = 0;
		      offlinechnum4->SetIntNumber(chanNumber4);
		    }
		}
	      break;
	    case OFFLINECHNUM5:
	      if (parm2 == 0)
		{
		  if (chanNumber5 != 15)
		    {
		      ++chanNumber5;
		      offlinechnum5->SetIntNumber(chanNumber5);
		    }
		}
	      else
		{
		  if (chanNumber5 != 0)
		    {
		      if (--chanNumber5 == 0)
			chanNumber5 = 0;
		      offlinechnum5->SetIntNumber(chanNumber5);
		    }
		}
	      break;
	    case OFFLINECHNUM6:
	      if (parm2 == 0)
		{
		  if (chanNumber6 != 15)
		    {
		      ++chanNumber6;
		      offlinechnum6->SetIntNumber(chanNumber6);
		    }
		}
	      else
		{
		  if (chanNumber6 != 0)
		    {
		      if (--chanNumber6 == 0)
			chanNumber6 = 0;
		      offlinechnum6->SetIntNumber(chanNumber6);
		    }
		}
	      break;
	      
	      
	      
	    default:
	      break;

	    }
	  break;
	}
      break;
    case kC_TEXTENTRY:
      switch (parm1)
	{
	case OFFLINEMODNUM:
	  switch (GET_SUBMSG(msg))
	    {
	    case kTE_ENTER:
	      OfflineDrawButton->SetEnabled(0);
	      OfflineDrawButton2->SetEnabled(0);
	      OfflineDrawButton3->SetEnabled(0);
	      OfflineDrawButton4->SetEnabled(0);
	      OfflineDrawButton5->SetEnabled(0);
	      OfflineDrawButton6->SetEnabled(0);
	      modNumber = offlinemodnum->GetIntNumber();
	      if(modNumber > detector->NumModules-1) modNumber = detector->NumModules-1;
	      if(modNumber < 0) modNumber = 0;
	      offlinemodnum->SetIntNumber(modNumber);
	      break;
	    default:
	      break;
	    }
	  break;
	case OFFLINECHNUM:
	  switch (GET_SUBMSG(msg))
	    {
	    case kTE_ENTER:
	      chanNumber = offlinechnum->GetIntNumber();
	      if(chanNumber > 15) chanNumber = 15;
	      if(chanNumber < 0) chanNumber = 0;
	      offlinechnum->SetIntNumber(chanNumber);
	      break;
	    default:
	      break;
	    }
	  break;
	case OFFLINECHNUM4:
	  switch (GET_SUBMSG(msg))
	    {
	    case kTE_ENTER:
	      chanNumber4 = offlinechnum4->GetIntNumber();
	      if(chanNumber4 > 15) chanNumber4 = 15;
	      if(chanNumber4 < 0) chanNumber4 = 0;
	      offlinechnum4->SetIntNumber(chanNumber4);
	      break;
	    default:
	      break;
	    }
	  break;
	case OFFLINECHNUM5:
	  switch (GET_SUBMSG(msg))
	    {
	    case kTE_ENTER:
	      chanNumber5 = offlinechnum5->GetIntNumber();
	      if(chanNumber5 > 15) chanNumber5 = 15;
	      if(chanNumber5 < 0) chanNumber5 = 0;
	      offlinechnum5->SetIntNumber(chanNumber5);
	      break;
	    default:
	      break;
	    }
	  break;
	case OFFLINECHNUM6:
	  switch (GET_SUBMSG(msg))
	    {
	    case kTE_ENTER:
	      chanNumber6 = offlinechnum4->GetIntNumber();
	      if(chanNumber6 > 15) chanNumber6 = 15;
	      if(chanNumber6 < 0) chanNumber6 = 0;
	      offlinechnum6->SetIntNumber(chanNumber6);
	      break;
	    default:
	      break;
	    }
	  break;

	  
	case OFFLINEFILTERRANGE:
	  switch (GET_SUBMSG(msg))
	    {
	    case kTE_ENTER:
	      OfflineDrawButton->SetEnabled(0);
	      gSystem->ProcessEvents();
	      OfflinefRange = offlinefilterrange->GetIntNumber();
	      if(OfflinefRange > 6) OfflinefRange = 6;
	      if(OfflinefRange < 1) OfflinefRange = 1;
	      offlinefilterrange->SetIntNumber(OfflinefRange);
	      retval = Pixie16WriteSglModPar((char*)"SLOW_FILTER_RANGE", OfflinefRange, offlinemodnum->GetIntNumber());
	      if(retval < 0) ErrorInfo("Offline.cc", "ProcessMessage(...)", "Pixie16WriteSglModPar/SLOW_FILTER_RANGE", retval);
	      OfflineDrawButton->SetEnabled(1);
	      break;
	    default:
	      break;
	    }
	  break;
	case OFFLINERUNNUM:
	  switch (GET_SUBMSG(msg))
	    {
	    case kTE_ENTER:
	      OfflineDrawButton->SetEnabled(0);
	      OfflineDrawButton2->SetEnabled(0);
	      OfflineDrawButton3->SetEnabled(0);
	      OfflineDrawButton4->SetEnabled(0);
	      OfflineDrawButton5->SetEnabled(0);
	      OfflineDrawButton6->SetEnabled(0);
	      fileRunNum = offlinefilerunnum->GetIntNumber();
	      if(fileRunNum < 0) fileRunNum = 0;
	      offlinefilerunnum->SetIntNumber(fileRunNum);
	      break;
	      
	    default:
	      break;
	    }
	  break;
	  
	default:
	  break;
	}
      break;
      
      /////////////////default for the most inclusive switch//////////          
    default:
      selected = gPad->GetSelected();
      break;
    }

  return kTRUE;
}

void Offline::OfflineChangeValues(int mod,int ch)
{
  int retval;
  
  double flength;
  double fgap;
  double slength;
  double sgap;
  double tau;
  double delay;
  double frac;
  double thresh;
  double thres;
  flength = offlinefilters[0]->GetNumber();
  retval = Pixie16WriteSglChanPar((char*)"TRIGGER_RISETIME", flength, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cc", "OfflineChangeValues(...)", "Pixie16WriteSglChanPar/TRIGGER_RISETIME", retval);
  fgap = offlinefilters[1]->GetNumber();
  retval = Pixie16WriteSglChanPar((char*)"TRIGGER_FLATTOP", fgap, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cc", "OfflineChangeValues(...)", "Pixie16WriteSglChanPar/TRIGGER_FLATTOP", retval);
  slength = offlinefilters[2]->GetNumber();
  retval = Pixie16WriteSglChanPar((char*)"ENERGY_RISETIME", slength, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cc", "OfflineChangeValues(...)", "Pixie16WriteSglChanPar/ENERGY_RISETIME", retval);
  sgap = offlinefilters[3]->GetNumber();
  retval = Pixie16WriteSglChanPar((char*)"ENERGY_FLATTOP", sgap, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cc", "OfflineChangeValues(...)", "Pixie16WriteSglChanPar/ENERGY_FLATTOP", retval);
  tau = offlinefilters[4]->GetNumber();
  retval = Pixie16WriteSglChanPar((char*)"TAU", tau, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cc", "OfflineChangeValues(...)", "Pixie16WriteSglChanPar/TAU", retval);
  delay = offlinefilters[5]->GetNumber();
  retval = Pixie16WriteSglChanPar((char*)"CFDDelay", delay, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cc", "OfflineChangeValues(...)", "Pixie16WriteSglChanPar/CFDDelay", retval);
  frac = offlinefilters[6]->GetNumber();
  retval = Pixie16WriteSglChanPar((char*)"CFDScale", frac, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cc", "OfflineChangeValues(...)", "Pixie16WriteSglChanPar/CFDScale", retval);
  thresh = offlinefilters[7]->GetNumber();
  retval = Pixie16WriteSglChanPar((char*)"TRIGGER_THRESHOLD", thresh, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cc", "OfflineChangeValues(...)", "Pixie16WriteSglChanPar/TRIGGER_THRESHOLD", retval);
  thres = offlinefilters[8]->GetNumber();
  retval = Pixie16WriteSglChanPar((char*)"CFDThresh", thres, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cc", "OfflineChangeValues(...)", "Pixie16WriteSglChanPar/CFDThresh", retval);
  
}

void Offline::OfflineLoadValues(int mod,int ch)
{
  double ChanParData = -1;
  int retval; 
  char text[20];
  
  retval = Pixie16ReadSglChanPar((char*)"TRIGGER_RISETIME", &ChanParData, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cc", "OfflineLoadValues(...)", "Pixie16ReadSglChanPar/TRIGGER_RISETIME", retval);
  sprintf(text, "%1.2f", ChanParData);
  offlinefilters[0]->SetText(text);

  retval = Pixie16ReadSglChanPar((char*)"TRIGGER_FLATTOP", &ChanParData, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cc", "OfflineLoadValues(...)", "Pixie16ReadSglChanPar/TRIGGER_FLATTOP", retval);  
  sprintf(text, "%1.2f", ChanParData);
  offlinefilters[1]->SetText(text);

  retval = Pixie16ReadSglChanPar((char*)"ENERGY_RISETIME", &ChanParData, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cc", "OfflineLoadValues(...)", "Pixie16ReadSglChanPar/ENERGY_RISETIME", retval);
  sprintf(text, "%1.2f", ChanParData);
  offlinefilters[2]->SetText(text);

  retval = Pixie16ReadSglChanPar((char*)"ENERGY_FLATTOP", &ChanParData, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cc", "OfflineLoadValues(...)", "Pixie16ReadSglChanPar/ENERGY_FLATTOP", retval);  
  sprintf(text, "%1.2f", ChanParData);
  offlinefilters[3]->SetText(text);

  retval = Pixie16ReadSglChanPar((char*)"TAU", &ChanParData, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cc", "OfflineLoadValues(...)", "Pixie16ReadSglChanPar/TAU", retval);
  sprintf(text, "%1.2f", ChanParData);
  offlinefilters[4]->SetText(text);

  retval = Pixie16ReadSglChanPar((char*)"CFDDelay", &ChanParData, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cc", "OfflineLoadValues(...)", "Pixie16ReadSglChanPar/CFDDelay", retval);
  sprintf(text, "%1.2f", ChanParData);
  offlinefilters[5]->SetText(text);

  retval = Pixie16ReadSglChanPar((char*)"CFDScale", &ChanParData, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cc", "OfflineLoadValues(...)", "Pixie16ReadSglChanPar/CFDScale", retval); 
  sprintf(text, "%1.2f", ChanParData);
  offlinefilters[6]->SetText(text);

  retval = Pixie16ReadSglChanPar((char*)"TRIGGER_THRESHOLD", &ChanParData, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cc", "OfflineLoadValues(...)", "Pixie16ReadSglChanPar/TRIGGER_THRESHOLD", retval);
  sprintf (text, "%d", (int)ChanParData);
  offlinefilters[7]->SetText(text);

  retval = Pixie16ReadSglChanPar((char*)"CFDThresh", &ChanParData, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cc", "OfflineLoadValues(...)", "Pixie16ReadSglChanPar/CFDThresh", retval);     
  sprintf(text, "%d", (int)ChanParData);
  offlinefilters[8]->SetText(text);


  
  retval = Pixie16ReadSglModPar((char*)"SLOW_FILTER_RANGE", &OfflinefRange, mod);
  if(retval < 0) ErrorInfo("Offline.cc", "OfflineLoadValues(...)", "Pixie16ReadSglModPar/SLOW_FILTER_RANGE", retval);
  offlinefilterrange->SetIntNumber(OfflinefRange);
}


unsigned int Offline::GetModuleEvents(char *FileName)
{
  int ModuleEvents = 0;
  unsigned int eventdata, eventlength;
  long TotalWords, TotalSkippedWords;
  FILE *ListModeFile = NULL;

  // Open the list mode file
  ListModeFile = fopen(FileName, "rb");
  if(ListModeFile != NULL)
    {	
      // Get file length
      fseek(ListModeFile, 0, SEEK_END);
      TotalWords = (ftell(ListModeFile) + 1) / 4;
      
      // Point ListModeFile to the beginning of file
      fseek(ListModeFile, 0, SEEK_SET);
		
      // Initialize indicator and counter
      TotalSkippedWords = 0;
		
      do
	{
	  fread(&eventdata, 4, 1, ListModeFile);
	  eventlength = (eventdata & 0x7FFE0000) >> 17;
	  TotalSkippedWords += eventlength;
	  fseek(ListModeFile, (eventlength-1)*4, SEEK_CUR);
	  ModuleEvents ++;
	}while( TotalSkippedWords < TotalWords );
		
      fclose(ListModeFile);
    }
  
  if(TotalWords == 0) ModuleEvents = 0;
  return ModuleEvents;
  
}

void Offline::GetEventsInfo(char *FileName, unsigned int *EventInformation)
{
  unsigned int eventdata, headerlength, eventlength;
  unsigned int  NumEvents;
  long TotalWords, TotalSkippedWords;
  FILE *ListModeFile = NULL;

  // Open the list mode file
  ListModeFile = fopen(FileName, "rb");
  if(ListModeFile != NULL)
    {	
      // Get file length
      fseek(ListModeFile, 0, SEEK_END);
      TotalWords = (ftell(ListModeFile) + 1) / 4;
      // Point ListModeFile to the beginning of file
      fseek(ListModeFile, 0, SEEK_SET);
		
      // Initialize indicator and counter
      TotalSkippedWords = 0;
      NumEvents = 0;
		
      do
	{
	  fread(&eventdata, 4, 1, ListModeFile);		
	  // Event #
	  EventInformation[12 * NumEvents]     = NumEvents;
	  // Channel #
	  EventInformation[12 * NumEvents + 1] = (eventdata & 0xF);	
	  // Slot #
	  EventInformation[12 * NumEvents + 2] = (eventdata & 0xF0) >> 4;	
	  // Crate #
	  EventInformation[12 * NumEvents + 3] = (eventdata & 0xF00) >> 8;	
	  // Header length
	  headerlength = (eventdata & 0x1F000) >> 12;
	  EventInformation[12 * NumEvents + 4] = (eventdata & 0x1F000) >> 12;	
	  // Event length
	  eventlength = (eventdata & 0x7FFE0000) >> 17;
	  EventInformation[12 * NumEvents + 5] = (eventdata & 0x7FFE0000) >> 17;	
	  // Finish code
	  EventInformation[12 * NumEvents + 6] = (eventdata & 0x80000000) >> 31;	
	  fread(&eventdata, 4, 1, ListModeFile);
	  // EventTime_Low
	  EventInformation[12 * NumEvents + 7] = eventdata;	
	  fread(&eventdata, 4, 1, ListModeFile);
	  // EventTime_High
	  EventInformation[12 * NumEvents + 8] = (eventdata & 0xFFFF);
	  fread(&eventdata, 4, 1, ListModeFile);
	  // Event Energy
	  EventInformation[12 * NumEvents + 9] = (eventdata & 0xFFFF);
	  // Trace Length
	  EventInformation[12 * NumEvents + 10] = (eventdata & 0x7FFF0000) >> 16;
	  // Trace location
	  EventInformation[12 * NumEvents + 11] = TotalSkippedWords + headerlength;

	  // if(eventlength != headerlength + EventInformation[12 * NumEvents + 10]/2)
	  //   std::cout<<"Data error..."<<std::endl;
	  
	  TotalSkippedWords += eventlength;
	  NumEvents ++;
	  fseek(ListModeFile, (eventlength - 4) * 4, SEEK_CUR);
	}while( TotalSkippedWords < TotalWords );
		
      fclose(ListModeFile);
    }
}

void Offline::Panel1Draw()
{
  if(rawdata != NULL)
    {
      delete rawdata;
      rawdata = NULL;
    }
  if(threshdata != NULL)
    {
      delete threshdata;
      threshdata = NULL;
    }
  if(cfddata != NULL)
    {
      delete cfddata;
      cfddata = NULL;
    }
  if(cfdthreshdata != NULL)
    {
      delete cfdthreshdata;
      cfdthreshdata = NULL;
    }
  if(sfilterdata != NULL)
    {
      delete sfilterdata;
      sfilterdata  = NULL;
    }
  if(ffilterdata != NULL)
    {
      delete ffilterdata;
      ffilterdata = NULL;
    }
  if(RcdTrace != NULL)
    {
      delete []RcdTrace;
      RcdTrace = NULL;
    }
  if(doublethresh != NULL)
    {
      delete []doublethresh;
      doublethresh = NULL;
    }
  if(doublecfdthresh != NULL)
    {
      delete []doublecfdthresh;
      doublecfdthresh = NULL;
    }
  if(doublesample != NULL)
    {
      delete []doublesample;
      doublesample = NULL;
    }	      
  if(doublercdtrace != NULL)
    {
      delete []doublercdtrace;
      doublercdtrace = NULL;
    }
  if(doublefastfilter != NULL)
    {
      delete []doublefastfilter;
      doublefastfilter = NULL;
    }
  if(doublecfd != NULL)
    {
      delete []doublecfd;
      doublecfd = NULL;
    }
  if(doubleslowfilter != NULL)
    {
      delete []doubleslowfilter;
      doubleslowfilter = NULL;
    }
  bool offlinedatastatus;
  unsigned int tempN1;
  tempN1 = -1;
  offlinedatastatus = false;

  for(;;)
    {
      OfflineCurrentCount++;
      tempN1++;
      if(tempN1 > OfflineModuleEventsCount)
	{
	  offlinedatastatus = true;
	  break;
	}
      if(OfflineCurrentCount == OfflineModuleEventsCount) OfflineCurrentCount = 0;

      if(offlinechnum->GetIntNumber() == OfflineEventInformation[12*OfflineCurrentCount+1]) break;
    }
  // do
  // 	{
  // 	  OfflineCurrentCount++;
  // 	  if(OfflineCurrentCount == OfflineModuleEventsCount) OfflineCurrentCount = 0;
		  
  // 	}while(offlinechnum->GetIntNumber() != OfflineEventInformation[12*OfflineCurrentCount+1]);
  cout<<"N: "<<OfflineCurrentCount<<endl;
	      
  if(offlinedatastatus)
    {
      adjustCanvas->cd();
      adjustCanvas->Clear();
      adjustCanvas->Modified();
      adjustCanvas->Update();
      gSystem->ProcessEvents();
      fClient->GetColorByName("red", color);
      OfflineCurrentCountText->SetTextColor(color, false);
      char staok[20];
      sprintf(staok,"Ch%dNotData",(int)offlinechnum->GetIntNumber());
      OfflineCurrentCountText->SetText(staok);
      // OfflineDrawButton->SetEnabled(0);
      return;
    }
	      
  fClient->GetColorByName("blue", color);
  OfflineCurrentCountText->SetTextColor(color, false);
  char stacurr[128];
  sprintf(stacurr,"%d / %d",OfflineCurrentCount+1,OfflineModuleEventsCount);
  OfflineCurrentCountText->SetText(stacurr);
	      
  RcdTrace = new unsigned short[OfflineEventInformation[12*OfflineCurrentCount+10]];
  doublethresh = new double[OfflineEventInformation[12*OfflineCurrentCount+10]];
  doublecfdthresh = new double[OfflineEventInformation[12*OfflineCurrentCount+10]];
  doublesample = new double[OfflineEventInformation[12*OfflineCurrentCount+10]];
  doublercdtrace = new double[OfflineEventInformation[12*OfflineCurrentCount+10]];
  doublefastfilter = new double[OfflineEventInformation[12*OfflineCurrentCount+10]];
  doublecfd = new double[OfflineEventInformation[12*OfflineCurrentCount+10]];
  doubleslowfilter = new double[OfflineEventInformation[12*OfflineCurrentCount+10]];

  int retval;
  retval = Pixie16ComputeFastFiltersOffline(offlinefilename, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short) offlinechnum->GetIntNumber(), OfflineEventInformation[12*OfflineCurrentCount+11], OfflineEventInformation[12*OfflineCurrentCount+10], RcdTrace, doublefastfilter, doublecfd );
  if(retval < 0) ErrorInfo("Offline.cc", "Panel1Draw()", "Pixie16ComputeFastFiltersOffline", retval);
  retval = Pixie16ComputeSlowFiltersOffline(offlinefilename, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short) offlinechnum->GetIntNumber(), OfflineEventInformation[12*OfflineCurrentCount+11], OfflineEventInformation[12*OfflineCurrentCount+10], RcdTrace,doubleslowfilter );
  if(retval < 0) ErrorInfo("Offline.cc", "Panel1Draw()", "Pixie16ComputeSlowFiltersOffline", retval);
  
  double ChanParData;
  retval = Pixie16ReadSglChanPar((char*)"TRIGGER_THRESHOLD", &ChanParData, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short) offlinechnum->GetIntNumber());
  if(retval < 0) ErrorInfo("Offline.cc", "Panel1Draw()", "Pixie16ReadSglChanPar/TRIGGER_THRESHOLD", retval);
  doublethresh[0] = ChanParData;
  retval = Pixie16ReadSglChanPar((char*)"CFDThresh", &ChanParData, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short) offlinechnum->GetIntNumber());
  if(retval < 0) ErrorInfo("Offline.cc", "Panel1Draw()", "Pixie16ReadSglChanPar/CFDThresh", retval);
  doublecfdthresh[0] = ChanParData;
  
  for (unsigned int i = 0; i < OfflineEventInformation[12*OfflineCurrentCount+10]; ++i)
    {
      doublesample[i] = i;
      doublercdtrace[i] = (double)RcdTrace[i];
      doublethresh[i] = doublethresh[0];
      doublecfdthresh[i] = doublecfdthresh[0];
    }

  rawdata = new TGraph(OfflineEventInformation[12*OfflineCurrentCount+10],doublesample,doublercdtrace);
  threshdata = new TGraph(OfflineEventInformation[12*OfflineCurrentCount+10],doublesample,doublethresh);
  cfddata = new TGraph(OfflineEventInformation[12*OfflineCurrentCount+10],doublesample,doublecfd);
  cfdthreshdata = new TGraph(OfflineEventInformation[12*OfflineCurrentCount+10],doublesample,doublecfdthresh);
  sfilterdata = new TGraph(OfflineEventInformation[12*OfflineCurrentCount+10],doublesample,doubleslowfilter);
  ffilterdata  = new TGraph(OfflineEventInformation[12*OfflineCurrentCount+10],doublesample,doublefastfilter);

  adjustCanvas->cd();
  adjustCanvas->Clear();
  cfddata->SetLineColor(2);
  cfdthreshdata->SetLineColor(2);
  sfilterdata->SetLineColor(3);
  ffilterdata->SetLineColor(4);
  threshdata->SetLineColor(4);
  // rawdata->SetLineWidth(3);
  // cfddata->SetLineWidth(3);
  // sfilterdata->SetLineWidth(3);
  // cfddata->SetLineWidth(3);
  offlinemultigraph->Clear();
  offlinemultigraph->Add(rawdata);
  offlinemultigraph->Add(threshdata);
  offlinemultigraph->Add(cfddata);
  offlinemultigraph->Add(cfdthreshdata);
  offlinemultigraph->Add(sfilterdata);
  offlinemultigraph->Add(ffilterdata);
  offlinemultigraph->Draw("AL");
  offlinemultigraph->GetXaxis()->SetRangeUser(double(dslider->GetMinPosition()),double(dslider->GetMaxPosition()));
  // offlinemultigraph->Draw("AL");
  adjustCanvas->Modified();
  adjustCanvas->Update();

  gSystem->ProcessEvents();
}

void Offline::Panel2Draw()
{
  for (int i = 0; i < 16; ++i)
    {
      if(rawdata2[i] != NULL)
	{
	  delete rawdata2[i];
	  rawdata2[i] = NULL;
	}
      if(threshdata2[i] != NULL)
	{
	  delete threshdata2[i];
	  threshdata2[i] = NULL;
	}
      if(cfdthreshdata2[i] != NULL)
	{
	  delete cfdthreshdata2[i];
	  cfdthreshdata2[i] = NULL;
	}
      if(cfddata2[i] != NULL)
	{
	  delete cfddata2[i];
	  cfddata2[i] = NULL;
	}
      if(sfilterdata2[i] != NULL)
	{
	  delete sfilterdata2[i];
	  sfilterdata2[i]  = NULL;
	}
      if(ffilterdata2[i] != NULL)
	{
	  delete ffilterdata2[i];
	  ffilterdata2[i] = NULL;
	}
      if(RcdTrace2[i] != NULL)
	{
	  delete []RcdTrace2[i];
	  RcdTrace2[i] = NULL;
	}
      if(doublethresh2[i] != NULL)
	{
	  delete []doublethresh2[i];
	  doublethresh2[i] = NULL;
	}
      if(doublesample2[i] != NULL)
	{
	  delete []doublesample2[i];
	  doublesample2[i] = NULL;
	}	      
      if(doublercdtrace2[i] != NULL)
	{
	  delete []doublercdtrace2[i];
	  doublercdtrace2[i] = NULL;
	}
      if(doublefastfilter2[i] != NULL)
	{
	  delete []doublefastfilter2[i];
	  doublefastfilter2[i] = NULL;
	}
      if(doublecfd2[i] != NULL)
	{
	  delete []doublecfd2[i];
	  doublecfd2[i] = NULL;
	}
      if(doubleslowfilter2[i] != NULL)
	{
	  delete []doubleslowfilter2[i];
	  doubleslowfilter2[i] = NULL;
	}
    }

  bool offlinedatastatus2[16];
  unsigned int tempN12[16];
  
  canvas2->cd();
  canvas2->Clear();
  canvas2->Divide(4,4);
  
  for (unsigned int i = 0; i < 16; ++i)
    {
      tempN12[i] = -1;
      offlinedatastatus2[i] = false;

      for(;;)
	{
	  OfflineCurrentCount2[i]++;
	  tempN12[i]++;
	  if(tempN12[i] > OfflineModuleEventsCount-1)
	    {
	      offlinedatastatus2[i] = true;
	      break;
	    }
	  if(OfflineCurrentCount2[i] >= OfflineModuleEventsCount) OfflineCurrentCount2[i] = 0;

	  if(i == OfflineEventInformation[12*OfflineCurrentCount2[i]+1]) break;
	}

      // cout<<"Ch: "<<i<<"  N: "<<OfflineCurrentCount2[i]<<endl;
      int retval;
      if(!offlinedatastatus2[i])
	{
	  RcdTrace2[i] = new unsigned short[OfflineEventInformation[12*OfflineCurrentCount2[i]+10]];
	  doublethresh2[i] = new double[OfflineEventInformation[12*OfflineCurrentCount2[i]+10]];
	  doublecfdthresh2[i] = new double[OfflineEventInformation[12*OfflineCurrentCount2[i]+10]];
	  doublesample2[i] = new double[OfflineEventInformation[12*OfflineCurrentCount2[i]+10]];
	  doublercdtrace2[i] = new double[OfflineEventInformation[12*OfflineCurrentCount2[i]+10]];
	  doublefastfilter2[i] = new double[OfflineEventInformation[12*OfflineCurrentCount2[i]+10]];
	  doublecfd2[i] = new double[OfflineEventInformation[12*OfflineCurrentCount2[i]+10]];
	  doubleslowfilter2[i] = new double[OfflineEventInformation[12*OfflineCurrentCount2[i]+10]];

	  retval = Pixie16ComputeFastFiltersOffline(offlinefilename, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short) i, OfflineEventInformation[12*OfflineCurrentCount2[i]+11], OfflineEventInformation[12*OfflineCurrentCount2[i]+10], RcdTrace2[i], doublefastfilter2[i], doublecfd2[i] );
	  if(retval < 0) ErrorInfo("Offline.cc", "Panel2Draw()", "Pixie16ComputeFastFiltersOffline", retval);
	  retval = Pixie16ComputeSlowFiltersOffline(offlinefilename, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short) i, OfflineEventInformation[12*OfflineCurrentCount2[i]+11], OfflineEventInformation[12*OfflineCurrentCount2[i]+10], RcdTrace2[i],doubleslowfilter2[i] );
	  if(retval < 0) ErrorInfo("Offline.cc", "Panel2Draw()", "Pixie16ComputeSlowFiltersOffline", retval);
	  
	  double ChanParData;
	  retval = Pixie16ReadSglChanPar((char*)"TRIGGER_THRESHOLD", &ChanParData, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short)i);
	  if(retval < 0) ErrorInfo("Offline.cc", "Panel2Draw()", "Pixie16ReadSglChanPar/TRIGGER_THRESHOLD", retval);
	  doublethresh2[i][0] = ChanParData;
	  retval = Pixie16ReadSglChanPar((char*)"CFDThresh", &ChanParData, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short)i);
	  if(retval < 0) ErrorInfo("Offline.cc", "Panel2Draw()", "Pixie16ReadSglChanPar/CFDThresh", retval);
	  doublecfdthresh2[i][0] = ChanParData;

	  
	  for (unsigned int j = 0; j < OfflineEventInformation[12*OfflineCurrentCount2[i]+10]; ++j)
	    {
	      doublesample2[i][j] = j;
	      doublercdtrace2[i][j] = (double)RcdTrace2[i][j];
	      doublethresh2[i][j] = doublethresh2[i][0];
	      doublecfdthresh2[i][j] = doublecfdthresh2[i][0];
	    }

	  rawdata2[i] = new TGraph(OfflineEventInformation[12*OfflineCurrentCount2[i]+10],doublesample2[i],doublercdtrace2[i]);
	  threshdata2[i] = new TGraph(OfflineEventInformation[12*OfflineCurrentCount2[i]+10],doublesample2[i],doublethresh2[i]);
	  cfdthreshdata2[i] = new TGraph(OfflineEventInformation[12*OfflineCurrentCount2[i]+10],doublesample2[i],doublecfdthresh2[i]);
	  cfddata2[i] = new TGraph(OfflineEventInformation[12*OfflineCurrentCount2[i]+10],doublesample2[i],doublecfd2[i]);
	  sfilterdata2[i] = new TGraph(OfflineEventInformation[12*OfflineCurrentCount2[i]+10],doublesample2[i],doubleslowfilter2[i]);
	  ffilterdata2[i] = new TGraph(OfflineEventInformation[12*OfflineCurrentCount2[i]+10],doublesample2[i],doublefastfilter2[i]);

	  
	  cfddata2[i]->SetLineColor(2);
	  cfdthreshdata2[i]->SetLineColor(2);
	  sfilterdata2[i]->SetLineColor(3);
	  ffilterdata2[i]->SetLineColor(4);
	  threshdata2[i]->SetLineColor(4);
	  offlinemultigraph2[i]->Clear();
	  offlinemultigraph2[i]->Add(rawdata2[i]);
	  offlinemultigraph2[i]->Add(threshdata2[i]);
	  offlinemultigraph2[i]->Add(cfdthreshdata2[i]);
	  offlinemultigraph2[i]->Add(cfddata2[i]);
	  offlinemultigraph2[i]->Add(sfilterdata2[i]);
	  offlinemultigraph2[i]->Add(ffilterdata2[i]);
	  canvas2->cd(1+i);
	  offlinemultigraph2[i]->SetTitle(TString::Format("Event: %d",OfflineCurrentCount2[i]).Data());
	  std::cout<<TString::Format("%d",OfflineCurrentCount2[i]).Data()<<std::endl;
	  offlinemultigraph2[i]->Draw("AL");
	}
    }

  canvas2->Modified();
  canvas2->Update();
  gSystem->ProcessEvents();
}


void Offline::Panel3Draw()
{
  canvas3->cd();
  canvas3->Clear();
  canvas3->Divide(4,4);

  for (int i = 0; i < 16; ++i)
    {
      if(offlineth1d3[i] != NULL)
	{
	  delete offlineth1d3[i];
	  offlineth1d3[i] = NULL;
	}
      char th1dname[16];
      sprintf(th1dname,"ch%02d",i);
      offlineth1d3[i] = new TH1D(th1dname,"",8192,0,65536);
    }
  
  for (unsigned int i = 0; i < OfflineModuleEventsCount; ++i)
    {
      offlineth1d3[OfflineEventInformation[12*i+1]]->Fill(OfflineEventInformation[12*i+9]);
    }

  for (int i = 0; i < 16; ++i)
    {
      if(offlineth1d3[i]->GetEntries() > 0)
	{
	  canvas3->cd(i+1);
	  offlineth1d3[i]->Draw();
	  offlineth1d3[i]->GetXaxis()->SetTitle("ch");
	  offlineth1d3[i]->GetXaxis()->SetLabelSize(0.06);
	  offlineth1d3[i]->GetYaxis()->SetLabelSize(0.06);
	}
    }
    
  canvas3->Modified();
  canvas3->Update();
  gSystem->ProcessEvents();
}

void Offline::Panel4Draw()
{
  canvas4->cd();
  canvas4->Clear();

  if(offlineth1d4 != NULL)
    {
      delete offlineth1d4;
      offlineth1d4 = NULL;
    }

  int intth1dbin4;
  switch(chooseth1dbin4->GetSelected())
    {
    case 1 :
      intth1dbin4 = 65536;
      break;
    case 2 :
      intth1dbin4 = 32768;
      break;
    case 3 :
      intth1dbin4 = 16384;
      break;
    case 4 :
      intth1dbin4 = 8192;
      break;
    case 5 :
      intth1dbin4 = 4096;
      break;
    case 6 :
      intth1dbin4 = 2048;
      break;
    case 7 :
      intth1dbin4 = 1024;
      break;
    default:
      intth1dbin4 = 65536;
      break;
    }
  
  offlineth1d4 = new TH1D("offlineth1d4","",intth1dbin4,0,65536);
  
  for (unsigned int i = 0; i < OfflineModuleEventsCount; ++i)
    {
      if(offlinechnum4->GetIntNumber() == OfflineEventInformation[12*i+1])
	{
	  offlineth1d4->Fill(OfflineEventInformation[12*i+9]);
	}
    }
  
  offlineth1d4->Draw();
  offlineth1d4->SetTitle(TString::Format("Ch: %d",int(offlinechnum4->GetIntNumber())).Data());
  offlineth1d4->GetXaxis()->SetTitle("ch");

  canvas4->Modified();
  canvas4->Update();
  gSystem->ProcessEvents();
}

void Offline::Panel5Draw()
{
  OfflineDrawButton5->SetEnabled(0);
  showprojectyFF5->SetEnabled(0);
  showprojectyCFD5->SetEnabled(0);
  originalcfd5->SetEnabled(0);
  calculatecfd5->SetEnabled(0);
  if(RcdTrace5 != NULL)
    {
      delete [] RcdTrace5;
      RcdTrace5 = NULL;
    }
  if(doublefastfilter5 != NULL)
    {
      delete []doublefastfilter5;
      doublefastfilter5 = NULL;
    }
  if(doublecfd5 != NULL)
    {
      delete []doublecfd5;
      doublecfd5 = NULL;
    }
  
  canvas5->cd();
  canvas5->Clear();
  canvas5->Modified();
  canvas5->Update();
  
  OriginalCFDcanvas5->cd();
  OriginalCFDcanvas5->Clear();
  CalculateCFDcanvas5->cd();
  CalculateCFDcanvas5->Clear();
  gSystem->ProcessEvents();
  canvas5->Divide(2,1);
    
  if(offlineth2d5_0 != NULL)
    {
      delete offlineth2d5_0;
      offlineth2d5_0 = NULL;
    }
  if(offlineth2d5_1 != NULL)
    {
      delete offlineth2d5_1;
      offlineth2d5_1 = NULL;
    }
  if(originalcfdth1d5 != NULL)
    {
      delete originalcfdth1d5;
      originalcfdth1d5 = NULL;
    }
  if(calculatecfdth1d5 != NULL)
    {
      delete calculatecfdth1d5;
      calculatecfdth1d5 = NULL;
    }

  
  int inttracelength = -1;
  for (unsigned int i = 0; i < OfflineModuleEventsCount; ++i)
    {
      if(offlinechnum5->GetIntNumber() == OfflineEventInformation[12*i+1])
	{
	  inttracelength = OfflineEventInformation[12*i+10];
	  break;
	}
    }

  if(inttracelength > -1)
    {
      RcdTrace5 = new unsigned short[inttracelength];
      doublefastfilter5 = new double[inttracelength];
      doublecfd5 = new double[inttracelength];

      offlineth2d5_0 = new TH2D("offlineth2d5_0","",inttracelength,0,inttracelength,1000,-100,900);
      offlineth2d5_1 = new TH2D("offlineth2d5_1","",inttracelength,0,inttracelength,1000,-100,900);

      originalcfdth1d5 = new TH1D("originalcfdth1d5","",65536,0,65536);
      calculatecfdth1d5 = new TH1D("calculatecfdth1d5","",65536,0,65536);
      
      int retval;
      for (unsigned int i = 0; i < OfflineModuleEventsCount; ++i)
	{
	  if(offlinechnum5->GetIntNumber() == OfflineEventInformation[12*i+1])
	    {
	      retval = Pixie16ComputeFastFiltersOffline(offlinefilename, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short) offlinechnum5->GetIntNumber(), OfflineEventInformation[12*i+11], OfflineEventInformation[12*i+10], RcdTrace5, doublefastfilter5, doublecfd5);
	      if(retval < 0) ErrorInfo("Offline.cc", "Panel5Draw()", "Pixie16ComputeFastFiltersOffline", retval);
	      
	      for (int j = 0; j < inttracelength; ++j)
		{
		  offlineth2d5_0->Fill(j,doublefastfilter5[j]);
		  offlineth2d5_1->Fill(j,doublecfd5[j]);
		}

	      // originalcfdth1d5->Dill(OfflineEventInformation[12*i+10]);

	      
	      // calculatecfdth1d5->Fill();
	    }

	  if(i%1000 == 0)
	    {
	      printtextinfor5->SetText(TString::Format("Drawing...please wait a moment. ==> %d/%d",i,OfflineModuleEventsCount).Data());
	      gSystem->ProcessEvents();
	    }
	}
  
      canvas5->cd(1);
      offlineth2d5_0->Draw("colz");
      offlineth2d5_0->SetTitle(TString::Format("FF  Ch: %d",int(offlinechnum5->GetIntNumber())).Data());
      offlineth2d5_0->GetXaxis()->SetTitle("sample");
      
      canvas5->cd(2);
      offlineth2d5_1->Draw("colz");
      offlineth2d5_1->SetTitle(TString::Format("CFD  Ch: %d",int(offlinechnum5->GetIntNumber())).Data());
      offlineth2d5_1->GetXaxis()->SetTitle("sample");

      showprojectyFF5->SetEnabled(1);
      showprojectyCFD5->SetEnabled(1);
    }

  printtextinfor5->SetText("Draw Done!");
  canvas5->Modified();
  canvas5->Update();
  gSystem->ProcessEvents();
  OfflineDrawButton5->SetEnabled(1);
}

void Offline::Panel6Draw()
{


}


void Offline::Panel0ReadFile()
{
  OfflineCurrentCount = -1;
  fClient->GetColorByName("green", color);
  OfflineFileStatus->SetTextColor(color, false);
  OfflineFileStatus->SetText("Waitting...");
  gSystem->ProcessEvents();

  sprintf(offlinefilename,"%s%s_R%04d_M%02d.bin",filepathtext->GetText(),filenametext->GetText(),int(offlinefilerunnum->GetIntNumber()),int(offlinemodnum->GetIntNumber()));
  // GOTO need check file 
  OfflineModuleEventsCount = GetModuleEvents(offlinefilename);
  if(OfflineModuleEventsCount > 0)
    {

      cout<<"!!!: "<<OfflineModuleEventsCount<<endl;

      if(OfflineEventInformation != NULL)
	{
	  delete []OfflineEventInformation;
	  OfflineEventInformation = NULL;
	}
      OfflineEventInformation = new unsigned int[12*OfflineModuleEventsCount];
      GetEventsInfo(offlinefilename,OfflineEventInformation);
	      
      //
      fClient->GetColorByName("red", color);
      OfflineFileStatus->SetTextColor(color, false);
      char staok[20];
      sprintf(staok,"Total Events: %d",OfflineModuleEventsCount);
      OfflineFileStatus->SetText(staok);
	      
      OfflineDrawButton->SetEnabled(1);
      OfflineDrawButton2->SetEnabled(1);
      OfflineDrawButton3->SetEnabled(1);
      OfflineDrawButton4->SetEnabled(1);
      OfflineDrawButton5->SetEnabled(1);
      OfflineDrawButton6->SetEnabled(1);
    }
  else
    {
      fClient->GetColorByName("red", color);
      OfflineFileStatus->SetTextColor(color, false);
      OfflineFileStatus->SetText("READ ERROR");
    }
}

void Offline::FFShowProjectY5()
{
  offlineth2d5_0->SetShowProjectionY(1);
}

void Offline::CFDShowProjectY5()
{
  offlineth2d5_1->SetShowProjectionY(1);
}

void Offline::OriginalCFDShow5()
{
  OriginalCFDcanvas5->cd();
  originalcfdth1d5->Draw();
  OriginalCFDcanvas5->Update();

}

void Offline::CalculateCFDShow5()
{
  CalculateCFDcanvas5->cd();
  calculatecfdth1d5->Draw();
  CalculateCFDcanvas5->Update();
}




// 
// Offline.cc ends here
