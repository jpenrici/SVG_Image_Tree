#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <math.h>

using namespace std;

constexpr auto PI = 3.1415926;
constexpr auto CROWN_DIAMETER = 250;	// Pixels

inline double Cos(double value, double radius, int angle)
{
	return value + radius * cos(angle * PI / 180);
}

inline double Sin(double value, double radius, int angle)
{
	return value + radius * sin(angle * PI / 180);
}

const string BRANCH_LABEL = "Branch";
const string LEAF_LABEL = "Leaf";

class SVG {

public:

	static const string svg(int width, int height, const string& figure)
	{
		return {
			"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
			"<svg\n"
			"   xmlns:dc=\"http://purl.org/dc/elements/1.1/\"\n"
			"   xmlns:cc=\"http://creativecommons.org/ns#\"\n"
			"   xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n"
			"   xmlns:svg=\"http://www.w3.org/2000/svg\"\n"
			"   xmlns=\"http://www.w3.org/2000/svg\"\n"
			"   xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n"
			"   width=\"" + to_string(width) + "\"\n"
			"   height=\"" + to_string(height) + "\"\n"
			"   viewBox= \"0 0 " + to_string(width) + " " + to_string(height) + "\"\n"
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
			"" + figure + ""
			"\n"
			"  <!-- Attention: do not modify this code. -->\n"
			"</svg>"
		};
	}

	static const string clone(string reference, int angle, double center_X, double center_Y, double new_X, double new_Y)
	{
		static int counter = 0;

		new_X += center_X < 0.0 ? -center_X : 0.0;
		new_Y += center_Y < 0.0 ? -center_Y : 0.0;

		// Matrix - Rotate and Translate
		double a = angle * PI / 180.0;
		vector<double> matrix{
			cos(a), -sin(a), -center_X * cos(a) + center_Y * sin(a) + center_X + new_X,
			sin(a),  cos(a), -center_X * sin(a) - center_Y * cos(a) + center_Y + new_Y,
			0,       0,      1
		};
		string transform = "";
		for (vector<int> index{ 0, 3, 1, 4, 2, 5 }; auto & i : index) {
			transform += to_string(matrix[i]) + " ";
		}

		string label = "Clone_" + reference + "_" + to_string(counter++);

		return {
			"     <use \n"
			"        x=\"0\"\n"
			"        y=\"0\"\n"
			"        xlink:href=\"#" + reference + "\"\n"
			"        id=\"" + label + "\"\n"
			"        transform=\"matrix( " + transform + ")\"\n"
			"        width=\"100%\"\n"
			"        height=\"100%\" />"
		};
	}

	static const string line(double x, double y, double length, int angle, int inner_angle, string label = "")
	{
		static int counter = 0;

		string p1 = to_string(x) + "," + to_string(y);
		string p2 = to_string(Cos(x, length, angle)) + "," + to_string(Sin(y, length, angle));

		string c1 = to_string(Cos(x, length / 2, angle + inner_angle)) + "," + to_string(Sin(y, length / 2, angle + inner_angle));
		string c2 = to_string(Cos(x, length / 2, angle - inner_angle)) + "," + to_string(Sin(y, length / 2, angle - inner_angle));
		c1 += " " + c1;
		c2 += " " + c2;

		if (label.empty()) {
			label = "#Path";
		}
		label += "_" + to_string(counter++);

		return {
			"     <path\n"
			"        id=\"" + label + "\"\n"
			"        style=\"opacity:1.0;fill:#00FF00;stroke:#000000;stroke-width:0.5;stroke-opacity:1\"\n"
			"        d=\"M " + p1 + " C " + c1 + " " + p2 + " " + c2 + " " + p1 + " Z\" />"
		};
	}

	static void save(const string& text, string filename = "")
	{
		if (filename.empty()) {
			filename = "out";
		}

		try {
			ofstream file(filename + ".svg", ios::out);
			file << text;
			file.close();
		}
		catch (const exception& e) {
			cout << "Error handling file writing.\n";
			cerr << e.what() << "\n";
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
	string label = "";

	Line() {};

	Line(Point origin, double length, int angle, string label = "")
		: x0(origin.x),
		y0(origin.y),
		x1(Cos(origin.x, length, angle)),
		y1(Sin(origin.y, length, angle)),
		length(length),
		angle(angle),
		label(label.empty() ? "Line" : label) {};

	bool intersection(Line line_1, Line line_2, double& xi, double& yi)
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

	bool intersection(Line line_2, double xi = 0.0, double yi = 0.0)
	{
		return intersection(*this, line_2, xi, yi);
	}

	bool intersection(vector<Line>& lines)
	{
		for (auto& e : lines) {
			if (intersection(e)) {
				return true;
			}
		}

		return false;
	}

	const string info()
	{
		return label + " (" + to_string(x0) + "," + to_string(y0) + ")-("
			+ to_string(x1) + "," + to_string(y1) + "): A = "
			+ to_string(angle) + " : L = " + to_string(length);
	}

	const string svg(int inner_angle = 0)
	{
		return SVG::line(x0, y0, length, angle, inner_angle, label);
	}

};
