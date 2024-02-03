.. FrontPanel.rst --- 
.. 
.. Description: 
.. Author: Hongyi Wu(吴鸿毅)
.. Email: wuhongyi@qq.com 
.. Created: 三 7月  3 13:58:24 2019 (+0800)
.. Last-Updated: 六 9月 21 10:28:12 2019 (+0800)
..           By: Hongyi Wu(吴鸿毅)
..     Update #: 7
.. URL: http://wuhongyi.cn 

=================================   
插件前面板
=================================

每个 Pixie-16 模块的前面板上都有 16 个模拟信号输入连接器，一个 LVDS I/O 端口，五个数字 I/O 连接器以及前面板底部附近的三个 LED。 另外，在前面板的顶部手柄上贴有显示 Pixie-16 型号的标签（例如P16L-250-14，表示 Pixie-16 250 MHz 14bit 模块）。 Pixie-16 模块的序列号（例如，S/N 1100）位于前面板的底部手柄处。

----------------------------------------------
模拟信号输入连接器(all revisions)
----------------------------------------------

- 连接器标签
	- 16 通道：0 至 15
- 连接器类型
	- SMB Jack

每个 Pixie-16 模块均接受 16 个模拟输入信号，每个输入连接器均为 SMB插孔（插头触点）连接器。	  

---------------------------------
LVDS I/O 端口 (all revisions)
---------------------------------

.. image:: /_static/img/lvdsioport.png

每个 Pixie-16 模块在其前面板上均配有一个 LVDS I/O 端口。 LVDS 代表低压差分信号。 LVDS I/O 连接器是 RJ45 连接器，这意味着可以使用相同的 Cat 5 或 Cat 6 以太网电缆将信号连接到该 I/O 端口或从该 I/O 端口连接信号。但是，此 Pixie-16 I/O 端口没有以太网连接。
	   
可从此 I/O 端口获得四个差分信号对，即引脚对 :math:`Fo_{1p}` / :math:`Fo_{1n}`, :math:`Fi_{1p}` / :math:`Fi_{1n}`, :math:`Fi_{5p}` / :math:`Fi_{5n}`, and :math:`Fo_{5p}` / :math:`Fo_{5n}`。每对都可以配置为输入或输出信号。
	   
------------------------------------
数字 I/O 连接器(Rev. F only)
------------------------------------

.. image:: /_static/img/digitalioconnectorsabcdande.png

Pixie-16 Rev. F 模块的前面板上装有五个 har-link 连接器，用作数字 I/O 连接器。 HARTING 的 2mm 间距 har-link 连接器专为高速数据传输而设计，速率高达 2 Gbit/s。 其电磁干扰屏蔽如图 *2mm pitch har-link connector from HARTING* 所示，可确保在电磁污染环境下的出色性能。	   
	   	   
.. image:: /_static/img/2mmpitchharlinkconnectorfromharting.png

每个 har-link 连接器有 2 行，每行有 5 个引脚，并使用从 A 到 E 的五个红色字母之一标记。表 *Rev. F Module’s Digital I/O Connectors* 中定义了连接到这五个连接器的每个引脚的信号。
	   
.. image:: /_static/img/revfmoduledigitalioconnectors.png

其中，FI0，FI2，FI3，FI4，FI6，FI7 是六个 TTL 数字输入信号。 它们可以是诸如全局快速触发（global validation trigger），全局验证触发（global validation trigger），外部时钟（external clock），运行禁止（run inhibit）等信号。每个输入引脚的具体用法取决于下载到 Pixie-16 模块的特定固件（标准固件支持的输入信号请参见上表）。 六个数字输出信号 FO0，FO2，FO3，FO4，FO6，FO7 连接到 Pixie-16 的系统 FPGA 上的六个测试输出引脚，可用于帮助用户进行系统设置。 这些测试引脚连接到 Pixie-16 的各种内部信号，以洞悉系统的当前状态。

通道门输入（16 通道为 0-15）是 LVDS 格式的输入信号，可独立控制 Pixie-16 模块 16 个通道中每个通道的数据采集。

通道门信号是电平敏感信号，即当通道门信号的电平为逻辑高电平（1）时，该门信号有效; 当通道门信号的电平为逻辑低电平（0）时，未使用门信号。 在正常情况下，将通道门信号设置为否决（veto）给定通道中的数据采集，即，在该通道中快速触发到达时，如果通道门信号为逻辑高（1），则该快速触发（fast filter）被否决，因此将其丢弃。 但是，可以通过软件在 FPGA 中设置相应的寄存器来反转这种逻辑。 在这种情况下，将通道门信号设置为验证给定通道中的数据采集，即在该通道中的快速触发时，仅当通道门信号为逻辑高电平（1）时，该触发才会被触发并记录事件。

模块门控输入是 LVDS 格式信号，用于门控 Pixie-16 模块所有 16 个通道中的数据采集。 它也是一个对电平敏感的信号，即当模块门信号的电平为逻辑高电平（1）时，门信号有效; 当模块的电平为逻辑低电平（0）时，未使用门信号。 在正常情况下，如果模块门信号为逻辑高（1），则将模块门信号设置为否决（veto）所有 16 个通道中的数据采集，即在 16 个通道中的任何一个通道中快速触发到达时，则该通道的快速触发已被否决，因此将被丢弃。 但是，可以通过软件在 FPGA 中设置相应的寄存器来反转这种逻辑。在这种情况下，将模块门信号设置为验证所有 16 个通道中的数据采集，即，在 16 个通道中的任何一个快速触发时，仅当模块门信号为逻辑高电平（1）时， 该通道的快速触发被接受以记录事件。

备用门输入是LVDS格式的信号，保留给特殊应用。

此类应用程序通常需要开发定制固件以支持 Pixie-16 系统的特殊功能。

.. image:: /_static/img/frontpanel_pku.png
	
---------------------------------
前面板 LEDs (all revisions)
---------------------------------

在 Pixie-16 前面板底部附近，有三个LED。 它们从左到右分别标记为 RUN，I/O 和 ERR。 它们分别对应三种不同的颜色，分别为**绿色，黄色和红色**。

.. image:: /_static/img/frontpanelledsforthepixie16modules.png

当 Pixie-16 模块中的运行正在进行时，**RUN LED** 将亮起，而当运行停止或尚未开始时，则熄灭。
	   
当 Pixie-16 模块和主机之间的 PCI 总线上有 I/O 活动时，**I/O LED** 将闪烁。

实际上，**ERR LED** 并不表示 Pixie-16 模块中的任何错误情况。 相反，它用于指示 Pixie-16 模块的外部 FIFO 是否已满。 当外部 FIFO 中没有足够的空间来存储列表模式事件数据时，该指示灯将亮起；如果有足够的空间来存储至少一个列表模式事件数据，则该指示灯将熄灭。当外部 FIFO 已满时，无法再将列表模式事件数据写入其中，直到主机软件通过 PCI 总线读出外部 FIFO 中的部分数据为止。

	   
---------------------------------
3.3V I/O 连接器(Rev. D only)
---------------------------------

.. image:: /_static/img/revdmodules33vioconnector.png

在版本 D Pixie-16 模块上，通道 7 和通道 8 的模拟输入 SMB 连接器之间是 3.3V I/O 连接器（J200）。 它具有 10 个 2mm 间距的单端引脚。引脚＃1、3、4、5、6 和 8 连接到来自 Pixie-16 模块的系统 FPGA 的六个数字输出信号，即 FO0，FO2，FO3，FO4，FO6，FO7，主要用于测试和调试。＃2 和 7 引脚为接地引脚，而 ＃9 和 10 引脚未使用。
	   
---------------------------------
GATE Inputs(Rev. D only)
---------------------------------

.. image:: /_static/img/revdmodulesgateinputs.png

在版本 D Pixie-16 模块上，通道 11 和通道 12 的模拟输入SMB连接器之间是门输入连接器。此连接器是 2mm 针间距的 Amphenol FCI 55 Position Header。这 55 个引脚的布局如图 *Rev. D Module’s GATE INPUTS Connector* 所示。中间引脚列（J150C）的11个引脚均接地。在门输入连接器的前 8 行中，来自 A/B 列（J150A/J150B）或 D/E列（J150D/J150E）的每对差分引脚对应于一个通道的输入门，其格式为LVDS ，例如 Gxin+/Gxin-(x=0-15)。 J150A3/J150B3 上的差分引脚对是模块栅极输入信号 MGin+/MGin-。通道门输入信号可用于否决或验证给定通道自己的触发信号。模块门输入信号在整个模块级别上起作用，即，它可以用于否决或验证给定模块的所有 16 个通道自己的触发信号。 J150D3/J150E3 上的差分引脚对是备用输入信号 SGin+/SGin-。备用门输入信号可用于需要定制固件的特殊应用。

.. image:: /_static/img/revdmodulesgateinputsconnector.png

