// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 8月 29 20:55:23 2019 (+0800)
// Last-Updated: 日 2月  9 01:04:30 2020 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 12
// URL: http://wuhongyi.cn 

//  g++ main.cc `mysql_config --cflags --libs` -o 123
// https://dev.mysql.com/doc/refman/5.6/en/c-api.html

#include <mysql/mysql.h>
#include <iostream>
#include <ctime>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
using namespace std;
int main(int argc, char *argv[])
{
  MYSQL       *mysql;//mysql数据库连接句柄。在执行任何数据库操作之前首先就需要创建一个MYSQL结构。
  
  mysql = mysql_init(NULL);
  if (!mysql_real_connect(mysql,"162.105.54.92","data","123456","GDDAQ",0,NULL,0))
    {
      fprintf(stderr, "Failed to connect to database: Error: %s\n", mysql_error(mysql));
    }

  const char *ins = "INSERT INTO gddaqshm VALUES ('%s', %d,"
    " %d, %d, %d, %10.2f, '%s', '%s', '1997-01-15 20:16:28',"
    " '1999-01-15 20:16:28', '%s', '%s')";
  
  const char *data = "INSERT INTO gddaqshm (dataset,run,timef,timel,rawfilepath) VALUES ('abcdabcd',102,now(),now(),'/home/wuhongyi/data/');";
   char dataset[32];
   char rawfile[128];
   int run;
   char sql[4096];
   for (int i = 0; i < 100; i++) {
      sprintf(dataset, "testrun_%d", i);
      sprintf(rawfile, "/v1/data/lead/test/run_%d.root", i);
      run = i;
      sprintf(sql, ins, dataset, i, evt, evt+10000, tag, 25.5, "test", "lead",
              rawfile, "test run dummy data");
      evt += 10000;
      res = db->Query(sql);
      delete res;
      //printf("%s\n", sql);
   }
  
  if(mysql_real_query(mysql, data,strlen(data)))
    {
      fprintf(stderr, "Failed to insert to database: Error: %s\n", mysql_error(mysql));
    }



  

  mysql_close(mysql);
  mysql_library_end();
  
  return 0;
}

// 
// main.cc ends here
