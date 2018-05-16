#include "Csra.hh"
#include "Global.hh"

#include "pixie16app_export.h"

#include "GuiTypes.h"
#include "TG3DLine.h"
#include "TGWidget.h"

#include <iostream>
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Csra::Csra(const TGWindow * p, const TGWindow * main, int NumModules)
  :TGTransientFrame(p, main, 10, 10, kHorizontalFrame)
{
  SetCleanup(kDeepCleanup);
  module_number1 = 0;

  numModules = NumModules;

  mn_vert = new TGVerticalFrame(this, 200, 300);
  AddFrame(mn_vert, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2));
  
  mn = new TGHorizontalFrame(mn_vert, 200, 300);
  mn_vert->AddFrame(mn, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  
  column1 = new TGVerticalFrame(mn, 200, 300);
  column2 = new TGVerticalFrame(mn, 200, 300);
  column3 = new TGVerticalFrame(mn, 200, 300);
  column4 = new TGVerticalFrame(mn, 200, 300);
  column5 = new TGVerticalFrame(mn, 400, 300);
  column6 = new TGVerticalFrame(mn, 400, 300);
  column7 = new TGVerticalFrame(mn, 400, 300);
  column8 = new TGVerticalFrame(mn, 400, 300);
  column9 = new TGVerticalFrame(mn, 400, 300);
  column10 = new TGVerticalFrame(mn, 400, 300);
  column11 = new TGVerticalFrame(mn, 400, 300);
  column12 = new TGVerticalFrame(mn, 400, 300);
  column13 = new TGVerticalFrame(mn, 400, 300);
  column14 = new TGVerticalFrame(mn, 400, 300);
  column15 = new TGVerticalFrame(mn, 400, 300);
  column16 = new TGVerticalFrame(mn, 400, 300);
  column17 = new TGVerticalFrame(mn, 400, 300);
  column18 = new TGVerticalFrame(mn, 400, 300);
  column19 = new TGVerticalFrame(mn, 400, 300);
  column20 = new TGVerticalFrame(mn, 400, 300);
  column21 = new TGVerticalFrame(mn, 400, 300);
  column22 = new TGVerticalFrame(mn, 400, 300);
  column23 = new TGVerticalFrame(mn, 400, 300);
  
  buttons = new TGHorizontalFrame(mn_vert, 400, 300);

  mn->AddFrame(column1, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  mn->AddFrame(column2, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  mn->AddFrame(column3, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  mn->AddFrame(column4, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  mn->AddFrame(column5, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  mn->AddFrame(column6, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  mn->AddFrame(column7, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  mn->AddFrame(column8, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  mn->AddFrame(column9, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  mn->AddFrame(column10, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  mn->AddFrame(column11, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  mn->AddFrame(column12, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  mn->AddFrame(column13, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  mn->AddFrame(column14, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  mn->AddFrame(column15, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  mn->AddFrame(column16, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  mn->AddFrame(column17, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  mn->AddFrame(column18, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  mn->AddFrame(column19, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  mn->AddFrame(column20, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  mn->AddFrame(column21, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  mn->AddFrame(column22, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  mn->AddFrame(column23, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  //////////////////////////first column////////////////////////

  TGTextEntry *te = new TGTextEntry(column1, new TGTextBuffer(100), 10000,
				    te->GetDefaultGC ()(),
				    te->GetDefaultFontStruct (),
				    kRaisedFrame | kDoubleBorder,
				    GetWhitePixel ());
  te->SetText("Ch #");
  te->Resize(35, 20);
  te->SetEnabled(kFALSE);
  te->SetFrameDrawn(kTRUE);
  column1->AddFrame(te, new TGLayoutHints(kLHintsCenterX, 0, 0, 10, 0));

  TGTextEntry *Labels[17];

  for (int i = 0; i < 16; i++)
    {
      Labels[i] = new TGTextEntry(column1, new TGTextBuffer(100), 10000,
				  Labels[i]->GetDefaultGC()(),
				  Labels[i]->GetDefaultFontStruct(),
				  kRaisedFrame | kDoubleBorder,
				  GetWhitePixel());
      Labels[i]->SetText(TString::Format("%d", i).Data());
      Labels[i]->Resize(35, 20);
      Labels[i]->SetEnabled(kFALSE);
      Labels[i]->SetFrameDrawn(kTRUE);

      column1->AddFrame(Labels[i], new TGLayoutHints(kLHintsCenterX, 0, 3, 0, 0));
    }
  
  // enable all channel
  Labels[16] = new TGTextEntry(column1, new TGTextBuffer(100), 10000,
			       Labels[16]->GetDefaultGC()(),
			       Labels[16]->GetDefaultFontStruct(),
			       kRaisedFrame | kDoubleBorder,
			       GetWhitePixel());
  Labels[16]->SetText("CAll");
  Labels[16]->Resize(35, 20);
  Labels[16]->SetEnabled(kFALSE);
  Labels[16]->SetFrameDrawn(kTRUE);
  
  column1->AddFrame(Labels[16], new TGLayoutHints(kLHintsCenterX, 0, 3, 0, 0));

  make_columns(column2, ckBtn, (char*)"FTS", (char*)"Fast trigger selection (local FiPPI trigger vs. external fast trigger from System FPGA)", 5000,5);
  make_columns(column3, ckBtn_1, (char*)"MSE", (char*)"Module validation signal selection (module validation trigger from System FPGA vs. module GATE from front panel)", 5100,4);
  make_columns(column4, ckBtn_2, (char*)"GC", (char*)"Good channel", 5200,2);
  make_columns(column5, ckBtn_3, (char*)"CSE", (char*)"Channel validation signal selection (channel validation trigger from System FPGA vs. channel GATE from front panel)", 5300,4);
  make_columns(column6, ckBtn_4, (char*)"BDA", (char*)"Block data acquisition if trace or header DPMs are full", 5400);
  make_columns(column7, ckBtn_5, (char*)"SP", (char*)"Input signal polarity control", 5500,2);
  make_columns(column8, ckBtn_6, (char*)"CTV", (char*)"Enable channel trigger veto", 5600,3);
  make_columns(column9, ckBtn_7, (char*)"HE", (char*)"Histograms energy in the on-chip MCA", 5700);
  make_columns(column10, ckBtn_8, (char*)"TC", (char*)"Trace capture and associated header data", 5800,2);
  make_columns(column11, ckBtn_9, (char*)"EQS", (char*)"QDC summing and associated header data", 5900,2);
  make_columns(column12, ckBtn_10, (char*)"ECT", (char*)"CFD for real time, trace capture and QDC capture", 6000,2);
  make_columns(column13, ckBtn_11, (char*)"MVT", (char*)"Require module validation trigger", 6100,4);
  make_columns(column14, ckBtn_12, (char*)"ERB", (char*)"Record raw energy sums and baseline in event header", 6200,2);
  make_columns(column15, ckBtn_13, (char*)"CVT", (char*)"Require channel validation trigger", 6300,4);
  make_columns(column16, ckBtn_14, (char*)"IR", (char*)"Control input relay: => Gain Smaller/Larger", 6400,2);
  // bits[15:16]
  // 00: record all events (trace, timestamps, etc., but no energy for piled-up events)
  // 10: only record single events (trace, energy, timestamps, etc.) (i.e., reject piled-up events)
  // 01: record trace, timestamps, etc., for piled-up events but do not record trace for single events
  // 11: only record trace, timestamps, etc., for piled-up events (i.e., reject single events)
  make_columns(column17, ckBtn_15, (char*)"NPR", (char*)"Control pileup rejection using bit 15 and 16 >> 00: record all events (trace, timestamps, etc., but no energy for piled-up events)  10: only record single events (trace, energy, timestamps, etc.) (i.e., reject piled-up events)", 6500,6);
  make_columns(column18, ckBtn_16, (char*)"IPR", (char*)"Control pileup rejection using bit 15 and 16 >> 01: record trace, timestamps, etc., for piled-up events but do not record trace for single events  11: only record trace, timestamps, etc., for piled-up events (i.e., reject single events)", 6600,6);
  make_columns(column19, ckBtn_17, (char*)"NTL", (char*)"Enable 'no traces for large pulses' feature", 6700);
  make_columns(column20, ckBtn_18, (char*)"GTS", (char*)"Group trigger selection (local FiPPI trigger vs. external group trigger from System FPGA)", 6800,5);
  make_columns(column21, ckBtn_19, (char*)"CVS", (char*)"Channel veto selection (front panel channel GATE vs. channel validation trigger)", 6900,3);
  make_columns(column22, ckBtn_20, (char*)"MVS", (char*)"Module veto selection (front panel module GATE vs. module validation trigger)", 7000,3);
  make_columns(column23, ckBtn_21, (char*)"ETS", (char*)"Enable (checked) or disable (unchecked) recording of external clock timestamps in event header", 7100);

  
  /////////////////////////////module entry///////////////////////////////

  TGHorizontal3DLine *ln1 = new TGHorizontal3DLine(column1, 50, 2);
  column1->AddFrame(ln1, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 10, 10));
  
  TGLabel *mod = new TGLabel(buttons, "Module #");
  buttons->AddFrame(mod, new TGLayoutHints(kLHintsCenterX, 5, 10, 3, 0));
  
  numericMod = new TGNumberEntry(buttons, 0, 4, 100, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3/*kNELLimitMinMax*/, 0, 3);
  numericMod->SetButtonToNum(0);
  buttons->AddFrame(numericMod, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 20, 0, 0));

  numericMod->Associate(this);
  mn_vert->AddFrame(buttons, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  /////////////////////////////////////////////////////////////////////////
  ////////////////////////////Buttons/////////////////////////////////////
  LoadButton = new TGTextButton(buttons, "&Load", 4000);
  LoadButton->Associate(this);
  ApplyButton = new TGTextButton(buttons, "&Apply", 4001);
  ApplyButton->Associate(this);
  CancelButton = new TGTextButton(buttons, "&Cancel", 4002);
  CancelButton->Associate(this);
  buttons->AddFrame(LoadButton, new TGLayoutHints(kLHintsCenterX, 0, 0, 0, 0));
  buttons->AddFrame(ApplyButton, new TGLayoutHints(kLHintsCenterX, 0, 0, 0, 0));
  buttons->AddFrame(CancelButton, new TGLayoutHints(kLHintsCenterX, 0, 0, 0, 0));

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

int Csra::make_columns(TGVerticalFrame * column, TGCheckButton * ckBtn_g[17],
		       char *title, char *tooltip, int id,int flagcolor)
{
  TGTextEntry *ra = new TGTextEntry(column, new TGTextBuffer(100),
				    10000, ra->GetDefaultGC()(),
				    ra->GetDefaultFontStruct(),
				    kRaisedFrame | kDoubleBorder,
				    GetWhitePixel());
  ra->SetText(title);
  ra->Resize(35, 20);
  ra->SetEnabled(kFALSE);
  ra->SetFrameDrawn(kTRUE);
  ra->SetAlignment(kTextCenterX);
  ra->SetToolTipText(tooltip, 0);

  if(flagcolor == 5)
    {
      fClient->GetColorByName("yellow", color);
      ra->SetTextColor(color, false);
    }  
  if(flagcolor == 4)
    {
      fClient->GetColorByName("blue", color);
      ra->SetTextColor(color, false);
    }
  if(flagcolor == 2)
    {
      fClient->GetColorByName("red", color);
      ra->SetTextColor(color, false);
    }
  if(flagcolor == 3)
    {
      fClient->GetColorByName("green", color);
      ra->SetTextColor(color, false);
    }  
  if(flagcolor == 6)
    {
      fClient->GetColorByName("pink", color);
      ra->SetTextColor(color, false);
    }
  
  column->AddFrame(ra, new TGLayoutHints(kLHintsCenterX, 0, 0, 10, 3));

  column->AddFrame(ckBtn_g[0] = new TGCheckButton(column, "", id),
		   new TGLayoutHints(kLHintsCenterX, 0, 0, 0, 0));
  ckBtn_g[0]->Associate(this);

  for (int i = 1; i < 17; i++)
    {
      column->AddFrame(ckBtn_g[i] =
		       new TGCheckButton(column, "", id + i),
		       new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 2));
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
