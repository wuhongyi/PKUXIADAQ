#!/bin/bash

for (( i=101; i<=1000; i++ ))
do
    echo "------ $i ------"
    ./Pixie16Test_RevF $i
    sleep 1
done
