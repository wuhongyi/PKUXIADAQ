#!/bin/bash

OBJ=pku

rm -f *.o *~ $OBJ *Dict*


CXX=g++

trap "rm -f *.cc; rm -f *.o;" 1 2 3 8 9 14 15

ROOTCFLAGS=$(root-config --cflags)
ROOTLIBS=$(root-config --libs)  
CXXFLAGS='-std=c++11 -g -Wall -lrt -I. -I../software/app -I../software/sys' 
LIBS='../software/libPixie16App.a  ../software/libPixie16Sys.a'


EncryptionMode='-aes-256-ecb'
Password='pkudaq'

rootcint -f MainDict.cc -c MainFrame.hh MainLinkdef.h


for file in $(ls *.cc.pku)
do
    echo $file
    name=`expr $file | sed 's/\.pku//g'` #去除.pku
    openssl enc $EncryptionMode -d -k $Password -in $file -out $name
    $CXX $ROOTCFLAGS $CXXFLAGS -c $name
    rm $name
done

$CXX $ROOTCFLAGS $CXXFLAGS -c MainDict.cc

OBJECTS='Global.o ExpertMod.o Csra.o Detector.o MainFrame.o main.o Table.o HistXDT.o TriggerFilter.o MainDict.o Cfd.o Qdc.o LogicTrigger.o Offline.o Base.o Energy.o Simulation.o OfflineData.o CopyPars.o ReadChanStatus.o'

$CXX $OBJECTS $CXXFLAGS $ROOTLIBS $ROOTCFLAGS $LIBS $PLX_SDK_DIR/PlxApi/Library/PlxApi.a -lGui  -o $OBJ

fileo=$(ls *.o)
rm $fileo *.cc
