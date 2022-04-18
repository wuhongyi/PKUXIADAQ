// Global.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 8月 15 16:49:38 2016 (+0800)
// Last-Updated: 一 4月 18 20:20:27 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 6
// URL: http://wuhongyi.cn 

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

const char gVERSION[] = "Version: STD - 2022.04.18";

#define RECODESHA256

void ErrorInfo(const char *filename,const char *funcname,const char *detial,int ErrorN);

// SLEEP
void Sleep(int t);

// GETCH
int getch(void);

// KBHIT
int kbhit();

// Print the interface to screen
void PrintInterface();

#endif /* _GLOBAL_H_ */
// 
// Global.hh ends here
