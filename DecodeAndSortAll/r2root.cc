// r2root.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 10月  2 19:11:39 2016 (+0800)
// Last-Updated: 四 9月  5 19:38:31 2024 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 156
// URL: http://wuhongyi.cn 

// 20220417 sprintf 套娃警告问题需要处理，应更改为 TString
// 20240713 解决 gcc10 以上版本 sprintf 套娃警告问题

#include "r2root.hh"
#include "UserDefine.hh"

#include <iostream>
#include <fstream>
#include <climits>
#include <cmath>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

r2root::r2root(int run[8])
{
  flagfile = 0;
  benchmark = new TBenchmark;
  memset(timeoffset,0,8*256*sizeof(Short_t));
  memset(chlow,0,8*256*sizeof(UShort_t));
  memset(chhigh,0,8*256*sizeof(UShort_t));

  rawdec = new decoder[MAXMODULENUMBER];
  
  std::string str_tmp;
  Short_t sid_tmp;
  Short_t ch_tmp;
  Short_t offset_tmp;
  UShort_t chlow_tmp;
  UShort_t chhigh_tmp;

  int craten = 0;
  mmod = 0;
  std::ifstream readtxt;
  char tempfilename[1024];

  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < MAXBOARD; ++j)
      for (int k = 0; k < MAXCHANNEL; ++k)
	{
	  timeoffset[i][j][k] = 0;
	  chlow[i][j][k] = 0;
	  chhigh[i][j][k] = 65535;
	}
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // crate 0
#ifdef Crate0
#if Crate0num > 0
  flagfile += Crate0num;

  readtxt.open("crate0.dat");
  if(!readtxt.is_open()) {std::cout<<"can't open file crate0.dat."<<std::endl; }
  getline(readtxt,str_tmp);
  while(!readtxt.eof())
    {
      readtxt>>sid_tmp>>ch_tmp>>offset_tmp>>chlow_tmp>>chhigh_tmp;
      if(readtxt.eof()) break;
      timeoffset[0][sid_tmp][ch_tmp] = offset_tmp;
      chlow[0][sid_tmp][ch_tmp] = chlow_tmp;
      chhigh[0][sid_tmp][ch_tmp] = chhigh_tmp;
    }
  readtxt.close();
  
  for (int i = 0; i < Crate0num; ++i)
    {
      if(Crate0SamplingRate[i] == 100 || Crate0SamplingRate[i] == 125 || Crate0SamplingRate[i] == 250 || Crate0SamplingRate[i] == 500)
	{
	  sprintf(tempfilename,"%s%04d/%s_R%04d_M%02d.bin",Crate0RAWFILEPATH,run[craten],Crate0RAWFILENAME,run[craten],i);
	  if(!IsFileExists(tempfilename))
	    {
	      std::cout<<"can't find raw data: "<<tempfilename<<std::endl;
	      exit(1);
	    }

	  rawdec[mmod].setsamplerate(Crate0SamplingRate[i]);
	  rawdec[mmod].setrevision(Crate0Revision[i]);
	  rawdec[mmod].openfile(tempfilename);
	}
      else
	{
	  rawdec[i].setsamplerate(0);
	}

      flagcrate[mmod] = 0;
      havedata[mmod] = false;
      mmod++;
    }

  craten++;
#else
  std::cout<<"Crate 0 num < 1. Please check the file ==> UserDefine.hh"<<std::endl;
  exit(1);
#endif
#endif

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // crate 1
#ifdef Crate1
#if Crate1num > 0
  flagfile += Crate1num;

  readtxt.open("crate1.dat");
  if(!readtxt.is_open()) {std::cout<<"can't open file crate1.dat."<<std::endl; }
  getline(readtxt,str_tmp);
  while(!readtxt.eof())
    {
      readtxt>>sid_tmp>>ch_tmp>>offset_tmp>>chlow_tmp>>chhigh_tmp;
      if(readtxt.eof()) break;
      timeoffset[1][sid_tmp][ch_tmp] = offset_tmp;
      chlow[1][sid_tmp][ch_tmp] = chlow_tmp;
      chhigh[1][sid_tmp][ch_tmp] = chhigh_tmp;
    }
  readtxt.close();

  for (int i = 0; i < Crate1num; ++i)
    {
      if(Crate1SamplingRate[i] == 100 || Crate1SamplingRate[i] == 125 || Crate1SamplingRate[i] == 250 || Crate1SamplingRate[i] == 500)
	{
	  sprintf(tempfilename,"%s%04d/%s_R%04d_M%02d.bin",Crate1RAWFILEPATH,run[craten],Crate1RAWFILENAME,run[craten],i);
	  if(!IsFileExists(tempfilename))
	    {
	      std::cout<<"can't find raw data: "<<tempfilename<<std::endl;
	      exit(1);
	    }

	  rawdec[mmod].setsamplerate(Crate1SamplingRate[i]);
	  rawdec[mmod].setrevision(Crate1Revision[i]);
	  rawdec[mmod].openfile(tempfilename);
	}
      else
	{
	  rawdec[i].setsamplerate(0);
	}
      flagcrate[mmod] = 1;
      havedata[mmod] = false;
      mmod++;
    }

  craten++;
#else
  std::cout<<"Crate 1 num < 1. Please check the file ==> UserDefine.hh"<<std::endl;
  exit(1);
#endif
#endif

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // crate 2
#ifdef Crate2
#if Crate2num > 0
  flagfile += Crate2num;

  readtxt.open("crate2.dat");
  if(!readtxt.is_open()) {std::cout<<"can't open file crate2.dat."<<std::endl; }
  getline(readtxt,str_tmp);
  while(!readtxt.eof())
    {
      readtxt>>sid_tmp>>ch_tmp>>offset_tmp>>chlow_tmp>>chhigh_tmp;
      if(readtxt.eof()) break;
      timeoffset[2][sid_tmp][ch_tmp] = offset_tmp;
      chlow[2][sid_tmp][ch_tmp] = chlow_tmp;
      chhigh[2][sid_tmp][ch_tmp] = chhigh_tmp;
    }
  readtxt.close();
  
  for (int i = 0; i < Crate2num; ++i)
    {
      if(Crate2SamplingRate[i] == 100 || Crate2SamplingRate[i] == 125 || Crate2SamplingRate[i] == 250 || Crate2SamplingRate[i] == 500)
	{
	  sprintf(tempfilename,"%s%04d/%s_R%04d_M%02d.bin",Crate2RAWFILEPATH,run[craten],Crate2RAWFILENAME,run[craten],i);
	  if(!IsFileExists(tempfilename))
	    {
	      std::cout<<"can't find raw data: "<<tempfilename<<std::endl;
	      exit(1);
	    }

	  rawdec[mmod].setsamplerate(Crate2SamplingRate[i]);
	  rawdec[mmod].setrevision(Crate2Revision[i]);
	  rawdec[mmod].openfile(tempfilename);
	}
      else
	{
	  rawdec[i].setsamplerate(0);
	}

      flagcrate[mmod] = 2;
      havedata[mmod] = false;
      mmod++;
    }

  craten++;
#else
  std::cout<<"Crate 2 num < 1. Please check the file ==> UserDefine.hh"<<std::endl;
  exit(1);
#endif
#endif

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // crate 3
#ifdef Crate3
#if Crate3num > 0
  flagfile += Crate3num;

  readtxt.open("crate3.dat");
  if(!readtxt.is_open()) {std::cout<<"can't open file crate3.dat."<<std::endl; }
  getline(readtxt,str_tmp);
  while(!readtxt.eof())
    {
      readtxt>>sid_tmp>>ch_tmp>>offset_tmp>>chlow_tmp>>chhigh_tmp;
      if(readtxt.eof()) break;
      timeoffset[3][sid_tmp][ch_tmp] = offset_tmp;
      chlow[3][sid_tmp][ch_tmp] = chlow_tmp;
      chhigh[3][sid_tmp][ch_tmp] = chhigh_tmp;
    }
  readtxt.close();
  
  for (int i = 0; i < Crate3num; ++i)
    {
      if(Crate3SamplingRate[i] == 100 || Crate3SamplingRate[i] == 125 || Crate3SamplingRate[i] == 250 || Crate3SamplingRate[i] == 500)
	{
	  sprintf(tempfilename,"%s%04d/%s_R%04d_M%02d.bin",Crate3RAWFILEPATH,run[craten],Crate3RAWFILENAME,run[craten],i);
	  if(!IsFileExists(tempfilename))
	    {
	      std::cout<<"can't find raw data: "<<tempfilename<<std::endl;
	      exit(1);
	    }

	  rawdec[mmod].setsamplerate(Crate3SamplingRate[i]);
	  rawdec[mmod].setrevision(Crate3Revision[i]);
	  rawdec[mmod].openfile(tempfilename);
	}
      else
	{
	  rawdec[i].setsamplerate(0);
	}

      flagcrate[mmod] = 3;
      havedata[mmod] = false;
      mmod++;
    }

  craten++;
