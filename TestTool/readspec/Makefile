### Makefile --- 
## 
## Description: 
## Author: Hongyi Wu(吴鸿毅)
## Email: wuhongyi@qq.com 
## Created: 日 10月  2 19:40:07 2016 (+0800)
## Last-Updated: 四 1月 31 19:54:38 2019 (+0800)
##           By: Hongyi Wu(吴鸿毅)
##     Update #: 8
## URL: http://wuhongyi.cn 

OBJ = readspec
sourcefile = main.cc readfile.hh readfile.cc UserDefine.hh

ROOTCFLAGS  = $(shell root-config --cflags)
ROOTLIBS    = $(shell root-config --libs)

CFLAGS = -g -Wall -O2 -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64

GXX = g++ 

all:$(OBJ)

$(OBJ): $(sourcefile)
	$(GXX) $(CFLAGS) $(ROOTCFLAGS) $(ROOTLIBS) -o $@ $(filter %.cc ,$(sourcefile))

clean:
	rm -f *~ *.o  $(OBJ)


######################################################################
### Makefile ends here
