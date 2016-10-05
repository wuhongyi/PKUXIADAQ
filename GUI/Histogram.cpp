#include "Histogram.h"
#include "pixie16app_export.h"
#include "Global.h"

Histogram::Histogram(const TGWindow * p, const TGWindow * main,  char *name,int columns,
		     int rows, int NumModules)
  :Table(p,main,columns,rows,name, NumModules)
{
  char n[10];
  cl0->SetText("ch #");
  for(int i  =0;i < rows;i++)
    {
      sprintf(n,"%2d",i);
      Labels[i]->SetText(n);
    }
  CLabel[0]->SetText("EMin [ADC u]");
  CLabel[0]->SetAlignment(kTextCenterX);
  CLabel[1]->SetText("BinFactor");
  CLabel[1]->SetAlignment(kTextCenterX);
	
	
  ////////////////////////Copy button//////////////////////////
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

  MapSubwindows();
  Resize();			// resize to default size
	
  modNumber = 0;
  Load_Once = true;
  emin = 0;
  bin = 0;
}

Histogram::~Histogram()
{
  
}

Bool_t Histogram::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
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
		}
	      else
		std::cout << "please load once first !\n";
	      break;
	    case CANCEL:		/// Cancel Button
	      DeleteWindow();
	      break;
	      /////////////////////////////
	    case (COPYBUTTON+1000):
	      emin = NumEntry[1][chanNumber]->GetNumber();
	      bin = NumEntry[2][chanNumber]->GetNumber();
	      for(int i = 0;i < 16;i++)
		{
		  if(i != (chanNumber))
		    {
		      char tmp[10]; sprintf(tmp,"%1.3f",emin);
		      NumEntry[1][i]->SetText(tmp);
		  				
		      sprintf(tmp,"%1.3f",bin);
		      NumEntry[2][i]->SetText(tmp);
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
    case kC_TEXTENTRY:
      switch (GET_SUBMSG(msg))
	{
	case kTE_TAB:
	  if (parm1 < 16)
	    NumEntry[2][parm1]->SetFocus();
	  if (parm1 > 15 && parm1 < 32)
	    {
	      if ((parm1 - 16) + 1 < 16)
		NumEntry[1][(parm1 - 16) + 1]->SetFocus();
	    }
	  break;
	}
      break;
    default:
      break;
    }
  return kTRUE;
}


int Histogram::load_info(Long_t mod)
{
  double ChanParData = -1;
  int retval;
  char text[20];

  for (int i = 0; i < 16; i++)
    {
      retval = Pixie16ReadSglChanPar((char*)"EMIN", &ChanParData, mod, i);
      if(retval < 0) ErrorInfo("Histogram.cpp", "load_info(...)", "Pixie16ReadSglChanPar/EMIN", retval);
      sprintf(text, "%d", (int)ChanParData);
      NumEntry[1][i]->SetText(text);

      retval = Pixie16ReadSglChanPar((char*)"BINFACTOR", &ChanParData, mod, i);
      if(retval < 0) ErrorInfo("Histogram.cpp", "load_info(...)", "Pixie16ReadSglChanPar/BINFACTOR", retval);
      sprintf(text, "%d", (int)ChanParData);
      NumEntry[2][i]->SetText(text);

    }
  //  std::cout << "loading info for module " << module << std::endl;

  return 1;
}

int
Histogram::change_values(Long_t mod)
{
  double cut;
  int retval;
  double percent;
  for (int i = 0; i < 16; i++)
    {
      cut = NumEntry[1][i]->GetNumber();
      retval = Pixie16WriteSglChanPar((char*)"EMIN", cut, mod, i);
      if(retval < 0) ErrorInfo("Histogram.cpp", "change_values(...)", "Pixie16WriteSglChanPar/EMIN", retval);
      
      percent = NumEntry[2][i]->GetNumber();
      retval = Pixie16WriteSglChanPar((char*)"BINFACTOR", percent, mod, i);
      if(retval < 0) ErrorInfo("Histogram.cpp", "change_values(...)", "Pixie16WriteSglChanPar/BINFACTOR", retval);
    }

  return 1;
}
