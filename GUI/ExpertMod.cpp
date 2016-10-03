#include "ExpertMod.h"
#include "pixie16app_defs.h"

ExpertMod::ExpertMod(const TGWindow * p, const TGWindow * main, char *name,int NumModules=24)
{
  SetCleanup(kDeepCleanup);

  numModules = PRESET_MAX_MODULES;

  mn_vert = new TGVerticalFrame(this, 200, 300);
  TGHorizontalFrame *Median = new TGHorizontalFrame(mn_vert, 400, 300);

  AddFrame(mn_vert,
	    new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2));
  Median->AddFrame(lstBox =
		    new TGComboBox(mn_vert, 2755),
		    new TGLayoutHints(kLHintsCenterX, 0, 0, 0, 0));
  lstBox->Associate(this);
  lstBox->AddEntry("SET MASTER", 0);
  lstBox->AddEntry("MAX EVENTS/BUFFER", 1);
  lstBox->AddEntry("CoinWindLen", 2);
  lstBox->AddEntry("WinDelayLen", 3);
  lstBox->AddEntry("NumMWModules", 4);
  lstBox->AddEntry("MWModAddress", 5);
  lstBox->Select(0);
  lstBox->Resize(150, 20);
  NumEntry = new TGNumberEntryField(Median, 1000, 0, TGNumberFormat::kNESReal);
  NumEntry->Resize(90, 20);

  Median->AddFrame(NumEntry, new TGLayoutHints(kLHintsCenterX, 0, 0, 0, 0));

  Buttons = new TGHorizontalFrame(mn_vert, 400, 300);
/////////////////////////////module entry///////////////////////////////

  TGHorizontal3DLine *ln1 = new TGHorizontal3DLine(mn_vert, 150, 2);
  TGLabel *mod = new TGLabel(Buttons, "Module #");

  numericMod = new TGNumberEntry(Buttons, 0, 4, MODNUMBER, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3/*kNELLimitMinMax*/, 0, 3);
  numericMod->SetButtonToNum(0);
  numericMod->IsEditable();
  mn_vert->AddFrame(Median, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
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
  Resize();			// resize to default size
  CenterOnParent();

  SetWindowName(name);

  MapWindow();
}

ExpertMod::~ExpertMod ()
{
}

Bool_t
ExpertMod::ProcessMessage (Long_t msg, Long_t parm1, Long_t parm2)
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
			    numericMod->SetIntNumber (modNumber);
			  }
		      }
		    else
		      {
			if (modNumber != 0)
			  {
			    if (--modNumber == 0)
			      modNumber = 0;
			    numericMod->SetIntNumber (modNumber);
			  }
		      }
		    break;
		  case LOAD:
		    {
		      Load_Once = true;
		      
		      load_info (modNumber,lstBox->GetSelected());
		    }
		    break;
		  case APPLY:
		    if (Load_Once)
		      change_values (modNumber,lstBox->GetSelected());
		    else
		      std::cout << "please load once first !\n";
		    break;
		  case CANCEL:	/// Cancel Button
		    DeleteWindow ();
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

int
ExpertMod::load_info (Long_t mod, int param)
{
  int retval=0;
  unsigned int ChanParData = 0;
  char text[20];

  if (param == 0 )
    {
      retval = Pixie16ReadSglModPar ((char*)"MODULE_CSRB", &ChanParData, modNumber);
      sprintf (text, "%u", ChanParData);
      NumEntry->SetText (text);
    }
  
  
  if (param == 1 )
    {
      retval = Pixie16ReadSglModPar ((char*)"MAX_EVENTS", &ChanParData, modNumber);
      sprintf (text, "%u", ChanParData);
      NumEntry->SetText (text);    
    }
  
  // GOTO
  // it error , need I/O it for DSP Address 
  if (param == 2 )
    {
      // retval = Pixie16ReadSglModPar ("COINWINDLEN", &ChanParData, mod);
      retval = Pixie_DSP_Memory_IO(&ChanParData, 0x4a007, 1,  MOD_READ, mod);
      sprintf (text, "%u", ChanParData);
      NumEntry->SetText (text);    
    }
  
  // GOTO 
  if (param == 3 )
    {
      // retval = Pixie16ReadSglModPar ("WINDELAYLEN", &ChanParData, mod);
      retval = Pixie_DSP_Memory_IO(&ChanParData, 0x4a008, 1,  MOD_READ, mod);
      sprintf (text, "%u", ChanParData);
      NumEntry->SetText (text);
    }
  
  // GOTO
  // need cheak
  if (param == 4 )
    {
      retval = Pixie16ReadSglModPar ((char*)"NUMMWMODULES", &ChanParData, mod);
      sprintf (text, "%u", ChanParData);
      NumEntry->SetText (text);
    }
    
  // GOTO
  // need cheak
  if (param == 5 )
    {
      retval = Pixie16ReadSglModPar ((char*)"MWMODADDR", &ChanParData, mod);
      sprintf (text, "%u", ChanParData);
      NumEntry->SetText (text);
    }
    

  return retval;
}

int
ExpertMod::change_values (Long_t mod, int param)
{
  double value;
  unsigned int value_i;
  char ErrMSG[MAX_ERRMSG_LENGTH];
  unsigned int Max_Poll = 10000;// The maximal waiting time is 10 s
  int retval=0;
  value = NumEntry->GetNumber ();
  if (param == 0 )
    {
      retval=Pixie16WriteSglModPar ((char*)"MODULE_CSRB",(long unsigned int)value,modNumber );    
    }
  
  if (param == 1 )
    {
      retval=Pixie16WriteSglModPar ((char*)"MAX_EVENTS",(long unsigned int)value,modNumber );    
    }

  // GOTO 
  if(param==2 )
    {
      // retval=Pixie16WriteSglModPar ("COINWINDLEN",(long unsigned int)value,modNumber );   // this function don't provide par "COINWINDLEN"

      value_i = (unsigned int)value;
      retval = Pixie_DSP_Memory_IO(&value_i, 0x4a007, 1,  MOD_WRITE, modNumber);
      if(retval < 0)
	{
	  sprintf(ErrMSG, "*ERROR* (CoincPattern): IMbuffer I/O failed for module %d, retval=%d",  modNumber, retval);
	  cout<<ErrMSG<<endl;
	  return -1;
	}

      retval = Pixie_Control_Task_Run( modNumber,PROGRAM_FIPPI, Max_Poll);
	if(retval == -1)
	{
	  sprintf(ErrMSG, "*ERROR* (ProgramFippi): failed to start PROGRAM_FIPPI run; retval=%d", retval);
	  cout<<ErrMSG<<endl;
	  return -2;
	}
	else if(retval == -2)
	{
	  sprintf(ErrMSG, "*ERROR* (ProgramFippi): PROGRAM_FIPPI run timed out; retval=%d", retval);
	  cout<<ErrMSG<<endl;
	  return -3; // Time Out
	}

    }

  // GOTO
  if(param==3)
    {
      // retval=Pixie16WriteSglModPar ("WINDELAYLEN",(long unsigned int)value,modNumber );    

      value_i = (unsigned int)value;
      retval = Pixie_DSP_Memory_IO(&value_i, 0x4a008, 1,  MOD_WRITE, modNumber);
      if(retval < 0)
	{
	  sprintf(ErrMSG, "*ERROR* (CoincWait): IMbuffer I/O failed for module %d, retval=%d",  modNumber, retval);
	  cout<<ErrMSG<<endl;
	  return -1;
	}

      retval = Pixie_Control_Task_Run( modNumber,PROGRAM_FIPPI, Max_Poll);
	if(retval == -1)
	{
	  sprintf(ErrMSG, "*ERROR* (ProgramFippi): failed to start PROGRAM_FIPPI run; retval=%d", retval);
	  cout<<ErrMSG<<endl;
	  return -2;
	}
	else if(retval == -2)
	{
	  sprintf(ErrMSG, "*ERROR* (ProgramFippi): PROGRAM_FIPPI run timed out; retval=%d", retval);
	  cout<<ErrMSG<<endl;
	  return -3; // Time Out
	}


    }
  
  // GOTO 
  // need cheak,this function don't provide par "NUMMWMODULES"
  if(param==4)
    {
      retval=Pixie16WriteSglModPar ((char*)"NUMMWMODULES",(long unsigned int)value,modNumber );    
    }

  // GOTO 
  // need cheak,this function don't provide par "MWMODADDR"
  if(param==5)
    {
      retval=Pixie16WriteSglModPar ((char*)"MWMODADDR",(long unsigned int)value,modNumber );    
    }
  return retval;
  
}
