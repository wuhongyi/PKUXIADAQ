.. DecodeAndSortAll.rst --- 
.. 
.. Description: 
.. Author: Hongyi Wu(吴鸿毅)
.. Email: wuhongyi@qq.com 
.. Created: 日 5月  1 20:31:53 2022 (+0800)
.. Last-Updated: 日 5月  1 21:02:24 2022 (+0800)
..           By: Hongyi Wu(吴鸿毅)
..     Update #: 2
.. URL: http://wuhongyi.cn 

##################################################
数据解码
##################################################

**DecodeAndSortAll 程序用来将同一轮数据不同采集卡采集的数据转为一个 ROOT 文件。用户的物理分析以本程序产生的 ROOT 文件为基准。本程序生成的数据已经按照时间戳从小到大排列。**

用户首先需要修改 **UesrDefine.hh** 文件中的定义

.. code:: cpp
	  
  #define WAVEFORM
  #define ENERGYSUM
  #define QDCSUM
  #define EXTERNALTS
   
  #define Crate0
  #define Crate1
  #define Crate2
  #define Crate3
  #define Crate4
  #define Crate5
  #define Crate6
  #define Crate7

以上是数据的最基本定义，WAVEFORM、ENERGYSUM、QDCSUM、EXTERNALTS 这四个变量分别表示 波形、能量计算相关的三段积分和基线、 8段 QDC 积分、外部时钟。如果数据中不需要该数据，或者记录的数据没有以上的数据，则注释掉相应的定义。抛弃不要的数据，可以极大提高数据转换速度。

Crate0-Crate7 分别表示机箱，当前程序支持最大 8 个机箱的数据进行合并，数据中有几个机箱则开启相应定义，其它则注释掉。多个机箱同步获取的数据使用本程序有个前提：每个机箱的 CrateID 需要设置不一样。

以下表示需要波形，一个机箱的数据

.. code:: cpp
	  
  #define WAVEFORM
  // #define ENERGYSUM
  // #define QDCSUM
  // #define EXTERNALTS
   
  #define Crate0
  // #define Crate1
  // #define Crate2
  // #define Crate3
  // #define Crate4
  // #define Crate5
  // #define Crate6
  // #define Crate7


.. code:: cpp
  
  #define ROOTFILEPATH "/wuhongyi1/dataroot/"  //要生成ROOT文件的路径
  #define ROOTFILENAME "data"                  //要生成ROOT文件的名字
  #define TimesHist 3600  // 单位秒，为了方便查看每个通道计数率随时间的变化，记录的数据中还包括每个通道的直方图，每个 bin 为 1秒。


.. code:: cpp
  
  #define Crate0RAWFILEPATH "/wuhongyi1/SHANS2021/C0/"   //第一个机箱原始二进制文件的路径
  #define Crate0RAWFILENAME "data"                   //第一个机箱原始文件的文件名
  #define Crate0num 7   //该机箱中使用插件数
  const int Crate0SamplingRate[Crate0num] = {100,100,100,100,100,100,100};//分别指定每个插件的采样率  100/250/500三种采样率  0为跳过该插件

  
文件夹内还有 crate0.dat - crate7.dat 等 8 个文件，分别表示每个机箱数据的筛选条件。crate.dat 文件为默认设置（不加任何筛选条件），方便用户替换文件。文件中包括五列数据，分别表示模块编号，通道编号，时间差修正量，能量道址下限，能量道址上限。其中前两列不可修改。时间差修正量表示该通道的时间戳会加上该修正量（单位为 ns）；能量道址下限表示当该通道的能量低于该数值时事件舍弃；能量道址上限表示当该通道的能量高于该数值时事件舍弃。


修改之后执行以下命令编译程序：
  
.. code:: bash
	  
  make clean
  make


编译成功之后将生成一个可执行文件 **decodeandsort**，程序运行方式：

.. code:: bash

  ./decodeandsort [Crate0 RunNnumber] [Crate2 RunNnumber] [Crate2 RunNnumber] ...

其中 *[CrateX RunNnumber]* 为想要对应机箱转换的文件运行编号。

例如：

.. code:: bash

  # 转换第一个机箱的第三轮
  ./decodeandsort  3

  #转换第一个机箱第 32 轮和第二个机箱第 61 轮
  ./decodeandsort  32 61


----

**ROOT 文件信息：**

- sr(short): sample rate，100/250/500，This value is specified in UesrDefine.hh. / 该数值由 UesrDefine.hh 中指定
- pileup(bool): 堆积标记。
- outofr(bool): 是否超量程标记。
- cid(short): 机箱编号
- sid(short): 采集卡所在插槽编号
- ch(short): 采集卡通道
- evte(unsigned short): 梯形算法的能量
- ts(long int 64 bit): 时间戳
- ets(long int 64 bit): 外部时间戳
- cfd(short): cfd 数值
- cfdft(bool): cfd 数值是否有效
- cfds(short):cfd source，仅适用于 250/500 MHz 采集卡
- trae(unsigned int): 计算能量梯形算法上升段积分
- leae(unsigned int): 计算能量梯形算法下降段积分
- gape(unsigned int): 计算能量梯形算法平台段积分
- base(double): 能量梯形算法的基线
- qs(unsigned int): 八个QDC面积的积分
- ltra(unsigned short): 波形采集点数
- data(unsigned short): 波形数据
- 未完待补充
  

下图展示一个文件中的 Branch 定义：    
  
.. 
.. DecodeAndSortAll.rst ends here
