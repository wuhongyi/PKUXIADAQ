// CopyPars.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 3月  8 14:18:45 2018 (+0800)
// Last-Updated: 二 10月 22 12:11:11 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 13
// URL: http://wuhongyi.cn 

#ifndef _COPYPARS_H_
#define _COPYPARS_H_

#include "TCanvas.h"
#include "TGApplication.h"
#include "TGButtonGroup.h"
#include "TGComboBox.h"
#include "TGDoubleSlider.h"
#include "TGFrame.h"
#include "TGImageMap.h"
#include "TGLabel.h"
#include "TLatex.h"
#include "TGNumberEntry.h"
#include "TGraph.h"
#include "TGStatusBar.h"
#include "TGTextEntry.h"
#include "TH1.h"
#include "TH2.h"
#include "TObject.h"
#include "TROOT.h"
#include "TString.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TGWindow.h"
#include "TMultiGraph.h"
#include "TRootEmbeddedCanvas.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class Detector;

enum CopyParsCommands
  {
    COPYPARS_CHNUM0,
    COPYPARS_MODNUM0
  };

class CopyPars : public TGTransientFrame
{
public:
  CopyPars(const TGWindow *p, const TGWindow *main,Detector *det);
  virtual ~CopyPars();



protected:
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

  void copy_values();
  void  checkbutton(Long_t parm1);
  
private:
  Detector *detector;

  TGVerticalFrame *columnlabel;
  TGVerticalFrame *column[13];
  TGCheckButton *ckBtn[17][13];//17=16ch+1all  13=mod

  
  TGTextButton *CopyButton, *CancelButton;
  TGCheckButton *checkbitmask[13];
  
private:
  TGNumberEntry	*chnum0;//int
  TGNumberEntry	*modnum0;
  int chanNumber0;
  int modNumber0;
  
};

#endif /* _COPYPARS_H_ */
// 
// CopyPars.hh ends here
