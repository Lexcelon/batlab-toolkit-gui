#!/usr/bin/env bash

# Echo commands as they are executed
set -x

cp ../build-Batlab-Desktop_Qt_5_9_0_GCC_64bit-Release/Batlab linux/packages/com.lexcelon.batlabtoolkitgui/data/Batlab

cp ../build-Batlab-Windows-Release/release/Batlab.exe windows/packages/com.lexcelon.batlabtoolkitgui/data/Batlab.exe

