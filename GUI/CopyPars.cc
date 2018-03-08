// CopyPars.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 3月  8 14:19:00 2018 (+0800)
// Last-Updated: 四 3月  8 14:30:26 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 2
// URL: http://wuhongyi.cn 

#include "CopyPars.hh"

#include "Detector.hh"
#include "Global.hh"

#include "pixie16app_export.h"
#include "pixie16sys_export.h"
#include "xia_common.h"

#include "TGTab.h"
#include "TString.h"
#include "TFitResultPtr.h"

#include <cstring>
#include <iostream>
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CopyPars::CopyPars(const TGWindow *p, const TGWindow *main,Detector *det)
{
  SetCleanup(kDeepCleanup);
  detector = det;



  
  SetWindowName("Copy Pars");
  MapSubwindows();
  MapWindow();
  Resize();
}

CopyPars::~CopyPars()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Bool_t CopyPars::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{




  
  return kTRUE;
}


// 
// CopyPars.cc ends here
