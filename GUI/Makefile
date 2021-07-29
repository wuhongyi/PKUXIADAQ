OBJ = pku

CXX=g++ #-std=c++11  
CXXFLAGS:=-fno-stack-protector -g -Wall -lrt -lfftw3 -I. -I../software/app -I../software/sys
ROOTLIBS = $(shell root-config --libs) -lGui
ROOTINC = -I$(shell root-config --cflags) 
CXXFLAGS+=$(ROOTINC)
LIBS:=../software/libPixie16App.a  ../software/libPixie16Sys.a  $(PLX_SDK_DIR)/PlxApi/Library/PlxApi.a



INCLUDES:=Global.hh Detector.hh ExpertMod.hh Table.hh Csra.hh MainFrame.hh HistXDT.hh TriggerFilter.hh Cfd.hh Qdc.hh Decimation.hh LogicTrigger.hh Offline.hh Base.hh Energy.hh Simulation.hh OfflineData.hh CopyPars.hh ReadChanStatus.hh pkuFFTW.hh wuReadData.hh ExpThresholdChange.hh

OBJECTS:=Global.o ExpertMod.o Csra.o Detector.o MainFrame.o main.o Table.o HistXDT.o TriggerFilter.o MainDict.o Cfd.o Qdc.o Decimation.o LogicTrigger.o Offline.o Base.o Energy.o Simulation.o OfflineData.o CopyPars.o ReadChanStatus.o pkuFFTW.o ExpThresholdChange.o

SRCS:=Global.cc ExpertMod.cc Csra.cc Detector.cc MainFrame.cc main.cc Table.cc HistXDT.cc TriggerFilter.cc  MainDict.cc Cfd.cc Qdc.cc Decimation.cc LogicTrigger.cc Offline.cc Base.cc Energy.cc Simulation.cc OfflineData.cc CopyPars.cc ReadChanStatus.cc pkuFFTW.cc ExpThresholdChange.cc

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
