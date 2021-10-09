# Installation Instructions

These instructions primarily focus on the PixieSDK. We encourage all users to migrate to the latest
version of the library. For convenience, we provide a backward compatible Pixie16Api library.

We will not be distributing any precompiled binaries for the time being. This is due to the extreme
variability of the compiler / architecture options for Windows.

There are a number of [CMake Options](cmake-options.md) available when generating the build system
files. The defaults will always build the PixieSDK and the Pixie16Api compatibility library.

## Prerequisites

You've already

1. installed the Broadcom PCI/PCIe SDK 8.00 or newer,
2. exported the `PLX_SDK_DIR` in your environment,
3. installed CMake 3.10+,
4. the cmake application is available in your path, and
5. installed a compiler that supports C++ 14 standard.

## Windows 10

At the moment we can only compile the 32-bit application on Windows. This is due to an issue
determining the appropriate system architecture, compiler architecture, and variability in
Broadcom's installation process.

### Instructions

1. Clone or download the project and put the `pixie_sdk` directory in your `Downloads` folder.
2. Open an
   [x64_x86 Cross Tools Visual Studio Developer Command Prompt](https://docs.microsoft.com/en-us/visualstudio/ide/reference/command-prompt-powershell?view=vs-2019)
3. Navigate to the source folder
   ```shell
   C:\Program Files (x86)\Microsoft Visual Studio\2019\Community>cd %USERPROFILE%\Downloads\pixie_sdk
   ```
4. Create and enter the build directory
   ```shell
   C:Users\username\Downloads\pixie_sdk>mkdir build && cd build
   ```
6. Generate the nmake build scripts
   ```shell
   C:Users\username\Downloads\pixie_sdk\build>cmake.exe -G "CodeBlocks - NMake Makefiles" ../
   ```
7. Build the project
   ```shell
   C:Users\username\Downloads\pixie_sdk\build>nmake
   ```
8. **Optional** Install the project (requires administrative command prompt)
   ```shell
   C:Users\username\Downloads\pixie_sdk\build>nmake install
   ```

## Linux

### Instructions

These installation instructions assume that you're using git to obtain a copy of the project. If
you're starting with a zip folder from a release then skip to Step 2.

1. Clone the project from [https://github.com/xiallc/pixie_sdk](https://github.com/xiallc/pixie_sdk)
   ```shell script
   [user@localhost ~]$ git clone https://github.com/xiallc/pixie_sdk.git
   ```
2. Navigate to the cloned project
   ```shell script
   [user@localhost ~]$ cd pixie_sdk
   ```
3. Configure and build the project
   ```shell script
   [user@localhost pixie_sdk]$ mkdir build; cd build; cmake ../
   ```
4. Compile the project
   ```shell
   [user@localhost pixie_sdk]$ make
   ```
5. (**Optional**) Install the software system-wide. Defaults to `/usr/local/xia/PixieSDK`.
   ```shell script
   [user@localhost pixie_sdk]$ sudo make install
   ```
6. (**Optional**) Add the `XIA_PIXIE_SDK` environment variable (assumes bash)
   ```shell
   [user@localhost pixie_sdk]$ echo -e "export XIA_PIXIE_SDK=/usr/local/xia/PixieSDK\n" | sudo tee -a /etc/profile.d/xia_pixie_sdk.sh
   ```