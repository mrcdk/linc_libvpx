#!/bin/bash

mkdir build
cd build
make clean
../libvpx/configure --disable-docs --disable-examples --disable-unit-tests --enable-webm-io --enable-libyuv
make
cp *.a ../lib/Linux64
