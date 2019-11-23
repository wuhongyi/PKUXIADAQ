// ReadChanStatus.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 3月  8 14:57:16 2018 (+0800)
// Last-Updated: 六 11月 23 12:09:24 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 8
// URL: http://wuhongyi.cn 

#ifndef _READCHANSTATUS_H_
#define _READCHANSTATUS_H_

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
#define READTRACELENGTH 8192
#define READBASELINELEENGTH 3640

class Detector;

enum ReadChanStatusCommands
  {
    READCHANSTATUS_DRAW0,
    READCHANSTATUS_DRAW1,
    READCHANSTATUS_CHNUM0,
    READCHANSTATUS_CHNUM1,
    READCHANSTATUS_MODNUM0,
    READCHANSTATUS_MODNUM1
  };

class ReadChanStatus : public TGTransientFrame
{
public:
  ReadChanStatus(const TGWindow *p, const TGWindow *main,Detector *det);
  virtual ~ReadChanStatus();

protected:
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

  void MakeFold0Panel(TGCompositeFrame *TabPanel);//ADC Trace
  void MakeFold1Panel(TGCompositeFrame *TabPanel);//Baseline


  void Panel0Draw();
  void Panel1Draw();
  
private:
  Detector *detector;

  // Fold0
  TCanvas *canvas0;
  TGTextButton* DrawButton0;  
  TH1I *traceth1s0[16];
  TGNumberEntry	*chnum0;//int
  TGNumberEntry	*modnum0;
  int chanNumber0;
  int modNumber0;
  TGComboBox *choosecanvasmode0;

  unsigned short trace[16][READTRACELENGTH];

  
  // Fold1
  TCanvas *canvas1;
  TGTextButton* DrawButton1;  
  TH1I *baselineth1i1[16];
  TGraph *baselinetgraph1[16];
  TGNumberEntry	*chnum1;//int
  TGNumberEntry	*modnum1;
  int chanNumber1;
  int modNumber1;
  TGComboBox *choosecanvasmode1;
  TGComboBox *choosebaselinedrawmode1;
  
  double baselines[16][READBASELINELEENGTH];
  double timestamps[16][READBASELINELEENGTH];

};

#endif /* _READCHANSTATUS_H_ */
// 
// ReadChanStatus.hh ends here
