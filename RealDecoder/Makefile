OBJS = httponline
sourcefile = main.cc UserDefine.hh decoding.hh decoding.cc

ROOTCFLAGS  = $(shell root-config --cflags)
ROOTLIBS    = $(shell root-config --libs) -lGui -lRHTTP -lrt

CFLAGS = -w -O2 -I$(ROOTSYS)/include 

GXX = g++ 

all: $(OBJS)

$(OBJS): $(sourcefile)
	$(GXX) $(CFLAGS) $(ROOTCFLAGS) -o $@ $(filter %.cc ,$(sourcefile)) $(ROOTLIBS)

clean:
	rm -f *~ *.o $(OBJS) 
