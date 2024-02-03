.. EventBuilder.rst --- 
.. 
.. Description: 
.. Author: Hongyi Wu(吴鸿毅)
.. Email: wuhongyi@qq.com 
.. Created: 日 5月  1 21:02:41 2022 (+0800)
.. Last-Updated: 日 5月  1 21:45:20 2022 (+0800)
..           By: Hongyi Wu(吴鸿毅)
..     Update #: 5
.. URL: http://wuhongyi.cn 

##################################################
事件重构
##################################################

**用户的物理分析以本程序产生的 ROOT 文件为基准。**

用户首先需要修改 **UesrDefine.hh** 文件中的定义

.. code:: cpp

  #define RAWFILEPATH "/wuhongyi1/dataroot/"   //原始ROOT文件的路径
  #define RAWFILENAME "data"                   //原始文件的文件名
  #define RAWFILECRATENUMBER "C2"              //原始文件中机箱数，一个机箱为 C1，两个为 C2，依次类推
   
  #define ROOTFILEPATH "/wuhongyi1/datarootevent/"  //要生成ROOT文件的路径
  #define ROOTFILENAME "event"                      //要生成ROOT文件的名字
   
  #define WAVEFORM
  #define ENERGYSUM
  #define QDCSUM
  #define EXTERNALTS
   
  #define EVENTTIMEWINDOWSWIDTH  8000   //ns   事件时间窗


WAVEFORM、ENERGYSUM、QDCSUM、EXTERNALTS 这四个变量分别表示 波形、能量计算相关的三段积分和基线、 8段 QDC 积分、外部时钟。如果数据中不需要该数据，或者记录的数据没有以上的数据，则注释掉相应的定义。抛弃不要的数据，可以极大提高数据转换速度。

文件夹内还有 par.dat 配置文件，该文件需要用户自行配置。该文件共 8 列，第一列表示机箱标号，第二列表示模块编号，第三列表示通道编号，第四列表示探测器类型编号，第五列表示同一类型探测器的子编号，第六、七、八表示刻度系数。生成数据中变量 raw 为 原始道址，变量 e 为原始道址添加刻度系数计算之后得到的能量值。该文件中没有配置的通道数据将被丢弃，当探测器编号或者探测器子编号设置为 -1 时，该通道数据同样丢弃。


修改之后执行以下命令编译程序：
  
.. code:: bash
	  
  make clean
  make


编译成功之后将生成一个可执行文件 **event**，程序运行方式：

.. code:: bash

  ./event  [RunNnumber] 

其中 *[RunNnumber]* 为想要转换的文件运行编号(数据中第一个机箱的运行编号)。

本程序转换之后，同一个事件被存在一个 vector 容器中，定义如下：

.. code:: cpp
	  
  std::vector<DATAEVENT> event_vec;
  // DATAEVENT 定义如下
  // 最小数据单元
  typedef struct DATAEVENT
  {
    Short_t      det;//探测器类型
    Short_t      id;//探测器编号
   
    Short_t         sr;
    Long64_t        ts;
    UShort_t        raw;//原始能量道址
    double	  e;//刻度能量
    
    Bool_t          pileup;
    Bool_t          outofr;
    Short_t         cfd;
    Bool_t          cfdft;
    Short_t         cfds;
   
  #ifdef ENERGYSUM
    Bool_t esumf;
    UInt_t trae;  // trailing energy sum
    UInt_t leae;	// leading energy sum
    UInt_t gape;	// Gap energy sum
    UInt_t base;	// baseline value
  #endif
    
  #ifdef QDCSUM
    Bool_t qsumf;
    UInt_t qs[8];
  #endif
   
  #ifdef EXTERNALTS
    Bool_t etsf;
    Long64_t ets;
  #endif
   
  #ifdef WAVEFORM
    unsigned short ltra;
    std::vector<unsigned short> data;
  #endif
   
  } DATAEVENT;


用户可参考以下代码读取该数据

.. code:: cpp

  typedef struct DATAEVENT
  {
    Short_t      det;//探测器类型
    Short_t      id;//探测器编号
   
    Short_t         sr;
    Long64_t        ts;
    UShort_t        raw;//原始能量道址
    double	  e;//刻度能量
    
    Bool_t          pileup;
    Bool_t          outofr;
    Short_t         cfd;
    Bool_t          cfdft;
    Short_t         cfds;
   
  #ifdef ENERGYSUM
    Bool_t esumf;
    UInt_t trae;  // trailing energy sum
    UInt_t leae;	// leading energy sum
    UInt_t gape;	// Gap energy sum
    UInt_t base;	// baseline value
  #endif
    
  #ifdef QDCSUM
    Bool_t qsumf;
    UInt_t qs[8];
  #endif
   
  #ifdef EXTERNALTS
    Bool_t etsf;
    Long64_t ets;
  #endif
   
  #ifdef WAVEFORM
    unsigned short ltra;
    std::vector<unsigned short> data;
  #endif
   
  } DATAEVENT;

  

  std::vector<DATAEVENT>* br_event;
  br_event = 0;
  tree->SetBranchAddress("event",&br_event);

  
  
  tree->GetEntry(xxx);  

  for(int i = 0; i < br_event->size(); i++) 
    {
	int flag = (*br_event)[i].det;
	Short_t id = (*br_event)[i].id;
	UShort_t raw = (*br_event)[i].raw;
	double e = (*br_event)[i].e;
	Long64_t ts = (*br_event)[i].ts;
	Bool_t pileup = (*br_event)[i].pileup;
	Bool_t outofr = (*br_event)[i].outofr;
	// ...
    }



    
.. 
.. EventBuilder.rst ends here
