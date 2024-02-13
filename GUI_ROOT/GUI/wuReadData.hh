// wuReadData.hh --- 
// 
// Description:  Coping from YushouSong,宋玉收.
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 4月 18 18:08:43 2014 (+0800)
// Last-Updated: 五 4月 15 15:44:49 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 17
// URL: http://wuhongyi.cn 

//20151112修复空行字符" "开头的bug
//20160324修复空行字符"\n"开头的bug

//20160415发现问题，G41002片与ROOT60602耦合使用时ReadVector中p->push_back(sss)有问题！！！

#ifndef _WUREADDATA_H_
#define _WUREADDATA_H_

#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <vector>
using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace wuReadData{

  // const int NC=4096; //long enough for 1-line chars

  //-----------------------
  template <class T>
  T ReadValue(string vname, string fname, bool verb=false)
  {
    ifstream in(fname.c_str(), ios::in);
    if(!in.is_open()){
      cout<<":-( failed to open the file:"<<fname<<endl;
      exit(0);
    }
    T v;
    string inname;
    // char cbf[NC];
    string cbf;
    char chd;
    while(true){
      chd=in.peek();    
      if(chd==EOF){//EOF(generally -1) is a micro in <cstdio>
	cout<<":-($"<<vname <<"$ not found@"<<fname<<endl;
	in.close();
	exit(0);
      }else if(chd=='#' || chd==0x20 || chd==0xA){//修复空行带来的bug,空行可能是" "或者"\n"
	getline(in,cbf);//一次读取一行
      }else{
	in>>inname;
	// cout<<inname<<endl;
	if(inname==vname) { 
	  in>>v;
	  if(verb)cout<<vname<<" = "<<v<<endl; 
	  in.close();     
	  break;
	}
      }     
    }    
    return v;
  }

  //-----------------------
  template <class T>
  int ReadVector(string vname, string fname, vector<T>* p, bool verb=false)
  {
    if(!p){
      cout<<":-(vector of "<<vname<<" is null"<<endl;
      exit(0);
    }
    //-------
    ifstream in(fname.c_str(), ios::in);
    if(!in.is_open()){
      cout<<":-( failed to open the file:"<<fname<<endl;
      exit(0);
    }  
    //-------
    int ndata=-1; 
    string inname; 
    // char cbf[NC];
    string cbf;
    char chd;
    while(true){   
      chd=in.peek();    
      if(chd==EOF){//EOF(generally -1) is a micro in <cstdio>
	cout<<":-($"<<vname <<"$ not found@"<<fname<<endl;
	in.close();
	exit(0);
      }else if(chd=='#' || chd==0x20 || chd==0xA){//修复空行带来的bug,空行可能是" "或者"\n"
	getline(in,cbf);//一次读取一行
      }else{
	in>>inname; 
	if(inname==vname){ 
	  in>>ndata;
	  if(verb)cout<<"$$"<<vname<<" number ="<<ndata<<endl;
	  for(int i=0; i<ndata; i++){
	    T sss;
	    in>>sss;
	    p->push_back(sss);
	  }
	  in.close();
	  break;
	}    
      }
    }
    return ndata;  
  }

  //-----------------------
  template <class T>
  int ReadArray(string vname, string fname, T* p, bool verb=false)
  {
    vector<T> v;
    int n=ReadVector(vname,fname, &v,verb);
    for(int i=0; i<n; i++) p[i]=v[i];
    return n;
  }


}

#endif /* _WUREADDATA_H_ */
// 
// wuReadData.hh ends here
