#!/bin/bash

# 4060-4332
for (( i=4100; i<=4149; i++ ))
do
    if [ "$i" -eq 4105 ] || [ "$i" -eq 4114 ] || [ "$i" -eq 4136 ] || [ "$i" -eq 4194 ] || [ "$i" -eq 4209 ] || [ "$i" -eq 4213 ] || [ "$i" -eq 4214 ] || [ "$i" -eq 4215 ] || [ "$i" -eq 4221 ] || [ "$i" -eq 4222 ] || [ "$i" -eq 4223 ] || [ "$i" -eq 4224 ] || [ "$i" -eq 4225 ] || [ "$i" -eq 4226 ] || [ "$i" -eq 4241 ] || [ "$i" -eq 4255 ] || [ "$i" -eq 4282 ] || [ "$i" -eq 4291 ] || [ "$i" -eq 4292 ] || [ "$i" -eq 4293 ] || [ "$i" -eq 4313 ] || [ "$i" -eq 4314 ] ;
    then
	continue
    fi
    echo "i=$i"
    ./decode $i $i
    sleep 1
done


# for (( i=4060; i<=4104; i++ ))
# do
#     echo "i=$i"
#     ./decode $i $i
#     sleep 1
# done

# for (( i=4106; i<=4113; i++ ))
# do
#     echo "i=$i"
#     ./decode $i $i
#     sleep 1
# done


# for (( i=4116; i<=4135; i++ ))
# do
#     echo "i=$i"
#     ./decode $i $i
#     sleep 1
# done

# for (( i=4137; i<=4193; i++ ))
# do
#     echo "i=$i"
#     ./decode $i $i
#     sleep 1
# done

# for (( i=4195; i<=4208; i++ ))
# do
#     echo "i=$i"
#     ./decode $i $i
#     sleep 1
# done


# for (( i=4227; i<=4254; i++ ))
# do
#     echo "i=$i"
#     ./decode $i $i
#     sleep 1
# done


# for (( i=4256; i<=4281; i++ ))
# do
#     echo "i=$i"
#     ./decode $i $i
#     sleep 1
# done

# for (( i=4283; i<=4291; i++ ))
# do
#     echo "i=$i"
#     ./decode $i $i
#     sleep 1
# done


# for (( i=4294; i<=4317; i++ ))
# do
#     echo "i=$i"
#     ./decode $i $i
#     sleep 1
# done
