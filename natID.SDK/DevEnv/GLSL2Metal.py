#!/usr/bin/env python3
import argparse, re, sys, os, glob, shutil, subprocess, struct
from pathlib import Path
from typing import List, Tuple, Dict, Optional

# --------------------------
# Config
# --------------------------
EXT_TO_STAGE = {
    ".vert": "vertex",
    ".frag": "fragment",
    ".comp": "compute",
    ".geom": "geom",
    ".tesc": "tesc",
    ".tese": "tese",
}
STAGE_TOKEN = {
    "vertex":"vert","fragment":"frag","compute":"comp",
    "geom":"geom","tesc":"tesc","tese":"tese"
}

# --------------------------
# Utilities
# --------------------------
def is_macos() -> bool:
    return sys.platform == "darwin"

def which(x: str) -> Optional[str]:
    return shutil.which(x)

def run(cmd: List[str], verbose: bool):
    if verbose:
        print("$", " ".join(cmd))
    subprocess.run(cmd, check=True)

def safe_unlink(p: str, verbose: bool):
    try:
        os.remove(p)
        if verbose:
            print("removed", p)
    except FileNotFoundError:
        pass

def discover_shaders(root: str = ".") -> List[str]:
    pats = [f"**/*{ext}" for ext in EXT_TO_STAGE.keys()]
    files: List[str] = []
    for p in pats:
        files += glob.glob(os.path.join(root, p), recursive=True)
    return [f for f in sorted(files) if os.path.isfile(f)]

def discover_metal_files(root: str = ".") -> List[str]:
    metal_files = glob.glob(os.path.join(root, "**/*.metal"), recursive=True)
    return [f for f in sorted(metal_files) if os.path.isfile(f)]

def discover_base_metals(base: str) -> List[str]:
    # Our generator emits base.<entry>.<stage>.metal
    return sorted(glob.glob(f"{base}.*.metal"))

def discover_base_airs(base: str) -> List[str]:
    return sorted(glob.glob(f"{base}.*.air"))

# Naive function detector for GLSL-like code: finds top-level function names (excludes main)
FUNC_RE = re.compile(r'^\s*(?:[a-zA-Z_]\w*\s+)*([a-zA-Z_]\w*)\s*\([^;{]*\)\s*\{', re.M)

def extract_functions(src_text: str) -> List[str]:
    blacklist = {"if","for","while","switch","layout","struct"}
    names = []
    for m in FUNC_RE.finditer(src_text):
        name = m.group(1)
        if name == "main" or name in blacklist:
            continue
        names.append(name)
    seen=set()
    out=[]
    for n in names:
        if n not in seen:
            seen.add(n)
            out.append(n)
    return out

# --------------------------
# SPIR-V entry string patch
# --------------------------
def patch_spirv_entry_string(spv_path: str, new_name: str):
    with open(spv_path, 'rb') as f:
        data = f.read()
    words = [struct.unpack('<I', data[i:i+4])[0] for i in range(0, len(data), 4)]
    i = 5
    while i < len(words):
        wc = words[i] >> 16
        op = words[i] & 0xFFFF
        if op == 15:  # OpEntryPoint
            s = i+3
            e = s
            while e < i+wc:
                w = words[e]
                # find NUL-terminated string boundary
                if ((w & 0xFF)==0) or ((w>>8 & 0xFF)==0) or ((w>>16 & 0xFF)==0) or ((w>>24 & 0xFF)==0):
                    e += 1
                    break
                e += 1
            old_len = e - s
            b = new_name.encode('utf-8') + b'\x00'
            b += b'\x00' * ((4 - (len(b) % 4)) % 4)
            new_words = [b[j] | (b[j+1]<<8) | (b[j+2]<<16) | (b[j+3]<<24) for j in range(0, len(b), 4)]
            diff = len(new_words) - old_len
            words[s:s+old_len] = new_words
            if diff:
                words[i] = ((wc + diff) << 16) | op
            break
        i += wc
    with open(spv_path, 'wb') as f:
        for w in words:
            f.write(struct.pack('<I', w))

