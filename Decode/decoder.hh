// decoder.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 10月  2 18:51:06 2016 (+0800)
// Last-Updated: 六 11月  5 09:17:34 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 4
// URL: http://wuhongyi.cn 

#ifndef _DECODER_H_
#define _DECODER_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h> 
#include <string.h>
#include <stdio.h>

#define MAXTRACEN 10000 // maximum number of trace points
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class decoder
{
public:
  decoder();
  virtual ~decoder();

  void clearall();
  bool openfile(const char*);

  void printword(); // for debug purpose
  bool getnextword(); // for debug purpose

  bool getnextevt();

  inline int getch() {return ch;}
  inline int getsid() {return sid;}
  inline int getcid() {return cid;}
  inline int getlhead() {return lhead;}
  inline int getlevt() {return levt;}
  inline bool getpileup() {return pileup;}

  inline unsigned long getts() {return ts;}
  inline int getcfd() {return cfd;}
  inline int getevte() {return evte;}
  inline int getltra() {return ltra;}

  inline bool getesumflag() {return esumf;}
  inline unsigned int gettrae() {return trae;}
  inline unsigned int getleae() {return leae;}
  inline unsigned int getgape() {return gape;}
  inline unsigned int getbase() {return base;}

  inline bool getqsumflag() {return qsumf;}
  inline void getqs(unsigned int *qqs) {memcpy(qqs,qs,sizeof(unsigned int)*8);}
  inline bool gettraceflag() {return tracef;}
  inline void gettrace(int *da) {memcpy(da,data,sizeof(int)*ltra);}

private:
  bool readword();
  bool decode();

  int fd; // File descripter
  unsigned int buff; // buffer to read 4 buytes from file(32bit)
	
  //Decoded values
  //_____________________________________________
	
  int ch;  		// channel number
  int sid; 		// slot id
  int cid; 		// crate id
  int lhead; 		// header length
  int levt; 		// event length
  bool pileup; 		// 0-->good event 1-->pileup event

  unsigned long ts; 	// timestamp of this event
  int cfd; 		// CFD fractional time
  int evte;  		// Event enrgy
  int ltra;		// Trace length
	
  bool esumf;
  unsigned int trae;	// trailing energy sum
  unsigned int leae;	// leading energy sum
  unsigned int gape;	// Gap energy sum
  unsigned int base;	// baseline value

  bool qsumf;
  unsigned int qs[8];	// qdc sum #0-#7

  // pulse shape (trace enabled)
  bool tracef;
  int  data[MAXTRACEN];

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
  const static unsigned int kMaskcfd = 		0xffff0000;
  const static unsigned int kShiftcfd =		16;
  const static unsigned int kMaskevte =         0x0000ffff;
  const static unsigned int kShiftevte =        0;
  const static unsigned int kMaskltra =		0xffff0000;
  const static unsigned int kShiftltra =        16;

  const static unsigned int kMaskesum =         0xffffffff;
  const static unsigned int kShiftesum =        0;
  const static unsigned int kMaskqs = 		0xffffffff;
  const static unsigned int kShiftqs = 		0;

  const static unsigned int kMaskdata1 =       	0x0000ffff;
  const static unsigned int kShiftdata1 = 	0;
  const static unsigned int kMaskdata2 =        0xffff0000;
  const static unsigned int kShiftdata2 = 	16;

};

#endif /* _DECODER_H_ */
// 
// decoder.hh ends here
