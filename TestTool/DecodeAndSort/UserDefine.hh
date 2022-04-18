// UserDefine.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 10月 20 16:16:28 2017 (+0800)
// Last-Updated: 六 11月  7 19:11:15 2020 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 26
// URL: http://wuhongyi.cn 

// The user needs to modify the parameters in this file.

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef _USERDEFINE_H_
#define _USERDEFINE_H_

#define Crate0
#define Crate1
// #define Crate2
// #define Crate3
// #define Crate4
// #define Crate5
// #define Crate6
// #define Crate7

#define ROOTFILEPATH "../"  //The path to generate the ROOT file
#define ROOTFILENAME "data"
#define TimesHist 3600  // second

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#define Crate0RAWFILEPATH "/home/wuhy/data/C0/"   //Path to the original binary
#define Crate0RAWFILENAME "data"                   //The file name of the original file
#define Crate0num 8   //Number of modules used in this crate
const int Crate0SamplingRate[Crate0num] = {100,100,100,100,100,100,100,500};//Specify the sampling rate of each modules separately; 100/250/500 sampling rates; 0 to skip the module


#define Crate1RAWFILEPATH "/home/wuhy/data/C1/"   //Path to the original binary
#define Crate1RAWFILENAME "data"                   //The file name of the original file
#define Crate1num 8   //Number of modules used in this crate
const int Crate1SamplingRate[Crate1num] = {250,250,250,250,250,250,250,250};//Specify the sampling rate of each modules separately; 100/250/500 sampling rates; 0 to skip the module


#define Crate2RAWFILEPATH "/home/wuhongyi/data/"   //Path to the original binary
#define Crate2RAWFILENAME "data"                   //The file name of the original file
#define Crate2num 6   //Number of modules used in this crate
const int Crate2SamplingRate[Crate2num] = {100,100,100,100,100,100};//Specify the sampling rate of each modules separately; 100/250/500 sampling rates; 0 to skip the module


#define Crate3RAWFILEPATH "/home/wuhongyi/data/"   //Path to the original binary
#define Crate3RAWFILENAME "data"                   //The file name of the original file
#define Crate3num 6   //Number of modules used in this crate
const int Crate3SamplingRate[Crate3num] = {100,100,100,100,100,100};//Specify the sampling rate of each modules separately; 100/250/500 sampling rates; 0 to skip the module


#define Crate4RAWFILEPATH "/home/wuhongyi/data/"   //Path to the original binary
#define Crate4RAWFILENAME "data"                   //The file name of the original file
#define Crate4num 6   //Number of modules used in this crate
const int Crate4SamplingRate[Crate4num] = {100,100,100,100,100,100};//Specify the sampling rate of each modules separately; 100/250/500 sampling rates; 0 to skip the module


#define Crate5RAWFILEPATH "/home/wuhongyi/data/"   //Path to the original binary
#define Crate5RAWFILENAME "data"                   //The file name of the original file
#define Crate5num 6   //Number of modules used in this crate
const int Crate5SamplingRate[Crate5num] = {100,100,100,100,100,100};//Specify the sampling rate of each modules separately; 100/250/500 sampling rates; 0 to skip the module


#define Crate6RAWFILEPATH "/home/wuhongyi/data/"   //Path to the original binary
#define Crate6RAWFILENAME "data"                   //The file name of the original file
#define Crate6num 6   //Number of modules used in this crate
const int Crate6SamplingRate[Crate6num] = {100,100,100,100,100,100};//Specify the sampling rate of each modules separately; 100/250/500 sampling rates; 0 to skip the module


#define Crate7RAWFILEPATH "/home/wuhongyi/data/"   //Path to the original binary
#define Crate7RAWFILENAME "data"                   //The file name of the original file
#define Crate7num 6   //Number of modules used in this crate
const int Crate7SamplingRate[Crate7num] = {100,100,100,100,100,100};//Specify the sampling rate of each modules separately; 100/250/500 sampling rates; 0 to skip the module


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Example
// For example, there are 5 100MHz modules

// #define Crate0num 5
// const int Crate0SamplingRate[Crate0num] = {100,100,100,100,100};


#endif /* _USERDEFINE_H_ */
// 
// UserDefine.hh ends here
