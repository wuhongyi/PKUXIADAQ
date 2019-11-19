.. DropDown4.rst --- 
.. 
.. Description: 
.. Author: Hongyi Wu(吴鸿毅)
.. Email: wuhongyi@qq.com 
.. Created: 三 7月  3 10:51:08 2019 (+0800)
.. Last-Updated: 二 11月 19 16:44:19 2019 (+0800)
..           By: Hongyi Wu(吴鸿毅)
..     Update #: 8
.. URL: http://wuhongyi.cn 

---------------------------------
Offiline
---------------------------------

The contents in this drop-down column are the offline parameter optimization adjustment.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
InitData
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. image:: /_static/img/AdjustPar.png

- *Run*: Select the file run number to be read, *Mod*: selects to read which module, and the button *Read* loads the file main information (channel address, energy, waveform position, etc.) into the memory.
- Additional analysis: Among the three options, the selection indicates that the information is included when reading the file data into memory. Some analysis methods can only be enabled if this data is read. But the premise is that this information needs to be recorded during data collection.


- InitData: This page. Read the binary file.
- Adjust Par:  Waveform/fast filter/energy filter/cfd/threshould of the select channel.
- Wave-16:  Waveform/fast filter/energy filter/cfd filter/threshould of the 16 channels.
- Energy-16:  The original energy spectrum of the 16 channel..
- Orig Energy:  The original energy spectrum of the select channel.
- Calc Energy:  Recalculation of energy spectrum by waveforms.
- FF/CFD Thre:   Accumulation of a large number of fast filter/cfd filter waveforms to determine the threshold.
- Energy-FF:  The two dimensional relationship between energy and fast filter first/second peak height.
- QDC:  It will be finished soon.
- FFT:  Fourier transformation of a single waveform. User can choose XIA/CAEN/FFTW3 functions.
- Time Diff:  Time difference of two signal.

----

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Adjust Par
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. image:: /_static/img/OfflineAPI.png

To calculate the fast filter and slow filter cfd curves offline from the acquired waveform, the following requirements are imposed on the acquired waveform. In the above figure, the calculated energy is the difference between the energy of the algorithm and the baseline of the algorithm. To get the correct trapezoid, the premise is that there is a long enough point in front to calculate the baseline.

In the figure, EFRT stands for Energy Filter Rise Time and EFFT stands for Energy Filter Flat Top.

To compute energy filter response offline, the ideal settings are:

- Total trance length > 2 x ( 2 x EFRT + EFFT)
- Pre-trigger trace (Trace-delay) length > ( 3 x EFRT + EFFT )

Of course, this is just one way to calculate the trapezoid. If we record the baseline of the energy trapezoid for each event and use the average of the pre-trigger partial points as the infinite extension to the left of the waveform, then this is not limited by this condition Pre-trigger trace length > ( 3 x EFRT + EFFT ) . In the following pages, when using the Old Baseline method to calculate the energy trapezoid, there is a premise that the pre-trigger trace length requires at least 200 points because the left side of the waveform is averaged by the first 200 points.

.. image:: /_static/img/Adjustpars3.png

When the acquired waveform pre-trigger trace length > 3 x EFRT + EFFT, the pre-trigger trace provides enough points to calculate the baseline, and the SF BL algorithm can choose Calculate, otherwise the Old Baseline algorithm needs to be selected. When recording data with the premise of the Old Baseline algorithm, select to turn on the baseline for the trapezoid and the raw E sums/baseline option for the InitData page. When the Old Baseline algorithm is selected, the next four option parameters take effect, which are the parameters of the energy trapezoid used in the data acquisition.

The green curve in the above figure is a typical condition that does not satisfy the pre-trigger trace length > 3 x EFRT + EFFT when the Calculate algorithm is used. The figure shows a pre-trigger trace length of 10 us, an EFRT of 5.04 us, and an EFFT of 1.60 us.

In this case, you should use the Old Baseline algorithm shown below.	   
	   
.. image:: /_static/img/Adjustpars1.png

The user can choose to view the channel of the waveform. The button *Load* can read and display the current parameter setting. When modifying the above parameters, you need to press the *Apply* button to make it effective. The button *Draw* is used to display the event waveform of the next channel.

