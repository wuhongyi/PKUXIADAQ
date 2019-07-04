#!/bin/bash

module load texlive/2019

rm -rf ../docs/doctrees/
rm -rf ../docs/en/*
rm -rf ../docs/zh/*

cp index.html ../docs/
touch ../docs/.nojekyll

cd en/
make clean
make html
make latexpdf
mv build/doctrees/ ../../docs/
cp -r build/html/* ../../docs/en/
cp build/latex/pkuxiadaq.pdf ../../README_en.pdf
make clean
cd ..

cd zh/
make clean
make html
make latexpdf
cp -r build/html/* ../../docs/zh/
cp build/latex/pkuxiadaq.pdf ../../README_zh.pdf
make clean
cd ../
