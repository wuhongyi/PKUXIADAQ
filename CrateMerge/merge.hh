// merge.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 1月  5 13:50:53 2019 (+0800)
// Last-Updated: 六 1月  5 21:58:55 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 6
// URL: http://wuhongyi.cn 

#ifndef _MERGE_H_
#define _MERGE_H_

#include "TString.h"
#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"
#include "TBenchmark.h"
#include "TH1.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#define MAXTRACEN 100000 // maximum number of trace points

class merge
{
public:
  merge(int an,char *av[]);
  virtual ~merge();

  void Process();
private:
  bool  havedata[8];//如果还有数据则为true
  int flagfile;
  TFile *fileread[8];
  TTree *treeread[8];
  Long64_t TotalEntry[8];
  Long64_t CrateEntry[8];
  
  TFile *file;
  TTree *t;
  TBenchmark *benchmark;

  int mhitcrate;
  int craterun[16];
  
private:
  Short_t sr;//sampling rate
  
  Short_t ch;//channel
  Short_t sid;//slot id
  Short_t cid;//crate id
  Bool_t pileup;
  Long64_t ts;//timestamp
  Short_t cfd;
  Bool_t cfdft;//CFD forced trigger bit
  Short_t cfds;//CFD trigger source bits
  UShort_t evte;//energy
  UShort_t ltra;
  Bool_t outofr;//Trace Out-of-Range Flag
  UInt_t trae;  // trailing energy sum
  UInt_t leae;	// leading energy sum
  UInt_t gape;	// Gap energy sum
  double base;	// baseline value

  UInt_t qs[8];
  Long64_t ets;
  UShort_t data[MAXTRACEN];
  UShort_t dt[MAXTRACEN];
  Int_t nevt;
  Int_t nevent;
  
  // List of branches
  TBranch        *b_sr;   //!
  TBranch        *b_pileup;   //!
  TBranch        *b_outofr;   //!
  TBranch        *b_cid;   //!
  TBranch        *b_sid;   //!
  TBranch        *b_ch;   //!
  TBranch        *b_evte;   //!
  TBranch        *b_ts;   //!
  TBranch        *b_ets;   //!
  TBranch        *b_cfd;   //!
  TBranch        *b_cfdft;   //!
  TBranch        *b_cfds;   //!
  TBranch        *b_trae;   //!
  TBranch        *b_leae;   //!
  TBranch        *b_gape;   //!
  TBranch        *b_base;   //!
  TBranch        *b_qs;   //!
  TBranch        *b_ltra;   //!
  TBranch        *b_data;   //!
  TBranch        *b_dt;   //!
  TBranch        *b_nevt;   //!
  
};

#endif /* _MERGE_H_ */
// 
// merge.hh ends here
