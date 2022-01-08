#include "Tree.h"

using namespace std;

int main(int argc, char const* argv[])
{
	// Default
	string filename = "tree_top_view";
	int crown_diameter = CROWN_DIAMETER;

	// Arguments
	string arg_filename = "filename=";
	string arg_diameter = "diameter=";
	for (int i = 1; i < argc; i++) {
		string arg = argv[i];
		// Check filename
		if (arg.size() > arg_filename.size()) {
			if (arg.substr(0, arg_filename.size()) == arg_filename) {
				filename = arg.substr(arg_filename.size(), arg.size() - arg_filename.size());
			}
		}
		// Check diameter
		if (arg.size() > arg_diameter.size()) {
			if (arg.substr(0, arg_diameter.size()) == arg_diameter) {
				int value = atoi((arg.substr(arg_diameter.size(), arg.size() - arg_diameter.size())).c_str());
				if (value > crown_diameter) {
					crown_diameter = value;
				}
				else {
					cout << "Crown diameter too small or invalid. Use default ...\n";
				}
			}
		}
	}

	cout << "Create Tree SVG image ...\n";
	cout << "Filename: " << filename << ".svg\n";
	cout << "Crown diameter: " << crown_diameter << "\n";

	Tree svg_tree(filename);
	svg_tree.create(crown_diameter);

	cout << "Finished.\n";

	return 0;
}
