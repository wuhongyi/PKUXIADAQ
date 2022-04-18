// timesort.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 1月 14 19:44:11 2019 (+0800)
// Last-Updated: 五 10月  4 13:26:06 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 50
// URL: http://wuhongyi.cn 

#include "timesort.hh"

#include "TBranch.h"
#include "TString.h"
#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"
#include "TBenchmark.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <string.h>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

timesort::timesort(int runnumber)
{
  snevt = 0;
  CurrentEntry = 0;
  benchmark = new TBenchmark;

  memset(timeoffset,0,256*sizeof(Short_t));
  memset(chlow,0,256*sizeof(UShort_t));
  memset(chhigh,0,256*sizeof(UShort_t));
  
  std::ifstream readtxt;
  readtxt.open("par.dat");
  if(!readtxt.is_open())
    {
      std::cout<<"can't open file."<<std::endl;
    }

  std::string str_tmp;
  getline(readtxt,str_tmp);
  // std::cout<<str_tmp<<std::endl;
  Short_t sid_tmp;
  Short_t ch_tmp;
  Short_t offset_tmp;
  UShort_t chlow_tmp;
  UShort_t chhigh_tmp;
  while(!readtxt.eof())
    {
      readtxt>>sid_tmp>>ch_tmp>>offset_tmp>>chlow_tmp>>chhigh_tmp;
      if(readtxt.eof()) break;
      timeoffset[(sid_tmp<<4)+ch_tmp] = offset_tmp;
      chlow[(sid_tmp<<4)+ch_tmp] = chlow_tmp;
      chhigh[(sid_tmp<<4)+ch_tmp] = chhigh_tmp;
    }
  readtxt.close();

  
  filefile = new TFile(TString::Format("%s%s_R%04d.root",RAWFILEPATH,RAWFILENAME,runnumber).Data(),"READ");
  if(!filefile->IsOpen())
    {
      std::cout<<"Can't open raw root file"<<std::endl;
      exit(1);
    }
  tt = (TTree*)filefile->Get("tree");
  tt->SetBranchAddress("sr", &sr, &b_sr);
  tt->SetBranchAddress("pileup", &pileup, &b_pileup);
  tt->SetBranchAddress("outofr", &outofr, &b_outofr);
  tt->SetBranchAddress("sid", &sid, &b_sid);
  tt->SetBranchAddress("ch", &ch, &b_ch);
  tt->SetBranchAddress("evte", &evte, &b_evte);
  tt->SetBranchAddress("ts", &ts, &b_ts);
  tt->SetBranchAddress("cfd", &cfd, &b_cfd);
  tt->SetBranchAddress("cfdft", &cfdft, &b_cfdft);
  tt->SetBranchAddress("cfds", &cfds, &b_cfds);
  tt->SetBranchAddress("qs", qs, &b_qs);
  tt->SetBranchAddress("nevt", &nevt, &b_nevt);
  TotalEntry = tt->GetEntries();

  file = new TFile(TString::Format("%s%s_R%04d_sort.root",ROOTFILEPATH,RAWFILENAME,runnumber).Data(),"RECREATE");
  t = new TTree("tree","PKU Pixie16 Time Sort Data");
  t->Branch("tsflag", &tsflag, "tsflag/L");
  t->Branch("sr", &sr, "sr/S");
  t->Branch("pileup", &pileup, "pileup/O");
  t->Branch("outofr", &outofr, "outofr/O");
  t->Branch("sid", &sid, "sid/S");
  t->Branch("ch", &ch, "ch/S");
  t->Branch("evte", &evte, "evte/s");
  t->Branch("ts", &ts, "ts/L");
  t->Branch("cfd", &cfd, "cfd/S");
  t->Branch("cfdft", &cfdft, "cfdft/O");
  t->Branch("cfds", &cfds, "cfds/S");
  t->Branch("qs", qs, "qs[8]/i");
  t->Branch("nevt", &nevt, "nevt/I");
  // t->SetCacheSize(100*1024*1024);
}

