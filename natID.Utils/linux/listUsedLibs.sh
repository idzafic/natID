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

# ldd shows all shared libs with resolved paths
ldd "$BINARY" | awk '
    /=>/ {
        print $3
    }
    /^[^[:space:]]/ && /not found/ {
        print $1 " (not found)"
    }
'