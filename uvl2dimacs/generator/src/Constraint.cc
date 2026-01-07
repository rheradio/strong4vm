/**
 * @file Constraint.cc
 * @brief Implementation of Constraint class
 *
 * This file implements the Constraint class which represents cross-tree
 * constraints in feature models. Constraints are stored as Abstract Syntax
 * Trees (AST) and can be converted to CNF clauses.
 *
 * @author UVL2Dimacs Team
 * @date 2024
 */

#include "Constraint.hh"

/**
 * @brief Constructs a new constraint with an AST
 *
 * Creates a constraint with the given name and constraint expression
 * represented as an Abstract Syntax Tree.
 *
 * @param constraint_name Identifier for this constraint
 * @param ast_root Root node of the AST representing the constraint
 */
Constraint::Constraint(const std::string& constraint_name, std::shared_ptr<ASTNode> ast_root)
    : name(constraint_name), ast(ast_root) {
}

/**
 * @brief Copy constructor
 *
 * Performs shallow copy of the constraint. The AST pointer is copied,
 * not the AST itself.
 *
 * @param other The constraint to copy from
 */
Constraint::Constraint(const Constraint& other)
    : name(other.name), ast(other.ast) {
}

/**
 * @brief Assignment operator
 *
 * Performs shallow copy with self-assignment check.
 *
 * @param other The constraint to assign from
 * @return Reference to this constraint
 */
Constraint& Constraint::operator=(const Constraint& other) {
    if (this != &other) {
        name = other.name;
        ast = other.ast;
    }
    return *this;
}

/**
 * @brief Converts this constraint to CNF clauses
 *
 * Delegates to the AST's get_clauses method to convert the constraint
 * expression to CNF format.
 *
 * @param get_variable Function to map feature names to variable IDs
 * @param create_aux_var Function to create auxiliary variables (Tseitin mode)
 * @param mode Conversion mode (TSEITIN or STRAIGHTFORWARD)
 * @return Vector of CNF clauses representing this constraint
 */
std::vector<std::vector<int>> Constraint::get_clauses(
    std::function<int(const std::string&)> get_variable,
    std::function<int()> create_aux_var,
    CNFMode mode
) const {
    if (!ast) {
        return {};
    }
    return ast->get_clauses(get_variable, create_aux_var, mode);
}

/**
 * @brief Checks if constraint uses only boolean operations
 *
 * Determines whether the constraint expression contains only pure boolean
 * operations (AND, OR, NOT, IMPLIES, EQUIVALENCE) without arithmetic or
 * comparison operations.
 *
 * @return true if constraint is pure boolean, false otherwise
 */
bool Constraint::is_pure_boolean() const {
    if (!ast) {
        return true;  // Empty constraint is trivially boolean
    }
    return ast->is_pure_boolean_tree();
}

/**
 * @brief Creates string representation of the constraint
 *
 * Format: "Constraint(name: expression)"
 *
 * @return String representation for debugging
 */
std::string Constraint::to_string() const {
    std::string result = "Constraint(" + name;
    if (ast) {
        result += ": " + ast->to_string();
    }
    result += ")";
    return result;
}
