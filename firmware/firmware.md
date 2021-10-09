<!-- firmware.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 二 12月 13 09:41:49 2016 (+0800)
;; Last-Updated: 五 10月  8 16:57:56 2021 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 54
;; URL: http://wuhongyi.cn -->

# FIRMWARE

## current firmware version

- pixie16_revfpku_14b500m_firmware_release_10062021
	- Output 4-ch debug signals of the front panel A to the chassis backplane's TriggerAll bits [28:31], TrigConfig3[0] control enable/disable
	- send the external timestamp clock and clear as well as the run inhibit to the backplane using the MZTIO card. TrigConfig3[1/2] control front panel/backplane
	- Multiplicity results output from RJ45 port regardless of MultiplicityMaskHigh[31]=0 or 1;
	- The value is set to 0 when the calculated energy is negative.
	- The pileup event energy is not set to 0, output calculated values directly.

- pixie16_revf_general_14b500m_firmware_release_09142021
    - standard firmware

- pixie16_revfpku_14b500m_firmware_release_06102020  PKU firmware(sometime not output when QDC enabled)
	- Output 4-ch debug signals of the front panel A to the chassis backplane's TriggerAll bits [28:31], TrigConfig3[0] control enable/disable
	- send the external timestamp clock and clear as well as the run inhibit to the backplane using the MZTIO card. TrigConfig3[1/2] control front panel/backplane
	- Multiplicity results output from RJ45 port regardless of MultiplicityMaskHigh[31]=0 or 1;
	- The value is set to 0 when the calculated energy is negative.
	- The pileup event energy is not set to 0, output calculated values directly.

- pixie16_revfpku_14b250m_release_11032020  PKU firmware
	- multiplicity results can be output regardless of MultiplicityMaskHigh[31]=0 or 1. Output from front panel A and RJ45.
	- send the external timestamp clock and clear as well as the run inhibit to the backplane using the MZTIO card. TrigConfig3[1/2] control front panel/backplane
	- The value is set to 0 when the calculated energy is negative.
	- The pileup event energy is not set to 0, output calculated values directly.
	- 4-ch debug signals of the front panel A to the chassis backplane’s TriggerAll bits 28 to 31. TrigConfig3[0] control
	- In the record waveform mode, when the waveform buffer is full, the module is not busy, and the header continues to record. In this case, the output event data has no waveform.
	- tried to speed up the event processing, by removing some of the unnecessary wait when reading trace from each channel.
	- Also removed a few unnecessary processing routines in the DSP code:
		- (1) no longer process pile-up rejection or inverse pile-up rejection, all events will be accepted, but still with pileup flag in the event header;
		- (2) removed "no traces for large pulses" feature.	
		
- pixie16_revfpku_16b250m_release_11022020  PKU firmware
	- multiplicity results can be output regardless of MultiplicityMaskHigh[31]=0 or 1. Output from front panel A and RJ45.
	- send the external timestamp clock and clear as well as the run inhibit to the backplane using the MZTIO card. TrigConfig3[1/2] control front panel/backplane
	- The value is set to 0 when the calculated energy is negative.
	- The pileup event energy is not set to 0, output calculated values directly.
	- 4-ch debug signals of the front panel A to the chassis backplane’s TriggerAll bits 28 to 31. TrigConfig3[0] control
	- In the record waveform mode, when the waveform buffer is full, the module is not busy, and the header continues to record. In this case, the output event data has no waveform.
	- tried to speed up the event processing, by removing some of the unnecessary wait when reading trace from each channel.
	- Also removed a few unnecessary processing routines in the DSP code:
		- (1) no longer process pile-up rejection or inverse pile-up rejection, all events will be accepted, but still with pileup flag in the event header;
		- (2) removed "no traces for large pulses" feature.

