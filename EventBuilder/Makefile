OBJ = event
sourcefile = main.cc event.hh event.cc LinkDict.cc UserDefine.hh

ROOTCFLAGS  = $(shell root-config --cflags)
ROOTLIBS    = $(shell root-config --libs) -lSpectrum

CFLAGS = -g -Wall -O2 -D_LARGEFILE_SOURCE

GXX = g++ 

all:$(OBJ)

LinkDict.cc : event.hh Linkdef.h
	rootcint -f LinkDict.cc -c $^		

$(OBJ): $(sourcefile)
	$(GXX) $(CFLAGS) $(ROOTCFLAGS) $(ROOTLIBS) -o $@ $(filter %.cc ,$(sourcefile))
clean:
	rm -f *~ *.o $(OBJ) *Dict*
