// LogicTrigger.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 7月 28 18:18:03 2016 (+0800)
// Last-Updated: 四 11月 17 13:07:41 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 85
// URL: http://wuhongyi.cn 

#include "LogicTrigger.hh"
#include "Global.hh"

#include "pixie16app_export.h"

#include <iostream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LogicTrigger::LogicTrigger(const TGWindow *p, const TGWindow *main, char *name, int columns, int rows,int NumModules)
  : Table(p,main,columns,rows,name, NumModules)
{
  char n[10];
  cl0->SetText("ch #");
  for(int i=0;i<rows;i++)
    {
      sprintf(n,"%2d",i);
      Labels[i]->SetText(n);
    }
  CLabel[0]->SetText("ExtTrigStr[us]");
  CLabel[0]->SetAlignment(kTextCenterX);
  CLabel[1]->SetText("ExtDelLen[us]");
  CLabel[1]->SetAlignment(kTextCenterX);
  CLabel[2]->SetText("FtrigoutDel[us]");
  CLabel[2]->SetAlignment(kTextCenterX);
  CLabel[3]->SetText("VetoStr[us]");
  CLabel[3]->SetAlignment(kTextCenterX);
  CLabel[4]->SetText("ChaTrigStr[us]");
  CLabel[4]->SetAlignment(kTextCenterX);
  CLabel[5]->SetText("FastTriBaLen[us]");
  CLabel[5]->SetAlignment(kTextCenterX);

  CLabel[0]->SetToolTipText("External trigger stretch length", 0);
  CLabel[1]->SetToolTipText("Extern delay", 0);
  CLabel[2]->SetToolTipText("Fast trigger delay length", 0);
  CLabel[3]->SetToolTipText("Veto stretch length", 0);
  CLabel[4]->SetToolTipText("Channel trigger stretch length", 0);
  CLabel[5]->SetToolTipText("Fast trigger stretch length", 0);
  
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
  TGHorizontalFrame *TrigConfig = new TGHorizontalFrame(mn_vert, 400, 300);
  mn_vert->AddFrame(TrigConfig, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  TGVerticalFrame *fV1 = new TGVerticalFrame(TrigConfig, 10, 10);
  TGVerticalFrame *fV2 = new TGVerticalFrame(TrigConfig, 10, 10);
  TGVerticalFrame *fV3 = new TGVerticalFrame(TrigConfig, 10, 10);
  TGVerticalFrame *fV4 = new TGVerticalFrame(TrigConfig, 10, 10);
  TrigConfig->AddFrame(fV1, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  TrigConfig->AddFrame(fV2, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  TrigConfig->AddFrame(fV3, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  TrigConfig->AddFrame(fV4, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  char tempname[16];
  
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
      sprintf(tempname,"Ch %02d",i);
      GroupTri0_0->AddEntry(tempname, i+1);
      GroupTri0_1->AddEntry(tempname, i+1);
      GroupTri0_2->AddEntry(tempname, i+1);
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
      sprintf(tempname,"Ch %02d",i);
      GroupTri1_0->AddEntry(tempname, i+1);
      GroupTri1_1->AddEntry(tempname, i+1);
      GroupTri1_2->AddEntry(tempname, i+1);
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
      sprintf(tempname,"Ch %02d",i);
      GroupTri2_0->AddEntry(tempname, i+1);
      GroupTri2_1->AddEntry(tempname, i+1);
      GroupTri2_2->AddEntry(tempname, i+1);
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
      sprintf(tempname,"Ch %02d",i);
      GroupTri3_0->AddEntry(tempname, i+1);
      GroupTri3_1->AddEntry(tempname, i+1);
      GroupTri3_2->AddEntry(tempname, i+1);
    }
  GroupTri3_0->Select(1);
  GroupTri3_1->Select(1);
  GroupTri3_2->Select(1);

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  TGHorizontal3DLine *ln222 = new TGHorizontal3DLine(mn_vert, 200, 2);
  mn_vert->AddFrame(ln222, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 10, 10));

  
  TGHorizontalFrame *fVV0 = new TGHorizontalFrame(mn_vert, 10, 10);
  mn_vert->AddFrame(fVV0, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  TGTextEntry *LabelChannelTrigger = new TGTextEntry(fVV0,new TGTextBuffer(100));
  TGTextEntryAddStyle(LabelChannelTrigger,(char *)"Channel Trigger:",(char *)"Select source channel for the channel trigger from one of 16 channel triggers");
  LabelChannelTrigger->Resize(200, 20);
  fVV0->AddFrame(LabelChannelTrigger, new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
  ChannelTrigger = new TGComboBox(fVV0);
  fVV0->AddFrame(ChannelTrigger, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  ChannelTrigger->Resize(100, 20);
  for (int i = 0; i < 16; ++i)
    {
      sprintf(tempname,"Ch %02d",i);
      ChannelTrigger->AddEntry(tempname, i+1);
    }
  ChannelTrigger->Select(1);

  TGHorizontal3DLine *ln333 = new TGHorizontal3DLine(mn_vert, 200, 2);
  mn_vert->AddFrame(ln333, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 10, 10));
  
  TGHorizontalFrame *fVV1 = new TGHorizontalFrame(mn_vert, 10, 10);
  mn_vert->AddFrame(fVV1, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  TGTextEntry *LabelInternalFastTrigger = new TGTextEntry(fVV1,new TGTextBuffer(100));
  TGTextEntryAddStyle(LabelInternalFastTrigger,(char *)"Internal Fast Trigger:",(char *)"Select source channel for the internal fast trigger from one of the 16 local channels(0-15)");
  LabelInternalFastTrigger->Resize(200, 20);
  fVV1->AddFrame(LabelInternalFastTrigger, new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
  InternalFastTrigger = new TGComboBox(fVV1);
  fVV1->AddFrame(InternalFastTrigger, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  InternalFastTrigger->Resize(100, 20);
  for (int i = 0; i < 16; ++i)
    {
      sprintf(tempname,"Ch %02d",i);
      InternalFastTrigger->AddEntry(tempname, i+1);
    }
  InternalFastTrigger->Select(1);

  TGHorizontalFrame *fVV2 = new TGHorizontalFrame(mn_vert, 10, 10);
  mn_vert->AddFrame(fVV2, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  TGTextEntry *LabelExternalFastTrigger = new TGTextEntry(fVV2,new TGTextBuffer(100));
  TGTextEntryAddStyle(LabelExternalFastTrigger,(char *)"External Fast Trigger:",(char *)"Select source for external fast trigger");
  LabelExternalFastTrigger->Resize(200, 20);
  fVV2->AddFrame(LabelExternalFastTrigger, new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
  ExternalFastTrigger = new TGComboBox(fVV2);
  fVV2->AddFrame(ExternalFastTrigger, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  ExternalFastTrigger->Resize(150, 20);
  ExternalFastTrigger->AddEntry("Ext_FastTrig_Sel", 1);
  ExternalFastTrigger->AddEntry("Int_FastTrig_Sgl", 2);
  ExternalFastTrigger->AddEntry("FTIN_Or", 3);
  ExternalFastTrigger->AddEntry("LVDS_ValidTrig_FP", 4);
  ExternalFastTrigger->AddEntry("ChanTrig_Sel", 5);
  ExternalFastTrigger->Select(1);

  TGHorizontalFrame *fVV4 = new TGHorizontalFrame(mn_vert, 10, 10);
  mn_vert->AddFrame(fVV4, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  TGTextEntry *LabelGlobalFastTrigger = new TGTextEntry(fVV4,new TGTextBuffer(100));
  TGTextEntryAddStyle(LabelGlobalFastTrigger,(char *)"Global Fast Trigger:",(char *)"Select source for global fast trigger");
  LabelGlobalFastTrigger->Resize(200, 20);
  fVV4->AddFrame(LabelGlobalFastTrigger, new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
  GlobalFastTrigger = new TGComboBox(fVV4);
  fVV4->AddFrame(GlobalFastTrigger, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  GlobalFastTrigger->Resize(280, 20);
  GlobalFastTrigger->AddEntry("current module", 1);
  GlobalFastTrigger->AddEntry("a master module in the crate through the backplane", 2);
  GlobalFastTrigger->AddEntry("trigger module between multiple crates", 3);
  GlobalFastTrigger->AddEntry("a wired or bus line on the backplane", 4);
  GlobalFastTrigger->Select(1);  


  TGHorizontal3DLine *ln444 = new TGHorizontal3DLine(mn_vert, 200, 2);
  mn_vert->AddFrame(ln444, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 10, 10));
  
  TGHorizontalFrame *fVV3 = new TGHorizontalFrame(mn_vert, 10, 10);
  mn_vert->AddFrame(fVV3, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  TGTextEntry *LabelInternalValidationTrigger = new TGTextEntry(fVV3,new TGTextBuffer(100));
  TGTextEntryAddStyle(LabelInternalValidationTrigger,(char *)"Internal Validation Trigger:",(char *)"Select source channel for the internal validation trigger from one of the 16 local channels(0-15)");
  LabelInternalValidationTrigger->Resize(200, 20);
  fVV3->AddFrame(LabelInternalValidationTrigger, new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
  InternalValidationTrigger = new TGComboBox(fVV3);
  fVV3->AddFrame(InternalValidationTrigger, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  InternalValidationTrigger->Resize(100, 20);
  for (int i = 0; i < 16; ++i)
    {
      sprintf(tempname,"Ch %02d",i);
      InternalValidationTrigger->AddEntry(tempname, i+1);
    }
  InternalValidationTrigger->Select(1);
  

  TGHorizontalFrame *fVV6 = new TGHorizontalFrame(mn_vert, 10, 10);
  mn_vert->AddFrame(fVV6, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  TGTextEntry *LabelExternalValidationTrigger = new TGTextEntry(fVV6,new TGTextBuffer(100));
  TGTextEntryAddStyle(LabelExternalValidationTrigger,(char *)"External Validation Trigger:",(char *)"Select source for external validation trigger");
  LabelExternalValidationTrigger->Resize(200, 20);
  fVV6->AddFrame(LabelExternalValidationTrigger, new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
  ExternalValidationTrigger = new TGComboBox(fVV6);
  fVV6->AddFrame(ExternalValidationTrigger, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  ExternalValidationTrigger->Resize(150, 20);
  ExternalValidationTrigger->AddEntry("Ext_ValidTrig_Sel", 1);
  ExternalValidationTrigger->AddEntry("Int_ValidTrig_Sgl", 2);
  ExternalValidationTrigger->AddEntry("FTIN_Or", 3);
  ExternalValidationTrigger->AddEntry("LVDS_ValidTrig_FP", 4);
  ExternalValidationTrigger->AddEntry("ChanTrig_Sel", 5);
  
  ExternalValidationTrigger->Select(1);

  TGHorizontalFrame *fVV5 = new TGHorizontalFrame(mn_vert, 10, 10);
  mn_vert->AddFrame(fVV5, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  TGTextEntry *LabelGlobalValidationTrigger = new TGTextEntry(fVV5,new TGTextBuffer(100));
  TGTextEntryAddStyle(LabelGlobalValidationTrigger,(char *)"Global Validation Trigger:",(char *)"Select source for global validation trigger");
  LabelGlobalValidationTrigger->Resize(200, 20);
  fVV5->AddFrame(LabelGlobalValidationTrigger, new TGLayoutHints(kLHintsCenterX, 0, 0, 3, 0));
  GlobalValidationTrigger = new TGComboBox(fVV5);
  fVV5->AddFrame(GlobalValidationTrigger, new TGLayoutHints(kLHintsLeft, 5, 5, 2, 2));
  GlobalValidationTrigger->Resize(280, 20);
  GlobalValidationTrigger->AddEntry("current module", 1);
  GlobalValidationTrigger->AddEntry("a master module in the crate through the backplane", 2);
  GlobalValidationTrigger->AddEntry("trigger module between multiple crates", 3);
  GlobalValidationTrigger->AddEntry("a wired or bus line on the backplane", 4);
  GlobalValidationTrigger->Select(1);  




  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    
  chanNumber = 0;

  MapSubwindows();
  Resize();			// resize to default size
	
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
	      exttrigstretch = NumEntry[1][chanNumber]->GetNumber();
	      externdelaylen = NumEntry[2][chanNumber]->GetNumber();
	      ftrigoutdelay = NumEntry[3][chanNumber]->GetNumber();
	      vetostretch = NumEntry[4][chanNumber]->GetNumber();
	      chantrigstretch = NumEntry[5][chanNumber]->GetNumber();
	      fasttrigbacklen = NumEntry[6][chanNumber]->GetNumber();
	      
	      for(int i=0;i<16;i++)
		{
		  if(i != (chanNumber))
		    {
		      char tmp[10];
		      sprintf(tmp,"%1.3f",exttrigstretch);
		      NumEntry[1][i]->SetText(tmp);
		      sprintf(tmp,"%1.3f",externdelaylen);
		      NumEntry[2][i]->SetText(tmp);
		      sprintf(tmp,"%1.3f",ftrigoutdelay);
		      NumEntry[3][i]->SetText(tmp);
		      sprintf(tmp,"%1.3f",vetostretch);
		      NumEntry[4][i]->SetText(tmp);
		      sprintf(tmp,"%1.3f",chantrigstretch);
		      NumEntry[5][i]->SetText(tmp);
		      sprintf(tmp,"%1.3f",fasttrigbacklen);
		      NumEntry[6][i]->SetText(tmp);
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

int LogicTrigger::load_info(Long_t mod)
{
  double ChanParData = -1;
  unsigned int ModParData;
  int retval;
  char text[20];

  for (int i = 0; i < 16; i++)
    {
      retval = Pixie16ReadSglChanPar((char*)"ExtTrigStretch", &ChanParData, mod, i);
      if(retval < 0)
	std::cout<<"Error: LogicTrigger.cc - Pixie16ReadSglChanPar - ExtTrigStretch"<<std::endl;      
      sprintf(text, "%1.2f", ChanParData);
      NumEntry[1][i]->SetText(text);

      retval = Pixie16ReadSglChanPar((char*)"ExternDelayLen", &ChanParData, mod, i);
      if(retval < 0)
	std::cout<<"Error: LogicTrigger.cc - Pixie16ReadSglChanPar - ExternDelayLen"<<std::endl;            
      sprintf(text, "%1.2f", ChanParData);
      NumEntry[2][i]->SetText(text);

      retval = Pixie16ReadSglChanPar((char*)"FtrigoutDelay", &ChanParData, mod, i);
      if(retval < 0)
	std::cout<<"Error: LogicTrigger.cc - Pixie16ReadSglChanPar - FtrigoutDelay"<<std::endl;            
      sprintf(text, "%1.2f", ChanParData);
      NumEntry[3][i]->SetText(text);

      retval = Pixie16ReadSglChanPar((char*)"VetoStretch", &ChanParData, mod, i);
      if(retval < 0)
	std::cout<<"Error: LogicTrigger.cc - Pixie16ReadSglChanPar - VetoStretch"<<std::endl;            
      sprintf(text, "%1.2f", ChanParData);
      NumEntry[4][i]->SetText(text);

      retval = Pixie16ReadSglChanPar((char*)"ChanTrigStretch", &ChanParData, mod, i);
      if(retval < 0)
	std::cout<<"Error: LogicTrigger.cc - Pixie16ReadSglChanPar - ChanTrigStretch"<<std::endl;      
      sprintf(text, "%1.2f", ChanParData);
      NumEntry[5][i]->SetText(text);

      retval = Pixie16ReadSglChanPar((char*)"FASTTRIGBACKLEN", &ChanParData, mod, i);
      if(retval < 0)
	std::cout<<"Error: LogicTrigger.cc - Pixie16ReadSglChanPar - FASTTRIGBACKLEN"<<std::endl;      
      sprintf(text, "%1.2f", ChanParData);
      NumEntry[6][i]->SetText(text);
    }

  retval = Pixie16ReadSglModPar((char*)"TrigConfig0", &ModParData, mod);
  if(retval < 0)
    std::cout<<"Error: LogicTrigger.cc - Pixie16ReadSglModPar - TrigConfig0"<<std::endl;        
  InternalFastTrigger->Select((APP32_TstBit(3, ModParData) << 3) + (APP32_TstBit(2, ModParData) << 2) + (APP32_TstBit(1, ModParData) << 1) + APP32_TstBit(0, ModParData) + 1);
  ExternalFastTrigger->Select((APP32_TstBit(7, ModParData) << 3) + (APP32_TstBit(6, ModParData) << 2) + (APP32_TstBit(5, ModParData) << 1) + APP32_TstBit(4, ModParData) + 1);
  InternalValidationTrigger->Select((APP32_TstBit(11, ModParData) << 3) + (APP32_TstBit(10, ModParData) << 2) + (APP32_TstBit(9, ModParData) << 1) + APP32_TstBit(8, ModParData) + 1);
  GlobalFastTrigger->Select((APP32_TstBit(25, ModParData) << 1) + APP32_TstBit(24, ModParData) + 1);
  GlobalValidationTrigger->Select((APP32_TstBit(27, ModParData) << 1) + APP32_TstBit(26, ModParData) + 1);
  ExternalValidationTrigger->Select((APP32_TstBit(31, ModParData) << 3) + (APP32_TstBit(30, ModParData) << 2) + (APP32_TstBit(29, ModParData) << 1) + APP32_TstBit(28, ModParData) + 1);

  retval = Pixie16ReadSglModPar((char*)"TrigConfig1", &ModParData, mod);
  if(retval < 0)
    std::cout<<"Error: LogicTrigger.cc - Pixie16ReadSglModPar - TrigConfig1"<<std::endl; 
  GroupTri0_0->Select((APP32_TstBit(3, ModParData) << 3) + (APP32_TstBit(2, ModParData) << 2) + (APP32_TstBit(1, ModParData) << 1) + APP32_TstBit(0, ModParData) + 1);
  GroupTri0_1->Select((APP32_TstBit(7, ModParData) << 3) + (APP32_TstBit(6, ModParData) << 2) + (APP32_TstBit(5, ModParData) << 1) + APP32_TstBit(4, ModParData) + 1);
  GroupTri0_2->Select((APP32_TstBit(11, ModParData) << 3) + (APP32_TstBit(10, ModParData) << 2) + (APP32_TstBit(9, ModParData) << 1) + APP32_TstBit(8, ModParData) + 1);
  GroupTri1_0->Select((APP32_TstBit(15, ModParData) << 3) + (APP32_TstBit(14, ModParData) << 2) + (APP32_TstBit(13, ModParData) << 1) + APP32_TstBit(12, ModParData) + 1);
  GroupTri1_1->Select((APP32_TstBit(19, ModParData) << 3) + (APP32_TstBit(18, ModParData) << 2) + (APP32_TstBit(17, ModParData) << 1) + APP32_TstBit(16, ModParData) + 1);
  GroupTri1_2->Select((APP32_TstBit(23, ModParData) << 3) + (APP32_TstBit(22, ModParData) << 2) + (APP32_TstBit(21, ModParData) << 1) + APP32_TstBit(20, ModParData) + 1);
  GroupTri2_0->Select((APP32_TstBit(27, ModParData) << 3) + (APP32_TstBit(26, ModParData) << 2) + (APP32_TstBit(25, ModParData) << 1) + APP32_TstBit(24, ModParData) + 1);
  GroupTri2_1->Select((APP32_TstBit(31, ModParData) << 3) + (APP32_TstBit(30, ModParData) << 2) + (APP32_TstBit(29, ModParData) << 1) + APP32_TstBit(28, ModParData) + 1);

  retval = Pixie16ReadSglModPar((char*)"TrigConfig2", &ModParData, mod);
  if(retval < 0)
    std::cout<<"Error: LogicTrigger.cc - Pixie16ReadSglModPar - TrigConfig2"<<std::endl; 
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
  SetMultipleBit(&ModParData,GlobalFastTrigger->GetSelected(),25.24);
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
