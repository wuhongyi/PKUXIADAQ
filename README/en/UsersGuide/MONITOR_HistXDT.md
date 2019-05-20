<!-- MONITOR_HistXDT.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 10月  7 09:05:37 2018 (+0800)
;; Last-Updated: 一 5月 20 10:59:57 2019 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 2
;; URL: http://wuhongyi.cn -->

# Hist & XDT

![Hist & XDT](/img/HistXDT.png)

The binning factor controls the number of MCA bins in the spectrum. Energies are computed as 16 bit numbers, allowing in principle 64K MCA bins.

However, spectrum memory for each channel is limited to 32K bins, so computed energy values are divided by $$2^{binning factor}$$ before building the histogram. Binning Factor is usually set to 1, but for low count rates and wide peaks, it might be useful to set it to a larger value to obtain a spectrum with fewer bins, but more counts per bin.

$$E_{min}$$ is reserved for a future function to subtract a constant “minimum energy” from the computed energy value before binning to essentially cut off the lower end of the spectrum.

<!-- MONITOR_HistXDT.md ends here -->
