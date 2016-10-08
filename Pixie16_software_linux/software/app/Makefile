CXX=gcc
CXXFLAGS:=-g -Wall -I../inc -I../sys -DPLX_LITTLE_ENDIAN -DPCI_CODE -DPLX_LINUX

OBJAPP:=pixie16app.o utilities.o
INCAPP:=pixie16app_common.h pixie16app_defs.h pixie16app_export.h pixie16app_globals.h xia_common.h

all: libPixie16App.a

libPixie16App.a: $(OBJAPP) $(INCAPP)
	ar -rc  $@  $(OBJAPP)
	mv *.a ../
	mv *.o ../
%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< 

clean: 
	rm -f ../*.o *~ ../libPixie16App.a


.PHONY: clean
#.SILENT:
