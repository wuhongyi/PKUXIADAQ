<!-- BASESETUP_Energy.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 10月  7 09:34:45 2018 (+0800)
;; Last-Updated: 五 10月 26 09:26:08 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 12
;; URL: http://wuhongyi.cn -->

# Energy

<!-- toc -->

## GUI

![Energy](/img/Energy.png)

When the status below the interface is displaying **green Ready**, it means that the interface can be operated, otherwise you need to wait. The operation of the bottom button is the same as above.

- The parameter Rise Time, please refer to *Trapezoidal Filtering*
- The parameter Flat Top, please refer to *Trapezoidal Filtering*
- The parameter Tau, please refer to *Baselines and Preamp. Decay Times*
- The parameter filter range, please refer to *Filter Range*

界面下方的 Status 显示为**绿色的 Ready** 时表示可操作该界面，否则需要等待。底下按钮的操作同上。

- 参数 Rise Time，请参考*Trapezoidal Filtering*部分
- 参数 Flat Top，请参考*Trapezoidal Filtering*部分
- 参数 Tau，请参考*Baselines and Preamp. Decay Times*部分
- 参数 filter range，请参考*Filter Range*部分

**TODO  这里应该补充不同探测器的推荐值**


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

From this point onward, only trapezoidal filtering will be considered as it is implemented in a Pixie-16 module according to Equation $$LV_{x,k}=-\sum_{i=k-2L-G+1}^{k-L-G}V_{i}+\sum_{i=k-L+1}^{k}V_{i}$$. The result of applying such a filter with Length L=1 us and Gap G=0.4 us to a gamma-ray event is shown in Figure. The filter output is clearly trapezoidal in shape and has a rise time equal to L, a flattop equal to G, and a symmetrical fall time equal to L. The basewidth, which is a first-order measure of the filter’s noise reduction properties, is thus 2L+G.

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

Figure shows an event over a longer time interval and how the filter treats the preamplifier noise in regions when no gamma-ray pulses are present. 

![A gamma-ray event displayed over a longer time period to show baseline noise and the effect of preamplifier decay time](/img/agammaeventdisplayedoveralongertimeperiodtoshowbaselinenoiseandtheeffectofpreamplifierdecaytime.png)

As may be seen the effect of the filter is both to reduce the amplitude of the fluctuations and reduce their high frequency content. This region is called the baseline because it establishes the reference level from which the gamma-ray peak amplitude Vx is to be measured. The fluctuations in the baseline have a standard deviation $\sigma_e$ which is referred to as the electronic noise of the system, a number which depends on the rise time of the filter used. Riding on top of this noise, the gamma-ray peaks contribute an additional noise term, the Fano noise, which arises from statistical fluctuations in the amount of charge Qx produced when the gamma-ray is absorbed in the detector. This Fano noise $$\sigma_f$$ adds in quadrature with the electronic noise, so that the total noise $$\sigma_t$$ in measuring Vx is found from:

$$\sigma_t=\sqrt{\sigma_{f}^{2}+\sigma_{e}^{2}}$$

The Fano noise is only a property of the detector material. The electronic noise, on the other hand, may have contributions from both the preamplifier and the amplifier. When the preamplifier and amplifier are both well designed and well matched, however, the amplifier’s noise contribution should be essentially negligible. Achieving this in the mixed analog-digital environment of a digital pulse processor is a non-trivial task, however.

With a RC-type preamplifier, the slope of the preamplifier is rarely zero. Every step decays exponentially back to the DC level of the preamplifier. During such a decay, the baselines are obviously not zero. This can be seen in Figure, where the filter output during the exponential decay after the pulse is below the initial level. Note also that the flat top region is sloped downwards.

Using the decay constant $$\tau$$, the baselines can be mapped back to the DC level. This allows precise determination of gamma-ray energies, even if the pulse sits on the falling slope of a previous pulse. The value of $$\tau$$, being a characteristic of the preamplifier, has to be determined by the user and host software and downloaded to the module.

----

## Pileup Inspection

As noted above, the goal is to capture a value of Vx for each amma-ray detected and use these values to construct a spectrum. 

