OBJ = algorithm
sourcefile = main.cc algorithm.hh algorithm.cc


ROOTCFLAGS  = $(shell root-config --cflags)
#ROOTINC     = $(shell root-config --incdir)
ROOTLIBS    = $(shell root-config --libs)

CFLAGS = -Wall -O2 -I$(TARTSYS)/include -L$(TARTSYS)/lib -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64
#-I$(ROOTSYS)/include 
GXX = g++ 

all:$(OBJ)

$(OBJ): $(sourcefile)
	$(GXX) $(CFLAGS) $(ROOTCFLAGS) $(ROOTLIBS) -o $@ $(filter %.cc ,$(sourcefile))

clean:
	rm -f *~ *.o $(OBJ)
