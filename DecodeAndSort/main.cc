// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 10月  2 19:01:31 2016 (+0800)
// Last-Updated: 三 9月 16 22:38:08 2020 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 14
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
  int craten = 0;
  int runnumber[8];
  
#ifdef Crate0
  TString RunNumber0(argv[1]);
  runnumber[craten] = RunNumber0.Atoi();
  craten++;
#endif
#ifdef Crate1
  TString RunNumber1(argv[2]);
  runnumber[craten] = RunNumber1.Atoi();
  craten++;
#endif
#ifdef Crate2
  TString RunNumber2(argv[3]);
  runnumber[craten] = RunNumber2.Atoi();
  craten++;
#endif
#ifdef Crate3
  TString RunNumber3(argv[4]);
  runnumber[craten] = RunNumber3.Atoi();
  craten++;
#endif
#ifdef Crate4
  TString RunNumber4(argv[5]);
  runnumber[craten] = RunNumber4.Atoi();
  craten++;
#endif
#ifdef Crate5
  TString RunNumber5(argv[6]);
  runnumber[craten] = RunNumber5.Atoi();
  craten++;
#endif
#ifdef Crate6
  TString RunNumber6(argv[7]);
  runnumber[craten] = RunNumber6.Atoi();
  craten++;
#endif
#ifdef Crate7
  TString RunNumber7(argv[8]);
  runnumber[craten] = RunNumber7.Atoi();
  craten++;
#endif

  if(argc != craten+1)
    {
      std::cout<<"error(argc-1 != NumberCrateRun)      "<<argv[0]<<" [Crate0RunNumber]  [Crate1RunNumber]  ..."<<std::endl;
      return 0;
    }

  r2root *r2r = new r2root(runnumber);
  r2r->Process();
  delete r2r;
  
  return 0;
}

// 
// main.cc ends here
