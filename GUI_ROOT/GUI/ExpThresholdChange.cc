// ExpThresholdChange.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 2月 15 20:36:25 2021 (+0800)
// Last-Updated: 二 2月 16 19:01:55 2021 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 39
// URL: http://wuhongyi.cn 

#include "ExpThresholdChange.hh"

#include "Detector.hh"
#include "Global.hh"

#include "pixie16app_export.h"
#include "pixie16sys_export.h"
#include "xia_common.h"

#include "TGTab.h"
#include "TString.h"
#include "TFitResultPtr.h"
#include "TG3DLine.h"
#include "TColor.h"
#include <cstring>
#include <iostream>
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExpThresholdChange::ExpThresholdChange(const TGWindow *p, const TGWindow *main,Detector *det)
  :TGTransientFrame(p, main, 10, 10, kVerticalFrame)//Horizontal
{
  SetCleanup(kDeepCleanup);
  detector = det;

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
  

  TGGroupFrame *opgroup = new TGGroupFrame(this,"Change");
  this->AddFrame(opgroup,new TGLayoutHints(kLHintsExpandX|kLHintsTop,4,4,0,0));
  opgroup->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  TGHorizontalFrame *opframe = new TGHorizontalFrame(opgroup);
  opgroup->AddFrame(opframe,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
  opframe->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));


  opframe->AddFrame(parsthreBox = new TGComboBox(opframe, 0), new TGLayoutHints(kLHintsCenterX, 0, 0, 0, 0));
  parsthreBox->Associate(this);
  parsthreBox->AddEntry("Crate", 0);
  parsthreBox->AddEntry("DSPpar", 1);
  parsthreBox->AddEntry("File 0", 2);
  parsthreBox->AddEntry("File 1", 3);
  parsthreBox->AddEntry("File 2", 4);
  parsthreBox->AddEntry("File 3", 5);
  parsthreBox->AddEntry("File 4", 6);
  parsthreBox->Select(0);
  parsthreBox->Resize(80, 20);


  ScanButton = new TGTextButton(opframe, "S&can", BUTTON_SCAN);
  opframe->AddFrame(ScanButton, new TGLayoutHints(kLHintsCenterX, 0, 0, 0, 0));
  ScanButton->Associate(this);
  ScanButton->ChangeOptions(ScanButton->GetOptions() ^ kRaisedFrame);
  ScanButton->SetFont(TEXTBUTTON_FONT, false);
  ScanButton->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  ScanButton->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));


  SaveButton = new TGTextButton(opframe, "&Save", BUTTON_SAVE);
  opframe->AddFrame(SaveButton, new TGLayoutHints(kLHintsCenterX, 0, 0, 0, 0));
  SaveButton->Associate(this);
  SaveButton->ChangeOptions(SaveButton->GetOptions() ^ kRaisedFrame);
  SaveButton->SetFont(TEXTBUTTON_FONT, false);
  SaveButton->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  SaveButton->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));
  SaveButton->SetEnabled(0);

  ApplyButton = new TGTextButton(opframe, "&Apply", BUTTON_APPLY);
  opframe->AddFrame(ApplyButton, new TGLayoutHints(kLHintsCenterX, 0, 0, 0, 0));
  ApplyButton->Associate(this);
  ApplyButton->ChangeOptions(ApplyButton->GetOptions() ^ kRaisedFrame);
  ApplyButton->SetFont(TEXTBUTTON_FONT, false);
  ApplyButton->SetTextColor(TColor::RGB2Pixel(TEXTBUTTON_TEXT_R,TEXTBUTTON_TEXT_G,TEXTBUTTON_TEXT_B));
  ApplyButton->SetBackgroundColor(TColor::RGB2Pixel(TEXTBUTTON_BG_R,TEXTBUTTON_BG_G,TEXTBUTTON_BG_B));
  ApplyButton->SetEnabled(0);

  
  
  
  // TGLabel *filepathlabel = new TGLabel(opframe,"File Path: ");
  // opframe->AddFrame(filepathlabel,new TGLayoutHints(kLHintsLeft | kLHintsTop, 10, 3, 4, 0));
  // filepathlabel->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  

  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


  TGGroupFrame *thresholdgroup = new TGGroupFrame(this,"Threshold");
  this->AddFrame(thresholdgroup,new TGLayoutHints(kLHintsExpandX|kLHintsTop,4,4,0,0));
  thresholdgroup->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

  TGHorizontalFrame *threframe = new TGHorizontalFrame(thresholdgroup);
  thresholdgroup->AddFrame(threframe,new TGLayoutHints(kLHintsExpandX|kLHintsTop,0,0,0,0));
  threframe->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));


  ThreNum = new TGNumberEntryField *[208];
  TGTextEntry **Labels = new TGTextEntry *[16];
  TGTextEntry **cl0 = new TGTextEntry *[14];
  TGVerticalFrame **Column1 = new TGVerticalFrame *[14];
  for (int i = 0; i < 14; i++)
    {
      Column1[i] = new TGVerticalFrame(threframe, 200,300);
      threframe->AddFrame(Column1[i], new TGLayoutHints(kLHintsTop | kLHintsExpandX, 0, 0, 0, 0));
      // Column1[i]->SetBackgroundColor(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

      cl0[i] = new TGTextEntry(Column1[i], new TGTextBuffer(100), 10000,
			       cl0[i]->GetDefaultGC()(),
			       cl0[i]->GetDefaultFontStruct(),
			       kRaisedFrame | kDoubleBorder, GetWhitePixel());
      Column1[i]->AddFrame(cl0[i], new TGLayoutHints(kLHintsExpandX | kLHintsTop, 0, 0, 10, 0));
      cl0[i]->SetFont("-adobe-helvetica-bold-r-*-*-11-*-*-*-*-*-iso8859-1", false);
      if(i == 0)
	{
	  cl0[i]->SetText("ch #");
	  cl0[i]->Resize(20, 20);
	}
      else
	{
	  cl0[i]->SetText(TString::Format("Mod:%02d",i-1).Data());
	  cl0[i]->Resize(35, 20);
	}
      
      cl0[i]->SetEnabled(kFALSE);
      cl0[i]->SetFrameDrawn(kFALSE);
      cl0[i]->SetAlignment(kTextCenterX);
      // cl0[i]->SetAlignment(kTextRight);
      // cl0[i]->ChangeBackground(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));

      for (int j = 0; j < 16; j++)
	{
	  if(i==0)
	    {
	      Labels[j] = new TGTextEntry(Column1[0], new TGTextBuffer(100), 10000,
					  Labels[j]->GetDefaultGC()(),
					  Labels[j]->GetDefaultFontStruct(),
					  kRaisedFrame | kDoubleBorder,
					  GetWhitePixel());
	      Column1[0]->AddFrame(Labels[j], new TGLayoutHints(kLHintsRight | kLHintsTop, 0, 0, 0, 0));
	      Labels[j]->SetText(TString::Format("%02d",j).Data());
	      Labels[j]->Resize(35, 20);
	      Labels[j]->SetEnabled(kFALSE);
	      Labels[j]->SetFrameDrawn(kFALSE);
	      // Labels[j]->ChangeBackground(TColor::RGB2Pixel(FRAME_BG_R,FRAME_BG_G,FRAME_BG_B));
	    }
	  else
	    {
	      ThreNum[16*(i-1)+j] =  new TGNumberEntryField(Column1[i], -1, 0, ThreNum[16*(i-1)+j]->GetDefaultGC()(),ThreNum[16*(i-1)+j]->GetDefaultFontStruct(),kRaisedFrame | kDoubleBorder, GetWhitePixel());
	      // new TGTextEntry(Column1[i], new TGTextBuffer(100), 10000,
	      // 		      ThreNum[16*(i-1)+j]->GetDefaultGC()(),
	      // 		      ThreNum[16*(i-1)+j]->GetDefaultFontStruct(),
	      // 		      kRaisedFrame | kDoubleBorder, GetWhitePixel());

	      ThreNum[16*(i-1)+j]->SetAlignment(kTextCenterX);
	      ThreNum[16*(i-1)+j]->SetText("");
	      ThreNum[16*(i-1)+j]->Resize(35, 20);
	      // ThreNum[16*(i-1)+j]->SetEnabled(kFALSE);
	      // ICR[16*(i-1)+j]->SetFrameDrawn(kFALSE);
	      Column1[i]->AddFrame(ThreNum[16*(i-1)+j], new TGLayoutHints(kLHintsExpandX, 0, 0, 0, 0));//kLHintsCenterX
	      
	    }



	}
	

      
    }
  

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  SetWindowName("Change Threshold");
  MapSubwindows();
  MapWindow();
  Resize(1300,500);

  
}



