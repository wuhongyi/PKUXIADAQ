#ifndef HISTOGRAM_H_
#define HISTOGRAM_H_
#include "Table.h"

class Histogram : public Table
{
public:
  Histogram(const TGWindow * p, const TGWindow * main, char *name,
	     int columns, int rows, int NumModules);
  virtual ~ Histogram ();
  
  int change_values(Long_t mod);
  int load_info(Long_t mod);
  Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);


protected:
  short int modNumber;
  bool Load_Once;
  TGNumberEntry *chanCopy;
  float emin, bin;
  unsigned short chanNumber;
};

#endif