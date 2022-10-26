<!-- TODO.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 三 10月 26 09:37:49 2022 (+0800)
;; Last-Updated: 三 10月 26 22:17:17 2022 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 2
;; URL: http://wuhongyi.cn -->

# README

**Pixie16LoadDSPParametersFromFile** and **Pixie16BootModule** accept both binary and JSON based settings files. **Pixie16SaveDSPParametersToFile** will write a JSON file containing only the online modules in the system. The API handles this conversion internally for the user.

No changes are necessary when calling **Pixie16LoadDSPParametersFromFile** and **Pixie16BootModule**. 

Users that use **Pixie16SaveDSPParametersToFile** to access run statistics data should update to use **Pixie16ReadStatisticsFromModule** and the associated support functions.

We now have a helper function (**Pixie16GetStatisticsSize()**) that returns the size for the user. This allows us to make modifications to the statistics without impacts to the user's code. We've found that customers often use the constant RANDOMINDICES_LENGTH to initialize trace containers. The correct constant to use is **MAX_ADC_TRACE_LEN**. We will be replacing many of these defines with helper functions in the future to accommodate changes in size.


- PixieBootCrate(const char* settings_file, const enum PIXIE_BOOT_MODE boot_mode)
- PixieGetWorkerConfiguration(unsigned short mod_num, struct fifo_worker_config* worker_config)
- PixieRegisterFirmware(const unsigned int version, const int revision, const int adc_msps, const int adc_bits, const char* device, const char* path, unsigned short ModNum)
- PixieSetWorkerConfiguration(unsigned short mod_num, struct fifo_worker_config* worker_config)
- PixieReadRunFifoStats(unsigned short mod_num, struct module_fifo_stats* fifo_stats)


- Pixie16GetStatisticsSize
- PixieGetReturnCodeText
- PixieGetHistogramLength
- PixieGetTraceLength
- PixieGetMaxNumBaselines


- IEEEFloating2Decimal 
- Decimal2IEEEFloating 


- Pixie16AcquireADCTrace 
- Pixie16AcquireBaselines 
- Pixie16AdjustOffsets 	
- Pixie16BLcutFinder 	
- Pixie16SetDACs 
- Pixie16TauFinder


- PixieGetModuleInfo(unsigned short mod_num, struct module_config* cfg)
- Pixie16ReadModuleInfo   	ok
- Pixie16BootModule 
- Pixie16EndRun 
- Pixie16ExitSystem
- Pixie16InitSystem     ok
- Pixie16StartHistogramRun 
- Pixie16StartListModeRun 


- Pixie16ReadDataFromExternalFIFO
- Pixie16CheckExternalFIFOStatus 	
- Pixie16CheckRunStatus


- Pixie16ComputeInputCountRate	
- Pixie16ComputeLiveTime
- Pixie16ComputeOutputCountRate
- Pixie16ComputeRealTime
- Pixie16ComputeRawInputCount
- Pixie16ComputeRawOutputCount
- Pixie16ReadHistogramFromModule
- Pixie16ReadStatisticsFromModule


- Pixie16CopyDSPParameters
- Pixie16SaveDSPParametersToFile
- Pixie16LoadDSPParametersFromFile
- Pixie16SaveHistogramToFile 


- Pixie16ReadSglChanADCTrace  	
- Pixie16ReadSglChanBaselines 	
- Pixie16ReadSglChanPar 	
- Pixie16ReadSglModPar 	
- Pixie16WriteSglChanPar 
- Pixie16WriteSglModPar








<!-- TODO.md ends here -->
