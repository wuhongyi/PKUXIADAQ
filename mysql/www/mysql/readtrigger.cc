// readtrigger.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: Tue Feb 11 22:58:42 2020 (+0800)
// Last-Updated: Wed Feb 12 00:39:56 2020 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 17
// URL: http://wuhongyi.cn 

// g++ readtrigger.cc `mysql_config --cflags --libs` -lz -o readtrigger.cgi

// #include <stdio.h>
// #include <unistd.h>
// #include <stdlib.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <time.h>
// #include <signal.h>
// #include <errno.h>
// #include <string.h>
// #include <sys/mman.h>
// #include <sys/file.h>

// #include <map>
// #include <vector>
// #include <string>
// #include <sstream>
// #include <fstream>
// #include <iostream>
// #include <algorithm>

#include <mysql/mysql.h>
#include <zlib.h>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <string>
#include <cmath>

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


int main(void)
{
  // char * data;
  // string label, values;
   
  // data = getenv("QUERY_STRING");                           // retrieve webpage arguments   
  // std::string webdata(data);                               // turn into string

  // printf("input data: %s\n",data);

  unsigned int Statistics[SHAREDMEMORYDATASTATISTICS];
  unsigned int Statistics_new[SHAREDMEMORYDATASTATISTICS];
  unsigned short tempsampingrate;
  int tempinputcountrate,tempoutputcountrate;// double
  double RealTime_new;
  double LiveTime_new[16];
  double FastPeaks_new[16];
  double ChanEvents_new[16];
  double RealTime;
  double LiveTime[16];
  double FastPeaks[16];
  double ChanEvents[16];

  unsigned char szstat[32768];
  unsigned char szrate[48];

  unsigned char stat[32768];
  unsigned char rate[48];
  unsigned char stat_new[32768];
  unsigned char rate_new[48];
  
  unsigned long nrate = 48;
  unsigned long nstat = 32768;

  unsigned char ts[128];
  unsigned char ts_new[128];
  int run;
  int run_new;
  int nmod;
  int nmod_new;
  int crate;
  int crate_new;

  
  MYSQL       *conn;
  MYSQL_RES   *result;
  MYSQL_ROW    row;
  // char        *w;

  int counter;
  
  conn = mysql_init(NULL);
  my_bool reconnect = 1;
  mysql_options(conn, MYSQL_OPT_RECONNECT, &reconnect);

  mysql_real_connect(conn, "222.29.111.140", "data", "xia17pxn", "GDDAQ", 3306, NULL, 0);
 
  mysql_query(conn, "select * from statistics order by ts desc limit 2");
 
  result = mysql_store_result(conn);
  if (result == NULL) {
    printf("%d:%s\n", mysql_errno(conn), mysql_error(conn));
    goto out;
  }

  counter = 0;
  while ((row = mysql_fetch_row(result))) {

    unsigned long *lengths;
    unsigned int num_fields;


    
    num_fields = mysql_num_fields(result);
    lengths = mysql_fetch_lengths(result);
    // for(int i = 0; i < num_fields; i++)
    //   {
    // 	printf("Column %u is %lu bytes in length.\n", i, lengths[i]);
    //   }
    
    // w = row[0];
    // std::cout<<row[0]<<"  "<<row[1]<<"  "<<row[2]<<"  "<<row[3]<<std::endl;
    if(counter==0)
      {
	memcpy(ts_new,row[0],lengths[0]);
	nmod_new = atoi(row[3]);
	run_new = atoi(row[2]);
	crate_new = atoi(row[1]);
      }
    else
      {
	memcpy(ts,row[0],lengths[0]);
	nmod = atoi(row[3]);
	run = atoi(row[2]);
	crate = atoi(row[1]);
      }
    memcpy(szrate,row[4],lengths[4]);
    memcpy(szstat,row[5],lengths[5]);

    nrate = 48;
    nstat = 32768;

    if(counter==0)
      {
	uncompress(rate_new, &nrate, szrate, lengths[4]);
	uncompress(stat_new, &nstat, szstat, lengths[5]);
      }
    else
      {
	uncompress(rate, &nrate, szrate, lengths[4]);
	uncompress(stat, &nstat, szstat, lengths[5]);
      }
    // printf("data length:  %d  %d \n",nrate,nstat);

    counter++;
  }


  // ==================================================================================

  // ts,crate,run,nmod,r0,r1,...,r12
  printf("%s,%d,%d,%d,",ts_new,crate_new,run_new,nmod);
  for (int i = 0; i < 13; ++i)
    {
      if(i < nmod_new)
	{
	  memcpy(&tempsampingrate,rate_new+2*i,2);
	  printf("%d,",tempsampingrate);
	}
      else
	{
	  printf(" ,");
	}
    }

  printf(";");

  for(int i = 0; i < 13;i++)
    {
      if(i< nmod_new)
	{      
	  memcpy(&tempsampingrate,rate_new+2*i,2);
	  memcpy(Statistics,stat+4*SHAREDMEMORYDATASTATISTICS*i,SHAREDMEMORYDATASTATISTICS*4);
	  memcpy(Statistics_new,stat_new+4*SHAREDMEMORYDATASTATISTICS*i,SHAREDMEMORYDATASTATISTICS*4);

	  RealTime = (double)Statistics[2] * std::pow(2.0, 32.0);
	  RealTime += (double)Statistics[3];
	  RealTime *= 1.0e-6 / (double)SYSTEM_CLOCK_MHZ;

	  RealTime_new = (double)Statistics_new[2] * std::pow(2.0, 32.0);
	  RealTime_new += (double)Statistics_new[3];
	  RealTime_new *= 1.0e-6 / (double)SYSTEM_CLOCK_MHZ;

	  for(int j=0;j<16;j++)
	    {
	      LiveTime[j] = (double)Statistics[63+j] * std::pow(2.0, 32.0);
	      LiveTime[j] += (double)Statistics[79+j];
		      
	      FastPeaks[j] = (double)Statistics[95+j] * std::pow(2.0, 32.0);
	      FastPeaks[j] += (double)Statistics[111+j];
	      ChanEvents[j] = (double)Statistics[223+j] * std::pow(2.0, 32.0);
	      ChanEvents[j] += (double)Statistics[239+j];

	      LiveTime_new[j] = (double)Statistics_new[63+j] * pow(2.0, 32.0);
	      LiveTime_new[j] += (double)Statistics_new[79+j];

	      if(tempsampingrate == 100)
		{
		  LiveTime[j] *= 1.0e-6 / (double)tempsampingrate;
		  LiveTime_new[j] *= 1.0e-6 / (double)tempsampingrate;
		}
	      else if(tempsampingrate == 250)
		{
		  LiveTime[j] *= 2.0 * 1.0e-6 / (double)tempsampingrate;
		  LiveTime_new[j] *= 2.0 * 1.0e-6 / (double)tempsampingrate;
		}
	      else if(tempsampingrate == 500)
		{
		  LiveTime[j] *= 5.0 * 1.0e-6 / (double)tempsampingrate;
		  LiveTime_new[j] *= 5.0 * 1.0e-6 / (double)tempsampingrate;
		}
		      
	      FastPeaks_new[j] = (double)Statistics_new[95+j] * std::pow(2.0, 32.0);
	      FastPeaks_new[j] += (double)Statistics_new[111+j];
	      ChanEvents_new[j] = (double)Statistics_new[223+j] * std::pow(2.0, 32.0);
	      ChanEvents_new[j] += (double)Statistics_new[239+j];

	      if(run_new=run)
		{
		  tempinputcountrate = int((FastPeaks_new[j]-FastPeaks[j])/(LiveTime_new[j]-LiveTime[j]) + 0.5);
		  tempoutputcountrate = int((ChanEvents_new[j]-ChanEvents[j])/(RealTime_new-RealTime) +0.5);
		}
	      else
		{
		  tempinputcountrate = int((FastPeaks_new[j])/(LiveTime_new[j]) + 0.5);
		  tempoutputcountrate = int((ChanEvents_new[j])/(RealTime_new) +0.5);
		}
	      printf("%d,%d,",tempinputcountrate,tempoutputcountrate);
	    }
	}
      else
	{
	  for(int j=0;j<16;j++)
	    printf(" , ,");
	}
      printf(";");
    }//for



  // ============================================================
  
 out:
  mysql_free_result(result);
  mysql_close(conn);
  mysql_library_end();





  // printf("%d,%d,%d,%d\n",monitor[0],monitor[1],monitor[2],monitor[3]);
  
  return 0;
}

// 
// readtrigger.cc ends here
