// decoding.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 9月 11 16:57:53 2020 (+0800)
// Last-Updated: 三 8月 16 14:57:15 2023 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 14
// URL: http://wuhongyi.cn 

#include "decoding.hh"
#include "UserDefine.hh"
#include <iostream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

decoding::decoding()
{
  qsumf = false;
  
  statemachine = 0;
}

decoding::~decoding()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void decoding::init()
{
  qsumf = false;
  statemachine = 0;
}

bool decoding::readword(unsigned int buff)
{
  bool flag = false;

  switch(statemachine)
    {
    case 0:
      // decode 1st word in this event
      ch = (buff & kMaskchannel) >> kShiftchannel;
      // sid = (buff & kMasksid) >> kShiftsid;
      // cid = (buff & kMaskcid) >> kShiftcid;
      lhead = (buff & kMasklhead) >> kShiftlhead;
      levt = (buff & kMasklevt) >> kShiftlevt;
      pileup = (buff & kMaskpileup) >> kShiftpileup;

      if(lhead == 12 || lhead == 14 || lhead == 16 || lhead == 18) qsumf = true;
      tmplength = levt-4;
      statemachine = 1;
      break;
      
    case 1:
      // decode 2nd word in this event
#ifdef CLEAR_IN_NEW_RUN
#ifdef SHOW_OUTPUT_RATE       
      ts = ( buff & kMasktslo ) >> kShifttslo;
#endif
#endif
      statemachine = 2;
      break;

    case 2:
      // decode 3rd
#ifdef CLEAR_IN_NEW_RUN
#ifdef SHOW_OUTPUT_RATE        
      ts = ts+(((long)( buff & kMasktshi ) >> kShifttshi)*0x100000000);
#endif
#endif
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
	  else cfdft = false;
	  break;
	default:
	  std::cout<<"error! Set Sample Rate: "<<samplerate<<std::endl;
	  break;
	}

      statemachine = 3;
      break;
	
    case 3:
      // decode 4th
      evte = ( buff & kMaskevte ) >> kShiftevte;
      ltra = ( buff & kMaskltra ) >> kShiftltra;
      outofr = ( buff & kMaskoutofr ) >> kShiftoutofr;

      if(tmplength==0) statemachine = 0;
      else statemachine = 4;
      break;
      
    default:
      if(lhead == 12 || lhead == 14)
	{
	  if(statemachine >= 4 && statemachine <= 11)
	    {
	      qs[statemachine-4] = buff;
	    }
	}
      if(lhead == 16 || lhead == 18)
	{
	  if(statemachine >= 8 && statemachine <= 15)
	    {
	      qs[statemachine-8] = buff;
	    }
	}
      statemachine++;
      tmplength--;
      if(tmplength==0) statemachine = 0;
      break;      
    }

  if(statemachine == 0) flag = true;
  return flag;
}

// 
// decoding.cc ends here








