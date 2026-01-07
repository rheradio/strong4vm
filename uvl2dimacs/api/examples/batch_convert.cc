/**
 * @file batch_convert.cc
 * @brief Advanced example showing batch conversion and comparison
 *
 * This example demonstrates:
 * - Converting multiple files
 * - Comparing STRAIGHTFORWARD vs TSEITIN modes
 * - Error handling
 * - Performance measurement
 */

#include "uvl2dimacs/UVL2Dimacs.hh"
#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>

struct ConversionJob {
    std::string input_file;
    std::string output_file;
};

void print_comparison(const std::string& file,
                      const uvl2dimacs::ConversionResult& straightforward,
                      const uvl2dimacs::ConversionResult& tseitin) {
    std::cout << std::endl;
    std::cout << "File: " << file << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    std::cout << std::left;
    std::cout << std::setw(25) << "Metric"
              << std::setw(18) << "Straightforward"
              << std::setw(18) << "Tseitin" << std::endl;
    std::cout << std::string(60, '-') << std::endl;

    if (straightforward.success && tseitin.success) {
        std::cout << std::setw(25) << "Features:"
                  << std::setw(18) << straightforward.num_features
                  << std::setw(18) << tseitin.num_features << std::endl;

        std::cout << std::setw(25) << "CNF Variables:"
                  << std::setw(18) << straightforward.num_variables
                  << std::setw(18) << tseitin.num_variables;
        if (tseitin.num_variables > straightforward.num_variables) {
            std::cout << " (+" << (tseitin.num_variables - straightforward.num_variables) << ")";
        }
        std::cout << std::endl;

        std::cout << std::setw(25) << "CNF Clauses:"
                  << std::setw(18) << straightforward.num_clauses
                  << std::setw(18) << tseitin.num_clauses;
        if (tseitin.num_clauses != straightforward.num_clauses) {
            int diff = tseitin.num_clauses - straightforward.num_clauses;
            std::cout << " (" << (diff > 0 ? "+" : "") << diff << ")";
        }
        std::cout << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input1.uvl> [input2.uvl ...]" << std::endl;
        std::cerr << std::endl;
        std::cerr << "This example converts each file using both STRAIGHTFORWARD and TSEITIN modes" << std::endl;
        std::cerr << "and compares the results." << std::endl;
        return 1;
    }

    // Collect input files
    std::vector<std::string> input_files;
    for (int i = 1; i < argc; i++) {
        input_files.push_back(argv[i]);
    }

    std::cout << "UVL2Dimacs API Example - Mode Comparison" << std::endl;
    std::cout << "=========================================" << std::endl;
    std::cout << "Files to process: " << input_files.size() << std::endl;
    std::cout << std::endl;

    // Process each file
    for (const auto& input_file : input_files) {
        // Generate output filenames
        std::string base_name = input_file.substr(0, input_file.find_last_of('.'));
        std::string output_straightforward = base_name + "_straightforward.dimacs";
        std::string output_tseitin = base_name + "_tseitin.dimacs";

        std::cout << "Processing: " << input_file << std::endl;

        // Create converter (reused for both modes)
        uvl2dimacs::UVL2Dimacs converter;
        converter.set_verbose(false);  // Disable verbose output for cleaner comparison

        // Convert with STRAIGHTFORWARD mode
        auto start = std::chrono::high_resolution_clock::now();
        auto result_straightforward = converter.convert(
            input_file,
            output_straightforward,
            uvl2dimacs::ConversionMode::STRAIGHTFORWARD
        );
        auto end = std::chrono::high_resolution_clock::now();
        auto duration_straightforward = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        // Convert with TSEITIN mode
        start = std::chrono::high_resolution_clock::now();
        auto result_tseitin = converter.convert(
            input_file,
            output_tseitin,
            uvl2dimacs::ConversionMode::TSEITIN
        );
        end = std::chrono::high_resolution_clock::now();
        auto duration_tseitin = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        // Report results
        if (result_straightforward.success && result_tseitin.success) {
            print_comparison(input_file, result_straightforward, result_tseitin);
            std::cout << std::setw(25) << "Time (straightforward):"
                      << std::setw(18) << (std::to_string(duration_straightforward.count()) + " ms") << std::endl;
            std::cout << std::setw(25) << "Time (Tseitin):"
                      << std::setw(18) << (std::to_string(duration_tseitin.count()) + " ms") << std::endl;
            std::cout << "✓ Both conversions successful" << std::endl;
        } else {
            std::cout << "✗ Conversion failed" << std::endl;
            if (!result_straightforward.success) {
                std::cout << "  Straightforward error: " << result_straightforward.error_message << std::endl;
            }
            if (!result_tseitin.success) {
                std::cout << "  Tseitin error: " << result_tseitin.error_message << std::endl;
            }
        }
        std::cout << std::endl;
    }

    std::cout << "Batch conversion complete!" << std::endl;
    return 0;
}
