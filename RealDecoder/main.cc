// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 三 2月 26 20:42:35 2020 (+0800)
// Last-Updated: 六 8月 19 11:09:57 2023 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 58
// URL: http://wuhongyi.cn 

// wugroot main.cc  -lrt  -o 123

#include "UserDefine.hh"
#include "decoding.hh"

#include "TSystem.h"
#include "THttpServer.h"
#include "TH1.h"
#include "TH2.h"
#include "TString.h"
#include "TRandom.h"

#include <fcntl.h>
#include <semaphore.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <cstdlib>
#include <cmath>
#include <sstream>
#include <errno.h>
#include <unistd.h>
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const char gVERSION[] = "Version: RealDecoder - 2023.08.19";

int main(int argc, char *argv[])
{
  std::cout<<gVERSION<<std::endl;
  std::cout<<"Copyright @ Hongyi Wu (wuhongyi@qq.com/wuhongyi@pku.edu.cn)"<<std::endl;
  
  int goodch[MODULE_NUM_MAX][16];
  int histbin[MODULE_NUM_MAX][16];
  double histmin[MODULE_NUM_MAX][16];
  double histmax[MODULE_NUM_MAX][16];
  double  calia0[MODULE_NUM_MAX][16];
  double  calia1[MODULE_NUM_MAX][16];
  double  calia2[MODULE_NUM_MAX][16];

  int psdgoodch[MODULE_NUM_MAX][16];
  int psdxhistbin[MODULE_NUM_MAX][16];
  double psdxhistmin[MODULE_NUM_MAX][16];
  double psdxhistmax[MODULE_NUM_MAX][16];
  int psdyhistbin[MODULE_NUM_MAX][16];
  double psdyhistmin[MODULE_NUM_MAX][16];
  double psdyhistmax[MODULE_NUM_MAX][16];
  double psdp0[MODULE_NUM_MAX][16];
  double psdp1[MODULE_NUM_MAX][16];

  
  for (int i = 0; i < MODULE_NUM_MAX; ++i)
    for (int j = 0; j < 16; ++j)
      {
	goodch[i][j] = 0;
	histbin[i][j] = 1000;
	histmin[i][j] = 0;
	histmax[i][j] = 10000;
	calia0[i][j] = 0.0;
	calia1[i][j] = 0.0;
	calia2[i][j] = 0.0;

	psdgoodch[i][j] = 0;
	psdxhistbin[i][j] = 1000;
	psdxhistmin[i][j] = 0;
	psdxhistmax[i][j] = 60000;
	psdyhistbin[i][j] = 1000;
	psdyhistmin[i][j] = 0;
	psdyhistmax[i][j] = 1;
	psdp0[i][j] = 0.0;
	psdp1[i][j] = 0.0;
      }
  
  std::ifstream readtxt;
  readtxt.open("par.dat");
  if(!readtxt.is_open())
    {
      std::cout<<"can't open file."<<std::endl;
    }

  std::string str_tmp;
  getline(readtxt,str_tmp);

  Short_t sid_tmp, ch_tmp;
  int goodch_tmp;
  int histbin_tmp;
  double histmin_tmp, histmax_tmp;
  Double_t a0_tmp, a1_tmp, a2_tmp;
  while(!readtxt.eof())
    {
      readtxt>>sid_tmp>>ch_tmp>>goodch_tmp>>histbin_tmp>>histmin_tmp>>histmax_tmp>>a0_tmp>>a1_tmp>>a2_tmp;
      if(readtxt.eof()) break;
      goodch[sid_tmp-2][ch_tmp] = goodch_tmp;
      histbin[sid_tmp-2][ch_tmp] = histbin_tmp;
      histmin[sid_tmp-2][ch_tmp] = histmin_tmp;
      histmax[sid_tmp-2][ch_tmp] = histmax_tmp;
      calia0[sid_tmp-2][ch_tmp] = a0_tmp;
      calia1[sid_tmp-2][ch_tmp] = a1_tmp;
      calia2[sid_tmp-2][ch_tmp] = a2_tmp;
    }
  readtxt.close();  


  Short_t psdsid_tmp, psdch_tmp;
  int psdgoodch_tmp;
  int psdxhistbin_tmp;
  double psdxhistmin_tmp, psdxhistmax_tmp;
  int psdyhistbin_tmp;
  double psdyhistmin_tmp, psdyhistmax_tmp;
  Double_t psdp0_tmp, psdp1_tmp;
  readtxt.open("psd.dat");
  if(!readtxt.is_open())
    {
      std::cout<<"can't open file."<<std::endl;
    }
  getline(readtxt,str_tmp);
  while(!readtxt.eof())
    {
      readtxt>>psdsid_tmp>>psdch_tmp>>psdgoodch_tmp>>psdxhistbin_tmp>>psdxhistmin_tmp>>psdxhistmax_tmp>>psdyhistbin_tmp>>psdyhistmin_tmp>>psdyhistmax_tmp>>psdp0_tmp>>psdp1_tmp;
      if(readtxt.eof()) break;
      psdgoodch[psdsid_tmp-2][psdch_tmp] = psdgoodch_tmp;
      psdxhistbin[psdsid_tmp-2][psdch_tmp] = psdxhistbin_tmp;
      psdxhistmin[psdsid_tmp-2][psdch_tmp] = psdxhistmin_tmp;
      psdxhistmax[psdsid_tmp-2][psdch_tmp] = psdxhistmax_tmp;
      psdyhistbin[psdsid_tmp-2][psdch_tmp] = psdyhistbin_tmp;
      psdyhistmin[psdsid_tmp-2][psdch_tmp] = psdyhistmin_tmp;
      psdyhistmax[psdsid_tmp-2][psdch_tmp] = psdyhistmax_tmp;
      psdp0[psdsid_tmp-2][psdch_tmp] = psdp0_tmp;
      psdp1[psdsid_tmp-2][psdch_tmp] = psdp1_tmp;
    }
  readtxt.close();  

  
  TH1I *adc[MODULE_NUM_MAX][16];
  TH1I *energy[MODULE_NUM_MAX][16];
  TH1I *rate[MODULE_NUM_MAX][16];

  TH2I *psd[MODULE_NUM_MAX][16];
  
  for (int i = 0; i < MODULE_NUM_MAX; ++i)
    for (int j = 0; j < 16; ++j)
      {
	adc[i][j] = new TH1I(TString::Format("ADC_%02d_%02d",i,j).Data(),"",ADC_HIST_BIN,ADC_HIST_MIN,ADC_HIST_MAX);
	adc[i][j]->SetDirectory(0);
	energy[i][j] = new TH1I(TString::Format("ENERGY_%02d_%02d",i,j).Data(),"",histbin[i][j],histmin[i][j],histmax[i][j]);
	energy[i][j]->SetDirectory(0);
	
#ifdef CLEAR_IN_NEW_RUN
#ifdef SHOW_OUTPUT_RATE
	rate[i][j] = new TH1I(TString::Format("RATE_%02d_%02d",i,j).Data(),"",RATE_HIST_BIN,RATE_HIST_MIN,RATE_HIST_MAX);
	rate[i][j]->SetDirectory(0);	
#endif
#endif

	if(psdgoodch[i][j] == 1)
	  {
	    psd[i][j] = new TH2I(TString::Format("PSD_%02d_%02d",i,j).Data(),"",psdxhistbin[i][j],psdxhistmin[i][j],psdxhistmax[i][j],psdyhistbin[i][j],psdyhistmin[i][j],psdyhistmax[i][j]);
	    psd[i][j]->SetDirectory(0);
	  }
      }

  
  if (gSystem->AccessPathName("auth.txt") != 0)
    {
      std::cout<<"Please start macro from directory where auth.txt file is available\n"<<std::endl;
      std::cout<<"It required to supply authentication information for the http server\n"<<std::endl;
      return 0;
    }

  THttpServer* serv = new THttpServer("http:8080?top=GDDAQ Online&auth_file=auth.txt&auth_domain=root");
  // serv->SetJSROOT("https://root.cern.ch/js/latest/");

  for (int i = 0; i < MODULE_NUM_MAX; ++i)
    for (int j = 0; j < 16; ++j)
      {
	if(goodch[i][j]==1)
	  {
	    serv->Register("ADC",adc[i][j]);
	    serv->Register("ENERGY",energy[i][j]);
#ifdef CLEAR_IN_NEW_RUN
#ifdef SHOW_OUTPUT_RATE
	    serv->Register("RATE",rate[i][j]);
#endif
#endif	    
	  }

	if(psdgoodch[i][j] == 1)
	  {
	    serv->Register("PSD", psd[i][j]);
	  }
      }

  
  for (int i = 0; i < MODULE_NUM_MAX; ++i)
    for (int j = 0; j < 16; ++j)
      {
	if(goodch[i][j]==1)
	  {
	    serv->RegisterCommand(TString::Format("/Control/ADC/ADC_%02d_%02d",i,j).Data(),TString::Format("/Objects/ADC/ADC_%02d_%02d/->Reset()",i,j).Data(), "rootsys/icons/ed_delete.png");
	    serv->SetItemField(TString::Format("/Control/ADC/ADC_%02d_%02d",i,j).Data(),"_update_item", TString::Format("ADC_%02d_%02d",i,j).Data()); // let browser update histogram view after commands execution

	    serv->RegisterCommand(TString::Format("/Control/ENERGY/ENERGY_%02d_%02d",i,j).Data(),TString::Format("/Objects/ENERGY/ENERGY_%02d_%02d/->Reset()",i,j).Data(), "rootsys/icons/ed_delete.png");
	    serv->SetItemField(TString::Format("/Control/ENERGY/ENERGY_%02d_%02d",i,j).Data(),"_update_item", TString::Format("ENERGY_%02d_%02d",i,j).Data()); // let browser update histogram view after commands execution
	  }

	if(psdgoodch[i][j] == 1)
	  {
	    serv->RegisterCommand(TString::Format("/Control/PSD/PSD_%02d_%02d",i,j).Data(),TString::Format("/Objects/PSD/PSD_%02d_%02d/->Reset()",i,j).Data(), "rootsys/icons/ed_delete.png");
	    serv->SetItemField(TString::Format("/Control/PSD/PSD_%02d_%02d",i,j).Data(),"_update_item", TString::Format("PSD_%02d_%02d",i,j).Data()); 
	  }
      }


  serv->Restrict("/Control",  "visible=admin");
  // serv->Restrict("/Control", "hidden=guest");
  serv->RegisterCommand("/Control/Exit","gSystem->Exit(0)");

  serv->SetItemField("/","_monitoring","5000");
  serv->SetItemField("/ADC/","_drawopt","");
  serv->SetItemField("/ENERGY/","_drawopt","");
  serv->SetItemField("/PSD/","_drawopt","");
#ifdef CLEAR_IN_NEW_RUN
#ifdef SHOW_OUTPUT_RATE
  serv->SetItemField("/RATE/","_drawopt","");
#endif
#endif	

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  unsigned int data;
  decoding *rawdec;
  rawdec = new decoding[MODULE_NUM_MAX];

  
  int nloop = 0;//the number of loop
  unsigned short ModNum = 0;
  
  int shm_id_modinfo;
  unsigned char *ptr_modinfo;
  shm_id_modinfo=shm_open("shmpixie16pkuxiadaq",O_RDWR,0);/*第一步：打开共享内存区*/
  if (shm_id_modinfo == -1)
    {
      printf( "open shared memory shmpixie16pkuxiadaq error.errno=%d,desc=%s.\n", errno, strerror(errno));
      return -1;
    }
  else
    {
      printf( "open shared memory ok.\n");
    }
  ftruncate(shm_id_modinfo,(off_t)(SHAREDMEMORYDATAOFFSET+PRESET_MAX_MODULES*2+PRESET_MAX_MODULES*4*SHAREDMEMORYDATASTATISTICS+PRESET_MAX_MODULES*4*SHAREDMEMORYDATAENERGYLENGTH*SHAREDMEMORYDATAMAXCHANNEL));
  ptr_modinfo = (unsigned char*)mmap(NULL,SHAREDMEMORYDATAOFFSET+PRESET_MAX_MODULES*2+(PRESET_MAX_MODULES*SHAREDMEMORYDATASTATISTICS*4)+PRESET_MAX_MODULES*4*SHAREDMEMORYDATAENERGYLENGTH*SHAREDMEMORYDATAMAXCHANNEL,PROT_READ|PROT_WRITE,MAP_SHARED,shm_id_modinfo,0);/*连接共享内存区*/


  int shm_id;
  unsigned char *ptr;

  shm_id=shm_open("shmpixie16pkuxiadaqdecoderonline",O_RDWR,0);/*第一步：打开共享内存区*/
  if (shm_id == -1)
    {
      printf( "open shared memory error.errno=%d,desc=%s.\n", errno, strerror(errno));
      return -1;
    }
  else
    {
      printf( "open shared memory ok.\n");
    }
  ftruncate(shm_id,(off_t)(60+BUFFLENGTH*MODULE_NUM_MAX*4));
  ptr = (unsigned char*)mmap(NULL,60+BUFFLENGTH*MODULE_NUM_MAX*4,PROT_READ|PROT_WRITE,MAP_SHARED,shm_id,0);/*连接共享内存区*/  

  
  int oldrun = -1;
  int run;
  int datalength;

  for (;;)
    {
      memcpy(&run,ptr,sizeof(int));

      if(oldrun != run)
	{
	  memcpy(&ModNum,ptr_modinfo+4,2);
	  for (int i = 0; i < ModNum; ++i)
	    {
	      short tempsampingrate;
	      memcpy(&tempsampingrate,ptr_modinfo+SHAREDMEMORYDATAOFFSET+2*i,2);
	      rawdec[i].setsamplerate(tempsampingrate);
	      rawdec[i].init();
	    }

	  oldrun = run;
	  printf("run: %d\n",run);
	  
#ifdef CLEAR_IN_NEW_RUN
	  for (int i = 0; i < MODULE_NUM_MAX; ++i)
	    for (int j = 0; j < 16; ++j)
	      {
		adc[i][j]->Reset("ICES");
		energy[i][j]->Reset("ICES");
		if(psdgoodch[i][j] == 1)
		  {
		    psd[i][j]->Reset("ICES");
		  }
#ifdef SHOW_OUTPUT_RATE
		rate[i][j]->Reset("ICES");
#endif 	
	      }  
#endif
	}

      for (int i = 0; i < ModNum; ++i)
	{
	  memcpy(&datalength,ptr+4*(i+1),sizeof(int));
	  if(datalength > 0)
	    {
	      // 数据解码/填充
	      for (int j = 0; j < datalength; ++j)
		{
		  // ptr+60+BUFFLENGTH*i*4+4*j
		  memcpy(&data,ptr+60+BUFFLENGTH*i*4+4*j,sizeof(unsigned int));
		  if(rawdec[i].readword(data))
		    {
		      adc[i][rawdec[i].getch()]->Fill(rawdec[i].getevte());
		      if(rawdec[i].getqsumflag())
			{
			  if(psdgoodch[i][rawdec[i].getch()] == 1)
			    {
			      psd[i][rawdec[i].getch()]->Fill(rawdec[i].getevte(), (rawdec[i].getqs1()-rawdec[i].getqs0()*psdp0[i][rawdec[i].getch()])/(rawdec[i].getqs1()+rawdec[i].getqs2()-rawdec[i].getqs0()*(psdp0[i][rawdec[i].getch()]+psdp1[i][rawdec[i].getch()])));
			    }
			}
		      double rawch = rawdec[i].getevte()+gRandom->Rndm();
		      energy[i][rawdec[i].getch()]->Fill(calia0[i][rawdec[i].getch()]+calia1[i][rawdec[i].getch()]*rawch+calia2[i][rawdec[i].getch()]*rawch*rawch);
#ifdef CLEAR_IN_NEW_RUN
#ifdef SHOW_OUTPUT_RATE
		      if(rawdec[i].getsamplerate()==250)
			{
			  rate[i][rawdec[i].getch()]->Fill(rawdec[i].getts()/125000000);
			  // std::cout<<rawdec[i].getts()<<std::endl;
			}
		      else
			{
			  rate[i][rawdec[i].getch()]->Fill(rawdec[i].getts()/100000000);
			  // std::cout<<rawdec[i].getts()<<std::endl;
			}
#endif
#endif 
		    }
		}
	      
	      datalength = 0;
	      memcpy(ptr+4*(i+1),&datalength,sizeof(int));
	    }
	}
      
      


      // for (int i = 0; i < 1000; ++i)
      // 	{
      // 	  int tmpi = int(gRandom->Uniform(0,13));
      // 	  int tmpj = int(gRandom->Uniform(0,16));
      // 	  adc[tmpi][tmpj]->Fill(gRandom->Uniform(0,65536));
      // 	}

      
      
      nloop++;
      if(nloop%100 == 0) if(gSystem->ProcessEvents()) break;
    }

  return 0;
}

// 
// main.cc ends here
