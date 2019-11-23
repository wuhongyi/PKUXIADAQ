// CopyPars.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 3月  8 14:19:00 2018 (+0800)
// Last-Updated: 六 11月 23 13:34:26 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 91
// URL: http://wuhongyi.cn 

#include "CopyPars.hh"

#include "Detector.hh"
#include "Global.hh"

#include "pixie16app_export.h"
#include "pixie16sys_export.h"
#include "xia_common.h"

#include "TGTab.h"
#include "TString.h"
#include "TFitResultPtr.h"
#include "TG3DLine.h"
#include "TColor.h"
#include <cstring>
#include <iostream>
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CopyPars::CopyPars(const TGWindow *p, const TGWindow *main,Detector *det)
  :TGTransientFrame(p, main, 10, 10, kHorizontalFrame)
{
  SetCleanup(kDeepCleanup);
  detector = det;

  modNumber0 = 0;
  chanNumber0 = 0;
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  TGVerticalFrame *mn_vert;
  mn_vert = new TGVerticalFrame(this, 200, 300);
  AddFrame(mn_vert, new TGLayoutHints(kLHintsTop | kLHintsLeft, 10, 10, 5, 5));
  mn_vert->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  TGCompositeFrame *parFrame0 = new TGCompositeFrame(mn_vert, 0, 0, kHorizontalFrame);
  mn_vert->AddFrame(parFrame0, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 2, 2, 1, 1));
  parFrame0->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  TGLabel *mod = new TGLabel(parFrame0, " Source Module:"); 
  parFrame0->AddFrame(mod, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 2, 3, 0));
  mod->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  mod->SetTextColor(TColor::RGB2Pixel(TABLE_LABEL_TEXT_R,TABLE_LABEL_TEXT_G,TABLE_LABEL_TEXT_B));
  
  modnum0 = new TGNumberEntry (parFrame0, 0, 2, COPYPARS_MODNUM0, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, PRESET_MAX_MODULES-1);
  parFrame0->AddFrame(modnum0, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 0, 0));
  modnum0->SetButtonToNum(0);
  modnum0->Associate(this);
  modnum0->GetNumberEntry()->ChangeOptions(modnum0->GetNumberEntry()->GetOptions() ^ kRaisedFrame);
  modnum0->GetNumberEntry()->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
  modnum0->GetButtonUp()->ChangeOptions(modnum0->GetButtonUp()->GetOptions() ^ kRaisedFrame);
  modnum0->GetButtonDown()->ChangeOptions(modnum0->GetButtonDown()->GetOptions() ^ kRaisedFrame);
  modnum0->ChangeSubframesBackground(TColor::RGB2Pixel(TEXTENTRY_BG_R,TEXTENTRY_BG_G,TEXTENTRY_BG_B));

  
  TGLabel *ch = new TGLabel(parFrame0, "source Channel:"); 
  parFrame0->AddFrame(ch, new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 2, 3, 0));
  ch->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  ch->SetTextColor(TColor::RGB2Pixel(TABLE_LABEL_TEXT_R,TABLE_LABEL_TEXT_G,TABLE_LABEL_TEXT_B));
  
  chnum0 = new TGNumberEntry (parFrame0, 0, 2, COPYPARS_CHNUM0, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3, 0, 15);
  parFrame0->AddFrame(chnum0, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 10, 0, 0));
  chnum0->SetButtonToNum(0);
  chnum0->Associate(this);
  chnum0->GetNumberEntry()->ChangeOptions(chnum0->GetNumberEntry()->GetOptions() ^ kRaisedFrame);
  chnum0->GetNumberEntry()->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
  chnum0->GetButtonUp()->ChangeOptions(chnum0->GetButtonUp()->GetOptions() ^ kRaisedFrame);
  chnum0->GetButtonDown()->ChangeOptions(chnum0->GetButtonDown()->GetOptions() ^ kRaisedFrame);
  chnum0->ChangeSubframesBackground(TColor::RGB2Pixel(TEXTENTRY_BG_R,TEXTENTRY_BG_G,TEXTENTRY_BG_B));



  TGHorizontal3DLine *ln1 = new TGHorizontal3DLine(mn_vert, 200, 2);
  mn_vert->AddFrame(ln1, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 10, 10));
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGCompositeFrame *parFrame1 = new TGCompositeFrame(mn_vert, 0, 0, kHorizontalFrame);
  mn_vert->AddFrame(parFrame1, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 2, 2, 1, 1));
  parFrame1->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  TGGroupFrame *copysetgroup = new TGGroupFrame(parFrame1,"Items to copy");
  parFrame1->AddFrame(copysetgroup,new TGLayoutHints(kLHintsLeft|kLHintsTop));
  copysetgroup->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  copysetgroup->SetTextColor(TColor::RGB2Pixel(TABLE_LABEL_TEXT_R,TABLE_LABEL_TEXT_G,TABLE_LABEL_TEXT_B));
  
  for (int i = 0; i < 13; ++i)
    {
      TString hotstring;
      switch(i)
	{
	case 0:
	  hotstring = "Filter(energy and trigger)";
	  break;
	case 1:
	  hotstring = "Analog signal conditioning\n\(polarity,dc-offset,gain/attenuation)";
	  break;
	case 2:
	  hotstring = "Histogram control\n\(minimum energy,binning factor)";
	  break;
	case 3:
	  hotstring = "Decay time";
	  break;
	case 4:
	  hotstring = "Pulse shape analysis\n\(trace length and delay)";
	  break;
	case 5:
	  hotstring = "Baseline control\n\(baseline cut,baseline percentage)";
	  break;
	case 6:
	  hotstring = "Module CSRB register\n\(module pull-ups,enable run inhibit, etc.)";
	  break;
	case 7:
	  hotstring = "Channel CSRA register\n\(good channel,trigger enable, etc.)";
	  break;
	case 8:
	  hotstring = "CFD trigger\n\(CFD delay,scaling factor)";
	  break;
	case 9:
	  hotstring = "Trigger stretch lengths\n\(veto,external trigger, ect.)";
	  break;
	case 10:
	  hotstring = "FIFO delays\n\(analog input delay,fast trigger output delay, etc.)";
	  break;
	case 11:
	  hotstring = "Multiplicity\n\(bit masks,thresholds, etc.)";
	  break;
	case 12:
	  hotstring = "QDC(QDC sum lengths)";
	  break;
	default:
	  break;
	}

      checkbitmask[i] = new TGCheckButton(copysetgroup,hotstring.Data());
      copysetgroup->AddFrame(checkbitmask[i],new TGLayoutHints(kLHintsExpandX|kLHintsTop,4,4,3,3));
      checkbitmask[i]->SetState(kButtonUp);
      checkbitmask[i]->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
      checkbitmask[i]->SetTextColor(TColor::RGB2Pixel(CHECKBUTTON_TEXT_R,CHECKBUTTON_TEXT_G,CHECKBUTTON_TEXT_B));
      checkbitmask[i]->SetFont(CHECKBUTTON_FONT, false);
    }


  TGGroupFrame *choosecopysetgroup = new TGGroupFrame(parFrame1,"Copy to:",kHorizontalFrame);
  parFrame1->AddFrame(choosecopysetgroup,new TGLayoutHints(kLHintsLeft|kLHintsTop));
  choosecopysetgroup->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  choosecopysetgroup->SetTextColor(TColor::RGB2Pixel(TABLE_LABEL_TEXT_R,TABLE_LABEL_TEXT_G,TABLE_LABEL_TEXT_B));
  
  columnlabel = new TGVerticalFrame(choosecopysetgroup, 200, 300);
  choosecopysetgroup->AddFrame(columnlabel, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  columnlabel->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  for (int i = 0; i < 13; ++i)
    {
      column[i] = new TGVerticalFrame(choosecopysetgroup, 200, 300);
      choosecopysetgroup->AddFrame(column[i], new TGLayoutHints(kLHintsTop | kLHintsExpandY, 0, 0, 0, 0));
      column[i]->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
    }
  


  TGTextEntry *te = new TGTextEntry(columnlabel, new TGTextBuffer(100), 10000,
				    te->GetDefaultGC ()(),
				    te->GetDefaultFontStruct (),
				    kRaisedFrame | kDoubleBorder,
				    GetWhitePixel ());
  columnlabel->AddFrame(te, new TGLayoutHints(kLHintsCenterX, 0, 0, 10, 0));
  te->SetText("Ch\\Mod");
  te->Resize(45, 20);
  te->SetAlignment(kTextCenterX);
  te->SetEnabled(kFALSE);
  te->SetFrameDrawn(kFALSE);
  te->ChangeBackground(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  te->SetTextColor(TColor::RGB2Pixel(LABEL_TEXT_R,LABEL_TEXT_G,LABEL_TEXT_B), false);

  
  TGTextEntry *Labels[17];
  for (int i = 0; i < 17; i++)
    {
      Labels[i] = new TGTextEntry(columnlabel, new TGTextBuffer(100), 10000,
				  Labels[i]->GetDefaultGC()(),
				  Labels[i]->GetDefaultFontStruct(),
				  kRaisedFrame | kDoubleBorder,
				  GetWhitePixel());
      columnlabel->AddFrame(Labels[i], new TGLayoutHints(kLHintsCenterX, 0, 3, 0, 0));
      Labels[i]->SetText(TString::Format("%02d",i).Data());
      Labels[i]->Resize(45, 20);
      Labels[i]->SetEnabled(kFALSE);
      Labels[i]->SetFrameDrawn(kFALSE);
      Labels[i]->SetAlignment(kTextCenterX);
      Labels[i]->SetFont(TABLE_LABEL_CH_FONT, false);
      Labels[i]->ChangeBackground(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
      Labels[i]->SetTextColor(TColor::RGB2Pixel(LABEL_TEXT_R,LABEL_TEXT_G,LABEL_TEXT_B), false);
    }
  Labels[16]->SetText("All");
  


  for (int i = 0; i < 13; ++i)
    {
      TGTextEntry *ra = new TGTextEntry(column[i], new TGTextBuffer(100),
					10000/*, ra->GetDefaultGC()(),
					ra->GetDefaultFontStruct(),
					kRaisedFrame | kDoubleBorder,
					GetWhitePixel()*/);
      column[i]->AddFrame(ra, new TGLayoutHints(kLHintsCenterX, 0, 0, 10, 3));
      ra->SetText(TString::Format("%02d",i).Data());
      ra->Resize(35, 20);
      ra->SetEnabled(kFALSE);
      ra->SetFrameDrawn(kFALSE);
      ra->SetAlignment(kTextCenterX);
      // ra->SetToolTipText("", 0);
      ra->ChangeBackground(TColor::RGB2Pixel(TABLE_LABELTITLE_BG_R,TABLE_LABELTITLE_BG_G,TABLE_LABELTITLE_BG_B));
      ra->SetTextColor(TColor::RGB2Pixel(TABLE_LABELTITLE_TEXT_R,TABLE_LABELTITLE_TEXT_G,TABLE_LABELTITLE_TEXT_B), false);

      
      for (int j = 0; j < 17; ++j)
	{
	  TGHorizontalFrame *rb = new TGHorizontalFrame(column[i], 0, 0);
	  column[i]->AddFrame(rb, new TGLayoutHints(kLHintsTop | kLHintsCenterX | kLHintsCenterY | kLHintsExpandY, 0, 0, 0, 0));
	  rb->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
	  rb->Resize(35, 20);

	  ckBtn[j][i] = new TGCheckButton(rb, "", 5000+100*i+j);
	  rb->AddFrame(ckBtn[j][i],new TGLayoutHints(kLHintsCenterX, 0, 0, 0, 0));
	  ckBtn[j][i]->Associate(this);
	} 
    }


  TGHorizontal3DLine *ln2 = new TGHorizontal3DLine(mn_vert, 50, 2);
  mn_vert->AddFrame(ln2, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 10, 10));
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  TGCompositeFrame *parFrame2 = new TGCompositeFrame(mn_vert, 0, 0, kHorizontalFrame);
  mn_vert->AddFrame(parFrame2, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 2, 2, 1, 1));
  parFrame2->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  CopyButton = new TGTextButton(parFrame2, "&Copy", 4000);
  parFrame2->AddFrame(CopyButton, new TGLayoutHints(kLHintsCenterX, 0, 0, 0, 0));
  CopyButton->Associate(this);
  CopyButton->ChangeOptions(CopyButton->GetOptions() ^ kRaisedFrame);
  CopyButton->SetFont(TEXTBUTTON_FONT, false);
  CopyButton->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  CopyButton->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));

  CancelButton = new TGTextButton(parFrame2, "&Cancel", 4001);
  parFrame2->AddFrame(CancelButton, new TGLayoutHints(kLHintsCenterX, 0, 0, 0, 0));
  CancelButton->Associate(this);
  CancelButton->ChangeOptions(CancelButton->GetOptions() ^ kRaisedFrame);
  CancelButton->SetFont(TEXTBUTTON_FONT, false);
  CancelButton->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  CancelButton->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  SetWindowName("Copy Pars");
  MapSubwindows();
  MapWindow();
  Resize();

  // for (int i = detector->NumModules; i < 13; ++i)
  //   {
  //     column[i]->UnmapWindow();
  //   }
  for (int i = detector->NumModules; i < 13; ++i)
    for (int j = 0; j < 17; ++j)
    {
      ckBtn[j][i]->SetEnabled(false);

    }
  
}

