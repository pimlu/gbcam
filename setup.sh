#!/bin/bash

export GBCAM_PATH="$(cd "$(dirname "$0")" && pwd)"

if ! [ -f "$GBCAM_PATH/.config.local.sh" ]; then
    echo "Error: Must create .config.local.sh based off .config.sh" 1>&2
    return 1
fi

source "$GBCAM_PATH/.config.local.sh"

(
    set -e
    cd "$GBCAM_PATH"

    rm -rf build/
    mkdir build
    cd build

    cmake ..
)

