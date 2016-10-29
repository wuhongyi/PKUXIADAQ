// Global.cpp --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 二 8月  9 10:42:20 2016 (+0800)
// Last-Updated: 二 8月  9 10:44:34 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 2
// URL: http://wuhongyi.cn 

#include "Global.h"

#include <iostream>

void ErrorInfo(const char *filename,const char *funcname,const char *detial,int ErrorN)
{
  std::cout << "<<Error>> File-" << filename << "-" << funcname << " ErrorCode:"<< detial << " " << ErrorN << std::endl;
}

// 
// Global.cpp ends here
