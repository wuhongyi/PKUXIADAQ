// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 1月 14 19:44:51 2019 (+0800)
// Last-Updated: 四 5月  9 13:49:04 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 8
// URL: http://wuhongyi.cn 

#include "timesort.hh"

#include "TString.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char *argv[])
{
  if(argc != 2)
    {
      std::cout<<"error(argc != 2)      "<<argv[0]<<"  [RunNnumber]"<<std::endl;
      return 1;
    }

  TString RunNumber(argv[1]);
  int  runnumber = RunNumber.Atoi();
  
  timesort *ts = new timesort(runnumber);
  ts->Process();
  delete ts;

  return 0;
}

// 
// main.cc ends here
