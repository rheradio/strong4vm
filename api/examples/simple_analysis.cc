/**
 * @file simple_analysis.cc
 * @brief Simple example of using the Strong4VM API
 *
 * This example demonstrates the basic usage of the Strong4VM API
 * with minimal configuration.
 */

#include <iostream>
#include "strong4vm/Strong4VMAPI.hh"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file.uvl|.dimacs>\n";
        std::cerr << "\nExample:\n";
        std::cerr << "  " << argv[0] << " model.uvl\n";
        std::cerr << "  " << argv[0] << " formula.dimacs\n";
        return 1;
    }

    std::string input_file = argv[1];

    // Create API instance
    strong4vm::Strong4VMAPI api;

    // Enable verbose output
    api.set_verbose(true);

    // Perform analysis with default settings
    std::cout << "Analyzing: " << input_file << "\n\n";
    auto result = api.analyze(input_file);

    // Check result
    if (!result.success) {
        std::cerr << "\n❌ Analysis failed!\n";
        std::cerr << "Error: " << result.error_message << "\n";
        return 1;
    }

    // Display results
    std::cout << "\n✓ Analysis successful!\n\n";

    std::cout << "Input Information:\n";
    std::cout << "  File type: "
              << (result.input_type == strong4vm::InputType::UVL ? "UVL" : "DIMACS")
              << "\n";

    if (result.input_type == strong4vm::InputType::UVL) {
        std::cout << "  Features:    " << result.num_features << "\n";
        std::cout << "  Relations:   " << result.num_relations << "\n";
        std::cout << "  Constraints: " << result.num_constraints << "\n";
    }

    std::cout << "\nCNF Formula:\n";
    std::cout << "  Variables: " << result.num_variables << "\n";
    std::cout << "  Clauses:   " << result.num_clauses << "\n";

    std::cout << "\nAnalysis Results:\n";
    std::cout << "  Core features: " << result.core_features.size() << "\n";
    std::cout << "  Dead features: " << result.dead_features.size() << "\n";

    std::cout << "\nOutput Files:\n";
    std::cout << "  Requires graph: " << result.requires_graph_file << "\n";
    std::cout << "  Excludes graph: " << result.excludes_graph_file << "\n";
    std::cout << "  Core features:  " << result.core_features_file << "\n";
    std::cout << "  Dead features:  " << result.dead_features_file << "\n";

    // Display core features if any
    if (!result.core_features.empty()) {
        std::cout << "\nCore features (variables): ";
        for (size_t i = 0; i < std::min(size_t(10), result.core_features.size()); ++i) {
            std::cout << result.core_features[i];
            if (i < std::min(size_t(10), result.core_features.size()) - 1) {
                std::cout << ", ";
            }
        }
        if (result.core_features.size() > 10) {
            std::cout << " ... (and " << (result.core_features.size() - 10) << " more)";
        }
        std::cout << "\n";
    }

    // Display dead features if any
    if (!result.dead_features.empty()) {
        std::cout << "\nDead features (variables): ";
        for (size_t i = 0; i < std::min(size_t(10), result.dead_features.size()); ++i) {
            std::cout << result.dead_features[i];
            if (i < std::min(size_t(10), result.dead_features.size()) - 1) {
                std::cout << ", ";
            }
        }
        if (result.dead_features.size() > 10) {
            std::cout << " ... (and " << (result.dead_features.size() - 10) << " more)";
        }
        std::cout << "\n";
    }

    return 0;
}
