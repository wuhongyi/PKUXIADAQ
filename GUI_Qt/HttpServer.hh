
// HttpServer.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 5月 25 14:13:31 2024 (+0800)
// Last-Updated: 一 10月 14 20:49:19 2024 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 11
// URL: http://wuhongyi.cn 

#ifndef _HTTPSERVER_H_
#define _HTTPSERVER_H_

#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "THttpServer.h"
#include "TSystem.h"
#include "TString.h"

#include "Global.hh"
#include "DeviceHandle.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HttpServer
{
public:
  static HttpServer* Instance(DeviceHandle *dev);
  
  void FillADC(unsigned short mod, unsigned short ch, unsigned short raw);
  void FillPSD(unsigned short mod, unsigned short ch, unsigned short raw, double p);
  void FillRate(unsigned short mod, unsigned short ch, double ts);
  void FillCFD(unsigned short mod, unsigned short ch, unsigned short raw);
  void DrawWave(unsigned short mod, unsigned short ch, unsigned short len, unsigned int *waveform);

  
  void ClearADC(unsigned short mod);
  void ClearRATE(unsigned short mod);
  void ClearPSD(unsigned short mod);
  void ClearCFD(unsigned short mod);
  
protected:
  HttpServer(DeviceHandle *dev);
  virtual ~HttpServer();


  

private:
  TH1I *adc[MAXMODULENUM][MAXCHANNELNUM];
  TH1I *rate[MAXMODULENUM][MAXCHANNELNUM];
  TH1I *cfd[MAXMODULENUM][MAXCHANNELNUM];
  TH2I *psd[MAXMODULENUM][MAXCHANNELNUM];
  TGraph *wave[MAXMODULENUM][MAXCHANNELNUM];
  
  THttpServer* serv;
  DeviceHandle *mDevice;

  static HttpServer *wuHttpServer;
};

#endif /* _HTTPSERVER_H_ */
// 
// HttpServer.hh ends here
