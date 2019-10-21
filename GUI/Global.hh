// Global.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 二 8月  9 09:24:46 2016 (+0800)
// Last-Updated: 一 10月 21 22:14:42 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 47
// URL: http://wuhongyi.cn 

#ifndef _GLOBAL_HH_
#define _GLOBAL_HH_

const char gVERSION[] = "Version: 2019.10.21";

const char gAbout[] = "\
Version: Beta ---> Developer version\n\
\n\
The program developed by Peking University.\n\
The program applies to XIA Pixie-16. \n\
Support 100 MHz(RevD 12-bit,RevF 14-bit), \n\
        250 MHz(RevF 12/14/16-bit), \n\
        500 MHz(RevF 12/14-bit)\n\
\n\
\n\
Program Developer:\n\
  Hongyi Wu (wuhongyi@qq.com   Peking University)\n\
  Jing Li (lijinger02@163.com   Peking University)\n\
\n\
Technical Support:\n\
  Zhihuan Li (Peking University)\n\
  Hui Tan (XIA LLC)\n\
  Wolfgang Hennig(XIA LLC)\n\
\n\
Art Director:\n\
  Yi Song\n\
\n\
";


#define FRAME_BG_R  62
#define FRAME_BG_G  62
#define FRAME_BG_B  62

#define TOPBAR_BG_R  211  //26
#define TOPBAR_BG_G  211  //26
#define TOPBAR_BG_B  211  //26

#define TOPBAR_TEXT_R  255
#define TOPBAR_TEXT_G  255
#define TOPBAR_TEXT_B  255

#define TITLE_TEXT_R  255
#define TITLE_TEXT_G  255
#define TITLE_TEXT_B  255

#define LABEL_TEXT_R  131
#define LABEL_TEXT_G  131
#define LABEL_TEXT_B  131

#define TEXTENTRY_BG_R  21
#define TEXTENTRY_BG_G  21
#define TEXTENTRY_BG_B  21

#define TEXTBUTTON_BG_R  34
#define TEXTBUTTON_BG_G  151
#define TEXTBUTTON_BG_B  159

#define TEXTBUTTON_TEXT_R  255
#define TEXTBUTTON_TEXT_G  255
#define TEXTBUTTON_TEXT_B  255

#define CHECKBUTTON_TEXT_R  255
#define CHECKBUTTON_TEXT_G  255
#define CHECKBUTTON_TEXT_B  255

#define COLOR_RED_R  255
#define COLOR_RED_G  0
#define COLOR_RED_B  0

#define COLOR_GREEN_R  0
#define COLOR_GREEN_G  255
#define COLOR_GREEN_B  0

#define COLOR_BLUE_R  0
#define COLOR_BLUE_G  0
#define COLOR_BLUE_B  255


#define INITIAL_HIGHT 850
#define INITIAL_CONTANT_WIDTH 300
#define INITIAL_SIDE_WIDTH 20
#define INITIAL_WIDTH (INITIAL_CONTANT_WIDTH+2*INITIAL_SIDE_WIDTH)
#define TITLE_LISTMODE_HIGHT  30
#define TITLE_TOP_HIGHT 10
#define TITLE_BOTTON_HIGHT 13
#define TITLE_FONT "-*-*-bold-r-*-*-20-*-*-*-*-*-iso8859-1"
#define INFORMATION_FONT "-*-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1"
#define CHECKBUTTON_FONT "-*-*-bold-r-*-*-14-*-*-*-*-*-iso8859-1"
#define LABEL_FONT "-*-arial-*-r-*-*-14-*-*-*-*-*-iso8859-1"
#define TEXTENTRY_FONT "-*-*-*-r-*-*-18-*-*-*-*-*-iso8859-1"
#define TEXTBUTTON_FONT "-*-*-bold-r-*-*-18-*-*-*-*-*-iso8859-1"
#define STARTSTOP_TEXTBUTTON_FONT "-adobe-helvetica-medium-r-*-*-20-*-*-*-*-*-iso8859-1"
// Normal Medium Bold Light DemiboldBlack
// Roman Italic Oblique





void ErrorInfo(const char *filename,const char *funcname,const char *detial,int ErrorN);

#endif /* _GLOBAL_HH_ */
// 
// Global.hh ends here
