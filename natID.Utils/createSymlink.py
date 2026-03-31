#!/usr/bin/env python3

import os
import sys
import platform

def createSymlink(target, link_name):
    if not os.path.exists(target):
        print(f"Error: Target '{target}' does not exist.")
        sys.exit(1)

    # Remove existing link/file if present
    if os.path.lexists(link_name):
        print(f"Removing existing file/link: {link_name}")
        os.remove(link_name)

    try:
        if platform.system() == "Windows":
            # On Windows we must specify if target is directory
            is_dir = os.path.isdir(target)
            os.symlink(target, link_name, target_is_directory=is_dir)
        else:
            os.symlink(target, link_name)

        print("Symbolic link created successfully:")
        print(f"{link_name} -> {target}")

    except OSError as e:
        print("Failed to create symbolic link.")
        print("Error:", e)
        if platform.system() == "Windows":
            print("\nOn Windows you may need:")
            print("- Administrator privileges OR")
            print("- Enable Developer Mode")
        sys.exit(1)

def main():
    if len(sys.argv) != 3:
        print("Usage:")
        print("  python createSymlink.py <target> <link_name>")
        sys.exit(1)

    target = sys.argv[1]
    link_name = sys.argv[2]

    createSymlink(target, link_name)

if __name__ == "__main__":
    main()