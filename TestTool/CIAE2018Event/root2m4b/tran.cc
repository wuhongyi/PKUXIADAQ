// tran.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 6月  3 09:05:30 2018 (+0800)
// Last-Updated: 日 6月  3 09:09:59 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 1
// URL: http://wuhongyi.cn 

void help()
{
 std:cout<<"root2m4b(const char *rootfilename,const char *m4bfilename)"<<std::endl;
}

void root2m4b(const char *rootfilename,const char *m4bfilename)
{
  TFile *file = new TFile(rootfilename,"READ");//"RECREATE" "READ"
  if(!file->IsOpen())
    {
      std::cout<<"Can't open root file"<<std::endl;
    }
 
  TH2I *matrix = (TH2I*)file->Get("matrix");


  // write matrix file
  FILE *fp;
  if((fp=fopen(m4bfilename,"ab")) == NULL)
    std::cout<<"can't open file .m4b"<<std::endl;
  
  int count;
  for (int i = 1; i <= 4096; ++i)
    for (int j = 1; j <= 4096; ++j)
      {
  	count = int(matrix->GetBinContent(i, j));
  	fwrite(&count,sizeof(int),1,fp);
      }
  
  fclose(fp);

  file->Close();
}

// 
// tran.cc ends here
