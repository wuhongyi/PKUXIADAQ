// r2root.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 10月  2 19:11:39 2016 (+0800)
// Last-Updated: 日 6月 10 04:16:57 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 72
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
  for (int i = 0; i < MAXBOARD; ++i)
    for (int j = 0; j < 16; ++j)
      {
	StatisticsOutOfRange[i][j] = 0;
	StatisticsPileup[i][j] = 0;
	StatisticsCfdForcedTrigger[i][j] = 0;
	StatisticsEnergy0[i][j] = 0;
	StatisticsWaveformCount[i][j] = 0;
	StatisticsEventCount[i][j] = 0;
      }
  Run = runnumber;
  FileName = filename;
  benchmark = new TBenchmark;
  
  char tempfilename[1024];

  // OLD 20180221
  // do
  //   {
  //     sprintf(tempfilename,"%s%s_R%04d_M%02d.bin",rawfilepath.Data(),filename.Data(),runnumber,flagfile);
  //     // std::cout<<tempfilename<<std::endl;
  //   } while (IsFileExists(tempfilename) && ++flagfile);
  
  // std::cout<<"Mod No. "<<flagfile<<std::endl;

  // if(flagfile == 0)
  //   {
  //     std::cout<<"can't find raw data!"<<std::endl;
  //     exit(1);
  //   }

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
	  sprintf(tempfilename,"%s%s_R%04d_M%02d.bin",rawfilepath.Data(),filename.Data(),runnumber,i);
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
      // OLD 20180221
      // rawdec[i].setsamplerate(100);
      // sprintf(tempfilename,"%s%s_R%04d_M%02d.bin",rawfilepath.Data(),filename.Data(),runnumber,i);
      // rawdec[i].openfile(tempfilename);
      // havedata[i] = false;

      if(Crate0SamplingRate[i] == 100 || Crate0SamplingRate[i] == 250 || Crate0SamplingRate[i] == 500)
	{
	  rawdec[i].setsamplerate(Crate0SamplingRate[i]);
	  sprintf(tempfilename,"%s%s_R%04d_M%02d.bin",rawfilepath.Data(),filename.Data(),runnumber,i);
	  rawdec[i].openfile(tempfilename);
	}
      else
	{
	  rawdec[i].setsamplerate(0);
	}
      havedata[i] = false;
    }

  sprintf(tempfilename,"%s%s_R%04d.root",rootfilepath.Data(),filename.Data(),runnumber);
  file = new TFile(tempfilename,"RECREATE");
  t = new TTree("tree","PKU XIA Pixie-16 Data");
  
  t->Branch("sr",&sr,"sr/S");
  
  t->Branch("pileup",&pileup,"pileup/O");
  t->Branch("outofr",&outofr,"outofr/O");

  t->Branch("cid",&cid,"cid/S");
  t->Branch("sid",&sid,"sid/S");
  t->Branch("ch",&ch,"ch/S");
  t->Branch("evte",&evte,"evte/s");
  t->Branch("ts",&ts,"ts/L");
  t->Branch("ets",&ets,"ets/L");
  
  t->Branch("cfd",&cfd,"cfd/S");
  t->Branch("cfdft",&cfdft,"cfdft/O");
  t->Branch("cfds",&cfds,"cfds/S");
  
  t->Branch("trae",&trae,"trae/i");
  t->Branch("leae",&leae,"leae/i");
  t->Branch("gape",&gape,"gape/i");
  t->Branch("base",&base,"base/D");

  t->Branch("qs",&qs,"qs[8]/i");

  t->Branch("ltra",&ltra,"ltra/s");
  t->Branch("data",&data,"data[ltra]/s");
  t->Branch("dt",&dt,"dt[ltra]/s");

  t->Branch("nevt",&nevt,"nevt/I");
}

r2root::~r2root()
{
  delete [] rawdec;
}

