#ifndef BASELINE_HH_
#define BASELINE_HH_

#include "Table.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class Baseline : public Table
{
  
public:
  Baseline(const TGWindow * p, const TGWindow * main, 
	   char *name, int columns,
	   int rows, int NumModules);
  virtual ~Baseline();
  
  int change_values(Long_t mod);
  int load_info(Long_t mod);
  Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

protected:
  short int modNumber;
  bool Load_Once;
  TGNumberEntry* chanCopy;
  float blcut,blpercent;
  unsigned short chanNumber;
};

#endif /*BASELINE_HH_*/
