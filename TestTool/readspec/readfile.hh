// readfile.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 1月 31 17:34:05 2019 (+0800)
// Last-Updated: 四 1月 31 19:47:44 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 6
// URL: http://wuhongyi.cn 

#ifndef _READFILE_H_
#define _READFILE_H_

#include "TFile.h"
#include "TH1.h"
#include "TString.h"
#include "TROOT.h"
#include "TTree.h"
#include "TBenchmark.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#define MAXBOARD 13

class readfile
{
public:
  readfile(int runnumber);
  virtual ~readfile();

  void Process();


private:
  void write2root();
  void write2mca();
  bool IsFileExists(const char *name);
  bool CreateDirectory(const char *path);
private:
  TH1I *eHist[MAXBOARD][16];
  TBenchmark *benchmark;

  int flagfile;
  int Run;
  TString FileName;
  TString RawFilePath;
};

#endif /* _READFILE_H_ */
// 
// readfile.hh ends here






