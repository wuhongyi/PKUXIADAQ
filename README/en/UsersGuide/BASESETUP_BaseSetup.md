<!-- BASESETUP_BaseSetup.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 10月  7 09:36:08 2018 (+0800)
;; Last-Updated: 一 11月  5 16:37:59 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 4
;; URL: http://wuhongyi.cn -->

# Base Setup

![Base Setup](/img/BaseSetup.png)

The status below the interface shows a **green Ready** to indicate that the interface can be operated, otherwise you need to wait.

In the interface, the parameters behind the **Module** are used to select the adjusted module, **Load** is used to read the parameter value of the modules, and **Apply** is to write the value in the interface to the module.

The parameter following the **Select Channel** at the bottom of the interface indicates that you can choose to copy the parameters of the channel on the interface to other channels, click **Copy** to complete the copy, and then **Apply** to write the parameters to the module.



The **Base Setup** page controls the analog gain, offset and polarity for each channel. It is useful to click on **Trace & Baseline** in the top control **Monitor** bar to view the signal read from the ADCs while adjusting these parameters. The display shows one or all 16 channels of a module; you can set the sampling interval for each block to capture a longer time frame in **Hist & XDT** page. Click **Draw** to update the graph.

Pulses from the detector should fall in the range from 0 to 16383(14 bit), with the baseline at ~1638 to allow for drifts and/or undershoots and no clipping at the upper limit. If there is clipping, adjust the Gain and Offset or click on the *AdjustOffset* button to let the software set the DC offsets to proper levels.

Since the trigger/filter circuits in the FPGA only act on rising pulses, negative pulses are inverted at the input of the FPGA, and the waveforms shown in the ADC trace display include this optional inversion. Thus set the channel’s Polarity such that pulses from the detector appear with positive amplitude (rising edge).


In the **Base Setup**  tab, you can set the total trace length and the pre-trigger trace delay for the waveforms to be acquired in list mode runs.

The trace delay cannot be longer than the trace length, and for each Pixie-16 variant, there is also a limit for the maximum value of trace delay and trace length.

----

## Parameters introduction

- The option *Gain* indicates the gain adjustment. The user can select the *Larger* or *Small*. The gain parameters corresponding to the two files of each module can be tested by the user or be chosen by consulting the manufacturer.
- The option *Sign* indicates the polarity of the selected input signal. The positive signal selects "+" and the negative signal selects "-".
- The option *GC* indicates whether the channel is data recorded. If it is checked, it means that the channel is recorded. If it is not checked, it means that it is not recorded.
- The option *ECT* means to enable CFD trigger. Otherwise, regular trapezoidal fast trigger will be used.



The red *TC*, *EQS*, *ERB* are used to select which raw data to output:  
- The option *TC* indicates the recording waveform. At this time, *TraceDelay* and *TraceLength* are valid. If not selected, the waveform is not recorded.
- The option *EQS* means that the scores of the eight QDCs are recorded, and if they are not selected, they are not recorded.
- The option *ERB* represents the three-part area integral of the energy trapezoid(raw energy sums) and the baseline value of the trapezoidal calculation.



The green *TraceDelay* and *TraceLength* are the points of the output data. The parameter is divided by the nominal sampling rate of the acquisition card to calculate the actual output data points of the waveform:
- *TraceDelay* indicates the length of the acquired waveform before the trigger.
- *TraceLength* represents the entire waveform acquisition length. 
It should be specially noted that when using the down frequency mode, the actual waveform length is TraceDelay x 2^N / TraceLength x 2^N (N is the down-frequency parameter)。



The blue *Baseline* is used to adjust the baseline position and adjust the baseline to the user's expected position by voltage compensation:
- The *Baseline* adjustable range is 0 - 100, which indicates the percentage of the waveform's baseline that falls within the full scale. For example, for a vertical precision 14-bit capture card, setting this parameter to 10 means that the baseline reduction compensation is adjusted to around 10% of the 16384 full-scale, near 1638.
- The purple *DCOffset* and *BLcut* users do not need to be modified, and the parameters can be adjusted automatically. After modifying *Baseline*, *Gain*, and *Sign* in this submenu, you need to press the bottom of the *AdjustOffset*, and then press *BLcutFinder* to automatically adjust these two parameters.



## Important note


> **[info] trace length in 500 MHz**
>
> For the 500 MHz Pixie-16 modules, the ADCs are running at 500 MHz, but the traces are recorded with 100 MHz clocks in the FPGA with 5 ADC samples captured in each 10 ns interval. In addition, the data packing from the FPGA to the onboard External FIFO is two sets of 5 ADC samples in one transfer. So the trace length should be multiples of 20 ns, i.e., 20 ns, 40 ns, ... for instance, a trace length of 500 ns and a trace delay of 200 ns.

----

> **[info] Good channel**
>
> Only channels marked as good will have their events recorded. 
>
> This setting has no bearing on the channel's capability to issue a trigger. 
>
> There can be a triggering channel whose data are discarded. 
>
> Channels not marked as good will be excluded from the automatic offset adjustment.



## Baseline measurements

The Pixie-16 constantly takes baseline measurements when no pulse is detected and keeps a baseline average to be subtracted from the energy filter output during pulse height reconstruction. Baseline measurements that differ from the average by more than the BaselineCut value will be rejected as they are likely contaminated with small pulses below the trigger threshold.

A series of baseline measurements for each channel can be viewed in **Trace & Baseline** page, and in the BASELINE panel a histogram of baselines can be built to verify that the Baseline Cut does not reject measurements falling into the main (ideally Gaussian) peak in the baseline distribution.

Usually, it is sufficient to keep Baseline Cut at its default value.


Note: Since the baseline computation takes into account the exponential decay, no pulses should be noticeable in the baseline display if  
- a) the decay time is set correctly and 
- b) the detector pulses are truly exponential.

Baseline Percent is a parameter used for automatic offset adjustment; by clicking on the *AdjustOffses* button, offsets will be set such that the baseline seen in the ADC trace display falls at the Baseline Percent fraction of the full ADC range (e.g. for a 12-bit ADC and Baseline Percent = 10% the baseline falls at ADC step 409 out of 4096 total).



<!-- BASESETUP_BaseSetup.md ends here -->
