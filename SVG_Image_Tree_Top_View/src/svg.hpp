#pragma once

#include <format>
#include <fstream>
#include <math.h>
#include <print>
#include <string>
#include <vector>

constexpr auto PI = 3.1415926;
constexpr auto CROWN_DIAMETER = 250;    // Pixels

inline auto calculate_x(double value, double radius, int angle) -> double
{
    return value + radius * cos(angle * PI / 180);
}

inline auto calculate_y(double value, double radius, int angle) -> double
{
    return value + radius * sin(angle * PI / 180);
}

constexpr std::string BRANCH_LABEL = "Branch";
constexpr std::string LEAF_LABEL = "Leaf";

class SVG {

public:

    static auto svg(int width, int height, const std::string_view figure) -> const std::string
    {
        return {
            std::format(
                "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
                "<svg\n"
                "   xmlns:dc=\"http://purl.org/dc/elements/1.1/\"\n"
                "   xmlns:cc=\"http://creativecommons.org/ns#\"\n"
                "   xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n"
                "   xmlns:svg=\"http://www.w3.org/2000/svg\"\n"
                "   xmlns=\"http://www.w3.org/2000/svg\"\n"
                "   xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n"
                "   width=\" {} \"\n"
                "   height=\" {} \"\n"
                "   viewBox= \"0 0 {} {} \"\n"
                "   version=\"1.1\"\n"
                "   id=\"svg8\">\n"
                "  <title\n"
                "     id=\"title1\">Tree Top View</title>\n"
                "  <defs\n"
                "     id=\"defs1\" />\n"
                "  <metadata\n"
                "     id=\"metadata1\">\n"
                "    <rdf:RDF>\n"
                "      <cc:Work\n"
                "         rdf:about=\"\">\n"
                "        <dc:format>image/svg+xml</dc:format>\n"
                "        <dc:type\n"
                "           rdf:resource=\"http://purl.org/dc/dcmitype/StillImage\" />\n"
                "        <dc:title>Tree Top View</dc:title>\n"
                "        <dc:subject>\n"
                "          <rdf:Bag>\n"
                "            <rdf:li>tree</rdf:li>\n"
                "            <rdf:li>plant</rdf:li>\n"
                "            <rdf:li>nature</rdf:li>\n"
                "            <rdf:li>landscaping</rdf:li>\n"
                "          </rdf:Bag>\n"
                "        </dc:subject>\n"
                "        <dc:creator>\n"
                "          <cc:Agent>\n"
                "            <dc:title>https://github.com/jpenrici/SVG_Image_Tree/tree/master/SVG_Image_Tree_Top_View</dc:title>\n"
                "          </cc:Agent>\n"
                "        </dc:creator>\n"
                "        <cc:license\n"
                "           rdf:resource=\"http://creativecommons.org/publicdomain/zero/1.0/\" />\n"
                "        <dc:description>SVG tree image in top view created automatically by algorithm in C++. Leaves are clones of the original figure placed in the upper left corner when SVG is opened in Inkscape.</dc:description>\n"
                "      </cc:Work>\n"
                "      <cc:License\n"
                "         rdf:about=\"http://creativecommons.org/publicdomain/zero/1.0/\">\n"
                "        <cc:permits\n"
                "           rdf:resource=\"http://creativecommons.org/ns#Reproduction\" />\n"
                "        <cc:permits\n"
                "           rdf:resource=\"http://creativecommons.org/ns#Distribution\" />\n"
                "        <cc:permits\n"
                "           rdf:resource=\"http://creativecommons.org/ns#DerivativeWorks\" />\n"
                "      </cc:License>\n"
                "    </rdf:RDF>\n"
                "  </metadata>\n"
                "  <!--      Created in C++ algorithm       -->\n"
                "  <!-- Attention: do not modify this code. -->\n"
                "\n"
                " {} "
                "\n"
                "  <!-- Attention: do not modify this code. -->\n"
                "</svg>",
                width, height, width, height, figure)
        };
    }

