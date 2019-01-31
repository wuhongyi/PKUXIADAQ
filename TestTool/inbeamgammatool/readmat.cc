// readmat.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 6月  5 20:32:23 2016 (+0800)
// Last-Updated: 日 6月  5 22:01:16 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 13
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


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// mat 文件存放4096*4096数组。每个数据类型为ushort，意味着每个pixel最大计数为65535


int readmat()
{
  TCanvas *c1 = new TCanvas("c1","",800,800);
  // gStyle->SetOptStat(0);
 


  
  TH2D *h2 = new TH2D("h2","",4096,0,4096,4096,0,4096);
  TH1D *h1;
  
  ushort data;

  std::ifstream readdata("0ALL-ALL.mat",std::ios::binary);

  for (int i = 0; i < 4096; ++i)
    for (int j = 0; j < 4096; ++j)
      {
	readdata.read((char*)&data,sizeof(ushort));
	if(data > 0)
	  {
	    h2->Fill(i,j,data);
	    
	    // for (int k = 0; k < data; ++k)
	    //   {
	    // 	h2->Fill(i,j);
	    //   }
	  }
      }

  readdata.close();

  h1 = h2->ProjectionX();

  c1->cd();
  
  // h2->Draw("colz");
  
  h1->Draw("hist");
  
  return 0;
}



// 
// readmat.cc ends here
