// UserDefine.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 9月 11 10:54:23 2020 (+0800)
// Last-Updated: 二 9月  7 10:31:47 2021 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 5
// URL: http://wuhongyi.cn 

#ifndef _USERDEFINE_H_
#define _USERDEFINE_H_

#define CLEAR_IN_NEW_RUN

#define SHOW_OUTPUT_RATE



#define BUFFLENGTH 1310720
// runnumber 4 byte
// bufflegth 4 byte * 13
// temp      4 byte
// buff * 14

#define MODULE_NUM_MAX 13

#define ADC_HIST_BIN 65536
#define ADC_HIST_MIN 0
#define ADC_HIST_MAX 65536

#define RATE_HIST_BIN 14400
#define RATE_HIST_MIN 0
#define RATE_HIST_MAX 14400


// from online
#define PRESET_MAX_MODULES 24
#define SHAREDMEMORYDATAOFFSET 1170 //BYTE
#define SHAREDMEMORYDATASTATISTICS 448
#define SHAREDMEMORYDATAENERGYLENGTH 32768
#define SHAREDMEMORYDATAMAXCHANNEL 16


#endif /* _USERDEFINE_H_ */

// 
// UserDefine.hh ends here
