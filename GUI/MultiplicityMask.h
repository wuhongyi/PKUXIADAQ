// MultiplicityMask.h --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 二 10月  4 20:33:42 2016 (+0800)
// Last-Updated: 三 10月 19 12:28:47 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 7
// URL: http://wuhongyi.cn 

#ifndef _MULTIPLICITYMASK_H_
#define _MULTIPLICITYMASK_H_

#include "Table.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MultiplicityMask : public Table
{
public:
  MultiplicityMask(const TGWindow *p, const TGWindow *main, char *name,int colunms,int rows,int NumModules);
  virtual ~MultiplicityMask();

  Bool_t ProcessMessage(Long_t msg,Long_t parm1,Long_t parm2);
  int load_info(Long_t mod);
  int change_values(Long_t mod);

protected:
  
  bool Load_Once;

  short int modNumber;
  short int chanNumber;
  char tmp[10];
  TGNumberEntry* chanCopy;
  Pixel_t color;

  TGNumberEntryField *fasttriggerbackplaneena[2];
  
  int MultiLeft;
  int MultiRight;
  int MultiItself;
  int Multi1st;
  int Multi2nd;
  int Multi3rd;
  int MultiThreshold;
  bool SelCoinMulti;
  bool SelChValidTrig;
};

#endif /* _MULTIPLICITYMASK_H_ */
// 
// MultiplicityMask.h ends here