- pixie16_revfpku_14b100m_firmware_release_06092020  PKU firmware(DSP down frequency 1/4 output has bug)
	- 4-ch debug signals of the front panel A to the chassis backplane's TriggerAll bits 28 to 31. TrigConfig3[0] control
	- send the external timestamp clock and clear as well as the run inhibit to the backplane using the MZTIO card. TrigConfig3[1/2] control front panel/backplane
	- multiplicity results can be output regardless of MultiplicityMaskHigh[31]=0 or 1.  Output from front panel A and RJ45.
	- The value is set to 0 when the calculated energy is negative. 
	- The pileup event energy is not set to 0, output calculated values directly.
	- In the record waveform mode, when the waveform buffer is full, the module is not busy, and the header continues to record. In this case, the output event data has no waveform.
	- The record waveform mode with down frequency output. The strategy adopted is to select the output of 1, 1/2, 1/4, 1/8, 1/16, 1/32, 1/64, 1/128 frequency, ie how many points retain one point. The points retained are the averaged values.
	- tried to speed up the event processing, by removing some of the unnecessary wait when reading trace from each channel.
	- Also removed a few unnecessary processing routines in the DSP code:
		- (1) no longer process pile-up rejection or inverse pile-up rejection, all events will be accepted, but still with pileup flag in the event header;
		- (2) removed "no traces for large pulses" feature.

- pixie16_revfpku_14b100m_firmware_release_05082018  PKU firmware
	- multiplicity results(front panel A) can be output regardless of MultiplicityMaskHigh[31]=0 or 1
	- The value is set to 0 when the calculated energy is negative. 
	- The pileup event energy is not set to 0, output calculated values directly.
	- In the record waveform mode, when the waveform buffer is full, the module is not busy, and the header continues to record. In this case, the output event data has no waveform.
	- The record waveform mode with down frequency output. The strategy adopted is to select the output of 1, 1/2, 1/4, 1/8, 1/16, 1/32, 1/64, 1/128 frequency, ie how many points retain one point. The points retained are the averaged values.



## testing firmware

- pixie16_revf_general_14b100m_firmware_release   standard firmware

- pixie16_revd_12b100m_firmware_release  standard firmware

- pixie16_revf_12b250m_firmware_release  standard firmware

- pixie16_revf_14b500m_firmware_release  standard firmware(sometime not output when QDC enabled) 
	
----

## history if firmware

- pixie16_revf_14b100m_firmware_release_11032016 Standard firmware
- syspixie16_revfpku_adc100mhz_r36473 PKU firmware（multiplicity results(front panel A) can be output when MultiplicityMaskHigh[31]=0）
- syspixie16_revfpku_adc100mhz_r36481 PKU firmware（multiplicity results(front panel A) can be output regardless of MultiplicityMaskHigh[31]=0 or 1）
- Pixie16DSP_revfgeneral_14b100m_r38495  PKU firmware. The value is set to 0 when the calculated energy is negative. 
- Pixie16DSP_revfgeneral_14b100m_r38896  PKU firmware. The value is set to 0 when the calculated energy is negative. The pileup event energy is not set to 0, output calculated values directly.

----

- pixie16_revfpku_14b100m_release_04222018  PKU firmware(DSP down frequency output has bug)
	- multiplicity results(front panel A) can be output regardless of MultiplicityMaskHigh[31]=0 or 1
	- The value is set to 0 when the calculated energy is negative. 
	- The pileup event energy is not set to 0, output calculated values directly.
	- In the record waveform mode, when the waveform buffer is full, the module is not busy, and the header continues to record. In this case, the output event data has no waveform.
	- The record waveform mode with down frequency output. The strategy adopted is to select the output of 1, 1/2, 1/4, 1/8, 1/16, 1/32, 1/64, 1/128 frequency, ie how many points retain one point. The points retained are the averaged values.
	
- pixie16_revf_14b250m_firmware_release_04222018  PKU firmware (OCR > ICR at same pars setting)
	- multiplicity results(front panel A) can be output regardless of MultiplicityMaskHigh[31]=0 or 1
	- The value is set to 0 when the calculated energy is negative. 
	- The pileup event energy is not set to 0, output calculated values directly.

