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
 * Specifies the strategy for converting constraint expressions to CNF:
 *
 * **STRAIGHTFORWARD Mode**:
 * - Direct conversion without introducing auxiliary variables
 * - Uses only the original feature variables
 * - Results in fewer total variables
 * - May produce longer clauses (potential clause explosion for complex constraints)
 * - Better when formula size (number of clauses) is more important than clause length
 *
 * **TSEITIN Mode**:
 * - Uses Tseitin transformation with auxiliary variables
 * - Introduces helper variables to decompose complex expressions
 * - Results in more variables but shorter, more uniform clauses (typically 3-CNF)
 * - Prevents clause explosion for deeply nested expressions
 * - Better for SAT solvers that perform well on 3-CNF
 *
 * @see ASTNode::get_clauses() for conversion implementation
 * @see FMToCNF::transform() for feature model transformation
 *
 * Example trade-offs:
 * @code
 * // Expression: (A & B) | (C & D)
 *
 * // STRAIGHTFORWARD: 4 clauses, 2-4 literals each
 * // Clauses: (A|C), (A|D), (B|C), (B|D)
 *
 * // TSEITIN: 7 clauses, 2-3 literals each, +2 auxiliary variables
 * // Introduces: T1 = (A & B), T2 = (C & D), result = (T1 | T2)
 * @endcode
 */
enum class CNFMode {
    TSEITIN,        ///< Use Tseitin transformation with auxiliary variables (more vars, shorter clauses)
    STRAIGHTFORWARD ///< Direct conversion without auxiliary variables (fewer vars, potentially longer clauses)
};

#endif // CNFMODE_H
