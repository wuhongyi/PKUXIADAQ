OBJ = gddaq

CXX=g++ #-std=c++11
CXXFLAGS:= -g -Wall -I. -I../software/app -I../software/sys
LIBS:= ../software/libPixie16App.a  ../software/libPixie16Sys.a $(PLX_SDK_DIR)/PlxApi/Library/PlxApi.a  -lrt -ldl -lm -lpthread -lcrypto



INCLUDES:=Detector.hh Global.hh Manager.hh wuReadData.hh

OBJECTS:=Detector.o Global.o Manager.o main.o

SRCS:=Detector.cc Global.cc Manager.cc main.cc

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
