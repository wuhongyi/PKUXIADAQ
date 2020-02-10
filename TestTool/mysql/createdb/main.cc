// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 8月 29 21:53:45 2019 (+0800)
// Last-Updated: 一 2月 10 00:09:50 2020 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 20
// URL: http://wuhongyi.cn 

// g++ main.cc `mysql_config --cflags --libs` -o 123

// https://dev.mysql.com/doc/refman/5.6/en/c-api.html

#include <mysql/mysql.h>
#include <iostream>
#include <ctime>
#include <string>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char *argv[])
{
  MYSQL       *mysql;//mysql数据库连接句柄。在执行任何数据库操作之前首先就需要创建一个MYSQL结构。
  // MYSQL_RES   *result;//执行查询语句（SELECT, SHOW, DESCRIBE, EXPLAIN）返回的结果。
  // MYSQL_ROW    row;//用来表示返回结果中的一行数据。由于每行数据格式不一致，因此使用此结构来统一表示。调用mysql_fetch_row()可从MYSQL_RES中返回一个MYSQL_ROW结构

  // char        *w;


  mysql = mysql_init(NULL);

  mysql_options(mysql,MYSQL_SET_CHARSET_NAME ,"utf8");
  my_bool reconnect = 1;
  mysql_options(mysql, MYSQL_OPT_RECONNECT, &reconnect);
  mysql_options(mysql,MYSQL_OPT_COMPRESS,0);
  
  if (!mysql_real_connect(mysql,"222.29.111.178","admin","123456",NULL,0,NULL,0))
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

   FILE *fp = fopen("statistics.sql", "r");
   char sql[4096];
   fread(sql, 1, 4096, fp);
   fclose(fp);
   if(mysql_query(mysql, sql))
     {
       fprintf(stderr, "Failed to create tables: Error: %s\n", mysql_error(mysql));
     }
  


   
  // mysql_real_mysqlect(mysql, "222.29.111.176", "data", "123456", "monitor", 3306, NULL, 0);

  // mysql_query(mysql, "select * from cool order by ts desc limit 1000");
  // result = mysql_store_result(mysql);
  // if (result == NULL)
  //   {
  //     printf("%d:%s\n", mysql_errno(mysql), mysql_error(mysql));
  //   }

  // while ((row = mysql_fetch_row(result)))
  //   {
  //     w = row[0];
  //     std::cout<<row[0]<<"  "<<row[1]<<"  "<<row[2]<<std::endl;
  //   }


  
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