# --------------------------
# Compile and link (Vulkan)
# --------------------------
def compile_glsl_with_stub(src_path: str, stage: str, entry_func: str, out_spv: str, verbose: bool):
    tmp_src = out_spv + ".tmp." + Path(src_path).suffix.lstrip(".")
    with open(src_path, "r", encoding="utf-8") as f:
        original = f.read()
    stub = "\n// auto-gen stub for entry: {0}\nvoid main() {{ {0}(); }}\n".format(entry_func)
    with open(tmp_src, "w", encoding="utf-8") as f:
        f.write(original + "\n" + stub)

    stage_flag = {
        "vertex":"-S vert","fragment":"-S frag","compute":"-S comp",
        "geom":"-S geom","tesc":"-S tesc","tese":"-S tese"
    }[stage].split()

    try:
        run(["glslangValidator", *stage_flag, "-V", tmp_src, "-o", out_spv], verbose)
    finally:
        os.remove(tmp_src)

    patch_spirv_entry_string(out_spv, entry_func)

def link_spvs(spvs: List[str], out_spv: str, verbose: bool):
    run(["spirv-link", *spvs, "-o", out_spv], verbose)

# --------------------------
# MSL emission and Metal toolchain
# --------------------------
def spv_to_msl_entries(bundle_spv: str, out_base: str, entries: List[Tuple[str,str]], verbose: bool) -> List[str]:
    tool = which("spirv-cross")
    if not tool:
        print("Warning: 'spirv-cross' not found; skipping Metal text emission.")
        return []
    paths=[]
    for (name, stage) in entries:
        token = STAGE_TOKEN[stage]
        safe = re.sub(r'[^A-Za-z0-9_]+', '_', name)
        out_file = f"{out_base}.{safe}.{token}.metal"
        cmd = [
            tool, 
            "--msl", 
            "--msl-decoration-binding",
            bundle_spv,
            "--entry", name,
            "--stage", token,
            "--output", out_file,
        ]
        run(cmd, verbose)
        paths.append(out_file)
    return paths

def compile_metal_to_air(msl_sources: List[str], verbose: bool) -> List[str]:
    if not is_macos() or not which("xcrun"):
        return []
    airs = []
    for src in msl_sources:
        air = str(Path(src).with_suffix(".air"))
        run(["xcrun","-sdk","macosx","metal","-c",src,"-o",air], verbose)
        airs.append(air)
    return airs

def link_airs_to_metallib(airs: List[str], out_metallib: str, verbose: bool) -> bool:
    if not airs or not is_macos() or not which("xcrun"):
        return False
    run(["xcrun","-sdk","macosx","metallib", *airs, "-o", out_metallib], verbose)
    return True

def up_to_date(src: str, dst: str) -> bool:
    try:
        return os.path.getmtime(dst) >= os.path.getmtime(src)
    except OSError:
        return False

def ensure_airs_current(msl_sources: List[str], verbose: bool) -> List[str]:
    if not is_macos() or not which("xcrun"):
        return []
    desired_airs = [str(Path(s).with_suffix(".air")) for s in msl_sources]
    need_compile = False
    for s, a in zip(msl_sources, desired_airs):
        if not os.path.exists(a) or not up_to_date(s, a):
            need_compile = True
            break
    if need_compile:
        return compile_metal_to_air(msl_sources, verbose)
    else:
        if verbose:
            print("Reusing up-to-date AIRs")
        return desired_airs

# --------------------------
# Entry resolution
# --------------------------
def parse_entry_args(args_list: List[str]) -> Dict[str,str]:
    out: Dict[str,str] = {}
    pos: List[str] = []
    for raw in args_list:
        parts = [p for p in raw.split(",") if p]
        for p in parts:
            if ":" in p:
                path, func = p.split(":",1)
                out[path] = func
            else:
                pos.append(p)
    out["__pos__"] = ",".join(pos)
    return out

