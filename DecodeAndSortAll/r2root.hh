// r2root.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 10月  2 19:11:31 2016 (+0800)
// Last-Updated: 六 2月 17 13:49:50 2024 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 53
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

#include <map>
#include <vector>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#define MAXMODULENUMBER 104  //13*8

struct eventdata
{
  Long64_t        tsflag;
  Short_t         sr;
  Bool_t          pileup;
  Bool_t          outofr;
  Short_t         sid;
  Short_t         cid;
  Short_t         ch;
  UShort_t        evte;
  Long64_t        ts;
  Short_t         cfd;
  Bool_t          cfdft;
  Short_t         cfds;

#ifdef ENERGYSUM
  Bool_t esumf;
  UInt_t trae;  // trailing energy sum
  UInt_t leae;	// leading energy sum
  UInt_t gape;	// Gap energy sum
  UInt_t base;	// baseline value
#endif
  
#ifdef QDCSUM
  Bool_t qsumf;
  UInt_t qs[8];
#endif

#ifdef EXTERNALTS
  Bool_t etsf;
  Long64_t ets;
#endif
  
#ifdef WAVEFORM
  unsigned short ltra;
  unsigned short  *data;
#endif
};

class r2root
{
public:
  r2root(int run[4]);
  virtual ~r2root();

  void Process();

  // Convert a IEEE 754 standrad floating point number (1-bit sign, 8-bit exponent, and 23-bit mantissa) to a decimal fractional number.
  double IEEEFloating2Decimal(unsigned int IEEEFloatingNumber);
  
private:  
  bool IsFileExists(const char *name);//判断文件是否存在

private:  
  decoder *rawdec;
  bool  havedata[MAXMODULENUMBER];//如果还有数据则为true
  int   flagcrate[MAXMODULENUMBER];
  int flagfile;
  Int_t nevt;
  int mmod;

  TFile *file;
  TTree *t;
  TBenchmark *benchmark;
  int Run;
  TString FileName;

  std::map<Long64_t,eventdata> sortdata;
  std::map<Long64_t,eventdata>::iterator itkey;

  Long64_t flagkey;
  eventdata mapvalue;

  Short_t timeoffset[4][MAXBOARD][MAXCHANNEL];
  UShort_t chlow[4][MAXBOARD][MAXCHANNEL];
  UShort_t chhigh[4][MAXBOARD][MAXCHANNEL];  
  
private:
  Short_t sr;//sampling rate
  Bool_t pileup;
  Bool_t outofr;//Trace Out-of-Range Flag
  Short_t ch;//channel
  Short_t sid;//slot id
  Short_t cid;//crate id
  UShort_t evte;//energy
  Long64_t ts;//timestamp
  Short_t cfd;
  Bool_t cfdft;//CFD forced trigger bit
  Short_t cfds;//CFD trigger source bits

#ifdef ENERGYSUM
  Bool_t esumf;
  UInt_t trae;  // trailing energy sum
  UInt_t leae;	// leading energy sum
  UInt_t gape;	// Gap energy sum
  UInt_t base;	// baseline value
#endif
  
#ifdef QDCSUM
  Bool_t qsumf;
  UInt_t qs[8];
#endif

#ifdef EXTERNALTS
  Bool_t etsf;
  Long64_t ets; 	// external clock timestamp
#endif
  
  UShort_t ltra;
  UShort_t data[MAXTRACEN];
  Long64_t tsflag;
  
};

#endif /* _R2ROOT_H_ */
// 
// r2root.hh ends here
