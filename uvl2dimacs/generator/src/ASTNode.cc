/**
 * @file ASTNode.cc
 * @brief Implementation of Abstract Syntax Tree (AST) node for constraint expressions
 *
 * This file implements the ASTNode class which represents constraint expressions
 * as an Abstract Syntax Tree. It provides two CNF conversion modes:
 * - Straightforward mode: Direct conversion using NNF and distribution (no auxiliary variables)
 * - Tseitin mode: Uses auxiliary variables for linear-size conversion
 *
 * The implementation handles both boolean operations (AND, OR, NOT, IMPLIES, EQUIVALENCE)
 * and non-boolean operations (comparison, arithmetic) which are treated as atomic propositions.
 *
 * @author UVL2Dimacs Team
 * @date 2024
 */

#include "ASTNode.hh"
#include <stdexcept>
#include <sstream>

/**
 * @brief Constructs a binary operation node
 *
 * Creates an AST node representing a binary operation (AND, OR, IMPLIES, etc.)
 * with two child nodes. The children are stored in the children vector.
 *
 * @param op The operation type (must be binary operation)
 * @param left Left operand as AST node
 * @param right Right operand as AST node
 */
ASTNode::ASTNode(ASTOperation op, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right)
    : type(Type::OPERATION), operation(op), int_value(0), float_value(0.0) {
    children.push_back(left);
    children.push_back(right);
}

/**
 * @brief Constructs a unary operation node
 *
 * Creates an AST node representing a unary operation (typically NOT)
 * with a single child node.
 *
 * @param op The operation type (must be unary operation like NOT)
 * @param child The operand as AST node
 */
ASTNode::ASTNode(ASTOperation op, std::shared_ptr<ASTNode> child)
    : type(Type::OPERATION), operation(op), int_value(0), float_value(0.0) {
    children.push_back(child);
}

/**
 * @brief Constructs a literal node (feature name)
 *
 * Creates an AST node representing a feature name or boolean variable.
 * This is a leaf node in the AST.
 *
 * @param literal The feature name or variable identifier
 */
ASTNode::ASTNode(const std::string& literal)
    : type(Type::LITERAL), literal_value(literal), int_value(0), float_value(0.0) {
}

/**
 * @brief Constructs an integer constant node
 *
 * Creates an AST node representing an integer value.
 * Used in arithmetic and comparison operations.
 *
 * @param value The integer value
 */
ASTNode::ASTNode(int value)
    : type(Type::INTEGER), int_value(value), float_value(0.0) {
}

/**
 * @brief Constructs a floating-point constant node
 *
 * Creates an AST node representing a floating-point value.
 * Used in arithmetic and comparison operations.
 *
 * @param value The floating-point value
 */
ASTNode::ASTNode(double value)
    : type(Type::FLOAT), float_value(value), int_value(0) {
}

/**
 * @brief Copy constructor with deep copy of children
 *
 * Creates a deep copy of the AST node including all child nodes.
 * Each child is recursively copied to create independent subtrees.
 *
 * @param other The AST node to copy from
 */
ASTNode::ASTNode(const ASTNode& other)
    : type(other.type),
      operation(other.operation),
      literal_value(other.literal_value),
      int_value(other.int_value),
      float_value(other.float_value) {

    // Deep copy children
    for (const auto& child : other.children) {
        children.push_back(std::make_shared<ASTNode>(*child));
    }
}

/**
 * @brief Assignment operator with deep copy
 *
 * Performs deep copy assignment including all child nodes.
 * Includes self-assignment check for safety.
 *
 * @param other The AST node to assign from
 * @return Reference to this node
 */
ASTNode& ASTNode::operator=(const ASTNode& other) {
    if (this != &other) {
        type = other.type;
        operation = other.operation;
        literal_value = other.literal_value;
        int_value = other.int_value;
        float_value = other.float_value;

        // Deep copy children
        children.clear();
        for (const auto& child : other.children) {
            children.push_back(std::make_shared<ASTNode>(*child));
        }
    }
    return *this;
}

