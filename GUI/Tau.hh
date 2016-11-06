#ifndef TAU_HH_
#define TAU_HH_

#include "Table.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class Tau : public Table
{
public:
  Tau(const TGWindow * p, const TGWindow * main, char *name, int columns, int rows, int NumModules);
  virtual ~ Tau();

  int change_values(Long_t mod);
  int load_info(Long_t mod);

protected:
  Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
  void findtau(short int mod);
  
  TGNumberEntry *chanCopy;
  short int chanNumber;
  short int modNumber;
  char tmp[10];

  bool Load_Once;
  float decay;
};

#endif /*TAU_HH_ */