Users can choose to display multiple waveforms in **Wave / Slow Filter / Fast filter / Thres / CFD / CFD Thres** or select the drawing style of the curve.
	   
.. image:: /_static/img/Adjustpars2.png

The figure above shows four waveforms，fast filter, Thres, CFD, and CFD Thres. The pattern displays in dots. The lowest level horizontal bar can be dragged at both ends, and the user can pull to control the display range of the waveform abscissa, as shown in the figure of 800 - 1300 points. In this case, clicking the Draw button will hold the specified coordinate range.
	   
----
  
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Wave-16
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. image:: /_static/img/Waveform16.png

This page is used to view 16 channels of raw waveforms, filter waveforms, thresholds, and so on. Users can choose to display multiple waveforms in **Wave/Slow Filter/Fast filter/Thres/CFD/CFD Thres** simultaneously.

Through this page, the user can quickly check whether the waveforms of all channels of the module are normal and whether the parameter settings are reasonable. Click the button **Draw** once to display the next waveform for all channels.

*It should be noted that when the acquired waveform pre-trigger trace length is greater than 3 x EFRT + EFFT, the Slow Filter waveform on this page is correct.*


----
  
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Energy-16
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. image:: /_static/img/Energy16.png

This interface is used to view the 1D spectrum of 16 channels simultaneously. Click the button *Draw* in the top right corner.

	   
----
  
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Orig Energy
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. image:: /_static/img/OrigEnergy.png

This page is used to quickly view the energy spectrum of a channel. The user selects the number of bins of the spectrum, which represents how many points we can didide 0-65536 into. Select to view the channel and then press the *Draw* button.

.. image:: /_static/img/OrigEnergyFit.png

The *Open Fit* button in the upper left corner is used for fast Gaussian fitting to see energy resolution. Click the button to turn on the fitting mode, and click the button again to turn it off. Move the mouse to the blue line of the histogram and the mouse cross will become a triangular arrow. The mouse of the triangle arrow clicks on two positions in the histogram, and the interval between the two points is the fitting interval, then the energy resolution can be viewed.

----
  
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Calc Energy
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. image:: /_static/img/CalcEnergy.png

This page recalculates the energy using the acquired waveform. Like the **Adjust Par** page, the SF BL algorithm can choose either the Calculate algorithm or the Old Baseline algorithm.

The fast filter and energy filter parameters used to calculate the energy adopt the setting parameters of the module. The user needs to select the number of bins that the energy about 65536 channels are divided into. You can select 1024/2048/4096/8192/16384/32768/65536. Select the calculated channel and then press the button *Draw* to start the calculation, the upper left corner will display the progress of the calculation, or press the button *Stop* to terminate the calculation early. When the calculation is terminated, the spectrum will be displayed on the artboard.
	   

----
  
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
FF/CFD Thre
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. image:: /_static/img/FFCFDThre.png

This interface is used for the accumulation of fast filter waveforms and cfd filter waveforms. The user selects the channel to view and presses the *Draw* button to start the calculation. The top left corner of the page can monitor the progress from time to time, or press the *Stop* button to terminate the calculation early. The calculation ends as shown in the figure above.


The upper buttons *FFProjectY*, *CFDProjectY*, *OriginalCFD*, and *CalculateCFD* respectively pop up the sub-painters.
	   
.. image:: /_static/img/FFCFDThreFFProjectY.png

Click the button *FFProjectY* to open the view of the fast filter projection. Click again to close the function. When the function is turned on, place the mouse on the 2D map and move the mouse left and right. The Fast Filter ShowProjectY sub-panel displays the projection distribution of the position pointed by the mouse. This distribution before triggering also characterizes the level of noise.
	   
.. image:: /_static/img/FFCFDThreCFDProjectY.png

Similarly, the button *CFDProjectY* function is as shown in the figure above.

.. image:: /_static/img/FFCFDThreCFD.png

