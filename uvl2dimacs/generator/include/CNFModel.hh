/**
 * @file CNFModel.hh
 * @brief CNF (Conjunctive Normal Form) model with variable mapping
 *
 * This file defines the CNFModel class which represents a boolean formula in CNF format.
 * It maintains mappings between feature names and variable IDs, manages auxiliary variables,
 * and stores the CNF clauses.
 *
 * @author UVL2Dimacs Team
 * @date 2024
 */

#ifndef CNFMODEL_H
#define CNFMODEL_H

#include <string>
#include <vector>
#include <map>

/**
 * @class CNFModel
 * @brief Represents a CNF (Conjunctive Normal Form) formula with variable mappings
 *
 * CNFModel stores a boolean formula in CNF format along with bidirectional mappings
 * between feature names and variable IDs. It supports both feature variables (from
 * the original feature model) and auxiliary variables (introduced during CNF conversion).
 *
 * **CNF Format**:
 * - A clause is a disjunction (OR) of literals: (A | ~B | C)
 * - A CNF formula is a conjunction (AND) of clauses: (A | B) & (~B | C) & (A | ~C)
 * - Variables are represented as positive integers (1, 2, 3, ...)
 * - Negated literals are represented as negative integers (-1, -2, -3, ...)
 *
 * **Variable Management**:
 * - Feature variables: Mapped from feature names to positive integers starting at 1
 * - Auxiliary variables: Additional variables created during Tseitin transformation
 * - Variable IDs are sequential and unique
 *
 * @see FMToCNF for conversion from FeatureModel to CNFModel
 * @see DimacsWriter for writing CNFModel to DIMACS format
 *
 * Example:
 * @code
 * CNFModel cnf;
 * cnf.add_feature("GPS");        // GPS gets variable ID 1
 * cnf.add_feature("Display");    // Display gets variable ID 2
 * cnf.add_clause({1, 2});        // GPS | Display
 * cnf.add_clause({-1, 2});       // ~GPS | Display (GPS => Display)
 * @endcode
 */
class CNFModel {
private:
    std::map<std::string, int> variables;           ///< Feature name to variable ID mapping
    std::map<int, std::string> features;            ///< Variable ID to feature name mapping
    std::map<int, std::string> auxiliary_variables; ///< Auxiliary variable ID to description mapping
    std::vector<std::vector<int>> clauses;          ///< CNF clauses (each clause is a vector of literals)

    int next_var_id;   ///< Next available variable ID (starts at 1)
    int aux_counter;   ///< Counter for auxiliary variable naming

public:
    /**
     * @brief Default constructor
     *
     * Initializes an empty CNF model with variable IDs starting at 1.
     */
    CNFModel();

    /**
     * @brief Destructor
     */
    ~CNFModel() = default;

    /**
     * @brief Adds a feature variable to the model
     *
     * Creates a new variable ID for the given feature name and establishes
     * bidirectional mapping. If the feature already exists, this is a no-op.
     *
     * @param name The feature name
     */
    void add_feature(const std::string& name);

    /**
     * @brief Gets the variable ID for a feature
     *
     * @param name The feature name
     * @return The variable ID (positive integer)
     * @throws std::runtime_error if the feature name is not found
     */
    int get_variable(const std::string& name) const;

    /**
     * @brief Checks if a variable exists for the given feature name
     *
     * @param name The feature name
     * @return true if the variable exists, false otherwise
     */
    bool has_variable(const std::string& name) const;

    /**
     * @brief Creates a new auxiliary variable
     *
     * Auxiliary variables are used in Tseitin transformation to represent
     * intermediate results of complex expressions.
     *
     * @param description Optional description of what this auxiliary variable represents
     * @return The variable ID of the newly created auxiliary variable
     */
    int create_auxiliary_variable(const std::string& description = "");

    /**
     * @brief Adds a clause to the CNF formula
     *
     * A clause is a disjunction (OR) of literals. Empty clauses represent
     * contradictions and should be avoided.
     *
     * @param clause Vector of literals (positive for variable, negative for negation)
     *
     * Example:
     * @code
     * cnf.add_clause({1, -2, 3});  // Adds clause: v1 | ~v2 | v3
     * @endcode
     */
    void add_clause(const std::vector<int>& clause);

    /**
     * @brief Gets the feature name to variable ID mapping
     * @return Constant reference to the variables map
     */
    const std::map<std::string, int>& get_variables() const { return variables; }

    /**
     * @brief Gets the variable ID to feature name mapping
     * @return Constant reference to the features map
     */
    const std::map<int, std::string>& get_features() const { return features; }

    /**
     * @brief Gets the auxiliary variable descriptions
     * @return Constant reference to the auxiliary variables map
     */
    const std::map<int, std::string>& get_auxiliary_variables() const { return auxiliary_variables; }

    /**
     * @brief Gets all CNF clauses
     * @return Constant reference to the clauses vector
     */
    const std::vector<std::vector<int>>& get_clauses() const { return clauses; }

    /**
     * @brief Gets the total number of variables
     *
     * Includes both feature variables and auxiliary variables.
     *
     * @return Total number of variables in the CNF model
     */
    int get_num_variables() const { return next_var_id - 1; }

    /**
     * @brief Gets the total number of clauses
     * @return Number of clauses in the CNF formula
     */
    int get_num_clauses() const { return clauses.size(); }

    /**
     * @brief Creates a string representation of the CNF model
     * @return String representation for debugging
     */
    std::string to_string() const;
};

#endif // CNFMODEL_H
