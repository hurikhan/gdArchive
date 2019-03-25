#! /usr/bin/env bash

# Filename: src/thirdparty/clean.sh 

_GIT_CLEAN_PARAM=-dfx

_ERR_CANNOT_FIND_CLEAN_SH=1
_ERR_CANNOT_CHANGE_TO_DIR=2
_ERR_CANNOT_IDENTIFY_CLEAN_SH=3

echo ======================================================================
figlet clean.sh
echo ======================================================================

if [ ! -f clean.sh ]; then
	echo "clean.sh: cannot find clean.sh in the current directory!"
	exit $_ERR_CANNOT_FIND_CLEAN_SH
fi

if [ $1 ]; then
	cd $1

	if [ $? != 0 ]; then
		echo "clean.sh: change to the directory given by the first parameter!"
		exit $_ERR_CANNOT_CHANGE_TO_DIR
	fi
fi

grep "^# Filename: src/thirdparty/clean.sh" clean.sh > /dev/null
if [ $? != 0 ]; then
	echo "clean.sh: not able to identify correct clean.sh!"
	exit $_ERR_CANNOT_IDENTIFY_CLEAN_SH
fi	

SRCS=(zstd:ninja zlib-1.2.11:ninja bzip2-1.0.6:make xz-5.2.4:make libarchive:ninja)

for SRC in ${SRCS[*]}
do
	_ARR=($(echo $SRC | tr ":" " "))
	_DIR=${_ARR[0]}
	_CMD=${_ARR[1]}

	figlet $_DIR
	cd $_DIR
	echo DIR: $PWD
	$_CMD clean
	cd ..
done

rm build/libarchive.so.16

exit 0
