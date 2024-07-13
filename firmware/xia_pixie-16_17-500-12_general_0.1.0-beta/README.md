FW preview release, version 0x3E_01,  2/26/24

- Pixie16DSP./ldr/lst/var: version 3.3E
  unchanged

- PXlarge500.bin: version 0x3EA1, build 0x0002 for DB10 12/500
  2nd draft for DB10, debugged ADC SPI

- PXlarge250.bin: version 0x3E41, build 0x0001 for DB04, DB08 14/250
  unchanged

- PXlarge125.bin: version 0x3C51, build 0x0001, for DB04, DB08 14/125
  unchanged, not updated, do not use with release 3D or 3E

- PXlargeSYS_500.bin: version 0x603D_A9A1 for DB10  12/500
  unchanged - first draft for DB10

- PXlargeSYS_250.bin: version 0x103E_A941 for DB04, DB08 14/250
  unchanged

- PXlargeSYS_125.bin: version 0x003C_A951  for DB04, DB08 14/125
  unchanged, not updated, do not use with release 3D or 3E


FW preview release, version 0x3E_01,  1/31/24

- Pixie16DSP./ldr/lst/var: version 3.3E
  fixed bug that would ingnore CCSRA_EXTTSENA bit for LM runs

- PXlarge500.bin: version 0x3DA1, build 0x000B for DB10 12/500
  unchanged - first draft for DB10

- PXlarge250.bin: version 0x3E41, build 0x0001 for DB04, DB08 14/250
  added VETOMODE=6, generating VETO for any channel pair 
  VETO can distributed to other channels/modules as in VETOMODE=2-5 (NEXT beta)

- PXlarge125.bin: version 0x3C51, build 0x0001, for DB04, DB08 14/125
  unchanged, not updated, do not use with release 3D or 3E

- PXlargeSYS_500.bin: version 0x603D_A9A1 for DB10  12/500
  unchanged - first draft for DB10

- PXlargeSYS_250.bin: version 0x103E_A941 for DB04, DB08 14/250
  the external clk/sync is now daisychanned right to left. (was left to right)
  This allows the inputs to be a PDM to the right of a group of modules

- PXlargeSYS_125.bin: version 0x003C_A951  for DB04, DB08 14/125
  unchanged, not updated, do not use with release 3D or 3E



FW preview release, version 0x3D_0B,  1/15/24

- Pixie16DSP./ldr/lst/var: version 3.3B
  unchanged

- PXlarge500.bin: version 0x3DA1, build 0x000B for DB10 12/500
  first draft for DB10

- PXlarge250.bin: version 0x3D41, build 0x0008 for DB04, DB08 14/250
  unchanged

- PXlarge125.bin: version 0x3C51, build 0x0001, for DB04, DB08 14/125
  unchanged, not updated, do not use with release 3D

- PXlargeSYS_500.bin: version 0x603D_A9A1 for DB10  12/500
  first draft for DB10

- PXlargeSYS_250.bin: version 0x603D_A941 for DB04, DB08 14/250
  unchanged

- PXlargeSYS_125.bin: version 0x003C_A951  for DB04, DB08 14/125
  unchanged, not updated, do not use with release 3D





FW preview release, version 0x3D,  11/29/23

- Pixie16DSP./ldr/lst/var: version 3.3B
  unchanged

- PXlarge250.bin: version 0x3D41 for DB04, DB08 14/250
  connected external clock/sync inputs to System signals, test outputs on 34x header

- PXlarge125.bin: unchanged, not updated, do not use with release 3D

- PXlargeSYS_250.bin: version 0x603D_A941 for DB04, DB08 14/250
  added external clock/sync, daisy chain from backplane and distributed to Fippi
  New parameter value: GROUPMODE_BP=2 outputs clock/sync on front panel Veto/SYNC MMCX
   (user trigconfig2[7:0] for GROUPMODE_BP as before)

- PXlargeSYS_125.bin: version 0x003C_A951  for DB04, DB08 14/125
  unchanged, not updated, do not use with release 3D



FW for in-house release, version 0x3C_01,  9/20/23

- Pixie16DSP./ldr/lst/var: version 3.3B
  unchanged

- PXlarge250.bin: version 0x3C41 for DB04, DB08 14/250
  unchanged

- PXlarge125.bin: version 0x3C51, build 0x0001, for DB04, DB08 14/125
  inverted bit 13 of ADC data
  added "build" number to upper 16 bit of version register for debug compiles. 
    previous value was 0x0000, current value is 0x0001

- PXlargeSYS_250.bin: version 0x003C_A941 for DB04, DB08 14/250
  unchanged

- PXlargeSYS_125.bin: version 0x003C_A951  for DB04, DB08 14/125
  unchanged
  FP veto/sync are outputs for debug
  from no-DSP branch


FW for in-house release, version 0x3C, 9/14/23

- Pixie16DSP./ldr/lst/var: version 3.3B
  unchanged

- PXlarge250.bin: version 0x3C41 for DB04, DB08 14/250
  added option to generate artificial pulses if CCSRC bit 10 = 1. (CCSRC_SIMADC_10 in Reg 2c, see FW spec)

- PXlarge125.bin: version 0x3C51 for DB04, DB08 14/125
  2nd draft of 125 MHz Fippi firmware
  added option to generate artificial pulses if CCSRC bit 10 = 1. (CCSRC_SIMADC_10 in Reg 2c, see FW spec)

- PXlargeSYS_250.bin: version 0x003C_A941 for DB04, DB08 14/250
  hard coded LMK programming for reworked boards (must not use with others!)
  hard coded for 250 MHz DBs until SW can specify DB type during booting
  FP veto/sync are outputs for debug
  from no-DSP branch

- PXlargeSYS_125.bin: version 0x003C_A951  for DB04, DB08 14/125
  hard coded LMK programming for reworked boards (must not use with others!)
  hard coded for 125 MHz DBs until SW can specify DB during booting
  FP veto/sync are outputs for debug
  from no-DSP branch


  Notes: Fippi  version register = [major rev] [minor rev] [DB type] [FW variant]
         System version register = [reserved] reserved] [major rev] [minor rev]   [always A] [product ID = 9] [DBtype] [FW variant]
         the [major rev] always matches, the [minor rev] can differ between the System and Fippi. 
         Fippi version is periodically synchronized with PNXL

         DSP is unchanged, so does not write CCSRC_SIMADC_10 to Fippi Register 2c. SW must write to Fippi to enable this option




FW for in-house release, version 0x3B, 4/21/23

- Pixie16DSP./ldr/lst/var: version 3.3B
  add programming of new CCSRA bits

- PXlarge250.bin: version 0x3A41
  improve DPM full logic (untested)

- PXlargeSYS.bin: version 0x203B_A941
  add backplane beta trigger logic and sync
  hard coded LMK programming for reworked boards (must not use with others!)
  FP veto/sync are outputs for debug
  from no-DSP branch

Temporary parameter mapping
Fippi       DSP
trigconfig0[27:24] = trigconfig[27:24] 
GROUPMODE_SYS = trigconfig1[7:0] 
GROUPMODE_BP  = trigconfig2[7:0] 
MIN_MULTIPLICITY = trigconfig3[7:0] 



  

FW preview r48588, 2/24/2023

- Pixie16DSP./ldr/lst/var: r48587
  add programming of DYNODE_WINDOW  UserIn[7]
  version 0x338 (= DSPrelease)

- PXlarge250.bin: r48588
  add new beta trigger logic
  version 0x3841

- PXlargeSYS.bin: r48509
  unchanged
  version 0x0001A941

Settings same as below, except
GROUPMODE_CH = 6 for channels 6 and 7 (to use the beta gated dynode trigger
DYNODE_WINDOW = time a first dynode pair trigger blanks out subsequent dynode pair triggers (in units of 8ns)




FW preview r48568, 2/21/2023

- Pixie16DSP./ldr/lst/var: r48567
  add programming of VETOMODE and GATE_LENGTH 
  VETO_MODE is UserIn[5] and GATE_LENGTH is UserIn[6]
  version 0x337 (= DSPrelease)

- PXlarge250.bin: r48568
  add beta gating logic
  version 0x3741

- PXlargeSYS.bin: r48509
  unchanged
  version 0x0001A941

  Suggested settings for beta gating with beta L/R in channels 6/7
   GROUPMODE_K70         6                                                                                       
   GROUPMODE_K71         0                                                                                       
   GROUPMODE_K72         0                                                                                       
   GROUPMODE_K73         0                                                                                       
   GATE_LENGTH           100                                                                                     
   VETO_MODE             3                                                                                       
   CCSRA_FTRIGSEL_00          1 1 1 1 1 1 1 1    1 1 1 1 1 1 1 1     1 1 1 1 1 1 1 1     1 1 1 1 1 1 1 1         
   CCSRA_GOOD_02              1 1 1 1 1 1 1 1    1 1 1 1 1 1 1 1     1 1 1 1 1 1 1 1     1 1 1 1 1 1 1 1         
   CCSRA_SYNCDATAACQ_04       1 1 1 1 1 1 1 1    1 1 1 1 1 1 1 1     1 1 1 1 1 1 1 1     1 1 1 1 1 1 1 1         
   CCSRA_POLARITY_05          0 0 0 0 0 0 0 0    0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0         
   CCSRA_VETOENA_06           1 1 1 1 1 1 0 0    0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0         
   CCSRA_TRACEENA_08          1 1 1 1 1 1 1 1    1 1 1 1 1 1 1 1     1 1 1 1 1 1 1 1     1 1 1 1 1 1 1 1         
   CCSRA_QDCENA_09            1 1 1 1 1 1 1 1    1 1 1 1 1 1 1 1     1 1 1 1 1 1 1 1     1 1 1 1 1 1 1 1         
   CCSRA_ESUMSENA_12          1 1 1 1 1 1 1 1    1 1 1 1 1 1 1 1     1 1 1 1 1 1 1 1     1 1 1 1 1 1 1 1         
   CCSRA_PILEUPCTRL_15        0 0 0 0 0 0 0 0    0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0         
   CCSRC_EXTTSENA_05          0 0 0 0 0 0 0 0    0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0         
   TRACE_LENGTH             1.024  1.024  1.024  1.024  1.024  1.024  1.024  1.024      1.024  1.024  1.024  1.02
   TRACE_DELAY              0.200  0.200  0.200  0.200  0.200  0.200  0.200  0.200      0.200  0.200  0.200  0.20
   GROUPMODE_CH             3      3      3      3      3      3      3      3          5      5      5      5   
   FASTTRIG_BACKLEN         1.0    1.0    1.0    1.0    1.0    1.0    1.0    1.0        1.0    1.0    1.0    1.0 
   EXTERN_DELAYLEN          0.4    0.4    0.4    0.4    0.4    0.4    0.008  0.008      0.4    0.4    0.4    0.4


   Suggested settings for beta gating with single beta signal in channels 6 or 7
   same, except 
      VETO_MODE             5                                                                                        
      CCSRA_FTRIGSEL_00          1 1 1 1 1 1 0 0  ...  





FW preview r48530, 2/14/2023

- Pixie16DSP./ldr/lst/var: r48504
  unchanged 
  version 0x335 (= DSPrelease)

- PXlarge250.bin: r48506
  fixed bug in order of coinc pair triggers
  version 0x3641

- PXlargeSYS.bin: r48509
  unchanged
  version 0x0001A941





FW preview r48504, 2/13/2023

- Pixie16DSP./ldr/lst/var: r48504
  debugged programming of Fippi GROUPMODE parameters (from UserIn)
  version 0x335 (= DSPrelease)

- PXlarge250.bin: r48506
  add trigger/coinc logic for dynode > anode option (GROUPMODE_CH=5). Use
  .. GROUPMODE_CH = 3 for channel 0-5       (dynodes)
  .. GROUPMODE_CH = 5 for channel 8-31      (anodes)
  .. GROUPMODE_K7 = 3 for Fippi 0
  .. GROUPMODE_K7 = any for Fippi 1-3
  
  CCSRA_SYNCDATAACQ_04 =1 now enforces common DAQ across all channels of a module (was: of a Fippi)  [not working yet?]
  fixed bug in event readout with QDC sums
  version 0x3541

- PXlargeSYS.bin: r48509
  hard wire FT[0/1/2] in to HIT[6/10/14] to support Fippi trigger modes (GROUPMODE_CH=5) (temp)
  revise clock PLL programming
  add option to reset time stamp counters via SW write to System CSR in addition to backplane SYNC (temp)
  version 0x0001A941

- confirmed no stall in LM run with latest SDK and low LM FIFO threshold 



FW preview r48458, 2/3/2023

- Pixie16DSP./ldr/lst/var: r48461
  add programming of Fippi GROUPMODE parameters (from UserIn)

- PXlarge250.bin: r48458
  add trigger/coinc logic (controlled by GROUPMODE)
  invert signal input
  version 0x3441

- PXlargeSYS.bin: r48460
  hard wire FT[0+4n] in to HIT[1+4n] to support Fippi trigger modes (GROUPMODE_CH=4) 
  version 0xA94F
