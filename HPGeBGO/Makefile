CXX=g++        
CXXFLAGS:=-std=c++11 -g -Wall -lrt -I. -I../software/app -I../software/sys
ROOTLIBS = $(shell root-config --libs)
ROOTINC = -I$(shell root-config --incdir) 
CXXFLAGS+=$(ROOTINC)
LIBS:=../software/libPixie16App.a  ../software/libPixie16Sys.a  $(PLX_SDK_DIR)/PlxApi/Library/PlxApi.a



INCLUDES:=wuReadData.hh



all: daq tran


daq: daq.o 
	$(CXX) -o  $@  daq.o $(CXXFLAGS) $(LIBS) $(ROOTLIBS)

tran: tran.o
	$(CXX) -o  $@  tran.o $(CXXFLAGS) $(ROOTLIBS) 


%.o: %.cxx 
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< 
%.o: %.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< 



clean: 
	rm -f *.o *~ daq tran 

very-clean:
	rm -f *.o *~ daq tran 

.PHONY: clean very-clean
#.SILENT:
