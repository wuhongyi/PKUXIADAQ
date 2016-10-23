// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 7月 22 16:09:53 2016 (+0800)
// Last-Updated: 日 10月 23 16:25:35 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 60
// URL: http://wuhongyi.github.io 

#include "MainFrame.hh"

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

  // check compiler
  if(sizeof(char) != 1) std::cout<<"sizeof(char) != 1 The current compiler is not suitable for running the program！"<<std::endl; 
  if(sizeof(short) != 2) std::cout<<"sizeof(short) != 2 The current compiler is not suitable for running the program！"<<std::endl; 
  if(sizeof(int) != 4) std::cout<<"sizeof(int) != 4 The current compiler is not suitable for running the program！"<<std::endl;
  
  
  MainFrame mainWindow(gClient->GetRoot());
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // and enter the event loop...
  theApp->Run();
  delete theApp;
  
  return 0;
}


// 
// main.cc ends here









