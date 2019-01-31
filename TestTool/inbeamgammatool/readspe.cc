// readspe.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 6月 12 19:36:02 2016 (+0800)
// Last-Updated: 五 9月 30 09:34:11 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 11
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

// spe 投影谱中，前40个字符为文件头信息，需要先抛弃。之后为4096个uint类型数据，分别为每道计数。

int readspe()
{
  TCanvas *c1 = new TCanvas("c1","",800,800);
  // gStyle->SetOptStat(0);

    TH1D *h1 = new TH1D("h1","",4096,0,4096);
  
  uint data;
  char temp[64];

  std::ifstream readdata("bg.spe",std::ios::binary);

  readdata.read(temp,sizeof(char)*40);
  std::cout<<temp<<std::endl;
  
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
// readspe.cc ends here