Click the button *OriginalCFD* to display the distribution of CFD values in the raw data on the left. Click the button *CalculateCFD* to display the result of the offline waveform calculation on the right, and the parameters used for the calculation are the current ones. For a suitable CFD parameter setting, the CFD should be evenly distributed.

	   
----
  
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Energy-FF
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. image:: /_static/img/EnergyFFGraph.png

This interface is a two-dimensional map of the energy and fast filter peak heights used to determine the appropriate threshold. The left picture is a two-dimensional correlation between energy and fast filter, which should have a good linear relationship. The right picture shows the two-dimensional correlation between the energy and the remaining maximum value of the part of the fast filer where the trapezoid is thrown away, which characterizes the noise level. And the energy should be uncorrelated with this value.

Firstly, for **Draw Style** we choose **Graph**, which is the 2D scatter plot mode. Select the channel you want to view, then press the *Draw* button to start the calculation, in the top left corner the progress from time to time can be monitored, or you can press the *Stop* button to terminate the calculation early. The calculation ends as shown in the figure above.
	  	   
.. image:: /_static/img/EnergyFFHist.png

The 2D scatter plot does not visually show the density distribution of the displayed data points, so for **Draw Style** we select the **Hist mode**. Select the bins or ranges of the X and Y axis, and then press the *Draw* button to start the calculation. The result is shown in the figure above, and the right figure reflects the level of noise.

	   
----

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
CFD D/F
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. image:: /_static/img/CFDDF.png
	   
This interface is used to assist in selecting a reasonable CFD filter parameters. Before using this interface, the waveform must be acquired with two channels(the detector output signal can be divided into two channels). It is well known that for a scale parameter, user need to choose the best delay parameter. This page calculates the time resolution under the different "scale - delay" combinations, from which the user can select the appropriate combination of parameters.

One thing to note is that time resolution depends not only on the CFD filter parameters, but also on the fast filter parameters.
	   
----

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
CFD Frac
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. image:: /_static/img/CFDFrac.png

This interface counts the distribution of the maximum position of the rising edge slope in the fast filter waveform.

	   
----

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Energy-CFD
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. image:: /_static/img/EnergyCFD.png

- The upper left picture shows the energy spectrum when cfd is invalid.
- The lower left picture shows the energy spectrum when cfd is valid.
- The upper right picture shows the CFD spectrum.
- The lower right picture is a two-dimensional map of energy and CFD.

首先选择能量与CFD二维关联图中bin数。其中 CFD 分 bin 可选择 4096，2084，1024；能量可选择bin数与道址范围。  
之后选择查看通道，然后按 Draw 按钮开始进入计算。

	   
----
  
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
QDC
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**to do not completed**

**QDC  TODO**


----
  
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
FFT
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. image:: /_static/img/FFT.png

This interface is used to quickly view the Fourier transform of the waveform. Users can choose different algorithms, such as **XIA, fftw3, CAEN(HANNING), CAEN(HAMMING), CAEN(BLACKMAN), CAEN(RECT)**. Select the channel you want to view. Then press the *Draw* button, and each time the button is clicked, the next result is displayed.

the ADC trace display also includes the option to view a FFT of the acquired trace. This is useful to diagnose noise contributions.

	   
----
  
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Time Diff
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. image:: /_static/img/TimeDiff.png

This interface is used to quickly view the time resolution of the two signals. The user can choose to view the time difference between the two signals of the zero crossing of the CFD algorithm or the time difference between two signals of the fast filter over threshold. **Xbin** represents the bin number of the abscissa, **Xmin** represents the minimum value of the abscissa, and **Xmax** represents the maximum value of the abscissa. With Ch A, Ch B we can select the two channels we want to view. Then press the *Draw* button.


The option **Limits** selection turns on the energy range constraint. After selecting this option, the following four parameters, AL, AR, BL, and BR, take effect, which respectively represent the left and right ranges of the Ch A/B energy address, and only events with energy falling in this interval are filled into the histogram. The **Orig Energy** page allows the user to select the appropriate energy channel interval.
	   

----

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Simulation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Different types of detection and different signal-to-noise ratio waveforms are generated by the model to assist the user in learning parameter optimization adjustment.

	   
.. 
.. DropDown4.rst ends here
