#!/bin/bash
THIS_SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

REPO_DIR=$THIS_SCRIPT_DIR/../..
BUILD_DIR=$REPO_DIR/build/examples/ubuntu_minimal
NUNAVUT_OUT_DIR=$REPO_DIR/build/nunavut_out
mkdir -p $BUILD_DIR

cd $REPO_DIR
if [ -d $NUNAVUT_OUT_DIR ] && [ "$(ls -A $NUNAVUT_OUT_DIR)" ]; then
    :
else
    ./scripts/nnvg_generate_c_headers.sh
fi


cd $BUILD_DIR
cmake ../../../examples/ubuntu_minimal && make
