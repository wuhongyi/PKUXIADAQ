<!-- BASESETUP_CFD.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 10月  7 09:27:38 2018 (+0800)
;; Last-Updated: 日 10月  7 09:28:11 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 2
;; URL: http://wuhongyi.cn -->

# CFD

![CFD](/img/CFDPars.png)

**TODO**


## 100 MHz and 250 MHz modules

The following CFD algorithm is implemented in the signal processing FPGA of the 100 MHz(Rev. B, C, D and F) and 250 MHz(Rev. F) Pixie-16 modules. 

Assume the digitized waveform can be represented by data series Trace[i], i = 0, 1, 2, ... First the fast filter response(FF) of the digitized waveform is computed as follows:

$$FF[i]=\sum_{j=i-(FL-1)}^{i}Trace[j]-\sum_{j=i-(2\timesFL+FG-1)}^{i-(FL+FG)}Trace[j]$$

Where FL is called the fast length and FG is called the fast gap of the digital trapezoidal filter. Then the CFD is computed as follows:

$$CFD[i+D]=FF[i+D]\times(1-w/8)-FF[i]$$

Where D is called the CFD delay length and w is called the CFD scaling factor(w=0, 1,..., 7).

The CFD zero crossing point(ZCP) is then determined when $$CFD[i]\leq0$$ and $$CFD[i+1]<0$$. The timestamp is latched at Trace point $$i$$, and the fraction time $$f$$ is given by the ratio of the two CFD response amplitudes right before and after the ZCP.

$$f=\frac{CFDout1}{CFDout1-CFDout2}$$

Where CFDout1 is the CFD response amplitude right before the ZCP, and CFDout2 is the CFD response amplitude right after the ZCP(subtraction is used in the denominator since CFDout2 is negative). The Pixie-16 DSP computes the CFD final value as follows and stores it in the output data stream for online or offline analysis.

$$CFD=\frac{CFDout1}{CFDout1-CFDout2} \times N$$

Where N is scaling factor, which equals to 32768 for 100 MHz modules and 16384 for 250 MHz modules, respectively.

![WAVEFORM](/img/adcsamples_cfd.png)  
![FASTFILTER](/img/fastfilter_cfd.png)  
![CFDFILTER](/img/cfdfilter_cfd.png)

Figure shows a sample ADC trace, its fast filter response and its CFD response, respectively. 

The top figure shows a raw ADC trace. After computing the fast filter response on the raw ADC trace using Equation $$FF[i]$$, the fast filter response is compared against the fast filter threshold as shown in the middle figure. The ADC sample where the fast filter response crosses the fast filter threshold is called the fast trigger point, which also starts the search for the CFD zero crossing point. 

The CFD response is computed using Equation $$CFD[i+D]$$ and is shown in the bottom figure(for actual implementation in the firmware, the fast filter response FF is delayed slightly before being used for computing the CFD response so that there are sufficient number of CFD response points to look for the zero crossing point after the fast trigger). To prevent premature CFD trigger as a result of the noise in the CFD response before the actual trigger, a DSP parameter called CFDThresh is used to suppress those noise-caused zero crossing. However, if a zero crossing point cannot be found within a certain period after the fast trigger (typically 32 clock cycles), e.g., due to unnecessarily high CFDThresh, a forced CFD Trigger will be issued and a flag will be set in an event header word to indicate that the recorded CFD time for this event is invalid.

However, the event will still have a valid timestamp which is latched by the fast filter trigger when fast filter crosses over the trigger threshold. The aforementioned CFD parameters correspond to the following DSP parameters.

![Corresponding DSP Parameters for the CFD Parameters](/img/correspondingdspparametersforthecfdparmeters.png)


> **[info] 250 MHz**
>
> In the 250 MHz Pixie-16 modules, the event timestamp is counted with 125 MHz clock ticks, i.e., 8 ns intervals, and two consecutive 250 MHz ADC samples are captured in one 8 ns interval as well. 
>
> The CFD trigger also runs at 125 MHz, but the CFD zero crossing point is still reported as a fractional time between two neighboring 250 MHz ADC samples, which are processed by the FPGA in one 125 MHz clock cycle. 
>
> However, the CFD zero crossing point could be in either the odd or even clock cycle of the captured 250 MHz ADC waveforms. 
>
> Therefore, the firmware outputs a "CFD trigger source" bit in the output data stream to indicate whether the CFD zero crossing point is in the odd or even clock cycle of the captured 250 MHz ADC waveforms.

> **[info] 100 MHz**
>
> In the 100 MHz Pixie-16 modules, event timestamp, CFD trigger, and ADC waveform capture are all carried out with the same 100 MHz clock. So there is no need to report "CFD trigger source" for the 100 MHz Pixie-16 modules.



## 500 MHz modules

The CFD algorithm discussed in the previous section for the 100 MHz and 250 MHz Pixie-16 modules can also be written in the following format:  
$$CFD(k)=w\cdot\left(\sum_{i=k}^{k+L}a(i)-\sum_{i=k-B}^{k-B+L}a(i)\right)-\left(\sum_{i=k-D}^{k-D+L}a(i)-\sum_{i=k-D-B}^{k-D-B+L}a(i)\right)$$

Where a(i) is the ADC trace data, k is the index, and w, B, D, and L are CFD parameters.

The CFD algorithm implemented in the 500 MHz Pixie-16 modules is special when compared to the one implemented in the 100 MHz and 250 MHz Pixie-16 modules in terms of the ability to adjust parameters w, B, D, and L. 

The reason for this is that in the 500 MHz Pixie-16 modules, ADC data that come into the FPGA at the speed of 500 MHz is first slowed down with a ratio of 1:5, in other words, the FPGA captures 5 ADC samples at the rate of 100 MHz, i.e., every 10 ns. The FPGA then tries to find the CFD trigger point between any two adjacent 2-ns ADC samples within that 10 ns by first building sums of ADC samples and then calculating differences between delayed and non-delayed sums until the zero crossing point is found. However, in the 500 MHz Pixie-16 modules, the FPGA does not have enough resources to build sums for 5 ADC samples in parallel with variable delays. Therefore, the CFD algorithm for the 500 MHz modules was implemented using a set of fixed CFD parameters as shown in Table *Fixed CFD Parameter Values for 500 MHz Pixie-16 Modules*. Tests show these fixed parameters give best performance for LaBr3(Ce) detectors.

![Fixed CFD Parameter Values for 500 MHz Pixie-16 Modules](/img/fixedcfdparametervaluesfor500mhzpixie16modules.png)

The CFD time given by the 500 MHz Pixie-16 modules consists of two parts: a shift within the 5 ADC samples and a fractional time between two ADC samples where the CFD zero crossing occurred. The shift within the 5 ADC samples is reported as the 3-bit CFD trigger source[2:0] is defined as follows.

![Meanings of the CFD Trigger Source for 500 MHz Pixie-16 Modules](/img/meaningofthecfdtriggersourcefor500mhzpixie16modules.png)

The CFD fractional time is given as follows:

$$CFD=\frac{CFDout1}{CFDout1-CFDout2}\times8192$$

<!-- BASESETUP_CFD.md ends here -->
