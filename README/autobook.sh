#!/bin/bash

rm -rf ../docs/*

gitbook build

cp -r _book/*  ../docs/

rm -r _book

cp en/README.md ..
cp zh/README.md ../README_zh.md

gitbook pdf . ../README.pdf
