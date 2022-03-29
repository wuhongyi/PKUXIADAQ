// th1tospe.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 二 3月 29 19:14:15 2022 (+0800)
// Last-Updated: 二 3月 29 20:46:03 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 13
// URL: http://wuhongyi.cn 



int writespe()
{
  TFile *filer = new TFile("hist133Ba.root","READ");//"RECREATE" "READ"
  if(!filer->IsOpen())
    {
      std::cout<<"Can't open root file"<<std::endl;
    }
 
  TH1I *h = (TH1I*)filer->Get("he90");


  float data;
  char temp1[36]={0x18,0x00,0x00,0x00,0x78,0x70,0x6a,0x20,0x20,0x20,0x20,0x20,0x00,0x10,0x00,0x00,0x01,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x00,0x40,0x00,0x00};//采用默认初始化值
  char temp2[4]={0x00,0x40,0x00,0x00};
  
  std::ofstream writedata;//fstream
  writedata.open("data.spe",std::ios::binary);//ios::bin ios::app
  if(!writedata.is_open())
    {
      std::cout<<"can't open file."<<std::endl;
    }

  writedata.write((char*)&temp1,sizeof(char)*36);
  for (int i = 1; i <= 4096; ++i)
    {
      data = (float)h->GetBinContent(i);
      // std::cout<<i<<"  "<<data<<std::endl;
      writedata.write((char*)&data,sizeof(float));
    }
  writedata.write((char*)&temp2,sizeof(char)*4);
  writedata.close();
  filer->Close();
  return 0;
}

// 
// th1tospe.cc ends here