/**
 * @brief Checks if this node represents a boolean operation
 *
 * Determines whether this node is an operation node with a boolean operator
 * (NOT, AND, OR, IMPLIES, EQUIVALENCE) as opposed to arithmetic or comparison operators.
 *
 * @return true if node is a boolean operation, false otherwise
 */
bool ASTNode::is_boolean_operation() const {
    if (type != Type::OPERATION) {
        return false;
    }

    return operation == ASTOperation::NOT ||
           operation == ASTOperation::AND ||
           operation == ASTOperation::OR ||
           operation == ASTOperation::IMPLIES ||
           operation == ASTOperation::EQUIVALENCE;
}

/**
 * @brief Checks if entire subtree contains only boolean operations
 *
 * Recursively checks whether this node and all descendants contain only
 * pure boolean operations and literals, without arithmetic or comparison operations.
 * This is used to filter out constraints that cannot be represented in CNF.
 *
 * @return true if entire subtree is pure boolean, false otherwise
 */
bool ASTNode::is_pure_boolean_tree() const {
    // Literals are pure boolean
    if (type == Type::LITERAL) {
        return true;
    }

    // Non-operation types (INTEGER, FLOAT, STRING) are not pure boolean
    if (type != Type::OPERATION) {
        return false;
    }

    // Check if this operation is boolean
    if (!is_boolean_operation()) {
        return false;  // Comparison or arithmetic operation
    }

    // Recursively check all children
    for (const auto& child : children) {
        if (!child->is_pure_boolean_tree()) {
            return false;
        }
    }

    return true;
}

/**
 * @brief Converts AST to string representation
 *
 * Creates a human-readable string representation of the AST node and its
 * subtree using prefix notation for operations (e.g., "(AND A B)").
 *
 * @return String representation of the AST
 */
std::string ASTNode::to_string() const {
    std::ostringstream oss;

    switch (type) {
        case Type::LITERAL:
            oss << literal_value;
            break;
        case Type::INTEGER:
            oss << int_value;
            break;
        case Type::FLOAT:
            oss << float_value;
            break;
        case Type::STRING:
            oss << "\"" << literal_value << "\"";
            break;
        case Type::OPERATION: {
            std::string op_str;
            switch (operation) {
                case ASTOperation::NOT: op_str = "NOT"; break;
                case ASTOperation::AND: op_str = "AND"; break;
                case ASTOperation::OR: op_str = "OR"; break;
                case ASTOperation::IMPLIES: op_str = "IMPLIES"; break;
                case ASTOperation::EQUIVALENCE: op_str = "EQUIVALENCE"; break;
                case ASTOperation::EQUALS: op_str = "EQUALS"; break;
                case ASTOperation::NOT_EQUALS: op_str = "NOT_EQUALS"; break;
                case ASTOperation::LOWER: op_str = "LOWER"; break;
                case ASTOperation::LOWER_EQUALS: op_str = "LOWER_EQUALS"; break;
                case ASTOperation::GREATER: op_str = "GREATER"; break;
                case ASTOperation::GREATER_EQUALS: op_str = "GREATER_EQUALS"; break;
                case ASTOperation::ADD: op_str = "ADD"; break;
                case ASTOperation::SUB: op_str = "SUB"; break;
                case ASTOperation::MUL: op_str = "MUL"; break;
                case ASTOperation::DIV: op_str = "DIV"; break;
                case ASTOperation::SUM: op_str = "SUM"; break;
                case ASTOperation::AVG: op_str = "AVG"; break;
                case ASTOperation::LEN: op_str = "LEN"; break;
                case ASTOperation::FLOOR: op_str = "FLOOR"; break;
                case ASTOperation::CEIL: op_str = "CEIL"; break;
            }
            oss << "(" << op_str;
            for (const auto& child : children) {
                oss << " " << child->to_string();
            }
            oss << ")";
            break;
        }
    }

    return oss.str();
}

