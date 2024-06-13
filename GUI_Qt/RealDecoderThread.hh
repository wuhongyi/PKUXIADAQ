// RealDecoderThread.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 3月 12 21:04:39 2023 (+0800)
// Last-Updated: 二 6月 11 22:47:58 2024 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 7
// URL: http://wuhongyi.cn 

#ifndef _REALDECODERTHREAD_H_
#define _REALDECODERTHREAD_H_

#include "Global.hh"
#include "decoding.hh"

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

class RealDecoderThread : public QThread
{
  Q_OBJECT
  
public:
  RealDecoderThread(MainWindow *w, DeviceHandle *dev);
  virtual ~RealDecoderThread();

protected:
  
  void run();


private:
  
  MainWindow *mMainWindow = nullptr;
  DeviceHandle *mDevice = nullptr;


  unsigned int buff[BUFFLENGTH];
  int buffid;
  int runno[MAXMODULENUM], runno_d[MAXMODULENUM];

  decoding *rawdec;


  bool firstbuffhit[MAXMODULENUM][MAXCHANNELNUM];
};

#endif /* _REALDECODERTHREAD_H_ */
// 
// RealDecoderThread.hh ends here
