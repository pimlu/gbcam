#!/bin/bash
set -e
cd "$(dirname "$0")"

function find_tty() {
    tty=`ls /dev/tty.usbmodem* 2>/dev/null`
    if [ "`echo "$tty" | wc -l | sed 's/^ *//'`" == 1 ]; then
        echo "$tty"
    fi
}

function wait_for_tty() {
    while true; do
        tty=`find_tty`
        if ! [ -z "$tty" ]; then
          echo "$tty"
          return
        fi
        sleep 0.1
    done
}

while true; do
    echo "waiting for tty..."
    tty=`wait_for_tty`
    set +e
    picocom $tty -b 115200
    rc=$?
    set -e
    if [ "$rc" == 1 ]; then
        echo "detected disconnect"
    else
        exit
    fi
done
        