/**
 * @brief Converts AST to CNF clauses
 *
 * Main entry point for converting the AST to Conjunctive Normal Form.
 * Supports two conversion modes:
 *
 * **Straightforward Mode**:
 * - Converts to NNF (Negation Normal Form) first
 * - Then distributes OR over AND to get CNF
 * - No auxiliary variables needed
 * - May produce longer clauses
 *
 * **Tseitin Mode**:
 * - Uses Tseitin transformation
 * - Creates auxiliary variables for each operation
 * - Results in linear-size CNF with shorter clauses (max 3 literals)
 * - More variables but often faster for SAT solvers
 *
 * @param get_variable Function to map feature names to variable IDs
 * @param create_aux_var Function to create new auxiliary variables (Tseitin mode)
 * @param mode Conversion mode (STRAIGHTFORWARD or TSEITIN)
 * @return Vector of CNF clauses, where each clause is a vector of literals
 */
std::vector<std::vector<int>> ASTNode::get_clauses(
    std::function<int(const std::string&)> get_variable,
    std::function<int()> create_aux_var,
    CNFMode mode
) const {
    if (mode == CNFMode::TSEITIN) {
        // Use Tseitin transformation with auxiliary variables
        std::vector<std::vector<int>> clauses;
        int root_var = tseitin_transform(clauses, get_variable, create_aux_var);
        // The root expression must be true
        clauses.push_back({root_var});
        return clauses;
    } else {
        // Use straightforward conversion without auxiliary variables
        // Step 1: Convert to NNF
        auto nnf = to_nnf(false);  // false = not negated
        // Step 2: Convert NNF to CNF
        return nnf->to_cnf_direct(get_variable);
    }
}

/**
 * @brief Performs Tseitin transformation on AST
 *
 * Recursively transforms the AST into CNF using the Tseitin encoding.
 * For each operation node, creates an auxiliary variable and adds clauses
 * expressing the equivalence between the auxiliary variable and the operation result.
 *
 * The transformation guarantees linear-size output and clauses with at most 3 literals.
 *
 * For each operator:
 * - NOT: result ⟺ ¬child
 * - AND: result ⟺ (left ∧ right)
 * - OR: result ⟺ (left ∨ right)
 * - IMPLIES: result ⟺ (left → right)
 * - EQUIVALENCE: result ⟺ (left ⟺ right)
 *
 * @param clauses Output vector to append generated clauses to
 * @param get_variable Function to map feature names to variable IDs
 * @param create_aux_var Function to create new auxiliary variables
 * @return Variable ID representing the result of this subtree
 */
int ASTNode::tseitin_transform(
    std::vector<std::vector<int>>& clauses,
    std::function<int(const std::string&)> get_variable,
    std::function<int()> create_aux_var
) const {
    // Base case: literal
    if (type == Type::LITERAL) {
        return get_variable(literal_value);
    }

    // For comparison and arithmetic operations, treat them as atomic boolean propositions
    // Create a pseudo-variable name from the string representation
    if (!is_boolean_operation()) {
        std::string atom_name = "_cmp_" + to_string();
        return get_variable(atom_name);
    }

    // Handle boolean operations with Tseitin transformation
    switch (operation) {
        case ASTOperation::NOT: {
            if (children.size() != 1) {
                throw std::runtime_error("NOT operation must have exactly 1 child");
            }
            int child_var = children[0]->tseitin_transform(clauses, get_variable, create_aux_var);
            int result_var = create_aux_var();
            add_not_clauses(result_var, child_var, clauses);
            return result_var;
        }

        case ASTOperation::AND: {
            if (children.size() != 2) {
                throw std::runtime_error("AND operation must have exactly 2 children");
            }
            int left_var = children[0]->tseitin_transform(clauses, get_variable, create_aux_var);
            int right_var = children[1]->tseitin_transform(clauses, get_variable, create_aux_var);
            int result_var = create_aux_var();
            add_and_clauses(result_var, left_var, right_var, clauses);
            return result_var;
        }

        case ASTOperation::OR: {
            if (children.size() != 2) {
                throw std::runtime_error("OR operation must have exactly 2 children");
            }
            int left_var = children[0]->tseitin_transform(clauses, get_variable, create_aux_var);
            int right_var = children[1]->tseitin_transform(clauses, get_variable, create_aux_var);
            int result_var = create_aux_var();
            add_or_clauses(result_var, left_var, right_var, clauses);
            return result_var;
        }

        case ASTOperation::IMPLIES: {
            if (children.size() != 2) {
                throw std::runtime_error("IMPLIES operation must have exactly 2 children");
            }
            int left_var = children[0]->tseitin_transform(clauses, get_variable, create_aux_var);
            int right_var = children[1]->tseitin_transform(clauses, get_variable, create_aux_var);
            int result_var = create_aux_var();
            add_implies_clauses(result_var, left_var, right_var, clauses);
            return result_var;
        }

        case ASTOperation::EQUIVALENCE: {
            if (children.size() != 2) {
                throw std::runtime_error("EQUIVALENCE operation must have exactly 2 children");
            }
            int left_var = children[0]->tseitin_transform(clauses, get_variable, create_aux_var);
            int right_var = children[1]->tseitin_transform(clauses, get_variable, create_aux_var);
            int result_var = create_aux_var();
            add_equivalence_clauses(result_var, left_var, right_var, clauses);
            return result_var;
        }

        default:
            throw std::runtime_error("Unsupported boolean operation in Tseitin transformation");
    }
}

/**
 * @brief Adds Tseitin clauses for NOT operation
 *
 * Encodes: result ⟺ ¬child
 *
 * Generated clauses:
 * 1. (result ∨ child) - if result is true, child must be false
 * 2. (¬result ∨ ¬child) - if child is true, result must be false
 *
 * @param result Variable ID representing the NOT result
 * @param child_var Variable ID of the operand
 * @param clauses Output vector to append clauses to
 */
void ASTNode::add_not_clauses(int result, int child_var, std::vector<std::vector<int>>& clauses) const {
    clauses.push_back({result, child_var});
    clauses.push_back({-result, -child_var});
}

/**
 * @brief Adds Tseitin clauses for AND operation
 *
 * Encodes: result ⟺ (left ∧ right)
 *
 * Generated clauses:
 * 1. (¬result ∨ left) - if result is true, left must be true
 * 2. (¬result ∨ right) - if result is true, right must be true
 * 3. (result ∨ ¬left ∨ ¬right) - if both are true, result must be true
 *
 * @param result Variable ID representing the AND result
 * @param left_var Variable ID of left operand
 * @param right_var Variable ID of right operand
 * @param clauses Output vector to append clauses to
 */
void ASTNode::add_and_clauses(int result, int left_var, int right_var, std::vector<std::vector<int>>& clauses) const {
    clauses.push_back({-result, left_var});
    clauses.push_back({-result, right_var});
    clauses.push_back({result, -left_var, -right_var});
}

/**
 * @brief Adds Tseitin clauses for OR operation
 *
 * Encodes: result ⟺ (left ∨ right)
 *
 * Generated clauses:
 * 1. (¬result ∨ left ∨ right) - if result is true, at least one must be true
 * 2. (result ∨ ¬left) - if left is true, result must be true
 * 3. (result ∨ ¬right) - if right is true, result must be true
 *
 * @param result Variable ID representing the OR result
 * @param left_var Variable ID of left operand
 * @param right_var Variable ID of right operand
 * @param clauses Output vector to append clauses to
 */
void ASTNode::add_or_clauses(int result, int left_var, int right_var, std::vector<std::vector<int>>& clauses) const {
    clauses.push_back({-result, left_var, right_var});
    clauses.push_back({result, -left_var});
    clauses.push_back({result, -right_var});
}

/**
 * @brief Adds Tseitin clauses for IMPLIES operation
 *
 * Encodes: result ⟺ (left → right)
 * Equivalent to: result ⟺ (¬left ∨ right)
 *
 * Generated clauses:
 * 1. (¬result ∨ ¬left ∨ right) - if result is true, implication holds
 * 2. (result ∨ left) - if left is false, result can be true
 * 3. (result ∨ ¬right) - if right is false, result can be true
 *
 * @param result Variable ID representing the IMPLIES result
 * @param left_var Variable ID of left operand (antecedent)
 * @param right_var Variable ID of right operand (consequent)
 * @param clauses Output vector to append clauses to
 */
void ASTNode::add_implies_clauses(int result, int left_var, int right_var, std::vector<std::vector<int>>& clauses) const {
    clauses.push_back({-result, -left_var, right_var});
    clauses.push_back({result, left_var});
    clauses.push_back({result, -right_var});
}

/**
 * @brief Adds Tseitin clauses for EQUIVALENCE operation
 *
 * Encodes: result ⟺ (left ⟺ right)
 * Equivalent to: result ⟺ ((left ∧ right) ∨ (¬left ∧ ¬right))
 *
 * Generated clauses:
 * 1. (¬result ∨ left ∨ ¬right) - covers case where both are same
 * 2. (¬result ∨ ¬left ∨ right) - covers case where both are same
 * 3. (result ∨ left ∨ right) - if different, result is false
 * 4. (result ∨ ¬left ∨ ¬right) - if different, result is false
 *
 * @param result Variable ID representing the EQUIVALENCE result
 * @param left_var Variable ID of left operand
 * @param right_var Variable ID of right operand
 * @param clauses Output vector to append clauses to
 */
void ASTNode::add_equivalence_clauses(int result, int left_var, int right_var, std::vector<std::vector<int>>& clauses) const {
    clauses.push_back({-result, left_var, -right_var});
    clauses.push_back({-result, -left_var, right_var});
    clauses.push_back({result, left_var, right_var});
    clauses.push_back({result, -left_var, -right_var});
}

// ===== Straightforward CNF Conversion (No Auxiliary Variables) =====

/**
 * @brief Converts AST to Negation Normal Form (NNF)
 *
 * NNF is a normalized form where negations only appear directly before literals.
 * This is achieved by:
 * - Applying De Morgan's laws: ¬(A ∧ B) = (¬A ∨ ¬B), ¬(A ∨ B) = (¬A ∧ ¬B)
 * - Eliminating double negations: ¬¬A = A
 * - Rewriting implications: A → B = ¬A ∨ B
 * - Expanding equivalences: A ⟺ B = (A ∧ B) ∨ (¬A ∧ ¬B)
 *
 * NNF is a prerequisite for straightforward CNF conversion.
 *
 * @param negated Whether this node should be negated (used for recursion)
 * @return New AST in Negation Normal Form
 */
