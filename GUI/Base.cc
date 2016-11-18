// Base.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 11月 18 19:24:01 2016 (+0800)
// Last-Updated: 五 11月 18 20:01:56 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 5
// URL: http://wuhongyi.cn 

#include "Base.hh"
#include "Global.hh"

#include "pixie16app_export.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Base::Base(const TGWindow * p, const TGWindow * main, char *name, int columns, int rows, int NumModules)
  : Table(p, main, columns, rows, name,NumModules)
{
  char n[10];
  cl0->SetText("ch #");
  for (int i = 0; i < rows; i++)
    {
      sprintf (n, "%2d", i);
      Labels[i]->SetText(n);
    }
  CLabel[0]->SetText("DCOffset");
  CLabel[0]->SetAlignment(kTextCenterX);
  CLabel[1]->SetText("BLcut");
  CLabel[1]->SetAlignment(kTextCenterX);
  CLabel[2]->SetText("Baseline[%]");
  CLabel[2]->SetAlignment(kTextCenterX);
  CLabel[3]->SetText("Thresh. [ADC u]");
  CLabel[3]->SetAlignment(kTextCenterX);
  CLabel[4]->SetText("Trace Length[us]");
  CLabel[4]->SetAlignment(kTextCenterX);
  CLabel[5]->SetText("Trace Delay[us]");
  CLabel[5]->SetAlignment(kTextCenterX);

  

  ColumnGain = new TGVerticalFrame(mn, 200, 300);
  mn->AddFrame(ColumnGain, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  TGTextEntry *LabelGain =
    new TGTextEntry(ColumnGain, new TGTextBuffer(100), 10000,
		    LabelGain->GetDefaultGC()(),
		    LabelGain->GetDefaultFontStruct(),
		    kRaisedFrame | kDoubleBorder, GetWhitePixel());
  LabelGain->SetFont("-adobe-helvetica-bold-r-*-*-10-*-*-*-*-*-iso8859-1", false);

  LabelGain->Resize(80, 20);
  LabelGain->SetEnabled(kFALSE);
  LabelGain->SetFrameDrawn(kTRUE);
  ColumnGain->AddFrame(LabelGain, new TGLayoutHints(kLHintsCenterX, 0, 0, 10, 0));
  LabelGain->SetText("Gain");
  for (int i = 0; i < 16; i++)
    {
      ColumnGain->AddFrame(lstBoxGain[i] = new TGComboBox(ColumnGain, 2755 + i), new TGLayoutHints(kLHintsCenterX, 0, 0, 0, 0));
      lstBoxGain[i]->Associate(this);
      lstBoxGain[i]->AddEntry("Larger", 0);
      lstBoxGain[i]->AddEntry("Smaller", 1);
      lstBoxGain[i]->Resize(80, 20);
    }

  ColumnSign = new TGVerticalFrame(mn, 200, 300);
  mn->AddFrame(ColumnSign, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  TGTextEntry *LabelSign =
    new TGTextEntry(ColumnSign, new TGTextBuffer(100), 10000,
		    LabelSign->GetDefaultGC()(),
		    LabelSign->GetDefaultFontStruct(),
		    kRaisedFrame | kDoubleBorder, GetWhitePixel());
  LabelSign->SetFont("-adobe-helvetica-bold-r-*-*-10-*-*-*-*-*-iso8859-1", false);

  LabelSign->Resize(40, 20);
  LabelSign->SetEnabled(kFALSE);
  LabelSign->SetFrameDrawn(kTRUE);
  ColumnSign->AddFrame(LabelSign, new TGLayoutHints(kLHintsCenterX, 0, 0, 10, 0));
  LabelSign->SetText("Sign");
  for (int i = 0; i < 16; i++)
    {
      ColumnSign->AddFrame(lstBox[i] = new TGComboBox(ColumnSign, 2555 + i), new TGLayoutHints(kLHintsCenterX, 0, 0, 0, 0));
      lstBox[i]->Associate(this);

      lstBox[i]->AddEntry("+", 0);
      lstBox[i]->AddEntry("-", 1);
      lstBox[i]->Resize(40, 20);
    }
  ////////////////Copy Button//////////////////////////////////////////////
  TGHorizontal3DLine *ln2 = new TGHorizontal3DLine(mn_vert, 200, 2);
  mn_vert->AddFrame(ln2, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 10, 10));
  TGHorizontalFrame *CopyButton = new TGHorizontalFrame(mn_vert, 400, 300);
  mn_vert->AddFrame(CopyButton, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  TGLabel *Copy = new TGLabel(CopyButton, "Select channel #");

  chanCopy = new TGNumberEntry(CopyButton, 0, 4, MODNUMBER+1000, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3/*kNELLimitMinMax*/, 0, 3);
  chanCopy->SetButtonToNum(0);
  chanCopy->IsEditable();
  chanCopy->SetIntNumber(0);
  CopyButton->AddFrame(Copy, new TGLayoutHints(kLHintsCenterX, 5, 10, 3, 0));
  CopyButton->AddFrame(chanCopy, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 20, 0, 0));

  chanCopy->Associate(this);

  ////////////////////Copy button per se///////////////////
  TGTextButton *copyB = new TGTextButton(CopyButton, "C&opy", COPYBUTTON+1000);
  copyB->Associate(this);
  copyB->SetToolTipText("Copy the setup of the selected channel to all channels of the module", 0);
  CopyButton->AddFrame(copyB, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 20, 0, 0));


  chanNumber = 0;

  TGTextButton *adjustB = new TGTextButton(CopyButton, "&AdjustOffset",COPYBUTTON+2000);
  adjustB->Associate(this);
  adjustB->SetToolTipText(" Adjust the DC offset of this module automatically by module!");
  CopyButton->AddFrame(adjustB,new TGLayoutHints(kLHintsTop|kLHintsLeft,0,20,0,0));
  ///////////////////////////////////////////////////////////////////////
  MapSubwindows();
  Resize();			// resize to default size

  modNumber = 0;
  Load_Once = true;
  pol_temp = 0;
  gain_temp = 0;
  offset_temp = 0;
  blcut = 0;
  blpercent = 0;
  thresh = 0;
  tlength = 0;
  tdelay = 0;
  
}

