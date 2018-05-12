// Energy.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 11月 18 20:32:43 2016 (+0800)
// Last-Updated: 六 5月 12 14:35:32 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 7
// URL: http://wuhongyi.cn 

#ifndef _ENERGY_H_
#define _ENERGY_H_

#include "Table.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class Energy : public Table
{
public:
  Energy(const TGWindow * p, const TGWindow * main, char *name, int columns, int rows, int NumModules);
  virtual ~Energy();

  int change_values(Long_t mod);
  int load_info(Long_t mod);
  Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
  
protected:

  void findtau(short int mod);
  
  TGNumberEntry *chanCopy;
  TGNumberEntry* filterRange;
  short int chanNumber;
  short int modNumber;
 
  unsigned int fRange;
  bool Load_Once;
  float decay;
  float risetime, flattop;

};

#endif /* _ENERGY_H_ */
// 
// Energy.hh ends here
