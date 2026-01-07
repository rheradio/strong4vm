/**
 * @file uvl2dimacs.cc
 * @brief Command-line interface for UVL to DIMACS converter
 *
 * This program converts Universal Variability Language (UVL) feature models
 * to DIMACS CNF format for SAT solver input.
 */

#include "FeatureModelBuilder.hh"
#include "FMToCNF.hh"
#include "DimacsWriter.hh"
#include "UVLCppLexer.h"
#include "UVLCppParser.h"
#include "antlr4-runtime.h"

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

using namespace antlr4;
using namespace antlr4::tree;

/**
 * @class CustomErrorListener
 * @brief Custom error listener for handling parsing errors
 */
class CustomErrorListener : public BaseErrorListener {
public:
    void syntaxError(
        Recognizer *recognizer,
        Token *offendingSymbol,
        size_t line,
        size_t charPositionInLine,
        const std::string &msg,
        std::exception_ptr e) override {

        // Ignore tab-related warnings
        if (msg.find("\\t") != std::string::npos) {
            std::cerr << "Warning at line " << line << ":" << charPositionInLine
                      << " - " << msg << std::endl;
            return;
        }

        // For other errors, throw exception
        std::ostringstream oss;
        oss << "The UVL has the following error that prevents reading it: Line "
            << line << ":" << charPositionInLine << " - " << msg;
        throw std::runtime_error(oss.str());
    }
};

/**
 * @brief Print usage information
 * @param program_name Name of the program executable
 */
void print_usage(const char* program_name) {
    std::cerr << "Usage: " << program_name << " [-t|-s] <input.uvl> <output.dimacs>" << std::endl;
    std::cerr << std::endl;
    std::cerr << "Description:" << std::endl;
    std::cerr << "  Converts a UVL (Universal Variability Language) feature model" << std::endl;
    std::cerr << "  to DIMACS CNF format for SAT solver input." << std::endl;
    std::cerr << std::endl;
    std::cerr << "Options:" << std::endl;
    std::cerr << "  -s            Use straightforward conversion without auxiliary variables (default)" << std::endl;
    std::cerr << "  -t            Use Tseitin transformation with auxiliary variables" << std::endl;
    std::cerr << std::endl;
    std::cerr << "Arguments:" << std::endl;
    std::cerr << "  input.uvl     Path to input UVL file" << std::endl;
    std::cerr << "  output.dimacs Path to output DIMACS file" << std::endl;
    std::cerr << std::endl;
    std::cerr << "Performance:" << std::endl;
    std::cerr << "  This version is compiled with -O3 optimization for maximum speed." << std::endl;
}

int main(int argc, char* argv[]) {
    // Parse arguments
    CNFMode mode = CNFMode::STRAIGHTFORWARD; // Default mode
    bool verbose = true;  // Default to showing progress
    std::string input_file;
    std::string output_file;

    // Parse flags
    int arg_index = 1;
    while (arg_index < argc && argv[arg_index][0] == '-') {
        std::string flag = argv[arg_index];
        if (flag == "-t") {
            mode = CNFMode::TSEITIN;
        } else if (flag == "-s") {
            mode = CNFMode::STRAIGHTFORWARD;
        } else {
            std::cerr << "Error: Unknown flag '" << flag << "'" << std::endl;
            print_usage(argv[0]);
            return 1;
        }
        arg_index++;
    }

    // Check argument count
    if (argc - arg_index != 2) {
        print_usage(argv[0]);
        return 1;
    }

    input_file = argv[arg_index];
    output_file = argv[arg_index + 1];

    // Start timer
    auto start_time = std::chrono::high_resolution_clock::now();

    try {
        if (verbose) {
            std::cout << "UVL to DIMACS Converter v1.0" << std::endl;
            std::cout << "=============================" << std::endl;
            std::cout << "CNF Mode: " << (mode == CNFMode::TSEITIN ? "Tseitin (with auxiliary variables)" : "Straightforward (no auxiliary variables)") << std::endl;
            std::cout << "Input:  " << input_file << std::endl;
            std::cout << "Output: " << output_file << std::endl;
            std::cout << std::endl;
        }

        // Open the UVL file
        if (verbose) std::cout << "[1/5] Reading UVL file..." << std::endl;
        std::ifstream stream(input_file);
        if (!stream.is_open()) {
            std::cerr << "Error: Could not open file: " << input_file << std::endl;
            return 1;
        }

        // Create ANTLR input stream
        ANTLRInputStream input(stream);

        // Create lexer
        UVLCppLexer lexer(&input);

        // Add custom error listener to lexer
        CustomErrorListener errorListener;
        lexer.removeErrorListeners();
        lexer.addErrorListener(&errorListener);

        // Create token stream
        CommonTokenStream tokens(&lexer);

        // Create parser
        UVLCppParser parser(&tokens);

        // Add custom error listener to parser
        parser.removeErrorListeners();
        parser.addErrorListener(&errorListener);

        // Parse the feature model
        if (verbose) std::cout << "[2/5] Parsing UVL syntax..." << std::endl;
        ParseTree* tree = parser.featureModel();

        // Build FeatureModel from parse tree
        if (verbose) std::cout << "[3/5] Building feature model..." << std::endl;
        FeatureModelBuilder builder;
        ParseTreeWalker::DEFAULT.walk(&builder, tree);

        auto feature_model = builder.get_feature_model();
        if (!feature_model) {
            std::cerr << "Error: Failed to build feature model" << std::endl;
            return 1;
        }

        if (verbose) {
            std::cout << "  Features:    " << feature_model->get_features().size() << std::endl;
            std::cout << "  Relations:   " << feature_model->get_relations().size() << std::endl;
            std::cout << "  Constraints: " << feature_model->get_constraints().size() << std::endl;
        }

        // Transform to CNF
        if (verbose) std::cout << "[4/5] Transforming to CNF..." << std::endl;
        FMToCNF transformer(feature_model);
        CNFModel cnf_model = transformer.transform(mode);

        if (verbose) {
            std::cout << "  Variables:   " << cnf_model.get_num_variables() << std::endl;
            std::cout << "  Clauses:     " << cnf_model.get_num_clauses() << std::endl;
        }

        // Write DIMACS file
        if (verbose) std::cout << "[5/5] Writing DIMACS file..." << std::endl;
        DimacsWriter writer(cnf_model);
        writer.write_to_file(output_file);

        // Calculate elapsed time
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

        if (true) {
            std::cout << std::endl;
            std::cout << "Success! DIMACS file created." << std::endl;
            if (verbose) {
                std::cout << "Time elapsed: " << duration.count() << " ms" << std::endl;
            }
        }

        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
