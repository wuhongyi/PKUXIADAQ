// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 9月 21 20:06:21 2020 (+0800)
// Last-Updated: 六 2月 17 14:16:47 2024 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 4
// URL: http://wuhongyi.cn 

#include "event.hh"
#include "TString.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const char gVERSION[] = "Version: EventBuilder - 2024.02.17";

int main(int argc, char *argv[])
{
  std::cout<<gVERSION<<std::endl;
  std::cout<<"Copyright @ Hongyi Wu (wuhongyi@qq.com/wuhongyi@pku.edu.cn)"<<std::endl;
  if(argc != 2)
    {
      std::cout<<"error(argc != 2)      "<<argv[0]<<"  [RunNnumber]"<<std::endl;
      return 1;
    }

  TString RunNumber(argv[1]);
  int  runnumber = RunNumber.Atoi();

  event *tran = new event(runnumber);
  tran->Process();
  delete tran;

  return 0;
}

// 
// main.cc ends here
