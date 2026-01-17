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
    if (argc < 3 || argc > 5) {
        std::cerr << "Usage: " << argv[0] << " [-t|-s] [-b] <input.uvl> <output.dimacs>" << std::endl;
        std::cerr << std::endl;
        std::cerr << "Options:" << std::endl;
        std::cerr << "  -s    Use straightforward conversion (default)" << std::endl;
        std::cerr << "  -t    Use Tseitin transformation (guarantees 3-CNF)" << std::endl;
        std::cerr << "  -b    Apply backbone simplification to reduce formula size" << std::endl;
        std::cerr << std::endl;
        std::cerr << "Examples:" << std::endl;
        std::cerr << "  " << argv[0] << " model.uvl model.dimacs                   # Basic conversion" << std::endl;
        std::cerr << "  " << argv[0] << " -b model.uvl model.dimacs                # With backbone" << std::endl;
        std::cerr << "  " << argv[0] << " -t model.uvl model.dimacs                # Tseitin mode" << std::endl;
        std::cerr << "  " << argv[0] << " -t -b model.uvl model.dimacs             # Tseitin + backbone" << std::endl;
        return 1;
    }

    // Parse flags
    bool use_backbone = false;
    uvl2dimacs::ConversionMode mode = uvl2dimacs::ConversionMode::STRAIGHTFORWARD;
    int arg_index = 1;

    // Parse optional flags
    for (int i = 1; i < argc - 2; i++) {
        std::string arg = argv[i];
        if (arg == "-b") {
            use_backbone = true;
            arg_index++;
        } else if (arg == "-t") {
            mode = uvl2dimacs::ConversionMode::TSEITIN;
            arg_index++;
        } else if (arg == "-s") {
            mode = uvl2dimacs::ConversionMode::STRAIGHTFORWARD;
            arg_index++;
        }
    }

    std::string input_file = argv[arg_index];
    std::string output_file = argv[arg_index + 1];

    // Create converter instance
    uvl2dimacs::UVL2Dimacs converter;

    // Enable verbose output to see progress
    converter.set_verbose(true);

    // Set conversion mode
    converter.set_mode(mode);

    // Enable backbone simplification if requested
    if (use_backbone) {
        converter.set_backbone_simplification(true);
    }

    // Display configuration
    std::cout << "Converting " << input_file << " to " << output_file << std::endl;
    std::cout << "Mode: " << (mode == uvl2dimacs::ConversionMode::TSEITIN ? "Tseitin (3-CNF)" : "Straightforward") << std::endl;
    if (use_backbone) {
        std::cout << "Backbone simplification: ENABLED" << std::endl;
    }
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
        std::cout << "    - Variables:   " << result.num_variables;

        // Show auxiliary variables if Tseitin mode introduced them
        if (mode == uvl2dimacs::ConversionMode::TSEITIN && result.num_variables > result.num_features) {
            int aux_vars = result.num_variables - result.num_features;
            std::cout << " (" << result.num_features << " features + " << aux_vars << " auxiliary)";
        }
        std::cout << std::endl;

        std::cout << "    - Clauses:     " << result.num_clauses << std::endl;

        if (mode == uvl2dimacs::ConversionMode::TSEITIN) {
            std::cout << std::endl;
            std::cout << "  Note: Tseitin mode guarantees all clauses have ≤3 literals (3-CNF)" << std::endl;
        }

        return 0;
    } else {
        std::cerr << std::endl;
        std::cerr << "✗ Conversion failed!" << std::endl;
        std::cerr << "Error: " << result.error_message << std::endl;
        return 1;
    }
}
