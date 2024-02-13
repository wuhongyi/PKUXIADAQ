// readds.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 2月 10 22:58:45 2020 (+0800)
// Last-Updated: 二 2月 11 19:42:25 2020 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 37
// URL: http://wuhongyi.cn 


// g++ readds.cc `mysql_config --cflags --libs` -lz -o readds

#include <mysql/mysql.h>
#include <zlib.h>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <string>
int main()
{
  MYSQL       *conn;
  MYSQL_RES   *result;
  MYSQL_ROW    row;
  char        *w;
 
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
 
  while ((row = mysql_fetch_row(result))) {

    unsigned long *lengths;
    unsigned int num_fields;

    unsigned char szstat[32768];
    unsigned char szrate[48];

    unsigned char stat[32768];
    unsigned char rate[48];
    unsigned long nrate = 48;
    unsigned long nstat = 32768;
    
    num_fields = mysql_num_fields(result);
    lengths = mysql_fetch_lengths(result);
    // for(int i = 0; i < num_fields; i++)
    //   {
    // 	printf("Column %u is %lu bytes in length.\n", i, lengths[i]);
    //   }
    
    w = row[0];
    std::cout<<row[0]<<"  "<<row[1]<<"  "<<row[2]<<"  "<<row[3]<<std::endl;
    int N = atoi(row[3]);

    memcpy(szrate,row[4],lengths[4]);
    memcpy(szstat,row[5],lengths[5]);

    nrate = 48;
    nstat = 32768;
    uncompress(rate, &nrate, szrate, lengths[4]);
    uncompress(stat, &nstat, szstat, lengths[5]);
    printf("data length:  %d  %d \n",nrate,nstat);
    

    // for (int i = 0; i < lengths[4]; ++i)
    //   {
    // 	printf("%02x ",szrate[i]);
    //   }
    // printf("\n");
    // for (int i = 0; i < nrate; ++i)
    //   {
    // 	printf("%02x ",rate[i]);
    //   }
    // printf("\n");

    
    // for (int i = 0; i < lengths[5]; ++i)
    //   {
    // 	printf("%02x ",szstat[i]);
    //   }
    // printf("\n");
    // for (int i = 0; i < nstat; ++i)
    //   {
    // 	printf("%02x ",stat[i]);
    //   }
    // printf("\n");

    
  }
 
 out:
  mysql_free_result(result);
  mysql_close(conn);
  mysql_library_end();
  // This function finalizes the MySQL library.
  // Call it when you are done using the library (for example, after disconnecting from the server).
  // The action taken by the call depends on whether your application is linked to the MySQL client library or the MySQL embedded server library.
  // For a client program linked against the libmysqlclient library by using the -lmysqlclient flag, mysql_library_end() performs some memory management to clean up.
  
  return 0;
}


// 
// readds.cc ends here
