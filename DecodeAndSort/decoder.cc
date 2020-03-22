// decoder.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 10月  2 18:51:18 2016 (+0800)
// Last-Updated: 五 3月 13 17:17:07 2020 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 40
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
  printf("0x%08x\n",buff[0]);
}

bool decoder::getnextword()
{
  return readword();
}

bool decoder::getnextevt()
{
  clearall();
  return decode();
}

bool decoder::decode()
{
  if(fd < 0)
    {
      printf("Error, no opened file found!\n");
      return false;
    }
  n = read(fd,&buff,16);
  if(n <= 0) return false;
  
  // decode 1st word in this event
  ch = (buff[0] & kMaskchannel) >> kShiftchannel;
  sid = (buff[0] & kMasksid) >> kShiftsid;
  cid = (buff[0] & kMaskcid) >> kShiftcid;
  lhead = (buff[0] & kMasklhead) >> kShiftlhead;
  levt = (buff[0] & kMasklevt) >> kShiftlevt;
  pileup = (buff[0] & kMaskpileup) >> kShiftpileup;

  // decode 2nd word in this event
  ts = ( buff[1] & kMasktslo ) >> kShifttslo;

  // decode 3rd
  ts = ts+(((long)( buff[2] & kMasktshi ) >> kShifttshi)*0x100000000);// fix bug 20180526
  switch(int(samplerate))
    {
    case 100:
      cfd = ( buff[2] & kMaskcfd100 ) >> kShiftcfd100; 
      cfdft = ( buff[2] & kMaskcfdft100 ) >> kShiftcfdft100; 
      break;
    case 250:
      cfd = ( buff[2] & kMaskcfd250 ) >> kShiftcfd250; 
      cfdft = ( buff[2] & kMaskcfdft250 ) >> kShiftcfdft250;
      cfds = ( buff[2] & kMaskcfds250 ) >> kShiftcfds250; 
      break;
    case 500:
      cfd = ( buff[2] & kMaskcfd500 ) >> kShiftcfd500;
      cfds = ( buff[2] & kMaskcfds500 ) >> kShiftcfds500;
      if(cfds == 7) cfdft = true;
      else cfdft = false;// fix bug 20191019
      break;
    default:
      printf("error! Set Sample Rate: %d\n",samplerate);
      break;
    }

  
  // decode 4th
  evte = ( buff[3] & kMaskevte ) >> kShiftevte;
  ltra = ( buff[3] & kMaskltra ) >> kShiftltra;
  outofr = ( buff[3] & kMaskoutofr ) >> kShiftoutofr;

  // printf("ch: %d  lhead: %d\n",ch,lhead);
  switch(int(lhead))
    {
    case 4:
      break;
    case 6:
      n = read(fd,&buff,8);
      if(n <= 0) return false;
      break;
    case 8:
      n = read(fd,&buff,16);
      if(n <= 0) return false;
      break;
    case 10:
      n = read(fd,&buff,24);
      if(n <= 0) return false;
      break;
    case 12:
      n = read(fd,&buff,32);
      if(n <= 0) return false;
      break;
    case 14:
      n = read(fd,&buff,40);
      if(n <= 0) return false;
      break;
    case 16:
      n = read(fd,&buff,48);
      if(n <= 0) return false;
      break;
    case 18:
      n = read(fd,&buff,56);
      if(n <= 0) return false;
      break;
    default:
      printf("error! Header Length: %d\n",lhead);
      break;
    }
  
  // decode trac.
  if(ltra)
    {
      if(ltra > MAXTRACEN)
	{
	  printf("error, the array to store the trace is not big enough\n");
	  return false;
	  // ltra = (levt-lhead)*2;
	}
      n = read(fd,&buff,ltra/2*4);
      if(n <= 0) return false;
    }
  return true;
}

// 
// decoder.cc ends here
