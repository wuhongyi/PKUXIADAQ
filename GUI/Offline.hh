// Offline.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 7月 29 20:40:09 2016 (+0800)
// Last-Updated: 四 8月 24 22:39:45 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 70
// URL: http://wuhongyi.cn 

#ifndef _OFFLINE_HH_
#define _OFFLINE_HH_

#include "TCanvas.h"
#include "TGApplication.h"
#include "TGComboBox.h"
#include "TGDoubleSlider.h"
#include "TGFrame.h"
#include "TGImageMap.h"
#include "TGLabel.h"
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
class Offline : public TGTransientFrame
{
public:
  Offline(const TGWindow *p, const TGWindow *main,Detector *det, TGTextEntry *filepath, TGTextEntry	*filename);
  virtual ~Offline();

  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t);//process message queue

  void MakeFold0Panel(TGCompositeFrame *TabPanel);//InitData
  void MakeFold1Panel(TGCompositeFrame *TabPanel);//Adjust Par
  void MakeFold2Panel(TGCompositeFrame *TabPanel);//Wave-16
  void MakeFold3Panel(TGCompositeFrame *TabPanel);//Energy-16
  void MakeFold4Panel(TGCompositeFrame *TabPanel);//Orig Energy
  void MakeFold5Panel(TGCompositeFrame *TabPanel);//FF Thre/CFD Thre
  void MakeFold6Panel(TGCompositeFrame *TabPanel);//Calc Energy
  void MakeFold7Panel(TGCompositeFrame *TabPanel);//QCD
  
