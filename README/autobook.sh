#!/bin/bash

rm -r ../docs/*

gitbook build

cp -r   _book/*  ../docs/

rm -r _book
