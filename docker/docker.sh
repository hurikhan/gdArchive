#! /usr/bin/env bash

_CONFIG=.docker.db
_IMAGE_NAME=gdarchive
_CONTAINER_NAME=gdarchive-worker
_MOUNTS="-v $PWD/worker:/worker" 

function _env {
	# set _PROVIDER
	if [ -z $_PROVIDER ]; then
		if [ -f $_CONFIG ]; then
			local _ARR=($(grep '^PROVIDER' $_CONFIG))
			_PROVIDER=${_ARR[1]}			
		else
			_PROVIDER=github.com
			echo "PROVIDER $_PROVIDER" > $_CONFIG
		fi
	fi
	_ENV="-e PROVIDER=$_PROVIDER"
}

function _image_create {
	echo "Creating Docker gdarchive image..."
	mkdir -p worker/opensuse/cache
	docker build -t $_IMAGE_NAME .
	_run cache
}

function _image_remove {
	echo "Removing Docker gdarchive image..."
	docker image rm gdarchive
}


function _image {
	case $1 in
		create)
			_image_create
			;;
		remove)
			_image_remove
			;;
		*)
			_help
	esac
}

function _list {
	echo 
	echo "Listing images..."
	docker image ls
	echo 
	echo "Listing containers..."
	docker container ls --all
	echo 
}

function _env_set {
	case $1 in
		provider)
			case $2 in
				gitee.com)
					echo "PROVIDER gitee.com" > $_CONFIG
					_env
					;;
				github.com)
					echo "PROVIDER github.com" > $_CONFIG
					_env
					;;
				*)
					echo "Unkown Provider: $2!"
					exit 1
					;;
				
			esac
			;;
		*)
			;;
	esac
	cat .docker.db
}

function _run {
	docker run -it --rm --name $_CONTAINER_NAME $_ENV $_MOUNTS $_IMAGE_NAME $1 $2 $3 $4
}

function _help {
	echo "Usage: $0 [action]"
}

_env

case $1 in
	image)
		_image $2
		;;
	list|ls)
		_list
		;;
	env)
		_env_set $2 $3
		;;
	run)
		_run $2 $3 $4
		;;
	*)
		_help
		;;
esac
