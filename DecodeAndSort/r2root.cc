// r2root.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 10月  2 19:11:39 2016 (+0800)
// Last-Updated: 日 3月 22 19:20:06 2020 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 105
// URL: http://wuhongyi.cn 

#include "r2root.hh"
#include "UserDefine.hh"

#include <iostream>
#include <fstream>
#include <climits>
#include <cmath>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

r2root::r2root(TString rawfilepath,TString rootfilepath,TString filename,int runnumber)
{
  flagfile = 0;

  Run = runnumber;
  FileName = filename;
  benchmark = new TBenchmark;
  


  memset(timeoffset,0,256*sizeof(Short_t));
  memset(chlow,0,256*sizeof(UShort_t));
  memset(chhigh,0,256*sizeof(UShort_t));
  
  std::ifstream readtxt;
  readtxt.open("par.dat");
  if(!readtxt.is_open())
    {
      std::cout<<"can't open file."<<std::endl;
    }

  std::string str_tmp;
  getline(readtxt,str_tmp);
  // std::cout<<str_tmp<<std::endl;
  Short_t sid_tmp;
  Short_t ch_tmp;
  Short_t offset_tmp;
  UShort_t chlow_tmp;
  UShort_t chhigh_tmp;
  while(!readtxt.eof())
    {
      readtxt>>sid_tmp>>ch_tmp>>offset_tmp>>chlow_tmp>>chhigh_tmp;
      if(readtxt.eof()) break;
      timeoffset[(sid_tmp<<4)+ch_tmp] = offset_tmp;
      chlow[(sid_tmp<<4)+ch_tmp] = chlow_tmp;
      chhigh[(sid_tmp<<4)+ch_tmp] = chhigh_tmp;
    }
  readtxt.close();


  char tempfilename[1024];
  

#ifdef Crate0
#if Crate0num > 0
  flagfile = Crate0num;
#else
  std::cout<<"Crate 0 num < 1. Please check the file ==> UserDefine.hh"<<std::endl;
  exit(1);
#endif
#else
  std::cout<<"Not define Crate0. Please check the file ==> UserDefine.hh"<<std::endl;
  exit(1);
#endif

  for (int i = 0; i < flagfile; ++i)
    {
      if(Crate0SamplingRate[i] == 100 || Crate0SamplingRate[i] == 250 || Crate0SamplingRate[i] == 500)
	{
	  sprintf(tempfilename,"%s%04d/%s_R%04d_M%02d.bin",rawfilepath.Data(),runnumber,filename.Data(),runnumber,i);
	  if(!IsFileExists(tempfilename))
	    {
	      std::cout<<"can't find raw data: "<<tempfilename<<std::endl;
	      exit(1);
	    }
	}
    }

  std::cout<<"Mod No. "<<flagfile<<std::endl;
  
  rawdec = new decoder[flagfile];
  for (int i = 0; i < flagfile; ++i)
    {
      if(Crate0SamplingRate[i] == 100 || Crate0SamplingRate[i] == 250 || Crate0SamplingRate[i] == 500)
	{
	  rawdec[i].setsamplerate(Crate0SamplingRate[i]);
	  sprintf(tempfilename,"%s%04d/%s_R%04d_M%02d.bin",rawfilepath.Data(),runnumber,filename.Data(),runnumber,i);
	  rawdec[i].openfile(tempfilename);
	}
      else
	{
	  rawdec[i].setsamplerate(0);
	}
      havedata[i] = false;
    }

  sprintf(tempfilename,"%s%s_R%04d_sort.root",rootfilepath.Data(),filename.Data(),runnumber);
  file = new TFile(tempfilename,"RECREATE");
  t = new TTree("tree","PKU XIA Pixie-16 sort Data");
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

  Long64_t deltat = 1000000000;
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
		  if(mapvalue.evte < chlow[posflag] || mapvalue.evte > chhigh[posflag]) continue;
		  if(sr == 250)
		    {
		      mapvalue.ts = mapvalue.ts+(timeoffset[posflag]/8);
		      mapvalue.tsflag = mapvalue.ts*8;
		    }
		  else
		    {
		      mapvalue.ts = mapvalue.ts+(timeoffset[posflag]/10);
		      mapvalue.tsflag = mapvalue.ts*10;
		    }
		  flagkey = ((((mapvalue.tsflag)<<4)+mapvalue.sid)<<4)+mapvalue.ch;
		  sortdata.insert(std::make_pair(flagkey,mapvalue));
		}
	      else
		break;
	      
	    }
	}

      deltat += 1000000000;

      sizelen = sortdata.size() * 0.9;
      for (unsigned int i = 0; i < sizelen; ++i)
	{
	  itkey = sortdata.begin();
	  tsflag = itkey->second.tsflag;
	  sr = itkey->second.sr;
	  pileup = itkey->second.pileup;
	  outofr = itkey->second.outofr;
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
