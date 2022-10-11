#!/bin/bash

source_dir="$1"
target_dir="$2"
module_name="$3"

mkdir -p $target_dir && \
  echo "Compiling $module_name...\n" && \
  gcc -c -std=c17 -x c -Wextra -g "$source_dir/$module_name.c" -o "$target_dir/$module_name.o" && \
  echo "Done"

