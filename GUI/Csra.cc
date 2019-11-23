#include "Csra.hh"
#include "Global.hh"
#include "Detector.hh"
#include "pixie16app_export.h"

#include "GuiTypes.h"
#include "TG3DLine.h"
#include "TGWidget.h"
#include "TColor.h"
#include <iostream>
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Csra::Csra(const TGWindow * p, const TGWindow * main, Detector *det)
  :TGTransientFrame(p, main, 10, 10, kHorizontalFrame)
{
  SetCleanup(kDeepCleanup);
  detector = det;
  module_number1 = 0;
  numModules = detector->NumModules;
  SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  mn_vert = new TGVerticalFrame(this, 200, 300);
  AddFrame(mn_vert, new TGLayoutHints(kLHintsTop | kLHintsLeft, 10, 10, 5, 5));
  mn_vert->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  mn = new TGHorizontalFrame(mn_vert, 200, 300);
  mn_vert->AddFrame(mn, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  mn->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));


  ccolumn = new TGVerticalFrame(mn, 200, 300);
  mn->AddFrame(ccolumn, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  ccolumn->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  for (int i = 0; i < CCSRAUSEDBITBUMBER; ++i)
    {
      column[i] = new TGVerticalFrame(mn, 200, 300);
      mn->AddFrame(column[i], new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
      column[i]->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
    }

  

  TGTextEntry *te = new TGTextEntry(ccolumn, new TGTextBuffer(100), 10000,
				    te->GetDefaultGC ()(),
				    te->GetDefaultFontStruct (),
				    kRaisedFrame | kDoubleBorder,
				    GetWhitePixel ());
  ccolumn->AddFrame(te, new TGLayoutHints(kLHintsCenterX, 0, 0, 10, 0));
  te->SetText("Ch #");
  te->Resize(35, 20);
  te->SetAlignment(kTextCenterX);
  te->SetEnabled(kFALSE);
  te->SetFrameDrawn(kFALSE);
  te->SetFont(TABLE_LABEL_CH_FONT, false);
  te->ChangeBackground(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  te->SetTextColor(TColor::RGB2Pixel(LABEL_TEXT_R,LABEL_TEXT_G,LABEL_TEXT_B), false);
  
  TGTextEntry *Labels[17];
  for (int i = 0; i < 17; i++)
    {
      Labels[i] = new TGTextEntry(ccolumn, new TGTextBuffer(100), 10000,
				  Labels[i]->GetDefaultGC()(),
				  Labels[i]->GetDefaultFontStruct(),
				  kRaisedFrame | kDoubleBorder,
				  GetWhitePixel());
      ccolumn->AddFrame(Labels[i], new TGLayoutHints(kLHintsCenterX, 0, 3, 0, 0));
      Labels[i]->SetText(TString::Format("%d", i).Data());
      Labels[i]->Resize(35, 20);
      Labels[i]->SetEnabled(kFALSE);
      Labels[i]->SetFrameDrawn(kFALSE);
      Labels[i]->SetAlignment(kTextCenterX);
      Labels[i]->SetFont(TABLE_LABEL_CH_FONT, false);
      Labels[i]->ChangeBackground(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
      Labels[i]->SetTextColor(TColor::RGB2Pixel(LABEL_TEXT_R,LABEL_TEXT_G,LABEL_TEXT_B), false);
    }
  Labels[16]->SetText("All");
  

  
  

  make_columns(column[0], ckBtn, (char*)"FTS", (char*)"Fast trigger selection (local FiPPI trigger vs. external fast trigger from System FPGA)", 5000,5);
  make_columns(column[1], ckBtn_1, (char*)"MSE", (char*)"Module validation signal selection (module validation trigger from System FPGA vs. module GATE from front panel)", 5100,4);
  make_columns(column[2], ckBtn_2, (char*)"GC", (char*)"Good channel", 5200,2);
  make_columns(column[3], ckBtn_3, (char*)"CSE", (char*)"Channel validation signal selection (channel validation trigger from System FPGA vs. channel GATE from front panel)", 5300,4);
  make_columns(column[4], ckBtn_4, (char*)"BDA", (char*)"Block data acquisition if trace or header DPMs are full", 5400);
  make_columns(column[5], ckBtn_5, (char*)"SP", (char*)"Input signal polarity control", 5500,2);
  make_columns(column[6], ckBtn_6, (char*)"CTV", (char*)"Enable channel trigger veto", 5600,3);
  make_columns(column[7], ckBtn_7, (char*)"HE", (char*)"Histograms energy in the on-chip MCA", 5700);
  make_columns(column[8], ckBtn_8, (char*)"TC", (char*)"Trace capture and associated header data", 5800,2);
  make_columns(column[9], ckBtn_9, (char*)"EQS", (char*)"QDC summing and associated header data", 5900,2);
  make_columns(column[10], ckBtn_10, (char*)"ECT", (char*)"CFD for real time, trace capture and QDC capture", 6000,2);
  make_columns(column[11], ckBtn_11, (char*)"MVT", (char*)"Require module validation trigger", 6100,4);
  make_columns(column[12], ckBtn_12, (char*)"ERB", (char*)"Record raw energy sums and baseline in event header", 6200,2);
  make_columns(column[13], ckBtn_13, (char*)"CVT", (char*)"Require channel validation trigger", 6300,4);
  make_columns(column[14], ckBtn_14, (char*)"IR", (char*)"Control input relay: => Gain Smaller/Larger", 6400,2);
  // bits[15:16]
  // 00: record all events (trace, timestamps, etc., but no energy for piled-up events)
  // 10: only record single events (trace, energy, timestamps, etc.) (i.e., reject piled-up events)
  // 01: record trace, timestamps, etc., for piled-up events but do not record trace for single events
  // 11: only record trace, timestamps, etc., for piled-up events (i.e., reject single events)
  make_columns(column[15], ckBtn_15, (char*)"NPR", (char*)"Control pileup rejection using bit 15 and 16 >> 00: record all events (trace, timestamps, etc., but no energy for piled-up events)  10: only record single events (trace, energy, timestamps, etc.) (i.e., reject piled-up events)", 6500,6);
  make_columns(column[16], ckBtn_16, (char*)"IPR", (char*)"Control pileup rejection using bit 15 and 16 >> 01: record trace, timestamps, etc., for piled-up events but do not record trace for single events  11: only record trace, timestamps, etc., for piled-up events (i.e., reject single events)", 6600,6);
  make_columns(column[17], ckBtn_17, (char*)"NTL", (char*)"Enable 'no traces for large pulses' feature", 6700);
  make_columns(column[18], ckBtn_18, (char*)"GTS", (char*)"Group trigger selection (local FiPPI trigger vs. external group trigger from System FPGA)", 6800,5);
  make_columns(column[19], ckBtn_19, (char*)"CVS", (char*)"Channel veto selection (front panel channel GATE vs. channel validation trigger)", 6900,3);
  make_columns(column[20], ckBtn_20, (char*)"MVS", (char*)"Module veto selection (front panel module GATE vs. module validation trigger)", 7000,3);
  make_columns(column[21], ckBtn_21, (char*)"ETS", (char*)"Enable (checked) or disable (unchecked) recording of external clock timestamps in event header", 7100);

  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGHorizontal3DLine *ln1 = new TGHorizontal3DLine(mn_vert, 250, 2);
  mn_vert->AddFrame(ln1, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 10, 10));

  
  buttons = new TGHorizontalFrame(mn_vert, 400, 300);
  mn_vert->AddFrame(buttons, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  buttons->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  TGLabel *mod = new TGLabel(buttons, "Module #");
  buttons->AddFrame(mod, new TGLayoutHints(kLHintsCenterX, 5, 10, 5, 0));
  mod->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  mod->SetTextColor(TColor::RGB2Pixel(TABLE_LABEL_TEXT_R,TABLE_LABEL_TEXT_G,TABLE_LABEL_TEXT_B));
  
  numericMod = new TGNumberEntry(buttons, 0, 4, 100, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3/*kNELLimitMinMax*/, 0, 3);
  buttons->AddFrame(numericMod, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 20, 3, 0));
  numericMod->SetButtonToNum(0);
  numericMod->Associate(this);
  numericMod->GetNumberEntry()->ChangeOptions(numericMod->GetNumberEntry()->GetOptions() ^ kRaisedFrame);
  numericMod->GetNumberEntry()->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
  numericMod->GetButtonUp()->ChangeOptions(numericMod->GetButtonUp()->GetOptions() ^ kRaisedFrame);
  numericMod->GetButtonDown()->ChangeOptions(numericMod->GetButtonDown()->GetOptions() ^ kRaisedFrame);
  numericMod->ChangeSubframesBackground(TColor::RGB2Pixel(TEXTENTRY_BG_R,TEXTENTRY_BG_G,TEXTENTRY_BG_B));
  
  

  LoadButton = new TGTextButton(buttons, "&Load", 4000);
  buttons->AddFrame(LoadButton, new TGLayoutHints(kLHintsCenterX, 0, 5, 0, 0));
  LoadButton->Associate(this);
  LoadButton->ChangeOptions(LoadButton->GetOptions() ^ kRaisedFrame);
  LoadButton->SetFont(TEXTBUTTON_FONT, false);
  LoadButton->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  LoadButton->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));
  
  ApplyButton = new TGTextButton(buttons, "&Apply", 4001);
  buttons->AddFrame(ApplyButton, new TGLayoutHints(kLHintsCenterX, 0, 5, 0, 0));
  ApplyButton->Associate(this);
  ApplyButton->ChangeOptions(ApplyButton->GetOptions() ^ kRaisedFrame);
  ApplyButton->SetFont(TEXTBUTTON_FONT, false);
  ApplyButton->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  ApplyButton->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));
  
  CancelButton = new TGTextButton(buttons, "&Cancel", 4002);
  buttons->AddFrame(CancelButton, new TGLayoutHints(kLHintsCenterX, 0, 5, 0, 0));
  CancelButton->Associate(this);
  CancelButton->ChangeOptions(CancelButton->GetOptions() ^ kRaisedFrame);
  CancelButton->SetFont(TEXTBUTTON_FONT, false);
  CancelButton->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  CancelButton->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  MapSubwindows();
  Resize();			// resize to default size
  CenterOnParent();

  SetWindowName("CSRA Register");

  MapWindow();

  getckbuttonaddress();
}

Csra::~Csra()
{
  std::cout<<"Csra is deleted!"<<std::endl;
}

int Csra::make_columns(TGVerticalFrame *column, TGCheckButton *ckBtn_g[17],
		       char *title, char *tooltip, int id,int flagcolor)
{
  TGTextEntry *ra = new TGTextEntry(column, new TGTextBuffer(100),
				    10000, ra->GetDefaultGC()(),
				    ra->GetDefaultFontStruct(),
				    kRaisedFrame | kDoubleBorder,
				    GetWhitePixel());
  column->AddFrame(ra, new TGLayoutHints(kLHintsCenterX, 0, 0, 10, 3));
  ra->SetText(title);
  ra->Resize(35, 20);
  ra->SetEnabled(kFALSE);
  ra->SetFrameDrawn(kFALSE);
  ra->SetAlignment(kTextCenterX);
  ra->SetToolTipText(tooltip, 0);
  ra->ChangeBackground(TColor::RGB2Pixel(TABLE_LABELTITLE_BG_R,TABLE_LABELTITLE_BG_G,TABLE_LABELTITLE_BG_B));

  if(flagcolor == 5)
    {
      ra->SetTextColor(TColor::RGB2Pixel(COLOR_YELLOW_R,COLOR_YELLOW_G,COLOR_YELLOW_B), false);
    }  
  else if(flagcolor == 4)
    {
      ra->SetTextColor(TColor::RGB2Pixel(COLOR_BLUE_R,COLOR_BLUE_G,COLOR_BLUE_B), false);
    }
  else if(flagcolor == 2)
    {
      ra->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
    }
  else if(flagcolor == 3)
    {
      ra->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
    }  
  else if(flagcolor == 6)
    {
      ra->SetTextColor(TColor::RGB2Pixel(COLOR_PINK_R,COLOR_PINK_G,COLOR_PINK_B), false);
    }
  else
    {
      ra->SetTextColor(TColor::RGB2Pixel(TABLE_LABELTITLE_TEXT_R,TABLE_LABELTITLE_TEXT_G,TABLE_LABELTITLE_TEXT_B), false);
    }
  

  for (int i = 0; i < 17; i++)
    {
      if(i==0)
	column->AddFrame(ckBtn_g[i] = new TGCheckButton(column, "", id+i), new TGLayoutHints(kLHintsCenterX, 0, 0, 0, 0));
      else
	column->AddFrame(ckBtn_g[i] = new TGCheckButton(column, "", id+i), new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 2));
      
      ckBtn_g[i]->Associate(this);
    }
  return 1;
}


/////////////////////////process message//////////////////////////////////

Bool_t Csra::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
  switch (GET_MSG(msg))
    {
    case kC_COMMAND:
      switch (GET_SUBMSG(msg))
	{
	case kCM_BUTTON:
	  switch (parm1)
	    {
	    case (100):
	      if (parm2 == 0)
		{
		  if (module_number1 != numModules-1)
		    {
		      ++module_number1;
		      numericMod->SetIntNumber(module_number1);
		      load_info(module_number1);
		    }
		}
	      else
		{
		  if (module_number1 != 0)
		    {
		      if (--module_number1 == 0)
			module_number1 = 0;
		      numericMod->SetIntNumber(module_number1);
		      load_info(module_number1);
		    }
		}
	      break;
	    case 4000:
	      {
		Load_Once = true;
		load_info(module_number1);
	      }
	      break;
	    case 4001:
	      if (Load_Once)
		{
		  change_values(module_number1);
		  load_info(module_number1);
		}
	      else
		std::cout << "please load once first !"<<std::endl;
	      break;
	    case 4002:		/// Cancel Button
	      DeleteWindow();
	      break;
	    default:
	      break;
	    }
	  break;
	  
	case kCM_CHECKBUTTON :
	  {
	    checkbutton(parm1);
	    break;
	  }
	  
	default://GET_SUBMSG(msg)
	  break;
	}//switch (GET_SUBMSG(msg))
      break;//kC_COMMAND


      
    default://GET_MSG(msg)
      break;
    }//switch (GET_MSG(msg))

  return kTRUE;
}

