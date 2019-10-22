#ifndef EXPERTMOD_HH_
#define EXPERTMOD_HH_

#include "TG3DLine.h"
#include "TGButton.h"
#include "TGFrame.h"
#include "TGLabel.h"
#include "TGNumberEntry.h"
#include "TGTextEntry.h"
#include "TGWindow.h"
#include "TColor.h"
#include <iostream>
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ExpertMod : public TGTransientFrame
{
public:
  ExpertMod(const TGWindow * p, const TGWindow * main,char *name,int NumModules);
  virtual ~ExpertMod();
  int load_info(Long_t mod);
  
private:
  Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
  TGVerticalFrame* mn_vert;
  TGHorizontalFrame *mn,* Buttons;

  TGVerticalFrame *column1, *column2, *column3;
  TGCheckButton *ckBtn[9];
    
  TGNumberEntry *numericMod;
  int numModules;

  short int modNumber;
  bool Load_Once;
  int change_values(Long_t mod);
  
  enum Commands
    {
      LOAD,
      APPLY,
      CANCEL,
      MODNUMBER,
      COPYBUTTON
    };
};

#endif /*EXPERTMOD_HH_*/
