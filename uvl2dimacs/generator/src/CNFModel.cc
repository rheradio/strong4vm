/**
 * @file CNFModel.cc
 * @brief Implementation of CNF (Conjunctive Normal Form) model
 *
 * This file implements the CNFModel class which represents a boolean formula
 * in CNF format. It manages:
 * - Feature variables (mapped from feature names)
 * - Auxiliary variables (created during Tseitin transformation)
 * - CNF clauses (vectors of literals)
 *
 * The model assigns unique integer IDs to variables and stores clauses
 * for output in DIMACS format.
 *
 * @author UVL2Dimacs Team
 * @date 2024
 */

#include "CNFModel.hh"
#include <stdexcept>
#include <sstream>

/**
 * @brief Constructs an empty CNF model
 *
 * Initializes variable ID counter to 1 (DIMACS convention: positive integers).
 * Variable 0 is reserved as the clause terminator in DIMACS format.
 */
CNFModel::CNFModel()
    : next_var_id(1), aux_counter(0) {
}

/**
 * @brief Adds a feature as a variable in the CNF model
 *
 * Creates a mapping from feature name to a unique variable ID.
 * If the feature already exists, this is a no-op.
 *
 * @param name Feature name to add
 */
void CNFModel::add_feature(const std::string& name) {
    if (variables.find(name) == variables.end()) {
        variables[name] = next_var_id;
        features[next_var_id] = name;
        next_var_id++;
    }
}

/**
 * @brief Retrieves the variable ID for a given feature name
 *
 * Looks up the variable ID associated with a feature name.
 * Throws exception if the feature hasn't been added to the model.
 *
 * @param name Feature name to look up
 * @return Variable ID (positive integer)
 * @throws std::runtime_error if variable name not found
 */
int CNFModel::get_variable(const std::string& name) const {
    auto it = variables.find(name);
    if (it == variables.end()) {
        throw std::runtime_error("Variable not found: " + name);
    }
    return it->second;
}

/**
 * @brief Checks if a variable exists in the model
 *
 * @param name Feature name to check
 * @return true if variable exists, false otherwise
 */
bool CNFModel::has_variable(const std::string& name) const {
    return variables.find(name) != variables.end();
}

/**
 * @brief Creates a new auxiliary variable for Tseitin transformation
 *
 * Generates a unique auxiliary variable with an auto-incremented counter.
 * Auxiliary variables are named "aux_N" or "aux_N_description" where N is
 * a sequential counter starting from 1.
 *
 * These variables are used in Tseitin transformation to represent
 * intermediate boolean operation results.
 *
 * @param description Optional description for debugging (e.g., "and", "or")
 * @return Variable ID for the new auxiliary variable
 */
int CNFModel::create_auxiliary_variable(const std::string& description) {
    int var_id = next_var_id++;
    aux_counter++;

    std::string aux_name;
    if (description.empty()) {
        aux_name = "aux_" + std::to_string(aux_counter);
    } else {
        aux_name = "aux_" + std::to_string(aux_counter) + "_" + description;
    }

    auxiliary_variables[var_id] = aux_name;
    return var_id;
}

/**
 * @brief Adds a CNF clause to the model
 *
 * A clause is a disjunction (OR) of literals. Each literal is represented
 * as a signed integer:
 * - Positive integer N: variable N is true
 * - Negative integer -N: variable N is false
 *
 * Example: {1, -2, 3} represents (v1 ∨ ¬v2 ∨ v3)
 *
 * @param clause Vector of literals (non-zero integers)
 */
void CNFModel::add_clause(const std::vector<int>& clause) {
    clauses.push_back(clause);
}

/**
 * @brief Creates a human-readable string representation of the CNF model
 *
 * Includes statistics (number of variables, clauses) and a sample of
 * the first few clauses for debugging purposes.
 *
 * @return String representation with statistics and sample clauses
 */
std::string CNFModel::to_string() const {
    std::ostringstream oss;

    oss << "CNFModel:\n";
    oss << "  Features: " << features.size() << "\n";
    oss << "  Auxiliary variables: " << auxiliary_variables.size() << "\n";
    oss << "  Total variables: " << get_num_variables() << "\n";
    oss << "  Clauses: " << get_num_clauses() << "\n";

    // Sample first few clauses
    if (!clauses.empty()) {
        oss << "\nSample clauses:\n";
        int count = 0;
        for (const auto& clause : clauses) {
            if (count >= 5) {
                oss << "  ... (" << (clauses.size() - 5) << " more)\n";
                break;
            }
            oss << "  ";
            for (int lit : clause) {
                oss << lit << " ";
            }
            oss << "0\n";
            count++;
        }
    }

    return oss.str();
}
