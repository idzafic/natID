#!/usr/bin/env python3
"""
Simple table plotting module that can be imported or used from command line.
Usage: 
  - From command line: python plot_utils.py file col1 col2 ... --wrt xcol
  - Programmatically: plot(["col1", "col2"], "xcol", file="filename.txt")
"""

import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import argparse
import re
import sys
import os
from pathlib import Path

# Detect if we're in an interactive environment
if sys.platform.startswith("linux"):
    if 'DISPLAY' not in os.environ:
        matplotlib.use("Agg")
        INTERACTIVE = False
    else:
        INTERACTIVE = True
else:
    # Windows + macOS
    INTERACTIVE = True


def expand_columns(spec, header):
    """Expand column specifications like w[1..8] to individual column names."""
    cols = []
    for s in spec:
        m = re.match(r"(\w+)\[(\d+)\.\.(\d+)\]", s)
        if m:
            base, a, b = m.groups()
            for i in range(int(a), int(b) + 1):
                name = f"{base}{i}"
                if name not in header:
                    sys.exit(f"Column '{name}' not found")
                cols.append(name)
        else:
            if s not in header:
                sys.exit(f"Column '{s}' not found")
            cols.append(s)
    return cols


def evaluate_x(expr, data, header):
    """Evaluate the x-axis expression."""
    if expr is None or expr == "row":
        return np.arange(len(data))

    # Replace column names with data[:,index] references
    for i, name in enumerate(header):
        expr = re.sub(rf"\b{name}\b", f"data[:,{i}]", expr)

    return eval(expr)


def plot(columns, wrt=None, file=None, show=None, **plot_kwargs):
    """
    Plot columns from a table file.
    
    Parameters:
    -----------
    columns : list or str
        Column names to plot (e.g., ["f", "P_gm"] or "f" or ["w[1..8]"])
    wrt : str, optional
        Column to plot against (x-axis). If None, uses row numbers.
    file : str, optional
        File to load data from. If None, prompts or uses default.
    show : bool, optional
        Whether to display the plot immediately. If None, auto-detects.
    **plot_kwargs : 
        Additional keyword arguments passed to plt.plot()
    
    Returns:
    --------
    fig : matplotlib.figure.Figure
        The figure object.
    ax : matplotlib.axes.Axes
        The axes object.
    
    Example:
    --------
    plot(["f", "P_gm"], "t")  # Equivalent to: plot.py f P_gm --wrt t
    """
    # Handle input types
    if isinstance(columns, str):
        columns = [columns]
    
    # If file is not provided, we can't proceed
    if file is None:
        raise ValueError("'file' parameter is required")
    
    # Auto-detect show parameter if not specified
    if show is None:
        # Show only in command-line mode or explicitly interactive environments
        show = (__name__ == "__main__" or is_interactive())
    
    # Load data
    with open(file) as f:
        header_line = f.readline()
        # Handle potential comment lines
        while header_line.startswith('#'):
            header_line = f.readline()
        header = header_line.split()
    
    # Count comment lines for proper skipping
    with open(file) as f:
        content = f.read()
    comment_lines = len(re.findall(r'^#.*', content, re.MULTILINE))
    
    data = np.loadtxt(file, skiprows=1 + comment_lines)
    
    # Expand column specifications
    y_cols = expand_columns(columns, header)
    x = evaluate_x(wrt, data, header)
    
    # Create plot
    fig, ax = plt.subplots(figsize=plot_kwargs.get('figsize', (8, 5)))
    
    for col in y_cols:
        idx = header.index(col)
        ax.plot(x, data[:, idx], label=col, 
                linewidth=plot_kwargs.get('linewidth', 1.5),
                marker=plot_kwargs.get('marker', None),
                markersize=plot_kwargs.get('markersize', 4))
    
    ax.set_xlabel(wrt if wrt else "row")
    ax.set_ylabel(", ".join(y_cols))
    if len(y_cols) > 1:
        ax.legend()
    ax.grid(True, alpha=0.3)
    
    if plot_kwargs.get('title'):
        ax.set_title(plot_kwargs['title'])
    
    plt.tight_layout()
    
    # Handle plot display/saving
    if show:
        if INTERACTIVE:
            try:
                plt.show()
            except Exception as e:
                print(f"Warning: Could not display plot interactively: {e}")
                # Try to save if output parameter is provided
                if 'output' in plot_kwargs:
                    plt.savefig(plot_kwargs['output'], dpi=150, bbox_inches='tight')
                    print(f"Plot saved to {plot_kwargs['output']}")
        else:
            # Non-interactive mode
            if 'output' in plot_kwargs:
                plt.savefig(plot_kwargs['output'], dpi=150, bbox_inches='tight')
                print(f"Plot saved to {plot_kwargs['output']}")
            else:
                print("Plot created (non-interactive environment). "
                      "Use 'output' parameter to save or call plt.show() from main script.")
    else:
        # show=False explicitly set
        if 'output' in plot_kwargs:
            plt.savefig(plot_kwargs['output'], dpi=150, bbox_inches='tight')
            print(f"Plot saved to {plot_kwargs['output']}")
    
    return fig, ax


def main():
    """Command line interface."""
    parser = argparse.ArgumentParser(description="Plot table columns")
    parser.add_argument("file", help="Input file (space separated)")
    parser.add_argument("columns", nargs="+",
                        help="Columns to plot (e.g. w1 w2 or w[1..8])")
    parser.add_argument("--wrt", default=None,
                        help="X axis (e.g. Ep, Ep+1, row)")
    parser.add_argument("--output", "-o", help="Save plot to file")
    parser.add_argument("--title", help="Plot title")
    parser.add_argument("--no-show", action="store_true", 
                        help="Don't display plot (just save if --output is given)")
    
    args = parser.parse_args()
    
    # Call the plot function
    fig, ax = plot(
        columns=args.columns,
        wrt=args.wrt,
        file=args.file,
        show=not args.no_show,  # Invert for clarity
        title=args.title,
        output=args.output
    )
    
    # If output was specified but we're in show mode, the plot function already saved it
    # Just print confirmation if needed
    if args.output and not args.no_show and INTERACTIVE:
        print(f"\nPlot also saved to {args.output}")


# Create a simple alias for easier import
plot_columns = plot

if __name__ == "__main__":
    main()