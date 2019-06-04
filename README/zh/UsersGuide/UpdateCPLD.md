<!-- UpdateCPLD.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 一 6月  3 21:15:44 2019 (+0800)
;; Last-Updated: 二 6月  4 14:19:56 2019 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 2
;; URL: http://wuhongyi.cn -->

# 升级 CPLD

<!-- toc -->

当您在使用更新、更强大的计算机时遇到初始化(boot) Pixie-16 模块的问题。

根本原因是 XIA 通过 API 软件延迟的方式来确保在将固件比特流下载到 Pixie-16 模块时出现适当的延迟。

如何使用更好、或者多核的更好的计算机，这样的延迟在软件中无法保证。因此，用户需要对 CPLD 固件进行更新，以确保使用硬件方法满足此类延迟。

**请注意，此 CPLD 代码应用于更新所有 Pixie-16 模块，100 MHz，250 MHz或 500 MHz。**

## 6 pin JTAG connector

You should now try to locate the 6-pin vertical Pixie-16 CPLD JTAG header J8 (circled in red in the screenshot below).

![6 pin connector](/img/6pinJTAGconnector.png)

The next step is to identify each of the 6 pins of the Pixie-16 CPLD JTAG header J8. When facing the top side of the Pixie-16 board, the 6 pins of the J8 connector can be identified as follows from top to bottom:  **TDO**, **TDI**, **TMS**, **TCK**, **GND**, and **VCC**. It is very important to match these pins with the colored cables from the green PCB connector that is plugged into the Xilinx Platform USB programmer.

One issue here is that the Pixie-16 CPLD JTAG header J8 has 2 mm pin spacing, and the colored cables from the green PCB connector of the Xilinx programmer might have too large header to be plugged into the J8 pins directly. In that case, an adapter might be needed to convert those colored cable headers to 2 mm pin spacing first.

## update the CPLD

Turn on your Pixie16 crate and the LED on the Xilinx Platform Cable USB should turn green.  If the LED does not turn green check you cable connection.  The grey INIT cable is purposely left unconnected.  Your start menu should now include the folder Xilinx ISE Design Suite 14.7. Run the 32 bit or 64 bit version of "iMPACT".  Click on **File->Initialize Chain** and the program should find the **xc2c256 CPLD**.  Click on **"No"** in the window **"Auto Assign Configuration Files Query"**, then click on **"Cancel"**.   Left click on the Xilinx part in the window once, then right click and select **"Assign New Configuration Files"** and **select the new "jed" file**.  Left click on the the Xilinx part, right click and then click on **"Program"**.  A device programming properties window appears.  Ensure that only **"Verify"** and **"Erase before Programming"** are checked, then click on "OK".   The program should now say **"program succeeded"** in blue at the bottom of the window.  The CPLD is now reprogrammed and the Pixie16 is ready to be tested.


I would suggest testing out your first board before reprogramming the CPLDs on all of your boards.  Once you are in the batch mode of reprogramming and you have the cable connected (the USB programmer LED is green), you only need to click on "Program" to reprogram CPLD on your next Pixie16.



<!-- UpdateCPLD.md ends here -->
