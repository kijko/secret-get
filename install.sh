#!/bin/bash

target=$1
app_version=$2
install_dir="$3/$app_version"
symlink="/usr/bin/srg"

sudo mkdir -p "$install_dir" && \
    sudo cp "$target" "$install_dir/exec" && \
    sudo rm -f "$symlink" && \
    sudo ln -s "$install_dir/exec" "$symlink" && \
    echo "Done"

