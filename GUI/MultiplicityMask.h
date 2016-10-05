// MultiplicityMask.h --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 二 10月  4 20:33:42 2016 (+0800)
// Last-Updated: 三 10月  5 14:29:45 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 2
// URL: http://wuhongyi.github.io 

#ifndef _MULTIPLICITYMASK_H_
#define _MULTIPLICITYMASK_H_

#include "Table.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MultiplicityMask : public Table
{
public:
  MultiplicityMask(const TGWindow *p, const TGWindow *main, char *name,int colunms =7,int rows =16,int NumModules=5);
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


};

#endif /* _MULTIPLICITYMASK_H_ */
// 
// MultiplicityMask.h ends here