def smart_match_entry_to_stage(candidate: str, stage: str) -> bool:
    """Check if a candidate function name is likely intended for a specific stage"""
    candidate_lower = candidate.lower()
    
    # Common vertex shader patterns
    if stage == "vertex":
        return any(pattern in candidate_lower for pattern in [
            "vert", "vs", "vertex", "main_v", "vmain", "v_main"
        ])
    
    # Common fragment shader patterns  
    elif stage == "fragment":
        return any(pattern in candidate_lower for pattern in [
            "frag", "fs", "fragment", "pixel", "main_f", "fmain", "f_main", "pmain", "p_main"
        ])
    
    # Common compute shader patterns
    elif stage == "compute":
        return any(pattern in candidate_lower for pattern in [
            "comp", "cs", "compute", "main_c", "cmain", "c_main"
        ])
    
    # Common geometry shader patterns
    elif stage == "geom":
        return any(pattern in candidate_lower for pattern in [
            "geom", "gs", "geometry", "main_g", "gmain", "g_main"
        ])
    
    # Tessellation control patterns
    elif stage == "tesc":
        return any(pattern in candidate_lower for pattern in [
            "tesc", "tcs", "tesscontrol", "main_tc", "tcmain", "tc_main"
        ])
    
    # Tessellation evaluation patterns
    elif stage == "tese":
        return any(pattern in candidate_lower for pattern in [
            "tese", "tes", "tesseval", "main_te", "temain", "te_main"
        ])
    
    return False

def resolve_entries(files: List[str], entry_arg: Dict[str,str]) -> Dict[str,str]:
    result: Dict[str,str] = {}
    pos_items = [p for p in entry_arg.get("__pos__", "").split(",") if p]

    # First pass: collect all functions from all files for intelligent matching
    file_functions: Dict[str, List[str]] = {}
    for path in files:
        with open(path,"r",encoding="utf-8") as f:
            text=f.read()
        file_functions[path] = extract_functions(text)

    # Second pass: resolve entries
    for path in files:
        funcs = file_functions[path]
        explicit = entry_arg.get(path)
        stage = EXT_TO_STAGE[Path(path).suffix.lower()]

        if explicit:
            if explicit not in funcs:
                raise SystemExit(f"[error] {path}: requested entry '{explicit}' not found. Candidates: {funcs or 'NONE'}")
            chosen = explicit
        else:
            if len(funcs) == 0:
                chosen = "__dummy__"
            elif len(funcs) == 1 and not pos_items:
                chosen = funcs[0]
            else:
                if pos_items:
                    # Try to find a matching function in this file
                    chosen = None
                    
                    # First, try exact match
                    for candidate in pos_items[:]:
                        if candidate in funcs:
                            chosen = candidate
                            pos_items.remove(candidate)
                            break
                    
                    # If no exact match, try smart matching based on naming patterns
                    if chosen is None:
                        for candidate in pos_items[:]:
                            if smart_match_entry_to_stage(candidate, stage):
                                # This candidate seems intended for this stage, look for it
                                if candidate in funcs:
                                    chosen = candidate
                                    pos_items.remove(candidate)
                                    break
                                else:
                                    # Function name suggests it should be here, but it's not
                                    # Look for similar functions in this file
                                    stage_appropriate_funcs = [f for f in funcs if smart_match_entry_to_stage(f, stage)]
                                    if stage_appropriate_funcs:
                                        chosen = stage_appropriate_funcs[0]
                                        pos_items.remove(candidate)
                                        print(f"[info] '{candidate}' not found in {path} ({stage}), using similar function '{chosen}'")
                                        break
                    
                    # If still no match, try to find best available function for this stage
                    if chosen is None:
                        # Look for stage-appropriate functions in this file
                        stage_appropriate_funcs = [f for f in funcs if smart_match_entry_to_stage(f, stage)]
                        if stage_appropriate_funcs and pos_items:
                            chosen = stage_appropriate_funcs[0]
                            consumed = pos_items.pop(0)  # Remove any positional entry
                            print(f"[info] No exact match for '{consumed}' in {path} ({stage}), using stage-appropriate function '{chosen}'")
                        elif pos_items:
                            # Fallback: try any available function
                            for candidate in pos_items[:]:
                                # Check if this candidate exists elsewhere and suggest alternatives
                                candidate_locations = []
                                for other_path, other_funcs in file_functions.items():
                                    if candidate in other_funcs:
                                        other_stage = EXT_TO_STAGE[Path(other_path).suffix.lower()]
                                        candidate_locations.append((other_path, other_stage))
                                
                                if candidate_locations and funcs:
                                    chosen = funcs[0]  # Use first available function
                                    pos_items.remove(candidate)
                                    print(f"[info] '{candidate}' not in {path} ({stage}), using '{chosen}'. '{candidate}' found in: {', '.join([f'{p} ({s})' for p, s in candidate_locations])}")
                                    break
                    
                    if chosen is None and pos_items:
                        # Final fallback
                        candidate = pos_items.pop(0)
                        if candidate not in funcs:
                            # Provide detailed error with suggestions
                            suggestions = []
                            for other_path, other_funcs in file_functions.items():
                                if candidate in other_funcs:
                                    other_stage = EXT_TO_STAGE[Path(other_path).suffix.lower()]
                                    suggestions.append(f"{other_path} ({other_stage} stage)")
                            
                            error_msg = f"[error] {path}: function '{candidate}' not found. Available in this file: {funcs or 'NONE'}"
                            if suggestions:
                                error_msg += f"\n[info] '{candidate}' is available in: {', '.join(suggestions)}"
                                if funcs:
                                    error_msg += f"\n[hint] For {path}, try: -e {Path(path).name}:{funcs[0]}"
                            raise SystemExit(error_msg)
                        chosen = candidate
                else:
                    raise SystemExit(f"[error] {path}: multiple functions {funcs}. Supply -e to pick one.")
        result[path] = chosen

    if pos_items:
        # Provide helpful information about unused entries
        unused_info = []
        for unused in pos_items:
            found_in = []
            for path, funcs in file_functions.items():
                if unused in funcs:
                    stage = EXT_TO_STAGE[Path(path).suffix.lower()]
                    found_in.append(f"{path} ({stage})")
            if found_in:
                unused_info.append(f"'{unused}' found in: {', '.join(found_in)}")
            else:
                unused_info.append(f"'{unused}' not found in any file")
        
        error_msg = f"[error] Extra -e entries unused: {pos_items}"
        if unused_info:
            error_msg += "\n" + "\n".join(unused_info)
        raise SystemExit(error_msg)
    
    return result

# --------------------------
# Tool checking
# --------------------------
def check_and_report_tools() -> bool:
    """Check for required Vulkan/Metal tools and provide installation guidance."""
    required = ["glslangValidator", "spirv-link", "spirv-cross"]
    missing = [t for t in required if not which(t)]
    
    if not missing:
        return True  # All tools found
    
    print(f"[error] Missing required tools: {', '.join(missing)}\n")
    print("=" * 70)
    print("Tool Installation Guide")
    print("=" * 70)
    print("\nRequired tools:")
    print("  - glslangValidator - GLSL to SPIR-V compiler")
    print("  - spirv-link       - SPIR-V linker")
    print("  - spirv-cross      - SPIR-V to Metal Shading Language converter\n")
    
    print("Installation options for your platform:\n")
    
    if is_macos():
        print("macOS:")
        print("  Option 1: Homebrew (recommended)")
        print("    $ brew install glslang spirv-tools spirv-cross\n")
        print("  Option 2: Vulkan SDK")
        print("    Download from: https://vulkan.lunarg.com/sdk/home")
        print("    Then: brew install spirv-cross\n")
    elif sys.platform.startswith("linux"):
        print("Linux (Ubuntu/Debian):")
        print("  $ sudo apt-get install glslang-tools spirv-tools spirv-cross\n")
        print("Linux (Fedora/RHEL):")
        print("  $ sudo dnf install glslang spirv-tools spirv-cross\n")
        print("Linux (Arch):")
        print("  $ sudo pacman -S glslang spirv-tools spirv-cross\n")
    elif sys.platform == "win32":
        print("Windows:")
        print("  Option 1: Vulkan SDK (includes glslangValidator & spirv-tools)")
        print("    Download from: https://vulkan.lunarg.com/sdk/home\n")

    
    print("Universal option: Build from source")
    print("  glslang:     https://github.com/KhronosGroup/glslang")
    print("  SPIRV-Tools: https://github.com/KhronosGroup/SPIRV-Tools")
    print("  SPIRV-Cross: https://github.com/KhronosGroup/SPIRV-Cross\n")
    
    print("=" * 70)
    print("After installation, verify tools are in PATH by running:")
    print("  $ which glslangValidator && which spirv-link && which spirv-cross")
    print("=" * 70)
    return False

