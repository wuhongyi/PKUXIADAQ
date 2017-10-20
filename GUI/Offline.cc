// Offline.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 7月 29 20:39:43 2016 (+0800)
// Last-Updated: 五 10月 20 13:13:59 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 580
// URL: http://wuhongyi.cn 


// 重新定义数据结构，4headr+1location
// 扩展支持 2+4+8数据（在数据初始化位置可选）  TODO

//ch (OfflineEventInformation[EventHeaderLength*i] & 0xF)
//slot ((OfflineEventInformation[EventHeaderLength*i] & 0xF0) >> 4)
//crate ((OfflineEventInformation[EventHeaderLength*i] & 0xF00) >> 8)
// Header length ((OfflineEventInformation[EventHeaderLength*i] & 0x1F000) >> 12)
// Event length((OfflineEventInformation[EventHeaderLength*i] & 0x7FFE0000) >> 17)
// Finish code ((OfflineEventInformation[EventHeaderLength*i] & 0x80000000) >> 31)

// EventTime_Low  OfflineEventInformation[EventHeaderLength*i+1]

// EventTime_High  (OfflineEventInformation[EventHeaderLength*i+2] & 0xFFFF)
//cfd ((OfflineEventInformation[EventHeaderLength*i+2] & 0x7FFF0000)>>16)
//cfd forced trigger bit ((OfflineEventInformation[EventHeaderLength*i+2] & 0x80000000) >> 31)

//trace length ((OfflineEventInformation[EventHeaderLength*i+3] & 0x7FFF0000)>>16)
//event energy (OfflineEventInformation[EventHeaderLength*i+3] & 0xFFFF)
//trace out-of-range flag ((OfflineEventInformation[EventHeaderLength*i+3] & 0x80000000) >> 31)

//trailing energy sum
//leading energy sum
//gap energy sum
//baseline value

//qdc sum 0
//qdc sum 1
//qdc sum 2
//qdc sum 3
//qdc sum 4
//qdc sum 5
//qdc sum 6
//qdc sum 7

//exttime_lo
//exttime_hi

#include "Offline.hh"
#include "Detector.hh"
#include "Global.hh"

#include "pixie16app_export.h"
#include "pixie16sys_export.h"
#include "xia_common.h"

#include "TGTab.h"
#include "TString.h"
#include "TFitResultPtr.h"

#include <cstring>
#include <iostream>
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#define BaseEventHeaderLength 5  //4headr+1location

Offline::Offline(const TGWindow * p, const TGWindow * main,Detector *det,TGTextEntry *filepath, TGTextEntry *filename)
{
  SetCleanup(kDeepCleanup);
  detector = det;
  filepathtext = filepath;
  filenametext = filename;

  EventHeaderLength = BaseEventHeaderLength;//可根据需要扩展
  modNumber = 0;
  chanNumber = 0;
  chanNumber4 = 0;
  chanNumber5 = 0;
  chanNumber6 = 0;
  chanNumber8 = 0;
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
  offlinemultigraph = NULL;
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
  falggausfit4 = false;
  
  offlineth2d5_0 = NULL;
  offlineth2d5_1 = NULL;
  originalcfdth1d5 = NULL;
  calculatecfdth1d5 = NULL;
  fitoriginalcfdth1d5 = NULL;
  fitcalculatecfdth1d5 = NULL;
  ltxoriginalcfdth1d5 = NULL;
  ltxcalculatecfdth1d5 = NULL;
  falgshowprojectyFF5 = false;
  flagshowprojectyCFD5 = false;
  
  RcdTrace5 = NULL;
  doublefastfilter5 = NULL;
  doublecfd5 = NULL;
  
  offlineth1d6 = NULL;
  RcdTrace6 = NULL;
  doublefastfilter6 = NULL;
  doublecfd6 = NULL;
  doubleslowfilter6 = NULL;
  falggausfit6 = false;
  
  RcdTrace8 = NULL;
  doublefastfilter8 = NULL;
  energyfffirst8 = NULL;
  energyffsecond8 = NULL;
  histenergyfffirst8 = NULL;
  histenergyffsecond8 = NULL;
  
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

  TGCompositeFrame *Tab4 = TabPanel->AddTab("Orig Energy");
  MakeFold4Panel(Tab4);

  TGCompositeFrame *Tab6 = TabPanel->AddTab("Calc Energy");
  MakeFold6Panel(Tab6);
  
  TGCompositeFrame *Tab5 = TabPanel->AddTab("FF/CFD Thre");
  MakeFold5Panel(Tab5);

  TGCompositeFrame *Tab8 = TabPanel->AddTab("Energy-FF");
  MakeFold8Panel(Tab8);
  
  TGCompositeFrame *Tab7 = TabPanel->AddTab("QCD");
  MakeFold7Panel(Tab7);

  TGCompositeFrame *Tab9 = TabPanel->AddTab("FFT");
  MakeFold9Panel(Tab9);
  
  
  SetWindowName("Review & Adjust Par");
  MapSubwindows();
  MapWindow();
  Resize();

  gStyle->SetOptStat(0);//不显示统计框
}

Offline::~Offline()
{
  if(rawdata != NULL) delete rawdata;
  if(threshdata != NULL) delete threshdata;
  if(cfddata != NULL) delete cfddata;
  if(cfdthreshdata != NULL) delete cfdthreshdata;
  if(sfilterdata != NULL) delete sfilterdata;
  if(ffilterdata != NULL) delete ffilterdata;
  if(RcdTrace != NULL) delete []RcdTrace;
  if(doublethresh != NULL) delete []doublethresh;
  if(doublecfdthresh != NULL) delete []doublecfdthresh;
  if(doublesample != NULL) delete []doublesample;
  if(doublercdtrace != NULL) delete []doublercdtrace;
  if(doublefastfilter != NULL) delete []doublefastfilter;
  if(doublecfd != NULL) delete []doublecfd;
  if(doubleslowfilter != NULL) delete []doubleslowfilter;

  if(RcdTrace5 != NULL) delete []RcdTrace5;
  if(doublefastfilter5 != NULL) delete []doublefastfilter5;
  if(doublecfd5 != NULL) delete []doublecfd5;
  
  if(RcdTrace6 != NULL) delete []RcdTrace6;
  if(doublefastfilter6 != NULL) delete []doublefastfilter6;
  if(doublecfd6 != NULL) delete []doublecfd6;
  if(doubleslowfilter6 != NULL) delete []doubleslowfilter6;

  if(RcdTrace8 != NULL) delete []RcdTrace8;
  if(doublefastfilter8 != NULL) delete []doublefastfilter8;
  if(energyfffirst8 != NULL) delete energyfffirst8;
  if(energyffsecond8 != NULL) delete energyffsecond8;
  if(histenergyfffirst8 != NULL) delete histenergyfffirst8;
  if(histenergyffsecond8 != NULL) delete histenergyffsecond8;

  
  for (int i = 0; i < 16; ++i)
    {
      if(rawdata2[i] != NULL) delete rawdata2[i];
      if(threshdata2[i] != NULL) delete threshdata2[i];
      if(cfdthreshdata2[i] != NULL) delete cfdthreshdata2[i];
      if(cfddata2[i] != NULL) delete cfddata2[i];
      if(sfilterdata2[i] != NULL) delete sfilterdata2[i];
      if(ffilterdata2[i] != NULL) delete ffilterdata2[i];
      if(RcdTrace2[i] != NULL) delete []RcdTrace2[i];
      if(doublethresh2[i] != NULL) delete []doublethresh2[i];
      if(doublesample2[i] != NULL) delete []doublesample2[i];	      
      if(doublercdtrace2[i] != NULL) delete []doublercdtrace2[i];
      if(doublefastfilter2[i] != NULL) delete []doublefastfilter2[i];
      if(doublecfd2[i] != NULL) delete []doublecfd2[i];
      if(doubleslowfilter2[i] != NULL) delete []doubleslowfilter2[i];
    }

  if(OfflineEventInformation != NULL) delete []OfflineEventInformation;
     
  if(offlinemultigraph != NULL) delete offlinemultigraph;

  for (int i = 0; i < 16; ++i)
    {
      if(offlinemultigraph2[i] != NULL) delete offlinemultigraph2[i];
      if(offlineth1d3[i] != NULL) delete offlineth1d3[i];
    }

  if(offlineth1d4 != NULL) delete offlineth1d4;
  if(offlineth2d5_0 != NULL) delete offlineth2d5_0;
  if(offlineth2d5_1 != NULL) delete offlineth2d5_1;
  if(originalcfdth1d5 != NULL) delete originalcfdth1d5;
  if(calculatecfdth1d5 != NULL) delete calculatecfdth1d5;
  if(fitoriginalcfdth1d5 != NULL) delete fitoriginalcfdth1d5;
  if(fitcalculatecfdth1d5 != NULL) delete fitcalculatecfdth1d5;
  if(ltxoriginalcfdth1d5 != NULL) delete ltxoriginalcfdth1d5;
  if(ltxcalculatecfdth1d5 != NULL) delete ltxcalculatecfdth1d5;

  
  if(offlineth1d6 != NULL) delete offlineth1d6;
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

  // MHz
  choosesamplemhz0 = new TGComboBox(parFrame);
  parFrame->AddFrame(choosesamplemhz0, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 0, 0));
  choosesamplemhz0->Resize(50, 20);
  choosesamplemhz0->AddEntry("100", 1);
  choosesamplemhz0->AddEntry("250", 2);
  choosesamplemhz0->AddEntry("500", 3);
  choosesamplemhz0->Select(1);
  // choosesamplemhz0->Connect("Selected(Int_t id)", "Offline", this, "SelectSamplingFrequency(Int_t id)");

  
  TGTextEntry *LabelMHz = new TGTextEntry(parFrame,new TGTextBuffer(30));
  LabelMHz->SetText("MHz");
  LabelMHz->Resize(30,20);
  LabelMHz->SetEnabled(kFALSE);
  // LabelMHz->SetToolTipText("Choose module ");
  parFrame->AddFrame(LabelMHz, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 20, 0, 0));
  

  // choose header length
  TGLabel *additionalanalysis = new TGLabel(parFrame, "Additional analysis:");
  parFrame->AddFrame(additionalanalysis, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 3, 0));
  fClient->GetColorByName("orange", color);
  additionalanalysis->SetTextColor(color, false);
  headerrawenergysumsandbaseline = new TGCheckButton(parFrame, "raw E sums/baseline");
  headerrawenergysumsandbaseline->SetOn(kFALSE);
  headerrawenergysumsandbaseline->Connect("Toggled(Bool_t)", "Offline", this, "SelectRawEnergySumsBaseline(Bool_t)");
  fClient->GetColorByName("orange", color);
  headerrawenergysumsandbaseline->SetTextColor(color, false);
  parFrame->AddFrame(headerrawenergysumsandbaseline, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 3, 0));

  headerqdcsums = new TGCheckButton(parFrame, "QDC sums");
  headerqdcsums->SetOn(kFALSE);
  headerqdcsums->Connect("Toggled(Bool_t)", "Offline", this, "SelectQDCSums(Bool_t)");
  fClient->GetColorByName("orange", color);
  headerqdcsums->SetTextColor(color, false);
  parFrame->AddFrame(headerqdcsums, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 3, 0));

  headerexternaltimestamp = new TGCheckButton(parFrame, "external timestamp");
  headerexternaltimestamp->SetOn(kFALSE);
  headerexternaltimestamp->Connect("Toggled(Bool_t)", "Offline", this, "SelectExternalTimestamp(Bool_t)");
  fClient->GetColorByName("orange", color);
  headerexternaltimestamp->SetTextColor(color, false);
  parFrame->AddFrame(headerexternaltimestamp, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 30, 3, 0));
  
   
   
  
  // run
  TGLabel *run = new TGLabel( parFrame, "Run:");
  parFrame->AddFrame(run, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 3, 0));
  fClient->GetColorByName("red", color);
  run->SetTextColor(color, false);
  
  offlinefilerunnum = new TGNumberEntry(parFrame, 0, 4, OFFLINERUNNUM, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, 9999);
  offlinefilerunnum->SetButtonToNum(0);
  offlinefilerunnum->Associate(this);
  parFrame->AddFrame(offlinefilerunnum, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 0, 0));

  // mod
  TGLabel *mod = new TGLabel( parFrame, "Mod:"); 
  parFrame->AddFrame(mod, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 3, 0));
  fClient->GetColorByName("red", color);
  mod->SetTextColor(color, false);
  
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

  // Draw option
  TGLabel *drawoptionlabel = new TGLabel(parFrame, "Draw option:");
  parFrame->AddFrame(drawoptionlabel, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 3, 0));
  // fClient->GetColorByName("red", color);
  // drawoptionlabel->SetTextColor(color, false);

  offlinedrawoption1[0] = new TGCheckButton(parFrame, "Wave");
  offlinedrawoption1[0]->SetOn(kTRUE);
  offlinedrawoption1[0]->Connect("Toggled(Bool_t)", "Offline", this, "SelectDrawOptionPanel1(Bool_t)");
  parFrame->AddFrame(offlinedrawoption1[0], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 3, 0));

  offlinedrawoption1[1] = new TGCheckButton(parFrame, "Slow Filter");
  offlinedrawoption1[1]->SetOn(kTRUE);
  offlinedrawoption1[1]->Connect("Toggled(Bool_t)", "Offline", this, "SelectDrawOptionPanel1(Bool_t)");
  fClient->GetColorByName("green", color);
  offlinedrawoption1[1]->SetTextColor(color, false);
  parFrame->AddFrame(offlinedrawoption1[1], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 3, 0));

  offlinedrawoption1[2] = new TGCheckButton(parFrame, "Fast Filter");
  offlinedrawoption1[2]->SetOn(kTRUE);
  offlinedrawoption1[2]->Connect("Toggled(Bool_t)", "Offline", this, "SelectDrawOptionPanel1(Bool_t)");
  fClient->GetColorByName("blue", color);
  offlinedrawoption1[2]->SetTextColor(color, false);
  parFrame->AddFrame(offlinedrawoption1[2], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 3, 0));

  offlinedrawoption1[3] = new TGCheckButton(parFrame, "Thres");
  offlinedrawoption1[3]->SetOn(kTRUE);
  offlinedrawoption1[3]->Connect("Toggled(Bool_t)", "Offline", this, "SelectDrawOptionPanel1(Bool_t)");
  fClient->GetColorByName("blue", color);
  offlinedrawoption1[3]->SetTextColor(color, false);
  parFrame->AddFrame(offlinedrawoption1[3], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 3, 0));  

  offlinedrawoption1[4] = new TGCheckButton(parFrame, "CFD");
  offlinedrawoption1[4]->SetOn(kTRUE);
  offlinedrawoption1[4]->Connect("Toggled(Bool_t)", "Offline", this, "SelectDrawOptionPanel1(Bool_t)");
  fClient->GetColorByName("red", color);
  offlinedrawoption1[4]->SetTextColor(color, false);
  parFrame->AddFrame(offlinedrawoption1[4], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 3, 0));

  offlinedrawoption1[5] = new TGCheckButton(parFrame, "CFD Thres");
  offlinedrawoption1[5]->SetOn(kTRUE);
  offlinedrawoption1[5]->Connect("Toggled(Bool_t)", "Offline", this, "SelectDrawOptionPanel1(Bool_t)");
  fClient->GetColorByName("red", color);
  offlinedrawoption1[5]->SetTextColor(color, false);
  parFrame->AddFrame(offlinedrawoption1[5], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 3, 0));


  //slow filter baseline
  TGLabel *LabelChooseSlowFIlterBaseline = new TGLabel(parFrame, "SF BL:"); 
  parFrame->AddFrame(LabelChooseSlowFIlterBaseline, new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 2, 5, 0));
  fClient->GetColorByName("green", color);
  LabelChooseSlowFIlterBaseline->SetTextColor(color, false);
  chooseslowfilterbaseline = new TGComboBox(parFrame);
  parFrame->AddFrame(chooseslowfilterbaseline, new TGLayoutHints(kLHintsLeft, 0, 0, 2, 2));
  chooseslowfilterbaseline->Resize(80, 20);
  chooseslowfilterbaseline->AddEntry("Calculate", 0);
  chooseslowfilterbaseline->AddEntry("Old Baseline", 1);
  chooseslowfilterbaseline->Select(0);

  TGLabel *LabelOldSlowFilterSL = new TGLabel(parFrame," 'Old BL' choose -> SL:");
  parFrame->AddFrame(LabelOldSlowFilterSL, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 5, 0));
  fClient->GetColorByName("green", color);
  LabelOldSlowFilterSL->SetTextColor(color, false);
  oldslowfilterparameter[0] = new TGNumberEntryField(parFrame, -1, 3.04, TGNumberFormat::kNESReal,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,0.04,81.28);
  parFrame->AddFrame(oldslowfilterparameter[0], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 2, 0));
  oldslowfilterparameter[0]->Resize(40, 20);
  TGLabel *LabelOldSlowFilterSG = new TGLabel(parFrame,"SG:");
  parFrame->AddFrame(LabelOldSlowFilterSG, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 5, 0));
  fClient->GetColorByName("green", color);
  LabelOldSlowFilterSG->SetTextColor(color, false);
  oldslowfilterparameter[1] = new TGNumberEntryField(parFrame, -1, 0.64, TGNumberFormat::kNESReal,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,0.06,81.28);
  parFrame->AddFrame(oldslowfilterparameter[1], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 20, 2, 0));
  oldslowfilterparameter[1]->Resize(40, 20);

  
  // current count
  OfflineCurrentCountText = new TGTextEntry(parFrame,new TGTextBuffer(30), 10000);
  OfflineCurrentCountText-> SetFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1", false);
  fClient->GetColorByName("blue", color);
  OfflineCurrentCountText->SetTextColor(color, false);
  // OfflineCurrentCountText->SetAlignment(kTextCenterX);
  OfflineCurrentCountText->SetText("/-1");
  OfflineCurrentCountText->Resize(150, 12);
  OfflineCurrentCountText->SetEnabled(kFALSE);
  OfflineCurrentCountText->SetFrameDrawn(kFALSE);
  parFrame->AddFrame(OfflineCurrentCountText, new TGLayoutHints(kLHintsRight | kLHintsTop, 0, 0, 6, 0));


  // CurrentCount
  offlinecurrentcountentry = new TGNumberEntryField(parFrame, -1, 0, TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative);
  parFrame->AddFrame(offlinecurrentcountentry, new TGLayoutHints(kLHintsRight | kLHintsTop, 20, 0, 0, 0));
  
  
  // draw
  OfflineDrawButton = new TGTextButton(parFrame, "&Draw", OFFLINEDRAW);
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
  // TODO
  // connect to picture


  // ===
  sbfold3 = new TGStatusBar(TabPanel,10,10);
  int temp[4];
  temp[0] = 33;
  temp[1] = 10;
  temp[2] = 10;
  temp[3] = 47;
  sbfold3->SetParts(temp,4);
  //TODO
  // connect to canvas
  TabPanel->AddFrame(sbfold3, new TGLayoutHints(kLHintsBottom | kLHintsLeft | kLHintsExpandX, 2, 2, 1, 1));
}

