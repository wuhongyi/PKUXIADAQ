<!-- UpdateCPLD.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 一 6月  3 21:15:44 2019 (+0800)
;; Last-Updated: 一 6月  3 21:25:34 2019 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 1
;; URL: http://wuhongyi.cn -->

# 升级 CPLD

当您在使用更新、更强大的计算机时遇到初始化(boot) Pixie-16 模块的问题。

根本原因是 XIA 通过 API 软件延迟的方式来确保在将固件比特流下载到 Pixie-16 模块时出现适当的延迟。

如何使用更好、或者多核的更好的计算机，这样的延迟在软件中无法保证。因此，用户需要对 CPLD 固件进行更新，以确保使用硬件方法满足此类延迟。

**请注意，此 CPLD 代码应用于更新所有 Pixie-16 模块，100 MHz，250 MHz或 500 MHz。**



<!-- UpdateCPLD.md ends here -->
