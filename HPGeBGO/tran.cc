// tran.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 12月  6 22:48:21 2018 (+0800)
// Last-Updated: 六 12月  8 08:59:47 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 17
// URL: http://wuhongyi.cn 

#include "wuReadData.hh"
#include "decoder.hh"
#include "TString.h"
#include "TRandom.h"
#include "TH1.h"
#include "TFile.h"
#include <iostream>
#include <fstream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char *argv[])
{
  int HPGeModule = wuReadData::ReadValue<int>("HPGeModule", "Pars.txt");
  int HPGeChannel = wuReadData::ReadValue<int>("HPGeChannel", "Pars.txt");
  int BGOModule = wuReadData::ReadValue<int>("BGOModule", "Pars.txt");  
  int BGOChannel = wuReadData::ReadValue<int>("BGOChannel", "Pars.txt");

  double Cali_k = wuReadData::ReadValue<double>("Cali_k", "Pars.txt");
  double Cali_b = wuReadData::ReadValue<double>("Cali_b", "Pars.txt");
  
  std::vector<int> *BGOThreshold = NULL;
  BGOThreshold = new std::vector<int>;
  BGOThreshold->clear();
  wuReadData::ReadVector("BGOThreshold", "Pars.txt", BGOThreshold);


  decoder *rawdec = NULL;
  bool  havedata;
  UShort_t evte;//energy

  TH1D *h[20];

  TFile *file = new TFile(TString::Format("HPGe_%02d_%02d_BGO_%02d_%02d.root",HPGeModule,HPGeChannel,BGOModule,BGOChannel).Data(),"RECREATE");//"RECREATE" "READ"
  if(!file->IsOpen())
    {
      std::cout<<"Can't open root file"<<std::endl;
    }

  std::ofstream writetxt;
  writetxt.open(TString::Format("HPGe_%02d_%02d_BGO_%02d_%02d.dat",HPGeModule,HPGeChannel,BGOModule,BGOChannel).Data());  

  for (unsigned int iii = 0; iii < BGOThreshold->size(); ++iii)
    {
      h[iii] = new TH1D(TString::Format("h%04d",BGOThreshold->at(iii)).Data(),"",2048,0,2048); 
      if(rawdec != NULL) delete rawdec;
      rawdec = new decoder;
      rawdec->openfile(TString::Format("data/HPGe_%02d_%02d_BGO_%02d_%02d_%04d.bin",HPGeModule,HPGeChannel,BGOModule,BGOChannel,BGOThreshold->at(iii)).Data());
      rawdec->setsamplerate(100);
      
      havedata = rawdec->getnextevt();
      while(true)
	{
	  if(havedata)
	    {
	      evte = rawdec->getevte();
	      // std::cout<<iii<<"  "<<evte<<std::endl;
	      double energy = Cali_k*(evte+gRandom->Rndm())+Cali_b;
	      if(rawdec->getch() == HPGeChannel && energy > 0 && energy < 2048)
		h[iii]->Fill(energy);
	    }
	  havedata = rawdec->getnextevt(); 
	  if(!havedata) break;
	}

      file->cd();
      h[iii]->Write();

      double counttotal = h[iii]->Integral(100,2048);
      double count1173 = h[iii]->Integral(1168,1178);
      double count1332 = h[iii]->Integral(1327,1337);
      double count1040_1096 = h[iii]->Integral(1040,1096);
      
      writetxt<<BGOThreshold->at(iii)<<" "<<(count1173+count1332)/counttotal*100.0<<" "<<count1332/count1040_1096<<std::endl;
    }
  
  file->Close();
  writetxt.close();
  return 0;
}

// 
// tran.cc ends here









