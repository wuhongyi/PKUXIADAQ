// test.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 6月 20 12:40:31 2021 (+0800)
// Last-Updated: 日 6月 20 16:05:45 2021 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 6
// URL: http://wuhongyi.cn 

#include "TString.h"
#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"
#include "TBenchmark.h"
#include "TH1.h"
#include "TRandom.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <unistd.h> 

#include <map>
#include <vector>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#define MAXTRACEN 65536

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
  unsigned short ltra;
  unsigned short  *data;
};

int main(int argc, char *argv[])
{

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

  UShort_t ltra;
  UShort_t data[MAXTRACEN];
  Long64_t tsflag;
  

  std::map<Long64_t,eventdata> sortdata;
  std::map<Long64_t,eventdata>::iterator itkey;

  Long64_t flagkey;
  eventdata mapvalue;

  unsigned int sizelen;
  
  while(true)
    {

      for (int i = 0; i < 100000; ++i)
	{
	  mapvalue.sr = Short_t(gRandom->Uniform(0,8));
	  mapvalue.ch = Short_t(gRandom->Uniform(0,16));
	  mapvalue.sid = Short_t(gRandom->Uniform(0,8));
	  mapvalue.cid = 0;
	  mapvalue.pileup = 0;
	  mapvalue.ts = Long64_t(gRandom->Uniform(0,1000000));
	  mapvalue.cfd = Short_t(gRandom->Uniform(0,32768));
	  mapvalue.cfdft = 0;
	  mapvalue.cfds = 0;
	  mapvalue.evte = UShort_t(gRandom->Uniform(0,65536));
	  mapvalue.outofr = 0;

	  if(gRandom->Rndm()<0.5) mapvalue.ltra = 0;
	  else mapvalue.ltra = UShort_t(gRandom->Uniform(4000,5000));
	  if(mapvalue.ltra > 0)
	    {
	      mapvalue.data = new unsigned short [mapvalue.ltra];
	      for (int j = 0; j < mapvalue.ltra; ++j)
		{
		  mapvalue.data[j] = UShort_t(gRandom->Uniform(0,65536));
		}
	    }

	  mapvalue.tsflag = mapvalue.ts*10;
	  flagkey = ((((((mapvalue.tsflag)<<4)+mapvalue.cid)<<4)+mapvalue.sid)<<4)+mapvalue.ch;
	  sortdata.insert(std::make_pair(flagkey,mapvalue));
	}


      sizelen = sortdata.size() * 0.9;// 0.9
      for (unsigned int i = 0; i < sizelen; ++i)
	{
	  itkey = sortdata.begin();
	  tsflag = itkey->second.tsflag;
	  sr = itkey->second.sr;
	  pileup = itkey->second.pileup;
	  outofr = itkey->second.outofr;
	  cid = itkey->second.cid;
	  sid = itkey->second.sid;
	  ch = itkey->second.ch;
	  evte = itkey->second.evte;
	  ts = itkey->second.ts;
	  cfd = itkey->second.cfd;
	  cfdft = itkey->second.cfdft;
	  cfds = itkey->second.cfds;
	  ltra = itkey->second.ltra;
	  if(ltra > 0)
	    {
	      memcpy(data,itkey->second.data,sizeof(unsigned short)*ltra);
	      delete itkey->second.data;
	    }

	  sortdata.erase(itkey);
	}
      
      sleep(1);
    }

  return 0;
}

// 
// test.cc ends here
