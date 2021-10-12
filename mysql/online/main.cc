// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 8月 29 20:55:23 2019 (+0800)
// Last-Updated: 六 9月 18 15:09:50 2021 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 112
// URL: http://wuhongyi.cn 

// uncompress(output,&nn,input,length)；
// //注：当nn的取值过小时会出现解压的数据只解压了前面一部分的情况。因此，压缩的数据过短时，需要人为补长数据

// g++ main.cc `mysql_config --cflags --libs` -lrt -lz  -o 123
// https://dev.mysql.com/doc/refman/5.6/en/c-api.html

#include "UserDefine.hh"
#include <mysql/mysql.h>
#include <iostream>
#include <ctime>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <semaphore.h>
#include <errno.h>
#include <climits>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <zlib.h>
//#include <cstdint> // include this header for uint64_t
using namespace std;

#define PRESET_MAX_MODULES 24

#define SHAREDMEMORYDATAOFFSET 1170 //BYTE
// 1st 4    bytes IDcode for event shared memory
// 2nd 2    bytes number of valid Num Modules in shared memory
// 3rd 4    bytes Crate ID/Run Number  [31:24]  [23:0]
// 4   4    bytes ID for update spectrum
// 5   128  bytes name of data file
// 6   1024 bytes path of data file
// 7   2    bytes software id
// 8   2    bytes software version
#define SHAREDMEMORYDATASTATISTICS 448
#define SHAREDMEMORYDATAENERGYLENGTH 32768
#define SHAREDMEMORYDATAMAXCHANNEL 16


#define SYSTEM_CLOCK_MHZ 100// System FPGA clock frequency in MHz (used for real time counting)

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

long get_time()
{
  long time_ms;
  struct timeval t1;
  struct timezone tz;
  gettimeofday(&t1, &tz);
  time_ms = (t1.tv_sec) * 1000 + t1.tv_usec / 1000;
  return time_ms;
}

double GetFileSizeMB(const char *name)
{
  struct stat statbuff;  
  if(stat(name, &statbuff) < 0){  
    printf("errno：%d\n",errno);
    printf("ERR  ：%s\n",strerror(errno));
    return 0;  
  }else{  
    return statbuff.st_size/(1024.0*1024.0);
  }
}

bool IsFileExists(const char *name)
{
  if((access(name,F_OK))!=-1)  
    {  
      return true;
    }  
  else  
    {  
      return false;
    } 
}

