// OfflineData.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 2月 22 09:08:20 2018 (+0800)
// Last-Updated: 四 2月 22 15:13:10 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 10
// URL: http://wuhongyi.cn 

#ifndef _OFFLINEDATA_H_
#define _OFFLINEDATA_H_

#include "TString.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EventData
{
public:
  EventData();
  virtual ~EventData();


public:
  //DATA
  unsigned int WaveLocation;
  unsigned int Header[4];

  unsigned int *SUMS4;//sums/baseline
  unsigned int *QDC8;//qdc
  unsigned int *EXTTIME2;
};


class OfflineData
{
public:
  OfflineData();
  virtual ~OfflineData();

  void ReadModuleEvents();
  void ReadEventsInfo();

  
  // Set
  void SetFileName(const char *filename) {fFileName = filename;}

  inline void SetSamplingRate(short sr) {SamplingRate = sr;}
  inline void FlagRawEnergySumsAndBaseline(bool flag) {flagrawenergysumsandbaseline = flag;}
  inline void FlagQDCs(bool flag) {flagqdcsums = flag;}
  inline void FlagExternalTimestamp(bool flag) {flagexternaltimestamp = flag;}
  
  // Get
  inline unsigned int GetOfflineModuleEventsCount() {return OfflineModuleEventsCount;}

  
  inline unsigned int GetEventWaveLocation(unsigned int event) {return OfflineEventInformation[event].WaveLocation;}
  unsigned int GetEventChannel(unsigned int event);
  unsigned int GetEventSlot(unsigned int event);
  unsigned int GetEventCrate(unsigned int event);
  unsigned int GetEventHeaderLength(unsigned int event);
  unsigned int GetEventLength(unsigned int event);
  unsigned int GetEventFinishCode(unsigned int event);
  unsigned int GetEventTime_Low(unsigned int event);
  unsigned int GetEventTime_High(unsigned int event);
  unsigned int GetEventCfd(unsigned int event);
  unsigned int GetEventCfdForcedTriggerBit(unsigned int event);
  unsigned int GetEventCfdTriggerSourceBits(unsigned int event);
  unsigned int GetEventTraceLength(unsigned int event);
  unsigned int GetEventEnergy(unsigned int event);
  unsigned int GetEventTraceOutOfRangeFlag(unsigned int event);

  
  inline unsigned int GetEventHeader(unsigned int event,int n) {return OfflineEventInformation[event].Header[n];}
  inline unsigned int GetEventSUMS4(unsigned int event,int n) {return OfflineEventInformation[event].SUMS4[n];}
  inline unsigned int GetEventQDC8(unsigned int event,int n) {return OfflineEventInformation[event].QDC8[n];}
  inline unsigned int GetEventEXTTIME2(unsigned int event,int n) {return OfflineEventInformation[event].EXTTIME2[n];}

  
private:
  unsigned int GetModuleEvents(const char *FileName);//return events
  

private:
  // int EventHeaderLength;
  
  EventData *OfflineEventInformation;
  unsigned int OfflineModuleEventsCount;


  bool flagrawenergysumsandbaseline;
  bool flagqdcsums;
  bool flagexternaltimestamp;

  short SamplingRate;

  TString fFileName;

private:
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

  const static unsigned int kMasketslo =         0xffffffff;
  const static unsigned int kShiftetslo =        0;
  const static unsigned int kMasketshi =         0x0000ffff;
  const static unsigned int kShiftetshi =        0;
  
};

#endif /* _OFFLINEDATA_H_ */
// 
// OfflineData.hh ends here
