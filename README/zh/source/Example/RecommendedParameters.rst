.. RecommendedParameters.rst --- 
.. 
.. Description: 
.. Author: Hongyi Wu(吴鸿毅)
.. Email: wuhongyi@qq.com 
.. Created: 三 7月  3 14:55:57 2019 (+0800)
.. Last-Updated: 一 11月  6 21:19:04 2023 (+0800)
..           By: Hongyi Wu(吴鸿毅)
..     Update #: 10
.. URL: http://wuhongyi.cn 

=================================   
推荐参数
=================================

**本页参数仅供参考**

---------------------------------
TAC
---------------------------------

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
100M
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- fast filter
	- FL: 
	- FG: 
- slow filter
	- SL: 
	- SG: 
	- Range: 1
	- TAU: 
- cfd filter
	- dealy: 
	- scale: 

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
250M
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- fast filter
	- FL: 
	- FG: 
- slow filter
	- SL: 
	- SG: 
	- Range: 1
	- TAU: 
- cfd filter
	- dealy: 
	- scale: 

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
500M
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- fast filter
	- FL: 
	- FG: 
- slow filter
	- SL: 
	- SG: 
	- Range: 1
	- TAU: 
- cfd filter
	- dealy: 
	- scale: 

----
	  
---------------------------------
NIM signal
---------------------------------


^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
100M
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- fast filter
	- FL: 
	- FG: 
- slow filter
	- SL: 
	- SG: 
	- Range: 1
	- TAU: 
- cfd filter
	- dealy: 
	- scale: 

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
250M
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- fast filter
	- FL: 
	- FG: 
- slow filter
	- SL: 
	- SG: 
	- Range: 1
	- TAU: 
- cfd filter
	- dealy: 
	- scale: 

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
500M
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- fast filter
	- FL: 
	- FG: 
- slow filter
	- SL: 
	- SG: 
	- Range: 1
	- TAU: 
- cfd filter
	- dealy: 
	- scale: 

----

---------------------------------
HPGe
---------------------------------

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
100M
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- fast filter
	- FL: 0.1
	- FG: 0.1
- slow filter
	- SL: 5.04
	- SG: 1.2/1.6
	- Range: 3
	- TAU: 以实际测量为准
- cfd filter
	- dealy: 0.12
	- scale: 3
	- 最佳分辨参数 0.06/6，但是将会导致大量的 cfd 无法过阈值，即低能部分 cfd 无法过阈值。

	  
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
250M
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- fast filter
	- FL: 0.13
	- FG: 0.13
- slow filter
	- SL: 5.04
	- SG: 1.2
	- Range: 3
	- TAU: 以实际测量为准
- cfd filter
	- dealy: 0.08
	- scale: 0

----	  

---------------------------------
BGO
---------------------------------

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
100M
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- fast filter
	- FL: 0.06
	- FG: 0.0
- slow filter
	- SL: 
	- SG: 
	- Range: 1
	- TAU: 以实际测量为准
- cfd filter
	- dealy: 0.08
	- scale: 0

----
	  
---------------------------------
He3
---------------------------------

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
100M
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- fast filter
	- FL: 0.2
	- FG: 0.2
- slow filter
	- SL: 
	- SG: 
	- Range: 1
	- TAU: 以实际测量为准
- CFD 不开启
	  
	  
----
	  
---------------------------------
Si
---------------------------------

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
100M
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

CIAE pre-amp

- fast filter
	- FL: 0.1
	- FG: 0.0
- slow filter
	- SL: 3.04
	- SG: 0.24
	- Range: 2
	- TAU: Based on actual measurements


mesytec pre-amp

- fast filter
	- FL: 0.2
	- FG: 0.0
- slow filter
	- SL: 3.52
	- SG: 0.8
	- Range: 2
	- TAU: Based on actual measurements
	  
----

---------------------------------
LaBr3
---------------------------------

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
250M
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- fast filter
	- FL: 0.08
	- FG: 0.016
- slow filter
	- SL: 0.144/0.128
	- SG: 0.048
	- Range: 1
	- TAU: 以实际测量为准(0.023)
- cfd filter
	- dealy: 0.024
	- scale: 0


^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
500M
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- fast filter
	- FL: 0.02
	- FG: 0.0
- slow filter
	- SL: 0.140
	- SG: 0.06
	- Range: 1
	- TAU: 以实际测量为准(0.023)



----

---------------------------------
BaF2
---------------------------------

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
500M 12bit
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- fast filter
	- FL: 0.02
	- FG: 0.0
- slow filter
	- SL: 1.24
	- SG: 0.06
	- Range: 1
	- TAU: 0.63
- CFD 开启时 PSD 参数
	- pre trigger 0.5
        - QDC 0.35/0.04/0.12
- CFD 不开启时 PSD 参数
	- pre trigger 0.4
        - 0.31/0.04/0.12
  

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
500M 14bit
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- fast filter
	- FL: 0.02
	- FG: 0.0
- slow filter
	- SL: 1.24
	- SG: 0.06
	- Range: 1
	- TAU: 0.63
- CFD 开启时 PSD 参数
	- pre trigger 0.5
        - QDC 0.35/0.04/0.16
- CFD 不开启时 PSD 参数
	- pre trigger 0.4
        - 0.31/0.04/0.16
	  

----

---------------------------------
LS
---------------------------------

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
500M 12/14bit
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- fast filter
	- FL: 0.02
	- FG: 0.0
- slow filter
	- SL: 0.14
	- SG: 0.06
	- Range: 1
	- TAU: 0.016
- CFD 开启时 PSD 参数
	- pre trigger 0.5
        - QDC 0.34/0.06/0.2

----

---------------------------------
He3
---------------------------------

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
100M 14bit
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- fast filter
	- FL: 0.2
	- FG: 0.4
- slow filter
	- SL: 4.96
	- SG: 6.08
	- Range: 4
	- TAU: 37.0(mesytec)
- CFD 不开启
- QDC 不开启


	  
.. 
.. RecommendedParameters.rst ends here
