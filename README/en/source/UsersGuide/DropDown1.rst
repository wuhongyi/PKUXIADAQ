.. DropDown1.rst --- 
.. 
.. Description: 
.. Author: Hongyi Wu(吴鸿毅)
.. Email: wuhongyi@qq.com 
.. Created: 三 7月  3 10:47:44 2019 (+0800)
.. Last-Updated: 六 9月 21 18:41:28 2019 (+0800)
..           By: Hongyi Wu(吴鸿毅)
..     Update #: 48
.. URL: http://wuhongyi.cn 

---------------------------------
Base
---------------------------------

The adjustments in this drop-down column are the basics, and anyone using the Pixie16 acquisition system should be familiar with and master the operating techniques.


^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Base Setup
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

"""""""""""""""""""""""""""""""""
Interface
"""""""""""""""""""""""""""""""""

.. image:: /_static/img/BaseSetup.png

The status below the interface shows a **green Ready** to indicate that the interface can be operated, otherwise you need to wait.

In the interface, the parameters behind the **Module** are used to select the adjusted module, **Load** is used to read the parameter value of the modules, and **Apply** is to write the value in the interface to the module.

The parameter following the **Select Channel** at the bottom of the interface indicates that you can choose to copy the parameters of the channel on the interface to other channels, click **Copy** to complete the copy, and then **Apply** to write the parameters to the module.
	   
The **Base Setup** page controls the analog gain, offset and polarity for each channel. It is useful to click on **Trace & Baseline** in the top control **Monitor** bar to view the signal read from the ADCs while adjusting these parameters. The display shows one or all 16 channels of a module; you can set the sampling interval for each block to capture a longer time frame in **Hist & XDT** page. Click **Draw** to update the graph.

Pulses from the detector should fall in the range from 0 to 16383(14 bit), with the baseline at ~1638 to allow for drifts and/or undershoots and no clipping at the upper limit. If there is clipping, adjust the Gain and Offset or click on the *AdjustOffset* button to let the software set the DC offsets to proper levels.

Since the trigger/filter circuits in the FPGA only act on rising pulses, negative pulses are inverted at the input of the FPGA, and the waveforms shown in the ADC trace display include this optional inversion. Thus set the channel’s Polarity such that pulses from the detector appear with positive amplitude (rising edge).


In the **Base Setup**  tab, you can set the total trace length and the pre-trigger trace delay for the waveforms to be acquired in list mode runs.

The trace delay cannot be longer than the trace length, and for each Pixie-16 variant, there is also a limit for the maximum value of trace delay and trace length.
	   

"""""""""""""""""""""""""""""""""
Parameters introduction
"""""""""""""""""""""""""""""""""

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
  
It should be specially noted that when using the down frequency mode, the actual waveform length is TraceDelay x 2^N / TraceLength x 2^N (N is the down-frequency parameter).

The blue *Baseline* is used to adjust the baseline position and adjust the baseline to the user's expected position by voltage compensation:

- The *Baseline* adjustable range is 0 - 100, which indicates the percentage of the waveform's baseline that falls within the full scale. For example, for a vertical precision 14-bit capture card, setting this parameter to 10 means that the baseline reduction compensation is adjusted to around 10% of the 16384 full-scale, near 1638.
- The purple *DCOffset* and *BLcut* users do not need to be modified, and the parameters can be adjusted automatically. After modifying *Baseline*, *Gain*, and *Sign* in this submenu, you need to press the bottom of the *AdjustOffset*, and then press *BLcutFinder* to automatically adjust these two parameters.


"""""""""""""""""""""""""""""""""
Important note
"""""""""""""""""""""""""""""""""

.. NOTE:: **trace length in 500 MHz**

   For the 500 MHz Pixie-16 modules, the ADCs are running at 500 MHz, but the traces are recorded with 100 MHz clocks in the FPGA with 5 ADC samples captured in each 10 ns interval. In addition, the data packing from the FPGA to the onboard External FIFO is two sets of 5 ADC samples in one transfer. So the trace length should be multiples of 20 ns, i.e., 20 ns, 40 ns, ... for instance, a trace length of 500 ns and a trace delay of 200 ns.



.. TIP::  **Good channel**	  

  Only channels marked as good will have their events recorded. 
  
  This setting has no bearing on the channel's capability to issue a trigger. 
  
  There can be a triggering channel whose data are discarded. 
  
  Channels not marked as good will be excluded from the automatic offset adjustment.

   
"""""""""""""""""""""""""""""""""
Baseline measurements
"""""""""""""""""""""""""""""""""

The Pixie-16 constantly takes baseline measurements when no pulse is detected and keeps a baseline average to be subtracted from the energy filter output during pulse height reconstruction. Baseline measurements that differ from the average by more than the BaselineCut value will be rejected as they are likely contaminated with small pulses below the trigger threshold.

A series of baseline measurements for each channel can be viewed in **Trace & Baseline** page, and in the BASELINE panel a histogram of baselines can be built to verify that the Baseline Cut does not reject measurements falling into the main (ideally Gaussian) peak in the baseline distribution.

Usually, it is sufficient to keep Baseline Cut at its default value.


Note: Since the baseline computation takes into account the exponential decay, no pulses should be noticeable in the baseline display if

- a) the decay time is set correctly and 
- b) the detector pulses are truly exponential.

Baseline Percent is a parameter used for automatic offset adjustment; by clicking on the *AdjustOffses* button, offsets will be set such that the baseline seen in the ADC trace display falls at the Baseline Percent fraction of the full ADC range (e.g. for a 12-bit ADC and Baseline Percent = 10% the baseline falls at ADC step 409 out of 4096 total).

----


^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Trigger Filter
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

"""""""""""""""""""""""""""""""""
Interface
"""""""""""""""""""""""""""""""""
.. image:: /_static/img/TriggerFilter.png

When the status below the interface is displaying **green Ready**, it means that the interface can be operated, otherwise you need to wait. The operation of the bottom button is the same as above.

- The parameter *Rise Time* mean trigger filter rise time.
- The parameter *Flat Top* mean trigger filter flat top time.
- The parameter *Thresh.* represents the threshold, which is set relative to the fast filter waveform.


General rules of thumb for the following important parameters are:

- A longer trigger filter rise time averages more samples and thus allows setting lower thresholds without triggering on noise.
- Typically the threshold should be set as low as possible, just above the noise level.
- A longer trigger filter flat top time makes it easier to detect slow rising pulses.
	   


"""""""""""""""""""""""""""""""""
Digital Filters
"""""""""""""""""""""""""""""""""

Energy dispersive detectors, which include such solid state detectors as Si(Li), HPGe, HgI2 , CdTe and CZT detectors, are generally operated with charge sensitive preamplifiers as shown in Figure. Here the detector **D** is biased by voltage source **V** and connected to the input of **preamplifier A** which has feedback capacitor **Cf** and feedback resistor **Rf** .

.. image:: /_static/img/chargesenitivepreamplifierrcfeedbackoutputonabsorptionofagammaray.png

Reducing noise in an electrical measurement is accomplished by filtering. Traditional analog filters use combinations of a differentiation stage and multiple integration stages to convert the preamp output steps, such as shown in Figure (b), into either triangular or semi-Gaussian pulses whose amplitudes (with respect to their baselines) are then proportional to **Vx** and thus to the gamma-ray’s energy.
	   
Digital filtering proceeds from a slightly different perspective. Here the signal has been digitized and is no longer continuous. Instead it is a string of discrete values as shown in Figure. Figure is actually just a subset of Figure (b), in which the signal was digitized by a Tektronix 544 TDS digital oscilloscope at 10 MSPS (mega samples per second). Given this data set, and some kind of arithmetic processor, the obvious approach to determining Vx is to take some sort of average over the points before the step and subtract it from the value of the average over the points after the step. That is, as shown in follow Figure, averages are computed over the two regions marked “Length” (the “Gap” region is omitted because the signal is changing rapidly here), and their difference taken as a measure of **Vx** . Thus the value Vx may be found from the following equation:	   