int main(int argc, char *argv[])
{
  uint64_t CurrentTime;
  uint64_t PrevTime;
  uint64_t ElapsedTime;
  uint64_t CurrentProtectionTime;
  uint64_t PrevProtectionTime;
  uint64_t ElapsedProtectionTime; 

  unsigned int number = UINT_MAX;
  unsigned int tempuint;
  unsigned short tempushort;

  char filepath[1024];
  char filename[128];

  
  int shm_id;
  unsigned char *ptr;
  sem_t *sem;
  int val;
  // int rc;
  unsigned char *buf;  
  buf = new unsigned char[(PRESET_MAX_MODULES*SHAREDMEMORYDATASTATISTICS*4)+PRESET_MAX_MODULES*2+SHAREDMEMORYDATAOFFSET];

  shm_id = shm_open("shmpixie16pkuxiadaq",O_RDWR,0);/*第一步：打开共享内存区*/
  if (shm_id == -1)
    {
      printf( "open shared memory error.errno=%d,desc=%s.\n", errno, strerror(errno));
      return -1;
    }
  else
    {
      printf( "open shared memory ok.\n");
    }

  sem = sem_open("sempixie16pkuxiadaq",1);/*打开信号量*/
  if (sem == SEM_FAILED)
    {
      printf( "open semaphore error.errno=%d,desc=%s.\n", errno, strerror(errno));
      return -1;
    }
  else
    {
      printf( "open semaphore ok.\n");
    }

  ftruncate(shm_id,(off_t)(SHAREDMEMORYDATAOFFSET+PRESET_MAX_MODULES*2+PRESET_MAX_MODULES*4*SHAREDMEMORYDATASTATISTICS+PRESET_MAX_MODULES*4*SHAREDMEMORYDATAENERGYLENGTH*SHAREDMEMORYDATAMAXCHANNEL));
  
  ptr = (unsigned char*)mmap(NULL,SHAREDMEMORYDATAOFFSET+PRESET_MAX_MODULES*2+(PRESET_MAX_MODULES*SHAREDMEMORYDATASTATISTICS*4)+PRESET_MAX_MODULES*4*SHAREDMEMORYDATAENERGYLENGTH*SHAREDMEMORYDATAMAXCHANNEL,PROT_READ|PROT_WRITE,MAP_SHARED,shm_id,0);/*连接共享内存区*/


  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  

  MYSQL       *mysql;//mysql数据库连接句柄。在执行任何数据库操作之前首先就需要创建一个MYSQL结构。
  
  mysql = mysql_init(NULL);

  // mysql_options(mysql,MYSQL_SET_CHARSET_NAME ,"utf8");

#if MYSQL_VERSION_ID < 80000  //version 5.6/5.7
  my_bool reconnect = 1;
#else // version 8.0
  bool reconnect = 1;
#endif  
  mysql_options(mysql, MYSQL_OPT_RECONNECT, &reconnect);
  mysql_options(mysql,MYSQL_OPT_COMPRESS,0);
  
  if (!mysql_real_connect(mysql,HOST_NAME,USER_NAME,USER_PASSWD,"GDDAQ",0,NULL,0))
    {
      fprintf(stderr, "Failed to connect to database GDDAQ: Error: %s\n", mysql_error(mysql));
      return -1;
    }


  unsigned char rate[2*PRESET_MAX_MODULES];
  unsigned char stat[32768];

  unsigned char szrate[2*PRESET_MAX_MODULES];
  unsigned long nszrate = 26;
  unsigned char szstat[32768];
  unsigned long nszstat = 23296;
  int crate = -1;
  int run = -1;
  int nmod = 0;
  char sql[65536], *end;
  char tempchar[4096];
  // const char *data = "INSERT INTO statistics (ts,crate,run,nmod,rate,stat) VALUES (NOW(),%d,%d,%d,%s,%s);";
  // const char *data = "INSERT INTO statistics (ts,crate,run,nmod,rate,stat) VALUES (NOW(),%d,%d,%d,%s,%s);";


  
  // int flagi;
  // for (int i = 0; i < 100; i++)
  //   {
  //     flagi = 10-i%10;
      
  //     crate = 0;
  //     run = i;
  //     nmod = flagi;
  //     memcpy(rate,tempdata,flagi*2);
  //     memcpy(stat,tempdata,flagi*448*4);
  //     rate[flagi*2] = '\0';
  //     stat[flagi*448*4] = '\0';
  //     sprintf(sql, data, crate,run,nmod,rate,stat);
      
  //     if(mysql_real_query(mysql, sql,strlen(sql)))
  // 	{
  // 	  fprintf(stderr, "Failed to insert to database: Error: %s\n", mysql_error(mysql));
  // 	}
  //     // printf("%s\n", sql);
  //   }
  

  PrevProtectionTime = get_time();
  PrevTime = PrevProtectionTime;
  
  while(true)
    {
      CurrentTime = get_time();
      ElapsedTime = CurrentTime - PrevTime; /* milliseconds */
      if (ElapsedTime > 1000)
	{
	  if(nmod > 0 && number!=UINT_MAX)
	    {
	      float filesize[13];
	      for (int i = 0; i < 13; ++i)
		{
		  char tempfilename[2048];
		  if(i < nmod)
		    {
		      sprintf(tempfilename,"%s%04d/%s_R%04d_M%02d.bin",filepath,run,filename,run,i);
		      if(IsFileExists(tempfilename))
			filesize[i] = float(GetFileSizeMB(tempfilename));
		      else
			filesize[i] = 0;
		    }
		  else
		    {
		      filesize[i] = 0;
		    }
		}

	      // df -hl %s | awk 'NR>1{print $3}'
	      // df -hl %s | awk 'NR>1{print $4}'
	      // df -hl %s | awk 'NR>1{print $5}'
	      sprintf(tempchar,"INSERT INTO fileinfo  VALUES (NOW(),%d,%d,%d,'%s','%s',%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f)",crate,run,nmod,filename,filepath,filesize[0],filesize[1],filesize[2],filesize[3],filesize[4],filesize[5],filesize[6],filesize[7],filesize[8],filesize[9],filesize[10],filesize[11],filesize[12]);
	      // printf("%s\n",tempchar);
	      if(mysql_real_query(mysql, tempchar,strlen(tempchar)))//strlen(sql)
		{
		  fprintf(stderr, "Failed to insert to database table fileinfo: Error: %s\n", mysql_error(mysql));
		}
	      else
		{
		  printf("insert to table fileinfo done.\n");
		}

	      
	    }
	  PrevTime = CurrentTime;
	}



      sem_getvalue(sem, &val);
      if(val > 0)
	{
	  memcpy(&tempuint,ptr,4);
	  if(number != tempuint)
	    {
	      PrevProtectionTime = get_time();
	      memcpy(buf,ptr,(PRESET_MAX_MODULES*SHAREDMEMORYDATASTATISTICS*4)+PRESET_MAX_MODULES*2+SHAREDMEMORYDATAOFFSET);
	      
	      if(number == UINT_MAX) 
		{
		  number = tempuint;
		  continue;
		}
	      
	      number = tempuint;
	      // for (int i = 0; i < (PRESET_MAX_MODULES*SHAREDMEMORYDATASTATISTICS*4)+PRESET_MAX_MODULES*2+SHAREDMEMORYDATAOFFSET; ++i)
	      // 	{
	      // 	  printf("%02x ",buf[i]);
	      // 	}
	      // printf("\n");

	      memcpy(filename,buf+14,128);
	      memcpy(filepath,buf+142,1024);
	      
	      memcpy(&tempushort,buf+4,2);
	      nmod = tempushort;
	      memcpy(&tempuint,buf+6,4);
	      run = tempuint & 0xFFFFFF;
	      crate = (tempuint & 0xFF000000)>>24;
	      
	      memcpy(rate,buf+SHAREDMEMORYDATAOFFSET,PRESET_MAX_MODULES*2);//nmod*2
	      memcpy(stat,buf+SHAREDMEMORYDATAOFFSET+PRESET_MAX_MODULES*2,nmod*SHAREDMEMORYDATASTATISTICS*4);

	      // rate[nmod*2] = '\0';
	      // stat[nmod*SHAREDMEMORYDATASTATISTICS*4] = '\0';
	      // nszrate = nmod*2;
	      // nszstat = nmod*SHAREDMEMORYDATASTATISTICS*4;
	      compress(szrate, &nszrate, rate, PRESET_MAX_MODULES*2);
	      compress(szstat, &nszstat, stat, nmod*SHAREDMEMORYDATASTATISTICS*4);
	      // szrate[nszrate] = '\0';
	      // szstat[nszstat] = '\0';

	      // for (int i = 0; i < nszstat; ++i)
	      // 	{
	      // 	  printf("%02x ",szstat[i]);
	      // 	}
	      // printf("\n");

	     
	      
	      // rate[nmod*2] = '\0';
	      // stat[nmod*SHAREDMEMORYDATASTATISTICS*4] = '\0';
	      // for (int i = 0; i < nmod*2; ++i)
	      // 	{
	      // 	  printf("%x ",rate[i]);
	      // 	}
	      // printf("\n");
	      
	      // printf("%d  %d %x\n",nszrate,strlen((char*)szrate),szrate);
	      // printf("%d  %d %x\n",nszstat,strlen((char*)szstat),szstat);
	      // sprintf(sql, data, crate,run,nmod,szrate,szstat);
	      
	      sprintf(tempchar,"INSERT INTO statistics (ts,crate,run,nmod,rate,stat) VALUES (NOW(),%d,%d,%d,",crate,run,nmod);
	      // end = strmov(sql,tempchar);
	      strcpy(sql,tempchar);
	      end = sql+strlen(tempchar);
	      *end++ = '\'';
	      end += mysql_real_escape_string(mysql,end,(char*)szrate,nszrate);
	      *end++ = '\'';
	      *end++ = ',';
	      *end++ = '\'';
	      end += mysql_real_escape_string(mysql,end,(char*)szstat,nszstat);
	      *end++ = '\'';
	      *end++ = ')';
	      
	      
	      if(mysql_real_query(mysql, sql,(unsigned int)(end-sql)))//strlen(sql)
		{
		  fprintf(stderr, "Failed to insert to database table statistics: Error: %s\n", mysql_error(mysql));
		}
	      else
		{
		  printf("insert to table statistics done.\n");
		}
	      
	      // printf("%s\n", sql);
	      // for (int i = 0; i < ( int)(end-sql); ++i)
	      // 	{
	      // 	  printf("%02x ",sql[i]);
	      // 	}
	      // printf("\n");
	    }
	}
      else
	{
	  CurrentProtectionTime = get_time();
	  ElapsedProtectionTime = CurrentProtectionTime - PrevProtectionTime;
	  if(ElapsedProtectionTime > 5000)
	    {

	      printf("NOT SHARE DATA\n");
	    }
	}


      usleep(400000);//sleep 400us
    }





  

  mysql_close(mysql);
  mysql_library_end();
  
  return 0;
}

// 
// main.cc ends here
