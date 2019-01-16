// sort.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 6月  3 09:27:02 2017 (+0800)
// Last-Updated: 二 1月 15 09:33:54 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 37
// URL: http://wuhongyi.cn 

#include "UserDefine.hh"
#include "sort.hh"

#include "TBranch.h"
#include "TMath.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

sort::sort(TString rawfilepath,TString outfilepath,TString filename,int runnumber,int timewindows)
{
  windows = timewindows;
  flag = false;
  nevt = 0;
  
  benchmark = new TBenchmark;
  char tempfilename[1024];

  sprintf(tempfilename,"%s%s_R%04d.root",rawfilepath.Data(),filename.Data(),runnumber);
  file_in = new TFile(tempfilename,"READ");
  if(!file_in->IsOpen())
    {
      std::cout<<"Can't open raw root file"<<std::endl;
      exit(1);
    }
  t_in = (TTree*)file_in->Get("tree");

  t_in->SetBranchAddress("sr",&sr_,&b_sr);
  
  t_in->SetBranchAddress("outofr",&outofr_,&b_outofr);
  t_in->SetBranchAddress("qs",&qs,&b_qs);
  
  t_in->SetBranchAddress("evte",&evte,&b_evte);
  t_in->SetBranchAddress("ts",&ts,&b_ts);
  t_in->SetBranchAddress("ch",&ch,&b_ch);
  t_in->SetBranchAddress("sid",&sid,&b_sid);
  t_in->SetBranchAddress("cfd",&cfd_,&b_cfd);
  t_in->SetBranchAddress("cfdft",&cfdft_,&b_cfdft);
  t_in->SetBranchAddress("cfds",&cfds_,&b_cfds);
  TotalEntry = t_in->GetEntries();
  // t_in->Print();
  // std::cout<<"TotalEntry: "<<TotalEntry<<std::endl;

  
  sprintf(tempfilename,"%s%s_R%04d_W%d.root",outfilepath.Data(),filename.Data(),runnumber,windows);
  file_out = new TFile(tempfilename,"RECREATE");
  t_out = new TTree("t","PKU XIA Pixie-16 Event Data");
  t_out->Branch("nevt",&nevt,"nevt/I");

#if BOARDNUMBER > 1
  t_out->Branch("sr",&sr,TString::Format("sr[%d][16]/S",BOARDNUMBER).Data());
  t_out->Branch("adc",&adc,TString::Format("adc[%d][16]/s",BOARDNUMBER).Data());
  t_out->Branch("outofr",&outofr,TString::Format("outofr[%d][16]/O",BOARDNUMBER).Data());
  t_out->Branch("qdc",&qdc,TString::Format("qdc[%d][16][8]/i",BOARDNUMBER).Data());
  t_out->Branch("tdc",&tdc,TString::Format("tdc[%d][16]/L",BOARDNUMBER).Data());
  t_out->Branch("cfd",&cfd,TString::Format("cfd[%d][16]/S",BOARDNUMBER).Data());
  t_out->Branch("cfdft",&cfdft,TString::Format("cfdft[%d][16]/O",BOARDNUMBER).Data());
  t_out->Branch("cfds",&cfds,TString::Format("cfds[%d][16]/S",BOARDNUMBER).Data());
#else
  t_out->Branch("sr",&sr,"sr[16]/S");
  t_out->Branch("adc",&adc,"adc[16]/s");
  t_out->Branch("outofr",&outofr,"outofr[16]/O");
  t_out->Branch("qdc",&qdc,"qdc[16][8]/i");
  t_out->Branch("tdc",&tdc,"tdc[16]/L");
  t_out->Branch("cfd",&cfd,"cfd[16]/S");
  t_out->Branch("cfdft",&cfdft,"cfdft[16]/O");
  t_out->Branch("cfds",&cfds,"cfds[16]/S");
#endif
  
  // t_out->Print();
}

sort::~sort()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void sort::clearopt()
{
  memset(sr,0,sizeof(sr));
  memset(adc,0,sizeof(adc));
  memset(outofr,0,sizeof(outofr));
  memset(qdc,0,sizeof(qdc));
  memset(tdc,0,sizeof(tdc));
  memset(cfd,0,sizeof(cfd));
  memset(cfdft,0,sizeof(cfdft));
  memset(cfds,0,sizeof(cfds));
}

void sort::Process()
{
  benchmark->Start("event");//计时开始

  for (Long64_t entry = 0; entry < TotalEntry; ++entry)
    {
      if(entry % 10000 == 0)
	{
	  std::cout<<"\r"<<"Entry: "<<entry<<"  |  Event: "<<nevt+1;
	  std::cout << std::flush;
	}
      file_in->cd();
      t_in->GetEvent(entry);
  
      if(flag)
	{
	  switch(int(sr_))
	    {
	    case 100:
	      tsnow = ts*10;
	      break;
	    case 250:
	      tsnow = ts*8;
	      break;
	    case 500:
	      tsnow = ts*10;
	      break;
	    default:
	      break;
	    }
	  
	  if(TMath::Abs(tsnow-inittime) <= Long64_t(windows))
	    {
	      ProcessEntry();
	    }
	  else
	    {
	      file_out->cd();	      
	      t_out->Fill();
	      nevt++;
	      InitEvent();
	    }
	}
      else
	{//event header
	  InitEvent();
	  // std::cout<<ts<<"  "<<ch<<"  "<<sid<<std::endl;
	  flag = true;
	}
    }

  std::cout<<std::endl;

  file_in->cd();
  file_in->Close();

  file_out->cd();
  t_out->Fill();
  // t_out->Print();
  t_out->Write();
  file_out->Close();

  
  benchmark->Show("event");//计时结束并输出时间
}

void sort::InitEvent()
{
  clearopt();

  switch(int(sr_))
    {
    case 100:
      inittime = ts*10;
      break;
    case 250:
      inittime = ts*8;
      break;
    case 500:
      inittime = ts*10;
      break;
    default:
      break;
    }
     

  ProcessEntry();
}

void sort::ProcessEntry()
{
#if BOARDNUMBER > 1
  if(evte >= adc[sid-2][ch])
    {
      sr[sid-2][ch] = sr_;
      adc[sid-2][ch] = evte;
      outofr[sid-2][ch] = outofr_;
      tdc[sid-2][ch] = ts;
      cfd[sid-2][ch] = cfd_;
      cfdft[sid-2][ch] = cfdft_;
      cfds[sid-2][ch] = cfds_;
      for (int i = 0; i < 8; ++i)
	{
	  qdc[sid-2][ch][i] = qs[i];
	}
    }
#else
  if(evte >= adc[ch])
    {
      sr[ch] = sr_;
      adc[ch] = evte;
      outofr[ch] = outofr_;
      tdc[ch] = ts;
      cfd[ch] = cfd_;
      cfdft[ch] = cfdft_;
      cfds[ch] = cfds_;
      for (int i = 0; i < 8; ++i)
	{
	  qdc[ch][i] = qs[i];
	}
    }
#endif

}

// 
// sort.cc ends here
