// ReadoutThread.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 三 6月 22 09:31:05 2022 (+0800)
// Last-Updated: 三 6月 22 11:00:36 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 3
// URL: http://wuhongyi.cn 

#include "ReadoutThread.hh"
#include "MainWindow.hh"
#include "DeviceHandle.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ReadoutThread::ReadoutThread(MainWindow *w, DeviceHandle *dev)
{
  mDevice = dev;
  mMainWindow = w;

  
}

ReadoutThread::~ReadoutThread()
{


}


void ReadoutThread::startRun()
{

  // mDoRead = true;
  // mRequestStop = false;
}


void ReadoutThread::stopRun()
{

  // mRequestStop = true;
}


void ReadoutThread::run()
{


}


// 
// ReadoutThread.cc ends here
