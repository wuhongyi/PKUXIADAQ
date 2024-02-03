.. DropDown3.rst --- 
.. 
.. Description: 
.. Author: Hongyi Wu(吴鸿毅)
.. Email: wuhongyi@qq.com 
.. Created: 三 7月  3 10:50:29 2019 (+0800)
.. Last-Updated: 四 8月 22 13:40:25 2019 (+0800)
..           By: Hongyi Wu(吴鸿毅)
..     Update #: 5
.. URL: http://wuhongyi.cn 

---------------------------------
Debug
---------------------------------

The adjustment in this drop-down column is to monitor the waveform noise level, baseline distribution, and so on.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Hist & XDT
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

"""""""""""""""""""""""""""""""""
Interface
"""""""""""""""""""""""""""""""""

.. image:: /_static/img/HistXDT.png

The binning factor controls the number of MCA bins in the spectrum. Energies are computed as 16 bit numbers, allowing in principle 64K MCA bins.

However, spectrum memory for each channel is limited to 32K bins, so computed energy values are divided by :math:`2^{binning factor}` before building the histogram. Binning Factor is usually set to 1, but for low count rates and wide peaks, it might be useful to set it to a larger value to obtain a spectrum with fewer bins, but more counts per bin.

:math:`E_{min}` is reserved for a future function to subtract a constant “minimum energy” from the computed energy value before binning to essentially cut off the lower end of the spectrum.
	   

----

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Trace & Baseline
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

"""""""""""""""""""""""""""""""""
Interface
"""""""""""""""""""""""""""""""""

.. image:: /_static/img/Trace.png
	   
.. image:: /_static/img/BaselineHist.png
	   
.. image:: /_static/img/BaselineTimestamps.png
      

**。。TODO。。**
	   
.. 
.. DropDown3.rst ends here
