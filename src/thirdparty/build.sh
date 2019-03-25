#! /usr/bin/env bash


echo ======================================================================
figlet build.sh
echo ======================================================================

# if the build.sh script is invoked by scons
#   changedir to the src/thirdparty directory
if [ $0 == "./src/thirdparty/build.sh" ]; then
	cd ./src/thirdparty/
else
	export CC=/usr/bin/clang
	export CXX=/usr/bin/clang++
fi

# ----------------------

#if [ -f libarchive/libarchive/libarchive.so ]; then
#	exit 0
#fi

# ----------------------

_MSG_ALREADY_CONF="build.sh: already configured."

function is_configured {
	local _RETVAL=false
	for var in "$@"
	do
		if [ $var == "configured" ]; then
			_RETVAL=true
		fi
	done
	echo $_RETVAL
}

DB=$PWD/build.db

if [ -f $DB ]; then
	_ZLIB=$(grep ^ZLIB $DB)
	_ZSTD=$(grep ^ZSTD $DB)
	_LZMA=$(grep ^LZMA $DB)		# LZMA Is included in the XZ directory
	_LIBARCHIVE=$(grep ^LIBARCHIVE $DB)
fi



#          _      _ 
#  _______| |_ __| |
# |_  / __| __/ _` |
#  / /\__ \ || (_| |
# /___|___/\__\__,_|

figlet zstd

cd zstd/build/cmake/

# configure
if [ $(is_configured $_ZSTD) != "true" ]; then
	cmake -DCMAKE_C_FLAGS="-fPIC" \
		-DCMAKE_CXX_FLAGS="-fPIC" \
		-G Ninja \
		.

	echo "ZSTD configured" >> $DB
else
	echo $_MSG_ALREADY_CONF
fi

# build
ninja libzstd_static 

cd ../../..


#      _ _ _     
#  ___| (_) |__  
# |_  / | | '_ \ 
#  / /| | | |_) |
# /___|_|_|_.__/ 

figlet zlib

cd zlib-1.2.11

# configure
if [ $(is_configured $_ZLIB) != "true" ]; then
	cmake -DCMAKE_C_FLAGS="-fPIC" -G Ninja .
	echo "ZLIB configured" >> $DB
else
	echo $_MSG_ALREADY_CONF
fi

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

# configure
# ^^^not needed

# build
make libbz2.a CFLAGS='-fPIC'

cd ..



#  _                     
# | |_____ __ ___   __ _ 
# | |_  / '_ ` _ \ / _` |
# | |/ /| | | | | | (_| |
# |_/___|_| |_| |_|\__,_|

figlet lzma

cd xz-5.2.4

# configure
if [ $(is_configured $_LZMA) != "true" ]; then
	./autogen.sh
	./configure --enable-debug --disable-shared
	echo "LZMA configured" >> $DB
else
	echo $_MSG_ALREADY_CONF
fi

# build
make CFLAGS=-fPIC

cd ..



#  _ _ _                    _     _           
# | (_) |__   __ _ _ __ ___| |__ (_)_   _____ 
# | | | '_ \ / _` | '__/ __| '_ \| \ \ / / _ \
# | | | |_) | (_| | | | (__| | | | |\ V /  __/
# |_|_|_.__/ \__,_|_|  \___|_| |_|_| \_/ \___|

figlet libarchive

D=$(pwd)

cd libarchive

# configure
if [ $(is_configured $_LIBARCHIVE) != "true" ]; then
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
	echo "LIBARCHIVE configured" >> $DB
else
	echo $_MSG_ALREADY_CONF
fi

# build
ninja archive


#  _ _     _ 
# | | | __| |
# | | |/ _` |
# | | | (_| |
# |_|_|\__,_|

figlet ldd
ldd -v libarchive/libarchive.so
cd ..


#   ___ ___  _ __  _   _ 
#  / __/ _ \| '_ \| | | |
# | (_| (_) | |_) | |_| |
#  \___\___/| .__/ \__, |
#           |_|    |___/ 

figlet copy

if [ -z $2 ]; then
	mkdir -p build
	cp libarchive/libarchive/libarchive.so.16 build/libarchive.64.so
else
	if [ $2 == "linux" ]; then
		mkdir -p build
		cp libarchive/libarchive/libarchive.so.16 build/libarchive.64.so
	fi
fi

echo 
echo COPY Library to:  build/libarchive.64.so
echo 
