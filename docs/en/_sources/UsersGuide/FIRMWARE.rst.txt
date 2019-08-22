.. FIRMWARE.rst --- 
.. 
.. Description: 
.. Author: Hongyi Wu(吴鸿毅)
.. Email: wuhongyi@qq.com 
.. Created: 二 7月  2 22:13:09 2019 (+0800)
.. Last-Updated: 四 8月 22 13:22:21 2019 (+0800)
..           By: Hongyi Wu(吴鸿毅)
..     Update #: 4
.. URL: http://wuhongyi.cn 

=================================
FIRMWARE
=================================

**Peking University Custom Firmware**  

Added the following features based on standard firmware：

- 100MHz 12 bit(pixie16_revd_12b100m_firmware_release)
	- standard firmware

- 100MHz 14 bit(pixie16_revfpku_14b100m_firmware_release_09272018)
	- multiplicity results can be output regardless of MultiplicityMaskHigh[31]=0 or 1.  Output from front panel A and RJ45.
	- The value is set to 0 when the calculated energy is negative. 
	- The pileup event energy is not set to 0, output calculated values directly.
	- In the record waveform mode, when the waveform buffer is full, the module is not busy, and the header continues to record. In this case, the output event data has no waveform.
	- The record waveform mode with down frequency output. The strategy adopted is to select the output of 1, 1/2, 1/4, 1/8, 1/16, 1/32, 1/64, 1/128 frequency, ie how many points retain one point. The points retained are the averaged values.
	- tried to speed up the event processing, by removing some of the unnecessary wait when reading trace from each channel.
	- Also removed a few unnecessary processing routines in the DSP code:
		- (1) no longer process pile-up rejection or inverse pile-up rejection, all events will be accepted, but still with pileup flag in the event header;
		- (2) removed "no traces for large pulses" feature.

- 100MHz 14 bit(pixie16_revfpku_14b100m_dsp_update_05082018)
	- multiplicity results(front panel A) can be output regardless of MultiplicityMaskHigh[31]=0 or 1
	- The value is set to 0 when the calculated energy is negative. 
	- The pileup event energy is not set to 0, output calculated values directly.
	- In the record waveform mode, when the waveform buffer is full, the module is not busy, and the header continues to record. In this case, the output event data has no waveform.
	- The record waveform mode with down frequency output. The strategy adopted is to select the output of 1, 1/2, 1/4, 1/8, 1/16, 1/32, 1/64, 1/128 frequency, ie how many points retain one point. The points retained are the averaged values.
	
- 250MHz 12bit(pixie16_revf_12b250m_firmware_release)	
	- standard firmware

- 250MHz 14bit(pixie16_revfpku_14b250m_release_06092019)
	- multiplicity results can be output regardless of MultiplicityMaskHigh[31]=0 or 1. Output from front panel A and RJ45.
	- The value is set to 0 when the calculated energy is negative. 
	- The pileup event energy is not set to 0, output calculated values directly.

- 250MHz 16bit(pixie16_revfpku_16b250m_release_06092019)
	- multiplicity results can be output regardless of MultiplicityMaskHigh[31]=0 or 1. Output from front panel A and RJ45.
	- The value is set to 0 when the calculated energy is negative. 
	- The pileup event energy is not set to 0, output calculated values directly.

- 500MHz 14bit(pixie16_revf_14b500m_firmware_release)
	- standard firmware
  
.. 
.. FIRMWARE.rst ends here
