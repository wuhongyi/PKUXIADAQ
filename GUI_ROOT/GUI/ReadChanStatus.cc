// ReadChanStatus.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 3月  8 14:57:31 2018 (+0800)
// Last-Updated: 六 11月 23 13:00:44 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 40
// URL: http://wuhongyi.cn 

#include "ReadChanStatus.hh"

#include "Detector.hh"
#include "Global.hh"

#include "pixie16app_export.h"
#include "pixie16sys_export.h"
#include "xia_common.h"

#include "TGTab.h"
#include "TString.h"
#include "TFitResultPtr.h"
#include "TColor.h"
#include <cstring>
#include <iostream>
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ReadChanStatus::ReadChanStatus(const TGWindow *p, const TGWindow *main,Detector *det)
{
  SetCleanup(kDeepCleanup);
  detector = det;

  modNumber0 = 0;
  chanNumber0 = 0;
  modNumber1 = 0;
  chanNumber1 = 0;
  
  for (int i = 0; i < 16; ++i)
    {
      traceth1s0[i] = NULL;
      baselineth1i1[i] = NULL;
      baselinetgraph1[i] = NULL;
    }

  SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGTab *TabPanel = new TGTab(this);
  this->AddFrame(TabPanel, new TGLayoutHints(kLHintsBottom | kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
  TGCompositeFrame *Tab0 = TabPanel->AddTab("ADC Trace");
  TabPanel->GetTabTab("ADC Trace")->ChangeBackground(TColor::RGB2Pixel(TAB_BG_R,TAB_BG_G,TAB_BG_B));
  MakeFold0Panel(Tab0);
  Tab0->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  TGCompositeFrame *Tab1 = TabPanel->AddTab("Baseline");
  TabPanel->GetTabTab("Baseline")->ChangeBackground(TColor::RGB2Pixel(TAB_BG_R,TAB_BG_G,TAB_BG_B));
  MakeFold1Panel(Tab1);
  Tab1->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  SetWindowName("Trace & Baseline");
  MapSubwindows();
  MapWindow();
  Resize();

  gStyle->SetOptStat(0);
}


ReadChanStatus::~ReadChanStatus()
{
  for (int i = 0; i < 16; ++i)
    {
      if(traceth1s0[i] != NULL) delete traceth1s0[i];
      if(baselineth1i1[i] != NULL) delete baselineth1i1[i];
      if(baselinetgraph1[i] != NULL) delete baselinetgraph1[i];
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Bool_t ReadChanStatus::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
  int test1 = 0;
  int test2 = 0;
  switch (test1 = GET_MSG(msg))
    {
    case kC_COMMAND:
      switch (test2 = GET_SUBMSG(msg))
	{
	case kCM_BUTTON:
	  switch (parm1)
	    {
	    case READCHANSTATUS_DRAW0:
	      Panel0Draw();
	      break;
	    case READCHANSTATUS_DRAW1:
	      Panel1Draw();
	      break;
	      
	    case READCHANSTATUS_MODNUM0:
	      if (parm2 == 0)
		{
		  if (modNumber0 != detector->NumModules-1)
		    {
		      ++modNumber0;
		      modnum0->SetIntNumber(modNumber0);
		    }
		}
	      else
		{
		  if (modNumber0 != 0)
		    {
		      if (--modNumber0 == 0)
			modNumber0 = 0;
		      modnum0->SetIntNumber(modNumber0);
		    }
		}
	      break;

	    case READCHANSTATUS_MODNUM1:
	      if (parm2 == 0)
		{
		  if (modNumber1 != detector->NumModules-1)
		    {
		      ++modNumber1;
		      modnum1->SetIntNumber(modNumber1);
		    }
		}
	      else
		{
		  if (modNumber1 != 0)
		    {
		      if (--modNumber1 == 0)
			modNumber1 = 0;
		      modnum1->SetIntNumber(modNumber1);
		    }
		}
	      break;
	      
	    case READCHANSTATUS_CHNUM0:
	      if (parm2 == 0)
		{
		  if (chanNumber0 != 15)
		    {
		      ++chanNumber0;
		      chnum0->SetIntNumber(chanNumber0);
		    }
		}
	      else
		{
		  if (chanNumber0 != 0)
		    {
		      if (--chanNumber0 == 0)
			chanNumber0 = 0;
		      chnum0->SetIntNumber(chanNumber0);
		    }
		}
	      break;

	    case READCHANSTATUS_CHNUM1:
	      if (parm2 == 0)
		{
		  if (chanNumber1 != 15)
		    {
		      ++chanNumber1;
		      chnum1->SetIntNumber(chanNumber1);
		    }
		}
	      else
		{
		  if (chanNumber1 != 0)
		    {
		      if (--chanNumber1 == 0)
			chanNumber1 = 0;
		      chnum1->SetIntNumber(chanNumber1);
		    }
		}
	      break;
	      

	    default:
	      break;
	    }
	  break;
	}

    case kC_TEXTENTRY:
      switch (parm1)
	{
	case READCHANSTATUS_MODNUM0:
	  switch (GET_SUBMSG(msg))
	    {
	    case kTE_ENTER:
	      modNumber0 = modnum0->GetIntNumber();
	      if(modNumber0 > detector->NumModules-1) modNumber0 = detector->NumModules-1;
	      if(modNumber0 < 0) modNumber0 = 0;
	      modnum0->SetIntNumber(modNumber0);
	      break;
	      
	    default:
	      break;
	    }
	  break;

	case READCHANSTATUS_MODNUM1:
	  switch (GET_SUBMSG(msg))
	    {
	    case kTE_ENTER:
	      modNumber1 = modnum1->GetIntNumber();
	      if(modNumber1 > detector->NumModules-1) modNumber1 = detector->NumModules-1;
	      if(modNumber1 < 0) modNumber1 = 0;
	      modnum1->SetIntNumber(modNumber1);
	      break;
	      
	    default:
	      break;
	    }
	  break;	  
	  	  
	case READCHANSTATUS_CHNUM0:
	  switch (GET_SUBMSG(msg))
	    {
	    case kTE_ENTER:
	      chanNumber0 = chnum0->GetIntNumber();
	      if(chanNumber0 > 15) chanNumber0 = 15;
	      if(chanNumber0 < 0) chanNumber0 = 0;
	      chnum0->SetIntNumber(chanNumber0);
	      break;
	      
	    default:
	      break;
	    }
	  break;

	case READCHANSTATUS_CHNUM1:
	  switch (GET_SUBMSG(msg))
	    {
	    case kTE_ENTER:
	      chanNumber1 = chnum1->GetIntNumber();
	      if(chanNumber1 > 15) chanNumber1 = 15;
	      if(chanNumber1 < 0) chanNumber1 = 0;
	      chnum1->SetIntNumber(chanNumber1);
	      break;
	      
	    default:
	      break;
	    }
	  break;
	  
	}
      break;

      
    default:
      break;
    }

  return kTRUE;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ReadChanStatus::MakeFold0Panel(TGCompositeFrame *TabPanel)
{
  TGCompositeFrame *parFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);
  TabPanel->AddFrame(parFrame, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 4, 4, 4, 4));
  parFrame->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  // draw
  DrawButton0 = new TGTextButton(parFrame, "&Draw", READCHANSTATUS_DRAW0);
  parFrame->AddFrame(DrawButton0, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 30, 0, 0));
  DrawButton0->Associate(this);
  DrawButton0->ChangeOptions(DrawButton0->GetOptions() ^ kRaisedFrame);
  DrawButton0->SetFont(TEXTBUTTONSMALL_FONT, false);
  DrawButton0->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  DrawButton0->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));  
  
  // ch
  chnum0 = new TGNumberEntry(parFrame, 0, 2, READCHANSTATUS_CHNUM0, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, 15);
  parFrame->AddFrame(chnum0, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 10, 0, 0));
  chnum0->SetButtonToNum(0);
  chnum0->Associate(this);
  chnum0->GetNumberEntry()->ChangeOptions(chnum0->GetNumberEntry()->GetOptions() ^ kRaisedFrame);
  chnum0->GetNumberEntry()->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
  chnum0->GetButtonUp()->ChangeOptions(chnum0->GetButtonUp()->GetOptions() ^ kRaisedFrame);
  chnum0->GetButtonDown()->ChangeOptions(chnum0->GetButtonDown()->GetOptions() ^ kRaisedFrame);
  chnum0->ChangeSubframesBackground(TColor::RGB2Pixel(TEXTENTRY_BG_R,TEXTENTRY_BG_G,TEXTENTRY_BG_B));

  TGLabel *ch = new TGLabel(parFrame, "Ch:"); 
  parFrame->AddFrame(ch, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 2, 3, 0));
  ch->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  ch->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B)); 

  modnum0 = new TGNumberEntry(parFrame, 0, 2, READCHANSTATUS_MODNUM0, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, PRESET_MAX_MODULES-1);
  parFrame->AddFrame(modnum0, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 10, 0, 0));
  modnum0->SetButtonToNum(0);
  modnum0->Associate(this);
  modnum0->GetNumberEntry()->ChangeOptions(modnum0->GetNumberEntry()->GetOptions() ^ kRaisedFrame);
  modnum0->GetNumberEntry()->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
  modnum0->GetButtonUp()->ChangeOptions(modnum0->GetButtonUp()->GetOptions() ^ kRaisedFrame);
  modnum0->GetButtonDown()->ChangeOptions(modnum0->GetButtonDown()->GetOptions() ^ kRaisedFrame);
  modnum0->ChangeSubframesBackground(TColor::RGB2Pixel(TEXTENTRY_BG_R,TEXTENTRY_BG_G,TEXTENTRY_BG_B));

  TGLabel *mod = new TGLabel(parFrame, "Mod:"); 
  parFrame->AddFrame(mod, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 2, 3, 0));
  mod->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  mod->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B)); 

  choosecanvasmode0 = new TGComboBox(parFrame);
  parFrame->AddFrame(choosecanvasmode0, new TGLayoutHints(kLHintsRight, 0, 10, 0, 0));
  choosecanvasmode0->Resize(150, 20);
  choosecanvasmode0->AddEntry("Single Channel Mode", 0);
  choosecanvasmode0->AddEntry("Multi Channel Mode", 1);
  choosecanvasmode0->Select(0);
  
  TGLabel *choosemode = new TGLabel(parFrame, "Mode:"); 
  parFrame->AddFrame(choosemode, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 2, 3, 0));
  choosemode->SetTextColor(TColor::RGB2Pixel(COLOR_DODERBLUE_R,COLOR_DODERBLUE_G,COLOR_DODERBLUE_B), false);
  choosemode->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGCompositeFrame *adCanvasFrame = new TGCompositeFrame(TabPanel, 800, 800, kHorizontalFrame);
  TabPanel->AddFrame(adCanvasFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 4, 4, 0, 4));

  TRootEmbeddedCanvas *adjCanvas = new TRootEmbeddedCanvas("canvas0", adCanvasFrame, 100, 100);
  adCanvasFrame->AddFrame(adjCanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
  canvas0 = adjCanvas->GetCanvas();
}

