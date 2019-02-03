<!-- BASESETUP_TriggerFilter.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 10月  7 09:35:24 2018 (+0800)
;; Last-Updated: 日 2月  3 15:35:37 2019 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 13
;; URL: http://wuhongyi.cn -->

# Trigger Filter

<!-- toc -->

## GUI

![Trigger Filter](/img/TriggerFilter.png)

When the status below the interface is displaying **green Ready**, it means that the interface can be operated, otherwise you need to wait. The operation of the bottom button is the same as above.

- The parameter *Rise Time* mean trigger filter rise time.
- The parameter *Flat Top* mean trigger filter flat top time.
- The parameter *Thresh.* represents the threshold, which is set relative to the fast filter waveform.



General rules of thumb for the following important parameters are:

- A longer trigger filter rise time averages more samples and thus allows setting lower thresholds without triggering on noise.
- Typically the threshold should be set as low as possible, just above the noise level.
- A longer trigger filter flat top time makes it easier to detect slow rising pulses.

----

## Digital Filters

Energy dispersive detectors, which include such solid state detectors as Si(Li), HPGe, HgI2 , CdTe and CZT detectors, are generally operated with charge sensitive preamplifiers as shown in Figure. Here the detector **D** is biased by voltage source **V** and connected to the input of **preamplifier A** which has feedback capacitor **Cf** and feedback resistor **Rf** .

![(a) Charge sensitive preamplifier with RC feedback; (b) Output on absorption of a gamma ray](/img/chargesenitivepreamplifierrcfeedbackoutputonabsorptionofagammaray.png)

Reducing noise in an electrical measurement is accomplished by filtering. Traditional analog filters use combinations of a differentiation stage and multiple integration stages to convert the preamp output steps, such as shown in Figure (b), into either triangular or semi-Gaussian pulses whose amplitudes (with respect to their baselines) are then proportional to Vx and thus to the gamma-ray’s energy.

Digital filtering proceeds from a slightly different perspective. Here the signal has been digitized and is no longer continuous. Instead it is a string of discrete values as shown in Figure. Figure is actually just a subset of Figure (b), in which the signal was digitized by a Tektronix 544 TDS digital oscilloscope at 10 MSPS (mega samples per second). Given this data set, and some kind of arithmetic processor, the obvious approach to determining Vx is to take some sort of average over the points before the step and subtract it from the value of the average over the points after the step. That is, as shown in Figure *Digitized version of the data of Figure (b) in the step region*, averages are computed over the two regions marked “Length” (the “Gap” region is omitted because the signal is changing rapidly here), and their difference taken as a measure of Vx . Thus the value Vx may be found from the following equation:

$$V_{x,k}=-\sum_{i(before)}W_{i}V_{i}+\sum_{i(after)}W_{i}V_{i}$$

Where the values of the weighting constants Wi determine the type of average being computed. The sums of the values of the two sets of weights must be individually normalized.

**The primary differences between different digital signal processors lie in two areas: what set of weights Wi is used and how the regions are selected for the computation of Equation. **

Thus, for example, when larger weighting values are used for the region close to the step while smaller values are used for the data away from the step, Equation  produces “cusp-like” filters. When the weighting values are constant, one obtains triangular (if the gap is zero) or trapezoidal filters. The concept behind cusp-like filters is that, since the points nearest the step carry the most information about its height, they should be most strongly weighted in the averaging process. How one chooses the filter lengths results in time variant (the lengths vary from pulse to pulse) or time invariant (the lengths are the same for all pulses) filters. Traditional analog filters are time invariant. The concept behind time variant filters is that, since the gamma-rays arrive randomly and the lengths between them vary accordingly, one can make maximum use of the available information by setting the length to the interpulse spacing.

In principle, the very best filtering is accomplished by using cusp-like weights and time variant filter length selection. There are serious costs associated with this approach however, both in terms of computational power required to evaluate the sums in real time and in the complexity of the electronics required to generate (usually from stored coefficients) normalized Wi sets on a pulse by pulse basis.

![Digitized version of the data of Figure (b) in the step region](/img/digitizedversionofthedataoffigurebinthestepregion.png)

**The Pixie-16 takes a different approach because it was optimized for high speed operation.**

It implements a fixed length filter with all W i values equal to unity and in fact computes this sum afresh for each new signal value k. Thus the equation implemented is:

$$LV_{x,k}=-\sum_{i=k-2L-G+1}^{k-L-G}V_{i}+\sum_{i=k-L+1}^{k}V_{i}$$

Where the filter length is **L** and the gap is **G**. The factor **L** multiplying $$V_{x,k}$$ arises because the sum of the weights here is not normalized. Accommodating this factor is trivial.


While this relationship is very simple, it is still very effective. In the first place, this is the digital equivalent of triangular (or trapezoidal if G ≠ 0) filtering which is the analog industry’s standard for high rate processing. In the second place, one can show theoretically that if the noise in the signal is white (i.e., Gaussian distributed) above and below the step, which is typically the case for the short shaping times used for high signal rate processing, then the average in Equation actually gives the best estimate of Vx in the least squares sense. This, of course, is why triangular filtering has been preferred at high rates.

Triangular filtering with time variant filter lengths can, in principle, achieve both somewhat superior resolution and higher throughputs but comes at the cost of a significantly more complex circuit and a rate dependent resolution, which is unacceptable for many types of precise analysis. In practice, XIA’s design has been found to duplicate the energy resolution of the best analog shapers while approximately doubling their throughput, providing experimental confirmation of the validity of the approach.




<!-- BASESETUP_TriggerFilter.md ends here -->
