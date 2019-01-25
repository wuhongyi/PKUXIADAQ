// Energy.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 11月 18 20:32:50 2016 (+0800)
// Last-Updated: 五 1月 25 11:27:48 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 22
// URL: http://wuhongyi.cn 

#include "Energy.hh"
#include "Global.hh"

#include "pixie16app_export.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Energy::Energy(const TGWindow * p, const TGWindow * main, char *name, int columns, int rows, int NumModules)
  : Table(p, main, columns, rows, name, NumModules)
{
  modNumber = 0;
  chanNumber = 0;
  decay = 0;
  Load_Once = true;
  risetime = 0;
  flattop = 0;
  fRange = 0;
  
  cl0->SetText("ch #");
  for (int i = 0; i < rows; i++)
    {
      Labels[i]->SetText(TString::Format("%2d", i).Data());
    }
  CLabel[0]->SetText("Rise Time[us]");
  CLabel[0]->SetAlignment(kTextCenterX);
  CLabel[1]->SetText("Flat Top[us]");
  CLabel[1]->SetAlignment(kTextCenterX);
  CLabel[2]->SetText("Tau [us]");
  CLabel[2]->SetAlignment(kTextCenterX);
  CLabel[3]->SetText("TauA[us]");
  CLabel[3]->SetAlignment(kTextCenterX);
  
  for(int i=0;i<16;i++)
    NumEntry[4][i]->SetEnabled(0);

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

  // load the current filter range:
  int retval; 
  retval = Pixie16ReadSglModPar((char*)"SLOW_FILTER_RANGE", &fRange, modNumber);
  if(retval < 0) ErrorInfo("Energy.cc", "Energy(...)", "Pixie16ReadSglModPar/SLOW_FILTER_RANGE", retval);
  filterRange->SetIntNumber(fRange);

  
  ////////////////Copy Button//////////////////////////////////////////////
  
  TGHorizontal3DLine *ln3 = new TGHorizontal3DLine(mn_vert, 200, 2);
  mn_vert->AddFrame(ln3, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 10, 10));
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
  CopyButton->AddFrame(copyB, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  
  TGTextButton *findTau = new TGTextButton(CopyButton, "&FindTau",COPYBUTTON + 2000);
  findTau->Associate(this);
  findTau->SetToolTipText("Find the selected module's decay time Tau automatically by module\n You should set an initial value at first");
  CopyButton->AddFrame(findTau,new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  TGTextButton *acceptTau = new TGTextButton(CopyButton, "&Accept",COPYBUTTON + 3000);
  acceptTau->Associate(this);
  acceptTau->SetToolTipText("Accept the decay time find by module");
  CopyButton->AddFrame(acceptTau,new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  
  ///////////////////////////////////////////////////////////////////////

  MapSubwindows();
  Resize();// resize to default size
}


Energy::~Energy()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Bool_t Energy::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
  PreFunction();
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
	    case FILTER:
	      if (parm2 == 0)
		{
		  if (fRange != 6)
		    {
		      ++fRange;
		      filterRange->SetIntNumber(fRange);
		      std::cout << fRange << " " << modNumber << "\n" << flush;
		      retval = Pixie16WriteSglModPar((char*)"SLOW_FILTER_RANGE",fRange, modNumber);
		      if(retval < 0) ErrorInfo("Energy.cc", "ProcessMessage(...)", "Pixie16WriteSglModPar/SLOW_FILTER_RANGE", retval);
		      load_info(modNumber);
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
		      if(retval < 0) ErrorInfo("Energy.cc", "ProcessMessage(...)", "Pixie16WriteSglModPar/SLOW_FILTER_RANGE", retval);
		      load_info(modNumber);
		    }
		}
	      break;
	      
	      ////////////////////////////////////////
	    case LOAD:
	      Load_Once = true;
	      load_info(modNumber);
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
	      //////////////////////////////
	    case (COPYBUTTON + 1000):
	      risetime = NumEntry[1][chanNumber]->GetNumber();
	      flattop = NumEntry[2][chanNumber]->GetNumber();
	      decay = NumEntry[3][chanNumber]->GetNumber();
	      for (int i = 0; i < 16; i++)
		{
		  if (i != (chanNumber))
		    {
		      NumEntry[1][i]->SetText(TString::Format("%1.3f", risetime).Data());
		      NumEntry[2][i]->SetText(TString::Format("%1.3f", flattop).Data());
		      NumEntry[3][i]->SetText(TString::Format("%1.3f", decay).Data());
		    }
		}
	      break;
	    case (COPYBUTTON + 2000):
	      findtau(modNumber);
	      break;

	    case (COPYBUTTON + 3000):
	      std::cout<<"Accept!"<<std::endl;
	      for (int i = 0; i < 16; i++)
		{
		  decay = NumEntry[4][i]->GetNumber();
		  NumEntry[3][i]->SetText(TString::Format("%1.3f", decay).Data());
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

int Energy::load_info(Long_t mod)
{
  double ChanParData = -1;
  int retval;
  
  for (int i = 0; i < 16; i++)
    {
      retval = Pixie16ReadSglChanPar((char*)"ENERGY_RISETIME", &ChanParData, mod, i);
      if(retval < 0) ErrorInfo("Energy.cc", "load_info(...)", "Pixie16ReadSglChanPar/ENERGY_RISETIME", retval);
      NumEntry[1][i]->SetText(TString::Format("%1.3f", ChanParData).Data());

      retval = Pixie16ReadSglChanPar((char*)"ENERGY_FLATTOP", &ChanParData, mod, i);
      if(retval < 0) ErrorInfo("Energy.cc", "load_info(...)", "Pixie16ReadSglChanPar/ENERGY_FLATTOP", retval);
      NumEntry[2][i]->SetText (TString::Format("%1.3f", ChanParData).Data());
      
      retval = Pixie16ReadSglChanPar((char*)"TAU", &ChanParData, mod, i);
      if(retval < 0) ErrorInfo("Energy.cc", "load_info(...)", "Pixie16ReadSglChanPar/TAU", retval);
      NumEntry[3][i]->SetText(TString::Format("%1.3f", ChanParData).Data());
    }

  unsigned int Range = 0; 
  retval = Pixie16ReadSglModPar((char*)"SLOW_FILTER_RANGE", &Range, mod);
  if(retval < 0) ErrorInfo("EnergyFilter.cc", "load_info(...)", "Pixie16ReadSglModPar/SLOW_FILTER_RANGE", retval);
  filterRange->SetIntNumber(Range);

  return 1;
}

int Energy::change_values(Long_t mod)
{
  int retval;
  double d;
  double length;
  double delay;
  
  for (int i = 0; i < 16; i++)
    {
      length = NumEntry[1][i]->GetNumber();
      retval = Pixie16WriteSglChanPar((char*)"ENERGY_RISETIME", length, mod, i);
      if(retval < 0) ErrorInfo("Energy.cc", "change_values(...)", "Pixie16WriteSglChanPar/ENERGY_RISETIME", retval);
      
      delay = NumEntry[2][i]->GetNumber();
      retval = Pixie16WriteSglChanPar((char*)"ENERGY_FLATTOP", delay, mod, i);
      if(retval < 0) ErrorInfo("Energy.cc", "change_values(...)", "Pixie16WriteSglChanPar/ENERGY_FLATTOP", retval);  
      
      d = NumEntry[3][i]->GetNumber();
      retval = Pixie16WriteSglChanPar((char*)"TAU", d, mod, i);
      if(retval < 0) ErrorInfo("Energy.cc", "change_values(...)", "Pixie16WriteSglChanPar/TAU", retval);
    }

  return 1;
}

void Energy::findtau(short int mod)
{
  double TauByFPGA[16];
  int retval = Pixie16TauFinder(mod,TauByFPGA);
  if(retval < 0)
    {
      if(retval < 0) ErrorInfo("Energy.cc", "findtau(...)", "Pixie16TauFinder", retval);
      switch (retval)
	{
	case -1:
	  std::cout<<"Error ModNum!"<<std::endl;
	  break;
	case -2:
	  std::cout<<"Error ChannleNum!"<<std::endl;
	  break;
	case -3:
	  std::cout<<"Tau Finder TIMED OUT"<<std::endl;
	  break;
	case -4:
	  std::cout<<"Failed to find sufficient number of pulses!"<<std::endl;
	  std::cout<<"Pls reboot the module!"<<std::endl;
	  break;
	case -5:
	  std::cout<<"Low input count rate, increase the input count rate pls!"<<std::endl;
	  break;
	default:
	  std::cout<<"ERROR!"<<std::endl;
	  break;
	}
    }
  else
    {
      for(int i = 0; i < 16; i++)
	{
	  NumEntry[4][i]->SetText(TString::Format("%1.3f", TauByFPGA[i]).Data());
	}
    }
}


// 
// Energy.cc ends here
