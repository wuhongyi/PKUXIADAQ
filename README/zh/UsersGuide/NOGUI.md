<!-- NOGUI.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 三 1月 30 19:20:43 2019 (+0800)
;; Last-Updated: 三 1月 30 19:45:50 2019 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 1
;; URL: http://wuhongyi.cn -->

# 非图形交互界面

当开启程序之后，将自动加载固件进行初始化，等待初始化结束后，出现以下界面

![Main Frame](/img/noguimainframe.png)

以下是所有命令的功能，

```
  [q]   退出程序
  [s]   控制获取的开始、结束
  [o]   控制在线共享内存的开启、关闭
  [w]   控制数据写入硬盘的开启、关闭
  [a]   控制自动运行模式的开启、关闭
  [e]   刷新在线监视中的能谱
  [c]   清除屏幕，显示本命令提示
  [h]   输出运行控制参数
```

下图是典型的运行界面信息，可以清晰看到当前的运行模式。

![run mode](/img/noguistartrun.png)

## 自动运行设置

当开启自动运行模式时，将会按照用户所设定的时间自动切换到下一轮运行。

时间参数设置在文件 parset/cfgPixie16.txt

```
# Only use in NOGUI, unit: second
AutoRunModeTimes   180
```


<!-- NOGUI.md ends here -->
