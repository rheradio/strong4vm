/**
 * @file strong4vm.cc
 * @brief Strong4VM - A tool for extracting Strong Transitive Dependency and Conflict Graphs from Variability Models
 *
 * This is the main CLI entry point that orchestrates the complete Strong4VM pipeline:
 * - Accepts UVL feature models or DIMACS CNF files as input
 * - Automatically converts UVL to DIMACS if needed
 * - Generates strong transitive dependency and conflict graphs
 * - Identifies core and dead features
 *
 * Usage:
 *   strong4vm <input_file> [options]
 */

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <cstring>
#include "icon_embedded.hh"
#include "../uvl2dimacs/api/include/uvl2dimacs/UVL2Dimacs.hh"
#include "../dimacs2graphs/api/Dimacs2GraphsAPI.hh"

namespace fs = std::filesystem;

// ANSI color codes
const std::string COLOR_GREEN = "\033[32m";
const std::string COLOR_YELLOW = "\033[33m";
const std::string COLOR_BLUE = "\033[34m";
const std::string COLOR_RESET = "\033[0m";

void print_header() {
    // Display the embedded icon in green
    std::cout << COLOR_GREEN;
    std::cout << strong4vm::ICON_ASCII;
    std::cout << COLOR_RESET;
    std::cout << "\n";
}

void print_usage(const char* program_name) {
    std::cout << "Usage: " << program_name << " <input_file> [options]\n\n";
    std::cout << "Arguments:\n";
    std::cout << "  input_file           Input file (<basename>.uvl or <basename>.dimacs)\n\n";
    std::cout << "Options:\n";
    std::cout << "  -t, --threads N      Number of threads for graph generation (default: 1)\n";
    std::cout << "  -o, --output DIR     Output directory (default: same as input file)\n";
    std::cout << "  -k, --keep-dimacs    Keep intermediate DIMACS file (UVL input only)\n";
    std::cout << "  -e, --enable-tseitin Enable Tseitin transformation for UVL conversion\n";
    std::cout << "  -h, --help           Display this help message\n\n";
    std::cout << "Output Files:\n";
    std::cout << "  <basename>__requires.net   Dependency graph (Pajek format)\n";
    std::cout << "  <basename>__excludes.net   Conflict graph (Pajek format)\n";
    std::cout << "  <basename>__core.txt       Core features (enabled in all configurations)\n";
    std::cout << "  <basename>__dead.txt       Dead features (disabled in all configurations)\n\n";
    std::cout << "Examples:\n";
    std::cout << "  " << program_name << " model.uvl\n";
    std::cout << "  " << program_name << " model.uvl -t 4\n";
    std::cout << "  " << program_name << " model.dimacs -t 8\n";
    std::cout << "  " << program_name << " model.uvl -o ./output -k\n\n";
    std::cout << "You may find UVL models in:\n";
    std::cout << "  - the directory \"examples\" of this tool\n";
    std::cout << "  - https://www.uvlhub.io/\n";
}

enum class FileType {
    UVL,
    DIMACS,
    UNKNOWN
};

FileType detect_file_type(const std::string& filename) {
    fs::path path(filename);
    std::string ext = path.extension().string();

    // Convert to lowercase for case-insensitive comparison
    for (char& c : ext) {
        c = std::tolower(c);
    }

    if (ext == ".uvl") {
        return FileType::UVL;
    } else if (ext == ".dimacs" || ext == ".cnf") {
        return FileType::DIMACS;
    }

    return FileType::UNKNOWN;
}

std::string get_basename(const std::string& filepath) {
    fs::path path(filepath);
    return path.stem().string();
}

std::string get_directory(const std::string& filepath) {
    fs::path path(filepath);
    if (path.has_parent_path()) {
        return path.parent_path().string();
    }
    return ".";
}

int main(int argc, char* argv[]) {
    // Print header
    print_header();

    // Parse command-line arguments
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    std::string input_file;
    std::string output_dir;
    int num_threads = 1;
    bool keep_dimacs = false;
    bool use_tseitin = false;

    // Parse arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            print_usage(argv[0]);
            return 0;
        } else if (arg == "-k" || arg == "--keep-dimacs") {
            keep_dimacs = true;
        } else if (arg == "-e" || arg == "--enable-tseitin") {
            use_tseitin = true;
        } else if ((arg == "-t" || arg == "--threads") && i + 1 < argc) {
            num_threads = std::atoi(argv[++i]);
            if (num_threads < 1) {
                std::cerr << "Error: Thread count must be at least 1\n";
                return 1;
            }
        } else if ((arg == "-o" || arg == "--output") && i + 1 < argc) {
            output_dir = argv[++i];
        } else if (arg[0] != '-') {
            input_file = arg;
        } else {
            std::cerr << "Error: Unknown option: " << arg << "\n";
            print_usage(argv[0]);
            return 1;
        }
    }

    if (input_file.empty()) {
        std::cerr << "Error: No input file specified\n";
        print_usage(argv[0]);
        return 1;
    }

    // Check if input file exists
    if (!fs::exists(input_file)) {
        std::cerr << "Error: Input file not found: " << input_file << "\n";
        return 1;
    }

    // Detect file type
    FileType file_type = detect_file_type(input_file);
    if (file_type == FileType::UNKNOWN) {
        std::cerr << "Error: Unknown file type. Expected .uvl or .dimacs file\n";
        return 1;
    }

    // Set default output directory if not specified
    if (output_dir.empty()) {
        output_dir = get_directory(input_file);
    }

    // Create output directory if it doesn't exist
    if (!fs::exists(output_dir)) {
        try {
            fs::create_directories(output_dir);
        } catch (const std::exception& e) {
            std::cerr << "Error: Could not create output directory: " << e.what() << "\n";
            return 1;
        }
    }

    std::string dimacs_file;
    std::string basename = get_basename(input_file);
    bool temp_dimacs = false;

    // Step 1: Ensure we have a DIMACS file
    if (file_type == FileType::UVL) {
        std::cout << "=================================================\n";
        std::cout << COLOR_YELLOW << "Step 1: Converting UVL to DIMACS\n" << COLOR_RESET;
        std::cout << "=================================================\n";

        // Determine DIMACS output path
        if (keep_dimacs) {
            dimacs_file = output_dir + "/" + basename + ".dimacs";
        } else {
            // Use the same naming for temp files - we'll delete it later
            dimacs_file = output_dir + "/" + basename + ".dimacs";
            temp_dimacs = true;
        }

        // Convert UVL to DIMACS
        uvl2dimacs::UVL2Dimacs converter;
        converter.set_verbose(true);
        if (use_tseitin) {
            converter.set_mode(uvl2dimacs::ConversionMode::TSEITIN);
        } else {
            converter.set_mode(uvl2dimacs::ConversionMode::STRAIGHTFORWARD);
        }
        std::cout << "  Mode: " << (use_tseitin ? "Tseitin" : "Straightforward") << "\n";

        auto result = converter.convert(input_file, dimacs_file);

        if (!result.success) {
            std::cerr << "\nError: UVL to DIMACS conversion failed\n";
            std::cerr << result.error_message << "\n";
            return 1;
        }

        std::cout << "\nConversion successful!\n";
        std::cout << "  Features:   " << result.num_features << "\n";
        std::cout << "  Variables:  " << result.num_variables << "\n";
        std::cout << "  Clauses:    " << result.num_clauses << "\n";

        if (keep_dimacs) {
            std::cout << "  DIMACS file: " << dimacs_file << "\n";
        }
        std::cout << "\n";

    } else {
        // Input is already DIMACS
        dimacs_file = input_file;
    }

    // Generate graphs from DIMACS
    std::cout << "=================================================\n";
    if (file_type == FileType::UVL) {
        std::cout << COLOR_BLUE << "Step 2: Generating Strong Transitive Graphs\n" << COLOR_RESET;
    } else {
        std::cout << COLOR_BLUE << "Generating Strong Transitive Graphs\n" << COLOR_RESET;
    }
    std::cout << "=================================================\n";

    dimacs2graphs::Dimacs2GraphsAPI graph_api;

    // When Tseitin mode is used, filter auxiliary variables from output
    if (use_tseitin) {
        graph_api.set_filter_auxiliary(true);
    }

    // Use the basename without path for graph generation
    std::string dimacs_basename = get_basename(dimacs_file);

    // The API expects output_dir as a directory path, not including basename
    // Always use "one" detector (with activity bumping)
    bool success = graph_api.generate_graphs(dimacs_file, output_dir, "one", num_threads);

    if (!success) {
        std::cerr << "\nError: Graph generation failed\n";
        std::cerr << graph_api.get_error_message() << "\n";

        // Clean up temporary DIMACS file if created
        if (temp_dimacs && fs::exists(dimacs_file)) {
            fs::remove(dimacs_file);
        }

        return 1;
    }

    std::cout << "\nGraph generation successful!\n";
    std::cout << "  Variables: " << graph_api.get_num_variables() << "\n";
    std::cout << "  Clauses:   " << graph_api.get_num_clauses() << "\n";
    std::cout << "\nOutput files:\n";
    std::cout << "  " << output_dir << "/" << dimacs_basename << "__requires.net\n";
    std::cout << "  " << output_dir << "/" << dimacs_basename << "__excludes.net\n";
    std::cout << "  " << output_dir << "/" << dimacs_basename << "__core.txt\n";
    std::cout << "  " << output_dir << "/" << dimacs_basename << "__dead.txt\n";

    // Clean up temporary DIMACS file if needed
    if (temp_dimacs && fs::exists(dimacs_file)) {
        fs::remove(dimacs_file);
    }

    std::cout << "\n=================================================\n";
    std::cout << "Graphs' Generation Complete!\n";
    std::cout << "=================================================\n";

    return 0;
}
