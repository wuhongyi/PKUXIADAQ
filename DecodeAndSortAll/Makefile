### Makefile --- 
## 
## Description: 
## Author: Hongyi Wu(吴鸿毅)
## Email: wuhongyi@qq.com 
## Created: 日 10月  2 19:40:07 2016 (+0800)
## Last-Updated: 五 8月 11 17:38:12 2023 (+0800)
##           By: Hongyi Wu(吴鸿毅)
##     Update #: 11
## URL: http://wuhongyi.cn 

OBJ = decodeandsort
sourcefile = main.cc decoder.hh decoder.cc r2root.hh r2root.cc UserDefine.hh

ROOTCFLAGS  = $(shell root-config --cflags)
ROOTLIBS    = $(shell root-config --libs)

CFLAGS = -g -w -O2

GXX = g++ 

all:$(OBJ)

$(OBJ): $(sourcefile)
	$(GXX) $(CFLAGS) $(ROOTCFLAGS) -o $@ $(filter %.cc ,$(sourcefile)) $(ROOTLIBS)

clean:
	rm -f *~ *.o *.txt *.root $(OBJ)


######################################################################
### Makefile ends here
