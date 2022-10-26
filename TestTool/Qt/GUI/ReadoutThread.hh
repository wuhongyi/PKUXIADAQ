// ReadoutThread.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 三 6月 22 09:30:18 2022 (+0800)
// Last-Updated: 二 10月 25 22:02:22 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 9
// URL: http://wuhongyi.cn 

#ifndef _READOUTTHREAD_H_
#define _READOUTTHREAD_H_

#include <QVector>
#include <QFile>
#include <QTextStream>
	       //#include <QWaitCondition>
#include <QThread>
	       //#include <QMutex>

#include <iostream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MainWindow;
class DeviceHandle;

class ReadoutThread : public QThread
{
  Q_OBJECT
  
public:
  ReadoutThread(MainWindow *w, DeviceHandle *dev);
  virtual ~ReadoutThread();


  void run();
  void startRun();

signals:
	// void ReadoutError(QString, CAEN_FELib_ErrorCode);
	// void GenericError(QString);

		 
public slots:
  
  void stopRun();


private:
  
  MainWindow *mMainWindow = nullptr;
  DeviceHandle *mDevice = nullptr;
  
  bool mDoRead = false;
  bool mSaving = false;
  bool mRequestStop = false;

  QVector<QFile*> mSaveFile;
  QVector<QTextStream *> outFile;


private:

  void InitFiles();

  
};


#endif /* _READOUTTHREAD_H_ */
// 
// ReadoutThread.hh ends here
