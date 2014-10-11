#!/bin/sh

# You can pass 32 to the command line if you want to build a 32bit version: ./release 32
d=${PWD}
is_32bit=0
cmake_extra_vars=""
triplet=""
extern_dir=""
is_mac=n
is_linux=n
is_win=n

if [ "${1}" = "32" ] ; then 
    cmake_extra_vars="-DREMOXLY_USE_32BIT=1"
    is_32bit=1
    echo "We need to update this script for 32bit."
    exit
fi

if [ ! -d build.release ] ; then 
    mkdir build.release
fi

if [ ! -d build.release ] ; then 
    mkdir build.release
fi

# Detect system, set triplet. For now the triplet is hardcoded, w/o checks.
if [ "$(uname)" = "Darwin" ]; then
    is_mac=y
    triplet="mac-clang-x86_64"
elif [ "$(expr substr $(uname -s) 1 5)" = "Linux" ]; then
    is_linux=y
    triplet="linux-gcc-x86_64"
elif [ "$(expr substr $(uname -s) 1 10)" = "MINGW32_NT" ]; then
    is_win=y
    triplet="win-vs2012-x86_64"
fi

extern_path=${d}/../../../extern/${triplet}
install_path=${d}/../../../install/${triplet}

echo "NEED TO UPDATE THE GUI BUILD SCRIPT SO IT USES BUILD_*_DEPENDENCIES.SH"
exit

cd build.release
cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=${install_path} \
    -DEXTERN_LIB_DIR=${extern_path}/lib \
    -DEXTERN_INC_DIR=${extern_path}/include \
    -DEXTERN_SRC_DIR=${extern_path}/src \
    -DTINYLIB_DIR=${d}/../../../build/sources/tinylib \
    ${cmake_extra_vars} \
    ../ 

cmake --build . --target install --config Release

if [ "$(uname)" == "Darwin" ] ; then 
    if [ ${is_32bit} -eq 1 ] ; then 
        cd ./../../../../install/mac-clang-i386/bin/
    else
        cd ${install_path}/bin
    fi
    echo ${install_path}
    ./gui
    #./test_app

else
    cd ${install_path}
    ./gui
fi
