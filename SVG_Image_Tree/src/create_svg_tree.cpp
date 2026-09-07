/*
 * Generate an SVG drawing of a simple fractal tree (front view).
 *
 * Build: g++ -std=c++23 -O2 -Wall -Wextra tree.cpp -o tree
 *
 * Run:   ./tree [branches] [output.svg]
 *        ./tree 4 tree.svg
 */
#include <cmath>
#include <cstdlib>
#include <format>
#include <fstream>
#include <iostream>
#include <numbers>
#include <ranges>
#include <string>

namespace Tree {

struct Config {
    int viewbox_w = 400, viewbox_h = 400;
    double stem_height = 20.0;
    double initial_radius = 250.0;
    double initial_angle = 90.0;
    std::string line_color = "#2B1100";
    int line_width = 5;
    std::string leaf_color = "#00A000";
};

const Config cfg {};

struct Point {
    double x = 0, y = 0;

    [[nodiscard]] auto moved(double radius, double angle_deg) const -> Point;
};

class SvgDocument {
public:
    explicit SvgDocument(const Config& c);

    void add_line(const std::string& id, Point p0, Point p1);
    void add_leaf(const std::string& id, Point c, double rx, double ry);
    void save(const std::string& filename);

private:
    [[nodiscard]] auto flip_y(double y) const -> double;

    const Config& cfg_;
    std::string buffer_;
}; // class SvgDocument

void ramify(const std::string& id, Point p0, double radius, double angle, int branches, SvgDocument& doc);
void generate(int branches, const std::string& output);

} // namespace Tree

int main(int argc, char** argv)
{
    const int branches = (argc > 1) ? std::atoi(argv[1]) : 4;
    const std::string output = (argc > 2) ? argv[2] : "tree.svg";

    if (branches <= 0) {
        std::cerr << "branches must be a positive integer\n";
        return EXIT_FAILURE;
    }

    Tree::generate(branches, output);
    return EXIT_SUCCESS;
}

auto Tree::Point::moved(double radius, double angle_deg) const -> Point
{
    const double rad = angle_deg * std::numbers::pi / 180.0;
    return { x + radius * std::cos(rad), y + radius * std::sin(rad) };
}

Tree::SvgDocument::SvgDocument(const Config& c)
    : cfg_ { c }
{
    buffer_ += std::format(
        "<?xml version=\"1.0\" standalone=\"no\"?>\n"
        "<svg width=\"{}\" height=\"{}\" viewBox=\"0 0 {} {}\"\n"
        "\txmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n"
        "\t<title>Tree SVG - C++</title>\n",
        cfg_.viewbox_w, cfg_.viewbox_h, cfg_.viewbox_w, cfg_.viewbox_h);
}

void Tree::SvgDocument::add_line(const std::string& id, Point p0, Point p1)
{
    buffer_ += std::format(
        "\t<path\n"
        "\t\tstyle=\"fill:none;stroke:{};stroke-width:{};"
        "stroke-linecap:round;stroke-linejoin:round\"\n"
        "\t\td=\"M {:.2f},{:.2f} {:.2f},{:.2f}\"\n"
        "\t\tid=\"{}\" />\n",
        cfg_.line_color, cfg_.line_width,
        p0.x, flip_y(p0.y), p1.x, flip_y(p1.y), id);
}

void Tree::SvgDocument::add_leaf(const std::string& id, Point c, double rx, double ry)
{
    buffer_ += std::format(
        "\t<ellipse\n"
        "\t\tstyle=\"opacity:0.6;fill:{};stroke:none\"\n"
        "\t\trx=\"{:.2f}\" ry=\"{:.2f}\" cx=\"{:.2f}\" cy=\"{:.2f}\"\n"
        "\t\tid=\"leaf_{}\" />\n",
        cfg_.leaf_color, rx, ry, c.x, flip_y(c.y), id);
}

void Tree::SvgDocument::save(const std::string& filename)
{
    buffer_ += "</svg>\n";
    std::ofstream out { filename };
    out << buffer_;
}

auto Tree::SvgDocument::flip_y(double y) const -> double { return cfg_.viewbox_h - y; }

void Tree::ramify(const std::string& id, Point p0, double radius, double angle, int branches, SvgDocument& doc)
{
    const Point p1 = p0.moved(radius, angle);

    doc.add_line(id, p0, p1);
    doc.add_leaf(id, p1, branches * 20.0, branches * 10.0);

    if (branches <= 1)
        return;

    const double step_x = (p1.x - p0.x) / branches;
    const double step_y = (p1.y - p0.y) / branches;
    const double child_radius = radius / branches;

    auto division_points = std::views::iota(1, branches)
        | std::views::transform([&](int i) {
              return Point { p0.x + i * step_x, p0.y + i * step_y };
          });

    int i = 1;
    for (const Point& p : division_points) {
        const std::string child_id = std::format("{}_{}", id, i++);
        ramify(child_id, p, child_radius, angle + 45, branches - 1, doc);
        ramify(child_id, p, child_radius, angle - 45, branches - 1, doc);
    }
}

void Tree::generate(int branches, const std::string& output)
{
    SvgDocument doc { cfg };
    const double mid_x = cfg.viewbox_w / 2.0;

    doc.add_line("ground", { 0, 0 }, { static_cast<double>(cfg.viewbox_w), 0 });
    doc.add_line("trunk", { mid_x, 0 }, { mid_x, cfg.stem_height });
    ramify("branch", { mid_x, cfg.stem_height }, cfg.initial_radius,
        cfg.initial_angle, branches, doc);

    doc.save(output);
    std::cout << std::format("Check {}\nFinished.\n", output);
}
