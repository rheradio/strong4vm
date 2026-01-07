/*
 *  api_example.cc
 *
 *  Example demonstrating how to use the Dimacs2GraphsAPI
 */

#include "Dimacs2GraphsAPI.hh"
#include <iostream>
#include <string>

using namespace std;
using namespace dimacs2graphs;

int main(int argc, char** argv) {
    cout << "=== Dimacs2GraphsAPI Example ===" << endl << endl;

    // Example 1: Basic usage with default settings
    cout << "Example 1: Basic usage" << endl;
    cout << "----------------------" << endl;

    Dimacs2GraphsAPI api1;

    // Process a DIMACS file with default settings (detector="one", output to same directory)
    if (api1.generate_graphs("bonedigger/examples/kconfig/fiasco")) {
        cout << "Success!" << endl;
        cout << "Variables: " << api1.get_num_variables() << endl;
        cout << "Clauses: " << api1.get_num_clauses() << endl;
        cout << "Backbone size: " << api1.get_global_backbone().size() << endl;
    } else {
        cerr << "Failed: " << api1.get_error_message() << endl;
    }
    cout << endl;

    // Example 2: Specify output folder
    cout << "Example 2: Custom output folder" << endl;
    cout << "--------------------------------" << endl;

    Dimacs2GraphsAPI api2;

    // Generate graphs and save to a specific output folder
    if (api2.generate_graphs("bonedigger/examples/kconfig/fiasco", "output_graphs")) {
        cout << "Success! Graphs saved to output_graphs/" << endl;

        // Access backbone information
        auto backbone = api2.get_global_backbone();
        cout << "Global backbone literals: ";
        for (size_t i = 0; i < min(backbone.size(), size_t(10)); i++) {
            cout << backbone[i] << " ";
        }
        if (backbone.size() > 10) {
            cout << "... (" << backbone.size() << " total)";
        }
        cout << endl;
    } else {
        cerr << "Failed: " << api2.get_error_message() << endl;
    }
    cout << endl;

    // Example 3: Use different detector
    cout << "Example 3: Using 'without' detector" << endl;
    cout << "------------------------------------" << endl;

    Dimacs2GraphsAPI api3;

    // Use the "without" detector (no activity bumping)
    if (api3.generate_graphs("bonedigger/examples/kconfig/fiasco", "", "without")) {
        cout << "Success using 'without' detector!" << endl;
        cout << "Variables: " << api3.get_num_variables() << endl;
        cout << "Clauses: " << api3.get_num_clauses() << endl;
    } else {
        cerr << "Failed: " << api3.get_error_message() << endl;
    }
    cout << endl;

    // Example 4: Error handling
    cout << "Example 4: Error handling" << endl;
    cout << "-------------------------" << endl;

    Dimacs2GraphsAPI api4;

    // Try to process a non-existent file
    if (!api4.generate_graphs("nonexistent_file")) {
        cout << "Expected error occurred: " << api4.get_error_message() << endl;
    }
    cout << endl;

    cout << "=== Examples completed ===" << endl;

    return 0;
}
