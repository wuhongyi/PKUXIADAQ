// sort.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 6月  3 09:26:49 2017 (+0800)
// Last-Updated: 四 2月 22 17:15:33 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 12
// URL: http://wuhongyi.cn 

#ifndef _SORT_H_
#define _SORT_H_

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TROOT.h"
#include "TBenchmark.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#define MAXBOARD  13

class sort
{
public:
  sort(TString rawfilepath,TString outfilepath,TString filename,int runnumber,int timewindows);
  virtual ~sort();

  void Process();
  
private:
  void clearopt();
  void InitEvent();
  void ProcessEntry();
  
private:
  TFile *file_in;
  TTree *t_in;
  Long64_t TotalEntry;
  TBranch *b_sr;
  TBranch *b_ch;
  TBranch *b_sid;
  TBranch *b_evte;
  TBranch *b_ts;
  TBranch *b_cfd;
  TBranch *b_cfdft;
  TBranch *b_cfds;
  TBranch *b_outofr;
  TBranch *b_qs;

  Short_t sr_;
  UShort_t evte;
  Long64_t ts;
  Short_t ch;
  Short_t sid;
  Short_t cfd_;   // CFD fractional time
  bool cfdft_;//CFD forced trigger bit
  Short_t cfds_;
  Bool_t outofr_;//Trace Out-of-Range Flag
  UInt_t qs[8];

  TFile *file_out;
  TTree *t_out;
  Int_t nevt;
#if BOARDNUMBER > 1
  Short_t sr[BOARDNUMBER][16];
  UShort_t adc[BOARDNUMBER][16];
  bool outofr[BOARDNUMBER][16];
  UInt_t qdc[BOARDNUMBER][16][8];
  Long64_t tdc[BOARDNUMBER][16];
  Short_t cfd[BOARDNUMBER][16]; 
  bool cfdft[BOARDNUMBER][16];
  Short_t cfds[BOARDNUMBER][16]; 
#else
  Short_t sr[16];
  UShort_t adc[16];
  bool outofr[16];
  UInt_t qdc[16][8];
  Long64_t tdc[16];
  Short_t cfd[16];
  bool cfdft[16];
  Short_t cfds[16]; 
#endif

  bool flag;
  Long64_t inittime;
  Long64_t tsnow;
  int windows;//组合事件门宽 ns

  TBenchmark *benchmark;

};

#endif /* _SORT_H_ */
// 
// sort.hh ends here
