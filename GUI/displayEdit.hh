#ifndef DISPLAYEDIT_HH_
#define DISPLAYEDIT_HH_

#include "TGTextEdit.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class displayEdit:public TGTextView
{
public:
  displayEdit(const TGWindow* parent = 0, UInt_t w = 1,UInt_t h = 1);
  //virtual ~displayEdit();
};

#endif /*DISPLAYEDIT_HH_*/
