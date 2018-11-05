<!-- OnlineStattics.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 5月 13 20:23:36 2018 (+0800)
;; Last-Updated: 一 11月  5 17:10:53 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 4
;; URL: http://wuhongyi.cn -->

# Online Stattics

Modify the file **PixieOnline.config** in **OnlineStattics**, where the first line is the original binary file storage path and the second line is the file name. The two-line parameters is file name. There are used to monitor the real-time size and disk usage of each file.


Open the online monitoring main interface by executing the following command:  
```bash
./online
```

Check the binary file path and file name for any problems. If there is no problem, click the button **Complete**, then click **RunStart** to enable online monitoring, and online monitoring will be refreshed every 3 seconds. The trigger rate of each channel and the actual event output rate of each channel can be monitored in real time.



The monitoring interface is as follows:  

![OnlineMainFrame](/img/OnlineMainFrame.png)




<!-- OnlineStattics.md ends here -->
