/**
 * @file RelationEncoder.hh
 * @brief Encoder for converting feature relations to CNF clauses
 *
 * This file defines the RelationEncoder class which converts parent-child relations
 * in a feature model into equivalent CNF (Conjunctive Normal Form) clauses.
 *
 * @author UVL2Dimacs Team
 * @date 2024
 */

#ifndef RELATIONENCODER_H
#define RELATIONENCODER_H

#include "Relation.hh"
#include "CNFModel.hh"
#include <vector>
#include <memory>

/**
 * @class RelationEncoder
 * @brief Encodes feature model relations as CNF clauses
 *
 * RelationEncoder converts parent-child relations with cardinality constraints
 * into logically equivalent CNF clauses. Each relation type has specific encoding rules:
 *
 * **MANDATORY** (parent <=> child):
 * - Bidirectional implication between parent and child
 * - Clauses: (~parent | child) & (~child | parent)
 *
 * **OPTIONAL** (child => parent):
 * - Child can only be selected if parent is selected
 * - Clause: (~child | parent)
 *
 * **OR** (parent => at least one child):
 * - If parent is selected, at least one child must be selected
 * - Clauses: (~parent | child1 | child2 | ... | childN)
 * - Plus: each child => parent
 *
 * **ALTERNATIVE** (parent => exactly one child):
 * - If parent is selected, exactly one child must be selected
 * - Clauses: (~parent | child1 | ... | childN) [at least one]
 * - Plus: (~childi | ~childj) for all pairs [at most one]
 * - Plus: each child => parent
 *
 * **CARDINALITY** (parent => min..max children):
 * - If parent is selected, between min and max children must be selected
 * - Uses enumeration approach with combinations
 * - Generates clauses enforcing min and max bounds
 *
 * @see Relation for relation types and cardinality semantics
 * @see CNFModel for the CNF representation
 * @see FMToCNF for the overall transformation process
 *
 * Example:
 * @code
 * CNFModel cnf;
 * RelationEncoder encoder(cnf);
 * encoder.encode_relation(mandatory_relation);  // Encodes parent <=> child
 * @endcode
 */
class RelationEncoder {
private:
    CNFModel& cnf_model;  ///< Reference to the CNF model to add clauses to

public:
    /**
     * @brief Constructs an encoder for the given CNF model
     *
     * @param model Reference to the CNF model where clauses will be added
     */
    explicit RelationEncoder(CNFModel& model);

    /**
     * @brief Destructor
     */
    ~RelationEncoder() = default;

    /**
     * @brief Encodes a relation into CNF clauses
     *
     * Determines the relation type and calls the appropriate encoding method.
     * The generated clauses are added directly to the CNF model.
     *
     * @param relation The relation to encode
     */
    void encode_relation(std::shared_ptr<Relation> relation);

private:
    /**
     * @brief Encodes a mandatory relation (parent <=> child)
     *
     * Generates clauses enforcing bidirectional implication:
     * - (~parent | child): parent implies child
     * - (~child | parent): child implies parent
     *
     * @param relation The mandatory relation (must have exactly one child)
     */
    void encode_mandatory(std::shared_ptr<Relation> relation);

    /**
     * @brief Encodes an optional relation (child => parent)
     *
     * Generates clause enforcing that child requires parent:
     * - (~child | parent): child can only be selected if parent is selected
     *
     * @param relation The optional relation (must have exactly one child)
     */
    void encode_optional(std::shared_ptr<Relation> relation);

    /**
     * @brief Encodes an OR relation (parent => at least one child)
     *
     * Generates clauses enforcing:
     * - (~parent | child1 | child2 | ... | childN): if parent, at least one child
     * - (~childi | parent) for each child: each child requires parent
     *
     * @param relation The OR relation (must have multiple children)
     */
    void encode_or(std::shared_ptr<Relation> relation);

    /**
     * @brief Encodes an alternative relation (parent => exactly one child)
     *
     * Generates clauses enforcing:
     * - (~parent | child1 | ... | childN): if parent, at least one child
     * - (~childi | ~childj) for all pairs: at most one child
     * - (~childi | parent) for each child: each child requires parent
     *
     * @param relation The alternative relation (must have multiple children)
     */
    void encode_alternative(std::shared_ptr<Relation> relation);

    /**
     * @brief Encodes a cardinality relation (parent => min..max children)
     *
     * Generates clauses enforcing minimum and maximum child selection bounds
     * using a combination enumeration approach. For efficiency, uses:
     * - At-least-min clauses: Forbids selecting fewer than min children
     * - At-most-max clauses: Forbids selecting more than max children
     *
     * @param relation The cardinality relation with custom min/max bounds
     */
    void encode_cardinality(std::shared_ptr<Relation> relation);

    /**
     * @brief Generates all combinations of k elements from n
     *
     * Helper method for cardinality encoding. Generates C(n,k) combinations
     * used to construct cardinality constraint clauses.
     *
     * @param n Total number of elements
     * @param k Number of elements to choose
     * @return Vector of all k-combinations (each combination is a vector of indices)
     *
     * Example: generate_combinations(3, 2) returns {{0,1}, {0,2}, {1,2}}
     */
    std::vector<std::vector<int>> generate_combinations(int n, int k);

    /**
     * @brief Adds a long OR clause using chain encoding for 3-CNF
     *
     * For clauses with more than 3 literals, this method introduces auxiliary
     * variables to break the clause into multiple 3-literal clauses using
     * the chain/ladder encoding technique.
     *
     * Given literals (a1 ∨ a2 ∨ ... ∨ an) with n > 3:
     * - Introduces n-3 auxiliary variables s1, s2, ..., s_{n-3}
     * - Generates clauses:
     *   - (a1 ∨ a2 ∨ s1)
     *   - (-s1 ∨ a3 ∨ s2)
     *   - (-s2 ∨ a4 ∨ s3)
     *   - ...
     *   - (-s_{n-3} ∨ a_{n-1} ∨ an)
     *
     * This encoding is equisatisfiable with the original clause:
     * - If all literals are false, the chain forces a contradiction
     * - If at least one literal is true, the formula is satisfiable
     *
     * @param literals Vector of literals forming the OR clause
     */
    void add_long_or_clause(const std::vector<int>& literals);
};

#endif // RELATIONENCODER_H
