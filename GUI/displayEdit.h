#ifndef DISPLAYEDIT_H_
#define DISPLAYEDIT_H_
#include "TGTextEdit.h"

class displayEdit:public TGTextView
{
public:
  displayEdit(const TGWindow* parent = 0, UInt_t w = 1,UInt_t h = 1);
  //virtual ~displayEdit();
};

#endif /*DISPLAYEDIT_H_*/