Base::~Base()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


Bool_t Base::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
  switch (GET_MSG(msg))
    {
    case kC_COMMAND:
      switch (GET_SUBMSG(msg))
	{
	case kCM_BUTTON:
	  switch (parm1)
	    {
	    case (MODNUMBER):
	      if (parm2 == 0)
		{
		  if (modNumber != numModules-1)
		    {
		      ++modNumber;
		      numericMod->SetIntNumber(modNumber);
		      load_info(modNumber);
		    }
		}
	      else
		{
		  if (modNumber != 0)
		    {
		      if (--modNumber == 0)
			modNumber = 0;
		      numericMod->SetIntNumber(modNumber);
		      load_info(modNumber);
		    }
		}
	      break;
	      /////////////////////////////
	    case (MODNUMBER+1000):
	      if (parm2 == 0)
		{
		  if (chanNumber != 15)
		    {
		      ++chanNumber;
		      chanCopy->SetIntNumber(chanNumber);
		    }
		}
	      else
		{
		  if (chanNumber != 0)
		    {
		      if (--chanNumber == 0)
			chanNumber = 0;
		      chanCopy->SetIntNumber(chanNumber);
			   
		    }
		}
	      break;
	      /////////////////////////////////////
	    case LOAD:
	      {
		Load_Once = true;
		load_info(modNumber);
	      }
	      break;
	    case APPLY:
	      if (Load_Once)
		change_values(modNumber);
	      else
		std::cout << "please load once first !\n";
	      break;
	    case CANCEL:	/// Cancel Button
	      DeleteWindow();
	      break;
	    case (COPYBUTTON+1000):
	      pol_temp = lstBox[chanNumber]->GetSelected();
	      gain_temp = lstBoxGain[chanNumber]->GetSelected();
	      offset_temp = NumEntry[1][chanNumber]->GetNumber();
	      blcut = NumEntry[2][chanNumber]->GetNumber();
	      blpercent = NumEntry[3][chanNumber]->GetNumber();
	      thresh = NumEntry[4][chanNumber]->GetNumber();
	      tlength = NumEntry[5][chanNumber]->GetNumber();
	      tdelay = NumEntry[6][chanNumber]->GetNumber();
	      
	      for(int i  =0;i < 16;i++)
		{
		  if(i != (chanNumber))
		    {
		      lstBox[i]->Select(pol_temp);
		      lstBoxGain[i]->Select(gain_temp);
		      char tmp[10];
		      sprintf(tmp,"%1.3f",offset_temp);
		      NumEntry[1][i]->SetText(tmp);
		      sprintf(tmp,"%1.3f",blcut);
		      NumEntry[2][i]->SetText(tmp);	
		      sprintf(tmp,"%1.3f",blpercent);
		      NumEntry[3][i]->SetText(tmp);
		      sprintf(tmp,"%1.3f",thresh);
		      NumEntry[4][i]->SetText(tmp);
		      sprintf(tmp,"%1.3f",tlength);
		      NumEntry[5][i]->SetText(tmp);
		      sprintf(tmp,"%1.3f",tdelay);
		      NumEntry[6][i]->SetText(tmp);
		    }
		}  
		    
	      break;
	    case (COPYBUTTON+2000) :
	      {
		int retval = Pixie16AdjustOffsets(modNumber);
		if(retval < 0){
		  ErrorInfo("Base.cc", "ProcessMessage(...)", "Pixie16AdjustOffsets", retval);
		  break;
		}
		load_info(modNumber);
	      }
	      break;
	    default:
	      break;
	    }
	  break;
	default:
	  break;
	}
    // case kC_TEXTENTRY:
    //   switch (GET_SUBMSG(msg))
    // 	{
    // 	case kTE_TAB:
    // 	  if (parm1 < 16)
    // 	    NumEntry[3][parm1]->SetFocus();
    // 	  if (parm1 > 15 && parm1 < 32)
    // 	    {
    // 	      if ((parm1 - 16) + 1 < 16)
    // 		NumEntry[1][(parm1 - 16) + 1]->SetFocus();
    // 	    }
    // 	  break;
    // 	}
      break;
    default:
      break;
    }

  return kTRUE;
}


