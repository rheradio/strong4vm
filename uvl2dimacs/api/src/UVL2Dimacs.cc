/*
 * MIT License
 *
 * Copyright (c) 2026 Rubén Heradio and David Fernández Amorós
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
 * @file UVL2Dimacs.cc
 * @brief Implementation of the UVL2Dimacs API
 */

#include "uvl2dimacs/UVL2Dimacs.hh"
#include "FeatureModelBuilder.hh"
#include "FMToCNF.hh"
#include "DimacsWriter.hh"
#include "BackboneSimplifier.hh"
#include "CNFMode.hh"
#include "UVLCppLexer.h"
#include "UVLCppParser.h"
#include "antlr4-runtime.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <cstdio>
#ifdef __APPLE__
#include <mach-o/dyld.h>
#include <climits>
#endif

using namespace antlr4;
using namespace antlr4::tree;

namespace uvl2dimacs {

/**
 * @brief Custom error listener for ANTLR parsing
 */
class CustomErrorListener : public BaseErrorListener {
private:
    std::string& error_msg_;

public:
    explicit CustomErrorListener(std::string& error_msg) : error_msg_(error_msg) {}

    void syntaxError(
        Recognizer *recognizer,
        Token *offendingSymbol,
        size_t line,
        size_t charPositionInLine,
        const std::string &msg,
        std::exception_ptr e) override {

        // Ignore tab-related warnings
        if (msg.find("\\t") != std::string::npos) {
            return;
        }

        // Store error message
        std::ostringstream oss;
        oss << "Syntax error at line " << line << ":" << charPositionInLine
            << " - " << msg;
        error_msg_ = oss.str();
    }
};

/**
 * @brief Convert ConversionMode to CNFMode
 */
static CNFMode to_cnf_mode(ConversionMode mode) {
    return (mode == ConversionMode::TSEITIN) ? CNFMode::TSEITIN : CNFMode::STRAIGHTFORWARD;
}

// Constructor
UVL2Dimacs::UVL2Dimacs(bool verbose)
    : verbose_(verbose)
    , mode_(ConversionMode::STRAIGHTFORWARD)
    , use_backbone_(false) {
}

// Destructor
UVL2Dimacs::~UVL2Dimacs() = default;

// Set verbose output mode
void UVL2Dimacs::set_verbose(bool verbose) {
    verbose_ = verbose;
}

// Set conversion mode
void UVL2Dimacs::set_mode(ConversionMode mode) {
    mode_ = mode;
}

// Get current conversion mode
ConversionMode UVL2Dimacs::get_mode() const {
    return mode_;
}

// Set backbone simplification
void UVL2Dimacs::set_backbone_simplification(bool use_backbone) {
    use_backbone_ = use_backbone;
}

// Get backbone simplification status
bool UVL2Dimacs::get_backbone_simplification() const {
    return use_backbone_;
}

// Convert with default mode
ConversionResult UVL2Dimacs::convert(const std::string& input_file,
                                     const std::string& output_file) {
    return convert(input_file, output_file, mode_);
}

// Convert with specified mode
ConversionResult UVL2Dimacs::convert(const std::string& input_file,
                                     const std::string& output_file,
                                     ConversionMode mode) {
    ConversionResult result;

    try {
        if (verbose_) {
            std::cout << "Reading UVL file: " << input_file << std::endl;
        }

        // Open the UVL file
        std::ifstream stream(input_file);
        if (!stream.is_open()) {
            result.error_message = "Could not open file: " + input_file;
            return result;
        }

        // Create ANTLR input stream
        ANTLRInputStream input(stream);

        // Create lexer
        UVLCppLexer lexer(&input);

        // Add custom error listener to lexer
        std::string parse_error;
        CustomErrorListener errorListener(parse_error);
        lexer.removeErrorListeners();
        lexer.addErrorListener(&errorListener);

        // Create token stream
        CommonTokenStream tokens(&lexer);

        // Create parser
        UVLCppParser parser(&tokens);
        parser.removeErrorListeners();
        parser.addErrorListener(&errorListener);

        // Parse the feature model
        if (verbose_) {
            std::cout << "Parsing UVL file..." << std::endl;
        }
        ParseTree* tree = parser.featureModel();

        // Check for parse errors
        if (!parse_error.empty()) {
            result.error_message = parse_error;
            return result;
        }

        // Build FeatureModel from parse tree
        if (verbose_) {
            std::cout << "Building feature model..." << std::endl;
        }
        FeatureModelBuilder builder;
        ParseTreeWalker::DEFAULT.walk(&builder, tree);

        auto feature_model = builder.get_feature_model();
        if (!feature_model) {
            result.error_message = "Failed to build feature model";
            return result;
        }

        // Store feature model statistics
        result.num_features = feature_model->get_features().size();
        result.num_relations = feature_model->get_relations().size();
        result.num_constraints = feature_model->get_constraints().size();

        if (verbose_) {
            std::cout << "Feature model built:" << std::endl;
            std::cout << "  Features: " << result.num_features << std::endl;
            std::cout << "  Relations: " << result.num_relations << std::endl;
            std::cout << "  Constraints: " << result.num_constraints << std::endl;
        }

        // Transform to CNF
        if (verbose_) {
            std::cout << "Transforming to CNF..." << std::endl;
        }
        FMToCNF transformer(feature_model);
        CNFModel cnf_model = transformer.transform(to_cnf_mode(mode));

        // Store CNF statistics
        result.num_variables = cnf_model.get_num_variables();
        result.num_clauses = cnf_model.get_num_clauses();
        result.num_skipped_constraints = transformer.get_skipped_constraints();

        if (verbose_) {
            std::cout << "CNF model created:" << std::endl;
            std::cout << "  Variables: " << result.num_variables << std::endl;
            std::cout << "  Clauses: " << result.num_clauses << std::endl;
        }

        // Write DIMACS file
        if (verbose_) {
            std::cout << "Writing DIMACS file: " << output_file << std::endl;
        }
        DimacsWriter writer(cnf_model);
        writer.write_to_file(output_file);

        // Apply backbone simplification if requested
        if (use_backbone_) {
            if (verbose_) {
                std::cout << "Applying backbone simplification..." << std::endl;
            }

            // Find backbone_solver executable
            std::string backbone_solver_path;

            // Check in project directory first
            char exe_path[1024];
            bool exe_found = false;
#ifdef __APPLE__
            uint32_t buf_size = sizeof(exe_path);
            if (_NSGetExecutablePath(exe_path, &buf_size) == 0)
                exe_found = true;
#else
            ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
            if (len != -1) { exe_path[len] = '\0'; exe_found = true; }
#endif
            if (exe_found) {
                std::string exe_dir = std::string(exe_path);
                size_t last_slash = exe_dir.find_last_of("/\\");
                if (last_slash != std::string::npos) {
                    exe_dir = exe_dir.substr(0, last_slash);
                }
                backbone_solver_path = exe_dir + "/../backbone_solver/bin/backbone_solver";
            }

            // Check if backbone_solver exists
            if (access(backbone_solver_path.c_str(), X_OK) != 0) {
                // Try PATH
                backbone_solver_path = "backbone_solver";
            }

            // Create temporary file for simplified output
            std::string temp_output = output_file + ".simplified";

            // Apply backbone simplification
            BackboneSimplifier simplifier;
            if (simplifier.simplify(output_file, temp_output, backbone_solver_path, verbose_)) {
                // Replace original file with simplified version
                if (std::rename(temp_output.c_str(), output_file.c_str()) != 0) {
                    if (verbose_) {
                        std::cerr << "Warning: Failed to replace original file with simplified version" << std::endl;
                    }
                    std::remove(temp_output.c_str());
                } else {
                    if (verbose_) {
                        std::cout << "  Backbone size: " << simplifier.get_backbone_size() << std::endl;
                        std::cout << "  Removed clauses: " << simplifier.get_removed_clauses() << std::endl;
                        std::cout << "  Shortened clauses: " << simplifier.get_shortened_clauses() << std::endl;
                    }
                }
            } else {
                if (verbose_) {
                    std::cerr << "Warning: Backbone simplification failed, keeping original output" << std::endl;
                }
                std::remove(temp_output.c_str());
            }
        }

        // Success!
        result.success = true;

        return result;

    } catch (const std::exception& e) {
        result.error_message = e.what();
        return result;
    }
}

// Convert to string with default mode
std::string UVL2Dimacs::convert_to_string(const std::string& input_file,
                                          ConversionResult& result) {
    return convert_to_string(input_file, mode_, result);
}

// Convert to string with specified mode
std::string UVL2Dimacs::convert_to_string(const std::string& input_file,
                                          ConversionMode mode,
                                          ConversionResult& result) {
    try {
        if (verbose_) {
            std::cout << "Reading UVL file: " << input_file << std::endl;
        }

        // Open the UVL file
        std::ifstream stream(input_file);
        if (!stream.is_open()) {
            result.error_message = "Could not open file: " + input_file;
            return "";
        }

        // Create ANTLR input stream
        ANTLRInputStream input(stream);

        // Create lexer
        UVLCppLexer lexer(&input);

        // Add custom error listener to lexer
        std::string parse_error;
        CustomErrorListener errorListener(parse_error);
        lexer.removeErrorListeners();
        lexer.addErrorListener(&errorListener);

        // Create token stream
        CommonTokenStream tokens(&lexer);

        // Create parser
        UVLCppParser parser(&tokens);
        parser.removeErrorListeners();
        parser.addErrorListener(&errorListener);

        // Parse the feature model
        if (verbose_) {
            std::cout << "Parsing UVL file..." << std::endl;
        }
        ParseTree* tree = parser.featureModel();

        // Check for parse errors
        if (!parse_error.empty()) {
            result.error_message = parse_error;
            return "";
        }

        // Build FeatureModel from parse tree
        if (verbose_) {
            std::cout << "Building feature model..." << std::endl;
        }
        FeatureModelBuilder builder;
        ParseTreeWalker::DEFAULT.walk(&builder, tree);

        auto feature_model = builder.get_feature_model();
        if (!feature_model) {
            result.error_message = "Failed to build feature model";
            return "";
        }

        // Store feature model statistics
        result.num_features = feature_model->get_features().size();
        result.num_relations = feature_model->get_relations().size();
        result.num_constraints = feature_model->get_constraints().size();

        // Transform to CNF
        if (verbose_) {
            std::cout << "Transforming to CNF..." << std::endl;
        }
        FMToCNF transformer(feature_model);
        CNFModel cnf_model = transformer.transform(to_cnf_mode(mode));

        // Store CNF statistics
        result.num_variables = cnf_model.get_num_variables();
        result.num_clauses = cnf_model.get_num_clauses();
        result.num_skipped_constraints = transformer.get_skipped_constraints();

        // Get DIMACS string
        DimacsWriter writer(cnf_model);
        std::string dimacs_str = writer.to_dimacs_string();

        // Success!
        result.success = true;

        return dimacs_str;

    } catch (const std::exception& e) {
        result.error_message = e.what();
        return "";
    }
}

} // namespace uvl2dimacs
