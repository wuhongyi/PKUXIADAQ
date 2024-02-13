// Simulation.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 6月  3 13:20:05 2017 (+0800)
// Last-Updated: 四 8月 24 22:56:05 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 5
// URL: http://wuhongyi.cn 

#include "Simulation.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Simulation::Simulation(const TGWindow *p, const TGWindow *main)
{
  SetCleanup(kDeepCleanup);



  SetWindowName("Simulation");
  MapSubwindows();
  MapWindow();
  Resize();
}

Simulation::~Simulation()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Bool_t Simulation::ProcessMessage(Long_t msg, Long_t parm1, Long_t)
{



  
  return kTRUE;
}




// 
// Simulation.cc ends here
