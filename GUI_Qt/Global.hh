// Global.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 二 10月 25 14:31:51 2022 (+0800)
// Last-Updated: 二 12月 24 18:29:58 2024 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 53
// URL: http://wuhongyi.cn 

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <unistd.h>

#include <bitset>
#include <limits>

#include "QVector"
#include "QMutex"
#include "QString"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#define VERSIONDATE  20241224
#define EXPIRATIONDATE 20250824

#define ONLINEMODE


// #define INBEAMGAMMA       // Use only in IMP/CIAE inbeam gamma experiment
#define INBEAMGAMMALIMIT  900 //s
#define INBEAMGAMMASTOPTIME  7200 //s


// Features customized for special users
#define DECODERONLINE


#define RECODESHA256


#define MAXMODULENUM 13
#define MAXCHANNELNUM 64
#define MAXHISTOGRAMLENGTH 65536
#define MAXSTATISTICS 4096

#define BUFFLENGTH 1258000

#define EXTFIFO_READ_THRESH 1024

#define SHAREDMEMORYDATAOFFSET 1170 //BYTE
// 1st 4    bytes IDcode for event shared memory
// 2nd 2    bytes number of valid Num Modules in shared memory
// 3rd 4    bytes Crate ID/Run Number  [31:24]  [23:0]
// 4   4    bytes ID for update spectrum
// 5   128  bytes name of data file
// 6   1024 bytes path of data file
// 7   2    bytes software id
// 8   2    bytes software version


typedef struct RAWDATABUFFER
{
  int runno;
  int buffid;
  unsigned int buff[BUFFLENGTH];
} RAWDATABUFFER;

extern QMutex datavectordecoder_mutex[];
extern QVector<RAWDATABUFFER> datavectordecoder[];
extern RAWDATABUFFER rawdatabuffer;
extern QString LOGINUSERNAME;


#ifdef WIN32
	#define ENDL  "\r\n"
	#define DIRSEP "/"
#else
	#define ENDL  "\n"
	#define DIRSEP "/"
#endif

#define CONFIG_DIR ".GDDAQ"
#define DEFAUL_RUN_CONFIG_FILENAME "GDDAQ_XIA.conf"


#define DEFAULT_SAVEDDATA true
#define DEFAULT_CLOCKRESET true


#define ONLINE_ENERGY_CALI_FILENAME "pars/energy.txt"
#define ONLINE_PSD_CALI_FILENAME "pars/psd.txt"


#define DEFAULT_JSON "pars/setting.json"


template<class T>
T set_bit(unsigned short bit, T value, const bool bit_status)
{
  auto value_bits = std::bitset<std::numeric_limits<T>::digits>(value);
  value_bits.set(bit, bit_status);
  return (T) value_bits.to_ulong();
}

template<class T>
T test_bit( unsigned short bit, T value)
{
  return std::bitset<std::numeric_limits<T>::digits>(value).test(bit);
}

void SetMultipleBit(unsigned int *Data, int Select, int bit0);
void SetMultipleBit(unsigned int *Data, int Select, int bit1, int bit0);
void SetMultipleBit(unsigned int *Data, int Select, int bit2, int bit1, int bit0);
void SetMultipleBit(unsigned int *Data, int Select, int bit3, int bit2, int bit1, int bit0);

#endif /* _GLOBAL_H_ */

// 
// Global.hh ends here
