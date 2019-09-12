// Offline.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 7月 29 20:40:09 2016 (+0800)
// Last-Updated: 四 9月 12 15:25:32 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 180
// URL: http://wuhongyi.cn 

#ifndef _OFFLINE_HH_
#define _OFFLINE_HH_


#define PANELNUMBER 13
const char gOfflineGuides[PANELNUMBER][128] =
  {
    "InitData: This page. Read the binary file.",
    "Adjust Par:  Waveform/fast filter/energy filter/cfd/threshould of the select channel.",
    "Wave-16:  Waveform/fast filter/energy filter/cfd filter/threshould of the 16 channels.",
    "Energy-16:  The original energy spectrum of the 16 channel.",
    "Orig Energy:  The original energy spectrum of the select channel.",
    "Calc Energy:  Recalculation of energy spectrum by waveforms.",
    "FF/CFD Thre:   Accumulation of a large number of fast filter/cfd filter waveforms to determine the threshold.",
    "Energy-FF:  The two dimensional relationship between energy and fast filter first/second peak height.",
    "Energy-CFD:   The two dimensional relationship between energy and cfd vaule.",
    "Event Flag:   Energy spectrum under different event flags.",
    "QDC:  It will be finished soon.",
    "FFT:  Fourier transformation of a single waveform. User can choose XIA/CAEN/FFTW3 functions.",
    "Time Diff:  Time difference of two signal."
  };

#include "pkuFFTW.hh"

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
class OfflineData;
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
  void MakeFold8Panel(TGCompositeFrame *TabPanel);//Energy-FF
  void MakeFold9Panel(TGCompositeFrame *TabPanel);//FFT
  void MakeFold10Panel(TGCompositeFrame *TabPanel);//Time Diff
  void MakeFold11Panel(TGCompositeFrame *TabPanel);//Energy-CFD
  void MakeFold12Panel(TGCompositeFrame *TabPanel);//Event Flag
  
  void SelectRawEnergySumsBaseline(Bool_t on);
  void SelectQDCSums(Bool_t on);
  void SelectExternalTimestamp(Bool_t on);
  void SelectDrawOptionPanel1(Bool_t on);
  void SelectDrawOptionPanel2(Bool_t on);
  // void SelectSamplingFrequency(Int_t id);

protected:
  // Description:
  // -----------------------------------------------------------------------------
  // This is a function that calculates the FFT for a vector of 'ns' samples.
  // The number ns must be a power of 2. In case it isn't, the closest power of 2
  // smaller than ns will be considered and exceeding samples ignored.
  // The calculation of the FFT is based on imaginary numbers (x = real part,
  // y = imaginary part). However, the input vector is represented as real numbers
  // (unsigned short) and the function returns a vector of real numbers (double)
  // that are the amplitude (i.e. sqrt(x^2 + y^2) ) of the FFT points. 
  // The amplitude is also normalized respect to the maximum amplitude (for 
  // example 4096 for 12 bit samples) and expressed in dB. A contant baseline (for
  // example 0.0000001 which is -140dB) is also added to the value in order to 
  // low clip the FFT points. Since the FFT has two symmetrical lobes, only half
  // points are returned.
  // Input Parameters: 
  // --------------------------------------------------------------------------
  // wave: pointer to the input vector (waveform samples)
  // fft: pointer to the output vector (fft amplitude in dB, half lobe)
  // ns: number of samples of the input vector wave
  // WindowType: Type of windowing for the FFT
  // Return:
  // --------------------------------------------------------------------------
  // Number of pointf of the output vector fft
  int CAEN_FFT(unsigned short *wave, double *fft, int ns, int WindowType);// source from www.caen.it  

  
