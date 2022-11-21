#!/bin/bash
set -e
cd "$(dirname "$0")"

rm -rf build/
mkdir build
cd build

# set this yourself
export PICO_SDK_PATH=/Users/stuart/Documents/builds/pico/pico-sdk
export PICO_TOOLCHAIN_PATH=/Applications/ARM/bin

cmake ..