.. math::
  V_{x,k}=-\sum_{i(before)}W_{i}V_{i}+\sum_{i(after)}W_{i}V_{i}

Where the values of the weighting constants **Wi** determine the type of average being computed. The sums of the values of the two sets of weights must be individually normalized.
  
**The primary differences between different digital signal processors lie in two areas: what set of weights Wi is used and how the regions are selected for the computation of Equation.**

Thus, for example, when larger weighting values are used for the region close to the step while smaller values are used for the data away from the step, Equation  produces “cusp-like” filters. When the weighting values are constant, one obtains triangular (if the gap is zero) or trapezoidal filters. The concept behind cusp-like filters is that, since the points nearest the step carry the most information about its height, they should be most strongly weighted in the averaging process. How one chooses the filter lengths results in time variant (the lengths vary from pulse to pulse) or time invariant (the lengths are the same for all pulses) filters. Traditional analog filters are time invariant. The concept behind time variant filters is that, since the gamma-rays arrive randomly and the lengths between them vary accordingly, one can make maximum use of the available information by setting the length to the interpulse spacing.

In principle, the very best filtering is accomplished by using cusp-like weights and time variant filter length selection. There are serious costs associated with this approach however, both in terms of computational power required to evaluate the sums in real time and in the complexity of the electronics required to generate (usually from stored coefficients) normalized **Wi** sets on a pulse by pulse basis.

.. image:: /_static/img/digitizedversionofthedataoffigurebinthestepregion.png

**The Pixie-16 takes a different approach because it was optimized for high speed operation.**

It implements a fixed length filter with all **Wi** values equal to unity and in fact computes this sum afresh for each new signal value k. Thus the equation implemented is:

.. math::
   LV_{x,k}=-\sum_{i=k-2L-G+1}^{k-L-G}V_{i}+\sum_{i=k-L+1}^{k}V_{i}

Where the filter length is **L** and the gap is **G**. The factor **L** multiplying :math:`V_{x,k}` arises because the sum of the weights here is not normalized. Accommodating this factor is trivial.
   
While this relationship is very simple, it is still very effective. In the first place, this is the digital equivalent of triangular (or trapezoidal if G ≠ 0) filtering which is the analog industry’s standard for high rate processing. In the second place, one can show theoretically that if the noise in the signal is white (i.e., Gaussian distributed) above and below the step, which is typically the case for the short shaping times used for high signal rate processing, then the average in Equation actually gives the best estimate of **Vx** in the least squares sense. This, of course, is why triangular filtering has been preferred at high rates.

Triangular filtering with time variant filter lengths can, in principle, achieve both somewhat superior resolution and higher throughputs but comes at the cost of a significantly more complex circuit and a rate dependent resolution, which is unacceptable for many types of precise analysis. In practice, XIA’s design has been found to duplicate the energy resolution of the best analog shapers while approximately doubling their throughput, providing experimental confirmation of the validity of the approach.


----

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Energy
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

"""""""""""""""""""""""""""""""""
Interface
"""""""""""""""""""""""""""""""""

.. image:: /_static/img/Energy.png

When the status below the interface is displaying **green Ready**, it means that the interface can be operated, otherwise you need to wait. The operation of the bottom button is the same as above.

- The parameter Rise Time, please refer to *Trapezoidal Filtering*
- The parameter Flat Top, please refer to *Trapezoidal Filtering*
- The parameter Tau, please refer to *Baselines and Preamp. Decay Times*
- The parameter filter range, please refer to *Filter Range*
	   
The most critical parameter for the energy computation is the signal decay time Tau. It is used to compensate for the falling edge of a previous pulse in the computation of the energy. You can either enter Tau directly for each channel, or enter an approximate value in the right control, select a channel, and click Find it to let the software determine the decay time automatically.

Click Accept it to apply the found value to the channel. (If the approximate value is unchanged,the software could not find a better value.)  

At high count rates, pulses overlap with each other at higher frequency. In order to compute the energy or pulse height of those pulses accurately without the need to wait until they decay back to baseline level completely, the pulse height computation algorithm implemented in the Pixie-16 uses the decay time to compute and remove the contribution from the exponentially decaying tail of the overlapping prior pulse when computing the pulse height of the current pulse.



.. DANGER:: **single exponential decay constant**

  It is assumed the pulses have only a single exponential decay constant. If pulses have multiple decay constants, it might be possible to use the decay constant that dominates the decay of the pulse, but the accuracy of pulse height computation will be degraded.

General rules of thumb for the following important parameters are:

- The energy filter flat top time should be larger than the longest pulse rise time.
- The energy filter rise time can be varied to balance the resolution and throughput.
- In general, energy resolution improves with the increase of energy filter rise time, up to an optimum when longer filters only add more noise into the measurement.
- The energy filter dead time TD is about :math:`2×(T_{rise}+T_{flat})`, and the maximum throughput for Poisson statistics is 1/(TD x e). For HPGe detectors, a rise time of 4-6us and a flat top of 1us are usually appropriate.
- Choose the smallest energy filter range that allows setting the optimum energy filter rise time. Larger filter ranges allow longer filter sums, but increase the granularity of possible values for the energy filter rise time and flat top time and increase the jitter of latching the energy filter output relative to the rising edge of the pulse. This is usually only important for very fast pulses.


  

"""""""""""""""""""""""""""""""""
Filter Range
"""""""""""""""""""""""""""""""""

To accommodate a wide range of energy filter rise times from tens of nanoseconds to tens of microseconds, the filters are implemented in the FPGA with different clock decimations(filter ranges). The ADC sampling rate is either 2ns, 4ns, or 10ns depending on the ADC variant that is used, but in higher clock decimations, several ADC samples are averaged before entering the energy filtering logic. In filter range 1, :math:`2^{1}` samples are averaged, :math:`2^{2}` samples in filter range 2, and so on. Since the sum of rise time and flat top is limited to 127 decimated clock cycles, filter time granularity and filter time are limited to the values listed in Table .

.. image:: /_static/img/filterclockdecimationsandfiltertimegranularityfor100mhzor500mhz.png
.. image:: /_static/img/filterclockdecimationsandfiltertimegranularityfor250mhz.png
	   

"""""""""""""""""""""""""""""""""
Trapezoidal Filtering
"""""""""""""""""""""""""""""""""

From this point onward, only trapezoidal filtering will be considered as it is implemented in a Pixie-16 module according to Equation :math:`LV_{x,k}=-\sum_{i=k-2L-G+1}^{k-L-G}V_{i}+\sum_{i=k-L+1}^{k}V_{i}`. The result of applying such a filter with Length L=1 us and Gap G=0.4 us to a gamma-ray event is shown in Figure. The filter output is clearly trapezoidal in shape and has a rise time equal to L, a flattop equal to G, and a symmetrical fall time equal to L. The basewidth, which is a first-order measure of the filter’s noise reduction properties, is thus 2L+G.

.. image:: /_static/img/trapezoidalfilteringofapreamplifierstepwithl1usandg04us.png

This raises several important points in comparing the noise performance of the Pixie-16 module to analog filtering amplifiers.

- First, semi-Gaussian filters are usually specified by a shaping time. 
    - Their rise time is typically twice this and their pulses are not symmetric so that the basewidth is about 5.6 times the shaping time or 2.8 times their rise time. 
- Thus a semi-Gaussian filter typically has a slightly better energy resolution than a triangular filter of the same rise time because it has a longer filtering time. 
    - This is typically accommodated in amplifiers offering both triangular and semi-Gaussian filtering by stretching the triangular rise time a bit, so that the true triangular rise time is typically 1.2 times the selected semi-Gaussian rise time. 
    - This also leads to an apparent advantage for the analog system when its energy resolution is compared to a digital system with the same nominal rise time.

