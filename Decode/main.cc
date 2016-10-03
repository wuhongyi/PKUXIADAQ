// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 10月  2 19:01:31 2016 (+0800)
// Last-Updated: 日 10月  2 20:42:43 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 6
// URL: http://wuhongyi.github.io 

/************************************
 *  Decoder for XIA pixie-16
 *  1st released by JingLi/Lee
 *
 *  2016/1/16
 *
 *  lijinger02@126.com
 ************************************/

#include "r2root.hh"

#include "TString.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char *argv[])
{
  if(argc != 3)
    {
      std::cout<<"error(argc != 3)      ./"<<argv[0]<<" [FileName]  [RuNnumber]"<<std::endl;
      return 0;
    }

  TString rawfilepath = "/home/wuhongyi/data/";
  TString rootfilepath = "/home/wuhongyi/data/";
  TString filename(argv[1]);
  TString RunNumber(argv[2]);
  int  runnumber = RunNumber.Atoi();

  r2root *r2r=new r2root(rawfilepath,rootfilepath,filename,runnumber);
  r2r->Process();
  delete r2r;
  
  return 0;
}

// 
// main.cc ends here










