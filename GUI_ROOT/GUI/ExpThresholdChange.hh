// ExpThresholdChange.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 2月 15 20:36:11 2021 (+0800)
// Last-Updated: 二 2月 16 18:48:29 2021 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 7
// URL: http://wuhongyi.cn 

#ifndef _EXPTHRESHOLDCHANGE_H_
#define _EXPTHRESHOLDCHANGE_H_


// Scan 可以从左侧的选项中选取，然后显示到面板中
// Save 将当前面板中的数据保存到左侧选项中
// Apply 将左侧的选项中的参数加载到机箱

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

#include <fstream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class Detector;

enum ExpThresholdChangeCommands
  {
   
   BUTTON_SCAN,
   BUTTON_SAVE,
   BUTTON_APPLY
   

  };


class ExpThresholdChange : public TGTransientFrame
{
public:
  ExpThresholdChange(const TGWindow *p, const TGWindow *main,Detector *det);
  virtual ~ExpThresholdChange();


protected:
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

  void Scan_Pars();
  void Save_Pars();
  void Apply_Pars();
  
private:
  Detector *detector;


  TGNumberEntryField **ThreNum;// 208
  TGComboBox *parsthreBox;
  TGTextButton *ScanButton, *SaveButton, *ApplyButton;
  
};


#endif /* _EXPTHRESHOLDCHANGE_H_ */

// 
// ExpThresholdChange.hh ends here
