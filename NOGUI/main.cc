// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 8月 15 16:53:40 2016 (+0800)
// Last-Updated: 一 4月 18 20:14:47 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 14
// URL: http://wuhongyi.cn 

#include "Manager.hh"
#include "Global.hh"
#include <iostream>
#include "pixie16app_defs.h"
#include "pixie16app_export.h"
#include "pixie16sys_export.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char *argv[])
{
  std::cout<<gVERSION<<std::endl;
  Manager *manager = new Manager();
  if(manager->BootSystem()) manager->RunManager();

  delete manager;
  return 0;
}

// 
// main.cc ends here
