<!-- TimeResolution.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 一 1月 14 17:09:20 2019 (+0800)
;; Last-Updated: 一 1月 14 17:57:47 2019 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 2
;; URL: http://wuhongyi.cn -->

# 时间分辨

<!-- toc -->

## 脉冲产生装置

- Model PB-5
   - https://www.berkeleynucleonics.com/model-pb-5

## 100M module

- Width:      100ns    设置为最小值，平台时间
- Rise time:  0.1 us   上升时间
- Fall time:  100 us   衰减时间常数
- Rate:       1.0 kHz  频率
- Delay:      250ns    任意设置
- Ampl:       10.0 V   根据需要设置
- Polarity:   Pos      根据需要设置
- Pulse Top:  Flat
- Atten:      1X       根据需要设置
- PB-5 Pulse: ON       开启脉冲输出

脉冲发生器产生信号经过分路器一分为二。

### 2 channel in one module

![2 channel in one module](/img/timeresolution_100m_1module.png)

本征分辨约为 60 ps

### 2 channel in one crate



### 2 channel in different crate

![2 channel in different crate](/img/timeresolution_100m_2crate.png)

本征分辨约为 360 ps

## 250M module

### 2 channel in one module


### 2 channel in one crate


### 2 channel in different crate


## 100M & 250M module

### 2 channel in one crate


### 2 channel in different crate


<!-- TimeResolution.md ends here -->