在 Rev. D Pixie-16 模块上，TTL 数字输入信号（最大5V），即 FI0，FI2，FI3，FI4，FI6，FI7，分布在门输入连接器的底部两行中，如图 *Rev. D Module’s GATE INPUTS Connector* 所示。

--------------------------------------
3.3V I/O 连接器(Rev. B and C only)
--------------------------------------

.. image:: /_static/img/revbandcmodules33vioconnector.png

在 B 和 C 版本的 Pixie-16 模块上，在通道 11 和通道 12 的模拟输入 SMB 连接器之间是 3.3V I/O连接器（J100）。 它具有 16 个单端引脚，间距为 2mm。 引脚 ＃1、3、4、5、6 和 8 连接到来自 Pixie-16 模块的系统 FPGA 的六个数字输出信号，即 FO0，FO2，FO3，FO4，FO6，FO7，主要用于测试和调试。 引脚 ＃2、7、10 和 15 是接地引脚。 引脚 ＃9、11、12、13、14 和 16 连接到六个 TTL 数字输入信号（最大5V），即 FI0，FI2，FI3，FI4，FI6，FI7。

	   
----------------------------------------------------
标准固件中的数字信号(all revisions)
----------------------------------------------------

Pixie-16 的标准固件通过其前面板 I/O 连接器支持数字信号的输入和输出，这已在前面进行了讨论。

.. image:: /_static/img/ttldigitalinputsignals.png

表 *TTL Digital Input Signals* 显示了 Pixie-16 标准固件支持的五个 TTL 数字输入信号。

其中，信号 **EXT_TS_CLK** 和 **EXT_TS_CLR** 用于 Pixie-16 中的外部时间戳记，即 Pixie-16 接受外部时钟信号（为了避免出现时钟信号完整性问题，不建议该外部时钟的频率超过 20 MHz），使用 48位 计数器对此类时钟信号进行计数，并在发生事件触发时将此类计数器值输出到列表模式数据流。

通过将两个系统记录的事件的外部时间戳相关联，外部时间戳可用于将 Pixie-16 数据获取系统与另一个数据获取系统同步。

当在 Pixie-16 模块中启用了同步要求时，外部系统会使用 **INHIBIT** 信号来禁止在 Pixie-16 系统中运行数据采集。 这是一个对电平敏感的信号，即当 **INHIBIT** 信号为逻辑高电平时，Pixie-16 的运行将不会开始。 只有当 **INHIBIT** 信号变为逻辑低电平时，Pixie-16 才会开始运行。 在运行期间，如果 **INHIBIT** 信号返回逻辑高电平，则运行将中止。

**EXT_FASTTRIG** 信号是外部快速触发信号，可用于代替本地快速触发来在 Pixie-16 模块中记录事件。 **EXT_VALIDTRIG** 信号是外部验证信号，可用于验证 Pixie-16 模块中的事件。

.. image:: /_static/img/connectorj101lvdsioportsignals.png

表 *Connector J101 LVDS I/O Port Signals* 显示了 Pixie-16 连接器 J101 LVDS I/O 端口信号。 此 J101 LVDS I/O 端口可以使用常规以太网电缆进行连接，但没有以太网连接。 该 J101 端口可提供的四对 LVDS 对中，当前未使用一对，输入使用两对，输出使用一对。 **LVDS_VALIDTRIG** 是 LVDS 格式的外部验证触发信号， **LVDS_FASTTRIG** 是 LVDS 格式的外部快速触发信号。 **SYNC_LVDS_FP** 是来自Pixie-16模块的输出信号，用于向外部数据采集系统指示 Pixie-16 系统的同步状态，以便可以同步两个数据采集系统。

.. image:: /_static/img/ttldigitaloutputsignals.png

表 *TTL Digital Output Signals* 列出了六个 Pixie-16 TTL 数字输出信号。 可以通过软件设置选择两组六个输出信号（请参见 TrigConfig0 的位[14:12]和[19:16]）。可以通过软件设置进一步选择最后的输出信号 TEST_SEL。 有关这些信号的更多详细信息将在本手册的后续部分中提供。


----

..
   FTRIG_DELAY  采集延迟时间  只要fast filter 过阈值就会产生
   FTRIG_VAL    基本同上，有效采集时候才有信号
   GLBETRIG_CE  stretched external global validation trigger 
   CHANETRIG_CE stretched channel validation trigger ，开启CSRA bit13 。采集延迟必须在这个时间窗内才能采集到    看到的信号实际起始位置在 100 ns，意味着System FPGA 处理时间需要 100 ns ？ 信号宽度由 ChanTrigStretch 控制。 
	   
.. 
.. FrontPanel.rst ends here
