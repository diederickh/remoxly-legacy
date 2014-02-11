#!/bin/sh

# You can pass 32 to the command line if you want to build a 32bit version: ./release 32

is_32bit=0
cmake_extra_vars=""

if [ "${1}" = "32" ] ; then 
    cmake_extra_vars="-DREMOXLY_USE_32BIT=1"
    is_32bit=1
fi

if [ ! -d build.release ] ; then 
    mkdir build.release
fi

cd build.release
cmake -DCMAKE_BUILD_TYPE=Release ${cmake_extra_vars} ../ 
cmake --build . --target install

if [ "$(uname)" == "Darwin" ] ; then 
    if [ ${is_32bit} -eq 1 ] ; then 
        cd ./../../../../install/mac-clang-i386/bin/
    else
        cd ./../../../../install/mac-clang-x86_64/bin/
    fi
    ./remote
else
    cd ./../../../../install/linux-gcc-x86_64/bin/
    ./gui
fi
