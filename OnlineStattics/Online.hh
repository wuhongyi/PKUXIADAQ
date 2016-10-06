// Online.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 10月  3 10:42:41 2016 (+0800)
// Last-Updated: 三 10月  5 19:51:43 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 31
// URL: http://wuhongyi.github.io 

#ifndef _ONLINE_H_
#define _ONLINE_H_

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
#include "TGFileDialog.h"
#include "TCanvas.h"
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
#define SYSTEM_CLOCK_MHZ 100

#define INITIAL_HIGHT 1000
#define INITIAL_WIDTH 1200

enum OnlineCommands
  {
    FILE_EXIT,
    INIT_BUTTON
  };

class Online : public TGMainFrame
{
public:
  Online(const TGWindow * p);
  virtual ~Online();

  void Init();
  void StartStop();
  
private:
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t);//process message queue
  void CloseWindow(); //close main window
  void CreateMenuBar(void); //creates menu bar of the main window
  void MakeFold1Panel(TGCompositeFrame *TabPanel);

  void LoopRun();
  double GetFileSizeMB(const char *name);//返回MB
  bool IsFileExists(const char *name);//判断文件是否存在
  bool IsDirectoryExists(const char *path);//判断文件夹是否存在

  long get_time();
  
private:
  TGVerticalFrame **Column1;
  
  TGTextEntry **cl0;
  TGTextEntry **LabelsI;
  TGTextEntry **LabelsO;
  
  TGTextEntry **ICR;//[0-207] Input rate   >=208 File size
  TGTextEntry **OCR;//[0-207] Output rate  >=208 not used
  TGTextEntry **Labels;
  
  uint64_t CurrentTime;
  uint64_t PrevRateTime;
  uint64_t ElapsedTime;
  char  Filename[PRESET_MAX_MODULES][256];

  
  TGTextButton	*startloop;
  bool            fstartloop;
  TGTextEntry	*filepathtext;
  TGTextEntry	*filenametext;
  TGTextButton	*filesetdone;
  TGTextEntry *StateMsg;

  Pixel_t color;
  bool flagrunnumber;
  char charrunstate[16];
  
private:
  int shm_id;
  unsigned char *ptr;
  sem_t *sem;
  int rc;
  unsigned char *buf;  
  unsigned char *buf_new;
  int val;

  unsigned int Statistics[448];
  unsigned int Statistics_new[448];

  double RealTime_new;
  double LiveTime_new[16];
  double FastPeaks_new[16];
  double ChanEvents_new[16];
  double RealTime;
  double LiveTime[16];
  double FastPeaks[16];
  double ChanEvents[16];

  unsigned int number;
  unsigned short ModNum;
  int RunNumber;
  unsigned int tempN;

  
  ClassDef(Online,1)
};

#endif /* _ONLINE_H_ */
// 
// Online.hh ends here










