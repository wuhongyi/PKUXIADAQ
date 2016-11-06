// Global.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 二 8月  9 10:42:20 2016 (+0800)
// Last-Updated: 日 11月  6 20:09:34 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 3
// URL: http://wuhongyi.cn 

#include "Global.hh"

#include <iostream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ErrorInfo(const char *filename,const char *funcname,const char *detial,int ErrorN)
{
  std::cout << "<<Error>> File-" << filename << "-" << funcname << " ErrorCode:"<< detial << " " << ErrorN << std::endl;
}

// 
// Global.cc ends here