- pixie16_revfpku_14b100m_firmware_update_05032018  PKU firmware(DSP down frequency output has bug)
	- multiplicity results(front panel A) can be output regardless of MultiplicityMaskHigh[31]=0 or 1
	- The value is set to 0 when the calculated energy is negative. 
	- The pileup event energy is not set to 0, output calculated values directly.
	- In the record waveform mode, when the waveform buffer is full, the module is not busy, and the header continues to record. In this case, the output event data has no waveform.
	- The record waveform mode with down frequency output. The strategy adopted is to select the output of 1, 1/2, 1/4, 1/8, 1/16, 1/32, 1/64, 1/128 frequency, ie how many points retain one point. The points retained are the averaged values.

- pixie16_revfpku_14b100m_dsp_update_05082018  PKU firmware
	- multiplicity results(front panel A) can be output regardless of MultiplicityMaskHigh[31]=0 or 1
	- The value is set to 0 when the calculated energy is negative. 
	- The pileup event energy is not set to 0, output calculated values directly.
	- In the record waveform mode, when the waveform buffer is full, the module is not busy, and the header continues to record. In this case, the output event data has no waveform.
	- The record waveform mode with down frequency output. The strategy adopted is to select the output of 1, 1/2, 1/4, 1/8, 1/16, 1/32, 1/64, 1/128 frequency, ie how many points retain one point. The points retained are the averaged values.

- pixie16_revfpku_14b100m_release_05102018  PKU firmware(When the transmission data is large per second, there are problems in the output data)
	- multiplicity results(front panel A) can be output regardless of MultiplicityMaskHigh[31]=0 or 1
	- The value is set to 0 when the calculated energy is negative. 
	- The pileup event energy is not set to 0, output calculated values directly.
	- In the record waveform mode, when the waveform buffer is full, the module is not busy, and the header continues to record. In this case, the output event data has no waveform.
	- The record waveform mode with down frequency output. The strategy adopted is to select the output of 1, 1/2, 1/4, 1/8, 1/16, 1/32, 1/64, 1/128 frequency, ie how many points retain one point. The points retained are the averaged values.
	- tried to speed up the event processing, by removing some of the unnecessary wait when reading trace from each channel.
	- Also removed a few unnecessary processing routines in the DSP code:
		- (1) no longer process pile-up rejection or inverse pile-up rejection, all events will be accepted, but still with pileup flag in the event header;
		- (2) removed "no traces for large pulses" feature.

- pixie16_revfpku_14b100m_dsp_code_update_08232018  PKU firmware(DSP down frequency 1/4 output has bug)
	- multiplicity results(front panel A) can be output regardless of MultiplicityMaskHigh[31]=0 or 1
	- The value is set to 0 when the calculated energy is negative. 
	- The pileup event energy is not set to 0, output calculated values directly.
	- In the record waveform mode, when the waveform buffer is full, the module is not busy, and the header continues to record. In this case, the output event data has no waveform.
	- The record waveform mode with down frequency output. The strategy adopted is to select the output of 1, 1/2, 1/4, 1/8, 1/16, 1/32, 1/64, 1/128 frequency, ie how many points retain one point. The points retained are the averaged values.
	- tried to speed up the event processing, by removing some of the unnecessary wait when reading trace from each channel.
	- Also removed a few unnecessary processing routines in the DSP code:
		- (1) no longer process pile-up rejection or inverse pile-up rejection, all events will be accepted, but still with pileup flag in the event header;
		- (2) removed "no traces for large pulses" feature.

- pixie16_revfpku_14b100m_firmware_release_09272018  PKU firmware(DSP down frequency 1/4 output has bug)
	- multiplicity results can be output regardless of MultiplicityMaskHigh[31]=0 or 1.  Output from front panel A and RJ45.
	- The value is set to 0 when the calculated energy is negative. 
	- The pileup event energy is not set to 0, output calculated values directly.
	- In the record waveform mode, when the waveform buffer is full, the module is not busy, and the header continues to record. In this case, the output event data has no waveform.
	- The record waveform mode with down frequency output. The strategy adopted is to select the output of 1, 1/2, 1/4, 1/8, 1/16, 1/32, 1/64, 1/128 frequency, ie how many points retain one point. The points retained are the averaged values.
	- tried to speed up the event processing, by removing some of the unnecessary wait when reading trace from each channel.
	- Also removed a few unnecessary processing routines in the DSP code:
		- (1) no longer process pile-up rejection or inverse pile-up rejection, all events will be accepted, but still with pileup flag in the event header;
		- (2) removed "no traces for large pulses" feature.

