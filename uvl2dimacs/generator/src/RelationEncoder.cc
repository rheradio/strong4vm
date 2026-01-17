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
 * @param conversion_mode CNF mode (STRAIGHTFORWARD or TSEITIN for 3-CNF)
 */
RelationEncoder::RelationEncoder(CNFModel& model, CNFMode conversion_mode)
    : cnf_model(model), mode(conversion_mode) {
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
 * STRAIGHTFORWARD mode (n+1 clauses):
 * 1. (¬parent ∨ child₁ ∨ child₂ ∨ ... ∨ childₙ) - at least one child
 * 2. For each child i: (¬childᵢ ∨ parent) - child implies parent
 *
 * TSEITIN mode (3-CNF with auxiliary variables):
 * Uses tree decomposition to ensure all clauses have ≤3 literals.
 *
 * @param relation The OR relation (must have at least 1 child)
 * @throws std::runtime_error if relation has no children
 */
void RelationEncoder::encode_or(std::shared_ptr<Relation> relation) {
    auto parent = relation->get_parent();
    const auto& children = relation->get_children();

    if (children.empty()) {
        throw std::runtime_error("Or relation must have at least 1 child");
    }

    int parent_var = cnf_model.get_variable(parent->get_name());

    // Collect child variables
    std::vector<int> child_vars;
    for (const auto& child : children) {
        child_vars.push_back(cnf_model.get_variable(child->get_name()));
    }

    // Encode "at least one child" constraint
    if (mode == CNFMode::TSEITIN && children.size() > 2) {
        // TSEITIN: Use tree decomposition for 3-CNF
        int or_result = encode_or_tree(child_vars);
        cnf_model.add_clause({-parent_var, or_result});
    } else {
        // STRAIGHTFORWARD: Direct encoding (may have >3 literals)
        std::vector<int> or_clause = {-parent_var};
        for (int cv : child_vars) {
            or_clause.push_back(cv);
        }
        cnf_model.add_clause(or_clause);
    }

    // Each child implies parent (always 2 literals)
    for (int child_var : child_vars) {
        cnf_model.add_clause({-child_var, parent_var});
    }
}

/**
 * @brief Encodes an ALTERNATIVE relation to CNF
 *
 * Semantics: parent → (exactly one of children)
 * If parent is selected, exactly one child must be selected.
 *
 * STRAIGHTFORWARD mode (1 + n(n-1)/2 + n clauses = O(n²)):
 * 1. (¬parent ∨ child₁ ∨ child₂ ∨ ... ∨ childₙ) - at least one child
 * 2. For each pair (i,j): (¬childᵢ ∨ ¬childⱼ) - at most one child (pairwise)
 * 3. For each child i: (¬childᵢ ∨ parent) - child implies parent
 *
 * TSEITIN mode (3-CNF with auxiliary variables):
 * Uses tree decomposition for "at least one" to ensure ≤3 literals.
 * Pairwise "at most one" already has 2 literals per clause.
 *
 * @param relation The alternative relation (must have at least 2 children)
 * @throws std::runtime_error if relation has fewer than 2 children
 */
void RelationEncoder::encode_alternative(std::shared_ptr<Relation> relation) {
    auto parent = relation->get_parent();
    const auto& children = relation->get_children();

    if (children.size() < 2) {
        throw std::runtime_error("Alternative relation must have at least 2 children");
    }

    int parent_var = cnf_model.get_variable(parent->get_name());

    // Collect child variables
    std::vector<int> child_vars;
    for (const auto& child : children) {
        child_vars.push_back(cnf_model.get_variable(child->get_name()));
    }

    // Encode "at least one child" constraint
    if (mode == CNFMode::TSEITIN && children.size() > 2) {
        // TSEITIN: Use tree decomposition for 3-CNF
        int or_result = encode_or_tree(child_vars);
        cnf_model.add_clause({-parent_var, or_result});
    } else {
        // STRAIGHTFORWARD: Direct encoding
        std::vector<int> or_clause = {-parent_var};
        for (int cv : child_vars) {
            or_clause.push_back(cv);
        }
        cnf_model.add_clause(or_clause);
    }

    // Encode "at most one child" constraint (pairwise - already 2 literals)
    for (size_t i = 0; i < child_vars.size(); ++i) {
        for (size_t j = i + 1; j < child_vars.size(); ++j) {
            cnf_model.add_clause({-child_vars[i], -child_vars[j]});
        }
    }

    // Each child implies parent (always 2 literals)
    for (int child_var : child_vars) {
        cnf_model.add_clause({-child_var, parent_var});
    }
}

/**
 * @brief Encodes a CARDINALITY relation to CNF
 *
 * Semantics: parent → (select between min and max children)
 * If parent is selected, between min and max children (inclusive) must be selected.
 *
 * STRAIGHTFORWARD mode (enumeration-based approach):
 * - For each possible count k of selected children (0 to n):
 *   - If k is invalid (k < min or k > max):
 *     For each combination C of k children:
 *       Add clause: (¬parent ∨ ¬(exactly C are selected))
 * - For each child: (¬child ∨ parent)
 *
 * TSEITIN mode (3-CNF with auxiliary variables):
 * When clauses would exceed 3 literals, uses OR tree decomposition.
 *
 * Complexity: Can generate many clauses for complex cardinalities.
 * Number of clauses ≈ Σ C(n,k) for invalid counts.
 *
 * @param relation The cardinality relation
 */
void RelationEncoder::encode_cardinality(std::shared_ptr<Relation> relation) {
    auto parent = relation->get_parent();
    const auto& children = relation->get_children();
    int card_min = relation->get_card_min();
    int card_max = relation->get_card_max();
    int num_children = children.size();

    int parent_var = cnf_model.get_variable(parent->get_name());

    // Collect child variables
    std::vector<int> child_vars;
    for (const auto& child : children) {
        child_vars.push_back(cnf_model.get_variable(child->get_name()));
    }

    // For each possible count of selected children
    for (int count = 0; count <= num_children; ++count) {
        bool is_invalid = (count < card_min || count > card_max);

        // Generate all combinations of 'count' children
        auto combinations = generate_combinations(num_children, count);

        for (const auto& combo : combinations) {
            // Determine the first literal (parent-related)
            int first_lit;
            if (is_invalid) {
                first_lit = -parent_var;
            } else if (count > 0) {
                first_lit = parent_var;
            } else {
                first_lit = -parent_var;
            }

            // Build literals for "NOT(exactly this combination)"
            std::vector<int> combo_lits;
            for (int i = 0; i < num_children; ++i) {
                bool in_combo = std::find(combo.begin(), combo.end(), i) != combo.end();
                if (in_combo) {
                    combo_lits.push_back(-child_vars[i]);
                } else {
                    combo_lits.push_back(child_vars[i]);
                }
            }

            // Add clause based on mode
            if (mode == CNFMode::TSEITIN && combo_lits.size() > 2) {
                // TSEITIN: Use OR tree to keep clauses at ≤3 literals
                int or_result = encode_or_tree(combo_lits);
                cnf_model.add_clause({first_lit, or_result});
            } else {
                // STRAIGHTFORWARD or small clause: Direct encoding
                std::vector<int> clause = {first_lit};
                for (int lit : combo_lits) {
                    clause.push_back(lit);
                }
                cnf_model.add_clause(clause);
            }
        }
    }

    // Add clauses: each child => parent (always 2 literals)
    for (int child_var : child_vars) {
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

/**
 * @brief Builds an OR tree with auxiliary variables for 3-CNF encoding
 *
 * Recursively decomposes an n-ary OR into binary ORs using auxiliary
 * variables. Each binary OR produces 3 clauses with at most 3 literals:
 * - (¬aux ∨ v1 ∨ v2): aux implies (v1 ∨ v2)
 * - (aux ∨ ¬v1): v1 implies aux
 * - (aux ∨ ¬v2): v2 implies aux
 *
 * For n variables, creates O(n) auxiliary variables and O(3n) clauses,
 * all with at most 3 literals (3-CNF compliant).
 *
 * @param vars Vector of variable IDs (can be positive or negative literals)
 * @return Variable ID representing the OR of all inputs
 */
int RelationEncoder::encode_or_tree(const std::vector<int>& vars) {
    if (vars.empty()) {
        // Should not happen, but handle gracefully
        return 0;
    }

    if (vars.size() == 1) {
        // Single variable: return as-is
        return vars[0];
    }

    if (vars.size() == 2) {
        // Base case: create aux = (v1 ∨ v2)
        int aux = cnf_model.create_auxiliary_variable("or_tree");

        // aux ↔ (v1 ∨ v2) requires 3 clauses:
        // (¬aux ∨ v1 ∨ v2): aux implies at least one of v1, v2
        cnf_model.add_clause({-aux, vars[0], vars[1]});
        // (aux ∨ ¬v1): v1 implies aux
        cnf_model.add_clause({aux, -vars[0]});
        // (aux ∨ ¬v2): v2 implies aux
        cnf_model.add_clause({aux, -vars[1]});

        return aux;
    }

    // Recursive case: split in half and combine
    size_t mid = vars.size() / 2;
    std::vector<int> left_half(vars.begin(), vars.begin() + mid);
    std::vector<int> right_half(vars.begin() + mid, vars.end());

    int left_result = encode_or_tree(left_half);
    int right_result = encode_or_tree(right_half);

    // Combine the two halves
    return encode_or_tree({left_result, right_result});
}
