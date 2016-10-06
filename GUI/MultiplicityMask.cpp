// MultiplicityMask.cpp --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 二 10月  4 20:33:32 2016 (+0800)
// Last-Updated: 四 10月  6 21:17:52 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 18
// URL: http://wuhongyi.github.io 

#include "MultiplicityMask.h"
#include "Detector.h"
#include "Global.h"
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
  CLabel[0]->SetText("Left[0-65535]");
  CLabel[0]->SetAlignment(kTextCenterX);
  CLabel[1]->SetText("Itself[0-65535]");
  CLabel[1]->SetAlignment(kTextCenterX);
  CLabel[2]->SetText("Right[0-65535]");
  CLabel[2]->SetAlignment(kTextCenterX);
  CLabel[3]->SetText("1st[0-7]");
  CLabel[3]->SetAlignment(kTextCenterX);
  CLabel[4]->SetText("2nd[0-7]");
  CLabel[4]->SetAlignment(kTextCenterX);
  CLabel[5]->SetText("3rd[0-7]");
  CLabel[5]->SetAlignment(kTextCenterX);  
   
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
	      // tdelay = NumEntry[1][chanNumber]->GetNumber();
	      // cfrac = NumEntry[2][chanNumber]->GetNumber();
	      // thresh = NumEntry[3][chanNumber]->GetNumber();
	      // for(int i = 0;i < 16;i++)
	      // 	{
	      // 	  if(i != (chanNumber))
	      // 	    {
	      // 	      sprintf(tmp,"%1.3f",tdelay);
	      // 	      NumEntry[1][i]->SetText(tmp);
	      // 	      sprintf(tmp,"%1.3f",cfrac);
	      // 	      NumEntry[2][i]->SetText(tmp);
	      // 	      sprintf(tmp,"%1.3f",thresh);
	      // 	      NumEntry[3][i]->SetText(tmp);
	      // 	    }
	      // 	}  
	      
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

  for (int i = 0; i < 16; i++)
    {
      retval = Pixie16ReadSglChanPar((char*)"MultiplicityMaskL", &ChanParData, mod, i);
      if(retval < 0) ErrorInfo("MultiplicityMask.cpp", "load_info(...)", "Pixie16ReadSglChanPar/MultiplicityMaskL", retval);

      b = 0;
      for (int j = 0; j < 16; j++)
	{
	  b += (APP32_TstBit(j, ChanParData)<<j);
	}
      sprintf(text, "%d", b);
      NumEntry[2][i]->SetText(text);

      b = 0;
      for (int j = 0; j < 16; j++)
	{
	  b += (APP32_TstBit(j+16, ChanParData)<<j);
	}
      sprintf(text, "%d", b);
      NumEntry[3][i]->SetText(text);

      retval = Pixie16ReadSglChanPar((char*)"MultiplicityMaskH", &ChanParData, mod, i);
      if(retval < 0) ErrorInfo("MultiplicityMask.cpp", "load_info(...)", "Pixie16ReadSglChanPar/MultiplicityMaskH", retval);

      b = 0;
      for (int j = 0; j < 16; j++)
	{
	  b += (APP32_TstBit(j, ChanParData)<<j);
	}
      sprintf(text, "%d", b);
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
     
    }
  //  std::cout << "loading info\n";
  return 1;
}

int MultiplicityMask::change_values(Long_t mod)
{
  double ChanParData = 0;
  unsigned short d1,d2,d3,d4,d5,d6;
  int retval;
  int a;
  for (int i = 0; i < 16; i++)
    {
      d2 = NumEntry[2][i]->GetNumber();
      d3 = NumEntry[3][i]->GetNumber();
      
      d1 = NumEntry[1][i]->GetNumber();
      d4 = NumEntry[4][i]->GetNumber();
      d5 = NumEntry[5][i]->GetNumber();
      d6 = NumEntry[6][i]->GetNumber();
      
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
      if(retval < 0) ErrorInfo("MultiplicityMask.cpp", "change_values(...)", "Pixie16WriteSglChanPar/MultiplicityMaskL", retval);
      
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
      retval = Pixie16WriteSglChanPar((char*)"MultiplicityMaskH", ChanParData, mod, i);
      if(retval < 0) ErrorInfo("MultiplicityMask.cpp", "change_values(...)", "Pixie16WriteSglChanPar/MultiplicityMaskH", retval);
      
    }
  // std::cout << "change values\n";

  return 1;
}



// 
// MultiplicityMask.cpp ends here