ExpThresholdChange::~ExpThresholdChange()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


Bool_t ExpThresholdChange::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
  int test1 = 0;
  int test2 = 0;
  switch (test1 = GET_MSG(msg))
    {
    case kC_COMMAND:
      switch (test2 = GET_SUBMSG(msg))
	{
	case kCM_COMBOBOX:
	  if(parsthreBox->GetSelected()==0 || parsthreBox->GetSelected()==1)
	    SaveButton->SetEnabled(0);
	  else
	    SaveButton->SetEnabled(1);
	  
	  if(parsthreBox->GetSelected()==0)
	    ApplyButton->SetEnabled(0);
	  else
	    ApplyButton->SetEnabled(1);
	  break;
	  
	case kCM_BUTTON:
	  switch (parm1)
	    {
	    case BUTTON_SCAN:
	      Scan_Pars();
	      break;
	    case BUTTON_SAVE:
	      Save_Pars();
	      break;
	    case BUTTON_APPLY:
	      Apply_Pars();
	      break;

	      
	      
	    // case COPYPARS_MODNUM0:
	    //   if (parm2 == 0)
	    // 	{
	    // 	  if (modNumber0 != detector->NumModules-1)
	    // 	    {
	    // 	      ++modNumber0;
	    // 	      modnum0->SetIntNumber(modNumber0);
	    // 	    }
	    // 	}
	    //   else
	    // 	{
	    // 	  if (modNumber0 != 0)
	    // 	    {
	    // 	      if (--modNumber0 == 0)
	    // 		modNumber0 = 0;
	    // 	      modnum0->SetIntNumber(modNumber0);
	    // 	    }
	    // 	}
	    //   break;

	      
	    // case COPYPARS_CHNUM0:
	    //   if (parm2 == 0)
	    // 	{
	    // 	  if (chanNumber0 != 15)
	    // 	    {
	    // 	      ++chanNumber0;
	    // 	      chnum0->SetIntNumber(chanNumber0);
	    // 	    }
	    // 	}
	    //   else
	    // 	{
	    // 	  if (chanNumber0 != 0)
	    // 	    {
	    // 	      if (--chanNumber0 == 0)
	    // 		chanNumber0 = 0;
	    // 	      chnum0->SetIntNumber(chanNumber0);
	    // 	    }
	    // 	}
	    //   break;

	    default:
	      break;
	    }
	  break;
	}
      break;

      
    // case kC_TEXTENTRY:
    //   switch (parm1)
    // 	{
    // 	case COPYPARS_MODNUM0:
    // 	  switch (GET_SUBMSG(msg))
    // 	    {
    // 	    case kTE_ENTER:
    // 	      modNumber0 = modnum0->GetIntNumber();
    // 	      if(modNumber0 > detector->NumModules-1) modNumber0 = detector->NumModules-1;
    // 	      if(modNumber0 < 0) modNumber0 = 0;
    // 	      modnum0->SetIntNumber(modNumber0);
    // 	      break;
	      
    // 	    default:
    // 	      break;
    // 	    }
    // 	  break;
	  
	  	  
    // 	case COPYPARS_CHNUM0:
    // 	  switch (GET_SUBMSG(msg))
    // 	    {
    // 	    case kTE_ENTER:
    // 	      chanNumber0 = chnum0->GetIntNumber();
    // 	      if(chanNumber0 > 15) chanNumber0 = 15;
    // 	      if(chanNumber0 < 0) chanNumber0 = 0;
    // 	      chnum0->SetIntNumber(chanNumber0);
    // 	      break;
	      
    // 	    default:
    // 	      break;
    // 	    }
    // 	  break;
 
    // 	}
    //   break;


    default:
      break;
    }

  
  return kTRUE;
}