int Base::load_info(Long_t mod)
{
  double ChanParData = -1;

  int retval;
  char text[20];
  unsigned short gain;
  unsigned short pol;

  for (int i = 0; i < 16; i++)
    {
      retval = Pixie16ReadSglChanPar((char*)"CHANNEL_CSRA", &ChanParData, mod, i);
      if(retval < 0)
	ErrorInfo("Base.cc", "load_info(...)", "Pixie16ReadSglChanPar/CHANNEL_CSRA", retval);
      gain = APP32_TstBit(14, (unsigned int) ChanParData);
      if (gain == 1)
	lstBoxGain[i]->Select(0);
      else if (gain == 0)
	lstBoxGain[i]->Select(1);

      pol = APP32_TstBit(5, (unsigned int) ChanParData);
      if (pol == 1)
	lstBox[i]->Select(0);
      else if (pol == 0)
	lstBox[i]->Select(1);

      retval = Pixie16ReadSglChanPar((char*)"VOFFSET", &ChanParData, mod, i);
      if(retval < 0)
	ErrorInfo("Base.cc", "load_info(...)", "Pixie16ReadSglChanPar/VOFFSET", retval);
      sprintf(text, "%1.3f", ChanParData);
      NumEntry[1][i]->SetText(text);

      retval = Pixie16ReadSglChanPar((char*)"BLCUT", &ChanParData, mod, i);
      if(retval < 0) ErrorInfo("Base.cc", "load_info(...)", "Pixie16ReadSglChanPar/BLCUT", retval);  
      sprintf(text, "%d", (int)ChanParData);
      NumEntry[2][i]->SetText(text);

      retval = Pixie16ReadSglChanPar((char*)"BASELINE_PERCENT", &ChanParData, mod, i);
      if(retval < 0) ErrorInfo("Base.cc", "load_info(...)", "Pixie16ReadSglChanPar/BASELINE_PERCENT", retval);  
      sprintf(text, "%d", (int)ChanParData);
      NumEntry[3][i]->SetText(text);

      retval = Pixie16ReadSglChanPar((char*)"TRIGGER_THRESHOLD", &ChanParData, mod, i);
      if(retval < 0) ErrorInfo("Base.cc", "load_info(...)", "Pixie16ReadSglChanPar/TRIGGER_THRESHOLD", retval);
      sprintf(text, "%d", (int)ChanParData);
      NumEntry[4][i]->SetText(text);

      retval = Pixie16ReadSglChanPar((char*)"TRACE_LENGTH", &ChanParData, mod, i);
      if(retval < 0) ErrorInfo("PulseShape.cc", "load_info(...)", "Pixie16ReadSglChanPar/TRACE_LENGTH", retval);
      sprintf (text, "%1.2f", ChanParData);
      NumEntry[5][i]->SetText(text);

      retval = Pixie16ReadSglChanPar((char*)"TRACE_DELAY", &ChanParData, mod, i);
      if(retval < 0) ErrorInfo("PulseShape.cc", "load_info(...)", "Pixie16ReadSglChanPar/TRACE_DELAY", retval);
      sprintf(text, "%1.2f", ChanParData);
      NumEntry[6][i]->SetText(text);

      
    }
  return 1;
}

