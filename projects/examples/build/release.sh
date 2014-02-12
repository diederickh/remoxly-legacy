#!/bin/sh

if [ "${1}" = "" ] ; then 
    echo "Usage: ./release.sh [glfw_minimal|remote_client|remote_server]"
    exit
fi

if [ ! -d build.release ] ; then 
    mkdir build.release
fi

cd build.release
cmake -DCMAKE_BUILD_TYPE=Release ${cmake_extra_vars} ../ 
cmake --build . --target install

if [ "$(uname)" == "Darwin" ] ; then 

    if [ "${1}" = "glfw_minimal" ] ; then 
        ./glfw_minimal
    elif [ "${1}" = "remote_client" ] ; then
        ./remote_client
    elif [ "${1}" = "remote_server" ] ; then
        ./remote_server
    fi
else
    cd ./../../../../install/linux-gcc-x86_64/bin/
    ./glfw_minimal
fi
