#include "Qdc.hh"
#include "Global.hh"
#include "Detector.hh"
#include "pixie16app_export.h"
#include "TColor.h"
#include <iostream>
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Qdc::Qdc(const TGWindow *p, const TGWindow *main, char *name, int columns, int rows,Detector *det)
  : Table(p,main,columns,rows,name, det->NumModules)
{
  detector = det;
  modNumber = 0;
  cl0->SetText("ch #");
  for(int i = 0; i < rows; i++)
    {
      Labels[i]->SetText(TString::Format("%2d",i).Data());
    }
  CLabel[0]->SetText("QDC len0[us]");
  CLabel[1]->SetText("QDC len1[us]");
  CLabel[2]->SetText("QDC len2[us]");
  CLabel[3]->SetText("QDC len3[us]");
  CLabel[4]->SetText("QDC len4[us]");
  CLabel[5]->SetText("QDC len5[us]");
  CLabel[6]->SetText("QDC len6[us]");
  CLabel[7]->SetText("QDC len7[us]");

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
  
  chanNumber = 0;
  MapSubwindows();
  Resize();// resize to default size

  Load_Once = true;
}


Qdc::~Qdc()
{

}

Bool_t Qdc::ProcessMessage(Long_t msg, Long_t parm1,Long_t parm2)
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
	      ////////////////////////////////////////
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

	      ////////////////////////////////////////
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
	      //////////////////////////////
	    case (COPYBUTTON+1000):
	      memset(qlen,0,sizeof(double)*8);
	      for(int i = 0; i < 8; i++)
		qlen[i]=NumEntry[i+1][chanNumber]->GetNumber();
	      for(int i = 0; i  < 16; i++)
		{
		  if(i != chanNumber)
		    {
		      for(int j = 0; j < 8; j++)
			{
			  NumEntry[j+1][i]->SetText(TString::Format("%1.3f",qlen[j]).Data());
			}
		    }
		}  
		    
	      break;
	      
	      /////////////////////////////

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

  PostFunction();
  return kTRUE;
}

int Qdc::change_values(Long_t mod)
{
  if(detector->GetRunFlag()) return 0;
  
  int retval;
  for(int i = 0; i < 16; i++)
    {
      for(int j = 0; j < 8; j++)
	{
	  qlen[j] = NumEntry[j+1][i]->GetNumber();
	  retval = Pixie16WriteSglChanPar((char*)TString::Format("QDCLen%d",j).Data(),qlen[j],mod,i);
	  if(retval<0)
	    {
	      ErrorInfo("Qdc.cc", "change_values(...)", "Pixie16WriteSglChanPar/QDCLen0-7", retval);
	      return retval;
	    }
	}
    }
  return 0;
}

int Qdc::load_info(Long_t mod)
{
  if(detector->GetRunFlag()) return 0;
  
  int retval;
  double ChanParData = -1;
  char varN[8][10] = {"QDCLen0","QDCLen1","QDCLen2","QDCLen3","QDCLen4","QDCLen5","QDCLen6","QDCLen7"};
  
  for(int i = 0; i < 16; i++)
    {
      for(int j = 0; j < 8; j++)
	{
	  retval = Pixie16ReadSglChanPar(varN[j],&ChanParData,mod,i);
	  if(retval < 0) ErrorInfo("Qdc.cc", "load_info(...)", "Pixie16ReadSglChanPar/QDCLen0-7", retval);
	  NumEntry[j+1][i]->SetText(TString::Format("%1.3f",ChanParData).Data());
	}
    }

  return 0;
}
