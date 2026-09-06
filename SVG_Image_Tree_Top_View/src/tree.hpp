#pragma once

#include "svg.hpp"

class Tree {

public:

    Tree(const std::string_view filename);
    ~Tree();

    void create(int crown_diameter);

private:

    const std::string filename;

    class Branch {

    public:

        Branch(double x, double y, double length, int angle, int sub_branches, std::string_view label);

        void prepare(double leaf_length, int leaf_angle);
        auto label() -> std::string;
        auto leafs() -> std::vector<Line>;
        auto branches() -> std::vector<Line>;
        auto info(std::string_view key) -> std::string;
        auto svg(std::string_view key) -> std::string;

    private:

        Line branch;
        int sub_branches;

        std::vector<Line> v_leafs;
        std::vector<Line> v_sub_branches;

        void ramify(double leaf_length, int leaf_angle);
        auto select(std::string_view key) -> std::vector<Line>;
    };
};
