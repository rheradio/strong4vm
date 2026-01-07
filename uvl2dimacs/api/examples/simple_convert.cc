/**
 * @file simple_convert.cc
 * @brief Simple example of using the UVL2Dimacs API
 *
 * This example demonstrates the basic usage of the UVL2Dimacs library API
 * for converting UVL files to DIMACS format.
 */

#include "uvl2dimacs/UVL2Dimacs.hh"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    // Check arguments
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input.uvl> <output.dimacs>" << std::endl;
        std::cerr << std::endl;
        std::cerr << "Example:" << std::endl;
        std::cerr << "  " << argv[0] << " model.uvl model.dimacs" << std::endl;
        return 1;
    }

    std::string input_file = argv[1];
    std::string output_file = argv[2];

    // Create converter instance
    uvl2dimacs::UVL2Dimacs converter;

    // Enable verbose output to see progress
    converter.set_verbose(true);

    // Set conversion mode (optional - defaults to STRAIGHTFORWARD)
    converter.set_mode(uvl2dimacs::ConversionMode::STRAIGHTFORWARD);

    std::cout << "Converting " << input_file << " to " << output_file << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << std::endl;

    // Perform conversion
    auto result = converter.convert(input_file, output_file);

    // Check result
    if (result.success) {
        std::cout << std::endl;
        std::cout << "✓ Conversion successful!" << std::endl;
        std::cout << std::endl;
        std::cout << "Statistics:" << std::endl;
        std::cout << "  Input Feature Model:" << std::endl;
        std::cout << "    - Features:    " << result.num_features << std::endl;
        std::cout << "    - Relations:   " << result.num_relations << std::endl;
        std::cout << "    - Constraints: " << result.num_constraints << std::endl;
        std::cout << std::endl;
        std::cout << "  Output CNF Formula:" << std::endl;
        std::cout << "    - Variables:   " << result.num_variables << std::endl;
        std::cout << "    - Clauses:     " << result.num_clauses << std::endl;
        return 0;
    } else {
        std::cerr << std::endl;
        std::cerr << "✗ Conversion failed!" << std::endl;
        std::cerr << "Error: " << result.error_message << std::endl;
        return 1;
    }
}
