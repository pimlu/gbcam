#!/bin/bash
set -e
cd "$(dirname "$0")"

# https://wellys.com/posts/rp2040_c_macos/
cd build
make -j8
echo "Waiting for RP2040..."
until [ -f '/Volumes/RPI-RP2/INFO_UF2.TXT' ]; do
    sleep 0.1
done
cp gbcam.uf2 /Volumes/RPI-RP2
