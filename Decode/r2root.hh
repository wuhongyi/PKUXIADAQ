// r2root.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 10月  2 19:11:31 2016 (+0800)
// Last-Updated: 一 5月 22 14:24:49 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 21
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

  // Convert a IEEE 754 standrad floating point number (1-bit sign, 8-bit exponent, and 23-bit mantissa) to a decimal fractional number.
  double IEEEFloating2Decimal(unsigned int IEEEFloatingNumber);
  
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
  Short_t ch;
  Short_t sid;
  Short_t cid;
  Bool_t pileup;
  ULong64_t ts;
  UShort_t cfd;
  Bool_t cfdft;//CFD forced trigger bit
  UShort_t evte;
  UShort_t ltra;
  Bool_t outofr;//Trace Out-of-Range Flag
  UInt_t trae;  // trailing energy sum
  UInt_t leae;	// leading energy sum
  UInt_t gape;	// Gap energy sum
  double base;	// baseline value

  UInt_t qs[8];
  UShort_t data[MAXTRACEN];
  UShort_t dt[MAXTRACEN];
};

#endif /* _R2ROOT_H_ */
// 
// r2root.hh ends here
