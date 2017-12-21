// sort.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 6月  3 09:26:49 2017 (+0800)
// Last-Updated: 日 11月 12 17:33:40 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 8
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
  TBranch *b_ch;
  TBranch *b_sid;
  TBranch *b_evte;
  TBranch *b_ts;
  TBranch *b_cfd;
  TBranch *b_cfdft;
  TBranch *b_outofr;
  TBranch *b_qs;
  
  UShort_t evte;
  ULong64_t ts;
  Short_t ch;
  Short_t sid;
  UShort_t cfd_;   // CFD fractional time
  bool cfdft_;//CFD forced trigger bit
  Bool_t outofr_;//Trace Out-of-Range Flag
  UInt_t qs[8];

  TFile *file_out;
  TTree *t_out;
  Int_t nevt;
#if BOARDNUMBER > 1
  UShort_t adc[BOARDNUMBER][16];
  bool outofr[BOARDNUMBER][16];
  UInt_t qdc[BOARDNUMBER][16][8];
  ULong64_t tdc[BOARDNUMBER][16];
  UShort_t cfd[BOARDNUMBER][16]; 
  bool cfdft[BOARDNUMBER][16];
#else
  UShort_t adc[16];
  bool outofr[16];
  UInt_t qdc[16][8];
  ULong64_t tdc[16];
  UShort_t cfd[16];
  bool cfdft[16];
#endif

  bool flag;
  ULong64_t inittime;
  int windows;//组合事件门宽

  TBenchmark *benchmark;
  
};

#endif /* _SORT_H_ */
// 
// sort.hh ends here
