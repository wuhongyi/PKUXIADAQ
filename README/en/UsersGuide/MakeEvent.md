<!-- MakeEvent.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 5月 13 20:24:12 2018 (+0800)
;; Last-Updated: 三 5月 16 18:30:44 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 2
;; URL: http://wuhongyi.cn -->

# MakeEvent

**本转换程序的使用前提，插件必须从第一个插槽开始，中间不留空插槽。**

**MakeEvent 程序用来快速将数据组装成与传统 VME 获取数据类似的结构，方便实验时的初步物理分析，最终的物理分析不能以本程序产生的数据为基准。**

用户首先需要修改 **UesrDefine.hh** 文件中的定义


```cpp
#define OUTFILEPATH "/home/wuhongyi/data/"
#define RAWFILEPATH "/home/wuhongyi/data/"
#define RAWFILENAME "data"


// 设置插件个数
#define BOARDNUMBER 5
```

用户需要修改：
- 原始 ROOT 文件的路径
- 生成的事件结构 ROOT 文件的存放路径
- 文件名
- 使用采集卡个数



修改之后执行以下命令编译程序：

```bash
make clean
make
```

编译成功之后将生成一个可执行文件 **event**，程序运行方式：

```bash
./event  [RunNnumber]  [windows]
```

其中 *[RuNnumber]* 为想要转换的文件运行编号，*[windows]* 为事件的时间窗，单位为 ns。


----

ROOT File Branch：

- sr: 采样率，该事件中该通道数值不为0表示探测到信号。
- adc: 能量
- outofr: 标记是否超模数转换的量程
- qdc: QDC的八段积分
- tdc: 时间
- cfd: cfd数值
- cfdft: 标记CFD数值是否有效
- cfds: 仅适用于 250/500 MHz 采集卡，cfd source


**TODO 这里添加一个Branch截图。。。**


<!-- MakeEvent.md ends here -->
