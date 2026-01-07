/**
 * @file batch_analysis.cc
 * @brief Example of batch processing multiple feature models
 *
 * This example demonstrates how to analyze multiple feature models
 * in a batch using the Strong4VM API.
 */

#include <iostream>
#include <vector>
#include <filesystem>
#include <chrono>
#include <algorithm>
#include "strong4vm/Strong4VMAPI.hh"

namespace fs = std::filesystem;

struct BatchResult {
    std::string filename;
    bool success;
    int num_variables;
    int num_clauses;
    int core_features;
    int dead_features;
    double elapsed_seconds;
    std::string error_message;
};

void print_batch_summary(const std::vector<BatchResult>& results) {
    std::cout << "\n" << std::string(80, '=') << "\n";
    std::cout << "Batch Analysis Summary\n";
    std::cout << std::string(80, '=') << "\n\n";

    int successful = 0;
    int failed = 0;
    double total_time = 0.0;

    for (const auto& result : results) {
        if (result.success) {
            successful++;
        } else {
            failed++;
        }
        total_time += result.elapsed_seconds;
    }

    std::cout << "Total files processed: " << results.size() << "\n";
    std::cout << "  Successful: " << successful << "\n";
    std::cout << "  Failed:     " << failed << "\n";
    std::cout << "  Total time: " << total_time << " seconds\n";
    std::cout << "  Avg time:   " << (results.empty() ? 0.0 : total_time / results.size())
              << " seconds/file\n\n";

    // Detailed results
    std::cout << "Detailed Results:\n";
    std::cout << std::string(80, '-') << "\n";

    for (const auto& result : results) {
        std::cout << "File: " << result.filename << "\n";
        if (result.success) {
            std::cout << "  ✓ Success (" << result.elapsed_seconds << "s)\n";
            std::cout << "    Variables: " << result.num_variables
                      << ", Clauses: " << result.num_clauses << "\n";
            std::cout << "    Core: " << result.core_features
                      << ", Dead: " << result.dead_features << "\n";
        } else {
            std::cout << "  ✗ Failed (" << result.elapsed_seconds << "s)\n";
            std::cout << "    Error: " << result.error_message << "\n";
        }
        std::cout << "\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_directory> [output_directory] [threads]\n";
        std::cerr << "\nExample:\n";
        std::cerr << "  " << argv[0] << " ./models ./output 4\n";
        std::cerr << "\nProcesses all .uvl and .dimacs files in the input directory.\n";
        return 1;
    }

    std::string input_dir = argv[1];
    std::string output_dir = (argc > 2) ? argv[2] : input_dir;
    int num_threads = (argc > 3) ? std::atoi(argv[3]) : 1;

    // Check if input directory exists
    if (!fs::exists(input_dir) || !fs::is_directory(input_dir)) {
        std::cerr << "Error: Input directory does not exist: " << input_dir << "\n";
        return 1;
    }

    // Find all .uvl and .dimacs files
    std::vector<std::string> input_files;
    for (const auto& entry : fs::directory_iterator(input_dir)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
            if (ext == ".uvl" || ext == ".dimacs" || ext == ".cnf") {
                input_files.push_back(entry.path().string());
            }
        }
    }

    if (input_files.empty()) {
        std::cerr << "No .uvl or .dimacs files found in: " << input_dir << "\n";
        return 1;
    }

    std::cout << "Found " << input_files.size() << " files to process\n";
    std::cout << "Output directory: " << output_dir << "\n";
    std::cout << "Threads per file: " << num_threads << "\n\n";

    // Create API instance
    strong4vm::Strong4VMAPI api;
    api.set_verbose(false);  // Disable verbose for batch processing
    api.set_default_threads(num_threads);

    // Process each file
    std::vector<BatchResult> results;
    int file_num = 0;

    for (const auto& input_file : input_files) {
        file_num++;
        BatchResult batch_result;
        batch_result.filename = fs::path(input_file).filename().string();

        std::cout << "[" << file_num << "/" << input_files.size() << "] "
                  << "Processing: " << batch_result.filename << " ... " << std::flush;

        // Time the analysis
        auto start = std::chrono::high_resolution_clock::now();

        // Analyze
        auto result = api.analyze(input_file, output_dir);

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        batch_result.elapsed_seconds = elapsed.count();

        // Store results
        batch_result.success = result.success;
        batch_result.num_variables = result.num_variables;
        batch_result.num_clauses = result.num_clauses;
        batch_result.core_features = result.core_features.size();
        batch_result.dead_features = result.dead_features.size();
        batch_result.error_message = result.error_message;

        if (result.success) {
            std::cout << "✓ (" << elapsed.count() << "s)\n";
        } else {
            std::cout << "✗ Failed\n";
        }

        results.push_back(batch_result);
    }

    // Print summary
    print_batch_summary(results);

    return 0;
}
