// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 1月  5 13:46:14 2019 (+0800)
// Last-Updated: 六 1月  5 21:15:44 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 5
// URL: http://wuhongyi.cn 

#include "UserDefine.hh"
#include "merge.hh"
#include "TString.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char *argv[])
{
  if(argc < 3)
    {
      std::cout<<"error(argc < 3)      "<<argv[0]<<" [R0] [R1] ..."<<std::endl;
      return 0;
    }


  

  merge *medata = new merge(argc,argv); 
  medata->Process();
  delete medata;
  
  return 0;
}

// 
// main.cc ends here














