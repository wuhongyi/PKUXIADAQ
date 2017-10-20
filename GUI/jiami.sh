#!/bin/bash

EncryptionMode='-aes-256-ecb'
Password='pkudaq'
suffix='.pku'

rm -rf ../gui
mkdir ../gui
cp *.jpg pxisys.ini *.hh *.h ../gui

CFLAGS='-static' shc -e 01/12/2017 -m "The code is now out of date. Please contact Hongyi Wu(wuhongyi@qq.com) to get the latest code."   -r -f makefile.sh
cp makefile.sh.x ../gui
rm makefile.sh.x makefile.sh.x.c

#将所有*
for file in $(ls *.cc)
do
    echo $file
    openssl enc $EncryptionMode -e -k $Password  -in $file -out ../gui/$file$suffix
done
