#!/bin/bash
# sign_ad_hoc.sh
# Ad-hoc sign a macOS app bundle or console executable

set -e  # Exit on any error

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <path-to-app-or-executable>"
    exit 1
fi

TARGET="$1"

if [ ! -e "$TARGET" ]; then
    echo "Error: '$TARGET' does not exist."
    exit 1
fi

echo "Signing '$TARGET' with ad-hoc identity (-)..."

# If it’s an .app bundle, use --deep to sign all contents
if [[ "$TARGET" == *.app ]]; then
    codesign --force --deep --sign - "$TARGET"
else
    # Console executable
    codesign --force --sign - "$TARGET"
fi

echo "Verifying signature..."
codesign -dv --verbose=4 "$TARGET"

echo "Done. '$TARGET' is ad-hoc signed and can be run from Terminal or Finder."