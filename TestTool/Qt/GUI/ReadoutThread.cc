// ReadoutThread.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 三 6月 22 09:31:05 2022 (+0800)
// Last-Updated: 三 10月 26 22:17:09 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 10
// URL: http://wuhongyi.cn 

#include "ReadoutThread.hh"
#include "MainWindow.hh"
#include "DeviceHandle.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ReadoutThread::ReadoutThread(MainWindow *w, DeviceHandle *dev)
{
  mDevice = dev;
  mMainWindow = w;

  mSaveFile.resize(mDevice->GetNumberOfDevice());
  outFile.resize(mDevice->GetNumberOfDevice());

  // connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
  
  // connect(this, SIGNAL(ReadoutError(QString, CAEN_FELib_ErrorCode)), mWaveDump2, SLOT(ReadoutError(QString, CAEN_FELib_ErrorCode)));
  // connect(this, SIGNAL(GenericError(QString)), mWaveDump2, SLOT(ReadoutError1(QString)));

  std::cout << "init ReadoutThread" << std::endl;
}

ReadoutThread::~ReadoutThread()
{

  std::cout << "delete ReadoutThread" << std::endl;
}


void ReadoutThread::startRun()
{
  std::cout << "start run" << std::endl;
  // mDoRead = true;
  mRequestStop = false;
}


void ReadoutThread::stopRun()
{
  mRequestStop = true;

  std::cout << "stop run" << std::endl;
}


void ReadoutThread::run()
{
  std::cout << "start a new run..." << std::endl;
  InitFiles();

  mDevice->StartAcquisition();

  
  while(!mRequestStop)
    {
      std::cout << "loop" << std::endl;

      for (int i = 0; i < mDevice->GetNumberOfDevice(); ++i)
	{
	  char data[16384] = {'1', '2', '3'};
	  mSaveFile[i]->write((const char*)data, sizeof(*data)*1000);

	  mSaveFile[i]->flush();
	}

      
      msleep(1000); //线程休眠100ms
    }


  

  mDevice->StopAcquisition();

  // int counter=0;
  // while(RunStatus())
  //   {
  //   ReadDataFromModules(0);
  //   counter++;
  //   if(counter > 10*NumModules) break;
  //   sleep(1); // wait 1s then try again  // add 20180504
  // }
  // if(counter >= 10*NumModules)
  //   {
  //   cout<<" ERROR! Some modules may not End Run correctly!"<<endl;
  // }

  // Make sure all data has been read out
  // ReadDataFromModules(0,1); // end of run  
  
  for (int i = 0; i < mDevice->GetNumberOfDevice(); ++i)
    {
      if (mSaveFile[i] != nullptr)
	{
	  mSaveFile[i]->close();
	  delete mSaveFile[i];
	  mSaveFile[i] = nullptr;
	  delete outFile[i];
	  outFile[i] = nullptr;
	}
    }
}


void ReadoutThread::InitFiles()
{

  for (int i = 0; i < mDevice->GetNumberOfDevice(); ++i)
    {
      mSaveFile[i] = new QFile(QString::number(i)+".bin");
      outFile[i] = new QTextStream(mSaveFile[i]);

      if (!mSaveFile[i]->open(QIODevice::WriteOnly))
	{
	  // emit GenericError("Unable to create file: " + this->mSaveFile[i]->errorString());
	  return;
	}
      
    }

}


// 
// ReadoutThread.cc ends here
