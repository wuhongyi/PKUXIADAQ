#ifndef QDC_H
#define QDC_H

#include "Table.h"

class Qdc : public Table 
{
public:
  Qdc(const TGWindow *p, const TGWindow *main, char *name, int colunms, int rows,int NumModules);
  ~Qdc();

  int change_values (Long_t mod);
  int load_info(Long_t mod);
  
protected:
  Bool_t ProcessMessage ( Long_t msg, Long_t parm1,Long_t parm2);
  
  bool Load_Once;
  short int modNumber;
  short int chanNumber;
  char tmp[10];
  TGNumberEntry* chanCopy;

  double qlen[8];
};

#endif