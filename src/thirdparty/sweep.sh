#! /usr/bin/env bash

# Filename: src/thirdparty/sweep.sh 

_GIT_CLEAN_PARAM=-dfx

_ERR_CANNOT_FIND_CLEAN_SH=1
_ERR_CANNOT_CHANGE_TO_DIR=2
_ERR_CANNOT_IDENTIFY_CLEAN_SH=3

echo ======================================================================
figlet sweep.sh
echo ======================================================================

if [ $1 ]; then
	cd $1

	if [ $? != 0 ]; then
		echo "sweep.sh: change to the directory given by the first parameter!"
		exit $_ERR_CANNOT_CHANGE_TO_DIR
	fi
fi

if [ ! -f sweep.sh ]; then
	echo "sweep.sh: cannot find sweep.sh in the current directory!"
	exit $_ERR_CANNOT_FIND_CLEAN_SH
fi


grep "^# Filename: src/thirdparty/sweep.sh" sweep.sh > /dev/null
if [ $? != 0 ]; then
	echo "sweep.sh: not able to identify correct sweep.sh!"
	exit $_ERR_CANNOT_IDENTIFY_CLEAN_SH
fi	

dirs=(zstd zlib-1.2.11 bzip2-1.0.6 xz-5.2.4 libarchive)

for d in ${dirs[*]}
do
	figlet $d
	cd $d
	echo DIR: $PWD
	git clean $_GIT_CLEAN_PARAM
	git checkout .
	cd ..
done

rm -f build.db
rm -Rf build

exit 0
