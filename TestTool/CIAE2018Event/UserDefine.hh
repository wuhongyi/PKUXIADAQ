// UserDefine.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 10月 20 16:20:34 2017 (+0800)
// Last-Updated: 一 6月 11 01:11:29 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 14
// URL: http://wuhongyi.cn 

// 用户需要修改本文件中的参数

// 只转第一个采集卡的
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef _USERDEFINE_H_
#define _USERDEFINE_H_

#define RAWFILEPATH "/home/wuhongyi/data/"  // 原始ROOT文件路径
#define RAWFILENAME "data"

#define OUTFILEPATH "./"  // 生成的 matrix 文件路径


// true 表示该路数据参与转换
// const bool FLAGCHANNEL[16] = {/*0-3*/true,true,true,true,/*4-7*/true,false,true,true,/*8-11*/false,true,true,true,/*12-15*/true,true,true,true};
const bool FLAGCHANNEL[16] = {/*0-3*/true,true,true,true,/*4-7*/true,false,false,false,/*8-11*/false,true,true,true,/*12-15*/false,false,false,false};

// 刻度系数
const double CALI_K[16] = {/*0-3*/0.183964105,0.4088328208,0.408355149,0.4358744159,/*4-7*/0.4260315101,0.4549185364,0.2719229018,0.2974862705,/*8-11*/1.0,0.2509013874,0.2032598979,0.2048907003,/*12-15*/0.4154797098,0.4090801932,0.42154523,0.4111400641};
const double CALI_B[16] = {/*0-3*/0.166084798,-0.6106085762,-0.4122527447,-0.1974703526,/*4-7*/0.2473155878,-0.3233458865,-10.4970986545,-0.02148561,/*8-11*/0.0,0.0010579672,0.2208476221,0.2616359658,/*12-15*/-0.7291470551,-0.0377191717,-0.0502003058,0.1015729129};

						       
#define WINDOWS 200 // ns


// 阈值 keV
#define THRE_LOW  50
#define THRE_HIGH 2048


// TH2I 4096*4096 bin
#define BIN_MIN 0   //keV
#define BIN_MAX 2048  //keV


#endif /* _USERDEFINE_H_ */
// 
// UserDefine.hh ends here










