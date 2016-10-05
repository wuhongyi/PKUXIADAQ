#ifndef CFD_H
#define CFD_H

#include "Table.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class Cfd : public Table
{
public:
  Cfd(const TGWindow *p, const TGWindow *main, char *name,int colunms =4,int rows =16,int NumModules=5);
  virtual ~Cfd();

  Bool_t ProcessMessage(Long_t msg,Long_t parm1,Long_t parm2);
  int load_info(Long_t mod);
  int change_values(Long_t mod);

protected:
  float tdelay,cfrac,thresh;
  bool Load_Once;

  short int modNumber;
  short int chanNumber;
  char tmp[10];
  TGNumberEntry* chanCopy;
};

#endif