private:
  Pixel_t color;
  Detector *detector;
  TObject *selected;
  TGTextEntry *filepathtext;
  TGTextEntry *filenametext;
  char offlinefilename[256];
  int modNumber, chanNumber,fileRunNum;

  void OfflineChangeValues(int mod,int ch);
  void OfflineLoadValues(int mod,int ch);

  enum Commands
    {
      ADJUSTPARSLIDER,
      OFFLINERUNNUM,
      OFFLINEMODNUM,
      OFFLINECHNUM,
      OFFLINECHNUM4,
      OFFLINECHNUM5,
      OFFLINECHNUM6,
      OFFLINECHNUM8,
      OFFLINECHNUM9,
      OFFLINECHNUMA10,
      OFFLINECHNUMB10,
      OFFLINECHNUM11,
      OFFLINECHNUM12,
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
      OFFLINEDRAW8,
      OFFLINEDRAW9,
      OFFLINEDRAW10,
      OFFLINEDRAW11,
      OFFLINEDRAW12,
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
      OFFLINEGAUSFIT4,
      OFFLINEGAUSFIT6,
      OFFLINEPROJECTYFF5,
      OFFLINEPROJECTYCFD5,
      OFFLINEORIGINALCFD5,
      OFFLINECALCULATECFD5,
      OFFLINESTOPDRAW5,
      OFFLINESTOPDRAW6,
      OFFLINESTOPDRAW8
    };

  // Fold0
  TGTextEntry* OfflineFileStatus;
  TGTextButton* OfflineReadFileButton;
  
  TGNumberEntry	*offlinefilerunnum;//int
  TGNumberEntry	*offlinemodnum;//int
  
  unsigned int OfflineModuleEventsCount;
  OfflineData *offlinedata;
  unsigned int OfflineCurrentCount;

  TGCheckButton *headerrawenergysumsandbaseline;
  TGCheckButton *headerqdcsums;
  TGCheckButton *headerexternaltimestamp;
  
  // Fold1
  TGTextButton* OfflineLoadButton;
  TGTextButton* OfflineApplyButton;
  TGTextButton* OfflineDrawButton;
  TGTextEntry* OfflineCurrentCountText;
  TGNumberEntry	*offlinechnum;//int
  TGCheckButton *offlinedrawoption1[7];//0-wave 1-slow filter 2-fast filter 3-thres 4-cfd 5-cfd thres 6-cfd ff scale
  TGCheckButton *offlineonlywaveformevent;
  
  // 0-fastlength 1-fastgap  2-slowlength  3-slowgap  4-preamptau  5-cfddelay  6-cfdscale 7-fast filterthreshold 8-cfd threshold
  TGNumberEntryField *offlinefilters[9];
  TGNumberEntry* offlinefilterrange;
  TGNumberEntryField *offlinecurrentcountentry;
  TCanvas *adjustCanvas;
  TGStatusBar* sbfold3;
  TGDoubleHSlider *dslider;

  TGComboBox *chooseslowfilterbaseline;
  TGComboBox *choosedrawmarkerstyle;
  TGNumberEntryField *oldslowfilterparameter[3];
  TGNumberEntry* oldofflinefilterrange;
  
  int tracelength;
  TMultiGraph *offlinemultigraph;
  TGraph *rawdata,*threshdata,*cfddata,*cfdsdata,*cfdthreshdata,*sfilterdata,*ffilterdata;
  unsigned short *RcdTrace;//
  double *doublesample;
  double *doublethresh;
  double *doublecfdthresh;
  double *doublercdtrace;
  double *doublefastfilter;//
  double *doublecfd;//
  double *doublecfds;
  double *doubleslowfilter;//
  unsigned int OfflinefRange;
  bool adjustdslider;
  
  // Fold2
  TCanvas *canvas2;
  TGCheckButton *offlinedrawoption2[6];//0-wave 1-slow filter 2-fast filter 3-thres 4-cfd 5-cfd thres
  TGTextButton* OfflineDrawButton2;
  bool offlinedatastatus2[16];
  TMultiGraph *offlinemultigraph2[16];
  unsigned int OfflineCurrentCount2[16];
  
  TGraph *rawdata2[16],*threshdata2[16],*cfdthreshdata2[16],*cfddata2[16],*sfilterdata2[16],*ffilterdata2[16];
  int tracelength2[16];
  unsigned short *RcdTrace2[16];//
  double *doublesample2[16];
  double *doublethresh2[16];
  double *doublecfdthresh2[16];
  double *doublercdtrace2[16];
  double *doublefastfilter2[16];//
  double *doublecfd2[16];//
  double *doublecfds2[16];//
  double *doubleslowfilter2[16];//

  // Fold3
  TCanvas *canvas3;
  TGTextButton* OfflineDrawButton3;  
  TH1D *offlineth1d3[16];
  int traceoutofrangecount3[16];
  int channeleventcount3[16];
  
  // Fold4
  TCanvas *canvas4;
  TGTextButton* OfflineDrawButton4;
  TGTextButton* GausFitButton4;
  bool falggausfit4;
  TH1D *offlineth1d4;
  TGNumberEntry	*offlinechnum4;//int
  int chanNumber4;
  TGComboBox *chooseth1dbin4;

  // Fold5
  TCanvas *canvas5;
  TCanvas * OriginalCFDcanvas5;
  TCanvas * CalculateCFDcanvas5;
  TGTextButton* OfflineDrawButton5;
  TGTextButton* OfflineStopButton5;
  TH2D *offlineth2d5_0;
  TH2D *offlineth2d5_1;
  TGNumberEntry	*offlinechnum5;//int
  int chanNumber5;
  unsigned short *RcdTrace5;//
  double *doublefastfilter5;//
  double *doublecfd5;//
  double *doublecfds5;//
  TGTextEntry* printtextinfor5;
  // TGComboBox *recommendedsigma5; // TODO
  TGTextButton* showprojectyFF5; 
  TGTextButton* showprojectyCFD5; 
  TGTextButton* originalcfd5;
  TGTextButton* calculatecfd5;
  TH1D* originalcfdth1d5;
  TH1D* calculatecfdth1d5;
  TF1* fitoriginalcfdth1d5;
  TF1* fitcalculatecfdth1d5;
  TLatex *ltxoriginalcfdth1d5;
  TLatex *ltxcalculatecfdth1d5;
  bool falgshowprojectyFF5;
  bool flagshowprojectyCFD5;
  int cfdevenycount5;
  int originalcfdvalidcount5;
  int calculatecfdvalidcount5;
  bool flagdrawstop5;
  
  // Fold6
  TCanvas *canvas6;
  TGTextButton* OfflineDrawButton6;
  TGTextButton* OfflineStopButton6;
  TH1D *offlineth1d6;
  TGTextButton* GausFitButton6;
  bool falggausfit6;
  TGNumberEntry	*offlinechnum6;//int
  int chanNumber6;
  TGComboBox *chooseth1dbin6;
  TGTextEntry* printtextinfor6;
  unsigned short *RcdTrace6;//
  double *doublefastfilter6;//
  double *doublecfd6;//
  double *doublecfds6;
  double *doubleslowfilter6;//
  bool flagdrawstop6;

  TGComboBox *chooseslowfilterbaselinep6;
  TGNumberEntryField *oldslowfilterparameterp6[3];
  TGNumberEntry* oldofflinefilterrangep6;
  
  
  // Fold7
  TCanvas *canvas7;


  // Fold8
  TCanvas *canvas8;
  TGTextButton* OfflineDrawButton8;
  TGTextButton* OfflineStopButton8;
  TGNumberEntry	*offlinechnum8;//int
  int chanNumber8;//
  unsigned short *RcdTrace8;//
  double *doublefastfilter8;//
  TGTextEntry* printtextinfor8;
  bool flagdrawstop8;
  TGraph *energyfffirst8,*energyffsecond8;
  TH2D *histenergyfffirst8,*histenergyffsecond8;
  int countenergyff8[2];
  TGComboBox *choosedrawstyle8;//0-Graph 1-Hist
  TGNumberEntryField *histxyminmax8[4];//0-xmin 1-xmax 2-ymin 3-ymax
  TGComboBox *choosehistbinxy[2];
  
  
  // Fold9
  TCanvas *canvas9;  
  TGTextButton* OfflineDrawButton9;
  TGNumberEntry	*offlinechnum9;//int
  int chanNumber9;//
  TGComboBox *choosedrawstyle9;//0-XIA 1-CAEN 2-fftw3
  TGTextEntry* OfflineCurrentCountText9;
  TGraph *graphfft9;
  unsigned short *RcdTrace9;
  double *BufferFFT9;
  unsigned int OfflineCurrentCount9;
  int tracelength9;
  fftw_complex *in9;
  fftw1d *fft1d9;


  // Fold10
  TCanvas *canvas10;
  TGTextButton* OfflineDrawButton10;
  TGNumberEntry	*offlinechnumA10;//int
  TGNumberEntry	*offlinechnumB10;//int
  int chanNumberA10;//
  int chanNumberB10;//
  TGNumberEntryField *histxminmax10[3];//0-bin 1-xmin 2-xmax
  TH1I *offlineth1i10;
  TGComboBox *choosedrawstyle10;//0-cfd 1-fast filter
  TGTextEntry* printtextinfor10;
  TGCheckButton *offlineenergylimit10;
  TGNumberEntryField *energylimitsab10[4];//0-AL  1-AR  2-BL  3-BR 

  // Fold11
  TCanvas *canvas11;
  TGTextButton* OfflineDrawButton11;
  TGNumberEntry	*offlinechnum11;//int
  int chanNumber11;//
  TH1I *offlineth1icfdinvalid11;
  TH1I *offlineth1icfdvalid11;
  TH1I *offlineth1icfd11;
  TH2I *offlineth2i11;
  TGComboBox *choosedenergybin11;
  TGComboBox *choosedcfdbin11;
  TGTextEntry* printtextinfor11;
  
  // Fold12
  TCanvas *canvas12;
  TGTextButton* OfflineDrawButton12;//
  TGNumberEntry	*offlinechnum12;//int
  int chanNumber12;
  TGTextEntry* printtextinfor12;//
  TH1I *offlineth1ipileupevent12;
  TH1I *offlineth1igoogevent12;
  TH1I *offlineth1itraceevent12;
  TH1I *offlineth1iwithoutteaceevent12;


  
  void DrawButtonStatus(bool flag);
  
  // function
  void Panel1Draw();
  void Panel2Draw();
  void Panel3Draw();
  void Panel4Draw();
  void Panel5Draw();
  void Panel6Draw();
  void Panel7Draw();
  void Panel8Draw();
  void Panel9Draw();
  void Panel10Draw();
  void Panel11Draw();
  void Panel12Draw();
  
  void Panel0ReadFile();

  void GausFit4();
  void GausFit6();
  void FFShowProjectY5();
  void CFDShowProjectY5();
  void OriginalCFDShow5();
  void CalculateCFDShow5();
  void Panel5StopDraw();
  void Panel6StopDraw();
  void Panel8StopDraw();
  
};

void DynamicFFShowProjectY5();
void DynamicCFDShowProjectY5();
void PanelGausFit();


#endif /* _OFFLINE_HH_ */
// 
// Offline.hh ends here
