OBJ = pku

CXX=g++        
CXXFLAGS:=-std=c++11 -g -Wall -lrt -I. -I../software/app -I../software/sys
ROOTLIBS = $(shell root-config --libs) -lGui
ROOTINC = -I$(shell root-config --incdir) 
CXXFLAGS+=$(ROOTINC)
LIBS:=../software/libPixie16App.a  ../software/libPixie16Sys.a  $(PLX_SDK_DIR)/PlxApi/Library/PlxApi.a



INCLUDES:=Global.hh AnalogSignal.hh Detector.hh ExpertMod.hh Table.hh Baseline.hh displayEdit.hh Histogram.hh PulseShape.hh Tau.hh Csra.hh EnergyFilter.hh MainFrame.hh ScopedT.hh TriggerFilter.hh Cfd.hh Qdc.hh LogicTrigger.hh FPOutputs.hh Offline.hh MultiplicityMask.hh

OBJECTS:=Global.o AnalogSignal.o ExpertMod.o Baseline.o Histogram.o Csra.o Detector.o displayEdit.o EnergyFilter.o MainFrame.o main.o PulseShape.o ScopedT.o Table.o Tau.o TriggerFilter.o MainDict.o Cfd.o Qdc.o LogicTrigger.o FPOutputs.o Offline.o MultiplicityMask.o

SRCS:=Global.cc AnalogSignal.cc ExpertMod.cc Baseline.cc Histogram.cc Csra.cc Detector.cc displayEdit.cc EnergyFilter.cc MainFrame.cc main.cc PulseShape.cc ScopedT.cc Table.cc Tau.cc TriggerFilter.cc  MainDict.cc Cfd.cc Qdc.cc LogicTrigger.cc FPOutputs.cc Offline.cc MultiplicityMask.cc

all: $(OBJ)


$(OBJ): $(SRCS) $(OBJECTS) $(INCLUDES)
	$(CXX) -o  $@  $(OBJECTS) $(CXXFLAGS) $(LIBS) $(ROOTLIBS)

%.o: %.cxx 
	$(CXX) $(CXXFLAGS) -c $< 
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< 
MainDict.cc:MainFrame.hh MainLinkdef.h
	rootcint -f $@ -c $^


clean: 
	rm -f *.o *~ $(OBJ) *Dict*

very-clean:
	rm -f $(OBJ) *.o *~

.PHONY: clean very-clean
#.SILENT:
