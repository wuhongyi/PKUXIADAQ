// pkuFFTW.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 5月 13 19:11:04 2017 (+0800)
// Last-Updated: 六 5月 27 20:16:11 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 38
// URL: http://wuhongyi.cn 

#ifndef _PKUFFTW_H_
#define _PKUFFTW_H_

#include <complex>
#include <fftw3.h>
#include <vector>
#include <map>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline fftw_complex* Malloc_fftw_complex(int n)
{
  return (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * n);
}

inline double* Malloc_fftw_real(int n)
{
  return (double *)fftw_malloc(sizeof(double) * n);
  // return (double*)fftw_alloc_real(n);//it need fftw3 version>=3.3-beta1
}

inline void Free_fftw_complex(fftw_complex* inout)
{
  fftw_free(inout);
}

inline void Free_fftw_real(double *inout)
{
  fftw_free(inout);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class fftwbase
{
public:
  fftwbase();
  virtual ~fftwbase();
  
protected:
  fftw_plan fftwplan;
  bool haveplan;
  
  // 1d
  int N;
  int Sign;
  unsigned Flags;
  fftw_r2r_kind Kind;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class fftw1d : public fftwbase
{
public:
  // n 输入数据点数； sign -1为正变换，+1为逆变换； flags FFTW_MEASURE或FFTW_ESTIMATE
  fftw1d(int n,int sign,unsigned flags = FFTW_MEASURE);// fftw_complex *in, *out;
  virtual ~fftw1d();

  //正变换返回值out数据结构具有对称性,因此只需取前一半
  void Execute(fftw_complex *in, fftw_complex *out);//执行变换
  void ExecuteNormalized(fftw_complex *in, fftw_complex *out);//执行变换并归一化获得真实幅值（直流分量没有除以2）

  
  //正变换添加函数直接得到幅值、相位等
  void ForwardGetAmplitude(fftw_complex *in,double *out);//获得真实幅值
  void ForwardGetPhase(fftw_complex *in,double *out){;}// TODO


private:
  fftw_complex *outresult;
};


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// 实输入数据，复Hermitian输出，正变换。
// 由于实数据的DFT具有 Hermitian对称性，所以只需要计算n/2+1（向下取整）个输出就可以了。比如对于r2c，输入in有n个数据，输出out有floor(n/2)＋1个数据。
class fftw1d_r2c : public fftwbase
{
public:
  fftw1d_r2c(int n,unsigned flags = FFTW_MEASURE);
  virtual ~fftw1d_r2c();

  void Execute(double *in, fftw_complex *out);
  void ExecuteNormalized(double *in, fftw_complex *out);
  
};

// 复Hermitian输入数据，实输出数据，逆变换。
class fftw1d_c2r : public fftwbase
{
public:
  fftw1d_c2r(int n,unsigned flags = FFTW_MEASURE);
  virtual ~fftw1d_c2r();

  void Execute(fftw_complex *in, double *out);
  void ExecuteNormalized(fftw_complex *in, double *out);
  
};


// class fftw1d_r2r : public fftwbase
// {
// public:
//   fftw1d_r2r(int n,fftw_r2r_kind kind,unsigned flags = FFTW_MEASURE);
//   virtual ~fftw1d_r2r();

// };


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class corr_fftw
{
public:
  corr_fftw(int n,bool biased = true);//n为相关计算的点数，biased为true表示有偏，false表示无偏
  virtual ~corr_fftw();

  void Execute(fftw_complex *in1, fftw_complex *in2, double *result);//result长度为n
  void Execute(double *in1, double *in2, double *result);//result长度为n 
  
private:
  int N;
  int N2;
  bool Biased;

  fftw1d *fft1d;
  fftw1d *fft1dback;
  fftw1d_c2r *fft1dc2r;

  fftw_complex * signala_ext;
  fftw_complex * signalb_ext;
  fftw_complex * signal_result;
  fftw_complex * outa;
  fftw_complex * outb;
  fftw_complex * out;

  std::complex<double> *outatemp;
  std::complex<double> *outbtemp;
  std::complex<double> *outtemp;
  std::complex<double> scale;
};


// 对于数据点少的情况，采用时域方法按照公式计算较快！！！
class corr_timedomain
{
public:
  corr_timedomain(bool biased = true);//biased为true表示有偏，false表示无偏
  virtual ~corr_timedomain();

  // 遍历所有点
  template<typename T>
  void corr_n_n(int n, T *in1,T *in2,double *out);//输出out为n个点
  
  template<typename T>
  void corr_n_n2(int n, T *in1,T *in2,double *out);//输出out为2n-1个点
  
  // 计算稀疏点  vector map mhit 结构
  void corr_n(std::map<int,double> *in1,std::map<int,double> *in2,int n,double *out);//in1 中 int类型数据为bin值，double类型数据为该bin的数值
  
  void corr_n(std::vector<int> *in1,std::vector<int> *in2,int n,double *out);//in1 in2 为原始数据，为有计数的bin值，如果同一个bin内有多个事件，则该bin值有多个
  void corr_n(int n1,int *in1,int n2,int *in2,int n,double *out);
  
private:
  bool Biased;
  
};

// 卷积
class conv_fftw
{
public:
  conv_fftw();
  virtual ~conv_fftw();

  // TODO


  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

template<typename T>
void corr_timedomain::corr_n_n(int n, T *in1,T *in2,double *out)
{
  if(Biased)
    {
      for (int i = 0; i < n; ++i)
	{
	  out[i] = 0;
	  for (int j = 0; j <= n-1-i; ++j)
	    {
	      out[i] += in1[j]*in2[j+i];
	    }
	  out[i] /= n;
	}
    }
  else
    {
      for (int i = 0; i < n; ++i)
	{
	  out[i] = 0;
	  for (int j = 0; j <= n-1-i; ++j)
	    {
	      out[i] += in1[j]*in2[j+i];
	    }
	  out[i] /= n-i;
	}
    }
}

template<typename T>
void corr_timedomain::corr_n_n2(int n, T *in1,T *in2,double *out)
{
  // C++98 std::abs 只支持 float、double、long double
  // C++11 std::abs 扩展了 T
  int n2 = 2*n-1;

  // TODO 应该改成分两段计算速度较快！！！
  if(Biased)
    {
      for (int i = 0; i < n2; ++i)
	{
	  double sum = 0;
	  for (int j = 0; j <= n-1-int(std::abs(double(i-(n-1)))); ++j)
	    {
	      sum += in1[j]*in2[j+int(std::abs(double(i-(n-1))))];
	    }
	  out[i] = sum/n;
	}
    }
  else
    {
      for (int i = 0; i < n2; ++i)
	{
	  double sum = 0;
	  for (int j = 0; j <= n-1-int(std::abs(double(i-(n-1)))); ++j)
	    {
	      sum += in1[j]*in2[j+int(std::abs(double(i-(n-1))))];
	    }
	  out[i] = sum/(n-std::abs(double(i-(n-1))));
	}
    }
}




#endif /* _PKUFFTW_H_ */
// 
// pkuFFTW.hh ends here
