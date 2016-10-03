CXX=g++        
CXXFLAGS:=-std=c++11 -g -Wall -lrt -ldl -I.
# ROOTLIBS = $(shell root-config --libs) -lGui
# ROOTINC = -I$(shell root-config --incdir)
# CXXFLAGS+=$(ROOTINC)
LIBS:=libPixie16App.a  libPixie16Sys.a  libPlxApi.a



INCLUDES:=Detector.hh Global.hh Manager.hh

OBJECTS:=Detector.o Global.o Manager.o main.o

SRCS:=Detector.cc Global.cc Manager.cc main.cc

all: pku


pku: $(SRCS) $(OBJECTS) $(INCLUDES)
	$(CXX) -o  $@  $(OBJECTS) $(CXXFLAGS) $(LIBS)
#$(ROOTLIBS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< 
%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< 



clean: 
	rm -f *.o *~ pku 

very-clean:
	rm -f pku *.o *~

.PHONY: clean very-clean
#.SILENT:
