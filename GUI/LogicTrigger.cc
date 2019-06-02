// LogicTrigger.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 7月 28 18:18:03 2016 (+0800)
// Last-Updated: 日 6月  2 21:00:03 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 151
// URL: http://wuhongyi.cn 

#include "LogicTrigger.hh"
#include "Global.hh"

#include "pixie16app_export.h"

#include <iostream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#define MULTIPLICITYOFFSET 6

LogicTrigger::LogicTrigger(const TGWindow *p, const TGWindow *main, char *name, int columns, int rows,int NumModules)
  : Table(p,main,columns,rows,name, NumModules)
{
  cl0->SetText("ch #");
  for(int i = 0; i < rows; i++)
    {
      Labels[i]->SetText(TString::Format("%2d",i).Data());
    }
  CLabel[0]->SetText("ExtTrigStr[us]");
  CLabel[0]->SetAlignment(kTextCenterX);
  fClient->GetColorByName("blue", color);
  CLabel[0]->SetTextColor(color, false);
  CLabel[1]->SetText("ExtDelLen[us]");
  CLabel[1]->SetAlignment(kTextCenterX);
  fClient->GetColorByName("green", color);
  CLabel[1]->SetTextColor(color, false);
  CLabel[2]->SetText("FtrigoutDel[us]");
  CLabel[2]->SetAlignment(kTextCenterX);
  fClient->GetColorByName("red", color);
  CLabel[2]->SetTextColor(color, false);
  CLabel[3]->SetText("VetoStr[us]");
  CLabel[3]->SetAlignment(kTextCenterX);
  fClient->GetColorByName("purple", color);
  CLabel[3]->SetTextColor(color, false);
  CLabel[4]->SetText("ChaTrigStr[us]");
  CLabel[4]->SetAlignment(kTextCenterX);
  fClient->GetColorByName("blue", color);
  CLabel[4]->SetTextColor(color, false);
  CLabel[5]->SetText("FastTriBaLen[us]");
  CLabel[5]->SetAlignment(kTextCenterX);
  fClient->GetColorByName("red", color);
  CLabel[5]->SetTextColor(color, false);
  CLabel[0]->SetToolTipText("External trigger stretch length", 0);
  CLabel[1]->SetToolTipText("Extern delay", 0);
  CLabel[2]->SetToolTipText("Fast trigger delay length", 0);
  CLabel[3]->SetToolTipText("Veto stretch length", 0);
  CLabel[4]->SetToolTipText("Channel trigger stretch length", 0);
  CLabel[5]->SetToolTipText("Fast trigger stretch length", 0);

  //-----------------------------------

  CLabel[MULTIPLICITYOFFSET+0]->SetText("Left[0-FFFF]");
  CLabel[MULTIPLICITYOFFSET+0]->SetAlignment(kTextCenterX);
  fClient->GetColorByName("orange", color);
  CLabel[MULTIPLICITYOFFSET+0]->SetTextColor(color, false);
  CLabel[MULTIPLICITYOFFSET+1]->SetText("Itself[0-FFFF]");
  CLabel[MULTIPLICITYOFFSET+1]->SetAlignment(kTextCenterX);
  fClient->GetColorByName("orange", color);
  CLabel[MULTIPLICITYOFFSET+1]->SetTextColor(color, false);
  CLabel[MULTIPLICITYOFFSET+2]->SetText("Right[0-FFFF]");
  CLabel[MULTIPLICITYOFFSET+2]->SetAlignment(kTextCenterX);
  fClient->GetColorByName("orange", color);
  CLabel[MULTIPLICITYOFFSET+2]->SetTextColor(color, false);
  CLabel[MULTIPLICITYOFFSET+3]->SetText("ItselfCoin[0-7]");//1st
  CLabel[MULTIPLICITYOFFSET+3]->SetTextColor(color, false);
  CLabel[MULTIPLICITYOFFSET+4]->SetText("RightCoin[0-7]");//2nd
  CLabel[MULTIPLICITYOFFSET+4]->SetTextColor(color, false);
  CLabel[MULTIPLICITYOFFSET+5]->SetText("LeftCoin[0-7]");//3rd
  CLabel[MULTIPLICITYOFFSET+5]->SetTextColor(color, false);
  CLabel[MULTIPLICITYOFFSET+6]->SetText("MultiThre[0-31]");
  CLabel[MULTIPLICITYOFFSET+6]->SetAlignment(kTextCenterX);

  CLabel[MULTIPLICITYOFFSET+7]->SetText("Sel Coin/Multi");
  CLabel[MULTIPLICITYOFFSET+7]->SetAlignment(kTextCenterX);  
  CLabel[MULTIPLICITYOFFSET+8]->SetText("Sel ChValidTrig");
  CLabel[MULTIPLICITYOFFSET+8]->SetAlignment(kTextCenterX);
  fClient->GetColorByName("red", color);
  CLabel[MULTIPLICITYOFFSET+7]->SetTextColor(color, false);
  CLabel[MULTIPLICITYOFFSET+8]->SetTextColor(color, false);
  
  CLabel[MULTIPLICITYOFFSET+0]->SetToolTipText("masking fast triggers from the module's left neighbor", 0);
  CLabel[MULTIPLICITYOFFSET+1]->SetToolTipText("masking fast triggers from the module itself", 0);
  CLabel[MULTIPLICITYOFFSET+2]->SetToolTipText("masking fast triggers from the module's right neighbor", 0);
  CLabel[MULTIPLICITYOFFSET+3]->SetToolTipText("Threshold for 1st 16-bit coincidence trigger", 0);
  CLabel[MULTIPLICITYOFFSET+4]->SetToolTipText("Threshold for 2nd 16-bit coincidence trigger", 0);
  CLabel[MULTIPLICITYOFFSET+5]->SetToolTipText("Threshold for 3rd 16-bit coincidence trigger", 0);
  CLabel[MULTIPLICITYOFFSET+6]->SetToolTipText("Threshold for the multiplicity trigger", 0);
  CLabel[MULTIPLICITYOFFSET+7]->SetToolTipText("Select eithor coincidence trigger(0) or multiplicity trigger(1) as the source of the the channel coincidence/multiplicity trigger", 0);
  CLabel[MULTIPLICITYOFFSET+8]->SetToolTipText("Select channel coincidence/multiplicity trigger(0) or other group trigger(1) as the source of the channel validation trigger", 0);

  for (int i = 1; i < 4; ++i)
    for (int j = 0; j < 16; ++j)
      {
	NumEntry[MULTIPLICITYOFFSET+i][j]->SetFormat(TGNumberFormat::kNESHex);
	NumEntry[MULTIPLICITYOFFSET+i][j]->SetLimits(TGNumberFormat::kNELLimitMinMax,0, 65535);
      }
  


  
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

  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  TGHorizontal3DLine *ln22 = new TGHorizontal3DLine(mn_vert, 200, 2);
  mn_vert->AddFrame(ln22, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 10, 10));

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGHorizontalFrame *Backplane = new TGHorizontalFrame(mn_vert, 400, 300);
  mn_vert->AddFrame(Backplane, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 10));

  TGHorizontalFrame *fVV1_ = new TGHorizontalFrame(Backplane, 10, 10);
  Backplane->AddFrame(fVV1_, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  TGTextEntry *LabelFastTriggerBackplaneLeft = new TGTextEntry(fVV1_,new TGTextBuffer(100));
  LabelFastTriggerBackplaneLeft->SetText("FastTrigBackplaneLeft[0-FFFF]");
  LabelFastTriggerBackplaneLeft->SetEnabled(kFALSE);
  LabelFastTriggerBackplaneLeft->SetAlignment(kTextCenterX);
  // LabelFastTriggerBackplaneLeft->SetToolTipText("", 0);
  LabelFastTriggerBackplaneLeft->Resize(200, 20);
  fVV1_->AddFrame(LabelFastTriggerBackplaneLeft, new TGLayoutHints(kLHintsCenterX, 0, 0, 0, 0));
  fasttriggerbackplaneena[0] = new TGNumberEntryField(fVV1_, 0, 0, TGNumberFormat::kNESHex);
  fasttriggerbackplaneena[0]->SetLimits(TGNumberFormat::kNELLimitMinMax,0, 65535);
  fVV1_->AddFrame(fasttriggerbackplaneena[0], new TGLayoutHints( kLHintsLeft | kLHintsTop, 1, 0, 0, 0));
  
  TGHorizontalFrame *fVV2_ = new TGHorizontalFrame(Backplane, 10, 10);
  Backplane->AddFrame(fVV2_, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  TGTextEntry *LabelFastTriggerBackplaneRight = new TGTextEntry(fVV2_,new TGTextBuffer(100));
  LabelFastTriggerBackplaneRight->SetText("FastTrigBackplaneRight[0-FFFF]");
  LabelFastTriggerBackplaneRight->SetEnabled(kFALSE);
  LabelFastTriggerBackplaneRight->SetAlignment(kTextCenterX);
  // LabelFastTriggerBackplaneRight->SetToolTipText("", 0);
  LabelFastTriggerBackplaneRight->Resize(200, 20);
  fVV2_->AddFrame(LabelFastTriggerBackplaneRight, new TGLayoutHints(kLHintsCenterX, 20, 0, 0, 0));
  fasttriggerbackplaneena[1] = new TGNumberEntryField(fVV2_, 1, 0, TGNumberFormat::kNESHex);
  fasttriggerbackplaneena[1]->SetLimits(TGNumberFormat::kNELLimitMinMax,0, 65535);
  fVV2_->AddFrame(fasttriggerbackplaneena[1], new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 0, 0));


  TGHorizontalFrame *frontpaneloutputframe = new TGHorizontalFrame(Backplane, 10, 10);
  Backplane->AddFrame(frontpaneloutputframe, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  TGTextEntry *LabelFrontPanelOutput = new TGTextEntry(frontpaneloutputframe,new TGTextBuffer(100));
  LabelFrontPanelOutput->SetText("Front Panel Monitor");
  LabelFrontPanelOutput->SetEnabled(kFALSE);
  LabelFrontPanelOutput->SetAlignment(kTextCenterX);
  fClient->GetColorByName("yellow", color);
  LabelFrontPanelOutput->SetBackgroundColor(color);
  // LabelFrontPanelOutput->SetToolTipText("", 0);
  LabelFrontPanelOutput->Resize(200, 20);
  frontpaneloutputframe->AddFrame(LabelFrontPanelOutput, new TGLayoutHints(kLHintsCenterX, 20, 0, 0, 0));


  
  EnableDisableOfTestSignals = new TGComboBox(frontpaneloutputframe);
  frontpaneloutputframe->AddFrame(EnableDisableOfTestSignals, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 0, 0));
  EnableDisableOfTestSignals->Resize(100, 20);
  EnableDisableOfTestSignals->AddEntry("Disable", 1);
  EnableDisableOfTestSignals->AddEntry("Enable", 2);
  EnableDisableOfTestSignals->Select(1);

  GroupOfTestSignals = new TGComboBox(frontpaneloutputframe);
  frontpaneloutputframe->AddFrame(GroupOfTestSignals, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 0, 0));
  GroupOfTestSignals->Resize(100, 20);
  GroupOfTestSignals->AddEntry("Group 000", 1);
  GroupOfTestSignals->AddEntry("Group 001", 2);
  GroupOfTestSignals->Select(1);

  ChannelOfTestSignals = new TGComboBox(frontpaneloutputframe);
  frontpaneloutputframe->AddFrame(ChannelOfTestSignals, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 0, 0));
  ChannelOfTestSignals->Resize(100, 20);
  for (int i = 0; i < 16; ++i)
    {
      ChannelOfTestSignals->AddEntry(TString::Format("Ch %02d",i).Data(), i+1);
    }
  ChannelOfTestSignals->Select(1);

  TestSignals = new TGComboBox(frontpaneloutputframe);
  frontpaneloutputframe->AddFrame(TestSignals, new TGLayoutHints(kLHintsLeft | kLHintsTop, 1, 0, 0, 0));
  TestSignals->Resize(100, 20);
  for (int i = 0; i < 16; ++i)
    {
      TestSignals->AddEntry(TString::Format("TestSig %02d",i).Data(), i+1);
    }
  TestSignals->Select(1);
  
  
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGHorizontalFrame *fVV00000 = new TGHorizontalFrame(mn_vert, 10, 10);
  mn_vert->AddFrame(fVV00000, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  TGTextEntry *textchannelvalidationtrigger = new TGTextEntry(fVV00000,new TGTextBuffer(30));
  fClient->GetColorByName("red", color);
  textchannelvalidationtrigger->SetTextColor(color, false);
  textchannelvalidationtrigger->SetText("Channel Validation Trigger(System FPGA group trigger):  (ExtFastTriGate => 'FT' AND 'Ext_Fast_Trig_In(module fast trigger)') [Choose 'System FPGA' or 'front panel channel GATE' in CSRA]");
  textchannelvalidationtrigger->Resize(1300, 12);
  textchannelvalidationtrigger->SetEnabled(kFALSE);
  textchannelvalidationtrigger->SetFrameDrawn(kFALSE);
  fVV00000->AddFrame(textchannelvalidationtrigger, new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 0, 6, 0));


  
  // TGHorizontalFrame *TrigConfig = new TGHorizontalFrame(mn_vert, 400, 300);
  // mn_vert->AddFrame(TrigConfig, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  // TGVerticalFrame *fV1 = new TGVerticalFrame(TrigConfig, 10, 10);
  // TGVerticalFrame *fV2 = new TGVerticalFrame(TrigConfig, 10, 10);
  // TGVerticalFrame *fV3 = new TGVerticalFrame(TrigConfig, 10, 10);
  // TGVerticalFrame *fV4 = new TGVerticalFrame(TrigConfig, 10, 10);
  // TrigConfig->AddFrame(fV1, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  // TrigConfig->AddFrame(fV2, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  // TrigConfig->AddFrame(fV3, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  // TrigConfig->AddFrame(fV4, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  TGHorizontalFrame *fV1 = new TGHorizontalFrame(mn_vert, 10, 10);
  TGHorizontalFrame *fV2 = new TGHorizontalFrame(mn_vert, 10, 10);
  TGHorizontalFrame *fV3 = new TGHorizontalFrame(mn_vert, 10, 10);
  TGHorizontalFrame *fV4 = new TGHorizontalFrame(mn_vert, 10, 10);
  mn_vert->AddFrame(fV1, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  mn_vert->AddFrame(fV2, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  mn_vert->AddFrame(fV3, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  mn_vert->AddFrame(fV4, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  
  // 0 - 3
  TGTextEntry *ch_0_3 = new TGTextEntry(fV1,new TGTextBuffer(100));
  TGTextEntryAddStyle(ch_0_3,(char *)"Ch 0-3:",(char *)"Select group trigger 0 or external fast trigger gated local fast trigger for channels 0 - 3");
  fV1->AddFrame(ch_0_3, new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
  ChTri_ch0_3 = new TGComboBox(fV1);
  fV1->AddFrame(ChTri_ch0_3, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  ChTri_ch0_3->Resize(100, 20);
  ChTri_ch0_3->AddEntry("ExtFastTriGate", 1);
  ChTri_ch0_3->AddEntry("GroupTri 0", 2);
  ChTri_ch0_3->Select(1);
  
  TGTextEntry *LabelGroupTri0 = new TGTextEntry(fV1,new TGTextBuffer(100));
  TGTextEntryAddStyle(LabelGroupTri0,(char *)"GroupTri 0:",(char *)"Select group trigger 0 from group trigger 0_0,group trigger 0_1,and group trigger 0_2");
  fV1->AddFrame(LabelGroupTri0, new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
  GroupTri0 = new TGComboBox(fV1);
  fV1->AddFrame(GroupTri0, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  GroupTri0->Resize(100, 20);
  GroupTri0->AddEntry("GroupTri 0_0", 1);
  GroupTri0->AddEntry("GroupTri 0_1", 2);
  GroupTri0->AddEntry("GroupTri 0_2", 3);
  GroupTri0->Select(1);
  TGTextEntry *LabelGroupTri0_0 = new TGTextEntry(fV1,new TGTextBuffer(100));
  TGTextEntryAddStyle(LabelGroupTri0_0,(char *)"LocalModule Ch",(char *)"Select source channel from local module for group trigger 0_0");
  fV1->AddFrame(LabelGroupTri0_0, new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
  GroupTri0_0 = new TGComboBox(fV1);
  fV1->AddFrame(GroupTri0_0, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  TGTextEntry *LabelGroupTri0_1 = new TGTextEntry(fV1,new TGTextBuffer(100));
  TGTextEntryAddStyle(LabelGroupTri0_1,(char *)"RightModule Ch",(char *)"Select source channel from right neighbor module for group trigger 0_1");
  fV1->AddFrame(LabelGroupTri0_1, new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
  GroupTri0_1 = new TGComboBox(fV1);
  fV1->AddFrame(GroupTri0_1, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  TGTextEntry *LabelGroupTri0_2 = new TGTextEntry(fV1,new TGTextBuffer(100));
  TGTextEntryAddStyle(LabelGroupTri0_2,(char *)"LeftModule Ch",(char *)"Select source channel from left neighbor module for group trigger 0_2");
  fV1->AddFrame(LabelGroupTri0_2, new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
  GroupTri0_2 = new TGComboBox(fV1);
  fV1->AddFrame(GroupTri0_2, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  GroupTri0_0->Resize(100, 20);
  GroupTri0_1->Resize(100, 20);
  GroupTri0_2->Resize(100, 20);
  for (int i = 0; i < 16; ++i)
    {
      GroupTri0_0->AddEntry(TString::Format("Ch %02d",i).Data(), i+1);
      GroupTri0_1->AddEntry(TString::Format("Ch %02d",i).Data(), i+1);
      GroupTri0_2->AddEntry(TString::Format("Ch %02d",i).Data(), i+1);
    }
  GroupTri0_0->Select(1);
  GroupTri0_1->Select(1);
  GroupTri0_2->Select(1);
  
  // 4 - 7
  TGTextEntry *ch_4_7 = new TGTextEntry(fV2,new TGTextBuffer(100));
  TGTextEntryAddStyle(ch_4_7,(char *)"Ch 4-7:",(char *)"Select group trigger 1 or external fast trigger gated local fast trigger for channels 4 - 7");
  fV2->AddFrame(ch_4_7, new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
  ChTri_ch4_7 = new TGComboBox(fV2);
  fV2->AddFrame(ChTri_ch4_7, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  ChTri_ch4_7->Resize(100, 20);
  ChTri_ch4_7->AddEntry("ExtFastTriGate", 1);
  ChTri_ch4_7->AddEntry("GroupTri 1", 2);
  ChTri_ch4_7->Select(1);
  TGTextEntry *LabelGroupTri1 = new TGTextEntry(fV2,new TGTextBuffer(100));
  TGTextEntryAddStyle(LabelGroupTri1,(char *)"GroupTri 1:",(char *)"Select group trigger 1 from group trigger 1_0,group trigger 1_1,and group trigger 1_2");
  fV2->AddFrame(LabelGroupTri1, new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
  GroupTri1 = new TGComboBox(fV2);
  fV2->AddFrame(GroupTri1, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  GroupTri1->Resize(100, 20);
  GroupTri1->AddEntry("GroupTri 1_0", 1);
  GroupTri1->AddEntry("GroupTri 1_1", 2);
  GroupTri1->AddEntry("GroupTri 1_2", 3);
  GroupTri1->Select(1);
  TGTextEntry *LabelGroupTri1_0 = new TGTextEntry(fV2,new TGTextBuffer(100));
  TGTextEntryAddStyle(LabelGroupTri1_0,(char *)"LocalModule Ch",(char *)"Select source channel from local module for group trigger 1_0");
  fV2->AddFrame(LabelGroupTri1_0, new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
  GroupTri1_0 = new TGComboBox(fV2);
  fV2->AddFrame(GroupTri1_0, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  TGTextEntry *LabelGroupTri1_1 = new TGTextEntry(fV2,new TGTextBuffer(100));
  TGTextEntryAddStyle(LabelGroupTri1_1,(char *)"RightModule Ch",(char *)"Select source channel from right neighbor module for group trigger 1_1");
  fV2->AddFrame(LabelGroupTri1_1, new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
  GroupTri1_1 = new TGComboBox(fV2);
  fV2->AddFrame(GroupTri1_1, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  TGTextEntry *LabelGroupTri1_2 = new TGTextEntry(fV2,new TGTextBuffer(100));
  TGTextEntryAddStyle(LabelGroupTri1_2,(char *)"LeftModule Ch",(char *)"Select source channel from left neighbor module for group trigger 1_2");
  fV2->AddFrame(LabelGroupTri1_2, new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
  GroupTri1_2 = new TGComboBox(fV2);
  fV2->AddFrame(GroupTri1_2, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  GroupTri1_0->Resize(100, 20);
  GroupTri1_1->Resize(100, 20);
  GroupTri1_2->Resize(100, 20);
  for (int i = 0; i < 16; ++i)
    {
      GroupTri1_0->AddEntry(TString::Format("Ch %02d",i).Data(), i+1);
      GroupTri1_1->AddEntry(TString::Format("Ch %02d",i).Data(), i+1);
      GroupTri1_2->AddEntry(TString::Format("Ch %02d",i).Data(), i+1);
    }
  GroupTri1_0->Select(1);
  GroupTri1_1->Select(1);
  GroupTri1_2->Select(1);
  

  // 8 - 11
  TGTextEntry *ch_8_11 = new TGTextEntry(fV3,new TGTextBuffer(100));
  TGTextEntryAddStyle(ch_8_11,(char *)"Ch 8-11:",(char *)"Select group trigger 2 or external fast trigger gated local fast trigger for channels 8 - 11");
  fV3->AddFrame(ch_8_11, new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
  ChTri_ch8_11 = new TGComboBox(fV3);
  fV3->AddFrame(ChTri_ch8_11, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  ChTri_ch8_11->Resize(100, 20);
  ChTri_ch8_11->AddEntry("ExtFastTriGate", 1);
  ChTri_ch8_11->AddEntry("GroupTri 2", 2);
  ChTri_ch8_11->Select(1);  
  TGTextEntry *LabelGroupTri2 = new TGTextEntry(fV3,new TGTextBuffer(100));
  TGTextEntryAddStyle(LabelGroupTri2,(char *)"GroupTri 2:",(char *)"Select group trigger 2 from group trigger 2_0,group trigger 2_1,and group trigger 2_2");
  fV3->AddFrame(LabelGroupTri2, new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
  GroupTri2 = new TGComboBox(fV3);
  fV3->AddFrame(GroupTri2, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  GroupTri2->Resize(100, 20);
  GroupTri2->AddEntry("GroupTri 2_0", 1);
  GroupTri2->AddEntry("GroupTri 2_1", 2);
  GroupTri2->AddEntry("GroupTri 2_2", 3);
  GroupTri2->Select(1);
  TGTextEntry *LabelGroupTri2_0 = new TGTextEntry(fV3,new TGTextBuffer(100));
  TGTextEntryAddStyle(LabelGroupTri2_0,(char *)"LocalModule Ch",(char *)"Select source channel from local module for group trigger 2_0");
  fV3->AddFrame(LabelGroupTri2_0, new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
  GroupTri2_0 = new TGComboBox(fV3);
  fV3->AddFrame(GroupTri2_0, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  TGTextEntry *LabelGroupTri2_1 = new TGTextEntry(fV3,new TGTextBuffer(100));
  TGTextEntryAddStyle(LabelGroupTri2_1,(char *)"RightModule Ch",(char *)"Select source channel from right neighbor module for group trigger 2_1");
  fV3->AddFrame(LabelGroupTri2_1, new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
  GroupTri2_1 = new TGComboBox(fV3);
  fV3->AddFrame(GroupTri2_1, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  TGTextEntry *LabelGroupTri2_2 = new TGTextEntry(fV3,new TGTextBuffer(100));
  TGTextEntryAddStyle(LabelGroupTri2_2,(char *)"LeftModule Ch",(char *)"Select source channel from left neighbor module for group trigger 2_2");
  fV3->AddFrame(LabelGroupTri2_2, new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
  GroupTri2_2 = new TGComboBox(fV3);
  fV3->AddFrame(GroupTri2_2, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  GroupTri2_0->Resize(100, 20);
  GroupTri2_1->Resize(100, 20);
  GroupTri2_2->Resize(100, 20);
  for (int i = 0; i < 16; ++i)
    {
      GroupTri2_0->AddEntry(TString::Format("Ch %02d",i).Data(), i+1);
      GroupTri2_1->AddEntry(TString::Format("Ch %02d",i).Data(), i+1);
      GroupTri2_2->AddEntry(TString::Format("Ch %02d",i).Data(), i+1);
    }
  GroupTri2_0->Select(1);
  GroupTri2_1->Select(1);
  GroupTri2_2->Select(1);

  
  // 12 -15
  TGTextEntry *ch_12_15 = new TGTextEntry(fV4,new TGTextBuffer(100));
  TGTextEntryAddStyle(ch_12_15,(char *)"Ch 12-15:",(char *)"Select group trigger 3 or external fast trigger gated local fast trigger for channels 12 - 15");
  fV4->AddFrame(ch_12_15, new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
  ChTri_ch12_15 = new TGComboBox(fV4);
  fV4->AddFrame(ChTri_ch12_15, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  ChTri_ch12_15->Resize(100, 20);
  ChTri_ch12_15->AddEntry("ExtFastTriGate", 1);
  ChTri_ch12_15->AddEntry("GroupTri 3", 2);
  ChTri_ch12_15->Select(1);  
  TGTextEntry *LabelGroupTri3 = new TGTextEntry(fV4,new TGTextBuffer(100));
  TGTextEntryAddStyle(LabelGroupTri3,(char *)"GroupTri 3:",(char *)"Select group trigger 3 from group trigger 3_0,group trigger 3_1,and group trigger 3_2");
  fV4->AddFrame(LabelGroupTri3, new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
  GroupTri3 = new TGComboBox(fV4);
  fV4->AddFrame(GroupTri3, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  GroupTri3->Resize(100, 20);
  GroupTri3->AddEntry("GroupTri 3_0", 1);
  GroupTri3->AddEntry("GroupTri 3_1", 2);
  GroupTri3->AddEntry("GroupTri 3_2", 3);
  GroupTri3->Select(1);
  TGTextEntry *LabelGroupTri3_0 = new TGTextEntry(fV4,new TGTextBuffer(100));
  TGTextEntryAddStyle(LabelGroupTri3_0,(char *)"LocalModule Ch",(char *)"Select source channel from local module for group trigger 3_0");
  fV4->AddFrame(LabelGroupTri3_0, new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
  GroupTri3_0 = new TGComboBox(fV4);
  fV4->AddFrame(GroupTri3_0, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  TGTextEntry *LabelGroupTri3_1 = new TGTextEntry(fV4,new TGTextBuffer(100));
  TGTextEntryAddStyle(LabelGroupTri3_1,(char *)"RightModule Ch",(char *)"Select source channel from right neighbor module for group trigger 3_1");
  fV4->AddFrame(LabelGroupTri3_1, new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
  GroupTri3_1 = new TGComboBox(fV4);
  fV4->AddFrame(GroupTri3_1, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  TGTextEntry *LabelGroupTri3_2 = new TGTextEntry(fV4,new TGTextBuffer(100));
  TGTextEntryAddStyle(LabelGroupTri3_2,(char *)"LeftModule Ch",(char *)"Select source channel from left neighbor module for group trigger 3_2");
  fV4->AddFrame(LabelGroupTri3_2, new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
  GroupTri3_2 = new TGComboBox(fV4);
  fV4->AddFrame(GroupTri3_2, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  GroupTri3_0->Resize(100, 20);
  GroupTri3_1->Resize(100, 20);
  GroupTri3_2->Resize(100, 20);
  for (int i = 0; i < 16; ++i)
    {
      GroupTri3_0->AddEntry(TString::Format("Ch %02d",i).Data(), i+1);
      GroupTri3_1->AddEntry(TString::Format("Ch %02d",i).Data(), i+1);
      GroupTri3_2->AddEntry(TString::Format("Ch %02d",i).Data(), i+1);
    }
  GroupTri3_0->Select(1);
  GroupTri3_1->Select(1);
  GroupTri3_2->Select(1);

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGHorizontal3DLine *ln222 = new TGHorizontal3DLine(mn_vert, 200, 2);
  mn_vert->AddFrame(ln222, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 10, 10));


  int intlengthlabelmoduletrigger = 160;
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGHorizontalFrame *fVV00 = new TGHorizontalFrame(mn_vert, 10, 10);
  mn_vert->AddFrame(fVV00, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  TGTextEntry *textmodulefasttrigger = new TGTextEntry(fVV00,new TGTextBuffer(30));
  fClient->GetColorByName("blue", color);
  textmodulefasttrigger->SetTextColor(color, false);
  textmodulefasttrigger->SetText("Module Fast Trigger:");
  textmodulefasttrigger->Resize(200, 12);
  textmodulefasttrigger->SetEnabled(kFALSE);
  textmodulefasttrigger->SetFrameDrawn(kFALSE);
  fVV00->AddFrame(textmodulefasttrigger, new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 0, 6, 0));

  TGHorizontalFrame *fVV4 = new TGHorizontalFrame(mn_vert, 10, 10);
  mn_vert->AddFrame(fVV4, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  TGTextEntry *LabelGlobalFastTrigger = new TGTextEntry(fVV4,new TGTextBuffer(100));
  TGTextEntryAddStyle(LabelGlobalFastTrigger,(char *)"Source",(char *)"Select source for global fast trigger");
  LabelGlobalFastTrigger->Resize(intlengthlabelmoduletrigger, 20);
  fVV4->AddFrame(LabelGlobalFastTrigger, new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
  GlobalFastTrigger = new TGComboBox(fVV4);
  fVV4->AddFrame(GlobalFastTrigger, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  GlobalFastTrigger->Resize(500, 20);
  GlobalFastTrigger->AddEntry("current module (Ext_FastTrig_In)", 1);
  GlobalFastTrigger->AddEntry("a master module in the crate through the backplane (FT_LocalCrate_BP)", 2);
  GlobalFastTrigger->AddEntry("trigger module between multiple crates (FT_In_BP)", 3);
  GlobalFastTrigger->AddEntry("a wired or bus line on the backplane (FT_WiredOr)", 4);
  GlobalFastTrigger->Select(1);  

  TGHorizontalFrame *fVV2 = new TGHorizontalFrame(mn_vert, 10, 10);
  mn_vert->AddFrame(fVV2, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  TGTextEntry *LabelExternalFastTrigger = new TGTextEntry(fVV2,new TGTextBuffer(100));
  TGTextEntryAddStyle(LabelExternalFastTrigger,(char *)"Current Module",(char *)"Select source for external fast trigger");
  LabelExternalFastTrigger->Resize(intlengthlabelmoduletrigger, 20);
  fVV2->AddFrame(LabelExternalFastTrigger, new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
  ExternalFastTrigger = new TGComboBox(fVV2);
  fVV2->AddFrame(ExternalFastTrigger, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  ExternalFastTrigger->Resize(320, 20);
  ExternalFastTrigger->AddEntry("Ext_FastTrig_Sel(front panel TTL)", 1);
  ExternalFastTrigger->AddEntry("Int_FastTrig_Sgl(1 out of 16 fast trigger)", 2);
  ExternalFastTrigger->AddEntry("FTIN_Or(OR of 16 fast trigger)", 3);
  ExternalFastTrigger->AddEntry("LVDS_ValidTrig_FP(front panel Ethernet cables)", 4);
  ExternalFastTrigger->AddEntry("ChanTrig_Sel(1 out of 16 valid trigger)", 5);
  ExternalFastTrigger->Select(1);

  TGTextEntry *LabelExternalFastTriggerNotes = new TGTextEntry(fVV2,new TGTextBuffer(100));
  LabelExternalFastTriggerNotes->Resize(600, 20);
  fVV2->AddFrame(LabelExternalFastTriggerNotes, new TGLayoutHints(kLHintsLeft, 10, 0, 3, 0));  
  LabelExternalFastTriggerNotes->SetText("FT_LocalCrate_BP from MODCSRB[6]=1 module / FT_In_BP from MODCSRB[4]=1 module");
  LabelExternalFastTriggerNotes->SetEnabled(kFALSE);
  LabelExternalFastTriggerNotes->SetFrameDrawn(kFALSE);

  
  TGHorizontalFrame *fVV1 = new TGHorizontalFrame(mn_vert, 10, 10);
  mn_vert->AddFrame(fVV1, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  TGTextEntry *LabelInternalFastTrigger = new TGTextEntry(fVV1,new TGTextBuffer(100));
  TGTextEntryAddStyle(LabelInternalFastTrigger,(char *)"Int_FastTrig_Sgl",(char *)"Select source channel for the internal fast trigger from one of the 16 local channels(0-15)");
  LabelInternalFastTrigger->Resize(intlengthlabelmoduletrigger, 20);
  fVV1->AddFrame(LabelInternalFastTrigger, new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
  InternalFastTrigger = new TGComboBox(fVV1);
  fVV1->AddFrame(InternalFastTrigger, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  InternalFastTrigger->Resize(100, 20);
  for (int i = 0; i < 16; ++i)
    {
      InternalFastTrigger->AddEntry(TString::Format("Ch %02d",i).Data(), i+1);
    }
  InternalFastTrigger->Select(1);


  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  TGHorizontal3DLine *ln333 = new TGHorizontal3DLine(mn_vert, 200, 2);
  mn_vert->AddFrame(ln333, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 10, 10));

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  TGHorizontalFrame *fVV000 = new TGHorizontalFrame(mn_vert, 10, 10);
  mn_vert->AddFrame(fVV000, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  TGTextEntry *textmodulevalidationtrigger = new TGTextEntry(fVV000,new TGTextBuffer(30));
  fClient->GetColorByName("blue", color);
  textmodulevalidationtrigger->SetTextColor(color, false);
  textmodulevalidationtrigger->SetText("Module Validation Trigger(System FPGA):  [Choose 'System FPGA' or 'front panel module GATE' in CSRA]");
  textmodulevalidationtrigger->Resize(1000, 12);
  textmodulevalidationtrigger->SetEnabled(kFALSE);
  textmodulevalidationtrigger->SetFrameDrawn(kFALSE);
  fVV000->AddFrame(textmodulevalidationtrigger, new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 0, 6, 0));
  
  TGHorizontalFrame *fVV5 = new TGHorizontalFrame(mn_vert, 10, 10);
  mn_vert->AddFrame(fVV5, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  TGTextEntry *LabelGlobalValidationTrigger = new TGTextEntry(fVV5,new TGTextBuffer(100));
  TGTextEntryAddStyle(LabelGlobalValidationTrigger,(char *)"source",(char *)"Select source for global validation trigger");
  LabelGlobalValidationTrigger->Resize(intlengthlabelmoduletrigger, 20);
  fVV5->AddFrame(LabelGlobalValidationTrigger, new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
  GlobalValidationTrigger = new TGComboBox(fVV5);
  fVV5->AddFrame(GlobalValidationTrigger, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  GlobalValidationTrigger->Resize(500, 20);
  GlobalValidationTrigger->AddEntry("current module (Ext_VaildTrig_In)", 1);
  GlobalValidationTrigger->AddEntry("a master module in the crate through the backplane (ET_LocalCrate_BP)", 2);
  GlobalValidationTrigger->AddEntry("trigger module between multiple crates (ET_In_BP)", 3);
  GlobalValidationTrigger->AddEntry("a wired or bus line on the backplane (ET_WiredOr)", 4);
  GlobalValidationTrigger->Select(1);  

  TGHorizontalFrame *fVV6 = new TGHorizontalFrame(mn_vert, 10, 10);
  mn_vert->AddFrame(fVV6, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  TGTextEntry *LabelExternalValidationTrigger = new TGTextEntry(fVV6,new TGTextBuffer(100));
  TGTextEntryAddStyle(LabelExternalValidationTrigger,(char *)"Current Module",(char *)"Select source for external validation trigger");
  LabelExternalValidationTrigger->Resize(intlengthlabelmoduletrigger, 20);
  fVV6->AddFrame(LabelExternalValidationTrigger, new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
  ExternalValidationTrigger = new TGComboBox(fVV6);
  fVV6->AddFrame(ExternalValidationTrigger, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  ExternalValidationTrigger->Resize(320, 20);
  ExternalValidationTrigger->AddEntry("Ext_ValidTrig_Sel(front panel TTL)", 1);
  ExternalValidationTrigger->AddEntry("Int_ValidTrig_Sgl(1 out of 16 fast trigger)", 2);
  ExternalValidationTrigger->AddEntry("FTIN_Or(OR of 16 fast trigger)", 3);
  ExternalValidationTrigger->AddEntry("LVDS_ValidTrig_FP(front panel Ethernet cables)", 4);
  ExternalValidationTrigger->AddEntry("ChanTrig_Sel(1 out of 16 valid trigger)", 5);
  ExternalValidationTrigger->Select(1);

  TGTextEntry *LabelExternalValidationTriggerNotes = new TGTextEntry(fVV6,new TGTextBuffer(100));
  LabelExternalValidationTriggerNotes->Resize(600, 20);
  fVV6->AddFrame(LabelExternalValidationTriggerNotes, new TGLayoutHints(kLHintsLeft, 10, 0, 3, 0));  
  LabelExternalValidationTriggerNotes->SetText("ET_LocalCrate_BP from MODCSRB[6]=1 module / ET_In_BP from MODCSRB[4]=1 module");
  LabelExternalValidationTriggerNotes->SetEnabled(kFALSE);
  LabelExternalValidationTriggerNotes->SetFrameDrawn(kFALSE);

  
  
  TGHorizontalFrame *fVV3 = new TGHorizontalFrame(mn_vert, 10, 10);
  mn_vert->AddFrame(fVV3, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  TGTextEntry *LabelInternalValidationTrigger = new TGTextEntry(fVV3,new TGTextBuffer(100));
  TGTextEntryAddStyle(LabelInternalValidationTrigger,(char *)"Int_ValidTrig_Sgl",(char *)"Select source channel for the internal validation trigger from one of the 16 local channels(0-15)");
  LabelInternalValidationTrigger->Resize(intlengthlabelmoduletrigger, 20);
  fVV3->AddFrame(LabelInternalValidationTrigger, new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
  InternalValidationTrigger = new TGComboBox(fVV3);
  fVV3->AddFrame(InternalValidationTrigger, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  InternalValidationTrigger->Resize(100, 20);
  for (int i = 0; i < 16; ++i)
    {
      InternalValidationTrigger->AddEntry(TString::Format("Ch %02d",i).Data(), i+1);
    }
  InternalValidationTrigger->Select(1);
  

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  TGHorizontal3DLine *ln444 = new TGHorizontal3DLine(mn_vert, 200, 2);
  mn_vert->AddFrame(ln444, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 10, 10));

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TGHorizontalFrame *fVV0000 = new TGHorizontalFrame(mn_vert, 10, 10);
  mn_vert->AddFrame(fVV0000, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  TGTextEntry *textmoduletriggerchantrigsel = new TGTextEntry(fVV0000,new TGTextBuffer(30));
  fClient->GetColorByName("blue", color);
  textmoduletriggerchantrigsel->SetTextColor(color, false);
  textmoduletriggerchantrigsel->SetText("Module Fast/Validation Trigger ChanTrig_Sel:");
  textmoduletriggerchantrigsel->Resize(300, 12);
  textmoduletriggerchantrigsel->SetEnabled(kFALSE);
  textmoduletriggerchantrigsel->SetFrameDrawn(kFALSE);
  fVV0000->AddFrame(textmoduletriggerchantrigsel, new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 0, 6, 0));
  
  TGHorizontalFrame *fVV0 = new TGHorizontalFrame(mn_vert, 10, 10);
  mn_vert->AddFrame(fVV0, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  TGTextEntry *LabelChannelTrigger = new TGTextEntry(fVV0,new TGTextBuffer(100));
  TGTextEntryAddStyle(LabelChannelTrigger,(char *)"Channel Trigger:",(char *)"Select source channel for the channel trigger from one of 16 channel triggers");
  LabelChannelTrigger->Resize(intlengthlabelmoduletrigger, 20);
  fVV0->AddFrame(LabelChannelTrigger, new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
  ChannelTrigger = new TGComboBox(fVV0);
  fVV0->AddFrame(ChannelTrigger, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  ChannelTrigger->Resize(100, 20);
  for (int i = 0; i < 16; ++i)
    {
      ChannelTrigger->AddEntry(TString::Format("Ch %02d",i).Data(), i+1);
    }
  ChannelTrigger->Select(1);

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  TGHorizontal3DLine *ln555 = new TGHorizontal3DLine(mn_vert, 200, 2);
  mn_vert->AddFrame(ln555, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 10, 10));

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  
  TGHorizontalFrame *fVV000000 = new TGHorizontalFrame(mn_vert, 10, 10);
  mn_vert->AddFrame(fVV000000, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  TGTextEntry *textaboutveto = new TGTextEntry(fVV000000,new TGTextBuffer(30));
  fClient->GetColorByName("purple", color);
  textaboutveto->SetTextColor(color, false);
  textaboutveto->SetText("Module/Channel Veto: [Choose 'front panel module/channel GATE' or 'module/channel vilidation trigger' in CSRA, channel veto need choose enable/disable]");
  textaboutveto->Resize(1300, 12);
  textaboutveto->SetEnabled(kFALSE);
  textaboutveto->SetFrameDrawn(kFALSE);
  fVV000000->AddFrame(textaboutveto, new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 0, 6, 0));
  


  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    
  MapSubwindows();
  Resize();			// resize to default size

  chanNumber = 0;
  modNumber = 0;
  Load_Once = true;

  exttrigstretch = 0;
  externdelaylen = 0;
  ftrigoutdelay = 0;
  vetostretch = 0;
  chantrigstretch = 0;
}

LogicTrigger::~LogicTrigger()
{

}

Bool_t LogicTrigger::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
  PreFunction();
  
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
		  load_info(modNumber);
		}
	      else
		std::cout << "please load once first !"<<std::endl;
	      break;
	    case CANCEL:		/// Cancel Button
	      DeleteWindow();
	      break;
	      /////////////////////////////
	    case (COPYBUTTON+1000):
	      exttrigstretch = NumEntry[1][chanNumber]->GetNumber();
	      externdelaylen = NumEntry[2][chanNumber]->GetNumber();
	      ftrigoutdelay = NumEntry[3][chanNumber]->GetNumber();
	      vetostretch = NumEntry[4][chanNumber]->GetNumber();
	      chantrigstretch = NumEntry[5][chanNumber]->GetNumber();
	      fasttrigbacklen = NumEntry[6][chanNumber]->GetNumber();
	      
	      MultiLeft = NumEntry[MULTIPLICITYOFFSET+1][chanNumber]->GetNumber();
	      MultiItself = NumEntry[MULTIPLICITYOFFSET+2][chanNumber]->GetNumber();
	      MultiRight = NumEntry[MULTIPLICITYOFFSET+3][chanNumber]->GetNumber();
	      Multi1st = NumEntry[MULTIPLICITYOFFSET+4][chanNumber]->GetNumber();
	      Multi2nd = NumEntry[MULTIPLICITYOFFSET+5][chanNumber]->GetNumber();
	      Multi3rd = NumEntry[MULTIPLICITYOFFSET+6][chanNumber]->GetNumber();
	      MultiThreshold = NumEntry[MULTIPLICITYOFFSET+7][chanNumber]->GetNumber();
	      SelCoinMulti = NumEntry[MULTIPLICITYOFFSET+8][chanNumber]->GetNumber();
	      SelChValidTrig = NumEntry[MULTIPLICITYOFFSET+9][chanNumber]->GetNumber();
	      
	      for(int i = 0; i < 16; i++)
		{
		  if(i != chanNumber)
		    {
		      NumEntry[1][i]->SetText(TString::Format("%1.3f",exttrigstretch).Data());
		      NumEntry[2][i]->SetText(TString::Format("%1.3f",externdelaylen).Data());
		      NumEntry[3][i]->SetText(TString::Format("%1.3f",ftrigoutdelay).Data());
		      NumEntry[4][i]->SetText(TString::Format("%1.3f",vetostretch).Data());
		      NumEntry[5][i]->SetText(TString::Format("%1.3f",chantrigstretch).Data());
		      NumEntry[6][i]->SetText(TString::Format("%1.3f",fasttrigbacklen).Data());
		      
	      	      NumEntry[MULTIPLICITYOFFSET+1][i]->SetText(TString::Format("%04X",MultiLeft).Data());
	      	      NumEntry[MULTIPLICITYOFFSET+2][i]->SetText(TString::Format("%04X",MultiItself).Data());
	      	      NumEntry[MULTIPLICITYOFFSET+3][i]->SetText(TString::Format("%04X",MultiRight).Data());
	      	      NumEntry[MULTIPLICITYOFFSET+4][i]->SetText(TString::Format("%d",Multi1st).Data());
	      	      NumEntry[MULTIPLICITYOFFSET+5][i]->SetText(TString::Format("%d",Multi2nd).Data());
	      	      NumEntry[MULTIPLICITYOFFSET+6][i]->SetText(TString::Format("%d",Multi3rd).Data());
	      	      NumEntry[MULTIPLICITYOFFSET+7][i]->SetText(TString::Format("%d",MultiThreshold).Data());
	      	      NumEntry[MULTIPLICITYOFFSET+8][i]->SetText(TString::Format("%d",SelCoinMulti).Data());
	      	      NumEntry[MULTIPLICITYOFFSET+9][i]->SetText(TString::Format("%d",SelChValidTrig).Data());
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

  PostFunction();
  return kTRUE;
}

int LogicTrigger::load_info(Long_t mod)
{
  double ChanParData = -1;
  unsigned int ModParData;
  unsigned int b;
  int retval;
  unsigned short gt;
  
  for (int i = 0; i < 16; i++)
    {
      retval = Pixie16ReadSglChanPar((char*)"ExtTrigStretch", &ChanParData, mod, i);
      if(retval < 0) ErrorInfo("LogicTrigger.cc", "load_info(...)", "Pixie16ReadSglChanPar/ExtTrigStretch", retval);
      NumEntry[1][i]->SetText(TString::Format("%1.3f", ChanParData).Data());

      retval = Pixie16ReadSglChanPar((char*)"ExternDelayLen", &ChanParData, mod, i);
      if(retval < 0) ErrorInfo("LogicTrigger.cc", "load_info(...)", "Pixie16ReadSglChanPar/ExternDelayLen", retval);
      NumEntry[2][i]->SetText(TString::Format("%1.3f", ChanParData).Data());

      retval = Pixie16ReadSglChanPar((char*)"FtrigoutDelay", &ChanParData, mod, i);
      if(retval < 0) ErrorInfo("LogicTrigger.cc", "load_info(...)", "Pixie16ReadSglChanPar/FtrigoutDelay", retval);    
      NumEntry[3][i]->SetText(TString::Format("%1.3f", ChanParData).Data());

      retval = Pixie16ReadSglChanPar((char*)"VetoStretch", &ChanParData, mod, i);
      if(retval < 0) ErrorInfo("LogicTrigger.cc", "load_info(...)", "Pixie16ReadSglChanPar/VetoStretch", retval);
      NumEntry[4][i]->SetText(TString::Format("%1.3f", ChanParData).Data());

      retval = Pixie16ReadSglChanPar((char*)"ChanTrigStretch", &ChanParData, mod, i);
      if(retval < 0) ErrorInfo("LogicTrigger.cc", "load_info(...)", "Pixie16ReadSglChanPar/ChanTrigStretch", retval);
      NumEntry[5][i]->SetText(TString::Format("%1.3f", ChanParData).Data());

      retval = Pixie16ReadSglChanPar((char*)"FASTTRIGBACKLEN", &ChanParData, mod, i);
      if(retval < 0) ErrorInfo("LogicTrigger.cc", "load_info(...)", "Pixie16ReadSglChanPar/FASTTRIGBACKLEN", retval);     
      NumEntry[6][i]->SetText(TString::Format("%1.3f", ChanParData).Data());
    }

  //=============================

  retval = Pixie16ReadSglModPar((char*)"TrigConfig0", &ModParData, mod);
  if(retval < 0) ErrorInfo("LogicTrigger.cc", "load_info(...)", "Pixie16ReadSglChanPar/TrigConfig0", retval);
  gt = APP32_TstBit(15, ModParData);
  EnableDisableOfTestSignals->Select(gt+1);

  GroupOfTestSignals->Select((APP32_TstBit(14, ModParData) << 2) + (APP32_TstBit(13, ModParData) << 1) + APP32_TstBit(12, ModParData) + 1);
  
  ChannelOfTestSignals->Select((APP32_TstBit(19, ModParData) << 3) + (APP32_TstBit(18, ModParData) << 2) + (APP32_TstBit(17, ModParData) << 1) + APP32_TstBit(16, ModParData) + 1);

  TestSignals->Select((APP32_TstBit(23, ModParData) << 3) + (APP32_TstBit(22, ModParData) << 2) + (APP32_TstBit(21, ModParData) << 1) + APP32_TstBit(20, ModParData) + 1);
  
  //=============================

  unsigned int BackplaneEna = 0; 
  retval = Pixie16ReadSglModPar((char*)"FastTrigBackplaneEna", &BackplaneEna, mod);
  if(retval < 0) ErrorInfo("LogicTrigger.cc", "load_info(...)", "Pixie16ReadSglModPar/FastTrigBackplaneEna", retval);
  b = 0;
  for (int j = 0; j < 16; j++)
    {
      b += (APP32_TstBit(j, BackplaneEna)<<j);
    }
  fasttriggerbackplaneena[0]->SetText(TString::Format("%04X", b).Data());
  b = 0;
  for (int j = 0; j < 16; j++)
    {
      b += (APP32_TstBit(j+16, BackplaneEna)<<j);
    }
  fasttriggerbackplaneena[1]->SetText(TString::Format("%04X", b).Data());
  
  
  for (int i = 0; i < 16; i++)
    {
      retval = Pixie16ReadSglChanPar((char*)"MultiplicityMaskL", &ChanParData, mod, i);
      if(retval < 0) ErrorInfo("LogicTrigger.cc", "load_info(...)", "Pixie16ReadSglChanPar/MultiplicityMaskL", retval);

      b = 0;
      for (int j = 0; j < 16; j++)
	{
	  b += (APP32_TstBit(j, ChanParData)<<j);
	}
      NumEntry[MULTIPLICITYOFFSET+2][i]->SetText(TString::Format("%04X", b).Data());

      b = 0;
      for (int j = 0; j < 16; j++)
	{
	  b += (APP32_TstBit(j+16, ChanParData)<<j);
	}
      NumEntry[MULTIPLICITYOFFSET+3][i]->SetText(TString::Format("%04X", b).Data());

      retval = Pixie16ReadSglChanPar((char*)"MultiplicityMaskH", &ChanParData, mod, i);
      if(retval < 0) ErrorInfo("LogicTrigger.cc", "load_info(...)", "Pixie16ReadSglChanPar/MultiplicityMaskH", retval);

      b = 0;
      for (int j = 0; j < 16; j++)
	{
	  b += (APP32_TstBit(j, ChanParData)<<j);
	}
      NumEntry[MULTIPLICITYOFFSET+1][i]->SetText(TString::Format("%04X", b).Data());

      b = (APP32_TstBit(24, ChanParData)<<2)+(APP32_TstBit(23, ChanParData)<<1)+APP32_TstBit(22, ChanParData);
      NumEntry[MULTIPLICITYOFFSET+4][i]->SetText(TString::Format("%d", b).Data());
      
      b = (APP32_TstBit(27, ChanParData)<<2)+(APP32_TstBit(26, ChanParData)<<1)+APP32_TstBit(25, ChanParData);
      NumEntry[MULTIPLICITYOFFSET+5][i]->SetText(TString::Format("%d", b).Data());

      b = (APP32_TstBit(30, ChanParData)<<2)+(APP32_TstBit(29, ChanParData)<<1)+APP32_TstBit(28, ChanParData);
      NumEntry[MULTIPLICITYOFFSET+6][i]->SetText(TString::Format("%d", b).Data());      

      b = (APP32_TstBit(21, ChanParData)<<4)+(APP32_TstBit(20, ChanParData)<<3)+(APP32_TstBit(19, ChanParData)<<2)+(APP32_TstBit(18, ChanParData)<<1)+APP32_TstBit(17, ChanParData);
      NumEntry[MULTIPLICITYOFFSET+7][i]->SetText(TString::Format("%d", b).Data());  

      b = APP32_TstBit(16, ChanParData);
      NumEntry[MULTIPLICITYOFFSET+8][i]->SetText(TString::Format("%d", b).Data());
      
      b = APP32_TstBit(31, ChanParData);
      NumEntry[MULTIPLICITYOFFSET+9][i]->SetText(TString::Format("%d", b).Data());      
    }

  //=============================
  
  retval = Pixie16ReadSglModPar((char*)"TrigConfig0", &ModParData, mod);
  if(retval < 0) ErrorInfo("LogicTrigger.cc", "load_info(...)", "Pixie16ReadSglModPar/TrigConfig0", retval);    
  InternalFastTrigger->Select((APP32_TstBit(3, ModParData) << 3) + (APP32_TstBit(2, ModParData) << 2) + (APP32_TstBit(1, ModParData) << 1) + APP32_TstBit(0, ModParData) + 1);
  ExternalFastTrigger->Select((APP32_TstBit(7, ModParData) << 3) + (APP32_TstBit(6, ModParData) << 2) + (APP32_TstBit(5, ModParData) << 1) + APP32_TstBit(4, ModParData) + 1);
  InternalValidationTrigger->Select((APP32_TstBit(11, ModParData) << 3) + (APP32_TstBit(10, ModParData) << 2) + (APP32_TstBit(9, ModParData) << 1) + APP32_TstBit(8, ModParData) + 1);
  GlobalFastTrigger->Select((APP32_TstBit(25, ModParData) << 1) + APP32_TstBit(24, ModParData) + 1);
  GlobalValidationTrigger->Select((APP32_TstBit(27, ModParData) << 1) + APP32_TstBit(26, ModParData) + 1);
  ExternalValidationTrigger->Select((APP32_TstBit(31, ModParData) << 3) + (APP32_TstBit(30, ModParData) << 2) + (APP32_TstBit(29, ModParData) << 1) + APP32_TstBit(28, ModParData) + 1);

  retval = Pixie16ReadSglModPar((char*)"TrigConfig1", &ModParData, mod);
  if(retval < 0) ErrorInfo("LogicTrigger.cc", "load_info(...)", "Pixie16ReadSglModPar/TrigConfig1", retval);
  GroupTri0_0->Select((APP32_TstBit(3, ModParData) << 3) + (APP32_TstBit(2, ModParData) << 2) + (APP32_TstBit(1, ModParData) << 1) + APP32_TstBit(0, ModParData) + 1);
  GroupTri0_1->Select((APP32_TstBit(7, ModParData) << 3) + (APP32_TstBit(6, ModParData) << 2) + (APP32_TstBit(5, ModParData) << 1) + APP32_TstBit(4, ModParData) + 1);
  GroupTri0_2->Select((APP32_TstBit(11, ModParData) << 3) + (APP32_TstBit(10, ModParData) << 2) + (APP32_TstBit(9, ModParData) << 1) + APP32_TstBit(8, ModParData) + 1);
  GroupTri1_0->Select((APP32_TstBit(15, ModParData) << 3) + (APP32_TstBit(14, ModParData) << 2) + (APP32_TstBit(13, ModParData) << 1) + APP32_TstBit(12, ModParData) + 1);
  GroupTri1_1->Select((APP32_TstBit(19, ModParData) << 3) + (APP32_TstBit(18, ModParData) << 2) + (APP32_TstBit(17, ModParData) << 1) + APP32_TstBit(16, ModParData) + 1);
  GroupTri1_2->Select((APP32_TstBit(23, ModParData) << 3) + (APP32_TstBit(22, ModParData) << 2) + (APP32_TstBit(21, ModParData) << 1) + APP32_TstBit(20, ModParData) + 1);
  GroupTri2_0->Select((APP32_TstBit(27, ModParData) << 3) + (APP32_TstBit(26, ModParData) << 2) + (APP32_TstBit(25, ModParData) << 1) + APP32_TstBit(24, ModParData) + 1);
  GroupTri2_1->Select((APP32_TstBit(31, ModParData) << 3) + (APP32_TstBit(30, ModParData) << 2) + (APP32_TstBit(29, ModParData) << 1) + APP32_TstBit(28, ModParData) + 1);

  retval = Pixie16ReadSglModPar((char*)"TrigConfig2", &ModParData, mod);
  if(retval < 0) ErrorInfo("LogicTrigger.cc", "load_info(...)", "Pixie16ReadSglModPar/TrigConfig2", retval);
  ChTri_ch0_3->Select(APP32_TstBit(24, ModParData)+1);
  ChTri_ch4_7->Select(APP32_TstBit(25, ModParData)+1);
  ChTri_ch8_11->Select(APP32_TstBit(26, ModParData)+1);
  ChTri_ch12_15->Select(APP32_TstBit(27, ModParData)+1);
  GroupTri0->Select((APP32_TstBit(17, ModParData) << 1) + APP32_TstBit(16, ModParData) + 1);
  GroupTri1->Select((APP32_TstBit(19, ModParData) << 1) + APP32_TstBit(18, ModParData) + 1);
  GroupTri2->Select((APP32_TstBit(21, ModParData) << 1) + APP32_TstBit(20, ModParData) + 1);
  GroupTri3->Select((APP32_TstBit(23, ModParData) << 1) + APP32_TstBit(22, ModParData) + 1);
  GroupTri2_2->Select((APP32_TstBit(3, ModParData) << 3) + (APP32_TstBit(2, ModParData) << 2) + (APP32_TstBit(1, ModParData) << 1) + APP32_TstBit(0, ModParData) + 1);
  GroupTri3_0->Select((APP32_TstBit(7, ModParData) << 3) + (APP32_TstBit(6, ModParData) << 2) + (APP32_TstBit(5, ModParData) << 1) + APP32_TstBit(4, ModParData) + 1);
  GroupTri3_1->Select((APP32_TstBit(11, ModParData) << 3) + (APP32_TstBit(10, ModParData) << 2) + (APP32_TstBit(9, ModParData) << 1) + APP32_TstBit(8, ModParData) + 1);
  GroupTri3_2->Select((APP32_TstBit(15, ModParData) << 3) + (APP32_TstBit(14, ModParData) << 2) + (APP32_TstBit(13, ModParData) << 1) + APP32_TstBit(12, ModParData) + 1);
  ChannelTrigger->Select((APP32_TstBit(31, ModParData) << 3) + (APP32_TstBit(30, ModParData) << 2) + (APP32_TstBit(29, ModParData) << 1) + APP32_TstBit(28, ModParData) + 1);
  //  std::cout << "loading info for module " << module << std::endl;
    
  return 1;
}

int LogicTrigger::change_values(Long_t mod)
{

  double ChanParData = -1;
  unsigned int ModParData;
  int retval;

  unsigned short d1,d2,d3,d4,d5,d6,d7;
  bool dd1,dd2;
  int a;
  unsigned int BackplaneEna = 0;
  unsigned int b1,b2;

  
  for (int i = 0; i < 16; i++)
    {
      ChanParData = NumEntry[1][i]->GetNumber();
      retval = Pixie16WriteSglChanPar((char*)"ExtTrigStretch", ChanParData, mod, i);
      if(retval < 0) ErrorInfo("LogicTrigger.cc", "change_values(...)", "Pixie16WriteSglChanPar/ExtTrigStretch", retval);
  
      ChanParData = NumEntry[2][i]->GetNumber();
      retval = Pixie16WriteSglChanPar((char*)"ExternDelayLen", ChanParData, mod, i);
      if(retval < 0) ErrorInfo("LogicTrigger.cc", "change_values(...)", "Pixie16WriteSglChanPar/ExternDelayLen", retval);
  
      ChanParData = NumEntry[3][i]->GetNumber();
      retval = Pixie16WriteSglChanPar((char*)"FtrigoutDelay", ChanParData, mod, i);
      if(retval < 0) ErrorInfo("LogicTrigger.cc", "change_values(...)", "Pixie16WriteSglChanPar/FtrigoutDelay", retval);
  
      ChanParData = NumEntry[4][i]->GetNumber();
      retval = Pixie16WriteSglChanPar((char*)"VetoStretch", ChanParData, mod, i);
      if(retval < 0) ErrorInfo("LogicTrigger.cc", "change_values(...)", "Pixie16WriteSglChanPar/VetoStretch", retval);
  
      ChanParData = NumEntry[5][i]->GetNumber();
      retval = Pixie16WriteSglChanPar((char*)"ChanTrigStretch", ChanParData, mod, i);
      if(retval < 0) ErrorInfo("LogicTrigger.cc", "change_values(...)", "Pixie16WriteSglChanPar/ChanTrigStretch", retval);
      
      ChanParData = NumEntry[6][i]->GetNumber();
      retval = Pixie16WriteSglChanPar((char*)"FASTTRIGBACKLEN", ChanParData, mod, i);
      if(retval < 0) ErrorInfo("LogicTrigger.cc", "change_values(...)", "Pixie16WriteSglChanPar/FASTTRIGBACKLEN", retval);
    }

  retval = Pixie16ReadSglModPar((char*)"TrigConfig0", &ModParData, mod);
  if(retval < 0) ErrorInfo("LogicTrigger.cc", "change_values(...)", "Pixie16ReadSglModPar/TrigConfig0", retval);
  SetMultipleBit(&ModParData,InternalFastTrigger->GetSelected(),3,2,1,0);
  SetMultipleBit(&ModParData,ExternalFastTrigger->GetSelected(),7,6,5,4);
  SetMultipleBit(&ModParData,InternalValidationTrigger->GetSelected(),11,10,9,8);
  SetMultipleBit(&ModParData,GlobalFastTrigger->GetSelected(),25,24);
  SetMultipleBit(&ModParData,GlobalValidationTrigger->GetSelected(),27,26);
  SetMultipleBit(&ModParData,ExternalValidationTrigger->GetSelected(),31,30,29,28);
  retval = Pixie16WriteSglModPar((char*)"TrigConfig0", ModParData, mod);
  if(retval < 0) ErrorInfo("LogicTrigger.cc", "change_values(...)", "Pixie16WriteSglModPar/TrigConfig0", retval);
  
  retval = Pixie16ReadSglModPar((char*)"TrigConfig1", &ModParData, mod);
  if(retval < 0) ErrorInfo("LogicTrigger.cc", "change_values(...)", "Pixie16ReadSglModPar/TrigConfig1", retval);
  SetMultipleBit(&ModParData,GroupTri0_0->GetSelected(),3,2,1,0);
  SetMultipleBit(&ModParData,GroupTri0_1->GetSelected(),7,6,5,4);
  SetMultipleBit(&ModParData,GroupTri0_2->GetSelected(),11,10,9,8);
  SetMultipleBit(&ModParData,GroupTri1_0->GetSelected(),15,14,13,12);
  SetMultipleBit(&ModParData,GroupTri1_1->GetSelected(),19,18,17,16);
  SetMultipleBit(&ModParData,GroupTri1_2->GetSelected(),23,22,21,20);
  SetMultipleBit(&ModParData,GroupTri2_0->GetSelected(),27,26,25,24);
  SetMultipleBit(&ModParData,GroupTri2_1->GetSelected(),31,30,29,28);
  retval = Pixie16WriteSglModPar((char*)"TrigConfig1", ModParData, mod);
  if(retval < 0) ErrorInfo("LogicTrigger.cc", "change_values(...)", "Pixie16WriteSglModPar/TrigConfig1", retval);
  
  retval = Pixie16ReadSglModPar((char*)"TrigConfig2", &ModParData, mod);
  if(retval < 0) ErrorInfo("LogicTrigger.cc", "change_values(...)", "Pixie16ReadSglModPar/TrigConfig2", retval);
  SetMultipleBit(&ModParData,ChTri_ch0_3->GetSelected(),24);
  SetMultipleBit(&ModParData,ChTri_ch4_7->GetSelected(),25);
  SetMultipleBit(&ModParData,ChTri_ch8_11->GetSelected(),26);
  SetMultipleBit(&ModParData,ChTri_ch12_15->GetSelected(),27);
  SetMultipleBit(&ModParData,GroupTri0->GetSelected(),17,16);
  SetMultipleBit(&ModParData,GroupTri1->GetSelected(),19,18);
  SetMultipleBit(&ModParData,GroupTri2->GetSelected(),21,20);
  SetMultipleBit(&ModParData,GroupTri3->GetSelected(),23,22);
  SetMultipleBit(&ModParData,GroupTri2_2->GetSelected(),3,2,1,0);
  SetMultipleBit(&ModParData,GroupTri3_0->GetSelected(),7,6,5,4);
  SetMultipleBit(&ModParData,GroupTri3_1->GetSelected(),11,10,9,8);
  SetMultipleBit(&ModParData,GroupTri3_2->GetSelected(),15,14,13,12);
  SetMultipleBit(&ModParData,ChannelTrigger->GetSelected(),31,30,29,28);
  retval = Pixie16WriteSglModPar((char*)"TrigConfig2", ModParData, mod);
  if(retval < 0) ErrorInfo("LogicTrigger.cc", "change_values(...)", "Pixie16WriteSglModPar/TrigConfig2", retval);

  //==================

  b1 = fasttriggerbackplaneena[0]->GetNumber();
  b2 = fasttriggerbackplaneena[1]->GetNumber();
  a = 0;
  for (int j = 0; j < 16; j++)
    {
      a = (1<<j);
      if(b1 & a) BackplaneEna = APP32_SetBit(j,BackplaneEna);
      else BackplaneEna = APP32_ClrBit(j,BackplaneEna);
      if(b2 & a) BackplaneEna = APP32_SetBit(j+16,BackplaneEna);
      else BackplaneEna = APP32_ClrBit(j+16,BackplaneEna);
    }
  retval = Pixie16WriteSglModPar((char*)"FastTrigBackplaneEna",BackplaneEna, mod);
  if(retval < 0) ErrorInfo("MultiplicityMask.cc", "change_values(...)", "Pixie16WriteSglModPar/FastTrigBackplaneEna", retval);

  
  for (int i = 0; i < 16; i++)
    {
      d2 = NumEntry[MULTIPLICITYOFFSET+2][i]->GetNumber();
      d3 = NumEntry[MULTIPLICITYOFFSET+3][i]->GetNumber();
      
      d1 = NumEntry[MULTIPLICITYOFFSET+1][i]->GetNumber();
      d4 = NumEntry[MULTIPLICITYOFFSET+4][i]->GetNumber();
      d5 = NumEntry[MULTIPLICITYOFFSET+5][i]->GetNumber();
      d6 = NumEntry[MULTIPLICITYOFFSET+6][i]->GetNumber();
      d7 = NumEntry[MULTIPLICITYOFFSET+7][i]->GetNumber();

      dd1 = NumEntry[MULTIPLICITYOFFSET+8][i]->GetNumber();
      dd2 = NumEntry[MULTIPLICITYOFFSET+9][i]->GetNumber();
      
      ChanParData = 0;
      a = 0;
      for (int j = 0; j < 16; j++)
	{
	  a = (1<<j);
	  if(d2 & a) ChanParData = APP32_SetBit(j,ChanParData);
	  else ChanParData = APP32_ClrBit(j,ChanParData);
	  if(d3 & a) ChanParData = APP32_SetBit(j+16,ChanParData);
	  else ChanParData = APP32_ClrBit(j+16,ChanParData);
	}
      
      retval = Pixie16WriteSglChanPar((char*)"MultiplicityMaskL", ChanParData, mod, i);
      if(retval < 0) ErrorInfo("MultiplicityMask.cc", "change_values(...)", "Pixie16WriteSglChanPar/MultiplicityMaskL", retval);
      
      ChanParData = 0;
      a = 0;
      for (int j = 0; j < 16; j++)
	{
	  a = (1<<j);
	  if(d1 & a) ChanParData = APP32_SetBit(j,ChanParData);
	  else ChanParData = APP32_ClrBit(j,ChanParData);
	}

      if(d4&1) ChanParData = APP32_SetBit(22,ChanParData);
      else ChanParData = APP32_ClrBit(22,ChanParData);
      if(d4&2) ChanParData = APP32_SetBit(23,ChanParData);
      else ChanParData = APP32_ClrBit(23,ChanParData);
      if(d4&4) ChanParData = APP32_SetBit(24,ChanParData);
      else ChanParData = APP32_ClrBit(24,ChanParData);

      if(d5&1) ChanParData = APP32_SetBit(25,ChanParData);
      else ChanParData = APP32_ClrBit(25,ChanParData);
      if(d5&2) ChanParData = APP32_SetBit(26,ChanParData);
      else ChanParData = APP32_ClrBit(26,ChanParData);
      if(d5&4) ChanParData = APP32_SetBit(27,ChanParData);
      else ChanParData = APP32_ClrBit(27,ChanParData);
      
      if(d6&1) ChanParData = APP32_SetBit(28,ChanParData);
      else ChanParData = APP32_ClrBit(28,ChanParData);
      if(d6&2) ChanParData = APP32_SetBit(29,ChanParData);
      else ChanParData = APP32_ClrBit(29,ChanParData);
      if(d6&4) ChanParData = APP32_SetBit(30,ChanParData);
      else ChanParData = APP32_ClrBit(30,ChanParData);

      if(d7&1) ChanParData = APP32_SetBit(17,ChanParData);
      else ChanParData = APP32_ClrBit(17,ChanParData);
      if(d7&2) ChanParData = APP32_SetBit(18,ChanParData);
      else ChanParData = APP32_ClrBit(18,ChanParData);
      if(d7&4) ChanParData = APP32_SetBit(19,ChanParData);
      else ChanParData = APP32_ClrBit(19,ChanParData);
      if(d7&8) ChanParData = APP32_SetBit(20,ChanParData);
      else ChanParData = APP32_ClrBit(20,ChanParData);
      if(d7&16) ChanParData = APP32_SetBit(21,ChanParData);
      else ChanParData = APP32_ClrBit(21,ChanParData);

      if(dd1) ChanParData = APP32_SetBit(16,ChanParData);
      else ChanParData = APP32_ClrBit(16,ChanParData);

      if(dd2) ChanParData = APP32_SetBit(31,ChanParData);
      else ChanParData = APP32_ClrBit(31,ChanParData);
      
      retval = Pixie16WriteSglChanPar((char*)"MultiplicityMaskH", ChanParData, mod, i);
      if(retval < 0) ErrorInfo("MultiplicityMask.cc", "change_values(...)", "Pixie16WriteSglChanPar/MultiplicityMaskH", retval);
    }


  //=============================

  retval = Pixie16ReadSglModPar((char*)"TrigConfig0", &ModParData, mod);
  if(retval < 0) ErrorInfo("LogicTrigger.cc", "change_values(...)", "Pixie16ReadSglModPar/TrigConfig0", retval);

  if(EnableDisableOfTestSignals->GetSelected() == 1)
    ModParData = APP32_ClrBit(15, ModParData);
  else
    ModParData = APP32_SetBit(15, ModParData);

  
  if((GroupOfTestSignals->GetSelected()-1) >> 2)
    ModParData = APP32_SetBit(14, ModParData);
  else
    ModParData = APP32_ClrBit(14, ModParData);

  if(((GroupOfTestSignals->GetSelected()-1)&2) >> 1)
    ModParData = APP32_SetBit(13, ModParData);
  else
    ModParData = APP32_ClrBit(13, ModParData);

  if((GroupOfTestSignals->GetSelected()-1)&1)
    ModParData = APP32_SetBit(12, ModParData);
  else
    ModParData = APP32_ClrBit(12, ModParData);  


  if((ChannelOfTestSignals->GetSelected()-1) >> 3)
    ModParData = APP32_SetBit(19, ModParData);
  else
    ModParData = APP32_ClrBit(19, ModParData);

  if(((ChannelOfTestSignals->GetSelected()-1)&4) >> 2)
    ModParData = APP32_SetBit(18, ModParData);
  else
    ModParData = APP32_ClrBit(18, ModParData);

  if(((ChannelOfTestSignals->GetSelected()-1)&2) >> 1)
    ModParData = APP32_SetBit(17, ModParData);
  else
    ModParData = APP32_ClrBit(17, ModParData);

  if((ChannelOfTestSignals->GetSelected()-1)&1)
    ModParData = APP32_SetBit(16, ModParData);
  else
    ModParData = APP32_ClrBit(16, ModParData);

  //
  if((TestSignals->GetSelected()-1) >> 3)
    ModParData = APP32_SetBit(23, ModParData);
  else
    ModParData = APP32_ClrBit(23, ModParData);

  if(((TestSignals->GetSelected()-1)&4) >> 2)
    ModParData = APP32_SetBit(22, ModParData);
  else
    ModParData = APP32_ClrBit(22, ModParData);

  if(((TestSignals->GetSelected()-1)&2) >> 1)
    ModParData = APP32_SetBit(21, ModParData);
  else
    ModParData = APP32_ClrBit(21, ModParData);

  if((TestSignals->GetSelected()-1)&1)
    ModParData = APP32_SetBit(20, ModParData);
  else
    ModParData = APP32_ClrBit(20, ModParData);
  
  retval = Pixie16WriteSglModPar((char*)"TrigConfig0", ModParData, mod);
  if(retval < 0) ErrorInfo("LogicTrigger.cc", "change_values(...)", "Pixie16WriteSglModPar/TrigConfig0", retval);

  load_info(mod);

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  // unsigned int test = 0xFFFFFFFF;
  // Pixie16WriteSglChanPar((char*)"MultiplicityMaskL", test, modNumber, 2);
  // Pixie16WriteSglChanPar((char*)"MultiplicityMaskL", test, modNumber, 4);

  
  return 1;
}

void LogicTrigger::TGTextEntryAddStyle(TGTextEntry *tgtextentry,char *title, char *tooltip)
{
  tgtextentry->SetText(title);
  tgtextentry->Resize(100, 15);
  tgtextentry->SetEnabled(kFALSE);
  // ch_0_3->SetFrameDrawn(kTRUE);
  tgtextentry->SetAlignment(kTextCenterX);
  tgtextentry->SetToolTipText(tooltip, 0);
}

void LogicTrigger::SetMultipleBit(unsigned int *Data,int Select,int bit0)
{
  if(Select == 1)
    *Data = APP32_ClrBit(bit0, *Data);
  else
    *Data = APP32_SetBit(bit0, *Data);
}

void LogicTrigger::SetMultipleBit(unsigned int *Data,int Select,int bit1,int bit0)
{
  if(((Select-1)&2) >> 1)
    *Data = APP32_SetBit(bit1, *Data);
  else
    *Data = APP32_ClrBit(bit1, *Data);

  if((Select-1)&1)
    *Data = APP32_SetBit(bit0, *Data);
  else
    *Data = APP32_ClrBit(bit0, *Data);
}

void LogicTrigger::SetMultipleBit(unsigned int *Data,int Select,int bit2,int bit1,int bit0)
{
  if(((Select-1)&4) >> 2)
    *Data = APP32_SetBit(bit2, *Data);
  else
    *Data = APP32_ClrBit(bit2, *Data);
  
  if(((Select-1)&2) >> 1)
    *Data = APP32_SetBit(bit1, *Data);
  else
    *Data = APP32_ClrBit(bit1, *Data);

  if((Select-1)&1)
    *Data = APP32_SetBit(bit0, *Data);
  else
    *Data = APP32_ClrBit(bit0, *Data);
}

void LogicTrigger::SetMultipleBit(unsigned int *Data,int Select,int bit3,int bit2,int bit1,int bit0)
{
  if((Select-1) >> 3)
    *Data = APP32_SetBit(bit3, *Data);
  else
    *Data = APP32_ClrBit(bit3, *Data);

  if(((Select-1)&4) >> 2)
    *Data = APP32_SetBit(bit2, *Data);
  else
    *Data = APP32_ClrBit(bit2, *Data);
  
  if(((Select-1)&2) >> 1)
    *Data = APP32_SetBit(bit1, *Data);
  else
    *Data = APP32_ClrBit(bit1, *Data);

  if((Select-1)&1)
    *Data = APP32_SetBit(bit0, *Data);
  else
    *Data = APP32_ClrBit(bit0, *Data);
}


// 
// LogicTrigger.cc ends here
