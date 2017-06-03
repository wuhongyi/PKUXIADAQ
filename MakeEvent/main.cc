// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 6月  3 09:24:39 2017 (+0800)
// Last-Updated: 六 6月  3 12:34:39 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 6
// URL: http://wuhongyi.cn 

#include "sort.hh"

#include "TString.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// 本转换程序的使用前提，插件必须从第一个插槽开始，中间不留空插槽。

int main(int argc, char *argv[])
{
  if(argc != 4)
    {
      std::cout<<"error(argc != 4)      "<<argv[0]<<"  [ModuleNumber]  [RuNnumber]  [windows]"<<std::endl;
      return 1;
    }
  std::cout<<"Tips: The software is only applicable to a crate!" <<std::endl;

  
  TString rawfilepath = "/home/wuhongyi/data/";
  TString outfilepath = "/home/wuhongyi/data/";
  TString filename = "data";
  TString ModuleNumber(argv[1]);
  int modulenumber = ModuleNumber.Atoi();
  TString RunNumber(argv[2]);
  int  runnumber = RunNumber.Atoi();
  TString TimeWindows(argv[3]);
  int timewindows = TimeWindows.Atoi();

  if(modulenumber < 0 || modulenumber > 13)
    {
      std::cout<<"Error: Input parameter ModuleNumber not in range 1 - 13 !!!"<<std::endl;
	return 1;
    }
  
  sort *tran = new sort(rawfilepath,outfilepath,filename,runnumber,timewindows,modulenumber);
  tran->Process();
  delete tran;
  
  return 0;
}

// 
// main.cc ends here
