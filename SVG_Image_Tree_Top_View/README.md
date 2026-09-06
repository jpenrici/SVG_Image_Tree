# SVG_Image_Tree_Top_View

Top-view SVG tree generator, with randomly grown branches and leaves.

## Building

Requires a C++26-capable compiler (developed and tested with GCC 16).

```sh
cmake -S . -B build
cmake --build build
```

The binary ends up at `build/create_svg_tree`.

## Usage

```sh
./build/create_svg_tree --help
```

```
Usage: ./build/create_svg_tree [options]

Generates a stylised, top-view SVG tree with randomly grown branches and leaves.

Options:
  -o, --output <name>       Output file (default: tree_top_view.svg;
                             '.svg' is appended automatically if missing)
  -d, --diameter <pixels>   Crown diameter in pixels, minimum 50 (default: 250)
  -s, --segments <count>    Segments per main branch, minimum 1 (default: 8)
  -a, --leaf-angle <deg>    Angle leaves/forks split off a branch (default: 30)
      --seed <number>       Random seed, for a reproducible tree
                             (default: a different tree every run)
  -h, --help                Show this help and exit
```

| Option                    | Short | Default             | Description                                              |
| ------------------------- | :---: | -------------------- | ---------------------------------------------------------- |
| `--output <name>`         | `-o`  | `tree_top_view`       | Output file (`.svg` is appended if missing)                 |
| `--diameter <px>`         | `-d`  | `250`                 | Crown diameter in pixels (minimum 50)                        |
| `--segments <n>`          | `-s`  | `8`                   | Segments per main branch (minimum 1)                         |
| `--leaf-angle <degrees>`  | `-a`  | `30`                  | Angle leaves/forks split away at                             |
| `--seed <n>`              | —     | random                | Random generator seed, to reproduce a tree                  |
| `--help`                  | `-h`  | —                     | Show help and exit                                           |

Both `--option value` and `--option=value` forms are accepted. Invalid or unknown arguments print a clear error message and the help text, instead of silently falling back to a default.

Examples:

```sh
./build/create_svg_tree
./build/create_svg_tree --output garden_oak --diameter 400
./build/create_svg_tree -o oak -d 400 -s 10 --seed 42
```

Every run prints the seed it used:

```
Creating tree SVG image...
  Output:         oak.svg
  Crown diameter: 400 px
  Branch segments: 8
  Leaf angle:     30 deg
  Seed:           42 (pass --seed 42 to reproduce this exact tree)
Done.
```

Running again with `--seed 42` (and the same other parameters) reproduces the exact same `.svg` file, byte for byte.

## Display

![example](https://github.com/jpenrici/SVG_Image_Tree/blob/master/SVG_Image_Tree_Top_View/tree_top_view.svg)
