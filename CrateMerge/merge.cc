// merge.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 1月  5 13:51:08 2019 (+0800)
// Last-Updated: 六 1月  5 22:05:45 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 20
// URL: http://wuhongyi.cn 

#include "UserDefine.hh"
#include "merge.hh"

#include "TString.h"
#include <iostream>
#include <fstream>
#include <climits>
#include <cmath>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

merge::merge(int an,char *av[])
{
  TString rootfilepath = ROOTFILEPATH;
  TString filename = RAWFILENAME;
  TString cratefilename[8];
  cratefilename[0] = CRATE0FILEPATH;
  cratefilename[1] = CRATE1FILEPATH;
  cratefilename[2] = CRATE2FILEPATH;
  cratefilename[3] = CRATE3FILEPATH;
  cratefilename[4] = CRATE4FILEPATH;
  cratefilename[5] = CRATE5FILEPATH;
  cratefilename[6] = CRATE6FILEPATH;
  cratefilename[7] = CRATE7FILEPATH;
  

  benchmark = new TBenchmark;
  for (int i = 0; i < 8; ++i)
    {
      fileread[i] = NULL;
      treeread[i] = NULL;

      havedata[i] = false;
      // std::cout<<cratefilename[i]<<std::endl;
    }

  nevent = 0;
  mhitcrate = 0;
  for (int i = 1; i < an; ++i)
    {
      TString RunNumber(av[i]);
      craterun[mhitcrate] = RunNumber.Atoi();
      mhitcrate++;
    }

  TString tempfilename;
  for (int i = 0; i < mhitcrate; ++i)
    {
      tempfilename += TString::Format("_%04d",craterun[i]);
      
      fileread[i] = new TFile(TString::Format("%s%s_R%04d.root",cratefilename[i].Data(),RAWFILENAME,craterun[i]).Data(),"READ");
      if(!fileread[i]->IsOpen())
	{
	  exit(1);
	}
      treeread[i] = (TTree*)fileread[i]->Get("tree");
      TotalEntry[i] = treeread[i]->GetEntries();
      if(TotalEntry[i] > 0) havedata[i] = true;

      std::cout<<TotalEntry[i]<<std::endl;

      treeread[i]->SetBranchAddress("sr", &sr, &b_sr);
      treeread[i]->SetBranchAddress("pileup", &pileup, &b_pileup);
      treeread[i]->SetBranchAddress("outofr", &outofr, &b_outofr);
      treeread[i]->SetBranchAddress("cid", &cid, &b_cid);
      treeread[i]->SetBranchAddress("sid", &sid, &b_sid);
      treeread[i]->SetBranchAddress("ch", &ch, &b_ch);
      treeread[i]->SetBranchAddress("evte", &evte, &b_evte);
      treeread[i]->SetBranchAddress("ts", &ts, &b_ts);
      treeread[i]->SetBranchAddress("ets", &ets, &b_ets);
      treeread[i]->SetBranchAddress("cfd", &cfd, &b_cfd);
      treeread[i]->SetBranchAddress("cfdft", &cfdft, &b_cfdft);
      treeread[i]->SetBranchAddress("cfds", &cfds, &b_cfds);
      treeread[i]->SetBranchAddress("trae", &trae, &b_trae);
      treeread[i]->SetBranchAddress("leae", &leae, &b_leae);
      treeread[i]->SetBranchAddress("gape", &gape, &b_gape);
      treeread[i]->SetBranchAddress("base", &base, &b_base);
      treeread[i]->SetBranchAddress("qs", qs, &b_qs);
      treeread[i]->SetBranchAddress("ltra", &ltra, &b_ltra);
      treeread[i]->SetBranchAddress("data", data, &b_data);
      treeread[i]->SetBranchAddress("dt", dt, &b_dt);
      treeread[i]->SetBranchAddress("nevt", &nevt, &b_nevt);      
    }

  // ==========

  file = new TFile(TString::Format("%s%s_C%d%s.root",rootfilepath.Data(),filename.Data(),mhitcrate,tempfilename.Data()).Data(),"RECREATE");
  t = new TTree("tree","PKU XIA Pixie-16 Multi Crate Data");
  
  t->Branch("sr",&sr,"sr/S");
  
  t->Branch("pileup",&pileup,"pileup/O");
  t->Branch("outofr",&outofr,"outofr/O");

  t->Branch("cid",&cid,"cid/S");
  t->Branch("sid",&sid,"sid/S");
  t->Branch("ch",&ch,"ch/S");
  t->Branch("evte",&evte,"evte/s");
  t->Branch("ts",&ts,"ts/L");
  t->Branch("ets",&ets,"ets/L");
  
  t->Branch("cfd",&cfd,"cfd/S");
  t->Branch("cfdft",&cfdft,"cfdft/O");
  t->Branch("cfds",&cfds,"cfds/S");
  
  t->Branch("trae",&trae,"trae/i");
  t->Branch("leae",&leae,"leae/i");
  t->Branch("gape",&gape,"gape/i");
  t->Branch("base",&base,"base/D");

  t->Branch("qs",&qs,"qs[8]/i");

  t->Branch("ltra",&ltra,"ltra/s");
  t->Branch("data",&data,"data[ltra]/s");
  t->Branch("dt",&dt,"dt[ltra]/s");

  t->Branch("nevt",&nevent,"nevt/I");  
 
}

merge::~merge()
{
for (int i = 0; i < mhitcrate; ++i)
  {
    fileread[i]->cd();
    fileread[i]->Close();
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void merge::Process()
{
  benchmark->Start("multicrate");//计时开始

  bool flagdata = false;
  Long64_t timestamp;
  int mark = -1; 
  nevent = 0;

  for (int i = 0; i < mhitcrate; ++i)
    {
      CrateEntry[i] = 0;
    }
  
  while(true)
    {
      timestamp = LLONG_MAX;

      for (int i = 0; i < mhitcrate; ++i)
	{
	  if(havedata[i])
	    {
	      fileread[i]->cd();
	      b_sr->GetEntry(CrateEntry[i]);
	      b_ts->GetEntry(CrateEntry[i]);

	      switch(sr)
	      	{
	      	case 100:
	      	  if(10*ts < timestamp)
	      	    {
	      	      timestamp = 10*ts;
	      	      mark = i;
	      	    }
	      	  break;
	      	case 250:
	      	  if(8*ts < timestamp)
	      	    {
	      	      timestamp = 8*ts;
	      	      mark = i;
	      	    }
	      	  break;
	      	case 500:
	      	  if(10*ts < timestamp)
	      	    {
	      	      timestamp = 10*ts;
	      	      mark = i;
	      	    }
	      	  break;
	      	default:
	      	  break;
	      	} 
	    }
	}

      fileread[mark]->cd();
      treeread[mark]->GetEvent(CrateEntry[mark]);
      t->Fill();
      nevent++;
      if(nevent%10000 == 0)
	{
	  std::cout<<"\r"<<"nevt: "<<nevent;
	  std::cout << std::flush;
	}

      CrateEntry[mark]++;
      if(CrateEntry[mark] >= TotalEntry[mark]) havedata[mark] = false;
      flagdata = false;
      for (int i = 0; i < mhitcrate; ++i)
	{
	  flagdata += havedata[i];
	}
      if(!flagdata) break;
      
    }
  
  file->cd();
  t->Write();
  file->Close();
  
  std::cout<<std::endl;
  benchmark->Show("multicrate");//计时结束并输出时间
}


// 
// merge.cc ends here
