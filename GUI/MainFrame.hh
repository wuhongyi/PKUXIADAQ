#ifndef MAINFRAME_HH_
#define MAINFRAME_HH_

const char gAbout[] = "\
Version: Beta ---> Developer version\n\
\n\
The program developed by Peking University.\n\
The program applies to XIA Pixie-16. \n\
Support 100 MHz(RevD 12bit,RevF 14-bit), \n\
        250 MHz(RevF 12/14/16-bit), \n\
        500 MHz(RevF 12/14-bit)\n\
\n\
\n\
Program Developer:\n\
  Hongyi Wu (wuhongyi@qq.com   Peking University)\n\
  Jing Li (lijinger02@163.com   Peking University)\n\
\n\
Other Contributor:\n\
  Zhihuan Li (Peking University)\n\
  Hui Tan (XIA LLC)\n\
\n\
";

#define INITIAL_HIGHT 500
#define INITIAL_WIDTH 700
#define MCA_SIZE 32768

#include "Base.hh"
#include "Cfd.hh"
#include "Csra.hh"
#include "Energy.hh"
#include "ExpertMod.hh"
#include "Histogram.hh"
#include "LogicTrigger.hh"
#include "Offline.hh"
#include "Qdc.hh"
#include "ScopedT.hh"
#include "Simulation.hh"
#include "TriggerFilter.hh"

#include "../software/app/pixie16app_export.h"

#include "TCanvas.h"
#include "TGApplication.h"
#include "TGButton.h"
#include "TGDoubleSlider.h"
#include "TGFileDialog.h"
#include "TGFrame.h"
#include "TGImageMap.h"
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
    BASE,
    ENERGY,
    TFILTER,
    CSRA,
    CFDP,
    QDCP,
    FILE_SAVE,
    MODVAR,
    LOGIC,
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
    OFFLINEADJUSTPAR,
    SIMULATION
  };

class Detector;
class MainFrame : public TGMainFrame
{
public:
  MainFrame(const TGWindow * p);
  virtual ~MainFrame();

  void SetLSFileName();
  void StartLSRun();
  void SetLSonlinedataf();
  void SetOnlineMode();
  
private:
  //variables
  TGTextEntry *StateMsgFold1;
  //, *StateMsgMCA;
  TH1S  *fHpx_wave;
  TCanvas *dCanvasF1;
  //, *dCanvasMCA;
  Cfd *cfd;
  Qdc *qdc;
	
  TRootHelpDialog *about;
  Energy *energy;
  Base *base;
  Csra *csra;
  TriggerFilter *triggerfilter;
  ExpertMod *expertmod;
  LogicTrigger *logictrigger;
  Offline *popupoffline;
  Simulation *simulation;
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




  TGPopupMenu *MenuFile;
  TGPopupMenu *MenuSetup;
  TGPopupMenu *MenuExpert;
  TGPopupMenu *MenuMonitor;
  TGPopupMenu *MenuOffline;
  TGTextButton *bootB;
  TGTextButton *acquireB;
  TGTextButton *saveB;

  
  // ******** File Setup Variables *********
  char  Filename[PRESET_MAX_MODULES][256];
  char  Histogramname[PRESET_MAX_MODULES][256];
  char  Histofile[100];
  char  Staticfile[100];
  int   runnum;

  TGCheckButton   *onlinemode;
  int flagonlinemode;//0 online 1 offline
  TGTextEntry	*filepathtext;
  TGTextEntry	*filenametext;
  TGNumberEntry	*filerunnum;
  TGTextButton	*filesetdone;

  // ******* ListMode Run Control **********
  TGTextButton	*startdaq;
  TGCheckButton   *onlinechk;
  TGCheckButton   *updateenergyonline;
  bool            fonlinedata;
  bool            fstartdaq;
  bool            fstopdaq;

  // ******* information *******
  TGTextEntry* lastruntextinfor;

  
  void 	        LSRunReadData();
  bool IsDirectoryExists(const char *path);//判断文件夹是否存在
  
  ClassDef(MainFrame,1)
};

void DynamicExec();

#endif /*MAINFRAME_HH_*/
