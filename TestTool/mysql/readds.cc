// readds.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 2月 10 22:58:45 2020 (+0800)
// Last-Updated: 一 2月 10 23:23:13 2020 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 16
// URL: http://wuhongyi.cn 


// g++ readds.cc `mysql_config --cflags --libs` -o readds

#include <mysql/mysql.h>
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
  mysql_real_connect(conn, "222.29.111.178", "data", "123456", "GDDAQ", 3306, NULL, 0);
 
  mysql_query(conn, "select * from statistics order by ts desc limit 2");
 
  result = mysql_store_result(conn);
  if (result == NULL) {
    printf("%d:%s\n", mysql_errno(conn), mysql_error(conn));
    goto out;
  }
 
  while ((row = mysql_fetch_row(result))) {
    w = row[0];
    std::cout<<row[0]<<"  "<<row[1]<<"  "<<row[2]<<"  "<<row[3]<<std::endl;
    int N = atoi(row[3]);
    unsigned char stat[65536];
    unsigned char rate[48];
    memcpy(rate,row[4],N*2);
    memcpy(stat,row[5],N*4*448);
    for (int i = 0; i < N*2; ++i)
      {
	printf("%02x ",rate[i]);
      }
    printf("\n");
    for (int i = 0; i < N*4*448; ++i)
      {
	printf("%02x ",stat[i]);
      }
    printf("\n");

    
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
