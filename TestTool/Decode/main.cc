// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 10月  2 19:01:31 2016 (+0800)
// Last-Updated: 四 1月 31 19:40:24 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 11
// URL: http://wuhongyi.cn 

/************************************
 *  Decoder for XIA pixie-16
 *  1st released by JingLi/Lee
 *
 *  2016/1/16
 *
 *  lijinger02@126.com
 ************************************/

#include "UserDefine.hh"
#include "r2root.hh"

#include "TString.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char *argv[])
{
  if(argc != 2)
    {
      std::cout<<"error(argc != 2)      "<<argv[0]<<" [RunNumber]"<<std::endl;
      return 0;
    }

  TString rawfilepath = RAWFILEPATH;
  TString rootfilepath = ROOTFILEPATH;
  TString filename = RAWFILENAME;
  TString RunNumber(argv[1]);
  int  runnumber = RunNumber.Atoi();

  r2root *r2r=new r2root(rawfilepath,rootfilepath,filename,runnumber);
  r2r->Process();
  delete r2r;
  
  return 0;
}

// 
// main.cc ends here
