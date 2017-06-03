// sort.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 6月  3 09:27:02 2017 (+0800)
// Last-Updated: 六 6月  3 12:31:55 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 19
// URL: http://wuhongyi.cn 

#include "sort.hh"

#include "TBranch.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

sort::sort(TString rawfilepath,TString outfilepath,TString filename,int runnumber,int timewindows,int modulenumber)
{
  windows = timewindows;
  module = modulenumber;
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
  TotalEntry = t_in->GetEntries();
  // t_in->Print();
  // std::cout<<"TotalEntry: "<<TotalEntry<<std::endl;

  
  sprintf(tempfilename,"%s%s_R%04d_W%d.root",outfilepath.Data(),filename.Data(),runnumber,windows);
  file_out = new TFile(tempfilename,"RECREATE");
  t_out = new TTree("t","PKU XIA Pixie-16 Event Data");
  t_out->Branch("nevt",&nevt,"nevt/I");
  switch(modulenumber)
    {
    case 1:
      t_out->Branch("adc",&adc,"adc[1][16]/s");
      t_out->Branch("tdc",&tdc,"tdc[1][16]/l");
      break;
    case 2:
      t_out->Branch("adc",&adc,"adc[2][16]/s");
      t_out->Branch("tdc",&tdc,"tdc[2][16]/l");
      break;
    case 3:
      t_out->Branch("adc",&adc,"adc[3][16]/s");
      t_out->Branch("tdc",&tdc,"tdc[3][16]/l");
      break;
    case 4:
      t_out->Branch("adc",&adc,"adc[4][16]/s");
      t_out->Branch("tdc",&tdc,"tdc[4][16]/l");
      break;
    case 5:
      t_out->Branch("adc",&adc,"adc[5][16]/s");
      t_out->Branch("tdc",&tdc,"tdc[5][16]/l");
      break;
    case 6:
      t_out->Branch("adc",&adc,"adc[6][16]/s");
      t_out->Branch("tdc",&tdc,"tdc[6][16]/l");
      break;
    case 7:
      t_out->Branch("adc",&adc,"adc[7][16]/s");
      t_out->Branch("tdc",&tdc,"tdc[7][16]/l");
      break;
    case 8:
      t_out->Branch("adc",&adc,"adc[8][16]/s");
      t_out->Branch("tdc",&tdc,"tdc[8][16]/l");
      break;
    case 9:
      t_out->Branch("adc",&adc,"adc[9][16]/s");
      t_out->Branch("tdc",&tdc,"tdc[9][16]/l");
      break;
    case 10:
      t_out->Branch("adc",&adc,"adc[10][16]/s");
      t_out->Branch("tdc",&tdc,"tdc[10][16]/l");
      break;
    case 11:
      t_out->Branch("adc",&adc,"adc[11][16]/s");
      t_out->Branch("tdc",&tdc,"tdc[11][16]/l");
      break;
    case 12:
      t_out->Branch("adc",&adc,"adc[12][16]/s");
      t_out->Branch("tdc",&tdc,"tdc[12][16]/l");
      break;
    case 13:
      t_out->Branch("adc",&adc,"adc[13][16]/s");
      t_out->Branch("tdc",&tdc,"tdc[13][16]/l");
      break;
    default:
      std::cout<<"Module Number Error !!!"<<std::endl;
      break;
    }
  // t_out->Print();
  
}

sort::~sort()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void sort::clearopt()
{
  memset(adc,0,sizeof(UShort_t)*MAXBOARD*16);
  memset(tdc,0,sizeof(ULong64_t)*MAXBOARD*16);
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

  if(evte > adc[sid-2][ch])
    {
      adc[sid-2][ch] = evte;
      tdc[sid-2][ch] = ts*10;
    }
}

void sort::ProcessEntry()
{
  if(evte > adc[sid-2][ch])
    {
      adc[sid-2][ch] = evte;
      tdc[sid-2][ch] = ts*10;
    }
}

// 
// sort.cc ends here
