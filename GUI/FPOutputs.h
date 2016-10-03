// FPOutputs.h --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 7月 29 10:48:48 2016 (+0800)
// Last-Updated: 二 8月  9 12:40:33 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 8
// URL: http://wuhongyi.github.io 

#ifndef _FPOUTPUTS_H_
#define _FPOUTPUTS_H_

#include "TGFrame.h"
#include "TGTextEntry.h"
#include "TGLabel.h"
#include "TGButton.h"
#include "TGNumberEntry.h"
#include "TG3DLine.h"
#include "TGComboBox.h"

class FPOutputs : public TGTransientFrame
{
public:
  FPOutputs(const TGWindow * p, const TGWindow * main,int NumModules=5);
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


#endif /* _FPOUTPUTS_H_ */
// 
// FPOutputs.h ends here
