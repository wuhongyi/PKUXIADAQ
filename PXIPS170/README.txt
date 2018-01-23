PXI Platform Services for Linux/x86 64-bit, Version 17.0
May 2017

-----------------------------------------------------------------------------
This file contains late breaking news about PXI Platform Services and
supersedes any information found in other documentation included in the
distribution. Refer to ni.com/linux for the most recent information about
Linux support at National Instruments.
-----------------------------------------------------------------------------

-----------------
Table of Contents
-----------------
System Requirements
Supported Hardware
Installation
Uninstallation
Device Configuration
Linux Limitations
FAQs
Legal Information

-------------------
System Requirements
-------------------
PXI Platform Services for Linux software for the Intel x86 64-bit
architecture has been tested for the following distributions:
   openSUSE Leap 42.1
   openSUSE Leap 42.2
   Red Hat Enterprise Linux Desktop + Workstation 6.x
   Red Hat Enterprise Linux Desktop + Workstation 7.x
   Scientific Linux 6.x
   CentOS 7.x

For more information about supported Linux versions and distributions supported
by NI, refer to ni.com/linux.

------------------
Supported Hardware
------------------
PXI Platform Services supports NI PXI Express Chassis, PXI Chassis,
Embedded Controllers, and Remote Controllers.

------------
Installation
------------
The installation process compiles a small amount of code to customize
the software to your Linux distribution.  Before running the installer, you
must have the following compiler facilities installed from your Linux
distribution CD or other Linux distribution medium:

* The kernel source files that exactly correspond to the kernel version
  you are running
* The GCC compiler and C++ support
* The make utility

Use the software-install facility of your Linux distribution to verify that the
above-listed packages are present.  You can then begin installation.

Complete the following steps to install PXI Platform Services:

1. Run the INSTALL script from this directory as root. The INSTALL script
ensures that all requirements are met and installs the PXI Platform Services
software. The script uses rpm to install packages on the system.

--------------
Uninstallation
--------------
To uninstall PXI Platform Services, complete the following steps:

1. Run the UNINSTALL script from this directory as root.

--------------------
Device Configuration
--------------------
PXI Platform Services for Linux is configured via the nipxiconfig utility
installed by the installation script into "/usr/bin/nipxiconfig". For a list of
commands and other relevant information about the utility, consult the man
page ("man nipxiconfig").

-----------------
Linux Limitations
-----------------
Some features discussed in the PXI Platform Services documentation and help
are unavailable on Linux:

PXI Platform Services does not provide the Measurement and Automation Explorer
(MAX) graphical configuration utility that is available for other operating
systems. Refer to DEVICE CONFIGURATION in the Installation section for
information about configuring your device in Linux.

----
FAQs
----
Q1: What action needs to be taken if I upgrade/change my kernel?

A1: Some action is required to make the existing PXI Platform Services
    installation work for the new kernel. After upgrading your kernel, run the
    updateNIDrivers utility as root. The utility is located in /usr/local/bin.
    Be sure to have sources for your new kernel properly installed on your
    system before running the utility. The utility rebuilds the driver using
    the new kernel sources and copies it to the appropriate location for the
    new kernel. For the changes to take place, you will be asked to reboot
    your machine after the utility completes.

-----------------
Legal Information
-----------------

----------
COPYRIGHT:
----------
(c) 1998-2017 National Instruments.
All rights reserved.

Under the copyright laws, this publication may not be reproduced or transmitted
in any form, electronic or mechanical, including photocopying, recording,
storing in an information retrieval system, or translating, in whole or in
part, without the prior written consent of National Instruments Corporation.

NI respects the intellectual property of others, and we ask
our users to do the same. NI software is protected by copyright and other
intellectual property laws. Where NI software may be used to reproduce
software or other materials belonging to others, you may use NI software
only to reproduce materials that you may reproduce in accordance with the
terms of any applicable license or other legal restriction.

U.S. Government Restricted Rights
If you are an agency, department, or other entity of the United States
Government ("Government"), the use, duplication, reproduction, release,
modification, disclosure or transfer of the technical data included in this
manual is governed by the Restricted Rights provisions under Federal
Acquisition Regulation 52.227-14 for civilian agencies and Defense Federal
Acquisition Regulation Supplement Section 252.227-7014 and 252.227-7015 for
military agencies.

IVI Foundation Copyright Notice
Content from the IVI specifications reproduced with permission from the IVI
Foundation.

The IVI Foundation and its member companies make no warranty of any kind with
regard to this material, including, but not limited to, the implied warranties
of merchantability and fitness for a particular purpose. The IVI Foundation and
its member companies shall not be liable for errors contained herein or for
incidental or consequential damages in connection with the furnishing,
performance, or use of this material.

-----------
TRADEMARKS:
-----------
Refer to the NI Trademarks and Logo Guidelines at ni.com/trademarks for
information on NI trademarks. Other product and company names
mentioned herein are trademarks or trade names of their respective companies.

--------
PATENTS:
--------
For patents covering the NI products/technology, refer to the
appropriate location: Help>>Patents in your software, the patents.txt file on
your media, or the NI Patent Notice at ni.com/patents.
