/*******************************************
 * Proveided by XIA 
 * 
 * Modified By JingLi(Lee) 2016/01/22
 *
 * lijinger02@126.com
 * *****************************************/
#ifndef CSRA_H_
#define CSRA_H_

#include "TGFrame.h"
#include "TGTextEntry.h"
#include "TGLabel.h"
#include "TGButton.h"
#include "TGNumberEntry.h"

class Csra : public TGTransientFrame
{
public:
    Csra(const TGWindow * p, const TGWindow * main,int NumModules);
    virtual ~Csra();

    int load_info(Long_t);
    
private:
    int numModules;
    TGHorizontalFrame * mn, *buttons;
    TGVerticalFrame *mn_vert;
    TGVerticalFrame *column1, *column2, *column3, *column4, *column5,
      *column6, *column7, *column8,*column9,*column10,*column11,*column12,
      *column13,*column14,*column15,*column16,*column17,*column18,*column19,
      *column20,*column21,*column22;//,*column23

    Pixel_t color;
    
  ///////////////////////////////first column//////////////////////
    TGLabel *Labels[17];
    
  //////////////////////////////second column/////////////////////
    TGCheckButton *ckBtn[17], *ckBtn_1[17], *ckBtn_2[17], *ckBtn_3[17],
      *ckBtn_4[17], *ckBtn_5[17], *ckBtn_6[17],*ckBtn_7[17],*ckBtn_8[17],
      *ckBtn_9[17],*ckBtn_10[17],*ckBtn_11[17],*ckBtn_12[17],*ckBtn_13[17],
      *ckBtn_14[17],*ckBtn_15[17],*ckBtn_16[17],*ckBtn_17[17],*ckBtn_18[17],*ckBtn_19[17],*ckBtn_20[17];//,*ckBtn_21[17]


    // 1=black   2=red   3=green   4=blue   5=yellow   6=hot pink
    int make_columns (TGVerticalFrame * column, TGCheckButton * ckBtn_g[17],
		      char *title, char *tooltip, int id,int flagcolor = 1);
    TGCheckButton **tmpckBtn[21];//

  ///////////////buttons//////////////////
    TGTextButton *LoadButton, *CancelButton;
    TGButton *ApplyButton, *ExitButton;
    TGNumberEntry *numericMod;
    int change_values(Long_t);
    bool Load_Once;
    Long_t module_number1;
    Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
    void  checkbutton(Long_t parm1);
    void  getckbuttonaddress();
};

#endif /*CSRA_H_ */
