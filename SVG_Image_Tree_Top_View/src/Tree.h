#pragma once

#include "Utils.h"

class Tree {

public:

	Tree(const string filename);
	~Tree();

	void create(int crown_diameter);

private:

	const string filename;

	class Branch {

	public:

		Branch(double x, double y, double length, int angle, int sub_branches, string label);

		void prepare(double leaf_length, int leaf_angle);
		string label();
		vector<Line> leafs();
		vector<Line> branches();
		string info(string key);
		string svg(string key);

	private:

		Line branch;
		int sub_branches;

		vector<Line> v_leafs;
		vector<Line> v_sub_branches;

		void ramify(double leaf_length, int leaf_angle);
		vector<Line> select(string key);
	};
};
