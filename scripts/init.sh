#!/bin/bash
# This software is distributed under the terms of the MIT License.
# Copyright (c) 2023 Dmitry Ponomarev.
# Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

if [ "${BASH_SOURCE[0]}" -ef "$0" ]
then
    echo "ERROR: you should source this script, not execute it!"
    exit
fi

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
REPOSITORY_PATH="$SCRIPT_DIR/.."
REG_DATA_TYPE_PATH="$REPOSITORY_PATH/Libs/public_regulated_data_types/uavcan $REPOSITORY_PATH/Libs/public_regulated_data_types/reg/"

res=$(ifconfig | grep slcan0)
if [ -z "$res" ]; then
    if [ -z "$CYPHAL_DEV_PATH_SYMLINK" ]; then
        $SCRIPT_DIR/create_slcan.sh
    else
        $SCRIPT_DIR/create_slcan.sh -d $CYPHAL_DEV_PATH_SYMLINK
    fi
fi

export YAKUT_COMPILE_OUTPUT="$REPOSITORY_PATH/build/compile_output"
export YAKUT_PATH="$YAKUT_COMPILE_OUTPUT"
export UAVCAN__CAN__IFACE='socketcan:slcan0'
export UAVCAN__CAN__MTU=8
export UAVCAN__NODE__ID=127

if [ -d $YAKUT_COMPILE_OUTPUT ] && [ "$(ls -A $YAKUT_COMPILE_OUTPUT)" ]; then
    :
else
    yakut compile $REG_DATA_TYPE_PATH -O $YAKUT_COMPILE_OUTPUT
fi
