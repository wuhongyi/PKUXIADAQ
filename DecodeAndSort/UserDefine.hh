// UserDefine.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 10月 20 16:16:28 2017 (+0800)
// Last-Updated: 五 3月 13 17:22:39 2020 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 22
// URL: http://wuhongyi.cn 

// The user needs to modify the parameters in this file.

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef _USERDEFINE_H_
#define _USERDEFINE_H_

#define RAWFILEPATH "/home/wuhongyi/data/"   //Path to the original binary
#define RAWFILENAME "data"                   //The file name of the original file
#define ROOTFILEPATH "./"  //The path to generate the ROOT file


#define TimesHist 3600  // second

#define Crate0
#define Crate0num 6   //Number of modules used in this crate
const int Crate0SamplingRate[Crate0num] = {100,100,100,100,100,100};//Specify the sampling rate of each modules separately; 100/250/500 sampling rates; 0 to skip the module

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Example
// For example, there are 5 100MHz modules

// #define Crate0num 5
// const int Crate0SamplingRate[Crate0num] = {100,100,100,100,100};


#endif /* _USERDEFINE_H_ */
// 
// UserDefine.hh ends here
