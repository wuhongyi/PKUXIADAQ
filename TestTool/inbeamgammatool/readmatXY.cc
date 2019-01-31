// readmatXY.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 6月  5 21:21:29 2016 (+0800)
// Last-Updated: 日 6月  5 22:53:47 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 12
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

// mat 投影谱中，前1024个字符为0，需要先抛弃。之后为4096个uint类型数据，分别为每道计数。

int readmatXY()
{
  TCanvas *c1 = new TCanvas("c1","",800,800);
  // gStyle->SetOptStat(0);
  std::cout<<c1->GetShowEventStatus()<<std::endl;

  std::cout<<c1->GetShowEventStatus()<<std::endl;

  
  TH1D *h1 = new TH1D("h1","",4096,0,4096);
  
  uint data;
  char temp[1024];

  std::ifstream readdata("0ALL-ALL.X",std::ios::binary);

  readdata.read(temp,sizeof(char)*1024);

  for (int i = 0; i < 4096; ++i)
    {
      readdata.read((char*)&data,sizeof(uint));
      if(data > 0)
	{
	  h1->Fill(i,data);
	  
	  // for (int j = 0; j < data; ++j)
	  //   {
	  //     h1->Fill(i);
	  //   }
	}
    }

  readdata.close();

  c1->cd();
  h1->Draw("hist");
  
  return 0;
}

// 
// readmatXY.cc ends here
