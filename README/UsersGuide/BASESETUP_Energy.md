<!-- BASESETUP_Energy.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 10月  7 09:34:45 2018 (+0800)
;; Last-Updated: 日 10月  7 11:14:03 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 7
;; URL: http://wuhongyi.cn -->

# Energy

<!-- toc -->

## GUI

![Energy](/img/Energy.png)

界面下方的 Status 显示为**绿色的 Ready** 时表示可操作该界面，否则需要等待。底下按钮的操作同上。

- 参数 *Rise Time* 。。。
- 参数 *Flat Top* 。。。
- 参数 *Tau* 。。。
- 参数 filter range

**TODO**


The most critical parameter for the energy computation is the signal decay time Tau. It is used to compensate for the falling edge of a previous pulse in the computation of the energy. You can either enter Tau directly for each channel, or enter an approximate value in the right control, select a channel, and click Find it to let the software determine the decay time automatically.
Click Accept it to apply the found value to the channel. (If the approximate value is unchanged,the software could not find a better value.)  


At high count rates, pulses overlap with each other at higher frequency. In order to compute the energy or pulse height of those pulses accurately without the need to wait until they decay back to baseline level completely, the pulse height computation algorithm implemented in the Pixie-16 uses the decay time to compute and remove the contribution from the exponentially decaying tail of the overlapping prior pulse when computing the pulse height of the current pulse.

> **[danger] single exponential decay constant**
>
> It is assumed the pulses have only a single exponential decay constant. If pulses have multiple decay constants, it might be possible to use the decay constant that dominates the decay of the pulse, but the accuracy of pulse height computation will be degraded.





General rules of thumb for the following important parameters are:

- The energy filter flat top time should be larger than the longest pulse rise time.
- The energy filter rise time can be varied to balance the resolution and throughput.
- In general, energy resolution improves with the increase of energy filter rise time, up to an optimum when longer filters only add more noise into the measurement.
- The energy filter dead time TD is about $$2×(T_{rise}+T_{flat})$$, and the maximum throughput for Poisson statistics is 1/(TD*e). For HPGe detectors, a rise time of 4-6us and a flat top of 1us are usually appropriate.
- Choose the smallest energy filter range that allows setting the optimum energy filter rise time. Larger filter ranges allow longer filter sums, but increase the granularity of possible values for the energy filter rise time and flat top time and increase the jitter of latching the energy filter output relative to the rising edge of the pulse. This is usually only important for very fast pulses.

----

## Filter Range

To accommodate a wide range of energy filter rise times from tens of nanoseconds to tens of microseconds, the filters are implemented in the FPGA with different clock decimations(filter ranges). The ADC sampling rate is either 2ns, 4ns, or 10ns depending on the ADC variant that is used, but in higher clock decimations, several ADC samples are averaged before entering the energy filtering logic. In filter range 1, $$2^{1}$$ samples are averaged, $$2^{2}$$ samples in filter range 2, and so on. Since the sum of rise time and flat top is limited to 127 decimated clock cycles, filter time granularity and filter time are limited to the values listed in Table .

![Filter clock decimations and filter time granularity for 100 MHz or 500 MHz](/img/filterclockdecimationsandfiltertimegranularityfor100mhzor500mhz.png)

![Filter clock decimations and filter time granularity for 250 MHz](/img/filterclockdecimationsandfiltertimegranularityfor250mhz.png)

----

## Trapezoidal Filtering

From this point onward, only trapezoidal filtering will be considered as it is implemented in a Pixie-16 module according to Equation $$LV_{x,k}=-\sum{i=k-2L-G+1}{k-L-G}V_{i}+\sum{i=k-L+1}{k}V_{i}$$. The result of applying such a filter with Length L=1 us and Gap G=0.4 us to a gamma-ray event is shown in Figure. The filter output is clearly trapezoidal in shape and has a rise time equal to L, a flattop equal to G, and a symmetrical fall time equal to L. The basewidth, which is a first-order measure of the filter’s noise reduction properties, is thus 2L+G.

![Trapezoidal filtering of a preamplifier step with L=1μs and G=0.4μs](/img/trapezoidalfilteringofapreamplifierstepwithl1usandg04us.png)


This raises several important points in comparing the noise performance of the Pixie-16 module to analog filtering amplifiers. 
- First, semi-Gaussian filters are usually specified by a shaping time. 
    - Their rise time is typically twice this and their pulses are not symmetric so that the basewidth is about 5.6 times the shaping time or 2.8 times their rise time. 
- Thus a semi-Gaussian filter typically has a slightly better energy resolution than a triangular filter of the same rise time because it has a longer filtering time. 
    - This is typically accommodated in amplifiers offering both triangular and semi-Gaussian filtering by stretching the triangular rise time a bit, so that the true triangular rise time is typically 1.2 times the selected semi-Gaussian rise time. 
	- This also leads to an apparent advantage for the analog system when its energy resolution is compared to a digital system with the same nominal rise time.

One important characteristic of a digitally shaped trapezoidal pulse is its extremely sharp termination on completion of the basewidth 2L+G. This may be compared to analog filtered pulses whose tails may persist up to 40% of the rise time, a phenomenon due to the finite bandwidth of the analog filter. As can be seen below, this sharp termination gives the digital filter a definite rate advantage in pileup free throughput.

----

## Baselines and Preamp. Decay Times

$$\sigma_t=\sqrt{\sigma_{f}^{2}+\sigma_{e}^{2}}$$


![A gamma-ray event displayed over a longer time period to show baseline noise and the effect of preamplifier decay time](/img/agammaeventdisplayedoveralongertimeperiodtoshowbaselinenoiseandtheeffectofpreamplifierdecaytime.png)

Using the decay constant $$\tau$$, the baselines can be mapped back to the DC level. This allows precise determination of gamma-ray energies, even if the pulse sits on the falling slope of a previous pulse. The value of $$\tau$$, being a characteristic of the preamplifier, has to be determined by the user and host software and downloaded to the module.


<!-- BASESETUP_Energy.md ends here -->
