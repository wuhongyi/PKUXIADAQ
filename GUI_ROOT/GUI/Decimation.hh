// Decimation.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 4月 23 10:49:30 2018 (+0800)
// Last-Updated: 六 11月 23 14:22:18 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 2
// URL: http://wuhongyi.cn 

#ifndef _DECIMATION_H_
#define _DECIMATION_H_

#include "Table.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class Detector;

class Decimation : public Table
{
public:
  Decimation(const TGWindow * p, const TGWindow * main, char *name, int columns, int rows, Detector *det);
  virtual ~Decimation();

  int change_values(Long_t mod);
  int load_info(Long_t mod);

protected:
  Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

protected:
  TGNumberEntry *chanCopy;
  short int modNumber;
  unsigned short chanNumber;
  bool Load_Once;
  
  float decimation;

private:
  Detector *detector;
};

#endif /* _DECIMATION_H_ */
// 
// Decimation.hh ends here
