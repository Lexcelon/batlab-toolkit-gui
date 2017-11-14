#!/usr/bin/env bash

wget https://raw.githubusercontent.com/benlau/qtci/master/bin/extract-qt-installer
chmod +x extract-qt-installer

wget https://download.qt.io/archive/qt/5.9/5.9.1/qt-opensource-linux-x64-5.9.1.run
chmod +x qt-opensource-linux-x64-5.9.1.run

./extract-qt-installer qt-opensource-linux-x64-5.9.1.run ~/Qt

