/**
 * @file DimacsWriter.cc
 * @brief Implementation of DIMACS CNF format writer
 *
 * This file implements the DimacsWriter class which outputs CNF formulas
 * in DIMACS format, the standard input format for SAT solvers.
 *
 * DIMACS CNF Format Specification:
 * - Comment lines start with 'c'
 * - Problem line: "p cnf <num_vars> <num_clauses>"
 * - Clauses: Space-separated literals terminated by '0'
 * - Literals: Positive integers (variable) or negative integers (negated variable)
 *
 * Example:
 * @code
 * c This is a comment
 * c 1 FeatureA
 * c 2 FeatureB
 * p cnf 2 3
 * 1 2 0
 * -1 0
 * 2 0
 * @endcode
 *
 * @author UVL2Dimacs Team
 * @date 2024
 */

#include "DimacsWriter.hh"
#include <fstream>
#include <sstream>
#include <stdexcept>

/**
 * @brief Constructs a DIMACS writer for the given CNF model
 *
 * @param model The CNF model to write
 */
DimacsWriter::DimacsWriter(const CNFModel& model)
    : cnf_model(model) {
}

/**
 * @brief Writes the CNF model to a file in DIMACS format
 *
 * Creates or overwrites the specified file with DIMACS CNF output.
 * Includes comment lines for variable mappings followed by all clauses.
 *
 * @param filepath Path to output file
 * @throws std::runtime_error if file cannot be opened for writing
 */
void DimacsWriter::write_to_file(const std::string& filepath) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for writing: " + filepath);
    }

    write_to_stream(file);
    file.close();
}

/**
 * @brief Writes the CNF model to an output stream in DIMACS format
 *
 * Output format:
 * 1. Problem line: "p cnf <variables> <clauses>"
 * 2. Comment lines for feature variables: "c <var_id> <feature_name>"
 * 3. Comment lines for auxiliary variables: "c <var_id> <aux_name>"
 * 4. Clauses: "<lit1> <lit2> ... 0" (one per line)
 *
 * @param out Output stream to write to
 */
void DimacsWriter::write_to_stream(std::ostream& out) {
    // Write problem line
    out << "p cnf " << cnf_model.get_num_variables() << " " << cnf_model.get_num_clauses() << "\n";

    // Write comment lines for feature variables
    const auto& features = cnf_model.get_features();
    for (const auto& [var_id, feature_name] : features) {
        out << "c " << var_id << " " << feature_name << "\n";
    }

    // Write comment lines for auxiliary variables (user requested this)
    const auto& aux_vars = cnf_model.get_auxiliary_variables();
    for (const auto& [var_id, aux_name] : aux_vars) {
        out << "c " << var_id << " " << aux_name << "\n";
    }

    // Write clauses
    const auto& clauses = cnf_model.get_clauses();
    for (const auto& clause : clauses) {
        for (int literal : clause) {
            out << literal << " ";
        }
        out << "0\n";
    }
}

/**
 * @brief Converts the CNF model to a DIMACS format string
 *
 * Convenience method that writes to a string stream and returns the result.
 * Useful for in-memory representation or testing.
 *
 * @return String containing the complete DIMACS CNF representation
 */
std::string DimacsWriter::to_dimacs_string() {
    std::ostringstream oss;
    write_to_stream(oss);
    return oss.str();
}
