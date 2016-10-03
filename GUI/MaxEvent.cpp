#include "MaxEvent.h"

#include "TGGC.h"
#include "TG3DLine.h"
#include "pixie16app_export.h"
#include "TGFrame.h"
#include <iostream>


MaxEvent::MaxEvent(const TGWindow * p, const TGWindow * main)
{
  char name[20];
  module_number1 = 0;
  mn_vert = new TGVerticalFrame(this, 200, 300);

  mn = new TGHorizontalFrame(mn_vert, 200, 300);
  mn_vert->AddFrame (mn, new TGLayoutHints (kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  
  column1 = new TGVerticalFrame(mn, 200, 300);
  column2 = new TGVerticalFrame(mn, 200, 300);
  buttons = new TGHorizontalFrame(mn_vert, 400, 300);
  //
  //  /////////////////////////////////////////////////////////////////
  mn->AddFrame(column1, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  mn->AddFrame(column2, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
 
  /////////////////////////////Labels///////////////////////////////////

  TGTextEntry *te = new TGTextEntry(column1, new TGTextBuffer(100), 10000,
				    te->GetDefaultGC()(),
				    te->GetDefaultFontStruct(),
				    kRaisedFrame | kDoubleBorder,
				    GetWhitePixel());
  te->SetFont("-adobe-helvetica-bold-r-*-*-10-*-*-*-*-*-iso8859-1", false);

  te->SetText("Mod #");
  te->Resize(35, 20);
  te->SetEnabled(kFALSE);
  te->SetFrameDrawn(kTRUE);
  column1->AddFrame(te, new TGLayoutHints(kLHintsCenterX, 0, 0, 10, 0));

  TGTextEntry *Labels[3];

  for (int i = 0; i < 3; i++)
    {
      sprintf (name, "%d", i);
      Labels[i] = new TGTextEntry(column1, new TGTextBuffer(100), 10000,
				  Labels[i]->GetDefaultGC()(),
				  Labels[i]->GetDefaultFontStruct(),
				  kRaisedFrame | kDoubleBorder,
				  GetWhitePixel());
      Labels[i]->SetText(name);
      Labels[i]->Resize(35, 20);
      Labels[i]->SetEnabled(kFALSE);
      Labels[i]->SetFrameDrawn(kTRUE);

      column1->AddFrame(Labels[i], new TGLayoutHints(kLHintsCenterX, 0, 3, 0, 0));
    }

  /////////////////column 1//////////////////////////////////////////
  TGTextEntry *Dec = new TGTextEntry(column2, new TGTextBuffer (100), 10000, Dec->GetDefaultGC()(),Dec->GetDefaultFontStruct(), kRaisedFrame | kDoubleBorder,GetWhitePixel());

  Dec->SetFont("-adobe-helvetica-bold-r-*-*-10-*-*-*-*-*-iso8859-1", false);
  Dec->SetText("Max Event");
  Dec->Resize(100, 20);
  Dec->SetEnabled(kFALSE);
  Dec->SetFrameDrawn(kTRUE);
  column2->AddFrame(Dec, new TGLayoutHints (kLHintsCenterX, 0, 0, 10, 0));
  for (int i = 0; i < 3; i++)
    {
      maxEvent[i] = new TGNumberEntryField (column2, i+250, 0,TGNumberFormat::kNESReal);
      //DecayTime[i]->Resize (80, 20);
      //DecayTime[i]->Associate (this);
      column2->AddFrame(maxEvent[i],new TGLayoutHints (kLHintsCenterX, 0, 0, 0, 0));
    }

  ///////////////////////buttons/////////////////////////////////////

  LoadButton = new TGTextButton(buttons, "&Load", 4000);
  LoadButton->Associate(this);
  ApplyButton = new TGTextButton(buttons, "&Apply", 4001);
  ApplyButton->Associate(this);
  CancelButton = new TGTextButton(buttons, "&Cancel", 4002);
  CancelButton->Associate(this);
  buttons->AddFrame(LoadButton, new TGLayoutHints (kLHintsCenterX, 0, 0, 0, 0));
  buttons->AddFrame(ApplyButton, new TGLayoutHints (kLHintsCenterX, 0, 0, 0, 0));
  buttons->AddFrame(CancelButton, new TGLayoutHints (kLHintsCenterX, 0, 0, 0, 0));


  mn_vert->AddFrame(buttons, new TGLayoutHints (kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  ///////////////////////////////////////////////////////////////
  AddFrame(mn_vert, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2));

  MapSubwindows();
  Resize();			// resize to default size
  CenterOnParent();

  SetWindowName("OnBoard Buffers");

  MapWindow();

}

MaxEvent::~MaxEvent ()
{
	
}

int MaxEvent::load_info()
{
  unsigned int ModParData = 0;
  int retval;
  char text[20];
  for (int i = 0; i < 3; i++)
    {
      retval = Pixie16ReadSglModPar((char*)"MAX_EVENTS", &ModParData,i);
      std::cout<<"maxevent read(MAX_EVENTS): "<<retval<< "in mod "<<i<<std::endl;
      sprintf(text, "%u", ModParData);
      maxEvent[i]->SetText(text);
      ModParData = 0;
    }
  // std::cout << "loading info\n";
  return 1;
}


int MaxEvent::change_values()
{
  double d;
  int retval;
  for (int i = 0; i < 3; i++)
    {
      // d = maxEvent[i]->GetNumber ();
      retval = Pixie16WriteSglModPar((char*)"MAX_EVENTS", (unsigned long)d,i);
      std::cout<<"maxevent write(MAX_EVENTS): "<<retval<< "in mod "<<i<<std::endl;
    }
  //std::cout << "change values\n";
  return 1;
}

Bool_t MaxEvent::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
  switch (GET_MSG(msg))
    {
    case kC_COMMAND:
      switch (GET_SUBMSG(msg))
	{
	case kCM_BUTTON:
	  switch (parm1)
	    {
	    case 4002:		/// Cancel Button
	      DeleteWindow();
	      break;
	    case 4000:
	      {
		Load_Once = true;
		load_info ();
	      }
	      break;
	    case 4001:
	      if (Load_Once)
		change_values();
	      else
		std::cout << "please load once first !\n";
	      break;
	
	    default:
	      break;
		   		
	    }
	}
    }
	
  return kTRUE;
}
