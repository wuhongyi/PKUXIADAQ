// decoding.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 9月 11 16:57:53 2020 (+0800)
// Last-Updated: 五 9月 11 17:41:27 2020 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 4
// URL: http://wuhongyi.cn 

#include "decoding.hh"
#include <iostream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

decoding::decoding()
{

  statemachine = 0;
}

decoding::~decoding()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void decoding::init()
{
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

      tmplength = levt-4;
      statemachine = 1;
      break;
      
    case 1:
      // decode 2nd word in this event
      // ts = ( buff & kMasktslo ) >> kShifttslo;

      statemachine = 2;
      break;

    case 2:
      // decode 3rd
      // ts = ts+(((long)( buff & kMasktshi ) >> kShifttshi)*0x100000000);
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
      flag = true;
      break;
      
    default:
      tmplength--;
      if(tmplength==0) statemachine = 0;
      break;      
    }

  return flag;
}

// 
// decoding.cc ends here
