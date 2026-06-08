/**
 * @file tseitin_convert.cc
 * @brief Example demonstrating Tseitin transformation
 *
 * This example shows how to use Tseitin mode to convert UVL feature models
 * to CNF DIMACS format.
 */

#include "uvl2dimacs/UVL2Dimacs.hh"
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 3 || argc > 4) {
        std::cerr << "Usage: " << argv[0] << " [-b] <input.uvl> <output.dimacs>" << std::endl;
        std::cerr << std::endl;
        std::cerr << "Converts UVL feature model to DIMACS using Tseitin transformation." << std::endl;
        std::cerr << std::endl;
        std::cerr << "Options:" << std::endl;
        std::cerr << "  -b    Apply backbone simplification" << std::endl;
        std::cerr << std::endl;
        std::cerr << "Examples:" << std::endl;
        std::cerr << "  " << argv[0] << " model.uvl model_tseitin.dimacs" << std::endl;
        std::cerr << "  " << argv[0] << " -b model.uvl model_tseitin_simplified.dimacs" << std::endl;
        return 1;
    }

    bool use_backbone = false;
    int arg_index = 1;

    // Parse -b flag
    if (argc == 4 && std::string(argv[1]) == "-b") {
        use_backbone = true;
        arg_index = 2;
    }

    std::string input_file = argv[arg_index];
    std::string output_file = argv[arg_index + 1];

    // Display header
    std::cout << "╔══════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║         UVL2Dimacs - Tseitin Transformation Example         ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════════╝" << std::endl;
    std::cout << std::endl;
    std::cout << "Input:  " << input_file << std::endl;
    std::cout << "Output: " << output_file << std::endl;
    std::cout << "Mode:   Tseitin" << std::endl;
    if (use_backbone) {
        std::cout << "Backbone simplification: ENABLED" << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Converting..." << std::endl;
    std::cout << std::string(60, '-') << std::endl;

    // Create converter and configure for Tseitin mode
    uvl2dimacs::UVL2Dimacs converter;
    converter.set_verbose(true);
    converter.set_mode(uvl2dimacs::ConversionMode::TSEITIN);  // Use Tseitin transformation

    if (use_backbone) {
        converter.set_backbone_simplification(true);
    }

    // Perform conversion
    auto result = converter.convert(input_file, output_file);

    std::cout << std::string(60, '-') << std::endl;

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
