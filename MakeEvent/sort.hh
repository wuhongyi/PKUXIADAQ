// sort.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 6月  3 09:26:49 2017 (+0800)
// Last-Updated: 一 8月 14 19:15:27 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 6
// URL: http://wuhongyi.cn 

#ifndef _SORT_H_
#define _SORT_H_

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// 以下部分用户需要修改

// 设置插件个数
#define BOARDNUMBER 5

// 以上部分用户需要修改
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


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
  UShort_t evte;
  ULong64_t ts;
  Short_t ch;
  Short_t sid;
  UShort_t cfd_;   // CFD fractional time
  bool cfdft_;//CFD forced trigger bit
  

  TFile *file_out;
  TTree *t_out;
  Int_t nevt;
#if BOARDNUMBER > 1
  UShort_t adc[BOARDNUMBER][16];
  ULong64_t tdc[BOARDNUMBER][16];
  UShort_t cfd[BOARDNUMBER][16]; 
  bool cfdft[BOARDNUMBER][16];
#else
  UShort_t adc[16];
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
