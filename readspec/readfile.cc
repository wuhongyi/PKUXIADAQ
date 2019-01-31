// readfile.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 1月 31 17:34:32 2019 (+0800)
// Last-Updated: 四 1月 31 19:48:15 2019 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 7
// URL: http://wuhongyi.cn 

#include "readfile.hh"
#include "UserDefine.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

readfile::readfile(int runnumber)
{
  Run = runnumber;
  FileName = RAWFILENAME;
  RawFilePath = RAWFILEPATH;
  benchmark = new TBenchmark;

  flagfile = MODULEnum;

  for (int i = 0; i < flagfile; ++i)
    if(!IsFileExists(TString::Format("%s%04d/%s_R%04d_M%02d.bin",RawFilePath.Data(),Run,FileName.Data(),Run,i).Data()))
      {
	std::cout<<"can't find raw data: "<<TString::Format("%s%04d/%s_R%04d_M%02d.bin",RawFilePath.Data(),Run,FileName.Data(),Run,i).Data()<<std::endl;
	exit(1);
      }
  std::cout<<"Mod No. "<<flagfile<<std::endl;


  for (int i = 0; i < flagfile; ++i)
    for (int j = 0; j < 16; ++j)
      {
	eHist[i][j] = new TH1I(TString::Format("E_%02d_%02d",i,j).Data(),"",65536,0,65536);
	eHist[i][j]->SetDirectory(0);
	eHist[i][j]->GetXaxis()->SetTitle("ch");
      }

  
}

readfile::~readfile()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void readfile::Process()
{
  benchmark->Start("r2cali");

  for (int i = 0; i < flagfile; ++i)
    {
      unsigned int eventdata, eventlength;
      long TotalWords, TotalSkippedWords;
      FILE *ListModeFile = NULL;
      int chflag;
      int energyflag;

      ListModeFile = fopen(TString::Format("%s%04d/%s_R%04d_M%02d.bin",RawFilePath.Data(),Run,FileName.Data(),Run,i).Data(), "rb");
      if(ListModeFile != NULL)
	{
	  fseek(ListModeFile, 0, SEEK_END);
	  TotalWords = (ftell(ListModeFile) + 1) / 4;
	  fseek(ListModeFile, 0, SEEK_SET);
	  TotalSkippedWords = 0;
	  do
	    {
	      fread(&eventdata, 4, 1, ListModeFile);
	      chflag = (eventdata & 0x0000000f);

	      eventlength = (eventdata & 0x7FFE0000) >> 17;// Event length
	      
	      fread(&eventdata, 4, 1, ListModeFile);

	      fread(&eventdata, 4, 1, ListModeFile);

	      fread(&eventdata, 4, 1, ListModeFile);
	      energyflag = (eventdata & 0x0000ffff);

	      eHist[i][chflag]->Fill(energyflag);
	      TotalSkippedWords += eventlength;
	      // NumEvents++;
	      fseek(ListModeFile, (eventlength-4)*4, SEEK_CUR);
	    }while(TotalSkippedWords < TotalWords);

	  fclose(ListModeFile);
	}
    }

  CreateDirectory(TString::Format("%04d",Run).Data());
  write2root();
  write2mca();
  benchmark->Show("r2cali");
}

void readfile::write2root()
{
  TFile *file = new TFile(TString::Format("%04d/%s_E_R%04d.root",Run,FileName.Data(), Run).Data(),"RECREATE");
  if(!file->IsOpen())
    {
      std::cout<<"Can't open root file"<<std::endl;
    }
 
  // filet->ls("");
  file->cd();
  for (int i = 0; i < flagfile; ++i)
    for (int j = 0; j < 16; ++j)
      {
  	eHist[i][j]->Write();
      }
  file->Close();
}

void readfile::write2mca()
{
  for (int i = 0; i < flagfile; ++i)
    for (int j = 0; j < 16; ++j)
      {
	std::ofstream writemca;//
	writemca.open(TString::Format("%04d/%s_R%04d_%02d_%02d.mca",Run,FileName.Data(), Run,i,j).Data());//ios::bin ios::app
	if(!writemca.is_open())
	  {
	    std::cout<<"can't open file."<<std::endl;
	  }

	writemca<<"<<PMCA SPECTRUM>>"<<std::endl;
	writemca<<"TAG - PKUXIADAQ"<<std::endl;
	writemca<<"DESCRIPTION - Hongyi Wu"<<std::endl;
	writemca<<"GAIN - 8"<<std::endl;
	writemca<<"THRESHOLD - "<<std::endl;
	writemca<<"LIVE_MODE - "<<std::endl;
	writemca<<"PRESET_TIME - "<<std::endl;
	writemca<<"LIVE_TIME - "<<std::endl;
	writemca<<"REAL_TIME - "<<std::endl;
	writemca<<"START_TIME - "<<std::endl;
	writemca<<"SERIAL_NUMBER - "<<std::endl;
	writemca<<"<<DATA>>"<<std::endl;
	for(int k = 0;k < 65536; k++)
	  writemca<<eHist[i][j]->GetBinContent(k+1)<<std::endl;
	writemca<<"<<END>>"<<std::endl;
	writemca.close();      
      }
}

bool readfile::IsFileExists(const char *name)
{
  if((access(name,F_OK)) != -1)  
    {  
      return true;
    }  
  else  
    {  
      return false;
    } 
}

bool readfile::CreateDirectory(const char *path)
{
  if(mkdir(path,0777)==0)
    {
      printf("created the directory %s.\n",path);
      return true;
    }
  else
    {   	  
      printf("cant't creat the directory %s.\n",path);
      printf("errno：%d\n",errno);
      printf("ERR  ：%s\n",strerror(errno));	
      return false;
    }
}

// 
// readfile.cc ends here
