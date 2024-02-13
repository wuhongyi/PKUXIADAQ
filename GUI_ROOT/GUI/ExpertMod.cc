#include "Global.hh"
#include "ExpertMod.hh"
#include "Detector.hh"
#include "pixie16app_common.h"
#include "pixie16app_export.h"
#include "pixie16sys_export.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExpertMod::ExpertMod(const TGWindow * p, const TGWindow * main, char *name,Detector *det)
{
  SetCleanup(kDeepCleanup);
  detector = det;
  numModules = detector->NumModules;

  SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));


  mn_vert = new TGVerticalFrame(this, 200, 300);
  AddFrame(mn_vert, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2));
  mn_vert->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  

  mn = new TGHorizontalFrame(mn_vert, 200, 300);
  mn_vert->AddFrame(mn, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  mn->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  
  column1 = new TGVerticalFrame(mn, 200, 300);
  mn->AddFrame(column1, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  column1->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  TGTextEntry *te = new TGTextEntry(column1, new TGTextBuffer(100), 10000,
				    te->GetDefaultGC()(),
				    te->GetDefaultFontStruct (),
				    kRaisedFrame | kDoubleBorder,
				    GetWhitePixel());
  column1->AddFrame(te, new TGLayoutHints(kLHintsCenterX, 0, 0, 5, 0));
  te->SetText("Bit Name");
  te->Resize(100, 20);
  te->SetEnabled(kFALSE);
  te->SetFrameDrawn(kFALSE);
  te->ChangeBackground(TColor::RGB2Pixel(TABLE_LABELTITLE_BG_R,TABLE_LABELTITLE_BG_G,TABLE_LABELTITLE_BG_B));
  te->SetTextColor(TColor::RGB2Pixel(TABLE_LABELTITLE_TEXT_R,TABLE_LABELTITLE_TEXT_G,TABLE_LABELTITLE_TEXT_B), false);
  
  TGTextEntry *Labels[9];
  for (int i = 0; i < 9; i++)
    {
      Labels[i] = new TGTextEntry(column1, new TGTextBuffer(100), 10000,
				  Labels[i]->GetDefaultGC()(),
				  Labels[i]->GetDefaultFontStruct(),
				  kRaisedFrame | kDoubleBorder,
				  GetWhitePixel());
      column1->AddFrame(Labels[i],new TGLayoutHints(kLHintsCenterX, 0, 3, 5, 0));
      Labels[i]->Resize(100, 20);
      Labels[i]->SetEnabled(kFALSE);
      Labels[i]->SetFrameDrawn(kFALSE);
      Labels[i]->SetAlignment(kTextCenterX);
      Labels[i]->ChangeBackground(TColor::RGB2Pixel(TABLE_LABELTITLE_BG_R,TABLE_LABELTITLE_BG_G,TABLE_LABELTITLE_BG_B));
      Labels[i]->SetTextColor(TColor::RGB2Pixel(TABLE_LABELTITLE_TEXT_R,TABLE_LABELTITLE_TEXT_G,TABLE_LABELTITLE_TEXT_B), false);
    }

  Labels[0]->SetText("CPLDPULLUP[0]");
  Labels[0]->SetToolTipText("Control pullups for PXI trigger lines on the backplane", 0);
  Labels[1]->SetText("DIRMOD[4]");
  Labels[1]->SetToolTipText("Set this module as the Director", 0);
  Labels[1]->SetTextColor(TColor::RGB2Pixel(COLOR_PINK_R,COLOR_PINK_G,COLOR_PINK_B), false);
  Labels[2]->SetText("CHASSISMASTER[6]");
  Labels[2]->SetToolTipText("Set this module as chassis master", 0);
  Labels[2]->SetTextColor(TColor::RGB2Pixel(COLOR_PURPLE_R,COLOR_PURPLE_G,COLOR_PURPLE_B), false);
  Labels[3]->SetText("GFTSEL[7]");
  Labels[3]->SetToolTipText("Select global fast trigger source (external validation trigger vs. external fast trigger, in case these two signals are swapped at the front panel)", 0);
  Labels[3]->SetTextColor(TColor::RGB2Pixel(COLOR_BLUE_R,COLOR_BLUE_G,COLOR_BLUE_B), false);
  Labels[4]->SetText("ETSEL[8]");
  Labels[4]->SetToolTipText("Select external trigger source (external fast trigger vs. external validation trigger, in case these two signals are swapped at the front panel)", 0);
  Labels[4]->SetTextColor(TColor::RGB2Pixel(COLOR_BLUE_R,COLOR_BLUE_G,COLOR_BLUE_B), false);
  Labels[5]->SetText("INHIBITENA[10]");
  Labels[5]->SetToolTipText("Control the use of external INHIBIT signal", 0);
  Labels[5]->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
  Labels[6]->SetText("MULTCRATES[11]");
  Labels[6]->SetToolTipText("Distribute clock and triggers in multiple crates", 0);
  Labels[6]->SetTextColor(TColor::RGB2Pixel(COLOR_YELLOW_R,COLOR_YELLOW_G,COLOR_YELLOW_B), false);
  Labels[7]->SetText("SORTEVENTS[12]");
  Labels[7]->SetToolTipText("Sort events based on their timestamps", 0);
  Labels[8]->SetText("BKPLFASTTRIG[13]");
  Labels[8]->SetToolTipText("Enable connection of fast triggers to backplane (only one module can enable this option in each PCI bus segment of a crate)", 0);

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  
  column2 = new TGVerticalFrame(mn, 200, 300);
  mn->AddFrame(column2, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  column2->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  TGTextEntry *ra = new TGTextEntry(column2, new TGTextBuffer(100),
				    10000, ra->GetDefaultGC()(),
				    ra->GetDefaultFontStruct(),
				    kRaisedFrame | kDoubleBorder,
				    GetWhitePixel());
  column2->AddFrame(ra, new TGLayoutHints(kLHintsCenterX, 0, 0, 5, 3));
  ra->SetText("Control");
  ra->Resize(50, 20);
  ra->SetEnabled(kFALSE);
  ra->SetFrameDrawn(kFALSE);
  ra->SetAlignment(kTextCenterX);
  ra->ChangeBackground(TColor::RGB2Pixel(TABLE_LABELTITLE_BG_R,TABLE_LABELTITLE_BG_G,TABLE_LABELTITLE_BG_B));
  ra->SetTextColor(TColor::RGB2Pixel(TABLE_LABELTITLE_TEXT_R,TABLE_LABELTITLE_TEXT_G,TABLE_LABELTITLE_TEXT_B), false);

  
  for (int i = 0; i < 9; i++)
    {
      column2->AddFrame(ckBtn[i] =
			new TGCheckButton(column2, "", 5000 + i),
			new TGLayoutHints(kLHintsCenterX, 0, 0, 5, 5));
      ckBtn[i]->Associate(this);
    }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    
  column3 = new TGVerticalFrame(mn, 200, 300);
  mn->AddFrame(column3, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  column3->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  TGTextEntry *LabelsNotes[10];
  for (int i = 0; i < 10; i++)
    {
      LabelsNotes[i] = new TGTextEntry(column3, new TGTextBuffer(100), 10000,
				  LabelsNotes[i]->GetDefaultGC()(),
				  LabelsNotes[i]->GetDefaultFontStruct(),
				  kRaisedFrame | kDoubleBorder,
				  GetWhitePixel());
      column3->AddFrame(LabelsNotes[i],new TGLayoutHints(kLHintsCenterX, 0, 3, 5, 0));
      LabelsNotes[i]->Resize(90, 20);
      LabelsNotes[i]->SetEnabled(kFALSE);
      LabelsNotes[i]->SetFrameDrawn(kFALSE);
      LabelsNotes[i]->SetAlignment(kTextCenterX);
      LabelsNotes[i]->ChangeBackground(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
      LabelsNotes[i]->SetTextColor(TColor::RGB2Pixel(TABLE_LABELTITLE_TEXT_R,TABLE_LABELTITLE_TEXT_G,TABLE_LABELTITLE_TEXT_B), false);
    }
  LabelsNotes[1]->SetText("Tip");
  LabelsNotes[2]->SetText("Tip");
  LabelsNotes[3]->SetText("Tip");
  LabelsNotes[6]->SetText("Tip");
  LabelsNotes[1]->SetToolTipText("Keep the select module 0. Only 1 module can select per crate.", 0);
  LabelsNotes[2]->SetToolTipText("Only 1 module can select in all crates. Send its Ext_FastTrig_In/Ext_ValidTrig_In to all crates for Module Fast/Validation Trigger.", 0);
  LabelsNotes[3]->SetToolTipText("Only 1 module can select per crate. Send its Ext_FastTrig_In/Ext_ValidTrig_In to the crate backplane for Module Fast/Validation Trigger.", 0);
  LabelsNotes[6]->SetToolTipText("Just choose 1 module and singal from it. When 'is was selected and front panel signal in high level', run inhibit", 0);
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  TGHorizontal3DLine *ln1 = new TGHorizontal3DLine(mn_vert, 150, 2);
  mn_vert->AddFrame(ln1, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 10, 10));
  
  Buttons = new TGHorizontalFrame(mn_vert, 400, 300);
  mn_vert->AddFrame(Buttons, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  Buttons->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));


  TGLabel *mod = new TGLabel(Buttons, "Module #");
  Buttons->AddFrame(mod, new TGLayoutHints(kLHintsCenterX, 5, 10, 3, 0));
  mod->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  mod->SetTextColor(TColor::RGB2Pixel(TABLE_LABEL_TEXT_R,TABLE_LABEL_TEXT_G,TABLE_LABEL_TEXT_B));
  
  numericMod = new TGNumberEntry(Buttons, 0, 4, MODNUMBER, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3/*kNELLimitMinMax*/, 0, 3);
  Buttons->AddFrame(numericMod, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 20, 0, 0));
  numericMod->SetButtonToNum(0);
  numericMod->IsEditable();
  numericMod->Associate(this);
  numericMod->GetNumberEntry()->ChangeOptions(numericMod->GetNumberEntry()->GetOptions() ^ kRaisedFrame);
  numericMod->GetNumberEntry()->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
  numericMod->GetButtonUp()->ChangeOptions(numericMod->GetButtonUp()->GetOptions() ^ kRaisedFrame);
  numericMod->GetButtonDown()->ChangeOptions(numericMod->GetButtonDown()->GetOptions() ^ kRaisedFrame);
  numericMod->ChangeSubframesBackground(TColor::RGB2Pixel(TEXTENTRY_BG_R,TEXTENTRY_BG_G,TEXTENTRY_BG_B));  

  TGTextButton *LoadButton = new TGTextButton(Buttons, "&Load", LOAD);
  Buttons->AddFrame(LoadButton, new TGLayoutHints(kLHintsCenterX, 0, 5, 0, 0));
  LoadButton->Associate (this);
  LoadButton->ChangeOptions(LoadButton->GetOptions() ^ kRaisedFrame);
  LoadButton->SetFont(TEXTBUTTON_FONT, false);
  LoadButton->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  LoadButton->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));
  
  TGTextButton *ApplyButton = new TGTextButton(Buttons, "&Apply", APPLY);
  Buttons->AddFrame(ApplyButton, new TGLayoutHints(kLHintsCenterX, 0, 5, 0, 0));
  ApplyButton->Associate (this);
  ApplyButton->ChangeOptions(ApplyButton->GetOptions() ^ kRaisedFrame);
  ApplyButton->SetFont(TEXTBUTTON_FONT, false);
  ApplyButton->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  ApplyButton->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));
  
  TGTextButton *CancelButton = new TGTextButton(Buttons, "&Cancel", CANCEL);
  Buttons->AddFrame(CancelButton, new TGLayoutHints(kLHintsCenterX, 0, 5, 0, 0));
  CancelButton->Associate (this);
  CancelButton->ChangeOptions(CancelButton->GetOptions() ^ kRaisedFrame);
  CancelButton->SetFont(TEXTBUTTON_FONT, false);
  CancelButton->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  CancelButton->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
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
		{
		  change_values(modNumber);
		  load_info(modNumber);
		}		  
	      else
		std::cout << "please load once first !"<<std::endl;
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
  if(detector->GetRunFlag()) return 0;
  
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
  if(detector->GetRunFlag()) return 0;
  
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
