#ifndef MAIN_H_
#define MAIN_H_

#define INITIAL_HIGHT 500
#define INITIAL_WIDTH 600
#define MCA_SIZE 32768

#include <TGFrame.h>
#include <TGMenu.h>
#include <TGApplication.h>
#include <TGTab.h>
#include <TGButton.h>
#include <TGNumberEntry.h>
#include <TGLabel.h>
#include <TGTextEntry.h>
#include <TRootEmbeddedCanvas.h>
#include <TH1S.h>
#include <TString.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TGFileDialog.h>
#include <TCanvas.h>
#include <iostream>
#include <fstream>

#include "PulseShape.h"
#include "Cfd.h"
#include "Qdc.h"
#include "EnergyFilter.h"
#include "AnalogSignal.h"
#include "TGStatusBar.h"
#include "Csra.h"
#include "TriggerFilter.h"
#include "Baseline.h"
#include "Tau.h"
#include "ExpertMod.h"
#include "LogicTrigger.h"
#include "FPOutputs.h"
#include "Offline.h"
#include "Histogram.h"
#include "ScopedT.h"
#include "Popup.h"
#include "MultiplicityMask.h"

#include "TVirtualPad.h"
#include "TObject.h"
#include "TGDoubleSlider.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "../software/app/pixie16app_export.h"
using namespace std;

enum Commands //commands for the menu bar popups
  {
    FILE_EXIT,
    BOOT_BUTTON,
    ABOUT,
    READ_WF,
    SAVE_SEC,
    MODULE_NUMBER,
    MODULE_NUMBER_MCA,
    CHANNEL_NUMBER,
    CHANNEL_NUMBER_MCA,
    ASG,
    BASELINE,
    EFILTER,
    TFILTER,
    CSRA,
    PULSE,
    CFDP,
    QDCP,
    DECAY,
    FILE_SAVE,
    MODVAR,
    LOGIC,
    MULTIPLICITYMASK,
    FRONTPANELOUTPUTS,
    CHANVAR,
    CLEARMCA,
    SAVEMCA,
    REFRESHMCA,
    STOPMCA,
    STARTMCA,
    CONTINUEMCA,
    MCACHECKBUTTON,
    HISTOGRAM,
    MAXMIN,
    FIND_WF,
    SCOPEDT,
    OFFLINEADJUSTPAR
  };

class Detector;
class Main : public TGMainFrame
{
public:
  //	Main();
  Main(const TGWindow * p);
  virtual ~Main();

  void SetLSFileName();
  void StartLSRun();
  void SetLSonlinedataf();

private:
  //variables
  TGTextEntry *StateMsgFold1;
  //, *StateMsgMCA;
  TH1S  *fHpx_wave;
  TCanvas *dCanvasF1;
  //, *dCanvasMCA;
  PulseShape *pulseshape;
  Cfd *cfd;
  Qdc *qdc;
	
  Popup *about;
  EnergyFilter *energyfilter;
  AnalogSignal *analogsignal;
  Baseline *baseline;
  Csra *csra;
  TriggerFilter *triggerfilter;
  Tau *tau;
  ExpertMod *expertmod;
  LogicTrigger *logictrigger;
  FPOutputs *fpoutputs;
  MultiplicityMask *multiplicitymask;
  Offline *popupoffline;
  Pixel_t color;
  ScopedT *scopedt;
  Histogram* histogram;
  int xmin, xmax, ymin, ymax;
  TGNumberEntry *numericMod, *numericCh;
  int moduleNr; //the module nr we are looking at 
  int channelNr;//the channel nr we are looking at
  unsigned short *trace, *filter_trace;
  float *trace_float;
  int size;//trace length in scope mode
  Detector *detector;
  bool hpx_once_wave;
  bool wave_once;
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t);//process message queue
  void CloseWindow(); //close main window
  void CreateMenuBar(void); //creates menu bar of the main window
  void MakeFold1Panel(TGCompositeFrame *TabPanel);
  void MakeFold2Panel(TGCompositeFrame *TabPanel);

  void NewTrace(unsigned long size, unsigned short module,
		unsigned short ChanNum);
  void writeSpe(const char *filename, float buf[], int dim);
  TGStatusBar* test;
  //, *testMCA;
  void save_setup(char *name);
  int range, separation;///filter parameters
  float fraction; ///filter parameters
  unsigned long mca[MCA_SIZE];
  int NUMBERofTRACES;
  TObject *selected;
  int parts[4];

  // ******** File Setup Variables *********
  char  Filename[PRESET_MAX_MODULES][100];
  char  Histofile[100];
  char  Staticfile[100];
  int   runnum;

  TGTextEntry	*filepathtext;
  TGTextEntry	*filenametext;
  TGNumberEntry	*filerunnum;
  TGTextButton	*filesetdone;

  // ******* ListMode Run Control **********
  TGTextButton	*startdaq;
  TGCheckButton   *onlinechk;
  bool            fonlinedata;
  bool            fstartdaq;
  bool            fstopdaq;
  void 	        LSRunReadData();
  bool IsDirectoryExists(const char *path);//判断文件夹是否存在
  
  ClassDef(Main,1)
};

void DynamicExec();

#endif /*MAIN_H_*/
