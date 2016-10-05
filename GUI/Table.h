#ifndef TABLE_H_
#define TABLE_H_
#include "TGWindow.h"
#include "TGFrame.h"
#include "TGTextEntry.h"
#include "TGNumberEntry.h"
#include "TGLabel.h"
#include "TG3DLine.h"
#include <iostream>

using namespace std;


class Table : public TGTransientFrame
{
public:
  Table(const TGWindow * p, const TGWindow * main, int columns,
	int rows,char *name,int NumModules);
  virtual ~Table();

  virtual int LoadInfo(Long_t mod, TGNumberEntryField *** NumEntry, int column, char *parameter);

protected:  
  TGVerticalFrame* mn_vert;
  TGHorizontalFrame* mn;
  int Rows;
  TGVerticalFrame **Column;
  TGTextEntry *cl0;//label for the title of the column[0] 
  TGTextEntry **CLabel;//labels for the numeric columns
  TGNumberEntryField ***NumEntry;//numeric entries [column][row], 
  //column[0] has the labels
  TGTextEntry **Labels; //labels in the left most column

  int numModules;
  TGNumberEntry *numericMod;
  TGHorizontalFrame* Buttons;

  enum Commands
    {
      LOAD,
      APPLY,
      CANCEL,
      MODNUMBER,
      FILTER,
      COPYBUTTON,
      FINDTAU
    };

};

#endif /*TABLE_H_*/