void ExpThresholdChange::Scan_Pars()
{
  double ChanParData = -1;
  int retval;
  int tmp0,tmp1;
  double tmp2;std::ifstream expthreread;
  
  for (int i = 0; i < 13; ++i)
    for (int j = 0; j < 16; j++)
      ThreNum[16*i+j]->SetText("");

  if(parsthreBox->GetSelected()==0)
    {
      for (int mod = 0; mod < detector->NumModules; ++mod)
	for (int i = 0; i < 16; i++)
	  {
	    retval = Pixie16ReadSglChanPar((char*)"TRIGGER_THRESHOLD", &ChanParData, mod, i);
	    if(retval < 0) ErrorInfo("ExpThresholdChange.cc", "Scan_Pars()", "Pixie16ReadSglChanPar/TRIGGER_THRESHOLD", retval);
	    ThreNum[16*mod+i]->SetText(TString::Format("%d", (int)ChanParData).Data());
	  }
    }
  else
    {
      
      if(parsthreBox->GetSelected()==1)
	expthreread.open("exp/DSPpar.thre");
      if(parsthreBox->GetSelected()==2)
	expthreread.open("exp/user0.thre");
      if(parsthreBox->GetSelected()==3)
	expthreread.open("exp/user1.thre");
      if(parsthreBox->GetSelected()==4)
	expthreread.open("exp/user2.thre");
      if(parsthreBox->GetSelected()==5)
	expthreread.open("exp/user3.thre");
      if(parsthreBox->GetSelected()==6)
	expthreread.open("exp/user4.thre");
	    
      if(!expthreread.is_open())
	{
	  std::cout<<"can't open file -- exp/*.thre"<<std::endl;
	  return;
	}
      while(!expthreread.eof())
	{
	  expthreread>>tmp0>>tmp1>>tmp2;
	  if(expthreread.eof()) break;
	  ThreNum[16*tmp0+tmp1]->SetText(TString::Format("%d", (int)tmp2).Data());
	}
      expthreread.close();
    }
}
    