- Pixie16DSP_RevDGeneral_r40804_NoNegEnergy_AlwaysCompEnergy   PKU firmware(cfd sometime not working)
	- The value is set to 0 when the calculated energy is negative. 
	- The pileup event energy is not set to 0, output calculated values directly.

- pixie16_revf_14b250m_firmware_release_11082018  PKU firmware (OCR > ICR at same pars setting)
	- multiplicity results can be output regardless of MultiplicityMaskHigh[31]=0 or 1. Output from front panel A and RJ45.
	- The value is set to 0 when the calculated energy is negative. 
	- The pileup event energy is not set to 0, output calculated values directly.

- pixie16_revf_16b250m_firmware standard firmware (OCR > ICR at same pars setting)

- pixie16_revfpku_16b250m_firmware_release_01132019   PKU firmware(not anything output)
	- multiplicity results can be output regardless of MultiplicityMaskHigh[31]=0 or 1. Output from front panel A and RJ45.
	- The value is set to 0 when the calculated energy is negative.
	- The pileup event energy is not set to 0, output calculated values directly.

- pixie16_revfpku_16b250m_release_01192019   PKU firmware(OCR > ICR at same pars setting)
	- multiplicity results can be output regardless of MultiplicityMaskHigh[31]=0 or 1. Output from front panel A and RJ45.
	- The value is set to 0 when the calculated energy is negative.
	- The pileup event energy is not set to 0, output calculated values directly.

- pixie16_revfpku_16b250m_release_02072019   PKU firmware(OCR > ICR at same pars setting)
	- multiplicity results can be output regardless of MultiplicityMaskHigh[31]=0 or 1. Output from front panel A and RJ45.
	- The value is set to 0 when the calculated energy is negative.
	- The pileup event energy is not set to 0, output calculated values directly.

- pixie16_revfpku_16b250m_release_02182019   PKU firmware(not data output when choose QDC sums)
	- multiplicity results can be output regardless of MultiplicityMaskHigh[31]=0 or 1. Output from front panel A and RJ45.
	- The value is set to 0 when the calculated energy is negative.
	- The pileup event energy is not set to 0, output calculated values directly.

- pixie16_revfpku_14b250m_release_02232019   PKU firmware(not data output when choose QDC sums)
	- multiplicity results can be output regardless of MultiplicityMaskHigh[31]=0 or 1. Output from front panel A and RJ45.
	- The value is set to 0 when the calculated energy is negative.
	- The pileup event energy is not set to 0, output calculated values directly.

- pixie16_revf_12b250m_firmware_release_05182019  standard firmware

- pixie16_revfpku_14b250m_release_06092019   PKU firmware(sometime not output)
	- multiplicity results can be output regardless of MultiplicityMaskHigh[31]=0 or 1. Output from front panel A and RJ45.
	- The value is set to 0 when the calculated energy is negative.
	- The pileup event energy is not set to 0, output calculated values directly.
	
- pixie16_revfpku_16b250m_release_06092019   PKU firmware(sometime not output)
	- multiplicity results can be output regardless of MultiplicityMaskHigh[31]=0 or 1. Output from front panel A and RJ45.
	- The value is set to 0 when the calculated energy is negative.
	- The pileup event energy is not set to 0, output calculated values directly.
	
- pixie16_revf_14b500m_firmware_release  standard firmware	

