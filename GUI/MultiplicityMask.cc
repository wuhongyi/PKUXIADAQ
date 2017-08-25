// MultiplicityMask.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 二 10月  4 20:33:32 2016 (+0800)
// Last-Updated: 五 8月 25 22:30:11 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 57
// URL: http://wuhongyi.cn 

#include "MultiplicityMask.hh"
#include "Detector.hh"
#include "Global.hh"

#include "pixie16app_export.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MultiplicityMask::MultiplicityMask(const TGWindow *p, const TGWindow * main, char *name, int columns, int rows, int NumModules)
  :Table(p,main,columns,rows,name, NumModules)
{
  modNumber = 0;
  char n[10];
  cl0->SetText("ch #");
  for(int i = 0;i < rows;i++){
    sprintf(n,"%2d",i);
    Labels[i]->SetText(n);
  }
  CLabel[0]->SetText("Left[0-FFFF]");
  CLabel[0]->SetAlignment(kTextCenterX);
  fClient->GetColorByName("purple", color);
  CLabel[0]->SetTextColor(color, false);
  CLabel[1]->SetText("Itself[0-FFFF]");
  CLabel[1]->SetAlignment(kTextCenterX);
  fClient->GetColorByName("blue", color);
  CLabel[1]->SetTextColor(color, false);
  CLabel[2]->SetText("Right[0-FFFF]");
  CLabel[2]->SetAlignment(kTextCenterX);
  fClient->GetColorByName("pink", color);
  CLabel[2]->SetTextColor(color, false);
  CLabel[3]->SetText("ItselfCoin[0-7]");//1st
  CLabel[3]->SetAlignment(kTextCenterX);
  fClient->GetColorByName("blue", color);
  CLabel[3]->SetTextColor(color, false);
  CLabel[4]->SetText("RightCoin[0-7]");//2nd
  CLabel[4]->SetAlignment(kTextCenterX);
  fClient->GetColorByName("pink", color);
  CLabel[4]->SetTextColor(color, false);
  CLabel[5]->SetText("LeftCoin[0-7]");//3rd
  CLabel[5]->SetAlignment(kTextCenterX);
  fClient->GetColorByName("purple", color);
  CLabel[5]->SetTextColor(color, false);
  CLabel[6]->SetText("MultiThre[0-31]");
  CLabel[6]->SetAlignment(kTextCenterX);

  
  CLabel[7]->SetText("Sel Coin/Multi");
  CLabel[7]->SetAlignment(kTextCenterX);  
  CLabel[8]->SetText("Sel ChValidTrig");
  CLabel[8]->SetAlignment(kTextCenterX);

  fClient->GetColorByName("red", color);
  CLabel[7]->SetTextColor(color, false);
  CLabel[8]->SetTextColor(color, false);
  
  CLabel[0]->SetToolTipText("masking fast triggers from the module's left neighbor", 0);
  CLabel[1]->SetToolTipText("masking fast triggers from the module itself", 0);
  CLabel[2]->SetToolTipText("masking fast triggers from the module's right neighbor", 0);
  CLabel[3]->SetToolTipText("Threshold for 1st 16-bit coincidence trigger", 0);
  CLabel[4]->SetToolTipText("Threshold for 2nd 16-bit coincidence trigger", 0);
  CLabel[5]->SetToolTipText("Threshold for 3rd 16-bit coincidence trigger", 0);
  CLabel[6]->SetToolTipText("Threshold for the multiplicity trigger", 0);
  CLabel[7]->SetToolTipText("Select eithor coincidence trigger(0) or multiplicity trigger(1) as the source of the the channel coincidence/multiplicity trigger", 0);
  CLabel[8]->SetToolTipText("Select channel coincidence/multiplicity trigger(0) or other group trigger(1) as the source of the channel validation trigger", 0);

  for (int i = 1; i < 4; ++i)
    for (int j = 0; j < 16; ++j)
      {
	NumEntry[i][j]->SetFormat(TGNumberFormat::kNESHex);
	NumEntry[i][j]->SetLimits(TGNumberFormat::kNELLimitMinMax,0, 65535);
      }

  
  // ***** COPY BUTTON *********
  TGHorizontal3DLine *ln2 = new TGHorizontal3DLine(mn_vert, 200, 2);
  mn_vert->AddFrame(ln2, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 10, 10));
  TGHorizontalFrame *CopyButton = new TGHorizontalFrame(mn_vert, 400, 300);
  mn_vert->AddFrame(CopyButton, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  TGLabel *Copy = new TGLabel(CopyButton, "Select channel #");

  chanCopy = new TGNumberEntry(CopyButton, 0, 4, MODNUMBER+1000, (TGNumberFormat::EStyle) 0, (TGNumberFormat::EAttribute) 1, (TGNumberFormat::ELimit) 3/*kNELLimitMinMax*/,0, 3);
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
  TGHorizontalFrame *Backplane = new TGHorizontalFrame(mn_vert, 400, 300);
  mn_vert->AddFrame(Backplane, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

  TGHorizontalFrame *fVV1 = new TGHorizontalFrame(Backplane, 10, 10);
  Backplane->AddFrame(fVV1, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  TGTextEntry *LabelFastTriggerBackplaneLeft = new TGTextEntry(fVV1,new TGTextBuffer(100));
  LabelFastTriggerBackplaneLeft->SetText("FastTrigBackplaneLeft[0-FFFF]");
  LabelFastTriggerBackplaneLeft->SetEnabled(kFALSE);
  LabelFastTriggerBackplaneLeft->SetAlignment(kTextCenterX);
  // LabelFastTriggerBackplaneLeft->SetToolTipText("", 0);
  LabelFastTriggerBackplaneLeft->Resize(200, 20);
  fVV1->AddFrame(LabelFastTriggerBackplaneLeft, new TGLayoutHints(kLHintsCenterX, 0, 0, 0, 0));

  fasttriggerbackplaneena[0] = new TGNumberEntryField(fVV1, 0, 0, TGNumberFormat::kNESHex);
  fasttriggerbackplaneena[0]->SetLimits(TGNumberFormat::kNELLimitMinMax,0, 65535);
  fVV1->AddFrame(fasttriggerbackplaneena[0], new TGLayoutHints( kLHintsLeft | kLHintsTop, 1, 0, 0, 0));
  
  TGHorizontalFrame *fVV2 = new TGHorizontalFrame(Backplane, 10, 10);
  Backplane->AddFrame(fVV2, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
  TGTextEntry *LabelFastTriggerBackplaneRight = new TGTextEntry(fVV2,new TGTextBuffer(100));
  LabelFastTriggerBackplaneRight->SetText("FastTrigBackplaneRight[0-FFFF]");
  LabelFastTriggerBackplaneRight->SetEnabled(kFALSE);
  LabelFastTriggerBackplaneRight->SetAlignment(kTextCenterX);
  // LabelFastTriggerBackplaneRight->SetToolTipText("", 0);
  LabelFastTriggerBackplaneRight->Resize(200, 20);
  fVV2->AddFrame(LabelFastTriggerBackplaneRight, new TGLayoutHints(kLHintsCenterX, 20, 0, 0, 0));
  fasttriggerbackplaneena[1] = new TGNumberEntryField(fVV2, 1, 0, TGNumberFormat::kNESHex);
  fasttriggerbackplaneena[1]->SetLimits(TGNumberFormat::kNELLimitMinMax,0, 65535);
  fVV2->AddFrame(fasttriggerbackplaneena[1], new TGLayoutHints( kLHintsLeft | kLHintsTop, 1, 0, 0, 0));
  
  
  chanNumber = 0;
  MapSubwindows();
  Resize();			// resize to default size

  Load_Once = true;
}

MultiplicityMask::~MultiplicityMask()
{
  cout<<"MultiplicityMask is deleted!"<<endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Bool_t MultiplicityMask::ProcessMessage(Long_t msg,Long_t parm1, Long_t parm2){

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
	    case CANCEL:		/// Cancel Button
	      DeleteWindow();
	      break;
	      //////////////////////////////
	    case (COPYBUTTON+1000):
	     MultiLeft = NumEntry[1][chanNumber]->GetNumber();
	     MultiItself = NumEntry[2][chanNumber]->GetNumber();
	     MultiRight = NumEntry[3][chanNumber]->GetNumber();
	     Multi1st = NumEntry[4][chanNumber]->GetNumber();
	     Multi2nd = NumEntry[5][chanNumber]->GetNumber();
	     Multi3rd = NumEntry[6][chanNumber]->GetNumber();
	     MultiThreshold = NumEntry[7][chanNumber]->GetNumber();
	     SelCoinMulti = NumEntry[8][chanNumber]->GetNumber();
	     SelChValidTrig = NumEntry[9][chanNumber]->GetNumber();
	       
	      for(int i = 0;i < 16;i++)
	      	{
	      	  if(i != (chanNumber))
	      	    {
	      	      sprintf(tmp,"%04X",MultiLeft);
	      	      NumEntry[1][i]->SetText(tmp);
	      	      sprintf(tmp,"%04X",MultiItself);
	      	      NumEntry[2][i]->SetText(tmp);
		      sprintf(tmp,"%04X",MultiRight);
	      	      NumEntry[3][i]->SetText(tmp);
		      sprintf(tmp,"%d",Multi1st);
	      	      NumEntry[4][i]->SetText(tmp);
		      sprintf(tmp,"%d",Multi2nd);
	      	      NumEntry[5][i]->SetText(tmp);
		      sprintf(tmp,"%d",Multi3rd);
	      	      NumEntry[6][i]->SetText(tmp);
		      sprintf(tmp,"%d",MultiThreshold);
	      	      NumEntry[7][i]->SetText(tmp);
		      sprintf(tmp,"%d",SelCoinMulti);
	      	      NumEntry[8][i]->SetText(tmp);
		      sprintf(tmp,"%d",SelChValidTrig);
	      	      NumEntry[9][i]->SetText(tmp);
	      	    }
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

int MultiplicityMask::load_info(Long_t mod)
{
  double ChanParData = -1;
  unsigned int b;
  int retval;
  char text[20];

  unsigned int BackplaneEna = 0; 
  retval = Pixie16ReadSglModPar((char*)"FastTrigBackplaneEna", &BackplaneEna, mod);
  if(retval < 0) ErrorInfo("MultiplicityMask.cc", "load_info(...)", "Pixie16ReadSglModPar/FastTrigBackplaneEna", retval);
  b = 0;
  for (int j = 0; j < 16; j++)
    {
      b += (APP32_TstBit(j, BackplaneEna)<<j);
    }
  sprintf(text, "%04X", b);
  fasttriggerbackplaneena[0]->SetText(text);
  b = 0;
  for (int j = 0; j < 16; j++)
    {
      b += (APP32_TstBit(j+16, BackplaneEna)<<j);
    }
  sprintf(text, "%04X", b);
  fasttriggerbackplaneena[1]->SetText(text);
  
  
  for (int i = 0; i < 16; i++)
    {
      retval = Pixie16ReadSglChanPar((char*)"MultiplicityMaskL", &ChanParData, mod, i);
      if(retval < 0) ErrorInfo("MultiplicityMask.cc", "load_info(...)", "Pixie16ReadSglChanPar/MultiplicityMaskL", retval);

      b = 0;
      for (int j = 0; j < 16; j++)
	{
	  b += (APP32_TstBit(j, ChanParData)<<j);
	}
      sprintf(text, "%04X", b);
      NumEntry[2][i]->SetText(text);

      b = 0;
      for (int j = 0; j < 16; j++)
	{
	  b += (APP32_TstBit(j+16, ChanParData)<<j);
	}
      sprintf(text, "%04X", b);
      NumEntry[3][i]->SetText(text);

      retval = Pixie16ReadSglChanPar((char*)"MultiplicityMaskH", &ChanParData, mod, i);
      if(retval < 0) ErrorInfo("MultiplicityMask.cc", "load_info(...)", "Pixie16ReadSglChanPar/MultiplicityMaskH", retval);

      b = 0;
      for (int j = 0; j < 16; j++)
	{
	  b += (APP32_TstBit(j, ChanParData)<<j);
	}
      sprintf(text, "%04X", b);
      NumEntry[1][i]->SetText(text);

      b = (APP32_TstBit(24, ChanParData)<<2)+(APP32_TstBit(23, ChanParData)<<1)+APP32_TstBit(22, ChanParData);
      sprintf(text, "%d", b);
      NumEntry[4][i]->SetText(text);
      
      b = (APP32_TstBit(27, ChanParData)<<2)+(APP32_TstBit(26, ChanParData)<<1)+APP32_TstBit(25, ChanParData);
      sprintf(text, "%d", b);
      NumEntry[5][i]->SetText(text);

      b = (APP32_TstBit(30, ChanParData)<<2)+(APP32_TstBit(29, ChanParData)<<1)+APP32_TstBit(28, ChanParData);
      sprintf(text, "%d", b);
      NumEntry[6][i]->SetText(text);      

      b = (APP32_TstBit(21, ChanParData)<<4)+(APP32_TstBit(20, ChanParData)<<3)+(APP32_TstBit(19, ChanParData)<<2)+(APP32_TstBit(18, ChanParData)<<1)+APP32_TstBit(17, ChanParData);
      sprintf(text, "%d", b);
      NumEntry[7][i]->SetText(text);  

      b = APP32_TstBit(16, ChanParData);
      sprintf(text, "%d", b);
      NumEntry[8][i]->SetText(text);
      
      b = APP32_TstBit(31, ChanParData);
      sprintf(text, "%d", b);
      NumEntry[9][i]->SetText(text);      
    }
  //  std::cout << "loading info\n";
  return 1;
}

int MultiplicityMask::change_values(Long_t mod)
{
  double ChanParData = 0;
  unsigned short d1,d2,d3,d4,d5,d6,d7;
  bool dd1,dd2;

  int retval;
  int a;
  unsigned int BackplaneEna = 0;
  unsigned int b1,b2;

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
      d2 = NumEntry[2][i]->GetNumber();
      d3 = NumEntry[3][i]->GetNumber();
      
      d1 = NumEntry[1][i]->GetNumber();
      d4 = NumEntry[4][i]->GetNumber();
      d5 = NumEntry[5][i]->GetNumber();
      d6 = NumEntry[6][i]->GetNumber();
      d7 = NumEntry[7][i]->GetNumber();

      dd1 = NumEntry[8][i]->GetNumber();
      dd2 = NumEntry[9][i]->GetNumber();
      
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
  // std::cout << "change values\n";

  return 1;
}



// 
// MultiplicityMask.cc ends here
