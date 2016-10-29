// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 二 7月 12 11:13:59 2016 (+0800)
// Last-Updated: 三 10月  5 19:56:18 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 16
// URL: http://wuhongyi.cn 

// 当前Online仅适用于采样率100MHz插件

#include "Online.hh"
#include "TGApplication.h"
#include "TSystem.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char **argv)
{
  TApplication theApp("App", &argc, argv);

  // check compiler
  if(sizeof(char) != 1) std::cout<<"sizeof(char) != 1 The current compiler is not suitable for running the program！"<<std::endl; 
  if(sizeof(short) != 2) std::cout<<"sizeof(short) != 2 The current compiler is not suitable for running the program！"<<std::endl; 
  if(sizeof(int) != 4) std::cout<<"sizeof(int) != 4 The current compiler is not suitable for running the program！"<<std::endl;
  
  Online mainWindow(gClient->GetRoot());

  theApp.Run();

  return 0;
}

// 
// main.cc ends here
