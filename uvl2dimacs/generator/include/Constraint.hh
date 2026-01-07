/**
 * @file Constraint.hh
 * @brief Cross-tree constraint in feature models
 *
 * Defines the Constraint class representing propositional constraints that
 * cross the feature tree hierarchy.
 *
 * @author UVL2Dimacs Team
 * @date 2024
 */

#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "ASTNode.hh"
#include "CNFMode.hh"
#include <string>
#include <memory>
#include <vector>
#include <functional>

/**
 * @class Constraint
 * @brief Represents a cross-tree constraint in the feature model
 *
 * Constraints express logical relationships between features that cannot be
 * captured by the parent-child tree structure alone. They are propositional
 * formulas over features.
 *
 * Examples of constraints:
 * - "GPS => Display" (if GPS is selected, Display must be selected)
 * - "Engine | ElectricMotor" (at least one propulsion method)
 * - "WiFi <=> NetworkCard" (WiFi if and only if NetworkCard)
 *
 * Constraints are internally represented as Abstract Syntax Trees (AST)
 * and can be converted to CNF clauses for SAT solving.
 *
 * @see ASTNode for the AST representation
 * @see FeatureModel for the containing model
 * @see CNFMode for conversion modes
 *
 * Example:
 * @code
 * // Create constraint: GPS => Display
 * auto gps_literal = std::make_shared<ASTNode>("GPS");
 * auto display_literal = std::make_shared<ASTNode>("Display");
 * auto implies_ast = std::make_shared<ASTNode>(
 *     ASTOperation::IMPLIES, gps_literal, display_literal
 * );
 * Constraint constraint("GPS_requires_Display", implies_ast);
 * @endcode
 */
class Constraint {
private:
    std::string name;                  ///< Name/identifier of this constraint
    std::shared_ptr<ASTNode> ast;      ///< AST representing the constraint expression

public:
    /**
     * @brief Constructs a new constraint
     * @param constraint_name Name or identifier for this constraint
     * @param ast_root Root node of the AST representing the constraint expression
     */
    Constraint(const std::string& constraint_name, std::shared_ptr<ASTNode> ast_root);

    /**
     * @brief Copy constructor
     * @param other The constraint to copy from
     */
    Constraint(const Constraint& other);

    /**
     * @brief Assignment operator
     * @param other The constraint to assign from
     * @return Reference to this constraint
     */
    Constraint& operator=(const Constraint& other);

    /**
     * @brief Destructor
     */
    ~Constraint() = default;

    /**
     * @brief Gets the name of this constraint
     * @return The constraint name/identifier
     */
    const std::string& get_name() const { return name; }

    /**
     * @brief Gets the AST representing this constraint
     * @return Shared pointer to the root AST node
     */
    std::shared_ptr<ASTNode> get_ast() const { return ast; }

    /**
     * @brief Converts this constraint to CNF clauses
     *
     * Converts the constraint expression to CNF format using the specified mode.
     *
     * @param get_variable Function to map feature names to variable IDs
     * @param create_aux_var Function to create new auxiliary variables (for Tseitin mode)
     * @param mode Conversion mode (TSEITIN or STRAIGHTFORWARD)
     * @return Vector of CNF clauses representing this constraint
     *
     * @see ASTNode::get_clauses() for detailed conversion process
     * @see CNFMode for mode descriptions
     */
    std::vector<std::vector<int>> get_clauses(
        std::function<int(const std::string&)> get_variable,
        std::function<int()> create_aux_var,
        CNFMode mode
    ) const;

    /**
     * @brief Checks if this constraint contains only boolean operations
     *
     * Determines if the constraint uses only pure boolean operations
     * (AND, OR, NOT, IMPLIES, EQUIVALENCE) without any arithmetic
     * or comparison operations.
     *
     * @return true if constraint is pure boolean
     */
    bool is_pure_boolean() const;

    /**
     * @brief Creates a string representation of this constraint
     * @return String representation for debugging (format: "Constraint(name: expression)")
     */
    std::string to_string() const;
};

#endif // CONSTRAINT_H
