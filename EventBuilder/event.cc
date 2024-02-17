// event.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 9月 21 16:28:37 2020 (+0800)
// Last-Updated: 六 2月 17 14:23:13 2024 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 41
// URL: http://wuhongyi.cn 

#include "event.hh"
#include "UserDefine.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

event::event(int run)
{
  flag = false;
  nevt = 0;
  
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < MAXBOARD; ++j)
      for (int k = 0; k < MAXCHANNEL; ++k)
      {
	flagdet[i][j][k] = -1;
	flagdetid[i][j][k] = -1;
	calia0[i][j][k] = 0.0;
	calia1[i][j][k] = 0.0;
	calia2[i][j][k] = 0.0;
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
      flagdet[cid_tmp][sid_tmp-2][ch_tmp] = det_tmp;
      flagdetid[cid_tmp][sid_tmp-2][ch_tmp] = id_tmp;
      calia0[cid_tmp][sid_tmp-2][ch_tmp] = a0_tmp;
      calia1[cid_tmp][sid_tmp-2][ch_tmp] = a1_tmp;
      calia2[cid_tmp][sid_tmp-2][ch_tmp] = a2_tmp;
      std::cout<<flagdet[cid_tmp][sid_tmp-2][ch_tmp]<<"  "<<flagdetid[cid_tmp][sid_tmp-2][ch_tmp]<<"  "<<calia0[cid_tmp][sid_tmp-2][ch_tmp]<<"  "<<calia1[cid_tmp][sid_tmp-2][ch_tmp]<<"  "<<calia2[cid_tmp][sid_tmp-2][ch_tmp]<<"  "<<std::endl;
    }
  readtxt.close();  

  
  t_in = new TChain("tree");
#ifdef WAVEFORM
  t_in->Add(TString::Format("%s%s_%s_%04d*_wave.root",RAWFILEPATH,RAWFILENAME,RAWFILECRATENUMBER,run).Data());
  t_in->Add(TString::Format("%s%s_%s_%04d*_wave_*.root",RAWFILEPATH,RAWFILENAME,RAWFILECRATENUMBER,run).Data());
#else
  t_in->Add(TString::Format("%s%s_%s_%04d*_notwave.root",RAWFILEPATH,RAWFILENAME,RAWFILECRATENUMBER,run).Data());
#endif
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

#ifdef ENERGYSUM
  t_in->SetBranchAddress("esumf", &esumf, &b_esumf);
  t_in->SetBranchAddress("trae", &trae, &b_trae);
  t_in->SetBranchAddress("leae", &leae, &b_leae);
  t_in->SetBranchAddress("gape", &gape, &b_gape);
  t_in->SetBranchAddress("base", &base, &b_base);
#endif
  
#ifdef QDCSUM
  t_in->SetBranchAddress("qsumf", &qsumf, &b_qsumf);
  t_in->SetBranchAddress("qs", &qs, &b_qs);
#endif

#ifdef EXTERNALTS
  t_in->SetBranchAddress("etsf", &etsf, &b_etsf);
  t_in->SetBranchAddress("ets", &ets, &b_ets);
#endif

#ifdef WAVEFORM
  t_in->SetBranchAddress("ltra", &ltra, &b_ltra);
  t_in->SetBranchAddress("data", &data, &b_data);
#endif
  TotalEntry = t_in->GetEntries();

#ifdef WAVEFORM
  file_out = new TFile(TString::Format("%s%s_%04d_W%d_wave.root",ROOTFILEPATH,ROOTFILENAME,run,EVENTTIMEWINDOWSWIDTH).Data(),"RECREATE");
#else
  file_out = new TFile(TString::Format("%s%s_%04d_W%d_notwave.root",ROOTFILEPATH,ROOTFILENAME,run,EVENTTIMEWINDOWSWIDTH).Data(),"RECREATE");
#endif
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
  // TotalEntry = 1000000;
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
	  if(sr == 250 || sr == 125) tsnow = ts*8;
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

  file_out = t_out->GetCurrentFile();
  file_out->cd();
  t_out->Write();
  file_out->Close();

  benchmark->Show("event");//计时结束并输出时间
}



void event::InitEvent()
{
  if(sr == 250 || sr == 125) inittime = ts*8;
  else inittime = ts*10;

  ProcessEntry();
}

void event::ProcessEntry()
{
  double rawch = gRandom->Rndm()+evte;
  hit.det = flagdet[cid][sid-2][ch];
  hit.id = flagdetid[cid][sid-2][ch];
  hit.sr = sr;
  hit.ts = ts;
  hit.raw = evte;
  hit.e = calia0[cid][sid-2][ch]+calia1[cid][sid-2][ch]*rawch+calia2[cid][sid-2][ch]*rawch*rawch;
  hit.pileup = pileup;
  hit.outofr = outofr;
  hit.cfd = cfd;
  hit.cfdft = cfdft;
  hit.cfds = cfds;

#ifdef ENERGYSUM
  hit.esumf = esumf;
  hit.trae = trae; 
  hit.leae = leae; 
  hit.gape = gape; 
  hit.base = base;   
#endif
  
#ifdef QDCSUM  
  hit.qsumf = qsumf;
  memcpy(hit.qs, qs, sizeof(unsigned int)*8);
#endif

#ifdef EXTERNALTS
  hit.etsf = etsf;
  hit.ets = ets;
#endif
  
#ifdef WAVEFORM
  hit.ltra = ltra;
  hit.data.clear();
  if(hit.ltra > 0)
    {
      hit.data.insert(hit.data.begin(),data,data+hit.ltra);
    }
#endif
  
  
  event_vec.push_back(hit);
}

void event::EndEvent()
{
  file_out = t_out->GetCurrentFile();
  file_out->cd();
  t_out->Fill();

  event_vec.clear();
  nevt++;
}


// 
// event.cc ends here
