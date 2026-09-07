#!/bin/bash

set -e

BRANCHES=4

OUTPUT_PATH="./output"
BIN_PATH="./bin"

mkdir -p "$OUTPUT_PATH" "$BIN_PATH"

run_task() {
    local msg="$1"
    shift
    echo "$msg"

    local out
    out=$("$@" 2>&1) || {
        echo -e "Error:\n$out"
        exit 1
    }
}

# --- C ---
run_task "create_svg_tree.c" gcc -std=c23 -O2 -Wall -Wextra create_svg_tree.c -o "$BIN_PATH/tree_c" -lm
"$BIN_PATH/tree_c" $BRANCHES "$OUTPUT_PATH/tree_c.svg"

# --- C++ ---
run_task "create_svg_tree.cpp" g++ -std=c++23 -O2 -Wall -Wextra create_svg_tree.cpp -o "$BIN_PATH/tree_cpp"
"$BIN_PATH/tree_cpp" $BRANCHES "$OUTPUT_PATH/tree_cpp.svg"

# --- Python ---
run_task "create_svg_tree.py" python3 create_svg_tree.py $BRANCHES "$OUTPUT_PATH/tree_py.svg"

# --- Perl ---
run_task "create_svg_tree.pl" perl create_svg_tree.pl $BRANCHES "$OUTPUT_PATH/tree_pl.svg"
