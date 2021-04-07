// Offline.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 7月 29 20:39:43 2016 (+0800)
// Last-Updated: 三 4月  7 15:34:31 2021 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 1082
// URL: http://wuhongyi.cn 

// offlinedata->GetEventWaveLocation()

//ch offlinedata->GetEventChannel()
//slot offlinedata->GetEventSlot()
//crate offlinedata->GetEventCrate()
// Header length offlinedata->GetEventHeaderLength()
// Event offlinedata->GetEventLength()
// Finish code offlinedata->GetEventFinishCode()

// EventTime_Low  offlinedata->GetEventTime_Low()
// EventTime_High  offlinedata->GetEventTime_High()
//(Long64_t(offlinedata->GetEventTime_High()))*0x100000000+offlinedata->GetEventTime_Low()

//cfd offlinedata->GetEventCfd()
//cfd forced trigger bit  offlinedata->GetEventCfdForcedTriggerBit()

//trace length offlinedata->GetEventTraceLength()
//event energy offlinedata->GetEventEnergy()
//trace out-of-range flag  offlinedata->GetEventTraceOutOfRangeFlag()

//trailing energy sum
//leading energy sum
//gap energy sum
//baseline value
// offlinedata->GetEventSUMS4(,3)

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

#include "OfflineData.hh"
#include "Offline.hh"
#include "Detector.hh"
#include "Global.hh"

#include "pixie16app_export.h"
#include "pixie16sys_export.h"
#include "xia_common.h"

#include "TLegend.h"
#include "TGTab.h"
#include "TString.h"
#include "TFitResultPtr.h"
#include "TMath.h"
#include "TColor.h"
#include <cmath>
#include <cstring>
#include <iostream>
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#define FFT_BASELINE  0.0000001  // Baseline for low clipping (-140dB)
#define NORM_FACTOR   4096       // Normalize the amplitide (12bit ADC)

// Types of windowing
#define HANNING_FFT_WINDOW    0
#define HAMMING_FFT_WINDOW    1
#define BLACKMAN_FFT_WINDOW   2
#define RECT_FFT_WINDOW       3


#define TIMEDIFFGATE  10000  //ns

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Offline::Offline(const TGWindow * p, const TGWindow * main,Detector *det,TGTextEntry *filepath, TGTextEntry *filename)
{
  SetCleanup(kDeepCleanup);
  detector = det;
  filepathtext = filepath;
  filenametext = filename;

  SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  modNumber = 0;
  chanNumber = 0;
  chanNumber4 = 0;
  chanNumber5 = 0;
  chanNumber6 = 0;
  chanNumber8 = 0;
  chanNumber9 = 0;
  chanNumberA10 = 0;
  chanNumberB10 = 0;
  chanNumber11 = 0;
  chanNumber12 = 0;
  chanNumberA13 = 0;
  chanNumberB13 = 0;
  chanNumber14 = 0;
  fileRunNum = 0;

  adjustdslider = false;
  offlinedata = NULL;
  OfflineCurrentCount = -1;
  rawdata = NULL;
  threshdata = NULL;
  cfdthreshdata = NULL;
  cfddata = NULL;
  cfdsdata = NULL;
  sfilterdata = NULL;
  ffilterdata = NULL;
  RcdTrace = NULL;
  doublethresh = NULL;
  doublecfdthresh = NULL;
  doublesample = NULL;
  doublercdtrace = NULL;
  doublefastfilter = NULL;
  doublecfd = NULL;
  doublecfds = NULL;
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
      doublecfds2[i] = NULL;
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
  doublecfds5 = NULL;
  
  offlineth1d6 = NULL;
  RcdTrace6 = NULL;
  doublefastfilter6 = NULL;
  doublecfd6 = NULL;
  doublecfds6 = NULL;
  doubleslowfilter6 = NULL;
  falggausfit6 = false;
  
  RcdTrace8 = NULL;
  doublefastfilter8 = NULL;
  energyfffirst8 = NULL;
  energyffsecond8 = NULL;
  histenergyfffirst8 = NULL;
  histenergyffsecond8 = NULL;

  RcdTrace9 = NULL;
  BufferFFT9 = NULL;
  graphfft9 = NULL;
  in9 = NULL;
  fft1d9 = NULL;
  OfflineCurrentCount9 = -1;

  offlineth1i10 = NULL;
  falggausfit10 = false;
  
  offlineth1icfdinvalid11 = NULL;
  offlineth1icfdvalid11 = NULL; 
  offlineth1icfd11 = NULL;      
  offlineth2i11 = NULL;         

  offlineth1ipileupevent12 = NULL;    
  offlineth1igoogevent12 = NULL;    
  offlineth1itraceevent12 = NULL;    
  offlineth1iwithoutteaceevent12 = NULL;    

  for (int i = 0; i < 8; ++i)
    for (int j = 0; j < 64; ++j)
      offlineth1i13[i][j] = NULL;

  RcdTrace14 = NULL;
  doublefastfilter14 = NULL;
  offlineth1i14 = NULL;
  offlineth2i14 = NULL;
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGTab *TabPanel = new TGTab(this);
  this->AddFrame(TabPanel, new TGLayoutHints(kLHintsBottom | kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
  
  TGCompositeFrame *Tab0 = TabPanel->AddTab("InitData");
  TabPanel->GetTabTab("InitData")->ChangeBackground(TColor::RGB2Pixel(TAB_BG_R,TAB_BG_G,TAB_BG_B));
  MakeFold0Panel(Tab0);
  Tab0->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  TGCompositeFrame *Tab1 = TabPanel->AddTab("Adjust Par");
  TabPanel->GetTabTab("Adjust Par")->ChangeBackground(TColor::RGB2Pixel(TAB_BG_R,TAB_BG_G,TAB_BG_B));
  MakeFold1Panel(Tab1);
  Tab1->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  TGCompositeFrame *Tab2 = TabPanel->AddTab("Wave-16");
  TabPanel->GetTabTab("Wave-16")->ChangeBackground(TColor::RGB2Pixel(TAB_BG_R,TAB_BG_G,TAB_BG_B));
  MakeFold2Panel(Tab2);
  Tab2->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  TGCompositeFrame *Tab3 = TabPanel->AddTab("Energy-16");
  TabPanel->GetTabTab("Energy-16")->ChangeBackground(TColor::RGB2Pixel(TAB_BG_R,TAB_BG_G,TAB_BG_B));
  MakeFold3Panel(Tab3);
  Tab3->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  TGCompositeFrame *Tab4 = TabPanel->AddTab("Orig Energy");
  TabPanel->GetTabTab("Orig Energy")->ChangeBackground(TColor::RGB2Pixel(TAB_BG_R,TAB_BG_G,TAB_BG_B));
  MakeFold4Panel(Tab4);
  Tab4->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  TGCompositeFrame *Tab6 = TabPanel->AddTab("Calc Energy");
  TabPanel->GetTabTab("Calc Energy")->ChangeBackground(TColor::RGB2Pixel(TAB_BG_R,TAB_BG_G,TAB_BG_B));
  MakeFold6Panel(Tab6);
  Tab6->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  TGCompositeFrame *Tab5 = TabPanel->AddTab("FF/CFD Thre");
  TabPanel->GetTabTab("FF/CFD Thre")->ChangeBackground(TColor::RGB2Pixel(TAB_BG_R,TAB_BG_G,TAB_BG_B));
  MakeFold5Panel(Tab5);
  Tab5->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  TGCompositeFrame *Tab8 = TabPanel->AddTab("Energy-FF");
  TabPanel->GetTabTab("Energy-FF")->ChangeBackground(TColor::RGB2Pixel(TAB_BG_R,TAB_BG_G,TAB_BG_B));
  MakeFold8Panel(Tab8);
  Tab8->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  TGCompositeFrame *Tab13 = TabPanel->AddTab("CFD D/F");
  TabPanel->GetTabTab("CFD D/F")->ChangeBackground(TColor::RGB2Pixel(TAB_BG_R,TAB_BG_G,TAB_BG_B));
  MakeFold13Panel(Tab13);
  Tab13->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  TGCompositeFrame *Tab14 = TabPanel->AddTab("CFD Frac");
  TabPanel->GetTabTab("CFD Frac")->ChangeBackground(TColor::RGB2Pixel(TAB_BG_R,TAB_BG_G,TAB_BG_B));
  MakeFold14Panel(Tab14);
  Tab14->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  TGCompositeFrame *Tab11 = TabPanel->AddTab("Energy-CFD");
  TabPanel->GetTabTab("Energy-CFD")->ChangeBackground(TColor::RGB2Pixel(TAB_BG_R,TAB_BG_G,TAB_BG_B));
  MakeFold11Panel(Tab11);
  Tab11->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  TGCompositeFrame *Tab12 = TabPanel->AddTab("Event Flag");
  TabPanel->GetTabTab("Event Flag")->ChangeBackground(TColor::RGB2Pixel(TAB_BG_R,TAB_BG_G,TAB_BG_B));
  MakeFold12Panel(Tab12);
  Tab12->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  TGCompositeFrame *Tab7 = TabPanel->AddTab("QDC");
  TabPanel->GetTabTab("QDC")->ChangeBackground(TColor::RGB2Pixel(TAB_BG_R,TAB_BG_G,TAB_BG_B));
  MakeFold7Panel(Tab7);
  Tab7->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  TGCompositeFrame *Tab9 = TabPanel->AddTab("FFT");
  TabPanel->GetTabTab("FFT")->ChangeBackground(TColor::RGB2Pixel(TAB_BG_R,TAB_BG_G,TAB_BG_B));
  MakeFold9Panel(Tab9);
  Tab9->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  TGCompositeFrame *Tab10 = TabPanel->AddTab("Time Diff");
  TabPanel->GetTabTab("Time Diff")->ChangeBackground(TColor::RGB2Pixel(TAB_BG_R,TAB_BG_G,TAB_BG_B));
  MakeFold10Panel(Tab10);
  Tab10->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  SetWindowName("Review & Offline optimization");
  MapSubwindows();
  MapWindow();
  Resize(1500,1000);

  gStyle->SetOptStat(0);
}

Offline::~Offline()
{
  if(rawdata != NULL) delete rawdata;
  if(threshdata != NULL) delete threshdata;
  if(cfddata != NULL) delete cfddata;
  if(cfdsdata != NULL) delete cfdsdata;
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
  if(doublecfds5 != NULL) delete []doublecfds5;
  
  if(RcdTrace6 != NULL) delete []RcdTrace6;
  if(doublefastfilter6 != NULL) delete []doublefastfilter6;
  if(doublecfd6 != NULL) delete []doublecfd6;
  if(doublecfds6 != NULL) delete []doublecfds6;
  if(doubleslowfilter6 != NULL) delete []doubleslowfilter6;

  if(RcdTrace8 != NULL) delete []RcdTrace8;
  if(doublefastfilter8 != NULL) delete []doublefastfilter8;
  if(energyfffirst8 != NULL) delete energyfffirst8;
  if(energyffsecond8 != NULL) delete energyffsecond8;
  if(histenergyfffirst8 != NULL) delete histenergyfffirst8;
  if(histenergyffsecond8 != NULL) delete histenergyffsecond8;

  if(RcdTrace9 != NULL) delete []RcdTrace9;
  if(BufferFFT9 != NULL) delete []BufferFFT9;
  if(graphfft9 != NULL) delete graphfft9;
  if(fft1d9 != NULL) delete fft1d9;
  if(in9 != NULL) Free_fftw_complex(in9);

  if(offlineth1i10 != NULL) delete offlineth1i10;

  if(offlineth1icfdinvalid11 != NULL) delete offlineth1icfdinvalid11;
  if(offlineth1icfdvalid11 != NULL) delete offlineth1icfdvalid11;
  if(offlineth1icfd11 != NULL) delete offlineth1icfd11;
  if(offlineth2i11 != NULL) delete offlineth2i11;

  if(offlineth1ipileupevent12 != NULL) delete offlineth1ipileupevent12;
  if(offlineth1igoogevent12 != NULL) delete offlineth1igoogevent12;
  if(offlineth1itraceevent12 != NULL) delete offlineth1itraceevent12;
  if(offlineth1iwithoutteaceevent12 != NULL) delete offlineth1iwithoutteaceevent12;

  for (int i = 0; i < 8; ++i)
    for (int j = 0; j < 64; ++j)
      if(offlineth1i13[i][j] != NULL) delete offlineth1i13[i][j];

  if(RcdTrace14 != NULL) delete []RcdTrace14;
  if(doublefastfilter14 != NULL) delete []doublefastfilter14;
  if(offlineth1i14 != NULL) delete offlineth1i14;
  if(offlineth2i14 != NULL) delete offlineth2i14;
  
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
      if(doublecfds2[i] != NULL) delete []doublecfds2[i];
      if(doubleslowfilter2[i] != NULL) delete []doubleslowfilter2[i];
    }

  if(offlinedata != NULL) delete offlinedata;
     
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
  TabPanel->AddFrame(LogoFrame, new TGLayoutHints(kLHintsTop | kLHintsLeft, 10,0,10, 0));
  LogoFrame->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  TGImageMap* fImagePKU = new TGImageMap(LogoFrame, "../icons/logo1.png");
  LogoFrame->AddFrame(fImagePKU,new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 0, 0, 0));
  fImagePKU->Resize(100,100);
  fImagePKU->ChangeOptions(fImagePKU->GetOptions() | kFixedSize);
  fImagePKU->ChangeOptions(fImagePKU->GetOptions() ^ kRaisedFrame);
  fImagePKU->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  TGImageMap* fImageWHY = new TGImageMap(LogoFrame, "../icons/logo2.png");
  LogoFrame->AddFrame(fImageWHY,new TGLayoutHints(kLHintsTop | kLHintsRight, 100, 0, 0, 0));
  fImageWHY->Resize(100,100);
  fImageWHY->ChangeOptions(fImageWHY->GetOptions() | kFixedSize);
  fImageWHY->ChangeOptions(fImageWHY->GetOptions() ^ kRaisedFrame);
  fImageWHY->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  TGCompositeFrame *parFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);
  TabPanel->AddFrame(parFrame, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 2, 2, 20, 1));
  parFrame->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  // choose header length
  TGLabel *additionalanalysis = new TGLabel(parFrame, "Additional analysis:  ");
  parFrame->AddFrame(additionalanalysis, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 3, 0));
  additionalanalysis->SetTextColor(TColor::RGB2Pixel(COLOR_ORANGE_R,COLOR_ORANGE_G,COLOR_ORANGE_B), false);
  additionalanalysis->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  headerrawenergysumsandbaseline = new TGCheckButton(parFrame, "raw E sums/baseline");
  parFrame->AddFrame(headerrawenergysumsandbaseline, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 3, 0));
  headerrawenergysumsandbaseline->SetOn(kFALSE);
  headerrawenergysumsandbaseline->Connect("Toggled(Bool_t)", "Offline", this, "SelectRawEnergySumsBaseline(Bool_t)");
  headerrawenergysumsandbaseline->SetTextColor(TColor::RGB2Pixel(COLOR_ORANGE_R,COLOR_ORANGE_G,COLOR_ORANGE_B), false);
  headerrawenergysumsandbaseline->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  headerqdcsums = new TGCheckButton(parFrame, "QDC sums");
  parFrame->AddFrame(headerqdcsums, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 3, 0));
  headerqdcsums->SetOn(kFALSE);
  headerqdcsums->Connect("Toggled(Bool_t)", "Offline", this, "SelectQDCSums(Bool_t)");
  headerqdcsums->SetTextColor(TColor::RGB2Pixel(COLOR_ORANGE_R,COLOR_ORANGE_G,COLOR_ORANGE_B), false);
  headerqdcsums->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  

  headerexternaltimestamp = new TGCheckButton(parFrame, "external timestamp");
  parFrame->AddFrame(headerexternaltimestamp, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 30, 3, 0));
  headerexternaltimestamp->SetOn(kFALSE);
  headerexternaltimestamp->Connect("Toggled(Bool_t)", "Offline", this, "SelectExternalTimestamp(Bool_t)");
  headerexternaltimestamp->SetTextColor(TColor::RGB2Pixel(COLOR_ORANGE_R,COLOR_ORANGE_G,COLOR_ORANGE_B), false);
  headerexternaltimestamp->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  
   
   
  
  // run
  TGLabel *run = new TGLabel( parFrame, "Run:");
  parFrame->AddFrame(run, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 5, 0));
  run->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
  run->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  offlinefilerunnum = new TGNumberEntry(parFrame, 0, 4, OFFLINERUNNUM, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, 9999);
  parFrame->AddFrame(offlinefilerunnum, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 3, 0));
  offlinefilerunnum->SetButtonToNum(0);
  offlinefilerunnum->Associate(this);
  offlinefilerunnum->GetNumberEntry()->ChangeOptions(offlinefilerunnum->GetNumberEntry()->GetOptions() ^ kRaisedFrame);
  offlinefilerunnum->GetNumberEntry()->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
  offlinefilerunnum->GetButtonUp()->ChangeOptions(offlinefilerunnum->GetButtonUp()->GetOptions() ^ kRaisedFrame);
  offlinefilerunnum->GetButtonDown()->ChangeOptions(offlinefilerunnum->GetButtonDown()->GetOptions() ^ kRaisedFrame);
  offlinefilerunnum->ChangeSubframesBackground(TColor::RGB2Pixel(TEXTENTRY_BG_R,TEXTENTRY_BG_G,TEXTENTRY_BG_B));
  

  // mod
  TGLabel *mod = new TGLabel( parFrame, "Mod:"); 
  parFrame->AddFrame(mod, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 5, 0));
  mod->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
  mod->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  offlinemodnum = new TGNumberEntry(parFrame, 0, 2, OFFLINEMODNUM, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, PRESET_MAX_MODULES-1);
  parFrame->AddFrame(offlinemodnum, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 3, 0));
  offlinemodnum->SetButtonToNum(0);
  offlinemodnum->Associate(this);
  offlinemodnum->GetNumberEntry()->ChangeOptions(offlinemodnum->GetNumberEntry()->GetOptions() ^ kRaisedFrame);
  offlinemodnum->GetNumberEntry()->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
  offlinemodnum->GetButtonUp()->ChangeOptions(offlinemodnum->GetButtonUp()->GetOptions() ^ kRaisedFrame);
  offlinemodnum->GetButtonDown()->ChangeOptions(offlinemodnum->GetButtonDown()->GetOptions() ^ kRaisedFrame);
  offlinemodnum->ChangeSubframesBackground(TColor::RGB2Pixel(TEXTENTRY_BG_R,TEXTENTRY_BG_G,TEXTENTRY_BG_B));  

  // read
  OfflineReadFileButton = new TGTextButton( parFrame, " &Read ", OFFLINEREAD);
  parFrame->AddFrame(OfflineReadFileButton, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 0, 0));
  OfflineReadFileButton->Associate(this);
  OfflineReadFileButton->ChangeOptions(OfflineReadFileButton->GetOptions() ^ kRaisedFrame);
  OfflineReadFileButton->SetFont(TEXTBUTTON_FONT, false);
  OfflineReadFileButton->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  OfflineReadFileButton->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));
  

  // status
  OfflineFileStatus = new TGTextEntry(parFrame,new TGTextBuffer(30), 10000);
  parFrame->AddFrame(OfflineFileStatus, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 6, 0)); 
  OfflineFileStatus-> SetFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1", false);
  OfflineFileStatus->SetTextColor(TColor::RGB2Pixel(COLOR_BLUE_R,COLOR_BLUE_G,COLOR_BLUE_B), false);
  OfflineFileStatus->SetText("NOT READ");
  OfflineFileStatus->Resize(200, 16);
  OfflineFileStatus->SetEnabled(kFALSE);
  OfflineFileStatus->SetFrameDrawn(kFALSE);
  OfflineFileStatus->ChangeBackground(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGGroupFrame *informationgroup = new TGGroupFrame(TabPanel,"Information");
  TabPanel->AddFrame(informationgroup,new TGLayoutHints(kLHintsExpandX | kLHintsTop,10,10,100,0));
  informationgroup->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  informationgroup->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B));
  
  TGHorizontalFrame *guideframe[10];
  TGTextEntry *guidetextinfor[10];
  
  for (int i = 0; i < PANELNUMBER; ++i)
    {
      guideframe[i] = new TGHorizontalFrame(informationgroup);
      informationgroup->AddFrame(guideframe[i],new TGLayoutHints(kLHintsExpandX | kLHintsTop,0,0,5,0));
      guideframe[i]->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
      
      guidetextinfor[i] = new TGTextEntry(guideframe[i],new TGTextBuffer(30), 10000);
      guideframe[i]->AddFrame(guidetextinfor[i], new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 0, 0, 0));
      guidetextinfor[i]-> SetFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1", false);

      guidetextinfor[i]->SetText(gOfflineGuides[i]);
      guidetextinfor[i]->Resize(1000, 18);
      guidetextinfor[i]->SetEnabled(kFALSE);
      guidetextinfor[i]->SetFrameDrawn(kFALSE);
      guidetextinfor[i]->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
      guidetextinfor[i]->ChangeBackground(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
    }
  
}


