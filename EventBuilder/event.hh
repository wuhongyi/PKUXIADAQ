// event.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 9月 21 16:28:28 2020 (+0800)
// Last-Updated: 六 2月 17 14:23:13 2024 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 19
// URL: http://wuhongyi.cn 

#ifndef _EVENT_H_
#define _EVENT_H_

#include "UserDefine.hh"
#include "TRandom.h"
#include "TString.h"
#include "TROOT.h"
#include "TChain.h"
#include "TTree.h"
#include "TFile.h"
#include "TBenchmark.h"
// #include "TH1.h"
// #include "TH2.h"
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <climits>
#include <cmath>
// #include <unistd.h> 

#define MAXBOARD  24  //预设最大24个
#define MAXCHANNEL 64

typedef struct DATAEVENT
{
  Short_t      det;//探测器类型
  Short_t      id;//探测器编号

  Short_t         sr;
  Long64_t        ts;
  UShort_t        raw;//原始能量道址
  double	  e;//刻度能量
  
  Bool_t          pileup;
  Bool_t          outofr;
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
  // unsigned short *data;
  std::vector<unsigned short> data;
#endif
  // Long64_t        tsflag;
  // Short_t         sid;
  // Short_t         cid;
  // Short_t         ch;
  // UShort_t        evte;

} DATAEVENT;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class event
{
public:
  event(int run);
  virtual ~event();

  void Process();
  void InitEvent();
  void ProcessEntry();
  void EndEvent();

private:
  Short_t flagdet[4][MAXBOARD][MAXCHANNEL];
  Short_t flagdetid[4][MAXBOARD][MAXCHANNEL];
  double  calia0[4][MAXBOARD][MAXCHANNEL];
  double  calia1[4][MAXBOARD][MAXCHANNEL];
  double  calia2[4][MAXBOARD][MAXCHANNEL];


  DATAEVENT hit;
  std::vector<DATAEVENT> event_vec;

  bool flag;
  Long64_t inittime;
  Long64_t tsnow;
  
private:

  Int_t nevt;
  TBenchmark *benchmark;
  
  // TFile *file_in;
  // TTree *t_in;
  TChain *t_in;
  Long64_t TotalEntry;

  TFile *file_out;
  TTree *t_out;

  
  TBranch        *b_sr;   //!
  TBranch        *b_pileup;   //!
  TBranch        *b_outofr;   //!
  TBranch        *b_cid;   //!
  TBranch        *b_sid;   //!
  TBranch        *b_ch;   //!
  TBranch        *b_evte;   //!
  TBranch        *b_ts;   //!
  TBranch        *b_cfd;   //!
  TBranch        *b_cfdft;   //!
  TBranch        *b_cfds;   //!

#ifdef ENERGYSUM
  TBranch        *b_esumf;
  TBranch        *b_trae;  // trailing energy sum
  TBranch        *b_leae;	// leading energy sum
  TBranch        *b_gape;	// Gap energy sum
  TBranch        *b_base;	// baseline value
#endif
  
#ifdef QDCSUM
  TBranch        *b_qsumf;
  TBranch        *b_qs;
#endif

#ifdef EXTERNALTS
  TBranch        *b_etsf;
  TBranch        *b_ets; 	// external clock timestamp
#endif
  
#ifdef WAVEFORM
  TBranch        *b_ltra;   //!
  TBranch        *b_data;   //!
#endif
  
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
  
#ifdef WAVEFORM
  UShort_t ltra;
  UShort_t data[65536];   //[ltra]
#endif
  
};

#endif /* _EVENT_H_ */
// 
// event.hh ends here