std::shared_ptr<ASTNode> ASTNode::to_nnf(bool negated) const {
    // Base case: literal
    if (type == Type::LITERAL) {
        if (negated) {
            // Create NOT of this literal
            return std::make_shared<ASTNode>(ASTOperation::NOT,
                std::make_shared<ASTNode>(literal_value));
        } else {
            return std::make_shared<ASTNode>(literal_value);
        }
    }

    // Base case: numeric values (integers, floats, strings)
    // These can't be negated, just return as-is
    if (type == Type::INTEGER || type == Type::FLOAT || type == Type::STRING) {
        return std::make_shared<ASTNode>(*this);
    }

    // For comparison and arithmetic operations, treat them as atomic boolean propositions
    // We create a string representation as a pseudo-variable name
    if (!is_boolean_operation()) {
        std::string atom_name = "_cmp_" + to_string();
        if (negated) {
            return std::make_shared<ASTNode>(ASTOperation::NOT,
                std::make_shared<ASTNode>(atom_name));
        } else {
            return std::make_shared<ASTNode>(atom_name);
        }
    }

    // Handle boolean operations
    switch (operation) {
        case ASTOperation::NOT: {
            // Double negation: NOT(NOT(A)) = A
            if (children.size() != 1) {
                throw std::runtime_error("NOT must have exactly 1 child");
            }
            // Flip the negation and recurse
            return children[0]->to_nnf(!negated);
        }

        case ASTOperation::AND: {
            if (children.size() != 2) {
                throw std::runtime_error("AND must have exactly 2 children");
            }
            if (negated) {
                // De Morgan: NOT(A AND B) = (NOT A) OR (NOT B)
                auto left_nnf = children[0]->to_nnf(true);
                auto right_nnf = children[1]->to_nnf(true);
                return std::make_shared<ASTNode>(ASTOperation::OR, left_nnf, right_nnf);
            } else {
                // A AND B
                auto left_nnf = children[0]->to_nnf(false);
                auto right_nnf = children[1]->to_nnf(false);
                return std::make_shared<ASTNode>(ASTOperation::AND, left_nnf, right_nnf);
            }
        }

        case ASTOperation::OR: {
            if (children.size() != 2) {
                throw std::runtime_error("OR must have exactly 2 children");
            }
            if (negated) {
                // De Morgan: NOT(A OR B) = (NOT A) AND (NOT B)
                auto left_nnf = children[0]->to_nnf(true);
                auto right_nnf = children[1]->to_nnf(true);
                return std::make_shared<ASTNode>(ASTOperation::AND, left_nnf, right_nnf);
            } else {
                // A OR B
                auto left_nnf = children[0]->to_nnf(false);
                auto right_nnf = children[1]->to_nnf(false);
                return std::make_shared<ASTNode>(ASTOperation::OR, left_nnf, right_nnf);
            }
        }

        case ASTOperation::IMPLIES: {
            if (children.size() != 2) {
                throw std::runtime_error("IMPLIES must have exactly 2 children");
            }
            // A IMPLIES B = NOT A OR B
            if (negated) {
                // NOT(A IMPLIES B) = NOT(NOT A OR B) = A AND NOT B
                auto left_nnf = children[0]->to_nnf(false);
                auto right_nnf = children[1]->to_nnf(true);
                return std::make_shared<ASTNode>(ASTOperation::AND, left_nnf, right_nnf);
            } else {
                // A IMPLIES B = NOT A OR B
                auto left_nnf = children[0]->to_nnf(true);
                auto right_nnf = children[1]->to_nnf(false);
                return std::make_shared<ASTNode>(ASTOperation::OR, left_nnf, right_nnf);
            }
        }

        case ASTOperation::EQUIVALENCE: {
            if (children.size() != 2) {
                throw std::runtime_error("EQUIVALENCE must have exactly 2 children");
            }
            // A <=> B = (A AND B) OR (NOT A AND NOT B)
            if (negated) {
                // NOT(A <=> B) = (A AND NOT B) OR (NOT A AND B)
                auto left_pos = children[0]->to_nnf(false);
                auto left_neg = children[0]->to_nnf(true);
                auto right_pos = children[1]->to_nnf(false);
                auto right_neg = children[1]->to_nnf(true);
                auto and1 = std::make_shared<ASTNode>(ASTOperation::AND, left_pos, right_neg);
                auto and2 = std::make_shared<ASTNode>(ASTOperation::AND, left_neg, right_pos);
                return std::make_shared<ASTNode>(ASTOperation::OR, and1, and2);
            } else {
                // A <=> B = (A AND B) OR (NOT A AND NOT B)
                auto left_pos = children[0]->to_nnf(false);
                auto left_neg = children[0]->to_nnf(true);
                auto right_pos = children[1]->to_nnf(false);
                auto right_neg = children[1]->to_nnf(true);
                auto and1 = std::make_shared<ASTNode>(ASTOperation::AND, left_pos, right_pos);
                auto and2 = std::make_shared<ASTNode>(ASTOperation::AND, left_neg, right_neg);
                return std::make_shared<ASTNode>(ASTOperation::OR, and1, and2);
            }
        }

        default:
            throw std::runtime_error("Unsupported operation in NNF conversion: " + to_string());
    }
}

