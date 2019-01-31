// reade4k.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 8月  8 08:54:14 2016 (+0800)
// Last-Updated: 一 8月  8 14:05:31 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 30
// URL: http://wuhongyi.cn 

#include "TCanvas.h"
#include "TPad.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TBranch.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TF1.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TMath.h"
#include "TRandom.h"
#include "TBenchmark.h"
#include "TString.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TSystem.h"

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstdio>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int reade4k()
{
  TCanvas *c1 = new TCanvas("c1","",800,800);
  gStyle->SetOptStat(0);
 
  TH2D *h2 = new TH2D("h2","",4096,0,4096,4096,0,4096);
  TH1D *h1;

  TH1D *hh[6];
  for (int i = 0; i < 6; ++i)
    {
      char name[32];
      sprintf(name,"hh%02d",i);
      hh[i] = new TH1D(name,"",4096,0,4096);
      hh[i]->SetLineColor(i+1);
    }
  
  uint data;

  std::ifstream readdata("2014.e4k",std::ios::binary);
  for (int i = 0; i < 4096; ++i)
    for (int j = 0; j < 4096; ++j)
      {
	readdata.read((char*)&data,sizeof(uint));
	if(data > 0)
	  {
	    h2->Fill(i,j,data);
	    
	    // for (int k = 0; k < data; ++k)
	    //   {
	    // 	h2->Fill(i,j);
	    //   }
	  }
      }

  for (int i = 0; i < 6; ++i)
    for (int j = 0; j < 4096; ++j)
      {
	readdata.read((char*)&data,sizeof(uint));
	if(data > 0)
	  hh[i]->Fill(j,data);
      }

  // 176 char
  double gain[6];// 刻度系数 a0 a1 a2 a3 a4 a5
  int    nterms;// 刻度系数个数
  float  eff_pars[9];// 效率曲线参数 A B C D E F G 100 1000
  int    esc_file_flags[10];
  float finest[5], swpars[3], h_norm;
  float bg_err, encal_err, effcal_err;

  // esc_file_flags[0]   : 0/1 for matrix compressed / not compressed
  // esc_file_flags[1]   : 1 for new versions with systematic errors
  //                          on background and energy, effic calibs
  // esc_file_flags[2]   : 0 for standard type file,
  //                       1 for file with extra data on statistical errors
  //                         (allows use on difference-matrices,
  //                          2d-unfolded matrices etc.)
  // esc_file_flags[3-9] : for future use 
  
  readdata.read((char*)&gain[0],sizeof(double)*6);
  readdata.read((char*)&nterms,sizeof(int));
  readdata.read((char*)&eff_pars[0],sizeof(float)*9);
  readdata.read((char*)&finest[0],sizeof(float)*5);
  readdata.read((char*)&swpars[0],sizeof(float)*3);
  readdata.read((char*)&h_norm,sizeof(float));
  readdata.read((char*)&esc_file_flags[0],sizeof(int)*10);
  readdata.read((char*)&bg_err,sizeof(float));
  readdata.read((char*)&encal_err,sizeof(float));
  readdata.read((char*)&effcal_err,sizeof(float));

  std::cout<< "gain: "<<gain[0]<<" "<<gain[1]<<" "<<gain[2]<<" "<<gain[3]<<" "<<gain[4]<<" "<<gain[5]<<std::endl;
  std::cout<< "nterms: "<<nterms<<std::endl;
  std::cout<< "eff_pars: "<<eff_pars[0]<<" "<<eff_pars[1]<<" "<<eff_pars[2]<<" "<<eff_pars[3]<<" "<<eff_pars[4]<<" "<<eff_pars[5]<<" "<<eff_pars[6]<<" "<<eff_pars[7]<<" "<<eff_pars[8]<<std::endl;
  std::cout<< "esc_file_flags: "<<esc_file_flags[0]<<" "<<esc_file_flags[1]<<" "<<esc_file_flags[2]<<" "<<esc_file_flags[3]<<" "<<esc_file_flags[4]<<" "<<esc_file_flags[5]<<" "<<esc_file_flags[6]<<" "<<esc_file_flags[7]<<" "<<esc_file_flags[8]<<" "<<esc_file_flags[9]<<std::endl; 
  std::cout<< "finest: "<<finest[0]<<" "<<finest[1]<<" "<<finest[2]<<" "<<finest[3]<<" "<<finest[4]<<std::endl;
  std::cout<< "swpars: "<<swpars[0]<<" "<<swpars[1]<<" "<<swpars[2]<<std::endl;
  std::cout<< "h_norm: "<<h_norm<<std::endl;
  std::cout<< "bg_err: "<<bg_err<<std::endl;
  std::cout<< "encal_err: "<<encal_err<<std::endl;
  std::cout<< "effcal_err: "<<effcal_err<<std::endl;
  
  
  readdata.close();

  h1 = h2->ProjectionX();

  c1->cd();
  
  // h2->Draw("surf");

  // std::cout<< int(h2->GetBinContent(3000, 4000)) << "  " << int(h2->GetBinContent(4000, 3000)) << std::endl;
 
  // h1->Draw("hist");

  hh[2]->Draw("hist");
  hh[0]->Draw("histsame");
  hh[1]->Draw("histsame");

  
	
  return 0;
}

// 
// reade4k.cc ends here













