#pragma once

#include "geometry.hpp"

#include <random>
#include <string>
#include <vector>

namespace tree_svg {

struct TreeOptions {
    int crown_diameter = 250;
    int branch_segments = 8;
    int leaf_angle_degrees = 30;
    unsigned seed = std::random_device{}();
};

class Tree {
public:
    explicit Tree(std::string filename);

    [[nodiscard]] auto create(const TreeOptions &options) -> bool;

private:
    std::string filename_;

    class Branch {
    public:
        Branch(Point origin, double length, double angle_degrees, int segment_count,
              int leaf_angle_degrees, std::mt19937 &rng);

        [[nodiscard]] auto segments() const -> const std::vector<Line> &;
        [[nodiscard]] auto leaves() const -> const std::vector<Line> &;

    private:
        std::vector<Line> segments_;
        std::vector<Line> leaves_;

        void grow(Line current, int remaining_segments, double leaf_length, int leaf_angle_degrees,
                  std::mt19937 &rng);
    };
};

} // namespace tree_svg
