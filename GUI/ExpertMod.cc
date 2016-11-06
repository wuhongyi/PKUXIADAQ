#include "Global.hh"
#include "ExpertMod.hh"

#include "pixie16app_common.h"
#include "pixie16app_export.h"
#include "pixie16sys_export.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExpertMod::ExpertMod(const TGWindow * p, const TGWindow * main, char *name,int NumModules)
{
  SetCleanup(kDeepCleanup);

  numModules = NumModules;

  mn_vert = new TGVerticalFrame(this, 200, 300);

  mn = new TGHorizontalFrame(mn_vert, 200, 300);
  mn_vert->AddFrame(mn, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  AddFrame(mn_vert, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2));
  column1 = new TGVerticalFrame(mn, 200, 300);
  column2 = new TGVerticalFrame(mn, 200, 300);
  mn->AddFrame(column1, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  mn->AddFrame(column2, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  
  TGTextEntry *te = new TGTextEntry(column1, new TGTextBuffer(100), 10000,
				    te->GetDefaultGC()(),
				    te->GetDefaultFontStruct (),
				    kRaisedFrame | kDoubleBorder,
				    GetWhitePixel());
  te->SetText("Bit Name");
  te->Resize(100, 20);
  te->SetEnabled(kFALSE);
  te->SetFrameDrawn(kTRUE);
  column1->AddFrame(te, new TGLayoutHints(kLHintsCenterX, 0, 0, 10, 0));
  
  TGTextEntry *Labels[9];
  for (int i = 0; i < 9; i++)
    {
      Labels[i] = new TGTextEntry(column1, new TGTextBuffer(100), 10000,
				  Labels[i]->GetDefaultGC()(),
				  Labels[i]->GetDefaultFontStruct(),
				  kRaisedFrame | kDoubleBorder,
				  GetWhitePixel());
      Labels[i]->Resize(100, 20);
      Labels[i]->SetEnabled(kFALSE);
      Labels[i]->SetFrameDrawn(kTRUE);
      Labels[i]->SetAlignment(kTextCenterX);
      column1->AddFrame (Labels[i], new TGLayoutHints(kLHintsCenterX, 0, 3, 5, 0));
    }

  Labels[0]->SetText("CPLDPULLUP");
  Labels[0]->SetToolTipText("Control pullups for PXI trigger lines on the backplane", 0);
  Labels[1]->SetText("DIRMOD");
  Labels[1]->SetToolTipText("Set this module as the Director", 0);
  Labels[2]->SetText("CHASSISMASTER");
  Labels[2]->SetToolTipText("Set this module as chassis master", 0);
  Labels[3]->SetText("GFTSEL");
  Labels[3]->SetToolTipText("Select global fast trigger source (external validation trigger vs. external fast trigger, in case these two signals are swapped at the front panel)", 0);
  Labels[4]->SetText("ETSEL");
  Labels[4]->SetToolTipText("Select external trigger source (external fast trigger vs. external validation trigger, in case these two signals are swapped at the front panel)", 0);
  Labels[5]->SetText("INHIBITENA");
  Labels[5]->SetToolTipText("Control the use of external INHIBIT signal", 0);
  Labels[6]->SetText("MULTCRATES");
  Labels[6]->SetToolTipText("Distribute clock and triggers in multiple crates", 0);
  Labels[7]->SetText("SORTEVENTS");
  Labels[7]->SetToolTipText("Sort events based on their timestamps", 0);
  Labels[8]->SetText("BKPLFASTTRIG");
  Labels[8]->SetToolTipText("Enable connection of fast triggers to backplane (only one module can enable this option in each PCI bus segment of a crate)", 0);


  TGTextEntry *ra = new TGTextEntry(column2, new TGTextBuffer(100),
				    10000, ra->GetDefaultGC()(),
				    ra->GetDefaultFontStruct(),
				    kRaisedFrame | kDoubleBorder,
				    GetWhitePixel());
  ra->SetText("Control");
  ra->Resize(50, 20);
  ra->SetEnabled(kFALSE);
  ra->SetFrameDrawn(kTRUE);
  ra->SetAlignment(kTextCenterX);
  column2->AddFrame(ra, new TGLayoutHints(kLHintsCenterX, 0, 0, 10, 3));

  for (int i = 0; i < 9; i++)
    {
      column2->AddFrame(ckBtn[i] =
			new TGCheckButton(column2, "", 5000 + i),
			new TGLayoutHints(kLHintsCenterX, 0, 0, 5, 5));
      ckBtn[i]->Associate(this);
    }
  
  Buttons = new TGHorizontalFrame(mn_vert, 400, 300);
  
  /////////////////////////////module entry///////////////////////////////

  TGHorizontal3DLine *ln1 = new TGHorizontal3DLine(mn_vert, 150, 2);
  TGLabel *mod = new TGLabel(Buttons, "Module #");

  numericMod = new TGNumberEntry(Buttons, 0, 4, MODNUMBER, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3/*kNELLimitMinMax*/, 0, 3);
  numericMod->SetButtonToNum(0);
  numericMod->IsEditable();
  mn_vert->AddFrame(ln1, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 10, 10));
  Buttons->AddFrame(mod, new TGLayoutHints(kLHintsCenterX, 5, 10, 3, 0));
  Buttons->AddFrame(numericMod, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 20, 0, 0));

  numericMod->Associate(this);
  mn_vert->AddFrame(Buttons, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  ////////////////////////////Buttons/////////////////////////////////////
  
  TGTextButton *LoadButton = new TGTextButton(Buttons, "&Load", LOAD);
  LoadButton->Associate (this);
  TGTextButton *ApplyButton = new TGTextButton(Buttons, "&Apply", APPLY);
  ApplyButton->Associate (this);
  TGTextButton *CancelButton = new TGTextButton(Buttons, "&Cancel", CANCEL);
  CancelButton->Associate (this);
  Buttons->AddFrame(LoadButton, new TGLayoutHints(kLHintsCenterX, 0, 0, 0, 0));
  Buttons->AddFrame(ApplyButton, new TGLayoutHints(kLHintsCenterX, 0, 0, 0, 0));
  Buttons->AddFrame(CancelButton, new TGLayoutHints(kLHintsCenterX, 0, 0, 0, 0));
		     
  modNumber = 0;
  MapSubwindows();
  Resize();// resize to default size
  CenterOnParent();

  SetWindowName(name);

  MapWindow();
}

ExpertMod::~ExpertMod()
{
  
}

Bool_t ExpertMod::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
  switch (GET_MSG (msg))
    {
    case kC_COMMAND:
      switch (GET_SUBMSG (msg))
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

	    default:
	      break;
	    }
	  break;
	      
	default:
	  break;
	}
      break;
    default:
      break;
    }

  return kTRUE;
}

