#include "Tau.h"
#include "Global.h"
#include "pixie16app_defs.h"

Tau::Tau (const TGWindow * p, const TGWindow * main, char *name, int columns, int rows, int NumModules)
  :Table(p, main, columns, rows, name, PRESET_MAX_MODULES)
{
  char n[10];
  cl0->SetText("ch #");
  for (int i = 0; i < rows; i++)
    {
      sprintf (n, "%2d", i);
      Labels[i]->SetText(n);
    }
  CLabel[0]->SetText("Tau [us]");
  CLabel[0]->SetAlignment(kTextCenterX);
  CLabel[1]->SetText("TauA[us]");
  CLabel[1]->SetAlignment(kTextCenterX);
  
  for(int i=0;i<16;i++)
    NumEntry[2][i]->SetEnabled(0);


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
  CopyButton->AddFrame(copyB, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  modNumber = 0;
  chanNumber = 0;
  decay = 0;
  Load_Once = true;


  TGTextButton *findTau = new TGTextButton( CopyButton, "&FindTau",COPYBUTTON + 2000);
  findTau->Associate(this);
  findTau->SetToolTipText("Find the selected module's decay time Tau automatically by module\n You should set an initial value at first");
  CopyButton->AddFrame(findTau,new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  TGTextButton *acceptTau = new TGTextButton( CopyButton, "&Accept",COPYBUTTON + 3000);
  acceptTau->Associate(this);
  acceptTau->SetToolTipText("Accept the decay time find by module");
  CopyButton->AddFrame(acceptTau,new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  //tlength=0;tdelay=0;
  ///////////////////////////////////////////////////////////////////////
  MapSubwindows();
  Resize();			// resize to default size
}

Tau::~Tau()
{
}

Bool_t Tau::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
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
	    case (COPYBUTTON + 2000):
	      {
		findtau();
		break;
	      }
	    case (COPYBUTTON + 3000):
	      cout<<"Accept!\n";
	      for (int i = 0; i < 16; i++)
		{
		  decay = NumEntry[2][i]->GetNumber();
		  sprintf(tmp, "%1.3f", decay);
		  NumEntry[1][i]->SetText(tmp);
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

int Tau::load_info(Long_t mod)
{
  double ChanParData = -1;
  int retval;
  char text[20];
  for (int i = 0; i < 16; i++)
    {
      retval = Pixie16ReadSglChanPar((char*)"TAU", &ChanParData, modNumber, i);
      if(retval < 0) ErrorInfo("Tau.cpp", "load_info(...)", "Pixie16ReadSglChanPar/TAU", retval);
      sprintf(text, "%1.2f", ChanParData);
      NumEntry[1][i]->SetText(text);
    }
  //  cout << "loading info\n";
  return 1;
}

int Tau::change_values(Long_t mod)
{
  int retval;
  double d;
  for (int i = 0; i < 16; i++)
    {
      d = NumEntry[1][i]->GetNumber();
      retval = Pixie16WriteSglChanPar((char*)"TAU", d, modNumber, i);
      if(retval < 0) ErrorInfo("Tau.cpp", "change_values(...)", "Pixie16WriteSglChanPar/TAU", retval);
    }
  //cout << "change values\n";
  return 1;
}

void Tau::findtau()
{
  double TauByFPGA[16];
  int retval = Pixie16TauFinder(modNumber,TauByFPGA);
  if(retval<0){
    if(retval < 0) ErrorInfo("Tau.cpp", "findtau(...)", "Pixie16TauFinder", retval);
    switch (retval) {
    case -1:
      cout<<"Error ModNum!"<<endl;
      break;
    case -2:
      cout<<"Error ChannleNum!"<<endl;
      break;
    case -3:
      cout<<"Tau Finder TIMED OUT"<<endl;
      break;
    case -4:
      cout<<"Failed to find sufficient number of pulses!"<<endl;
      cout<<"Pls reboot the module!"<<endl;
      break;
    case -5:
      cout<<"Low input count rate, increase the input count rate pls!"<<endl;
      break;
    default:
      cout<<"ERROR!"<<endl;
      break;
    }
  }else {
    for(int i=0;i<16;i++){
      sprintf (tmp, "%1.3f", TauByFPGA[i]);
      NumEntry[2][i]->SetText(tmp);
    }
  }

}
