#!/bin/bash

# Check if a parameter was provided
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

echo "Listing LC_RPATH entries for: $BINARY"
echo "----------------------------------------"

# Extract and print RPATH entries
otool -l "$BINARY" | awk '
    /LC_RPATH/ { found=1 }
    found && /path/ { print $2; found=0 }
'

exit 0