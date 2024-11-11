
// HttpServer.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 5月 25 14:13:31 2024 (+0800)
// Last-Updated: 五 11月  8 20:42:35 2024 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 18
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
  void FillEnergy(unsigned short mod, unsigned short ch, unsigned short raw);
  void FillPSD(unsigned short mod, unsigned short ch, unsigned short raw, unsigned int s0, unsigned int s1, unsigned int s2);
  void FillRate(unsigned short mod, unsigned short ch, double ts);
  void FillCFD(unsigned short mod, unsigned short ch, unsigned short raw);
  void DrawWave(unsigned short mod, unsigned short ch, unsigned short len, unsigned int *waveform);

  
  void ClearADC(unsigned short mod);
  void ClearEnergy(unsigned short mod);
  void ClearRATE(unsigned short mod);
  void ClearPSD(unsigned short mod);
  void ClearCFD(unsigned short mod);
  
protected:
  HttpServer(DeviceHandle *dev);
  virtual ~HttpServer();


  

private:
  TH1I *adc[MAXMODULENUM][MAXCHANNELNUM];
  
  bool flag_energy[MAXMODULENUM][MAXCHANNELNUM];
  TH1I *energy[MAXMODULENUM][MAXCHANNELNUM];
  double energy_a0[MAXMODULENUM][MAXCHANNELNUM];
  double energy_a1[MAXMODULENUM][MAXCHANNELNUM];
  double energy_a2[MAXMODULENUM][MAXCHANNELNUM];
  int energy_bin[MAXMODULENUM][MAXCHANNELNUM];
  double energy_min[MAXMODULENUM][MAXCHANNELNUM];
  double energy_max[MAXMODULENUM][MAXCHANNELNUM];
  // mod ch a0 a1 a2 bin min max

  
  TH1I *rate[MAXMODULENUM][MAXCHANNELNUM];
  TH1I *cfd[MAXMODULENUM][MAXCHANNELNUM];
  
  bool flag_psd[MAXMODULENUM][MAXCHANNELNUM];
  TH2I *psd[MAXMODULENUM][MAXCHANNELNUM];
  double psd_a0[MAXMODULENUM][MAXCHANNELNUM];
  double psd_a1[MAXMODULENUM][MAXCHANNELNUM];
  double psd_a2[MAXMODULENUM][MAXCHANNELNUM];
  double psd_q0[MAXMODULENUM][MAXCHANNELNUM];
  double psd_q1[MAXMODULENUM][MAXCHANNELNUM];
  double psd_q2[MAXMODULENUM][MAXCHANNELNUM];
  int psd_binx[MAXMODULENUM][MAXCHANNELNUM];
  double psd_minx[MAXMODULENUM][MAXCHANNELNUM];
  double psd_maxx[MAXMODULENUM][MAXCHANNELNUM];
  int psd_biny[MAXMODULENUM][MAXCHANNELNUM];
  double psd_miny[MAXMODULENUM][MAXCHANNELNUM];
  double psd_maxy[MAXMODULENUM][MAXCHANNELNUM];
  // mod ch a0 a1 a2 q0 q1 q2 binx minx maxx biny miny maxy

  
  TGraph *wave[MAXMODULENUM][MAXCHANNELNUM];
  
  THttpServer* serv;
  DeviceHandle *mDevice;

  static HttpServer *wuHttpServer;
};

#endif /* _HTTPSERVER_H_ */
// 
// HttpServer.hh ends here
