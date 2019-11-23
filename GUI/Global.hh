// Global.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 二 8月  9 09:24:46 2016 (+0800)
// Last-Updated: 六 11月 23 14:34:30 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 78
// URL: http://wuhongyi.cn 

#ifndef _GLOBAL_HH_
#define _GLOBAL_HH_

const char gVERSION[] = "Version: STD - 2019.11.23";

const char gAbout[] = "\
Version: Beta ---> Developer version\n\
This program was developed by Peking University and is applicable to XIA Pixie16. \n\
Support 100 MHz(RevD 12-bit,RevF 14-bit), \n\
        250 MHz(RevF 12/14/16-bit), \n\
        500 MHz(RevF 12/14-bit)\n\
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

// TColor::RGB2Pixel(COLOR_BLUE_R,COLOR_BLUE_G,COLOR_BLUE_B)
// TColor::RGB2Pixel(COLOR_RED_R,COLOR_RED_G,COLOR_RED_B)
// TColor::RGB2Pixel(COLOR_GREEN_R,COLOR_GREEN_G,COLOR_GREEN_B)
// TColor::RGB2Pixel(COLOR_PURPLE_R,COLOR_PURPLE_G,COLOR_PURPLE_B)
// TColor::RGB2Pixel(COLOR_PINK_R,COLOR_PINK_G,COLOR_PINK_B)
// TColor::RGB2Pixel(COLOR_YELLOW_R,COLOR_YELLOW_G,COLOR_YELLOW_B)   
// TColor::RGB2Pixel(COLOR_ORANGE_R,COLOR_ORANGE_G,COLOR_ORANGE_B)    
// TColor::RGB2Pixel(COLOR_DODERBLUE_R,COLOR_DODERBLUE_G,COLOR_DODERBLUE_B)


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

#define TABLE_LABELTITLE_BG_R  7
#define TABLE_LABELTITLE_BG_G  7
#define TABLE_LABELTITLE_BG_B  7

#define TABLE_LABELTITLE_TEXT_R  255
#define TABLE_LABELTITLE_TEXT_G  255
#define TABLE_LABELTITLE_TEXT_B  255

#define TABLE_LABEL_TEXT_R  255
#define TABLE_LABEL_TEXT_G  255
#define TABLE_LABEL_TEXT_B  255

#define TAB_BG_R  211  //26
#define TAB_BG_G  211  //26
#define TAB_BG_B  211  //26


#define COLOR_RED_R  255
#define COLOR_RED_G  0
#define COLOR_RED_B  0

#define COLOR_YELLOW_R  255
#define COLOR_YELLOW_G  255
#define COLOR_YELLOW_B  0

#define COLOR_GREEN_R  0
#define COLOR_GREEN_G  255
#define COLOR_GREEN_B  0

#define COLOR_BLUE_R  0
#define COLOR_BLUE_G  0
#define COLOR_BLUE_B  255

#define COLOR_DODERBLUE_R  30
#define COLOR_DODERBLUE_G  144
#define COLOR_DODERBLUE_B  255

#define COLOR_PURPLE_R  128
#define COLOR_PURPLE_G  0
#define COLOR_PURPLE_B  128

#define COLOR_PINK_R  255
#define COLOR_PINK_G  192
#define COLOR_PINK_B  203

#define COLOR_ORANGE_R  255
#define COLOR_ORANGE_G  165
#define COLOR_ORANGE_B  0

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
#define TEXTBUTTONSMALL_FONT "-*-*-bold-r-*-*-14-*-*-*-*-*-iso8859-1"
#define STARTSTOP_TEXTBUTTON_FONT "-adobe-helvetica-medium-r-*-*-20-*-*-*-*-*-iso8859-1"
#define TABLE_LABEL_CH_FONT "-adobe-helvetica-bold-r-*-*-10-*-*-*-*-*-iso8859-1"
#define TABLE_LABEL_TITLE_FONT "-adobe-helvetica-bold-r-*-*-10-*-*-*-*-*-iso8859-1"
#define TABLE_LABEL_LOGIC_FONT "-*-*-bold-r-*-*-14-*-*-*-*-*-iso8859-1"

// Normal Medium Bold Light DemiboldBlack
// Roman Italic Oblique

// times italic
// times bold
// times bold italic
// helvetica
// helvetica italic
// helvetica bold
// helvetica bold italic
// courier
// courier italic
// courier bold
// courier bold italic
// symbol
// times


void ErrorInfo(const char *filename,const char *funcname,const char *detial,int ErrorN);

#endif /* _GLOBAL_HH_ */
// 
// Global.hh ends here
