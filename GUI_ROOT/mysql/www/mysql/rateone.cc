// rateone.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: Tue Feb 11 22:58:42 2020 (+0800)
// Last-Updated: 六 9月 18 15:15:46 2021 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 18
// URL: http://wuhongyi.cn

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
  printf("Content-type:text/html\n\n");
  
  char * data;
  std::string label, values;
   
  data = getenv("QUERY_STRING");                           // retrieve webpage arguments   
  std::string webdata(data);                               // turn into string

  // printf("input data: %s\n",data);

  int n0,n1,n2,n3,n4;

  n0 = webdata.find("&v0="); 
  n1 = webdata.find("&v1=");
  n2 = webdata.find("&v2=");
  n3 = webdata.find("&v3=");
  n4 = webdata.find("&v4=");
  // printf("%d  %d  %d  %d\n",n0,n1,n2,n3);

  std::string s0 = webdata.substr(n0+4, n1-n0-4);
  std::string s1 = webdata.substr(n1+4, n2-n1-4);
  std::string s2 = webdata.substr(n2+4, n3-n2-4);
  std::string s3 = webdata.substr(n3+4, n4-n3-4);
  std::string s4 = webdata.substr(n4+4);

  // printf("%s  %s  %s  %s\n",s0.c_str(),s1.c_str(), s2.c_str(), s3.c_str());

  int data0 = atoi(s0.c_str());
  int data1 = atoi(s1.c_str());
  int data2 = atoi(s2.c_str());
  int data3 = atoi(s3.c_str());
  int data4 = atoi(s4.c_str());
  // printf("%d  %d  %d  %d\n",data0,data1,data2,data3);

  if(data0<0 || data0>7) {printf("1,"); return 0;}
  else if(data1<0 || data1>12) {printf("2,"); return 0;}
  else if(data2<0 || data2>15) {printf("3,"); return 0;}
  else if(data3<0 || data3>1) {printf("4,"); return 0;}
  else if(data4<0 || data4>9999) {printf("5,"); return 0;}
  else printf("0,");


  
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

  mysql_real_connect(conn, "162.105.54.101", "data", "xia17pxn", "GDDAQ", 3306, NULL, 0);
 
  //mysql_query(conn, "select * from statistics order by ts desc limit 2");
  char chartmp[1024];
  // mysql_query(conn, "select * from statistics where run >= 0 and run = 2");
  sprintf(chartmp,"select * from statistics where crate = %d and run = %d",data0,data4);
  mysql_query(conn, chartmp);
  result = mysql_store_result(conn);
  if (result == NULL) {
    printf("%d:%s\n", mysql_errno(conn), mysql_error(conn));
    goto out;
  }
  printf("%d,",mysql_num_rows(result)-1);
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
	memcpy(ts,row[0],lengths[0]);
	nmod = atoi(row[3]);
	run = atoi(row[2]);
	crate = atoi(row[1]);
      }
    else
      {
	memcpy(ts_new,row[0],lengths[0]);
	nmod_new = atoi(row[3]);
	run_new = atoi(row[2]);
	crate_new = atoi(row[1]);
      }
    memcpy(szrate,row[4],lengths[4]);
    memcpy(szstat,row[5],lengths[5]);

    nrate = 48;
    nstat = 32768;

    if(counter==0)
      {
	uncompress(rate, &nrate, szrate, lengths[4]);
	uncompress(stat, &nstat, szstat, lengths[5]);
      }
    else
      {
	memcpy(rate,rate_new,nrate);
	memcpy(stat,stat_new,nstat);
	uncompress(rate_new, &nrate, szrate, lengths[4]);
	uncompress(stat_new, &nstat, szstat, lengths[5]);


	int i = data1;
	int j = data2;
    
	memcpy(&tempsampingrate,rate_new+2*i,2);
	memcpy(Statistics,stat+4*SHAREDMEMORYDATASTATISTICS*i,SHAREDMEMORYDATASTATISTICS*4);
	memcpy(Statistics_new,stat_new+4*SHAREDMEMORYDATASTATISTICS*i,SHAREDMEMORYDATASTATISTICS*4);

	RealTime = (double)Statistics[2] * std::pow(2.0, 32.0);
	RealTime += (double)Statistics[3];
	RealTime *= 1.0e-6 / (double)SYSTEM_CLOCK_MHZ;

	RealTime_new = (double)Statistics_new[2] * std::pow(2.0, 32.0);
	RealTime_new += (double)Statistics_new[3];
	RealTime_new *= 1.0e-6 / (double)SYSTEM_CLOCK_MHZ;


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


	tempinputcountrate = int((FastPeaks_new[j]-FastPeaks[j])/(LiveTime_new[j]-LiveTime[j]) + 0.5);
	tempoutputcountrate = int((ChanEvents_new[j]-ChanEvents[j])/(RealTime_new-RealTime) +0.5);

	if(data3 == 0)
	  printf("%s,%d,",row[0],tempinputcountrate);
	else
	  printf("%s,%d,",row[0],tempoutputcountrate);
	
      }
    // printf("data length:  %d  %d \n",nrate,nstat);

    counter++;


    
  }
  
  // int point = 1000;
  // printf("%d,",point);
  // for(int i = 0; i < point; i++)
  //   {
  //     printf("%d, %d,",i,i*i);

  //   }


  // ============================================================

 out:
  mysql_free_result(result);
  mysql_close(conn);
  mysql_library_end();
  
  return 0;
}

// 
// rateone.cc ends here
