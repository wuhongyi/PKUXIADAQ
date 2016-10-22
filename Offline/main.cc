// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 7月 22 16:09:53 2016 (+0800)
// Last-Updated: 六 10月 22 11:39:20 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 58
// URL: http://wuhongyi.github.io 

#include "algorithm.hh"

#include "TRint.h"
#include "TObject.h"
#include "TString.h"
#include "TTree.h"
#include "TChain.h"
#include "TSystem.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char *argv[])
{
  // Create an interactive ROOT application
  TRint *theApp = new TRint("Rint", &argc, argv);
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  //初始参数
  TString TreeName="tree";//这里为要处理的文件中 tree name
  // create first the chain with all the files
  TChain *fChain=new TChain(TreeName);
  // std::cout << "Creating the chain" << std::endl;
  fChain->SetCacheSize(20*1024*1024);

  if(argc != 2)
    {
      printf("run:  ./XXX rootfilename \n");
      exit(-1);
    }

  TString dir = gSystem->DirName(__FILE__);//获取当前文件main.cc所在路径 
  dir.ReplaceAll("/./","/");
  // std::cout<<dir<<std::endl;
  gSystem->Setenv("Dir","/home/wuhongyi/data");//手动填写路径
  // gSystem->Setenv("Dir",dir);//当前文件路径
  char rootfilename[128];
  sprintf(rootfilename,"$Dir/%s",argv[1]);
  fChain->Add(rootfilename);
  
  
  fChain->Print();

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  algorithm *xia;
  xia = new algorithm();
  xia->InitSystem(3,2);
  
  xia->SetTTree(fChain);
  char  *FileName = (char *)"20161007.set";
  if(xia->LoadDSPParametersFromFile(FileName ) == 0)
    
  if(xia->Init_DSPVarAddress((char *)"Pixie16DSP_revfgeneral_14b100m_r33354.var", 0) == 0)
    printf("zzzzzzzzz\n");
  xia->Init_DSPVarAddress((char *)"Pixie16DSP_revfgeneral_14b100m_r33354.var", 1);
  xia->Init_DSPVarAddress((char *)"Pixie16DSP_revfgeneral_14b100m_r33354.var", 2);
   
  xia->DrawEntry(500);

  double ENERGY_RISETIME;
  double ENERGY_FLATTOP;
  unsigned int SLOW_FILTER_RANGE;
  xia->ReadSglChanPar((char *)"ENERGY_RISETIME",&ENERGY_RISETIME,0,2);
  xia->ReadSglChanPar((char *)"ENERGY_FLATTOP",&ENERGY_FLATTOP,0,2);
  xia->ReadSglModPar((char *)"SLOW_FILTER_RANGE",&SLOW_FILTER_RANGE,0);
  std::cout<<SLOW_FILTER_RANGE<<"  "<<ENERGY_RISETIME<<"  "<<ENERGY_FLATTOP<<std::endl;

  SLOW_FILTER_RANGE = 1;
  xia->WriteSglModPar((char *)"SLOW_FILTER_RANGE",SLOW_FILTER_RANGE,0);
  xia->ReadSglChanPar((char *)"ENERGY_RISETIME",&ENERGY_RISETIME,0,2);
  xia->ReadSglChanPar((char *)"ENERGY_FLATTOP",&ENERGY_FLATTOP,0,2);
  xia->ReadSglModPar((char *)"SLOW_FILTER_RANGE",&SLOW_FILTER_RANGE,0);
  std::cout<<SLOW_FILTER_RANGE<<"  "<<ENERGY_RISETIME<<"  "<<ENERGY_FLATTOP<<std::endl;  
    
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // and enter the event loop...
  theApp->Run();
  delete theApp;
  
  return 0;
}


// 
// main.cc ends here









