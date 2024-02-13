// OfflineData.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 2月 22 09:13:30 2018 (+0800)
// Last-Updated: 六 10月 19 16:54:55 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 15
// URL: http://wuhongyi.cn 

#include "OfflineData.hh"

#include <iostream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventData::EventData()
{
  SUMS4 = NULL;
  QDC8 = NULL;
  EXTTIME2 = NULL;
}

EventData::~EventData()
{
  if(SUMS4 != NULL)
    {
      delete SUMS4;
      SUMS4 = NULL;
    }

  if(QDC8 != NULL)
    {
      delete QDC8;
      QDC8 = NULL;
    }

  if(EXTTIME2 != NULL)
    {
      delete EXTTIME2;
      EXTTIME2 = NULL;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OfflineData::OfflineData()
{
  SamplingRate = 0;
  flagrawenergysumsandbaseline = false;
  flagqdcsums = false;
  flagexternaltimestamp = false;
  
  OfflineEventInformation = NULL;
  OfflineModuleEventsCount = 0;
}

OfflineData::~OfflineData()
{
  // if(OfflineEventInformation != NULL)
  //   {
  //     delete []OfflineEventInformation;
  //   }

}


void OfflineData::ReadModuleEvents()
{
  OfflineModuleEventsCount = GetModuleEvents(fFileName.Data());
}

void OfflineData::ReadEventsInfo()
{
  if(OfflineModuleEventsCount > 0)
    {
      OfflineEventInformation = new EventData[OfflineModuleEventsCount];
      memset(OfflineEventInformation, 0, sizeof(EventData)*OfflineModuleEventsCount);//置零

      unsigned int eventdata, headerlength, eventlength;
      // unsigned int tracelength;
      unsigned int  NumEvents;
      long TotalWords, TotalSkippedWords;
      FILE *ListModeFile = NULL;

      // Open the list mode file
      ListModeFile = fopen(fFileName.Data(), "rb");
      if(ListModeFile != NULL)
	{	
	  // Get file length
	  fseek(ListModeFile, 0, SEEK_END);
	  TotalWords = (ftell(ListModeFile) + 1) / 4;
	  // Point ListModeFile to the beginning of file
	  fseek(ListModeFile, 0, SEEK_SET);
		
	  // Initialize indicator and counter
	  TotalSkippedWords = 0;
	  NumEvents = 0;
      
	  if(!flagrawenergysumsandbaseline && !flagqdcsums && !flagexternaltimestamp)
	    {
	      do
		{
		  fread(&eventdata, 4, 1, ListModeFile);
		  OfflineEventInformation[NumEvents].Header[0] = eventdata;

		  headerlength = (eventdata & 0x1F000) >> 12;// Header length
		  eventlength = (eventdata & 0x7FFE0000) >> 17;// Event length
	      
		  fread(&eventdata, 4, 1, ListModeFile);
		  OfflineEventInformation[NumEvents].Header[1] = eventdata;

		  fread(&eventdata, 4, 1, ListModeFile);
		  OfflineEventInformation[NumEvents].Header[2] = eventdata;

		  fread(&eventdata, 4, 1, ListModeFile);
		  OfflineEventInformation[NumEvents].Header[3] = eventdata;
		  // tracelength = (eventdata & 0x7FFF0000) >> 16;// Trace Length
	  
		  OfflineEventInformation[NumEvents].WaveLocation = TotalSkippedWords+headerlength;// Trace location

		  // if(eventlength != headerlength + tracelength/2)
		  //   std::cout<<"Data error..."<<std::endl;
	  
		  TotalSkippedWords += eventlength;
		  NumEvents++;
		  fseek(ListModeFile, (eventlength-4)*4, SEEK_CUR);
		}while(TotalSkippedWords < TotalWords);
	    }
	  else
	    {
	      
	      unsigned int eventdatarawenergysumsandbaseline[4];
	      unsigned int eventdataqdcsums[8];
	      unsigned int eventdataexternaltimestamp[2];
	      // int offsetheaderrawenergysumsandbaseline = BaseEventHeaderLength;
	      // int offsetheaderqdcsums = BaseEventHeaderLength+4*int(headerrawenergysumsandbaseline->IsOn());
	      // int offsetheaderexternaltimestamp = BaseEventHeaderLength+4*int(headerrawenergysumsandbaseline->IsOn())+8*int(headerqdcsums->IsOn());
	      do
		{
		  fread(&eventdata, 4, 1, ListModeFile);
		  OfflineEventInformation[NumEvents].Header[0] = eventdata;
		  headerlength = (eventdata & 0x1F000) >> 12;// Header length
		  eventlength = (eventdata & 0x7FFE0000) >> 17;// Event length
	      
		  fread(&eventdata, 4, 1, ListModeFile);
		  OfflineEventInformation[NumEvents].Header[1] = eventdata;

		  fread(&eventdata, 4, 1, ListModeFile);
		  OfflineEventInformation[NumEvents].Header[2] = eventdata;

		  fread(&eventdata, 4, 1, ListModeFile);
		  OfflineEventInformation[NumEvents].Header[3] = eventdata;
	  
		  OfflineEventInformation[NumEvents].WaveLocation = TotalSkippedWords+headerlength;// Trace location

		  switch(int(headerlength))
		    {
		    case 4:
		      break;
		    case 6:
		      fread(&eventdataexternaltimestamp, 4, 2, ListModeFile);
		      if(flagexternaltimestamp)
			{
			  OfflineEventInformation[NumEvents].EXTTIME2 = new unsigned int[2];
			  OfflineEventInformation[NumEvents].EXTTIME2[0] = eventdataexternaltimestamp[0];
			  OfflineEventInformation[NumEvents].EXTTIME2[1] = eventdataexternaltimestamp[1];
			}
		      break;
		    case 8:
		      fread(&eventdatarawenergysumsandbaseline, 4, 4, ListModeFile);
		      if(flagrawenergysumsandbaseline)
			{
			  OfflineEventInformation[NumEvents].SUMS4 = new unsigned int[4];
			  OfflineEventInformation[NumEvents].SUMS4[0] = eventdatarawenergysumsandbaseline[0];
			  OfflineEventInformation[NumEvents].SUMS4[1] = eventdatarawenergysumsandbaseline[1];
			  OfflineEventInformation[NumEvents].SUMS4[2] = eventdatarawenergysumsandbaseline[2];
			  OfflineEventInformation[NumEvents].SUMS4[3] = eventdatarawenergysumsandbaseline[3];
			}
		      break;
		    case 10:
		      fread(&eventdatarawenergysumsandbaseline, 4, 4, ListModeFile);
		      if(flagrawenergysumsandbaseline)
			{ 
			  OfflineEventInformation[NumEvents].SUMS4 = new unsigned int[4];
			  OfflineEventInformation[NumEvents].SUMS4[0] = eventdatarawenergysumsandbaseline[0];
			  OfflineEventInformation[NumEvents].SUMS4[1] = eventdatarawenergysumsandbaseline[1];
			  OfflineEventInformation[NumEvents].SUMS4[2] = eventdatarawenergysumsandbaseline[2];
			  OfflineEventInformation[NumEvents].SUMS4[3] = eventdatarawenergysumsandbaseline[3];
			}

		      fread(&eventdataexternaltimestamp, 4, 2, ListModeFile);
		      if(flagexternaltimestamp)
			{
			  OfflineEventInformation[NumEvents].EXTTIME2 = new unsigned int[2];
			  OfflineEventInformation[NumEvents].EXTTIME2[0] = eventdataexternaltimestamp[0];
			  OfflineEventInformation[NumEvents].EXTTIME2[1] = eventdataexternaltimestamp[1];
			}		  
		      break;
		    case 12:
		      fread(&eventdataqdcsums, 4, 8, ListModeFile);
		      if(flagqdcsums)
			{
			  OfflineEventInformation[NumEvents].QDC8 = new unsigned int[8];
			  OfflineEventInformation[NumEvents].QDC8[0] = eventdataqdcsums[0];
			  OfflineEventInformation[NumEvents].QDC8[1] = eventdataqdcsums[1];
			  OfflineEventInformation[NumEvents].QDC8[2] = eventdataqdcsums[2];
			  OfflineEventInformation[NumEvents].QDC8[3] = eventdataqdcsums[3];
			  OfflineEventInformation[NumEvents].QDC8[4] = eventdataqdcsums[4];
			  OfflineEventInformation[NumEvents].QDC8[5] = eventdataqdcsums[5];
			  OfflineEventInformation[NumEvents].QDC8[6] = eventdataqdcsums[6];
			  OfflineEventInformation[NumEvents].QDC8[7] = eventdataqdcsums[7];
			}
		      break;
		    case 14:
		      fread(&eventdataqdcsums, 4, 8, ListModeFile);
		      if(flagqdcsums)
			{
			  OfflineEventInformation[NumEvents].QDC8 = new unsigned int[8];
			  OfflineEventInformation[NumEvents].QDC8[0] = eventdataqdcsums[0];
			  OfflineEventInformation[NumEvents].QDC8[1] = eventdataqdcsums[1];
			  OfflineEventInformation[NumEvents].QDC8[2] = eventdataqdcsums[2];
			  OfflineEventInformation[NumEvents].QDC8[3] = eventdataqdcsums[3];
			  OfflineEventInformation[NumEvents].QDC8[4] = eventdataqdcsums[4];
			  OfflineEventInformation[NumEvents].QDC8[5] = eventdataqdcsums[5];
			  OfflineEventInformation[NumEvents].QDC8[6] = eventdataqdcsums[6];
			  OfflineEventInformation[NumEvents].QDC8[7] = eventdataqdcsums[7];
			}

		      fread(&eventdataexternaltimestamp, 4, 2, ListModeFile);
		      if(flagexternaltimestamp)
			{
			  OfflineEventInformation[NumEvents].EXTTIME2 = new unsigned int[2];
			  OfflineEventInformation[NumEvents].EXTTIME2[0] = eventdataexternaltimestamp[0];
			  OfflineEventInformation[NumEvents].EXTTIME2[1] = eventdataexternaltimestamp[1];
			}
		      break;
		    case 16:
		      fread(&eventdatarawenergysumsandbaseline, 4, 4, ListModeFile);
		      if(flagrawenergysumsandbaseline)
			{ 
			  OfflineEventInformation[NumEvents].SUMS4 = new unsigned int[4];
			  OfflineEventInformation[NumEvents].SUMS4[0] = eventdatarawenergysumsandbaseline[0];
			  OfflineEventInformation[NumEvents].SUMS4[1] = eventdatarawenergysumsandbaseline[1];
			  OfflineEventInformation[NumEvents].SUMS4[2] = eventdatarawenergysumsandbaseline[2];
			  OfflineEventInformation[NumEvents].SUMS4[3] = eventdatarawenergysumsandbaseline[3];
			}

		      fread(&eventdataqdcsums, 4, 8, ListModeFile);
		      if(flagqdcsums)
			{
			  OfflineEventInformation[NumEvents].QDC8 = new unsigned int[8];
			  OfflineEventInformation[NumEvents].QDC8[0] = eventdataqdcsums[0];
			  OfflineEventInformation[NumEvents].QDC8[1] = eventdataqdcsums[1];
			  OfflineEventInformation[NumEvents].QDC8[2] = eventdataqdcsums[2];
			  OfflineEventInformation[NumEvents].QDC8[3] = eventdataqdcsums[3];
			  OfflineEventInformation[NumEvents].QDC8[4] = eventdataqdcsums[4];
			  OfflineEventInformation[NumEvents].QDC8[5] = eventdataqdcsums[5];
			  OfflineEventInformation[NumEvents].QDC8[6] = eventdataqdcsums[6];
			  OfflineEventInformation[NumEvents].QDC8[7] = eventdataqdcsums[7];
			}
		      break;
		    case 18:
		      fread(&eventdatarawenergysumsandbaseline, 4, 4, ListModeFile);
		      if(flagrawenergysumsandbaseline)
			{ 
			  OfflineEventInformation[NumEvents].SUMS4 = new unsigned int[4];
			  OfflineEventInformation[NumEvents].SUMS4[0] = eventdatarawenergysumsandbaseline[0];
			  OfflineEventInformation[NumEvents].SUMS4[1] = eventdatarawenergysumsandbaseline[1];
			  OfflineEventInformation[NumEvents].SUMS4[2] = eventdatarawenergysumsandbaseline[2];
			  OfflineEventInformation[NumEvents].SUMS4[3] = eventdatarawenergysumsandbaseline[3];
			}

		      fread(&eventdataqdcsums, 4, 8, ListModeFile);
		      if(flagqdcsums)
			{
			  OfflineEventInformation[NumEvents].QDC8 = new unsigned int[8];
			  OfflineEventInformation[NumEvents].QDC8[0] = eventdataqdcsums[0];
			  OfflineEventInformation[NumEvents].QDC8[1] = eventdataqdcsums[1];
			  OfflineEventInformation[NumEvents].QDC8[2] = eventdataqdcsums[2];
			  OfflineEventInformation[NumEvents].QDC8[3] = eventdataqdcsums[3];
			  OfflineEventInformation[NumEvents].QDC8[4] = eventdataqdcsums[4];
			  OfflineEventInformation[NumEvents].QDC8[5] = eventdataqdcsums[5];
			  OfflineEventInformation[NumEvents].QDC8[6] = eventdataqdcsums[6];
			  OfflineEventInformation[NumEvents].QDC8[7] = eventdataqdcsums[7];
			}
		  
		      fread(&eventdataexternaltimestamp, 4, 2, ListModeFile);
		      if(flagexternaltimestamp)
			{
			  OfflineEventInformation[NumEvents].EXTTIME2 = new unsigned int[2];
			  OfflineEventInformation[NumEvents].EXTTIME2[0] = eventdataexternaltimestamp[0];
			  OfflineEventInformation[NumEvents].EXTTIME2[1] = eventdataexternaltimestamp[1];
			}
		      break;
		    default:
		      std::cout<<"Header Length Error!"<<std::endl;
		      break;
		    }
		  TotalSkippedWords += eventlength;
		  NumEvents++;
		  fseek(ListModeFile, (eventlength-headerlength)*4, SEEK_CUR);
		}while(TotalSkippedWords < TotalWords);

	    }
	  fclose(ListModeFile);
	}
      
    }
}




unsigned int OfflineData::GetModuleEvents(const char *FileName)
{
  int ModuleEvents = 0;
  unsigned int eventdata, eventlength;
  long TotalWords, TotalSkippedWords;
  FILE *ListModeFile = NULL;

  // Open the list mode file
  ListModeFile = fopen(FileName, "rb");
  if(ListModeFile != NULL)
    {	
      // Get file length
      fseek(ListModeFile, 0, SEEK_END);
      TotalWords = (ftell(ListModeFile) + 1) / 4;
      
      // Point ListModeFile to the beginning of file
      fseek(ListModeFile, 0, SEEK_SET);
		
      // Initialize indicator and counter
      TotalSkippedWords = 0;
		
      do
	{
	  fread(&eventdata, 4, 1, ListModeFile);
	  eventlength = (eventdata & 0x7FFE0000) >> 17;
	  TotalSkippedWords += eventlength;
	  fseek(ListModeFile, (eventlength-1)*4, SEEK_CUR);
	  ModuleEvents ++;
	}while( TotalSkippedWords < TotalWords );
		
      fclose(ListModeFile);
    }
  
  if(TotalWords == 0) ModuleEvents = 0;
  return ModuleEvents;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

unsigned int OfflineData::GetEventChannel(unsigned int event)
{
  return (OfflineEventInformation[event].Header[0] & kMaskchannel) >> kShiftchannel;
}

unsigned int OfflineData::GetEventSlot(unsigned int event)
{
  return (OfflineEventInformation[event].Header[0] & kMasksid) >> kShiftsid;
}

unsigned int OfflineData::GetEventCrate(unsigned int event)
{
  return (OfflineEventInformation[event].Header[0] & kMaskcid) >> kShiftcid;
}

unsigned int OfflineData::GetEventHeaderLength(unsigned int event)
{
 return (OfflineEventInformation[event].Header[0] & kMasklhead) >> kShiftlhead;
}

unsigned int OfflineData::GetEventLength(unsigned int event)
{
  return (OfflineEventInformation[event].Header[0] & kMasklevt) >> kShiftlevt;
}

unsigned int OfflineData::GetEventFinishCode(unsigned int event)
{
  return (OfflineEventInformation[event].Header[0] & kMaskpileup) >> kShiftpileup;
}

unsigned int OfflineData::GetEventTime_Low(unsigned int event)
{
  return (OfflineEventInformation[event].Header[1] & kMasktslo) >> kShifttslo;
}

unsigned int OfflineData::GetEventTime_High(unsigned int event)
{
  return (OfflineEventInformation[event].Header[2] & kMasktshi) >> kShifttshi;
}

unsigned int OfflineData::GetEventCfd(unsigned int event)
{
  unsigned int temp;
  switch(int(SamplingRate))
    {
    case 100:
      temp = (OfflineEventInformation[event].Header[2] & kMaskcfd100) >> kShiftcfd100;
      break;
    case 250:
      temp = (OfflineEventInformation[event].Header[2] & kMaskcfd250) >> kShiftcfd250;
      break;
    case 500:
      temp = (OfflineEventInformation[event].Header[2] & kMaskcfd500) >> kShiftcfd500;
      break;
    default:
      std::cout<<"ERROR Sample Rate."<<std::endl;
      temp = 0;
      break;
    }
  return temp;
}

unsigned int OfflineData::GetEventCfdForcedTriggerBit(unsigned int event)
{
  unsigned int temp;
  switch(int(SamplingRate))
    {
    case 100:
      temp = (OfflineEventInformation[event].Header[2] & kMaskcfdft100) >> kShiftcfdft100;
      break;
    case 250:
      temp = (OfflineEventInformation[event].Header[2] & kMaskcfdft250) >> kShiftcfdft250;
      break;
    case 500:
      temp = (OfflineEventInformation[event].Header[2] & kMaskcfds500) >> kShiftcfds500;
      if(temp == 7) temp = 1;
      else temp = 0;
      break;
    default:
      std::cout<<"ERROR Sample Rate."<<std::endl;
      temp = 0;
      break;
    }
  return temp;
}

unsigned int OfflineData::GetEventCfdTriggerSourceBits(unsigned int event)
{
  unsigned int temp;
  switch(int(SamplingRate))
    {
    case 100:
      temp = 0;
      break;
    case 250:
      temp = (OfflineEventInformation[event].Header[2] & kMaskcfds250) >> kShiftcfds250; 
      break;
    case 500:
      temp = (OfflineEventInformation[event].Header[2] & kMaskcfds500) >> kShiftcfds500;
      break;
    default:
      std::cout<<"ERROR Sample Rate."<<std::endl;
      temp = 0;
      break;
    }
  return temp;
}

unsigned int OfflineData::GetEventTraceLength(unsigned int event)
{
  return (OfflineEventInformation[event].Header[3] & kMaskltra) >> kShiftltra;
}

unsigned int OfflineData::GetEventEnergy(unsigned int event)
{
  return (OfflineEventInformation[event].Header[3] & kMaskevte) >> kShiftevte;
}

unsigned int OfflineData::GetEventTraceOutOfRangeFlag(unsigned int event)
{
  return (OfflineEventInformation[event].Header[3] & kMaskoutofr) >> kShiftoutofr;
}



// 
// OfflineData.cc ends here
