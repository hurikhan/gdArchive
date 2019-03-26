#! /usr/bin/env bash

case $1 in
	bash)
		bash
		;;
	*)
		echo "worker.sh: Unknown command!"
		exit 1
esac
