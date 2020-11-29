// event.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 9月 21 16:28:28 2020 (+0800)
// Last-Updated: 五 11月 27 19:53:16 2020 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 11
// URL: http://wuhongyi.cn 

#ifndef _EVENT_H_
#define _EVENT_H_

#include "TRandom.h"
#include "TString.h"
#include "TROOT.h"
#include "TChain.h"
#include "TTree.h"
#include "TFile.h"
#include "TBenchmark.h"
// #include "TH1.h"
// #include "TH2.h"
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <climits>
#include <cmath>
// #include <unistd.h> 

typedef struct DATAEVENT
{
  Short_t      det;//探测器类型
  Short_t      id;//探测器编号

  Short_t         sr;
  Long64_t        ts;
  UShort_t        raw;//原始能量道址
  double	  e;//刻度能量
  
  Bool_t          pileup;
  Bool_t          outofr;
  Short_t         cfd;
  Bool_t          cfdft;
  Short_t         cfds;

  // Long64_t        tsflag;
  // Short_t         sid;
  // Short_t         cid;
  // Short_t         ch;
  // UShort_t        evte;

} DATAEVENT;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class event
{
public:
  event(int run);
  virtual ~event();

  void Process();
  void InitEvent();
  void ProcessEntry();
  void EndEvent();

private:
  Short_t flagdet[8][208];
  Short_t flagdetid[8][208];
  double  calia0[8][208];
  double  calia1[8][208];
  double  calia2[8][208];


  DATAEVENT hit;
  std::vector<DATAEVENT> event_vec;

  bool flag;
  Long64_t inittime;
  Long64_t tsnow;
  
private:

  Int_t nevt;
  TBenchmark *benchmark;
  
  // TFile *file_in;
  // TTree *t_in;
  TChain *t_in;
  Long64_t TotalEntry;

  TFile *file_out;
  TTree *t_out;

  
  TBranch        *b_sr;   //!
  TBranch        *b_pileup;   //!
  TBranch        *b_outofr;   //!
  TBranch        *b_cid;   //!
  TBranch        *b_sid;   //!
  TBranch        *b_ch;   //!
  TBranch        *b_evte;   //!
  TBranch        *b_ts;   //!
  TBranch        *b_cfd;   //!
  TBranch        *b_cfdft;   //!
  TBranch        *b_cfds;   //!
  
  Short_t sr;//sampling rate
  Bool_t pileup;
  Bool_t outofr;//Trace Out-of-Range Flag
  Short_t ch;//channel
  Short_t sid;//slot id
  Short_t cid;//crate id
  UShort_t evte;//energy
  Long64_t ts;//timestamp
  Short_t cfd;
  Bool_t cfdft;//CFD forced trigger bit
  Short_t cfds;//CFD trigger source bits
  
};

#endif /* _EVENT_H_ */
// 
// event.hh ends here
