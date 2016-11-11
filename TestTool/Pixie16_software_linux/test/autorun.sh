#!/bin/bash

for (( i=1; i<=1200; i++ ))
do
    echo "------ $i ------"
    ./Pixie16Test_RevF $i
    sleep 1
done
