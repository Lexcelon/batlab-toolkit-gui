#!/bin/sh
appname=`basename $0 | sed s,\.sh$,,`

dirname=`dirname $0`
tmp="${dirname#?}"

if [ "${dirname%$tmp}" != "/" ]; then
    dirname=$PWD/$dirname
fi

LD_LIBRARY_PATH=$dirname/lib
export LD_LIBRARY_PATH

# Uncomment to debug plugin loading for Qt
# QT_DEBUG_PLUGINS=1
# export QT_DEBUG_PLUGINS

appname='BatlabToolkitGUI'
gdb "$dirname/$appname" "$@"