void Offline::SelectDrawOptionPanel1(Bool_t on)
{
  if(rawdata != NULL || threshdata != NULL || cfddata != NULL || cfdthreshdata != NULL || sfilterdata != NULL || ffilterdata != NULL)
    {
      if(rawdata != NULL)
	{
	  delete rawdata;
	  rawdata = new TGraph(tracelength,doublesample,doublercdtrace);
	}
      if(threshdata != NULL)
	{
	  delete threshdata;
	  threshdata = new TGraph(tracelength,doublesample,doublethresh);
	}
      if(cfddata != NULL)
	{
	  delete cfddata;
	  cfddata = new TGraph(tracelength,doublesample,doublecfd);
	}
      if(cfdthreshdata != NULL)
	{
	  delete cfdthreshdata;
	  cfdthreshdata = new TGraph(tracelength,doublesample,doublecfdthresh);
	}
      if(sfilterdata != NULL)
	{
	  delete sfilterdata;
	  sfilterdata = new TGraph(tracelength,doublesample,doubleslowfilter);
	}
      if(ffilterdata != NULL)
	{
	  delete ffilterdata;
	  ffilterdata  = new TGraph(tracelength,doublesample,doublefastfilter);
	}
      
      cfddata->SetLineColor(2);
      cfdthreshdata->SetLineColor(2);
      sfilterdata->SetLineColor(3);
      ffilterdata->SetLineColor(4);
      threshdata->SetLineColor(4);
      if(offlinedrawoption1[0]->IsOn())
	offlinemultigraph->Add(rawdata);
      if(offlinedrawoption1[3]->IsOn())
	offlinemultigraph->Add(threshdata);
      if(offlinedrawoption1[4]->IsOn())
	offlinemultigraph->Add(cfddata);
      if(offlinedrawoption1[5]->IsOn())
	offlinemultigraph->Add(cfdthreshdata);
      if(offlinedrawoption1[1]->IsOn())
	offlinemultigraph->Add(sfilterdata);
      if(offlinedrawoption1[2]->IsOn())
	offlinemultigraph->Add(ffilterdata);
      
      adjustCanvas->cd();
      adjustCanvas->Clear();
      offlinemultigraph->Draw("AL");
      if(offlinedrawoption1[0]->IsOn() || offlinedrawoption1[1]->IsOn() || offlinedrawoption1[2]->IsOn() || offlinedrawoption1[3]->IsOn() || offlinedrawoption1[4]->IsOn() || offlinedrawoption1[5]->IsOn())
	offlinemultigraph->GetXaxis()->SetRangeUser(double(dslider->GetMinPosition()),double(dslider->GetMaxPosition()));
      adjustCanvas->Modified();
      adjustCanvas->Update();
    }

  // gSystem->ProcessEvents();

}


void Offline::MakeFold2Panel(TGCompositeFrame *TabPanel)
{
  TGCompositeFrame *parFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);

  // Draw option
  TGLabel *drawoptionlabel = new TGLabel(parFrame, "Draw option:");
  parFrame->AddFrame(drawoptionlabel, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 3, 0));
  // fClient->GetColorByName("red", color);
  // drawoptionlabel->SetTextColor(color, false);

  offlinedrawoption2[0] = new TGCheckButton(parFrame, "Wave");
  offlinedrawoption2[0]->SetOn(kTRUE);
  offlinedrawoption2[0]->Connect("Toggled(Bool_t)", "Offline", this, "SelectDrawOptionPanel2(Bool_t)");
  parFrame->AddFrame(offlinedrawoption2[0], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 3, 0));

  offlinedrawoption2[1] = new TGCheckButton(parFrame, "Slow Filter");
  offlinedrawoption2[1]->SetOn(kTRUE);
  offlinedrawoption2[1]->Connect("Toggled(Bool_t)", "Offline", this, "SelectDrawOptionPanel2(Bool_t)");
  fClient->GetColorByName("green", color);
  offlinedrawoption2[1]->SetTextColor(color, false);
  parFrame->AddFrame(offlinedrawoption2[1], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 3, 0));

  offlinedrawoption2[2] = new TGCheckButton(parFrame, "Fast Filter");
  offlinedrawoption2[2]->SetOn(kTRUE);
  offlinedrawoption2[2]->Connect("Toggled(Bool_t)", "Offline", this, "SelectDrawOptionPanel2(Bool_t)");
  fClient->GetColorByName("blue", color);
  offlinedrawoption2[2]->SetTextColor(color, false);
  parFrame->AddFrame(offlinedrawoption2[2], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 3, 0));

  offlinedrawoption2[3] = new TGCheckButton(parFrame, "Thres");
  offlinedrawoption2[3]->SetOn(kTRUE);
  offlinedrawoption2[3]->Connect("Toggled(Bool_t)", "Offline", this, "SelectDrawOptionPanel2(Bool_t)");
  fClient->GetColorByName("blue", color);
  offlinedrawoption2[3]->SetTextColor(color, false);
  parFrame->AddFrame(offlinedrawoption2[3], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 3, 0));  

  offlinedrawoption2[4] = new TGCheckButton(parFrame, "CFD");
  offlinedrawoption2[4]->SetOn(kTRUE);
  offlinedrawoption2[4]->Connect("Toggled(Bool_t)", "Offline", this, "SelectDrawOptionPanel2(Bool_t)");
  fClient->GetColorByName("red", color);
  offlinedrawoption2[4]->SetTextColor(color, false);
  parFrame->AddFrame(offlinedrawoption2[4], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 3, 0));

  offlinedrawoption2[5] = new TGCheckButton(parFrame, "CFD Thres");
  offlinedrawoption2[5]->SetOn(kTRUE);
  offlinedrawoption2[5]->Connect("Toggled(Bool_t)", "Offline", this, "SelectDrawOptionPanel2(Bool_t)");
  fClient->GetColorByName("red", color);
  offlinedrawoption2[5]->SetTextColor(color, false);
  parFrame->AddFrame(offlinedrawoption2[5], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 3, 0));

  
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

