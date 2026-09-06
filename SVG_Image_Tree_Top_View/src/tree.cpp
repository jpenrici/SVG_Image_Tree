#include "tree.hpp"
#include <random>

Tree::Tree(const std::string_view filename) : filename(filename) {};
Tree::~Tree() {};

void Tree::create(int crown_diameter)
{
    // Tree
    crown_diameter = std::max(CROWN_DIAMETER, crown_diameter);
    int width = crown_diameter;
    int height = width;

    // Branch
    int sub_branches = 8;
    double x0 = width / 2.0;
    double y0 = height / 2.0;
    double length = (width * 0.8) / sub_branches;

    // Leaf
    int leaf_angle = 30;
    double leaf_length = length / 2.0;

    // Leaf Original
    std::string leaf_reference = LEAF_LABEL + "_Original";
    Line leaf(Point(-leaf_length, 0.0), leaf_length, 0, leaf_reference);

    // SVG
    std::vector<Line> v_leafs;
    std::vector<Line> v_sub_branches;

    std::string svg_branches;
    std::string svg_leaf_clones;

    double radius = length * 0.2;
    for (int a = 15; a < 360; a += 30) {
        Branch branch(calculate_x(x0, radius, a),
                      calculate_y(y0, radius, a),
                      length, a, sub_branches, BRANCH_LABEL);
        branch.prepare(leaf_length, leaf_angle);
        svg_branches.append(branch.svg(BRANCH_LABEL));

        for (auto &e : branch.leafs()) {
            v_leafs.push_back(e);
        }
    }

    for (auto &e : v_leafs) {
        if (!e.intersection(v_leafs)) {
            svg_leaf_clones.append(SVG::clone(leaf_reference, e.angle, leaf.x0, leaf.y0, e.x0, e.y0));
            svg_leaf_clones.append("\n");
        }
    }

    std::string svg_leaf_original{
        "  <g id=\"" + leaf_reference + "\">\n"
        "" + leaf.svg(30) + "\n"
        "  </g>\n"
    };

    std::string svg_tree{
        "  <g id=\"G-Tree \">\n"
        "" + svg_branches + "\n"
        "" + svg_leaf_clones +
        "  </g>\n"
    };

    std::string figure;
    figure.append(svg_leaf_original);
    figure.append(svg_tree);

    SVG::save(SVG::svg(width, height, figure), filename);
}

Tree::Branch::Branch(double x, double y, double length, int angle, int sub_branches,
                     std::string_view label = "")
    : branch(Line(Point(x, y), length, angle, label)), sub_branches(sub_branches) {};

void Tree::Branch::prepare(double leaf_length, int leaf_angle)
{
    // Initialize
    v_leafs.clear();
    v_sub_branches.clear();
    v_sub_branches.push_back(branch);

    // Random
    ramify(leaf_length, leaf_angle);

    // Intersections
    std::vector<Line> temp;
    for (auto &e : v_leafs) {
        if (!e.intersection(v_leafs)) {
            temp.push_back(e);
        }
    }
    v_leafs = temp;
}

auto Tree::Branch::label() -> std::string
{
    return branch.label;
}

auto Tree::Branch::leafs() -> std::vector<Line>
{
    return v_leafs;
}

auto Tree::Branch::branches() -> std::vector<Line>
{
    return v_sub_branches;
}

auto Tree::Branch::info(std::string_view key) -> std::string
{
    std::string text;
    for (auto &e : select(key)) {
        text.append(e.info()).append("\n");
    }

    return text;
}

auto Tree::Branch::svg(std::string_view key) -> std::string
{
    std::string text;
    for (auto &e : select(key)) {
        if (e.label == LEAF_LABEL) {
            text.append(e.svg(15)).append("\n");
        }
        else {
            text.append(e.svg()).append("\n");
        }
    }

    return text;
}

void Tree::Branch::ramify(double leaf_length, int leaf_angle)
{
    // Sub-branch
    int segment = sub_branches <= 0 ? 0 : static_cast<int>(branch.length / sub_branches);

    // Random
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(1, 4);
    std::vector<int> numbers = {
        int(dist(rng)), int(dist(rng)), int(dist(rng)), int(dist(rng))
    };

    // Insert sub-branches and leafs
    for (int i = 1; i < sub_branches; ++i) {
        // Coordinate
        double x = calculate_x(branch.x0, i * segment, branch.angle);
        double y = calculate_y(branch.y0, i * segment, branch.angle);
        // New angle
        int a1 = branch.angle + leaf_angle;
        int a2 = branch.angle - leaf_angle;
        // Temp
        Line line;
        switch (numbers[static_cast<size_t>(i % 4)]) {
        // Leaf
        case 1:
            line = Line(Point(x, y), leaf_length, a1, LEAF_LABEL);
            v_leafs.push_back(line);
            break;
        case 2:
            line = Line(Point(x, y), leaf_length, a2, LEAF_LABEL);
            v_leafs.push_back(line);
            break;
        // Sub-branch
        case 3:
            line = Line(Point(x, y), branch.length, a1, BRANCH_LABEL);
            v_sub_branches.push_back(line);
            break;
        case 4:
            line = Line(Point(x, y), branch.length, a2, BRANCH_LABEL);
            v_sub_branches.push_back(line);
            break;
        };

        if (line.label == BRANCH_LABEL) {
            // Update
            branch = line;
            // Last leaf
            line = Line(Point(line.x1, line.y1), leaf_length, branch.angle, LEAF_LABEL);
            v_leafs.push_back(line);
            // Recursive
            sub_branches--;
            ramify(leaf_length, leaf_angle);
        }
    }
}

auto Tree::Branch::select(std::string_view key = "") -> std::vector<Line>
{
    std::vector<Line> temp;
    if (key.empty() || key == BRANCH_LABEL) {
        temp = v_sub_branches;
    }

    if (key.empty() || key == LEAF_LABEL) {
        for (auto &e : v_leafs) {
            temp.push_back(e);
        }
    }

    return temp;
}
