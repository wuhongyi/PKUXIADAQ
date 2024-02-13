// zlib.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 2月 10 18:59:03 2020 (+0800)
// Last-Updated: 一 2月 10 19:07:55 2020 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 8
// URL: http://wuhongyi.cn 

// g++ zlib.cc -lz -o 456

#include <iostream>
#include <string>
 
#include <zlib.h>
// #include <zconf.h>
 
using namespace std;
 
 
int main(int argc, char *argv[])
{
  //
  unsigned char szSrc[] = "test the compression\0and uncompression of zlib.";
  unsigned long nSrcLen = 48;//sizeof(szSrc);
 
  unsigned char szZip[1024] = {0};
  unsigned long nZipLen = 1024;
 
  compress(szZip, &nZipLen, szSrc, nSrcLen);
 
  //
  unsigned char szUnZip[1024] = {0};
  unsigned long nUnZipLen = 1024;
 
  uncompress(szUnZip, &nUnZipLen, szZip, nZipLen);
 
  //
  cout<<"Src:"<<szSrc<<", len:"<<nSrcLen<<endl;		
  cout<<"Zip:"<<szZip<<", len:"<<nZipLen<<endl;
  cout<<"UnZip:"<<szUnZip<<", len:"<<nUnZipLen<<endl;
 
 for (int i = 0; i < 48; ++i)
   {
     if(szSrc[i]!=szUnZip[i]) cout<<i<<endl;
   }
 
  return 0;
}


// 
// zlib.cc ends here
