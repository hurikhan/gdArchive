#! /usr/bin/env bash

export CC=/usr/bin/clang
export CXX=/usr/bin/clang++

#          _      _ 
#  _______| |_ __| |
# |_  / __| __/ _` |
#  / /\__ \ || (_| |
# /___|___/\__\__,_|

figlet zstd

cd zstd/build/cmake/

cmake -DCMAKE_C_FLAGS="-fPIC" \
	-DCMAKE_CXX_FLAGS="-fPIC" \
	-G Ninja \
	.

ninja libzstd_static 

cd ../../..



#      _ _ _     
#  ___| (_) |__  
# |_  / | | '_ \ 
#  / /| | | |_) |
# /___|_|_|_.__/ 

figlet zlib

cd zlib-1.2.11

cmake -DCMAKE_C_FLAGS="-fPIC" -G Ninja .
ninja zlibstatic

cd ..

#  _        _      ____  
# | |__ ___(_)_ __|___ \ 
# | '_ \_  / | '_ \ __) |
# | |_) / /| | |_) / __/ 
# |_.__/___|_| .__/_____|
#            |_|  

figlet bzip2

cd bzip2-1.0.6

make libbz2.a CFLAGS='-fPIC'

cd ..



#  _                     
# | |_____ __ ___   __ _ 
# | |_  / '_ ` _ \ / _` |
# | |/ /| | | | | | (_| |
# |_/___|_| |_| |_|\__,_|

figlet lzma

cd xz-5.2.4

./autogen.sh
./configure --enable-debug --disable-shared
make CFLAGS=-fPIC

cd ..



#  _ _ _                    _     _           
# | (_) |__   __ _ _ __ ___| |__ (_)_   _____ 
# | | | '_ \ / _` | '__/ __| '_ \| \ \ / / _ \
# | | | |_) | (_| | | | (__| | | | |\ V /  __/
# |_|_|_.__/ \__,_|_|  \___|_| |_|_| \_/ \___|

figlet libarchive

D=$(pwd)
echo $D

cd libarchive

# cmake	-DZSTD_INCLUDE_DIR="$PWD/zstd/lib/" \
# 	-DZSTD_LIBRARY="$PWD/zstd/build/cmake/lib/libzstd.a" \
# 	-DZLIB_INCLUDE_DIR="$PWD/zlib-1.2.11/" \
# 	-DZLIB_LIBRARY="$PWD/zlib-1.2.11/libz.a" \
# 	-DBZIP2_INCLUDE_DIR="$PWD/bzip2-1.0.6/" \
# 	-DBZIP2_LIBRARY_DEBUG="$PWD/bzip2-1.0.6/libbz2.a" \
# 	-DBZIP2_LIBRARY_RELEASE="$PWD/bzip2-1.0.6/libbz2.a" \
# 	-DENABLE_OPENSSL=false \
# 	-G Ninja libarchive

echo $D

cmake	-DZSTD_INCLUDE_DIR="$D/zstd/lib/" \
 	-DZSTD_LIBRARY="$D/zstd/build/cmake/lib/libzstd.a" \
 	-DZLIB_INCLUDE_DIR="$D/zlib-1.2.11/" \
 	-DZLIB_LIBRARY_DEBUG="$D/zlib-1.2.11/libz.a" \
 	-DZLIB_LIBRARY_RELEASE="$D/zlib-1.2.11/libz.a" \
 	-DBZIP2_INCLUDE_DIR="$D/bzip2-1.0.6/" \
 	-DBZIP2_LIBRARY_DEBUG="$D/bzip2-1.0.6/libbz2.a" \
 	-DBZIP2_LIBRARY_RELEASE="$D/bzip2-1.0.6/libbz2.a" \
	-DLIBLZMA_INCLUDE_DIR="$D/xz-5.2.4/src/liblzma/api/" \
	-DLIBLZMA_LIBRARY="$D/xz-5.2.4/src/liblzma/.libs/liblzma.a" \
 	-DENABLE_OPENSSL=false \
 	-G Ninja .

ninja archive

figlet ldd
ldd -v libarchive/libarchive.so
cd ..
