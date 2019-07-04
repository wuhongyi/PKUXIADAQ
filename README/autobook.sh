#!/bin/bash

rm -rf ../docs/doctrees/
rm -rf ../docs/en/*
rm -rf ../docs/zh/*

cp index.html ../docs/
touch ../docs/.nojekyll

cd en/
make clean
make html
mv build/doctrees/ ../../docs/
cp -r build/html/* ../../docs/en/
make clean
cd ..

cd zh/
make clean
make html
cp -r build/html/* ../../docs/zh/
make clean
cd ../
