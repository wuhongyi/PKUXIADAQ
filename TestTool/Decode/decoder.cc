// decoder.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 10月  2 18:51:18 2016 (+0800)
// Last-Updated: 六 10月 19 17:00:17 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 35
// URL: http://wuhongyi.cn 

#include "decoder.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

decoder::decoder()
{
  clearall();
  fd = -1;
  samplerate = 0;
}

decoder::~decoder()
{

}

void decoder::clearall()
{
  ch = -1;
  sid = -1;
  cid = -1;
  lhead = 0;
  levt = 0;
  pileup = 0;

  ts = 0;
  cfd = 0;
  cfdft = false;
  cfds = 0;
  evte = 0;
  ltra = 0;
  outofr = 0;
  
  esumf = false;
  trae = 0;
  leae = 0;
  gape = 0;
  base = 0;
  
  qsumf = false;
  memset(qs,0,sizeof(unsigned int)*8);

  etsf = false;
  ets = 0;
  
  tracef = false;
  memset(data,0,sizeof(unsigned short)*MAXTRACEN);
}

bool decoder::openfile(const char* fileN)
{
  if((fd = open(fileN,O_RDONLY))<0)
    {
      printf("cannot open file %s\n",fileN);
      return false;
    }
  return true;
}

bool decoder::readword()
{
  if(fd < 0)
    {
      printf("Error, no opened file found!\n");
      return false;
    }
  size_t n = read(fd,&buff,4);
  if(n <= 0) return false;
  //  printf("read %d bytes!\n",n);
  //  memcpy(&tmpd,buff,sizeof(char)*4);
  return true;
}

void decoder::printword()
{
  printf("0x%08x\n",buff);
}

bool decoder::getnextword()
{
  return readword();
}

bool decoder::getnextevt()
{
  clearall();
  if(!getnextword()) return false; // get the end of this file
  return decode();
}

bool decoder::decode()
{
  // decode 1st word in this event
  ch = (buff & kMaskchannel) >> kShiftchannel;
  sid = (buff & kMasksid) >> kShiftsid;
  cid = (buff & kMaskcid) >> kShiftcid;
  lhead = (buff & kMasklhead) >> kShiftlhead;
  levt = (buff & kMasklevt) >> kShiftlevt;
  pileup = (buff & kMaskpileup) >> kShiftpileup;

  // decode 2nd word in this event
  if(!getnextword())
    {
      printf("error! 2nd word in header is missing!");
      return false;
    }
  ts = ( buff & kMasktslo ) >> kShifttslo;

  // decode 3rd
  if(!getnextword())
    {
      printf("error! 3rd word in header is missing!");
      return false;
    }
  ts = ts+(((long)( buff & kMasktshi ) >> kShifttshi)*0x100000000);// fix bug 20180526
  switch(int(samplerate))
    {
    case 100:
      cfd = ( buff & kMaskcfd100 ) >> kShiftcfd100; 
      cfdft = ( buff & kMaskcfdft100 ) >> kShiftcfdft100; 
      break;
    case 250:
      cfd = ( buff & kMaskcfd250 ) >> kShiftcfd250; 
      cfdft = ( buff & kMaskcfdft250 ) >> kShiftcfdft250;
      cfds = ( buff & kMaskcfds250 ) >> kShiftcfds250; 
      break;
    case 500:
      cfd = ( buff & kMaskcfd500 ) >> kShiftcfd500;
      cfds = ( buff & kMaskcfds500 ) >> kShiftcfds500;
      if(cfds == 7) cfdft = true;
      else cfdft = false;// fix bug 20191019
      break;
    default:
      printf("error! Set Sample Rate: %d\n",samplerate);
      break;
    }

  
  // decode 4th
  if(!getnextword())
    {
      printf("error! 4th word in header is missing!");
      return false;
    }

  evte = ( buff & kMaskevte ) >> kShiftevte;
  // if(evte > 65000) printf("%x\n",buff);
  ltra = ( buff & kMaskltra ) >> kShiftltra;
  outofr = ( buff & kMaskoutofr ) >> kShiftoutofr;

  // printf("ch: %d  lhead: %d\n",ch,lhead);
  switch(int(lhead))
    {
    case 4:
      esumf = false;
      qsumf = false;
      etsf = false;
      break;
    case 6:
      esumf = false;
      qsumf = false;
      etsf = true;
      break;
    case 8:
      esumf = true;
      qsumf = false;
      etsf = false;
      break;
    case 10:
      esumf = true;
      qsumf = false;
      etsf = true;
      break;
    case 12:
      esumf = false;
      qsumf = true;
      etsf = false;
      break;
    case 14:
      esumf = false;
      qsumf = true;
      etsf = true;
      break;
    case 16:
      esumf = true;
      qsumf = true;
      etsf = false;
      break;
    case 18:
      esumf = true;
      qsumf = true;
      etsf = true;
      break;
    default:
      printf("error! Header Length: %d\n",lhead);
      break;
    }
  
  if(ltra > 0) tracef = true;

  // decode esum
  if(esumf)
    {
      if(!getnextword())
	{
	  printf("error! 1st word in esum is missing!\n");
	  return false;
	}
      trae = ( buff & kMaskesum ) >> kShiftesum;
      
      if(!getnextword())
	{
	  printf("error! 2nd word in esum is missing!\n");
	  return false;
	}
      leae = ( buff & kMaskesum ) >> kShiftesum;
      
      if(!getnextword())
	{
	  printf("error! 3rd word in esum is missing!\n");
	  return false;
	}
      gape = ( buff & kMaskesum ) >> kShiftesum;
      
      if(!getnextword())
	{
	  printf("error! 4th word in esum is missing!\n");
	  return false;
	}
      base = ( buff & kMaskesum ) >> kShiftesum;
    }

  // decode qsum
  if(qsumf)
    {
      for(int i=0;i<8;i++)
	{
	  if(!getnextword())
	    {
	      printf("error! %dth word in qsum is missing!\n",i);
	      return false;
	    }
	  qs[i] = (buff & kMaskqs)>>kShiftqs;
	}
    }

  // decode ets
  if(etsf)
    {
      if(!getnextword())
	{
	  printf("error! 1st word in ets is missing!\n");
	  return false;
	}
      ets = ( buff & kMasketslo ) >> kShiftetslo;
      
      if(!getnextword())
	{
	  printf("error! 2nd word in ets is missing!\n");
	  return false;
	}
      ets = ets+(((long)( buff & kMasketshi ) >> kShiftetshi)*0x100000000);// fix bug 20180526
    }
  
  // decode trac.
  if(tracef)
    {
      if(ltra > MAXTRACEN)
	{
	  printf("error, the array to store the trace is not big enough\n");
	  return false;
	  // ltra = (levt-lhead)*2;
	}
      for(int i = 0;i < ltra/2;i++)
	{
	  if(!getnextword())
	    {
	      printf("error! %dth word in trace is missing!\n",i);
	      return false;
	    }
	  data[i*2] = (buff&kMaskdata1) >> kShiftdata1;
	  data[i*2+1] = (buff&kMaskdata2) >> kShiftdata2;
	}
    }
  return true;
}

// 
// decoder.cc ends here
