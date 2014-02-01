#!/bin/sh

if [ ! -d build.debug ] ; then 
    mkdir build.debug
fi

cd build.debug
cmake -DCMAKE_BUILD_TYPE=Debug ../ 
cmake --build . --target install

if [ "$(uname)" == "Darwin" ] ; then 
    lldb ./../../../../install/mac-clang-x86_64d/bin/bitmapfont_debug
else 
    gdb ./../../../../install/linux-gcc-x86_64d/bin/bitmapfont_debug
fi