void ReadChanStatus::MakeFold1Panel(TGCompositeFrame *TabPanel)
{
  TGCompositeFrame *parFrame = new TGCompositeFrame(TabPanel, 0, 0, kHorizontalFrame);
  TabPanel->AddFrame(parFrame, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 4, 4, 4, 4));
  parFrame->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  // draw
  DrawButton1 = new TGTextButton(parFrame, "&Draw", READCHANSTATUS_DRAW1);
  DrawButton1->Associate(this);
  parFrame->AddFrame(DrawButton1, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 30, 0, 0));

  DrawButton1->ChangeOptions(DrawButton1->GetOptions() ^ kRaisedFrame);
  DrawButton1->SetFont(TEXTBUTTONSMALL_FONT, false);
  DrawButton1->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  DrawButton1->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));

  // ch
  chnum1 = new TGNumberEntry (parFrame, 0, 2, READCHANSTATUS_CHNUM1, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, 15);
  parFrame->AddFrame(chnum1, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 10, 0, 0));
  chnum1->SetButtonToNum(0);
  chnum1->Associate(this);
  chnum1->GetNumberEntry()->ChangeOptions(chnum1->GetNumberEntry()->GetOptions() ^ kRaisedFrame);
  chnum1->GetNumberEntry()->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
  chnum1->GetButtonUp()->ChangeOptions(chnum1->GetButtonUp()->GetOptions() ^ kRaisedFrame);
  chnum1->GetButtonDown()->ChangeOptions(chnum1->GetButtonDown()->GetOptions() ^ kRaisedFrame);
  chnum1->ChangeSubframesBackground(TColor::RGB2Pixel(TEXTENTRY_BG_R,TEXTENTRY_BG_G,TEXTENTRY_BG_B));

  TGLabel *ch = new TGLabel(parFrame, "Ch:"); 
  parFrame->AddFrame(ch, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 2, 3, 0));
  ch->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  ch->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B));
  
  modnum1 = new TGNumberEntry (parFrame, 0, 2, READCHANSTATUS_MODNUM1, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, PRESET_MAX_MODULES-1);
  parFrame->AddFrame(modnum1, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 10, 0, 0));
  modnum1->SetButtonToNum(0);
  modnum1->Associate(this);
  modnum1->GetNumberEntry()->ChangeOptions(modnum1->GetNumberEntry()->GetOptions() ^ kRaisedFrame);
  modnum1->GetNumberEntry()->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
  modnum1->GetButtonUp()->ChangeOptions(modnum1->GetButtonUp()->GetOptions() ^ kRaisedFrame);
  modnum1->GetButtonDown()->ChangeOptions(modnum1->GetButtonDown()->GetOptions() ^ kRaisedFrame);
  modnum1->ChangeSubframesBackground(TColor::RGB2Pixel(TEXTENTRY_BG_R,TEXTENTRY_BG_G,TEXTENTRY_BG_B));

  TGLabel *mod = new TGLabel(parFrame, "Mod:"); 
  parFrame->AddFrame(mod, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 2, 3, 0));
  mod->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  mod->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B));

  choosecanvasmode1 = new TGComboBox(parFrame);
  parFrame->AddFrame(choosecanvasmode1, new TGLayoutHints(kLHintsRight, 0, 10, 0, 0));
  choosecanvasmode1->Resize(140, 20);
  choosecanvasmode1->AddEntry("Single Channel Mode", 0);
  choosecanvasmode1->AddEntry("Multi Channel Mode", 1);
  choosecanvasmode1->Select(0);
  
  TGLabel *choosemode = new TGLabel(parFrame, "Mode:"); 
  parFrame->AddFrame(choosemode, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 2, 3, 0));
  choosemode->SetTextColor(TColor::RGB2Pixel(COLOR_DODERBLUE_R,COLOR_DODERBLUE_G,COLOR_DODERBLUE_B), false);
  choosemode->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  choosebaselinedrawmode1 = new TGComboBox(parFrame);
  parFrame->AddFrame(choosebaselinedrawmode1, new TGLayoutHints(kLHintsRight, 0, 15, 0, 0));
  choosebaselinedrawmode1->Resize(140, 20);
  choosebaselinedrawmode1->AddEntry("baseline(hist)", 0);
  choosebaselinedrawmode1->AddEntry("baseline-timestamp", 1);
  choosebaselinedrawmode1->Select(0);

  TGLabel *choosetype = new TGLabel(parFrame, "Type:"); 
  parFrame->AddFrame(choosetype, new TGLayoutHints(kLHintsRight | kLHintsTop, 1, 2, 3, 0));
  choosetype->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
  choosetype->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGCompositeFrame *adCanvasFrame = new TGCompositeFrame(TabPanel, 800, 800, kHorizontalFrame);
  TGLayoutHints *Hint = new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 1, 1, 1, 1);

  TRootEmbeddedCanvas *adjCanvas = new TRootEmbeddedCanvas("canvas1", adCanvasFrame, 100, 100);

  canvas1 = adjCanvas->GetCanvas();
  adCanvasFrame->AddFrame(adjCanvas, Hint);
  TabPanel->AddFrame(adCanvasFrame, Hint);
}

