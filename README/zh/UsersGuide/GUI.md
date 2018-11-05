<!-- GUI.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 5月 13 20:23:55 2018 (+0800)
;; Last-Updated: 一 11月  5 16:34:02 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 49
;; URL: http://wuhongyi.cn -->

# 图形交互界面

<!-- toc -->

配置好 **parset** 内参数文件  
进入 GUI 目录，执行以下命令即可弹出主控制界面

```bash
./pku
```

## 主控制界面

![MainFrame](/img/mainframe.png)

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
	- Simulation(暂未实现)


开启获取主界面之后，选择 **Online Mode** 选项表示在线模式，需要连接机箱，该模式下可使用所有的功能（包括离线分析功能）。不选择**Online Mode** 选项则表示开启离线模式，可设置、修改获取参数，分析已采集波形。

选择、或者不选择 **Online Mode** 选项之后，按 **Boot** 按钮开启初始化过程，可看到最下方 *Information* 栏目中的状态变化。

系统初始化成功后，再次确认 *Setup* 栏中的获取文件存放路径、文件名、文件编号是否有问题，如果有问题则直接修改，确认之后按 **Complete**。

确认 *Setup* 栏中的信息之后， *Control* 栏中的主按钮 **LSRunStart** 则开启，此时点击该按钮，获取则开启，按钮状态更改为 **LSRunStop**，再次点击该按钮，获取结束，运行的 *Run Num* 号码自动加一。再次点击 **LSRunStart** 开启下轮获取。

当前，获取之前可通过最上方的下拉栏里面的子菜单来调节、修改参数。获取数据时请勿操作 *Control*栏之外的所有选项。

*Control* 栏内的 **Online Statistics** 选项开启则获取每 **3 s** 向 *OnineStatistics* 程序发送时时每路信号的输入率、输出率信息。

**点击 Update Energy Monitor 选项一次，则将所有采集卡内部寄存器中每路的一维能谱发送到 Online Statistics 程序，发送该信息会导致一定的死时间，请不要频繁点击该选项。** 





<!-- GUI.md ends here -->