int Csra::load_info(Long_t mod)
{
  if(detector->GetRunFlag()) return 1;
  
  double ChanParData = -1;
  int retval;
  unsigned short gt;

  for (int i = 0; i < 16; i++)
    {
      retval = Pixie16ReadSglChanPar((char*)"CHANNEL_CSRA", &ChanParData, mod, i);
      if(retval < 0) ErrorInfo("Csra.cc", "load_info(...)", "Pixie16ReadSglChanPar/CHANNEL_CSRA", retval);
      
      for(int j = 0;j < CCSRAUSEDBITBUMBER;j++){
        gt = APP32_TstBit(j, ChanParData);
	if(gt==0) 
	  tmpckBtn[j][i]->SetState(kButtonUp);
	else 
	  tmpckBtn[j][i]->SetState(kButtonDown);
      }
    }

  //std::cout << "loading info\n";
  return 1;
}

int Csra::change_values(Long_t mod)
{
  if(detector->GetRunFlag()) return 1;
  
  double ChanParData = 0;
  int retval;

  for (int i = 0; i < 16; i++)
    {
      for(int j = 0;j < CCSRAUSEDBITBUMBER;j++){
        if(tmpckBtn[j][i]->IsDown())
	  ChanParData = APP32_SetBit(j,ChanParData);
	else 
	  ChanParData = APP32_ClrBit(j,ChanParData);
      }
      
      retval = Pixie16WriteSglChanPar((char*)"CHANNEL_CSRA", ChanParData, mod, i);
      if(retval < 0) ErrorInfo("Csra.cc", "change_values(...)", "Pixie16WriteSglChanPar/CHANNEL_CSRA", retval);
    } 

  return 1;
}

void Csra::checkbutton(Long_t parm1)
{
  // 添加功能类需要修改本函数
  if(parm1 > 7116 || parm1 < 5000) return;
  if(parm1%100 == 16 ) // All
    {
      int k = parm1/100-50;//第几个功能类
      if(k < 0 || k > 21) return;
      if(tmpckBtn[k][16]->IsDown()){
	for(int i = 0;i < 16;i++)
	  tmpckBtn[k][i]->SetState(kButtonDown);
      }else {
	for(int i = 0;i < 16;i++)
	  tmpckBtn[k][i]->SetState(kButtonUp);
      }
    }
  else //0-15
    {
      int k = parm1/100-50;
      if(k < 0 || k > 21) return;
      int j = parm1%100;
      if(j < 0 || j > 15) return;
      if(!tmpckBtn[k][j]->IsDown()){
	if(tmpckBtn[k][16]->IsDown()){
	  tmpckBtn[k][16]->SetState(kButtonUp);
	}
      }
    }
}

void Csra::getckbuttonaddress()
{
  tmpckBtn[0] = ckBtn;
  tmpckBtn[1] = ckBtn_1;
  tmpckBtn[2] = ckBtn_2;
  tmpckBtn[3] = ckBtn_3;
  tmpckBtn[4] = ckBtn_4;
  tmpckBtn[5] = ckBtn_5;
  tmpckBtn[6] = ckBtn_6;
  tmpckBtn[7] = ckBtn_7;
  tmpckBtn[8] = ckBtn_8;
  tmpckBtn[9] = ckBtn_9;
  tmpckBtn[10] = ckBtn_10;
  tmpckBtn[11] = ckBtn_11;
  tmpckBtn[12] = ckBtn_12;
  tmpckBtn[13] = ckBtn_13;
  tmpckBtn[14] = ckBtn_14;
  tmpckBtn[15] = ckBtn_15;
  tmpckBtn[16] = ckBtn_16;
  tmpckBtn[17] = ckBtn_17;
  tmpckBtn[18] = ckBtn_18;
  tmpckBtn[19] = ckBtn_19;
  tmpckBtn[20] = ckBtn_20;
  tmpckBtn[21] = ckBtn_21;
}
