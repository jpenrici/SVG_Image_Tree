#pragma once

#include "tree.hpp"

#include <expected>
#include <string>
#include <string_view>
#include <vector>

namespace tree_svg {

struct CliOptions {
    std::string output_filename = "tree_top_view";
    TreeOptions tree;

    bool show_help = false;
};

[[nodiscard]] auto parse_cli(const std::vector<std::string> &args) -> std::expected<CliOptions, std::string>;

[[nodiscard]] auto usage_text(std::string_view program_name) -> std::string;

} // namespace tree_svg
