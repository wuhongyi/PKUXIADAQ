// readMTSort.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 6月 12 21:50:00 2016 (+0800)
// Last-Updated: 二 6月 14 13:01:05 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 26
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

// 512+4096*4096*4+1016

int readMTSort()
{
  TCanvas *c1 = new TCanvas("c1","",800,800);
  // gStyle->SetOptStat(0);

  TH2D *h2 = new TH2D("h2","",4096,0,4096,4096,0,4096);
  TH1D *h1;
  
  uint data;
  char temp[2048];
  
  std::ifstream readdata("MTSort",std::ios::binary);

  readdata.read(temp,sizeof(char)*512);
  
  for (int i = 0; i < 4096; ++i)
    for (int j = 0; j < 4096; ++j)
      {
	readdata.read((char*)&data,sizeof(uint));
	// std::cout<<data<<std::endl;
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
  h1->Print();

  return 0;
}

// 
// readMTSort.cc ends here
