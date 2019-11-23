// HistXDT.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 3月  8 13:33:01 2018 (+0800)
// Last-Updated: 六 11月 23 13:55:20 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 16
// URL: http://wuhongyi.cn 

#include "HistXDT.hh"
#include "Global.hh"
#include "Detector.hh"
#include "pixie16app_export.h"
#include "TColor.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistXDT::HistXDT(const TGWindow *p, const TGWindow *main,  char *name, int columns, int rows, Detector *det)
  :Table(p,main,columns,rows,name, det->NumModules)
{
  detector = det;
  
  cl0->SetText("ch #");
  for(int i = 0; i < rows; i++)
    {
      Labels[i]->SetText(TString::Format("%2d",i).Data());
    }
  CLabel[0]->SetText("EMin [ADC u]");
  CLabel[0]->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);  
  CLabel[1]->SetText("BinFactor");
  CLabel[1]->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);  
  CLabel[2]->SetText("dT [us]");
  CLabel[2]->SetTextColor(TColor::RGB2Pixel(COLOR_BLUE_R,COLOR_BLUE_G,COLOR_BLUE_B), false);  
	
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGHorizontal3DLine *ln2 = new TGHorizontal3DLine(mn_vert, 200, 2);
  mn_vert->AddFrame(ln2, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 10, 10));
  
  TGHorizontalFrame *CopyButton = new TGHorizontalFrame(mn_vert, 400, 300);
  mn_vert->AddFrame(CopyButton, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  CopyButton->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  TGLabel *Copy = new TGLabel(CopyButton, "Select channel #");
  CopyButton->AddFrame(Copy, new TGLayoutHints(kLHintsCenterX, 5, 10, 5, 0));
  Copy->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  Copy->SetTextColor(TColor::RGB2Pixel(TABLE_LABEL_TEXT_R,TABLE_LABEL_TEXT_G,TABLE_LABEL_TEXT_B));
  
  chanCopy = new TGNumberEntry(CopyButton, 0, 4, MODNUMBER+1000, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3/*kNELLimitMinMax*/, 0, 3);
  CopyButton->AddFrame(chanCopy, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 20, 3, 0));
  chanCopy->SetButtonToNum(0);
  chanCopy->IsEditable();
  chanCopy->SetIntNumber(0);
  chanCopy->Associate(this);
  chanCopy->GetNumberEntry()->ChangeOptions(chanCopy->GetNumberEntry()->GetOptions() ^ kRaisedFrame);
  chanCopy->GetNumberEntry()->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
  chanCopy->GetButtonUp()->ChangeOptions(chanCopy->GetButtonUp()->GetOptions() ^ kRaisedFrame);
  chanCopy->GetButtonDown()->ChangeOptions(chanCopy->GetButtonDown()->GetOptions() ^ kRaisedFrame);
  chanCopy->ChangeSubframesBackground(TColor::RGB2Pixel(TEXTENTRY_BG_R,TEXTENTRY_BG_G,TEXTENTRY_BG_B));


  TGTextButton *copyB = new TGTextButton(CopyButton, "C&opy", COPYBUTTON+1000);
  CopyButton->AddFrame(copyB, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 20, 0, 0));
  copyB->Associate(this);
  copyB->SetToolTipText("Copy the setup of the selected channel to all channels of the module", 0);
  copyB->ChangeOptions(copyB->GetOptions() ^ kRaisedFrame);
  copyB->SetFont(TEXTBUTTON_FONT, false);
  copyB->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  copyB->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));
  

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  MapSubwindows();
  Resize();// resize to default size
	
  modNumber = 0;
  chanNumber = 0;
  Load_Once = true;
  emin = 0;
  bin = 0;
  decay = 0;
}

HistXDT::~HistXDT()
{
  
}

Bool_t HistXDT::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
  PreFunction();
  
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
	      //////////////////////////////
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
	      
	      ////////////////////
	      
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
	    case CANCEL:		/// Cancel Button
	      DeleteWindow();
	      break;
	      /////////////////////////////
	    case (COPYBUTTON+1000):
	      emin = NumEntry[1][chanNumber]->GetNumber();
	      bin = NumEntry[2][chanNumber]->GetNumber();
	      decay = NumEntry[3][chanNumber]->GetNumber();
	      for(int i = 0; i < 16; i++)
		{
		  if(i != chanNumber)
		    {
		      NumEntry[1][i]->SetText(TString::Format("%1.3f",emin).Data());
		      NumEntry[2][i]->SetText(TString::Format("%1.3f",bin).Data());
		      NumEntry[3][i]->SetText(TString::Format("%1.3f",decay).Data());
		    }
		}   
	      break;
	      
	      ///////////////////////
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
    // 	    NumEntry[2][parm1]->SetFocus();
    // 	  if (parm1 > 15 && parm1 < 32)
    // 	    {
    // 	      if ((parm1 - 16) + 1 < 16)
    // 		NumEntry[1][(parm1 - 16) + 1]->SetFocus();
    // 	    }
    // 	  break;
    // 	}
    //   break;
      
    default:
      break;
    }

  PostFunction();
  return kTRUE;
}


int HistXDT::load_info(Long_t mod)
{
  if(detector->GetRunFlag()) return 1;
  
  double ChanParData = -1;
  int retval;

  for (int i = 0; i < 16; i++)
    {
      retval = Pixie16ReadSglChanPar((char*)"EMIN", &ChanParData, mod, i);
      if(retval < 0) ErrorInfo("HistXDT.cc", "load_info(...)", "Pixie16ReadSglChanPar/EMIN", retval);
      NumEntry[1][i]->SetText(TString::Format("%d", (int)ChanParData).Data());

      retval = Pixie16ReadSglChanPar((char*)"BINFACTOR", &ChanParData, mod, i);
      if(retval < 0) ErrorInfo("HistXDT.cc", "load_info(...)", "Pixie16ReadSglChanPar/BINFACTOR", retval);
      NumEntry[2][i]->SetText(TString::Format("%d", (int)ChanParData).Data());

      retval = Pixie16ReadSglChanPar((char*)"XDT", &ChanParData, mod, i);
      if(retval < 0) ErrorInfo("HistXDT.cc", "load_info(...)", "Pixie16ReadSglChanPar/XDT", retval);
      NumEntry[3][i]->SetText(TString::Format("%1.2f", ChanParData).Data());
    }
  //  std::cout << "loading info for module " << module << std::endl;

  return 1;
}

int HistXDT::change_values(Long_t mod)
{
  if(detector->GetRunFlag()) return 1;
  
  double cut;
  int retval;
  double percent;
  double d;
  for (int i = 0; i < 16; i++)
    {
      cut = NumEntry[1][i]->GetNumber();
      retval = Pixie16WriteSglChanPar((char*)"EMIN", cut, mod, i);
      if(retval < 0) ErrorInfo("HistXDT.cc", "change_values(...)", "Pixie16WriteSglChanPar/EMIN", retval);
      
      percent = NumEntry[2][i]->GetNumber();
      retval = Pixie16WriteSglChanPar((char*)"BINFACTOR", percent, mod, i);
      if(retval < 0) ErrorInfo("HistXDT.cc", "change_values(...)", "Pixie16WriteSglChanPar/BINFACTOR", retval);

      d = NumEntry[3][i]->GetNumber();
      retval = Pixie16WriteSglChanPar((char*)"XDT", d, mod, i);
      if(retval < 0) ErrorInfo("HistXDT.cc", "change_values(...)", "Pixie16WriteSglChanPar/XDT", retval);
    }

  return 1;
}


// 
// HistXDT.cc ends here
