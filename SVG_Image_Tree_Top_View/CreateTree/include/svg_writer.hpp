#pragma once

#include "geometry.hpp"

#include <string>
#include <string_view>

namespace tree_svg {

class SvgWriter {
public:
    [[nodiscard]] static auto document(int width, int height, std::string_view body) -> std::string;
    [[nodiscard]] static auto shape(const Line &line, double curvature_degrees) -> std::string;
    [[nodiscard]] static auto clone(std::string_view reference_id, double angle_degrees, Point pivot,
                                    Point target) -> std::string;
    static auto save(std::string_view contents, std::string_view filename) -> bool;

private:
    static inline int shape_counter_ = 0;
    static inline int clone_counter_ = 0;
};

} // namespace tree_svg