void ReadChanStatus::Panel0Draw()
{
  if(detector->GetRunFlag()) return;
  
  for (int i = 0; i < 16; ++i)
    {
      if(traceth1s0[i] == NULL)
	{
	  traceth1s0[i] = new TH1I(TString::Format("trace_ch%02d",i).Data(),"",8192,0,8192);
	  traceth1s0[i]->SetTitle(TString::Format("Ch: %d",i).Data());
	  traceth1s0[i]->GetXaxis()->SetTitle("sample");
	  traceth1s0[i]->GetXaxis()->SetLabelSize(0.06);
	  traceth1s0[i]->GetYaxis()->SetLabelSize(0.06);
	}
      else
	{
	  traceth1s0[i]->Reset("ICES");
	}
    }
  
  
  canvas0->cd();
  canvas0->Clear();

  int retval;
  retval = Pixie16AcquireADCTrace(modnum0->GetIntNumber());
  if (retval < 0)
    {
      ErrorInfo("ReadChanStatus.cc", "Panel0Draw()", "Pixie16AcquireADCTrace", retval);
    }
  

  switch(choosecanvasmode0->GetSelected())
    {
    case 0://single
      retval = Pixie16ReadSglChanADCTrace(trace[chnum0->GetIntNumber()],READTRACELENGTH,modnum0->GetIntNumber(),chnum0->GetIntNumber());
      if (retval < 0)
	{
	  ErrorInfo("ReadChanStatus.cc", "Panel0Draw()", "Pixie16ReadSglChanADCTrace", retval);
	}
      
      for (int j = 0; j < READTRACELENGTH; ++j)
  	{
  	  traceth1s0[chnum0->GetIntNumber()]->SetBinContent(j+1, trace[chnum0->GetIntNumber()][j]);
  	}
      traceth1s0[chnum0->GetIntNumber()]->Draw();
      break;

    case 1://multi
      canvas0->Divide(4,4);

      for (int i = 0; i < 16; ++i)
  	{
  	  canvas0->cd(i+1);

	  retval = Pixie16ReadSglChanADCTrace(trace[i],READTRACELENGTH,modnum0->GetIntNumber(),(unsigned short)i);
	  if (retval < 0)
	    {
	      ErrorInfo("ReadChanStatus.cc", "Panel0Draw()", "Pixie16ReadSglChanADCTrace", retval);
	    }
	  
  	  for (int j = 0; j < READTRACELENGTH; ++j)
  	    {
  	      traceth1s0[i]->SetBinContent(j+1, trace[i][j]);
  	    }
  	  traceth1s0[i]->Draw();
  	}
      break;


    default:
      break;
    }

  canvas0->Modified();
  canvas0->Update();
  gSystem->ProcessEvents();
}

