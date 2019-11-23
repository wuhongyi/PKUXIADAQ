// Base.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 11月 18 19:23:48 2016 (+0800)
// Last-Updated: 六 11月 23 14:32:22 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 10
// URL: http://wuhongyi.cn 

#ifndef _BASE_H_
#define _BASE_H_

#include "Table.hh"
#include "TGComboBox.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class Detector;

class Base : public Table
{
public:
  Base(const TGWindow * p, const TGWindow * main, char *name, int columns, int rows, Detector *det);
  virtual ~Base();

  Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
  int change_values(Long_t mod);
  int load_info(Long_t mod);

protected:
  short int modNumber; 
  short int chanNumber;
  TGNumberEntry* chanCopy;
  bool Load_Once;
  
  TGVerticalFrame *ColumnSign, *ColumnGain;
  TGVerticalFrame *ColumnGC, *ColumnTC,*ColumnEQS,*ColumnECT,*ColumnERB;
  TGComboBox *lstBox[16], *lstBoxGain[16];
  TGCheckButton *ckGC[16],*ckTC[16],*ckEQS[16],*ckECT[16],*ckERB[16];

private:
  Detector *detector;
};

#endif /* _BASE_H_ */
// 
// Base.hh ends here
