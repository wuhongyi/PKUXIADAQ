#include "Global.hh"

#include "Table.hh"
#include "TSystem.h"
#include "TColor.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Table::Table(const TGWindow * p, const TGWindow * main, int columns,
	      int rows, char *name, int NumModules)
{
  SetCleanup(kDeepCleanup);

  numModules = NumModules;

  SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  mn_vert = new TGVerticalFrame(this, 200, 300);
  AddFrame(mn_vert, new TGLayoutHints(kLHintsTop | kLHintsLeft, 10, 10, 5, 5));
  mn_vert->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  mn = new TGHorizontalFrame(mn_vert, 200, 300);
  mn_vert->AddFrame(mn, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  mn->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  Column = new TGVerticalFrame *[columns];
  for (int i = 0; i < columns; i++)
    {
      Column[i] = new TGVerticalFrame(mn, 200, 300);
      mn->AddFrame(Column[i], new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
      Column[i]->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
    }


  cl0 = new TGTextEntry(Column[0], new TGTextBuffer(100), 10000/*,
			cl0->GetDefaultGC()(),
			cl0->GetDefaultFontStruct(),
			kRaisedFrame | kDoubleBorder, GetWhitePixel()*/);
  Column[0]->AddFrame(cl0, new TGLayoutHints(kLHintsCenterX, 0, 0, 10, 0));
  cl0->SetFont(TABLE_LABEL_CH_FONT, false);
  cl0->SetAlignment(kTextCenterX);
  cl0->Resize(35, 20);
  cl0->SetEnabled(kFALSE);
  cl0->SetFrameDrawn(kFALSE);
  cl0->ChangeBackground(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  cl0->SetTextColor(TColor::RGB2Pixel(LABEL_TEXT_R,LABEL_TEXT_G,LABEL_TEXT_B), false);

  Labels = new TGTextEntry *[rows];
  for (int i = 0; i < rows; i++)
    {
      Labels[i] = new TGTextEntry(Column[0], new TGTextBuffer(100), 10000/*,
				  Labels[i]->GetDefaultGC()(),
				  Labels[i]->GetDefaultFontStruct(),
				  kRaisedFrame | kDoubleBorder,
				  GetWhitePixel()*/);
      Column[0]->AddFrame(Labels[i], new TGLayoutHints(kLHintsCenterX, 0, 3, 0, 0));
      Labels[i]->SetAlignment(kTextCenterX);
      Labels[i]->Resize(35, 20);
      Labels[i]->SetEnabled(kFALSE);
      Labels[i]->SetFrameDrawn(kFALSE);
      Labels[i]->SetFont(TABLE_LABEL_CH_FONT, false);
      Labels[i]->ChangeBackground(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
      Labels[i]->SetTextColor(TColor::RGB2Pixel(LABEL_TEXT_R,LABEL_TEXT_G,LABEL_TEXT_B), false);
    }

  /////////////////////////////////////////////////

  CLabel = new TGTextEntry *[columns - 1];
  for (int i = 0; i < columns-1; i++)
    {
      CLabel[i] =
	new TGTextEntry(Column[i+1], new TGTextBuffer(100), 10000/*,
			CLabel[i]->GetDefaultGC()(),
			CLabel[i]->GetDefaultFontStruct(),
			kRaisedFrame | kDoubleBorder, GetWhitePixel()*/);
      Column[i+1]->AddFrame(CLabel[i], new TGLayoutHints(kLHintsCenterX, 0, 0, 10, 0));
      CLabel[i]->SetFont(TABLE_LABEL_TITLE_FONT, false);
      CLabel[i]->Resize(90, 20);
      CLabel[i]->SetEnabled(kFALSE);
      CLabel[i]->SetFrameDrawn(kFALSE);
      CLabel[i]->ChangeBackground(TColor::RGB2Pixel(TABLE_LABELTITLE_BG_R,TABLE_LABELTITLE_BG_G,TABLE_LABELTITLE_BG_B));
      CLabel[i]->SetTextColor(TColor::RGB2Pixel(TABLE_LABELTITLE_TEXT_R,TABLE_LABELTITLE_TEXT_G,TABLE_LABELTITLE_TEXT_B), false);
      CLabel[i]->SetAlignment(kTextCenterX);
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
	  Column[i]->AddFrame(NumEntry[i][j], new TGLayoutHints(kLHintsCenterX, 0, 0, 0, 0));
	  NumEntry[i][j]->Resize(90, 20);
	  NumEntry[i][j]->Associate(this);
	}

    }
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  Buttons = new TGHorizontalFrame(mn_vert, 400, 300);
  mn_vert->AddFrame(Buttons, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  Buttons->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  TGLabel *mod = new TGLabel(Buttons, "Module #");
  Buttons->AddFrame(mod, new TGLayoutHints(kLHintsCenterX, 5, 0, 5, 0));
  mod->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  mod->SetTextColor(TColor::RGB2Pixel(TABLE_LABEL_TEXT_R,TABLE_LABEL_TEXT_G,TABLE_LABEL_TEXT_B));
  
  numericMod = new TGNumberEntry(Buttons, 0, 4, MODNUMBER, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) NumModules/*kNELLimitMinMax*/, 0, NumModules);
  Buttons->AddFrame(numericMod, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 20, 3, 0));
  numericMod->Associate(this);
  numericMod->SetButtonToNum(0);
  numericMod->IsEditable();
  numericMod->GetNumberEntry()->ChangeOptions(numericMod->GetNumberEntry()->GetOptions() ^ kRaisedFrame);
  numericMod->GetNumberEntry()->SetTextColor(TColor::RGB2Pixel(TITLE_TEXT_R,TITLE_TEXT_G,TITLE_TEXT_B), false);
  numericMod->GetButtonUp()->ChangeOptions(numericMod->GetButtonUp()->GetOptions() ^ kRaisedFrame);
  numericMod->GetButtonDown()->ChangeOptions(numericMod->GetButtonDown()->GetOptions() ^ kRaisedFrame);
  numericMod->ChangeSubframesBackground(TColor::RGB2Pixel(TEXTENTRY_BG_R,TEXTENTRY_BG_G,TEXTENTRY_BG_B));
  

  
  TGTextButton* LoadButton = new TGTextButton(Buttons, "&Load", LOAD);
  Buttons->AddFrame(LoadButton, new TGLayoutHints(kLHintsCenterX, 5, 0, 0, 0));
  LoadButton->Associate(this);
  LoadButton->ChangeOptions(LoadButton->GetOptions() ^ kRaisedFrame);
  LoadButton->SetFont(TEXTBUTTON_FONT, false);
  LoadButton->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  LoadButton->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));
  
  TGTextButton* ApplyButton = new TGTextButton(Buttons, "&Apply", APPLY);
  Buttons->AddFrame(ApplyButton, new TGLayoutHints(kLHintsCenterX, 5, 0, 0, 0));
  ApplyButton->Associate(this);
  ApplyButton->ChangeOptions(ApplyButton->GetOptions() ^ kRaisedFrame);
  ApplyButton->SetFont(TEXTBUTTON_FONT, false);
  ApplyButton->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  ApplyButton->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));
  
  TGTextButton* CancelButton = new TGTextButton(Buttons, "&Cancel", CANCEL);
  Buttons->AddFrame(CancelButton, new TGLayoutHints(kLHintsCenterX, 5, 0, 0, 0));
  CancelButton->Associate(this);
  CancelButton->ChangeOptions(CancelButton->GetOptions() ^ kRaisedFrame);
  CancelButton->SetFont(TEXTBUTTON_FONT, false);
  CancelButton->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  CancelButton->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));  

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  
  TGHorizontal3DLine *ln1 = new TGHorizontal3DLine(Column[0], 30, 2);
  Column[0]->AddFrame(ln1, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 10, 10));

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  StatusFrame = new TGHorizontalFrame(mn_vert, 400, 300);
  mn_vert->AddFrame(StatusFrame, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 7, 0));
  StatusFrame->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  
  TGLabel *labelstatus = new TGLabel(StatusFrame, "Status:");
  StatusFrame->AddFrame(labelstatus, new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 10, 3, 0));
  labelstatus->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  labelstatus->SetTextColor(TColor::RGB2Pixel(TABLE_LABEL_TEXT_R,TABLE_LABEL_TEXT_G,TABLE_LABEL_TEXT_B));

  userstatus = new TGTextEntry(StatusFrame,new TGTextBuffer(30), 10000);
  StatusFrame->AddFrame(userstatus, new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 0, 0, 0));
  userstatus->SetEnabled(kFALSE);
  userstatus->SetFrameDrawn(kFALSE);
  userstatus->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  userstatus->SetText("Ready");
  userstatus->ChangeBackground(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  // userstatus->Resize(100, 16);


  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

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
  userstatus->SetTextColor(TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B), false);
  userstatus->SetText("Waitting...");
  gSystem->ProcessEvents();
}

void Table::PostFunction()
{
  userstatus->SetTextColor(TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B), false);
  userstatus->SetText("Ready");
}
