#pragma once

#include <numbers>
#include <string>
#include <string_view>
#include <vector>

namespace tree_svg {

inline constexpr double pi = std::numbers::pi_v<double>;

struct Point {
    double x = 0.0;
    double y = 0.0;
};

[[nodiscard]] auto polar_x(double value, double radius, double angle_degrees) -> double;
[[nodiscard]] auto polar_y(double value, double radius, double angle_degrees) -> double;

enum class ElementKind {
    Branch,
    Leaf,
};

[[nodiscard]] constexpr auto to_string(ElementKind kind) -> std::string_view
{
    switch (kind) {
    case ElementKind::Branch:
        return "Branch";
    case ElementKind::Leaf:
        return "Leaf";
    }
    return "Unknown";
}

struct Line {
    Point origin{};
    Point end{};
    double length = 0.0;
    double angle = 0.0;
    ElementKind kind = ElementKind::Branch;
    std::string label;

    Line() = default;
    Line(Point origin, double length, double angle_degrees, ElementKind kind,
         std::string label = "");

    [[nodiscard]] auto intersects_any(const std::vector<Line> &others) const -> bool;
    [[nodiscard]] auto describe() const -> std::string;

private:
    [[nodiscard]] auto intersects(const Line &other) const -> bool;
};

[[nodiscard]] auto remove_overlaps(const std::vector<Line> &lines) -> std::vector<Line>;

} // namespace tree_svg
