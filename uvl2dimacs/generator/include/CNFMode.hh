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
 * Specifies the strategy for converting constraint expressions and feature
 * tree relations to CNF:
 *
 * **STRAIGHTFORWARD Mode**:
 * - Direct conversion without introducing auxiliary variables
 * - Uses only the original feature variables
 * - Results in fewer total variables
 * - May produce longer clauses (no limit on literals per clause)
 * - Better when variable count is more important than clause length
 *
 * **TSEITIN Mode** (3-CNF):
 * - Uses Tseitin transformation with auxiliary variables
 * - Introduces helper variables to decompose complex expressions
 * - **Guarantees all clauses have at most 3 literals (3-CNF)**
 * - Uses tree-based decomposition for n-ary OR operations
 * - Prevents clause explosion for deeply nested expressions
 * - Better for SAT solvers optimized for 3-CNF
 *
 * The 3-CNF guarantee applies to:
 * - Boolean constraint expressions (AND, OR, NOT, IMPLIES, IFF)
 * - Feature tree relations (OR groups, ALTERNATIVE groups, CARDINALITY)
 *
 * @see ASTNode::get_clauses() for constraint conversion
 * @see RelationEncoder for feature tree relation encoding
 * @see FMToCNF::transform() for feature model transformation
 *
 * Example trade-offs:
 * @code
 * // OR group with 4 children
 *
 * // STRAIGHTFORWARD: 1 clause with 5 literals
 * // Clause: (-parent | c1 | c2 | c3 | c4)
 *
 * // TSEITIN: 10 clauses with max 3 literals, +3 auxiliary variables
 * // Uses tree decomposition: aux1=(c1|c2), aux2=(c3|c4), aux3=(aux1|aux2)
 * @endcode
 */
enum class CNFMode {
    TSEITIN,        ///< Tseitin transformation: guarantees 3-CNF (max 3 literals per clause)
    STRAIGHTFORWARD ///< Direct conversion: fewer variables, potentially longer clauses
};

#endif // CNFMODE_H
