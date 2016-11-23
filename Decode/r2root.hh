// r2root.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 10月  2 19:11:31 2016 (+0800)
// Last-Updated: 三 11月 23 09:34:28 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 19
// URL: http://wuhongyi.cn 

#ifndef _R2ROOT_H_
#define _R2ROOT_H_

#include "decoder.hh"

#include "TString.h"
#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"
#include "TBenchmark.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <unistd.h> 
// #define MAXTRACEN 10000
#define MAXBOARD  24  //预设最大24个
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class r2root
{
public:
  r2root(TString rawfilepath,TString rootfilepath,TString filename,int  runnumber);
  virtual ~r2root();

  void Process();

private:  
  bool IsFileExists(const char *name);//判断文件是否存在
  void clearopt();

private:  
  decoder *rawdec;
  bool  havedata[MAXBOARD];//如果还有数据则为true
  int flagfile;
  Int_t nevt;
  
  TFile *file;
  TTree *t;
  TBenchmark *benchmark;
  
private:
  Int_t ch;
  Int_t sid;
  Int_t cid;
  Bool_t pileup;
  ULong64_t ts;
  Int_t cfd;
  Bool_t cfdft;//CFD forced trigger bit
  Int_t evte;
  Int_t ltra;
  Bool_t outofr;//Trace Out-of-Range Flag
  Int_t trae;
  UInt_t leae;	// leading energy sum
  UInt_t gape;	// Gap energy sum
  UInt_t base;	// baseline value

  UInt_t qs[8];
  Int_t data[MAXTRACEN];
  Int_t dt[MAXTRACEN];
};

#endif /* _R2ROOT_H_ */
// 
// r2root.hh ends here