void Offline::SelectDrawOptionPanel2(Bool_t on)
{
  bool tempstatus[16];
  for (unsigned int i = 0; i < 16; ++i)
    {
      tempstatus[i] = false;
      if(rawdata2[i] != NULL || threshdata2[i] != NULL || cfddata2[i] != NULL || cfdthreshdata2[i] != NULL || sfilterdata2[i] != NULL || ffilterdata2[i] != NULL)
	{
	  tempstatus[i] = true;
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
	  if(cfddata2[i] != NULL)
	    {
	      delete cfddata2[i];
	      cfddata2[i] = NULL;
	    }
	  if(cfdthreshdata2[i] != NULL)
	    {
	      delete cfdthreshdata2[i];
	      cfdthreshdata2[i] = NULL;
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
	}
    }
  for (unsigned int i = 0; i < 16; ++i)
    {
      if(tempstatus[i])
	{
	  rawdata2[i] = new TGraph(tracelength2[i],doublesample2[i],doublercdtrace2[i]);
	  threshdata2[i] = new TGraph(tracelength2[i],doublesample2[i],doublethresh2[i]);
	  cfddata2[i] = new TGraph(tracelength2[i],doublesample2[i],doublecfd2[i]);
	  cfdthreshdata2[i] = new TGraph(tracelength2[i],doublesample2[i],doublecfdthresh2[i]);
	  sfilterdata2[i] = new TGraph(tracelength2[i],doublesample2[i],doubleslowfilter2[i]);
	  ffilterdata2[i] = new TGraph(tracelength2[i],doublesample2[i],doublefastfilter2[i]);
	  cfddata2[i]->SetLineColor(2);
	  cfdthreshdata2[i]->SetLineColor(2);
	  sfilterdata2[i]->SetLineColor(3);
	  ffilterdata2[i]->SetLineColor(4);
	  threshdata2[i]->SetLineColor(4);
	  offlinemultigraph2[i]->Clear();
	  if(offlinedrawoption2[0]->IsOn())
	    offlinemultigraph2[i]->Add(rawdata2[i]);
	  if(offlinedrawoption2[3]->IsOn())
	    offlinemultigraph2[i]->Add(threshdata2[i]);
	  if(offlinedrawoption2[5]->IsOn())
	    offlinemultigraph2[i]->Add(cfdthreshdata2[i]);
	  if(offlinedrawoption2[4]->IsOn())
	    offlinemultigraph2[i]->Add(cfddata2[i]);
	  if(offlinedrawoption2[1]->IsOn())
	    offlinemultigraph2[i]->Add(sfilterdata2[i]);
	  if(offlinedrawoption2[2]->IsOn())
	    offlinemultigraph2[i]->Add(ffilterdata2[i]);
	  canvas2->cd(1+i);
	  offlinemultigraph2[i]->SetTitle(TString::Format("Event: %d",OfflineCurrentCount2[i]).Data());
	  offlinemultigraph2[i]->Draw("AL");
	} //!= NULL
    } //0-15
  canvas2->Modified();
  canvas2->Update();
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

  // Fit
  GausFitButton4 = new TGTextButton( parFrame, "Open  Fit", OFFLINEGAUSFIT4);
  GausFitButton4->SetEnabled(0);
  GausFitButton4->Associate(this);
  parFrame->AddFrame(GausFitButton4, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 30, 0, 0));
  
  
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

void Offline::GausFit4()
{
  if(falggausfit4)
    {
      GausFitButton4->SetText("Open  Fit");
      // offlineth1d4->SetTitle("");
      falggausfit4 = false;
      canvas4->DeleteExec("dynamicPanel4GausFit");
    }
  else
    {
      GausFitButton4->SetText("Close Fit");
      falggausfit4 = true;
      canvas4->AddExec("dynamicPanel4GausFit","PanelGausFit()");
    }
}

void Offline::GausFit6()
{
  if(falggausfit6)
    {
      GausFitButton6->SetText("Open  Fit");
      falggausfit6 = false;
      canvas6->DeleteExec("dynamicPanel6GausFit");
    }
  else
    {
      GausFitButton6->SetText("Close Fit");
      falggausfit6 = true;
      canvas6->AddExec("dynamicPanel6GausFit","PanelGausFit()");
    }
}

void PanelGausFit()
{
  // TODO  可以加显示选择的线
  int pe = gPad->GetEvent();
  if(pe != 11) return;
  gPad->GetCanvas()->FeedbackMode(kTRUE);

  if(gPad->GetUniqueID() == 0)
    {
      gPad->SetUniqueID(gPad->GetEventX());
    }
  else
    {
      int pxold = gPad->GetUniqueID();
      Float_t upxold = gPad->AbsPixeltoX(pxold);
      int px = gPad->GetEventX();
      Float_t upx = gPad->AbsPixeltoX(px);

      TObject *select = gPad->GetSelected();
      if(!select) {gPad->SetUniqueID(0); return;}
      if (!select->InheritsFrom(TH1::Class())) {gPad->SetUniqueID(0); return;}
      TH1 *h = (TH1*)select;					      
      if(upxold > upx)
	{
	  Float_t temp;
	  temp = upxold;
	  upxold = upx;
	  upx = temp;
	}
      if(h->Fit("gaus","QL","",upxold,upx) == 0)
	{
	  h->SetTitle(TString::Format("Peak: %0.2f  #DeltaE/E: %0.2f%%",h->GetFunction("gaus")->GetParameter(1),h->GetFunction("gaus")->GetParameter(2)*2.355/h->GetFunction("gaus")->GetParameter(1)*100.0).Data());
	}
      else
	{
	  h->SetTitle("Please choose Fit range again.");
	}
      gPad->SetUniqueID(0);
    }
}

  
void Offline::MakeFold5Panel(TGCompositeFrame *TabPanel)
{
  TGCompositeFrame *parFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);

  // stop
  OfflineStopButton5 = new TGTextButton(parFrame, "&Stop", OFFLINESTOPDRAW5);
  OfflineStopButton5->SetEnabled(0);
  OfflineStopButton5->Associate(this);
  parFrame->AddFrame(OfflineStopButton5, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 0, 0));
  
  // text
  printtextinfor5 = new TGTextEntry(parFrame,new TGTextBuffer(30), 10000);
  printtextinfor5->SetFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1", false);
  fClient->GetColorByName("blue", color);
  printtextinfor5->SetTextColor(color, false);
  printtextinfor5->SetText("Choose 'Ch' then enter button 'Draw'.");
  printtextinfor5->Resize(500, 12);
  printtextinfor5->SetEnabled(kFALSE);
  printtextinfor5->SetFrameDrawn(kFALSE);
  parFrame->AddFrame(printtextinfor5, new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 0, 6, 0));


  
  // TODO
  // Sigma
  // TGLabel *sigma = new TGLabel( parFrame, "RecomSigma:"); 
  // parFrame->AddFrame(sigma, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 2, 3, 0));
  // recommendedsigma5 = new TGComboBox(parFrame);
  // parFrame->AddFrame(recommendedsigma5, new TGLayoutHints(kLHintsLeft, 0, 10, 2, 2));
  // recommendedsigma5->Resize(40, 20);
  // recommendedsigma5->AddEntry("x 1", 1);
  // recommendedsigma5->AddEntry("x 2", 2);
  // recommendedsigma5->AddEntry("x 3", 3);
  // recommendedsigma5->AddEntry("x 4", 4);
  // recommendedsigma5->AddEntry("x 5", 5);
  // recommendedsigma5->AddEntry("x 6", 6);
  // recommendedsigma5->AddEntry("x 7", 7);
  // recommendedsigma5->AddEntry("x 8", 8);
  // recommendedsigma5->AddEntry("x 9", 9);
  // recommendedsigma5->AddEntry("x10", 10);
  // recommendedsigma5->Select(3);
  
  // FFProjectY
  showprojectyFF5 = new TGTextButton(parFrame, "&FFProjectY", OFFLINEPROJECTYFF5);
  showprojectyFF5->SetEnabled(0);
  showprojectyFF5->Associate(this);
  parFrame->AddFrame(showprojectyFF5, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 0, 0));

  // CFDProjectY
  showprojectyCFD5 = new TGTextButton(parFrame, "&CFDProjectY", OFFLINEPROJECTYCFD5);
  showprojectyCFD5->SetEnabled(0);
  showprojectyCFD5->Associate(this);
  parFrame->AddFrame(showprojectyCFD5, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 30, 0, 0));


  // OriginalCFD
  originalcfd5 = new TGTextButton(parFrame, "&OriginalCFD", OFFLINEORIGINALCFD5);
  originalcfd5->SetEnabled(0);
  originalcfd5->Associate(this);
  parFrame->AddFrame(originalcfd5, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 0, 0));

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
  
  OriginalCFDcanvas5 = NULL;
  CalculateCFDcanvas5 = NULL;
}

void Offline::MakeFold6Panel(TGCompositeFrame *TabPanel)
{
  TGCompositeFrame *parFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);

  // stop
  OfflineStopButton6 = new TGTextButton( parFrame, "&Stop", OFFLINESTOPDRAW6);
  OfflineStopButton6->SetEnabled(0);
  OfflineStopButton6->Associate(this);
  parFrame->AddFrame(OfflineStopButton6, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 0, 0));
  
  // text
  printtextinfor6 = new TGTextEntry(parFrame,new TGTextBuffer(30), 10000);
  printtextinfor6-> SetFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1", false);
  fClient->GetColorByName("blue", color);
  printtextinfor6->SetTextColor(color, false);
  printtextinfor6->SetText("Choose 'Bin' and 'Ch' then enter button 'Draw'.");
  printtextinfor6->Resize(500, 12);
  printtextinfor6->SetEnabled(kFALSE);
  printtextinfor6->SetFrameDrawn(kFALSE);
  parFrame->AddFrame(printtextinfor6, new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 0, 6, 0));



  // Fit
  GausFitButton6 = new TGTextButton( parFrame, "Open  Fit", OFFLINEGAUSFIT6);
  GausFitButton6->SetEnabled(0);
  GausFitButton6->Associate(this);
  parFrame->AddFrame(GausFitButton6, new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 30, 0, 0));
  
 
  
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

  // bin
  chooseth1dbin6 = new TGComboBox(parFrame);
  parFrame->AddFrame(chooseth1dbin6, new TGLayoutHints(kLHintsRight, 5, 25, 2, 2));
  chooseth1dbin6->Resize(80, 20);
  chooseth1dbin6->AddEntry("65536", 1);
  chooseth1dbin6->AddEntry("32768", 2);
  chooseth1dbin6->AddEntry("16384", 3);
  chooseth1dbin6->AddEntry("8192", 4);
  chooseth1dbin6->AddEntry("4096", 5);
  chooseth1dbin6->AddEntry("2048", 6);
  chooseth1dbin6->AddEntry("1024", 7);
  chooseth1dbin6->Select(1);
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

  TRootEmbeddedCanvas *adjCanvas = new TRootEmbeddedCanvas("canvas6", adCanvasFrame, 100, 100);

  canvas6 = adjCanvas->GetCanvas();
  adCanvasFrame->AddFrame(adjCanvas, Hint);
  TabPanel->AddFrame(adCanvasFrame, Hint);
}


void Offline::MakeFold7Panel(TGCompositeFrame *TabPanel)
{
  TGCompositeFrame *parFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);

  // // draw
  // OfflineDrawButton6 = new TGTextButton( parFrame, "&Draw", OFFLINEDRAW6);
  // OfflineDrawButton6->SetEnabled(0);
  // OfflineDrawButton6->Associate(this);
  // parFrame->AddFrame(OfflineDrawButton6, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 30, 0, 0));

  //   // ch
  // offlinechnum6 = new TGNumberEntry (parFrame, 0, 2, OFFLINECHNUM6, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, 15);
  // parFrame->AddFrame(offlinechnum6, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 10, 0, 0));
  // offlinechnum6->SetButtonToNum(0);
  // offlinechnum6->Associate(this);
  // TGLabel *ch = new TGLabel( parFrame, "Ch:"); 
  // parFrame->AddFrame(ch, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 2, 3, 0));
  

  TabPanel->AddFrame(parFrame, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 2, 2, 1, 1));

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGCompositeFrame *adCanvasFrame = new TGCompositeFrame(TabPanel, 800, 800, kHorizontalFrame);
  TGLayoutHints *Hint = new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 1, 1, 1, 1);

  TRootEmbeddedCanvas *adjCanvas = new TRootEmbeddedCanvas("canvas7", adCanvasFrame, 100, 100);

  canvas7 = adjCanvas->GetCanvas();
  adCanvasFrame->AddFrame(adjCanvas, Hint);
  TabPanel->AddFrame(adCanvasFrame, Hint);
}


void Offline::MakeFold8Panel(TGCompositeFrame *TabPanel)
{
  TGCompositeFrame *parFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);

  // stop
  OfflineStopButton8 = new TGTextButton(parFrame, "&Stop", OFFLINESTOPDRAW8);
  OfflineStopButton8->SetEnabled(0);
  OfflineStopButton8->Associate(this);
  parFrame->AddFrame(OfflineStopButton8, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 0, 0));
  
  // text
  printtextinfor8 = new TGTextEntry(parFrame,new TGTextBuffer(30), 10000);
  printtextinfor8->SetFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1", false);
  fClient->GetColorByName("blue", color);
  printtextinfor8->SetTextColor(color, false);
  printtextinfor8->SetText("Choose 'Ch' then enter button 'Draw'.");
  printtextinfor8->Resize(450, 12);
  printtextinfor8->SetEnabled(kFALSE);
  printtextinfor8->SetFrameDrawn(kFALSE);
  parFrame->AddFrame(printtextinfor8, new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 0, 6, 0));

  
  // Draw Style
  TGLabel *LabelChooseDrawStyle = new TGLabel(parFrame, "Draw Style:"); 
  parFrame->AddFrame(LabelChooseDrawStyle, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 5, 0));
  fClient->GetColorByName("red", color);
  LabelChooseDrawStyle->SetTextColor(color, false);
  choosedrawstyle8 = new TGComboBox(parFrame);
  parFrame->AddFrame(choosedrawstyle8, new TGLayoutHints(kLHintsLeft, 0, 0, 2, 2));
  choosedrawstyle8->Resize(50, 20);
  choosedrawstyle8->AddEntry("Graph", 0);
  choosedrawstyle8->AddEntry("Hist", 1);
  choosedrawstyle8->Select(0);

  TGLabel *LabelChooseHistBinX = new TGLabel(parFrame,"  'Hist' mode choose -> BinX:");
  parFrame->AddFrame(LabelChooseHistBinX, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 5, 0));
  fClient->GetColorByName("green", color);
  LabelChooseHistBinX->SetTextColor(color, false);
  choosehistbinxy[0] = new TGComboBox(parFrame);
  parFrame->AddFrame(choosehistbinxy[0], new TGLayoutHints(kLHintsLeft, 0, 0, 2, 2));
  choosehistbinxy[0]->Resize(50, 20);
  choosehistbinxy[0]->AddEntry("100", 100);
  choosehistbinxy[0]->AddEntry("200", 200);
  choosehistbinxy[0]->AddEntry("500", 500);
  choosehistbinxy[0]->AddEntry("1000", 1000);
  choosehistbinxy[0]->AddEntry("1500", 1500);
  choosehistbinxy[0]->AddEntry("2000", 2000);
  choosehistbinxy[0]->Select(1000);

  TGLabel *LabelChooseHistXmin = new TGLabel(parFrame,"Xmin:");
  parFrame->AddFrame(LabelChooseHistXmin, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 5, 0));
  fClient->GetColorByName("green", color);
  LabelChooseHistXmin->SetTextColor(color, false);
  histxyminmax8[0] = new TGNumberEntryField(parFrame, -1, 0, TGNumberFormat::kNESReal,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,0,65536);
  parFrame->AddFrame(histxyminmax8[0], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 2, 0));
  histxyminmax8[0]->Resize(40, 20);
  
  TGLabel *LabelChooseHistXmax = new TGLabel(parFrame,"Xmax:");
  parFrame->AddFrame(LabelChooseHistXmax, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 5, 0));
  fClient->GetColorByName("green", color);
  LabelChooseHistXmax->SetTextColor(color, false);
  histxyminmax8[1] = new TGNumberEntryField(parFrame, -1, 1000, TGNumberFormat::kNESReal,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,0,65536);
  parFrame->AddFrame(histxyminmax8[1], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 2, 0));
  histxyminmax8[1]->Resize(40, 20);  

  TGLabel *LabelChooseHistBinY = new TGLabel(parFrame,"BinY:");
  parFrame->AddFrame(LabelChooseHistBinY, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 5, 0));
  fClient->GetColorByName("green", color);
  LabelChooseHistBinY->SetTextColor(color, false);
  choosehistbinxy[1] = new TGComboBox(parFrame);
  parFrame->AddFrame(choosehistbinxy[1], new TGLayoutHints(kLHintsLeft, 0, 0, 2, 2));
  choosehistbinxy[1]->Resize(50, 20);
  choosehistbinxy[1]->AddEntry("512", 512);
  choosehistbinxy[1]->AddEntry("1024", 1024);
  choosehistbinxy[1]->AddEntry("2048", 2048);
  choosehistbinxy[1]->Select(1024);

  TGLabel *LabelChooseHistYmin = new TGLabel(parFrame,"Ymin:");
  parFrame->AddFrame(LabelChooseHistYmin, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 5, 0));
  fClient->GetColorByName("green", color);
  LabelChooseHistYmin->SetTextColor(color, false);
  histxyminmax8[2] = new TGNumberEntryField(parFrame, -1, 0, TGNumberFormat::kNESReal,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,0,65536);
  parFrame->AddFrame(histxyminmax8[2], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 2, 0));
  histxyminmax8[2]->Resize(40, 20);

  TGLabel *LabelChooseHistYmax = new TGLabel(parFrame,"Ymax:");
  parFrame->AddFrame(LabelChooseHistYmax, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 5, 0));
  fClient->GetColorByName("green", color);
  LabelChooseHistYmax->SetTextColor(color, false);
  histxyminmax8[3] = new TGNumberEntryField(parFrame, -1, 65536, TGNumberFormat::kNESReal,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,0,65536);
  parFrame->AddFrame(histxyminmax8[3], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 2, 0));
  histxyminmax8[3]->Resize(40, 20);


  
  
  // draw
  OfflineDrawButton8 = new TGTextButton( parFrame, "&Draw", OFFLINEDRAW8);
  OfflineDrawButton8->SetEnabled(0);
  OfflineDrawButton8->Associate(this);
  parFrame->AddFrame(OfflineDrawButton8, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 30, 0, 0));

  // ch
  offlinechnum8 = new TGNumberEntry(parFrame, 0, 2, OFFLINECHNUM8, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, 15);
  parFrame->AddFrame(offlinechnum8, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 10, 0, 0));
  offlinechnum8->SetButtonToNum(0);
  offlinechnum8->Associate(this);
  TGLabel *ch = new TGLabel(parFrame, "Ch:"); 
  parFrame->AddFrame(ch, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 2, 3, 0));
  

  TabPanel->AddFrame(parFrame, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 2, 2, 1, 1));

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGCompositeFrame *adCanvasFrame = new TGCompositeFrame(TabPanel, 800, 800, kHorizontalFrame);
  TGLayoutHints *Hint = new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 1, 1, 1, 1);

  TRootEmbeddedCanvas *adjCanvas = new TRootEmbeddedCanvas("canvas8", adCanvasFrame, 100, 100);

  canvas8 = adjCanvas->GetCanvas();
  adCanvasFrame->AddFrame(adjCanvas, Hint);
  TabPanel->AddFrame(adCanvasFrame, Hint);
}

