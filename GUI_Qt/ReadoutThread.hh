// ReadoutThread.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 三 6月 22 09:30:18 2022 (+0800)
// Last-Updated: 六 10月 26 23:13:09 2024 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 31
// URL: http://wuhongyi.cn 

#ifndef _READOUTTHREAD_H_
#define _READOUTTHREAD_H_

#include "Global.hh"

#include <QVector>
#include <QFile>
#include <QTextStream>
//#include <QWaitCondition>
#include <QThread>
//#include <QMutex>
#include <QElapsedTimer>

// #include <fcntl.h>
// #include <unistd.h>
// #include <sys/mman.h>
// #include <sys/stat.h>
// #include <semaphore.h>

#include <iostream>
#include <fstream>

#ifdef RECODESHA256
#include <openssl/sha.h>
#endif
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MainWindow;
class DeviceHandle;

class ReadoutThread : public QThread
{
  Q_OBJECT
  
public:
  ReadoutThread(MainWindow *w, DeviceHandle *dev);
  virtual ~ReadoutThread();

  bool IsRunning() {return mRunning;}

  void startRun();


// #ifdef RECODESHA256
//   void GetSHA256(int mod, unsigned char *sha) {memcpy(sha, SHA256result[mod], 32*sizeof(unsigned char));}
// #endif
  
signals:
	// void ReadoutError(QString, CAEN_FELib_ErrorCode);
	// void GenericError(QString);

		 
public slots:
  
  void stopRun();

protected:
  
  void run();
  
private:
  
  MainWindow *mMainWindow = nullptr;
  DeviceHandle *mDevice = nullptr;
  
  bool mDoRead = false;
  bool mSaving = false;
  bool mResetClock = false;
  bool mRequestStop = false;

  bool mRunning = false;
  
  QVector<QFile*> mSaveFile;
  QVector<QTextStream *> outFile;
  int runno;

  unsigned int buff[MAXMODULENUM][BUFFLENGTH];
  int buffid[MAXMODULENUM];
  unsigned long filesize[MAXMODULENUM];

  unsigned int buffevent[BUFFLENGTH];
  int buffeventid;

  QString folderfile;

  // variables for shared memory
  // sem_t *shmsem; // shared memory semaphore
  // int    shmfd;  // shared memory id
  // unsigned char *shmptr;// pointer to shm
  // unsigned int shmid1, shmid2;

#ifdef DECODERONLINE
  int    shmfd_dec;  // shared memory id
  unsigned char *shmptr_dec;// pointer to shm
  // runnumber 4 byte
  // bufflegth 4 byte * 13
  // temp      4 byte
  // buff * 14
  
  void InitDecoderOnline();
  bool   fdecoder;
#endif

#ifdef RECODESHA256
  SHA256_CTX sha256_ctx[MAXMODULENUM];
  unsigned char SHA256result[MAXMODULENUM][32];
#endif

  QElapsedTimer timerrun;
  int runtime;
  
private:

  void InitFiles();
  void CloseFile();
  void ReadDataFromModules(unsigned int thres, bool endofrun);

  void FlushBuffer(unsigned short mod);

  // void OpenSharedMemory();


  // void UpdateSharedMemory();
  // void UpdateEnergySpectrumForModule();
  // void UpdateFilePathAndNameInSharedMemory(const char *path,const char *filen);
  
};


#endif /* _READOUTTHREAD_H_ */
// 
// ReadoutThread.hh ends here
