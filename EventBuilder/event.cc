// event.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 9月 21 16:28:37 2020 (+0800)
// Last-Updated: 五 11月 27 20:00:23 2020 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 16
// URL: http://wuhongyi.cn 

#include "event.hh"
#include "UserDefine.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

event::event(int run)
{
  flag = false;
  nevt = 0;
  
  for (int i = 0; i < 8; ++i)
    for (int j = 0; j < 208; ++j)
      {
	flagdet[i][j] = -1;
	flagdetid[i][j] = -1;
	calia0[i][j] = 0.0;
	calia1[i][j] = 0.0;
	calia2[i][j] = 0.0;
      }

  std::ifstream readtxt;
  readtxt.open("par.dat");
  if(!readtxt.is_open())
    {
      std::cout<<"can't open file."<<std::endl;
    }

  std::string str_tmp;
  getline(readtxt,str_tmp);


  Short_t cid_tmp, sid_tmp, ch_tmp;
  Short_t det_tmp, id_tmp;
  Double_t a0_tmp, a1_tmp, a2_tmp;
  while(!readtxt.eof())
    {
      readtxt>>cid_tmp>>sid_tmp>>ch_tmp>>det_tmp>>id_tmp>>a0_tmp>>a1_tmp>>a2_tmp;
      if(readtxt.eof()) break;
      Short_t flagchtmp = (sid_tmp-2)*16+ch_tmp;
      flagdet[cid_tmp][flagchtmp] = det_tmp;
      flagdetid[cid_tmp][flagchtmp] = id_tmp;
      calia0[cid_tmp][flagchtmp] = a0_tmp;
      calia1[cid_tmp][flagchtmp] = a1_tmp;
      calia2[cid_tmp][flagchtmp] = a2_tmp;
      std::cout<<flagdet[cid_tmp][flagchtmp]<<"  "<<flagdetid[cid_tmp][flagchtmp]<<"  "<<calia0[cid_tmp][flagchtmp]<<"  "<<calia1[cid_tmp][flagchtmp]<<"  "<<calia2[cid_tmp][flagchtmp]<<"  "<<std::endl;
    }
  readtxt.close();  

  
  t_in = new TChain("tree");
  t_in->Add(TString::Format("%s%s_%s_%04d*.root",RAWFILEPATH,RAWFILENAME,RAWFILECRATENUMBER,run).Data());
  // file_in = new TFile(TString::Format("%s%s_%s_%04d*.root",RAWFILEPATH,RAWFILENAME,RAWFILECRATENUMBER,run).Data(),"READ");
  // if(!file_in->IsOpen())
  //   {
  //     std::cout<<"Can't open raw root file"<<std::endl;
  //     exit(1);
  //   }
  // t_in = (TTree*)file_in->Get("tree");
  t_in->SetBranchAddress("sr", &sr, &b_sr);
  t_in->SetBranchAddress("pileup", &pileup, &b_pileup);
  t_in->SetBranchAddress("outofr", &outofr, &b_outofr);
  t_in->SetBranchAddress("cid", &cid, &b_cid);
  t_in->SetBranchAddress("sid", &sid, &b_sid);
  t_in->SetBranchAddress("ch", &ch, &b_ch);
  t_in->SetBranchAddress("evte", &evte, &b_evte);
  t_in->SetBranchAddress("ts", &ts, &b_ts);
  t_in->SetBranchAddress("cfd", &cfd, &b_cfd);
  t_in->SetBranchAddress("cfdft", &cfdft, &b_cfdft);
  t_in->SetBranchAddress("cfds", &cfds, &b_cfds);
  TotalEntry = t_in->GetEntries();
  
  file_out = new TFile(TString::Format("%s%s_%04d_W%d.root",ROOTFILEPATH,ROOTFILENAME,run,EVENTTIMEWINDOWSWIDTH).Data(),"RECREATE");
  t_out = new TTree("tree","GDDAQ event data.");
  t_out->Branch("event",&event_vec);


  benchmark = new TBenchmark;
}


event::~event()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void event::Process()
{
  benchmark->Start("event");//计时开始

  for (Long64_t entry = 0; entry < TotalEntry; ++entry)
    {
      if(entry % 10000 == 0)
	{
	  std::cout<<"\r"<<"Entry: "<<entry<<"  |  Event: "<<nevt+1;
	  std::cout << std::flush;
	}
      
      // file_in->cd();
      t_in->GetEvent(entry);

      if(flag)
	{
	  if(sr==250) tsnow = ts*8;
	  else tsnow = ts*10;

	  if(TMath::Abs(tsnow-inittime) <= Long64_t(EVENTTIMEWINDOWSWIDTH))
	    {
	      ProcessEntry();
	    }
	  else
	    {
	      EndEvent();
	      InitEvent();
	    }
	}
      else
	{//event header
	  InitEvent();
	  flag = true;
	}

    }

  std::cout<<std::endl;

  // file_in->cd();
  // file_in->Close();

  file_out->cd();
  t_out->Write();
  file_out->Close();

  benchmark->Show("event");//计时结束并输出时间
}



void event::InitEvent()
{
  if(sr==250) inittime = ts*8;
  else inittime = ts*10;

  ProcessEntry();
}

void event::ProcessEntry()
{
  double rawch = gRandom->Rndm()+evte;
  int tmpch = (sid-2)*16+ch;
  hit.det = flagdet[cid][tmpch];
  hit.id = flagdetid[cid][tmpch];
  hit.sr = sr;
  hit.ts = ts;
  hit.raw = evte;
  hit.e = calia0[cid][tmpch]+calia1[cid][tmpch]*rawch+calia2[cid][tmpch]*rawch*rawch;
  hit.pileup = pileup;
  hit.outofr = outofr;
  hit.cfd = cfd;
  hit.cfdft = cfdft;
  hit.cfds = cfds;

  
  event_vec.push_back(hit);
}

void event::EndEvent()
{
  t_out->Fill();
  
  event_vec.clear();
  nevt++;
}


// 
// event.cc ends here
