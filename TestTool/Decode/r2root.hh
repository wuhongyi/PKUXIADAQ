// r2root.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 10月  2 19:11:31 2016 (+0800)
// Last-Updated: 四 12月 17 16:39:41 2020 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 33
// URL: http://wuhongyi.cn 

#ifndef _R2ROOT_H_
#define _R2ROOT_H_

#include "decoder.hh"

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

  int StatisticsOutOfRange[MAXBOARD][16];//invalid count
  int StatisticsPileup[MAXBOARD][16];
  int StatisticsCfdForcedTrigger[MAXBOARD][16];
  int StatisticsEnergy0[MAXBOARD][16];
  int StatisticsWaveformCount[MAXBOARD][16];
  int StatisticsEventCount[MAXBOARD][16];
  
  TFile *file;
  TTree *t;
  TBenchmark *benchmark;
  int Run;
  TString FileName;
  TH1I *tHist[MAXBOARD][16];
  TH1I *eHist[MAXBOARD][16];
  TH1I *cfdHist[MAXBOARD][16];
  
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
  // UShort_t dt[MAXTRACEN];
};

#endif /* _R2ROOT_H_ */
// 
// r2root.hh ends here
