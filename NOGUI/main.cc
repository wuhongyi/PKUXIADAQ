// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 8月 15 16:53:40 2016 (+0800)
// Last-Updated: 四 8月 29 20:01:59 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 13
// URL: http://wuhongyi.cn 

#include "Manager.hh"

#include <iostream>
#include "pixie16app_defs.h"
#include "pixie16app_export.h"
#include "pixie16sys_export.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char *argv[])
{
  Manager *manager = new Manager();
  if(manager->BootSystem()) manager->RunManager();

  delete manager;
  return 0;
}

// 
// main.cc ends here
