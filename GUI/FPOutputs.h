// FPOutputs.h --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 7月 29 10:48:48 2016 (+0800)
// Last-Updated: 三 10月  5 17:16:24 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 9
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


#endif /* _FPOUTPUTS_H_ */
// 
// FPOutputs.h ends here
