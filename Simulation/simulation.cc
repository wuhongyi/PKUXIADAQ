// simulation.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 6月  3 13:20:05 2017 (+0800)
// Last-Updated: 六 6月  3 15:22:24 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 5
// URL: http://wuhongyi.cn 

#include "simulation.hh"

#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <string>
#include <cmath>
#include <iostream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

simulation::simulation()
{
  Module_ADCBits = 14;
  Module_ADCMSPS = 100;
}

simulation::~simulation()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void simulation::ShowTrace(unsigned short *RcdTrace)
{

}

void simulation::ComputeFastFilters(double *fastfilter,double *cfd)
{
  unsigned int fastlen, fastgap;
  unsigned int fsum0, fsum1;

  double cfdscale;// Decide CFD Scale value
  cfdscale = 1.0 - (double)CFDScale * 0.125;
  
  fastlen = FastLength*(unsigned int)std::pow(2.0, (double)FastFilterRange);
  fastgap = FastGap*(unsigned int)std::pow(2.0, (double)FastFilterRange);

  // Check if trace length is sufficiently long
  if(TraceLength < ((2*fastlen + fastgap)*2))
    {
      printf("*Error* : the length of recorded trace is too short\n");
    }

  unsigned int offset = 2*fastlen + fastgap - 1;
  for(unsigned int x = offset; x < TraceLength; x++)
    {
      fsum0 = 0;
      for(unsigned int y = (x-offset); y < (x-offset+fastlen); y++)
	{
	  fsum0 += Trace[y];
	}
      fsum1 = 0;
      for(unsigned int y = (x-offset+fastlen+fastgap); y < (x-offset+2*fastlen+fastgap); y++)
	{
	  fsum1 += Trace[y];
	}
      fastfilter[x] = ((double)fsum1 - (double)fsum0)/(double)fastlen;
    }

  for(unsigned int x = 0; x < offset; x++)
    {
      fastfilter[x] = fastfilter[offset];
    }

  for(unsigned int x = CFDDelay; x < TraceLength; x++)
    {
      cfd[x] = -fastfilter[x-CFDDelay]+fastfilter[x]*cfdscale;

    }

  for(unsigned int x = 0; x < CFDDelay; x++)
    {
      cfd[x] = cfd[CFDDelay];
    }
}

void simulation::ComputeSlowFilters(double *slowfilter)
{
  unsigned int slowlen, slowgap;
  double preamptau, deltaT;
  double b1, c0, c1, c2;
  unsigned int bsum0, bsum1, bsum2;
  unsigned int esum0, esum1, esum2;
  double baseline;
  
  slowlen = SlowLength*(unsigned int)std::pow(2.0, (double)SlowFilterRange);
  slowgap = SlowGap*(unsigned int)std::pow(2.0, (double)SlowFilterRange);
  preamptau = IEEEFloating2Decimal(PreampTau);

  // Check if trace length is sufficiently long
  if(TraceLength < ((2*slowlen + slowgap)*2))
    {
      printf("*Error* : the length of recorded trace is too short\n");
    }

  deltaT = 1.0/((double)Module_ADCMSPS);
  b1 = std::exp(-1.0 * deltaT / preamptau);
  c0 = -(1.0 - b1) * std::pow(b1, (double)slowlen) * 4.0 / (1.0 - std::pow(b1, (double)slowlen));
  c1 = (1.0 - b1) * 4.0;
  c2 = (1.0 - b1) * 4.0 / (1.0 - std::pow(b1, (double)slowlen));


  // Compute baseline
  bsum0 = 0;
  for(unsigned int y = 0; y < slowlen; y++)
    {
      bsum0 += Trace[y];
    }
  bsum1 = 0;
  for(unsigned int y = slowlen; y < slowlen+slowgap; y++)
    {
      bsum1 += Trace[y];
    }
  bsum2 = 0;
  for(unsigned int y = (slowlen+slowgap); y < (2*slowlen+slowgap); y++)
    {
      bsum2 += Trace[y];
    }
  baseline = c0*(double)bsum0+c1*(double)bsum1+c2*(double)bsum2;

  // Compute slow filter response
  unsigned int offset = 2*slowlen + slowgap - 1;
  for(unsigned int x = offset; x < TraceLength; x++)
    {
      esum0 = 0;
      for(unsigned int y = (x-offset); y<(x-offset+slowlen); y++)
	{
	  esum0 += Trace[y];
	}
      esum1 = 0;
      for(unsigned int y = (x-offset+slowlen); y<(x-offset+slowlen+slowgap); y++)
	{
	  esum1 += Trace[y];
	}
      esum2 = 0;
      for(unsigned int y = (x-offset+slowlen+slowgap); y < (x-offset+2*slowlen+slowgap); y++)
	{
	  esum2 += Trace[y];
	}
      slowfilter[x] = c0*(double)esum0+c1*(double)esum1+c2*(double)esum2-baseline;
    }

  for(unsigned int x = 0; x < offset; x++)
    {
      slowfilter[x] = slowfilter[offset];
    }
}

