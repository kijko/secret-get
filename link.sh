#!/bin/bash

target=$1

echo "Linking...\n" && \
    gcc "${@:2}" -lyajl -o $target && \
    echo "Done"

