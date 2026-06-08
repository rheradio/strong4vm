/**
 * @file backbone_convert.cc
 * @brief Example demonstrating backbone simplification
 *
 * This example shows how to use backbone simplification to reduce the size
 * of the CNF formula produced from a UVL feature model. It converts the
 * same model twice — once without and once with backbone simplification —
 * so the reduction in variables and clauses can be observed directly.
 *
 * Backbone literals are variables that take the same value in every
 * satisfying assignment of the formula. Once identified they can be
 * substituted out, removing entire clauses (those already satisfied) and
 * shortening others, without changing the solution count.
 */

#include "uvl2dimacs/UVL2Dimacs.hh"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input.uvl> <output_base>" << std::endl;
        std::cerr << std::endl;
        std::cerr << "Converts a UVL feature model twice and prints the reduction achieved" << std::endl;
        std::cerr << "by backbone simplification." << std::endl;
        std::cerr << std::endl;
        std::cerr << "Output files:" << std::endl;
        std::cerr << "  <output_base>.dimacs            (no backbone simplification)" << std::endl;
        std::cerr << "  <output_base>_backbone.dimacs   (with backbone simplification)" << std::endl;
        std::cerr << std::endl;
        std::cerr << "Example:" << std::endl;
        std::cerr << "  " << argv[0] << " model.uvl result" << std::endl;
        return 1;
    }

    std::string input_file   = argv[1];
    std::string output_base  = argv[2];
    std::string plain_output    = output_base + ".dimacs";
    std::string backbone_output = output_base + "_backbone.dimacs";

    std::cout << "╔══════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║         UVL2Dimacs - Backbone Simplification Example        ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════════╝" << std::endl;
    std::cout << std::endl;
    std::cout << "Input:  " << input_file << std::endl;
    std::cout << std::endl;

    // --- Pass 1: without backbone simplification ---

    std::cout << "Pass 1: Converting without backbone simplification..." << std::endl;
    std::cout << std::string(60, '-') << std::endl;

    uvl2dimacs::UVL2Dimacs converter;
    converter.set_verbose(true);
    converter.set_mode(uvl2dimacs::ConversionMode::STRAIGHTFORWARD);

    auto plain = converter.convert(input_file, plain_output);

    std::cout << std::string(60, '-') << std::endl;

    if (!plain.success) {
        std::cerr << "Conversion failed: " << plain.error_message << std::endl;
        return 1;
    }

    std::cout << "Output: " << plain_output << std::endl;
    std::cout << std::endl;

    // --- Pass 2: with backbone simplification ---

    std::cout << "Pass 2: Converting with backbone simplification..." << std::endl;
    std::cout << std::string(60, '-') << std::endl;

    uvl2dimacs::UVL2Dimacs backbone_converter;
    backbone_converter.set_verbose(true);
    backbone_converter.set_mode(uvl2dimacs::ConversionMode::STRAIGHTFORWARD);
    backbone_converter.set_backbone_simplification(true);

    auto backbone = backbone_converter.convert(input_file, backbone_output);

    std::cout << std::string(60, '-') << std::endl;

    if (!backbone.success) {
        std::cerr << "Conversion failed: " << backbone.error_message << std::endl;
        return 1;
    }

    std::cout << "Output: " << backbone_output << std::endl;
    std::cout << std::endl;

    // --- Comparison ---

    int var_reduction    = plain.num_variables - backbone.num_variables;
    int clause_reduction = plain.num_clauses   - backbone.num_clauses;

    double var_pct    = plain.num_variables > 0
                        ? 100.0 * var_reduction    / plain.num_variables : 0.0;
    double clause_pct = plain.num_clauses   > 0
                        ? 100.0 * clause_reduction / plain.num_clauses   : 0.0;

    std::cout << "Feature Model:" << std::endl;
    std::cout << "  Features:    " << plain.num_features    << std::endl;
    std::cout << "  Relations:   " << plain.num_relations   << std::endl;
    std::cout << "  Constraints: " << plain.num_constraints << std::endl;
    std::cout << std::endl;
    std::cout << "CNF Comparison:" << std::endl;
    std::cout << "                      Plain       Backbone    Reduction" << std::endl;
    std::cout << "  Variables:      "
              << std::string(6 - std::to_string(plain.num_variables).size(), ' ')
              << plain.num_variables
              << "          "
              << std::string(6 - std::to_string(backbone.num_variables).size(), ' ')
              << backbone.num_variables
              << "        "
              << var_reduction << " (" << (int)var_pct << "%)" << std::endl;
    std::cout << "  Clauses:        "
              << std::string(6 - std::to_string(plain.num_clauses).size(), ' ')
              << plain.num_clauses
              << "          "
              << std::string(6 - std::to_string(backbone.num_clauses).size(), ' ')
              << backbone.num_clauses
              << "        "
              << clause_reduction << " (" << (int)clause_pct << "%)" << std::endl;
    std::cout << std::endl;
    std::cout << "About Backbone Simplification:" << std::endl;
    std::cout << "  Backbone literals are variables whose value is the same in every" << std::endl;
    std::cout << "  satisfying assignment. Once identified they can be fixed and" << std::endl;
    std::cout << "  substituted out of the formula:" << std::endl;
    std::cout << "    - Satisfied clauses are removed entirely" << std::endl;
    std::cout << "    - Remaining clauses are shortened by eliminating fixed literals" << std::endl;
    std::cout << "    - The number of satisfying assignments is preserved exactly" << std::endl;
    std::cout << "  Typical reduction: 30-50% fewer variables and clauses." << std::endl;

    return 0;
}
