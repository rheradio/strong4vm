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
#include "CNFMode.hh"
#include "UVLCppLexer.h"
#include "UVLCppParser.h"
#include "antlr4-runtime.h"

#include <iostream>
#include <fstream>
#include <sstream>

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
    , mode_(ConversionMode::STRAIGHTFORWARD) {
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
