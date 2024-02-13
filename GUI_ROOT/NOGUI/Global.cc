// Global.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 8月 15 16:50:10 2016 (+0800)
// Last-Updated: 三 1月 30 01:55:38 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 12
// URL: http://wuhongyi.cn 

#include "Global.hh"

#include <sys/time.h> // struct timeval, select()
#include <sys/stat.h>//stat(const char *file_name,struct stat *buf)
#include <termios.h> // tcgetattr(), tcsetattr()
#include <unistd.h> // read()
#include <inttypes.h>
#include <stdint.h>   // C99 compliant compilers: uint64_t
#include <ctype.h>    // toupper()
#include <fcntl.h>
#include <errno.h>//define error
#define CLEARSCR "clear"

#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>

static struct termios g_old_kbd_mode;

void Sleep(int t) 
{
  usleep( t*1000 );
}

static void cooked(void)
{
  tcsetattr(0, TCSANOW, &g_old_kbd_mode);
}

static void raw(void)
{
  static char init;
  struct termios new_kbd_mode;

  if(init)
    return;
  /* put keyboard (stdin, actually) in raw, unbuffered mode */
  tcgetattr(0, &g_old_kbd_mode);
  memcpy(&new_kbd_mode, &g_old_kbd_mode, sizeof(struct termios));
  new_kbd_mode.c_lflag &= ~(ICANON | ECHO);
  new_kbd_mode.c_cc[VTIME] = 0;
  new_kbd_mode.c_cc[VMIN] = 1;
  tcsetattr(0, TCSANOW, &new_kbd_mode);
  /* when we exit, go back to normal, "cooked" mode */
  atexit(cooked);

  init = 1;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ErrorInfo(const char *filename,const char *funcname,const char *detial,int ErrorN)
{
  std::cout << "<<Error>> File-" << filename << "-" << funcname << " ErrorCode:"<< detial << " " << ErrorN << std::endl;
}


int getch(void)
{
  unsigned char temp;

  raw();
  /* stdin = fd 0 */
  if(read(0, &temp, 1) != 1)
    return 0;
  return temp;

}

int kbhit()
{
  struct timeval timeout;
  fd_set read_handles;
  int status;

  raw();
  /* check stdin (fd 0) for activity */
  FD_ZERO(&read_handles);
  FD_SET(0, &read_handles);
  timeout.tv_sec = timeout.tv_usec = 0;
  status = select(0 + 1, &read_handles, NULL, NULL, &timeout);
  if(status < 0)
    {
      printf("select() failed in kbhit()\n");
      exit(1);
    }
  return (status);
}


void PrintInterface() 
{
  printf("\n  [q]   Quit\n");
  printf("  [s]   Start/Stop Acquisition\n");
  printf("  [o]   Open/Close Send Shared Memory Online\n");
  printf("  [w]   Open/Close Write Data To Harddisk\n");
  printf("  [a]   OPen/Close Auto Run Mode\n");
  printf("  [e]   Update Online Energy Spectrum\n");
  printf("  [c]   Clean Screen\n");
  printf("  [h]   Print Run Status\n");
  printf("--------------------------------------------------------------------------\n");

}

// 
// Global.cc ends here
