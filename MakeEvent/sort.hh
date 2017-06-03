// sort.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 6月  3 09:26:49 2017 (+0800)
// Last-Updated: 六 6月  3 12:11:25 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 4
// URL: http://wuhongyi.cn 

#ifndef _SORT_H_
#define _SORT_H_

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TROOT.h"
#include "TBenchmark.h"
#define MAXBOARD  13
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class sort
{
public:
  sort(TString rawfilepath,TString outfilepath,TString filename,int runnumber,int timewindows,int modulenumber);
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
  UShort_t adc[MAXBOARD][16];
  ULong64_t tdc[MAXBOARD][16];

  bool flag;
  ULong64_t inittime;
  int module;//插件个数
  int windows;//组合事件门宽

  TBenchmark *benchmark;
  
};

#endif /* _SORT_H_ */
// 
// sort.hh ends here