#else
  std::cout<<"Crate 3 num < 1. Please check the file ==> UserDefine.hh"<<std::endl;
  exit(1);
#endif
#endif

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  char *tempname;
  tempname = (char *)malloc(1024);
  size_t extra_len = 0;
  extra_len += sprintf(tempname+extra_len,"%s%s_C%d",ROOTFILEPATH,ROOTFILENAME,craten);

  for (int i = 0; i < craten; ++i)
    {
      extra_len += sprintf(tempname+extra_len,"_%04d",run[i]);
    }

#ifdef WAVEFORM
  extra_len += sprintf(tempname+extra_len,"%s","_wave.root");
#else
  extra_len += sprintf(tempname+extra_len,"%s","_notwave.root");
#endif

  file = new TFile(tempname,"RECREATE");
  t = new TTree("tree","GDDAQ Multi-Crate sort Data");
  // t->Branch("tsflag", &tsflag, "tsflag/L");
  t->Branch("sr", &sr, "sr/S");
  t->Branch("pileup", &pileup, "pileup/O");
  t->Branch("outofr", &outofr, "outofr/O");
  t->Branch("cid",&cid,"cid/S");
  t->Branch("sid", &sid, "sid/S");
  t->Branch("ch", &ch, "ch/S");
  t->Branch("evte", &evte, "evte/s");
  t->Branch("ts", &ts, "ts/L");
  t->Branch("cfd", &cfd, "cfd/S");
  t->Branch("cfdft", &cfdft, "cfdft/O");
  t->Branch("cfds", &cfds, "cfds/S");

#ifdef ENERGYSUM
  t->Branch("esumf", &esumf, "esumf/O");
  t->Branch("trae",&trae,"trae/i");
  t->Branch("leae",&leae,"leae/i");
  t->Branch("gape",&gape,"gape/i");
  t->Branch("base",&base,"base/i");
#endif
  
#ifdef QDCSUM
  t->Branch("qsumf", &qsumf, "qsumf/O");
  t->Branch("qs",&qs,"qs[8]/i");
#endif

#ifdef EXTERNALTS
  t->Branch("etsf", &etsf, "etsf/O");
  t->Branch("ets",&ets,"ets/L");
#endif
  
#ifdef WAVEFORM
  t->Branch("ltra",&ltra,"ltra/s");
  t->Branch("data",&data,"data[ltra]/s");
#endif
  // t->SetCacheSize(100*1024*1024);
}

r2root::~r2root()
{
  delete [] rawdec;
}

