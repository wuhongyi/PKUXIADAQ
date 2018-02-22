// OfflineData.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 2月 22 09:08:20 2018 (+0800)
// Last-Updated: 四 2月 22 11:41:51 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 7
// URL: http://wuhongyi.cn 

// 重新定义数据结构，4headr+1location
// 扩展支持 2+4+8数据（在数据初始化位置可选）  TODO

//ch (OfflineEventInformation[EventHeaderLength*i] & 0xF)
//slot ((OfflineEventInformation[EventHeaderLength*i] & 0xF0) >> 4)
//crate ((OfflineEventInformation[EventHeaderLength*i] & 0xF00) >> 8)
// Header length ((OfflineEventInformation[EventHeaderLength*i] & 0x1F000) >> 12)
// Event length((OfflineEventInformation[EventHeaderLength*i] & 0x7FFE0000) >> 17)
// Finish code ((OfflineEventInformation[EventHeaderLength*i] & 0x80000000) >> 31)

// EventTime_Low  OfflineEventInformation[EventHeaderLength*i+1]

// EventTime_High  (OfflineEventInformation[EventHeaderLength*i+2] & 0xFFFF)
//cfd ((OfflineEventInformation[EventHeaderLength*i+2] & 0x7FFF0000)>>16)
//cfd forced trigger bit ((OfflineEventInformation[EventHeaderLength*i+2] & 0x80000000) >> 31)

//trace length ((OfflineEventInformation[EventHeaderLength*i+3] & 0x7FFF0000)>>16)
//event energy (OfflineEventInformation[EventHeaderLength*i+3] & 0xFFFF)
//trace out-of-range flag ((OfflineEventInformation[EventHeaderLength*i+3] & 0x80000000) >> 31)

//trailing energy sum
//leading energy sum
//gap energy sum
//baseline value

//qdc sum 0
//qdc sum 1
//qdc sum 2
//qdc sum 3
//qdc sum 4
//qdc sum 5
//qdc sum 6
//qdc sum 7

//exttime_lo
//exttime_hi


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

  
  
  // Test
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
};

#endif /* _OFFLINEDATA_H_ */
// 
// OfflineData.hh ends here
