#ifndef CFD_HH
#define CFD_HH

#include "Table.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class Detector;

class Cfd : public Table
{
public:
  Cfd(const TGWindow *p, const TGWindow *main, char *name,int colunms,int rows,Detector *det);
  virtual ~Cfd();

  Bool_t ProcessMessage(Long_t msg,Long_t parm1,Long_t parm2);
  int load_info(Long_t mod);
  int change_values(Long_t mod);

protected:
  float tdelay,cfrac,thresh;
  bool Load_Once;

  short int modNumber;
  short int chanNumber;
  TGNumberEntry* chanCopy;

private:
  Detector *detector;  
};

#endif
