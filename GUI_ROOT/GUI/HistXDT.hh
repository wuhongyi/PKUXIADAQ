// HistXDT.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 3月  8 13:32:50 2018 (+0800)
// Last-Updated: 六 11月 23 13:55:05 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 5
// URL: http://wuhongyi.cn 

#ifndef _HISTXDT_H_
#define _HISTXDT_H_

#include "Table.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class Detector;

class HistXDT : public Table
{
public:
  HistXDT(const TGWindow * p, const TGWindow * main, char *name, int columns, int rows, Detector *det);
  virtual ~HistXDT();

  int change_values(Long_t mod);
  int load_info(Long_t mod);

protected:
  Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

  
protected:
  TGNumberEntry *chanCopy;
  
  short int modNumber;
  unsigned short chanNumber;
  
  bool Load_Once;
  
  float emin, bin;
  float decay;
private:
  Detector *detector;  
};


#endif /* _HISTXDT_H_ */
// 
// HistXDT.hh ends here
