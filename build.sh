#! /usr/bin/env bash

if [ ! -d godot_headers ]
then
	git clone https://github.com/GodotNativeTools/godot_headers.git
fi

mkdir -p bin
clang -std=c11 -fPIC -c -g -I godot_headers gdns.c -o bin/gdns.os && \
clang -std=c11 -fPIC -c -g -I godot_headers gdarchive.c -o bin/gdarchive.os && \
clang -shared -larchive bin/gdns.os bin/gdarchive.os -o bin/gdarchive.so && \
rm bin/gdns.os && \
rm bin/gdarchive.os

#ld -r bin/gdarchive.os bin/libarchive.a -o bin/gdarchive.os2 && \
#ld -shared bin/gdarchive.os2 -o bin/gdarchive.so