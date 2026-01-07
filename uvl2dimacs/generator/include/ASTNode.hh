/**
 * @file ASTNode.hh
 * @brief Abstract Syntax Tree node for constraint expressions
 *
 * This file defines the ASTNode class which represents constraint expressions
 * as abstract syntax trees. It supports conversion to CNF (Conjunctive Normal Form)
 * using either Tseitin transformation or direct conversion methods.
 *
 * @author UVL2Dimacs Team
 * @date 2024
 */

#ifndef ASTNODE_H
#define ASTNODE_H

#include "CNFMode.hh"
#include <string>
#include <vector>
#include <memory>
#include <functional>

/**
 * @enum ASTOperation
 * @brief Operation types for constraint expression AST nodes
 *
 * Defines all supported operations in UVL constraint expressions including
 * logical operations, comparisons, arithmetic, and aggregate functions.
 */
enum class ASTOperation {
    // Logical operations
    NOT,            ///< Logical NOT (~A)
    AND,            ///< Logical AND (A & B)
    OR,             ///< Logical OR (A | B)
    IMPLIES,        ///< Logical implication (A => B)
    EQUIVALENCE,    ///< Logical equivalence (A <=> B)

    // Comparison operations
    EQUALS,         ///< Equality comparison (A == B)
    NOT_EQUALS,     ///< Inequality comparison (A != B)
    LOWER,          ///< Less than comparison (A < B)
    LOWER_EQUALS,   ///< Less than or equal (A <= B)
    GREATER,        ///< Greater than comparison (A > B)
    GREATER_EQUALS, ///< Greater than or equal (A >= B)

    // Arithmetic operations
    ADD,            ///< Addition (A + B)
    SUB,            ///< Subtraction (A - B)
    MUL,            ///< Multiplication (A * B)
    DIV,            ///< Division (A / B)

    // Aggregate functions
    SUM,            ///< Sum of elements
    AVG,            ///< Average of elements
    LEN,            ///< Length/count of elements
    FLOOR,          ///< Floor function
    CEIL            ///< Ceiling function
};

/**
 * @class ASTNode
 * @brief Node in an Abstract Syntax Tree representing constraint expressions
 *
 * ASTNode represents constraint expressions as a tree structure supporting
 * logical, arithmetic, and comparison operations. The tree can be converted
 * to CNF (Conjunctive Normal Form) for SAT solving using two methods:
 *
 * - **Tseitin transformation**: Uses auxiliary variables to create shorter clauses
 *   (results in more variables but smaller clause sizes)
 * - **Direct conversion**: Converts directly to CNF without auxiliary variables
 *   (results in fewer variables but potentially longer clauses)
 *
 * @see CNFMode for conversion mode options
 * @see get_clauses() for CNF conversion
 */
class ASTNode {
public:
    /**
     * @enum Type
     * @brief Type of AST node (operation or leaf value)
     */
    enum class Type {
        OPERATION,    ///< Internal node with an operation and children
        LITERAL,      ///< Leaf node containing a feature name (boolean variable)
        INTEGER,      ///< Leaf node containing an integer constant
        FLOAT,        ///< Leaf node containing a floating-point constant
        STRING        ///< Leaf node containing a string constant
    };

private:
    Type type;                                             ///< Type of this node (operation or leaf)
    ASTOperation operation;                                ///< Operation type (used when type == OPERATION)
    std::string literal_value;                             ///< Feature name or string value
    int int_value;                                         ///< Integer value (used when type == INTEGER)
    double float_value;                                    ///< Float value (used when type == FLOAT)
    std::vector<std::shared_ptr<ASTNode>> children;        ///< Child nodes for operations

public:
    // Constructors for different node types

    /**
     * @brief Constructs a binary operation node
     * @param op The operation type (AND, OR, IMPLIES, etc.)
     * @param left Left operand child node
     * @param right Right operand child node
     */
    ASTNode(ASTOperation op, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right);

    /**
     * @brief Constructs a unary operation node
     * @param op The operation type (typically NOT)
     * @param child The operand child node
     */
    ASTNode(ASTOperation op, std::shared_ptr<ASTNode> child);

    /**
     * @brief Constructs a literal node (feature reference)
     * @param literal The feature name
     */
    explicit ASTNode(const std::string& literal);

    /**
     * @brief Constructs an integer constant node
     * @param value The integer value
     */
    explicit ASTNode(int value);

    /**
     * @brief Constructs a float constant node
     * @param value The floating-point value
     */
    explicit ASTNode(double value);

    /**
     * @brief Copy constructor
     * @param other The node to copy from
     */
    ASTNode(const ASTNode& other);

    /**
     * @brief Assignment operator
     * @param other The node to assign from
     * @return Reference to this node
     */
    ASTNode& operator=(const ASTNode& other);

    /**
     * @brief Destructor
     */
    ~ASTNode() = default;

    /**
     * @brief Gets the type of this node
     * @return The node type (OPERATION, LITERAL, INTEGER, FLOAT, or STRING)
     */
    Type get_type() const { return type; }

    /**
     * @brief Gets the operation type (for OPERATION nodes)
     * @return The operation type
     */
    ASTOperation get_operation() const { return operation; }

    /**
     * @brief Gets the literal/string value (for LITERAL/STRING nodes)
     * @return The feature name or string value
     */
    const std::string& get_literal() const { return literal_value; }

    /**
     * @brief Gets the integer value (for INTEGER nodes)
     * @return The integer value
     */
    int get_int_value() const { return int_value; }

    /**
     * @brief Gets the float value (for FLOAT nodes)
     * @return The floating-point value
     */
    double get_float_value() const { return float_value; }

    /**
     * @brief Gets the child nodes (for OPERATION nodes)
     * @return Vector of child node pointers
     */
    const std::vector<std::shared_ptr<ASTNode>>& get_children() const { return children; }

    /**
     * @brief Converts this AST to CNF clauses
     *
     * Converts the constraint expression represented by this AST to CNF format
     * using either Tseitin transformation or direct conversion.
     *
     * @param get_variable Function to map feature names to variable IDs
     * @param create_aux_var Function to create new auxiliary variables (for Tseitin mode)
     * @param mode Conversion mode (TSEITIN or STRAIGHTFORWARD)
     * @return Vector of CNF clauses, where each clause is a vector of literals
     */
    std::vector<std::vector<int>> get_clauses(
        std::function<int(const std::string&)> get_variable,
        std::function<int()> create_aux_var,
        CNFMode mode
    ) const;

    /**
     * @brief Checks if this is a simple literal node
     * @return true if this node is a LITERAL type
     */
    bool is_literal() const { return type == Type::LITERAL; }

    /**
     * @brief Checks if this is a boolean operation
     * @return true if operation is NOT, AND, OR, IMPLIES, or EQUIVALENCE
     */
    bool is_boolean_operation() const;

    /**
     * @brief Checks if this tree contains only boolean operations
     *
     * Recursively checks if this node and all descendants are pure boolean
     * operations (no arithmetic or comparison operations).
     *
     * @return true if tree is pure boolean
     */
    bool is_pure_boolean_tree() const;

    /**
     * @brief Converts this AST to a string representation
     * @return String representation for debugging
     */
    std::string to_string() const;

private:
    /**
     * @brief Performs Tseitin transformation on this AST
     *
     * Converts the AST to CNF using Tseitin transformation, which introduces
     * auxiliary variables to avoid clause explosion.
     *
     * @param clauses Output vector to append generated clauses
     * @param get_variable Function to map feature names to variable IDs
     * @param create_aux_var Function to create new auxiliary variables
     * @return Variable ID representing the result of this subtree
     */
    int tseitin_transform(
        std::vector<std::vector<int>>& clauses,
        std::function<int(const std::string&)> get_variable,
        std::function<int()> create_aux_var
    ) const;

    /// @brief Adds clauses for NOT operation (result <=> ~child)
    void add_not_clauses(int result, int child_var, std::vector<std::vector<int>>& clauses) const;

    /// @brief Adds clauses for AND operation (result <=> left & right)
    void add_and_clauses(int result, int left_var, int right_var, std::vector<std::vector<int>>& clauses) const;

    /// @brief Adds clauses for OR operation (result <=> left | right)
    void add_or_clauses(int result, int left_var, int right_var, std::vector<std::vector<int>>& clauses) const;

    /// @brief Adds clauses for IMPLIES operation (result <=> left => right)
    void add_implies_clauses(int result, int left_var, int right_var, std::vector<std::vector<int>>& clauses) const;

    /// @brief Adds clauses for EQUIVALENCE operation (result <=> left <=> right)
    void add_equivalence_clauses(int result, int left_var, int right_var, std::vector<std::vector<int>>& clauses) const;

    /**
     * @brief Converts AST to Negation Normal Form
     *
     * Transforms the AST by pushing negations down to literals using
     * De Morgan's laws and other logical equivalences.
     *
     * @param negated Whether this subtree is negated
     * @return New AST in NNF
     */
    std::shared_ptr<ASTNode> to_nnf(bool negated) const;

    /**
     * @brief Converts NNF to CNF using direct transformation
     *
     * Converts a Negation Normal Form AST directly to CNF clauses
     * using distributive laws (no auxiliary variables).
     *
     * @param get_variable Function to map feature names to variable IDs
     * @return Vector of CNF clauses
     */
    std::vector<std::vector<int>> to_cnf_direct(
        std::function<int(const std::string&)> get_variable
    ) const;

    /**
     * @brief Distributes OR over AND for CNF conversion
     *
     * Applies distributive law: (A & B) | C => (A | C) & (B | C)
     *
     * @param left_clauses Clauses from left operand
     * @param right_clauses Clauses from right operand
     * @return Distributed clauses
     */
    std::vector<std::vector<int>> distribute_or(
        const std::vector<std::vector<int>>& left_clauses,
        const std::vector<std::vector<int>>& right_clauses
    ) const;
};

#endif // ASTNODE_H
