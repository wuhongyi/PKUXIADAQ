
all: pixie16sys.a pixie16app.a

SRC:=sys/communication.c  sys/i2cm24c64.c sys/pixie16sys.c  sys/tools.c sys/pixie16sys_common.h  sys/pixie16sys_defs.h  sys/pixie16sys_export.h  sys/pixie16sys_globals.h app/pixie16app_common.h  app/pixie16app_export.h   app/xia_common.h app/pixie16app_defs.h    app/pixie16app_globals.h app/pixie16app.c  app/utilities.c  inc/PciRegs.h  inc/PciTypes.h  inc/PlxApi.h  inc/PexApi.h  inc/Plx.h  inc/Plx_sysdep.h  inc/PlxApi.h  inc/PlxDefCk.h  inc/PlxIoctl.h  inc/PlxStat.h  inc/PlxTypes.h  inc/Reg9054.h

pixie16sys.a: $(SRC)
	cd sys; make
pixie16app.a: $(SRC)
	cd app; make

#%.o: %.c
#	$(CXX) $(CXXFLAGS) -c $< 

clean: 
	rm -f *.o *~ pixie16sys.a pixie16app.a
	cd sys; make clean
	cd app; make clean


.PHONY: clean
#.SILENT:
