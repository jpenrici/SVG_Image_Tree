#include "Tree.h"

Tree::Tree(const string filename) : filename(filename) {};
Tree::~Tree() {};

void Tree::create(int crown_diameter)
{
	// Tree
	crown_diameter = max(CROWN_DIAMETER, crown_diameter);
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
	string leaf_reference = LEAF_LABEL + "_Original";
	Line leaf(Point(-leaf_length, 0.0), leaf_length, 0, leaf_reference);

	// SVG
	vector<Line> v_leafs;
	vector<Line> v_sub_branches;

	string svg_branches = "";
	string svg_leaf_clones = "";

	double radius = length * 0.2;
	for (int a = 15; a < 360; a += 30) {
		Branch branch(Cos(x0, radius, a), Sin(y0, radius, a), length, a, sub_branches, BRANCH_LABEL);
		branch.prepare(leaf_length, leaf_angle);
		svg_branches.append(branch.svg(BRANCH_LABEL));

		for (auto& e : branch.leafs()) {
			v_leafs.push_back(e);
		}
	}

	for (auto& e : v_leafs) {
		if (!e.intersection(v_leafs)) {
			svg_leaf_clones.append(SVG::clone(leaf_reference, e.angle, leaf.x0, leaf.y0, e.x0, e.y0));
			svg_leaf_clones.append("\n");
		}
	}

	string svg_leaf_original{
		"  <g id=\"" + leaf_reference + "\">\n"
		"" + leaf.svg(30) + "\n"
		"  </g>\n"
	};

	string svg_tree{
		"  <g id=\"G-Tree \">\n"
		"" + svg_branches + "\n"
		"" + svg_leaf_clones +
		"  </g>\n"
	};

	string figure = "";
	figure.append(svg_leaf_original);
	figure.append(svg_tree);

	SVG::save(SVG::svg(width, height, figure), filename);
}

Tree::Branch::Branch(double x, double y, double length, int angle, int sub_branches, string label = "")
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
	vector<Line> temp;
	for (auto& e : v_leafs) {
		if (!e.intersection(v_leafs)) {
			temp.push_back(e);
		}
	}
	v_leafs = temp;
}

string Tree::Branch::label()
{
	return branch.label;
}

vector<Line> Tree::Branch::leafs()
{
	return v_leafs;
}

vector<Line> Tree::Branch::branches()
{
	return v_sub_branches;
}

string Tree::Branch::info(string key = "")
{
	string text = "";
	for (auto& e : select(key)) {
		text.append(e.info()).append("\n");
	}

	return text;
}

string Tree::Branch::svg(string key = "")
{
	string text = "";
	for (auto& e : select(key)) {
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
	random_device dev;
	mt19937 rng(dev());
	uniform_int_distribution<mt19937::result_type> dist(1, 4);
	vector<int> numbers = {
		int(dist(rng)), int(dist(rng)), int(dist(rng)), int(dist(rng))
	};

	// Insert sub-branches and leafs
	for (int i = 1; i < sub_branches; ++i) {
		// Coordinate
		double x = Cos(branch.x0, i * segment, branch.angle);
		double y = Sin(branch.y0, i * segment, branch.angle);
		// New angle
		int a1 = branch.angle + leaf_angle;
		int a2 = branch.angle - leaf_angle;
		// Temp
		Line line;
		switch (numbers[i % 4]) {
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

vector<Line> Tree::Branch::select(string key = "")
{
	vector<Line> temp;
	if (key.empty() || key == BRANCH_LABEL) {
		temp = v_sub_branches;
	}

	if (key.empty() || key == LEAF_LABEL) {
		for (auto& e : v_leafs) {
			temp.push_back(e);
		}
	}

	return temp;
}