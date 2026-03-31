#!/usr/bin/env python3

import os
import sys
import subprocess
from pathlib import Path

def run(cmd):
    try:
        result = subprocess.run(
            cmd,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            check=False
        )
        return result.returncode, result.stdout
    except Exception as e:
        return -1, str(e)


def get_dependencies(binary):
    code, output = run(["otool", "-L", str(binary)])
    deps = []

    if code != 0:
        return deps

    lines = output.splitlines()[1:]  # skip first line (self reference)
    for line in lines:
        dep = line.strip().split(" ")[0]
        if "@rpath" in dep or dep.startswith("/"):
            deps.append(dep)

    return deps


def get_signature_info(binary):
    info = {
        "signed": False,
        "adhoc": False,
        "hardened": False,
        "valid": False,
        "error": None
    }

    code, output = run(["codesign", "-dv", "--verbose=4", str(binary)])

    if "Signature=adhoc" in output:
        info["signed"] = True
        info["adhoc"] = True
    elif "Authority=" in output:
        info["signed"] = True

    if "Runtime Version:" in output:
        info["hardened"] = True

    code_verify, output_verify = run(
        ["codesign", "--verify", "--deep", "--strict", str(binary)]
    )

    if code_verify == 0:
        info["valid"] = True
    else:
        info["error"] = output_verify.strip()

    return info


def main(folder):
    # Expand ~ in paths
    root = Path(folder).expanduser()

    if not root.exists():
        print(f"Folder does not exist: {root}")
        return

    dylibs = list(root.glob("*.dylib"))

    if not dylibs:
        print(f"No dylibs found in {root}")
        return

    print(f"\nScanning {len(dylibs)} dylibs in {root}\n")

    all_info = {}
    issues = []

    for dylib in dylibs:
        sig = get_signature_info(dylib)
        deps = get_dependencies(dylib)

        all_info[dylib.name] = {
            "path": dylib,
            "signature": sig,
            "dependencies": deps
        }

        # Collect dylibs with issues
        if (not sig["signed"] or not sig["valid"] or sig["hardened"] or sig["error"]):
            issues.append((dylib.name, sig))

    # Print detailed info
    for name, data in all_info.items():
        print("=" * 70)
        print(f"Library: {name}")
        print(f"Path: {data['path']}\n")

        sig = data["signature"]
        print("Signature:")
        print(f"  Signed:     {sig['signed']}")
        print(f"  Ad-hoc:     {sig['adhoc']}")
        print(f"  Hardened:   {sig['hardened']}")
        print(f"  Valid:      {sig['valid']}")
        if sig["error"]:
            print("  Verification Error:")
            print(f"    {sig['error']}")

        print("\nDependencies:")
        for dep in data["dependencies"]:
            print(f"  {dep}")
        print()

    # Print summary
    print("=" * 70)
    print("SUMMARY OF ISSUES\n")

    if not issues:
        print("All dylibs are signed and valid.\n")
    else:
        print(f"{len(issues)} dylibs have issues:\n")
        for name, sig in issues:
            print(f"- {name}")
            if not sig["signed"]:
                print("    Unsigned")
            if not sig["valid"]:
                print("    Invalid signature")
            if sig["hardened"]:
                print("    Hardened runtime enabled")
            if sig["error"]:
                print(f"    Verification error: {sig['error']}")
        print()

    print("=" * 70)
    print("Scan complete.\n")


if __name__ == "__main__":
    # Use default folder if no argument provided
    if len(sys.argv) == 2:
        folder_to_scan = sys.argv[1]
    else:
        folder_to_scan = "~/natID.SDK/bin/lib"

    main(folder_to_scan)