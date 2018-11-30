// sort.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 6月  3 09:26:49 2017 (+0800)
// Last-Updated: 日 6月  3 06:42:30 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 17
// URL: http://wuhongyi.cn 

#ifndef _SORT_H_
#define _SORT_H_

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TROOT.h"
#include "TBenchmark.h"
#include "TH2.h"
#include "TRandom.h"
#include "TStyle.h"
#include "TSystem.h"
#include <fstream>
#include <cstdlib>
#include <cstdio>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class sort
{
public:
  sort(TString rawfilepath,TString outfilepath,TString filename,int runnumber);
  virtual ~sort();

  void Process();
  
private:
  void clearopt();
  void InitEvent();
  void ProcessEntry();
  void FillMatrix();
  
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

  Int_t nevt;
  
  char tempfilename[1024];
  int RunNo;
  bool flag;
  Long64_t inittime;
  Long64_t tsnow;

  TBenchmark *benchmark;
  TRandom *ran;
  int mhit;
  int flagch[100];
  double flage[100];
  double tempenergy;
  TH2I *matrix;
};

#endif /* _SORT_H_ */
// 
// sort.hh ends here