    static auto clone(std::string_view reference, int angle, double center_X, double center_Y, double new_X,
                      double new_Y) -> const std::string
    {
        static int counter = 0;

        new_X += center_X < 0.0 ? -center_X : 0.0;
        new_Y += center_Y < 0.0 ? -center_Y : 0.0;

        // Matrix - Rotate and Translate
        double a = angle * PI / 180.0;
        std::vector<double> matrix{
            cos(a), -sin(a), -center_X * cos(a) + center_Y * sin(a) + center_X + new_X,
            sin(a),  cos(a), -center_X * sin(a) - center_Y * cos(a) + center_Y + new_Y,
            0,       0,      1
        };
        std::string transform;
        for (std::vector<size_t> index{ 0, 3, 1, 4, 2, 5 }; auto &i : index) {
            transform += std::format("{} ", matrix[i]);
        }

        std::string label = std::format("Clone_{}_{}", reference, counter++);

        return {
            std::format(
                "     <use \n"
                "        x=\"0\"\n"
                "        y=\"0\"\n"
                "        xlink:href=\"#{} \"\n"
                "        id=\"{} \"\n"
                "        transform=\"matrix({})\"\n"
                "        width=\"100%\"\n"
                "        height=\"100%\" />", reference, label, transform)
        };
    }

    static auto line(double x, double y, double length, int angle, int inner_angle,
                     std::string_view label = "") -> const std::string
    {
        static int counter = 0;
        label = label.empty() ? "#Path" : std::format("{}_{}", label, counter++);

        std::string p1 = std::format("{},{}", x, y);
        std::string p2 = std::format("{},{}", calculate_x(x, length, angle),
                                     calculate_y(y, length, angle));
        std::string c1 = std::format(" {},{}", calculate_x(x, length / 2, angle + inner_angle),
                                     calculate_y(y, length / 2, angle + inner_angle));
        std::string c2 = std::format(" {},{}", calculate_x(x, length / 2, angle - inner_angle),
                                     calculate_y(y, length / 2, angle - inner_angle));

        return {
            std::format(
                "     <path\n"
                "        id=\" {} \"\n"
                "        style=\"opacity:1.0;fill:#00FF00;stroke:#000000;stroke-width:0.5;stroke-opacity:1\"\n"
                "        d=\"M {} C {} {} {} {} Z\" />",
                label, p1, c1, p2, c2, p1)
        };
    }

    static void save(const std::string_view text, std::string_view filename = "")
    {
        if (filename.empty()) {
            filename = "out";
        }

        if (!filename.ends_with(".svg")) {
            filename = std::format("{}.svg", filename);
        }

        try {
            std::ofstream file(std::string(filename), std::ios::out);
            file << text;
            file.close();
        }
        catch (const std::exception &e) {
            std::println("Error handling file writing.\n{}", e.what());
        }
    }

};

struct Point {
    double x = 0.0;
    double y = 0.0;
    Point(double x, double y) : x(x), y(y) {};
};

struct Line {
    double x0 = 0.0;
    double y0 = 0.0;
    double x1 = 0.0;
    double y1 = 0.0;
    double length = 0.0;
    int angle = 0;
    std::string label = "";

    Line() {};

    Line(Point origin, double length, int angle, std::string_view label = "")
        : x0(origin.x),
          y0(origin.y),
          x1(calculate_x(origin.x, length, angle)),
          y1(calculate_y(origin.y, length, angle)),
          length(length),
          angle(angle),
          label(label.empty() ? "Line" : label) {};

    auto intersection(Line line_1, Line line_2, double &xi, double &yi) -> bool
    {
        // Line 1 (x1, y1) - (x2, y2)
        double x1 = line_1.x0;
        double y1 = line_1.y0;
        double x2 = line_1.x1;
        double y2 = line_1.y1;

        // Line 2 (x3, y3) - (x4, y4)
        double x3 = line_2.x0;
        double y3 = line_2.y0;
        double x4 = line_2.x1;
        double y4 = line_2.y1;

        double d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
        if (d == 0) {   // Two lines are parallel or coincident ...
            return false;
        }

        double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / d;
        double u = ((x1 - x3) * (y1 - y2) - (y1 - y3) * (x1 - x2)) / d;

        if (t >= 0.0 && t <= 1.0 && u >= 0 && u <= 1.0) {
            xi = (x0 + t * (x1 - x0));
            yi = (y0 + t * (y1 - y0));
            return true;
        }

        // Lines do not intersect
        return false;
    }

    auto intersection(Line line_2, double xi = 0.0, double yi = 0.0) -> bool
    {
        return intersection(*this, line_2, xi, yi);
    }

    auto intersection(std::vector<Line> &lines) -> bool
    {
        for (auto &e : lines) {
            if (intersection(e)) {
                return true;
            }
        }

        return false;
    }

    auto info() -> const std::string
    {
        return std::format("{}({},{})-({},{}): A = {} : L = {}", label, x0, y0, x1, y1, angle, length);
    }

    auto svg(int inner_angle = 0) -> const std::string
    {
        return SVG::line(x0, y0, length, angle, inner_angle, label);
    }

};