- pixie16_revfpku_14b100m_firmware_release_10142019  PKU firmware(DSP down frequency 1/4 output has bug)
	- 4-ch debug signals of the front panel A to the chassis backplane's TriggerAll bits 28 to 31. TrigConfig3[0] control
	- multiplicity results can be output regardless of MultiplicityMaskHigh[31]=0 or 1.  Output from front panel A and RJ45.
	- The value is set to 0 when the calculated energy is negative. 
	- The pileup event energy is not set to 0, output calculated values directly.
	- In the record waveform mode, when the waveform buffer is full, the module is not busy, and the header continues to record. In this case, the output event data has no waveform.
	- The record waveform mode with down frequency output. The strategy adopted is to select the output of 1, 1/2, 1/4, 1/8, 1/16, 1/32, 1/64, 1/128 frequency, ie how many points retain one point. The points retained are the averaged values.
	- tried to speed up the event processing, by removing some of the unnecessary wait when reading trace from each channel.
	- Also removed a few unnecessary processing routines in the DSP code:
		- (1) no longer process pile-up rejection or inverse pile-up rejection, all events will be accepted, but still with pileup flag in the event header;
		- (2) removed "no traces for large pulses" feature.

- pixie16_revfpku_14b100m_firmware_release_06092020  PKU firmware(DSP down frequency 1/4 output has bug)
	- 4-ch debug signals of the front panel A to the chassis backplane's TriggerAll bits 28 to 31. TrigConfig3[0] control
	- send the external timestamp clock and clear as well as the run inhibit to the backplane using the MZTIO card. TrigConfig3[1/2] control front panel/backplane
	- multiplicity results can be output regardless of MultiplicityMaskHigh[31]=0 or 1.  Output from front panel A and RJ45.
	- The value is set to 0 when the calculated energy is negative. 
	- The pileup event energy is not set to 0, output calculated values directly.
	- In the record waveform mode, when the waveform buffer is full, the module is not busy, and the header continues to record. In this case, the output event data has no waveform.
	- The record waveform mode with down frequency output. The strategy adopted is to select the output of 1, 1/2, 1/4, 1/8, 1/16, 1/32, 1/64, 1/128 frequency, ie how many points retain one point. The points retained are the averaged values.
	- tried to speed up the event processing, by removing some of the unnecessary wait when reading trace from each channel.
	- Also removed a few unnecessary processing routines in the DSP code:
		- (1) no longer process pile-up rejection or inverse pile-up rejection, all events will be accepted, but still with pileup flag in the event header;
		- (2) removed "no traces for large pulses" feature.

- pixie16_revfpku_14b500m_firmware_release_06102020  PKU firmware(sometime not output when QDC enabled)
	- Output 4-ch debug signals of the front panel A to the chassis backplane's TriggerAll bits [28:31], TrigConfig3[0] control enable/disable
	- send the external timestamp clock and clear as well as the run inhibit to the backplane using the MZTIO card. TrigConfig3[1/2] control front panel/backplane
	- Multiplicity results output from RJ45 port regardless of MultiplicityMaskHigh[31]=0 or 1;
	- The value is set to 0 when the calculated energy is negative.
	- The pileup event energy is not set to 0, output calculated values directly.

- pixie16_revfpku_14b250m_release_06102020  PKU firmware(sometime not output)
	- multiplicity results can be output regardless of MultiplicityMaskHigh[31]=0 or 1. Output from front panel A and RJ45.
	- send the external timestamp clock and clear as well as the run inhibit to the backplane using the MZTIO card. TrigConfig3[1/2] control front panel/backplane
	- The value is set to 0 when the calculated energy is negative.
	- The pileup event energy is not set to 0, output calculated values directly.
	- 4-ch debug signals of the front panel A to the chassis backplane’s TriggerAll bits 28 to 31. TrigConfig3[0] control
	- In the record waveform mode, when the waveform buffer is full, the module is not busy, and the header continues to record. In this case, the output event data has no waveform.
	- tried to speed up the event processing, by removing some of the unnecessary wait when reading trace from each channel.
	- Also removed a few unnecessary processing routines in the DSP code:
		- (1) no longer process pile-up rejection or inverse pile-up rejection, all events will be accepted, but still with pileup flag in the event header;
		- (2) removed "no traces for large pulses" feature.	
	