CopyPars::~CopyPars()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void  CopyPars::checkbutton(Long_t parm1)
{
  // 添加功能类需要修改本函数
  if(parm1 > 6216 || parm1 < 5000) return;
  if(parm1%100 == 16) // All
    {
      int k = parm1/100-50;//第几个功能类
      if(k < 0 || k > 12) return;
      if(ckBtn[16][k]->IsDown())
	{
	  for(int i = 0;i < 16; i++)
	    ckBtn[i][k]->SetState(kButtonDown);
	}
      else
	{
	  for(int i = 0;i < 16; i++)
	    ckBtn[i][k]->SetState(kButtonUp);
	}
    }
  else //0-15
    {
      int k = parm1/100-50;
      if(k < 0 || k > 12) return;
      int j = parm1%100;
      if(j < 0 || j > 15) return;
      if(!ckBtn[j][k]->IsDown())
  	{
  	  if(ckBtn[16][k]->IsDown())
  	    {
  	      ckBtn[16][k]->SetState(kButtonUp);
  	    }
  	}
    }
}


void CopyPars::copy_values()
{
  if(detector->GetRunFlag()) return;
  
  int retval;
  unsigned short BitMask = 0;
  unsigned short DestinationMask[13*16] ={0};

  for (int i = 0; i < 13; ++i)//bit
  {
    if(checkbitmask[i]->IsDown())
      BitMask += (1<<i);
  }

 for (int i = 0; i < detector->NumModules; ++i)
   for (int j = 0; j < 16; ++j)
     {
       if(ckBtn[j][i]->IsDown())
	 DestinationMask[16*i+j] = 1;
       else
	 DestinationMask[16*i+j] = 0;
     }

 retval = Pixie16CopyDSPParameters(BitMask,modnum0->GetIntNumber(),chnum0->GetIntNumber(),DestinationMask);
 if(retval < 0) ErrorInfo("CopyPars.cc", "copy_values()", "Pixie16CopyDSPParameters", retval);

}


Bool_t CopyPars::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
  int test1 = 0;
  int test2 = 0;
  switch (test1 = GET_MSG(msg))
    {
    case kC_COMMAND:
      switch (test2 = GET_SUBMSG(msg))
	{
	case kCM_CHECKBUTTON :
	  checkbutton(parm1);
	  break;
	  
	case kCM_BUTTON:
	  switch (parm1)
	    {
	    case 4000:
	      copy_values();
	      break;
	    case 4001:		/// Cancel Button
	      DeleteWindow();
	      break;
	    case COPYPARS_MODNUM0:
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

	      
	    case COPYPARS_CHNUM0:
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

	    default:
	      break;
	    }
	  break;
	}
      break;

      
    case kC_TEXTENTRY:
      switch (parm1)
	{
	case COPYPARS_MODNUM0:
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
	  
	  	  
	case COPYPARS_CHNUM0:
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
 
	}
      break;


      
      
    default:
      break;
    }

  
  return kTRUE;
}


// 
// CopyPars.cc ends here
