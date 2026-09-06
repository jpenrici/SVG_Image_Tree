#include "cli.hpp"
#include "tree.hpp"

#include <print>
#include <vector>

auto main(int argc, char *argv[]) -> int
{
    const std::vector<std::string> args(argv + 1, argv + argc);

    const auto options = tree_svg::parse_cli(args);
    if (!options) {
        std::println(stderr, "error: {}", options.error());
        std::println(stderr, "{}", tree_svg::usage_text(argv[0]));
        return 1;
    }

    if (options->show_help) {
        std::println("{}", tree_svg::usage_text(argv[0]));
        return 0;
    }

    std::println("Creating tree SVG image...");
    std::println("  Output:         {}.svg", options->output_filename);
    std::println("  Crown diameter: {} px", options->tree.crown_diameter);
    std::println("  Branch segments: {}", options->tree.branch_segments);
    std::println("  Leaf angle:     {} deg", options->tree.leaf_angle_degrees);
    std::println("  Seed:           {} (pass --seed {} to reproduce this exact tree)",
                 options->tree.seed, options->tree.seed);

    tree_svg::Tree tree(options->output_filename);
    if (!tree.create(options->tree)) {
        std::println(stderr, "error: could not save the tree, see above for details");
        return 1;
    }

    std::println("Done.");
    return 0;
}
