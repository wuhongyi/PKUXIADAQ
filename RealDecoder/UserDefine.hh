// UserDefine.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 9月 11 10:54:23 2020 (+0800)
// Last-Updated: 五 9月 11 17:35:33 2020 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 4
// URL: http://wuhongyi.cn 

#ifndef _USERDEFINE_H_
#define _USERDEFINE_H_

#define CLEAR_IN_NEW_RUN

#define BUFFLENGTH 1310720
// runnumber 4 byte
// bufflegth 4 byte * 13
// temp      4 byte
// buff * 14

#define MODULE_NUM_MAX 13

#define ADC_HIST_BIN 65536
#define ADC_HIST_MIN 0
#define ADC_HIST_MAX 65536


// from online
#define PRESET_MAX_MODULES 24
#define SHAREDMEMORYDATAOFFSET 1170 //BYTE
#define SHAREDMEMORYDATASTATISTICS 448
#define SHAREDMEMORYDATAENERGYLENGTH 32768
#define SHAREDMEMORYDATAMAXCHANNEL 16


#endif /* _USERDEFINE_H_ */

// 
// UserDefine.hh ends here
