// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 三 4月 27 20:21:40 2022 (+0800)
// Last-Updated: 四 4月 28 15:19:02 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 29
// URL: http://wuhongyi.cn 

// https://blog.csdn.net/qq_39568245/article/details/115312690
// https://blog.csdn.net/xhtchina/article/details/116460785?spm=1001.2101.3001.6650.2&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7Edefault-2.pc_relevant_aa&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7Edefault-2.pc_relevant_aa&utm_relevant_index=5

#include "json.hpp"
#include <iostream>
#include <fstream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char *argv[])
{
  std::ifstream ifs("data.dat");
  nlohmann::json  json = nlohmann::json::parse(ifs);
  

  std::cout<<json.dump()<<std::endl;

  int modules = json["modules"];
  std::cout<<"modules: "<<modules<<std::endl;

  std::cout<<json["slotid"]<<"  "<<json["slotid"].size()<<std::endl;
  std::cout<<json["firmware"]<<"  "<<json["firmware"].size()<<std::endl;
  
  for (int i = 0; i < json["slotid"].size(); ++i)
    {
      std::cout<<json["slotid"][i].get<short>()<<std::endl;
      std::cout<<json["firmware"][i].get<std::string>()<<std::endl;
    }

  std::cout<<json["dspfpga_100_12_std"]<<std::endl;
  std::cout<<json["dspfpga_100_12_std"]["ldr"]<<std::endl;
  std::cout<<json["dspfpga_100_12_std"]["var"]<<std::endl;
  std::cout<<json["dspfpga_100_12_std"]["fippi"]<<std::endl;
  std::cout<<json["dspfpga_100_12_std"]["trig"]<<std::endl;

  std::cout<<json["firmware"].is_array()<<std::endl;
  std::cout<<json["dspfpga_100_12_std"].empty()<<std::endl;
  std::cout<<json["dspfpga_100_16_std"].empty()<<std::endl;
  
  return 0;
}

// 
// main.cc ends here
