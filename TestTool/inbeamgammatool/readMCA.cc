// readMCA.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 4月 22 19:13:27 2016 (+0800)
// Last-Updated: 五 4月 22 21:44:01 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 50
// URL: http://wuhongyi.cn 

#include "TCanvas.h"
#include "TH1.h"
#include "TLegend.h"
#include "TStyle.h"
#include <iostream>
#include <cstdio>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sstream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TCanvas *c1;
TLegend *legend;

TH1D *h;
int Channel;
int Gain;//MCA channel 1<<(8+Gain)

TH1D *hh[2];
int channel2[2];
int gain2[2];
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Init()
{
  if(!c1)
    {
      c1 = new TCanvas("c1","MCA DATA",1200,600);
      gStyle->SetOptStat(0);
    }
}

void Read(string mcafilename)
{
  Init();
  if(h) delete h;
  
  ifstream mcafile;
  string onelinestring;
  stringstream ss;
  Gain = -1;
  Channel = -1;
  
  mcafile.open(mcafilename.c_str());
  if(mcafile.fail())
    {
      cout<<"Can't open the file : "<<mcafilename<<". Please check it."<<endl;
    }
  else
    {
      while(getline(mcafile,onelinestring))
	{
	  // cout<<onelinestring<<endl;
	  if(onelinestring[0] == 'G' && onelinestring[1] == 'A' && onelinestring[2] == 'I' && onelinestring[3] == 'N' && onelinestring[5] == '-')
	    {
	      // cout<<onelinestring<<endl;
	      ss.clear();
	      ss<<onelinestring.replace(0, 6, "");
	      ss>>Gain;
	      // cout<<Gain<<endl;
	      Channel = 1 << (8+Gain);
	      // cout<<Channel<<endl;
	    }
	  if(onelinestring[0] == '<' && onelinestring[1] == '<' && onelinestring[2] == 'D')//onelinestring.size() == 9 && 
	    break;
	}

      if(Gain < 0)
	return;
      
      int count;
      h = new TH1D("h","",Channel,0,Channel);
      for (int i = 0; i < Channel; ++i)
	{
	  mcafile>>count;
	  // cout<<i<<"  "<<count<<endl;
	  for (int j = 0; j < count; ++j)
	    {
	      h->Fill(i);
	    }
	}

      getline(mcafile,onelinestring);
      // cout<<onelinestring<<endl;
      getline(mcafile,onelinestring);
      // cout<<onelinestring<<endl;
      if(onelinestring[0] == '<' && onelinestring[1] == '<' && onelinestring[2] == 'E')
	cout<<"Read file :"<<mcafilename<<" done."<<endl;
      mcafile.close();
    }

  if(h)
    {
      c1->cd();
      h->GetXaxis()->SetTitle("channel");
      h->Draw();
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Read(string mcafilename1,string mcafilename2)
{
  Init();
  if(hh[0]) delete hh[0];
  if(hh[1]) delete hh[1];
  if(legend) delete legend;

  
  ifstream mcafile1,mcafile2;
  string onelinestring;
  stringstream ss;

  channel2[0] = -1;
  channel2[1] = -1;
  gain2[0] = -1;
  gain2[1] = -1;

  mcafile1.open(mcafilename1.c_str());
  if(mcafile1.fail())
    {
      cout<<"Can't open the file : "<<mcafilename1<<". Please check it."<<endl;
    }
    mcafile2.open(mcafilename2.c_str());
  if(mcafile2.fail())
    {
      cout<<"Can't open the file : "<<mcafilename2<<". Please check it."<<endl;
    }

  while(getline(mcafile1,onelinestring))
    {
      if(onelinestring[0] == 'G' && onelinestring[1] == 'A' && onelinestring[2] == 'I' && onelinestring[3] == 'N' && onelinestring[5] == '-')
	{
	  ss.clear();
	  ss<<onelinestring.replace(0, 6, "");
	  ss>>gain2[0];
	  channel2[0] = 1 << (8+gain2[0]);
	}
      if(onelinestring[0] == '<' && onelinestring[1] == '<' && onelinestring[2] == 'D')
	break;
    }

  while(getline(mcafile2,onelinestring))
    {
      if(onelinestring[0] == 'G' && onelinestring[1] == 'A' && onelinestring[2] == 'I' && onelinestring[3] == 'N' && onelinestring[5] == '-')
	{
	  ss.clear();
	  ss<<onelinestring.replace(0, 6, "");
	  ss>>gain2[1];
	  channel2[1] = 1 << (8+gain2[1]);
	}
      if(onelinestring[0] == '<' && onelinestring[1] == '<' && onelinestring[2] == 'D')
	break;
    }

  if(gain2[0] != gain2[1])
    {
      cout<<"The Gain are different in file :"<<mcafilename1<<"/"<<mcafilename2<<"."<<endl;
      return;
    }

  if(gain2[0] < 0 && gain2[1] < 0)
    return;
  
  int count;
  
  hh[0] = new TH1D("h1","",channel2[0],0,channel2[0]);
  for (int i = 0; i < channel2[0]; ++i)
    {
      mcafile1>>count;
      for (int j = 0; j < count; ++j)
	{
	  hh[0]->Fill(i);
	}
    }
  getline(mcafile1,onelinestring);
  getline(mcafile1,onelinestring);
  if(onelinestring[0] == '<' && onelinestring[1] == '<' && onelinestring[2] == 'E')
    cout<<"Read file :"<<mcafilename1<<" done."<<endl;

  hh[1] = new TH1D("h2","",channel2[1],0,channel2[1]);
  for (int i = 0; i < channel2[1]; ++i)
    {
      mcafile2>>count;
      for (int j = 0; j < count; ++j)
	{
	  hh[1]->Fill(i);
	}
    }
  getline(mcafile2,onelinestring);
  getline(mcafile2,onelinestring);
  if(onelinestring[0] == '<' && onelinestring[1] == '<' && onelinestring[2] == 'E')
    cout<<"Read file :"<<mcafilename2<<" done."<<endl;

  if(hh[0] && hh[1])
    {
      c1->cd();
      hh[0]->SetMaximum(hh[0]->GetBinContent(hh[0]->GetMaximumBin())>hh[1]->GetBinContent(hh[1]->GetMaximumBin()) ? 1.05*hh[0]->GetBinContent(hh[0]->GetMaximumBin()) : 1.05*hh[1]->GetBinContent(hh[1]->GetMaximumBin()));
      hh[0]->GetXaxis()->SetTitle("channel");
      hh[0]->Draw();
      hh[1]->SetLineColor(6);
      hh[1]->Draw("same");	
    }

  legend = new TLegend(0.60,0.70,0.90,0.85);
  legend->SetHeader("count");
  legend->SetTextSize(0.05);
  legend->SetBorderSize(0);

  legend->AddEntry(hh[0],mcafilename1.c_str());
  legend->AddEntry(hh[1],mcafilename2.c_str());
  // mcafilename1+=" %.0f";
  // legend->AddEntry(hh[0],  Form(mcafilename1.c_str(),hh[0]->GetEntries()));
  // mcafilename2+=" %.0f";
  // legend->AddEntry(hh[1],  Form(mcafilename2.c_str(),hh[1]->GetEntries()));

  legend->Draw();
  
  mcafile1.close();
  mcafile2.close();
}



// 
// readMCA.cc ends here










