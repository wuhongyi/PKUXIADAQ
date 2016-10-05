#ifndef PULSESHAPE_H_
#define PULSESHAPE_H_

#include "Table.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PulseShape : public Table
{
public:
  PulseShape(const TGWindow * p, const TGWindow * main, char *name,int NumModules=5, int columns=3,  int rows=16);
  virtual ~PulseShape();
  
  int change_values(Long_t mod);
  int load_info(Long_t mod);
  
protected:
  Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

  short int modNumber;
  short int chanNumber;
  char tmp[10];
  float tlength,tdelay;
  bool Load_Once;

  TGNumberEntry* chanCopy;
  //	int PulseShape::LoadInfo(Long_t mod);
};

#endif /*PULSESHAPE_H_*/