One important characteristic of a digitally shaped trapezoidal pulse is its extremely sharp termination on completion of the basewidth 2L+G. This may be compared to analog filtered pulses whose tails may persist up to 40% of the rise time, a phenomenon due to the finite bandwidth of the analog filter. As can be seen below, this sharp termination gives the digital filter a definite rate advantage in pileup free throughput.

	  
"""""""""""""""""""""""""""""""""""""""
Baselines and Preamp. Decay Times
"""""""""""""""""""""""""""""""""""""""

Figure shows an event over a longer time interval and how the filter treats the preamplifier noise in regions when no gamma-ray pulses are present. 

.. image:: /_static/img/agammaeventdisplayedoveralongertimeperiodtoshowbaselinenoiseandtheeffectofpreamplifierdecaytime.png

As may be seen the effect of the filter is both to reduce the amplitude of the fluctuations and reduce their high frequency content. This region is called the baseline because it establishes the reference level from which the gamma-ray peak amplitude **Vx** is to be measured. The fluctuations in the baseline have a standard deviation :math:`\sigma_e` which is referred to as the electronic noise of the system, a number which depends on the rise time of the filter used. Riding on top of this noise, the gamma-ray peaks contribute an additional noise term, the Fano noise, which arises from statistical fluctuations in the amount of charge **Qx** produced when the gamma-ray is absorbed in the detector. This Fano noise :math:`\sigma_f` adds in quadrature with the electronic noise, so that the total noise :math:`\sigma_t` in measuring **Vx** is found from:
	   
.. math::
   \sigma_t=\sqrt{\sigma_{f}^{2}+\sigma_{e}^{2}}

The Fano noise is only a property of the detector material. The electronic noise, on the other hand, may have contributions from both the preamplifier and the amplifier. When the preamplifier and amplifier are both well designed and well matched, however, the amplifier’s noise contribution should be essentially negligible. Achieving this in the mixed analog-digital environment of a digital pulse processor is a non-trivial task, however.

With a RC-type preamplifier, the slope of the preamplifier is rarely zero. Every step decays exponentially back to the DC level of the preamplifier. During such a decay, the baselines are obviously not zero. This can be seen in Figure, where the filter output during the exponential decay after the pulse is below the initial level. Note also that the flat top region is sloped downwards.

Using the decay constant :math:`\tau`, the baselines can be mapped back to the DC level. This allows precise determination of gamma-ray energies, even if the pulse sits on the falling slope of a previous pulse. The value of  :math:`\tau`, being a characteristic of the preamplifier, has to be determined by the user and host software and downloaded to the module.


"""""""""""""""""""""""""""""""""
Pileup Inspection
"""""""""""""""""""""""""""""""""

As noted above, the goal is to capture a value of **Vx** for each gamma-ray detected and use these values to construct a spectrum. 


.. NOTE::
  This process is also significantly different between digital and analog systems. 
  In the analog system the peak value must be “captured” into an analog storage device, usually a capacitor, and “held” until it is digitized. 
  Then the digital value is used to update a memory location to build the desired spectrum. 
  During this analog to digital conversion process the system is dead to other events, which can severely reduce system throughput. 
  Even single channel analyzer systems introduce significant deadtime at this stage since they must wait some period (typically a few microseconds) to determine whether or not the window condition is satisfied.
 
  Digital systems are much more efficient in this regard, since the values output by the filter are already digital values. 
  All that is required is to take the filter sums, reconstruct the energy Vx , and add it to the spectrum. 
  In the Pixie-16, the filter sums are continuously updated in the FPGA, and are captured into event buffers. 
  Reconstructing the energy and incrementing the spectrum is done by the DSP, so that the FPGA is ready to take new data immediately (unless the buffers are full). 
  This is a significant source of the enhanced throughput found in digital systems.

The peak detection and sampling in a Pixie-16 module is handled as indicated in follow Figure. Two trapezoidal filters are implemented, a fast filter and a slow filter. The fast filter is used to detect the arrival of gamma-rays, the slow filter is used for the measurement of **Vx** , with reduced noise at longer filter rise times. The fast filter has a filter length Lf = 0.1us and a gap Gf = 0.1us. The slow filter has Ls = 1.2us and Gs = 0.35us.
   
