#ifndef TRIGGERFILTER_H_
#define TRIGGERFILTER_H_
#include "Table.h"

class TriggerFilter : public Table
{
public:
  TriggerFilter(const TGWindow * p, const TGWindow * main, char *name, int columns, int rows, int NumModules);
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
  float tpeak,tgap,thresh;
};

#endif /*TRIGGERFILTER_H_*/
