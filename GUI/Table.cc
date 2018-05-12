#include "Table.hh"
#include "TSystem.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Table::Table(const TGWindow * p, const TGWindow * main, int columns,
	      int rows, char *name, int NumModules)
{
  SetCleanup(kDeepCleanup);

  numModules = NumModules;

  mn_vert = new TGVerticalFrame(this, 200, 300);
  mn = new TGHorizontalFrame(mn_vert, 200, 300);
  mn_vert->AddFrame(mn, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  AddFrame(mn_vert, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2));
	    
  Buttons = new TGHorizontalFrame(mn_vert, 400, 300);
  StatusFrame = new TGHorizontalFrame(mn_vert, 400, 300);
  	    
  Column = new TGVerticalFrame *[columns];
  for (int i = 0; i < columns; i++)
    {
      Column[i] = new TGVerticalFrame(mn, 200, 300);
      mn->AddFrame(Column[i], new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
    }

  //////////////////labels column///////////////////////////////////////////  
  cl0 = new TGTextEntry(Column[0], new TGTextBuffer(100), 10000,
			cl0->GetDefaultGC()(),
			cl0->GetDefaultFontStruct(),
			kRaisedFrame | kDoubleBorder, GetWhitePixel());
  cl0->SetFont("-adobe-helvetica-bold-r-*-*-10-*-*-*-*-*-iso8859-1", false);

  //cl0->SetText ("Ch #");
  cl0->Resize(35, 20);
  cl0->SetEnabled(kFALSE);
  cl0->SetFrameDrawn(kTRUE);
  Column[0]->AddFrame(cl0, new TGLayoutHints(kLHintsCenterX, 0, 0, 10, 0));

  Labels = new TGTextEntry *[rows];

  for (int i = 0; i < rows; i++)
    {
      Labels[i] = new TGTextEntry(Column[0], new TGTextBuffer(100), 10000,
				  Labels[i]->GetDefaultGC()(),
				  Labels[i]->GetDefaultFontStruct(),
				  kRaisedFrame | kDoubleBorder,
				  GetWhitePixel());
      Labels[i]->Resize(35, 20);
      Labels[i]->SetEnabled(kFALSE);
      Labels[i]->SetFrameDrawn(kTRUE);

      Column[0]->AddFrame(Labels[i], new TGLayoutHints(kLHintsCenterX, 0, 3, 0, 0));
    }


  /////////////////////////////////////////////////

  CLabel = new TGTextEntry *[columns - 1];
  for (int i = 0; i < columns - 1; i++)
    {
      CLabel[i] =
	new TGTextEntry(Column[i + 1], new TGTextBuffer(100), 10000,
			CLabel[i]->GetDefaultGC()(),
			CLabel[i]->GetDefaultFontStruct(),
			kRaisedFrame | kDoubleBorder, GetWhitePixel());

      CLabel[i]->SetFont("-adobe-helvetica-bold-r-*-*-10-*-*-*-*-*-iso8859-1", false);
      CLabel[i]->Resize(90, 20);
      CLabel[i]->SetEnabled(kFALSE);
      CLabel[i]->SetFrameDrawn(kTRUE);
      Column[i + 1]->AddFrame(CLabel[i], new TGLayoutHints(kLHintsCenterX, 0, 0, 10, 0));
    }

  /////////////////////////////////////////////////
  NumEntry = new TGNumberEntryField **[columns];
  for (int i = 0; i < columns; i++)
    NumEntry[i] = new TGNumberEntryField *[rows];

  for (int i = 1; i < columns; i++)
    {

      for (int j = 0; j < rows; j++)
	{
	  NumEntry[i][j] = new TGNumberEntryField(Column[i], i, 0, TGNumberFormat::kNESReal);
	  NumEntry[i][j]->Resize(90, 20);
	  NumEntry[i][j]->Associate(this);
	  Column[i]->AddFrame(NumEntry[i][j], new TGLayoutHints(kLHintsCenterX, 0, 0, 0, 0));
	}

    }
  /////////////////////////////////////////////////////////////////
  ///////////////////////Buttons making ...///////////////////////

  /////////////////////////////module entry///////////////////////////////

  TGHorizontal3DLine *ln1 = new TGHorizontal3DLine(Column[0], 30, 2);

  mn_vert->AddFrame(StatusFrame, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  TGLabel *labelstatus = new TGLabel(StatusFrame, "Status:");
  StatusFrame->AddFrame(labelstatus, new TGLayoutHints(kLHintsLeft, 5, 10, 3, 0));
  userstatus = new TGTextEntry(StatusFrame,new TGTextBuffer(30), 10000);
  StatusFrame->AddFrame(userstatus, new TGLayoutHints(kLHintsLeft, 5, 0, 0, 0));
  userstatus->SetEnabled(kFALSE);
  userstatus->SetFrameDrawn(kFALSE);
  fClient->GetColorByName("green", color);
  userstatus->SetTextColor(color, false);
  userstatus->SetText("Ready");
  userstatus->Resize(100, 16);
  
  
  TGLabel *mod = new TGLabel(Buttons, "Module #");

  numericMod = new TGNumberEntry(Buttons, 0, 4, MODNUMBER, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) NumModules/*kNELLimitMinMax*/, 0, NumModules);
  numericMod->SetButtonToNum(0);
  numericMod->IsEditable();
  Column[0]->AddFrame(ln1, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 10, 10));
  Buttons->AddFrame(mod, new TGLayoutHints(kLHintsCenterX, 5, 10, 3, 0));
  Buttons->AddFrame(numericMod, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 20, 0, 0));

  numericMod->Associate(this);
  mn_vert->AddFrame(Buttons, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  /////////////////////////////////////////////////////////////////////////
  ////////////////////////////Buttons/////////////////////////////////////
  TGTextButton* LoadButton = new TGTextButton(Buttons, "&Load", LOAD);
  LoadButton->Associate(this);
  TGTextButton* ApplyButton = new TGTextButton(Buttons, "&Apply", APPLY);
  ApplyButton->Associate(this);
  TGTextButton* CancelButton = new TGTextButton(Buttons, "&Cancel", CANCEL);
  CancelButton->Associate(this);
  Buttons->AddFrame(LoadButton, new TGLayoutHints(kLHintsCenterX, 0, 0, 0, 0));
  Buttons->AddFrame(ApplyButton, new TGLayoutHints(kLHintsCenterX, 0, 0, 0, 0));
  Buttons->AddFrame(CancelButton, new TGLayoutHints(kLHintsCenterX, 0, 0, 0, 0));

  
  ///////////////////////////////////////////////////////////////////

  MapSubwindows();
  Resize();			// resize to default size
  CenterOnParent();
  SetWindowName(name);
  MapWindow();
}

Table::~Table()
{
}

int Table::LoadInfo(Long_t mod, TGNumberEntryField *** NumEntry, int column, char *parameter)
{

  return 1;
}

void Table::PreFunction()
{
  fClient->GetColorByName("red", color);
  userstatus->SetTextColor(color, false);
  userstatus->SetText("Waitting...");
  gSystem->ProcessEvents();
}

void Table::PostFunction()
{
  fClient->GetColorByName("green", color);
  userstatus->SetTextColor(color, false);
  userstatus->SetText("Ready");
}
