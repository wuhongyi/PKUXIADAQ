// event.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 9月 21 16:28:37 2020 (+0800)
// Last-Updated: 日 12月  4 01:32:32 2022 (+0800)
//	     By: Hongyi Wu(吴鸿毅)
//     Update #: 42
// URL: http://wuhongyi.cn

#include "DataAnalysis.hh"

#include "UserDefine.hh"
#include "TMath.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DataAnalysis::Init()
{
  if(ipt == NULL) return;
  ipt->SetBranchAddress("event", &br_event);

  benchmark = new TBenchmark;
  
  // init TH1/TH2

  


}

void DataAnalysis::Loop(TTree *opt_)
{
  if(opt_ == NULL) return;

  opt = opt_;    
  BranchOpt();
  Long64_t TotalEntry = ipt->GetEntries();

  benchmark->Start("dataana");//计时开始
  
  for(Long64_t jentry = 0; jentry  < TotalEntry; jentry++)
    {
      ipt->GetEntry(jentry);  
      // vref.clear();
      vge.clear();
      vlabr.clear();
      vsi.clear();



      for(int i = 0; i < br_event->size(); i++) 
	{
	  int flag = (*br_event)[i].det;
	  det.det = (*br_event)[i].det;
	  det.id = (*br_event)[i].id;
	  det.raw = (*br_event)[i].raw;
	  det.e = (*br_event)[i].e;
	  det.ts = (*br_event)[i].ts;
	  det.pileup = (*br_event)[i].pileup;
	  det.outofr = (*br_event)[i].outofr;

	  det.subts = 0.0;

	  det.ts = 10*det.ts;
	  if((*br_event)[i].sr == 500)
	    det.subts = ((*br_event)[i].cfds-1+(*br_event)[i].cfd/8192.0)*2.0;

#ifdef WAVEFORM
	det.wave.clear();
	if((*br_event)[i].ltra > 0) //!!!!!!
	  {
	    det.wave.assign((*br_event)[i].data.begin(),(*br_event)[i].data.end());
	    
	    // det.wave.size()    det.wave[i]
	    // 
	    
	  }
#endif	  
	  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

	  
	  if(flag == 0) //ref for eam bursts
	    {
	      vref.clear();
	      vref.push_back(det);
	    }
	  
	  if(flag == 1 && det.raw > 0) 
	    {
	      
	      vge.push_back(det);
	    }
	  
	  if(flag == 2 && det.raw > 100)
	    {

	      vlabr.push_back(det);
	    }

	  if(flag == 4)
	    {


	      vsi.push_back(det);
	    }


	}// br_event->size()

      //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

      // 

      // if(vref.size() > 0 && vppac.size() > 0 && (vls.size() > 0 || vli.size() > 0))
      //  	{
      //  	  for(int i = 0; i < vppac.size(); i++)
      //  	    {
      //  	      ppacref[vppac[i].id]->Fill((vppac[i].ts-vref[0].ts)/1000.0);
      //  
      //  	      double dt = vppac[i].ts+vppac[i].subts-vref[0].ts-vref[0].subts;
      //  	      double ne = TMath::Power(72.3*55/(dt), 2);
      //  	      if(dt > 0)
      //  		nppac[vppac[i].id]->Fill(ne, 1.0/nppac[vppac[i].id]->GetBinWidth(nppac[vppac[i].id]->FindBin(ne)));
      //  	      
      //  	    }
      //  	}


 




      // if(vref.size()>0 || vge.size()>0 || vlabr.size()>0 || vsi.size()>0) 
      // opt->Fill();

	   if((jentry) % 10000 == 0) 
	     {
	       std::cout<< jentry << "/" << TotalEntry << "  process: " <<double(jentry)/TotalEntry << std::endl;
	     }
    }// loop for entry
  std::cout << std::endl;





  // TH1/TH2 write


  benchmark->Show("dataana");//计时结束并输出时间
}

void DataAnalysis::BranchOpt()
{
  br_event = 0;

  // opt->Branch("ref", &vref);
  // opt->Branch("ge", &ge);
  // opt->Branch("labr", &labr);
  // opt->Branch("si", &vsi);


}

// 
// event.cc ends here
