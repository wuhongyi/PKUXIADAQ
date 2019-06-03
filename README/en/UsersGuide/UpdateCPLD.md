<!-- UpdateCPLD.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 一 6月  3 21:16:13 2019 (+0800)
;; Last-Updated: 一 6月  3 21:25:34 2019 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 1
;; URL: http://wuhongyi.cn -->

# Update CPLD

If you encountered issues with booting the Pixie-16 modules when you were using a newer and more powerful computer.

The root cause is that XIA API software replies on delays in the software to ensure proper delays when downloading the firmware bitstream to the Pixie-16 modules. 

However, as the computer gets faster and faster with multi-cores, such delays can no longer be guaranteed in the software. So you had to update XIA CPLD firmware to ensure such delay is met using hardware methods.  

**Please note this CPLD code should be used to update all of your Pixie-16 modules, 100 MHz, 250 MHz or 500 MHz.**




<!-- UpdateCPLD.md ends here -->