void ReadChanStatus::Panel1Draw()
{
  if(detector->GetRunFlag()) return;
  
  for (int i = 0; i < 16; ++i)
    {
      if(choosebaselinedrawmode1->GetSelected() == 0)
	{
	  if(baselineth1i1[i] == NULL)
	    {
	      baselineth1i1[i] = new TH1I(TString::Format("baseline_ch%02d",i).Data(),"",65536,0,65536);
	      baselineth1i1[i]->SetTitle(TString::Format("Ch: %d",i).Data());
	      baselineth1i1[i]->GetXaxis()->SetTitle("bl");
	      baselineth1i1[i]->GetXaxis()->SetLabelSize(0.06);
	      baselineth1i1[i]->GetYaxis()->SetLabelSize(0.06);
	    }
	  else
	    {
	      baselineth1i1[i]->Reset("ICES");
	    }
	}
      else
	{
	  if(baselinetgraph1[i] == NULL)
	    {
	      baselinetgraph1[i] = new TGraph(READBASELINELEENGTH);
	      baselinetgraph1[i]->SetTitle(TString::Format("Ch: %d",i).Data());
	      baselinetgraph1[i]->GetXaxis()->SetTitle("ts");
	      baselinetgraph1[i]->GetYaxis()->SetTitle("bl");
	      // baselinetgraph1[i]->GetXaxis()->SetLabelSize(0.06);
	      // baselinetgraph1[i]->GetYaxis()->SetLabelSize(0.06);
	    }
	}
    }
  
  
  canvas1->cd();
  canvas1->Clear();

  int retval;
  retval = Pixie16AcquireBaselines(modnum1->GetIntNumber());
  if (retval < 0)
    {
      ErrorInfo("ReadChanStatus.cc", "Panel1Draw()", "Pixie16AcquireBaselines", retval);
    }


  switch(choosecanvasmode1->GetSelected())
    {
    case 0://single
      retval = Pixie16ReadSglChanBaselines(baselines[chnum1->GetIntNumber()],timestamps[chnum1->GetIntNumber()],READBASELINELEENGTH,modnum1->GetIntNumber(),chnum1->GetIntNumber());
      
      if (retval < 0)
	{
	  ErrorInfo("ReadChanStatus.cc", "Panel1Draw()", "Pixie16ReadSglChanBaselines", retval);
	}
      
      for (int j = 0; j < READBASELINELEENGTH; ++j)
  	{
	  if(choosebaselinedrawmode1->GetSelected() == 0)
	    baselineth1i1[chnum1->GetIntNumber()]->Fill(baselines[chnum1->GetIntNumber()][j]);
	  else
	    baselinetgraph1[chnum1->GetIntNumber()]->SetPoint(j,timestamps[chnum1->GetIntNumber()][j],baselines[chnum1->GetIntNumber()][j]);
  	}
      if(choosebaselinedrawmode1->GetSelected() == 0)
	baselineth1i1[chnum1->GetIntNumber()]->Draw();
      else
	baselinetgraph1[chnum1->GetIntNumber()]->Draw("AP*");
      break;

    case 1://multi
      canvas1->Divide(4,4);

      for (int i = 0; i < 16; ++i)
  	{
  	  canvas1->cd(i+1);

	  retval = Pixie16ReadSglChanBaselines(baselines[i], timestamps[i], READBASELINELEENGTH, modnum1->GetIntNumber(), (unsigned short)i);
	  if (retval < 0)
	    {
	      ErrorInfo("ReadChanStatus.cc", "Panel1Draw()", "Pixie16ReadSglChanBaselines", retval);
	    }
	  
  	  for (int j = 0; j < READBASELINELEENGTH; ++j)
  	    {
	      if(choosebaselinedrawmode1->GetSelected() == 0)
		baselineth1i1[i]->Fill(baselines[i][j]);
	      else
		baselinetgraph1[i]->SetPoint(j,timestamps[i][j],baselines[i][j]);
  	    }
	  if(choosebaselinedrawmode1->GetSelected() == 0)
	    baselineth1i1[i]->Draw();
	  else
	    baselinetgraph1[i]->Draw("AP*");
  	}
      break;


    default:
      break;
    }

  canvas1->Modified();
  canvas1->Update();
  gSystem->ProcessEvents();  
}
  
// 
// ReadChanStatus.cc ends here