timesort::~timesort()
{
  filefile->cd();
  filefile->Close();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void timesort::Process()
{
  benchmark->Start("timesort");//计时开始

  int posflag;
  while(true)
    {
      if(sortdata.size() <= READENTRYUNDER && CurrentEntry < TotalEntry)
	{
	  for (Long64_t entry = CurrentEntry; entry < TotalEntry; ++entry)
	    {
	      if(entry % 10000 == 0) std::cout<<"Read: "<<entry<< " / "<<TotalEntry<<std::endl;
	      filefile->cd();
	      tt->GetEvent(entry);
	      CurrentEntry = entry+1;
	      posflag = (sid<<4)+ch;
	      if(evte < chlow[posflag] || evte > chhigh[posflag]) continue;

	      mapvalue.sr = sr;
	      mapvalue.pileup = pileup;
	      mapvalue.outofr = outofr;
	      mapvalue.sid = sid;
	      mapvalue.ch = ch;
	      mapvalue.evte = evte;
	      mapvalue.cfd = cfd;
	      mapvalue.cfds = cfds;
	      mapvalue.cfdft = cfdft;
	      for (int i = 0; i < 8; ++i)
		mapvalue.qs[i] = qs[i];
	      mapvalue.nevt = nevt;
	      if(sr == 250)
		{
		  mapvalue.ts = ts+(timeoffset[posflag]/8);
		  mapvalue.tsflag = mapvalue.ts*8;
		}
	      else
		{
		  mapvalue.ts = ts+(timeoffset[posflag]/10);
		  mapvalue.tsflag = mapvalue.ts*10;
		}
	      flagkey = ((((mapvalue.tsflag)<<4)+sid)<<4)+ch;
	      sortdata.insert(std::make_pair(flagkey,mapvalue));
	      
	      
	      if(sortdata.size() >= MAXENTRYINMAP) break;
	    }
	}

      
      if(CurrentEntry >= TotalEntry)
	{
	  // std::cout<<">0  "<<sortdata.size()<<std::endl;
	  while(sortdata.size() > 0)
	    {
	      itkey = sortdata.begin();
	      tsflag = itkey->second.tsflag;
	      sr = itkey->second.sr;
	      pileup = itkey->second.pileup;
	      outofr = itkey->second.outofr;
	      sid = itkey->second.sid;
	      ch = itkey->second.ch;
	      evte = itkey->second.evte;
	      ts = itkey->second.ts;
	      cfd = itkey->second.cfd;
	      cfdft = itkey->second.cfdft;
	      cfds = itkey->second.cfds;
	      for (int i = 0; i < 8; ++i)
		qs[i] = itkey->second.qs[i];
	      nevt = itkey->second.nevt;
	      sortdata.erase(itkey);
	      file->cd();
	      t->Fill();
	      snevt++;
	      // std::cout<<">0  "<<sortdata.size()<<std::endl;
	    }
	}
      else
	{
	  // std::cout<<">50000  "<<sortdata.size()<<std::endl;
	  while(sortdata.size() > READENTRYUNDER)
	    {
	      itkey = sortdata.begin();
	      tsflag = itkey->second.tsflag;
	      sr = itkey->second.sr;
	      pileup = itkey->second.pileup;
	      outofr = itkey->second.outofr;
	      sid = itkey->second.sid;
	      ch = itkey->second.ch;
	      evte = itkey->second.evte;
	      ts = itkey->second.ts;
	      cfd = itkey->second.cfd;
	      cfdft = itkey->second.cfdft;
	      cfds = itkey->second.cfds;
	      for (int i = 0; i < 8; ++i)
		qs[i] = itkey->second.qs[i];
	      nevt = itkey->second.nevt;
	      sortdata.erase(itkey);
	      file->cd();
	      t->Fill();
	      snevt++;
	      // std::cout<<">50000  "<<sortdata.size()<<std::endl;
	    }
	}


     if(CurrentEntry >= TotalEntry && sortdata.size()==0) break;
    }

  file->cd();
  t->Write();
  file->Close();
  
  benchmark->Show("timesort");//计时结束并输出时间
}

// void timesort::SortByTime()
// {
//   std::map<Long64_t, ULong64_t> rawdata;
//   for (Long64_t entry = 0; entry < TotalEntry; ++entry)
//     {
//       if(entry % 10000 == 0) std::cout<<"Read: "<<entry<< " / "<<TotalEntry<<std::endl;
//       filefile->cd();
//       tt->GetEvent(entry);
//       rawdata.insert(std::make_pair(entry,ts));
//     }

//   sortdata = std::vector<PAIR>(rawdata.begin(), rawdata.end());
//   sort(sortdata.begin(), sortdata.end(), CmpByValue());
  
//   // std::cout<< TotalEntry<<"  "<<rawdata.size()<<"  "<<sortdata.size()<<std::endl;

//   // for (int i = 0; i < sortdata.size(); ++i)
//   //   {
//   //     std::cout<<sortdata[i].first << "  " << sortdata[i].second<<std::endl;
//   //   }
// }


// void timesort::SaveByTime()
// {
//   for (Long64_t entry = 0; entry < TotalEntry; ++entry)
//     {
//       if(entry % 10000 == 0) std::cout<<"Save: "<<entry<< " / "<<TotalEntry<<std::endl;
//       // std::cout<<"Save: "<<entry<< " / "<<TotalEntry<<std::endl;
//       filefile->cd();
//       // std::cout<<sortdata[entry].first<<std::endl;
//       tt->GetEvent(sortdata[entry].first);
//       file->cd();
//       t->Fill();
//       snevt++;
//     }

//   file->cd();
//   t->Write();
//   file->Close();
// }


// 
// timesort.cc ends here
