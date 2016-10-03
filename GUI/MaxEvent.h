#ifndef MAXEVENT_H_
#define MAXEVENT_H_
#include "TGWindow.h"
#include "TGFrame.h"
#include "TGTextEntry.h"
#include "TGNumberEntry.h"
#include "TGLabel.h"
// TODO 这个需要大改
class MaxEvent : public TGTransientFrame
{
public:
  MaxEvent (const TGWindow * p, const TGWindow * main);
  virtual ~ MaxEvent ();
  
private:
  TGHorizontalFrame * mn, *buttons;
  TGVerticalFrame *mn_vert;
  TGVerticalFrame *column1, *column2;
  TGNumberEntry *numericMod;
  TGTextButton *LoadButton, *CancelButton;
  TGButton *ApplyButton;
  //, *ExitButton;
  Long_t module_number1;
  bool Load_Once;
  Bool_t ProcessMessage (Long_t msg, Long_t parm1, Long_t parm2);
  int load_info ();
  int change_values ();
  TGNumberEntryField *maxEvent[3];
};

#endif /*MAXEVENT_H_*/