void Offline::MakeFold1Panel(TGCompositeFrame *TabPanel)
{
  TGCompositeFrame *adCanvasFrame = new TGCompositeFrame(TabPanel, 800, 800, kHorizontalFrame);
  TabPanel->AddFrame(adCanvasFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 4, 4, 4, 4));
  adCanvasFrame->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));


  TRootEmbeddedCanvas *adjCanvas = new TRootEmbeddedCanvas("offlineadcanvas", adCanvasFrame, 100, 100);
  adCanvasFrame->AddFrame(adjCanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
  adjustCanvas = adjCanvas->GetCanvas();
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
  TGCompositeFrame *filterFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);
  TabPanel->AddFrame(filterFrame, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 2, 2, 1, 1));
  filterFrame->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  // fastlength
  TGLabel *fastlength = new TGLabel( filterFrame, "FastLen:");
  filterFrame->AddFrame(fastlength, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 2, 3, 0));
  fastlength->SetTextColor(TColor::RGB2Pixel(COLOR_DODERBLUE_R,COLOR_DODERBLUE_G,COLOR_DODERBLUE_B), false);
  fastlength->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  offlinefilters[0] = new TGNumberEntryField(filterFrame, OFFLINEFASTLENGTH, 0, TGNumberFormat::kNESReal);
  filterFrame->AddFrame(offlinefilters[0], new TGLayoutHints( kLHintsExpandX | kLHintsTop, 1, 0, 0, 0));

  // fastgap
  TGLabel *fastgap = new TGLabel( filterFrame, "FastGap:");
  filterFrame->AddFrame(fastgap, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 2, 3, 0));
  fastgap->SetTextColor(TColor::RGB2Pixel(COLOR_DODERBLUE_R,COLOR_DODERBLUE_G,COLOR_DODERBLUE_B), false);
  fastgap->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  offlinefilters[1] = new TGNumberEntryField(filterFrame, OFFLINEFASTGAP, 0, TGNumberFormat::kNESReal);
  filterFrame->AddFrame(offlinefilters[1], new TGLayoutHints( kLHintsExpandX | kLHintsTop, 1, 0, 0, 0));

  // fast filter thresh
  TGLabel *thresh = new TGLabel( filterFrame, "FFThr:");
  filterFrame->AddFrame(thresh, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 2, 3, 0));
  thresh->SetTextColor(TColor::RGB2Pixel(COLOR_DODERBLUE_R,COLOR_DODERBLUE_G,COLOR_DODERBLUE_B), false);
  thresh->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  offlinefilters[7] = new TGNumberEntryField(filterFrame, OFFLINETHRESH, 0, TGNumberFormat::kNESReal);
  filterFrame->AddFrame(offlinefilters[7], new TGLayoutHints( kLHintsExpandX | kLHintsTop, 1, 0, 0, 0));


  // cfddelay
  TGLabel *cfddelay = new TGLabel( filterFrame, "CFDDelay:");
  filterFrame->AddFrame(cfddelay, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 2, 3, 0));
  cfddelay->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
  cfddelay->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  offlinefilters[5] = new TGNumberEntryField(filterFrame, OFFLINECFDDELAY, 0, TGNumberFormat::kNESReal);
  filterFrame->AddFrame(offlinefilters[5], new TGLayoutHints( kLHintsExpandX | kLHintsTop, 1, 0, 0, 0));

  // cfdscale
  TGLabel *cfdscale = new TGLabel( filterFrame, "CFDScale:");
  filterFrame->AddFrame(cfdscale, new TGLayoutHints (kLHintsLeft | kLHintsTop, 3, 2, 3, 0));
  cfdscale->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
  cfdscale->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  offlinefilters[6] = new TGNumberEntryField(filterFrame, OFFLINECFDSCALE, 0, TGNumberFormat::kNESReal);
  filterFrame->AddFrame(offlinefilters[6], new TGLayoutHints( kLHintsExpandX | kLHintsTop, 1, 0, 0, 0));

  // cfd thresh
  TGLabel *cfdthresh = new TGLabel( filterFrame, "CFDThr:");
  filterFrame->AddFrame(cfdthresh, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 2, 3, 0));
  cfdthresh->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
  cfdthresh->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  offlinefilters[8] = new TGNumberEntryField(filterFrame, OFFLINECFDTHRESH, 0, TGNumberFormat::kNESReal);
  filterFrame->AddFrame(offlinefilters[8], new TGLayoutHints( kLHintsExpandX | kLHintsTop, 1, 0, 0, 0));
  
  // slowlength
  TGLabel *lowlength = new TGLabel( filterFrame, "SlowLen:");
  filterFrame->AddFrame(lowlength, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 2, 3, 0));
  lowlength->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  lowlength->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  offlinefilters[2] = new TGNumberEntryField(filterFrame, OFFLINESLOWLENGTH, 0, TGNumberFormat::kNESReal);
  filterFrame->AddFrame(offlinefilters[2], new TGLayoutHints(kLHintsExpandX | kLHintsTop, 1, 0, 0, 0));

  // slowgap
  TGLabel *lowgap = new TGLabel( filterFrame, "SlowGap:");
  filterFrame->AddFrame(lowgap, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 2, 3, 0));
  lowgap->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  lowgap->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  offlinefilters[3] = new TGNumberEntryField(filterFrame, OFFLINESLOWGAP, 0, TGNumberFormat::kNESReal);
  filterFrame->AddFrame(offlinefilters[3], new TGLayoutHints(kLHintsExpandX | kLHintsTop, 1, 0, 0, 0));

  // preamptau
  TGLabel *preamptau = new TGLabel(filterFrame, "Tau:");
  filterFrame->AddFrame(preamptau, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 2, 3, 0));
  preamptau->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  preamptau->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  offlinefilters[4] = new TGNumberEntryField(filterFrame, OFFLINEPREAMPTAU, 0, TGNumberFormat::kNESReal);
  filterFrame->AddFrame(offlinefilters[4], new TGLayoutHints(kLHintsExpandX | kLHintsTop, 1, 0, 0, 0));


  // slowfilterrange
  TGLabel *slowfilterrange = new TGLabel(filterFrame, "FilterRan:");
  filterFrame->AddFrame(slowfilterrange, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 2, 3, 0));
  slowfilterrange->SetTextColor(TColor::RGB2Pixel(COLOR_PINK_R,COLOR_PINK_G,COLOR_PINK_B), false);
  slowfilterrange->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  offlinefilterrange = new TGNumberEntry(filterFrame, 0, 2, OFFLINEFILTERRANGE, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 1, 6);
  filterFrame->AddFrame(offlinefilterrange, new TGLayoutHints(kLHintsExpandX | kLHintsTop, 1, 0, 0, 0));
  offlinefilterrange->SetButtonToNum(0);
  offlinefilterrange->Associate(this);
  offlinefilterrange->GetNumberEntry()->ChangeOptions(offlinefilterrange->GetNumberEntry()->GetOptions() ^ kRaisedFrame);
  offlinefilterrange->GetNumberEntry()->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
  offlinefilterrange->GetButtonUp()->ChangeOptions(offlinefilterrange->GetButtonUp()->GetOptions() ^ kRaisedFrame);
  offlinefilterrange->GetButtonDown()->ChangeOptions(offlinefilterrange->GetButtonDown()->GetOptions() ^ kRaisedFrame);
  offlinefilterrange->ChangeSubframesBackground(TColor::RGB2Pixel(TEXTENTRY_BG_R,TEXTENTRY_BG_G,TEXTENTRY_BG_B));

  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  

  TGCompositeFrame *parFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);
  TabPanel->AddFrame(parFrame, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 2, 2, 1, 1));
  parFrame->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  // Draw option
  TGLabel *drawoptionlabel = new TGLabel(parFrame, "Draw option:");
  parFrame->AddFrame(drawoptionlabel, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 3, 0));
  drawoptionlabel->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  drawoptionlabel->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B));
  
  offlinedrawoption1[0] = new TGCheckButton(parFrame, "Wave");
  parFrame->AddFrame(offlinedrawoption1[0], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 3, 0));
  offlinedrawoption1[0]->SetOn(kTRUE);
  offlinedrawoption1[0]->Connect("Toggled(Bool_t)", "Offline", this, "SelectDrawOptionPanel1(Bool_t)");
  offlinedrawoption1[0]->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  offlinedrawoption1[0]->SetTextColor(TColor::RGB2Pixel(CHECKBUTTON_TEXT_R,CHECKBUTTON_TEXT_G,CHECKBUTTON_TEXT_B));

  offlinedrawoption1[1] = new TGCheckButton(parFrame, "Slow Filter");
  parFrame->AddFrame(offlinedrawoption1[1], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 3, 0));
  offlinedrawoption1[1]->SetOn(kTRUE);
  offlinedrawoption1[1]->Connect("Toggled(Bool_t)", "Offline", this, "SelectDrawOptionPanel1(Bool_t)");
  offlinedrawoption1[1]->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  offlinedrawoption1[1]->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  offlinedrawoption1[2] = new TGCheckButton(parFrame, "Fast Filter");
  parFrame->AddFrame(offlinedrawoption1[2], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 3, 0));
  offlinedrawoption1[2]->SetOn(kTRUE);
  offlinedrawoption1[2]->Connect("Toggled(Bool_t)", "Offline", this, "SelectDrawOptionPanel1(Bool_t)");
  offlinedrawoption1[2]->SetTextColor(TColor::RGB2Pixel(COLOR_DODERBLUE_R,COLOR_DODERBLUE_G,COLOR_DODERBLUE_B), false);
  offlinedrawoption1[2]->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  offlinedrawoption1[3] = new TGCheckButton(parFrame, "Thres");
  parFrame->AddFrame(offlinedrawoption1[3], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 3, 0));  
  offlinedrawoption1[3]->SetOn(kTRUE);
  offlinedrawoption1[3]->Connect("Toggled(Bool_t)", "Offline", this, "SelectDrawOptionPanel1(Bool_t)");
  offlinedrawoption1[3]->SetTextColor(TColor::RGB2Pixel(COLOR_DODERBLUE_R,COLOR_DODERBLUE_G,COLOR_DODERBLUE_B), false);
  offlinedrawoption1[3]->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  offlinedrawoption1[4] = new TGCheckButton(parFrame, "CFD");
  parFrame->AddFrame(offlinedrawoption1[4], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 3, 0));
  offlinedrawoption1[4]->SetOn(kTRUE);
  offlinedrawoption1[4]->Connect("Toggled(Bool_t)", "Offline", this, "SelectDrawOptionPanel1(Bool_t)");
  offlinedrawoption1[4]->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
  offlinedrawoption1[4]->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  offlinedrawoption1[5] = new TGCheckButton(parFrame, "CFD Thres");
  parFrame->AddFrame(offlinedrawoption1[5], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 3, 0));
  offlinedrawoption1[5]->SetOn(kTRUE);
  offlinedrawoption1[5]->Connect("Toggled(Bool_t)", "Offline", this, "SelectDrawOptionPanel1(Bool_t)");
  offlinedrawoption1[5]->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
  offlinedrawoption1[5]->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  offlinedrawoption1[6] = new TGCheckButton(parFrame, "CFDScale");
  parFrame->AddFrame(offlinedrawoption1[6], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 3, 0));
  offlinedrawoption1[6]->SetOn(kFALSE);
  offlinedrawoption1[6]->Connect("Toggled(Bool_t)", "Offline", this, "SelectDrawOptionPanel1(Bool_t)");
  offlinedrawoption1[6]->SetTextColor(TColor::RGB2Pixel(COLOR_PINK_R,COLOR_PINK_G,COLOR_PINK_B), false);
  offlinedrawoption1[6]->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  
  // current count
  OfflineCurrentCountText = new TGTextEntry(parFrame,new TGTextBuffer(30), 10000);
  parFrame->AddFrame(OfflineCurrentCountText, new TGLayoutHints(kLHintsRight | kLHintsTop, 0, 0, 6, 0));
  OfflineCurrentCountText-> SetFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1", false);
  OfflineCurrentCountText->SetTextColor(TColor::RGB2Pixel(COLOR_DODERBLUE_R,COLOR_DODERBLUE_G,COLOR_DODERBLUE_B), false);
  OfflineCurrentCountText->SetText("/-1");
  OfflineCurrentCountText->Resize(150, 12);
  OfflineCurrentCountText->SetEnabled(kFALSE);
  OfflineCurrentCountText->SetFrameDrawn(kFALSE);
  OfflineCurrentCountText->ChangeBackground(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));


  // CurrentCount
  offlinecurrentcountentry = new TGNumberEntryField(parFrame, -1, 0, TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative);
  parFrame->AddFrame(offlinecurrentcountentry, new TGLayoutHints(kLHintsRight | kLHintsTop, 20, 0, 3, 0));

  // draw
  OfflineDrawButton = new TGTextButton(parFrame, " &Draw ", OFFLINEDRAW);
  parFrame->AddFrame(OfflineDrawButton, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 10, 1, 0));
  OfflineDrawButton->SetEnabled(0);
  OfflineDrawButton->Associate(this);
  OfflineDrawButton->ChangeOptions(OfflineDrawButton->GetOptions() ^ kRaisedFrame);
  OfflineDrawButton->SetFont(TEXTBUTTONSMALL_FONT, false);
  OfflineDrawButton->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  OfflineDrawButton->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));

  
  // draw style
  choosedrawmarkerstyle = new TGComboBox(parFrame);
  parFrame->AddFrame(choosedrawmarkerstyle, new TGLayoutHints(kLHintsRight, 0, 5, 2, 2));
  choosedrawmarkerstyle->Resize(80, 20);//注意编号与类型的对应关系
  choosedrawmarkerstyle->AddEntry("Line1", 0);
  choosedrawmarkerstyle->AddEntry("Line2", 1);
  choosedrawmarkerstyle->AddEntry("Point", 2);
  choosedrawmarkerstyle->AddEntry("Line1-Point", 3);
  choosedrawmarkerstyle->AddEntry("Line2-Point", 4);
  choosedrawmarkerstyle->Select(0);
  
  TGLabel *LabelDrawStyle = new TGLabel(parFrame, "Style:"); 
  parFrame->AddFrame(LabelDrawStyle, new TGLayoutHints(kLHintsRight | kLHintsTop, 5, 2, 7, 0));
  LabelDrawStyle->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  LabelDrawStyle->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B));

  
  // apply
  OfflineApplyButton = new TGTextButton( parFrame, "&Apply", OFFLINEAPPLY);
  OfflineApplyButton->Associate(this);
  parFrame->AddFrame(OfflineApplyButton , new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 5, 1, 0));
  OfflineApplyButton->ChangeOptions(OfflineApplyButton->GetOptions() ^ kRaisedFrame);
  OfflineApplyButton->SetFont(TEXTBUTTONSMALL_FONT, false);
  OfflineApplyButton->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  OfflineApplyButton->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));

  // load
  OfflineLoadButton = new TGTextButton( parFrame, " &Load ", OFFLINELOAD);
  OfflineLoadButton->Associate(this);
  parFrame->AddFrame(OfflineLoadButton, new TGLayoutHints(kLHintsRight | kLHintsTop, 20, 5, 1, 0));
  OfflineLoadButton->ChangeOptions(OfflineLoadButton->GetOptions() ^ kRaisedFrame);
  OfflineLoadButton->SetFont(TEXTBUTTONSMALL_FONT, false);
  OfflineLoadButton->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  OfflineLoadButton->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));

  // ch
  offlinechnum = new TGNumberEntry(parFrame, 0, 2, OFFLINECHNUM, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, 15);
  parFrame->AddFrame(offlinechnum, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 0, 2, 0));
  offlinechnum->SetButtonToNum(0);
  offlinechnum->Associate(this);
  offlinechnum->GetNumberEntry()->ChangeOptions(offlinechnum->GetNumberEntry()->GetOptions() ^ kRaisedFrame);
  offlinechnum->GetNumberEntry()->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
  offlinechnum->GetButtonUp()->ChangeOptions(offlinechnum->GetButtonUp()->GetOptions() ^ kRaisedFrame);
  offlinechnum->GetButtonDown()->ChangeOptions(offlinechnum->GetButtonDown()->GetOptions() ^ kRaisedFrame);
  offlinechnum->ChangeSubframesBackground(TColor::RGB2Pixel(TEXTENTRY_BG_R,TEXTENTRY_BG_G,TEXTENTRY_BG_B));

  
  TGLabel *ch = new TGLabel( parFrame, "Ch:"); 
  parFrame->AddFrame(ch, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 2, 7, 0));
  ch->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  ch->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B));


  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  

  TGCompositeFrame *oldparFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);
  TabPanel->AddFrame(oldparFrame, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 2, 2, 1, 1));
  oldparFrame->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  //slow filter baseline
  TGLabel *LabelChooseSlowFIlterBaseline = new TGLabel(oldparFrame, "Slow Filter Baseline:"); 
  oldparFrame->AddFrame(LabelChooseSlowFIlterBaseline, new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 2, 5, 0));
  LabelChooseSlowFIlterBaseline->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  LabelChooseSlowFIlterBaseline->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  chooseslowfilterbaseline = new TGComboBox(oldparFrame);
  oldparFrame->AddFrame(chooseslowfilterbaseline, new TGLayoutHints(kLHintsLeft, 0, 0, 2, 2));
  chooseslowfilterbaseline->Resize(100, 20);
  chooseslowfilterbaseline->AddEntry("Calculate", 0);
  chooseslowfilterbaseline->AddEntry("Old Baseline", 1);
  chooseslowfilterbaseline->Select(0);

  TGLabel *LabelOldSlowFilterSL = new TGLabel(oldparFrame," 'Old Baseline Pars' choose -> SLen:");
  oldparFrame->AddFrame(LabelOldSlowFilterSL, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 5, 0));
  LabelOldSlowFilterSL->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  LabelOldSlowFilterSL->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  oldslowfilterparameter[0] = new TGNumberEntryField(oldparFrame, -1, 3.040, TGNumberFormat::kNESReal,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,0.032,81.280);
  oldparFrame->AddFrame(oldslowfilterparameter[0], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 2, 0));
  oldslowfilterparameter[0]->Resize(40, 20);
  
  TGLabel *LabelOldSlowFilterSG = new TGLabel(oldparFrame,"SGap:");
  oldparFrame->AddFrame(LabelOldSlowFilterSG, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 5, 0));
  LabelOldSlowFilterSG->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  LabelOldSlowFilterSG->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  oldslowfilterparameter[1] = new TGNumberEntryField(oldparFrame, -1, 0.640, TGNumberFormat::kNESReal,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,0.048,81.280);
  oldparFrame->AddFrame(oldslowfilterparameter[1], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 2, 0));
  oldslowfilterparameter[1]->Resize(40, 20);
  
  TGLabel *LabelOldSlowFilterTAU = new TGLabel(oldparFrame,"Tau:");
  oldparFrame->AddFrame(LabelOldSlowFilterTAU, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 5, 0));
  LabelOldSlowFilterTAU->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  LabelOldSlowFilterTAU->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  oldslowfilterparameter[2] = new TGNumberEntryField(oldparFrame, -1, 0, TGNumberFormat::kNESReal);
  oldparFrame->AddFrame(oldslowfilterparameter[2], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 2, 0));
  oldslowfilterparameter[2]->Resize(40, 20);
  
  TGLabel *oldslowfilterrange = new TGLabel(oldparFrame, "FilRan:");
  oldparFrame->AddFrame(oldslowfilterrange, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 5, 0));
  oldslowfilterrange->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  oldslowfilterrange->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  
  oldofflinefilterrange = new TGNumberEntry(oldparFrame, 0, 2, -1, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 1, 6);
  oldparFrame->AddFrame(oldofflinefilterrange, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 2, 0));
  oldofflinefilterrange->SetIntNumber(2);
  oldofflinefilterrange->GetNumberEntry()->ChangeOptions(oldofflinefilterrange->GetNumberEntry()->GetOptions() ^ kRaisedFrame);
  oldofflinefilterrange->GetNumberEntry()->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
  oldofflinefilterrange->GetButtonUp()->ChangeOptions(oldofflinefilterrange->GetButtonUp()->GetOptions() ^ kRaisedFrame);
  oldofflinefilterrange->GetButtonDown()->ChangeOptions(oldofflinefilterrange->GetButtonDown()->GetOptions() ^ kRaisedFrame);
  oldofflinefilterrange->ChangeSubframesBackground(TColor::RGB2Pixel(TEXTENTRY_BG_R,TEXTENTRY_BG_G,TEXTENTRY_BG_B));
  

  TGLabel *Label500MCFD = new TGLabel(oldparFrame,"500M CFD  ");
  oldparFrame->AddFrame(Label500MCFD, new TGLayoutHints(kLHintsLeft | kLHintsTop, 20, 2, 5, 0));
  Label500MCFD->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  Label500MCFD->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);

  TGLabel *Label500MCFDw = new TGLabel(oldparFrame,"w:");
  oldparFrame->AddFrame(Label500MCFDw, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 2, 5, 0));
  Label500MCFDw->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  Label500MCFDw->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
  
  cfdfilter500Mparameter[0] = new TGNumberEntryField(oldparFrame, -1, 1, TGNumberFormat::kNESReal);
  oldparFrame->AddFrame(cfdfilter500Mparameter[0], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 2, 0));
  cfdfilter500Mparameter[0]->Resize(40, 20);

  TGLabel *Label500MCFDB = new TGLabel(oldparFrame,"B:");
  oldparFrame->AddFrame(Label500MCFDB, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 2, 5, 0));
  Label500MCFDB->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  Label500MCFDB->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
  
  cfdfilter500Mparameter[1] = new TGNumberEntryField(oldparFrame, -1, 5, TGNumberFormat::kNESReal);
  oldparFrame->AddFrame(cfdfilter500Mparameter[1], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 2, 0));
  cfdfilter500Mparameter[1]->Resize(40, 20);

  TGLabel *Label500MCFDD = new TGLabel(oldparFrame,"D:");
  oldparFrame->AddFrame(Label500MCFDD, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 2, 5, 0));
  Label500MCFDD->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  Label500MCFDD->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
  
  cfdfilter500Mparameter[2] = new TGNumberEntryField(oldparFrame, -1, 5, TGNumberFormat::kNESReal);
  oldparFrame->AddFrame(cfdfilter500Mparameter[2], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 2, 0));
  cfdfilter500Mparameter[2]->Resize(40, 20);

  TGLabel *Label500MCFDL = new TGLabel(oldparFrame,"L:");
  oldparFrame->AddFrame(Label500MCFDL, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 2, 5, 0));
  Label500MCFDL->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  Label500MCFDL->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
  
  cfdfilter500Mparameter[3] = new TGNumberEntryField(oldparFrame, -1, 1, TGNumberFormat::kNESReal);
  oldparFrame->AddFrame(cfdfilter500Mparameter[3], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 2, 0));
  cfdfilter500Mparameter[3]->Resize(40, 20);

  
  offlineonlywaveformevent = new TGCheckButton(oldparFrame, "Only Waveform Event");
  oldparFrame->AddFrame(offlineonlywaveformevent, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 0, 3, 0));
  offlineonlywaveformevent->SetOn(kTRUE);
  offlineonlywaveformevent->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  offlineonlywaveformevent->SetTextColor(TColor::RGB2Pixel(CHECKBUTTON_TEXT_R,CHECKBUTTON_TEXT_G,CHECKBUTTON_TEXT_B));
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  dslider = new TGDoubleHSlider(TabPanel,4, kDoubleScaleBoth,ADJUSTPARSLIDER);
  TabPanel->AddFrame(dslider, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 2, 2, 1, 1));
  dslider->Associate(this);
  dslider->SetRange(0,5000);
  dslider->SetPosition(0,5000);
  dslider->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  // sbfold3 = new TGStatusBar(TabPanel,10,10);
  // TabPanel->AddFrame(sbfold3, new TGLayoutHints(kLHintsBottom | kLHintsLeft | kLHintsExpandX, 2, 2, 1, 1));
  // int temp[4];
  // temp[0] = 33;
  // temp[1] = 10;
  // temp[2] = 10;
  // temp[3] = 47;
  // sbfold3->SetParts(temp,4);
  // //TODO
  // // connect to canvas
  
}

void Offline::SelectDrawOptionPanel1(Bool_t on)
{
  if(tracelength == 0) return;
  
  if(rawdata != NULL || threshdata != NULL || cfddata != NULL || cfdsdata != NULL || cfdthreshdata != NULL || sfilterdata != NULL || ffilterdata != NULL)
    {
      if(rawdata != NULL)
	{
	  delete rawdata;
	  rawdata = new TGraph(tracelength,doublesample,doublercdtrace);
	  rawdata->SetMarkerStyle(7);
	}
      if(threshdata != NULL)
	{
	  delete threshdata;
	  threshdata = new TGraph(tracelength,doublesample,doublethresh);
	  threshdata->SetLineColor(4);
	  threshdata->SetMarkerStyle(7);
	  threshdata->SetMarkerColor(4);
	}
      if(cfddata != NULL)
	{
	  delete cfddata;
	  cfddata = new TGraph(tracelength,doublesample,doublecfd);
	  cfddata->SetLineColor(2);
	  cfddata->SetMarkerStyle(7);
	  cfddata->SetMarkerColor(2);
	}
      if(cfdsdata != NULL)
	{
	  delete cfdsdata;
	  cfdsdata = new TGraph(tracelength,doublesample,doublecfds);
	  cfdsdata->SetLineColor(6);
	  cfdsdata->SetMarkerStyle(7);
	  cfdsdata->SetMarkerColor(6);
	}
      if(cfdthreshdata != NULL)
	{
	  delete cfdthreshdata;
	  cfdthreshdata = new TGraph(tracelength,doublesample,doublecfdthresh);
	  cfdthreshdata->SetLineColor(2);
	  cfdthreshdata->SetMarkerStyle(7);
	  cfdthreshdata->SetMarkerColor(2);
	}
      if(sfilterdata != NULL)
	{
	  delete sfilterdata;
	  sfilterdata = new TGraph(tracelength,doublesample,doubleslowfilter);
	  sfilterdata->SetLineColor(3);
	  sfilterdata->SetMarkerStyle(7);
	  sfilterdata->SetMarkerColor(3);
	}
      if(ffilterdata != NULL)
	{
	  delete ffilterdata;
	  ffilterdata  = new TGraph(tracelength,doublesample,doublefastfilter);
	  ffilterdata->SetLineColor(4);
	  ffilterdata->SetMarkerStyle(7);
	  ffilterdata->SetMarkerColor(4);
	}

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
      if(offlinedrawoption1[6]->IsOn())
	offlinemultigraph->Add(cfdsdata);

      if(choosedrawmarkerstyle->GetSelected() == 3 || choosedrawmarkerstyle->GetSelected() == 4)
	{
	  rawdata->SetMarkerStyle(2);
	  threshdata->SetMarkerStyle(2);
	  cfddata->SetMarkerStyle(2);
	  cfdsdata->SetMarkerStyle(2);
	  cfdthreshdata->SetMarkerStyle(2);
	  sfilterdata->SetMarkerStyle(2);
	  ffilterdata->SetMarkerStyle(2);
	}
      if(choosedrawmarkerstyle->GetSelected() == 0 || choosedrawmarkerstyle->GetSelected() == 3)
	{
	  rawdata->SetLineWidth(1);
	  threshdata->SetLineWidth(1);
	  cfddata->SetLineWidth(1);
	  cfdsdata->SetLineWidth(1);
	  cfdthreshdata->SetLineWidth(1);
	  sfilterdata->SetLineWidth(1);
	  ffilterdata->SetLineWidth(1);
	}
      if(choosedrawmarkerstyle->GetSelected() == 1 || choosedrawmarkerstyle->GetSelected() == 4)
	{
	  rawdata->SetLineWidth(2);
	  threshdata->SetLineWidth(2);
	  cfddata->SetLineWidth(2);
	  cfdsdata->SetLineWidth(2);
	  cfdthreshdata->SetLineWidth(2);
	  sfilterdata->SetLineWidth(2);
	  ffilterdata->SetLineWidth(2);
	}
      
      adjustCanvas->cd();
      adjustCanvas->Clear();
      
      offlinemultigraph->SetTitle(TString::Format("Event: %d   e: %d   ts: %lld   %s",OfflineCurrentCount,offlinedata->GetEventEnergy(OfflineCurrentCount),(Long64_t(offlinedata->GetEventTime_High(OfflineCurrentCount)))*0x100000000+offlinedata->GetEventTime_Low(OfflineCurrentCount),offlinedata->GetEventCfdForcedTriggerBit(OfflineCurrentCount)?"cfd: invalid":"cfd: valid").Data());
      if(choosedrawmarkerstyle->GetSelected() == 0 || choosedrawmarkerstyle->GetSelected() == 1) offlinemultigraph->Draw("AL");
      if(choosedrawmarkerstyle->GetSelected() == 2) offlinemultigraph->Draw("AP");
      if(choosedrawmarkerstyle->GetSelected() == 3 || choosedrawmarkerstyle->GetSelected() == 4) offlinemultigraph->Draw("ALP");
      
      if(offlinedrawoption1[0]->IsOn() || offlinedrawoption1[1]->IsOn() || offlinedrawoption1[2]->IsOn() || offlinedrawoption1[3]->IsOn() || offlinedrawoption1[4]->IsOn() || offlinedrawoption1[5]->IsOn() || offlinedrawoption1[6]->IsOn())
	offlinemultigraph->GetXaxis()->SetRangeUser(double(dslider->GetMinPosition()),double(dslider->GetMaxPosition()));
      adjustCanvas->Modified();
      adjustCanvas->Update();
    }

  // gSystem->ProcessEvents();

}


void Offline::MakeFold2Panel(TGCompositeFrame *TabPanel)
{
  TGCompositeFrame *parFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);
  TabPanel->AddFrame(parFrame, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 4, 4, 4, 4));
  parFrame->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  // Draw option
  TGLabel *drawoptionlabel = new TGLabel(parFrame, "Draw option:");
  parFrame->AddFrame(drawoptionlabel, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 3, 0));
  drawoptionlabel->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  drawoptionlabel->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B));


  offlinedrawoption2[0] = new TGCheckButton(parFrame, "Wave");
  parFrame->AddFrame(offlinedrawoption2[0], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 3, 0));
  offlinedrawoption2[0]->SetOn(kTRUE);
  offlinedrawoption2[0]->Connect("Toggled(Bool_t)", "Offline", this, "SelectDrawOptionPanel2(Bool_t)");
  offlinedrawoption2[0]->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  offlinedrawoption2[0]->SetTextColor(TColor::RGB2Pixel(CHECKBUTTON_TEXT_R,CHECKBUTTON_TEXT_G,CHECKBUTTON_TEXT_B));

  offlinedrawoption2[1] = new TGCheckButton(parFrame, "Slow Filter");
  parFrame->AddFrame(offlinedrawoption2[1], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 3, 0));
  offlinedrawoption2[1]->SetOn(kTRUE);
  offlinedrawoption2[1]->Connect("Toggled(Bool_t)", "Offline", this, "SelectDrawOptionPanel2(Bool_t)");
  offlinedrawoption2[1]->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  offlinedrawoption2[1]->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
 
  offlinedrawoption2[2] = new TGCheckButton(parFrame, "Fast Filter");
  parFrame->AddFrame(offlinedrawoption2[2], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 3, 0));
  offlinedrawoption2[2]->SetOn(kTRUE);
  offlinedrawoption2[2]->Connect("Toggled(Bool_t)", "Offline", this, "SelectDrawOptionPanel2(Bool_t)");
  offlinedrawoption2[2]->SetTextColor(TColor::RGB2Pixel(COLOR_DODERBLUE_R,COLOR_DODERBLUE_G,COLOR_DODERBLUE_B), false);
  offlinedrawoption2[2]->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  offlinedrawoption2[3] = new TGCheckButton(parFrame, "Thres");
  parFrame->AddFrame(offlinedrawoption2[3], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 3, 0));  
  offlinedrawoption2[3]->SetOn(kTRUE);
  offlinedrawoption2[3]->Connect("Toggled(Bool_t)", "Offline", this, "SelectDrawOptionPanel2(Bool_t)");
  offlinedrawoption2[3]->SetTextColor(TColor::RGB2Pixel(COLOR_DODERBLUE_R,COLOR_DODERBLUE_G,COLOR_DODERBLUE_B), false);
  offlinedrawoption2[3]->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  offlinedrawoption2[4] = new TGCheckButton(parFrame, "CFD");
  parFrame->AddFrame(offlinedrawoption2[4], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 3, 0));
  offlinedrawoption2[4]->SetOn(kTRUE);
  offlinedrawoption2[4]->Connect("Toggled(Bool_t)", "Offline", this, "SelectDrawOptionPanel2(Bool_t)");
  offlinedrawoption2[4]->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
  offlinedrawoption2[4]->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  offlinedrawoption2[5] = new TGCheckButton(parFrame, "CFD Thres");
  parFrame->AddFrame(offlinedrawoption2[5], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 3, 0));
  offlinedrawoption2[5]->SetOn(kTRUE);
  offlinedrawoption2[5]->Connect("Toggled(Bool_t)", "Offline", this, "SelectDrawOptionPanel2(Bool_t)");
  offlinedrawoption2[5]->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
  offlinedrawoption2[5]->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  
  // draw
  OfflineDrawButton2 = new TGTextButton( parFrame, "&Draw", OFFLINEDRAW2);
  parFrame->AddFrame(OfflineDrawButton2, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 30, 0, 0));
  OfflineDrawButton2->SetEnabled(0);
  OfflineDrawButton2->Associate(this);
  OfflineDrawButton2->ChangeOptions(OfflineDrawButton2->GetOptions() ^ kRaisedFrame);
  OfflineDrawButton2->SetFont(TEXTBUTTONSMALL_FONT, false);
  OfflineDrawButton2->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  OfflineDrawButton2->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGCompositeFrame *adCanvasFrame = new TGCompositeFrame(TabPanel, 800, 800, kHorizontalFrame);
  TabPanel->AddFrame(adCanvasFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 4, 4, 0, 4));

  TRootEmbeddedCanvas *adjCanvas = new TRootEmbeddedCanvas("canvas2", adCanvasFrame, 100, 100);
  adCanvasFrame->AddFrame(adjCanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
  canvas2 = adjCanvas->GetCanvas();
  canvas2->Divide(4,4);

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
      if(tracelength2[i] == 0) continue;
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
	  // offlinemultigraph2[i]->SetTitle(TString::Format("Event: %d",OfflineCurrentCount2[i]).Data());
	  offlinemultigraph2[i]->SetTitle(TString::Format("Event: %d   e: %d   ts: %lld",OfflineCurrentCount2[i],offlinedata->GetEventEnergy(OfflineCurrentCount2[i]),(Long64_t(offlinedata->GetEventTime_High(OfflineCurrentCount2[i])))*0x100000000+offlinedata->GetEventTime_Low(OfflineCurrentCount2[i])).Data());//energy timestamp
	  offlinemultigraph2[i]->Draw("AL");
	} //!= NULL
    } //0-15
  canvas2->Modified();
  canvas2->Update();
}

void Offline::MakeFold3Panel(TGCompositeFrame *TabPanel)
{
  TGCompositeFrame *parFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);
  TabPanel->AddFrame(parFrame, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 4, 4, 4, 4));
  parFrame->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  // draw
  OfflineDrawButton3 = new TGTextButton( parFrame, "&Draw", OFFLINEDRAW3);
  parFrame->AddFrame(OfflineDrawButton3, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 30, 0, 0));
  OfflineDrawButton3->SetEnabled(0);
  OfflineDrawButton3->Associate(this);
  OfflineDrawButton3->ChangeOptions(OfflineDrawButton3->GetOptions() ^ kRaisedFrame);
  OfflineDrawButton3->SetFont(TEXTBUTTONSMALL_FONT, false);
  OfflineDrawButton3->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  OfflineDrawButton3->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGCompositeFrame *adCanvasFrame = new TGCompositeFrame(TabPanel, 800, 800, kHorizontalFrame);
  TabPanel->AddFrame(adCanvasFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 4, 4, 0, 4));
  adCanvasFrame->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  TRootEmbeddedCanvas *adjCanvas = new TRootEmbeddedCanvas("canvas3", adCanvasFrame, 100, 100);
  adCanvasFrame->AddFrame(adjCanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
  canvas3 = adjCanvas->GetCanvas();
}

void Offline::MakeFold4Panel(TGCompositeFrame *TabPanel)
{
  TGCompositeFrame *parFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);
  TabPanel->AddFrame(parFrame, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 4, 4, 4, 4));
  parFrame->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  // Fit
  GausFitButton4 = new TGTextButton(parFrame, "Open  Fit", OFFLINEGAUSFIT4);
  parFrame->AddFrame(GausFitButton4, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 30, 0, 0));
  GausFitButton4->SetEnabled(0);
  GausFitButton4->Associate(this);
  GausFitButton4->ChangeOptions(GausFitButton4->GetOptions() ^ kRaisedFrame);
  GausFitButton4->SetFont(TEXTBUTTONSMALL_FONT, false);
  GausFitButton4->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  GausFitButton4->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));


  // FitButtonGroup4 = new TGHButtonGroup(parFrame, "");
  // parFrame->AddFrame(FitButtonGroup4, new TGLayoutHints(kLHintsLeft | kLHintsTop,1, 1, 1, 1));
  // FitButtonGroup4->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  // // FitButtonGroup4->ChangeSubframesBackground(TColor::RGB2Pixel(100,0,0));
  // FitButtonGroup4->SetLayoutHints( new TGLayoutHints(kLHintsLeft | kLHintsTop,0, 0, 0, 0),  0);
  // FitButtonGroup4->SetBorderDrawn(kFALSE);
  // FitButtonGroup4->SetTitlePos(TGGroupFrame::kLeft);
  // FitButtonGroup4->Show();
  // FitButtonGroup4->SetState(kTRUE);//是否开启，开启才可以选，不开启是灰色的
  // FitButtonGroup4->SetRadioButtonExclusive(kTRUE);
  // FitRadioButton4[0] = new TGRadioButton(FitButtonGroup4, new TGHotString("Gaus"));
  // FitRadioButton4[1] = new TGRadioButton(FitButtonGroup4, new TGHotString("Exp+Gaus"));
  // FitRadioButton4[0]->SetOn();//Default state
  // FitRadioButton4[0]->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  // FitRadioButton4[1]->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  // FitRadioButton4[0]->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B));
  // FitRadioButton4[1]->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B));

  
  // draw
  OfflineDrawButton4 = new TGTextButton(parFrame, "&Draw", OFFLINEDRAW4);
  parFrame->AddFrame(OfflineDrawButton4, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 30, 0, 0));
  OfflineDrawButton4->SetEnabled(0);
  OfflineDrawButton4->Associate(this);
  OfflineDrawButton4->ChangeOptions(OfflineDrawButton4->GetOptions() ^ kRaisedFrame);
  OfflineDrawButton4->SetFont(TEXTBUTTONSMALL_FONT, false);
  OfflineDrawButton4->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  OfflineDrawButton4->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));
  
  
  // ch
  offlinechnum4 = new TGNumberEntry(parFrame, 0, 2, OFFLINECHNUM4, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, 15);
  parFrame->AddFrame(offlinechnum4, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 10, 0, 0));
  offlinechnum4->SetButtonToNum(0);
  offlinechnum4->Associate(this);
  offlinechnum4->GetNumberEntry()->ChangeOptions(offlinechnum4->GetNumberEntry()->GetOptions() ^ kRaisedFrame);
  offlinechnum4->GetNumberEntry()->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
  offlinechnum4->GetButtonUp()->ChangeOptions(offlinechnum4->GetButtonUp()->GetOptions() ^ kRaisedFrame);
  offlinechnum4->GetButtonDown()->ChangeOptions(offlinechnum4->GetButtonDown()->GetOptions() ^ kRaisedFrame);
  offlinechnum4->ChangeSubframesBackground(TColor::RGB2Pixel(TEXTENTRY_BG_R,TEXTENTRY_BG_G,TEXTENTRY_BG_B));  

  TGLabel *ch = new TGLabel(parFrame, "Ch:"); 
  parFrame->AddFrame(ch, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 2, 3, 0));
  ch->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B));
  ch->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  // bin
  chooseth1dbin4 = new TGComboBox(parFrame);
  parFrame->AddFrame(chooseth1dbin4, new TGLayoutHints(kLHintsRight, 5, 25, 0, 0));
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
  parFrame->AddFrame(LabelBinNumber, new TGLayoutHints(kLHintsRight | kLHintsTop, 0, 0, 3, 0));
  LabelBinNumber->SetText("Bin Number:");
  LabelBinNumber->Resize(65,15);
  LabelBinNumber->SetEnabled(kFALSE);
  LabelBinNumber->SetToolTipText("Choose TH1D bin number.");
  LabelBinNumber->SetFrameDrawn(kFALSE);
  LabelBinNumber->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B));
  LabelBinNumber->ChangeBackground(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  
  TGCompositeFrame *adCanvasFrame = new TGCompositeFrame(TabPanel, 800, 800, kHorizontalFrame);
  TabPanel->AddFrame(adCanvasFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 4, 4, 0, 4));
  adCanvasFrame->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  TRootEmbeddedCanvas *adjCanvas = new TRootEmbeddedCanvas("canvas4", adCanvasFrame, 100, 100);
  adCanvasFrame->AddFrame(adjCanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
  canvas4 = adjCanvas->GetCanvas();
  
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

void Offline::GausFit10()
{
  if(falggausfit10)
    {
      GausFitButton10->SetText("Open  Fit");
      falggausfit10 = false;
      canvas10->DeleteExec("dynamicPanel10GausFit");
    }
  else
    {
      GausFitButton10->SetText("Close Fit");
      falggausfit10 = true;
      canvas10->AddExec("dynamicPanel10GausFit","PanelTimeGausFit()");
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

      // FitRadioButton4[0]->IsOn()
      if(h->Fit("gaus","QL","",upxold,upx) == 0)
	{
	  double posl = h->GetFunction("gaus")->GetParameter(1)-3*h->GetFunction("gaus")->GetParameter(2);
	  double posr = h->GetFunction("gaus")->GetParameter(1)+3*h->GetFunction("gaus")->GetParameter(2);	  
	    
	  h->SetTitle(TString::Format("Peak: %0.2f  P/T: %0.2f%%  #DeltaE/E: %0.2f%%",h->GetFunction("gaus")->GetParameter(1),h->Integral(h->FindBin(posl),h->FindBin(posr))/h->GetEntries()*100,h->GetFunction("gaus")->GetParameter(2)*2.355/h->GetFunction("gaus")->GetParameter(1)*100.0).Data());
	}
      else
	{
	  h->SetTitle("Please choose Fit range again.");
	}
      gPad->SetUniqueID(0);
    }
}


void PanelTimeGausFit()
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
	  h->SetTitle(TString::Format("FWHM:  %0.2f ps",h->GetFunction("gaus")->GetParameter(2)*2.355*1000).Data());
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
  TabPanel->AddFrame(parFrame, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 4, 4, 4, 4));
  parFrame->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  // stop
  OfflineStopButton5 = new TGTextButton(parFrame, "&Stop", OFFLINESTOPDRAW5);
  parFrame->AddFrame(OfflineStopButton5, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 0, 0));
  OfflineStopButton5->SetEnabled(0);
  OfflineStopButton5->Associate(this);
  OfflineStopButton5->ChangeOptions(OfflineStopButton5->GetOptions() ^ kRaisedFrame);
  OfflineStopButton5->SetFont(TEXTBUTTONSMALL_FONT, false);
  OfflineStopButton5->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  OfflineStopButton5->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));

  
  // text
  printtextinfor5 = new TGTextEntry(parFrame,new TGTextBuffer(30), 10000);
  parFrame->AddFrame(printtextinfor5, new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 0, 3, 0));
  printtextinfor5->SetFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1", false);
  printtextinfor5->SetTextColor(TColor::RGB2Pixel(COLOR_DODERBLUE_R,COLOR_DODERBLUE_G,COLOR_DODERBLUE_B), false);
  printtextinfor5->SetText("Choose 'Ch' then enter button 'Draw'.");
  printtextinfor5->Resize(500, 12);
  printtextinfor5->SetEnabled(kFALSE);
  printtextinfor5->SetFrameDrawn(kFALSE);
  printtextinfor5->ChangeBackground(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));  

  
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
  parFrame->AddFrame(showprojectyFF5, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 0, 0));
  showprojectyFF5->SetEnabled(0);
  showprojectyFF5->Associate(this);
  showprojectyFF5->ChangeOptions(showprojectyFF5->GetOptions() ^ kRaisedFrame);
  showprojectyFF5->SetFont(TEXTBUTTONSMALL_FONT, false);
  showprojectyFF5->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  showprojectyFF5->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));
  
  // CFDProjectY
  showprojectyCFD5 = new TGTextButton(parFrame, "&CFDProjectY", OFFLINEPROJECTYCFD5);
  parFrame->AddFrame(showprojectyCFD5, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 30, 0, 0));
  showprojectyCFD5->SetEnabled(0);
  showprojectyCFD5->Associate(this);
  showprojectyCFD5->ChangeOptions(showprojectyCFD5->GetOptions() ^ kRaisedFrame);
  showprojectyCFD5->SetFont(TEXTBUTTONSMALL_FONT, false);
  showprojectyCFD5->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  showprojectyCFD5->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));
  
  // OriginalCFD
  originalcfd5 = new TGTextButton(parFrame, "&OriginalCFD", OFFLINEORIGINALCFD5);
  parFrame->AddFrame(originalcfd5, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 0, 0));
  originalcfd5->SetEnabled(0);
  originalcfd5->Associate(this);
  originalcfd5->ChangeOptions(originalcfd5->GetOptions() ^ kRaisedFrame);
  originalcfd5->SetFont(TEXTBUTTONSMALL_FONT, false);
  originalcfd5->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  originalcfd5->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));
  
  // CalculateCFD
  calculatecfd5 = new TGTextButton(parFrame, "&CalculateCFD", OFFLINECALCULATECFD5);
  parFrame->AddFrame(calculatecfd5, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 30, 0, 0));
  calculatecfd5->SetEnabled(0);
  calculatecfd5->Associate(this);
  calculatecfd5->ChangeOptions(calculatecfd5->GetOptions() ^ kRaisedFrame);
  calculatecfd5->SetFont(TEXTBUTTONSMALL_FONT, false);
  calculatecfd5->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  calculatecfd5->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));
  
  // draw
  OfflineDrawButton5 = new TGTextButton( parFrame, "&Draw", OFFLINEDRAW5);
  parFrame->AddFrame(OfflineDrawButton5, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 30, 0, 0));
  OfflineDrawButton5->SetEnabled(0);
  OfflineDrawButton5->Associate(this);
  OfflineDrawButton5->ChangeOptions(OfflineDrawButton5->GetOptions() ^ kRaisedFrame);
  OfflineDrawButton5->SetFont(TEXTBUTTONSMALL_FONT, false);
  OfflineDrawButton5->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  OfflineDrawButton5->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));

  
  // ch
  offlinechnum5 = new TGNumberEntry (parFrame, 0, 2, OFFLINECHNUM5, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, 15);
  parFrame->AddFrame(offlinechnum5, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 10, 0, 0));
  offlinechnum5->SetButtonToNum(0);
  offlinechnum5->Associate(this);
  offlinechnum5->GetNumberEntry()->ChangeOptions(offlinechnum5->GetNumberEntry()->GetOptions() ^ kRaisedFrame);
  offlinechnum5->GetNumberEntry()->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
  offlinechnum5->GetButtonUp()->ChangeOptions(offlinechnum5->GetButtonUp()->GetOptions() ^ kRaisedFrame);
  offlinechnum5->GetButtonDown()->ChangeOptions(offlinechnum5->GetButtonDown()->GetOptions() ^ kRaisedFrame);
  offlinechnum5->ChangeSubframesBackground(TColor::RGB2Pixel(TEXTENTRY_BG_R,TEXTENTRY_BG_G,TEXTENTRY_BG_B));
  
  TGLabel *ch = new TGLabel( parFrame, "Ch:"); 
  parFrame->AddFrame(ch, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 2, 3, 0));
  ch->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  ch->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B));  
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGCompositeFrame *adCanvasFrame = new TGCompositeFrame(TabPanel, 800, 800, kHorizontalFrame);
  TabPanel->AddFrame(adCanvasFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 4, 4, 0, 4));
  adCanvasFrame->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  TRootEmbeddedCanvas *adjCanvas = new TRootEmbeddedCanvas("canvas5", adCanvasFrame, 100, 100);
  adCanvasFrame->AddFrame(adjCanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
  canvas5 = adjCanvas->GetCanvas();
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  OriginalCFDcanvas5 = NULL;
  CalculateCFDcanvas5 = NULL;
}

void Offline::MakeFold6Panel(TGCompositeFrame *TabPanel)
{
  TGCompositeFrame *parFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);
  TabPanel->AddFrame(parFrame, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 4, 4, 4, 2));
  parFrame->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  // stop
  OfflineStopButton6 = new TGTextButton(parFrame, "&Stop", OFFLINESTOPDRAW6);
  parFrame->AddFrame(OfflineStopButton6, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 0, 0));
  OfflineStopButton6->SetEnabled(0);
  OfflineStopButton6->Associate(this);
  OfflineStopButton6->ChangeOptions(OfflineStopButton6->GetOptions() ^ kRaisedFrame);
  OfflineStopButton6->SetFont(TEXTBUTTONSMALL_FONT, false);
  OfflineStopButton6->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  OfflineStopButton6->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));
  
  
  // text
  printtextinfor6 = new TGTextEntry(parFrame,new TGTextBuffer(30), 10000);
  parFrame->AddFrame(printtextinfor6, new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 0, 3, 0));
  printtextinfor6-> SetFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1", false);
  printtextinfor6->SetTextColor(TColor::RGB2Pixel(COLOR_DODERBLUE_R,COLOR_DODERBLUE_G,COLOR_DODERBLUE_B), false);
  printtextinfor6->SetText("Choose 'Bin' and 'Ch' then enter button 'Draw'.");
  printtextinfor6->Resize(500, 12);
  printtextinfor6->SetEnabled(kFALSE);
  printtextinfor6->SetFrameDrawn(kFALSE);
  printtextinfor6->ChangeBackground(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));


  // Fit
  GausFitButton6 = new TGTextButton(parFrame, "Open  Fit", OFFLINEGAUSFIT6);
  parFrame->AddFrame(GausFitButton6, new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 10, 0, 0));
  GausFitButton6->SetEnabled(0);
  GausFitButton6->Associate(this);
  GausFitButton6->ChangeOptions(GausFitButton6->GetOptions() ^ kRaisedFrame);
  GausFitButton6->SetFont(TEXTBUTTONSMALL_FONT, false);
  GausFitButton6->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  GausFitButton6->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));
    

  // draw
  OfflineDrawButton6 = new TGTextButton(parFrame, "&Draw", OFFLINEDRAW6);
  parFrame->AddFrame(OfflineDrawButton6, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 30, 0, 0));
  OfflineDrawButton6->SetEnabled(0);
  OfflineDrawButton6->Associate(this);
  OfflineDrawButton6->ChangeOptions(OfflineDrawButton6->GetOptions() ^ kRaisedFrame);
  OfflineDrawButton6->SetFont(TEXTBUTTONSMALL_FONT, false);
  OfflineDrawButton6->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  OfflineDrawButton6->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));
    
  // ch
  offlinechnum6 = new TGNumberEntry(parFrame, 0, 2, OFFLINECHNUM6, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, 15);
  parFrame->AddFrame(offlinechnum6, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 10, 0, 0));
  offlinechnum6->SetButtonToNum(0);
  offlinechnum6->Associate(this);
  offlinechnum6->GetNumberEntry()->ChangeOptions(offlinechnum6->GetNumberEntry()->GetOptions() ^ kRaisedFrame);
  offlinechnum6->GetNumberEntry()->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
  offlinechnum6->GetButtonUp()->ChangeOptions(offlinechnum6->GetButtonUp()->GetOptions() ^ kRaisedFrame);
  offlinechnum6->GetButtonDown()->ChangeOptions(offlinechnum6->GetButtonDown()->GetOptions() ^ kRaisedFrame);
  offlinechnum6->ChangeSubframesBackground(TColor::RGB2Pixel(TEXTENTRY_BG_R,TEXTENTRY_BG_G,TEXTENTRY_BG_B));

  
  TGLabel *ch = new TGLabel( parFrame, "Ch:"); 
  parFrame->AddFrame(ch, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 2, 3, 0));
  ch->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  ch->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B));
  
  // bin
  chooseth1dbin6 = new TGComboBox(parFrame);
  parFrame->AddFrame(chooseth1dbin6, new TGLayoutHints(kLHintsRight, 5, 25, 0, 0));
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
  parFrame->AddFrame(LabelBinNumber, new TGLayoutHints(kLHintsRight | kLHintsExpandY, 0, 0, 3, 0));
  LabelBinNumber->SetText("Bin Number:");
  LabelBinNumber->Resize(65,15);
  LabelBinNumber->SetEnabled(kFALSE);
  LabelBinNumber->SetToolTipText("Choose TH1D bin number.");
  LabelBinNumber->SetFrameDrawn(kFALSE);
  LabelBinNumber->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B));
  LabelBinNumber->ChangeBackground(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGCompositeFrame *slowfilterparFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);
  TabPanel->AddFrame(slowfilterparFrame, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 4, 4, 2, 4));
  slowfilterparFrame->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  //slow filter baseline
  TGLabel *LabelChooseSlowFIlterBaseline = new TGLabel(slowfilterparFrame, "SF BL:"); 
  slowfilterparFrame->AddFrame(LabelChooseSlowFIlterBaseline, new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 2, 5, 0));
  LabelChooseSlowFIlterBaseline->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  LabelChooseSlowFIlterBaseline->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  chooseslowfilterbaselinep6 = new TGComboBox(slowfilterparFrame);
  slowfilterparFrame->AddFrame(chooseslowfilterbaselinep6, new TGLayoutHints(kLHintsLeft, 0, 0, 0, 0));
  chooseslowfilterbaselinep6->Resize(80, 20);
  chooseslowfilterbaselinep6->AddEntry("Calculate", 0);
  chooseslowfilterbaselinep6->AddEntry("Old Baseline", 1);
  chooseslowfilterbaselinep6->Select(0);

  TGLabel *LabelOldSlowFilterSL = new TGLabel(slowfilterparFrame," 'Old BL' choose -> SL:");
  slowfilterparFrame->AddFrame(LabelOldSlowFilterSL, new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 2, 5, 0));
  LabelOldSlowFilterSL->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  LabelOldSlowFilterSL->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  oldslowfilterparameterp6[0] = new TGNumberEntryField(slowfilterparFrame, -1, 3.040, TGNumberFormat::kNESReal,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,0.032,81.280);
  slowfilterparFrame->AddFrame(oldslowfilterparameterp6[0], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 2, 0));
  oldslowfilterparameterp6[0]->Resize(40, 20);
  
  TGLabel *LabelOldSlowFilterSG = new TGLabel(slowfilterparFrame,"SG:");
  slowfilterparFrame->AddFrame(LabelOldSlowFilterSG, new TGLayoutHints(kLHintsLeft | kLHintsTop, 4, 2, 5, 0));
  LabelOldSlowFilterSG->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  LabelOldSlowFilterSG->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  oldslowfilterparameterp6[1] = new TGNumberEntryField(slowfilterparFrame, -1, 0.640, TGNumberFormat::kNESReal,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,0.048,81.280);
  slowfilterparFrame->AddFrame(oldslowfilterparameterp6[1], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 2, 0));
  oldslowfilterparameterp6[1]->Resize(40, 20);
  
  TGLabel *LabelOldSlowFilterTAU = new TGLabel(slowfilterparFrame,"Tau:");
  slowfilterparFrame->AddFrame(LabelOldSlowFilterTAU, new TGLayoutHints(kLHintsLeft | kLHintsTop, 4, 2, 5, 0));
  LabelOldSlowFilterTAU->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  LabelOldSlowFilterTAU->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  oldslowfilterparameterp6[2] = new TGNumberEntryField(slowfilterparFrame, -1, 0, TGNumberFormat::kNESReal);
  slowfilterparFrame->AddFrame(oldslowfilterparameterp6[2], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 2, 0));
  oldslowfilterparameterp6[2]->Resize(40, 20);
  
  TGLabel *oldslowfilterrange = new TGLabel(slowfilterparFrame, "FilRan:");
  slowfilterparFrame->AddFrame(oldslowfilterrange, new TGLayoutHints(kLHintsLeft | kLHintsTop, 4, 2, 5, 0));
  oldslowfilterrange->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  oldslowfilterrange->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  oldofflinefilterrangep6 = new TGNumberEntry(slowfilterparFrame, 0, 2, -1, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 1, 6);
  slowfilterparFrame->AddFrame(oldofflinefilterrangep6, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 2, 0));
  oldofflinefilterrangep6->SetIntNumber(2);

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGCompositeFrame *adCanvasFrame = new TGCompositeFrame(TabPanel, 800, 800, kHorizontalFrame);
  TabPanel->AddFrame(adCanvasFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 4, 4, 0, 4));
  adCanvasFrame->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  TRootEmbeddedCanvas *adjCanvas = new TRootEmbeddedCanvas("canvas6", adCanvasFrame, 100, 100);
  adCanvasFrame->AddFrame(adjCanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
  canvas6 = adjCanvas->GetCanvas();
}


void Offline::MakeFold7Panel(TGCompositeFrame *TabPanel)
{
  TGCompositeFrame *parFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);
  TabPanel->AddFrame(parFrame, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 4, 4, 4, 4));
  parFrame->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
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
  


  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGCompositeFrame *adCanvasFrame = new TGCompositeFrame(TabPanel, 800, 800, kHorizontalFrame);
  TabPanel->AddFrame(adCanvasFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 4, 4, 0, 4));

  TRootEmbeddedCanvas *adjCanvas = new TRootEmbeddedCanvas("canvas7", adCanvasFrame, 100, 100);
  adCanvasFrame->AddFrame(adjCanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 1, 1, 1, 1));
  canvas7 = adjCanvas->GetCanvas();
}


void Offline::MakeFold8Panel(TGCompositeFrame *TabPanel)
{
  TGCompositeFrame *parFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);
  TabPanel->AddFrame(parFrame, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 4, 4, 4, 2));
  parFrame->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  // stop
  OfflineStopButton8 = new TGTextButton(parFrame, "&Stop", OFFLINESTOPDRAW8);
  parFrame->AddFrame(OfflineStopButton8, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 0, 0));
  OfflineStopButton8->SetEnabled(0);
  OfflineStopButton8->Associate(this);
  OfflineStopButton8->ChangeOptions(OfflineStopButton8->GetOptions() ^ kRaisedFrame);
  OfflineStopButton8->SetFont(TEXTBUTTONSMALL_FONT, false);
  OfflineStopButton8->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  OfflineStopButton8->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));

  
  // text
  printtextinfor8 = new TGTextEntry(parFrame,new TGTextBuffer(30), 10000);
  parFrame->AddFrame(printtextinfor8, new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 0, 3, 0));
  printtextinfor8->SetFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1", false);
  printtextinfor8->SetTextColor(TColor::RGB2Pixel(COLOR_DODERBLUE_R,COLOR_DODERBLUE_G,COLOR_DODERBLUE_B), false);
  printtextinfor8->SetText("Choose 'Ch' then enter button 'Draw'.");
  printtextinfor8->Resize(450, 12);
  printtextinfor8->SetEnabled(kFALSE);
  printtextinfor8->SetFrameDrawn(kFALSE);
  printtextinfor8->ChangeBackground(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));


  // draw
  OfflineDrawButton8 = new TGTextButton( parFrame, "&Draw", OFFLINEDRAW8);
  parFrame->AddFrame(OfflineDrawButton8, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 30, 0, 0));
  OfflineDrawButton8->SetEnabled(0);
  OfflineDrawButton8->Associate(this);
  OfflineDrawButton8->ChangeOptions(OfflineDrawButton8->GetOptions() ^ kRaisedFrame);
  OfflineDrawButton8->SetFont(TEXTBUTTONSMALL_FONT, false);
  OfflineDrawButton8->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  OfflineDrawButton8->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));

  // ch
  offlinechnum8 = new TGNumberEntry(parFrame, 0, 2, OFFLINECHNUM8, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, 15);
  parFrame->AddFrame(offlinechnum8, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 10, 0, 0));
  offlinechnum8->SetButtonToNum(0);
  offlinechnum8->Associate(this);
  offlinechnum8->GetNumberEntry()->ChangeOptions(offlinechnum8->GetNumberEntry()->GetOptions() ^ kRaisedFrame);
  offlinechnum8->GetNumberEntry()->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
  offlinechnum8->GetButtonUp()->ChangeOptions(offlinechnum8->GetButtonUp()->GetOptions() ^ kRaisedFrame);
  offlinechnum8->GetButtonDown()->ChangeOptions(offlinechnum8->GetButtonDown()->GetOptions() ^ kRaisedFrame);
  offlinechnum8->ChangeSubframesBackground(TColor::RGB2Pixel(TEXTENTRY_BG_R,TEXTENTRY_BG_G,TEXTENTRY_BG_B));
  
  TGLabel *ch = new TGLabel(parFrame, "Ch:"); 
  parFrame->AddFrame(ch, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 2, 3, 0));
  ch->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  ch->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B));  

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  // Draw Style
  TGCompositeFrame *drawstyleparFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);
  TabPanel->AddFrame(drawstyleparFrame, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 4, 4, 2, 4));
  drawstyleparFrame->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  TGLabel *LabelChooseDrawStyle = new TGLabel(drawstyleparFrame, "Draw Style:"); 
  drawstyleparFrame->AddFrame(LabelChooseDrawStyle, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 5, 0));
  LabelChooseDrawStyle->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
  LabelChooseDrawStyle->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  choosedrawstyle8 = new TGComboBox(drawstyleparFrame);
  drawstyleparFrame->AddFrame(choosedrawstyle8, new TGLayoutHints(kLHintsLeft, 0, 0, 2, 0));
  choosedrawstyle8->Resize(55, 20);
  choosedrawstyle8->AddEntry("Graph", 0);
  choosedrawstyle8->AddEntry("Hist", 1);
  choosedrawstyle8->Select(0);

  TGLabel *LabelChooseHistBinX = new TGLabel(drawstyleparFrame,"  'Hist' mode choose -> BinX:");
  drawstyleparFrame->AddFrame(LabelChooseHistBinX, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 2, 5, 0));
  LabelChooseHistBinX->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  LabelChooseHistBinX->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  choosehistbinxy[0] = new TGComboBox(drawstyleparFrame);
  drawstyleparFrame->AddFrame(choosehistbinxy[0], new TGLayoutHints(kLHintsLeft, 0, 0, 2, 0));
  choosehistbinxy[0]->Resize(50, 20);
  choosehistbinxy[0]->AddEntry("100", 100);
  choosehistbinxy[0]->AddEntry("200", 200);
  choosehistbinxy[0]->AddEntry("500", 500);
  choosehistbinxy[0]->AddEntry("1000", 1000);
  choosehistbinxy[0]->AddEntry("1500", 1500);
  choosehistbinxy[0]->AddEntry("2000", 2000);
  choosehistbinxy[0]->Select(1000);

  TGLabel *LabelChooseHistXmin = new TGLabel(drawstyleparFrame,"Xmin:");
  drawstyleparFrame->AddFrame(LabelChooseHistXmin, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 2, 5, 0));
  LabelChooseHistXmin->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  LabelChooseHistXmin->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  histxyminmax8[0] = new TGNumberEntryField(drawstyleparFrame, -1, 0, TGNumberFormat::kNESReal,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,0,65536);
  drawstyleparFrame->AddFrame(histxyminmax8[0], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 2, 0));
  histxyminmax8[0]->Resize(40, 20);
  
  TGLabel *LabelChooseHistXmax = new TGLabel(drawstyleparFrame,"Xmax:");
  drawstyleparFrame->AddFrame(LabelChooseHistXmax, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 2, 5, 0));
  LabelChooseHistXmax->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  LabelChooseHistXmax->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  histxyminmax8[1] = new TGNumberEntryField(drawstyleparFrame, -1, 1000, TGNumberFormat::kNESReal,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,0,65536);
  drawstyleparFrame->AddFrame(histxyminmax8[1], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 2, 0));
  histxyminmax8[1]->Resize(40, 20);  

  TGLabel *LabelChooseHistBinY = new TGLabel(drawstyleparFrame,"BinY:");
  drawstyleparFrame->AddFrame(LabelChooseHistBinY, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 2, 5, 0));
  LabelChooseHistBinY->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  LabelChooseHistBinY->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  choosehistbinxy[1] = new TGComboBox(drawstyleparFrame);
  drawstyleparFrame->AddFrame(choosehistbinxy[1], new TGLayoutHints(kLHintsLeft, 0, 0, 0, 0));
  choosehistbinxy[1]->Resize(50, 20);
  choosehistbinxy[1]->AddEntry("512", 512);
  choosehistbinxy[1]->AddEntry("1024", 1024);
  choosehistbinxy[1]->AddEntry("2048", 2048);
  choosehistbinxy[1]->Select(1024);

  TGLabel *LabelChooseHistYmin = new TGLabel(drawstyleparFrame,"Ymin:");
  drawstyleparFrame->AddFrame(LabelChooseHistYmin, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 2, 5, 0));
  LabelChooseHistYmin->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  LabelChooseHistYmin->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  histxyminmax8[2] = new TGNumberEntryField(drawstyleparFrame, -1, 0, TGNumberFormat::kNESReal,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,0,65536);
  drawstyleparFrame->AddFrame(histxyminmax8[2], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 2, 0));
  histxyminmax8[2]->Resize(40, 20);

  TGLabel *LabelChooseHistYmax = new TGLabel(drawstyleparFrame,"Ymax:");
  drawstyleparFrame->AddFrame(LabelChooseHistYmax, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 2, 5, 0));
  LabelChooseHistYmax->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  LabelChooseHistYmax->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  histxyminmax8[3] = new TGNumberEntryField(drawstyleparFrame, -1, 65536, TGNumberFormat::kNESReal,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,0,65536);
  drawstyleparFrame->AddFrame(histxyminmax8[3], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 2, 0));
  histxyminmax8[3]->Resize(40, 20);

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
  TabPanel->AddFrame(parFrame, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 4, 4, 4, 4));
  parFrame->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  // current count
  OfflineCurrentCountText9 = new TGTextEntry(parFrame,new TGTextBuffer(30), 10000);
  parFrame->AddFrame(OfflineCurrentCountText9, new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 0, 6, 0));
  OfflineCurrentCountText9-> SetFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1", false);
  OfflineCurrentCountText9->SetTextColor(TColor::RGB2Pixel(COLOR_DODERBLUE_R,COLOR_DODERBLUE_G,COLOR_DODERBLUE_B), false);
  OfflineCurrentCountText9->SetText("");
  OfflineCurrentCountText9->Resize(200, 12);
  OfflineCurrentCountText9->SetEnabled(kFALSE);
  OfflineCurrentCountText9->SetFrameDrawn(kFALSE);
  OfflineCurrentCountText9->ChangeBackground(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));  
  

  // draw
  OfflineDrawButton9 = new TGTextButton(parFrame, "&Draw", OFFLINEDRAW9);
  parFrame->AddFrame(OfflineDrawButton9, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 30, 0, 0));
  OfflineDrawButton9->SetEnabled(0);
  OfflineDrawButton9->Associate(this);
  OfflineDrawButton9->ChangeOptions(OfflineDrawButton9->GetOptions() ^ kRaisedFrame);
  OfflineDrawButton9->SetFont(TEXTBUTTONSMALL_FONT, false);
  OfflineDrawButton9->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  OfflineDrawButton9->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));
  
  // ch
  offlinechnum9 = new TGNumberEntry (parFrame, 0, 2, OFFLINECHNUM9, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, 15);
  parFrame->AddFrame(offlinechnum9, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 10, 0, 0));
  offlinechnum9->SetButtonToNum(0);
  offlinechnum9->Associate(this);
  offlinechnum9->GetNumberEntry()->ChangeOptions(offlinechnum9->GetNumberEntry()->GetOptions() ^ kRaisedFrame);
  offlinechnum9->GetNumberEntry()->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
  offlinechnum9->GetButtonUp()->ChangeOptions(offlinechnum9->GetButtonUp()->GetOptions() ^ kRaisedFrame);
  offlinechnum9->GetButtonDown()->ChangeOptions(offlinechnum9->GetButtonDown()->GetOptions() ^ kRaisedFrame);
  offlinechnum9->ChangeSubframesBackground(TColor::RGB2Pixel(TEXTENTRY_BG_R,TEXTENTRY_BG_G,TEXTENTRY_BG_B));
  
  TGLabel *ch = new TGLabel( parFrame, "Ch:"); 
  parFrame->AddFrame(ch, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 2, 3, 0));
  ch->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  ch->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B));    

  // Draw Style
  choosedrawstyle9 = new TGComboBox(parFrame);
  parFrame->AddFrame(choosedrawstyle9, new TGLayoutHints(kLHintsRight, 0, 20, 0, 0));
  choosedrawstyle9->Resize(100, 20);
  choosedrawstyle9->AddEntry("XIA", 0);
  choosedrawstyle9->AddEntry("fftw3", 1);
  choosedrawstyle9->AddEntry("CAEN(HANNING)", 10);
  choosedrawstyle9->AddEntry("CAEN(HAMMING)", 11);
  choosedrawstyle9->AddEntry("CAEN(BLACKMAN)", 12);
  choosedrawstyle9->AddEntry("CAEN(RECT)", 13);
  choosedrawstyle9->Select(0);
  
  TGLabel *LabelChooseDrawStyle = new TGLabel(parFrame, "Algorithm:"); 
  parFrame->AddFrame(LabelChooseDrawStyle, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 2, 5, 0));
  LabelChooseDrawStyle->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
  LabelChooseDrawStyle->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGCompositeFrame *adCanvasFrame = new TGCompositeFrame(TabPanel, 800, 800, kHorizontalFrame);
  TabPanel->AddFrame(adCanvasFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 4, 4, 0, 4));

  TRootEmbeddedCanvas *adjCanvas = new TRootEmbeddedCanvas("canvas9", adCanvasFrame, 100, 100);
  adCanvasFrame->AddFrame(adjCanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
  canvas9 = adjCanvas->GetCanvas();
}



void Offline::MakeFold10Panel(TGCompositeFrame *TabPanel)
{
  TGCompositeFrame *parFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);
  TabPanel->AddFrame(parFrame, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 4, 4, 4, 2));
  parFrame->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  // draw
  OfflineDrawButton10 = new TGTextButton(parFrame, "&Draw", OFFLINEDRAW10);
  parFrame->AddFrame(OfflineDrawButton10, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 30, 0, 0));
  OfflineDrawButton10->SetEnabled(0);
  OfflineDrawButton10->Associate(this);
  OfflineDrawButton10->ChangeOptions(OfflineDrawButton10->GetOptions() ^ kRaisedFrame);
  OfflineDrawButton10->SetFont(TEXTBUTTONSMALL_FONT, false);
  OfflineDrawButton10->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  OfflineDrawButton10->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));  


  // ch
  offlinechnumB10 = new TGNumberEntry(parFrame, 0, 2, OFFLINECHNUMB10, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, 15);
  parFrame->AddFrame(offlinechnumB10, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 10, 0, 0));
  offlinechnumB10->SetButtonToNum(0);
  offlinechnumB10->Associate(this);
  offlinechnumB10->GetNumberEntry()->ChangeOptions(offlinechnumB10->GetNumberEntry()->GetOptions() ^ kRaisedFrame);
  offlinechnumB10->GetNumberEntry()->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
  offlinechnumB10->GetButtonUp()->ChangeOptions(offlinechnumB10->GetButtonUp()->GetOptions() ^ kRaisedFrame);
  offlinechnumB10->GetButtonDown()->ChangeOptions(offlinechnumB10->GetButtonDown()->GetOptions() ^ kRaisedFrame);
  offlinechnumB10->ChangeSubframesBackground(TColor::RGB2Pixel(TEXTENTRY_BG_R,TEXTENTRY_BG_G,TEXTENTRY_BG_B));
  
  TGLabel *chB = new TGLabel(parFrame, "Ch B:"); 
  parFrame->AddFrame(chB, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 2, 3, 0)); 
  chB->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  chB->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B));  
  
  offlinechnumA10 = new TGNumberEntry(parFrame, 0, 2, OFFLINECHNUMA10, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, 15);
  parFrame->AddFrame(offlinechnumA10, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 10, 0, 0));
  offlinechnumA10->SetButtonToNum(0);
  offlinechnumA10->Associate(this);
  offlinechnumA10->GetNumberEntry()->ChangeOptions(offlinechnumA10->GetNumberEntry()->GetOptions() ^ kRaisedFrame);
  offlinechnumA10->GetNumberEntry()->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
  offlinechnumA10->GetButtonUp()->ChangeOptions(offlinechnumA10->GetButtonUp()->GetOptions() ^ kRaisedFrame);
  offlinechnumA10->GetButtonDown()->ChangeOptions(offlinechnumA10->GetButtonDown()->GetOptions() ^ kRaisedFrame);
  offlinechnumA10->ChangeSubframesBackground(TColor::RGB2Pixel(TEXTENTRY_BG_R,TEXTENTRY_BG_G,TEXTENTRY_BG_B));
  
  TGLabel *chA = new TGLabel(parFrame, "Ch A:"); 
  parFrame->AddFrame(chA, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 2, 3, 0));
  chA->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  chA->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B));  
 
  // text
  printtextinfor10 = new TGTextEntry(parFrame,new TGTextBuffer(30), 10000);
  parFrame->AddFrame(printtextinfor10, new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 0, 6, 0));
  printtextinfor10->SetFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1", false);
  printtextinfor10->SetTextColor(TColor::RGB2Pixel(COLOR_DODERBLUE_R,COLOR_DODERBLUE_G,COLOR_DODERBLUE_B), false);
  printtextinfor10->SetText("Choose 'Ch A' and 'Ch B', enter button 'Draw'.");
  printtextinfor10->Resize(350, 12);
  printtextinfor10->SetEnabled(kFALSE);
  printtextinfor10->SetFrameDrawn(kFALSE);
  printtextinfor10->ChangeBackground(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  // Fit
  GausFitButton10 = new TGTextButton(parFrame, "Open  Fit", OFFLINEGAUSFIT10);
  parFrame->AddFrame(GausFitButton10, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 30, 0, 0));
  GausFitButton10->SetEnabled(0);
  GausFitButton10->Associate(this);
  GausFitButton10->ChangeOptions(GausFitButton10->GetOptions() ^ kRaisedFrame);
  GausFitButton10->SetFont(TEXTBUTTONSMALL_FONT, false);
  GausFitButton10->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  GausFitButton10->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));

  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  // Draw Style
  TGCompositeFrame *drawstyleparFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);
  TabPanel->AddFrame(drawstyleparFrame, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 4, 4, 2, 4));
  drawstyleparFrame->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  TGLabel *LabelChooseDrawStyle = new TGLabel(drawstyleparFrame, "Draw Style:"); 
  drawstyleparFrame->AddFrame(LabelChooseDrawStyle, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 5, 0));
  LabelChooseDrawStyle->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
  LabelChooseDrawStyle->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  choosedrawstyle10 = new TGComboBox(drawstyleparFrame);
  drawstyleparFrame->AddFrame(choosedrawstyle10, new TGLayoutHints(kLHintsLeft, 0, 10, 0, 0));
  choosedrawstyle10->Resize(55, 20);
  choosedrawstyle10->AddEntry("CFD", 0);
  choosedrawstyle10->AddEntry("FF", 1);
  choosedrawstyle10->Select(0);

  
  TGLabel *LabelChooseHistXbin = new TGLabel(drawstyleparFrame,"Xbin:");
  drawstyleparFrame->AddFrame(LabelChooseHistXbin, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 2, 5, 0));
  LabelChooseHistXbin->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  LabelChooseHistXbin->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B));
  
  histxminmax10[0] = new TGNumberEntryField(drawstyleparFrame, -1, 10000, TGNumberFormat::kNESInteger,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,1,100000);
  drawstyleparFrame->AddFrame(histxminmax10[0], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 5, 2, 0));
  histxminmax10[0]->Resize(50, 20);
  
  TGLabel *LabelChooseHistXmin = new TGLabel(drawstyleparFrame,"Xmin(ns):");
  drawstyleparFrame->AddFrame(LabelChooseHistXmin, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 2, 5, 0));
  LabelChooseHistXmin->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  LabelChooseHistXmin->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B));
  
  histxminmax10[1] = new TGNumberEntryField(drawstyleparFrame, -1, -100, TGNumberFormat::kNESInteger,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,-TIMEDIFFGATE,TIMEDIFFGATE);
  drawstyleparFrame->AddFrame(histxminmax10[1], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 5, 2, 0));
  histxminmax10[1]->Resize(40, 20);
  
  TGLabel *LabelChooseHistXmax = new TGLabel(drawstyleparFrame,"Xmax(ns):");
  drawstyleparFrame->AddFrame(LabelChooseHistXmax, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 2, 5, 0));
  LabelChooseHistXmax->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  LabelChooseHistXmax->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B));
  
  histxminmax10[2] = new TGNumberEntryField(drawstyleparFrame, -1, 100, TGNumberFormat::kNESInteger,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,-TIMEDIFFGATE,TIMEDIFFGATE);
  drawstyleparFrame->AddFrame(histxminmax10[2], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 5, 2, 0));
  histxminmax10[2]->Resize(40, 20);  


  
  offlineenergylimit10 = new TGCheckButton(drawstyleparFrame, "Limits");
  drawstyleparFrame->AddFrame(offlineenergylimit10, new TGLayoutHints(kLHintsLeft | kLHintsTop, 35, 5, 5, 0));
  offlineenergylimit10->SetOn(kFALSE);
  offlineenergylimit10->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  offlineenergylimit10->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));


  TGLabel *LabelELimits0 = new TGLabel(drawstyleparFrame,"AL:");
  drawstyleparFrame->AddFrame(LabelELimits0, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 2, 5, 0));
  LabelELimits0->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  LabelELimits0->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  energylimitsab10[0] = new TGNumberEntryField(drawstyleparFrame, -1, 0, TGNumberFormat::kNESInteger,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,0,65536);
  drawstyleparFrame->AddFrame(energylimitsab10[0], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 5, 2, 0));
  energylimitsab10[0]->Resize(40, 20);

  TGLabel *LabelELimits1 = new TGLabel(drawstyleparFrame,"AR:");
  drawstyleparFrame->AddFrame(LabelELimits1, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 2, 5, 0));
  LabelELimits1->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  LabelELimits1->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  energylimitsab10[1] = new TGNumberEntryField(drawstyleparFrame, -1, 65536, TGNumberFormat::kNESInteger,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,0,65536);
  drawstyleparFrame->AddFrame(energylimitsab10[1], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 5, 2, 0));
  energylimitsab10[1]->Resize(40, 20);

  TGLabel *LabelELimits2 = new TGLabel(drawstyleparFrame,"BL:");
  drawstyleparFrame->AddFrame(LabelELimits2, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 2, 5, 0));
  LabelELimits2->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  LabelELimits2->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  energylimitsab10[2] = new TGNumberEntryField(drawstyleparFrame, -1, 0, TGNumberFormat::kNESInteger,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,0,65536);
  drawstyleparFrame->AddFrame(energylimitsab10[2], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 5, 2, 0));
  energylimitsab10[2]->Resize(40, 20);
  
  TGLabel *LabelELimits3 = new TGLabel(drawstyleparFrame,"BR:");
  drawstyleparFrame->AddFrame(LabelELimits3, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 2, 5, 0));
  LabelELimits3->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  LabelELimits3->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  energylimitsab10[3] = new TGNumberEntryField(drawstyleparFrame, -1, 65536, TGNumberFormat::kNESInteger,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,0,65536);
  drawstyleparFrame->AddFrame(energylimitsab10[3], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 5, 2, 0));
  energylimitsab10[3]->Resize(40, 20);
  

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGCompositeFrame *adCanvasFrame = new TGCompositeFrame(TabPanel, 800, 800, kHorizontalFrame);
  TGLayoutHints *Hint = new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 1, 1, 1, 1);

  TRootEmbeddedCanvas *adjCanvas = new TRootEmbeddedCanvas("canvas10", adCanvasFrame, 100, 100);

  canvas10 = adjCanvas->GetCanvas();
  adCanvasFrame->AddFrame(adjCanvas, Hint);
  TabPanel->AddFrame(adCanvasFrame, Hint);
}


void Offline::MakeFold11Panel(TGCompositeFrame *TabPanel)
{
  TGCompositeFrame *parFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);
  TabPanel->AddFrame(parFrame, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 4, 4, 4, 4));
  parFrame->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  // text
  printtextinfor11 = new TGTextEntry(parFrame,new TGTextBuffer(30), 10000);
  parFrame->AddFrame(printtextinfor11, new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 0, 6, 0));
  printtextinfor11->SetFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1", false);
  printtextinfor11->SetTextColor(TColor::RGB2Pixel(COLOR_DODERBLUE_R,COLOR_DODERBLUE_G,COLOR_DODERBLUE_B), false);
  printtextinfor11->SetText("Choose 'Bin/Ch' then enter button 'Draw'.");
  printtextinfor11->Resize(450, 12);
  printtextinfor11->SetEnabled(kFALSE);
  printtextinfor11->SetFrameDrawn(kFALSE);
  printtextinfor11->ChangeBackground(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  // draw
  OfflineDrawButton11 = new TGTextButton(parFrame, "&Draw", OFFLINEDRAW11);
  parFrame->AddFrame(OfflineDrawButton11, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 30, 0, 0));   
  OfflineDrawButton11->SetEnabled(0);
  OfflineDrawButton11->Associate(this);
  OfflineDrawButton11->ChangeOptions(OfflineDrawButton11->GetOptions() ^ kRaisedFrame);
  OfflineDrawButton11->SetFont(TEXTBUTTONSMALL_FONT, false);
  OfflineDrawButton11->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  OfflineDrawButton11->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));
  
  // ch
  offlinechnum11 = new TGNumberEntry(parFrame, 0, 2, OFFLINECHNUM11, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, 15);
  parFrame->AddFrame(offlinechnum11, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 10, 0, 0));
  offlinechnum11->SetButtonToNum(0);
  offlinechnum11->Associate(this);
  offlinechnum11->GetNumberEntry()->ChangeOptions(offlinechnum11->GetNumberEntry()->GetOptions() ^ kRaisedFrame);
  offlinechnum11->GetNumberEntry()->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
  offlinechnum11->GetButtonUp()->ChangeOptions(offlinechnum11->GetButtonUp()->GetOptions() ^ kRaisedFrame);
  offlinechnum11->GetButtonDown()->ChangeOptions(offlinechnum11->GetButtonDown()->GetOptions() ^ kRaisedFrame);
  offlinechnum11->ChangeSubframesBackground(TColor::RGB2Pixel(TEXTENTRY_BG_R,TEXTENTRY_BG_G,TEXTENTRY_BG_B));
  
  TGLabel *ch = new TGLabel(parFrame, "Ch:"); 
  parFrame->AddFrame(ch, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 2, 3, 0));
  ch->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  ch->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B));   

  // Draw Style
  choosedenergybin11 = new TGComboBox(parFrame);
  parFrame->AddFrame(choosedenergybin11, new TGLayoutHints(kLHintsRight, 0, 20, 0, 0));
  choosedenergybin11->Resize(100, 20);
  choosedenergybin11->AddEntry("4096(65536)", 0);
  choosedenergybin11->AddEntry("2048(65536)", 1);
  choosedenergybin11->AddEntry("1024(65536)", 2);
  choosedenergybin11->AddEntry("4096(32768)", 3);
  choosedenergybin11->AddEntry("2048(32768)", 4);
  choosedenergybin11->AddEntry("1024(32768)", 5);
  choosedenergybin11->AddEntry("4096(16384)", 6);
  choosedenergybin11->AddEntry("2048(16384)", 7);
  choosedenergybin11->AddEntry("1024(16384)", 8);
  choosedenergybin11->AddEntry("5000(10000)", 9);
  choosedenergybin11->AddEntry("2000(10000)", 10);
  choosedenergybin11->AddEntry("5000(5000)", 11);
  choosedenergybin11->AddEntry("2500(5000)", 12);
  choosedenergybin11->AddEntry("2000(2000)", 13);
  choosedenergybin11->AddEntry("1000(1000)", 14);  
  choosedenergybin11->Select(0);
  
  TGLabel *LabelChooseEnergyBinDrawStyle = new TGLabel(parFrame, "E Bin:"); 
  parFrame->AddFrame(LabelChooseEnergyBinDrawStyle, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 2, 3, 0));
  LabelChooseEnergyBinDrawStyle->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  LabelChooseEnergyBinDrawStyle->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B));
  
  choosedcfdbin11 = new TGComboBox(parFrame);
  parFrame->AddFrame(choosedcfdbin11, new TGLayoutHints(kLHintsRight, 0, 20, 0, 0));
  choosedcfdbin11->Resize(50, 20);
  choosedcfdbin11->AddEntry("4096", 0);
  choosedcfdbin11->AddEntry("2048", 1);
  choosedcfdbin11->AddEntry("1024", 2);
  choosedcfdbin11->Select(0);
  
  TGLabel *LabelChooseCFDBinDrawStyle = new TGLabel(parFrame, "CFD Bin:"); 
  parFrame->AddFrame(LabelChooseCFDBinDrawStyle, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 2, 3, 0));  
  LabelChooseCFDBinDrawStyle->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  LabelChooseCFDBinDrawStyle->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B));
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGCompositeFrame *adCanvasFrame = new TGCompositeFrame(TabPanel, 800, 800, kHorizontalFrame);
  TabPanel->AddFrame(adCanvasFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 4, 4, 0, 4));

  TRootEmbeddedCanvas *adjCanvas = new TRootEmbeddedCanvas("canvas11", adCanvasFrame, 100, 100);
  adCanvasFrame->AddFrame(adjCanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
  canvas11 = adjCanvas->GetCanvas();
}


void Offline::MakeFold12Panel(TGCompositeFrame *TabPanel)
{
  TGCompositeFrame *parFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);
  TabPanel->AddFrame(parFrame, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 4, 4, 4, 4));
  parFrame->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  // text
  printtextinfor12 = new TGTextEntry(parFrame,new TGTextBuffer(30), 10000);
  parFrame->AddFrame(printtextinfor12, new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 0, 6, 0));
  printtextinfor12->SetFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1", false);
  printtextinfor12->SetTextColor(TColor::RGB2Pixel(COLOR_DODERBLUE_R,COLOR_DODERBLUE_G,COLOR_DODERBLUE_B), false);
  printtextinfor12->SetText("Choose 'Ch' then enter button 'Draw'.");
  printtextinfor12->Resize(450, 12);
  printtextinfor12->SetEnabled(kFALSE);
  printtextinfor12->SetFrameDrawn(kFALSE);
  printtextinfor12->ChangeBackground(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));  
    
  
  // draw
  OfflineDrawButton12 = new TGTextButton(parFrame, "&Draw", OFFLINEDRAW12);
  parFrame->AddFrame(OfflineDrawButton12, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 30, 0, 0));   
  OfflineDrawButton12->SetEnabled(0);
  OfflineDrawButton12->Associate(this);
  OfflineDrawButton12->ChangeOptions(OfflineDrawButton12->GetOptions() ^ kRaisedFrame);
  OfflineDrawButton12->SetFont(TEXTBUTTONSMALL_FONT, false);
  OfflineDrawButton12->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  OfflineDrawButton12->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));

  
  // ch
  offlinechnum12 = new TGNumberEntry (parFrame, 0, 2, OFFLINECHNUM12, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, 15);
  parFrame->AddFrame(offlinechnum12, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 10, 0, 0));
  offlinechnum12->SetButtonToNum(0);
  offlinechnum12->Associate(this);
  offlinechnum12->GetNumberEntry()->ChangeOptions(offlinechnum12->GetNumberEntry()->GetOptions() ^ kRaisedFrame);
  offlinechnum12->GetNumberEntry()->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
  offlinechnum12->GetButtonUp()->ChangeOptions(offlinechnum12->GetButtonUp()->GetOptions() ^ kRaisedFrame);
  offlinechnum12->GetButtonDown()->ChangeOptions(offlinechnum12->GetButtonDown()->GetOptions() ^ kRaisedFrame);
  offlinechnum12->ChangeSubframesBackground(TColor::RGB2Pixel(TEXTENTRY_BG_R,TEXTENTRY_BG_G,TEXTENTRY_BG_B));
  
  TGLabel *ch = new TGLabel(parFrame, "Ch:"); 
  parFrame->AddFrame(ch, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 2, 3, 0));
  ch->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  ch->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B));     

  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGCompositeFrame *adCanvasFrame = new TGCompositeFrame(TabPanel, 800, 800, kHorizontalFrame);
  TabPanel->AddFrame(adCanvasFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 4, 4, 0, 4));

  TRootEmbeddedCanvas *adjCanvas = new TRootEmbeddedCanvas("canvas12", adCanvasFrame, 100, 100);
  adCanvasFrame->AddFrame(adjCanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
  canvas12 = adjCanvas->GetCanvas();
}

void Offline::MakeFold13Panel(TGCompositeFrame *TabPanel)
{
  TGCompositeFrame *parFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);
  TabPanel->AddFrame(parFrame, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 4, 4, 4, 4));
  parFrame->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  // draw
  OfflineDrawButton13 = new TGTextButton(parFrame, "&Draw", OFFLINEDRAW13);
  parFrame->AddFrame(OfflineDrawButton13, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 30, 0, 0));
  OfflineDrawButton13->SetEnabled(0);
  OfflineDrawButton13->Associate(this);
  OfflineDrawButton13->ChangeOptions(OfflineDrawButton13->GetOptions() ^ kRaisedFrame);
  OfflineDrawButton13->SetFont(TEXTBUTTONSMALL_FONT, false);
  OfflineDrawButton13->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  OfflineDrawButton13->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));	


  // ch
  offlinechnumB13 = new TGNumberEntry(parFrame, 0, 2, OFFLINECHNUMB13, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, 15);
  parFrame->AddFrame(offlinechnumB13, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 10, 0, 0));
  offlinechnumB13->SetButtonToNum(0);
  offlinechnumB13->Associate(this);
  offlinechnumB13->GetNumberEntry()->ChangeOptions(offlinechnumB13->GetNumberEntry()->GetOptions() ^ kRaisedFrame);
  offlinechnumB13->GetNumberEntry()->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
  offlinechnumB13->GetButtonUp()->ChangeOptions(offlinechnumB13->GetButtonUp()->GetOptions() ^ kRaisedFrame);
  offlinechnumB13->GetButtonDown()->ChangeOptions(offlinechnumB13->GetButtonDown()->GetOptions() ^ kRaisedFrame);
  offlinechnumB13->ChangeSubframesBackground(TColor::RGB2Pixel(TEXTENTRY_BG_R,TEXTENTRY_BG_G,TEXTENTRY_BG_B));
  
  TGLabel *chB = new TGLabel(parFrame, "Ch B:"); 
  parFrame->AddFrame(chB, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 2, 3, 0)); 
  chB->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  chB->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B));  
  
  offlinechnumA13 = new TGNumberEntry(parFrame, 0, 2, OFFLINECHNUMA13, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, 15);
  parFrame->AddFrame(offlinechnumA13, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 10, 0, 0));
  offlinechnumA13->SetButtonToNum(0);
  offlinechnumA13->Associate(this);
  offlinechnumA13->GetNumberEntry()->ChangeOptions(offlinechnumA13->GetNumberEntry()->GetOptions() ^ kRaisedFrame);
  offlinechnumA13->GetNumberEntry()->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
  offlinechnumA13->GetButtonUp()->ChangeOptions(offlinechnumA13->GetButtonUp()->GetOptions() ^ kRaisedFrame);
  offlinechnumA13->GetButtonDown()->ChangeOptions(offlinechnumA13->GetButtonDown()->GetOptions() ^ kRaisedFrame);
  offlinechnumA13->ChangeSubframesBackground(TColor::RGB2Pixel(TEXTENTRY_BG_R,TEXTENTRY_BG_G,TEXTENTRY_BG_B));
  
  TGLabel *chA = new TGLabel(parFrame, "Ch A:"); 
  parFrame->AddFrame(chA, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 2, 3, 0));
  chA->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  chA->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B));  


  choosedelay13 = new TGComboBox(parFrame);
  parFrame->AddFrame(choosedelay13, new TGLayoutHints(kLHintsRight, 0, 20, 0, 0));
  choosedelay13->Resize(100, 20);
  choosedelay13->AddEntry("Delay 1-5", 5);
  choosedelay13->AddEntry("Delay 1-10", 10);
  choosedelay13->AddEntry("Delay 1-15", 15);
  choosedelay13->AddEntry("Delay 1-20", 20);
  choosedelay13->Select(5);
    
  // stop
  OfflineStopButton13 = new TGTextButton(parFrame, "&Stop", OFFLINESTOPDRAW13);
  parFrame->AddFrame(OfflineStopButton13, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 0, 0));
  OfflineStopButton13->SetEnabled(0);
  OfflineStopButton13->Associate(this);
  OfflineStopButton13->ChangeOptions(OfflineStopButton13->GetOptions() ^ kRaisedFrame);
  OfflineStopButton13->SetFont(TEXTBUTTONSMALL_FONT, false);
  OfflineStopButton13->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  OfflineStopButton13->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));

  
  // text
  printtextinfor13 = new TGTextEntry(parFrame,new TGTextBuffer(30), 10000);
  parFrame->AddFrame(printtextinfor13, new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 0, 6, 0));
  printtextinfor13->SetFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1", false);
  printtextinfor13->SetTextColor(TColor::RGB2Pixel(COLOR_DODERBLUE_R,COLOR_DODERBLUE_G,COLOR_DODERBLUE_B), false);
  printtextinfor13->SetText("Choose 'Ch A' and 'Ch B', enter button 'Draw'.");
  printtextinfor13->Resize(350, 12);
  printtextinfor13->SetEnabled(kFALSE);
  printtextinfor13->SetFrameDrawn(kFALSE);
  printtextinfor13->ChangeBackground(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  // style

    
  TGLabel *LabelChooseHistXbin = new TGLabel(parFrame,"Xbin:");
  parFrame->AddFrame(LabelChooseHistXbin, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 2, 5, 0));
  LabelChooseHistXbin->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  LabelChooseHistXbin->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B));
  
  histxminmax13[0] = new TGNumberEntryField(parFrame, -1, 10000, TGNumberFormat::kNESInteger,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,1,100000);
  parFrame->AddFrame(histxminmax13[0], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 5, 2, 0));
  histxminmax13[0]->Resize(50, 20);
  
  TGLabel *LabelChooseHistXmin = new TGLabel(parFrame,"Xmin(ns):");
  parFrame->AddFrame(LabelChooseHistXmin, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 2, 5, 0));
  LabelChooseHistXmin->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  LabelChooseHistXmin->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B));
  
  histxminmax13[1] = new TGNumberEntryField(parFrame, -1, -100, TGNumberFormat::kNESInteger,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,-500,500);
  parFrame->AddFrame(histxminmax13[1], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 5, 2, 0));
  histxminmax13[1]->Resize(40, 20);
  
  TGLabel *LabelChooseHistXmax = new TGLabel(parFrame,"Xmax(ns):");
  parFrame->AddFrame(LabelChooseHistXmax, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 2, 5, 0));
  LabelChooseHistXmax->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  LabelChooseHistXmax->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B));
  
  histxminmax13[2] = new TGNumberEntryField(parFrame, -1, 100, TGNumberFormat::kNESInteger,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,-500,500);
  parFrame->AddFrame(histxminmax13[2], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 5, 2, 0));
  histxminmax13[2]->Resize(40, 20);  



  
  offlineenergylimit13 = new TGCheckButton(parFrame, "Limits");
  parFrame->AddFrame(offlineenergylimit13, new TGLayoutHints(kLHintsLeft | kLHintsTop, 35, 5, 5, 0));
  offlineenergylimit13->SetOn(kFALSE);
  offlineenergylimit13->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  offlineenergylimit13->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));


  TGLabel *LabelELimits0 = new TGLabel(parFrame,"AL:");
  parFrame->AddFrame(LabelELimits0, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 2, 5, 0));
  LabelELimits0->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  LabelELimits0->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  energylimitsab13[0] = new TGNumberEntryField(parFrame, -1, 0, TGNumberFormat::kNESInteger,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,0,65536);
  parFrame->AddFrame(energylimitsab13[0], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 5, 2, 0));
  energylimitsab13[0]->Resize(40, 20);

  TGLabel *LabelELimits1 = new TGLabel(parFrame,"AR:");
  parFrame->AddFrame(LabelELimits1, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 2, 5, 0));
  LabelELimits1->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  LabelELimits1->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  energylimitsab13[1] = new TGNumberEntryField(parFrame, -1, 65536, TGNumberFormat::kNESInteger,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,0,65536);
  parFrame->AddFrame(energylimitsab13[1], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 5, 2, 0));
  energylimitsab13[1]->Resize(40, 20);

  TGLabel *LabelELimits2 = new TGLabel(parFrame,"BL:");
  parFrame->AddFrame(LabelELimits2, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 2, 5, 0));
  LabelELimits2->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  LabelELimits2->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  energylimitsab13[2] = new TGNumberEntryField(parFrame, -1, 0, TGNumberFormat::kNESInteger,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,0,65536);
  parFrame->AddFrame(energylimitsab13[2], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 5, 2, 0));
  energylimitsab13[2]->Resize(40, 20);
  
  TGLabel *LabelELimits3 = new TGLabel(parFrame,"BR:");
  parFrame->AddFrame(LabelELimits3, new TGLayoutHints(kLHintsLeft | kLHintsTop, 3, 2, 5, 0));
  LabelELimits3->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  LabelELimits3->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  energylimitsab13[3] = new TGNumberEntryField(parFrame, -1, 65536, TGNumberFormat::kNESInteger,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax,0,65536);
  parFrame->AddFrame(energylimitsab13[3], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 5, 2, 0));
  energylimitsab13[3]->Resize(40, 20);
  
  
  

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGCompositeFrame *adCanvasFrame = new TGCompositeFrame(TabPanel, 800, 800, kHorizontalFrame);
  TabPanel->AddFrame(adCanvasFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 4, 4, 0, 4));

  TRootEmbeddedCanvas *adjCanvas = new TRootEmbeddedCanvas("canvas13", adCanvasFrame, 100, 100);
  adCanvasFrame->AddFrame(adjCanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
  canvas13 = adjCanvas->GetCanvas();
}

void Offline::MakeFold14Panel(TGCompositeFrame *TabPanel)
{
  TGCompositeFrame *parFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);
  TabPanel->AddFrame(parFrame, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 4, 4, 4, 4));
  parFrame->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  // stop
  OfflineStopButton14 = new TGTextButton(parFrame, "&Stop", OFFLINESTOPDRAW14);
  parFrame->AddFrame(OfflineStopButton14, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 0, 0));
  OfflineStopButton14->SetEnabled(0);
  OfflineStopButton14->Associate(this);
  OfflineStopButton14->ChangeOptions(OfflineStopButton14->GetOptions() ^ kRaisedFrame);
  OfflineStopButton14->SetFont(TEXTBUTTONSMALL_FONT, false);
  OfflineStopButton14->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  OfflineStopButton14->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));

  // text
  printtextinfor14 = new TGTextEntry(parFrame,new TGTextBuffer(30), 10000);
  parFrame->AddFrame(printtextinfor14, new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 0, 3, 0));
  printtextinfor14->SetFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1", false);
  printtextinfor14->SetTextColor(TColor::RGB2Pixel(COLOR_DODERBLUE_R,COLOR_DODERBLUE_G,COLOR_DODERBLUE_B), false);
  printtextinfor14->SetText("Choose 'Ch' then enter button 'Draw'.");
  printtextinfor14->Resize(500, 12);
  printtextinfor14->SetEnabled(kFALSE);
  printtextinfor14->SetFrameDrawn(kFALSE);
  printtextinfor14->ChangeBackground(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));  

  // draw
  OfflineDrawButton14 = new TGTextButton(parFrame, "&Draw", OFFLINEDRAW14);
  parFrame->AddFrame(OfflineDrawButton14, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 30, 0, 0));
  OfflineDrawButton14->SetEnabled(0);
  OfflineDrawButton14->Associate(this);
  OfflineDrawButton14->ChangeOptions(OfflineDrawButton14->GetOptions() ^ kRaisedFrame);
  OfflineDrawButton14->SetFont(TEXTBUTTONSMALL_FONT, false);
  OfflineDrawButton14->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  OfflineDrawButton14->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));

  // ch
  offlinechnum14 = new TGNumberEntry (parFrame, 0, 2, OFFLINECHNUM14, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, 15);
  parFrame->AddFrame(offlinechnum14, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 10, 0, 0));
  offlinechnum14->SetButtonToNum(0);
  offlinechnum14->Associate(this);
  offlinechnum14->GetNumberEntry()->ChangeOptions(offlinechnum14->GetNumberEntry()->GetOptions() ^ kRaisedFrame);
  offlinechnum14->GetNumberEntry()->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
  offlinechnum14->GetButtonUp()->ChangeOptions(offlinechnum14->GetButtonUp()->GetOptions() ^ kRaisedFrame);
  offlinechnum14->GetButtonDown()->ChangeOptions(offlinechnum14->GetButtonDown()->GetOptions() ^ kRaisedFrame);
  offlinechnum14->ChangeSubframesBackground(TColor::RGB2Pixel(TEXTENTRY_BG_R,TEXTENTRY_BG_G,TEXTENTRY_BG_B));
  
  TGLabel *ch = new TGLabel(parFrame, "Ch:"); 
  parFrame->AddFrame(ch, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 2, 3, 0));
  ch->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  ch->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B));  

  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGCompositeFrame *adCanvasFrame = new TGCompositeFrame(TabPanel, 800, 800, kHorizontalFrame);
  TabPanel->AddFrame(adCanvasFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 4, 4, 0, 4));

  TRootEmbeddedCanvas *adjCanvas = new TRootEmbeddedCanvas("canvas14", adCanvasFrame, 100, 100);
  adCanvasFrame->AddFrame(adjCanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
  canvas14 = adjCanvas->GetCanvas();
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
	      if(adjustdslider)
		{
		  offlinemultigraph->GetXaxis()->SetRangeUser(dslider->GetMinPosition(),dslider->GetMaxPosition());
		  // std::cout<<"Slider Pos -  min:"<<dslider->GetMinPosition()<<" max:"<<dslider->GetMaxPosition()<<std::endl;
		}
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
		  if(offlinechnum->GetIntNumber() == offlinedata->GetEventChannel(i) && offlinedata->GetEventTraceLength(i) > 0)//ch / trace length > 0
		    {
		      dslider->SetRange(0,offlinedata->GetEventTraceLength(i));//trace length
		      dslider->SetPosition(0,offlinedata->GetEventTraceLength(i));//trace length
		      break;
		    }
		}
	      break;
	    case OFFLINELOAD:
	      OfflineLoadValues(offlinemodnum->GetIntNumber(),offlinechnum->GetIntNumber());
	      break;
	    case OFFLINEAPPLY:
	      OfflineChangeValues(offlinemodnum->GetIntNumber(),offlinechnum->GetIntNumber());
	      OfflineLoadValues(offlinemodnum->GetIntNumber(),offlinechnum->GetIntNumber());
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
	    case OFFLINEDRAW10:
	      Panel10Draw();
	      break;
	    case OFFLINEDRAW11:
	      Panel11Draw();
	      break;
	    case OFFLINEDRAW12:
	      Panel12Draw();
	      break;
	    case OFFLINEDRAW13:
	      Panel13Draw();
	      break;
	    case OFFLINEDRAW14:
	      Panel14Draw();
	      break;

	      
	    case OFFLINEGAUSFIT4:
	      GausFit4();
	      break;
	    case OFFLINEGAUSFIT6:
	      GausFit6();
	      break;
	    case OFFLINEGAUSFIT10:
	      GausFit10();
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
	    case OFFLINESTOPDRAW13:
	      Panel13StopDraw();
	      break;	
	    case OFFLINESTOPDRAW14:
	      Panel14StopDraw();
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
	      DrawButtonStatus(false);
	      
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
	      DrawButtonStatus(false);
	      
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
	      for (unsigned int i = 0; i < OfflineModuleEventsCount; ++i)//when change channel, X axis range return to [0,sample]
		{
		  if(offlinechnum->GetIntNumber() == offlinedata->GetEventChannel(i) && offlinedata->GetEventTraceLength(i) > 0)//ch / trace length > 0
		    {
		      dslider->SetRange(0,offlinedata->GetEventTraceLength(i));//trace length
		      dslider->SetPosition(0,offlinedata->GetEventTraceLength(i));//trace length
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
	    case OFFLINECHNUM9:
	      if (parm2 == 0)
		{
		  if (chanNumber9 != 15)
		    {
		      ++chanNumber9;
		      offlinechnum9->SetIntNumber(chanNumber9);
		    }
		}
	      else
		{
		  if (chanNumber9 != 0)
		    {
		      if (--chanNumber9 == 0)
			chanNumber9 = 0;
		      offlinechnum9->SetIntNumber(chanNumber9);
		    }
		}
	      break;
	    case OFFLINECHNUMA10:
	      if (parm2 == 0)
		{
		  if (chanNumberA10 != 15)
		    {
		      ++chanNumberA10;
		      offlinechnumA10->SetIntNumber(chanNumberA10);
		    }
		}
	      else
		{
		  if (chanNumberA10 != 0)
		    {
		      if (--chanNumberA10 == 0)
			chanNumberA10 = 0;
		      offlinechnumA10->SetIntNumber(chanNumberA10);
		    }
		}
	      break;
	    case OFFLINECHNUMB10:
	      if (parm2 == 0)
		{
		  if (chanNumberB10 != 15)
		    {
		      ++chanNumberB10;
		      offlinechnumB10->SetIntNumber(chanNumberB10);
		    }
		}
	      else
		{
		  if (chanNumberB10 != 0)
		    {
		      if (--chanNumberB10 == 0)
			chanNumberB10 = 0;
		      offlinechnumB10->SetIntNumber(chanNumberB10);
		    }
		}
	      break;
	    case OFFLINECHNUM11:
	      if (parm2 == 0)
		{
		  if (chanNumber11 != 15)
		    {
		      ++chanNumber11;
		      offlinechnum11->SetIntNumber(chanNumber11);
		    }
		}
	      else
		{
		  if (chanNumber11 != 0)
		    {
		      if (--chanNumber11 == 0)
			chanNumber11 = 0;
		      offlinechnum11->SetIntNumber(chanNumber11);
		    }
		}
	      break;	      	      
	    case OFFLINECHNUM12:
	      if (parm2 == 0)
		{
		  if (chanNumber12 != 15)
		    {
		      ++chanNumber12;
		      offlinechnum12->SetIntNumber(chanNumber12);
		    }
		}
	      else
		{
		  if (chanNumber12 != 0)
		    {
		      if (--chanNumber12 == 0)
			chanNumber12 = 0;
		      offlinechnum12->SetIntNumber(chanNumber12);
		    }
		}
	      break;
	    case OFFLINECHNUMA13:
	      if (parm2 == 0)
		{
		  if (chanNumberA13 != 15)
		    {
		      ++chanNumberA13;
		      offlinechnumA13->SetIntNumber(chanNumberA13);
		    }
		}
	      else
		{
		  if (chanNumberA13 != 0)
		    {
		      if (--chanNumberA13 == 0)
			chanNumberA13 = 0;
		      offlinechnumA13->SetIntNumber(chanNumberA13);
		    }
		}
	      break;
	    case OFFLINECHNUMB13:
	      if (parm2 == 0)
		{
		  if (chanNumberB13 != 15)
		    {
		      ++chanNumberB13;
		      offlinechnumB13->SetIntNumber(chanNumberB13);
		    }
		}
	      else
		{
		  if (chanNumberB13 != 0)
		    {
		      if (--chanNumberB13 == 0)
			chanNumberB13 = 0;
		      offlinechnumB13->SetIntNumber(chanNumberB13);
		    }
		}
	      break;
	    case OFFLINECHNUM14:
	      if (parm2 == 0)
		{
		  if (chanNumber14 != 15)
		    {
		      ++chanNumber14;
		      offlinechnum14->SetIntNumber(chanNumber14);
		    }
		}
	      else
		{
		  if (chanNumber14 != 0)
		    {
		      if (--chanNumber14 == 0)
			chanNumber14 = 0;
		      offlinechnum14->SetIntNumber(chanNumber14);
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
	      DrawButtonStatus(false);
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
	      chanNumber6 = offlinechnum6->GetIntNumber();
	      if(chanNumber6 > 15) chanNumber6 = 15;
	      if(chanNumber6 < 0) chanNumber6 = 0;
	      offlinechnum6->SetIntNumber(chanNumber6);
	      break;
	    default:
	      break;
	    }
	  break;
	case OFFLINECHNUM8:
	  switch (GET_SUBMSG(msg))
	    {
	    case kTE_ENTER:
	      chanNumber8 = offlinechnum8->GetIntNumber();
	      if(chanNumber8 > 15) chanNumber8 = 15;
	      if(chanNumber8 < 0) chanNumber8 = 0;
	      offlinechnum8->SetIntNumber(chanNumber8);
	      break;
	    default:
	      break;
	    }
	  break;
	case OFFLINECHNUM9:
	  switch (GET_SUBMSG(msg))
	    {
	    case kTE_ENTER:
	      chanNumber9 = offlinechnum9->GetIntNumber();
	      if(chanNumber9 > 15) chanNumber9 = 15;
	      if(chanNumber9 < 0) chanNumber9 = 0;
	      offlinechnum9->SetIntNumber(chanNumber9);
	      break;
	    default:
	      break;
	    }
	  break;
	case OFFLINECHNUMA10:
	  switch (GET_SUBMSG(msg))
	    {
	    case kTE_ENTER:
	      chanNumberA10 = offlinechnumA10->GetIntNumber();
	      if(chanNumberA10 > 15) chanNumberA10 = 15;
	      if(chanNumberA10 < 0) chanNumberA10 = 0;
	      offlinechnumA10->SetIntNumber(chanNumberA10);
	      break;
	    default:
	      break;
	    }
	  break;
	case OFFLINECHNUMB10:
	  switch (GET_SUBMSG(msg))
	    {
	    case kTE_ENTER:
	      chanNumberB10 = offlinechnumB10->GetIntNumber();
	      if(chanNumberB10 > 15) chanNumberB10 = 15;
	      if(chanNumberB10 < 0) chanNumberB10 = 0;
	      offlinechnumB10->SetIntNumber(chanNumberB10);
	      break;
	    default:
	      break;
	    }
	  break;
	case OFFLINECHNUM11:
	  switch (GET_SUBMSG(msg))
	    {
	    case kTE_ENTER:
	      chanNumber11 = offlinechnum11->GetIntNumber();
	      if(chanNumber11 > 15) chanNumber11 = 15;
	      if(chanNumber11 < 0) chanNumber11 = 0;
	      offlinechnum11->SetIntNumber(chanNumber11);
	      break;
	    default:
	      break;
	    }
	  break;	  
	case OFFLINECHNUM12:
	  switch (GET_SUBMSG(msg))
	    {
	    case kTE_ENTER:
	      chanNumber12 = offlinechnum12->GetIntNumber();
	      if(chanNumber12 > 15) chanNumber12 = 15;
	      if(chanNumber12 < 0) chanNumber12 = 0;
	      offlinechnum12->SetIntNumber(chanNumber12);
	      break;
	    default:
	      break;
	    }
	  break;	  
	case OFFLINECHNUMA13:
	  switch (GET_SUBMSG(msg))
	    {
	    case kTE_ENTER:
	      chanNumberA13 = offlinechnumA13->GetIntNumber();
	      if(chanNumberA13 > 15) chanNumberA13 = 15;
	      if(chanNumberA13 < 0) chanNumberA13 = 0;
	      offlinechnumA13->SetIntNumber(chanNumberA13);
	      break;
	    default:
	      break;
	    }
	  break;
	case OFFLINECHNUMB13:
	  switch (GET_SUBMSG(msg))
	    {
	    case kTE_ENTER:
	      chanNumberB13 = offlinechnumB13->GetIntNumber();
	      if(chanNumberB13 > 15) chanNumberB13 = 15;
	      if(chanNumberB13 < 0) chanNumberB13 = 0;
	      offlinechnumB13->SetIntNumber(chanNumberB13);
	      break;
	    default:
	      break;
	    }
	  break;
	case OFFLINECHNUM14:
	  switch (GET_SUBMSG(msg))
	    {
	    case kTE_ENTER:
	      chanNumber14 = offlinechnum14->GetIntNumber();
	      if(chanNumber14 > 15) chanNumber14 = 15;
	      if(chanNumber14 < 0) chanNumber14 = 0;
	      offlinechnum14->SetIntNumber(chanNumber14);
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
	      DrawButtonStatus(false);
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
  
  retval = Pixie16ReadSglChanPar((char*)"TRIGGER_RISETIME", &ChanParData, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cc", "OfflineLoadValues(...)", "Pixie16ReadSglChanPar/TRIGGER_RISETIME", retval);
  offlinefilters[0]->SetText(TString::Format("%1.3f", ChanParData).Data());

  retval = Pixie16ReadSglChanPar((char*)"TRIGGER_FLATTOP", &ChanParData, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cc", "OfflineLoadValues(...)", "Pixie16ReadSglChanPar/TRIGGER_FLATTOP", retval);  
  offlinefilters[1]->SetText(TString::Format("%1.3f", ChanParData).Data());

  retval = Pixie16ReadSglChanPar((char*)"ENERGY_RISETIME", &ChanParData, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cc", "OfflineLoadValues(...)", "Pixie16ReadSglChanPar/ENERGY_RISETIME", retval);
  offlinefilters[2]->SetText(TString::Format("%1.3f", ChanParData).Data());

  retval = Pixie16ReadSglChanPar((char*)"ENERGY_FLATTOP", &ChanParData, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cc", "OfflineLoadValues(...)", "Pixie16ReadSglChanPar/ENERGY_FLATTOP", retval);  
  offlinefilters[3]->SetText(TString::Format("%1.3f", ChanParData).Data());

  retval = Pixie16ReadSglChanPar((char*)"TAU", &ChanParData, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cc", "OfflineLoadValues(...)", "Pixie16ReadSglChanPar/TAU", retval);
  offlinefilters[4]->SetText(TString::Format("%1.3f", ChanParData).Data());

  retval = Pixie16ReadSglChanPar((char*)"CFDDelay", &ChanParData, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cc", "OfflineLoadValues(...)", "Pixie16ReadSglChanPar/CFDDelay", retval);
  offlinefilters[5]->SetText(TString::Format("%1.3f", ChanParData).Data());

  retval = Pixie16ReadSglChanPar((char*)"CFDScale", &ChanParData, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cc", "OfflineLoadValues(...)", "Pixie16ReadSglChanPar/CFDScale", retval); 
  offlinefilters[6]->SetText(TString::Format("%d", (int)ChanParData).Data());

  retval = Pixie16ReadSglChanPar((char*)"TRIGGER_THRESHOLD", &ChanParData, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cc", "OfflineLoadValues(...)", "Pixie16ReadSglChanPar/TRIGGER_THRESHOLD", retval);
  offlinefilters[7]->SetText(TString::Format("%d", (int)ChanParData).Data());

  retval = Pixie16ReadSglChanPar((char*)"CFDThresh", &ChanParData, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cc", "OfflineLoadValues(...)", "Pixie16ReadSglChanPar/CFDThresh", retval);     
  offlinefilters[8]->SetText(TString::Format("%d", (int)ChanParData).Data());


  
  retval = Pixie16ReadSglModPar((char*)"SLOW_FILTER_RANGE", &OfflinefRange, mod);
  if(retval < 0) ErrorInfo("Offline.cc", "OfflineLoadValues(...)", "Pixie16ReadSglModPar/SLOW_FILTER_RANGE", retval);
  offlinefilterrange->SetIntNumber(OfflinefRange);
}

void Offline::Panel1Draw()
{
  adjustdslider = false;
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
  if(cfdsdata != NULL)
    {
      delete cfdsdata;
      cfdsdata = NULL;
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
  if(doublecfds != NULL)
    {
      delete []doublecfds;
      doublecfds = NULL;
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
      OfflineCurrentCount = offlinecurrentcountentry->GetIntNumber();
    }

  // ===

  // if(offlineonlywaveformevent->IsOn())
  //   {
  //     for(;;)
  // 	{
  // 	  tempN1++;
  // 	  if(tempN1 >= OfflineModuleEventsCount)
  // 	    {
  // 	      offlinedatastatus = true;
  // 	      break;
  // 	    }

  // 	  // OfflineCurrentCount++;
  // 	  if(OfflineCurrentCount == OfflineModuleEventsCount) OfflineCurrentCount = 0;

  // 	  if(offlinechnum->GetIntNumber() == offlinedata->GetEventChannel(OfflineCurrentCount) && offlinedata->GetEventTraceLength(OfflineCurrentCount) > 0)//ch / trace length > 0
  // 	    {
  // 	      adjustdslider = true;
  // 	      break;
  // 	    }
  // 	}
  //   }
  // else
  //   {
  //     for(;;)
  // 	{
  // 	  tempN1++;
  // 	  if(tempN1 >= OfflineModuleEventsCount)
  // 	    {
  // 	      offlinedatastatus = true;
  // 	      break;
  // 	    }

  // 	  // OfflineCurrentCount++;
  // 	  if(OfflineCurrentCount == OfflineModuleEventsCount) OfflineCurrentCount = 0;

  // 	  if(offlinechnum->GetIntNumber() == offlinedata->GetEventChannel(OfflineCurrentCount)) break;//ch

  // 	}

  //   }
  
  // ===
  
  for(;;)
    {
      tempN1++;
      if(tempN1 >= OfflineModuleEventsCount)
  	{
  	  offlinedatastatus = true;
  	  break;
  	}

      // OfflineCurrentCount++;
      if(OfflineCurrentCount == OfflineModuleEventsCount) OfflineCurrentCount = 0;

      if(offlineonlywaveformevent->IsOn())
  	{
  	  if(offlinechnum->GetIntNumber() == offlinedata->GetEventChannel(OfflineCurrentCount) && offlinedata->GetEventTraceLength(OfflineCurrentCount) > 0)//ch / trace length > 0
  	    {
  	      adjustdslider = true;
  	      break;
  	    }
	  else
	    {
	      OfflineCurrentCount++;
	    }
  	}
      else
  	{
  	  if(offlinechnum->GetIntNumber() == offlinedata->GetEventChannel(OfflineCurrentCount)) break;//ch
	  OfflineCurrentCount++;
  	}
    }

  // cout<<"N: "<<OfflineCurrentCount<<endl;
	      
  if(offlinedatastatus)
    {
      adjustCanvas->cd();
      adjustCanvas->Clear();
      adjustCanvas->Modified();
      adjustCanvas->Update();
      gSystem->ProcessEvents();
      OfflineCurrentCountText->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
      if(offlineonlywaveformevent->IsOn())
	{
	  OfflineCurrentCountText->SetText(TString::Format("Ch%dNotWaveformData",(int)offlinechnum->GetIntNumber()).Data());
	}
      else
	{
	  OfflineCurrentCountText->SetText(TString::Format("Ch%dNotData",(int)offlinechnum->GetIntNumber()).Data());
	}
      OfflineReadFileButton->SetEnabled(1);
      // offlinecurrentcountentry->SetIntNumber(Long_t(OfflineCurrentCount)+1);
      return;
    }

  
  if(chooseslowfilterbaseline->GetSelected() == 1)//如果没有baseline数据，则采用计算基线的方法
    {
      int tempheaderlength = offlinedata->GetEventHeaderLength(OfflineCurrentCount);//header length
      if(!(headerrawenergysumsandbaseline->IsOn() && (tempheaderlength == 8 || tempheaderlength == 10 || tempheaderlength == 16 || tempheaderlength == 18)))
	{
	  chooseslowfilterbaseline->Select(0);
	}
    }
  
  OfflineCurrentCountText->SetTextColor(TColor::RGB2Pixel(COLOR_DODERBLUE_R,COLOR_DODERBLUE_G,COLOR_DODERBLUE_B), false);
  OfflineCurrentCountText->SetText(TString::Format("/ %d",OfflineModuleEventsCount).Data());
  offlinecurrentcountentry->SetIntNumber(Long_t(OfflineCurrentCount)+1);


  tracelength = offlinedata->GetEventTraceLength(OfflineCurrentCount);//trace length
  
  if(tracelength == 0)//没有波形事件
    {
      adjustCanvas->cd();
      adjustCanvas->Clear();
      TLatex l;
      l.SetTextSize(0.1);
      l.DrawLatex(0.1,0.5,TString::Format("Event: %d   e: %d   ts: %lld   %s",OfflineCurrentCount,offlinedata->GetEventEnergy(OfflineCurrentCount),(Long64_t(offlinedata->GetEventTime_High(OfflineCurrentCount)))*0x100000000+offlinedata->GetEventTime_Low(OfflineCurrentCount),offlinedata->GetEventCfdForcedTriggerBit(OfflineCurrentCount)?"cfd: invalid":"cfd: valid").Data());
      
      adjustCanvas->Modified();
      adjustCanvas->Update();
      OfflineReadFileButton->SetEnabled(1);
      gSystem->ProcessEvents();
      return;
    }


  RcdTrace = new unsigned short[tracelength];
  doublethresh = new double[tracelength];
  doublecfdthresh = new double[tracelength];
  doublesample = new double[tracelength];
  doublercdtrace = new double[tracelength];
  doublefastfilter = new double[tracelength];
  doublecfd = new double[tracelength];
  doublecfds = new double[tracelength];
  doubleslowfilter = new double[tracelength];

  int retval;
  retval = HongyiWuPixie16ComputeFastFiltersOffline(offlinefilename, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short) offlinechnum->GetIntNumber(), offlinedata->GetEventWaveLocation(OfflineCurrentCount),tracelength, RcdTrace, doublefastfilter, doublecfd, doublecfds);//trace length/trace location
  if(retval < 0) ErrorInfo("Offline.cc", "Panel1Draw()", "HongyiWuPixie16ComputeFastFiltersOffline", retval);

  if(detector->GetModuleADCMSPS(offlinemodnum->GetIntNumber()) == 500)
    {
      retval = HongyiWuPixie16ComputeCFDFiltersOffline(tracelength,(double)cfdfilter500Mparameter[0]->GetNumber(),(unsigned short)cfdfilter500Mparameter[1]->GetNumber(),(unsigned short)cfdfilter500Mparameter[2]->GetNumber(),(unsigned short)cfdfilter500Mparameter[3]->GetNumber(),RcdTrace,doublecfd);
      if(retval < 0) ErrorInfo("Offline.cc", "Panel1Draw()", "HongyiWuPixie16ComputeCFDFiltersOffline", retval);
    }
  
  if(chooseslowfilterbaseline->GetSelected() == 0)
    {
      retval = HongyiWuPixie16ComputeSlowFiltersOffline(offlinefilename, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short) offlinechnum->GetIntNumber(), offlinedata->GetEventWaveLocation(OfflineCurrentCount), tracelength, RcdTrace,doubleslowfilter );//trace length/trace location
      if(retval < 0) ErrorInfo("Offline.cc", "Panel1Draw()", "HongyiWuPixie16ComputeSlowFiltersOffline", retval);
    }
  else
    {
      retval = HongyiWuPixie16ComputeSlowFiltersOfflineExtendBaseline(offlinefilename, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short) offlinechnum->GetIntNumber(), offlinedata->GetEventWaveLocation(OfflineCurrentCount), tracelength, RcdTrace,doubleslowfilter ,offlinedata->GetEventSUMS4(OfflineCurrentCount,3),oldslowfilterparameter[0]->GetNumber(),oldslowfilterparameter[1]->GetNumber(),oldslowfilterparameter[2]->GetNumber(),oldofflinefilterrange->GetIntNumber(),200);//trace length/trace location
      if(retval < 0) ErrorInfo("Offline.cc", "Panel1Draw()", "HongyiWuPixie16ComputeSlowFiltersOfflineExtendBaseline", retval);
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
  cfdsdata = new TGraph(tracelength,doublesample,doublecfds);
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
  cfdsdata->SetLineColor(6);
  
  rawdata->SetMarkerStyle(7);
  threshdata->SetMarkerStyle(7);
  threshdata->SetMarkerColor(4);
  cfddata->SetMarkerStyle(7);
  cfddata->SetMarkerColor(2);
  cfdthreshdata->SetMarkerStyle(7);
  cfdthreshdata->SetMarkerColor(2);
  sfilterdata->SetMarkerStyle(7);
  sfilterdata->SetMarkerColor(3);
  ffilterdata->SetMarkerStyle(7);
  ffilterdata->SetMarkerColor(4);
  cfdsdata->SetMarkerStyle(7);
  cfdsdata->SetMarkerColor(6);
  if(choosedrawmarkerstyle->GetSelected() == 3 || choosedrawmarkerstyle->GetSelected() == 4)
    {
      rawdata->SetMarkerStyle(2);
      threshdata->SetMarkerStyle(2);
      cfddata->SetMarkerStyle(2);
      cfdsdata->SetMarkerStyle(2);
      cfdthreshdata->SetMarkerStyle(2);
      sfilterdata->SetMarkerStyle(2);
      ffilterdata->SetMarkerStyle(2);
    }
  if(choosedrawmarkerstyle->GetSelected() == 0 || choosedrawmarkerstyle->GetSelected() == 3)
    {
      rawdata->SetLineWidth(1);
      threshdata->SetLineWidth(1);
      cfddata->SetLineWidth(1);
      cfdsdata->SetLineWidth(1);
      cfdthreshdata->SetLineWidth(1);
      sfilterdata->SetLineWidth(1);
      ffilterdata->SetLineWidth(1);
    }
  if(choosedrawmarkerstyle->GetSelected() == 1 || choosedrawmarkerstyle->GetSelected() == 4)
    {
      rawdata->SetLineWidth(2);
      threshdata->SetLineWidth(2);
      cfddata->SetLineWidth(2);
      cfdsdata->SetLineWidth(2);
      cfdthreshdata->SetLineWidth(2);
      sfilterdata->SetLineWidth(2);
      ffilterdata->SetLineWidth(2);
    }
  
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
  if(offlinedrawoption1[6]->IsOn())
    offlinemultigraph->Add(cfdsdata);
  
  offlinemultigraph->SetTitle(TString::Format("Event: %d   e: %d   ts: %lld   %s",OfflineCurrentCount,offlinedata->GetEventEnergy(OfflineCurrentCount),(Long64_t(offlinedata->GetEventTime_High(OfflineCurrentCount)))*0x100000000+offlinedata->GetEventTime_Low(OfflineCurrentCount),offlinedata->GetEventCfdForcedTriggerBit(OfflineCurrentCount)?"cfd: invalid":"cfd: valid").Data());
  
  if(choosedrawmarkerstyle->GetSelected() == 0 || choosedrawmarkerstyle->GetSelected() == 1) offlinemultigraph->Draw("AL");
  if(choosedrawmarkerstyle->GetSelected() == 2) offlinemultigraph->Draw("AP");
  if(choosedrawmarkerstyle->GetSelected() == 3 || choosedrawmarkerstyle->GetSelected() == 4) offlinemultigraph->Draw("ALP");

  if(offlinedrawoption1[0]->IsOn() || offlinedrawoption1[1]->IsOn() || offlinedrawoption1[2]->IsOn() || offlinedrawoption1[3]->IsOn() || offlinedrawoption1[4]->IsOn() || offlinedrawoption1[5]->IsOn() || offlinedrawoption1[6]->IsOn())
    offlinemultigraph->GetXaxis()->SetRangeUser(double(dslider->GetMinPosition()),double(dslider->GetMaxPosition()));
  adjustCanvas->Modified();
  adjustCanvas->Update();
  OfflineReadFileButton->SetEnabled(1);
  // OfflineCurrentCount++;
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
      if(doublecfds2[i] != NULL)
	{
	  delete []doublecfds2[i];
	  doublecfds2[i] = NULL;
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
	  tempN12[i]++;
	  if(tempN12[i] >= OfflineModuleEventsCount-1)
	    {
	      offlinedatastatus2[i] = true;
	      break;
	    }

	  OfflineCurrentCount2[i]++;
	  if(OfflineCurrentCount2[i] >= OfflineModuleEventsCount) OfflineCurrentCount2[i] = 0;

	  if(i == offlinedata->GetEventChannel(OfflineCurrentCount2[i])) break;//ch
	}

      // cout<<"Ch: "<<i<<"  N: "<<OfflineCurrentCount2[i]<<endl;
      
      int retval;
      if(!offlinedatastatus2[i])
	{
	  tracelength2[i] = offlinedata->GetEventTraceLength(OfflineCurrentCount2[i]);//trace length

	  if(tracelength2[i] == 0)//not waveform event
	    {
	      offlinemultigraph2[i]->Clear();
	      canvas2->cd(1+i);
	      TLatex l;
	      l.SetTextSize(0.04);
	      l.DrawLatex(0.1,0.5,TString::Format("Event: %d   e: %d   ts: %lld   %s",OfflineCurrentCount2[i],offlinedata->GetEventEnergy(OfflineCurrentCount2[i]),(Long64_t(offlinedata->GetEventTime_High(OfflineCurrentCount2[i])))*0x100000000+offlinedata->GetEventTime_Low(OfflineCurrentCount2[i]),offlinedata->GetEventCfdForcedTriggerBit(OfflineCurrentCount2[i])?"cfd: invalid":"cfd: valid").Data());
	      continue;
	    }
	  
	  RcdTrace2[i] = new unsigned short[tracelength2[i]];
	  doublethresh2[i] = new double[tracelength2[i]];
	  doublecfdthresh2[i] = new double[tracelength2[i]];
	  doublesample2[i] = new double[tracelength2[i]];
	  doublercdtrace2[i] = new double[tracelength2[i]];
	  doublefastfilter2[i] = new double[tracelength2[i]];
	  doublecfd2[i] = new double[tracelength2[i]];
	  doublecfds2[i] = new double[tracelength2[i]];
	  doubleslowfilter2[i] = new double[tracelength2[i]];

	  retval = HongyiWuPixie16ComputeFastFiltersOffline(offlinefilename, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short)i, offlinedata->GetEventWaveLocation(OfflineCurrentCount2[i]), tracelength2[i], RcdTrace2[i], doublefastfilter2[i], doublecfd2[i], doublecfds2[i]);//trace location
	  if(retval < 0) ErrorInfo("Offline.cc", "Panel2Draw()", "HongyiWuPixie16ComputeFastFiltersOffline", retval);	  
	  if(detector->GetModuleADCMSPS(offlinemodnum->GetIntNumber()) == 500)
	    {
	      retval = HongyiWuPixie16ComputeCFDFiltersOffline(tracelength2[i],1,5,5,1,RcdTrace2[i],doublecfd2[i]);
	      if(retval < 0) ErrorInfo("Offline.cc", "Panel2Draw()", "HongyiWuPixie16ComputeCFDFiltersOffline", retval);
	    }	
	  retval = HongyiWuPixie16ComputeSlowFiltersOffline(offlinefilename, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short)i, offlinedata->GetEventWaveLocation(OfflineCurrentCount2[i]), tracelength2[i], RcdTrace2[i],doubleslowfilter2[i]);//trace localtion
	  if(retval < 0) ErrorInfo("Offline.cc", "Panel2Draw()", "HongyiWuPixie16ComputeSlowFiltersOffline", retval);
	  
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
	  offlinemultigraph2[i]->SetTitle(TString::Format("Event: %d   e: %d   ts: %lld   %s",OfflineCurrentCount2[i],offlinedata->GetEventEnergy(OfflineCurrentCount2[i]),(Long64_t(offlinedata->GetEventTime_High(OfflineCurrentCount2[i])))*0x100000000+offlinedata->GetEventTime_Low(OfflineCurrentCount2[i]),offlinedata->GetEventCfdForcedTriggerBit(OfflineCurrentCount2[i])?"cfd: invalid":"cfd: valid").Data());//energy timestamp
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
      channeleventcount3[offlinedata->GetEventChannel(i)]++;//ch
      if(offlinedata->GetEventTraceOutOfRangeFlag(i) == 1)//out of range
	{
	  traceoutofrangecount3[offlinedata->GetEventChannel(i)]++;//ch
	}
      else
	{
	  offlineth1d3[offlinedata->GetEventChannel(i)]->Fill(offlinedata->GetEventEnergy(i));//ch/event energy
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
  OfflineDrawButton4->SetEnabled(0);
  
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
      if(offlinechnum4->GetIntNumber() == offlinedata->GetEventChannel(i))//ch
	{
	  offlineth1d4->Fill(offlinedata->GetEventEnergy(i));//event energy
	}
    }
  
  offlineth1d4->Draw();
  offlineth1d4->SetTitle(TString::Format("Ch: %d",int(offlinechnum4->GetIntNumber())).Data());
  offlineth1d4->GetXaxis()->SetTitle("ch");

  canvas4->Modified();
  canvas4->Update();
  GausFitButton4->SetEnabled(1);
  OfflineDrawButton4->SetEnabled(1);
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
  if(doublecfds5 != NULL)
    {
      delete []doublecfds5;
      doublecfds5 = NULL;
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
      if(offlinechnum5->GetIntNumber() == offlinedata->GetEventChannel(i) && offlinedata->GetEventTraceLength(i) > 0)//ch / trcae length>0
	{
	  inttracelength = offlinedata->GetEventTraceLength(i);//trace length
	  break;
	}
    }

  if(inttracelength > -1)
    {
      RcdTrace5 = new unsigned short[inttracelength];
      doublefastfilter5 = new double[inttracelength];
      doublecfd5 = new double[inttracelength];
      doublecfds5 = new double[inttracelength];
      
      offlineth2d5_0 = new TH2D("offlineth2d5_0","",inttracelength,0,inttracelength,1000,-100,900);
      offlineth2d5_1 = new TH2D("offlineth2d5_1","",inttracelength,0,inttracelength,1000,-1000,9000);

      switch(detector->GetModuleADCMSPS(offlinemodnum->GetIntNumber()))
	{
	case 100 ://100
	  originalcfdth1d5 = new TH1D("originalcfdth1d5","",512,0,32768);//32768
	  calculatecfdth1d5 = new TH1D("calculatecfdth1d5","",512,0,32768);//32768
	  break;
	case 250 ://250
	  originalcfdth1d5 = new TH1D("originalcfdth1d5","",512,0,16384);//16384
	  calculatecfdth1d5 = new TH1D("calculatecfdth1d5","",512,0,16384);//16384
	  break;	  
	case 500 ://500
	  originalcfdth1d5 = new TH1D("originalcfdth1d5","",512,0,8192);//8192
	  calculatecfdth1d5 = new TH1D("calculatecfdth1d5","",512,0,8192);//8192
	  break;
	default:
	  std::cout<<"ERROR: Please call Hongyi Wu(wuhongyi@qq.com)"<<std::endl;
	  break;
	}


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
	  if(offlinechnum5->GetIntNumber() == offlinedata->GetEventChannel(i) && offlinedata->GetEventTraceLength(i) > 0)//ch / trace length>0
	    {
	      cfdevenycount5++;
	      if(offlinedata->GetEventCfdForcedTriggerBit(i) == 0) originalcfdvalidcount5++;////cfd forced trigger bit
	      
	      retval = HongyiWuPixie16ComputeFastFiltersOffline(offlinefilename, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short) offlinechnum5->GetIntNumber(), offlinedata->GetEventWaveLocation(i), offlinedata->GetEventTraceLength(i), RcdTrace5, doublefastfilter5, doublecfd5, doublecfds5);//trace location/trace length
	      if(retval < 0) ErrorInfo("Offline.cc", "Panel5Draw()", "HongyiWuPixie16ComputeFastFiltersOffline", retval);
	      if(detector->GetModuleADCMSPS(offlinemodnum->GetIntNumber()) == 500)
		{
		  retval = HongyiWuPixie16ComputeCFDFiltersOffline(offlinedata->GetEventTraceLength(i),1,5,5,1,RcdTrace5,doublecfd5);
		  if(retval < 0) ErrorInfo("Offline.cc", "Panel5Draw()", "HongyiWuPixie16ComputeCFDFiltersOffline", retval);
		}

	      
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
			  switch(detector->GetModuleADCMSPS(offlinemodnum->GetIntNumber()))
			    {
			    case 100 ://100
			      calculatecfdth1d5->Fill(doublecfd5[j]/(doublecfd5[j]-doublecfd5[j+1])*32768);//
			      break;
			    case 250 ://250
			      calculatecfdth1d5->Fill(doublecfd5[j]/(doublecfd5[j]-doublecfd5[j+1])*16384);//
			      break;	  
			    case 500 ://500
			      calculatecfdth1d5->Fill(doublecfd5[j]/(doublecfd5[j]-doublecfd5[j+1])*8192);//
			      break;
			    default:
			      std::cout<<"ERROR: Please call Hongyi Wu(wuhongyi@qq.com)"<<std::endl;
			      break;
			    }

			  break;
			}
		    }
		}

	      if(offlinedata->GetEventCfdForcedTriggerBit(i) == 0)
		originalcfdth1d5->Fill(offlinedata->GetEventCfd(i));//cfd
	    }

	  if(i%500 == 0)
	    {
	      if(flagdrawstop5) break;
	      printtextinfor5->SetText(TString::Format("Drawing... ==> %d/%d",i,OfflineModuleEventsCount).Data());
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
  if(doublecfds6 != NULL)
    {
      delete []doublecfds6;
      doublecfds6 = NULL;
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
      if(offlinechnum6->GetIntNumber() == offlinedata->GetEventChannel(i) && offlinedata->GetEventTraceLength(i) > 0)//ch
	{
	  inttracelength = offlinedata->GetEventTraceLength(i);//trace length
	  break;
	}
    }

  if(inttracelength > -1)
    {
      offlineth1d6 = new TH1D("offlineth1d6","",intth1dbin6,0,65536);
      RcdTrace6 = new unsigned short[inttracelength];
      doublefastfilter6 = new double[inttracelength];
      doublecfd6 = new double[inttracelength];
      doublecfds6 = new double[inttracelength];
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
      switch(detector->GetModuleADCMSPS(offlinemodnum->GetIntNumber()))
	{
	case 100 ://100
	  SlowLen = ROUND(ChanParData*100/std::pow(2.0,(double)SlowFilterRange))*std::pow(2.0,(double)SlowFilterRange);
	  break;
	case 250 ://250
	  SlowLen = ROUND(ChanParData*125/std::pow(2.0,(double)SlowFilterRange))*std::pow(2.0,(double)SlowFilterRange) *2;
	  break;	  
	case 500 ://500
	  SlowLen = ROUND(ChanParData*100/std::pow(2.0,(double)SlowFilterRange))*std::pow(2.0,(double)SlowFilterRange) *5;
	  break;
	default:
	  std::cout<<"ERROR: Please call Hongyi Wu(wuhongyi@qq.com)"<<std::endl;
	  break;
	}
      
      retval = Pixie16ReadSglChanPar((char*)"ENERGY_FLATTOP", &ChanParData, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short)offlinechnum6->GetIntNumber());
      if(retval < 0) ErrorInfo("Offline.cc", "Panel6Draw()", "Pixie16ReadSglChanPar/ENERGY_FLATTOP", retval);
      switch(detector->GetModuleADCMSPS(offlinemodnum->GetIntNumber()))
	{
	case 100 ://100
	  SlowGap = ROUND(ChanParData*100/std::pow(2.0,(double)SlowFilterRange))*std::pow(2.0,(double)SlowFilterRange);
	  break;
	case 250 ://250
	  SlowGap = ROUND(ChanParData*125/std::pow(2.0,(double)SlowFilterRange))*std::pow(2.0,(double)SlowFilterRange) *2;
	  break;	  
	case 500 ://500
	  SlowGap = ROUND(ChanParData*100/std::pow(2.0,(double)SlowFilterRange))*std::pow(2.0,(double)SlowFilterRange) *5;
	  break;
	default:
	  std::cout<<"ERROR: Please call Hongyi Wu(wuhongyi@qq.com)"<<std::endl;
	  break;
	}

      int intflagenergy;
      intflagenergy = SlowLen+SlowGap-1;
      // switch(SlowFilterRange)
      // 	{
      // 	case 1:
      // 	  intflagenergy = SlowLen+SlowGap-3;
      // 	  break;
      // 	case 2:
      // 	  intflagenergy = SlowLen+SlowGap-2;
      // 	  break;
      // 	case 3:
      // 	  intflagenergy = SlowLen+SlowGap-2;
      // 	  break;
      // 	case 4:
      // 	  intflagenergy = SlowLen+SlowGap-1;
      // 	  break;
      // 	case 5:
      // 	  intflagenergy = SlowLen+SlowGap;
      // 	  break;
      // 	case 6:
      // 	  intflagenergy = SlowLen+SlowGap+1;
      // 	  break;
      // 	default:
      // 	  intflagenergy = SlowLen+SlowGap-2;
      // 	  break;
      // 	}

      
      for (unsigned int i = 0; i < OfflineModuleEventsCount; ++i)
	{
	  if(offlinechnum6->GetIntNumber() == offlinedata->GetEventChannel(i) && offlinedata->GetEventTraceLength(i) > 0)//ch / trace length>0
	    {
	      if(chooseslowfilterbaselinep6->GetSelected() == 1)//如果没有baseline数据，则采用计算基线的方法
		{
		  int tempheaderlength = offlinedata->GetEventHeaderLength(i);// Header length
		  if(!(headerrawenergysumsandbaseline->IsOn() && (tempheaderlength == 8 || tempheaderlength == 10 || tempheaderlength == 16 || tempheaderlength == 18)))
		    {
		      chooseslowfilterbaselinep6->Select(0);
		    }
		}

	      
	      retval = HongyiWuPixie16ComputeFastFiltersOffline(offlinefilename, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short) offlinechnum6->GetIntNumber(), offlinedata->GetEventWaveLocation(i), offlinedata->GetEventTraceLength(i), RcdTrace6, doublefastfilter6, doublecfd6, doublecfds6);//trace location/trace length
	      if(retval < 0) ErrorInfo("Offline.cc", "Panel6Draw()", "HongyiWuPixie16ComputeFastFiltersOffline", retval);

	      if(chooseslowfilterbaselinep6->GetSelected() == 0)
		{
		  retval = HongyiWuPixie16ComputeSlowFiltersOffline(offlinefilename, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short) offlinechnum6->GetIntNumber(), offlinedata->GetEventWaveLocation(i), offlinedata->GetEventTraceLength(i), RcdTrace6,doubleslowfilter6);//trace location/trace length
		  if(retval < 0) ErrorInfo("Offline.cc", "Panel6Draw()", "HongyiWuPixie16ComputeSlowFiltersOffline", retval);
		}
	      else
		{
		  retval = HongyiWuPixie16ComputeSlowFiltersOfflineExtendBaseline(offlinefilename, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short) offlinechnum6->GetIntNumber(), offlinedata->GetEventWaveLocation(i), offlinedata->GetEventTraceLength(i), RcdTrace6,doubleslowfilter6, offlinedata->GetEventSUMS4(i,3),oldslowfilterparameterp6[0]->GetNumber(),oldslowfilterparameterp6[1]->GetNumber(),oldslowfilterparameterp6[2]->GetNumber(),oldofflinefilterrangep6->GetIntNumber(),200);//trace location/trace length
		  if(retval < 0) ErrorInfo("Offline.cc", "Panel6Draw()", "HongyiWuPixie16ComputeSlowFiltersOfflineExtendBaseline", retval);
		}
	      
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
	      printtextinfor6->SetText(TString::Format("Drawing... ==> %d/%d",i,OfflineModuleEventsCount).Data());
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
      if(offlinechnum8->GetIntNumber() == offlinedata->GetEventChannel(i) && offlinedata->GetEventTraceLength(i) > 0)//ch / trace length>0
	{
	  inttracelength = offlinedata->GetEventTraceLength(i);//trace length
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
      switch(detector->GetModuleADCMSPS(offlinemodnum->GetIntNumber()))
	{
	case 100 ://100
	  FastLen = ROUND(ChanParData*100/std::pow(2.0,(double)FastFilterRange))*std::pow(2.0,(double)FastFilterRange);
	  break;
	case 250 ://250
	  FastLen = ROUND(ChanParData*125/std::pow(2.0,(double)FastFilterRange))*std::pow(2.0,(double)FastFilterRange) *2;
	  break;	  
	case 500 ://500
	  FastLen = ROUND(ChanParData*100/std::pow(2.0,(double)FastFilterRange))*std::pow(2.0,(double)FastFilterRange) *5;
	  break;
	default:
	  std::cout<<"ERROR: Please call Hongyi Wu(wuhongyi@qq.com)"<<std::endl;
	  break;
	}

      retval = Pixie16ReadSglChanPar((char*)"TRIGGER_FLATTOP", &ChanParData, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short)offlinechnum8->GetIntNumber());
      if(retval < 0) ErrorInfo("Offline.cc", "Panel8Draw()", "Pixie16ReadSglChanPar/TRIGGER_FLATTOP", retval);
      switch(detector->GetModuleADCMSPS(offlinemodnum->GetIntNumber()))
	{
	case 100 ://100
	  FastGap = ROUND(ChanParData*100/std::pow(2.0,(double)FastFilterRange))*std::pow(2.0,(double)FastFilterRange);
	  break;
	case 250 ://250
	  FastGap = ROUND(ChanParData*125/std::pow(2.0,(double)FastFilterRange))*std::pow(2.0,(double)FastFilterRange) *2;
	  break;	  
	case 500 ://500
	  FastGap = ROUND(ChanParData*100/std::pow(2.0,(double)FastFilterRange))*std::pow(2.0,(double)FastFilterRange) *5;
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
	  if(offlinechnum8->GetIntNumber() == offlinedata->GetEventChannel(i) && offlinedata->GetEventTraceLength(i) > 0)//ch / trace length>0
	    {	      
	      ListModeFile = fopen(offlinefilename, "rb");
	      if(ListModeFile != NULL)
		{
		  fseek(ListModeFile, offlinedata->GetEventWaveLocation(i)*4, SEEK_SET);// Position ListModeFile to the requested trace location
		  fread(RcdTrace8, 2, offlinedata->GetEventTraceLength(i), ListModeFile);// Read trace
		  fclose(ListModeFile);// Close file
		  ListModeFile = NULL;
		
		  // Compute fast filter response
		  offset = 2*FastLen + FastGap - 1;
		  for(x = offset; x < offlinedata->GetEventTraceLength(i); x++)
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
		  for (int z = 0; z < (int)offlinedata->GetEventTraceLength(i); ++z)//trace length
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
		  if(flagfffirst+(FastLen+FastGap)*2 < offlinedata->GetEventTraceLength(i))//trace length
		    {
		      for (int z = flagfffirst+(int(FastLen+FastGap))*2; z < (int)offlinedata->GetEventTraceLength(i); ++z)//trace length
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
			  energyfffirst8->SetPoint(countenergyff8[0],fffirst,double(offlinedata->GetEventEnergy(i)));//energy
			  countenergyff8[0]++;
			}
		      if(ffsecond > 0)
			{
			  energyffsecond8->SetPoint(countenergyff8[1],ffsecond,double(offlinedata->GetEventEnergy(i)));//energy
			  countenergyff8[1]++;
			}
		    }
		  else
		    {
		      if(fffirst > 0)
			{
			  histenergyfffirst8->Fill(fffirst,double(offlinedata->GetEventEnergy(i)));//energy
			}
		      if(ffsecond > 0)
			{
			  histenergyffsecond8->Fill(ffsecond,double(offlinedata->GetEventEnergy(i)));//energy
			}
		    }
		}//fast filter

	      
	    }//ch

	  if(i%500 == 0)
	    {
	      if(flagdrawstop8) break;
	      printtextinfor8->SetText(TString::Format("Drawing... ==> %d/%d",i,OfflineModuleEventsCount).Data());
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
  OfflineDrawButton9->SetEnabled(0);

  if(fft1d9 != NULL)
    {
      delete fft1d9;
      fft1d9 = NULL;
    }

  if(in9 != NULL)
    {
      Free_fftw_complex(in9);
      in9 = NULL;
    }
  
  if(graphfft9 != NULL)
    {
      delete graphfft9;
      graphfft9 = NULL;
    }
  
  if(RcdTrace9 != NULL)
    {
      delete []RcdTrace9;
      RcdTrace9 = NULL;
    }

  if(BufferFFT9 != NULL)
    {
      delete []BufferFFT9;
      BufferFFT9 = NULL;
    }

  bool offlinedatastatus;
  unsigned int tempN1;
  tempN1 = -1;
  offlinedatastatus = false;  
  for(;;)
    {
      tempN1++;
      if(tempN1 >= OfflineModuleEventsCount)
	{
	  offlinedatastatus = true;
	  break;
	}
      
      OfflineCurrentCount9++;
      if(OfflineCurrentCount9 >= OfflineModuleEventsCount) OfflineCurrentCount9 = 0;

      if(offlinechnum9->GetIntNumber() == offlinedata->GetEventChannel(OfflineCurrentCount9) && offlinedata->GetEventTraceLength(OfflineCurrentCount9) > 0) break;//ch / trace length > 0
    }

  if(offlinedatastatus)
    {
      canvas9->cd();
      canvas9->Clear();
      canvas9->Modified();
      canvas9->Update();
      OfflineCurrentCountText9->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
      OfflineCurrentCountText9->SetText(TString::Format("Ch%dNotWaveformData",(int)offlinechnum9->GetIntNumber()).Data());
      OfflineReadFileButton->SetEnabled(1);
      OfflineDrawButton9->SetEnabled(1);
      gSystem->ProcessEvents();
      return;
    }

  OfflineCurrentCountText9->SetTextColor(TColor::RGB2Pixel(COLOR_DODERBLUE_R,COLOR_DODERBLUE_G,COLOR_DODERBLUE_B), false);
  OfflineCurrentCountText9->SetText(TString::Format("Event: %d / %d",OfflineCurrentCount9,OfflineModuleEventsCount).Data());
  
  tracelength9 = offlinedata->GetEventTraceLength(OfflineCurrentCount9);//trace length

  RcdTrace9 = new unsigned short[tracelength9];
  BufferFFT9 = new double[2*tracelength9];
  in9 = Malloc_fftw_complex(tracelength9);
  graphfft9 = new TGraph;
  fft1d9 = new fftw1d(tracelength9,-1);
  
  FILE *ListModeFile = NULL;
  ListModeFile = fopen(offlinefilename, "rb");
  if(ListModeFile != NULL)
    {
      fseek(ListModeFile, offlinedata->GetEventWaveLocation(OfflineCurrentCount9)*4, SEEK_SET);// Position ListModeFile to the requested trace location
      fread(RcdTrace9, 2, offlinedata->GetEventTraceLength(OfflineCurrentCount9), ListModeFile);// Read trace
      fclose(ListModeFile);// Close file
      ListModeFile = NULL;
    }

  int power2;
  unsigned int sizecomplexfft;
  int SizeFFT;
  switch(choosedrawstyle9->GetSelected())
    {
    case 0://XIA
      for (int point = 0; point < tracelength9; ++point)
	{
	  BufferFFT9[2*point+1] = 0;
	  BufferFFT9[2*point] = double(RcdTrace9[point]);
	}

      power2 = 1;
      while((1<<power2) < tracelength9) power2++;
      if((1<<power2) == tracelength9) sizecomplexfft = (unsigned int)(1<<power2);
      else sizecomplexfft = (unsigned int)(1<<(power2-1));

      Pixie16complexFFT(BufferFFT9, sizecomplexfft);
      
      for (int point = 0; point < (int)(sizecomplexfft)/2; ++point)
	{
	  graphfft9->SetPoint(point,double(point),std::sqrt(BufferFFT9[2*point]*BufferFFT9[2*point]+BufferFFT9[2*point+1]*BufferFFT9[2*point+1]));
	}
      break;

    case 1://FFTW3
      for (int point = 0; point < tracelength9; ++point)
	{
	  in9[point][0] = double(RcdTrace9[point]);
	  in9[point][1] = 0;
	}
      
      fft1d9->ForwardGetAmplitude(in9,BufferFFT9);
      
      for (int point = 0; point < tracelength9/2; ++point)
	{
	  graphfft9->SetPoint(point,double(point),BufferFFT9[point]);
	}
      break;


    default://CAEN
      SizeFFT = CAEN_FFT(RcdTrace9,BufferFFT9,tracelength9,choosedrawstyle9->GetSelected()%10);
      for (int point = 0; point < SizeFFT; ++point)
	{
	  graphfft9->SetPoint(point,point,BufferFFT9[point]);
	}
      break;
    }

  canvas9->cd();
  canvas9->Clear();
  canvas9->SetLogy(0);

  graphfft9->SetLineColor(kRed);
  graphfft9->Draw("AL");

  if(choosedrawstyle9->GetSelected() >= 10)
    {
      graphfft9->GetYaxis()->SetTitle("dB");
    }
  else
    {
      canvas9->SetLogy(1);
    }
  

  canvas9->Modified();
  canvas9->Update();
  OfflineDrawButton9->SetEnabled(1);
  OfflineReadFileButton->SetEnabled(1);
  gSystem->ProcessEvents();
}


void Offline::Panel10Draw()
{
  OfflineReadFileButton->SetEnabled(0);
  OfflineDrawButton10->SetEnabled(0);

  GausFitButton10->SetEnabled(0);
  GausFitButton10->SetText("Open  Fit");
  canvas10->DeleteExec("dynamicPanel10GausFit");
  falggausfit10 = false;
  
  printtextinfor10->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
  printtextinfor10->SetText("Waitting ...");
  gSystem->ProcessEvents();
  
  if(offlineth1i10 != NULL)
    {
      delete offlineth1i10;
      offlineth1i10 = NULL;
    }

  if(offlinechnumA10->GetIntNumber() == offlinechnumB10->GetIntNumber())
    {
      printtextinfor10->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
      printtextinfor10->SetText("Ch A == Ch B. Pelse re-select it.");
      
      canvas10->cd();
      canvas10->Clear();
      canvas10->Modified();
      canvas10->Update();
      OfflineDrawButton10->SetEnabled(1);
      OfflineReadFileButton->SetEnabled(1);
      gSystem->ProcessEvents();
      return;
    }
  
  if(histxminmax10[1]->GetNumber() >= histxminmax10[2]->GetNumber())
    {
      std::cout<<"The range of the histogram is not suitable (xmin >= xmax). The recommended value will be used."<<std::endl;
      histxminmax10[1]->SetNumber(-TIMEDIFFGATE);
      histxminmax10[2]->SetNumber(TIMEDIFFGATE);
    }

  if(offlineenergylimit10->IsOn())
    {
      
      if(energylimitsab10[0]->GetNumber() >= energylimitsab10[1]->GetNumber())
	{
	  std::cout<<"The range of limits of energy is not suitable (A Left >= A Right). The recommended value will be used."<<std::endl;
	  energylimitsab10[0]->SetNumber(0);
	  energylimitsab10[1]->SetNumber(65536);
	}

      if(energylimitsab10[2]->GetNumber() >= energylimitsab10[3]->GetNumber())
	{
	  std::cout<<"The range of limits of energy is not suitable (B Left >= B Right). The recommended value will be used."<<std::endl;
	  energylimitsab10[2]->SetNumber(0);
	  energylimitsab10[3]->SetNumber(65536);
	}
    }

  
  offlineth1i10 = new TH1I("offlineth1i10","",histxminmax10[0]->GetNumber(),histxminmax10[1]->GetNumber(),histxminmax10[2]->GetNumber());
  offlineth1i10->GetXaxis()->SetTitle("#DeltaT / ns");
  offlineth1i10->SetTitle("T_{A}-T_{B}");


  double deltat;
  Long64_t deltaft;
  bool flagenergylimits;
  for (unsigned int i = 0; i < OfflineModuleEventsCount; ++i)
    {
      if(offlinechnumA10->GetIntNumber() == offlinedata->GetEventChannel(i))//ch
	{

	  for (unsigned int j = i; j < OfflineModuleEventsCount; ++j)
	    {
	      deltaft = ((Long64_t(offlinedata->GetEventTime_High(i)))*0x100000000+offlinedata->GetEventTime_Low(i))-((Long64_t(offlinedata->GetEventTime_High(j)))*0x100000000+offlinedata->GetEventTime_Low(j));
	      // std::cout<<i<<"  "<<j<<"  Post:"<<deltaft<<std::endl;
	      if(TMath::Abs(deltaft) < 100000)
	  	{
	  	  if(offlinechnumB10->GetIntNumber() == offlinedata->GetEventChannel(j))//ch
	  	    {
	  	      switch(detector->GetModuleADCMSPS(offlinemodnum->GetIntNumber()))
	  		{
	  		case 100 ://100
	  		  if(choosedrawstyle10->GetSelected() == 0)
	  		    deltat = deltaft*10.0 + (int(offlinedata->GetEventCfd(i))-int(offlinedata->GetEventCfd(j)))*10.0/32768;
	  		  else
	  		    deltat = deltaft*10.0;
	  		  break;
	  		case 250 ://250
	  		  if(choosedrawstyle10->GetSelected() == 0)
	  		    deltat = deltaft*8.0 - (int(offlinedata->GetEventCfdTriggerSourceBits(i))-int(offlinedata->GetEventCfdTriggerSourceBits(j)))*4.0 + (int(offlinedata->GetEventCfd(i))-int(offlinedata->GetEventCfd(j)))*4.0/16384;
	  		  else
	  		    deltat = deltaft*8.0;
	  		  break;	  
	  		case 500 ://500
	  		  if(choosedrawstyle10->GetSelected() == 0)
	  		    deltat = deltaft*10.0 + (int(offlinedata->GetEventCfdTriggerSourceBits(i))-int(offlinedata->GetEventCfdTriggerSourceBits(j)))*2.0 + (int(offlinedata->GetEventCfd(i))-int(offlinedata->GetEventCfd(j)))*2.0/8192;
	  		  else
	  		    deltat = deltaft*10.0;
	  		  break;
	  		default:
	  		  std::cout<<"ERROR: Please call Hongyi Wu(wuhongyi@qq.com)"<<std::endl;
	  		  break;
	  		}
		      
		      if(deltat >= histxminmax10[1]->GetNumber() && deltat <= histxminmax10[2]->GetNumber())
		      	{

			  flagenergylimits = true;
			  if(offlineenergylimit10->IsOn())
			    {
			      if((int(offlinedata->GetEventEnergy(i)) > energylimitsab10[0]->GetNumber()) && (int(offlinedata->GetEventEnergy(i)) < energylimitsab10[1]->GetNumber()) && (int(offlinedata->GetEventEnergy(j)) > energylimitsab10[2]->GetNumber()) && (int(offlinedata->GetEventEnergy(j)) < energylimitsab10[3]->GetNumber()))
				flagenergylimits = true;
			      else
				flagenergylimits = false;
			    }
			  
			  if(flagenergylimits)
			    {
			      if(choosedrawstyle10->GetSelected() == 0)
				{
				  if(offlinedata->GetEventCfdForcedTriggerBit(i) == 0 && offlinedata->GetEventCfdForcedTriggerBit(j) == 0) offlineth1i10->Fill(deltat);
				}
			      else
				offlineth1i10->Fill(deltat);
			    }
		      	}
	  	    }

	  	}
	      else
	  	{
	  	  break;
	  	}
	    }

	  
	  // =====
	  
	  for (unsigned int j = i; j > 0; --j)
	    {
	      deltaft = ((Long64_t(offlinedata->GetEventTime_High(i)))*0x100000000+offlinedata->GetEventTime_Low(i))-((Long64_t(offlinedata->GetEventTime_High(j)))*0x100000000+offlinedata->GetEventTime_Low(j));
	      // std::cout<<i<<"  "<<j<<"  Pre:"<<deltaft<<std::endl;
	      if(TMath::Abs(deltaft) < 100000)
		{
		  if(offlinechnumB10->GetIntNumber() == offlinedata->GetEventChannel(j))//ch
		    {
		      switch(detector->GetModuleADCMSPS(offlinemodnum->GetIntNumber()))
			{
			case 100 ://100
			  if(choosedrawstyle10->GetSelected() == 0)
			    deltat = deltaft*10.0 + (int(offlinedata->GetEventCfd(i))-int(offlinedata->GetEventCfd(j)))*10.0/32768;
			  else
			    deltat = deltaft*10.0;
			  break;
			case 250 ://250
			  if(choosedrawstyle10->GetSelected() == 0)
			    deltat = deltaft*8.0 - (int(offlinedata->GetEventCfdTriggerSourceBits(i))-int(offlinedata->GetEventCfdTriggerSourceBits(j)))*4.0 + (int(offlinedata->GetEventCfd(i))-int(offlinedata->GetEventCfd(j)))*4.0/16384;
			  else
			    deltat = deltaft*8.0;
			  break;	  
			case 500 ://500
			  if(choosedrawstyle10->GetSelected() == 0)
			    deltat = deltaft*10.0 + (int(offlinedata->GetEventCfdTriggerSourceBits(i))-int(offlinedata->GetEventCfdTriggerSourceBits(j)))*2.0 + (int(offlinedata->GetEventCfd(i))-int(offlinedata->GetEventCfd(j)))*2.0/8192;
			  else
			    deltat = deltaft*10.0;
			  break;
			default:
			  std::cout<<"ERROR: Please call Hongyi Wu(wuhongyi@qq.com)"<<std::endl;
			  break;
			}
		      
		      if(deltat >= histxminmax10[1]->GetNumber() && deltat <= histxminmax10[2]->GetNumber())
		      	{
			  flagenergylimits = true;
			  if(offlineenergylimit10->IsOn())
			    {
			      if((int(offlinedata->GetEventEnergy(i)) > energylimitsab10[0]->GetNumber()) && (int(offlinedata->GetEventEnergy(i)) < energylimitsab10[1]->GetNumber()) && (int(offlinedata->GetEventEnergy(j)) > energylimitsab10[2]->GetNumber()) && (int(offlinedata->GetEventEnergy(j)) < energylimitsab10[3]->GetNumber()))
				flagenergylimits = true;
			      else
				flagenergylimits = false;
			    }
			  
			  if(flagenergylimits)
			    {
			      if(choosedrawstyle10->GetSelected() == 0)
				{
				  if(offlinedata->GetEventCfdForcedTriggerBit(i) == 0 && offlinedata->GetEventCfdForcedTriggerBit(j) == 0) offlineth1i10->Fill(deltat);
				}
			      else
				offlineth1i10->Fill(deltat);
			    }
		      	}
		    }

		}
	      else
		{
		  break;
		}
	    }	 

	}// ch
    }// for OfflineModuleEventsCount


  printtextinfor10->SetTextColor(TColor::RGB2Pixel(COLOR_DODERBLUE_R,COLOR_DODERBLUE_G,COLOR_DODERBLUE_B), false);
  printtextinfor10->SetText("Done!");

  canvas10->cd();
  canvas10->Clear();

  offlineth1i10->Draw();
  
  canvas10->Modified();
  canvas10->Update();
  GausFitButton10->SetEnabled(1);
  OfflineDrawButton10->SetEnabled(1);
  OfflineReadFileButton->SetEnabled(1);
  gSystem->ProcessEvents();
}


void Offline::Panel11Draw()
{
  OfflineReadFileButton->SetEnabled(0);
  OfflineDrawButton11->SetEnabled(0);

  printtextinfor11->SetText("Drawing...");
  canvas11->cd();
  canvas11->Clear();
  canvas11->Divide(2,2);
  canvas11->Modified();
  canvas11->Update();
  gSystem->ProcessEvents();
  
  if(offlineth1icfdinvalid11 != NULL)
    {
      delete offlineth1icfdinvalid11;
      offlineth1icfdinvalid11 = NULL;
    }
  if(offlineth1icfdvalid11 != NULL)
    {
      delete offlineth1icfdvalid11;
      offlineth1icfdvalid11 = NULL;
    }
  if(offlineth1icfd11 != NULL)
    {
      delete offlineth1icfd11;
      offlineth1icfd11 = NULL;
    }
  if(offlineth2i11 != NULL)
    {
      delete offlineth2i11;
      offlineth2i11 = NULL;
    }

  int cfdmax,energymax,energybin,cfdbin;
  switch(detector->GetModuleADCMSPS(offlinemodnum->GetIntNumber()))
    {
    case 100://100
      cfdmax = 32768;
      break;
    case 250://250
      cfdmax = 16384;
      break;
    case 500://500
      cfdmax = 8192;
      break;
    default:
      std::cout<<"ERROR: Please call Hongyi Wu(wuhongyi@qq.com)"<<std::endl;
      break;
    }

  switch(choosedenergybin11->GetSelected())
    {
    case 0:
      energymax = 65536;
      energybin = 4096;
      break;
    case 1:
      energymax = 65536;
      energybin = 2048;
      break;
    case 2:
      energymax = 65536;
      energybin = 1024;
      break;
    case 3:
      energymax = 32768;
      energybin = 4096;
      break;
    case 4:
      energymax = 32768;
      energybin = 2048;
      break;
    case 5:
      energymax = 32768;
      energybin = 1024;
      break;
    case 6:
      energymax = 16384;
      energybin = 4096;
      break;
    case 7:
      energymax = 16384;
      energybin = 2048;
      break;
    case 8:
      energymax = 16384;
      energybin = 1024;
      break;
    case 9:
      energymax = 10000;
      energybin = 5000;
      break;
    case 10:
      energymax = 10000;
      energybin = 2000;
      break;
    case 11:
      energymax = 5000;
      energybin = 5000;
      break;
    case 12:
      energymax = 5000;
      energybin = 2500;
      break;
    case 13:
      energymax = 2000;
      energybin = 2000;
      break;
    case 14:
      energymax = 1000;
      energybin = 1000;
      break;
    default:
      std::cout<<"ERROR: Please check TGComboBox number."<<std::endl;
      break;     
    }
  
  if(choosedcfdbin11->GetSelected() == 0)
    cfdbin = 4096;
  else if(choosedcfdbin11->GetSelected() == 1)
    cfdbin = 2048;
  else if(choosedcfdbin11->GetSelected() == 2)
    cfdbin = 1024;
  else
    std::cout<<"ERROR: Please check TGComboBox number."<<std::endl;
  
  offlineth1icfdinvalid11 = new TH1I("energy_cfdinvalid11","energy(cfd invalid)",65536,0,65536);
  offlineth1icfdinvalid11->GetXaxis()->SetTitle("ch");
  offlineth1icfdvalid11 = new TH1I("energy_cfdvalid11","energy(cfd valid)",65536,0,65536);
  offlineth1icfdvalid11->GetXaxis()->SetTitle("ch");
  offlineth1icfd11 = new TH1I("cfd_dis11","cfd",cfdmax,0,cfdmax);
  offlineth1icfd11->GetXaxis()->SetTitle("cfd vaule");
  offlineth2i11 = new TH2I("energycfd11","energy-cfd",cfdbin,0,cfdmax,energybin,0,energymax);
  offlineth2i11->GetXaxis()->SetTitle("cfd vaule");
  offlineth2i11->GetYaxis()->SetTitle("ch");
  for (unsigned int i = 0; i < OfflineModuleEventsCount; ++i)
    {
      if(offlinechnum11->GetIntNumber() == offlinedata->GetEventChannel(i))//ch
	{
	  if(offlinedata->GetEventCfdForcedTriggerBit(i))
	    {// forced cfd trigger
	      offlineth1icfdinvalid11->Fill(offlinedata->GetEventEnergy(i));
	    }
	  else
	    {
	      offlineth1icfdvalid11->Fill(offlinedata->GetEventEnergy(i));
	      offlineth1icfd11->Fill(offlinedata->GetEventCfd(i));
	      offlineth2i11->Fill(offlinedata->GetEventCfd(i),offlinedata->GetEventEnergy(i));
	    }
	}
    }

  


  canvas11->cd(1);
  offlineth1icfdinvalid11->Draw();
  canvas11->cd(2);
  offlineth1icfd11->Draw();
  canvas11->cd(3);
  offlineth1icfdvalid11->Draw();
  canvas11->cd(4);
  offlineth2i11->Draw("colz");
  canvas11->Modified();
  canvas11->Update();

  printtextinfor11->SetText("Draw Done!");
  OfflineDrawButton11->SetEnabled(1);
  OfflineReadFileButton->SetEnabled(1);
  gSystem->ProcessEvents();  
}

void Offline::Panel12Draw()
{
  OfflineReadFileButton->SetEnabled(0);
  OfflineDrawButton12->SetEnabled(0);

  printtextinfor12->SetText("Drawing...");
  canvas12->cd();
  canvas12->Clear();
  canvas12->Divide(2,2);
  canvas12->Modified();
  canvas12->Update();
  gSystem->ProcessEvents();

  if(offlineth1ipileupevent12 != NULL)
    {
      delete offlineth1ipileupevent12;
      offlineth1ipileupevent12 = NULL;
    }
  if(offlineth1igoogevent12 != NULL)
    {
      delete offlineth1igoogevent12;
      offlineth1igoogevent12 = NULL;
    }
  if(offlineth1itraceevent12 != NULL)
    {
      delete offlineth1itraceevent12;
      offlineth1itraceevent12 = NULL;
    }
  if(offlineth1iwithoutteaceevent12 != NULL)
    {
      delete offlineth1iwithoutteaceevent12;
      offlineth1iwithoutteaceevent12 = NULL;
    }

  
  offlineth1ipileupevent12 = new TH1I("energy_pileupevent12","energy(pileup event)",65536,0,65536);
  offlineth1ipileupevent12->GetXaxis()->SetTitle("ch");
  offlineth1igoogevent12 = new TH1I("energy_googevent12","energy(good event)",65536,0,65536);
  offlineth1igoogevent12->GetXaxis()->SetTitle("ch");
  offlineth1itraceevent12 = new TH1I("energy_traceevent12","energy(trace length > 0)",65536,0,65536);
  offlineth1itraceevent12->GetXaxis()->SetTitle("ch");
  offlineth1iwithoutteaceevent12 = new TH1I("energy_withoutteaceevent12","energy(trace length == 0)",65536,0,65536);
  offlineth1iwithoutteaceevent12->GetXaxis()->SetTitle("ch");


  for (unsigned int i = 0; i < OfflineModuleEventsCount; ++i)
    {
      if(offlinechnum12->GetIntNumber() == offlinedata->GetEventChannel(i))//ch
	{
	  if(offlinedata->GetEventFinishCode(i))
	    {
	      offlineth1ipileupevent12->Fill(offlinedata->GetEventEnergy(i));
	    }
	  else
	    {
	      offlineth1igoogevent12->Fill(offlinedata->GetEventEnergy(i));
	    }

	  if(offlinedata->GetEventTraceLength(i) > 0)
	    {
	      offlineth1itraceevent12->Fill(offlinedata->GetEventEnergy(i));
	    }
	  else
	    {
	      offlineth1iwithoutteaceevent12->Fill(offlinedata->GetEventEnergy(i));
	    }
	}
    }

  canvas12->cd(1);
  offlineth1ipileupevent12->Draw();
  canvas12->cd(2);
  offlineth1itraceevent12->Draw();
  canvas12->cd(3);
  offlineth1igoogevent12->Draw();
  canvas12->cd(4);
  offlineth1iwithoutteaceevent12->Draw();
  canvas12->Modified();
  canvas12->Update();
  
  printtextinfor12->SetText("Draw Done!");
  OfflineDrawButton12->SetEnabled(1);
  OfflineReadFileButton->SetEnabled(1);
  gSystem->ProcessEvents();  
}

void Offline::Panel13StopDraw()
{
  flagdrawstop13 = true;
}

void Offline::Panel13Draw()
{
  OfflineReadFileButton->SetEnabled(0);
  OfflineDrawButton13->SetEnabled(0);
  OfflineStopButton13->SetEnabled(1);
  flagdrawstop13 = false;
  printtextinfor13->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
  printtextinfor13->SetText("Waitting ...");
  gSystem->ProcessEvents();

  for (int i = 0; i < 8; ++i)
    for (int j = 0; j < 64; ++j)
      if(offlineth1i13[i][j] != NULL)
	{
	  delete offlineth1i13[i][j];
	  offlineth1i13[i][j] = NULL;
	}

  if(detector->GetModuleADCMSPS(offlinemodnum->GetIntNumber()) == 500)
    {
      printtextinfor13->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
      printtextinfor13->SetText("500M module CFD is not adjustable.");
      
      canvas13->cd();
      canvas13->Clear();
      canvas13->Modified();
      canvas13->Update();
      OfflineDrawButton13->SetEnabled(1);
      OfflineStopButton13->SetEnabled(0);
      OfflineReadFileButton->SetEnabled(1);
      gSystem->ProcessEvents();
      return;
    }

  if(offlinechnumA13->GetIntNumber() == offlinechnumB13->GetIntNumber())
    {
      printtextinfor13->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
      printtextinfor13->SetText("Ch A == Ch B. Pelse re-select it.");
      
      canvas13->cd();
      canvas13->Clear();
      canvas13->Modified();
      canvas13->Update();
      OfflineDrawButton13->SetEnabled(1);
      OfflineStopButton13->SetEnabled(0);
      OfflineReadFileButton->SetEnabled(1);
      gSystem->ProcessEvents();
      return;
    }

  if(histxminmax13[1]->GetNumber() >= histxminmax13[2]->GetNumber())
    {
      std::cout<<"The range of the histogram is not suitable (xmin >= xmax). The recommended value will be used."<<std::endl;
      histxminmax13[1]->SetNumber(-500);
      histxminmax13[2]->SetNumber(500);
    }

  if(offlineenergylimit13->IsOn())
    {
      
      if(energylimitsab13[0]->GetNumber() >= energylimitsab13[1]->GetNumber())
	{
	  std::cout<<"The range of limits of energy is not suitable (A Left >= A Right). The recommended value will be used."<<std::endl;
	  energylimitsab13[0]->SetNumber(0);
	  energylimitsab13[1]->SetNumber(65536);
	}

      if(energylimitsab13[2]->GetNumber() >= energylimitsab13[3]->GetNumber())
	{
	  std::cout<<"The range of limits of energy is not suitable (B Left >= B Right). The recommended value will be used."<<std::endl;
	  energylimitsab13[2]->SetNumber(0);
	  energylimitsab13[3]->SetNumber(65536);
	}
    }

  for (int i = 0; i < 8; ++i)
    for (int j = 0; j < 64; ++j)
      {
	offlineth1i13[i][j] = new TH1I(TString::Format("offlineth1i13_%d_%d",i,j+1).Data(),"",histxminmax13[0]->GetNumber(),histxminmax13[1]->GetNumber(),histxminmax13[2]->GetNumber());
	offlineth1i13[i][j]->GetXaxis()->SetTitle("#DeltaT / ns");
	offlineth1i13[i][j]->SetTitle(TString::Format("W_%d",i).Data());
      }
  
  Long64_t deltaft;
  double deltat;
  bool flagenergylimits;
  short pointcfdA[8][64],pointcfdB[8][64];
  double cfdA[8][64],cfdB[8][64];
  for (unsigned int i = 0; i < OfflineModuleEventsCount; ++i)
    {
      if(offlinechnumA13->GetIntNumber() == offlinedata->GetEventChannel(i) && offlinedata->GetEventTraceLength(i) > 0)//ch
	{
	  for (unsigned int j = i; j < OfflineModuleEventsCount; ++j)
	    {
	      deltaft = ((Long64_t(offlinedata->GetEventTime_High(i)))*0x100000000+offlinedata->GetEventTime_Low(i))-((Long64_t(offlinedata->GetEventTime_High(j)))*0x100000000+offlinedata->GetEventTime_Low(j));
	      if(TMath::Abs(deltaft) < 100000)
		{
		  if(offlinechnumB13->GetIntNumber() == offlinedata->GetEventChannel(j) && offlinedata->GetEventTraceLength(j) > 0)//ch
		    {
		      if(detector->GetModuleADCMSPS(offlinemodnum->GetIntNumber()) == 250)
			deltat = deltaft*8.0;
		      else
			deltat = deltaft*10.0;

		      if(deltat >= histxminmax13[1]->GetNumber() && deltat <= histxminmax13[2]->GetNumber())
			{
			  flagenergylimits = true;
			  if(offlineenergylimit13->IsOn())
			    {
			      if((int(offlinedata->GetEventEnergy(i)) > energylimitsab13[0]->GetNumber()) && (int(offlinedata->GetEventEnergy(i)) < energylimitsab13[1]->GetNumber()) && (int(offlinedata->GetEventEnergy(j)) > energylimitsab13[2]->GetNumber()) && (int(offlinedata->GetEventEnergy(j)) < energylimitsab13[3]->GetNumber()))
				flagenergylimits = true;
			      else
				flagenergylimits = false;
			    }
			  if(flagenergylimits)
			    {


			      
			      int retval;
			      retval = Pixie16ComputeFastFiltersOffline(offlinefilename, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short)offlinechnumA13->GetIntNumber(), offlinedata->GetEventWaveLocation(i),offlinedata->GetEventTraceLength(i), RcdTraceA13, doublefastfilterA13, doublecfdA13);//trace length/trace location
			      if(retval < 0) ErrorInfo("Offline.cc", "Panel13Draw()", "Pixie16ComputeFastFiltersOffline", retval);
			      retval = Pixie16ComputeFastFiltersOffline(offlinefilename, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short)offlinechnumB13->GetIntNumber(), offlinedata->GetEventWaveLocation(j),offlinedata->GetEventTraceLength(j), RcdTraceB13, doublefastfilterB13, doublecfdB13);//trace length/trace location
			      if(retval < 0) ErrorInfo("Offline.cc", "Panel13Draw()", "Pixie16ComputeFastFiltersOffline", retval);

			      for (int mm = 0; mm < 8; ++mm)
				for (int nn = 0; nn < choosedelay13->GetSelected(); ++nn)
				{
				  HongyiWuPixie16ComputeCFDOffline(offlinedata->GetEventTraceLength(i),doublefastfilterA13,nn+1,mm,&pointcfdA[mm][nn],&cfdA[mm][nn]);
				  HongyiWuPixie16ComputeCFDOffline(offlinedata->GetEventTraceLength(j),doublefastfilterB13,nn+1,mm,&pointcfdB[mm][nn],&cfdB[mm][nn]);
				  if(pointcfdA[mm][nn] > -1 && pointcfdB[mm][nn] > -1)
				    {
				      if(detector->GetModuleADCMSPS(offlinemodnum->GetIntNumber()) == 100)
					offlineth1i13[mm][nn]->Fill((pointcfdA[mm][nn]-pointcfdB[mm][nn]+cfdA[mm][nn]-cfdB[mm][nn])*10+(((Long64_t(offlinedata->GetEventTime_High(i)))*0x100000000+offlinedata->GetEventTime_Low(i))-((Long64_t(offlinedata->GetEventTime_High(j)))*0x100000000+offlinedata->GetEventTime_Low(j)))*10);
				      else if(detector->GetModuleADCMSPS(offlinemodnum->GetIntNumber()) == 250)
					offlineth1i13[mm][nn]->Fill((pointcfdA[mm][nn]-pointcfdB[mm][nn]+cfdA[mm][nn]-cfdB[mm][nn])*4+(((Long64_t(offlinedata->GetEventTime_High(i)))*0x100000000+offlinedata->GetEventTime_Low(i))-((Long64_t(offlinedata->GetEventTime_High(j)))*0x100000000+offlinedata->GetEventTime_Low(j)))*8);
				    }
				}
			      
			     

		
		

			    }
			}
		    }
		}
	      else
	  	{
	  	  break;
	  	}
	    }

	  // =====

	  for (unsigned int j = i; j > 0; --j)
	    {
	      deltaft = ((Long64_t(offlinedata->GetEventTime_High(i)))*0x100000000+offlinedata->GetEventTime_Low(i))-((Long64_t(offlinedata->GetEventTime_High(j)))*0x100000000+offlinedata->GetEventTime_Low(j));
	      if(TMath::Abs(deltaft) < 100000)
		{
		  if(offlinechnumB13->GetIntNumber() == offlinedata->GetEventChannel(j) && offlinedata->GetEventTraceLength(j) > 0)//ch
		    {
		      if(detector->GetModuleADCMSPS(offlinemodnum->GetIntNumber()) == 250)
			deltat = deltaft*8.0;
		      else
			deltat = deltaft*10.0;

		      if(deltat >= histxminmax13[1]->GetNumber() && deltat <= histxminmax13[2]->GetNumber())
			{
			  flagenergylimits = true;
			  if(offlineenergylimit13->IsOn())
			    {
			      if((int(offlinedata->GetEventEnergy(i)) > energylimitsab13[0]->GetNumber()) && (int(offlinedata->GetEventEnergy(i)) < energylimitsab13[1]->GetNumber()) && (int(offlinedata->GetEventEnergy(j)) > energylimitsab13[2]->GetNumber()) && (int(offlinedata->GetEventEnergy(j)) < energylimitsab13[3]->GetNumber()))
				flagenergylimits = true;
			      else
				flagenergylimits = false;
			    }
			  if(flagenergylimits)
			    {


			      int retval;
			      retval = Pixie16ComputeFastFiltersOffline(offlinefilename, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short)offlinechnumA13->GetIntNumber(), offlinedata->GetEventWaveLocation(i),offlinedata->GetEventTraceLength(i), RcdTraceA13, doublefastfilterA13, doublecfdA13);//trace length/trace location
			      if(retval < 0) ErrorInfo("Offline.cc", "Panel13Draw()", "Pixie16ComputeFastFiltersOffline", retval);
			      retval = Pixie16ComputeFastFiltersOffline(offlinefilename, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short)offlinechnumB13->GetIntNumber(), offlinedata->GetEventWaveLocation(j),offlinedata->GetEventTraceLength(j), RcdTraceB13, doublefastfilterB13, doublecfdB13);//trace length/trace location
			      if(retval < 0) ErrorInfo("Offline.cc", "Panel13Draw()", "Pixie16ComputeFastFiltersOffline", retval);

			      for (int mm = 0; mm < 8; ++mm)
				for (int nn = 0; nn < choosedelay13->GetSelected(); ++nn)
				{
				  HongyiWuPixie16ComputeCFDOffline(offlinedata->GetEventTraceLength(i),doublefastfilterA13,nn+1,mm,&pointcfdA[mm][nn],&cfdA[mm][nn]);
				  HongyiWuPixie16ComputeCFDOffline(offlinedata->GetEventTraceLength(j),doublefastfilterB13,nn+1,mm,&pointcfdB[mm][nn],&cfdB[mm][nn]);
				  if(pointcfdA[mm][nn] > -1 && pointcfdB[mm][nn] > -1)
				    {
				      if(detector->GetModuleADCMSPS(offlinemodnum->GetIntNumber()) == 100)
					offlineth1i13[mm][nn]->Fill((pointcfdA[mm][nn]-pointcfdB[mm][nn]+cfdA[mm][nn]-cfdB[mm][nn])*10+(((Long64_t(offlinedata->GetEventTime_High(i)))*0x100000000+offlinedata->GetEventTime_Low(i))-((Long64_t(offlinedata->GetEventTime_High(j)))*0x100000000+offlinedata->GetEventTime_Low(j)))*10);
				      else if(detector->GetModuleADCMSPS(offlinemodnum->GetIntNumber()) == 250)
					offlineth1i13[mm][nn]->Fill((pointcfdA[mm][nn]-pointcfdB[mm][nn]+cfdA[mm][nn]-cfdB[mm][nn])*4+(((Long64_t(offlinedata->GetEventTime_High(i)))*0x100000000+offlinedata->GetEventTime_Low(i))-((Long64_t(offlinedata->GetEventTime_High(j)))*0x100000000+offlinedata->GetEventTime_Low(j)))*8);
				    }
				}




			      

			    }
			}
		    }
		}
	      else
	  	{
	  	  break;
	  	}
	    }
	  
	}// ch

      if(i%500 == 0)
	{
	  if(flagdrawstop13) break;
	  printtextinfor13->SetText(TString::Format("Drawing... ==> %d/%d",i,OfflineModuleEventsCount).Data());
	  gSystem->ProcessEvents();
	}

    }// for OfflineModuleEventsCount


  printtextinfor13->SetTextColor(TColor::RGB2Pixel(COLOR_DODERBLUE_R,COLOR_DODERBLUE_G,COLOR_DODERBLUE_B), false);
  printtextinfor13->SetText("Done!");

  canvas13->cd();
  canvas13->Clear();
  canvas13->Divide(4,2);

  TLegend *legend[8];
  double histmax[8] = {0};
  for (int mm = 0; mm < 8; ++mm)
    for (int nn = 0; nn < choosedelay13->GetSelected(); ++nn)
      {
	if(offlineth1i13[mm][nn]->GetMaximum() > histmax[mm])
	  histmax[mm] = offlineth1i13[mm][nn]->GetMaximum();

	if(nn==0)
	  {
	    legend[mm] = new TLegend(0.77,0.25,0.87,0.82);
	    legend[mm]->SetBorderSize(0);
	  }
	legend[mm]->AddEntry(offlineth1i13[mm][nn],detector->GetModuleADCMSPS(offlinemodnum->GetIntNumber()) == 250?TString::Format("%0.3f",(nn+1)*0.004).Data():TString::Format("%0.3f",(nn+1)*0.01).Data(),"L");
      }

  for (int mm = 0; mm < 8; ++mm)
    for (int nn = 0; nn < choosedelay13->GetSelected(); ++nn)
      {
	offlineth1i13[mm][nn]->SetLineColor(nn+1);
	if(nn==9 || nn==19) offlineth1i13[mm][nn]->SetLineColor(21);

	offlineth1i13[mm][nn]->GetYaxis()->SetRangeUser(0,1.1*histmax[mm]);
	
	canvas13->cd(mm+1);
	if(nn==0)
	  offlineth1i13[mm][nn]->Draw();
	else
	  offlineth1i13[mm][nn]->Draw("same");

	if(nn == choosedelay13->GetSelected()-1) legend[mm]->Draw();
      }
  canvas13->Modified();
  canvas13->Update();
  OfflineDrawButton13->SetEnabled(1);
  OfflineStopButton13->SetEnabled(0);
  OfflineReadFileButton->SetEnabled(1);
  gSystem->ProcessEvents();
}

void Offline::Panel14StopDraw()
{
  flagdrawstop14 = true;
}

void Offline::Panel14Draw()
{
  OfflineReadFileButton->SetEnabled(0);
  OfflineDrawButton14->SetEnabled(0);
  OfflineStopButton14->SetEnabled(1);
  flagdrawstop14 = false;
  printtextinfor14->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
  printtextinfor14->SetText("Waitting ...");
  gSystem->ProcessEvents();

  if(RcdTrace14 != NULL)
    {
      delete []RcdTrace14;
      RcdTrace14 = NULL;
    }
  if(doublefastfilter14 != NULL)
    {
      delete []doublefastfilter14;
      doublefastfilter14 = NULL;
    }
  if(offlineth1i14 != NULL)
    {
      delete offlineth1i14;
      offlineth1i14 = NULL;
    }
  if(offlineth2i14 != NULL)
    {
      delete offlineth2i14;
      offlineth2i14 = NULL;
    }

  
  canvas14->cd();
  canvas14->Clear();
  canvas14->Modified();
  canvas14->Update();
  gSystem->ProcessEvents();

  offlineth1i14 = new TH1I("offlineth1i14","",1000,0,1);
  offlineth1i14->GetXaxis()->SetTitle("fraction");
  offlineth1i14->SetTitle("0.125->7 0.25->6 0.375->5 0.5->4 0.625->3 0.75->2 0.875->1 1.0->0");
  offlineth2i14 = new TH2I("offlineth2i14","",1000,0,1,1024,0,65536);
  offlineth2i14->GetXaxis()->SetTitle("fraction");
  offlineth2i14->GetYaxis()->SetTitle("Energy[ch]");
  
  int inttracelength = -1;
  for (unsigned int i = 0; i < OfflineModuleEventsCount; ++i)
    {
      if(offlinechnum14->GetIntNumber() == offlinedata->GetEventChannel(i) && offlinedata->GetEventTraceLength(i) > 0)//ch / trace length>0
	{
	  inttracelength = offlinedata->GetEventTraceLength(i);//trace length
	  break;
	}
    }

  if(inttracelength > -1)
    {
      RcdTrace14 = new unsigned short[65536];
      doublefastfilter14 = new double[65536];

      int retval;
      unsigned int FastLen, FastGap, FastFilterRange;
      double ChanParData;

      retval = Pixie16ReadSglModPar((char*)"FAST_FILTER_RANGE", &FastFilterRange, (unsigned short)offlinemodnum->GetIntNumber());
      if(retval < 0) ErrorInfo("Offline.cc", "Panel14Draw()", "Pixie16ReadSglModPar/FAST_FILTER_RANGE", retval);

      retval = Pixie16ReadSglChanPar((char*)"TRIGGER_RISETIME", &ChanParData, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short)offlinechnum14->GetIntNumber());
      if(retval < 0) ErrorInfo("Offline.cc", "Panel14Draw()", "Pixie16ReadSglChanPar/TRIGGER_RISETIME", retval);
      switch(detector->GetModuleADCMSPS(offlinemodnum->GetIntNumber()))
	{
	case 100 ://100
	  FastLen = ROUND(ChanParData*100/std::pow(2.0,(double)FastFilterRange))*std::pow(2.0,(double)FastFilterRange);
	  break;
	case 250 ://250
	  FastLen = ROUND(ChanParData*125/std::pow(2.0,(double)FastFilterRange))*std::pow(2.0,(double)FastFilterRange);
	  break;	  
	case 500 ://500
	  FastLen = ROUND(ChanParData*100/std::pow(2.0,(double)FastFilterRange))*std::pow(2.0,(double)FastFilterRange);
	  break;
	default:
	  std::cout<<"ERROR: Please call Hongyi Wu(wuhongyi@qq.com)"<<std::endl;
	  break;
	}

      retval = Pixie16ReadSglChanPar((char*)"TRIGGER_FLATTOP", &ChanParData, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short)offlinechnum14->GetIntNumber());
      if(retval < 0) ErrorInfo("Offline.cc", "Panel14Draw()", "Pixie16ReadSglChanPar/TRIGGER_FLATTOP", retval);
      switch(detector->GetModuleADCMSPS(offlinemodnum->GetIntNumber()))
	{
	case 100 ://100
	  FastGap = ROUND(ChanParData*100/std::pow(2.0,(double)FastFilterRange))*std::pow(2.0,(double)FastFilterRange);
	  break;
	case 250 ://250
	  FastGap = ROUND(ChanParData*125/std::pow(2.0,(double)FastFilterRange))*std::pow(2.0,(double)FastFilterRange);
	  break;	  
	case 500 ://500
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
	  if(offlinechnum14->GetIntNumber() == offlinedata->GetEventChannel(i) && offlinedata->GetEventTraceLength(i) > 0)//ch / trace length>0
	    {	      
	      ListModeFile = fopen(offlinefilename, "rb");
	      if(ListModeFile != NULL)
		{
		  fseek(ListModeFile, offlinedata->GetEventWaveLocation(i)*4, SEEK_SET);// Position ListModeFile to the requested trace location
		  fread(RcdTrace14, 2, offlinedata->GetEventTraceLength(i), ListModeFile);// Read trace
		  fclose(ListModeFile);// Close file
		  ListModeFile = NULL;
		
		  // Compute fast filter response
		  offset = 2*FastLen + FastGap - 1;
		  for(x = offset; x < offlinedata->GetEventTraceLength(i); x++)
		    {
		      fsum0 = 0;
		      for(y = (x-offset); y < (x-offset+FastLen); y++)
			{
			  fsum0 += RcdTrace14[y];
			}
		      fsum1 = 0;
		      for(y = (x-offset+FastLen+FastGap); y < (x-offset+2*FastLen+FastGap); y++)
			{
			  fsum1 += RcdTrace14[y];
			}
		      doublefastfilter14[x] = ((double)fsum1 - (double)fsum0)/(double)FastLen;
		    }

		  for(x = 0; x < offset; x++)
		    {
		      doublefastfilter14[x] = doublefastfilter14[offset];
		    }

		  int flagfffirst = -1;
		  double fffirst = -1;
		  for (int z = 0; z < (int)offlinedata->GetEventTraceLength(i); ++z)//trace length
		    {
		      if(doublefastfilter14[z] > fffirst)
			{
			  fffirst = doublefastfilter14[z];
			  flagfffirst = z;
			}
		    }

		  double slope = -1;
		  int flagslopemax = -1;
		  if(flagfffirst > 0)
		    {
		      for (int z = flagfffirst-1; z > 0; --z)
			{
			  if(doublefastfilter14[z]/doublefastfilter14[flagfffirst] > 0.01 && doublefastfilter14[z] > 1)
			    {
			      if(doublefastfilter14[z+1]-doublefastfilter14[z] >slope)
				{
				  slope = doublefastfilter14[z+1]-doublefastfilter14[z];
				  flagslopemax = z;
				}
			    }
			  else
			    {
			      break;
			    }
			}
		      if(flagslopemax > 0)
			{
			  offlineth1i14->Fill((doublefastfilter14[flagslopemax]+doublefastfilter14[flagslopemax+1])/2.0/doublefastfilter14[flagfffirst]);
			  offlineth2i14->Fill((doublefastfilter14[flagslopemax]+doublefastfilter14[flagslopemax+1])/2.0/doublefastfilter14[flagfffirst],offlinedata->GetEventEnergy(i));
			}
		    }

		}//fast filter
       
	    }//ch

	  if(i%500 == 0)
	    {
	      if(flagdrawstop14) break;
	      printtextinfor14->SetText(TString::Format("Drawing... ==> %d/%d",i,OfflineModuleEventsCount).Data());
	      gSystem->ProcessEvents();
	    }
	}//for i
      
  
    }// inttracelength > -1
  
  printtextinfor14->SetText("Draw Done!");
  canvas14->Divide(2,1);
  canvas14->cd(1);
  offlineth2i14->Draw("colz");
  canvas14->cd(2);
  offlineth1i14->Draw();
  canvas14->Modified();
  canvas14->Update();
  OfflineDrawButton14->SetEnabled(1);
  OfflineStopButton14->SetEnabled(0);
  OfflineReadFileButton->SetEnabled(1);
  gSystem->ProcessEvents();
}
  

void Offline::Panel0ReadFile()
{
  modNumber = offlinemodnum->GetIntNumber();
  if(modNumber > detector->NumModules-1) modNumber = detector->NumModules-1;
  if(modNumber < 0) modNumber = 0;
  offlinemodnum->SetIntNumber(modNumber);
  
  if(offlinedata != NULL)
    {
      delete offlinedata;
      offlinedata = NULL;
    }
  offlinedata = new OfflineData;
  switch(detector->GetModuleADCMSPS(offlinemodnum->GetIntNumber()))
    {
    case 100 ://100
      offlinedata->SetSamplingRate(100);
      break;
    case 250 ://250
      offlinedata->SetSamplingRate(250);
      break;	  
    case 500 ://500
      offlinedata->SetSamplingRate(500);
      break;
    default:
      std::cout<<"ERROR: Please call Hongyi Wu(wuhongyi@qq.com)"<<std::endl;
      break;
    }


  
  OfflineCurrentCount = -1;
  OfflineFileStatus->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  OfflineFileStatus->SetText("Waitting...");
  gSystem->ProcessEvents();

  if(headerrawenergysumsandbaseline->IsOn())
    {
      offlinedata->FlagRawEnergySumsAndBaseline(true);
    }
  if(headerqdcsums->IsOn())
    {
      offlinedata->FlagQDCs(true);
    }
  if(headerexternaltimestamp->IsOn())
    {
      offlinedata->FlagExternalTimestamp(true);
    }
  
  sprintf(offlinefilename,"%s%04d/%s_R%04d_M%02d.bin",filepathtext->GetText(),int(offlinefilerunnum->GetIntNumber()),filenametext->GetText(),int(offlinefilerunnum->GetIntNumber()),int(offlinemodnum->GetIntNumber()));

  offlinedata->SetFileName(offlinefilename);
  offlinedata->ReadModuleEvents();
  
  // GOTO need check file 
  OfflineModuleEventsCount = offlinedata->GetOfflineModuleEventsCount();

  if(OfflineModuleEventsCount > 0)
    {
      offlinedata->ReadEventsInfo();

      OfflineFileStatus->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
      char staok[20];
      sprintf(staok,"Total Events: %d",OfflineModuleEventsCount);
      OfflineFileStatus->SetText(staok);
	      
      DrawButtonStatus(true);
    }
  else
    {
      OfflineFileStatus->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
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
  DrawButtonStatus(false);
  // OfflineFileStatus->SetText("NOT READ");
  // if(headerrawenergysumsandbaseline->IsOn())
  //   {
  //     chooseslowfilterbaseline->SetEnabled(1);
  //     oldslowfilterparameter[0]->SetEnabled(1);
  //     oldslowfilterparameter[1]->SetEnabled(1);
  //     oldslowfilterparameter[2]->SetEnabled(1);
  //     oldofflinefilterrange->SetEditDisabled(0);
      
  //     chooseslowfilterbaselinep6->SetEnabled(1);
  //     oldslowfilterparameterp6[0]->SetEnabled(1);
  //     oldslowfilterparameterp6[1]->SetEnabled(1);
  //     oldslowfilterparameterp6[2]->SetEnabled(1);
  //     oldofflinefilterrangep6->SetEditDisabled(0);
  //   }
  // else
  //   {
  //     chooseslowfilterbaseline->SetEnabled(0);
  //     oldslowfilterparameter[0]->SetEnabled(0);
  //     oldslowfilterparameter[1]->SetEnabled(0);
  //     oldslowfilterparameter[2]->SetEnabled(0);
  //     oldofflinefilterrange->SetEditDisabled(1);
	
  //     chooseslowfilterbaselinep6->SetEnabled(0);
  //     oldslowfilterparameterp6[0]->SetEnabled(0);
  //     oldslowfilterparameterp6[1]->SetEnabled(0);
  //     oldslowfilterparameterp6[2]->SetEnabled(0);
  //     oldofflinefilterrangep6->SetEditDisabled(1);
  //   }
}

void Offline::SelectQDCSums(Bool_t on)
{
  std::cout<<"TODO  =  SelectQDCSums: "<<headerqdcsums->IsOn()<<std::endl;
  DrawButtonStatus(false);
  // OfflineFileStatus->SetText("NOT READ");
}

void Offline::SelectExternalTimestamp(Bool_t on)
{
  std::cout<<"TODO  =  SelectExternalTimestamp: "<<headerexternaltimestamp->IsOn()<<std::endl;
  DrawButtonStatus(false);
  // OfflineFileStatus->SetText("NOT READ");
}

void Offline::DrawButtonStatus(bool flag)
{
  OfflineDrawButton->SetEnabled(flag);
  OfflineDrawButton2->SetEnabled(flag);
  OfflineDrawButton3->SetEnabled(flag);
  OfflineDrawButton4->SetEnabled(flag);
  OfflineDrawButton5->SetEnabled(flag);
  OfflineDrawButton6->SetEnabled(flag);
  OfflineDrawButton8->SetEnabled(flag);
  OfflineDrawButton9->SetEnabled(flag);
  OfflineDrawButton10->SetEnabled(flag);
  OfflineDrawButton11->SetEnabled(flag);
  OfflineDrawButton12->SetEnabled(flag);
  OfflineDrawButton13->SetEnabled(flag);
  OfflineDrawButton14->SetEnabled(flag);
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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int Offline::CAEN_FFT(unsigned short *wave, double *fft, int ns, int WindowType)
{
  int m,n,ip,le,le1,nm1,k,l,j,i,nv2;
  double u1,u2,u3,arg,c,s,t1,t2,t3,t4;
  double *x, *y;

  /* ns should be a power of 2. If it is not, find the maximum m
     such that n = 2^m < ns and get n samples instead of ns.*/
  i = 1;
  while((int)std::pow(2,i) < ns)
    i++;
  if(ns == (int)(std::pow(2,i))) {
    m = i;
    n = ns;
  }
  else {
    m = i - 1;
    n = (int)std::pow(2,m);
  }

  /* allocate the memory buffers for the real and imaginary parts of the fft */
  x = (double *)malloc(n * sizeof(double));
  y = (double *)malloc(n * sizeof(double));

  /* apply the windowing to the input vector */
  for(i=0; i<n; i++) {
    y[i] = 0.0; /* imaginary part of the input vector (always 0) */
    switch (WindowType) {
    case HANNING_FFT_WINDOW  :  
      x[i] = wave[i] * (0.5 - 0.5 * std::cos(2*TMath::Pi() * i/n));
      break;
    case HAMMING_FFT_WINDOW  :  
      x[i] = wave[i] * (0.54 - 0.46 * std::cos(2*TMath::Pi() * i/n)); 
      break;
    case BLACKMAN_FFT_WINDOW  :  
      x[i] = wave[i] * (2.4 * (0.42323 - 0.49755*cos(2*TMath::Pi()*i/n) + 0.07922*std::cos(4*TMath::Pi()*i/n)));
      break;
    case RECT_FFT_WINDOW  :  
      x[i] = wave[i];
      break;
    default :  
      x[i] = wave[i] * (2.4*(0.42323-0.49755*std::cos(2*TMath::Pi()*(i)/n)+0.07922*std::cos(4*TMath::Pi()*(i)/n)));
      break;
    }
  }

  /* now the vectors x and y represents the input waveform expressed as imaginary numbers
     after the appplication of the windowing. */

  /* calculate the FFT */
  for(l=0; l<m; l++) {
    le=(int)std::pow(2,m-l);
    le1=le/2;
    u1=1.0;
    u2=0.0;
    arg=TMath::Pi()/le1;
    c=std::cos(arg);
    s=-std::sin(arg);

    for (j=0; j<le1; j++) {
      for (i=j; i<n; i=i+le) {
	ip=i+le1;
	t1=x[i]+x[ip];
	t2=y[i]+y[ip];
	t3=x[i]-x[ip];
	t4=y[i]-y[ip];
	x[ip]=t3*u1-t4*u2;
	y[ip]=t4*u1+t3*u2;
	x[i]=t1;
	y[i]=t2;
      }
      u3=u1*c-u2*s;
      u2=u2*c+u1*s;
      u1=u3;
    }
  }

  nv2=n/2;
  nm1=n-1;
  j=0;
  i=0;
  while (i<nm1) {
    if(i>j)
      goto rif;
    t1=x[j];
    t2=y[j];
    x[j]=x[i];
    y[j]=y[i];
    x[i]=t1;
    y[i]=t2;
  rif:
    k=nv2;
  rife:
    if (k>j)
      goto rifer;
    j=j-k;
    k=k/2;
    goto rife;
  rifer:
    j=j+k;
    i++;
  }

  /* get the amplitude of the FFT (modulo) */
  y[0]=y[0]/n;
  x[0]=x[0]/n;
  fft[0]=std::sqrt(x[0]*x[0] + y[0]*y[0]);
  for(i=1;i<n/2;i++) {
    y[i]=2*y[i]/n;
    x[i]=2*x[i]/n;
    fft[i]=std::sqrt(x[i]*x[i] + y[i]*y[i]);
  }

  /* Add the baseline, normalize and transform in dB */
  for(i=0; i<n/2; i++) 
    fft[i] = 20 * std::log10( fft[i]/NORM_FACTOR + FFT_BASELINE);

  /* free the buffers and return the number of points (only half FFT) */
  free(x);
  free(y);
  return (n/2);
}






// 
// Offline.cc ends here
