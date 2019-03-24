#! /usr/bin/env bash

figlet zstd
cd zstd/build/cmake/
ninja clean
cd ../../..
cd zstd
rm build/cmake/.ninja_deps
rm build/cmake/.ninja_log
rm build/cmake/build.ninja
rm build/cmake/rules.ninja
rm build/cmake/CMakeCache.txt
cd ..


figlet zlib
cd zlib-1.2.11
ninja clean
cd ..
rm zlib-1.2.11/.ninja_deps
rm zlib-1.2.11/.ninja_log
rm zlib-1.2.11/CMakeCache.txt
rm -Rf zlib-1.2.11/CMakeFiles/
rm zlib-1.2.11/CTestTestfile.cmake
rm zlib-1.2.11/build.ninja
rm zlib-1.2.11/cmake_install.cmake
rm zlib-1.2.11/rules.ninja
rm zlib-1.2.11/zlib.pc
git checkout zlib-1.2.11/zconf.h

figlet bzip2
cd bzip2-1.0.6
make clean
cd ..

figlet lzma
cd xz-5.2.4
make clean
cd ..
rm xz-5.2.4/Doxyfile
rm xz-5.2.4/Makefile
rm xz-5.2.4/config.h
rm xz-5.2.4/config.log
rm xz-5.2.4/config.status
rm -Rf xz-5.2.4/debug/.deps/
rm xz-5.2.4/debug/Makefile
rm -Rf xz-5.2.4/lib/.deps/
rm xz-5.2.4/lib/Makefile
rm xz-5.2.4/libtool
rm xz-5.2.4/po/Makefile
rm xz-5.2.4/po/Makefile.in
rm xz-5.2.4/po/POTFILES
rm xz-5.2.4/src/Makefile
rm -Rf xz-5.2.4/src/liblzma/.deps/
rm xz-5.2.4/src/liblzma/Makefile
rm xz-5.2.4/src/liblzma/api/Makefile
rm -Rf xz-5.2.4/src/lzmainfo/.deps/
rm xz-5.2.4/src/lzmainfo/Makefile
rm xz-5.2.4/src/scripts/Makefile
rm xz-5.2.4/src/scripts/xzdiff
rm xz-5.2.4/src/scripts/xzgrep
rm xz-5.2.4/src/scripts/xzless
rm xz-5.2.4/src/scripts/xzmore
rm -Rf xz-5.2.4/src/xz/.deps/
rm xz-5.2.4/src/xz/Makefile
rm -Rf xz-5.2.4/src/xzdec/.deps/
rm xz-5.2.4/src/xzdec/Makefile
rm xz-5.2.4/stamp-h1
rm -Rf xz-5.2.4/tests/.deps/
rm xz-5.2.4/tests/Makefile
rm -Rf xz-5.2.4/autom4te.cache/
rm xz-5.2.4/m4/codeset.m4
rm xz-5.2.4/m4/extern-inline.m4
rm xz-5.2.4/m4/fcntl-o.m4
rm xz-5.2.4/m4/glibc2.m4
rm xz-5.2.4/m4/glibc21.m4
rm xz-5.2.4/m4/intdiv0.m4
rm xz-5.2.4/m4/intl.m4
rm xz-5.2.4/m4/intldir.m4
rm xz-5.2.4/m4/intmax.m4
rm xz-5.2.4/m4/inttypes-pri.m4
rm xz-5.2.4/m4/inttypes_h.m4
rm xz-5.2.4/m4/lcmessage.m4
rm xz-5.2.4/m4/lock.m4
rm xz-5.2.4/m4/longlong.m4
rm xz-5.2.4/m4/printf-posix.m4
rm xz-5.2.4/m4/size_max.m4
rm xz-5.2.4/m4/stdint_h.m4
rm xz-5.2.4/m4/threadlib.m4
rm xz-5.2.4/m4/uintmax_t.m4
rm xz-5.2.4/m4/wchar_t.m4
rm xz-5.2.4/m4/wint_t.m4
rm xz-5.2.4/m4/xsize.m4
rm xz-5.2.4/po/Makevars.template
git checkout xz-5.2.4/


figlet libarchive
cd libarchive
ninja clean
rm .ninja_deps
rm .ninja_log
rm build.ninja
rm rules.ninja
rm CMakeCache.txt
cd ..

rm build.db