int Base::change_values(Long_t mod)
{
  int retval;
  double offset;
  double ChanParData = -1;
  double gain;
  int pol = 0;
  double cut;
  double percent;
  double thresh;
  double length;
  double delay;
  
  for (int i = 0; i < 16; i++)
    {
      offset = NumEntry[1][i]->GetNumber();

      retval = Pixie16WriteSglChanPar((char*)"VOFFSET", offset, mod, i);
      if(retval < 0) ErrorInfo("Base.cc", "change_values(...)", "Pixie16WriteSglChanPar/VOFFSET", retval);
      retval = Pixie16ReadSglChanPar((char*)"CHANNEL_CSRA", &ChanParData, mod, i);
      if(retval < 0) ErrorInfo("Base.cc", "change_values(...)", "Pixie16ReadSglChanPar/CHANNEL_CSRA", retval);     
      pol = lstBox[i]->GetSelected();
      if (pol == 0)
	{
	  ChanParData = APP32_SetBit(5, (unsigned int) ChanParData);
	  retval = Pixie16WriteSglChanPar((char*)"CHANNEL_CSRA", ChanParData, mod, i);
	  if(retval < 0) ErrorInfo("Base.cc", "change_values(...)", "Pixie16WriteSglChanPar/CHANNEL_CSRA", retval);   
	}
      else
	{
	  ChanParData = APP32_ClrBit(5, (unsigned int) ChanParData);
	  retval = Pixie16WriteSglChanPar((char*)"CHANNEL_CSRA", ChanParData, mod, i);
	  if(retval < 0) ErrorInfo("Base.cc", "change_values(...)", "Pixie16WriteSglChanPar/CHANNEL_CSRA", retval);   
	}

      retval = Pixie16ReadSglChanPar((char*)"CHANNEL_CSRA", &ChanParData, mod, i);
      if(retval < 0) ErrorInfo("Base.cc", "change_values(...)", "Pixie16ReadSglChanPar/CHANNEL_CSRA", retval);   
      gain = lstBoxGain[i]->GetSelected();
      if (gain == 1)
	{
	  ChanParData = APP32_ClrBit(14, (unsigned int) ChanParData);
	  retval = Pixie16WriteSglChanPar((char*)"CHANNEL_CSRA", ChanParData, mod, i);
	  if(retval < 0) ErrorInfo("Base.cc", "change_values(...)", "Pixie16WriteSglChanPar/CHANNEL_CSRA", retval);   
	}
      else
	{
	  ChanParData = APP32_SetBit(14, (unsigned int) ChanParData);
	  retval = Pixie16WriteSglChanPar((char*)"CHANNEL_CSRA", ChanParData, mod, i);
	  if(retval < 0) ErrorInfo("Base.cc", "change_values(...)", "Pixie16WriteSglChanPar/CHANNEL_CSRA", retval);   
	}


      cut = NumEntry[2][i]->GetNumber();
      retval = Pixie16WriteSglChanPar((char*)"BLCUT", cut, mod, i);
      if(retval < 0) ErrorInfo("Base.cc", "change_values(...)", "Pixie16WriteSglChanPar/BLCUT", retval);  
      percent = NumEntry[3][i]->GetNumber();
      retval = Pixie16WriteSglChanPar((char*)"BASELINE_PERCENT", percent, mod, i);
      if(retval < 0) ErrorInfo("Base.cc", "change_values(...)", "Pixie16WriteSglChanPar/BASELINE_PERCENT", retval);  

      thresh = NumEntry[4][i]->GetNumber();
      retval = Pixie16WriteSglChanPar((char*)"TRIGGER_THRESHOLD", thresh, mod, i);
      if(retval < 0) ErrorInfo("Base.cc", "change_values(...)", "Pixie16WriteSglChanPar/TRIGGER_THRESHOLD", retval);


      length = NumEntry[5][i]->GetNumber();
      retval = Pixie16WriteSglChanPar((char*)"TRACE_LENGTH", length, mod, i);
      if(retval < 0) ErrorInfo("Base.cc", "change_values(...)", "Pixie16WriteSglChanPar/TRACE_LENGTH", retval);
      delay = NumEntry[6][i]->GetNumber();
      retval = Pixie16WriteSglChanPar((char*)"TRACE_DELAY", delay, mod, i);
      if(retval < 0) ErrorInfo("Base.cc", "change_values(...)", "Pixie16WriteSglChanPar/TRACE_DELAY", retval);







      
    }
  return 1;
}














// 
// Base.cc ends here
