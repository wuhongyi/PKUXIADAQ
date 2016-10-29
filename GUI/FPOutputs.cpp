// FPOutputs.cpp --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 7月 29 10:48:22 2016 (+0800)
// Last-Updated: 三 10月  5 18:04:38 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 32
// URL: http://wuhongyi.cn 

#include "FPOutputs.h"
#include "Global.h"
#include "pixie16app_export.h"
#include <iostream>
using namespace std;

FPOutputs::FPOutputs(const TGWindow * p, const TGWindow * main, int NumModules)
  :TGTransientFrame(p, main, 10, 10, kHorizontalFrame)
{
  SetCleanup(kDeepCleanup);
  
  numModules = NumModules;
  modNumber = 0;
  Load_Once = true;
  
  mn_vert = new TGVerticalFrame(this, 200, 300);
  mn = new TGHorizontalFrame(mn_vert, 200, 300);
  mn_vert->AddFrame(mn, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  AddFrame(mn_vert, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2));

  buttons = new TGHorizontalFrame(mn_vert, 400, 300);

  char tempname[16];
  
  EnableDisableOfTestSignals = new TGComboBox(mn_vert);
  mn_vert->AddFrame(EnableDisableOfTestSignals, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  EnableDisableOfTestSignals->Resize(100, 20);
  EnableDisableOfTestSignals->AddEntry("Disable", 1);
  EnableDisableOfTestSignals->AddEntry("Enable", 2);
  EnableDisableOfTestSignals->Select(1);

  GroupOfTestSignals = new TGComboBox(mn_vert);
  mn_vert->AddFrame(GroupOfTestSignals, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  GroupOfTestSignals->Resize(100, 20);
  GroupOfTestSignals->AddEntry("Group 000", 1);
  GroupOfTestSignals->AddEntry("Group 001", 2);
  GroupOfTestSignals->Select(1);

  ChannelOfTestSignals = new TGComboBox(mn_vert);
  mn_vert->AddFrame(ChannelOfTestSignals, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  ChannelOfTestSignals->Resize(100, 20);
  for (int i = 0; i < 16; ++i)
    {
      sprintf(tempname,"Ch %02d",i);
      ChannelOfTestSignals->AddEntry(tempname, i+1);
    }
  ChannelOfTestSignals->Select(1);

  TestSignals = new TGComboBox(mn_vert);
  mn_vert->AddFrame(TestSignals, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  TestSignals->Resize(100, 20);
  for (int i = 0; i < 16; ++i)
    {
      sprintf(tempname,"TestSig %02d",i);
      TestSignals->AddEntry(tempname, i+1);
    }
  TestSignals->Select(1);
  
  column1 = new TGVerticalFrame(mn, 200, 300);
  mn->AddFrame(column1, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  
  // TGHorizontal3DLine *ln1 = new TGHorizontal3DLine(column1, 50, 2);
  TGLabel *mod = new TGLabel(buttons, "Module #");

  numericMod = new TGNumberEntry(buttons, 0, 4, 100, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3/*kNELLimitMinMax*/, 0, 3);
  numericMod->SetButtonToNum(0);

  // column1->AddFrame(ln1, new TGLayoutHints (kLHintsCenterX | kLHintsCenterY, 0, 0, 10, 10));
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
  SetWindowName("Front Panel Outputs");

  MapWindow();
}

FPOutputs::~FPOutputs()
{

}

Bool_t FPOutputs::ProcessMessage(Long_t msg, Long_t parm1,Long_t parm2)
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
	    case 4000:
	      {
		Load_Once = true;
		load_info(modNumber);
	      }
	      break;
	    case 4001:
	      if (Load_Once)
		change_values(modNumber);
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
	    // checkbutton(parm1);
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

int FPOutputs::load_info(Long_t mod)
{
  unsigned int ModParData;
 
  int retval;
  unsigned short gt;

  retval = Pixie16ReadSglModPar((char*)"TrigConfig0", &ModParData, mod);
  if(retval < 0) ErrorInfo("FPOutputs.cpp", "load_info(...)", "Pixie16ReadSglChanPar/TrigConfig0", retval);
  gt = APP32_TstBit(15, ModParData);
  EnableDisableOfTestSignals->Select(gt+1);

  GroupOfTestSignals->Select((APP32_TstBit(14, ModParData) << 2) + (APP32_TstBit(13, ModParData) << 1) + APP32_TstBit(12, ModParData) + 1);
  
  ChannelOfTestSignals->Select((APP32_TstBit(19, ModParData) << 3) + (APP32_TstBit(18, ModParData) << 2) + (APP32_TstBit(17, ModParData) << 1) + APP32_TstBit(16, ModParData) + 1);

  TestSignals->Select((APP32_TstBit(23, ModParData) << 3) + (APP32_TstBit(22, ModParData) << 2) + (APP32_TstBit(21, ModParData) << 1) + APP32_TstBit(20, ModParData) + 1);
  
  return 1;
}


int FPOutputs::change_values(Long_t mod)
{
  
  unsigned int ModParData;
  int retval;
  retval = Pixie16ReadSglModPar((char*)"TrigConfig0", &ModParData, mod);
  if(retval < 0) ErrorInfo("FPOutputs.cpp", "change_values(...)", "Pixie16ReadSglModPar/TrigConfig0", retval);

  if(EnableDisableOfTestSignals->GetSelected() == 1)
    ModParData = APP32_ClrBit(15, ModParData);
  else
    ModParData = APP32_SetBit(15, ModParData);

  
  if((GroupOfTestSignals->GetSelected()-1) >> 2)
    ModParData = APP32_SetBit(14, ModParData);
  else
    ModParData = APP32_ClrBit(14, ModParData);

  if(((GroupOfTestSignals->GetSelected()-1)&2) >> 1)
    ModParData = APP32_SetBit(13, ModParData);
  else
    ModParData = APP32_ClrBit(13, ModParData);

  if((GroupOfTestSignals->GetSelected()-1)&1)
    ModParData = APP32_SetBit(12, ModParData);
  else
    ModParData = APP32_ClrBit(12, ModParData);  


  if((ChannelOfTestSignals->GetSelected()-1) >> 3)
    ModParData = APP32_SetBit(19, ModParData);
  else
    ModParData = APP32_ClrBit(19, ModParData);

  if(((ChannelOfTestSignals->GetSelected()-1)&4) >> 2)
    ModParData = APP32_SetBit(18, ModParData);
  else
    ModParData = APP32_ClrBit(18, ModParData);

  if(((ChannelOfTestSignals->GetSelected()-1)&2) >> 1)
    ModParData = APP32_SetBit(17, ModParData);
  else
    ModParData = APP32_ClrBit(17, ModParData);

  if((ChannelOfTestSignals->GetSelected()-1)&1)
    ModParData = APP32_SetBit(16, ModParData);
  else
    ModParData = APP32_ClrBit(16, ModParData);

  //
  if((TestSignals->GetSelected()-1) >> 3)
    ModParData = APP32_SetBit(23, ModParData);
  else
    ModParData = APP32_ClrBit(23, ModParData);

  if(((TestSignals->GetSelected()-1)&4) >> 2)
    ModParData = APP32_SetBit(22, ModParData);
  else
    ModParData = APP32_ClrBit(22, ModParData);

  if(((TestSignals->GetSelected()-1)&2) >> 1)
    ModParData = APP32_SetBit(21, ModParData);
  else
    ModParData = APP32_ClrBit(21, ModParData);

  if((TestSignals->GetSelected()-1)&1)
    ModParData = APP32_SetBit(20, ModParData);
  else
    ModParData = APP32_ClrBit(20, ModParData);
  
  retval = Pixie16WriteSglModPar((char*)"TrigConfig0", ModParData, mod);
  if(retval < 0) ErrorInfo("FPOutputs.cpp", "change_values(...)", "Pixie16WriteSglModPar/TrigConfig0", retval);

  load_info(mod);

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  // unsigned int test = 0xFFFFFFFF;
  // Pixie16WriteSglChanPar((char*)"MultiplicityMaskL", test, modNumber, 2);
  // Pixie16WriteSglChanPar((char*)"MultiplicityMaskL", test, modNumber, 4);


  return 1;

}


// 
// FPOutputs.cpp ends here