.. image:: /_static/img/peakdetectionandsampling.png

The arrival of the gamma-ray step(in the preamplifier output) is detected by digitally comparing the fast filter output to **THRESHOLD**, a digital constant set by the user. Crossing the threshold starts a delay line to wait **PEAKSAMP** clock cycles to arrive at the appropriate time to sample the value of the slow filter. Because the digital filtering processes are deterministic, **PEAKSAMP** depends only on the values of the fast and slow filter constants.

The slow filter value captured following **PEAKSAMP** is then the slow digital filter’s estimate of **Vx** . Using a delay line allows to stage sampling of multiple pulses even within a **PEAKSAMP** interval (though the filter values themselves are then not correct representations of a single pulse’s height).

The value **Vx** captured will only be a valid measure of the associated gamma-ray’s energy provided that the filtered pulse is sufficiently well separated in time from its preceding and succeeding neighbor pulses so that their peak amplitudes are not distorted by the action of the trapezoidal filter. That is, if the pulse is not piled up. The relevant issues may be understood by reference to Figure, which shows 3 gamma-rays arriving separated by various intervals. The fast filter has a filter length Lf = 0.1us and a gap Gf =0.1us. The slow filter has Ls = 1.2us and Gs = 0.35us.	  

.. image:: /_static/img/asequenceof3gammaraypulsesseparatedbyvariousintervalstoshowtheoriginofpileupanddemonstratehowitisdetected.png

Because the trapezoidal filter is a linear filter, its output for a series of pulses is the linear sum of its outputs for the individual members in the series. Pileup occurs when the rising edge of one pulse lies under the peak (specifically the sampling point) of its neighbor. Thus, in Figure , peaks 1 an 2 are sufficiently well separated so that the leading edge of peak 2 falls after the peak of pulse 1. Because the trapezoidal filter function is symmetrical, this also means that pulse 1’s trailing edge also does not fall under the peak of pulse 2. For this to be true, the two pulses must be separated by at least an interval of L+G. Peaks 2 and 3, which are separated by less than 1.0 us, are thus seen to pileup in the present example with a 1.2 us rise time.

This leads to an important point: whether pulses suffer slow pileup depends critically on the rise time of the filter being used. The amount of pileup which occurs at a given average signal rate will increase with longer rise times.

Because the fast filter rise time is only 0.1 us, these gamma-ray pulses do not pileup in the fast filter channel. The Pixie-16 module can therefore test for slow channel pileup by measuring the fast filter for the interval PEAKSEP after a pulse arrival time. If no second pulse occurs in this interval, then there is no trailing edge pileup and the pulse is validated for acquisition. **PEAKSEP** is usually set to a value close to L+G+1. Pulse 1 passes this test, as shown in Figure. Pulse 2, however, fails the **PEAKSEP** test because pulse 3 follows less than 1.0 us. Notice, by the symmetry of the trapezoidal filter, if pulse 2 is rejected because of pulse 3, then pulse 3 is similarly rejected because of pulse 2.

----

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
CFD
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

"""""""""""""""""""""""""""""""""
Interface
"""""""""""""""""""""""""""""""""

.. image:: /_static/img/CFDPars.png

**TODO**

"""""""""""""""""""""""""""""""""
100 MHz and 250 MHz modules
"""""""""""""""""""""""""""""""""

The following CFD algorithm is implemented in the signal processing FPGA of the 100 MHz(Rev. B, C, D and F) and 250 MHz(Rev. F) Pixie-16 modules. 

Assume the digitized waveform can be represented by data series Trace[i], i = 0, 1, 2, ... First the fast filter response(FF) of the digitized waveform is computed as follows:

.. math::
   FF[i]=\sum_{j=i-(FL-1)}^{i}Trace[j]-\sum_{j=i-(2\times FL+FG-1)}^{i-(FL+FG)}Trace[j]

