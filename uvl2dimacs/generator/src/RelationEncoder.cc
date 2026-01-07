/**
 * @file RelationEncoder.cc
 * @brief Implementation of relation type encodings to CNF
 *
 * This file implements the RelationEncoder class which converts different
 * types of parent-child relations in feature models to CNF clauses:
 *
 * - **MANDATORY**: Child ⟺ Parent (2 clauses)
 * - **OPTIONAL**: Child → Parent (1 clause)
 * - **OR**: Parent → (at least one child) (n+1 clauses)
 * - **ALTERNATIVE**: Parent → (exactly one child) (O(n²) clauses pairwise)
 * - **CARDINALITY**: Parent → (min..max children) (enumeration-based)
 *
 * Each encoding follows standard feature model semantics and SAT encoding
 * techniques from the literature.
 *
 * @author UVL2Dimacs Team
 * @date 2024
 */

#include "RelationEncoder.hh"
#include "Feature.hh"
#include <stdexcept>
#include <algorithm>
#include <functional>

/**
 * @brief Constructs an encoder for the given CNF model
 *
 * @param model CNF model to add relation clauses to
 */
RelationEncoder::RelationEncoder(CNFModel& model)
    : cnf_model(model) {
}

/**
 * @brief Encodes a relation to CNF based on its type
 *
 * Dispatches to the appropriate encoding method based on relation type:
 * - MANDATORY → encode_mandatory()
 * - OPTIONAL → encode_optional()
 * - OR → encode_or()
 * - ALTERNATIVE → encode_alternative()
 * - CARDINALITY → encode_cardinality()
 *
 * @param relation The relation to encode
 * @throws std::runtime_error if relation type is unknown
 */
void RelationEncoder::encode_relation(std::shared_ptr<Relation> relation) {
    switch (relation->get_type()) {
        case Relation::Type::MANDATORY:
            encode_mandatory(relation);
            break;
        case Relation::Type::OPTIONAL:
            encode_optional(relation);
            break;
        case Relation::Type::OR:
            encode_or(relation);
            break;
        case Relation::Type::ALTERNATIVE:
            encode_alternative(relation);
            break;
        case Relation::Type::CARDINALITY:
            encode_cardinality(relation);
            break;
        default:
            throw std::runtime_error("Unknown relation type");
    }
}

/**
 * @brief Encodes a MANDATORY relation to CNF
 *
 * Semantics: parent ⟺ child (child is selected if and only if parent is selected)
 *
 * CNF Encoding (2 clauses):
 * 1. (¬parent ∨ child) - if parent is selected, child must be selected
 * 2. (¬child ∨ parent) - if child is selected, parent must be selected
 *
 * @param relation The mandatory relation (must have exactly 1 child)
 * @throws std::runtime_error if relation doesn't have exactly 1 child
 */
void RelationEncoder::encode_mandatory(std::shared_ptr<Relation> relation) {
    // Mandatory: parent <=> child
    // Clauses: (-parent OR child) AND (-child OR parent)

    auto parent = relation->get_parent();
    const auto& children = relation->get_children();

    if (children.size() != 1) {
        throw std::runtime_error("Mandatory relation must have exactly 1 child");
    }

    int parent_var = cnf_model.get_variable(parent->get_name());
    int child_var = cnf_model.get_variable(children[0]->get_name());

    // -parent OR child
    cnf_model.add_clause({-parent_var, child_var});

    // -child OR parent
    cnf_model.add_clause({-child_var, parent_var});
}

/**
 * @brief Encodes an OPTIONAL relation to CNF
 *
 * Semantics: child → parent (if child is selected, parent must be selected)
 *
 * CNF Encoding (1 clause):
 * - (¬child ∨ parent) - child implies parent
 *
 * Note: Parent can be selected without child (this is what makes it optional).
 *
 * @param relation The optional relation (must have exactly 1 child)
 * @throws std::runtime_error if relation doesn't have exactly 1 child
 */
void RelationEncoder::encode_optional(std::shared_ptr<Relation> relation) {
    // Optional: child => parent
    // Clause: (-child OR parent)

    auto parent = relation->get_parent();
    const auto& children = relation->get_children();

    if (children.size() != 1) {
        throw std::runtime_error("Optional relation must have exactly 1 child");
    }

    int parent_var = cnf_model.get_variable(parent->get_name());
    int child_var = cnf_model.get_variable(children[0]->get_name());

    // -child OR parent
    cnf_model.add_clause({-child_var, parent_var});
}

