OBJS = httponline
sourcefile = main.cc UserDefine.hh decoding.hh decoding.cc

ROOTCFLAGS  = $(shell root-config --cflags)
ROOTLIBS    = $(shell root-config --libs) -lGui -lRHTTP

CFLAGS = -Wall -O2 -I$(ROOTSYS)/include -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -lrt

GXX = g++ 

all: $(OBJS)

$(OBJS): $(sourcefile)
	$(GXX) $(CFLAGS) $(ROOTCFLAGS) $(ROOTLIBS) -o $@ $(filter %.cc ,$(sourcefile))

clean:
	rm -f *~ *.o $(OBJS) 
