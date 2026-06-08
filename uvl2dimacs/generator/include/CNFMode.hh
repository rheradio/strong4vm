/**
 * @file CNFMode.hh
 * @brief CNF conversion mode enumeration
 *
 * Defines the conversion modes for transforming feature models to CNF format.
 *
 * @author UVL2Dimacs Team
 * @date 2024
 */

#ifndef CNFMODE_H
#define CNFMODE_H

/**
 * @enum CNFMode
 * @brief Conversion modes for generating CNF from feature models
 *
 * Specifies the strategy for converting cross-tree constraint expressions to CNF.
 * Feature tree relation clauses (OR groups, ALTERNATIVE groups, CARDINALITY) are
 * always emitted directly as CNF disjunctions of arbitrary length in both modes.
 *
 * **STRAIGHTFORWARD Mode**:
 * - Converts cross-tree constraints via NNF + distribution law
 * - No auxiliary variables introduced
 * - Uses only the original feature variables
 * - May produce exponentially many clauses for deeply nested expressions
 *
 * **TSEITIN Mode**:
 * - Uses Tseitin transformation with auxiliary variables for cross-tree constraints
 * - Introduces one helper variable per Boolean sub-expression
 * - Produces linear-size output for constraint expressions (max 3 literals per clause)
 * - Prevents clause explosion for deeply nested Boolean expressions
 * - Feature tree relation clauses are still emitted directly (arbitrary length)
 *
 * @see ASTNode::get_clauses() for cross-tree constraint conversion
 * @see RelationEncoder for feature tree relation encoding
 * @see FMToCNF::transform() for feature model transformation
 *
 * Example trade-offs:
 * @code
 * // OR group with 4 children — emitted identically in both modes:
 * // Clause: (-parent | c1 | c2 | c3 | c4)
 *
 * // Cross-tree constraint (A => (B | C | D)):
 * // STRAIGHTFORWARD: 1 clause with 4 literals: (-A | B | C | D)
 * // TSEITIN: uses aux variables, all clauses have at most 3 literals
 * @endcode
 */
enum class CNFMode {
    TSEITIN,        ///< Tseitin transformation: auxiliary variables for cross-tree constraints
    STRAIGHTFORWARD ///< Direct conversion: no auxiliary variables, potentially longer clauses
};

#endif // CNFMODE_H
