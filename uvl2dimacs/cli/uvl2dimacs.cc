/*
 * MIT License
 *
 * Copyright (c) 2026 Rubén Heradio, David Fernández Amorós, 
 *                    Ismael Abad Cardiel, Ernesto Aranda Escolástico
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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
#include <cstdlib>

using namespace antlr4;
using namespace antlr4::tree;

// Program information constants
namespace {
    constexpr const char* PROGRAM_TITLE = "UVL2DIMACS: A UVL TRANSLATOR INTO BOOLEAN LOGIC, 2026";
    constexpr const char* PROGRAM_AUTHORS = "Authors: Rubén Heradio, David Fernández Amorós, Ismael Abad Cardiel, Ernesto Aranda Escolástico";
}

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
 * @brief Print ASCII banner and program information
 * @param out Output stream to write to
 */
void print_banner(std::ostream& out) {
    out << "###########################################################" << std::endl;
    out << "             _   ____        _                             " << std::endl;
    out << " _   ___   _| | |___ \\    __| (_)_ __ ___   __ _  ___ ___ " << std::endl;
    out << "| | | \\ \\ / / |   __) |  / _` | | '_ ` _ \\ / _` |/ __/ __|" << std::endl;
    out << "| |_| |\\ V /| |  / __/  | (_| | | | | | | | (_| | (__\\__ \\" << std::endl;
    out << " \\__,_| \\_/ |_| |_____|  \\__,_|_|_| |_| |_|\\__,_|\\___|___/" << std::endl;
    out << "" << std::endl;
    out << "                            @@@@@@@@@@@@        " << std::endl;
    out << "                         @@@@@@@@@@@@@@@@@@     " << std::endl;
    out << "                       @@@@@            @@@@@   " << std::endl;
    out << "                      @@@@                @@@@  " << std::endl;
    out << "                     @@@      @@@@@@@@@@   @@@@ " << std::endl;
    out << "                    @@@       @@@@@@@@@@    @@@@" << std::endl;
    out << "                  @@@@@@@@        @@@@@@     @@@" << std::endl;
    out << "              @@@@@@@@@@@@@@@@  @@@@@@@@     @@@" << std::endl;
    out << "            @@@@@@         @@@@@@@@  @@@     @@@" << std::endl;
    out << "          @@@@@             @@@@@@   @@@     @@@" << std::endl;
    out << "         @@@@              @@@@@@@@         @@@@" << std::endl;
    out << "        @@@@             @@@@   @@@@       @@@@ " << std::endl;
    out << "        @@@            @@@@@     @@@     @@@@@  " << std::endl;
    out << "        @@@          @@@@@       @@@  @@@@@@    " << std::endl;
    out << "        @@@         @@@@         @@@@@@@@@      " << std::endl;
    out << "        @@@                      @@@@@          " << std::endl;
    out << "        @@@@                     @@@            " << std::endl;
    out << "         @@@@                   @@@@            " << std::endl;
    out << "          @@@@                @@@@              " << std::endl;
    out << "           @@@@@            @@@@@               " << std::endl;
    out << "             @@@@@@@@@@@@@@@@@@                 " << std::endl;
    out << "                @@@@@@@@@@@@                    " << std::endl;
    out << "" << std::endl;
    out << "" << std::endl;
    out << "  " << PROGRAM_TITLE << std::endl;
    out << "    " << PROGRAM_AUTHORS << std::endl;
    out << "    " << std::endl;
    out << "###########################################################" << std::endl;
    out << "" << std::endl;
    out << "" << std::endl;
}

/**
 * @brief Print usage information
 * @param program_name Name of the program executable
 */
