// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 二 7月 12 11:13:59 2016 (+0800)
// Last-Updated: 一 10月  3 12:37:58 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 8
// URL: http://wuhongyi.github.io 

// 当前Online仅适用于采样率100MHz插件

#include "Online.hh"
#include <iostream>
#include <cstdio>
#include <cstdlib>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char **argv)
{
  TApplication theApp("App", &argc, argv);

  // check compiler
  if(sizeof(char) != 1) std::cout<<"sizeof(char) != 1 The current compiler is not suitable for running the program！"<<std::endl; 
  if(sizeof(short) != 2) std::cout<<"sizeof(short) != 2 The current compiler is not suitable for running the program！"<<std::endl; 
  if(sizeof(int) != 4) std::cout<<"sizeof(int) != 4 The current compiler is not suitable for running the program！"<<std::endl;
  
  Online mainWindow(gClient->GetRoot());

  theApp.Run();

  return 0;
}

// // g++ main.cc -lpthread  -lrt -o 123
// #include <stdio.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <sys/mman.h>
// #include <unistd.h>
// #include <semaphore.h>
// #include <string.h>
// #include <errno.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include <math.h>
// #include <iostream>
// //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// int main(int argc, char *argv[])
// {
//   int PRESET_MAX_MODULES = 24;
//   int SYSTEM_CLOCK_MHZ = 100;
  
//   int shm_id;
//   unsigned char *ptr;
//   sem_t *sem;
//   int rc;
//   unsigned char *buf;  
//   unsigned char *buf_new;
//   int val;

//   buf = new unsigned char[(PRESET_MAX_MODULES*448*4)+10];
//   buf_new = new unsigned char[(PRESET_MAX_MODULES*448*4)+10];
//   unsigned int Statistics[448];
//   unsigned int Statistics_new[448];

//   double RealTime_new;
//   double LiveTime_new[16];
//   double FastPeaks_new[16];
//   double ChanEvents_new[16];
//   double RealTime;
//   double LiveTime[16];
//   double FastPeaks[16];
//   double ChanEvents[16];


//   shm_id=shm_open("shmpixie16lee",O_RDWR,0);/*第一步：打开共享内存区*/
//   if (shm_id==-1)
//     {
//       printf( "open shared memory error.errno=%d,desc=%s.\n", errno, strerror(errno));
//       return -1;
//     }
//   else
//     {
//       printf( "open shared memory ok.\n");
//     }

//   sem=sem_open("sempixie16lee",1);/*打开信号量*/
//   if (sem==SEM_FAILED)
//     {
//       printf( "open semaphore error.errno=%d,desc=%s.\n", errno, strerror(errno));
//       return -1;
//     }
//   else
//     {
//       printf( "open semaphore ok.\n");
//     }

//   ptr=(unsigned char*)mmap(NULL,(PRESET_MAX_MODULES*448*4)+10,PROT_READ|PROT_WRITE,MAP_SHARED,shm_id,0);/*连接共享内存区*/

//   unsigned int number = -1;
//   unsigned short ModNum;
//   int RunNumber;
//   unsigned int tempN;

//   while(1 == 1)
//     {
//       // if(sem_wait(sem) == -1) continue;
//       // sem_post(sem);
//       sem_getvalue(sem, &val);
//       // printf("sem: %d\n",val);
//       if(val > 0)
// 	{
// 	  memcpy(&tempN,ptr,4);
// 	  if(number != tempN)
// 	    {
// 	      memcpy(buf_new,ptr,(PRESET_MAX_MODULES*448*4)+10);
// 	      if(number == -1) 
// 		{
// 		  number = tempN;
// 		  memcpy(buf,buf_new,(PRESET_MAX_MODULES*448*4)+10);
// 		  continue;
// 		}
// 	      number = tempN;
	      
// 	      // =======
// 	      memcpy(&ModNum,buf_new+4,2);
// 	      memcpy(&RunNumber,buf_new+6,4);
// 	      printf("RunNumber: %d -- Num: %d\n",RunNumber,number);

	      
// 	      for(int i = 0; i < ModNum;i++)
// 		{
// 		  memcpy(Statistics,buf+10+4*448*i,448*4);
// 		  memcpy(Statistics_new,buf_new+10+4*448*i,448*4);

// 		  RealTime = (double)Statistics[2] * pow(2.0, 32.0);
// 		  RealTime += (double)Statistics[3];
// 		  RealTime *= 1.0e-6 / (double)SYSTEM_CLOCK_MHZ;

// 		  RealTime_new = (double)Statistics_new[2] * pow(2.0, 32.0);
// 		  RealTime_new += (double)Statistics_new[3];
// 		  RealTime_new *= 1.0e-6 / (double)SYSTEM_CLOCK_MHZ;
		  
// 		  std::cout<<"======================="<<std::endl;
// 		  std::cout<<"Mod: "<<i<<std::endl;
// 		  std::cout<<"Real Time: "<<RealTime_new<<std::endl;
// 		  std::cout<<"Cha:  LiveTime:   InputCountRate:  OutputCountRate:"<<std::endl;
// 		  for(int j=0;j<16;j++)
// 		    {
// 		      LiveTime[j] = (double)Statistics[63+j] * pow(2.0, 32.0);
// 		      LiveTime[j] += (double)Statistics[79+j];
// 		      LiveTime[j] *= 1.0e-6 / (double)100;
// 		      FastPeaks[j] = (double)Statistics[95+j] * pow(2.0, 32.0);
// 		      FastPeaks[j] += (double)Statistics[111+j];
// 		      ChanEvents[j] = (double)Statistics[223+j] * pow(2.0, 32.0);
// 		      ChanEvents[j] += (double)Statistics[239+j];


// 		      LiveTime_new[j] = (double)Statistics_new[63+j] * pow(2.0, 32.0);
// 		      LiveTime_new[j] += (double)Statistics_new[79+j];
// 		      LiveTime_new[j] *= 1.0e-6 / (double)100;
// 		      FastPeaks_new[j] = (double)Statistics_new[95+j] * pow(2.0, 32.0);
// 		      FastPeaks_new[j] += (double)Statistics_new[111+j];
// 		      ChanEvents_new[j] = (double)Statistics_new[223+j] * pow(2.0, 32.0);
// 		      ChanEvents_new[j] += (double)Statistics_new[239+j];

// 		      std::cout<<j<<"  "<<LiveTime_new[j] <<"  "<<(FastPeaks_new[j]-FastPeaks[j])/(LiveTime_new[j]-LiveTime[j])<<"  "<<(ChanEvents_new[j]-ChanEvents[j])/(RealTime_new-RealTime)<<std::endl;
// 		    }


// 		}

// 	      memcpy(buf,buf_new,(PRESET_MAX_MODULES*448*4)+10);
// 	    }
// 	  else
// 	    {
// 	      printf("...\n");
// 	      sleep(1);
// 	      continue;
// 	    }
// 	  // memcpy(&ModNum,ptr+4,2);
	 

// 	}
//     }
//   // memcpy(buf,ptr,(PRESET_MAX_MODULES*448*4)+6);

//   delete []buf;
//   delete []buf_new;
//   return 0;
// }


// 
// main.cc ends here











