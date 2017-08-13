// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 6月  3 09:24:39 2017 (+0800)
// Last-Updated: 日 8月 13 18:08:53 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 9
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
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 以下部分用户需要修改
  TString rawfilepath = "/home/wuhongyi/data/";
  TString outfilepath = "/home/wuhongyi/data/";

  TString filename = "data";
  // 以上部分用户需要修改
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



  
  if(argc != 3)
    {
      std::cout<<"error(argc != 3)      "<<argv[0]<<"  [RunNnumber]  [windows]"<<std::endl;
      return 1;
    }
  std::cout<<"Tips: The software is only applicable to a crate!" <<std::endl;


  TString RunNumber(argv[1]);
  int  runnumber = RunNumber.Atoi();
  TString TimeWindows(argv[2]);
  int timewindows = TimeWindows.Atoi();

  if(BOARDNUMBER < 0 || BOARDNUMBER > 13)
    {
      std::cout<<"Error: Parameter 'BOARDNUMBER' in file sort.hh not in range 1 - 13 !!!"<<std::endl;
	return 1;
    }
  
  sort *tran = new sort(rawfilepath,outfilepath,filename,runnumber,timewindows);
  tran->Process();
  delete tran;
  
  return 0;
}

// 
// main.cc ends here
