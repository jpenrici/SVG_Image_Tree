#include "geometry.hpp"

#include <cmath>
#include <format>

namespace tree_svg {

auto polar_x(double value, double radius, double angle_degrees) -> double
{
    return value + radius * std::cos(angle_degrees * pi / 180.0);
}

auto polar_y(double value, double radius, double angle_degrees) -> double
{
    return value + radius * std::sin(angle_degrees * pi / 180.0);
}

Line::Line(Point origin, double length, double angle_degrees, ElementKind kind, std::string label)
    : origin(origin),
      end{polar_x(origin.x, length, angle_degrees), polar_y(origin.y, length, angle_degrees)},
      length(length),
      angle(angle_degrees),
      kind(kind),
      label(std::move(label))
{
    if (this->label.empty()) {
        this->label = std::string{to_string(kind)};
    }
}

auto Line::intersects(const Line &other) const -> bool
{
    const double x1 = origin.x, y1 = origin.y;
    const double x2 = end.x, y2 = end.y;
    const double x3 = other.origin.x, y3 = other.origin.y;
    const double x4 = other.end.x, y4 = other.end.y;

    const double d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (d == 0.0) {
        // Parallel, coincident, or (in practice) the very same segment.
        return false;
    }

    const double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / d;
    const double u = ((x1 - x3) * (y1 - y2) - (y1 - y3) * (x1 - x2)) / d;

    return t >= 0.0 && t <= 1.0 && u >= 0.0 && u <= 1.0;
}

auto Line::intersects_any(const std::vector<Line> &others) const -> bool
{
    for (const auto &other : others) {
        if (&other == this) {
            continue;
        }
        if (intersects(other)) {
            return true;
        }
    }
    return false;
}

auto Line::describe() const -> std::string
{
    return std::format("{}: ({:.2f},{:.2f})-({:.2f},{:.2f}) angle={:.1f} length={:.2f}",
                       label, origin.x, origin.y, end.x, end.y, angle, length);
}

auto remove_overlaps(const std::vector<Line> &lines) -> std::vector<Line>
{
    std::vector<Line> kept;
    kept.reserve(lines.size());
    for (const auto &candidate : lines) {
        if (!candidate.intersects_any(kept)) {
            kept.push_back(candidate);
        }
    }
    return kept;
}

} // namespace tree_svg
