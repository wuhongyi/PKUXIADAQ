// root2m4b.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 8月  8 16:05:35 2016 (+0800)
// Last-Updated: 一 8月  8 16:19:18 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 4
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

int root2m4b()
{
  TFile *file = new TFile("Ge75_i600.root","READ");//"RECREATE" "READ"

  TH2I *h = (TH2I*)file->Get("spectrum2");
  h->Print();

  ofstream writefile;//fstream
  writefile.open("ge75i600.m4b",std::ios::binary);//ios::bin ios::app

  double raw;
  int data;
   for (int i = 0; i < 4096; ++i)
    for (int j = 0; j < 4096; ++j)
      {
	raw = h->GetBinContent(i+1, j+1);
	data = (int) raw;
	writefile.write((char*)&data,sizeof(int));
      }
   
  writefile.close();
  
  // TObject->Write();
  file->Close();


  
  return 0;
}
// 
// root2m4b.cc ends here
