#! /usr/bin/env bash

_CORES=$(cat /proc/cpuinfo | grep processor | wc -l)
_PLATFORM=linux

function _init {
	echo "Git clone gdArchive from $PROVIDER..."
	git clone https://$PROVIDER/hurikhan/gdArchive.git
	cd gdArchive
	echo "Init submodules..."
	./init.sh sub $PROVIDER
	cd ..
}

function _compile {
	if [ ! -d gdArchive ]; then
		_init
	else
		cd gdArchive
		git pull
		cd ..
	fi	

	echo "Start compiling..."

	cd gdArchive
	scons platform=$_PLATFORM builtin=true
	cd ..
}

function _test {
	if [ ! -d godot ]; then
		echo "Git clone godot from $PROVIDER..."
		git clone --depth 1 https://$PROVIDER/hurikhan/godot.git
	else
		cd godot
		git pull
		cd ..
	fi

	cd godot
	scons p=server use_llvm=yes -j $_CORES
	cd ..

	cd gdArchive/test
	./test.py
	cd ..	
}

case $1 in
	bash)
		echo "Starting interactive bash session..."
		bash
		;;
	init)
		_init
		;;
	compile)
		_compile
		;;
	test)	
		_test
		;;
	all)
		_compile
		_test
		;;
	sweep)
		echo "Sweeping the /worker directory..."
		rm -Rf gdArchive
		rm -Rf godot
		;;
	cache)
		echo "Caching installed packages..."
		cp -R /var/cache/zypp/* /worker/opensuse/cache/
		;;
	*)
		echo "worker.sh: Unknown command!"
		exit 1
esac
