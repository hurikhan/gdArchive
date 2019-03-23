#! /usr/bin/env bash

if [ -z "$1" ]; then
	echo "Usage: compress <filename>"
	exit 1
fi

echo -n "gzip......................" && \
gzip -k -9 $1 && \
echo "[OK]"

echo -n "bzip2....................." && \
bzip2 -k -9 $1 && \
echo "[OK]"


echo -n "xz........................" && \
xz -z -k -e $1 && \
echo "[OK]"

echo -n "zstd......................" && \
zstd -19 -k -q --no-progress 测试_0001.tar && \
echo "[OK]"

#rm $1.gz
#rm $1.bz2
#rm $1.xz
#rm $1.zst
