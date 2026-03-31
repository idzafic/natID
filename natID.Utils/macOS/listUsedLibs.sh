#!/bin/bash

# Check for input parameter
if [ -z "$1" ]; then
    echo "Usage: $0 <binary_file>"
    exit 1
fi

BINARY="$1"

# Check if file exists
if [ ! -f "$BINARY" ]; then
    echo "Error: File '$BINARY' does not exist."
    exit 1
fi

echo "Listing used libraries for: $BINARY"
echo "------------------------------------"

# otool -L shows all linked libraries with resolved paths
otool -L "$BINARY" | tail -n +2 | awk '{print $1}'