#!/bin/bash
# This software is distributed under the terms of the MIT License.
# Copyright (c) 2023-2024 Dmitry Ponomarev.
# Author: Dmitry Ponomarev <ponomarevda96@gmail.com>
SCRIPT_NAME=$(basename $BASH_SOURCE)
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
REPOSITORY_PATH="$(dirname "$SCRIPT_DIR")"
PUBLIC_DSDL_DIR="$REPOSITORY_PATH/Libs/public_regulated_data_types"
DS015_DSDL_DIR="$REPOSITORY_PATH/Libs/ds015"

if [ "$#" -eq 1 ]; then
    OUT_DIR=$1
else
    OUT_DIR="$REPOSITORY_PATH/build/nunavut_out"
fi

mkdir -p $OUT_DIR
PARAMS="--target-language c --target-endianness=little -v" # without asserts yet: --enable-serialization-asserts
nnvg $PARAMS "$DS015_DSDL_DIR" --lookup-dir "$PUBLIC_DSDL_DIR/uavcan" --outdir $OUT_DIR
nnvg $PARAMS "$PUBLIC_DSDL_DIR/reg" --lookup-dir "$PUBLIC_DSDL_DIR/uavcan" --outdir $OUT_DIR
nnvg $PARAMS "$PUBLIC_DSDL_DIR/uavcan" --outdir $OUT_DIR