void Offline::MakeFold9Panel(TGCompositeFrame *TabPanel)
{
  TGCompositeFrame *parFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);

  // // draw
  // OfflineDrawButton6 = new TGTextButton( parFrame, "&Draw", OFFLINEDRAW6);
  // OfflineDrawButton6->SetEnabled(0);
  // OfflineDrawButton6->Associate(this);
  // parFrame->AddFrame(OfflineDrawButton6, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 30, 0, 0));

  //   // ch
  // offlinechnum6 = new TGNumberEntry (parFrame, 0, 2, OFFLINECHNUM6, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, 15);
  // parFrame->AddFrame(offlinechnum6, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 10, 0, 0));
  // offlinechnum6->SetButtonToNum(0);
  // offlinechnum6->Associate(this);
  // TGLabel *ch = new TGLabel( parFrame, "Ch:"); 
  // parFrame->AddFrame(ch, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 2, 3, 0));
  

  TabPanel->AddFrame(parFrame, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 2, 2, 1, 1));

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGCompositeFrame *adCanvasFrame = new TGCompositeFrame(TabPanel, 800, 800, kHorizontalFrame);
  TGLayoutHints *Hint = new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 1, 1, 1, 1);

  TRootEmbeddedCanvas *adjCanvas = new TRootEmbeddedCanvas("canvas9", adCanvasFrame, 100, 100);

  canvas9 = adjCanvas->GetCanvas();
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
		  if(offlinechnum->GetIntNumber() == (OfflineEventInformation[EventHeaderLength*i] & 0xF))//ch
		    {
		      dslider->SetRange(0,((OfflineEventInformation[EventHeaderLength*i+3] & 0x7FFF0000)>>16));//trace length
		      dslider->SetPosition(0,((OfflineEventInformation[EventHeaderLength*i+3] & 0x7FFF0000)>>16));//trace length
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
	    case OFFLINEDRAW7:
	      Panel7Draw();
	      break;
	    case OFFLINEDRAW8:
	      Panel8Draw();
	      break;
	    case OFFLINEDRAW9:
	      Panel9Draw();
	      break;

	    case OFFLINEGAUSFIT4:
	      GausFit4();
	      break;
	    case OFFLINEGAUSFIT6:
	      GausFit6();
	      break;

	    case OFFLINESTOPDRAW5:
	      Panel5StopDraw();
	      break;
	    case OFFLINESTOPDRAW6:
	      Panel6StopDraw();
	      break;	      
	    case OFFLINESTOPDRAW8:
	      Panel8StopDraw();
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
	      OfflineFileStatus->SetText("NOT READ");
	      OfflineDrawButton->SetEnabled(0);
	      OfflineDrawButton2->SetEnabled(0);
	      OfflineDrawButton3->SetEnabled(0);
	      OfflineDrawButton4->SetEnabled(0);
	      OfflineDrawButton5->SetEnabled(0);
	      OfflineDrawButton6->SetEnabled(0);
	      OfflineDrawButton8->SetEnabled(0);
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
	      OfflineFileStatus->SetText("NOT READ");
	      OfflineDrawButton->SetEnabled(0);
	      OfflineDrawButton2->SetEnabled(0);
	      OfflineDrawButton3->SetEnabled(0);
	      OfflineDrawButton4->SetEnabled(0);
	      OfflineDrawButton5->SetEnabled(0);
	      OfflineDrawButton6->SetEnabled(0);
	      OfflineDrawButton8->SetEnabled(0);
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
		  if(offlinechnum->GetIntNumber() == (OfflineEventInformation[EventHeaderLength*i] & 0xF))//ch
		    {
		      dslider->SetRange(0,((OfflineEventInformation[EventHeaderLength*i+3] & 0x7FFF0000)>>16));//trace length
		      dslider->SetPosition(0,((OfflineEventInformation[EventHeaderLength*i+3] & 0x7FFF0000)>>16));//trace length
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
	    case OFFLINECHNUM8:
	      if (parm2 == 0)
		{
		  if (chanNumber8 != 15)
		    {
		      ++chanNumber8;
		      offlinechnum8->SetIntNumber(chanNumber8);
		    }
		}
	      else
		{
		  if (chanNumber8 != 0)
		    {
		      if (--chanNumber8 == 0)
			chanNumber8 = 0;
		      offlinechnum8->SetIntNumber(chanNumber8);
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
	      OfflineFileStatus->SetText("NOT READ");
	      OfflineDrawButton->SetEnabled(0);
	      OfflineDrawButton2->SetEnabled(0);
	      OfflineDrawButton3->SetEnabled(0);
	      OfflineDrawButton4->SetEnabled(0);
	      OfflineDrawButton5->SetEnabled(0);
	      OfflineDrawButton6->SetEnabled(0);
	      OfflineDrawButton8->SetEnabled(0);
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
	      OfflineFileStatus->SetText("NOT READ");
	      OfflineDrawButton->SetEnabled(0);
	      OfflineDrawButton2->SetEnabled(0);
	      OfflineDrawButton3->SetEnabled(0);
	      OfflineDrawButton4->SetEnabled(0);
	      OfflineDrawButton5->SetEnabled(0);
	      OfflineDrawButton6->SetEnabled(0);
	      OfflineDrawButton8->SetEnabled(0);
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
  // unsigned int tracelength;
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
      
      if(EventHeaderLength == BaseEventHeaderLength)
      	{
	  do
	    {
	      fread(&eventdata, 4, 1, ListModeFile);
	      EventInformation[EventHeaderLength*NumEvents] = eventdata;
	      headerlength = (eventdata & 0x1F000) >> 12;// Header length
	      eventlength = (eventdata & 0x7FFE0000) >> 17;// Event length
	      
	      fread(&eventdata, 4, 1, ListModeFile);
	      EventInformation[EventHeaderLength*NumEvents+1] = eventdata;

	      fread(&eventdata, 4, 1, ListModeFile);
	      EventInformation[EventHeaderLength*NumEvents+2] = eventdata;

	      fread(&eventdata, 4, 1, ListModeFile);
	      EventInformation[EventHeaderLength*NumEvents+3] = eventdata;
	      // tracelength = (eventdata & 0x7FFF0000) >> 16;// Trace Length
	  
	      EventInformation[EventHeaderLength*NumEvents+4] = TotalSkippedWords+headerlength;// Trace location

	      // if(eventlength != headerlength + tracelength/2)
	      //   std::cout<<"Data error..."<<std::endl;
	  
	      TotalSkippedWords += eventlength;
	      NumEvents++;
	      fseek(ListModeFile, (eventlength-4)*4, SEEK_CUR);
	    }while(TotalSkippedWords < TotalWords);
	}
      else
	{
	  unsigned int eventdatarawenergysumsandbaseline[4];
	  unsigned int eventdataqdcsums[8];
	  unsigned int eventdataexternaltimestamp[2];
	  int offsetheaderrawenergysumsandbaseline = BaseEventHeaderLength;
	  int offsetheaderqdcsums = BaseEventHeaderLength+4*int(headerrawenergysumsandbaseline->IsOn());
	  int offsetheaderexternaltimestamp = BaseEventHeaderLength+4*int(headerrawenergysumsandbaseline->IsOn())+8*int(headerqdcsums->IsOn());
	  do
	    {
	      fread(&eventdata, 4, 1, ListModeFile);
	      EventInformation[EventHeaderLength*NumEvents] = eventdata;
	      headerlength = (eventdata & 0x1F000) >> 12;// Header length
	      eventlength = (eventdata & 0x7FFE0000) >> 17;// Event length
	      
	      fread(&eventdata, 4, 1, ListModeFile);
	      EventInformation[EventHeaderLength*NumEvents+1] = eventdata;

	      fread(&eventdata, 4, 1, ListModeFile);
	      EventInformation[EventHeaderLength*NumEvents+2] = eventdata;

	      fread(&eventdata, 4, 1, ListModeFile);
	      EventInformation[EventHeaderLength*NumEvents+3] = eventdata;
	  
	      EventInformation[EventHeaderLength*NumEvents+4] = TotalSkippedWords+headerlength;// Trace location

	      switch(int(headerlength))
		{
		case 4:
		  break;
		case 6:
		  fread(&eventdataexternaltimestamp, 4, 2, ListModeFile);
		  if(headerexternaltimestamp->IsOn())
		    {
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderexternaltimestamp] = eventdataexternaltimestamp[0];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderexternaltimestamp+1] = eventdataexternaltimestamp[1];
		    }
		  break;
		case 8:
		  fread(&eventdatarawenergysumsandbaseline, 4, 4, ListModeFile);
		  if(headerrawenergysumsandbaseline->IsOn())
		    { 
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderrawenergysumsandbaseline] = eventdatarawenergysumsandbaseline[0];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderrawenergysumsandbaseline+1] = eventdatarawenergysumsandbaseline[1];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderrawenergysumsandbaseline+2] = eventdatarawenergysumsandbaseline[2];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderrawenergysumsandbaseline+3] = eventdatarawenergysumsandbaseline[3];
		    }
		  break;
		case 10:
		  fread(&eventdatarawenergysumsandbaseline, 4, 4, ListModeFile);
		  if(headerrawenergysumsandbaseline->IsOn())
		    { 
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderrawenergysumsandbaseline] = eventdatarawenergysumsandbaseline[0];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderrawenergysumsandbaseline+1] = eventdatarawenergysumsandbaseline[1];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderrawenergysumsandbaseline+2] = eventdatarawenergysumsandbaseline[2];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderrawenergysumsandbaseline+3] = eventdatarawenergysumsandbaseline[3];
		    }

		  fread(&eventdataexternaltimestamp, 4, 2, ListModeFile);
		  if(headerexternaltimestamp->IsOn())
		    {
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderexternaltimestamp] = eventdataexternaltimestamp[0];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderexternaltimestamp+1] = eventdataexternaltimestamp[1];
		    }		  
		  break;
		case 12:
		  fread(&eventdataqdcsums, 4, 8, ListModeFile);
		  if(headerqdcsums->IsOn())
		    { 
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderqdcsums] = eventdataqdcsums[0];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderqdcsums+1] = eventdataqdcsums[1];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderqdcsums+2] = eventdataqdcsums[2];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderqdcsums+3] = eventdataqdcsums[3];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderqdcsums+4] = eventdataqdcsums[4];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderqdcsums+5] = eventdataqdcsums[5];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderqdcsums+6] = eventdataqdcsums[6];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderqdcsums+7] = eventdataqdcsums[7];
		    }
		  break;
		case 14:
		  fread(&eventdataqdcsums, 4, 8, ListModeFile);
		  if(headerqdcsums->IsOn())
		    {
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderqdcsums] = eventdataqdcsums[0];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderqdcsums+1] = eventdataqdcsums[1];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderqdcsums+2] = eventdataqdcsums[2];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderqdcsums+3] = eventdataqdcsums[3];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderqdcsums+4] = eventdataqdcsums[4];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderqdcsums+5] = eventdataqdcsums[5];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderqdcsums+6] = eventdataqdcsums[6];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderqdcsums+7] = eventdataqdcsums[7];
		    }

		  fread(&eventdataexternaltimestamp, 4, 2, ListModeFile);
		  if(headerexternaltimestamp->IsOn())
		    {
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderexternaltimestamp] = eventdataexternaltimestamp[0];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderexternaltimestamp+1] = eventdataexternaltimestamp[1];
		    }
		  break;
		case 16:
		  fread(&eventdatarawenergysumsandbaseline, 4, 4, ListModeFile);
		  if(headerrawenergysumsandbaseline->IsOn())
		    { 
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderrawenergysumsandbaseline] = eventdatarawenergysumsandbaseline[0];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderrawenergysumsandbaseline+1] = eventdatarawenergysumsandbaseline[1];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderrawenergysumsandbaseline+2] = eventdatarawenergysumsandbaseline[2];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderrawenergysumsandbaseline+3] = eventdatarawenergysumsandbaseline[3];
		    }

		  fread(&eventdataqdcsums, 4, 8, ListModeFile);
		  if(headerqdcsums->IsOn())
		    {
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderqdcsums] = eventdataqdcsums[0];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderqdcsums+1] = eventdataqdcsums[1];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderqdcsums+2] = eventdataqdcsums[2];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderqdcsums+3] = eventdataqdcsums[3];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderqdcsums+4] = eventdataqdcsums[4];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderqdcsums+5] = eventdataqdcsums[5];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderqdcsums+6] = eventdataqdcsums[6];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderqdcsums+7] = eventdataqdcsums[7];
		    }
		  break;
		case 18:
		  fread(&eventdatarawenergysumsandbaseline, 4, 4, ListModeFile);
		  if(headerrawenergysumsandbaseline->IsOn())
		    { 
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderrawenergysumsandbaseline] = eventdatarawenergysumsandbaseline[0];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderrawenergysumsandbaseline+1] = eventdatarawenergysumsandbaseline[1];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderrawenergysumsandbaseline+2] = eventdatarawenergysumsandbaseline[2];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderrawenergysumsandbaseline+3] = eventdatarawenergysumsandbaseline[3];
		    }

		  fread(&eventdataqdcsums, 4, 8, ListModeFile);
		  if(headerqdcsums->IsOn())
		    {
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderqdcsums] = eventdataqdcsums[0];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderqdcsums+1] = eventdataqdcsums[1];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderqdcsums+2] = eventdataqdcsums[2];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderqdcsums+3] = eventdataqdcsums[3];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderqdcsums+4] = eventdataqdcsums[4];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderqdcsums+5] = eventdataqdcsums[5];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderqdcsums+6] = eventdataqdcsums[6];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderqdcsums+7] = eventdataqdcsums[7];
		    }
		  
		  fread(&eventdataexternaltimestamp, 4, 2, ListModeFile);
		  if(headerexternaltimestamp->IsOn())
		    {
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderexternaltimestamp] = eventdataexternaltimestamp[0];
		      EventInformation[EventHeaderLength*NumEvents+offsetheaderexternaltimestamp+1] = eventdataexternaltimestamp[1];
		    }
		  break;
		default:
		  std::cout<<"Header Length Error!"<<std::endl;
		  break;
		}
	      TotalSkippedWords += eventlength;
	      NumEvents++;
	      fseek(ListModeFile, (eventlength-headerlength)*4, SEEK_CUR);
	    }while(TotalSkippedWords < TotalWords);
	}
      fclose(ListModeFile);
    }
}

