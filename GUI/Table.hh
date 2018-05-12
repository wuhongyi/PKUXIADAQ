#ifndef TABLE_HH_
#define TABLE_HH_

#include "TG3DLine.h"
#include "TGFrame.h"
#include "TGLabel.h"
#include "TGNumberEntry.h"
#include "TGWindow.h"
#include "TGTextEntry.h"

#include <iostream>
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class Table : public TGTransientFrame
{
public:
  Table(const TGWindow * p, const TGWindow * main, int columns,
	int rows,char *name,int NumModules);
  virtual ~Table();

  virtual int LoadInfo(Long_t mod, TGNumberEntryField *** NumEntry, int column, char *parameter);

  void PreFunction();//waittung status info
  void PostFunction();
  
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

  TGHorizontalFrame *StatusFrame;
  TGTextEntry *userstatus;
  Pixel_t color;
  
  int numModules;
  TGNumberEntry *numericMod;
  TGHorizontalFrame *Buttons;

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

#endif /*TABLE_HH_*/
