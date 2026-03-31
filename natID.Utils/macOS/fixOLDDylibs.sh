#!/bin/bash

LIB_DIR="$HOME/other_bin/myLib"

if [ ! -d "$LIB_DIR" ]; then
    echo "Directory not found: $LIB_DIR"
    exit 1
fi

echo "Processing dylibs in: $LIB_DIR"
echo "-----------------------------------"

find "$LIB_DIR" -type f -name "*.dylib" | while read -r file; do
    echo "Removing quarantine from: $file"
    xattr -dr com.apple.quarantine "$file"
done

echo "-----------------------------------"
echo "Done."