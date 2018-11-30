// sort.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 6月  3 09:27:02 2017 (+0800)
// Last-Updated: 日 6月  3 06:42:28 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 43
// URL: http://wuhongyi.cn 

#include "UserDefine.hh"
#include "sort.hh"

#include "TBranch.h"
#include "TMath.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

sort::sort(TString rawfilepath,TString outfilepath,TString filename,int runnumber)
{
  flag = false;
  nevt = 0;
  RunNo = runnumber;
  
  benchmark = new TBenchmark;
  ran = new TRandom(0);

  sprintf(tempfilename,"%s%s_R%04d.root",rawfilepath.Data(),filename.Data(),RunNo);
  file_in = new TFile(tempfilename,"READ");
  if(!file_in->IsOpen())
    {
      std::cout<<"Can't open raw root file"<<std::endl;
      exit(1);
    }
  t_in = (TTree*)file_in->Get("tree");

  t_in->SetBranchAddress("sr",&sr_,&b_sr);
  
  t_in->SetBranchAddress("outofr",&outofr_,&b_outofr);
  // t_in->SetBranchAddress("qs",&qs,&b_qs);
  
  t_in->SetBranchAddress("evte",&evte,&b_evte);
  t_in->SetBranchAddress("ts",&ts,&b_ts);
  t_in->SetBranchAddress("ch",&ch,&b_ch);
  t_in->SetBranchAddress("sid",&sid,&b_sid);
  // t_in->SetBranchAddress("cfd",&cfd_,&b_cfd);
  // t_in->SetBranchAddress("cfdft",&cfdft_,&b_cfdft);
  // t_in->SetBranchAddress("cfds",&cfds_,&b_cfds);
  TotalEntry = t_in->GetEntries();
  // t_in->Print();
  // std::cout<<"TotalEntry: "<<TotalEntry<<std::endl;

  
  sprintf(tempfilename,"%s%s_R%04d_m%d.root",outfilepath.Data(),filename.Data(),RunNo,WINDOWS);


  file_out = new TFile(tempfilename,"RECREATE");
  matrix = new TH2I("matrix","PKU in beam gamma matrix",4096,BIN_MIN,BIN_MAX,4096,BIN_MIN,BIN_MAX);
  
}

sort::~sort()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void sort::clearopt()
{
  mhit = 0;
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

      if(sid != 2) continue;
      if(!FLAGCHANNEL[ch]) continue;
      
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
	  
	  if(TMath::Abs(tsnow-inittime) <= Long64_t(WINDOWS))
	    {
	      ProcessEntry();
	    }
	  else
	    {
	      FillMatrix();

	      InitEvent();
	    }
	}
      else
	{//event header
	  InitEvent();
	  flag = true;
	}
    }

  std::cout<<std::endl;

  // write matrix file
  FILE *fp;
  sprintf(tempfilename,"R%04d_m%d.m4b",RunNo,WINDOWS);
  if((fp=fopen(tempfilename,"ab")) == NULL)
    std::cout<<"can't open file .m4b"<<std::endl;
  
  int count;
  for (int i = 1; i <= 4096; ++i)
    for (int j = 1; j <= 4096; ++j)
      {
  	count = int(matrix->GetBinContent(i, j));
  	fwrite(&count,sizeof(int),1,fp);
      }
  
  fclose(fp);



  
  file_in->cd();
  file_in->Close();

  file_out->cd();
  matrix->Write();
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
  tempenergy = (evte + ran->Rndm())*CALI_K[ch]+CALI_B[ch];
  if(tempenergy < THRE_LOW || tempenergy > THRE_HIGH) return;

  flagch[mhit] = ch;
  flage[mhit] = tempenergy;
  mhit++;
}

void sort::FillMatrix()
{
  if(mhit >= 2)
    {
      nevt++;
      // std::cout<<"Event multi 2: "<<nevt<<std::endl;
      for (int i = 0; i < mhit; ++i)
	for (int j = 0; j < mhit; ++j)
	{
	  if(i == j) continue;
	  matrix->Fill(flage[i],flage[j]);
	  
	}

    }

}

// 
// sort.cc ends here
