#!/bin/sh

d=${PWD}
my_build_dir=${d}

# Set environment variables
vs="2013"
source ./../../../build/dependencies/environment.sh

if [ ! -d ${build_dir} ] ; then
    mkdir ${build_dir}
    fi

cd ${build_dir}
cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=${install_path} \
    -DEXTERN_LIB_DIR=${extern_path}/lib \
    -DEXTERN_INC_DIR=${extern_path}/include \
    -DEXTERN_SRC_DIR=${extern_path}/src \
    -DTINYLIB_DIR=${sd}/tinylib/ \
    ../

cmake --build . --target install --config Release

cd ${install_path}
./gui

