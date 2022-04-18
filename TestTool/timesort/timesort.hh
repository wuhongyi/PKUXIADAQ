// timesort.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 1月 14 19:41:44 2019 (+0800)
// Last-Updated: 五 9月 13 12:15:55 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 24
// URL: http://wuhongyi.cn 

#ifndef _TIMESORT_H_
#define _TIMESORT_H_

#include "UserDefine.hh"

#include "TString.h"

#include<map>
#include <vector>
#include <algorithm>
#include<iostream>
#include <map>
class TBranch;
class TFile;
class TTree;
class TBenchmark;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

struct eventdata
{
  Long64_t        tsflag;
  Short_t         sr;
  Bool_t          pileup;
  Bool_t          outofr;
  Short_t         sid;
  Short_t         ch;
  UShort_t        evte;
  Long64_t        ts;
  Short_t         cfd;
  Bool_t          cfdft;
  Short_t         cfds;
  UInt_t          qs[8];
  Int_t           nevt;
};

class timesort
{
public:
  timesort(int runnumber);
  virtual ~timesort();

  void Process();

private:


private:
  std::map<Long64_t,eventdata> sortdata;
  std::map<Long64_t,eventdata>::iterator itkey;

  Long64_t flagkey;
  eventdata mapvalue;

  Short_t timeoffset[256];
  UShort_t chlow[256];
  UShort_t chhigh[256];
  
  //old root
  TFile *filefile;
  TTree *tt;
  // Int_t nevt;
  
  //new root
  TFile *file;
  TTree *t;
  Int_t snevt;
  Long64_t tsflag;//ns
  
  TBenchmark *benchmark;
  Long64_t TotalEntry;
  Long64_t CurrentEntry;
  
  
  // Declaration of leaf types
  Short_t         sr;
  Bool_t          pileup;
  Bool_t          outofr;
  Short_t         sid;
  Short_t         ch;
  UShort_t        evte;
  Long64_t        ts;
  Short_t         cfd;
  Bool_t          cfdft;
  Short_t         cfds;
  UInt_t          qs[8];
  Int_t           nevt;
  
   // List of branches
  TBranch        *b_sr;   //!
  TBranch        *b_pileup;   //!
  TBranch        *b_outofr;   //!
  TBranch        *b_sid;   //!
  TBranch        *b_ch;   //!
  TBranch        *b_evte;   //!
  TBranch        *b_ts;   //!
  TBranch        *b_cfd;   //!
  TBranch        *b_cfdft;   //!
  TBranch        *b_cfds;   //!
  TBranch        *b_qs;   //!
  TBranch        *b_nevt;   //!
};

#endif /* _TIMESORT_H_ */
// 
// timesort.hh ends here
