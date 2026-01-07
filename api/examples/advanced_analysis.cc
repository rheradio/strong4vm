/**
 * @file advanced_analysis.cc
 * @brief Advanced example of using the Strong4VM API with custom configuration
 *
 * This example demonstrates advanced usage with custom configuration options.
 */

#include <iostream>
#include <iomanip>
#include "strong4vm/Strong4VMAPI.hh"

void print_separator() {
    std::cout << std::string(60, '=') << "\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file.uvl|.dimacs> [output_dir] [threads]\n";
        std::cerr << "\nExample:\n";
        std::cerr << "  " << argv[0] << " model.uvl ./output 4\n";
        return 1;
    }

    std::string input_file = argv[1];
    std::string output_dir = (argc > 2) ? argv[2] : "";
    int num_threads = (argc > 3) ? std::atoi(argv[3]) : 1;

    // Create API instance
    strong4vm::Strong4VMAPI api;

    // Create custom configuration
    strong4vm::AnalysisConfig config;
    config.input_file = input_file;
    config.output_dir = output_dir;
    config.conversion_mode = strong4vm::ConversionMode::STRAIGHTFORWARD;
    config.keep_dimacs = true;  // Keep intermediate DIMACS file
    config.detector = strong4vm::BackboneDetector::ONE;  // Use fast detector
    config.num_threads = num_threads;
    config.verbose = true;

    // Validate configuration before running
    std::string validation_error = api.validate_config(config);
    if (!validation_error.empty()) {
        std::cerr << "❌ Configuration error: " << validation_error << "\n";
        return 1;
    }

    print_separator();
    std::cout << "Strong4VM API - Advanced Analysis Example\n";
    print_separator();

    std::cout << "\nConfiguration:\n";
    std::cout << "  Input file:       " << config.input_file << "\n";
    std::cout << "  Output directory: "
              << (config.output_dir.empty() ? "(same as input)" : config.output_dir)
              << "\n";
    std::cout << "  Conversion mode:  "
              << (config.conversion_mode == strong4vm::ConversionMode::TSEITIN
                  ? "Tseitin" : "Straightforward")
              << "\n";
    std::cout << "  Keep DIMACS:      " << (config.keep_dimacs ? "Yes" : "No") << "\n";
    std::cout << "  Detector:         "
              << (config.detector == strong4vm::BackboneDetector::ONE
                  ? "One (with activity bumping)" : "Without attention")
              << "\n";
    std::cout << "  Threads:          " << config.num_threads << "\n";
    std::cout << "\n";

    // Perform analysis
    auto result = api.analyze(config);

    // Check result
    if (!result.success) {
        print_separator();
        std::cerr << "❌ Analysis Failed!\n";
        print_separator();
        std::cerr << "Error: " << result.error_message << "\n";
        return 1;
    }

    // Display detailed results
    print_separator();
    std::cout << "✓ Analysis Successful!\n";
    print_separator();

    std::cout << "\n📊 Statistics:\n\n";

    if (result.input_type == strong4vm::InputType::UVL) {
        std::cout << "Feature Model:\n";
        std::cout << "  Features:         " << std::setw(6) << result.num_features << "\n";
        std::cout << "  Relations:        " << std::setw(6) << result.num_relations << "\n";
        std::cout << "  Constraints:      " << std::setw(6) << result.num_constraints << "\n";
        std::cout << "\n";
    }

    std::cout << "CNF Formula:\n";
    std::cout << "  Variables:        " << std::setw(6) << result.num_variables << "\n";
    std::cout << "  Clauses:          " << std::setw(6) << result.num_clauses << "\n";
    std::cout << "  Avg clause size:  " << std::setw(6) << std::fixed << std::setprecision(2)
              << (result.num_variables > 0 ?
                  static_cast<double>(result.num_clauses) / result.num_variables : 0.0)
              << "\n";

    std::cout << "\nBackbone Analysis:\n";
    std::cout << "  Global backbone:  " << std::setw(6) << result.global_backbone.size() << "\n";
    std::cout << "  Core features:    " << std::setw(6) << result.core_features.size()
              << "  (" << std::fixed << std::setprecision(1)
              << (result.num_variables > 0 ?
                  (100.0 * result.core_features.size() / result.num_variables) : 0.0)
              << "%)\n";
    std::cout << "  Dead features:    " << std::setw(6) << result.dead_features.size()
              << "  (" << std::fixed << std::setprecision(1)
              << (result.num_variables > 0 ?
                  (100.0 * result.dead_features.size() / result.num_variables) : 0.0)
              << "%)\n";

    std::cout << "\n📁 Output Files:\n\n";
    std::cout << "  Requires graph:   " << result.requires_graph_file << "\n";
    std::cout << "  Excludes graph:   " << result.excludes_graph_file << "\n";
    std::cout << "  Core features:    " << result.core_features_file << "\n";
    std::cout << "  Dead features:    " << result.dead_features_file << "\n";

    if (!result.dimacs_file.empty()) {
        std::cout << "  DIMACS formula:   " << result.dimacs_file << "\n";
    }

    // Display sample of core features
    if (!result.core_features.empty()) {
        std::cout << "\n💎 Sample Core Features (first 20):\n";
        std::cout << "  ";
        for (size_t i = 0; i < std::min(size_t(20), result.core_features.size()); ++i) {
            std::cout << result.core_features[i];
            if (i < std::min(size_t(20), result.core_features.size()) - 1) {
                std::cout << ", ";
            }
            if ((i + 1) % 10 == 0 && i < std::min(size_t(20), result.core_features.size()) - 1) {
                std::cout << "\n  ";
            }
        }
        if (result.core_features.size() > 20) {
            std::cout << "\n  ... and " << (result.core_features.size() - 20) << " more";
        }
        std::cout << "\n";
    }

    // Display sample of dead features
    if (!result.dead_features.empty()) {
        std::cout << "\n☠️  Sample Dead Features (first 20):\n";
        std::cout << "  ";
        for (size_t i = 0; i < std::min(size_t(20), result.dead_features.size()); ++i) {
            std::cout << result.dead_features[i];
            if (i < std::min(size_t(20), result.dead_features.size()) - 1) {
                std::cout << ", ";
            }
            if ((i + 1) % 10 == 0 && i < std::min(size_t(20), result.dead_features.size()) - 1) {
                std::cout << "\n  ";
            }
        }
        if (result.dead_features.size() > 20) {
            std::cout << "\n  ... and " << (result.dead_features.size() - 20) << " more";
        }
        std::cout << "\n";
    }

    print_separator();
    std::cout << "✓ Complete!\n";
    print_separator();

    return 0;
}
