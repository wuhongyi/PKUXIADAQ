// Base.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 11月 18 19:23:48 2016 (+0800)
// Last-Updated: 五 11月 18 20:01:56 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 3
// URL: http://wuhongyi.cn 

#ifndef _BASE_H_
#define _BASE_H_

#include "Table.hh"
#include "TGComboBox.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class Base : public Table
{
public:
  Base(const TGWindow * p, const TGWindow * main, char *name, int columns, int rows, int NumModules);
  virtual ~Base();

  Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
  int change_values(Long_t mod);
  int load_info(Long_t mod);

protected:
  short int modNumber; 
  short int chanNumber;
  TGNumberEntry* chanCopy;
  bool Load_Once;
  
  Pixel_t color;
  TGVerticalFrame* ColumnSign, *ColumnGain;
  TGComboBox *lstBox[17], *lstBoxGain[17];

  int pol_temp;
  int gain_temp;
  float offset_temp;
  float blcut,blpercent;
  float thresh;
  float tlength,tdelay;
  
};

#endif /* _BASE_H_ */
// 
// Base.hh ends here
