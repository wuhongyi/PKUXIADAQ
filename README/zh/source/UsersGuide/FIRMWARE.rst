.. FIRMWARE.rst --- 
.. 
.. Description: 
.. Author: Hongyi Wu(吴鸿毅)
.. Email: wuhongyi@qq.com 
.. Created: 二 7月  2 22:13:09 2019 (+0800)
.. Last-Updated: 四 6月 11 23:35:05 2020 (+0800)
..           By: Hongyi Wu(吴鸿毅)
..     Update #: 10
.. URL: http://wuhongyi.cn 

=================================
固件
=================================

**北京大学定制固件**  

在标准固件的基础上添加了以下功能：

- 100MHz 12 bit(pixie16_revd_12b100m_firmware_release)
	- 标准固件

- 100MHz 14 bit(pixie16_revf_general_14b100m_firmware_release)	  
	- 标准固件

- 100MHz 14 bit(pixie16_revfpku_14b100m_firmware_release_06092020) 波形降频1/4时存在问题
        - 4 路调试监视信号输出到机箱背板 TriggerAll[28:31]，通过 TrigConfig3[0] 来控制是否开启
	- 通过机箱背板传输外部时钟清除信号、外部时钟信号、run inhibit 信号。通过 TrigConfig3[1/2] 来控制信号源是前面板还是机箱背板
	- MultiplicityMaskHigh[31]=0和1 时候RJ45口与前面板A均能输出 multiplicity 结果。
	- 当计算的能量为负数时，该值设置为 0。
	- pileup 事件能量保留，不设置为 0。
	- 在记录波形模式下，waveform 的buffer满了的时候，插件不busy，header继续记录，该情况下，输出的数据没有波形。
	- 在采集波形时候，增加了降频输出的功能，采取的策略为可选择1，1/2，1/4，1/8，1/16，1/32，1/64，1/128频率的输出，即多少个点保留一个点。保留的点是平均后的值。
	- 删除一些非必要的等待来加速事件处理。
	- 删除DSP中非必要的处理：包括堆积检测、超量程波形检测等
	  
- 100MHz 14 bit(pixie16_revfpku_14b100m_dsp_update_05082018)
	- MultiplicityMaskHigh[31]=0和1 时候前面板均能输出 multiplicity 结果。
	- 当计算的能量为负数时，该值设置为 0。
	- pileup 事件能量保留，不设置为 0。
	- 在记录波形模式下，waveform 的buffer满了的时候，插件不busy，header继续记录，该情况下，输出的数据没有波形。
	- 在采集波形时候，增加了降频输出的功能，采取的策略为可选择1，1/2，1/4，1/8，1/16，1/32，1/64，1/128频率的输出，即多少个点保留一个点。保留的点是平均后的值。
	
- 250MHz 12bit(pixie16_revf_12b250m_firmware_release)	
	- 标准固件
  
- 250MHz 14bit(pixie16_revfpku_14b250m_release_06102020) 存在问题
	- MultiplicityMaskHigh[31]=0和1 时候RJ45口与前面板均能输出 multiplicity 结果。
	- 通过机箱背板传输外部时钟清除信号、外部时钟信号、run inhibit 信号。通过 TrigConfig3[1/2] 来控制信号源是前面板还是机箱背板
	- 当计算的能量为负数时，该值设置为 0。
	- pileup 事件能量保留，不设置为 0。
	- 4 路调试监视信号输出到机箱背板 TriggerAll[28:31]，通过 TrigConfig3[0] 来控制是否开启
	  

- 250MHz 16bit(pixie16_revfpku_16b250m_release_06102020) 存在问题
	- MultiplicityMaskHigh[31]=0和1 时候RJ45口与前面板均能输出 multiplicity 结果。
	- 通过机箱背板传输外部时钟清除信号、外部时钟信号、run inhibit 信号。通过 TrigConfig3[1/2] 来控制信号源是前面板还是机箱背板
	- 当计算的能量为负数时，该值设置为 0。
	- pileup 事件能量保留，不设置为 0。
	- 4 路调试监视信号输出到机箱背板 TriggerAll[28:31]，通过 TrigConfig3[0] 来控制是否开启
	- 删除一些非必要的等待来加速事件处理。
	- 删除DSP中非必要的处理：包括堆积检测、超量程波形检测等	  

- pixie16_revfpku_14b500m_firmware_release_06102020  PKU firmware
	- 4 路调试监视信号输出到机箱背板 TriggerAll[28:31]，通过 TrigConfig3[0] 来控制是否开启
	- 通过机箱背板传输外部时钟清除信号、外部时钟信号、run inhibit 信号。通过 TrigConfig3[1/2] 来控制信号源是前面板还是机箱背板
	- MultiplicityMaskHigh[31]=0和1 时候RJ45口输出 multiplicity 结果。
	- 当计算的能量为负数时，该值设置为 0。
	- pileup 事件能量保留，不设置为 0。
	- 删除一些非必要的等待来加速事件处理。
	- 删除DSP中非必要的处理：包括堆积检测、超量程波形检测等

	  
- 500MHz 14bit(pixie16_revf_14b500m_firmware_release)	  
	- 标准固件
	  
.. 
.. FIRMWARE.rst ends here
