// RateThread.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 3月 12 14:30:22 2023 (+0800)
// Last-Updated: 日 3月 12 21:04:19 2023 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 3
// URL: http://wuhongyi.cn 

#ifndef _RATETHREAD_H_
#define _RATETHREAD_H_

#include "Global.hh"

#include <QVector>
#include <QFile>
#include <QTextStream>
//#include <QWaitCondition>
#include <QThread>
//#include <QMutex>
#include <QElapsedTimer>

#include <iostream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MainWindow;
class DeviceHandle;

class RateThread : public QThread
{
  Q_OBJECT
  
public:
  RateThread(MainWindow *w, DeviceHandle *dev);
  virtual ~RateThread();


protected:
  
  void run();


private:
  
  MainWindow *mMainWindow = nullptr;
  DeviceHandle *mDevice = nullptr;

  QElapsedTimer timershm;

  double RealTime[MAXMODULENUM];
  double LiveTime[MAXMODULENUM][MAXCHANNELNUM];
  double RawOutputCount[MAXMODULENUM][MAXCHANNELNUM];//real
  double RawInputCount[MAXMODULENUM][MAXCHANNELNUM];//live
};

#endif /* _RATETHREAD_H_ */
// 
// RateThread.hh ends here
