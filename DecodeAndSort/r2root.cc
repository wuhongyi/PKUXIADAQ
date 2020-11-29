// r2root.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 10月  2 19:11:39 2016 (+0800)
// Last-Updated: 日 11月 29 15:19:48 2020 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 124
// URL: http://wuhongyi.cn 

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
      timeoffset[0][(sid_tmp<<4)+ch_tmp] = offset_tmp;
      chlow[0][(sid_tmp<<4)+ch_tmp] = chlow_tmp;
      chhigh[0][(sid_tmp<<4)+ch_tmp] = chhigh_tmp;
    }
  readtxt.close();
  
  for (int i = 0; i < Crate0num; ++i)
    {
      if(Crate0SamplingRate[i] == 100 || Crate0SamplingRate[i] == 250 || Crate0SamplingRate[i] == 500)
	{
	  sprintf(tempfilename,"%s%04d/%s_R%04d_M%02d.bin",Crate0RAWFILEPATH,run[craten],Crate0RAWFILENAME,run[craten],i);
	  if(!IsFileExists(tempfilename))
	    {
	      std::cout<<"can't find raw data: "<<tempfilename<<std::endl;
	      exit(1);
	    }

	  rawdec[mmod].setsamplerate(Crate0SamplingRate[i]);
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
      timeoffset[1][(sid_tmp<<4)+ch_tmp] = offset_tmp;
      chlow[1][(sid_tmp<<4)+ch_tmp] = chlow_tmp;
      chhigh[1][(sid_tmp<<4)+ch_tmp] = chhigh_tmp;
    }
  readtxt.close();

  for (int i = 0; i < Crate1num; ++i)
    {
      if(Crate1SamplingRate[i] == 100 || Crate1SamplingRate[i] == 250 || Crate1SamplingRate[i] == 500)
	{
	  sprintf(tempfilename,"%s%04d/%s_R%04d_M%02d.bin",Crate1RAWFILEPATH,run[craten],Crate1RAWFILENAME,run[craten],i);
	  if(!IsFileExists(tempfilename))
	    {
	      std::cout<<"can't find raw data: "<<tempfilename<<std::endl;
	      exit(1);
	    }

	  rawdec[mmod].setsamplerate(Crate1SamplingRate[i]);
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
      timeoffset[2][(sid_tmp<<4)+ch_tmp] = offset_tmp;
      chlow[2][(sid_tmp<<4)+ch_tmp] = chlow_tmp;
      chhigh[2][(sid_tmp<<4)+ch_tmp] = chhigh_tmp;
    }
  readtxt.close();
  
  for (int i = 0; i < Crate2num; ++i)
    {
      if(Crate2SamplingRate[i] == 100 || Crate2SamplingRate[i] == 250 || Crate2SamplingRate[i] == 500)
	{
	  sprintf(tempfilename,"%s%04d/%s_R%04d_M%02d.bin",Crate2RAWFILEPATH,run[craten],Crate2RAWFILENAME,run[craten],i);
	  if(!IsFileExists(tempfilename))
	    {
	      std::cout<<"can't find raw data: "<<tempfilename<<std::endl;
	      exit(1);
	    }

	  rawdec[mmod].setsamplerate(Crate2SamplingRate[i]);
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
      timeoffset[3][(sid_tmp<<4)+ch_tmp] = offset_tmp;
      chlow[3][(sid_tmp<<4)+ch_tmp] = chlow_tmp;
      chhigh[3][(sid_tmp<<4)+ch_tmp] = chhigh_tmp;
    }
  readtxt.close();
  
  for (int i = 0; i < Crate3num; ++i)
    {
      if(Crate3SamplingRate[i] == 100 || Crate3SamplingRate[i] == 250 || Crate3SamplingRate[i] == 500)
	{
	  sprintf(tempfilename,"%s%04d/%s_R%04d_M%02d.bin",Crate3RAWFILEPATH,run[craten],Crate3RAWFILENAME,run[craten],i);
	  if(!IsFileExists(tempfilename))
	    {
	      std::cout<<"can't find raw data: "<<tempfilename<<std::endl;
	      exit(1);
	    }

	  rawdec[mmod].setsamplerate(Crate3SamplingRate[i]);
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
  // crate 4
#ifdef Crate4
#if Crate4num > 0
  flagfile += Crate4num;

  readtxt.open("crate4.dat");
  if(!readtxt.is_open()) {std::cout<<"can't open file crate4.dat."<<std::endl; }
  getline(readtxt,str_tmp);
  while(!readtxt.eof())
    {
      readtxt>>sid_tmp>>ch_tmp>>offset_tmp>>chlow_tmp>>chhigh_tmp;
      if(readtxt.eof()) break;
      timeoffset[4][(sid_tmp<<4)+ch_tmp] = offset_tmp;
      chlow[4][(sid_tmp<<4)+ch_tmp] = chlow_tmp;
      chhigh[4][(sid_tmp<<4)+ch_tmp] = chhigh_tmp;
    }
  readtxt.close();

  for (int i = 0; i < Crate4num; ++i)
    {
      if(Crate4SamplingRate[i] == 100 || Crate4SamplingRate[i] == 250 || Crate4SamplingRate[i] == 500)
	{
	  sprintf(tempfilename,"%s%04d/%s_R%04d_M%02d.bin",Crate4RAWFILEPATH,run[craten],Crate4RAWFILENAME,run[craten],i);
	  if(!IsFileExists(tempfilename))
	    {
	      std::cout<<"can't find raw data: "<<tempfilename<<std::endl;
	      exit(1);
	    }

	  rawdec[mmod].setsamplerate(Crate4SamplingRate[i]);
	  rawdec[mmod].openfile(tempfilename);
	}
      else
	{
	  rawdec[i].setsamplerate(0);
	}

      flagcrate[mmod] = 4;
      havedata[mmod] = false;
      mmod++;
    }

  craten++;
#else
  std::cout<<"Crate 4 num < 1. Please check the file ==> UserDefine.hh"<<std::endl;
  exit(1);
#endif
#endif
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // crate 5
#ifdef Crate5
#if Crate5num > 0
  flagfile += Crate5num;

  readtxt.open("crate5.dat");
  if(!readtxt.is_open()) {std::cout<<"can't open file crate5.dat."<<std::endl; }
  getline(readtxt,str_tmp);
  while(!readtxt.eof())
    {
      readtxt>>sid_tmp>>ch_tmp>>offset_tmp>>chlow_tmp>>chhigh_tmp;
      if(readtxt.eof()) break;
      timeoffset[5][(sid_tmp<<4)+ch_tmp] = offset_tmp;
      chlow[5][(sid_tmp<<4)+ch_tmp] = chlow_tmp;
      chhigh[5][(sid_tmp<<4)+ch_tmp] = chhigh_tmp;
    }
  readtxt.close();

  for (int i = 0; i < Crate5num; ++i)
    {
      if(Crate5SamplingRate[i] == 100 || Crate5SamplingRate[i] == 250 || Crate5SamplingRate[i] == 500)
	{
	  sprintf(tempfilename,"%s%04d/%s_R%04d_M%02d.bin",Crate5RAWFILEPATH,run[craten],Crate5RAWFILENAME,run[craten],i);
	  if(!IsFileExists(tempfilename))
	    {
	      std::cout<<"can't find raw data: "<<tempfilename<<std::endl;
	      exit(1);
	    }

	  rawdec[mmod].setsamplerate(Crate5SamplingRate[i]);
	  rawdec[mmod].openfile(tempfilename);
	}
      else
	{
	  rawdec[i].setsamplerate(0);
	}

      flagcrate[mmod] = 5;
      havedata[mmod] = false;
      mmod++;
    }

  craten++;
#else
  std::cout<<"Crate 5 num < 1. Please check the file ==> UserDefine.hh"<<std::endl;
  exit(1);
#endif
#endif

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // crate 6
#ifdef Crate6
#if Crate6num > 0
  flagfile += Crate6num;

  readtxt.open("crate6.dat");
  if(!readtxt.is_open()) {std::cout<<"can't open file crate6.dat."<<std::endl; }
  getline(readtxt,str_tmp);
  while(!readtxt.eof())
    {
      readtxt>>sid_tmp>>ch_tmp>>offset_tmp>>chlow_tmp>>chhigh_tmp;
      if(readtxt.eof()) break;
      timeoffset[6][(sid_tmp<<4)+ch_tmp] = offset_tmp;
      chlow[6][(sid_tmp<<4)+ch_tmp] = chlow_tmp;
      chhigh[6][(sid_tmp<<4)+ch_tmp] = chhigh_tmp;
    }
  readtxt.close();

  for (int i = 0; i < Crate6num; ++i)
    {
      if(Crate6SamplingRate[i] == 100 || Crate6SamplingRate[i] == 250 || Crate6SamplingRate[i] == 500)
	{
	  sprintf(tempfilename,"%s%04d/%s_R%04d_M%02d.bin",Crate6RAWFILEPATH,run[craten],Crate6RAWFILENAME,run[craten],i);
	  if(!IsFileExists(tempfilename))
	    {
	      std::cout<<"can't find raw data: "<<tempfilename<<std::endl;
	      exit(1);
	    }

	  rawdec[mmod].setsamplerate(Crate6SamplingRate[i]);
	  rawdec[mmod].openfile(tempfilename);
	}
      else
	{
	  rawdec[i].setsamplerate(0);
	}

      flagcrate[mmod] = 6;
      havedata[mmod] = false;
      mmod++;
    }

  craten++;
#else
  std::cout<<"Crate 6 num < 1. Please check the file ==> UserDefine.hh"<<std::endl;
  exit(1);
#endif
#endif

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // crate 7
#ifdef Crate7
#if Crate7num > 0
  flagfile += Crate7num;

  readtxt.open("crate7.dat");
  if(!readtxt.is_open()) {std::cout<<"can't open file crate7.dat."<<std::endl; }
  getline(readtxt,str_tmp);
  while(!readtxt.eof())
    {
      readtxt>>sid_tmp>>ch_tmp>>offset_tmp>>chlow_tmp>>chhigh_tmp;
      if(readtxt.eof()) break;
      timeoffset[7][(sid_tmp<<4)+ch_tmp] = offset_tmp;
      chlow[7][(sid_tmp<<4)+ch_tmp] = chlow_tmp;
      chhigh[7][(sid_tmp<<4)+ch_tmp] = chhigh_tmp;
    }
  readtxt.close();

  for (int i = 0; i < Crate7num; ++i)
    {
      if(Crate7SamplingRate[i] == 100 || Crate7SamplingRate[i] == 250 || Crate7SamplingRate[i] == 500)
	{
	  sprintf(tempfilename,"%s%04d/%s_R%04d_M%02d.bin",Crate7RAWFILEPATH,run[craten],Crate7RAWFILENAME,run[craten],i);
	  if(!IsFileExists(tempfilename))
	    {
	      std::cout<<"can't find raw data: "<<tempfilename<<std::endl;
	      exit(1);
	    }

	  rawdec[mmod].setsamplerate(Crate7SamplingRate[i]);
	  rawdec[mmod].openfile(tempfilename);
	}
      else
	{
	  rawdec[i].setsamplerate(0);
	}

      flagcrate[mmod] = 7;
      havedata[mmod] = false;
      mmod++;
    }

  craten++;
#else
  std::cout<<"Crate 7 num < 1. Please check the file ==> UserDefine.hh"<<std::endl;
  exit(1);
#endif
#endif

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  sprintf(tempfilename,"%s%s_C%d",ROOTFILEPATH,ROOTFILENAME,craten);
  for (int i = 0; i < craten; ++i)
    {
      sprintf(tempfilename,"%s_%04d",tempfilename,run[i]);
    }
  sprintf(tempfilename,"%s.root",tempfilename);
  
  file = new TFile(tempfilename,"RECREATE");
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
  // t->SetCacheSize(100*1024*1024);
}

r2root::~r2root()
{
  delete [] rawdec;
}

void r2root::Process()
{
  benchmark->Start("r2root");//计时开始

  int posflag;
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

  Long64_t deltat = 1000000000;//1 s
  while(true)
    {
      timestamp = LLONG_MAX;
      for (int i = 0; i < flagfile; ++i)
	{

	  while(havedata[i])
	    {
	      if(rawdec[i].getsamplerate() == 250)
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
		  havedata[mark] = rawdec[mark].getnextevt();

		  posflag = (mapvalue.sid<<4)+mapvalue.ch;
		  if(mapvalue.evte < chlow[flagcrate[i]][posflag] || mapvalue.evte > chhigh[flagcrate[i]][posflag]) continue;
		  if(mapvalue.sr == 250)
		    {
		      mapvalue.ts = mapvalue.ts+(timeoffset[flagcrate[i]][posflag]/8);
		      mapvalue.tsflag = mapvalue.ts*8;
		    }
		  else
		    {
		      mapvalue.ts = mapvalue.ts+(timeoffset[flagcrate[i]][posflag]/10);
		      mapvalue.tsflag = mapvalue.ts*10;
		    }

		  flagkey = ((((((mapvalue.tsflag)<<4)+mapvalue.cid)<<4)+mapvalue.sid)<<4)+mapvalue.ch;
		  // flagkey = ((((mapvalue.tsflag)<<4)+mapvalue.sid)<<4)+mapvalue.ch;
		  sortdata.insert(std::make_pair(flagkey,mapvalue));
		}
	      else
		break;
	      
	    }
	}

      deltat += 1000000000;//1 s

      sizelen = sortdata.size() * 0.9;// 0.9
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
	  sortdata.erase(itkey);
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
	      sortdata.erase(itkey);
	      file->cd();
	      t->Fill();
	    }
	  break;
	}

    }

  std::cout<<std::endl;
  
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
