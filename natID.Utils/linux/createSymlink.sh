#!/usr/bin/env bash

# Usage:
# ./createSymlink.sh /full/path/to/executable /full/path/to/link

TARGET="$1"
LINK_NAME="$2"

if [ -z "$TARGET" ] || [ -z "$LINK_NAME" ]; then
    echo "Usage: $0 <target> <link_name>"
    exit 1
fi

if [ ! -e "$TARGET" ]; then
    echo "Error: Target '$TARGET' does not exist."
    exit 1
fi

ln -sf "$TARGET" "$LINK_NAME"

if [ $? -eq 0 ]; then
    echo "Symbolic link created:"
    echo "$LINK_NAME -> $TARGET"
else
    echo "Failed to create symbolic link."
fi