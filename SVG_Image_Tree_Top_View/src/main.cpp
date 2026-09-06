#include "tree.hpp"

#include <print>

auto main(int argc, char const *argv[]) -> int
{
    // Default
    std::string filename = "tree_top_view";
    int crown_diameter = CROWN_DIAMETER;

    // Arguments
    std::string arg_filename = "filename=";
    std::string arg_diameter = "diameter=";
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        // Check filename
        if (arg.size() > arg_filename.size()) {
            if (arg.substr(0, arg_filename.size()) == arg_filename) {
                filename = arg.substr(arg_filename.size(), arg.size() - arg_filename.size());
            }
        }
        // Check diameter
        if (arg.size() > arg_diameter.size()) {
            if (arg.substr(0, arg_diameter.size()) == arg_diameter) {
                int value = atoi((arg.substr(arg_diameter.size(), arg.size() - arg_diameter.size())).c_str());
                if (value > crown_diameter) {
                    crown_diameter = value;
                }
                else {
                    std::println("Crown diameter too small or invalid. Use default ...");
                }
            }
        }
    }

    std::println("Create Tree SVG image ...");
    std::println("Filename: {}.svg", filename);
    std::println("Crown diameter: {}", crown_diameter);

    Tree svg_tree(filename);
    svg_tree.create(crown_diameter);

    std::println("Finished.\n");

    return 0;
}
