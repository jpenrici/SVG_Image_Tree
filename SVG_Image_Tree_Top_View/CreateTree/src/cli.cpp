#include "cli.hpp"

#include <charconv>
#include <format>

namespace tree_svg {

namespace {

[[nodiscard]] auto parse_int(std::string_view flag, std::string_view text) -> std::expected<int, std::string>
{
    int value = 0;
    const auto *begin = text.data();
    const auto *end = text.data() + text.size();
    const auto result = std::from_chars(begin, end, value);

    if (result.ec != std::errc{} || result.ptr != end) {
        return std::unexpected(std::format("{} expects an integer, got '{}'", flag, text));
    }
    return value;
}

struct SplitFlag {
    std::string_view flag;
    std::string_view inline_value;
    bool has_inline_value = false;
};

[[nodiscard]] auto split_flag(std::string_view arg) -> SplitFlag
{
    if (const auto pos = arg.find('='); pos != std::string_view::npos) {
        return {arg.substr(0, pos), arg.substr(pos + 1), true};
    }
    return {arg, {}, false};
}

} // namespace

auto usage_text(std::string_view program_name) -> std::string
{
    return std::format(
               "Usage: {0} [options]\n"
               "\n"
               "Generates a stylised, top-view SVG tree with randomly grown branches and leaves.\n"
               "\n"
               "Options:\n"
               "  -o, --output <name>       Output file (default: tree_top_view.svg;\n"
               "                             '.svg' is appended automatically if missing)\n"
               "  -d, --diameter <pixels>   Crown diameter in pixels, minimum 50 (default: {1})\n"
               "  -s, --segments <count>    Segments per main branch, minimum 1 (default: {2})\n"
               "  -a, --leaf-angle <deg>    Angle leaves/forks split off a branch (default: {3})\n"
               "      --seed <number>       Random seed, for a reproducible tree\n"
               "                             (default: a different tree every run)\n"
               "  -h, --help                Show this help and exit\n"
               "\n"
               "Examples:\n"
               "  {0}\n"
               "  {0} --output garden_oak --diameter 400\n"
               "  {0} -o oak -d 400 -s 10 --seed 42\n",
               program_name, TreeOptions{}.crown_diameter, TreeOptions{}.branch_segments,
               TreeOptions{}.leaf_angle_degrees);
}

auto parse_cli(const std::vector<std::string> &args) -> std::expected<CliOptions, std::string>
{
    CliOptions options;

    auto take_value = [&](std::size_t &index, const SplitFlag & split) -> std::expected<std::string_view, std::string> {
        if (split.has_inline_value)
        {
            return split.inline_value;
        }
        if (index + 1 >= args.size())
        {
            return std::unexpected(std::format("{} expects a value", split.flag));
        }
        return args[++index];
    };

    for (std::size_t i = 0; i < args.size(); ++i) {
        const SplitFlag split = split_flag(args[i]);
        const std::string_view flag = split.flag;

        if (flag == "-h" || flag == "--help") {
            options.show_help = true;
            return options;
        }
        if (flag == "-o" || flag == "--output") {
            const auto value = take_value(i, split);
            if (!value) {
                return std::unexpected(value.error());
            }
            options.output_filename = std::string{*value};
            continue;
        }
        if (flag == "-d" || flag == "--diameter") {
            const auto value = take_value(i, split);
            if (!value) {
                return std::unexpected(value.error());
            }
            const auto parsed = parse_int("--diameter", *value);
            if (!parsed) {
                return std::unexpected(parsed.error());
            }
            if (*parsed < 50) {
                return std::unexpected(std::format("--diameter must be at least 50, got {}", *parsed));
            }
            options.tree.crown_diameter = *parsed;
            continue;
        }
        if (flag == "-s" || flag == "--segments") {
            const auto value = take_value(i, split);
            if (!value) {
                return std::unexpected(value.error());
            }
            const auto parsed = parse_int("--segments", *value);
            if (!parsed) {
                return std::unexpected(parsed.error());
            }
            if (*parsed < 1) {
                return std::unexpected(std::format("--segments must be at least 1, got {}", *parsed));
            }
            options.tree.branch_segments = *parsed;
            continue;
        }
        if (flag == "-a" || flag == "--leaf-angle") {
            const auto value = take_value(i, split);
            if (!value) {
                return std::unexpected(value.error());
            }
            const auto parsed = parse_int("--leaf-angle", *value);
            if (!parsed) {
                return std::unexpected(parsed.error());
            }
            options.tree.leaf_angle_degrees = *parsed;
            continue;
        }
        if (flag == "--seed") {
            const auto value = take_value(i, split);
            if (!value) {
                return std::unexpected(value.error());
            }
            const auto parsed = parse_int("--seed", *value);
            if (!parsed) {
                return std::unexpected(parsed.error());
            }
            options.tree.seed = static_cast<unsigned>(*parsed);
            continue;
        }

        return std::unexpected(std::format("unknown option '{}'", args[i]));
    }

    return options;
}

} // namespace tree_svg
