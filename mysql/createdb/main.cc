// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 8月 29 21:53:45 2019 (+0800)
// Last-Updated: 三 2月 12 22:17:29 2020 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 24
// URL: http://wuhongyi.cn 

// g++ main.cc `mysql_config --cflags --libs` -o 123

// https://dev.mysql.com/doc/refman/5.6/en/c-api.html

#include "UserDefine.hh"
#include <mysql/mysql.h>
#include <iostream>
#include <ctime>
#include <string>
#include <cstdio>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char *argv[])
{
  MYSQL       *mysql;

  mysql = mysql_init(NULL);

  mysql_options(mysql,MYSQL_SET_CHARSET_NAME ,"utf8");
  my_bool reconnect = 1;
  mysql_options(mysql, MYSQL_OPT_RECONNECT, &reconnect);
  mysql_options(mysql,MYSQL_OPT_COMPRESS,0);
  
  if (!mysql_real_connect(mysql,HOST_NAME,USER_NAME,USER_PASSWD,NULL,0,NULL,0))
    {
      fprintf(stderr, "Failed to connect to database: Error: %s\n", mysql_error(mysql));
    }

  if(mysql_query(mysql, "CREATE DATABASE IF NOT EXISTS GDDAQ;"))
    {
      fprintf(stderr, "Failed to create database GDDAQ: Error: %s\n", mysql_error(mysql));
    }

  if(mysql_select_db(mysql,"GDDAQ"))
    {
      fprintf(stderr, "Failed to select database GDDAQ: Error: %s\n", mysql_error(mysql));
    }

  
  if(mysql_query(mysql, "DROP TABLE IF EXISTS statistics,spectrum,fileinfo;"))
    {
      fprintf(stderr, "Failed to drop tables: Error: %s\n", mysql_error(mysql));
    }



  
  FILE *fp;
  char sql[4096];
  
  fp = fopen("statistics.sql", "r");
  fread(sql, 1, 4096, fp);
  fclose(fp);
  if(mysql_query(mysql, sql))
    {
      fprintf(stderr, "Failed to create tables statistics: Error: %s\n", mysql_error(mysql));
    }
  
  fp = fopen("fileinfo.sql", "r");
  fread(sql, 1, 4096, fp);
  fclose(fp);
  if(mysql_query(mysql, sql))
    {
      fprintf(stderr, "Failed to create tables fileinfo: Error: %s\n", mysql_error(mysql));
    }


  
  // mysql_free_result(result);
  mysql_close(mysql);
  mysql_library_end();
  // This function finalizes the MySQL library.
  // Call it when you are done using the library (for example, after disconnecting from the server).
  // The action taken by the call depends on whether your application is linked to the MySQL client library or the MySQL embedded server library.
  // For a client program linked against the libmysqlclient library by using the -lmysqlclient flag, mysql_library_end() performs some memory management to clean up.

  
  return 0;
}

// 
// main.cc ends here
