// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 8月 29 20:55:23 2019 (+0800)
// Last-Updated: 六 2月  8 23:49:05 2020 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 2
// URL: http://wuhongyi.cn 

//  g++ main.cc `mysql_config --cflags --libs` -o 123
// https://dev.mysql.com/doc/refman/5.6/en/c-api.html

#include <mysql/mysql.h>
#include <iostream>
#include <ctime>
#include <string>
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

  char *data = "INSERT INTO gddaqshm () VALUES ()";
  if(!mysql_real_query(mysql, const char *stmt_str, unsigned long length))
    {
      fprintf(stderr, "Failed to INSERT to database: Error: %s\n", mysql_error(mysql));
    }


  

  mysql_close(mysql);
  mysql_library_end();
  
  return 0;
}

// 
// main.cc ends here
