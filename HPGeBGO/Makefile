OBJ = pku

CXX=g++        
CXXFLAGS:=-std=c++11 -g -Wall -lrt -I. -I../software/app -I../software/sys
ROOTLIBS = $(shell root-config --libs)
ROOTINC = -I$(shell root-config --incdir) 
CXXFLAGS+=$(ROOTINC)
LIBS:=../software/libPixie16App.a  ../software/libPixie16Sys.a  $(PLX_SDK_DIR)/PlxApi/Library/PlxApi.a



INCLUDES:=wuReadData.hh

OBJECTS:=main.o

SRCS:=main.cc

all: $(OBJ)


$(OBJ): $(SRCS) $(OBJECTS) $(INCLUDES)
	$(CXX) -o  $@  $(OBJECTS) $(CXXFLAGS) $(LIBS) $(ROOTLIBS)

%.o: %.cxx 
	$(CXX) $(CXXFLAGS) -c $< 
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< 



clean: 
	rm -f *.o *~ $(OBJ) 

very-clean:
	rm -f $(OBJ) *.o *~

.PHONY: clean very-clean
#.SILENT:
