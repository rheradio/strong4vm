// Example usage of Backbone Solver API
#include "BackboneSolverAPI.hh"
#include <iostream>
#include <string>

using namespace backbone_solver;
using std::cout;
using std::cerr;
using std::endl;
using std::string;

int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <dimacs_file> [one|without]" << endl;
        return 1;
    }

    string filename = argv[1];
    string detector_type = (argc >= 3) ? argv[2] : "one";

    BackboneSolverAPI api;

    cout << "Reading DIMACS file: " << filename << endl;
    if (!api.read_dimacs(filename)) {
        cerr << "Error: Failed to read DIMACS file" << endl;
        return 1;
    }
    cout << "Successfully read file with " << api.get_max_variable() << " variables" << endl;

    cout << "Creating " << detector_type << " backbone detector..." << endl;
    if (!api.create_backbone_detector(detector_type)) {
        cerr << "Error: Failed to create backbone detector" << endl;
        return 1;
    }

    cout << "Computing backbone..." << endl;
    auto backbone = api.compute_backbone();

    cout << "\nBackbone results:" << endl;
    cout << "Backbone size: " << backbone.size() << " literals" << endl;
    
    if (!backbone.empty()) {
        cout << "Backbone literals: ";
        for (size_t i = 0; i < backbone.size(); ++i) {
            cout << backbone[i];
            if (i < backbone.size() - 1) {
                cout << " ";
            }
        }
        cout << endl;
    }
    
    cout << "\nUsing print_backbone() method:" << endl;
    api.print_backbone();

    return 0;
}
