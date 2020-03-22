// decoder.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 10月  2 18:51:06 2016 (+0800)
// Last-Updated: 五 3月 13 17:17:17 2020 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 22
// URL: http://wuhongyi.cn 

// 20170825 add external clock timestamp 



#ifndef _DECODER_H_
#define _DECODER_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h> 
#include <string.h>
#include <stdio.h>

#define MAXBOARD  24  //预设最大24个
#define MAXTRACEN 100000 // maximum number of trace points
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class decoder
{
public:
  decoder();
  virtual ~decoder();

  inline void setsamplerate(short sr) {samplerate = sr;}//MHz
  inline short getsamplerate() {return samplerate;}
  
  void clearall();
  bool openfile(const char*);

  void printword(); // for debug purpose
  bool getnextword(); // for debug purpose

  bool getnextevt();

  inline short getch() {return ch;}
  inline short getsid() {return sid;}
  inline short getcid() {return cid;}
  inline bool getpileup() {return pileup;}

  inline long getts() {return ts;}
  inline short getcfd() {return cfd;}
  inline bool getcfdft() {return cfdft;}
  inline short getcfds() {return cfds;}
  inline unsigned short getevte() {return evte;}
  inline bool getoutofr() {return outofr;}
  

private:
  bool readword();
  bool decode();

  short samplerate;
  
  int fd; // File descripter
  unsigned int buff[32768]; // buffer to read 4 buytes from file(32bit)
  size_t n;
  
  //Decoded values
  //_____________________________________________
	
  short ch;  		// channel number
  short sid; 		// slot id
  short cid; 		// crate id
  unsigned short lhead; // header length
  unsigned short levt;  // event length
  bool pileup; 		// 0-->good event 1-->pileup event

  long ts; 	        // timestamp of this event
  short cfd;            // CFD fractional time
  bool cfdft;           //CFD forced trigger bit
  short cfds;           //CFD trigger source bits
  
  unsigned short evte;  // Event enrgy
  unsigned short ltra;  // Trace length
  bool outofr;          //Trace Out-of-Range Flag
  


  // const parameters for decoder
  const static unsigned int kMaskchannel =	0x0000000f;
  const static unsigned int kShiftchannel =	0x0;
  const static unsigned int kMasksid =		0x000000f0;
  const static unsigned int kShiftsid =		4;
  const static unsigned int kMaskcid =		0x00000f00;
  const static unsigned int kShiftcid =		8;
  const static unsigned int kMasklhead =        0x0001f000;
  const static unsigned int kShiftlhead = 	12;
  const static unsigned int kMasklevt =         0x7ffe0000;
  const static unsigned int kShiftlevt =        17;
  const static unsigned int kMaskpileup =       0x80000000;
  const static unsigned int kShiftpileup = 	31;

  const static unsigned int kMasktslo =         0xffffffff;
  const static unsigned int kShifttslo =        0;
  const static unsigned int kMasktshi =         0x0000ffff;
  const static unsigned int kShifttshi =        0;
  const static unsigned int kMaskcfd100 =       0x7fff0000;
  const static unsigned int kShiftcfd100 =      16;
  const static unsigned int kMaskcfdft100 =     0x80000000;
  const static unsigned int kShiftcfdft100 =    31;
  const static unsigned int kMaskcfd250 =       0x3fff0000;
  const static unsigned int kShiftcfd250 =      16;
  const static unsigned int kMaskcfdft250 =     0x80000000;
  const static unsigned int kShiftcfdft250 =    31;
  const static unsigned int kMaskcfds250 =      0x40000000;
  const static unsigned int kShiftcfds250 =     30;
  const static unsigned int kMaskcfd500 =       0x1fff0000;
  const static unsigned int kShiftcfd500 =      16;
  const static unsigned int kMaskcfds500 =      0xE0000000;
  const static unsigned int kShiftcfds500 =     29;
  
  const static unsigned int kMaskevte =         0x0000ffff;
  const static unsigned int kShiftevte =        0;
  const static unsigned int kMaskltra =		0x7fff0000;
  const static unsigned int kShiftltra =        16;
  const static unsigned int kMaskoutofr =       0x80000000;
  const static unsigned int kShiftoutofr =      31;
  
  const static unsigned int kMaskesum =         0xffffffff;
  const static unsigned int kShiftesum =        0;
  const static unsigned int kMaskqs = 		0xffffffff;
  const static unsigned int kShiftqs = 		0;

  const static unsigned int kMasketslo =         0xffffffff;
  const static unsigned int kShiftetslo =        0;
  const static unsigned int kMasketshi =         0x0000ffff;
  const static unsigned int kShiftetshi =        0;

  const static unsigned int kMaskdata1 =       	0x0000ffff;
  const static unsigned int kShiftdata1 = 	0;
  const static unsigned int kMaskdata2 =        0xffff0000;
  const static unsigned int kShiftdata2 = 	16;

};

#endif /* _DECODER_H_ */
// 
// decoder.hh ends here