void Offline::Panel1Draw()
{
  OfflineReadFileButton->SetEnabled(0);
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

  if(offlinecurrentcountentry->GetIntNumber() >= OfflineModuleEventsCount)
    {
      OfflineCurrentCount = 0;
    }
  else
    {
      OfflineCurrentCount = offlinecurrentcountentry->GetIntNumber()-1;
    }
  
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

      if(offlinechnum->GetIntNumber() == (OfflineEventInformation[EventHeaderLength*OfflineCurrentCount] & 0xF)) break;//ch  
    }

  // cout<<"N: "<<OfflineCurrentCount<<endl;
	      
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

  if(chooseslowfilterbaseline->GetSelected() == 1)//如果没有baseline数据，则采用计算基线的方法
    {
      int tempheaderlength = ((OfflineEventInformation[EventHeaderLength*OfflineCurrentCount] & 0x1F000) >> 12);
      if(!(headerrawenergysumsandbaseline->IsOn() && (tempheaderlength == 8 || tempheaderlength == 10 || tempheaderlength == 16 || tempheaderlength == 18)))
	{
	  chooseslowfilterbaseline->Select(0);
	}
    }
  
  fClient->GetColorByName("blue", color);
  OfflineCurrentCountText->SetTextColor(color, false);
  char stacurr[128];
  sprintf(stacurr,"/ %d",OfflineModuleEventsCount);
  OfflineCurrentCountText->SetText(stacurr);
  offlinecurrentcountentry->SetIntNumber(Long_t(OfflineCurrentCount+1));
  
  tracelength = ((OfflineEventInformation[EventHeaderLength*OfflineCurrentCount+3] & 0x7FFF0000)>>16);//trace length  
  RcdTrace = new unsigned short[tracelength];
  doublethresh = new double[tracelength];
  doublecfdthresh = new double[tracelength];
  doublesample = new double[tracelength];
  doublercdtrace = new double[tracelength];
  doublefastfilter = new double[tracelength];
  doublecfd = new double[tracelength];
  doubleslowfilter = new double[tracelength];

  int retval;
  retval = Pixie16ComputeFastFiltersOffline(offlinefilename, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short) offlinechnum->GetIntNumber(), OfflineEventInformation[EventHeaderLength*OfflineCurrentCount+4],tracelength, RcdTrace, doublefastfilter, doublecfd );//trace length/trace location
  if(retval < 0) ErrorInfo("Offline.cc", "Panel1Draw()", "Pixie16ComputeFastFiltersOffline", retval);

  if(chooseslowfilterbaseline->GetSelected() == 0)
    {
      retval = Pixie16ComputeSlowFiltersOffline(offlinefilename, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short) offlinechnum->GetIntNumber(), OfflineEventInformation[EventHeaderLength*OfflineCurrentCount+4], tracelength, RcdTrace,doubleslowfilter );//trace length/trace location
      if(retval < 0) ErrorInfo("Offline.cc", "Panel1Draw()", "Pixie16ComputeSlowFiltersOffline", retval);
    }
  else
    {
      retval = HongyiWuPixie16ComputeSlowFiltersOffline(offlinefilename, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short) offlinechnum->GetIntNumber(), OfflineEventInformation[EventHeaderLength*OfflineCurrentCount+4], tracelength, RcdTrace,doubleslowfilter ,OfflineEventInformation[EventHeaderLength*OfflineCurrentCount+8],oldslowfilterparameter[0]->GetNumber(),oldslowfilterparameter[1]->GetNumber(),200);//trace length/trace location
      if(retval < 0) ErrorInfo("Offline.cc", "Panel1Draw()", "Pixie16ComputeSlowFiltersOffline", retval);

    }
  

  
  double ChanParData;
  retval = Pixie16ReadSglChanPar((char*)"TRIGGER_THRESHOLD", &ChanParData, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short) offlinechnum->GetIntNumber());
  if(retval < 0) ErrorInfo("Offline.cc", "Panel1Draw()", "Pixie16ReadSglChanPar/TRIGGER_THRESHOLD", retval);
  doublethresh[0] = ChanParData;
  retval = Pixie16ReadSglChanPar((char*)"CFDThresh", &ChanParData, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short) offlinechnum->GetIntNumber());
  if(retval < 0) ErrorInfo("Offline.cc", "Panel1Draw()", "Pixie16ReadSglChanPar/CFDThresh", retval);
  doublecfdthresh[0] = ChanParData;

  for (int i = 0; i < tracelength; ++i)
    {
      doublesample[i] = i;
      doublercdtrace[i] = (double)RcdTrace[i];
      doublethresh[i] = doublethresh[0];
      doublecfdthresh[i] = doublecfdthresh[0];
    }
  rawdata = new TGraph(tracelength,doublesample,doublercdtrace);
  threshdata = new TGraph(tracelength,doublesample,doublethresh);
  cfddata = new TGraph(tracelength,doublesample,doublecfd);
  cfdthreshdata = new TGraph(tracelength,doublesample,doublecfdthresh);
  sfilterdata = new TGraph(tracelength,doublesample,doubleslowfilter);
  ffilterdata  = new TGraph(tracelength,doublesample,doublefastfilter);

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
  // offlinemultigraph->Clear();
  if(offlinedrawoption1[0]->IsOn())
    offlinemultigraph->Add(rawdata);
  if(offlinedrawoption1[3]->IsOn())
    offlinemultigraph->Add(threshdata);
  if(offlinedrawoption1[4]->IsOn())
    offlinemultigraph->Add(cfddata);
  if(offlinedrawoption1[5]->IsOn())
    offlinemultigraph->Add(cfdthreshdata);
  if(offlinedrawoption1[1]->IsOn())
    offlinemultigraph->Add(sfilterdata);
  if(offlinedrawoption1[2]->IsOn())
    offlinemultigraph->Add(ffilterdata);
  offlinemultigraph->Draw("AL");
  if(offlinedrawoption1[0]->IsOn() || offlinedrawoption1[1]->IsOn() || offlinedrawoption1[2]->IsOn() || offlinedrawoption1[3]->IsOn() || offlinedrawoption1[4]->IsOn() || offlinedrawoption1[5]->IsOn())
    offlinemultigraph->GetXaxis()->SetRangeUser(double(dslider->GetMinPosition()),double(dslider->GetMaxPosition()));
  // offlinemultigraph->Draw("AL");
  adjustCanvas->Modified();
  adjustCanvas->Update();
  OfflineReadFileButton->SetEnabled(1);
  gSystem->ProcessEvents();
}

void Offline::Panel2Draw()
{
  OfflineReadFileButton->SetEnabled(0);
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

	  if(i == (OfflineEventInformation[EventHeaderLength*OfflineCurrentCount2[i]] & 0xF)) break;//ch  
	}

      // cout<<"Ch: "<<i<<"  N: "<<OfflineCurrentCount2[i]<<endl;
      int retval;
      if(!offlinedatastatus2[i])
	{
	  tracelength2[i] = ((OfflineEventInformation[EventHeaderLength*OfflineCurrentCount2[i]+3] & 0x7FFF0000)>>16);//trace length  
	  RcdTrace2[i] = new unsigned short[tracelength2[i]];
	  doublethresh2[i] = new double[tracelength2[i]];
	  doublecfdthresh2[i] = new double[tracelength2[i]];
	  doublesample2[i] = new double[tracelength2[i]];
	  doublercdtrace2[i] = new double[tracelength2[i]];
	  doublefastfilter2[i] = new double[tracelength2[i]];
	  doublecfd2[i] = new double[tracelength2[i]];
	  doubleslowfilter2[i] = new double[tracelength2[i]];

	  retval = Pixie16ComputeFastFiltersOffline(offlinefilename, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short)i, OfflineEventInformation[EventHeaderLength*OfflineCurrentCount2[i]+4], tracelength2[i], RcdTrace2[i], doublefastfilter2[i], doublecfd2[i]);//trace location
	  if(retval < 0) ErrorInfo("Offline.cc", "Panel2Draw()", "Pixie16ComputeFastFiltersOffline", retval);
	  retval = Pixie16ComputeSlowFiltersOffline(offlinefilename, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short)i, OfflineEventInformation[EventHeaderLength*OfflineCurrentCount2[i]+4], tracelength2[i], RcdTrace2[i],doubleslowfilter2[i]);//trace localtion
	  if(retval < 0) ErrorInfo("Offline.cc", "Panel2Draw()", "Pixie16ComputeSlowFiltersOffline", retval);
	  
	  double ChanParData;
	  retval = Pixie16ReadSglChanPar((char*)"TRIGGER_THRESHOLD", &ChanParData, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short)i);
	  if(retval < 0) ErrorInfo("Offline.cc", "Panel2Draw()", "Pixie16ReadSglChanPar/TRIGGER_THRESHOLD", retval);
	  doublethresh2[i][0] = ChanParData;
	  retval = Pixie16ReadSglChanPar((char*)"CFDThresh", &ChanParData, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short)i);
	  if(retval < 0) ErrorInfo("Offline.cc", "Panel2Draw()", "Pixie16ReadSglChanPar/CFDThresh", retval);
	  doublecfdthresh2[i][0] = ChanParData;

	  
	  for (int j = 0; j < tracelength2[i]; ++j)
	    {
	      doublesample2[i][j] = j;
	      doublercdtrace2[i][j] = (double)RcdTrace2[i][j];
	      doublethresh2[i][j] = doublethresh2[i][0];
	      doublecfdthresh2[i][j] = doublecfdthresh2[i][0];
	    }
	  rawdata2[i] = new TGraph(tracelength2[i],doublesample2[i],doublercdtrace2[i]);
	  threshdata2[i] = new TGraph(tracelength2[i],doublesample2[i],doublethresh2[i]);
	  cfdthreshdata2[i] = new TGraph(tracelength2[i],doublesample2[i],doublecfdthresh2[i]);
	  cfddata2[i] = new TGraph(tracelength2[i],doublesample2[i],doublecfd2[i]);
	  sfilterdata2[i] = new TGraph(tracelength2[i],doublesample2[i],doubleslowfilter2[i]);
	  ffilterdata2[i] = new TGraph(tracelength2[i],doublesample2[i],doublefastfilter2[i]);

	  
	  cfddata2[i]->SetLineColor(2);
	  cfdthreshdata2[i]->SetLineColor(2);
	  sfilterdata2[i]->SetLineColor(3);
	  ffilterdata2[i]->SetLineColor(4);
	  threshdata2[i]->SetLineColor(4);
	  offlinemultigraph2[i]->Clear();
	  if(offlinedrawoption2[0]->IsOn())
	    offlinemultigraph2[i]->Add(rawdata2[i]);
	  if(offlinedrawoption2[3]->IsOn())
	  offlinemultigraph2[i]->Add(threshdata2[i]);
	  if(offlinedrawoption2[5]->IsOn())
	    offlinemultigraph2[i]->Add(cfdthreshdata2[i]);
	  if(offlinedrawoption2[4]->IsOn())
	    offlinemultigraph2[i]->Add(cfddata2[i]);
	  if(offlinedrawoption2[1]->IsOn())
	    offlinemultigraph2[i]->Add(sfilterdata2[i]);
	  if(offlinedrawoption2[2]->IsOn())
	    offlinemultigraph2[i]->Add(ffilterdata2[i]);
	  canvas2->cd(1+i);
	  offlinemultigraph2[i]->SetTitle(TString::Format("Event: %d",OfflineCurrentCount2[i]).Data());
	  // std::cout<<TString::Format("%d",OfflineCurrentCount2[i]).Data()<<std::endl;
	  offlinemultigraph2[i]->Draw("AL");
	}
    }

  canvas2->Modified();
  canvas2->Update();
  OfflineReadFileButton->SetEnabled(1);
  gSystem->ProcessEvents();
}


