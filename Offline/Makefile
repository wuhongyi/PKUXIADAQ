OBJ = offline
sourcefile = main.cc algorithm.hh algorithm.cc MainFrame.hh MainFrame.cc OfflineDict.cc


ROOTCFLAGS  = $(shell root-config --cflags)
#ROOTINC     = $(shell root-config --incdir)
ROOTLIBS    = $(shell root-config --libs) -lGui

CFLAGS = -Wall -O2 -I$(TARTSYS)/include -L$(TARTSYS)/lib -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64
#-I$(ROOTSYS)/include 
GXX = g++ 

all:$(OBJ)

$(OBJ): $(sourcefile)
	$(GXX) $(CFLAGS) $(ROOTCFLAGS) $(ROOTLIBS) -o $@ $(filter %.cc ,$(sourcefile))

OfflineDict.cc:MainFrame.hh Linkdef.h
	rootcint -f $@ -c $^

clean:
	rm -f *~ *.o $(OBJ) *Dict*
