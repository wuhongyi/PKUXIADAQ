#include "EnergyFilter.h"

#include "Global.h"
#include "pixie16app_export.h"

EnergyFilter::EnergyFilter(const TGWindow * p, const TGWindow * main,
			   char *name, int columns, int rows, int NumModules)
  :Table(p, main, columns, rows, name, NumModules)
{
  char n[10];
  cl0->SetText ("ch #");
  for (int i = 0; i < rows; i++)
    {
      sprintf (n, "%2d", i);
      Labels[i]->SetText (n);
    }
  CLabel[0]->SetText("TPeaking[us]");
  CLabel[0]->SetAlignment(kTextCenterX);
  CLabel[1]->SetText("TGap[us]");
  CLabel[1]->SetAlignment(kTextCenterX);

  risetime = 0;
  flattop = 0;
  fRange = 0;
  modNumber = 0;
  Load_Once = true;


  TGHorizontalFrame *Details = new TGHorizontalFrame(mn_vert, 400, 300);
  /////////////////////////////Filter range entry///////////////////////////////

  TGHorizontal3DLine *ln2 = new TGHorizontal3DLine(mn_vert, 60, 2);
  TGLabel *fil = new TGLabel(Details, "Filter Range");

  filterRange = new TGNumberEntry(Details, 0, 4, FILTER, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3/*kNELLimitMinMax*/, 0, 3);
  filterRange->SetButtonToNum(0);

  mn_vert->AddFrame(ln2, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 10, 10));
  Details->AddFrame(fil, new TGLayoutHints(kLHintsCenterX, 5, 10, 3, 0));
  Details->AddFrame(filterRange, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 20, 0, 0));

  filterRange->Associate(this);
  mn_vert->AddFrame(Details, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  //      load the current filter range:
  int retval; 
  retval = Pixie16ReadSglModPar((char*)"SLOW_FILTER_RANGE", &fRange, modNumber);
  if(retval < 0) ErrorInfo("EnergyFilter.cpp", "EnergyFilter(...)", "Pixie16ReadSglModPar/SLOW_FILTER_RANGE", retval);
  //      fRange=1;
  filterRange->SetIntNumber(fRange);

  ////////////////////////Copy Button//////////////////////////////////

  TGHorizontal3DLine *ln3 = new TGHorizontal3DLine(mn_vert, 200, 2);
  mn_vert->AddFrame (ln3, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 10, 10));
  TGHorizontalFrame *CopyButton = new TGHorizontalFrame(mn_vert, 400, 300);
  mn_vert->AddFrame(CopyButton, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  TGLabel *Copy = new TGLabel(CopyButton, "Select channel #");

  chanCopy = new TGNumberEntry(CopyButton, 0, 4, MODNUMBER + 1000, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3/*kNELLimitMinMax*/, 0, 3);
  chanCopy->SetButtonToNum(0);
  chanCopy->IsEditable();

  CopyButton->AddFrame(Copy, new TGLayoutHints(kLHintsCenterX, 5, 10, 3, 0));
  CopyButton->AddFrame(chanCopy, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 20, 0, 0));

  chanCopy->Associate (this);
  ///////////////////////copy button per se///////////////////////////
  TGTextButton *copyB = new TGTextButton(CopyButton, "C&opy", COPYBUTTON + 1000);
  copyB->Associate(this);
  copyB->SetToolTipText("Copy the setup of the selected channel to all channels of the module", 0);
  CopyButton->AddFrame (copyB, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 20, 0, 0));

  chanNumber = 0;
  chanCopy->SetIntNumber(0);
  MapSubwindows();
  Resize();			// resize to default size
}


EnergyFilter::~EnergyFilter()
{
}

Bool_t EnergyFilter::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
  int retval;
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
	      ///////////////////////////////////////
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
	    case (FILTER):
	      if (parm2 == 0)
		{
		  if (fRange != 6)
		    {
		      ++fRange;
		      filterRange->SetIntNumber(fRange);
		      cout << fRange << " " << modNumber << "\n" << flush;
		      retval = Pixie16WriteSglModPar((char*)"SLOW_FILTER_RANGE",fRange, modNumber);
		      if(retval < 0) ErrorInfo("EnergyFilter.cpp", "ProcessMessage(...)", "Pixie16WriteSglModPar/SLOW_FILTER_RANGE", retval);
		    }
		}
	      else
		{
		  if (fRange != 1)
		    {
		      if (--fRange == 1)
			fRange = 1;
		      filterRange->SetIntNumber(fRange);
		      retval = Pixie16WriteSglModPar((char*)"SLOW_FILTER_RANGE",fRange, modNumber);
		      if(retval < 0) ErrorInfo("EnergyFilter.cpp", "ProcessMessage(...)", "Pixie16WriteSglModPar/SLOW_FILTER_RANGE", retval);
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
	      ///////////////////////////////////////////
	    case (COPYBUTTON + 1000):
	      risetime = NumEntry[1][chanNumber]->GetNumber();
	      flattop = NumEntry[2][chanNumber]->GetNumber();

	      for (int i = 0; i < 16; i++)
		{
		  if (i != (chanNumber))
		    {
		      char tmp[10];
		      sprintf (tmp, "%1.3f", risetime);
		      NumEntry[1][i]->SetText (tmp);
		      sprintf (tmp, "%1.3f", flattop);
		      NumEntry[2][i]->SetText (tmp);
		    }
		}

	      break;

	      ///////////////////////////////////////////

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

int
EnergyFilter::load_info(Long_t mod)
{
  double ChanParData = -1;

  int retval;
  char text[20];

  for (int i = 0; i < 16; i++)
    {
      retval = Pixie16ReadSglChanPar((char*)"ENERGY_RISETIME", &ChanParData, mod, i);
      if(retval < 0) ErrorInfo("EnergyFilter.cpp", "load_info(...)", "Pixie16ReadSglChanPar/ENERGY_RISETIME", retval);
      sprintf(text, "%1.2f", ChanParData);
      NumEntry[1][i]->SetText(text);

      retval = Pixie16ReadSglChanPar((char*)"ENERGY_FLATTOP", &ChanParData, mod, i);
      if(retval < 0) ErrorInfo("EnergyFilter.cpp", "load_info(...)", "Pixie16ReadSglChanPar/ENERGY_FLATTOP", retval);
      sprintf(text, "%1.2f", ChanParData);
      NumEntry[2][i]->SetText (text);
    }

  unsigned int Range = 0; 
  retval = Pixie16ReadSglModPar((char*)"SLOW_FILTER_RANGE", &Range, mod);
  if(retval < 0) ErrorInfo("EnergyFilter.cpp", "load_info(...)", "Pixie16ReadSglModPar/SLOW_FILTER_RANGE", retval);
  filterRange->SetIntNumber(Range);

  return 1;
}

int EnergyFilter::change_values(Long_t mod)
{
  int retval;
  double length;
  double delay;
  for (int i = 0; i < 16; i++)
    {
      length = NumEntry[1][i]->GetNumber();
      retval = Pixie16WriteSglChanPar((char*)"ENERGY_RISETIME", length, mod, i);
      if(retval < 0) ErrorInfo("EnergyFilter.cpp", "change_values(...)", "Pixie16WriteSglChanPar/ENERGY_RISETIME", retval);
      
      delay = NumEntry[2][i]->GetNumber();
      retval = Pixie16WriteSglChanPar((char*)"ENERGY_FLATTOP", delay, mod, i);
      if(retval < 0) ErrorInfo("EnergyFilter.cpp", "change_values(...)", "Pixie16WriteSglChanPar/ENERGY_FLATTOP", retval);  
    }

  return 1;
}






