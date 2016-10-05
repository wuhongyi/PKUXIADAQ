#include "Popup.h"

Popup::Popup(const TGWindow * p, const TGWindow * main,char *message)
{
  TGVerticalFrame *mn_vert = new TGVerticalFrame(this, 200, 300);

  TGGroupFrame *StateMsgFrame = new TGGroupFrame (mn_vert, "", kVerticalFrame);
  
  TGTextEntry *Message = new TGTextEntry(StateMsgFrame,
					 new TGTextBuffer(30), 10000,
					 Message->GetDefaultGC()(),
					 Message->GetDefaultFontStruct(),
					 kRaisedFrame | kDoubleBorder,
					 GetWhitePixel());
  Message->SetFont("-adobe-helvetica-bold-r-*-*-10-*-*-*-*-*-iso8859-1", false);
  
  fClient->GetColorByName("red", color);
  Message->SetTextColor(color, true);
  Message->SetText("About \n         \n          \n");
  Message->Resize(100, 12);
  Message->SetEnabled(kFALSE);
  Message->SetFrameDrawn(kFALSE);


  StateMsgFrame->AddFrame(Message,new TGLayoutHints(kLHintsTop, 0, 0, 0, 0));
  mn_vert->AddFrame(StateMsgFrame, new TGLayoutHints (kLHintsTop, 2, 5, 1, 5));
  fClient->GetColorByName("red", color);
  Message->SetTextColor(color, true);
  Message->SetText(message);



  TGTextButton* OkButton = new TGTextButton(mn_vert, "&Ok", OK);
  OkButton->Associate(this);

  mn_vert->AddFrame(OkButton, new TGLayoutHints (kLHintsTop, 0, 20, 0, 0));
  AddFrame(mn_vert, new TGLayoutHints(kLHintsTop, 2, 2, 2, 2));

  MapSubwindows();
  Resize();			// resize to default size
  CenterOnParent();
  
  SetWindowName("About");
  
  MapWindow();
}

Popup::~Popup()
{
}


Bool_t Popup::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
  switch (GET_MSG(msg))
    {
    case kC_COMMAND:
      switch (GET_SUBMSG(msg))
	{
	case kCM_BUTTON:
	  switch (parm1)
	    {
	    case OK:	/// Cancel Button
	      DeleteWindow();
	      break;
	    default:
	      break;
		   
	    }
	}
    }
  return kTRUE;
}