void r2root::Process()
{
  benchmark->Start("r2root");//计时开始
  
  Long64_t timestamp;
  int mark = -1; 
  bool flagdata = false;
  nevt = 0;

  for (int i = 0; i < flagfile; ++i)
    {
      // OLD 20180221
      // havedata[i] = rawdec[i].getnextevt();
      
      if(rawdec[i].getsamplerate() != 0)
	havedata[i] = rawdec[i].getnextevt();
    }

  
  while(true)
    {
      timestamp = LLONG_MAX;
      for (int i = 0; i < flagfile; ++i)
	{
	  if(havedata[i])
	    {
	      // OLD 20180221
	      // if(rawdec[i].getts() < timestamp)
	      // 	{
	      // 	  timestamp = rawdec[i].getts();
	      // 	  mark = i;
	      // 	}

	      switch(int(rawdec[i].getsamplerate()))
	      	{
	      	case 100:
	      	  if(10*rawdec[i].getts() < timestamp)
	      	    {
	      	      timestamp = 10*rawdec[i].getts();
	      	      mark = i;
	      	    }
	      	  break;
	      	case 250:
	      	  if(8*rawdec[i].getts() < timestamp)
	      	    {
	      	      timestamp = 8*rawdec[i].getts();
	      	      mark = i;
	      	    }
	      	  break;
	      	case 500:
	      	  if(10*rawdec[i].getts() < timestamp)
	      	    {
	      	      timestamp = 10*rawdec[i].getts();
	      	      mark = i;
	      	    }
	      	  break;
	      	default:
	      	  break;
	      	}
	    }
	}
      
      clearopt();
      sr = rawdec[mark].getsamplerate();
      ch = rawdec[mark].getch();
      sid = rawdec[mark].getsid();
      cid = rawdec[mark].getcid();
      pileup = rawdec[mark].getpileup();
      ts = rawdec[mark].getts();
      cfd = rawdec[mark].getcfd();
      cfdft = rawdec[mark].getcfdft();
      cfds = rawdec[mark].getcfds();
      evte = rawdec[mark].getevte();
      ltra = rawdec[mark].getltra();
      outofr = rawdec[mark].getoutofr();
      if(rawdec[mark].getesumflag())
	{
	  trae = rawdec[mark].gettrae();
	  leae = rawdec[mark].getleae();
	  gape = rawdec[mark].getgape();
	  // base = rawdec[mark].getbase();
	  base = IEEEFloating2Decimal(rawdec[mark].getbase());
	}
      if(rawdec[mark].getqsumflag())
	{
	  rawdec[mark].getqs(qs);
	}
      if(rawdec[mark].getetsflag())
	{
	  ets = rawdec[mark].getets();
	}
      if(rawdec[mark].gettraceflag())
	{
	  rawdec[mark].gettrace(data);
	  for(int i = 0;i < int(ltra);i++) dt[i] = i;
	}
      t->Fill();

      StatisticsEventCount[mark][ch]++;
      if(outofr) StatisticsOutOfRange[mark][ch]++;
      if(pileup) StatisticsPileup[mark][ch]++;
      if(cfdft) StatisticsCfdForcedTrigger[mark][ch]++;
      if(ltra > 0) StatisticsWaveformCount[mark][ch]++;
      if(evte == 0) StatisticsEnergy0[mark][ch]++;
      
      nevt++;
      if(nevt%10000 == 0)
	{
	  std::cout<<"\r"<<"nevt: "<<nevt;
	  std::cout << std::flush;
	}
      havedata[mark] = rawdec[mark].getnextevt(); 
      flagdata = false;
      for (int i = 0; i < flagfile; ++i)
	{
	  flagdata += havedata[i];
	}
      if(!flagdata) break;
    }

  std::cout<<std::endl;
  
  file->cd();
  t->Write();
  file->Close();

  
  std::ofstream writetxt;
  writetxt.open(TString::Format("%s_R%04d.txt",FileName.Data(), Run).Data());
  if(!writetxt.is_open())
    {
      std::cout<<"can't open text file."<<std::endl;
    }
  writetxt<<"Mod  Channel  OutOfRange  Pileup  CfdForcedTrigger  Energy->0  WaveformCount  TotalEvent"<<std::endl;

  for (int i = 0; i < flagfile; ++i)
    {
      writetxt<<std::endl;
      for (int j = 0; j < 16; ++j)
	{
	  writetxt<<i<<"  "<<j<<"  "<<StatisticsOutOfRange[i][j]<<"  "<<StatisticsPileup[i][j]<<"  "<<StatisticsCfdForcedTrigger[i][j]<<"  "<<StatisticsEnergy0[i][j]<<"  "<<StatisticsWaveformCount[i][j]<<"  "<<StatisticsEventCount[i][j]<<std::endl;
	}
    }
 
  writetxt.close();

  
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

void r2root::clearopt()
{
  sr = 0;
  ch = -1;
  sid = -1;
  cid = -1;
  pileup = 0;
  ts = 0;
  cfd = 0;
  cfdft = 0;
  cfds = 0;
  evte = 0;
  ltra = 0;
  outofr = 0;
  trae = 0;
  leae = 0;
  gape = 0;
  base = 0;
  ets = 0;
  memset(qs,0,sizeof(UInt_t)*8);
  memset(data,0,sizeof(UShort_t)*MAXTRACEN);
  memset(dt,0,sizeof(UShort_t)*MAXTRACEN);
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
