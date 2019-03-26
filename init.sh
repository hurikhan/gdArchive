#! /usr/bin/env bash

if [ ! $0 == "./init.sh" ]; then
	echo "init.sh has to be executed in the directory directly."
	exit 1
fi

function print_help {
	echo "Usage: $0 [component]"
	echo ''
	echo '[component]'
	echo '	sub [provider]	--	initilizes and updates the git submodules'
	echo ''
	echo '[provider]'
	echo '	github.com	--	Sets the .gitmodules entries to github.com. <default>'
	echo '	gitee.com	--	Sets the .gitmodules entries to gitee.com.'
}

function init_sub {
	grep '^\[submodule' .git/config > /dev/null
	if [ $? == 0 ]; then
		echo "Git submodules already initialized! Aborting!"
		exit 2
	fi

	if [ ! -z $1 ]; then
		case $1 in
			github.com)
				;;
			
			gitee.com)
				sed -i 's/github.com\/GodotNativeTools/gitee.com\/hurikhan/g' .gitmodules
				sed -i 's/github.com\/facebook/gitee.com\/hurikhan/g' .gitmodules
				sed -i 's/github.com\/libarchive/gitee.com\/hurikhan/g' .gitmodules
				;;
			*)
				echo "Unknow provider! Aborting!"
				echo 
				print_help
				exit 3
		esac
	fi

	git submodule init
	git submodule update
}

case $1 in
	sub)
		if [ -z $2 ]; then
			init_sub
		else
			init_sub $2
		fi
		;;
	*)
		print_help
		;;
esac
