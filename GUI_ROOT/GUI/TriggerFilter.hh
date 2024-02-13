#ifndef TRIGGERFILTER_HH_
#define TRIGGERFILTER_HH_

#include "Table.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class Detector;

class TriggerFilter : public Table
{
public:
  TriggerFilter(const TGWindow * p, const TGWindow * main, char *name, int columns, int rows, Detector *det);
  virtual ~TriggerFilter();

  int change_values(Long_t mod);
  int load_info(Long_t mod);
  
protected:
  Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

  short int chanNumber;
  short int modNumber; 
  TGNumberEntry* chanCopy;
  bool Load_Once;
  char tmp[10];
  float tpeak,tgap,thre;
  
private:
  Detector *detector;
};

#endif /*TRIGGERFILTER_HH_*/