> **[info] info**
>
> This process is also significantly different between digital and analog systems. 
> In the analog system the peak value must be “captured” into an analog storage device, usually a capacitor, and “held” until it is digitized. 
> Then the digital value is used to update a memory location to build the desired spectrum. 
> During this analog to digital conversion process the system is dead to other events, which can severely reduce system throughput. 
> Even single channel analyzer systems introduce significant deadtime at this stage since they must wait some period (typically a few microseconds) to determine whether or not the window condition is satisfied.
>
>
> Digital systems are much more efficient in this regard, since the values output by the filter are already digital values. 
> All that is required is to take the filter sums, reconstruct the energy Vx , and add it to the spectrum. 
> In the Pixie-16, the filter sums are continuously updated in the FPGA, and are captured into event buffers. 
> Reconstructing the energy and incrementing the spectrum is done by the DSP, so that the FPGA is ready to take new data immediately (unless the buffers are full). 
> This is a significant source of the enhanced throughput found in digital systems.

The peak detection and sampling in a Pixie-16 module is handled as indicated in Figure *Peak detection and sampling*. Two trapezoidal filters are implemented, a fast filter and a slow filter. The fast filter is used to detect the arrival of gamma-rays, the slow filter is used for the measurement of Vx , with reduced noise at longer filter rise times. The fast filter has a filter length Lf = 0.1us and a gap Gf = 0.1us. The slow filter has Ls = 1.2us and Gs = 0.35us.

![Peak detection and sampling](/img/peakdetectionandsampling.png)

The arrival of the gamma-ray step(in the preamplifier output) is detected by digitally comparing the fast filter output to **THRESHOLD**, a digital constant set by the user. Crossing the threshold starts a delay line to wait **PEAKSAMP** clock cycles to arrive at the appropriate time to sample the value of the slow filter. Because the digital filtering processes are deterministic, P**EAKSAMP** depends only on the values of the fast and slow filter constants.
The slow filter value captured following **PEAKSAMP** is then the slow digital filter’s estimate of Vx . Using a delay line allows to stage sampling of multiple pulses even within a **PEAKSAMP** interval (though the filter values themselves are then not correct representations of a single pulse’s height).


The value Vx captured will only be a valid measure of the associated gamma-ray’s energy provided that the filtered pulse is sufficiently well separated in time from its preceding and succeeding neighbor pulses so that their peak amplitudes are not distorted by the action of the trapezoidal filter. That is, if the pulse is not piled up. The relevant issues may be understood by reference to Figure, which shows 3 gamma-rays arriving separated by various intervals. The fast filter has a filter length Lf = 0.1us and a gap Gf =0.1us. The slow filter has Ls = 1.2us and Gs = 0.35us.

![A sequence of 3 gamma-ray pulses separated by various intervals to show the origin of pileup and demonstrate how it is detected by the Pixie module](/img/asequenceof3gammaraypulsesseparatedbyvariousintervalstoshowtheoriginofpileupanddemonstratehowitisdetected.png)

Because the trapezoidal filter is a linear filter, its output for a series of pulses is the linear sum of its outputs for the individual members in the series. Pileup occurs when the rising edge of one pulse lies under the peak (specifically the sampling point) of its neighbor. Thus, in Figure , peaks 1 an 2 are sufficiently well separated so that the leading edge of peak 2 falls after the peak of pulse 1. Because the trapezoidal filter function is symmetrical, this also means that pulse 1’s trailing edge also does not fall under the peak of pulse 2. For this to be true, the two pulses must be separated by at least an interval of L+G. Peaks 2 and 3, which are separated by less than 1.0 us, are thus seen to pileup in the present example with a 1.2 us rise time.

This leads to an important point: whether pulses suffer slow pileup depends critically on the rise time of the filter being used. The amount of pileup which occurs at a given average signal rate will increase with longer rise times.

Because the fast filter rise time is only 0.1 us, these gamma-ray pulses do not pileup in the fast filter channel. The Pixie-16 module can therefore test for slow channel pileup by measuring the fast filter for the interval PEAKSEP after a pulse arrival time. If no second pulse occurs in this interval, then there is no trailing edge pileup and the pulse is validated for acquisition. **PEAKSEP** is usually set to a value close to L+G+1. Pulse 1 passes this test, as shown in Figure. Pulse 2, however, fails the **PEAKSEP** test because pulse 3 follows less than 1.0 us. Notice, by the symmetry of the trapezoidal filter, if pulse 2 is rejected because of pulse 3, then pulse 3 is similarly rejected because of pulse 2.


<!-- BASESETUP_Energy.md ends here -->
