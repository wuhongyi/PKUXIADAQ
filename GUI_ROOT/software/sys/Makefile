CXX=gcc
CXXFLAGS:=-g -Wall -I../inc -DPLX_LITTLE_ENDIAN -DPCI_CODE -DPLX_LINUX

OBJSYS:=communication.o pixie16sys.o tools.o i2cm24c64.o pcf8574.o
INCSYS:=pixie16sys_common.h pixie16sys_defs.h pixie16sys_globals.h pixie16sys_export.h ../inc/PciRegs.h  ../inc/PexApi.h  ../inc/PlxDefCk.h  ../inc/PlxIoctl.h  ../inc/Plx_sysdep.h  ../inc/PciTypes.h  ../inc/PlxApi.h  ../inc/Plx.h  ../inc/PlxStat.h  ../inc/PlxTypes.h
#../inc/def21160.h ../inc/PciRegs.h ../inc/PciTypes.h ../inc/PexApi.h ../inc/Plx.h ../inc/Plx_sysdep.h ../inc/PlxApi.h ../inc/PlxDefCk.h ../inc/PlxIoctl.h ../inc/PlxStat.h ../inc/PlxTypes.h ../inc/Reg9054.h


all: libPixie16Sys.a

libPixie16Sys.a: $(OBJSYS) $(INCSYS)
	ar -rc  $@  $(OBJSYS)
	mv *.a ../
	mv *.o ../

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< 

clean: 
	rm -f ../*.o *~ ../libPixie16Sys.a


.PHONY: clean
.SILENT:
