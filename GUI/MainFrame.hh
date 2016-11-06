#ifndef MAINFRAME_HH_
#define MAINFRAME_HH_

const char gAbout[] = "\
Version: 1.0 ---> 2016.11.06\n\
\n\
The program developed by Peking University.\n\
The program applies to Pixie16-RevF.\n\
\n\
\n\
Program Developer:\n\
Hongyi Wu (wuhongyi@qq.com)\n\
Jing Li (lijinger02@163.com)\n\
\n\
Other Contributor:\n\
Zhihuan Li\n\
Hui Tan\n\
\n\
";

#define INITIAL_HIGHT 500
#define INITIAL_WIDTH 600
#define MCA_SIZE 32768

#include "AnalogSignal.hh"
#include "Baseline.hh"
#include "Cfd.hh"
#include "Csra.hh"
#include "EnergyFilter.hh"
#include "ExpertMod.hh"
#include "FPOutputs.hh"
#include "Histogram.hh"
#include "LogicTrigger.hh"
#include "MultiplicityMask.hh"
#include "Offline.hh"
#include "PulseShape.hh"
#include "Qdc.hh"
#include "ScopedT.hh"
#include "Tau.hh"
#include "TriggerFilter.hh"

#include "../software/app/pixie16app_export.h"

#include "TCanvas.h"
#include "TGApplication.h"
#include "TGButton.h"
#include "TGDoubleSlider.h"
#include "TGFileDialog.h"
#include "TGFrame.h"
#include "TGMenu.h"
#include "TGNumberEntry.h"
#include "TGLabel.h"
#include "TGraph.h"
#include "TGStatusBar.h"
#include "TGTab.h"
#include "TGTextEntry.h"
#include "TH1S.h"
#include "TMultiGraph.h"
#include "TObject.h"
#include "TRootEmbeddedCanvas.h"
#include "TRootHelpDialog.h"
#include "TString.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TVirtualPad.h"

#include <fstream>
#include <iostream>
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

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
class MainFrame : public TGMainFrame
{
public:
  //	MainFrame();
  MainFrame(const TGWindow * p);
  virtual ~MainFrame();

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
	
  TRootHelpDialog *about;
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
  
  ClassDef(MainFrame,1)
};

void DynamicExec();

#endif /*MAINFRAME_HH_*/