void Offline::Panel3Draw()
{
  OfflineReadFileButton->SetEnabled(0);
  canvas3->cd();
  canvas3->Clear();
  canvas3->Divide(4,4);

  for (int i = 0; i < 16; ++i)
    {
      traceoutofrangecount3[i] = 0;
      channeleventcount3[i] = 0;
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
      channeleventcount3[(OfflineEventInformation[EventHeaderLength*i] & 0xF)]++;
      if(((OfflineEventInformation[EventHeaderLength*i+3] & 0x80000000) >> 31) == 1)//out of range
	{
	  traceoutofrangecount3[(OfflineEventInformation[EventHeaderLength*i] & 0xF)]++;
	}
      else
	{
	  offlineth1d3[(OfflineEventInformation[EventHeaderLength*i] & 0xF)]->Fill((OfflineEventInformation[EventHeaderLength*i+3] & 0xFFFF));//ch/event energy
	}
    }

  for (int i = 0; i < 16; ++i)
    {
      if(offlineth1d3[i]->GetEntries() > 0)
	{
	  canvas3->cd(i+1);
	  offlineth1d3[i]->Draw();
	  offlineth1d3[i]->SetTitle(TString::Format("Ch: %d   Trae out-of-range: %d / %d",i,traceoutofrangecount3[i],channeleventcount3[i]).Data());
	  offlineth1d3[i]->GetXaxis()->SetTitle("ch");
	  offlineth1d3[i]->GetXaxis()->SetLabelSize(0.06);
	  offlineth1d3[i]->GetYaxis()->SetLabelSize(0.06);
	}
    }
    
  canvas3->Modified();
  canvas3->Update();
  OfflineReadFileButton->SetEnabled(1);
  gSystem->ProcessEvents();
}

void Offline::Panel4Draw()
{
  OfflineReadFileButton->SetEnabled(0);
  GausFitButton4->SetEnabled(0);
  GausFitButton4->SetText("Open  Fit");
  canvas4->DeleteExec("dynamicPanel4GausFit");
  falggausfit4 = false;
  canvas4->cd();
  canvas4->Clear();
  canvas4->Modified();
  canvas4->Update();
  gPad->SetUniqueID(0);
  gSystem->ProcessEvents();

  
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
      if(offlinechnum4->GetIntNumber() == (OfflineEventInformation[EventHeaderLength*i] & 0xF))//ch
	{
	  offlineth1d4->Fill((OfflineEventInformation[EventHeaderLength*i+3] & 0xFFFF));//event energy
	}
    }
  
  offlineth1d4->Draw();
  offlineth1d4->SetTitle(TString::Format("Ch: %d",int(offlinechnum4->GetIntNumber())).Data());
  offlineth1d4->GetXaxis()->SetTitle("ch");

  canvas4->Modified();
  canvas4->Update();
  GausFitButton4->SetEnabled(1);
  OfflineReadFileButton->SetEnabled(1);
  gSystem->ProcessEvents();
}

void Offline::Panel5StopDraw()
{
  flagdrawstop5 = true;
}

void Offline::Panel5Draw()
{
  OfflineReadFileButton->SetEnabled(0);
  OfflineDrawButton5->SetEnabled(0);
  OfflineStopButton5->SetEnabled(1);
  showprojectyFF5->SetEnabled(0);
  showprojectyCFD5->SetEnabled(0);
  originalcfd5->SetEnabled(0);
  calculatecfd5->SetEnabled(0);
  flagdrawstop5 = false;
  falgshowprojectyFF5 = false;
  flagshowprojectyCFD5 = false;
  
  if(RcdTrace5 != NULL)
    {
      delete []RcdTrace5;
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
  if(fitoriginalcfdth1d5 != NULL)
    {
      delete fitoriginalcfdth1d5;
      fitoriginalcfdth1d5 = NULL;
    }
  if(fitcalculatecfdth1d5 != NULL)
    {
      delete fitcalculatecfdth1d5;
      fitcalculatecfdth1d5 = NULL;
    }
  if(ltxoriginalcfdth1d5 != NULL)
    {
      delete ltxoriginalcfdth1d5;
      ltxoriginalcfdth1d5 = NULL;
    }
  if(ltxcalculatecfdth1d5 != NULL)
    {
      delete ltxcalculatecfdth1d5;
      ltxcalculatecfdth1d5 = NULL;
    }
  
  canvas5->cd();
  canvas5->Clear();
  canvas5->Modified();
  canvas5->Update();
  gSystem->ProcessEvents();
  canvas5->Divide(2,1);

  
  if(OriginalCFDcanvas5 != NULL)
    {
      TCanvas *c = ((TCanvas *)(gROOT->GetListOfCanvases()->FindObject("OriginalCFDcanvas5")));
      if(c) delete OriginalCFDcanvas5;
      OriginalCFDcanvas5 = NULL;
    }
  if(CalculateCFDcanvas5 != NULL)
    {
      TCanvas *c = ((TCanvas *)(gROOT->GetListOfCanvases()->FindObject("CalculateCFDcanvas5"))); 
      if(c) delete CalculateCFDcanvas5;
      CalculateCFDcanvas5 = NULL;
    }
  
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
      if(offlinechnum5->GetIntNumber() == (OfflineEventInformation[EventHeaderLength*i] & 0xF))//ch
	{
	  inttracelength = ((OfflineEventInformation[EventHeaderLength*i+3] & 0x7FFF0000)>>16);//trace length
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

      originalcfdth1d5 = new TH1D("originalcfdth1d5","",330,0,33000);//32768
      calculatecfdth1d5 = new TH1D("calculatecfdth1d5","",330,0,33000);//32768

      fitoriginalcfdth1d5 = new TF1("fitoriginalcfdth1d5","pol1");
      fitcalculatecfdth1d5 = new TF1("fitcalculatecfdth1d5","pol1");
      ltxoriginalcfdth1d5 = new TLatex();
      ltxoriginalcfdth1d5->SetNDC(kTRUE);
      ltxoriginalcfdth1d5->SetTextFont(22);
      ltxoriginalcfdth1d5->SetTextSize(0.06);
      ltxoriginalcfdth1d5->SetTextColor(4);
      ltxcalculatecfdth1d5 = new TLatex();
      ltxcalculatecfdth1d5 ->SetNDC(kTRUE);
      ltxcalculatecfdth1d5 ->SetTextFont(22);
      ltxcalculatecfdth1d5 ->SetTextSize(0.06);
      ltxcalculatecfdth1d5 ->SetTextColor(4);

      
      int retval;
      cfdevenycount5 = 0;
      originalcfdvalidcount5 = 0;
      calculatecfdvalidcount5 = 0;
      double ChanCFDThre;
      retval = Pixie16ReadSglChanPar((char*)"CFDThresh", &ChanCFDThre, offlinemodnum->GetIntNumber(), offlinechnum5->GetIntNumber());
      if(retval < 0) ErrorInfo("Offline.cc", "Panel5Draw()", "Pixie16ReadSglChanPar/CFDThresh", retval);
  
      for (unsigned int i = 0; i < OfflineModuleEventsCount; ++i)
	{
	  if(offlinechnum5->GetIntNumber() == (OfflineEventInformation[EventHeaderLength*i] & 0xF))//ch
	    {
	      cfdevenycount5++;
	      if(((OfflineEventInformation[EventHeaderLength*i+2] & 0x80000000) >> 31) == 0) originalcfdvalidcount5++;
	      
	      retval = Pixie16ComputeFastFiltersOffline(offlinefilename, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short) offlinechnum5->GetIntNumber(), OfflineEventInformation[EventHeaderLength*i+4], ((OfflineEventInformation[EventHeaderLength*i+3] & 0x7FFF0000)>>16), RcdTrace5, doublefastfilter5, doublecfd5);//trace location/trace length
	      if(retval < 0) ErrorInfo("Offline.cc", "Panel5Draw()", "Pixie16ComputeFastFiltersOffline", retval);
	      int intmaxcfd = -1;
	      double doublemaxcfd = -1;
	      for (int j = 0; j < inttracelength; ++j)
		{
		  offlineth2d5_0->Fill(j,doublefastfilter5[j]);
		  offlineth2d5_1->Fill(j,doublecfd5[j]);

		  if(doublecfd5[j] > doublemaxcfd)
		    {
		      doublemaxcfd = doublecfd5[j];
		      intmaxcfd = j;
		    }
		}
	      if(doublecfd5[intmaxcfd] >= ChanCFDThre)
		{
		  calculatecfdvalidcount5++;
		  for (int j = intmaxcfd; j < inttracelength; ++j)
		    {
		      if(doublecfd5[j]>= 0 && doublecfd5[j+1] < 0)
			{
			  calculatecfdth1d5->Fill(doublecfd5[j]/(doublecfd5[j]-doublecfd5[j+1])*32768);
			  break;
			}
		    }
		}
	      
	      originalcfdth1d5->Fill(((OfflineEventInformation[EventHeaderLength*i+2] & 0x7FFF0000)>>16));//cfd
	    }

	  if(i%500 == 0)
	    {
	      if(flagdrawstop5) break;
	      printtextinfor5->SetText(TString::Format("Drawing...please wait a moment. ==> %d/%d",i,OfflineModuleEventsCount).Data());
	      gSystem->ProcessEvents();
	    }
	}

      originalcfdth1d5->SetTitle(TString::Format("Vaild: %d / %d",originalcfdvalidcount5,cfdevenycount5).Data());
      calculatecfdth1d5->SetTitle(TString::Format("Vaild: %d / %d",calculatecfdvalidcount5,cfdevenycount5).Data());
  
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
      originalcfd5->SetEnabled(1);
      calculatecfd5->SetEnabled(1);
    }

  printtextinfor5->SetText("Draw Done!");
  canvas5->Modified();
  canvas5->Update();
  OfflineDrawButton5->SetEnabled(1);
  OfflineStopButton5->SetEnabled(0);
  OfflineReadFileButton->SetEnabled(1);
  gSystem->ProcessEvents();
}

void Offline::Panel6StopDraw()
{
  flagdrawstop6 = true;
}

