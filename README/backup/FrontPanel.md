<!-- FrontPanel.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 五 7月 29 15:38:49 2016 (+0800)
;; Last-Updated: 日 10月 30 16:21:33 2016 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 16
;; URL: http://wuhongyi.cn -->

# Front Panel

## LVDS I/O port(J101) 

普通网口

当前标准固件中还没定义

## J155（letter"A"）

**输出信号**

3.3V I/O port，输出阻抗是 50 欧

A2、D1 地
A1、B1、B2、C1、C2、D2 输出信号

## J151-J154

**输入信号**

differential LVDS signals

- 16 channel gate input
- 1 module gate input
- 1 not use current

----

## J151-J155

**输入信号**

single-ended TTL external input signals

FI0、FI2、FI3、FI4、FI6、FI7

这是什么输入信号？external trigger？clock signals？


----

从前面板输出监视来看，fast trigger stretch length(FastTrigBackLen) 实际值为16路中最大的值。

前面板监视信号中，左边的矩形波是fast trigger，其宽度表示时间。
右边的信号表示Extern delay(ExternDelayLen) 表示为了等待trigger而延迟采集的时间

## group 000

FTRIG_DELAY  采集延迟时间  只要fast filter 过阈值就会产生
FTRIG_VAL    基本同上，有效采集时候才有信号
GLBETRIG_CE  stretched external global validation trigger 
CHANETRIG_CE stretched channel validation trigger ，开启CSRA bit13 。采集延迟必须在这个时间窗内才能采集到    看到的信号实际起始位置在 100 ns，意味着System FPGA 处理时间需要 100 ns ？ 信号宽度由 ChanTrigStretch 控制。 


选择 CSRA bit0 bit18 FTRIG_DELAY 总会多 100 ns


<!-- FrontPanel.md ends here -->