double simulation::ComputeEnergy()
{

  return 0;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// IEEEFloating2Decimal:
// Convert a IEEE 754 standrad floating point number (1-bit sign, 8-bit exponent, and 23-bit mantissa) to a decimal fractional number.
// Return Value:
// the converted decimal fractional number
double simulation::IEEEFloating2Decimal(unsigned int IEEEFloatingNumber)
{
  short signbit, exponent;
  double mantissa, DecimalNumber;
	
  signbit = (short)(IEEEFloatingNumber >> 31);
  exponent = (short)((IEEEFloatingNumber & 0x7F800000) >> 23) - 127;
  mantissa = 1.0 + (double)(IEEEFloatingNumber & 0x7FFFFF) / pow(2.0, 23.0);
  if(signbit == 0)
    {
      DecimalNumber = mantissa * pow(2.0, (double)exponent);
    }
  else
    {
      DecimalNumber = - mantissa * pow(2.0, (double)exponent);
    }
	
  return(DecimalNumber);
	
}


// Decimal2IEEEFloating:
// Convert a decimal fractional number to a IEEE 754 standrad floating point number (1-bit sign, 8-bit exponent, and 23-bit mantissa).
// Return Value:
// the converted IEEE floating point number
unsigned int simulation::Decimal2IEEEFloating(double DecimalNumber)
{
  unsigned int IEEEFloatingNum, IntPart, IntPartHex, FractPartHex;
  double AbsDecNum, FractPart;
  unsigned short k, m, exponent;
  short n;
  char signbit;
	
  // Check signbit
  if(DecimalNumber > 0)
    {
      signbit = 0;
    }
  else if(DecimalNumber < 0)
    {
      signbit = 1;
    }
  else	// DecimalNumber = 0
    {
      return(0);
    }
	
  // Get the absolute value
  AbsDecNum = fabs(DecimalNumber);
	
  // Get the integer part
  IntPart = (unsigned int)floor(AbsDecNum);
	
  // Convert the integer part
  IntPartHex = 0;
  k = 0;
  do
    {
      if((IntPart % 2) == 1)
	{
	  IntPartHex = SetBit_32(k, IntPartHex);
	}
      else
	{
	  IntPartHex = ClrBit_32(k, IntPartHex);
	}
		
      k ++;
		
      IntPart /= 2;
		
    } while((IntPart > 0) && (k < 32));
	
  // Get the fractional part
  FractPart = AbsDecNum - floor(AbsDecNum);
	
  // Convert the fractional part
  FractPartHex = 0;
  m = 0;
  do
    {
      if((FractPart * 2.0) >= 1.0)
	{
	  FractPartHex = SetBit_32((unsigned short)(31 - m), FractPartHex);
	  FractPart = FractPart * 2.0 - 1.0;
	}
      else
	{
	  FractPartHex = ClrBit_32((unsigned short)(31 - m), FractPartHex);
	  FractPart = FractPart * 2.0;
	}
		
      m ++;
		
    } while((FractPart > 0) && (m < 32));
	
  // Combine the integer and fractional part
  // First, find the exponent
  if(IntPartHex > 0)
    {
      exponent = 127 + k - 1;	// 127 is the exponent bias
		
      // Add sign bit first
      IEEEFloatingNum = signbit * (unsigned int)pow(2.0, 31.0);
		
      // Add the exponent
      IEEEFloatingNum += exponent * (unsigned int)pow(2.0, 23.0);
		
      // Add the mantissa
      IntPartHex = ClrBit_32((unsigned short)(k - 1), IntPartHex);
      IEEEFloatingNum += IntPartHex * (unsigned int)pow(2.0, (double)(22 - (k - 2)));
      IEEEFloatingNum += FractPartHex >> (32 - (23 - (k - 1)));
    }
  else	// IntPartHex = 0
    {
      // Search the fist non-zero bit in FractPartHex
      for(n = 31; n >= 0; n --)
	{
	  if(TstBit_32(n, FractPartHex) == 1)
	    {
	      break;
	    }
	}
		
      exponent = 127 - (32 - n);	// 127 is the exponent bias
		
      // Add sign bit first
      IEEEFloatingNum = signbit * (unsigned int)pow(2.0, 31.0);
		
      // Add the exponent
      IEEEFloatingNum += exponent * (unsigned int)pow(2.0, 23.0);
		
      // Add the mantissa
      FractPartHex = ClrBit_32(n, FractPartHex);
      if(n >= 23)
	{
	  IEEEFloatingNum += FractPartHex >> (n - 23);
	}
      else
	{
	  IEEEFloatingNum += FractPartHex << (23 - n);
	}
    }
	
  return(IEEEFloatingNum);												
}

// Test a bit in a 16-bit unsigned integer.
unsigned short  simulation::TstBit_16(unsigned short bit,unsigned short value)//获得value第bit位数值
{
  return(((value & (unsigned short)(std::pow(2.0, (double)bit))) >> bit));
}

// Set a bit in a 16-bit unsigned integer.
unsigned short simulation::SetBit_16(unsigned short bit,unsigned short value)//将value第bit位设为1
{
  return(value | (unsigned short)(std::pow(2.0, (double)bit)));
}

// Clear a bit in a 16-bit unsigned integer.
unsigned short simulation::ClrBit_16(unsigned short bit,unsigned short value)//将value第bit位设为0
{
  value = SetBit_16(bit, value);
  return(value ^ (unsigned short)(std::pow(2.0, (double)bit)));
}

// Set a bit in a 32-bit unsigned integer.
unsigned int simulation::SetBit_32(unsigned short bit,unsigned int value)//将value第bit位设为1
{
  return(value | (unsigned int)(std::pow(2.0, (double)bit)));
}

// Clear a bit in a 32-bit unsigned integer.
unsigned int simulation::ClrBit_32(unsigned short bit,unsigned int value)//将value第bit位设为0
{
  value = SetBit_32(bit, value);
  return(value ^ (unsigned int)(std::pow(2.0, (double)bit)));
}

// Test a bit in a 32-bit unsigned integer.
unsigned int simulation::TstBit_32(unsigned short bit,unsigned int value )//获得value第bit位数值
{
  return(((value & (unsigned int)(std::pow(2.0, (double)bit))) >> bit));
}


// 
// simulation.cc ends here
