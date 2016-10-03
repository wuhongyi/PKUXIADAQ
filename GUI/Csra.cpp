#include "Csra.h"

#include "Global.h"
#include "GuiTypes.h"
#include "TGWidget.h"
#include "TG3DLine.h"
#include "pixie16app_export.h"
#include "pixie16app_defs.h"
#include <iostream>
using namespace std;

Csra::Csra(const TGWindow * p, const TGWindow * main, int NumModules)
  :TGTransientFrame(p, main, 10, 10, kHorizontalFrame)
{
  SetCleanup(kDeepCleanup);
  module_number1 = 0;
  char name[20];

  //numModules=NumModules;
  numModules = PRESET_MAX_MODULES;

  mn_vert = new TGVerticalFrame(this, 200, 300);
  mn = new TGHorizontalFrame(mn_vert, 200, 300);
  mn_vert->AddFrame(mn, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  AddFrame(mn_vert, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2));
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
      sprintf(name, "%d", i);
      Labels[i] = new TGTextEntry(column1, new TGTextBuffer(100), 10000,
				  Labels[i]->GetDefaultGC()(),
				  Labels[i]->GetDefaultFontStruct(),
				  kRaisedFrame | kDoubleBorder,
				  GetWhitePixel());
      Labels[i]->SetText(name);
      Labels[i]->Resize(35, 20);
      Labels[i]->SetEnabled(kFALSE);
      Labels[i]->SetFrameDrawn(kTRUE);

      //      Labels[i] = new TGLabel (column1, name);
      column1->AddFrame (Labels[i],
			 new TGLayoutHints(kLHintsCenterX, 0, 3, 0, 0));
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

  make_columns(column2, ckBtn, (char*)"RGT", (char*)"Respond to group triggers only: Rev-A", 5000);
  make_columns(column3, ckBtn_1, (char*)"MIL", (char*)"Measure individual live time: Rev-A ", 5100);
  make_columns(column4, ckBtn_2, (char*)"GC", (char*)"Good channel: All types", 5200);
  make_columns(column5, ckBtn_3, (char*)"RA", (char*)"Read allways: Rev-A", 5300);
  make_columns(column6, ckBtn_4, (char*)"ET", (char*)"Enable trigger: Rev-A", 5400);
  make_columns(column7, ckBtn_5, (char*)"TP", (char*)"Trigger Positive: All types", 5500);
  make_columns(column8, ckBtn_6, (char*)"GT", (char*)"GFLT input: Rev-A", 5600);
  make_columns(column9, ckBtn_7, (char*)"HE", (char*)"Histogram energies: All types", 5700);
  make_columns(column10, ckBtn_8, (char*)"ETC", (char*)"Enable Trace Capture: All But Rev-A", 5800);
  make_columns(column11, ckBtn_9, (char*)"EQS", (char*)"Enable QDC sums Capture: All But Rev-A", 5900);
  make_columns(column12, ckBtn_10, (char*)"ECT", (char*)"Enable CFD trigger mode: All But Rev-A", 6000);
  make_columns(column13, ckBtn_11, (char*)"EGT", (char*)"Enable Global trigger validation: All But Rev-A", 6100);
  make_columns(column14, ckBtn_12, (char*)"ERB", (char*)"Enable capture raw energy sums and baselines: All But Rev-A", 6200);
  make_columns(column15, ckBtn_13, (char*)"CTV", (char*)"Enable Channel trigger validation: All But Rev-A", 6300);
  make_columns(column16, ckBtn_14, (char*)"EIR", (char*)"Enable input relay: All types", 6400);
  make_columns(column17, ckBtn_15, (char*)"PRC", (char*)"Pileup rejection control: All But Rev-A", 6500);

  /////////////////////////////module entry///////////////////////////////

  TGHorizontal3DLine *ln1 = new TGHorizontal3DLine(column1, 50, 2);
  TGLabel *mod = new TGLabel(buttons, "Module #");

  numericMod = new TGNumberEntry(buttons, 0, 4, 100, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3/*kNELLimitMinMax*/, 0, 3);
  numericMod->SetButtonToNum(0);

  column1->AddFrame(ln1, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 10, 10));
  buttons->AddFrame(mod, new TGLayoutHints(kLHintsCenterX, 5, 10, 3, 0));
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

Csra::~Csra ()
{
  cout<<"Csra is deleted!"<<endl;
}

int Csra::make_columns(TGVerticalFrame * column, TGCheckButton * ckBtn_g[17],
		       char *title, char *tooltip, int id)
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

  column->AddFrame(ra, new TGLayoutHints(kLHintsCenterX, 0, 0, 10, 3));

  column->AddFrame(ckBtn_g[0] = new TGCheckButton(column, "", id),
		   new TGLayoutHints(kLHintsCenterX, 0, 0, 0, 0));
  ckBtn_g[0]->Associate(this);

  for (int i = 1; i < 17; i++)
    {
      column->AddFrame(ckBtn_g[i] =
		       new TGCheckButton(column, "", id + i),
		       new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
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
		    }
		}
	      else
		{
		  if (module_number1 != 0)
		    {
		      if (--module_number1 == 0)
			module_number1 = 0;
		      numericMod->SetIntNumber(module_number1);
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
		change_values(module_number1);
	      else
		std::cout << "please load once first !\n";
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
	default:
	  break;
	}
      break;
    default:
      break;
    }

  return kTRUE;
}

int Csra::load_info(Long_t mod)
{
  double ChanParData = -1;
  int retval;
  unsigned short gt;

  for (int i = 0; i < 16; i++)
    {
      retval = Pixie16ReadSglChanPar((char*)"CHANNEL_CSRA", &ChanParData, module_number1, i);
      if(retval < 0) ErrorInfo("Csra.cpp", "load_info(...)", "Pixie16ReadSglChanPar/CHANNEL_CSRA", retval);
      
      for(int j = 0;j < 16;j++){
        gt = APP32_TstBit(j, (unsigned long) ChanParData);
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
      for(int j = 0;j < 16;j++){
        if(tmpckBtn[j][i]->IsDown())
	  ChanParData = APP32_SetBit(j,(unsigned short)ChanParData);
	else 
	  ChanParData = APP32_ClrBit(j,(unsigned short)ChanParData);
      }
      
      retval = Pixie16WriteSglChanPar((char*)"CHANNEL_CSRA", ChanParData, module_number1, i);
      if(retval < 0) ErrorInfo("Csra.cpp", "change_values(...)", "Pixie16WriteSglChanPar/CHANNEL_CSRA", retval);
    } 

  return 1;
}

void Csra::checkbutton(Long_t parm1)
{
  if(parm1 > 6516 || parm1 < 5000) return;
  if(parm1%100 == 16 ){
    int k = parm1/100-50;
    if(k < 0 || k > 15) return;
    if(tmpckBtn[k][16]->IsDown()){
      for(int i = 0;i < 16;i++)
        tmpckBtn[k][i]->SetState(kButtonDown);
    }else {
      for(int i = 0;i < 16;i++)
        tmpckBtn[k][i]->SetState(kButtonUp);
    }
  } else {
    int k = parm1/100-50;
    if(k < 0 || k > 15) return;
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
}
