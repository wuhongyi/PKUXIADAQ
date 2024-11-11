// RateThread.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 3月 12 14:30:22 2023 (+0800)
// Last-Updated: 日 11月  3 21:33:33 2024 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 13
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

  int GetCountRateUpper(unsigned short mod, unsigned short ch);
  int GetCountRateLower(unsigned short mod, unsigned short ch);

  void SetCountRateUpper(unsigned short mod, unsigned short ch, int value);
  void SetCountRateLower(unsigned short mod, unsigned short ch, int value);

  void SaveCountRateLimit();
  
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


  int CountRateUpper[MAXMODULENUM][MAXCHANNELNUM];//upper limit
  int CountRateLower[MAXMODULENUM][MAXCHANNELNUM];//lower limit
  QString CountRateFile;
  
#ifdef INBEAMGAMMA
  bool FlagGeDet[MAXMODULENUM][MAXCHANNELNUM];
  int CountLimitGe[MAXMODULENUM][MAXCHANNELNUM];
  unsigned int CountM2;
  unsigned int CountM2Time;//s  +3

  QString InBeamGammaGeFile;
#endif  
};

#endif /* _RATETHREAD_H_ */
// 
// RateThread.hh ends here
