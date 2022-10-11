#!/bin/bash

install_dir="$1"

sudo rm -f /usr/bin/srg &&
    sudo rm -fr "$install_dir"