Where FL is called the fast length and FG is called the fast gap of the digital trapezoidal filter. Then the CFD is computed as follows:

.. math::
   CFD[i+D]=FF[i+D]\times(1-w/8)-FF[i]

Where D is called the CFD delay length and w is called the CFD scaling factor(w=0, 1,..., 7).

The CFD zero crossing point(ZCP) is then determined when :math:`CFD[i]\geq 0` and :math:`CFD[i+1]<0`. The timestamp is latched at Trace point :math:`i`, and the fraction time :math:`f` is given by the ratio of the two CFD response amplitudes right before and after the ZCP.

.. math::
   f=\frac{CFDout1}{CFDout1-CFDout2}

Where CFDout1 is the CFD response amplitude right before the ZCP, and CFDout2 is the CFD response amplitude right after the ZCP(subtraction is used in the denominator since CFDout2 is negative). The Pixie-16 DSP computes the CFD final value as follows and stores it in the output data stream for online or offline analysis.

.. math::
   CFD=\frac{CFDout1}{CFDout1-CFDout2} \times N

Where N is scaling factor, which equals to 32768 for 100 MHz modules and 16384 for 250 MHz modules, respectively.

.. image:: /_static/img/adcsamples_cfd.png
.. image:: /_static/img/fastfilter_cfd.png	   
.. image:: /_static/img/cfdfilter_cfd.png

Figure shows a sample ADC trace, its fast filter response and its CFD response, respectively. 

The top figure shows a raw ADC trace. After computing the fast filter response on the raw ADC trace using Equation :math:`FF[i]`, the fast filter response is compared against the fast filter threshold as shown in the middle figure. The ADC sample where the fast filter response crosses the fast filter threshold is called the fast trigger point, which also starts the search for the CFD zero crossing point. 

The CFD response is computed using Equation :math:`CFD[i+D]` and is shown in the bottom figure(for actual implementation in the firmware, the fast filter response FF is delayed slightly before being used for computing the CFD response so that there are sufficient number of CFD response points to look for the zero crossing point after the fast trigger). To prevent premature CFD trigger as a result of the noise in the CFD response before the actual trigger, a DSP parameter called CFDThresh is used to suppress those noise-caused zero crossing. However, if a zero crossing point cannot be found within a certain period after the fast trigger (typically 32 clock cycles), e.g., due to unnecessarily high CFDThresh, a forced CFD Trigger will be issued and a flag will be set in an event header word to indicate that the recorded CFD time for this event is invalid.

However, the event will still have a valid timestamp which is latched by the fast filter trigger when fast filter crosses over the trigger threshold. The aforementioned CFD parameters correspond to the following DSP parameters.

.. image:: /_static/img/correspondingdspparametersforthecfdparmeters.png

.. NOTE:: **250 MHz**

  In the 250 MHz Pixie-16 modules, the event timestamp is counted with 125 MHz clock ticks, i.e., 8 ns intervals, and two consecutive 250 MHz ADC samples are captured in one 8 ns interval as well. 
 
  The CFD trigger also runs at 125 MHz, but the CFD zero crossing point is still reported as a fractional time between two neighboring 250 MHz ADC samples, which are processed by the FPGA in one 125 MHz clock cycle. 
 
  However, the CFD zero crossing point could be in either the odd or even clock cycle of the captured 250 MHz ADC waveforms. 
 
  Therefore, the firmware outputs a "CFD trigger source" bit in the output data stream to indicate whether the CFD zero crossing point is in the odd or even clock cycle of the captured 250 MHz ADC waveforms.

.. NOTE:: **100 MHz**

  In the 100 MHz Pixie-16 modules, event timestamp, CFD trigger, and ADC waveform capture are all carried out with the same 100 MHz clock. So there is no need to report "CFD trigger source" for the 100 MHz Pixie-16 modules.

	  
	  
"""""""""""""""""""""""""""""""""
500 MHz modules
"""""""""""""""""""""""""""""""""

The CFD algorithm discussed in the previous section for the 100 MHz and 250 MHz Pixie-16 modules can also be written in the following format:

