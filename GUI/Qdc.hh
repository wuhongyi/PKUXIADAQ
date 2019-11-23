#ifndef QDC_HH
#define QDC_HH

#include "Table.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class Detector;

class Qdc : public Table 
{
public:
  Qdc(const TGWindow *p, const TGWindow *main, char *name, int colunms, int rows,Detector *det);
  ~Qdc();

  int change_values(Long_t mod);
  int load_info(Long_t mod);
  
protected:
  Bool_t ProcessMessage(Long_t msg, Long_t parm1,Long_t parm2);
  
  bool Load_Once;
  short int modNumber;
  short int chanNumber;

  TGNumberEntry* chanCopy;

  double qlen[8];

private:
  Detector *detector;
};

#endif
