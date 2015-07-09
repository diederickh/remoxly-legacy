#!/bin/sh

d=${PWD}
my_build_dir=${d}

# Checkout the dependencies module
if [ ! -d ${d}/dependencies ] ; then
    git clone git@github.com:roxlu/dependencies.git
fi

# Set environment variables
vs="2013"
source ./dependencies/build.sh

if [ ! -d ${my_build_dir}/build.release ] ; then
    mkdir ${my_build_dir}/build.release
fi
