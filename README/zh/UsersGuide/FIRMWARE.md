<!-- FIRMWARE.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 三 5月 16 19:00:58 2018 (+0800)
;; Last-Updated: 五 3月  8 16:48:10 2019 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 5
;; URL: http://wuhongyi.cn -->

# FIRMWARE

**北京大学定制固件**  

在标准固件的基础上添加了以下功能：

- 100MHz 12 bit(pixie16_revd_12b100m_firmware_release_10092018)
	- 当计算的能量为负数时，该值设置为 0。
	- pileup 事件能量保留，不设置为 0。


- 100MHz 14 bit(pixie16_revfpku_14b100m_dsp_update_05082018)
	- MultiplicityMaskHigh[31]=0和1 时候前面版均能输出 multiplicity 结果。
	- 当计算的能量为负数时，该值设置为 0。
	- pileup 事件能量保留，不设置为 0。
	- 在记录波形模式下，waveform 的buffer满了的时候，插件不busy，header继续记录，该情况下，输出的数据没有波形。
	- 在采集波形时候，增加了降频输出的功能，采取的策略为可选择1，1/2，1/4，1/8，1/16，1/32，1/64，1/128频率的输出，即多少个点保留一个点。保留的点是平均后的值。
	
	
	
- 250MHz 14bit(pixie16_revfpku_14b250m_release_02232019)
	- MultiplicityMaskHigh[31]=0和1 时候RJ45口与前面板均能输出 multiplicity 结果。
	- 当计算的能量为负数时，该值设置为 0。
	- pileup 事件能量保留，不设置为 0。


- 250MHz 16bit(pixie16_revfpku_16b250m_release_02182019)
	- MultiplicityMaskHigh[31]=0和1 时候RJ45口与前面板均能输出 multiplicity 结果。
	- 当计算的能量为负数时，该值设置为 0。
	- pileup 事件能量保留，不设置为 0。



<!-- FIRMWARE.md ends here -->
