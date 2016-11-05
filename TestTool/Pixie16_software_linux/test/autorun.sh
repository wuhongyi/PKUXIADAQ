#!/bin/bash

for (( i=1; i<=2000; i++ ))
do
    echo "------ $i ------"
    ./Pixie16Test_RevF $i
    sleep 1
done
