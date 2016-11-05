OBJ = boot

CXX=g++        
CXXFLAGS:=-std=c++11 -g -Wall -lrt -ldl -I. -I../software/app -I../software/sys
LIBS:= ../software/libPixie16App.a  ../software/libPixie16Sys.a $(PLX_SDK_DIR)/PlxApi/Library/PlxApi.a


INCLUDES:=

OBJECTS:=Boot.o

SRCS:=Boot.cc

all: $(OBJ)


$(OBJ): $(SRCS) $(OBJECTS) $(INCLUDES)
	$(CXX) -o  $@  $(OBJECTS) $(CXXFLAGS) $(LIBS)
#$(ROOTLIBS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< 
%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< 



clean: 
	rm -f *.o *~ $(OBJ) 

very-clean:
	rm -f $(OBJ) *.o *~

.PHONY: clean very-clean
#.SILENT:
