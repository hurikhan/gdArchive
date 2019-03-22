#! /usr/bin/env bash

figlet zstd
cd zstd/build/cmake/
ninja clean
cd ../../..

figlet zlib
cd zlib-1.2.11
ninja clean
cd ..

figlet bzip2
cd bzip2-1.0.6
make clean
cd ..

figlet libarchive
cd libarchive
ninja clean
cd ..
