// ResetDelay.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 12月 16 14:43:28 2021 (+0800)
// Last-Updated: 四 12月 16 14:59:59 2021 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 1
// URL: http://wuhongyi.cn 

#ifndef _RESETDELAY_H_
#define _RESETDELAY_H_

#include "Table.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class Detector;

class ResetDelay : public Table
{
public:
  ResetDelay(const TGWindow * p, const TGWindow * main, char *name, int columns, int rows, Detector *det);
  virtual ~ResetDelay();

  int change_values(Long_t mod);
  int load_info(Long_t mod);

protected:
  Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

protected:
  TGNumberEntry *chanCopy;
  short int modNumber;
  unsigned short chanNumber;
  bool Load_Once;
  
  float resetdelay;

private:
  Detector *detector;
};

#endif /* _RESETDELAY_H_ */

// 
// ResetDelay.hh ends here