/**
 * @brief Encodes an OR relation to CNF
 *
 * Semantics: parent → (child₁ ∨ child₂ ∨ ... ∨ childₙ)
 * If parent is selected, at least one child must be selected.
 *
 * CNF Encoding (n+1 clauses):
 * 1. (¬parent ∨ child₁ ∨ child₂ ∨ ... ∨ childₙ) - at least one child
 * 2. For each child i: (¬childᵢ ∨ parent) - child implies parent
 *
 * @param relation The OR relation (must have at least 1 child)
 * @throws std::runtime_error if relation has no children
 */
void RelationEncoder::encode_or(std::shared_ptr<Relation> relation) {
    // Or: parent => (child1 OR child2 OR ... OR childN)
    //     AND each child => parent
    //
    // Clauses:
    // 1. (-parent OR child1 OR child2 OR ... OR childN)
    // 2. For each child i: (-child_i OR parent)

    auto parent = relation->get_parent();
    const auto& children = relation->get_children();

    if (children.empty()) {
        throw std::runtime_error("Or relation must have at least 1 child");
    }

    int parent_var = cnf_model.get_variable(parent->get_name());

    // Clause 1: -parent OR child1 OR child2 OR ... OR childN
    std::vector<int> or_clause = {-parent_var};
    for (const auto& child : children) {
        or_clause.push_back(cnf_model.get_variable(child->get_name()));
    }
    cnf_model.add_clause(or_clause);

    // Clause 2: For each child, -child OR parent
    for (const auto& child : children) {
        int child_var = cnf_model.get_variable(child->get_name());
        cnf_model.add_clause({-child_var, parent_var});
    }
}

/**
 * @brief Encodes an ALTERNATIVE relation to CNF
 *
 * Semantics: parent → (exactly one of children)
 * If parent is selected, exactly one child must be selected.
 *
 * CNF Encoding (1 + n(n-1)/2 + n clauses = O(n²)):
 * 1. (¬parent ∨ child₁ ∨ child₂ ∨ ... ∨ childₙ) - at least one child
 * 2. For each pair (i,j): (¬childᵢ ∨ ¬childⱼ) - at most one child (pairwise)
 * 3. For each child i: (¬childᵢ ∨ parent) - child implies parent
 *
 * Note: This uses pairwise encoding which is O(n²) in number of clauses.
 * For large n (≥10), consider using sequential counter encoding (Tseitin).
 *
 * @param relation The alternative relation (must have at least 2 children)
 * @throws std::runtime_error if relation has fewer than 2 children
 */
void RelationEncoder::encode_alternative(std::shared_ptr<Relation> relation) {
    // Alternative: parent => (exactly one of children)
    //
    // Clauses:
    // 1. (-parent OR child1 OR child2 OR ... OR childN)  [at least one]
    // 2. For each pair (i,j): (-child_i OR -child_j)     [at most one]
    // 3. For each child i: (-child_i OR parent)          [children => parent]

    auto parent = relation->get_parent();
    const auto& children = relation->get_children();

    if (children.size() < 2) {
        throw std::runtime_error("Alternative relation must have at least 2 children");
    }

    int parent_var = cnf_model.get_variable(parent->get_name());

    // Clause 1: -parent OR child1 OR child2 OR ... OR childN
    std::vector<int> or_clause = {-parent_var};
    for (const auto& child : children) {
        or_clause.push_back(cnf_model.get_variable(child->get_name()));
    }
    cnf_model.add_clause(or_clause);

    // Clause 2: For each pair (i,j), -child_i OR -child_j
    for (size_t i = 0; i < children.size(); ++i) {
        for (size_t j = i + 1; j < children.size(); ++j) {
            int child_i_var = cnf_model.get_variable(children[i]->get_name());
            int child_j_var = cnf_model.get_variable(children[j]->get_name());
            cnf_model.add_clause({-child_i_var, -child_j_var});
        }
    }

    // Clause 3: For each child, -child OR parent
    for (const auto& child : children) {
        int child_var = cnf_model.get_variable(child->get_name());
        cnf_model.add_clause({-child_var, parent_var});
    }
}

/**
 * @brief Encodes a CARDINALITY relation to CNF
 *
 * Semantics: parent → (select between min and max children)
 * If parent is selected, between min and max children (inclusive) must be selected.
 *
 * CNF Encoding (enumeration-based approach):
 * - For each possible count k of selected children (0 to n):
 *   - If k is invalid (k < min or k > max):
 *     For each combination C of k children:
 *       Add clause: (¬parent ∨ ¬(exactly C are selected))
 *   - If k is valid and k > 0:
 *     For each combination C of k children:
 *       Add clause: (parent ∨ ¬(exactly C are selected))
 * - For each child: (¬child ∨ parent)
 *
 * The clause "¬(exactly C are selected)" means at least one child in C is not
 * selected OR at least one child not in C is selected.
 *
 * Complexity: Can generate many clauses for complex cardinalities.
 * Number of clauses ≈ Σ C(n,k) for invalid counts, where C(n,k) = n!/(k!(n-k)!)
 *
 * Example: For [2..3] with 4 children, forbids counts 0, 1, and 4.
 *
 * @param relation The cardinality relation
 */
void RelationEncoder::encode_cardinality(std::shared_ptr<Relation> relation) {
    // Cardinality: parent => (select min..max children)
    // Uses enumeration approach from Python implementation
    //
    // For each invalid count (< min or > max):
    //   For each combination of that count:
    //     (-parent OR NOT(exactly this combination))
    //
    // Special case for valid counts with parent selected:
    //   For each valid count > 0:
    //     For each combination of that count:
    //       (parent OR NOT(exactly this combination))

    auto parent = relation->get_parent();
    const auto& children = relation->get_children();
    int card_min = relation->get_card_min();
    int card_max = relation->get_card_max();
    int num_children = children.size();

    int parent_var = cnf_model.get_variable(parent->get_name());

    // For each possible count of selected children
    for (int count = 0; count <= num_children; ++count) {
        bool is_invalid = (count < card_min || count > card_max);

        // Generate all combinations of 'count' children
        auto combinations = generate_combinations(num_children, count);

        for (const auto& combo : combinations) {
            std::vector<int> clause;

            if (is_invalid) {
                // Invalid count: -parent OR NOT(exactly this combination)
                clause.push_back(-parent_var);
            } else if (count > 0) {
                // Valid count > 0: parent OR NOT(exactly this combination)
                clause.push_back(parent_var);
            } else {
                // count == 0 and is valid: this handles the case where min=0
                // -parent OR NOT(exactly this combination)
                clause.push_back(-parent_var);
            }

            // Add literals for "NOT(exactly this combination)"
            // This means: at least one child in combo is false OR at least one child not in combo is true
            for (int i = 0; i < num_children; ++i) {
                int child_var = cnf_model.get_variable(children[i]->get_name());
                bool in_combo = std::find(combo.begin(), combo.end(), i) != combo.end();

                if (in_combo) {
                    // This child should be false (negated)
                    clause.push_back(-child_var);
                } else {
                    // This child should be true (positive)
                    clause.push_back(child_var);
                }
            }

            cnf_model.add_clause(clause);
        }
    }

    // Add clauses: each child => parent
    for (const auto& child : children) {
        int child_var = cnf_model.get_variable(child->get_name());
        cnf_model.add_clause({-child_var, parent_var});
    }
}

/**
 * @brief Generates all combinations of k elements from n elements
 *
 * Uses backtracking algorithm to generate all C(n,k) combinations,
 * where each combination is represented as a vector of indices [0, n-1].
 *
 * Mathematical formula: C(n,k) = n! / (k! × (n-k)!)
 *
 * Examples:
 * - generate_combinations(3, 2) → {{0,1}, {0,2}, {1,2}}
 * - generate_combinations(4, 0) → {{}}  (empty combination)
 * - generate_combinations(2, 3) → {}    (impossible, k > n)
 *
 * Algorithm complexity: O(C(n,k) × k) = O(n! / (k! × (n-k)!))
 *
 * @param n Total number of elements (children)
 * @param k Number of elements to choose
 * @return Vector of all combinations, where each combination is a vector of indices
 */
std::vector<std::vector<int>> RelationEncoder::generate_combinations(int n, int k) {
    std::vector<std::vector<int>> result;

    if (k > n || k < 0) {
        return result;
    }

    if (k == 0) {
        result.push_back({});
        return result;
    }

    // Generate combinations using backtracking
    std::vector<int> current;
    std::function<void(int, int)> backtrack = [&](int start, int depth) {
        if (depth == k) {
            result.push_back(current);
            return;
        }

        for (int i = start; i < n; ++i) {
            current.push_back(i);
            backtrack(i + 1, depth + 1);
            current.pop_back();
        }
    };

    backtrack(0, 0);
    return result;
}
