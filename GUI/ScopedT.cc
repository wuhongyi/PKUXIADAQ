#include "ScopedT.hh"
#include "Global.hh"

#include "pixie16app_export.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ScopedT::~ScopedT()
{
}

ScopedT::ScopedT(const TGWindow * p, const TGWindow * main, char *name, int columns, int rows, int NumModules)
  :Table(p, main, columns, rows, name, NumModules)
{
  char n[10];
  cl0->SetText ("ch #");
  for (int i = 0; i < rows; i++)
    {
      sprintf (n, "%2d", i);
      Labels[i]->SetText(n);
    }
  CLabel[0]->SetText("dT [us]");
  CLabel[0]->SetAlignment(kTextCenterX);

  ////////////////Copy Button//////////////////////////////////////////////
  
  TGHorizontal3DLine *ln2 = new TGHorizontal3DLine(mn_vert, 200, 2);
  mn_vert->AddFrame(ln2, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 10, 10));
  TGHorizontalFrame *CopyButton = new TGHorizontalFrame(mn_vert, 400, 300);
  mn_vert->AddFrame(CopyButton, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  TGLabel *Copy = new TGLabel(CopyButton, "Select channel #");

  chanCopy = new TGNumberEntry(CopyButton, 0, 4, MODNUMBER + 1000, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3/*kNELLimitMinMax*/, 0, 3);
  chanCopy->SetButtonToNum(0);
  chanCopy->IsEditable();
  chanCopy->SetIntNumber(0);
  CopyButton->AddFrame(Copy, new TGLayoutHints(kLHintsCenterX, 5, 10, 3, 0));
  CopyButton->AddFrame(chanCopy, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 20, 0, 0));

  chanCopy->Associate(this);

  ////////////////////Copy button per se///////////////////
  
  TGTextButton *copyB = new TGTextButton(CopyButton, "C&opy", COPYBUTTON + 1000);
  copyB->Associate(this);
  copyB->SetToolTipText("Copy the setup of the selected channel to all channels of the module", 0);
  CopyButton->AddFrame(copyB, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 20, 0, 0));

  modNumber = 0;
  chanNumber = 0;
  decay = 0;
  
  ///////////////////////////////////////////////////////////////////////
  
  MapSubwindows();
  Resize();			// resize to default size
}

Bool_t ScopedT::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
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
	      ////////////////////////////////////////
	    case (MODNUMBER + 1000):
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
		change_values(modNumber);
	      else
		std::cout << "please load once first !\n";
	      break;
	    case CANCEL:	/// Cancel Button
	      DeleteWindow();
	      break;
	      //////////////////////////////
	    case (COPYBUTTON + 1000):
	      decay = NumEntry[1][chanNumber]->GetNumber();
	      for (int i = 0; i < 16; i++)
		{
		  if (i != (chanNumber))
		    {

		      sprintf (tmp, "%1.3f", decay);

		      NumEntry[1][i]->SetText(tmp);

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

  return kTRUE;
}

int ScopedT::load_info(Long_t mod)
{
  double ChanParData = -1;
  int retval;
  char text[20];
  for (int i = 0; i < 16; i++)
    {
      retval = Pixie16ReadSglChanPar((char*)"XDT", &ChanParData, mod, i);
      if(retval < 0) ErrorInfo("ScopedT.cc", "load_info(...)", "Pixie16ReadSglChanPar/XDT", retval);
      sprintf(text, "%1.2f", ChanParData);
      NumEntry[1][i]->SetText(text);
    }

  return 1;
}

int
ScopedT::change_values(Long_t mod)
{
  int retval;
  double d;
  for (int i = 0; i < 16; i++)
    {
      d = NumEntry[1][i]->GetNumber();
      retval = Pixie16WriteSglChanPar((char*)"XDT", d, mod, i);
      if(retval < 0) ErrorInfo("ScopedT.cc", "change_values(...)", "Pixie16WriteSglChanPar/XDT", retval);
    }
  // cout << "change values\n";
  return 1;
}
