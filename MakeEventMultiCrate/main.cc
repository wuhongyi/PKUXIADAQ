// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 6月  3 09:24:39 2017 (+0800)
// Last-Updated: 日 1月 13 20:59:05 2019 (+0800)
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

// 本转换程序的使用前提，插件必须从第一个插槽开始，中间不留空插槽。

int main(int argc, char *argv[])
{
  if(argc < 3)
    {
      std::cout<<"error(argc < 3)      "<<argv[0]<<" [R0] [R1] ..."<<std::endl;
      return 0;
    }
  
  sort *tran = new sort(argc,argv);
  tran->Process();
  delete tran;
  
  return 0;
}

// 
// main.cc ends here
