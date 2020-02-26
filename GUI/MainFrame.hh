// MainFrame.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 3月  9 13:01:17 2018 (+0800)
// Last-Updated: 三 2月 26 22:10:55 2020 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 58
// URL: http://wuhongyi.cn 

#ifndef MAINFRAME_HH_
#define MAINFRAME_HH_

#include "Global.hh"
#include "Base.hh"
#include "Cfd.hh"
#include "Csra.hh"
#include "Energy.hh"
#include "ExpertMod.hh"
#include "HistXDT.hh"
#include "LogicTrigger.hh"
#include "Offline.hh"
#include "Qdc.hh"
#include "Decimation.hh"
#include "CopyPars.hh"
#include "Simulation.hh"
#include "TriggerFilter.hh"
#include "ReadChanStatus.hh"

#include "../software/app/pixie16app_export.h"

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
#include "TGSplitter.h"
#include "TGStatusBar.h"
#include "TGTab.h"
#include "TGTextEntry.h"
#include "TH1.h"
#include "TMultiGraph.h"
#include "TObject.h"
#include "TRootHelpDialog.h"
#include "TString.h"
#include "TStyle.h"
#include "TSystem.h"


#include <fstream>
#include <iostream>
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

enum Commands //commands for the menu bar popups
  {
    FILE_EXIT,
    BOOT_BUTTON,
    ABOUT,
    BASE,
    ENERGY,
    TFILTER,
    CSRA,
    CFDP,
    QDCP,
    DECIMATIONP,
    COPYPARS,
    FILE_SAVE,
    MODVAR,
    LOGIC,
    CHANVAR,
    HISTXDT,
    READCHANSTATUS,
    OFFLINEADJUSTPAR,
    SIMULATION
  };

class Detector;
class MainFrame : public TGMainFrame
{
public:
  MainFrame(const TGWindow * p);
  virtual ~MainFrame();

  void SetFileName();
  void ConfigFileInfo();
  void StartRun();
  void SetOnlineDataFlag();
  void SetRecordDataFlag();
  void SetOnlineMode();
#ifdef DECODERONLINE
  void SetDecoderDataFlag();
#endif
  
  
private:
  TGTextEntry *StateMsgFold1;

  TRootHelpDialog *about;

  Decimation *decimation;
  Cfd *cfd;
  Qdc *qdc;
  Energy *energy;
  Base *base;
  TriggerFilter *triggerfilter;
  CopyPars *copypars;
  
  Csra *csra;	
  ExpertMod *expertmod;
  LogicTrigger *logictrigger;

  HistXDT *histxdt;
  ReadChanStatus *readchanstatus;
  
  Offline *popupoffline;
  Simulation *simulation;

  Detector *detector;

  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t);//process message queue
  void CloseWindow(); //close main window
  void CreateMenuBar(void); //creates menu bar of the main window

  void ControlPanel(TGCompositeFrame *TabPanel);

  void save_setup(char *name);

  // TObject *selected;
  int parts[4];

  void SetMenuStatus(bool flag,int flagonline);

  TGPopupMenu *MenuFile;
  TGPopupMenu *MenuSetup;
  TGPopupMenu *MenuExpert;
  TGPopupMenu *MenuMonitor;
  TGPopupMenu *MenuOffline;
  TGTextButton *bootB;

  
  // ******** File Setup Variables *********
  char  LogFileName[1024];
  char  DSPParsFileName[1024];
  char  Filename[PRESET_MAX_MODULES][1024];
  char  Histogramname[PRESET_MAX_MODULES][1024];
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
  TGCheckButton   *recordchk;
  TGCheckButton   *updateenergyonline;
  bool            fonlinedata;
  bool            frecorddata;
  bool            fstartdaq;

#ifdef DECODERONLINE
TGCheckButton   *decoderchk;
#endif
  
  // ******* information *******
  TGTextEntry* lastruntextinfor;

  
  void RunReadData();
  bool IsDirectoryExists(const char *path);//Determine if the folder exists
  bool CreateDirectory(const char *path);//Create folder
  
  ClassDef(MainFrame,1)
};

void DynamicExec();

#endif /*MAINFRAME_HH_*/

// 
// MainFrame.hh ends here

