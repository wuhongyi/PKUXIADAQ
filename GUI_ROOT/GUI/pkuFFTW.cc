// pkuFFTW.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 5月 13 19:11:24 2017 (+0800)
// Last-Updated: 六 5月 27 20:17:15 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 58
// URL: http://wuhongyi.cn 

#include "pkuFFTW.hh"
#include <iostream>
#include <cstring>
#include <cmath>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

fftw1d::fftw1d(int n,int sign,unsigned flags)
{
  N = n;
  Sign = sign;
  Flags = flags;

  fftw_complex *in, *out;
  in = Malloc_fftw_complex(N);
  out = Malloc_fftw_complex(N);
 
  fftwplan = fftw_plan_dft_1d(N,in,out,Sign,Flags);
  if(fftwplan) haveplan = true;

  Free_fftw_complex(in);
  Free_fftw_complex(out);


  outresult = Malloc_fftw_complex(N);
}

fftw1d::~fftw1d()
{
  Free_fftw_complex(outresult);
}

void fftw1d::Execute(fftw_complex *in, fftw_complex *out)
{
  if(haveplan) fftw_execute_dft(fftwplan,in,out);
  else std::cout<<"You need "<<std::endl;
}

void fftw1d::ExecuteNormalized(fftw_complex *in, fftw_complex *out)
{
  if(haveplan) fftw_execute_dft(fftwplan,in,out);
  else std::cout<<"You need fftw_plan first."<<std::endl;

  if(Sign == -1)
    {
      for (int i = 0; i < N; ++i)
	{
	  out[i][0] = out[i][0]/N*2;
	  out[i][1] = out[i][1]/N*2;
	}
      // out[0][0]/=2.;
      // out[0][1]/=2.;
    }
  else
    {
      for (int i = 0; i < N; ++i)
	{
	  out[i][0] = out[i][0]/2;
	  out[i][1] = out[i][1]/2;
	}
    }
}