void r2root::Process()
{
  benchmark->Start("r2root");//计时开始

  unsigned int sizelen;
  Long64_t timestamp;
  int mark = -1; 
  bool flagdata = false;
  nevt = 0;

  for (int i = 0; i < flagfile; ++i)
    {
      if(rawdec[i].getsamplerate() != 0)
	havedata[i] = rawdec[i].getnextevt();
    }

  Long64_t deltat = TIMEBUFF;
  while(true)
    {
      timestamp = LLONG_MAX;
      for (int i = 0; i < flagfile; ++i)
	{

	  while(havedata[i])
	    {
	      if(rawdec[i].getsamplerate() == 250 || rawdec[i].getsamplerate() == 125)
		timestamp = 8*rawdec[i].getts();
	      else
		timestamp = 10*rawdec[i].getts();
	      
	      if(timestamp < deltat)
		{
		  mark = i;
		  mapvalue.sr = rawdec[mark].getsamplerate();
		  mapvalue.ch = rawdec[mark].getch();
		  mapvalue.sid = rawdec[mark].getsid();
		  mapvalue.cid = rawdec[mark].getcid();
		  mapvalue.pileup = rawdec[mark].getpileup();
		  mapvalue.ts = rawdec[mark].getts();
		  mapvalue.cfd = rawdec[mark].getcfd();
		  mapvalue.cfdft = rawdec[mark].getcfdft();
		  mapvalue.cfds = rawdec[mark].getcfds();
		  mapvalue.evte = rawdec[mark].getevte();
		  mapvalue.outofr = rawdec[mark].getoutofr();
		  
#ifdef ENERGYSUM
		  mapvalue.esumf = rawdec[mark].getesumflag();
		  if(mapvalue.esumf)
		    {
		      mapvalue.trae = rawdec[mark].gettrae(); 
		      mapvalue.leae = rawdec[mark].getleae(); 
		      mapvalue.gape = rawdec[mark].getgape(); 		      
		      mapvalue.base = rawdec[mark].getbase(); 
		    }
		  else
		    {
		      mapvalue.trae = 0; 
		      mapvalue.leae = 0; 
		      mapvalue.gape = 0; 		      
		      mapvalue.base = 0; 
		    }
#endif
		  
#ifdef QDCSUM
		  mapvalue.qsumf = rawdec[mark].getqsumflag();
		  if(mapvalue.qsumf)
		    {
		      rawdec[mark].getqs(qs);
		      memcpy(mapvalue.qs,qs,sizeof(unsigned int)*8);
		    }
		  else
		    {
		      memset(mapvalue.qs,0,sizeof(unsigned int)*8);
		    }
#endif

#ifdef EXTERNALTS
		  mapvalue.etsf = rawdec[mark].getetsflag();
		  if(mapvalue.etsf)
		    mapvalue.ets = rawdec[mark].getets();
		  else
		    mapvalue.ets = 0;
#endif
		  
#ifdef WAVEFORM
		  mapvalue.ltra = rawdec[mark].getltra();
		  if(mapvalue.ltra > 0)
		    {
		      mapvalue.data = new unsigned short [mapvalue.ltra];
		      rawdec[mark].gettrace(mapvalue.data);
		    }
#endif		  
		  havedata[mark] = rawdec[mark].getnextevt();

		  if(mapvalue.evte < chlow[flagcrate[i]][mapvalue.sid][mapvalue.ch] || mapvalue.evte > chhigh[flagcrate[i]][mapvalue.sid][mapvalue.ch]) continue;
		  if(mapvalue.sr == 250 || mapvalue.sr == 125)
		    {
		      mapvalue.ts = mapvalue.ts+(timeoffset[flagcrate[i]][mapvalue.sid][mapvalue.ch]/8);
		      mapvalue.tsflag = mapvalue.ts*8;
		    }
		  else
		    {
		      mapvalue.ts = mapvalue.ts+(timeoffset[flagcrate[i]][mapvalue.sid][mapvalue.ch]/10);
		      mapvalue.tsflag = mapvalue.ts*10;
		    }

		  flagkey = ((((((mapvalue.tsflag)<<4)+mapvalue.cid)<<4)+mapvalue.sid)<<6)+mapvalue.ch;

		  sortdata.insert(std::make_pair(flagkey,mapvalue));

// #ifdef WAVEFORM
// 		  if(mapvalue.ltra > 0) delete mapvalue.data;
// #endif	
		}
	      else
		break;
	      
	    }
	}

      deltat += TIMEBUFF;//

      sizelen = sortdata.size() * PROCESSBUFF;//
      for (unsigned int i = 0; i < sizelen; ++i)
	{
	  itkey = sortdata.begin();
	  tsflag = itkey->second.tsflag;
	  sr = itkey->second.sr;
	  pileup = itkey->second.pileup;
	  outofr = itkey->second.outofr;
	  cid = itkey->second.cid;
	  sid = itkey->second.sid;
	  ch = itkey->second.ch;
	  evte = itkey->second.evte;
	  ts = itkey->second.ts;
	  cfd = itkey->second.cfd;
	  cfdft = itkey->second.cfdft;
	  cfds = itkey->second.cfds;

#ifdef ENERGYSUM
	  esumf = itkey->second.esumf;
	  trae = itkey->second.trae; 
	  leae = itkey->second.leae; 
	  gape = itkey->second.gape; 
	  base = itkey->second.base; 	  
#endif	  
	  
#ifdef QDCSUM
	  qsumf = itkey->second.qsumf;
	  memcpy(qs,itkey->second.qs,sizeof(unsigned int)*8);
#endif

#ifdef EXTERNALTS
	  etsf = itkey->second.etsf;
	  ets = itkey->second.ets;
#endif
	  
#ifdef WAVEFORM
	  ltra = itkey->second.ltra;
	  if(ltra > 0)
	    {
	      memcpy(data,itkey->second.data,sizeof(unsigned short)*ltra);
	      delete itkey->second.data;
	    }
#endif	  
	  sortdata.erase(itkey);
	  file = t->GetCurrentFile();
	  file->cd();
	  t->Fill();
	}
      
  

      nevt++;
      // if(nevt%10000 == 0)
      // 	{
	  std::cout<<"\r"<<"nt: "<<nevt;
	  std::cout << std::flush;
	// }
     
      flagdata = false;
      for (int i = 0; i < flagfile; ++i)
	{
	  flagdata += havedata[i];
	}
      if(!flagdata)
	{
	  while(sortdata.size() > 0)
	    {
	      itkey = sortdata.begin();
	      tsflag = itkey->second.tsflag;
	      sr = itkey->second.sr;
	      pileup = itkey->second.pileup;
	      outofr = itkey->second.outofr;
	      cid = itkey->second.cid;
	      sid = itkey->second.sid;
	      ch = itkey->second.ch;
	      evte = itkey->second.evte;
	      ts = itkey->second.ts;
	      cfd = itkey->second.cfd;
	      cfdft = itkey->second.cfdft;
	      cfds = itkey->second.cfds;

#ifdef ENERGYSUM
	      esumf = itkey->second.esumf;
	      trae = itkey->second.trae; 
	      leae = itkey->second.leae; 
	      gape = itkey->second.gape; 
	      base = itkey->second.base; 	  
#endif	  
	 	      
#ifdef QDCSUM
	      qsumf = itkey->second.qsumf;
	      memcpy(qs,itkey->second.qs,sizeof(unsigned int)*8);
#endif

#ifdef EXTERNALTS
	      etsf = itkey->second.etsf;
	      ets = itkey->second.ets;
#endif
	      
#ifdef WAVEFORM
	      ltra = itkey->second.ltra;
	      if(ltra > 0)
		{
		  memcpy(data,itkey->second.data,sizeof(unsigned short)*ltra);
		  delete itkey->second.data;
		}
#endif
	      sortdata.erase(itkey);
	      file = t->GetCurrentFile();
	      file->cd();
	      t->Fill();
	    }
	  break;
	}

    }

  std::cout<<std::endl;

  file = t->GetCurrentFile();
  file->cd();
  t->Write();
  file->Close();


  
  
  benchmark->Show("r2root");//计时结束并输出时间
}

bool r2root::IsFileExists(const char *name)
{
  if((access(name,F_OK)) != -1)  
    {  
      return true;
    }  
  else  
    {  
      return false;
    } 
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// IEEEFloating2Decimal:
// Convert a IEEE 754 standrad floating point number (1-bit sign, 8-bit exponent, and 23-bit mantissa) to a decimal fractional number.
// Return Value:
// the converted decimal fractional number
double r2root::IEEEFloating2Decimal(unsigned int IEEEFloatingNumber)
{
  short signbit, exponent;
  double mantissa, DecimalNumber;
	
  signbit = (short)(IEEEFloatingNumber >> 31);
  exponent = (short)((IEEEFloatingNumber & 0x7F800000) >> 23) - 127;
  mantissa = 1.0 + (double)(IEEEFloatingNumber & 0x7FFFFF) / std::pow(2.0, 23.0);
  if(signbit == 0)
    {
      DecimalNumber = mantissa * std::pow(2.0, (double)exponent);
    }
  else
    {
      DecimalNumber = - mantissa * std::pow(2.0, (double)exponent);
    }
	
  return(DecimalNumber);	
}



// 
// r2root.cc ends here
