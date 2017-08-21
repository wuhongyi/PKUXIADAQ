// sort.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 6月  3 09:27:02 2017 (+0800)
// Last-Updated: 五 8月 18 20:03:32 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 23
// URL: http://wuhongyi.cn 

#include "sort.hh"

#include "TBranch.h"
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
  file_in = new TFile(tempfilename,"READ");//"RECREATE" "READ"
  if(!file_in->IsOpen())
    {
      std::cout<<"Can't open raw root file"<<std::endl;
      exit(1);
    }
  t_in = (TTree*)file_in->Get("tree");
  t_in->SetBranchAddress("evte",&evte,&b_evte);
  t_in->SetBranchAddress("ts",&ts,&b_ts);
  t_in->SetBranchAddress("ch",&ch,&b_ch);
  t_in->SetBranchAddress("sid",&sid,&b_sid);
  t_in->SetBranchAddress("cfd",&cfd_,&b_cfd);
  t_in->SetBranchAddress("cfdft",&cfdft_,&b_cfdft);
  TotalEntry = t_in->GetEntries();
  // t_in->Print();
  // std::cout<<"TotalEntry: "<<TotalEntry<<std::endl;

  
  sprintf(tempfilename,"%s%s_R%04d_W%d.root",outfilepath.Data(),filename.Data(),runnumber,windows);
  file_out = new TFile(tempfilename,"RECREATE");
  t_out = new TTree("t","PKU XIA Pixie-16 Event Data");
  t_out->Branch("nevt",&nevt,"nevt/I");

#if BOARDNUMBER > 1
  t_out->Branch("adc",&adc,TString::Format("adc[%d][16]/s",BOARDNUMBER).Data());
  t_out->Branch("tdc",&tdc,TString::Format("tdc[%d][16]/l",BOARDNUMBER).Data());
  t_out->Branch("cfd",&cfd,TString::Format("cfd[%d][16]/s",BOARDNUMBER).Data());
  t_out->Branch("cfdft",&cfdft,TString::Format("cfdft[%d][16]/O",BOARDNUMBER).Data());
#else
  t_out->Branch("adc",&adc,"adc[16]/s");
  t_out->Branch("tdc",&tdc,"tdc[16]/l");
  t_out->Branch("cfd",&cfd,"cfd[16]/s");
  t_out->Branch("cfdft",&cfdft,"cfdft[16]/O");
#endif
  
  // t_out->Print();
  
}

sort::~sort()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void sort::clearopt()
{
  memset(adc,0,sizeof(adc));
  memset(tdc,0,sizeof(tdc));
  memset(cfd,0,sizeof(cfd));
  memset(cfdft,0,sizeof(cfdft));
}

void sort::Process()
{
  benchmark->Start("event");//计时开始

  for (Long64_t entry = 0; entry < TotalEntry; ++entry)
    {
      if(entry % 10000 == 0) std::cout<<entry<<std::endl;
      file_in->cd();
      t_in->GetEvent(entry);
  
      if(flag)
	{
	  if(ts*10-inittime <= ULong64_t(windows))
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
  inittime = ts*10;

#if BOARDNUMBER > 1
  if(evte > adc[sid-2][ch])
    {
      adc[sid-2][ch] = evte;
      tdc[sid-2][ch] = ts;
      cfd[sid-2][ch] = cfd_;
      cfdft[sid-2][ch] = cfdft_;
    }
#else
  if(evte > adc[ch])
    {
      adc[ch] = evte;
      tdc[ch] = ts;
      cfd[ch] = cfd_;
      cfdft[ch] = cfdft_;
    }
#endif

}

void sort::ProcessEntry()
{
#if BOARDNUMBER > 1
  if(evte > adc[sid-2][ch])
    {
      adc[sid-2][ch] = evte;
      tdc[sid-2][ch] = ts;
      cfd[sid-2][ch] = cfd_;
      cfdft[sid-2][ch] = cfdft_;
    }
#else
  if(evte > adc[ch])
    {
      adc[ch] = evte;
      tdc[ch] = ts;
      cfd[ch] = cfd_;
      cfdft[ch] = cfdft_;
    }
#endif

}

// 
// sort.cc ends here