void fftw1d::ForwardGetAmplitude(fftw_complex *in,double *out)
{
  if(Sign == -1)
    {
      if(haveplan) fftw_execute_dft(fftwplan,in,outresult);
      else std::cout<<"You need fftw_plan first."<<std::endl;

      for (int i = 0; i < N; ++i)
	{
	  outresult[i][0] = outresult[i][0]/N*2;
	  outresult[i][1] = outresult[i][1]/N*2;
	  out[i] = std::sqrt(outresult[i][0]*outresult[i][0]+outresult[i][1]*outresult[i][1]);
	}
      out[0] /= 2.;
    }

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

fftw1d_r2c::fftw1d_r2c(int n,unsigned flags)
{
  N = n;
  Flags = flags;

  fftw_complex *out;
  double *in;
  out = Malloc_fftw_complex(N);
  in = Malloc_fftw_real(N);
 
  fftwplan = fftw_plan_dft_r2c_1d(N,in,out,Flags);
  if(fftwplan) haveplan = true;

  Free_fftw_complex(out);
  Free_fftw_real(in);
}

fftw1d_r2c::~fftw1d_r2c()
{

}

void fftw1d_r2c::Execute(double *in, fftw_complex *out)
{
  if(haveplan) fftw_execute_dft_r2c(fftwplan,in,out);
  else std::cout<<"You need fftw_plan first."<<std::endl;
}

void fftw1d_r2c::ExecuteNormalized(double *in, fftw_complex *out)
{
  if(haveplan) fftw_execute_dft_r2c(fftwplan,in,out);
  else std::cout<<"You need fftw_plan first."<<std::endl;

  for (int i = 0; i < N; ++i)
    {
      out[i][0] = out[i][0]/N*2;
      out[i][1] = out[i][1]/N*2;
    }
  // out[0][0]/=2.;
  // out[0][1]/=2.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

fftw1d_c2r::fftw1d_c2r(int n,unsigned flags)
{
  N = n;
  Flags = flags;

  fftw_complex *in;
  double *out;
  in = Malloc_fftw_complex(N);
  out = Malloc_fftw_real(N);
 
  fftwplan = fftw_plan_dft_c2r_1d(N,in,out,Flags);
  if(fftwplan) haveplan = true;

  Free_fftw_complex(in);
  Free_fftw_real(out);
}

fftw1d_c2r::~fftw1d_c2r()
{
  
}

void fftw1d_c2r::Execute(fftw_complex *in, double *out)
{
  if(haveplan) fftw_execute_dft_c2r(fftwplan,in,out);
  else std::cout<<"You need fftw_plan first."<<std::endl;
}

void fftw1d_c2r::ExecuteNormalized(fftw_complex *in, double *out)
{
  if(haveplan) fftw_execute_dft_c2r(fftwplan,in,out);
  else std::cout<<"You need fftw_plan first."<<std::endl;
  
  for (int i = 0; i < N; ++i)
    {
      out[i] = out[i]/2;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

corr_fftw::corr_fftw(int n,bool biased)
{
  N = n;
  N2 = 2*N;
  Biased = biased;
  
  fft1d = new fftw1d(N2,-1);
  fft1dback = new fftw1d(N2,+1);
  
  signala_ext = Malloc_fftw_complex(N2);
  signalb_ext = Malloc_fftw_complex(N2);
  signal_result = Malloc_fftw_complex(N2);
  outa = Malloc_fftw_complex(N2);
  outb = Malloc_fftw_complex(N2);
  out = Malloc_fftw_complex(N2);

  outatemp = reinterpret_cast<std::complex<double>*>(outa);
  outbtemp = reinterpret_cast<std::complex<double>*>(outb);
  outtemp = reinterpret_cast<std::complex<double>*>(out);
  scale = 1.0/N2;
  
}

corr_fftw::~corr_fftw()
{
  delete fft1d;
  delete fft1dback;
  // delete fft1dc2r;

  Free_fftw_complex(signala_ext);
  Free_fftw_complex(signalb_ext);
  Free_fftw_complex(outa);
  Free_fftw_complex(outb);
  Free_fftw_complex(out);
}

void corr_fftw::Execute(fftw_complex *in1, fftw_complex *in2, double *result)
{
  memset(signala_ext, 0, sizeof(fftw_complex) * N2);
  memset(signalb_ext, 0, sizeof(fftw_complex) * N2);

  // zero padding
  memcpy(signala_ext + (N - 1), in1, sizeof(fftw_complex) * N);
  memcpy(signalb_ext, in2, sizeof(fftw_complex) * N);

  fft1d->Execute(signala_ext, outa);
  fft1d->Execute(signalb_ext, outb);

  for (int i = 0; i < N2; ++i)
    outtemp[i] = outatemp[i] * std::conj(outbtemp[i]) * scale * scale ;

  fft1dback->Execute(out, signal_result);

  if(Biased)
    {
      for (int i = 0; i < N; ++i)
	{
	  result[i] = signal_result[N-1-i][0]*2;
	}
    }
  else
    {
      for (int i = 0; i < N; ++i)
	{
	  result[i] = signal_result[N-1-i][0]*2*N/(N-i);
	}
    }
}

void corr_fftw::Execute(double *in1, double *in2, double *result)
{
  memset(signala_ext, 0, sizeof(fftw_complex) * N2);
  memset(signalb_ext, 0, sizeof(fftw_complex) * N2);

  for (int i = 0; i < N; ++i)
    {
      signala_ext[i+N-1][0] = in1[i];
      signalb_ext[i][0] = in2[i];
    }

  fft1d->Execute(signala_ext, outa);
  fft1d->Execute(signalb_ext, outb);

  for (int i = 0; i < N2; ++i)
    outtemp[i] = outatemp[i] * std::conj(outbtemp[i]) * scale * scale ;

  fft1dback->Execute(out, signal_result);

  if(Biased)
    {
      for (int i = 0; i < N; ++i)
	{
	  result[i] = signal_result[N-1-i][0]*2;
	}
    }
  else
    {
      for (int i = 0; i < N; ++i)
	{
	  result[i] = signal_result[N-1-i][0]*2*N/(N-i);
	}
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

corr_timedomain::corr_timedomain(bool biased)
{
  Biased = biased;
}

corr_timedomain::~corr_timedomain()
{

}

void corr_timedomain::corr_n(std::map<int,double> *in1,std::map<int,double> *in2,int n,double *out)
{
  memset(out, 0, sizeof(double) * n);
  for (std::map<int,double>::iterator it1 = in1->begin(); it1 != in1->end(); ++it1)
    for (std::map<int,double>::iterator it2 = in2->begin(); it2 != in2->end(); ++it2)
      {
	if(it1->first <= it2->first) out[it2->first - it1->first] += it1->second*it2->second;
      }

  if(Biased)
    {
      for (int i = 0; i < n; ++i)
	out[i] /= n;
    }
  else
    {
      for (int i = 0; i < n; ++i)
	out[i] /= n-i;
    }
}

void corr_timedomain::corr_n(std::vector<int> *in1,std::vector<int> *in2,int n,double *out)
{
  memset(out, 0, sizeof(double) * n);
  for (unsigned int i = 0; i < in1->size(); ++i)
    for (unsigned int j = 0; j < in2->size(); ++j)
      {
	if(in1->at(i) <= in2->at(j)) out[in2->at(j)-in1->at(i)] += 1;
      }

  if(Biased)
    {
      for (int i = 0; i < n; ++i)
	out[i] /= n;
    }
  else
    {
      for (int i = 0; i < n; ++i)
	out[i] /= n-i;
    }
}

void corr_timedomain::corr_n(int n1,int *in1,int n2,int *in2,int n,double *out)
{
  memset(out, 0, sizeof(double) * n);
 
  for (int i = 0; i < n1; ++i)
    for (int j = 0; j < n2; ++j)
      {
	if(in1[i]<=in2[j]) out[in2[j]-in1[i]] += 1;
      }

  if(Biased)
    {
      for (int i = 0; i < n; ++i)
	out[i] /= n;
    }
  else
    {
      for (int i = 0; i < n; ++i)
	out[i] /= n-i;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

conv_fftw::conv_fftw()
{

}

conv_fftw::~conv_fftw()
{

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

fftwbase::fftwbase()
{
  haveplan = false;
  fftwplan = NULL;

  N = -1;
}

fftwbase::~fftwbase()
{
  if(haveplan)
    {
      fftw_destroy_plan(fftwplan);
      fftwplan = NULL;
    }
}


// 
// pkuFFTW.cc ends here
