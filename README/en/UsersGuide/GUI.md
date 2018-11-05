<!-- GUI.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 5月 13 20:23:55 2018 (+0800)
;; Last-Updated: 日 10月  7 09:38:06 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 48
;; URL: http://wuhongyi.cn -->

# GUI

<!-- toc -->

After setting the parameter file in the **parset**, enter the GUI directory and execute the following command to pop up the main control interface.

配置好 **parset** 内参数文件  
进入 GUI 目录，执行以下命令即可弹出主控制界面

```bash
./pku
```

## The main control interface

![MainFrame](/img/mainframe.png)

At the top of the main interface are five drop-down columns: File, UV_Setup, Expert, Monitor, and Offiline. The submenu inside is as follows:  
主界面最上方是File、UV_Setup、Expert、Monitor、Offiline五个下拉栏。里面的子菜单如下所示：

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
	- Simulation(Not yet implemented / 暂未实现)

After the main interface is open, select the **Online Mode** option. You need to connect to the chassis. All functions (including offline analysis) can be used in this mode. If the **Online Mode** option is not selected, it means that the offline mode is enabled, and the acquired parameters can be set and modified or analyze the acquired waveform.

开启获取主界面之后，选择 **Online Mode** 选项表示在线模式，需要连接机箱，该模式下可使用所有的功能（包括离线分析功能）。不选择**Online Mode** 选项则表示开启离线模式，可设置、修改获取参数，分析已采集波形。

After selecting or not selecting the **Online Mode** option, press the **Boot** button to start the initialization process and see the status changes in the *Information* section at the bottom.

选择、或者不选择 **Online Mode** 选项之后，按 **Boot** 按钮开启初始化过程，可看到最下方 *Information* 栏目中的状态变化。

After the system is successfully initialized, confirm the file storage path, file name, and file number in the *Setup* column. If there is any problem, modify it directly. After confirming, press **Complete**.

系统初始化成功后，再次确认 *Setup* 栏中的获取文件存放路径、文件名、文件编号是否有问题，如果有问题则直接修改，确认之后按 **Complete**。

After confirming the information in the *Setup* column, the main button **LSRunStart** in the *Control* column is open. Click this button to get data acquisition open, the button status changes to **LSRunStop**, click the button again, the data acquisition completed, and the *Run Num* number is automatically added.  Click **LSRunStart** again to open the next run.

确认 *Setup* 栏中的信息之后， *Control* 栏中的主按钮 **LSRunStart** 则开启，此时点击该按钮，获取则开启，按钮状态更改为 **LSRunStop**，再次点击该按钮，获取结束，运行的 *Run Num* 号码自动加一。再次点击 **LSRunStart** 开启下轮获取。

Currently, you can adjust and modify the parameters through the submenu in the top drop-down bar before you acquire data. Do not manipulate all options except the *Control* bar when acquiring data.

当前，获取之前可通过最上方的下拉栏里面的子菜单来调节、修改参数。获取数据时请勿操作 *Control*栏之外的所有选项。

The **Online Statistics** option in the *Control* column is turned on to get the input rate and output rate information for each signal sent to the *OnineStatistics* program every **3 s**.

*Control* 栏内的 **Online Statistics** 选项开启则获取每 **3 s** 向 *OnineStatistics* 程序发送时时每路信号的输入率、输出率信息。

**Clicking the Update Energy Monitor option once will send the one-dimensional spectrum of each channel in all the internal registers of the modules to the Online Statistics program. Sending this information will cause a certain dead time. Please do not click this option frequently.**

**点击 Update Energy Monitor 选项一次，则将所有采集卡内部寄存器中每路的一维能谱发送到 Online Statistics 程序，发送该信息会导致一定的死时间，请不要频繁点击该选项。** 





<!-- GUI.md ends here -->
