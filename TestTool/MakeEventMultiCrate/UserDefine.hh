// UserDefine.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 10月 20 16:20:34 2017 (+0800)
// Last-Updated: 日 1月 13 21:08:30 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 7
// URL: http://wuhongyi.cn 

// 本转换程序的使用前提，插件必须从第一个插槽开始，中间不留空插槽。

// 用户需要修改本文件中的参数
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef _USERDEFINE_H_
#define _USERDEFINE_H_

#define OUTFILEPATH "/home/wuhongyi/data/"

#define RAWFILEPATH "/home/wuhongyi/data/"
#define RAWFILENAME "data"

#define EVENTTIMEWINDOWS  400 //ns

// 设置插件个数
#define CRATE0BOARDNUMBER 4
#define CRATE1BOARDNUMBER 3
#define CRATE2BOARDNUMBER 0
#define CRATE3BOARDNUMBER 0
#define CRATE4BOARDNUMBER 0
#define CRATE5BOARDNUMBER 0
#define CRATE6BOARDNUMBER 0
#define CRATE7BOARDNUMBER 0

#define BOARDNUMBER (CRATE0BOARDNUMBER+CRATE1BOARDNUMBER+CRATE2BOARDNUMBER+CRATE3BOARDNUMBER+CRATE4BOARDNUMBER+CRATE5BOARDNUMBER+CRATE6BOARDNUMBER+CRATE7BOARDNUMBER)

#endif /* _USERDEFINE_H_ */
// 
// UserDefine.hh ends here
