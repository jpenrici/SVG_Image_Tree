#include "tree.hpp"

#include "svg_writer.hpp"

#include <algorithm>
#include <format>

namespace tree_svg {

Tree::Tree(std::string filename) : filename_(std::move(filename)) {}

Tree::Branch::Branch(Point origin, double length, double angle_degrees, int segment_count,
                     int leaf_angle_degrees, std::mt19937 &rng)
{
    const Line trunk{origin, length, angle_degrees, ElementKind::Branch};
    segments_.push_back(trunk);
    grow(trunk, segment_count, length / 2.0, leaf_angle_degrees, rng);
}

auto Tree::Branch::segments() const -> const std::vector<Line> &
{
    return segments_;
}

auto Tree::Branch::leaves() const -> const std::vector<Line> &
{
    return leaves_;
}

void Tree::Branch::grow(Line current, int remaining_segments, double leaf_length, int leaf_angle_degrees,
                        std::mt19937 &rng)
{
    if (remaining_segments <= 0) {
        return;
    }

    const double step = current.length / remaining_segments;
    std::uniform_int_distribution<int> choice(0, 3); // 0/1: sprout a leaf, 2/3: fork a sub-branch

    for (int i = 1; i < remaining_segments; ++i) {
        const Point p{polar_x(current.origin.x, i * step, current.angle),
                      polar_y(current.origin.y, i * step, current.angle)};
        const double angle_up = current.angle + leaf_angle_degrees;
        const double angle_down = current.angle - leaf_angle_degrees;

        switch (choice(rng)) {
        case 0:
            leaves_.emplace_back(p, leaf_length, angle_up, ElementKind::Leaf);
            break;
        case 1:
            leaves_.emplace_back(p, leaf_length, angle_down, ElementKind::Leaf);
            break;
        case 2:
        case 3: {
            const double fork_angle = choice(rng) < 2 ? angle_up : angle_down;
            Line fork{p, current.length, fork_angle, ElementKind::Branch};
            segments_.push_back(fork);
            // A branch always ends in a leaf, even if it doesn't fork again.
            leaves_.emplace_back(fork.end, leaf_length, fork.angle, ElementKind::Leaf);
            grow(fork, remaining_segments - 1, leaf_length, leaf_angle_degrees, rng);
            return;
        }
        }
    }
}

auto Tree::create(const TreeOptions &options) -> bool
{
    const int crown_diameter = std::max(50, options.crown_diameter);
    const int segments = std::max(1, options.branch_segments);
    const int width = crown_diameter;
    const int height = crown_diameter;

    const double center_x = width / 2.0;
    const double center_y = height / 2.0;
    const double branch_length = (width * 0.8) / segments;
    const double leaf_length = branch_length / 2.0;
    const double trunk_radius = branch_length * 0.2;

    std::mt19937 rng(options.seed);

    const std::string leaf_template_id = "Leaf_Original";
    const Line leaf_template{Point{0.0, 0.0}, leaf_length, 0.0, ElementKind::Leaf, leaf_template_id};

    std::string branch_markup;
    std::vector<Line> all_leaves;

    for (int angle = 15; angle < 360; angle += 30) {
        const Point start{polar_x(center_x, trunk_radius, angle), polar_y(center_y, trunk_radius, angle)};
        const Branch branch{start, branch_length, static_cast<double>(angle), segments,
                            options.leaf_angle_degrees, rng};

        for (const auto &segment : branch.segments()) {
            branch_markup += SvgWriter::shape(segment, 0.0);
            branch_markup += '\n';
        }
        all_leaves.insert(all_leaves.end(), branch.leaves().begin(), branch.leaves().end());
    }

    const auto final_leaves = remove_overlaps(all_leaves);

    std::string leaf_clones;
    for (const auto &leaf : final_leaves) {
        leaf_clones += SvgWriter::clone(leaf_template_id, leaf.angle, leaf_template.origin, leaf.origin);
        leaf_clones += '\n';
    }

    const std::string leaf_definition = std::format(
                                            "  <defs>\n    <g id=\"{}\">\n{}\n    </g>\n  </defs>\n", leaf_template_id,
                                            SvgWriter::shape(leaf_template, 30.0));
    const std::string crown = std::format("  <g id=\"Tree\">\n{}\n{}  </g>\n", branch_markup, leaf_clones);

    return SvgWriter::save(SvgWriter::document(width, height, leaf_definition + crown), filename_);
}

} // namespace tree_svg