private:
  Pixel_t color;
  Detector *detector;
  TObject *selected;
  TGTextEntry *filepathtext;
  TGTextEntry *filenametext;
  
  TCanvas *adjustCanvas;
  TGStatusBar* sbfold3;
  TGDoubleHSlider *dslider;

  TMultiGraph *offlinemultigraph;
  TGraph *rawdata,*threshdata,*cfddata,*cfdthreshdata,*sfilterdata,*ffilterdata;
  unsigned short *RcdTrace;//
  double *doublesample;
  double *doublethresh;
  double *doublecfdthresh;
  double *doublercdtrace;
  double *doublefastfilter;//
  double *doublecfd;//
  double *doubleslowfilter;//

  char offlinefilename[256];
  int modNumber, chanNumber,fileRunNum;//the par used to offline
  TGNumberEntry	*offlinefilerunnum;//int
  TGNumberEntry	*offlinemodnum;//int
  TGNumberEntry	*offlinechnum;//int
  TGNumberEntry* offlinefilterrange;
  void OfflineChangeValues(int mod,int ch);
  void OfflineLoadValues(int mod,int ch);
  unsigned int GetModuleEvents(char *FileName);//return events
  void GetEventsInfo(char *FileName, unsigned int *EventInformation);
  unsigned int *OfflineEventInformation;
  unsigned int OfflineModuleEventsCount;
  unsigned int OfflineCurrentCount;
  
  unsigned int OfflinefRange;
  // 0-fastlength 1-fastgap  2-slowlength  3-slowgap  4-preamptau  5-cfddelay  6-cfdscale 7-fast filterthreshold 8-cfd threshold
  TGNumberEntryField *offlinefilters[9];

  TGTextEntry* OfflineFileStatus;
  TGTextEntry* OfflineCurrentCountText;
  TGTextButton* OfflineReadFileButton;
  TGTextButton* OfflineLoadButton;
  TGTextButton* OfflineApplyButton;
  TGTextButton* OfflineDrawButton;

  enum Commands
    {
      ADJUSTPARSLIDER,
      OFFLINERUNNUM,
      OFFLINEMODNUM,
      OFFLINECHNUM,
      OFFLINECHNUM4,
      OFFLINECHNUM5,
      OFFLINECHNUM6,
      OFFLINEREAD,
      OFFLINELOAD,
      OFFLINEAPPLY,
      OFFLINEDRAW,
      OFFLINEDRAW2,
      OFFLINEDRAW3,
      OFFLINEDRAW4,
      OFFLINEDRAW5,
      OFFLINEDRAW6,
      OFFLINEDRAW7,
      OFFLINEFASTLENGTH,
      OFFLINEFASTGAP,
      OFFLINESLOWLENGTH,
      OFFLINESLOWGAP,
      OFFLINEPREAMPTAU,
      OFFLINECFDDELAY,
      OFFLINECFDSCALE,
      OFFLINECFDTHRESH,
      OFFLINETHRESH,
      OFFLINEFILTERRANGE,
      OFFLINEPROJECTYFF5,
      OFFLINEPROJECTYCFD5,
      OFFLINEORIGINALCFD5,
      OFFLINECALCULATECFD5,
      OFFLINESTOPDRAW6
    };

  // Fold0

  
  // Fold2
  TCanvas *canvas2;
  TGTextButton* OfflineDrawButton2;
  TMultiGraph *offlinemultigraph2[16];
  unsigned int OfflineCurrentCount2[16];
  
  TGraph *rawdata2[16],*threshdata2[16],*cfdthreshdata2[16],*cfddata2[16],*sfilterdata2[16],*ffilterdata2[16];
  unsigned short *RcdTrace2[16];//
  double *doublesample2[16];
  double *doublethresh2[16];
  double *doublecfdthresh2[16];
  double *doublercdtrace2[16];
  double *doublefastfilter2[16];//
  double *doublecfd2[16];//
  double *doubleslowfilter2[16];//

  // Fold3
  TCanvas *canvas3;
  TGTextButton* OfflineDrawButton3;  
  TH1D *offlineth1d3[16];
  
  // Fold4
  TCanvas *canvas4;
  TGTextButton* OfflineDrawButton4;  
  TH1D *offlineth1d4;
  TGNumberEntry	*offlinechnum4;//int
  int chanNumber4;
  TGComboBox *chooseth1dbin4;

  // Fold5
  TCanvas *canvas5;
  TCanvas * OriginalCFDcanvas5;
  TCanvas * CalculateCFDcanvas5;
  TGTextButton* OfflineDrawButton5;  
  TH2D *offlineth2d5_0;
  TH2D *offlineth2d5_1;
  TGNumberEntry	*offlinechnum5;//int
  int chanNumber5;
  unsigned short *RcdTrace5;//
  double *doublefastfilter5;//
  double *doublecfd5;//
  TGTextEntry* printtextinfor5;
  TGTextButton* showprojectyFF5; 
  TGTextButton* showprojectyCFD5; 
  TGTextButton* originalcfd5;
  TGTextButton* calculatecfd5;
  TH1D* originalcfdth1d5;
  TH1D* calculatecfdth1d5;
  
  // Fold6
  TCanvas *canvas6;
  TGTextButton* OfflineDrawButton6;
  TGTextButton* OfflineStopButton6;
  TH1D *offlineth1d6;
  TGNumberEntry	*offlinechnum6;//int
  int chanNumber6;
  TGComboBox *chooseth1dbin6;
  TGTextEntry* printtextinfor6;
  unsigned short *RcdTrace6;//
  double *doublefastfilter6;//
  double *doublecfd6;//
  double *doubleslowfilter6;//
  bool flagdrawstop6;
  // 需要设置一个采样率选择器

  
  // Fold7
  TCanvas *canvas7;



  
  // function
  void Panel1Draw();
  void Panel2Draw();
  void Panel3Draw();
  void Panel4Draw();
  void Panel5Draw();
  void Panel6Draw();
  void Panel7Draw();
  
  void Panel0ReadFile();

  void FFShowProjectY5();
  void CFDShowProjectY5();
  void OriginalCFDShow5();
  void CalculateCFDShow5();
  void Panel6StopDraw();
  
};

#endif /* _OFFLINE_HH_ */
// 
// Offline.hh ends here
