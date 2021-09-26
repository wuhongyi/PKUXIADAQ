// readfileinfo.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: Wed Feb 12 22:47:33 2020 (+0800)
// Last-Updated: 六 9月 18 15:15:26 2021 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 3
// URL: http://wuhongyi.cn 

// g++ readfileinfo.cc `mysql_config --cflags --libs` -o readfileinfo.cgi

#include <mysql/mysql.h>
#include <zlib.h>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <string>
#include <cmath>

int main(void)
{
  printf("Content-type:text/html\n\n");
  
  MYSQL       *conn;
  MYSQL_RES   *result;
  MYSQL_ROW    row;

  conn = mysql_init(NULL);
  my_bool reconnect = 1;
  mysql_options(conn, MYSQL_OPT_RECONNECT, &reconnect);

  mysql_real_connect(conn, "162.105.54.101", "data", "xia17pxn", "GDDAQ", 3306, NULL, 0);
 
  mysql_query(conn, "select * from fileinfo order by ts desc limit 1");
 
  result = mysql_store_result(conn);
  if (result == NULL) {
    printf("%d:%s\n", mysql_errno(conn), mysql_error(conn));
    goto out;
  }

 while ((row = mysql_fetch_row(result))) {

    unsigned long *lengths;
    unsigned int num_fields;


    num_fields = mysql_num_fields(result);
    lengths = mysql_fetch_lengths(result);
    // for(int i = 0; i < num_fields; i++)
    //   {
    // 	printf("Column %u is %lu bytes in length.\n", i, lengths[i]);
    //   }
    
    // std::cout<<row[0]<<"  "<<row[1]<<"  "<<row[2]<<"  "<<row[3]<<std::endl;

    for (int i = 0; i < num_fields; ++i)
      {
	printf("%s,",row[i]);
      }
    // printf("%s,%d,%d,%d,",ts_new,crate_new,run_new,nmod);
  }


  
 out:
  mysql_free_result(result);
  mysql_close(conn);
  mysql_library_end();

  return 0;
}


// 
// readfileinfo.cc ends here
