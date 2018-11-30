// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 6月  3 09:24:39 2017 (+0800)
// Last-Updated: 日 6月  3 05:43:16 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 11
// URL: http://wuhongyi.cn 

#include "UserDefine.hh"
#include "sort.hh"

#include "TString.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char *argv[])
{
  TString rawfilepath = RAWFILEPATH;
  TString outfilepath = OUTFILEPATH;
  TString filename = RAWFILENAME;

  
  if(argc != 2)
    {
      std::cout<<"error(argc != 2)      "<<argv[0]<<"  [RunNnumber] "<<std::endl;
      return 1;
    }

  TString RunNumber(argv[1]);
  int  runnumber = RunNumber.Atoi();

  sort *tran = new sort(rawfilepath,outfilepath,filename,runnumber);
  tran->Process();
  delete tran;
  
  return 0;
}

// 
// main.cc ends here
