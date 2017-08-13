// sort.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 6月  3 09:26:49 2017 (+0800)
// Last-Updated: 日 8月 13 17:05:36 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 5
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
  UShort_t evte;
  ULong64_t ts;
  Short_t ch;
  Short_t sid;

  TFile *file_out;
  TTree *t_out;
  Int_t nevt;
#if BOARDNUMBER > 1
  UShort_t adc[BOARDNUMBER][16];
  ULong64_t tdc[BOARDNUMBER][16];
#else
  UShort_t adc[16];
  ULong64_t tdc[16];
#endif

  bool flag;
  ULong64_t inittime;
  int windows;//组合事件门宽

  TBenchmark *benchmark;
  
};

#endif /* _SORT_H_ */
// 
// sort.hh ends here