.. math::
   CFD(k)=w\cdot\left(\sum_{i=k}^{k+L}a(i)-\sum_{i=k-B}^{k-B+L}a(i)\right)-\left(\sum_{i=k-D}^{k-D+L}a(i)-\sum_{i=k-D-B}^{k-D-B+L}a(i)\right)

Where a(i) is the ADC trace data, k is the index, and w, B, D, and L are CFD parameters.

The CFD algorithm implemented in the 500 MHz Pixie-16 modules is special when compared to the one implemented in the 100 MHz and 250 MHz Pixie-16 modules in terms of the ability to adjust parameters w, B, D, and L. 

The reason for this is that in the 500 MHz Pixie-16 modules, ADC data that come into the FPGA at the speed of 500 MHz is first slowed down with a ratio of 1:5, in other words, the FPGA captures 5 ADC samples at the rate of 100 MHz, i.e., every 10 ns. The FPGA then tries to find the CFD trigger point between any two adjacent 2-ns ADC samples within that 10 ns by first building sums of ADC samples and then calculating differences between delayed and non-delayed sums until the zero crossing point is found. However, in the 500 MHz Pixie-16 modules, the FPGA does not have enough resources to build sums for 5 ADC samples in parallel with variable delays. Therefore, the CFD algorithm for the 500 MHz modules was implemented using a set of fixed CFD parameters as shown in Table *Fixed CFD Parameter Values for 500 MHz Pixie-16 Modules*. Tests show these fixed parameters give best performance for LaBr3(Ce) detectors.

.. image:: /_static/img/fixedcfdparametervaluesfor500mhzpixie16modules.png

The CFD time given by the 500 MHz Pixie-16 modules consists of two parts: a shift within the 5 ADC samples and a fractional time between two ADC samples where the CFD zero crossing occurred. The shift within the 5 ADC samples is reported as the 3-bit CFD trigger source[2:0] is defined as follows.
	   
.. image:: /_static/img/meaningofthecfdtriggersourcefor500mhzpixie16modules.png
	   
CFD 分数时间如下：

.. math::
   CFD=\frac{CFDout1}{CFDout1-CFDout2}\times8192


----

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
QDC
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

"""""""""""""""""""""""""""""""""
Interface
"""""""""""""""""""""""""""""""""

.. image:: /_static/img/QDCPars.png

Eight QDC sums, each of which can have different lengths, are computed in the Signal Processing FPGA of a Pixie-16 module for each channel and the sums are written to the list mode output data stream if the user requests so. 

The recording of QDC sums starts at the waveform point which is *Pre-trigger Trace Length* or *Trace Delay* earlier than the trigger point, which is either the CFD trigger or channel fast trigger depending on whether or not CFD trigger mode is enabled. 

The eight QDC sums are computed one by one continuously, but they are not overlapping. The recording of QDC sums ends when the eight intervals have all passed.

.. image:: /_static/img/the8qdcsumsofatriggeredevent.png	   

----

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Decimation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

"""""""""""""""""""""""""""""""""
Interface
"""""""""""""""""""""""""""""""""

.. image:: /_static/img/Decimation.png

Waveform down frequency outputs are customized for the 100 MHz module in the PKU firmware.

The record waveform mode with down frequency output. The strategy adopted is to select the output of 1, 1/2, 1/4, 1/8, 1/16, 1/32, 1/64, 1/128 frequency, ie how many points retain one point. The points retained are the averaged values.

----

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Copy Pars
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

"""""""""""""""""""""""""""""""""
Interface
"""""""""""""""""""""""""""""""""

.. image:: /_static/img/CopyPars.png


This window is used to quickly copy parameters between channels.


----

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Save2File
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

"""""""""""""""""""""""""""""""""
Interface
"""""""""""""""""""""""""""""""""

.. image:: /_static/img/Save2File.png

As you know, the DSP parameters are stored in a file with the suffix "set". Once you have configured the parameters, you should save them for later use.



.. 
.. DropDown1.rst ends here