int ExpertMod::load_info(Long_t mod)
{
  int retval = 0;
  unsigned int ModParData = 0;
  unsigned short gt;
  
  retval = Pixie16ReadSglModPar((char*)"MODULE_CSRB", &ModParData, mod);
  if(retval < 0) ErrorInfo("ExpertMod.cc", "load_info(...)", "Pixie16ReadSglModPar/MODULE_CSRB", retval);

  for(int j = 0;j < 9;j++)
    {
      if(j == 0) gt = APP32_TstBit(0, ModParData);
      if(j == 1) gt = APP32_TstBit(4, ModParData);
      if(j == 2) gt = APP32_TstBit(6, ModParData);
      if(j == 3) gt = APP32_TstBit(7, ModParData);
      if(j == 4) gt = APP32_TstBit(8, ModParData);
      if(j == 5) gt = APP32_TstBit(10, ModParData);
      if(j == 6) gt = APP32_TstBit(11, ModParData);
      if(j == 7) gt = APP32_TstBit(12, ModParData);
      if(j == 8) gt = APP32_TstBit(13, ModParData);
      
      if(gt == 0) 
	ckBtn[j]->SetState(kButtonUp);
      else 
	ckBtn[j]->SetState(kButtonDown);
    }

  return retval;
}

int ExpertMod::change_values(Long_t mod)
{
  unsigned int ModParData = 0;
  int retval = 0;

  if(ckBtn[0]->IsDown())
    ModParData = APP32_SetBit(0,ModParData);
  else 
    ModParData = APP32_ClrBit(0,ModParData);

  if(ckBtn[1]->IsDown())
    ModParData = APP32_SetBit(4,ModParData);
  else 
    ModParData = APP32_ClrBit(4,ModParData);

  if(ckBtn[2]->IsDown())
    ModParData = APP32_SetBit(6,ModParData);
  else 
    ModParData = APP32_ClrBit(6,ModParData);

  if(ckBtn[3]->IsDown())
    ModParData = APP32_SetBit(7,ModParData);
  else 
    ModParData = APP32_ClrBit(7,ModParData);

  if(ckBtn[4]->IsDown())
    ModParData = APP32_SetBit(8,ModParData);
  else 
    ModParData = APP32_ClrBit(8,ModParData);

  if(ckBtn[5]->IsDown())
    ModParData = APP32_SetBit(10,ModParData);
  else 
    ModParData = APP32_ClrBit(10,ModParData);

  if(ckBtn[6]->IsDown())
    ModParData = APP32_SetBit(11,ModParData);
  else 
    ModParData = APP32_ClrBit(11,ModParData);

  if(ckBtn[7]->IsDown())
    ModParData = APP32_SetBit(12,ModParData);
  else 
    ModParData = APP32_ClrBit(12,ModParData);

  if(ckBtn[8]->IsDown())
    ModParData = APP32_SetBit(13,ModParData);
  else 
    ModParData = APP32_ClrBit(13,ModParData);
  
  retval = Pixie16WriteSglModPar((char*)"MODULE_CSRB",ModParData,mod);    
  if(retval < 0) ErrorInfo("ExpertMod.cc", "change_values(...)", "Pixie16ReadSglModPar/MODULE_CSRB", retval);
  
  return retval;
  
}
