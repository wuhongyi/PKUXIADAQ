// simulation.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 6月  3 13:19:29 2017 (+0800)
// Last-Updated: 六 6月  3 15:23:09 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 5
// URL: http://wuhongyi.cn 

#ifndef _SIMULATION_H_
#define _SIMULATION_H_

// #define NUMBER_OF_CHANNELS            16
// #define DSP_CLOCK_MHZ                100      // DSP clock frequency in MHz
// #define SYSTEM_CLOCK_MHZ             100      // System FPGA clock frequency in MHz (used for real time counting)

#define DAC_VOLTAGE_RANGE            3.0      // Pixie-16 DAC range is -1.5 V to +1.5 V

#define FASTFILTER_MAX_LEN 127
#define FAST_THRESHOLD_MAX 65535
#define MIN_FASTLENGTH_LEN 2

#define SLOWFILTER_MAX_LEN 127
#define MIN_SLOWLENGTH_LEN 2
#define MIN_SLOWGAP_LEN 3

#define FASTFILTERRANGE_MAX 0
#define FASTFILTERRANGE_MIN 0

#define SLOWFILTERRANGE_MAX 6
#define SLOWFILTERRANGE_MIN 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class simulation
{
public:
  simulation();
  virtual ~simulation();


  void ShowTrace(unsigned short *RcdTrace);
  void ComputeFastFilters(double *fastfilter,double *cfd);
  void ComputeCfd(double *cfd);
  void ComputeSlowFilters(double *slowfilter);
  double ComputeEnergy();

  
private:
  // Convert a IEEE 754 standrad floating point number (1-bit sign, 8-bit exponent, and 23-bit mantissa) to a decimal fractional number.
  double IEEEFloating2Decimal(unsigned int IEEEFloatingNumber);
  // Convert a decimal fractional number to a IEEE 754 standrad floating point number (1-bit sign, 8-bit exponent, and 23-bit mantissa).
  unsigned int Decimal2IEEEFloating(double DecimalNumber);

  // Test a bit in a 16-bit unsigned integer.
  unsigned short  TstBit_16(unsigned short bit,unsigned short value);
  // Set a bit in a 16-bit unsigned integer.
  unsigned short SetBit_16(unsigned short bit,unsigned short value);
  // Clear a bit in a 16-bit unsigned integer.
  unsigned short ClrBit_16(unsigned short bit,unsigned short value);
  // Set a bit in a 32-bit unsigned integer.
  unsigned int SetBit_32(unsigned short bit,unsigned int value);
  // Clear a bit in a 32-bit unsigned integer.
  unsigned int ClrBit_32(unsigned short bit,unsigned int value);
  // Test a bit in a 32-bit unsigned integer.
  unsigned int TstBit_32(unsigned short bit,unsigned int value );

  
private:
  unsigned short TraceLength;
  unsigned short Trace[65536];

  unsigned short Module_ADCBits;
  unsigned short Module_ADCMSPS;
  
  unsigned int FastFilterRange;//
  unsigned int FastLength;
  unsigned int FastGap;
  unsigned int FastThresh;
  
  unsigned int SlowFilterRange;
  unsigned int SlowLength;
  unsigned int SlowGap;
  unsigned int PreampTau;
  
  unsigned int PeakSample;
  unsigned int PeakSep;

  unsigned int CFDThresh;
  unsigned int CFDDelay;
  unsigned int CFDScale;
  
};

#endif /* _SIMULATION_H_ */
// 
// simulation.hh ends here
