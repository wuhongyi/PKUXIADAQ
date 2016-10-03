CXX=g++        
CXXFLAGS:=-std=c++11 -g -Wall -lrt -I.
ROOTLIBS = $(shell root-config --libs) -lGui
ROOTINC = -I$(shell root-config --incdir)
CXXFLAGS+=$(ROOTINC)
LIBS:=libPixie16App.a  libPixie16Sys.a  libPlxApi.a



INCLUDES:=Global.h AnalogSignal.h Detector.h ExpertMod.h MaxEvent.h Table.h Baseline.h displayEdit.h Histogram.h PulseShape.h Tau.h Csra.h EnergyFilter.h Main.h ScopedT.h TriggerFilter.h Popup.h Cfd.h Qdc.h LogicTrigger.h FPOutputs.h Offline.h

OBJECTS:=Global.o AnalogSignal.o ExpertMod.o MaxEvent.o Baseline.o Histogram.o Csra.o Detector.o displayEdit.o EnergyFilter.o Main.o nscope.o PulseShape.o ScopedT.o Table.o Tau.o TriggerFilter.o Popup.o MainDict.o Cfd.o Qdc.o LogicTrigger.o FPOutputs.o Offline.o

SRCS:=Global.cpp AnalogSignal.cpp ExpertMod.cpp MaxEvent.cpp Baseline.cpp Histogram.cpp Csra.cpp Detector.cpp displayEdit.cpp EnergyFilter.cpp Main.cpp nscope.cpp PulseShape.cpp ScopedT.cpp Table.cpp Tau.cpp TriggerFilter.cpp Popup.cpp  MainDict.cpp Cfd.cpp Qdc.cpp LogicTrigger.cpp FPOutputs.cpp Offline.cpp

all: nscope


nscope: $(SRCS) $(OBJECTS) $(INCLUDES)
	$(CXX) -o  $@  $(OBJECTS) $(CXXFLAGS) $(LIBS) $(ROOTLIBS)

%.o: %.cxx 
	$(CXX) $(CXXFLAGS) -c $< 
%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< 
MainDict.cpp:Main.h MainLinkdef.h
	rootcint -f $@ -c $^


clean: 
	rm -f *.o *~ nscope *Dict*

very-clean:
	rm -f nscope *.o *~

.PHONY: clean very-clean
#.SILENT:
