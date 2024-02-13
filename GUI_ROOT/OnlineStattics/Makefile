### Makefile --- 
## 
## Description: 
## Author: Hongyi Wu(吴鸿毅)
## Email: wuhongyi@qq.com 
## Created: 一 10月  3 12:40:11 2016 (+0800)
## Last-Updated: 四 7月 29 16:40:31 2021 (+0800)
##           By: Hongyi Wu(吴鸿毅)
##     Update #: 10
## URL: http://wuhongyi.cn 

OBJ = online
CXX=g++
CXXFLAGS:=-g -Wall -lpthread -lrt -ldl -I. -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64
ROOTLIBS = $(shell root-config --libs) -lGui
ROOTINC = -I$(shell root-config --cflags)
CXXFLAGS+=$(ROOTINC)

INCLUDES:=Online.hh
OBJECTS:=Online.o OnlineDict.o main.o
SRCS:=Online.cc OnlineDict.cc main.cc

all: $(OBJ)

$(OBJ): $(SRCS) $(OBJECTS) $(INCLUDES)
	$(CXX) -o  $@  $(OBJECTS) $(CXXFLAGS) $(ROOTLIBS)

%.o: %.cxx 
	$(CXX) $(CXXFLAGS) -c $< 
%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< 
OnlineDict.cc: Online.hh Linkdef.hh
	rootcint -f $@ -c $^


clean: 
	rm -f *.o *~ $(OBJ) *Dict*

very-clean:
	rm -f $(OBJ) *.o *~

.PHONY: clean very-clean


######################################################################
### Makefile ends here
