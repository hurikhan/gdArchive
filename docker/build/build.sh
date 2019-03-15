#!/usr/bin/env fish
cd libarchive

figlet git pull
git pull

figlet cmake
cmake .

figlet make
make

figlet test
make test
