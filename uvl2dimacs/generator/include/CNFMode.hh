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
 * **TSEITIN Mode**:
 * - Uses Tseitin transformation with auxiliary variables for cross-tree
 *   constraint expressions, preventing exponential clause growth from
 *   nested boolean operations
 * - Feature tree relations (OR, ALTERNATIVE, CARDINALITY) are encoded
 *   directly since they are already valid CNF disjunctions
 * - Introduces fewer auxiliary variables than a strict 3-CNF encoding
 *
 * @see ASTNode::get_clauses() for constraint conversion (uses auxiliary variables)
 * @see RelationEncoder for feature tree relation encoding (direct CNF)
 * @see FMToCNF::transform() for feature model transformation
 */
enum class CNFMode {
    TSEITIN,        ///< Tseitin transformation for cross-tree constraints (auxiliary variables)
    STRAIGHTFORWARD ///< Direct conversion: fewer variables, potentially longer clauses
};

#endif // CNFMODE_H