- pixie16_revfpku_16b250m_release_06102020  PKU firmware(sometime not output)
	- multiplicity results can be output regardless of MultiplicityMaskHigh[31]=0 or 1. Output from front panel A and RJ45.
	- send the external timestamp clock and clear as well as the run inhibit to the backplane using the MZTIO card. TrigConfig3[1/2] control front panel/backplane
	- The value is set to 0 when the calculated energy is negative.
	- The pileup event energy is not set to 0, output calculated values directly.
	- 4-ch debug signals of the front panel A to the chassis backplane’s TriggerAll bits 28 to 31. TrigConfig3[0] control
	- In the record waveform mode, when the waveform buffer is full, the module is not busy, and the header continues to record. In this case, the output event data has no waveform.
	- tried to speed up the event processing, by removing some of the unnecessary wait when reading trace from each channel.
	- Also removed a few unnecessary processing routines in the DSP code:
		- (1) no longer process pile-up rejection or inverse pile-up rejection, all events will be accepted, but still with pileup flag in the event header;
		- (2) removed "no traces for large pulses" feature.

- pixie16_revfpku_16b250m_release_11022020  PKU firmware
	- multiplicity results can be output regardless of MultiplicityMaskHigh[31]=0 or 1. Output from front panel A and RJ45.
	- send the external timestamp clock and clear as well as the run inhibit to the backplane using the MZTIO card. TrigConfig3[1/2] control front panel/backplane
	- The value is set to 0 when the calculated energy is negative.
	- The pileup event energy is not set to 0, output calculated values directly.
	- 4-ch debug signals of the front panel A to the chassis backplane’s TriggerAll bits 28 to 31. TrigConfig3[0] control
	- In the record waveform mode, when the waveform buffer is full, the module is not busy, and the header continues to record. In this case, the output event data has no waveform.
	- tried to speed up the event processing, by removing some of the unnecessary wait when reading trace from each channel.
	- Also removed a few unnecessary processing routines in the DSP code:
		- (1) no longer process pile-up rejection or inverse pile-up rejection, all events will be accepted, but still with pileup flag in the event header;
		- (2) removed "no traces for large pulses" feature.

- pixie16_revfpku_14b250m_release_11032020  PKU firmware
	- multiplicity results can be output regardless of MultiplicityMaskHigh[31]=0 or 1. Output from front panel A and RJ45.
	- send the external timestamp clock and clear as well as the run inhibit to the backplane using the MZTIO card. TrigConfig3[1/2] control front panel/backplane
	- The value is set to 0 when the calculated energy is negative.
	- The pileup event energy is not set to 0, output calculated values directly.
	- 4-ch debug signals of the front panel A to the chassis backplane’s TriggerAll bits 28 to 31. TrigConfig3[0] control
	- In the record waveform mode, when the waveform buffer is full, the module is not busy, and the header continues to record. In this case, the output event data has no waveform.
	- tried to speed up the event processing, by removing some of the unnecessary wait when reading trace from each channel.
	- Also removed a few unnecessary processing routines in the DSP code:
		- (1) no longer process pile-up rejection or inverse pile-up rejection, all events will be accepted, but still with pileup flag in the event header;
		- (2) removed "no traces for large pulses" feature.	
		

- pixie16_revf_general_14b500m_firmware_release_09142021
    - standard firmware

- pixie16_revfpku_14b500m_firmware_release_10062021
	- Output 4-ch debug signals of the front panel A to the chassis backplane's TriggerAll bits [28:31], TrigConfig3[0] control enable/disable
	- send the external timestamp clock and clear as well as the run inhibit to the backplane using the MZTIO card. TrigConfig3[1/2] control front panel/backplane
	- Multiplicity results output from RJ45 port regardless of MultiplicityMaskHigh[31]=0 or 1;
	- The value is set to 0 when the calculated energy is negative.
	- The pileup event energy is not set to 0, output calculated values directly.



<!-- firmware.md ends here -->
