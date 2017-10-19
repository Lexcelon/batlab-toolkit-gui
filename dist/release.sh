#!/usr/bin/env bash

# Echo commands as they are executed
set -x

# Copy over built programs to package directories
cp ../build-Batlab-Desktop_Qt_5_9_0_GCC_64bit-Release/Batlab linux/packages/com.lexcelon.batlabtoolkitgui/data/Batlab

cp ../build-Batlab-Windows-Release/release/Batlab.exe windows/packages/com.lexcelon.batlabtoolkitgui/data/Batlab.exe

# Clear out repositories and put new ones in their places
rm -rf ../docs/linux/*
~/Qt/QtIFW-3.0.1/linux/bin/repogen -p linux/packages/ ../docs/linux/

rm -rf ../docs/windows/*
wine ~/Qt/QtIFW-3.0.1/windows/bin/repogen.exe -p windows/packages/ ../docs/windows/