void Offline::Panel6Draw()
{
  OfflineReadFileButton->SetEnabled(0);
  GausFitButton6->SetEnabled(0);
  GausFitButton6->SetText("Open  Fit");
  canvas6->DeleteExec("dynamicPanel6GausFit");
  falggausfit6 = false;
  
  OfflineDrawButton6->SetEnabled(0);
  OfflineStopButton6->SetEnabled(1);
  flagdrawstop6 = false;
  canvas6->cd();
  canvas6->Clear();
  canvas6->Modified();
  canvas6->Update();
  gPad->SetUniqueID(0);
  gSystem->ProcessEvents();
  
  if(RcdTrace6 != NULL)
    {
      delete []RcdTrace6;
      RcdTrace6 = NULL;
    }
  if(doublefastfilter6 != NULL)
    {
      delete []doublefastfilter6;
      doublefastfilter6 = NULL;
    }
  if(doublecfd6 != NULL)
    {
      delete []doublecfd6;
      doublecfd6 = NULL;
    }
  if(doubleslowfilter6 != NULL)
    {
      delete []doubleslowfilter6;
      doubleslowfilter6 = NULL;
    }
  if(offlineth1d6 != NULL)
    {
      delete offlineth1d6;
      offlineth1d6 = NULL;
    }

  int intth1dbin6;
  switch(chooseth1dbin6->GetSelected())
    {
    case 1 :
      intth1dbin6 = 65536;
      break;
    case 2 :
      intth1dbin6 = 32768;
      break;
    case 3 :
      intth1dbin6 = 16384;
      break;
    case 4 :
      intth1dbin6 = 8192;
      break;
    case 5 :
      intth1dbin6 = 4096;
      break;
    case 6 :
      intth1dbin6 = 2048;
      break;
    case 7 :
      intth1dbin6 = 1024;
      break;
    default:
      intth1dbin6 = 65536;
      break;
    }

  int inttracelength = -1;
  for (unsigned int i = 0; i < OfflineModuleEventsCount; ++i)
    {
      if(offlinechnum6->GetIntNumber() == (OfflineEventInformation[EventHeaderLength*i] & 0xF))//ch
	{
	  inttracelength = ((OfflineEventInformation[EventHeaderLength*i+3] & 0x7FFF0000)>>16);//trace length
	  break;
	}
    }

  if(inttracelength > -1)
    {
      offlineth1d6 = new TH1D("offlineth1d6","",intth1dbin6,0,65536);
      RcdTrace6 = new unsigned short[inttracelength];
      doublefastfilter6 = new double[inttracelength];
      doublecfd6 = new double[inttracelength];
      doubleslowfilter6 = new double[inttracelength];
      
      int retval;
      double ChanParTrig;
      unsigned int SlowLen, SlowGap, SlowFilterRange;
      double ChanParData;
      retval = Pixie16ReadSglChanPar((char*)"TRIGGER_THRESHOLD", &ChanParTrig, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short)offlinechnum6->GetIntNumber());
      if(retval < 0) ErrorInfo("Offline.cc", "Panel6Draw()", "Pixie16ReadSglChanPar/TRIGGER_THRESHOLD", retval);

      retval = Pixie16ReadSglModPar((char*)"SLOW_FILTER_RANGE", &SlowFilterRange, (unsigned short)offlinemodnum->GetIntNumber());
      if(retval < 0) ErrorInfo("Offline.cc", "Panel6Draw()", "Pixie16ReadSglModPar/SLOW_FILTER_RANGE", retval);
      
      retval = Pixie16ReadSglChanPar((char*)"ENERGY_RISETIME", &ChanParData, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short)offlinechnum6->GetIntNumber());
      if(retval < 0) ErrorInfo("Offline.cc", "Panel6Draw()", "Pixie16ReadSglChanPar/ENERGY_RISETIME", retval);
      switch(choosesamplemhz0->GetSelected())
	{
	case 1 ://100
	  SlowLen = ROUND(ChanParData*100/std::pow(2.0,(double)SlowFilterRange))*std::pow(2.0,(double)SlowFilterRange);
	  break;
	case 2 ://250
	  SlowLen = ROUND(ChanParData*125/std::pow(2.0,(double)SlowFilterRange))*std::pow(2.0,(double)SlowFilterRange);
	  break;	  
	case 3 ://500
	  SlowLen = ROUND(ChanParData*100/std::pow(2.0,(double)SlowFilterRange))*std::pow(2.0,(double)SlowFilterRange);
	  break;
	default:
	  std::cout<<"ERROR: Please call Hongyi Wu(wuhongyi@qq.com)"<<std::endl;
	  break;
	}
      
      retval = Pixie16ReadSglChanPar((char*)"ENERGY_FLATTOP", &ChanParData, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short)offlinechnum6->GetIntNumber());
      if(retval < 0) ErrorInfo("Offline.cc", "Panel6Draw()", "Pixie16ReadSglChanPar/ENERGY_FLATTOP", retval);
      switch(choosesamplemhz0->GetSelected())
	{
	case 1 ://100
	  SlowGap = ROUND(ChanParData*100/std::pow(2.0,(double)SlowFilterRange))*std::pow(2.0,(double)SlowFilterRange);
	  break;
	case 2 ://250
	  SlowGap = ROUND(ChanParData*125/std::pow(2.0,(double)SlowFilterRange))*std::pow(2.0,(double)SlowFilterRange);
	  break;	  
	case 3 ://500
	  SlowGap = ROUND(ChanParData*100/std::pow(2.0,(double)SlowFilterRange))*std::pow(2.0,(double)SlowFilterRange);
	  break;
	default:
	  break;
	}

      int intflagenergy;
      switch(SlowFilterRange)
	{
	case 1:
	  intflagenergy = SlowLen+SlowGap-3;
	  break;
	case 2:
	  intflagenergy = SlowLen+SlowGap-2;
	  break;
	case 3:
	  intflagenergy = SlowLen+SlowGap-2;
	  break;
	case 4:
	  intflagenergy = SlowLen+SlowGap-1;
	  break;
	case 5:
	  intflagenergy = SlowLen+SlowGap;
	  break;
	case 6:
	  intflagenergy = SlowLen+SlowGap+1;
	  break;
	default:
	  intflagenergy = SlowLen+SlowGap-2;
	  break;
	}
  
      for (unsigned int i = 0; i < OfflineModuleEventsCount; ++i)
	{
	  if(offlinechnum6->GetIntNumber() == (OfflineEventInformation[EventHeaderLength*i] & 0xF))//ch
	    {
	      retval = Pixie16ComputeFastFiltersOffline(offlinefilename, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short) offlinechnum6->GetIntNumber(), OfflineEventInformation[EventHeaderLength*i+4], ((OfflineEventInformation[EventHeaderLength*i+3] & 0x7FFF0000)>>16), RcdTrace6, doublefastfilter6, doublecfd6);//trace location/trace length
	      if(retval < 0) ErrorInfo("Offline.cc", "Panel6Draw()", "Pixie16ComputeFastFiltersOffline", retval);
	      retval = Pixie16ComputeSlowFiltersOffline(offlinefilename, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short) offlinechnum6->GetIntNumber(), OfflineEventInformation[EventHeaderLength*i+4], ((OfflineEventInformation[EventHeaderLength*i+3] & 0x7FFF0000)>>16), RcdTrace6,doubleslowfilter6);//trace location/trace length
	      if(retval < 0) ErrorInfo("Offline.cc", "Panel6Draw()", "Pixie16ComputeSlowFiltersOffline", retval);

	      
	      int intflagtrigger = -1;
	      for (int j = 0; j < inttracelength; ++j)
		{
		  if(doublefastfilter6[j] >= ChanParTrig)
		    {
		      intflagtrigger = j;
		      break;
		    }
		  if(j == inttracelength-1) intflagtrigger = -1;
		}
	      if(intflagtrigger == -1) continue;
	      
	      intflagtrigger += intflagenergy;
	      offlineth1d6->Fill(doubleslowfilter6[intflagtrigger]);
	    }

	  if(i%500 == 0)
	    {
	      if(flagdrawstop6) break;
	      printtextinfor6->SetText(TString::Format("Drawing...please wait a moment. ==> %d/%d",i,OfflineModuleEventsCount).Data());
	      gSystem->ProcessEvents();
	    }
	}

      
      offlineth1d6->Draw();
      offlineth1d6->SetTitle(TString::Format("Ch: %d",int(offlinechnum6->GetIntNumber())).Data());
      offlineth1d6->GetXaxis()->SetTitle("ch");
    }

  printtextinfor6->SetText("Draw Done!");
  canvas6->Modified();
  canvas6->Update();
  GausFitButton6->SetEnabled(1);
  OfflineReadFileButton->SetEnabled(1);
  gSystem->ProcessEvents();
  OfflineDrawButton6->SetEnabled(1);
  OfflineStopButton6->SetEnabled(0);
}

void Offline::Panel7Draw()
{
  OfflineReadFileButton->SetEnabled(0);

  
  OfflineReadFileButton->SetEnabled(1);
}

void Offline::Panel8StopDraw()
{
  flagdrawstop8 = true;
}

void Offline::Panel8Draw()
{
  OfflineReadFileButton->SetEnabled(0);
  OfflineDrawButton8->SetEnabled(0);
  OfflineStopButton8->SetEnabled(1);
  flagdrawstop8 = false;

  countenergyff8[0] = 0;
  countenergyff8[1] = 0;

  if(choosedrawstyle8->GetSelected() == 0)
    {
      //Graph
      if(energyfffirst8 != NULL)
	{
	  delete energyfffirst8;
	  energyfffirst8 = NULL;
	}
      if(energyffsecond8 != NULL)
	{
	  delete energyffsecond8;
	  energyffsecond8 = NULL;
	}
      energyfffirst8 = new TGraph();
      energyffsecond8 = new TGraph();
    }
  else
    {
      //Hist
      if(histenergyfffirst8 != NULL)
	{
	  delete histenergyfffirst8;
	  histenergyfffirst8 = NULL;
	}
      if(histenergyffsecond8 != NULL)
	{
	  delete histenergyffsecond8;
	  histenergyffsecond8 = NULL;
	}
      if(histxyminmax8[0]->GetNumber() >= histxyminmax8[1]->GetNumber())
	{
	  histxyminmax8[0]->SetNumber(0);
	  histxyminmax8[1]->SetNumber(1000);
	}
      if(histxyminmax8[2]->GetNumber() >= histxyminmax8[3]->GetNumber())
	{
	  histxyminmax8[2]->SetNumber(0);
	  histxyminmax8[3]->SetNumber(65536);
	}   
      histenergyfffirst8 = new TH2D("histenergyfffirst8","",choosehistbinxy[0]->GetSelected(),histxyminmax8[0]->GetNumber(),histxyminmax8[1]->GetNumber(),choosehistbinxy[1]->GetSelected(),histxyminmax8[2]->GetNumber(),histxyminmax8[3]->GetNumber());
      histenergyffsecond8 = new TH2D("histenergyffsecond8","",choosehistbinxy[0]->GetSelected(),histxyminmax8[0]->GetNumber(),histxyminmax8[1]->GetNumber(),choosehistbinxy[1]->GetSelected(),histxyminmax8[2]->GetNumber(),histxyminmax8[3]->GetNumber());
    }
  

  if(RcdTrace8 != NULL)
    {
      delete []RcdTrace8;
      RcdTrace8 = NULL;
    }
  if(doublefastfilter8 != NULL)
    {
      delete []doublefastfilter8;
      doublefastfilter8 = NULL;
    }

  canvas8->cd();
  canvas8->Clear();
  canvas8->Modified();
  canvas8->Update();
  gSystem->ProcessEvents();
  canvas8->Divide(2,1);

  int inttracelength = -1;
  for (unsigned int i = 0; i < OfflineModuleEventsCount; ++i)
    {
      if(offlinechnum8->GetIntNumber() == (OfflineEventInformation[EventHeaderLength*i] & 0xF))//ch
	{
	  inttracelength = ((OfflineEventInformation[EventHeaderLength*i+3] & 0x7FFF0000)>>16);//trace length
	  break;
	}
    }

  if(inttracelength > -1)
    {
      RcdTrace8 = new unsigned short[inttracelength];
      doublefastfilter8 = new double[inttracelength];

      int retval;
      unsigned int FastLen, FastGap, FastFilterRange;
      double ChanParData;

      retval = Pixie16ReadSglModPar((char*)"FAST_FILTER_RANGE", &FastFilterRange, (unsigned short)offlinemodnum->GetIntNumber());
      if(retval < 0) ErrorInfo("Offline.cc", "Panel8Draw()", "Pixie16ReadSglModPar/FAST_FILTER_RANGE", retval);

      retval = Pixie16ReadSglChanPar((char*)"TRIGGER_RISETIME", &ChanParData, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short)offlinechnum8->GetIntNumber());
      if(retval < 0) ErrorInfo("Offline.cc", "Panel8Draw()", "Pixie16ReadSglChanPar/TRIGGER_RISETIME", retval);
      switch(choosesamplemhz0->GetSelected())
	{
	case 1 ://100
	  FastLen = ROUND(ChanParData*100/std::pow(2.0,(double)FastFilterRange))*std::pow(2.0,(double)FastFilterRange);
	  break;
	case 2 ://250
	  FastLen = ROUND(ChanParData*125/std::pow(2.0,(double)FastFilterRange))*std::pow(2.0,(double)FastFilterRange);
	  break;	  
	case 3 ://500
	  FastLen = ROUND(ChanParData*100/std::pow(2.0,(double)FastFilterRange))*std::pow(2.0,(double)FastFilterRange);
	  break;
	default:
	  std::cout<<"ERROR: Please call Hongyi Wu(wuhongyi@qq.com)"<<std::endl;
	  break;
	}

      retval = Pixie16ReadSglChanPar((char*)"TRIGGER_FLATTOP", &ChanParData, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short)offlinechnum8->GetIntNumber());
      if(retval < 0) ErrorInfo("Offline.cc", "Panel8Draw()", "Pixie16ReadSglChanPar/TRIGGER_FLATTOP", retval);
      switch(choosesamplemhz0->GetSelected())
	{
	case 1 ://100
	  FastGap = ROUND(ChanParData*100/std::pow(2.0,(double)FastFilterRange))*std::pow(2.0,(double)FastFilterRange);
	  break;
	case 2 ://250
	  FastGap = ROUND(ChanParData*125/std::pow(2.0,(double)FastFilterRange))*std::pow(2.0,(double)FastFilterRange);
	  break;	  
	case 3 ://500
	  FastGap = ROUND(ChanParData*100/std::pow(2.0,(double)FastFilterRange))*std::pow(2.0,(double)FastFilterRange);
	  break;
	default:
	  std::cout<<"ERROR: Please call Hongyi Wu(wuhongyi@qq.com)"<<std::endl;
	  break;
	}

      FILE *ListModeFile = NULL;
      unsigned int fsum0, fsum1;
      unsigned int offset, x, y;
      for (unsigned int i = 0; i < OfflineModuleEventsCount; ++i)
	{
	  if(offlinechnum6->GetIntNumber() == (OfflineEventInformation[EventHeaderLength*i] & 0xF))//ch
	    {
	      ListModeFile = fopen(offlinefilename, "rb");
	      if(ListModeFile != NULL)
		{
		  fseek(ListModeFile, OfflineEventInformation[EventHeaderLength*i+4]*4, SEEK_SET);// Position ListModeFile to the requested trace location
		  fread(RcdTrace8, 2, ((OfflineEventInformation[EventHeaderLength*i+3] & 0x7FFF0000)>>16), ListModeFile);// Read trace
		  fclose(ListModeFile);// Close file
		  ListModeFile = NULL;
		
		  // Compute fast filter response
		  offset = 2*FastLen + FastGap - 1;
		  for(x = offset; x < ((OfflineEventInformation[EventHeaderLength*i+3] & 0x7FFF0000)>>16); x++)
		    {
		      fsum0 = 0;
		      for(y = (x-offset); y < (x-offset+FastLen); y++)
			{
			  fsum0 += RcdTrace8[y];
			}
		      fsum1 = 0;
		      for(y = (x-offset+FastLen+FastGap); y < (x-offset+2*FastLen+FastGap); y++)
			{
			  fsum1 += RcdTrace8[y];
			}
		      doublefastfilter8[x] = ((double)fsum1 - (double)fsum0)/(double)FastLen;
		    }

		  for(x = 0; x < offset; x++)
		    {
		      doublefastfilter8[x] = doublefastfilter8[offset];
		    }

		  int flagfffirst = -1;
		  double fffirst = -1;
		  double ffsecond = -1;
		  for (int z = 0; z < (int)((OfflineEventInformation[EventHeaderLength*i+3] & 0x7FFF0000)>>16); ++z)
		    {
		      if(doublefastfilter8[z] > fffirst)
			{
			  fffirst = doublefastfilter8[z];
			  flagfffirst = z;
			}
		    }

		  if(flagfffirst-(FastLen+FastGap)*2 > 0)
		    {
		      for (int z = 0; z < flagfffirst-(int(FastLen+FastGap))*2; ++z)
			{
			  if(doublefastfilter8[z] > ffsecond)
			    {
			      ffsecond = doublefastfilter8[z];
			    }
			}
		    }
		  if(flagfffirst+(FastLen+FastGap)*2 < ((OfflineEventInformation[EventHeaderLength*i+3] & 0x7FFF0000)>>16))
		    {
		      for (int z = flagfffirst+(int(FastLen+FastGap))*2; z < (int)((OfflineEventInformation[EventHeaderLength*i+3] & 0x7FFF0000)>>16); ++z)
			{
			  if(doublefastfilter8[z] > ffsecond)
			    {
			      ffsecond = doublefastfilter8[z];
			    }
			}
		    }

		  if(choosedrawstyle8->GetSelected() == 0)
		    {
		      if(fffirst > 0)
			{
			  energyfffirst8->SetPoint(countenergyff8[0],fffirst,(double(OfflineEventInformation[EventHeaderLength*i+3]&0xFFFF)));
			  countenergyff8[0]++;
			}
		      if(ffsecond > 0)
			{
			  energyffsecond8->SetPoint(countenergyff8[1],ffsecond,(double(OfflineEventInformation[EventHeaderLength*i+3]&0xFFFF)));
			  countenergyff8[1]++;
			}
		    }
		  else
		    {
		      if(fffirst > 0)
			{
			  histenergyfffirst8->Fill(fffirst,(double(OfflineEventInformation[EventHeaderLength*i+3]&0xFFFF)));
			}
		      if(ffsecond > 0)
			{
			  histenergyffsecond8->Fill(ffsecond,(double(OfflineEventInformation[EventHeaderLength*i+3]&0xFFFF)));
			}
		    }
		}//fast filter

	      
	    }//ch

	  if(i%500 == 0)
	    {
	      if(flagdrawstop8) break;
	      printtextinfor8->SetText(TString::Format("Drawing...please wait a moment. ==> %d/%d",i,OfflineModuleEventsCount).Data());
	      gSystem->ProcessEvents();
	    }

	}//for i

      // Draw
      if(choosedrawstyle8->GetSelected() == 0)
	{
	  canvas8->cd(1);
	  energyfffirst8->Draw("AP*");
	  energyfffirst8->GetXaxis()->SetTitle("First Peak FF");
	  energyfffirst8->GetYaxis()->SetTitle("Orig E[ch]");
	  canvas8->cd(2);
	  energyffsecond8->Draw("AP*");
	  energyffsecond8->GetXaxis()->SetTitle("Second Peak FF");
	  energyffsecond8->GetYaxis()->SetTitle("Orig E[ch]");
	}
      else
	{
	  canvas8->cd(1);
	  histenergyfffirst8->Draw("colz");
	  histenergyfffirst8->GetXaxis()->SetTitle("First Peak FF");
	  histenergyfffirst8->GetYaxis()->SetTitle("Orig E[ch]");
	  canvas8->cd(2);
	  histenergyffsecond8->Draw("colz");
	  histenergyffsecond8->GetXaxis()->SetTitle("Second Peak FF");
	  histenergyffsecond8->GetYaxis()->SetTitle("Orig E[ch]");
	}

    }// inttracelength > -1
      

  printtextinfor8->SetText("Draw Done!");
  canvas8->Modified();
  canvas8->Update();
  OfflineDrawButton8->SetEnabled(1);
  OfflineStopButton8->SetEnabled(0);
  OfflineReadFileButton->SetEnabled(1);
  gSystem->ProcessEvents();
}

