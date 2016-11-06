// FPOutputs.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 7月 29 10:48:48 2016 (+0800)
// Last-Updated: 日 11月  6 20:09:08 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 10
// URL: http://wuhongyi.cn 

#ifndef _FPOUTPUTS_HH_
#define _FPOUTPUTS_HH_

#include "TG3DLine.h"
#include "TGButton.h"
#include "TGComboBox.h"
#include "TGFrame.h"
#include "TGLabel.h"
#include "TGNumberEntry.h"
#include "TGTextEntry.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class FPOutputs : public TGTransientFrame
{
public:
  FPOutputs(const TGWindow * p, const TGWindow * main,int NumModules);
  virtual ~FPOutputs();

  int change_values(Long_t mod);
  int load_info(Long_t mod);
  Bool_t ProcessMessage(Long_t msg, Long_t parm1,Long_t parm2);

private:
  int numModules;

  TGVerticalFrame *mn_vert;
  TGHorizontalFrame *mn;
  TGHorizontalFrame *buttons;
  TGVerticalFrame *column1;
  
  bool Load_Once;
  TGTextButton *LoadButton, *CancelButton;
  TGButton *ApplyButton, *ExitButton;
  TGNumberEntry *numericMod;

  TGComboBox *EnableDisableOfTestSignals;
  TGComboBox *GroupOfTestSignals;
  TGComboBox *ChannelOfTestSignals;
  TGComboBox *TestSignals;
  
  short int modNumber;
};


#endif /* _FPOUTPUTS_HH_ */
// 
// FPOutputs.hh ends here
