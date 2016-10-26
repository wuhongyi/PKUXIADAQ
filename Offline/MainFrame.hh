// MainFrame.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 10月 23 15:38:41 2016 (+0800)
// Last-Updated: 三 10月 26 20:20:36 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 8
// URL: http://wuhongyi.github.io 

#ifndef _MAINFRAME_H_
#define _MAINFRAME_H_

#include "algorithm.hh"

#include "TString.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TMultiGraph.h"
#include "TGraph.h"
#include "TBranch.h"


#include "TG3DLine.h"
#include "TGButton.h"
#include "TGButtonGroup.h"
#include "TGCanvas.h"
#include "TGClient.h"
#include "TGColorSelect.h"
#include "TGComboBox.h"
#include "TGDoubleSlider.h"
#include "TGFileDialog.h"
#include "TGFontDialog.h"
#include "TGFrame.h"
#include "TGHtml.h"
#include "TGIcon.h"
#include "TGImageMap.h"
#include "TGLabel.h"
#include "TGLayout.h"
#include "TGListBox.h"
#include "TGListTree.h"
#include "TGMenu.h"
#include "TGMsgBox.h"
#include "TGNumberEntry.h"
#include "TGPicture.h"
#include "TGResourcePool.h"
#include "TGSimpleTable.h"
#include "TGSlider.h"
#include "TGSpeedo.h"
#include "TGSplitter.h"
#include "TGTab.h"
#include "TGTextEdit.h"
#include "TGTextEntry.h"
#include "TGTextView.h"
#include "TGTextViewStream.h"
#include "TGTreeTable.h"
#include "TGTripleSlider.h"
#include "TGWindow.h"
#include "TRootEmbeddedCanvas.h"
#include "TRootHelpDialog.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

enum EMenuIds {
   M_FILE_NEW,
   M_FILE_CLOSE,
   M_FILE_EXIT,

   M_WINDOW_HOR,
   M_WINDOW_VERT,
   M_WINDOW_CASCADE,
   M_WINDOW_OPAQUE,
   M_WINDOW_ARRANGE,

   M_HELP_CONTENTS,
   M_HELP_ABOUT
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MainFrame : public TGMainFrame
{
public:
  MainFrame(const TGWindow * p, UInt_t w, UInt_t h);
  virtual ~MainFrame();

 
  void MakeFold1Panel(TGCompositeFrame *TabPanel);
  void MakeFold2Panel(TGCompositeFrame *TabPanel);

  void HandleMenu(Int_t id);

  
  void SetTTree(TTree *tree);
  Bool_t DrawEntry(Long64_t entry);

private:
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t);//process message queue
  void InitMenu();
  
  Bool_t IsDirectoryExists(const char *path);//判断文件夹是否存在
  
  
private:


private:
  TCanvas *adjustCanvas;
  TMultiGraph *offlinemultigraph;
  TGraph *rawdata,*threshdata,*cfddata,*sfilterdata,*ffilterdata;
  unsigned short *RcdTrace;//
  double *doublesample;
  double *doublethresh;
  double *doublercdtrace;
  double *doublefastfilter;//
  double *doublecfd;//
  double *doubleslowfilter;//


private:
  algorithm *xia;
  

  
  ClassDef(MainFrame,1)
};

#endif /* _MAINFRAME_H_ */
// 
// MainFrame.hh ends here
