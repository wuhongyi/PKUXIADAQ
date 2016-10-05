#ifndef EXPERTMOD_H_
#define EXPERTMOD_H_
#include "TGWindow.h"
#include "TGFrame.h"
#include "TGComboBox.h"
#include "TGNumberEntry.h"
#include "TG3DLine.h"
#include "TGLabel.h"
#include <iostream>
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// TODO 这个类有问题，需要大改

class ExpertMod : public TGTransientFrame
{
public:
  ExpertMod(const TGWindow * p, const TGWindow * main,char *name,int NumModules);
  virtual ~ExpertMod();
  
  Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
  TGVerticalFrame* mn_vert;
  TGComboBox *lstBox;
  TGNumberEntry *numericMod;
  int numModules;
  TGHorizontalFrame* Buttons ;
  TGNumberEntryField *NumEntry;
  short int modNumber;
  bool Load_Once;
  int change_values(Long_t mod, int param);
  int load_info(Long_t mod, int param);
  enum Commands
    {
      LOAD,
      APPLY,
      CANCEL,
      MODNUMBER,
      COPYBUTTON
    };
};

#endif /*EXPERTMOD_H_*/
