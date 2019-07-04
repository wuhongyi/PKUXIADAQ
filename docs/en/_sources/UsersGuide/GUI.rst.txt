.. GUI.rst --- 
.. 
.. Description: 
.. Author: Hongyi Wu(吴鸿毅)
.. Email: wuhongyi@qq.com 
.. Created: 三 7月  3 10:39:34 2019 (+0800)
.. Last-Updated: 三 7月  3 16:30:26 2019 (+0800)
..           By: Hongyi Wu(吴鸿毅)
..     Update #: 4
.. URL: http://wuhongyi.cn 

=================================
GUI
=================================

After setting the parameter file in the **parset**, enter the GUI directory and execute the following command to pop up the main control interface.

.. code:: bash

  ./pku	 

---------------------------------
The main control interface
---------------------------------

.. image:: /_static/img/mainframe.png

At the top of the main interface are five drop-down columns: File, UV_Setup, Expert, Monitor, and Offiline. The submenu inside is as follows:

- File
	- Exit
	- About
- UV_Setup
	- Base Setup
	- Trigger Filter
	- Energy
	- CFD
	- QDC
	- Decimation
	- Copy Pars
	- Save2File
- Expert
	- Module Variables
	- CSRA
	- Logic Set
- Monitor
	- Hist & XDT
	- Trace & Baseline
- Offiline
	- Adjust Par
	- Simulation(Not yet implemented)

After the main interface is open, select the **Online Mode** option. You need to connect to the chassis. All functions (including offline analysis) can be used in this mode. If the **Online Mode** option is not selected, it means that the offline mode is enabled, and the acquired parameters can be set and modified or analyze the acquired waveform.

After selecting or not selecting the **Online Mode** option, press the **Boot** button to start the initialization process and see the status changes in the *Information* section at the bottom.

After the system is successfully initialized, confirm the file storage path, file name, and file number in the *Setup* column. If there is any problem, modify it directly. After confirming, press **Complete**.

After confirming the information in the *Setup* column, the main button **LSRunStart** in the *Control* column is open. Click this button to get data acquisition open, the button status changes to **LSRunStop**, click the button again, the data acquisition completed, and the *Run Num* number is automatically added.  Click **LSRunStart** again to open the next run.

Currently, you can adjust and modify the parameters through the submenu in the top drop-down bar before you acquire data. Do not manipulate all options except the *Control* bar when acquiring data.

The **Online Statistics** option in the *Control* column is turned on to get the input rate and output rate information for each signal sent to the *OnineStatistics* program every **3 s**.

**Clicking the Update Energy Monitor option once will send the one-dimensional spectrum of each channel in all the internal registers of the modules to the Online Statistics program. Sending this information will cause a certain dead time. Please do not click this option frequently.**

	
----

.. include:: DropDown0.rst
	     
----

.. include:: DropDown1.rst
	     
----

.. include:: DropDown2.rst
	     
----

.. include:: DropDown3.rst
	     
----

.. include:: DropDown4.rst
	     


.. 
.. GUI.rst ends here
