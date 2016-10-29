### Makefile --- 
## 
## Description: 
## Author: Hongyi Wu(吴鸿毅)
## Email: wuhongyi@qq.com 
## Created: 日 10月  2 19:40:07 2016 (+0800)
## Last-Updated: 日 10月  2 20:30:08 2016 (+0800)
##           By: Hongyi Wu(吴鸿毅)
##     Update #: 2
## URL: http://wuhongyi.cn 

OBJ = decode
sourcefile = main.cc decoder.hh decoder.cc r2root.hh r2root.cc

ROOTCFLAGS  = $(shell root-config --cflags)
ROOTLIBS    = $(shell root-config --libs)

CFLAGS = -g -Wall -O2 -I$(TARTSYS)/include -L$(TARTSYS)/lib -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64

GXX = g++ 

all:$(OBJ)

$(OBJ): $(sourcefile)
	$(GXX) $(CFLAGS) $(ROOTCFLAGS) $(ROOTLIBS) -o $@ $(filter %.cc ,$(sourcefile))

clean:
	rm -f *~ *.o $(OBJ)


######################################################################
### Makefile ends here
