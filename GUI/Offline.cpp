// Offline.cpp --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 7月 29 20:39:43 2016 (+0800)
// Last-Updated: 三 10月  5 14:39:29 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 105
// URL: http://wuhongyi.cn 

#include "Offline.h"
#include "Detector.h"
#include "Global.h"

#include "TGTab.h"
#include "pixie16app_export.h"
#include "pixie16sys_export.h"
#include <iostream>
using namespace std;

Offline::Offline(const TGWindow * p, const TGWindow * main,Detector *det,TGTextEntry *filepath, TGTextEntry *filename)
{
  SetCleanup(kDeepCleanup);
  detector = det;
  filepathtext = filepath;
  filenametext = filename;

  modNumber = 0;
  chanNumber = 0;
  fileRunNum = 0;
  
  OfflineEventInformation = NULL;
  OfflineCurrentCount = -1;
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
  offlinemultigraph = new TMultiGraph();

  fileRunNum2 = 0;
  modNumber2 = 0;
  for (int i = 0; i < 16; ++i)
    {
      offlinemultigraph2[i] = new TMultiGraph();
      OfflineCurrentCount2[i] = -1;
      
      rawdata2[i] = NULL;
      threshdata2[i] = NULL;
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
  OfflineEventInformation2 = NULL;

  fileRunNum3 = 0;
  modNumber3 = 0;
  for (int i = 0; i < 16; ++i)
    {
      offlineth1d3[i] = NULL;
    }
  OfflineEventInformation3 = NULL;

  
  TGTab *TabPanel = new TGTab(this);
  this->AddFrame(TabPanel, new TGLayoutHints(kLHintsBottom | kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
  TGCompositeFrame *Tab1 = TabPanel->AddTab("Adjust Par");
  MakeFold1Panel(Tab1);

  TGCompositeFrame *Tab2 = TabPanel->AddTab("16 Chan");
  MakeFold2Panel(Tab2);

  TGCompositeFrame *Tab3 = TabPanel->AddTab("Energy");
  MakeFold3Panel(Tab3);


  SetWindowName("Adjust Par");
  MapSubwindows();
  MapWindow();
  Resize();
}

Offline::~Offline()
{
  if(rawdata != NULL)
    delete rawdata;
  if(threshdata != NULL)
    delete threshdata;
  if(cfddata != NULL)
    delete cfddata;
  if(sfilterdata != NULL)
    delete sfilterdata;
  if(ffilterdata != NULL)
    delete ffilterdata;
  if(RcdTrace != NULL)
    delete []RcdTrace;
  if(doublethresh != NULL)
    delete []doublethresh;
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

  for (int i = 0; i < 16; ++i)
    {
      if(rawdata2[i] != NULL)
  	delete rawdata2[i];
      if(threshdata2[i] != NULL)
  	delete threshdata2[i];
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

  // for (int i = 0; i < 16; ++i)
  //   {
  //     if(offlineth1d3[i] != NULL)
  // 	delete offlineth1d3[i];
  //   }
  

  if(OfflineEventInformation != NULL)
    delete []OfflineEventInformation;
  if(OfflineEventInformation2 != NULL)
    delete []OfflineEventInformation2;
  if(OfflineEventInformation3 != NULL)
    delete []OfflineEventInformation3;

      
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
  filterFrame->AddFrame(fastlength, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 0, 0));
  offlinefilters[0] = new TGNumberEntryField(filterFrame, OFFLINEFASTLENGTH, 0, TGNumberFormat::kNESReal);
  filterFrame->AddFrame(offlinefilters[0], new TGLayoutHints( kLHintsExpandX | kLHintsTop, 1, 0, 0, 0));

  // fastgap
  TGLabel *fastgap = new TGLabel( filterFrame, "FGap:");
  filterFrame->AddFrame(fastgap, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 0, 0));
  offlinefilters[1] = new TGNumberEntryField(filterFrame, OFFLINEFASTGAP, 0, TGNumberFormat::kNESReal);
  filterFrame->AddFrame(offlinefilters[1], new TGLayoutHints( kLHintsExpandX | kLHintsTop, 1, 0, 0, 0));

  // slowlength
  TGLabel *lowlength = new TGLabel( filterFrame, "SLen:");
  filterFrame->AddFrame(lowlength, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 0, 0));
  offlinefilters[2] = new TGNumberEntryField(filterFrame, OFFLINESLOWLENGTH, 0, TGNumberFormat::kNESReal);
  filterFrame->AddFrame(offlinefilters[2], new TGLayoutHints( kLHintsExpandX | kLHintsTop, 1, 0, 0, 0));

  // slowgap
  TGLabel *lowgap = new TGLabel( filterFrame, "SGap:");
  filterFrame->AddFrame(lowgap, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 0, 0));
  offlinefilters[3] = new TGNumberEntryField(filterFrame, OFFLINESLOWGAP, 0, TGNumberFormat::kNESReal);
  filterFrame->AddFrame(offlinefilters[3], new TGLayoutHints( kLHintsExpandX | kLHintsTop, 1, 0, 0, 0));

  // preamptau
  TGLabel *preamptau = new TGLabel( filterFrame, "Tau:");
  filterFrame->AddFrame(preamptau, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 0, 0));
  offlinefilters[4] = new TGNumberEntryField(filterFrame, OFFLINEPREAMPTAU, 0, TGNumberFormat::kNESReal);
  filterFrame->AddFrame(offlinefilters[4], new TGLayoutHints( kLHintsExpandX | kLHintsTop, 1, 0, 0, 0));

  // cfddelay
  TGLabel *cfddelay = new TGLabel( filterFrame, "CFDDe:");
  filterFrame->AddFrame(cfddelay, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 0, 0));
  offlinefilters[5] = new TGNumberEntryField(filterFrame, OFFLINECFDDELAY, 0, TGNumberFormat::kNESReal);
  filterFrame->AddFrame(offlinefilters[5], new TGLayoutHints( kLHintsExpandX | kLHintsTop, 1, 0, 0, 0));

  // cfdscale
  TGLabel *cfdscale = new TGLabel( filterFrame, "CFDSc:");
  filterFrame->AddFrame(cfdscale, new TGLayoutHints (kLHintsLeft | kLHintsTop, 1, 2, 0, 0));
  offlinefilters[6] = new TGNumberEntryField(filterFrame, OFFLINECFDSCALE, 0, TGNumberFormat::kNESReal);
  filterFrame->AddFrame(offlinefilters[6], new TGLayoutHints( kLHintsExpandX | kLHintsTop, 1, 0, 0, 0));

  // thresh
  TGLabel *thresh = new TGLabel( filterFrame, "Thresh:");
  filterFrame->AddFrame(thresh, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 0, 0));
  offlinefilters[7] = new TGNumberEntryField(filterFrame, OFFLINETHRESH, 0, TGNumberFormat::kNESReal);
  filterFrame->AddFrame(offlinefilters[7], new TGLayoutHints( kLHintsExpandX | kLHintsTop, 1, 0, 0, 0));

  // slowfilterrange
  TGLabel *slowfilterrange = new TGLabel(filterFrame, "FilRan:");
  fClient->GetColorByName("red", color);
  slowfilterrange->SetTextColor(color, false);
  filterFrame->AddFrame(slowfilterrange, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 0, 0));
  offlinefilterrange = new TGNumberEntry(filterFrame, 0, 2, OFFLINEFILTERRANGE, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 1, 6);
  filterFrame->AddFrame(offlinefilterrange, new TGLayoutHints( kLHintsExpandX | kLHintsTop, 1, 0, 0, 0));
  offlinefilterrange->SetButtonToNum(0);
  offlinefilterrange->Associate(this);

  
  TabPanel->AddFrame(filterFrame, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 2, 2, 1, 1));

  // ===

  TGCompositeFrame *parFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);

  // run
  TGLabel *run = new TGLabel( parFrame, "Run:");
  parFrame->AddFrame(run, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 0, 0));
  offlinefilerunnum = new TGNumberEntry(parFrame, 0, 4, OFFLINERUNNUM, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, 9999);
  offlinefilerunnum->SetButtonToNum(0);
  offlinefilerunnum->Associate(this);
  parFrame->AddFrame(offlinefilerunnum, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 0, 0));

  // mod
  TGLabel *mod = new TGLabel( parFrame, "Mod:"); 
  parFrame->AddFrame(mod, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 0, 0));
  offlinemodnum = new TGNumberEntry (parFrame, 0, 2, OFFLINEMODNUM, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, PRESET_MAX_MODULES-1);
  offlinemodnum->SetButtonToNum(0);
  offlinemodnum->Associate(this);
  parFrame->AddFrame(offlinemodnum, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 0, 0));

  
  // read
  OfflineReadFileButton = new TGTextButton( parFrame, "&Read", OFFLINEREAD);
  OfflineReadFileButton->Associate (this);
  parFrame->AddFrame(OfflineReadFileButton, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 0, 0));

  // status
  OfflineFileStatus = new TGTextEntry(parFrame,new TGTextBuffer(30), 10000);
  OfflineFileStatus-> SetFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1", false);
  fClient->GetColorByName("blue", color);
  OfflineFileStatus->SetTextColor(color, false);
  OfflineFileStatus->SetText("NOT READ");
  OfflineFileStatus->Resize(100, 12);
  OfflineFileStatus->SetEnabled(kFALSE);
  OfflineFileStatus->SetFrameDrawn(kFALSE);
  parFrame->AddFrame(OfflineFileStatus, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 6, 0));// 
  
  // ch
  TGLabel *ch = new TGLabel( parFrame, "Ch:"); 
  parFrame->AddFrame(ch, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 0, 0));
  offlinechnum = new TGNumberEntry (parFrame, 0, 2, OFFLINECHNUM, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, 15);
  parFrame->AddFrame(offlinechnum, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 0, 0));
  offlinechnum->SetButtonToNum(0);
  offlinechnum->Associate(this);


  
  // load
  OfflineLoadButton = new TGTextButton( parFrame, "&Load", OFFLINELOAD);
  OfflineLoadButton->Associate(this);
  parFrame->AddFrame( OfflineLoadButton, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 0, 0));
  // apply
  OfflineApplyButton = new TGTextButton( parFrame, "&Apply", OFFLINEAPPLY);
  OfflineApplyButton->Associate(this);
  parFrame->AddFrame( OfflineApplyButton , new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 0, 0));
  // draw
  OfflineDrawButton = new TGTextButton( parFrame, "&Draw", OFFLINEDRAW);
  OfflineDrawButton->SetEnabled(0);
  OfflineDrawButton->Associate(this);
  parFrame->AddFrame(OfflineDrawButton, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 0, 0));


  // current count
  OfflineCurrentCountText = new TGTextEntry(parFrame,new TGTextBuffer(30), 10000);
  OfflineCurrentCountText-> SetFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1", false);
  fClient->GetColorByName("blue", color);
  OfflineCurrentCountText->SetTextColor(color, false);
  OfflineCurrentCountText->SetText("-1");
  OfflineCurrentCountText->Resize(100, 12);
  OfflineCurrentCountText->SetEnabled(kFALSE);
  OfflineCurrentCountText->SetFrameDrawn(kFALSE);
  parFrame->AddFrame(OfflineCurrentCountText, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 6, 0));

  
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
  TGCompositeFrame *adCanvasFrame = new TGCompositeFrame(TabPanel, 800, 800, kHorizontalFrame);
  TGLayoutHints *Hint = new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 1, 1, 1, 1);

  TRootEmbeddedCanvas *adjCanvas = new TRootEmbeddedCanvas("canvas2", adCanvasFrame, 100, 100);

  canvas2 = adjCanvas->GetCanvas();
  canvas2->Divide(4,4);
  adCanvasFrame->AddFrame(adjCanvas, Hint);
  TabPanel->AddFrame(adCanvasFrame, Hint);


  
  TGCompositeFrame *parFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);

  // run
  TGLabel *run = new TGLabel( parFrame, "Run:");
  parFrame->AddFrame(run, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 0, 0));
  offlinefilerunnum2 = new TGNumberEntry(parFrame, 0, 4, OFFLINERUNNUM2, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, 9999);
  offlinefilerunnum2->SetButtonToNum(0);
  offlinefilerunnum2->Associate(this);
  parFrame->AddFrame(offlinefilerunnum2, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 0, 0));

  // mod
  TGLabel *mod = new TGLabel( parFrame, "Mod:"); 
  parFrame->AddFrame(mod, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 0, 0));
  offlinemodnum2 = new TGNumberEntry(parFrame, 0, 2, OFFLINEMODNUM2, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, PRESET_MAX_MODULES-1);
  offlinemodnum2->SetButtonToNum(0);
  offlinemodnum2->Associate(this);
  parFrame->AddFrame(offlinemodnum2, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 0, 0));

  // read
  OfflineReadFileButton2 = new TGTextButton( parFrame, "&Read", OFFLINEREAD2);
  OfflineReadFileButton2->Associate (this);
  parFrame->AddFrame(OfflineReadFileButton2, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 0, 0));

  // status
  OfflineFileStatus2 = new TGTextEntry(parFrame,new TGTextBuffer(30), 10000);
  OfflineFileStatus2-> SetFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1", false);
  fClient->GetColorByName("blue", color);
  OfflineFileStatus2->SetTextColor(color, false);
  OfflineFileStatus2->SetText("NOT READ");
  OfflineFileStatus2->Resize(100, 12);
  OfflineFileStatus2->SetEnabled(kFALSE);
  OfflineFileStatus2->SetFrameDrawn(kFALSE);
  parFrame->AddFrame(OfflineFileStatus2, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 6, 0));// 
  // draw
  OfflineDrawButton2 = new TGTextButton( parFrame, "&Draw", OFFLINEDRAW2);
  OfflineDrawButton2->SetEnabled(0);
  OfflineDrawButton2->Associate(this);
  parFrame->AddFrame(OfflineDrawButton2, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 0, 0));


  TabPanel->AddFrame(parFrame, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 2, 2, 1, 1));
  
}

void Offline::MakeFold3Panel(TGCompositeFrame *TabPanel)
{
  TGCompositeFrame *adCanvasFrame = new TGCompositeFrame(TabPanel, 800, 800, kHorizontalFrame);
  TGLayoutHints *Hint = new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 1, 1, 1, 1);

  TRootEmbeddedCanvas *adjCanvas = new TRootEmbeddedCanvas("canvas3", adCanvasFrame, 100, 100);

  canvas3 = adjCanvas->GetCanvas();
  adCanvasFrame->AddFrame(adjCanvas, Hint);
  TabPanel->AddFrame(adCanvasFrame, Hint);

  TGCompositeFrame *parFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);

  // run
  TGLabel *run = new TGLabel( parFrame, "Run:");
  parFrame->AddFrame(run, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 0, 0));
  offlinefilerunnum3 = new TGNumberEntry(parFrame, 0, 4, OFFLINERUNNUM3, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, 9999);
  offlinefilerunnum3->SetButtonToNum(0);
  offlinefilerunnum3->Associate(this);
  parFrame->AddFrame(offlinefilerunnum3, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 0, 0));

  // mod
  TGLabel *mod = new TGLabel( parFrame, "Mod:"); 
  parFrame->AddFrame(mod, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 0, 0));
  offlinemodnum3 = new TGNumberEntry(parFrame, 0, 2, OFFLINEMODNUM3, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, PRESET_MAX_MODULES-1);
  offlinemodnum3->SetButtonToNum(0);
  offlinemodnum3->Associate(this);
  parFrame->AddFrame(offlinemodnum3, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 0, 0));

  // read
  OfflineReadFileButton3 = new TGTextButton( parFrame, "&Read", OFFLINEREAD3);
  OfflineReadFileButton3->Associate(this);
  parFrame->AddFrame(OfflineReadFileButton3, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 0, 0));

  // status
  OfflineFileStatus3 = new TGTextEntry(parFrame,new TGTextBuffer(30), 10000);
  OfflineFileStatus3-> SetFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1", false);
  fClient->GetColorByName("blue", color);
  OfflineFileStatus3->SetTextColor(color, false);
  OfflineFileStatus3->SetText("NOT READ");
  OfflineFileStatus3->Resize(100, 12);
  OfflineFileStatus3->SetEnabled(kFALSE);
  OfflineFileStatus3->SetFrameDrawn(kFALSE);
  parFrame->AddFrame(OfflineFileStatus3, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 6, 0));// 
  // draw
  OfflineDrawButton3 = new TGTextButton( parFrame, "&Draw", OFFLINEDRAW3);
  OfflineDrawButton3->SetEnabled(0);
  OfflineDrawButton3->Associate(this);
  parFrame->AddFrame(OfflineDrawButton3, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 0, 0));


  TabPanel->AddFrame(parFrame, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 2, 2, 1, 1));

  
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
	      std::cout<<"Slider Pos -  min:"<<dslider->GetMinPosition()<<" max:"<<dslider->GetMaxPosition()<<std::endl;

	      // adjustCanvas->Modified();
	      // adjustCanvas->Update();
	      // adCanvas->GetCanvas()->Modified();
	      // adCanvas->GetCanvas()->Update();
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
	      Panel1ReadFile();
	      break;
	    case OFFLINEREAD2:
	      Panel2ReadFile();
	      break;
	    case OFFLINEREAD3:
	      Panel3ReadFile();
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
		      if(retval < 0) ErrorInfo("Offline.cpp", "ProcessMessage(...)", "Pixie16WriteSglModPar/SLOW_FILTER_RANGE", retval);
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
		      if(retval < 0) ErrorInfo("Offline.cpp", "ProcessMessage(...)", "Pixie16WriteSglModPar/SLOW_FILTER_RANGE", retval);
		      OfflineDrawButton->SetEnabled(1);
	      	    }
	      	}
	      break;  
	    case OFFLINERUNNUM:
	      OfflineDrawButton->SetEnabled(0);
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
	    case OFFLINERUNNUM2:
	      OfflineDrawButton2->SetEnabled(0);
	      if(parm2 == 0)
		{
		  ++fileRunNum2;
		  offlinefilerunnum2->SetIntNumber(fileRunNum2);
		}
	      else
		{
		  if(fileRunNum2 != 0)
		    {
		      if(--fileRunNum2 == 0)
			fileRunNum2 = 0;
		      offlinefilerunnum2->SetIntNumber(fileRunNum2);
		    }
		}
	      break;
	    case OFFLINERUNNUM3:
	      OfflineDrawButton3->SetEnabled(0);
	      if(parm2 == 0)
		{
		  ++fileRunNum3;
		  offlinefilerunnum3->SetIntNumber(fileRunNum3);
		}
	      else
		{
		  if(fileRunNum3 != 0)
		    {
		      if(--fileRunNum3 == 0)
			fileRunNum3 = 0;
		      offlinefilerunnum3->SetIntNumber(fileRunNum3);
		    }
		}
	      break;
	    case OFFLINEMODNUM:
	      OfflineDrawButton->SetEnabled(0);
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
	    case OFFLINEMODNUM2:
	      OfflineDrawButton2->SetEnabled(0);
	      if (parm2 == 0)
		{
		  if (modNumber2 != detector->NumModules-1)
		    {
		      ++modNumber2;
		      offlinemodnum2->SetIntNumber(modNumber2);
		    }
		}
	      else
		{
		  if (modNumber2 != 0)
		    {
		      if (--modNumber2 == 0)
			modNumber2 = 0;
		      offlinemodnum2->SetIntNumber(modNumber2);
		    }
		}
	      break;
	    case OFFLINEMODNUM3:
	      OfflineDrawButton3->SetEnabled(0);
	      if (parm2 == 0)
		{
		  if (modNumber3 != detector->NumModules-1)
		    {
		      ++modNumber3;
		      offlinemodnum3->SetIntNumber(modNumber3);
		    }
		}
	      else
		{
		  if (modNumber3 != 0)
		    {
		      if (--modNumber3 == 0)
			modNumber3 = 0;
		      offlinemodnum3->SetIntNumber(modNumber3);
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
	      modNumber = offlinemodnum->GetIntNumber();
	      if(modNumber > detector->NumModules-1) modNumber = detector->NumModules-1;
	      if(modNumber < 0) modNumber = 0;
	      offlinemodnum->SetIntNumber(modNumber);
	      break;
	    default:
	      break;
	    }
	  break;
	case OFFLINEMODNUM2:
	  switch (GET_SUBMSG(msg))
	    {
	    case kTE_ENTER:
	      OfflineDrawButton2->SetEnabled(0);
	      modNumber2 = offlinemodnum2->GetIntNumber();
	      if(modNumber2 > detector->NumModules-1) modNumber2 = detector->NumModules-1;
	      if(modNumber2 < 0) modNumber2 = 0;
	      offlinemodnum2->SetIntNumber(modNumber2);
	      break;
	    default:
	      break;
	    }
	  break;
	case OFFLINEMODNUM3:
	  switch (GET_SUBMSG(msg))
	    {
	    case kTE_ENTER:
	      OfflineDrawButton3->SetEnabled(0);
	      modNumber3 = offlinemodnum3->GetIntNumber();
	      if(modNumber3 > detector->NumModules-1) modNumber3 = detector->NumModules-1;
	      if(modNumber3 < 0) modNumber3 = 0;
	      offlinemodnum3->SetIntNumber(modNumber3);
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
	      if(retval < 0) ErrorInfo("Offline.cpp", "ProcessMessage(...)", "Pixie16WriteSglModPar/SLOW_FILTER_RANGE", retval);
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
	      fileRunNum = offlinefilerunnum->GetIntNumber();
	      if(fileRunNum < 0) fileRunNum = 0;
	      offlinefilerunnum->SetIntNumber(fileRunNum);
	      break;
	      
	    default:
	      break;
	    }
	  break;
	case OFFLINERUNNUM2:
	  switch (GET_SUBMSG(msg))
	    {
	    case kTE_ENTER:
	      OfflineDrawButton2->SetEnabled(0);
	      fileRunNum2 = offlinefilerunnum2->GetIntNumber();
	      if(fileRunNum2 < 0) fileRunNum2 = 0;
	      offlinefilerunnum2->SetIntNumber(fileRunNum2);
	      break;
	    default:
	      break;
	    }
	  break;
	case OFFLINERUNNUM3:
	  switch (GET_SUBMSG(msg))
	    {
	    case kTE_ENTER:
	      OfflineDrawButton3->SetEnabled(0);
	      fileRunNum3 = offlinefilerunnum3->GetIntNumber();
	      if(fileRunNum3 < 0) fileRunNum3 = 0;
	      offlinefilerunnum3->SetIntNumber(fileRunNum3);
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
  flength = offlinefilters[0]->GetNumber();
  retval = Pixie16WriteSglChanPar((char*)"TRIGGER_RISETIME", flength, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cpp", "OfflineChangeValues(...)", "Pixie16WriteSglChanPar/TRIGGER_RISETIME", retval);
  fgap = offlinefilters[1]->GetNumber();
  retval = Pixie16WriteSglChanPar((char*)"TRIGGER_FLATTOP", fgap, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cpp", "OfflineChangeValues(...)", "Pixie16WriteSglChanPar/TRIGGER_FLATTOP", retval);
  slength = offlinefilters[2]->GetNumber ();
  retval = Pixie16WriteSglChanPar((char*)"ENERGY_RISETIME", slength, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cpp", "OfflineChangeValues(...)", "Pixie16WriteSglChanPar/ENERGY_RISETIME", retval);
  sgap = offlinefilters[3]->GetNumber ();
  retval = Pixie16WriteSglChanPar((char*)"ENERGY_FLATTOP", sgap, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cpp", "OfflineChangeValues(...)", "Pixie16WriteSglChanPar/ENERGY_FLATTOP", retval);
  tau = offlinefilters[4]->GetNumber ();
  retval = Pixie16WriteSglChanPar((char*)"TAU", tau, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cpp", "OfflineChangeValues(...)", "Pixie16WriteSglChanPar/TAU", retval);
  delay = offlinefilters[5]->GetNumber();
  retval = Pixie16WriteSglChanPar((char*)"CFDDelay", delay, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cpp", "OfflineChangeValues(...)", "Pixie16WriteSglChanPar/CFDDelay", retval);
  frac = offlinefilters[6]->GetNumber();
  retval = Pixie16WriteSglChanPar((char*)"CFDScale", frac, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cpp", "OfflineChangeValues(...)", "Pixie16WriteSglChanPar/CFDScale", retval);
  thresh = offlinefilters[7]->GetNumber();
  retval = Pixie16WriteSglChanPar((char*)"TRIGGER_THRESHOLD", thresh, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cpp", "OfflineChangeValues(...)", "Pixie16WriteSglChanPar/TRIGGER_THRESHOLD", retval);
}

void Offline::OfflineLoadValues(int mod,int ch)
{
  double ChanParData = -1;
  int retval; 
  char text[20];
  
  retval = Pixie16ReadSglChanPar((char*)"TRIGGER_RISETIME", &ChanParData, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cpp", "OfflineLoadValues(...)", "Pixie16ReadSglChanPar/TRIGGER_RISETIME", retval);
  sprintf(text, "%1.2f", ChanParData);
  offlinefilters[0]->SetText(text);

  retval = Pixie16ReadSglChanPar((char*)"TRIGGER_FLATTOP", &ChanParData, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cpp", "OfflineLoadValues(...)", "Pixie16ReadSglChanPar/TRIGGER_FLATTOP", retval);  
  sprintf(text, "%1.2f", ChanParData);
  offlinefilters[1]->SetText(text);

  retval = Pixie16ReadSglChanPar((char*)"ENERGY_RISETIME", &ChanParData, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cpp", "OfflineLoadValues(...)", "Pixie16ReadSglChanPar/ENERGY_RISETIME", retval);
  sprintf(text, "%1.2f", ChanParData);
  offlinefilters[2]->SetText(text);

  retval = Pixie16ReadSglChanPar((char*)"ENERGY_FLATTOP", &ChanParData, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cpp", "OfflineLoadValues(...)", "Pixie16ReadSglChanPar/ENERGY_FLATTOP", retval);  
  sprintf(text, "%1.2f", ChanParData);
  offlinefilters[3]->SetText(text);

  retval = Pixie16ReadSglChanPar((char*)"TAU", &ChanParData, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cpp", "OfflineLoadValues(...)", "Pixie16ReadSglChanPar/TAU", retval);
  sprintf(text, "%1.2f", ChanParData);
  offlinefilters[4]->SetText(text);

  retval = Pixie16ReadSglChanPar((char*)"CFDDelay", &ChanParData, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cpp", "OfflineLoadValues(...)", "Pixie16ReadSglChanPar/CFDDelay", retval);
  sprintf(text, "%1.2f", ChanParData);
  offlinefilters[5]->SetText(text);

  retval = Pixie16ReadSglChanPar((char*)"CFDScale", &ChanParData, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cpp", "OfflineLoadValues(...)", "Pixie16ReadSglChanPar/CFDScale", retval); 
  sprintf(text, "%1.2f", ChanParData);
  offlinefilters[6]->SetText(text);

  retval = Pixie16ReadSglChanPar((char*)"TRIGGER_THRESHOLD", &ChanParData, mod, ch);
  if(retval < 0) ErrorInfo("Offline.cpp", "OfflineLoadValues(...)", "Pixie16ReadSglChanPar/TRIGGER_THRESHOLD", retval);
  sprintf (text, "%d", (int)ChanParData);
  offlinefilters[7]->SetText(text);

  
  retval = Pixie16ReadSglModPar((char*)"SLOW_FILTER_RANGE", &OfflinefRange, mod);
  if(retval < 0) ErrorInfo("Offline.cpp", "OfflineLoadValues(...)", "Pixie16ReadSglModPar/SLOW_FILTER_RANGE", retval);
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
  char stacurr[20];
  sprintf(stacurr,"%d",OfflineCurrentCount+1);
  OfflineCurrentCountText->SetText(stacurr);
	      
  RcdTrace = new unsigned short[OfflineEventInformation[12*OfflineCurrentCount+10]];
  doublethresh = new double[OfflineEventInformation[12*OfflineCurrentCount+10]];
  doublesample = new double[OfflineEventInformation[12*OfflineCurrentCount+10]];
  doublercdtrace = new double[OfflineEventInformation[12*OfflineCurrentCount+10]];
  doublefastfilter = new double[OfflineEventInformation[12*OfflineCurrentCount+10]];
  doublecfd = new double[OfflineEventInformation[12*OfflineCurrentCount+10]];
  doubleslowfilter = new double[OfflineEventInformation[12*OfflineCurrentCount+10]];

  int retval;
  retval = Pixie16ComputeFastFiltersOffline(offlinefilename, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short) offlinechnum->GetIntNumber(), OfflineEventInformation[12*OfflineCurrentCount+11], OfflineEventInformation[12*OfflineCurrentCount+10], RcdTrace, doublefastfilter, doublecfd );
  if(retval < 0) ErrorInfo("Offline.cpp", "Panel1Draw()", "Pixie16ComputeFastFiltersOffline", retval);
  retval = Pixie16ComputeSlowFiltersOffline(offlinefilename, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short) offlinechnum->GetIntNumber(), OfflineEventInformation[12*OfflineCurrentCount+11], OfflineEventInformation[12*OfflineCurrentCount+10], RcdTrace,doubleslowfilter );
  if(retval < 0) ErrorInfo("Offline.cpp", "Panel1Draw()", "Pixie16ComputeSlowFiltersOffline", retval);
  
  double ChanParData;
  retval = Pixie16ReadSglChanPar((char*)"TRIGGER_THRESHOLD", &ChanParData, (unsigned short)offlinemodnum->GetIntNumber(), (unsigned short) offlinechnum->GetIntNumber());
  if(retval < 0) ErrorInfo("Offline.cpp", "Panel1Draw()", "Pixie16ReadSglChanPar/TRIGGER_THRESHOLD", retval);
  
  for (unsigned int i = 0; i < OfflineEventInformation[12*OfflineCurrentCount+10]; ++i)
    {
      doublesample[i] = i;
      doublercdtrace[i] = (double)RcdTrace[i];
      doublethresh[i] = ChanParData;
    }

  rawdata = new TGraph(OfflineEventInformation[12*OfflineCurrentCount+10],doublesample,doublercdtrace);
  threshdata = new TGraph(OfflineEventInformation[12*OfflineCurrentCount+10],doublesample,doublethresh);
  cfddata = new TGraph(OfflineEventInformation[12*OfflineCurrentCount+10],doublesample,doublecfd);
  sfilterdata = new TGraph(OfflineEventInformation[12*OfflineCurrentCount+10],doublesample,doubleslowfilter);
  ffilterdata  = new TGraph(OfflineEventInformation[12*OfflineCurrentCount+10],doublesample,doublefastfilter);

  adjustCanvas->cd();
  adjustCanvas->Clear();
  cfddata->SetLineColor(2);
  sfilterdata->SetLineColor(3);
  ffilterdata->SetLineColor(4);
  threshdata->SetLineColor(5);
  // rawdata->SetLineWidth(3);
  // cfddata->SetLineWidth(3);
  // sfilterdata->SetLineWidth(3);
  // cfddata->SetLineWidth(3);
  offlinemultigraph->Clear();
  offlinemultigraph->Add(rawdata);
  offlinemultigraph->Add(threshdata);
  offlinemultigraph->Add(cfddata);
  offlinemultigraph->Add(sfilterdata);
  offlinemultigraph->Add(ffilterdata);
  offlinemultigraph->Draw("AL");
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
	  if(tempN12[i] > OfflineModuleEventsCount2-1)
	    {
	      offlinedatastatus2[i] = true;
	      break;
	    }
	  if(OfflineCurrentCount2[i] >= OfflineModuleEventsCount2) OfflineCurrentCount2[i] = 0;

	  if(i == OfflineEventInformation2[12*OfflineCurrentCount2[i]+1]) break;
	}

      // cout<<"Ch: "<<i<<"  N: "<<OfflineCurrentCount2[i]<<endl;
      int retval;
      if(!offlinedatastatus2[i])
	{
	  RcdTrace2[i] = new unsigned short[OfflineEventInformation2[12*OfflineCurrentCount2[i]+10]];
	  doublethresh2[i] = new double[OfflineEventInformation2[12*OfflineCurrentCount2[i]+10]];
	  doublesample2[i] = new double[OfflineEventInformation2[12*OfflineCurrentCount2[i]+10]];
	  doublercdtrace2[i] = new double[OfflineEventInformation2[12*OfflineCurrentCount2[i]+10]];
	  doublefastfilter2[i] = new double[OfflineEventInformation2[12*OfflineCurrentCount2[i]+10]];
	  doublecfd2[i] = new double[OfflineEventInformation2[12*OfflineCurrentCount2[i]+10]];
	  doubleslowfilter2[i] = new double[OfflineEventInformation2[12*OfflineCurrentCount2[i]+10]];

	  retval = Pixie16ComputeFastFiltersOffline(offlinefilename2, (unsigned short)offlinemodnum2->GetIntNumber(), (unsigned short) i, OfflineEventInformation2[12*OfflineCurrentCount2[i]+11], OfflineEventInformation2[12*OfflineCurrentCount2[i]+10], RcdTrace2[i], doublefastfilter2[i], doublecfd2[i] );
	  if(retval < 0) ErrorInfo("Offline.cpp", "Panel2Draw()", "Pixie16ComputeFastFiltersOffline", retval);
	  retval = Pixie16ComputeSlowFiltersOffline(offlinefilename2, (unsigned short)offlinemodnum2->GetIntNumber(), (unsigned short) i, OfflineEventInformation2[12*OfflineCurrentCount2[i]+11], OfflineEventInformation2[12*OfflineCurrentCount2[i]+10], RcdTrace2[i],doubleslowfilter2[i] );
	  if(retval < 0) ErrorInfo("Offline.cpp", "Panel2Draw()", "Pixie16ComputeSlowFiltersOffline", retval);
	  
	  double ChanParData;
	  retval = Pixie16ReadSglChanPar((char*)"TRIGGER_THRESHOLD", &ChanParData, (unsigned short)offlinemodnum2->GetIntNumber(), (unsigned short)i);
	  if(retval < 0) ErrorInfo("Offline.cpp", "Panel2Draw()", "Pixie16ReadSglChanPar/TRIGGER_THRESHOLD", retval);
	  for (unsigned int j = 0; j < OfflineEventInformation2[12*OfflineCurrentCount2[i]+10]; ++j)
	    {
	      doublesample2[i][j] = j;
	      doublercdtrace2[i][j] = (double)RcdTrace2[i][j];
	      doublethresh2[i][j] = ChanParData;
	    }

	  rawdata2[i] = new TGraph(OfflineEventInformation2[12*OfflineCurrentCount2[i]+10],doublesample2[i],doublercdtrace2[i]);
	  threshdata2[i] = new TGraph(OfflineEventInformation2[12*OfflineCurrentCount2[i]+10],doublesample2[i],doublethresh2[i]);
	  cfddata2[i] = new TGraph(OfflineEventInformation2[12*OfflineCurrentCount2[i]+10],doublesample2[i],doublecfd2[i]);
	  sfilterdata2[i] = new TGraph(OfflineEventInformation2[12*OfflineCurrentCount2[i]+10],doublesample2[i],doubleslowfilter2[i]);
	  ffilterdata2[i] = new TGraph(OfflineEventInformation2[12*OfflineCurrentCount2[i]+10],doublesample2[i],doublefastfilter2[i]);

	  cfddata2[i]->SetLineColor(2);
	  sfilterdata2[i]->SetLineColor(3);
	  ffilterdata2[i]->SetLineColor(4);
	  threshdata2[i]->SetLineColor(5);
	  offlinemultigraph2[i]->Clear();
	  offlinemultigraph2[i]->Add(rawdata2[i]);
	  offlinemultigraph2[i]->Add(threshdata2[i]);
	  offlinemultigraph2[i]->Add(cfddata2[i]);
	  offlinemultigraph2[i]->Add(sfilterdata2[i]);
	  offlinemultigraph2[i]->Add(ffilterdata2[i]);
	  canvas2->cd(1+i);
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
      offlineth1d3[i] = new TH1D(th1dname,"",16384,0,16384);
    }
  
  
  for (unsigned int i = 0; i < OfflineModuleEventsCount3; ++i)
    {
      offlineth1d3[OfflineEventInformation3[12*i+1]]->Fill(OfflineEventInformation3[12*i+9]);
    }

  for (int i = 0; i < 16; ++i)
    {
      if(offlineth1d3[i]->GetEntries() > 0)
	{
	  canvas3->cd(i+1);
	  offlineth1d3[i]->Draw();
	}
    }
    
  canvas3->Modified();
  canvas3->Update();
  gSystem->ProcessEvents();
}

void Offline::Panel1ReadFile()
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
      sprintf(staok,"N %d",OfflineModuleEventsCount);
      OfflineFileStatus->SetText(staok);
	      
      OfflineDrawButton->SetEnabled(1);
    }
  else
    {
      fClient->GetColorByName("red", color);
      OfflineFileStatus->SetTextColor(color, false);
      OfflineFileStatus->SetText("READ ERROR");
    }
}

void Offline::Panel2ReadFile()
{
  for (int i = 0; i < 16; ++i)
    {
      OfflineCurrentCount2[i] = -1;
    }
  fClient->GetColorByName("green", color);
  OfflineFileStatus2->SetTextColor(color, false);
  OfflineFileStatus2->SetText("Waitting...");
  gSystem->ProcessEvents();

  sprintf(offlinefilename2,"%s%s_R%04d_M%02d.bin",filepathtext->GetText(),filenametext->GetText(),int(offlinefilerunnum2->GetIntNumber()),int(offlinemodnum2->GetIntNumber()));
  // GOTO need check file 
  OfflineModuleEventsCount2 = GetModuleEvents(offlinefilename2);

  if(OfflineModuleEventsCount2 > 0)
    {

      cout<<"!!!!!: "<<OfflineModuleEventsCount2<<endl;

      if(OfflineEventInformation2 != NULL)
	{
	  delete []OfflineEventInformation2;
	  OfflineEventInformation2 = NULL;
	}
      OfflineEventInformation2 = new unsigned int[12*OfflineModuleEventsCount2];
      GetEventsInfo(offlinefilename2,OfflineEventInformation2);
	      
      //
      fClient->GetColorByName("red", color);
      OfflineFileStatus2->SetTextColor(color, false);
      char staok[20];
      sprintf(staok,"N %d",OfflineModuleEventsCount2);
      OfflineFileStatus2->SetText(staok);
	      
      OfflineDrawButton2->SetEnabled(1);
    }
  else
    {
      fClient->GetColorByName("red", color);
      OfflineFileStatus2->SetTextColor(color, false);
      OfflineFileStatus2->SetText("READ ERROR");
    }
}

void Offline::Panel3ReadFile()
{
  fClient->GetColorByName("green", color);
  OfflineFileStatus3->SetTextColor(color, false);
  OfflineFileStatus3->SetText("Waitting...");
  gSystem->ProcessEvents();

  sprintf(offlinefilename3,"%s%s_R%04d_M%02d.bin",filepathtext->GetText(),filenametext->GetText(),int(offlinefilerunnum3->GetIntNumber()),int(offlinemodnum3->GetIntNumber()));
  // GOTO need check file 
  OfflineModuleEventsCount3 = GetModuleEvents(offlinefilename3);

  if(OfflineModuleEventsCount3 > 0)
    {
      cout<<"!!!!!!!!: "<<OfflineModuleEventsCount3<<endl;

      if(OfflineEventInformation3 != NULL)
	{
	  delete []OfflineEventInformation3;
	  OfflineEventInformation3 = NULL;
	}
      OfflineEventInformation3 = new unsigned int[12*OfflineModuleEventsCount3];
      GetEventsInfo(offlinefilename3,OfflineEventInformation3);
	      
      //
      fClient->GetColorByName("red", color);
      OfflineFileStatus3->SetTextColor(color, false);
      char staok[20];
      sprintf(staok,"N %d",OfflineModuleEventsCount3);
      OfflineFileStatus3->SetText(staok);
	      
      OfflineDrawButton3->SetEnabled(1);

    }
  else
    {
      fClient->GetColorByName("red", color);
      OfflineFileStatus3->SetTextColor(color, false);
      OfflineFileStatus3->SetText("READ ERROR");
    }
  
}



// 
// Offline.cpp ends here