void ExpThresholdChange::Save_Pars()
{
  std::ofstream expthrewrite;
  if(parsthreBox->GetSelected()==2)
    expthrewrite.open("exp/user0.thre");
  if(parsthreBox->GetSelected()==3)
    expthrewrite.open("exp/user1.thre");
  if(parsthreBox->GetSelected()==4)
    expthrewrite.open("exp/user2.thre");
  if(parsthreBox->GetSelected()==5)
    expthrewrite.open("exp/user3.thre");
  if(parsthreBox->GetSelected()==6)
    expthrewrite.open("exp/user4.thre");

  if(!expthrewrite.is_open())
    {
      std::cout<<"can't open file -- exp/*.thre"<<std::endl;
    }
  for (int mod = 0; mod < detector->NumModules; ++mod)
    for (int i = 0; i < 16; i++)
      {
	expthrewrite<<mod<<"  "<<i<<"  "<<ThreNum[16*mod+i]->GetNumber()<<endl;

      }
  expthrewrite.close();
}
    
void ExpThresholdChange::Apply_Pars()
{
  int tmp0,tmp1;
  double thresh;
  int retval;
  std::ifstream expthreread;
  
  if(parsthreBox->GetSelected()==1)
    expthreread.open("exp/DSPpar.thre");
  if(parsthreBox->GetSelected()==2)
    expthreread.open("exp/user0.thre");
  if(parsthreBox->GetSelected()==3)
    expthreread.open("exp/user1.thre");
  if(parsthreBox->GetSelected()==4)
    expthreread.open("exp/user2.thre");
  if(parsthreBox->GetSelected()==5)
    expthreread.open("exp/user3.thre");
  if(parsthreBox->GetSelected()==6)
    expthreread.open("exp/user4.thre");
  
  if(!expthreread.is_open())
    {
      std::cout<<"can't open file -- exp/*.thre"<<std::endl;
      return;
    }
  while(!expthreread.eof())
    {
      expthreread>>tmp0>>tmp1>>thresh;
      if(expthreread.eof()) break;//避免最后一个重复读取
      if(tmp0<0 || tmp0 >= detector->NumModules || tmp1<0 || tmp1>=16) continue;
      retval = Pixie16WriteSglChanPar((char*)"TRIGGER_THRESHOLD", thresh, tmp0, tmp1);
      if(retval < 0) ErrorInfo("ExpThresholdChange.cc", "Apply_Pars()", "Pixie16WriteSglChanPar/TRIGGER_THRESHOLD", retval);
    }
  expthreread.close();
}

// 
// ExpThresholdChange.cc ends here
