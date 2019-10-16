#!/bin/bash

for (( i=472; i<=549; i++ ))
do
    echo "i=$i"
    ./timesort $i
    sleep 1
done