/**
 * @brief Converts NNF to CNF using distribution law
 *
 * Converts an AST in Negation Normal Form to Conjunctive Normal Form
 * by applying the distributive law: (A ∨ (B ∧ C)) = (A ∨ B) ∧ (A ∨ C)
 *
 * The NNF structure guarantees that:
 * - Only AND, OR, and NOT operations appear
 * - NOT only appears before literals
 *
 * Algorithm:
 * - Literals and negated literals become unit clauses
 * - AND nodes: concatenate clauses from both children
 * - OR nodes: distribute OR over AND using distribute_or()
 *
 * Note: This may produce exponentially many clauses for deeply nested expressions.
 * For complex constraints, Tseitin mode is recommended.
 *
 * @param get_variable Function to map feature names to variable IDs
 * @return Vector of CNF clauses
 */
std::vector<std::vector<int>> ASTNode::to_cnf_direct(
    std::function<int(const std::string&)> get_variable
) const {
    // Base case: literal
    if (type == Type::LITERAL) {
        int var = get_variable(literal_value);
        return {{var}};
    }

    // Handle NOT of literal (from NNF conversion)
    if (operation == ASTOperation::NOT) {
        if (children.size() != 1 || children[0]->get_type() != Type::LITERAL) {
            throw std::runtime_error("In NNF, NOT should only apply to literals");
        }
        int var = get_variable(children[0]->get_literal());
        return {{-var}};
    }

    // Handle AND: concatenate clauses from both children
    if (operation == ASTOperation::AND) {
        if (children.size() != 2) {
            throw std::runtime_error("AND must have exactly 2 children");
        }
        auto left_clauses = children[0]->to_cnf_direct(get_variable);
        auto right_clauses = children[1]->to_cnf_direct(get_variable);
        
        // Concatenate clauses
        std::vector<std::vector<int>> result = left_clauses;
        result.insert(result.end(), right_clauses.begin(), right_clauses.end());
        return result;
    }

    // Handle OR: distribute over AND
    if (operation == ASTOperation::OR) {
        if (children.size() != 2) {
            throw std::runtime_error("OR must have exactly 2 children");
        }
        auto left_clauses = children[0]->to_cnf_direct(get_variable);
        auto right_clauses = children[1]->to_cnf_direct(get_variable);
        
        // Distribute OR over AND
        return distribute_or(left_clauses, right_clauses);
    }

    throw std::runtime_error("Unexpected operation in CNF conversion: " + to_string());
}

/**
 * @brief Distributes OR over AND to convert to CNF
 *
 * Implements the distributive law for CNF conversion:
 * (A₁ ∧ A₂ ∧ ...) ∨ (B₁ ∧ B₂ ∧ ...) = (A₁ ∨ B₁) ∧ (A₁ ∨ B₂) ∧ ... ∧ (A₂ ∨ B₁) ∧ ...
 *
 * For each clause from the left side and each clause from the right side,
 * creates a new clause that is the union (OR) of both clauses.
 *
 * Complexity: O(|left_clauses| × |right_clauses|)
 * Warning: Can produce exponentially many clauses for deeply nested expressions.
 *
 * Example:
 * - Left: {{1}, {2}} represents (1) ∧ (2)
 * - Right: {{3, 4}} represents (3 ∨ 4)
 * - Result: {{1, 3, 4}, {2, 3, 4}} represents (1 ∨ 3 ∨ 4) ∧ (2 ∨ 3 ∨ 4)
 *
 * @param left_clauses CNF clauses from left operand
 * @param right_clauses CNF clauses from right operand
 * @return Distributed CNF clauses
 */
std::vector<std::vector<int>> ASTNode::distribute_or(
    const std::vector<std::vector<int>>& left_clauses,
    const std::vector<std::vector<int>>& right_clauses
) const {
    std::vector<std::vector<int>> result;
    
    // For each clause from left and each clause from right, merge them
    for (const auto& left_clause : left_clauses) {
        for (const auto& right_clause : right_clauses) {
            // Merge the two clauses (union of literals)
            std::vector<int> merged = left_clause;
            merged.insert(merged.end(), right_clause.begin(), right_clause.end());
            result.push_back(merged);
        }
    }
    
    return result;
}
