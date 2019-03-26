#! /usr/bin/env bash

_IMAGE_NAME=gdarchive
_CONTAINER_NAME=gdarchive-worker


function _help {
	echo "Usage: $0 [action]"
}

function _image_create {
	echo "Creating Docker gdarchive image..."
	docker build -t $_IMAGE_NAME .
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

function _bash {
	echo "Starting interactive bash session..."
	docker run -it --rm --name gdarchive-worker -v $PWD/opensuse:/opensuse -v $PWD/worker:/worker gdarchive bash
}

case $1 in
	image)
		_image $2
		;;
	bash)
		_bash
		;;
	*)
		_help
		;;
esac