void Offline::Panel9Draw()
{
  OfflineReadFileButton->SetEnabled(0);

  
  OfflineReadFileButton->SetEnabled(1);
}

void Offline::Panel0ReadFile()
{
  OfflineCurrentCount = -1;
  fClient->GetColorByName("green", color);
  OfflineFileStatus->SetTextColor(color, false);
  OfflineFileStatus->SetText("Waitting...");
  gSystem->ProcessEvents();

  //定义数据长度
  EventHeaderLength = 5;
  if(headerrawenergysumsandbaseline->IsOn())
    {
      EventHeaderLength += 4;
    }
  if(headerqdcsums->IsOn())
    {
      EventHeaderLength += 8;
    }
  if(headerexternaltimestamp->IsOn())
    {
      EventHeaderLength += 2;
    }
  
  sprintf(offlinefilename,"%s%s_R%04d_M%02d.bin",filepathtext->GetText(),filenametext->GetText(),int(offlinefilerunnum->GetIntNumber()),int(offlinemodnum->GetIntNumber()));
  // GOTO need check file 
  OfflineModuleEventsCount = GetModuleEvents(offlinefilename);
  if(OfflineModuleEventsCount > 0)
    {
      if(OfflineEventInformation != NULL)
	{
	  delete []OfflineEventInformation;
	  OfflineEventInformation = NULL;
	}
      OfflineEventInformation = new unsigned int[EventHeaderLength*OfflineModuleEventsCount];
      // std::cout<<"int length size: "<<EventHeaderLength*OfflineModuleEventsCount<<std::endl;
      memset(OfflineEventInformation, 0, sizeof(unsigned int)*OfflineModuleEventsCount*EventHeaderLength);//置零
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
      OfflineDrawButton8->SetEnabled(1);
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
  // offlineth2d5_0->SetShowProjectionY(1);
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  if(falgshowprojectyFF5)
    {
      falgshowprojectyFF5 = false;
      canvas5->cd(1)->DeleteExec("dynamicFFShowProjectY5");
    }
  else
    {
      falgshowprojectyFF5 = true;
      canvas5->cd(1)->AddExec("dynamicFFShowProjectY5","DynamicFFShowProjectY5()");
    }
}

void Offline::CFDShowProjectY5()
{
  // offlineth2d5_1->SetShowProjectionY(1);

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  if(flagshowprojectyCFD5)
    {
      flagshowprojectyCFD5 = false;
      canvas5->cd(2)->DeleteExec("dynamicCFDShowProjectY5");
    }
  else
    {
      flagshowprojectyCFD5 = true;
      canvas5->cd(2)->AddExec("dynamicCFDShowProjectY5","DynamicCFDShowProjectY5()");
    }
}

void Offline::OriginalCFDShow5()
{
  TCanvas *c = ((TCanvas *)(gROOT->GetListOfCanvases()->FindObject("OriginalCFDcanvas5")));
  if(!c) OriginalCFDcanvas5 = new TCanvas("OriginalCFDcanvas5","Original CFD",600,400);
  OriginalCFDcanvas5->cd();
  originalcfdth1d5->Draw();
  originalcfdth1d5->Fit("fitoriginalcfdth1d5", "Q");
  ltxoriginalcfdth1d5->DrawLatex(0.1,0.9,TString::Format("Chi2: %0.2f  NDF: %d  Chi2/NDF: %0.3f",fitoriginalcfdth1d5->GetChisquare(),fitoriginalcfdth1d5->GetNDF(),fitoriginalcfdth1d5->GetChisquare()/fitoriginalcfdth1d5->GetNDF()).Data());
  OriginalCFDcanvas5->Update();
}

void Offline::CalculateCFDShow5()
{
  TCanvas *c = ((TCanvas *)(gROOT->GetListOfCanvases()->FindObject("CalculateCFDcanvas5")));
  if(!c) CalculateCFDcanvas5 = new TCanvas("CalculateCFDcanvas5","Calculate CFD",600,400);
  CalculateCFDcanvas5->cd();
  calculatecfdth1d5->Draw();
  calculatecfdth1d5->Fit("fitcalculatecfdth1d5", "Q");
  ltxcalculatecfdth1d5->DrawLatex(0.1,0.9,TString::Format("Chi2: %0.2f  NDF: %d  Chi2/NDF: %0.3f",fitcalculatecfdth1d5->GetChisquare(),fitcalculatecfdth1d5->GetNDF(),fitcalculatecfdth1d5->GetChisquare()/fitcalculatecfdth1d5->GetNDF()).Data());
  CalculateCFDcanvas5->Update();
}

void Offline::SelectRawEnergySumsBaseline(Bool_t on)
{
  std::cout<<"GOTO  =  SelectRawEnergySumsBaseline: "<<headerrawenergysumsandbaseline->IsOn()<<std::endl;

  OfflineDrawButton->SetEnabled(0);
  OfflineDrawButton2->SetEnabled(0);
  OfflineDrawButton3->SetEnabled(0);
  OfflineDrawButton4->SetEnabled(0);
  OfflineDrawButton5->SetEnabled(0);
  OfflineDrawButton6->SetEnabled(0);
  OfflineDrawButton8->SetEnabled(0);
  // OfflineFileStatus->SetText("NOT READ");
}

void Offline::SelectQDCSums(Bool_t on)
{
  std::cout<<"GOTO  =  SelectQDCSums: "<<headerqdcsums->IsOn()<<std::endl;
  OfflineDrawButton->SetEnabled(0);
  OfflineDrawButton2->SetEnabled(0);
  OfflineDrawButton3->SetEnabled(0);
  OfflineDrawButton4->SetEnabled(0);
  OfflineDrawButton5->SetEnabled(0);
  OfflineDrawButton6->SetEnabled(0);
  OfflineDrawButton8->SetEnabled(0);
  // OfflineFileStatus->SetText("NOT READ");
}

void Offline::SelectExternalTimestamp(Bool_t on)
{
  std::cout<<"GOTO  =  SelectExternalTimestamp: "<<headerexternaltimestamp->IsOn()<<std::endl;
  OfflineDrawButton->SetEnabled(0);
  OfflineDrawButton2->SetEnabled(0);
  OfflineDrawButton3->SetEnabled(0);
  OfflineDrawButton4->SetEnabled(0);
  OfflineDrawButton5->SetEnabled(0);
  OfflineDrawButton6->SetEnabled(0);
  OfflineDrawButton8->SetEnabled(0);
  // OfflineFileStatus->SetText("NOT READ");
}



void DynamicFFShowProjectY5()
{
  TObject *select = gPad->GetSelected();
  if(!select) return;
  if (!select->InheritsFrom(TH2::Class())) {gPad->SetUniqueID(0); return;}
  TH2 *h = (TH2*)select;
  gPad->GetCanvas()->FeedbackMode(kTRUE);

  int pyold = gPad->GetUniqueID();
  int px = gPad->GetEventX();
  // int py = gPad->GetEventY();
  float uymin = gPad->GetUymin();
  float uymax = gPad->GetUymax();
  int pymin = gPad->YtoAbsPixel(uymin);
  int pymax = gPad->YtoAbsPixel(uymax);
  if(pyold) gVirtualX->DrawLine(pyold,pymin,pyold,pymax);
  gVirtualX->DrawLine(px,pymin,px,pymax);
  gPad->SetUniqueID(px);
  Float_t upx = gPad->AbsPixeltoX(px);
  Float_t x = gPad->PadtoX(upx);

  TVirtualPad *padsav = gPad;
  TCanvas *c2 = (TCanvas*)gROOT->GetListOfCanvases()->FindObject("FFShowProjectY5");
  if(c2) delete c2->GetPrimitive("Projection");
  else c2 = new TCanvas("FFShowProjectY5","Fast Filter ShowProjectY",710,10,700,500);
  c2->SetGrid();
  c2->cd();

  Int_t binx = h->GetXaxis()->FindBin(x);
  TH1D *hp = h->ProjectionY("",binx,binx);
  // TH1D *hpp = h->ProjectionY("_pxpx",0,binx);
  hp->SetFillColor(38);
  char title[80];
  sprintf(title,"Projection of bin X = %d",binx);
  hp->SetName("ProjectionFF5");
  hp->SetTitle(title);
  TLatex *ltx = new TLatex();
  if(hp->Fit("gaus","QL") == 0)
    {
      hp->GetFunction("gaus")->SetNpx(1000);
      hp->GetFunction("gaus")->SetLineColor(kRed);
      ltx->SetNDC(kTRUE);
      ltx->SetTextFont(22);
      ltx->SetTextSize(0.06);
      ltx->SetTextColor(4);
      ltx->DrawLatex(0.01,0.9,TString::Format("Mean: %0.3f  Sigma: %0.3f       Rec Thre: %d",hp->GetFunction("gaus")->GetParameter(1),hp->GetFunction("gaus")->GetParameter(2),int(hp->GetFunction("gaus")->GetParameter(1)+3*hp->GetFunction("gaus")->GetParameter(2)+1)).Data());
    }
  // hpp->Draw("same");
  c2->Update();
  padsav->cd();
  delete ltx;
}

void DynamicCFDShowProjectY5()
{
  TObject *select = gPad->GetSelected();
  if(!select) return;
  if (!select->InheritsFrom(TH2::Class())) {gPad->SetUniqueID(0); return;}
  TH2 *h = (TH2*)select;
  gPad->GetCanvas()->FeedbackMode(kTRUE);

  int pyold = gPad->GetUniqueID();
  int px = gPad->GetEventX();
  // int py = gPad->GetEventY();
  float uymin = gPad->GetUymin();
  float uymax = gPad->GetUymax();
  int pymin = gPad->YtoAbsPixel(uymin);
  int pymax = gPad->YtoAbsPixel(uymax);
  if(pyold) gVirtualX->DrawLine(pyold,pymin,pyold,pymax);
  gVirtualX->DrawLine(px,pymin,px,pymax);
  gPad->SetUniqueID(px);
  Float_t upx = gPad->AbsPixeltoX(px);
  Float_t x = gPad->PadtoX(upx);

  TVirtualPad *padsav = gPad;
  TCanvas *c2 = (TCanvas*)gROOT->GetListOfCanvases()->FindObject("CFDShowProjectY5");
  if(c2) delete c2->GetPrimitive("Projection");
  else c2 = new TCanvas("CFDShowProjectY5","CFD ShowProjectY",710,10,700,500);
  c2->SetGrid();
  c2->cd();

  Int_t binx = h->GetXaxis()->FindBin(x);
  TH1D *hp = h->ProjectionY("",binx,binx);
  hp->SetFillColor(38);
  char title[80];
  sprintf(title,"Projection of bin X = %d",binx);
  hp->SetName("ProjectionCFD5");
  hp->SetTitle(title);
  TLatex *ltx = new TLatex();
  if(hp->Fit("gaus","QL") == 0)
    {
      hp->GetFunction("gaus")->SetNpx(1000);
      hp->GetFunction("gaus")->SetLineColor(kRed);
      ltx->SetNDC(kTRUE);
      ltx->SetTextFont(22);
      ltx->SetTextSize(0.06);
      ltx->SetTextColor(4);
      ltx->DrawLatex(0.01,0.9,TString::Format("Mean: %0.3f  Sigma: %0.3f       Rec Thre: %d",hp->GetFunction("gaus")->GetParameter(1),hp->GetFunction("gaus")->GetParameter(2),int(hp->GetFunction("gaus")->GetParameter(1)+3*hp->GetFunction("gaus")->GetParameter(2)+1)).Data());
    }
  c2->Update();
  padsav->cd();
  delete ltx;
}

// 
// Offline.cc ends here