void print_usage(const char* program_name) {
    print_banner(std::cerr);
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

/**
 * @brief Structure to hold command-line arguments
 */
struct CommandLineArgs {
    CNFMode mode = CNFMode::STRAIGHTFORWARD;
    bool verbose = true;
    std::string input_file;
    std::string output_file;
};

/**
 * @brief Parse command-line arguments
 * @param argc Argument count
 * @param argv Argument values
 * @return Parsed arguments or throws on error
 */
CommandLineArgs parse_arguments(int argc, char* argv[]) {
    CommandLineArgs args;

    // Parse flags
    int arg_index = 1;
    while (arg_index < argc && argv[arg_index][0] == '-') {
        std::string flag = argv[arg_index];
        if (flag == "-t") {
            args.mode = CNFMode::TSEITIN;
        } else if (flag == "-s") {
            args.mode = CNFMode::STRAIGHTFORWARD;
        } else {
            std::cerr << "Error: Unknown flag '" << flag << "'" << std::endl;
            print_usage(argv[0]);
            exit(1);
        }
        arg_index++;
    }

    // Check argument count
    if (argc - arg_index != 2) {
        print_usage(argv[0]);
        exit(1);
    }

    args.input_file = argv[arg_index];
    args.output_file = argv[arg_index + 1];

    return args;
}

/**
 * @brief Parse UVL file and build feature model
 * @param input_file Path to input UVL file
 * @param verbose Whether to print progress
 * @return Feature model
 */
std::shared_ptr<FeatureModel> parse_uvl_file(const std::string& input_file, bool verbose) {
    if (verbose) std::cout << "[1/5] Reading UVL file..." << std::endl;

    std::ifstream stream(input_file);
    if (!stream.is_open()) {
        throw std::runtime_error("Could not open file: " + input_file);
    }

    // Create ANTLR input stream and lexer
    ANTLRInputStream input(stream);
    UVLCppLexer lexer(&input);

    // Add custom error listener
    CustomErrorListener errorListener;
    lexer.removeErrorListeners();
    lexer.addErrorListener(&errorListener);

    // Create token stream and parser
    CommonTokenStream tokens(&lexer);
    UVLCppParser parser(&tokens);
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
        throw std::runtime_error("Failed to build feature model");
    }

    if (verbose) {
        std::cout << "  Features:    " << feature_model->get_features().size() << std::endl;
        std::cout << "  Relations:   " << feature_model->get_relations().size() << std::endl;
        std::cout << "  Constraints: " << feature_model->get_constraints().size() << std::endl;
    }

    return feature_model;
}

int main(int argc, char* argv[]) {
    // Parse command-line arguments
    CommandLineArgs args = parse_arguments(argc, argv);

    // Start timer
    auto start_time = std::chrono::high_resolution_clock::now();

    try {
        // Print banner and configuration
        if (args.verbose) {
            print_banner(std::cout);
            std::cout << "CNF Mode: " << (args.mode == CNFMode::TSEITIN ?
                "Tseitin (with auxiliary variables)" : "Straightforward (no auxiliary variables)") << std::endl;
            std::cout << "Input:  " << args.input_file << std::endl;
            std::cout << "Output: " << args.output_file << std::endl;
            std::cout << std::endl;
        }

        // Parse UVL file and build feature model
        auto feature_model = parse_uvl_file(args.input_file, args.verbose);

        // Transform to CNF
        if (args.verbose) std::cout << "[4/5] Transforming to CNF..." << std::endl;
        FMToCNF transformer(feature_model);
        CNFModel cnf_model = transformer.transform(args.mode);

        if (args.verbose) {
            std::cout << "  Variables:   " << cnf_model.get_num_variables() << std::endl;
            std::cout << "  Clauses:     " << cnf_model.get_num_clauses() << std::endl;
        }

        // Write DIMACS file
        if (args.verbose) std::cout << "[5/5] Writing DIMACS file..." << std::endl;
        DimacsWriter writer(cnf_model);
        writer.write_to_file(args.output_file);

        // Calculate elapsed time
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

        std::cout << std::endl;
        std::cout << "Success! DIMACS file created." << std::endl;
        if (args.verbose) {
            std::cout << "Time elapsed: " << duration.count() << " ms" << std::endl;
        }

        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
