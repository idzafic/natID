#!/bin/bash

# Check if parameter exists
if [ -z "$1" ]; then
    echo "Usage: $0 <binary_path>"
    exit 1
fi

BINARY="$1"

# Check if file exists
if [ ! -f "$BINARY" ]; then
    echo "Error: File '$BINARY' does not exist."
    exit 1
fi

echo "Listing RPATH/RUNPATH entries for: $BINARY"
echo "-------------------------------------------"

# Extract RPATH and RUNPATH values
readelf -d "$BINARY" 2>/dev/null | awk '
    /RPATH|RUNPATH/ {
        match($0, /\[(.*)\]/, arr)
        if (arr[1] != "")
            print arr[1]
    }
'

exit 0