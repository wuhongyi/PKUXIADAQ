// Online.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 10月  3 10:42:41 2016 (+0800)
// Last-Updated: 三 10月 23 13:39:41 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 74
// URL: http://wuhongyi.cn 

#ifndef _ONLINE_H_
#define _ONLINE_H_

#define FRAME_BG_R  255
#define FRAME_BG_G  255
#define FRAME_BG_B  255

#define TOPBAR_BG_R  211  //26
#define TOPBAR_BG_G  211  //26
#define TOPBAR_BG_B  211  //26

#define TOPBAR_TEXT_R  255
#define TOPBAR_TEXT_G  255
#define TOPBAR_TEXT_B  255

#define TEXTENTRY_TEXT_R  0
#define TEXTENTRY_TEXT_G  0
#define TEXTENTRY_TEXT_B  0


#define COLOR_WHITE_R  255
#define COLOR_WHITE_G  255
#define COLOR_WHITE_B  255

#define COLOR_RED_R  255
#define COLOR_RED_G  0
#define COLOR_RED_B  0

#define COLOR_YELLOW_R  255
#define COLOR_YELLOW_G  255
#define COLOR_YELLOW_B  0

#define COLOR_GREEN_R  0
#define COLOR_GREEN_G  255
#define COLOR_GREEN_B  0

#define COLOR_BLUE_R  0
#define COLOR_BLUE_G  0
#define COLOR_BLUE_B  255

#define COLOR_BLACK_R  0
#define COLOR_BLACK_G  0
#define COLOR_BLACK_B  0

#include "TGButton.h"
#include "TGComboBox.h"
#include "TGFrame.h"
#include "TGMenu.h"
#include "TGTab.h"
#include "TGButton.h"
#include "TGNumberEntry.h"
#include "TGLabel.h"
#include "TGTextEntry.h"
#include "TRootEmbeddedCanvas.h"
#include "TGApplication.h"

#include "TString.h"
#include "TSystem.h"
#include "TStyle.h"
// #include "TGFileDialog.h"
#include "TCanvas.h"
#include "TH1.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <string>
#include <sstream>
#include <semaphore.h>
#include <errno.h>

using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#define PRESET_MAX_MODULES 24

#define SHAREDMEMORYDATAOFFSET 1170 //BYTE
// 1st 4    bytes IDcode for event shared memory
// 2nd 2    bytes number of valid Num Modules in shared memory
// 3rd 4    bytes Run Number
// 4   4    bytes ID for update spectrum
// 5   128  bytes name of data file
// 6   1024 bytes path of data file
// 7   2    bytes software id
// 8   2    bytes software version
#define SHAREDMEMORYDATASTATISTICS 448
#define SHAREDMEMORYDATAENERGYLENGTH 32768
#define SHAREDMEMORYDATAMAXCHANNEL 16


#define SYSTEM_CLOCK_MHZ 100// System FPGA clock frequency in MHz (used for real time counting)


#define INITIAL_HIGHT 850
#define INITIAL_WIDTH 1500

enum OnlineCommands
  {
   FILE_EXIT,
   INIT_BUTTON,
   ONLINEDRAW3,
   ONLINECHNUM3,
   ONLINEMODNUM3
  };

class Online : public TGMainFrame
{
public:
  Online(const TGWindow * p);
  virtual ~Online();

  void Init();
  void StartStop();
  void AlertRead();
  void AlertSave();
  void AlertDefault();

  
private:
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t);//process message queue
  void CloseWindow(); //close main window
  void CreateMenuBar(void); //creates menu bar of the main window
  void MakeFold1Panel(TGCompositeFrame *TabPanel);//Count Rate
  void MakeFold2Panel(TGCompositeFrame *TabPanel);//Alert
  void MakeFold3Panel(TGCompositeFrame *TabPanel);//energy


  void Panel3Draw();


  void LoopRun();
  double GetFileSizeMB(const char *name);//返回MB
  bool IsFileExists(const char *name);//判断文件是否存在
  bool IsDirectoryExists(const char *path);//判断文件夹是否存在

  long get_time();
  
private:
 
  TGTextEntry **ICR;//[0-207] Input rate   >=208 File size
  TGTextEntry **OCR;//[0-207] Output rate  >=208 not used
  TGTextEntry **SampleRate;
  
  uint64_t CurrentTime;
  uint64_t PrevTime;
  uint64_t ElapsedTime;
  char  Filename[PRESET_MAX_MODULES][256];

  uint64_t CurrentProtectionTime;
  uint64_t PrevProtectionTime;
  uint64_t ElapsedProtectionTime;  
  
  TGTextButton	*startloop;
  bool            fstartloop;
  TGTextEntry	*filepathtext;
  TGTextEntry	*filenametext;
  TGTextEntry *StateMsg;

  bool flagrunnumber;
  char charrunstate[16];

  // alert
  TGTextEntry	*alertfilenametext;

  TGNumberEntryField **LowR;//[0-207] Low rate   
  TGNumberEntryField **HighR;//[0-207] High rate  

  
  TGTextButton	*alertread;
  TGTextButton	*alertsave;
  TGTextButton	*alertdefault;
  
  // energy
  TCanvas *canvas3;
  TGTextButton* OnlineDrawButton3;  
  TH1I *onlineth1i3[16];
  TGNumberEntry	*onlinechnum3;//int
  TGNumberEntry	*onlinemodnum3;
  int chanNumber3;
  int modNumber3;
  TGComboBox *chooseenergycanvasmode3;


private:
  int shm_id;
  unsigned char *ptr;
  sem_t *sem;
  int rc;
  unsigned char *buf;  
  unsigned char *buf_new;
  int val;
  
  unsigned int Statistics[SHAREDMEMORYDATASTATISTICS];
  unsigned int Statistics_new[SHAREDMEMORYDATASTATISTICS];

  double RealTime_new;
  double LiveTime_new[16];
  double FastPeaks_new[16];
  double ChanEvents_new[16];
  double RealTime;
  double LiveTime[16];
  double FastPeaks[16];
  double ChanEvents[16];

  unsigned int EnergySpec[SHAREDMEMORYDATAENERGYLENGTH*SHAREDMEMORYDATAMAXCHANNEL];

  
  unsigned int number;
  unsigned short ModNum;
  int RunNumber;
  unsigned int tempN;
  unsigned short tempsampingrate;
  
  ClassDef(Online,1)
};

#endif /* _ONLINE_H_ */
// 
// Online.hh ends here
