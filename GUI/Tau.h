#ifndef TAU_H_
#define TAU_H_
#include "Table.h"

class Tau : public Table
{
public:
  Tau(const TGWindow * p, const TGWindow * main, char *name, int columns = 3,
      int rows = 16, int NumModules=5);
  virtual ~ Tau();

  int change_values(Long_t mod);
  int load_info(Long_t mod);

protected:
  Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
  void findtau();
  
  TGNumberEntry *chanCopy;
  short int chanNumber;
  short int modNumber;
  char tmp[10];

  bool Load_Once;
  float decay;
};

#endif /*TAU_H_ */