# --------------------------
# Main
# --------------------------
def main():
    ap = argparse.ArgumentParser(description="GLSL -> SPIR-V bundle; emit per-entry MSL; link metallib on macOS")
    ap.add_argument("-o", "--output", default="main", help="Output basename (default: main)")
    ap.add_argument("-e", "--entry", action="append", default=[],
                    help="Entry selection: name[,name...] OR path:func (can repeat)")
    ap.add_argument("-v", "--verbose", action="store_true", help="Verbose; keep intermediates")
    ap.add_argument("-i", "--input", default=".", help="Input directory to search for shaders (default: current directory)")
    ap.add_argument("--metal-only", action="store_true", help="Skip Vulkan path, only process Metal files")
    args = ap.parse_args()

    base = args.output
    verbose = args.verbose
    input_dir = args.input

    vulkan_tools_available = all(which(t) for t in ["glslangValidator", "spirv-link", "spirv-cross"])
    can_use_xcrun = is_macos() and bool(which("xcrun"))

    # Warn if Vulkan tools are not available and we're falling back to metal-only (only if not explicitly requested)
    if not args.metal_only and not vulkan_tools_available and is_macos():
        print("[warning] Vulkan tools not available. Running in Metal-only mode.")
        check_and_report_tools()

    # Fast path: if we're on macOS and metallib missing but base.*.metal exist from a previous non-macOS run, link them now.
    # (Commented out to always run the full pipeline)
    # if can_use_xcrun and not os.path.exists(f"{base}.metallib"):
    #     existing_metals = discover_base_metals(base)
    #     if existing_metals:
    #         try:
    #             airs = ensure_airs_current(existing_metals, verbose)
    #             if airs and link_airs_to_metallib(airs, f"{base}.metallib", verbose):
    #                 if not verbose:
    #                     for a in airs:
    #                         safe_unlink(a, verbose)
    #                     for m in existing_metals:
    #                         safe_unlink(m, verbose)
    #                 print(f"Linked existing .metal -> {base}.metallib")
    #         except subprocess.CalledProcessError:
    #             print("[warning] Linking from existing .metal failed; continuing.")

    metal_only = args.metal_only or (is_macos() and not vulkan_tools_available)

    if metal_only:
        if not can_use_xcrun:
            print("[warning] Metal-only mode without xcrun. Nothing to do.")
            sys.exit(1)
        metal_files = discover_metal_files(input_dir)
        if not metal_files:
            if not os.path.exists(f"{base}.metallib"):
                print("[warning] Metal-only mode: no .metallib files and no .metal files found.")
            print("No .metal files found.")
            sys.exit(1)
        try:
            airs = ensure_airs_current(metal_files, verbose)
            ok = link_airs_to_metallib(airs, f"{base}.metallib", verbose)
            if ok and not verbose:
                for a in airs:
                    safe_unlink(a, verbose)
                for m in metal_files:
                    safe_unlink(m, verbose)
            kept = [f"{base}.metallib"] if ok else metal_files
            print("Done. Kept:", ", ".join(kept))
        except subprocess.CalledProcessError:
            print("Error building metallib; keeping .metal files.")
            print("Done. Kept:", ", ".join(metal_files))
        return 0

    # Vulkan path (can run on any OS with Vulkan tools)
    if not vulkan_tools_available:
        print("[error] Required Vulkan tools missing:")
        check_and_report_tools()
        sys.exit(1)

    files = discover_shaders(input_dir)
    if not files:
        print("No shaders found (.vert, .frag, .comp, .geom, .tesc, .tese).")
        sys.exit(1)

    entries_map = resolve_entries(files, parse_entry_args(args.entry))

    per_spv: List[str] = []
    bundled_entries: List[Tuple[str,str]] = []

    metallib_built = False
    msl_files: List[str] = []

    try:
        for path in files:
            stage = EXT_TO_STAGE[Path(path).suffix.lower()]
            chosen = entries_map[path]
            out_spv = f"{Path(path).with_suffix('')}.{STAGE_TOKEN[stage]}.spv.tmp"

            if chosen == "__dummy__":
                tmp_src = out_spv + ".tmp.glsl"
                with open(path,"r",encoding="utf-8") as f:
                    original = f.read()
                stub = "\n// auto-gen dummy main\nvoid main() { /* no-op */ }\n"
                with open(tmp_src,"w",encoding="utf-8") as f:
                    f.write(original + "\n" + stub)
                stage_flag = {"vertex":"-S vert","fragment":"-S frag","compute":"-S comp",
                              "geom":"-S geom","tesc":"-S tesc","tese":"-S tese"}[stage].split()
                try:
                    run(["glslangValidator",*stage_flag,"-V",tmp_src,"-o",out_spv], verbose)
                finally:
                    os.remove(tmp_src)
                patch_spirv_entry_string(out_spv, "__dummy__")
            else:
                compile_glsl_with_stub(path, stage, chosen, out_spv, verbose)

            per_spv.append(out_spv)
            bundled_entries.append((chosen, stage))

        # Uniqueness checks
        per_stage: Dict[str, set] = {st:set() for st in EXT_TO_STAGE.values()}
        for name, st in bundled_entries:
            if name in per_stage[st]:
                raise SystemExit(f"[error] duplicate entry name '{name}' within stage '{st}'. Disambiguate with -e or rename.")
            per_stage[st].add(name)

        global_seen: Dict[str,str] = {}
        for name, st in bundled_entries:
            if name in global_seen and global_seen[name] != st:
                raise SystemExit(f"[error] entry name '{name}' reused across stages ('{global_seen[name]}' and '{st}'). "
                                 f"Metal requires unique function names across all stages. Rename one of them.")
            global_seen[name] = st

        # Link to a single SPIR-V bundle (for Vulkan and MSL emission)
        bundle_spv = f"{base}.spv"
        link_spvs(per_spv, bundle_spv, verbose)
        if verbose:
            print(f"Linked -> {bundle_spv}")

        # Emit per-entry .metal
        msl_files = spv_to_msl_entries(bundle_spv, base, bundled_entries, verbose)

        # If on macOS with xcrun, build metallib now; else keep .metal files for later macOS run.
        if is_macos() and which("xcrun"):
            try:
                airs = ensure_airs_current(msl_files, verbose)
                metallib_built = link_airs_to_metallib(airs, f"{base}.metallib", verbose)
                if metallib_built and not verbose:
                    for a in airs:
                        safe_unlink(a, verbose)
                    for m in msl_files:
                        safe_unlink(m, verbose)
            except subprocess.CalledProcessError:
                print("Error during Metal build; keeping .metal files for later linking.")
                metallib_built = False

    finally:
        # Always remove per-file temporary SPIR-Vs unless verbose
        if not verbose:
            for p in per_spv:
                safe_unlink(p, verbose)
        # Keep .metal files if we didn't produce a metallib (portable hand-off). If metallib exists, we still keep .metal.

    kept = []
    if os.path.exists(f"{base}.spv"):
        kept.append(f"{base}.spv")
    if os.path.exists(f"{base}.metallib"):
        kept.append(f"{base}.metallib")
    elif not metallib_built:
        # Metallib not present -> ensure we list the .metal hand-off artifacts.
        metals_now = discover_base_metals(base)
        kept.extend(metals_now)
    print("Done. Kept:", ", ".join(kept) if kept else "(nothing)")

if __name__ == "__main__":
    main()
