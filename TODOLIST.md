<!-- TODOLIST.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 五 11月 30 20:59:18 2018 (+0800)
;; Last-Updated: 四 11月  3 20:59:06 2022 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 12
;; URL: http://wuhongyi.cn -->

# TODO

## 新版本

- 老版本驱动，文件夹 software
- GUI 基于 ROOT，不再更新
- 支持现有所有类型硬件，不支持新的 x2 硬件


- 新版本驱动，安装后生成文件夹 pixiesdk
- GUI 基于 QT
- 支持所有版本硬件


## 测试部分

- offline 添加PSD优化窗口功能(这个存在的难点在于,local trigger 时候记录的波形存在一个流水线延迟,需要重新找真实的触发点)
- cfd 过阈值与没过阈值时，timestamp 对比,流水线延迟有多少?




## 基于网页的控制界面

**采用 MYSQL 数据库**

外部所有设备(真空计\温度计\冷却机),每个采用小程序单独控制,连续采集数据并上传到 MYSQL 数据中心

获取运行时,online 监视数据时时上传到 MYSQL 数据中心


网页分权控制
- admin
	- 拥有最高权限,可以强制重新初始化获取程序
- exp
	- 控制获取的采集,start,stop
- guest
	- 仅仅能监视外部设备状态
	- 监视获取online


一个思路是(比较适合有台服务器,IO比常规电脑强),数据decode转换时候,将数据上传到 MYSQL 数据中心(但是存在的可能问题是数据过大,效率可能较低或者需要及时删除MYSQL不使用数据),这样通过网页即可进行离线分析,参数优化等.

离线参数优化时候,也可不用将所有数据全部decode之后放进MYSQL,需要查看哪一轮数据\哪一个模块数据时候,先判断数据是否被解析放在MYSQL中,如果没有,先解析数据放到MYSQL(类似图形界面的offline的read操作),然后即可进行分析操作.但是该操作只允许在exp帐号使用



## err info

```bash
Build: Plx9054

- PLA: CentOS Linux release 7.6.1810 (Core) 
- KER: 3.10.0-957.12.2.el7.x86_64
- INC: /lib/modules/3.10.0-957.12.2.el7.x86_64/build/include
- CPU: x86_64 (64-bit Little Endian)
- CMP: Gcc
- TYP: Driver
- PLX: 9054
- CFG: Release

make[1]: 进入目录“/usr/src/kernels/3.10.0-957.12.2.el7.x86_64”
arch/x86/Makefile:166: *** CONFIG_RETPOLINE=y, but not supported by the compiler. Compiler update recommended.。 停止。
make[1]: 离开目录“/usr/src/kernels/3.10.0-957.12.2.el7.x86_64”
make: *** [BuildDriver] 错误 2

make[1]: Entering directory '/usr/src/kernels/3.10.0-957.12.2.el7.x86_64'
arch/x86/Makefile:166: *** CONFIG_RETPOLINE=y, but not supported by the compiler. Compiler update recommended.。 Stop.
make[1]: Leaving directory '/usr/src/kernels/3.10.0-957.12.2.el7.x86_64'
make: *** [BuildDriver] Error 2

[wuhongyi@pkudaq Driver]$ su
密码：
[root@pkudaq Driver]# emacs /usr/src/kernels/3.10.0-957.12.2.el7.x86_64/
arch/           firmware/       Kconfig         mm/             security/       virt/
block/          fs/             kernel/         Module.symvers  sound/          vmlinux.id
.config         include/        lib/            net/            System.map      
crypto/         init/           Makefile        samples/        tools/          
drivers/        ipc/            Makefile.qlock  scripts/        usr/            
[root@pkudaq Driver]# emacs /usr/src/kernels/3.10.0-957.12.2.el7.x86_64/arch/x86/Makefile
[root@pkudaq Driver]# exit
exit
[wuhongyi@pkudaq Driver]$ ./builddriver 9054
```

注释掉

```bash
    ifneq ($(RETPOLINE_CFLAGS),)
        KBUILD_CFLAGS += $(RETPOLINE_CFLAGS) -DRETPOLINE
    else
        $(error CONFIG_RETPOLINE=y, but not supported by the compiler. Compiler update recommended.)
    endif
```



警告：teamviewer_15.2.2756.x86_64.rpm: 头V4 RSA/SHA1 Signature, 密钥 ID 0c1289c0: NOKEY
gtk-update-icon-cache: Cache file created successfully.
Error in file "/usr/share/applications/org.gnome.font-viewer.desktop": "font/ttf" is an invalid MIME type ("font" is an unregistered media type)
Error in file "/usr/share/applications/org.gnome.font-viewer.desktop": "font/otf" is an invalid MIME type ("font" is an unregistered media type)



Skipping BTF generation for /home/wuhongyi/PKUXIADAQ/PlxSdk/Driver/Plx9054.ko due to unavailability of vmlinux

Build: Plx9054

- PLA: Ubuntu 20.04.3 LTS
- KER: ver 5.11.0-40-generic 40
- INC: /lib/modules/5.11.0-40-generic/build/include
- CPU: x86_64 (64-bit Little Endian)
- CMP: gcc
- TYP: 9054 Driver
- PKG: PCI/PCIe SDK v8.23
- CFG: Release

  MODPOST /home/wuhongyi/PKUXIADAQ/PlxSdk/Driver/Module.symvers
  LD [M]  /home/wuhongyi/PKUXIADAQ/PlxSdk/Driver/Plx9054.ko
  BTF [M] /home/wuhongyi/PKUXIADAQ/PlxSdk/Driver/Plx9054.ko
Skipping BTF generation for /home/wuhongyi/PKUXIADAQ/PlxSdk/Driver/Plx9054.ko due to unavailability of vmlinux

Driver "Source.Plx9000/Output/Plx9054.ko" built sucessfully










<!-- TODOLIST.md ends here -->
