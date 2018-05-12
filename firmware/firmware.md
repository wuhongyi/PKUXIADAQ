<!-- firmware.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 二 12月 13 09:41:49 2016 (+0800)
;; Last-Updated: 六 5月 12 20:28:59 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 9
;; URL: http://wuhongyi.cn -->

# 固件说明

- pixie16_revf_14b100m_firmware_release_11032016 标准固件
- syspixie16_revfpku_adc100mhz_r36473 北大定制特殊固件（MultiplicityMaskHigh[31]=1时候不能输出multiplicity结果）
- syspixie16_revfpku_adc100mhz_r36481 北大定制特殊固件（MultiplicityMaskHigh[31]=0和1时候均能输出multiplicity结果）
- Pixie16DSP_revfgeneral_14b100m_r38495  北大定制固件，当计算的能量为负数时，该值设置为0
- Pixie16DSP_revfgeneral_14b100m_r38896  北大定制固件，当计算的能量为负数时，该值设置为0。pileup事件能量保留，不设置为0。

----

- pixie16_revfpku_14b100m_release_04222018  北大定制固件(DSP降频存在bug)
	- MultiplicityMaskHigh[31]=0和1时候均能输出multiplicity结果
	- 当计算的能量为负数时，该值设置为0。pileup事件能量保留，不设置为0。
	- 在记录波形模式下，waveform 的buffer满了的时候，插件不busy，header继续记录，该情况下，输出的数据没有波形。
	- 在采集波形时候，增加了降频输出的功能，采取的策略为可选择1，1/2，1/4，1/8，1/16，1/32，1/64，1/128频率的输出，即多少个点保留一个点。保留的点是平均后的值。
	
- pixie16_revf_14b250m_firmware_release_04222018  北大定制固件
	- 前面板多重性MultiplicityMaskHigh[31]=0和1时候均能输出multiplicity结果。
	- 当计算的能量为负数时，该值设置为0。pileup事件能量保留，不设置为0。

- pixie16_revfpku_14b100m_firmware_update_05032018  北大定制固件(DSP降频存在bug)
	- MultiplicityMaskHigh[31]=0和1时候均能输出multiplicity结果
	- 当计算的能量为负数时，该值设置为0。pileup事件能量保留，不设置为0。
	- 在记录波形模式下，waveform 的buffer满了的时候，插件不busy，header继续记录，该情况下，输出的数据没有波形。
	- 在采集波形时候，增加了降频输出的功能，采取的策略为可选择1，1/2，1/4，1/8，1/16，1/32，1/64，1/128频率的输出，即多少个点保留一个点。保留的点是平均后的值。

- pixie16_revfpku_14b100m_dsp_update_05082018  北大定制固件
	- MultiplicityMaskHigh[31]=0和1时候均能输出multiplicity结果
	- 当计算的能量为负数时，该值设置为0。pileup事件能量保留，不设置为0。
	- 在记录波形模式下，waveform 的buffer满了的时候，插件不busy，header继续记录，该情况下，输出的数据没有波形。
	- 在采集波形时候，增加了降频输出的功能，采取的策略为可选择1，1/2，1/4，1/8，1/16，1/32，1/64，1/128频率的输出，即多少个点保留一个点。保留的点是平均后的值。

- pixie16_revfpku_14b100m_release_05102018  北大定制固件
	- MultiplicityMaskHigh[31]=0和1时候均能输出multiplicity结果
	- 当计算的能量为负数时，该值设置为0。pileup事件能量保留，不设置为0。
	- 在记录波形模式下，waveform 的buffer满了的时候，插件不busy，header继续记录，该情况下，输出的数据没有波形。
	- 在采集波形时候，增加了降频输出的功能，采取的策略为可选择1，1/2，1/4，1/8，1/16，1/32，1/64，1/128频率的输出，即多少个点保留一个点。保留的点是平均后的值。
	- tried to speed up the event processing, by removing some of the unnecessary wait when reading trace from each channel.
	- Also removed a few unnecessary processing routines in the DSP code:
		- (1) no longer process pile-up rejection or inverse pile-up rejection, all events will be accepted, but still with pileup flag in the event header;
		- (2) removed "no traces for large pulses" feature.




<!-- firmware.md ends here -->
