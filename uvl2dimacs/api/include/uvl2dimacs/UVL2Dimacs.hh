/*
 * MIT License
 *
 * Copyright (c) 2026 Rubén Heradio and David Fernández Amorós
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @file UVL2Dimacs.hh
 * @brief High-level API for UVL to DIMACS conversion
 *
 * This header provides a simple, easy-to-use interface for converting
 * UVL (Universal Variability Language) feature models to DIMACS CNF format.
 */

/**
 * @defgroup UVL2Dimacs UVL to DIMACS Conversion
 * @brief API for converting UVL feature models to CNF format with UVL grammar support
 *
 * ## Overview
 *
 * The UVL2Dimacs component converts Universal Variability Language (UVL) feature models
 * into Boolean formulas in DIMACS CNF format. This enables the use of SAT solvers for
 * automated analysis of feature model properties.
 *
 * ## UVL Grammar Basics
 *
 * UVL (Universal Variability Language) is a textual language for feature models with:
 *
 * **Feature Declarations:**
 * - Features are organized in a tree hierarchy under a root feature
 * - Child features can be declared with different group types
 *
 * **Group Types:**
 * - **Mandatory**: Child must be selected if parent is selected
 * - **Optional**: Child may or may not be selected when parent is selected
 * - **Or**: At least one child must be selected when parent is selected
 * - **Alternative**: Exactly one child must be selected when parent is selected
 * - **Cardinality [m..n]**: Between m and n children must be selected
 *
 * **Cross-Tree Constraints:**
 * - Boolean expressions relating features from different tree branches
 * - Uses operators: `&` (AND), `|` (OR), `!` (NOT), `=>` (IMPLIES), `<=>` (IFF)
 * - Example: `A => B` (selecting feature A requires selecting feature B)
 *
 * **Example UVL Model:**
 * ```
 * features
 *     Car
 *         mandatory
 *             Engine
 *         optional
 *             GPS
 *         alternative
 *             Gasoline
 *             Electric
 *
 * constraints
 *     Electric => GPS
 * ```
 *
 * ## Conversion Modes
 *
 * The API supports two CNF transformation strategies, each optimized for different use cases:
 *
 * **Straightforward (Default - ConversionMode::STRAIGHTFORWARD):**
 * - Direct transformation using NNF (Negation Normal Form) and distribution law
 * - Fewer variables (1 variable per feature, no auxiliary variables)
 * - May produce longer clauses for complex constraints (no clause size limit)
 * - More compact representation (fewer total clauses)
 * - **Best for:** Most models, simple to medium complexity, when variable count is important
 * - **When to use:** General purpose feature models, simple Boolean constraints
 *
 * **Tseitin (ConversionMode::TSEITIN):**
 * - Introduces auxiliary variables for Boolean sub-expressions in cross-tree constraints
 * - Feature tree relation clauses (OR groups, ALTERNATIVE groups, CARDINALITY) are emitted
 *   directly as CNF disjunctions of arbitrary length — no decomposition
 * - Prevents exponential clause explosion in deeply nested cross-tree constraint expressions
 * - **Best for:** Models with complex, deeply nested Boolean cross-tree constraints
 * - **When to use:** Cross-tree constraints with many nested AND/OR/IMPLIES/IFF operations
 *
 * **Comparison Table:**
 *
 * | Aspect                         | Straightforward              | Tseitin                            |
 * |--------------------------------|------------------------------|------------------------------------|
 * | Variables                      | n features                   | n features + m auxiliary           |
 * | Clauses                        | Fewer total                  | More total                         |
 * | Feature tree relation clauses  | Direct (arbitrary length)    | Direct (arbitrary length)          |
 * | Cross-tree constraint clauses  | May grow exponentially       | Linear size                        |
 * | Technique                      | NNF + distribution           | Tseitin auxiliary variables        |
 * | Best for                       | General use                  | Deeply nested cross-tree constraints|
 *
 * See docs/translation.md for detailed transformation rules and examples.
 *
 * ## Backbone Simplification
 *
 * Optional feature to reduce formula size while preserving solution count:
 * - **What it does**: Removes satisfied clauses and shortens others using backbone literals
 * - **Backbone**: Set of literals that must be true/false in all satisfying assignments
 * - **Performance**: Typical reduction of 30-50% in formula size
 * - **Guarantees**: Preserves the exact number of satisfying assignments (verified by test suite)
 * - **Works with**: Both STRAIGHTFORWARD and TSEITIN modes
 * - **Requires**: backbone_solver executable (included in project)
 *
 * Enable with `set_backbone_simplification(true)` before calling `convert()`.
 *
 * ## API Usage
 *
 * **Basic conversion (Straightforward mode):**
 * @code
 * uvl2dimacs::UVL2Dimacs converter;
 * converter.set_mode(ConversionMode::STRAIGHTFORWARD);  // Default mode
 * auto result = converter.convert("model.uvl", "output.dimacs");
 * if (result.success) {
 *     std::cout << "Features: " << result.num_features << std::endl;
 *     std::cout << "CNF Variables: " << result.num_variables << std::endl;
 *     std::cout << "CNF Clauses: " << result.num_clauses << std::endl;
 * }
 * @endcode
 *
 * **Tseitin mode:**
 * @code
 * uvl2dimacs::UVL2Dimacs converter;
 * converter.set_mode(ConversionMode::TSEITIN);  // Use Tseitin transformation
 * auto result = converter.convert("model.uvl", "output.dimacs");
 * // Auxiliary variables introduced for cross-tree constraint sub-expressions
 * int aux_variables = result.num_variables - result.num_features;
 * @endcode
 *
 * **With backbone simplification:**
 * @code
 * uvl2dimacs::UVL2Dimacs converter;
 * converter.set_mode(ConversionMode::STRAIGHTFORWARD);
 * converter.set_backbone_simplification(true);  // Reduces formula size by 30-50%
 * auto result = converter.convert("model.uvl", "output.dimacs");
 * @endcode
 *
 * **Combining Tseitin and Backbone:**
 * @code
 * uvl2dimacs::UVL2Dimacs converter;
 * converter.set_mode(ConversionMode::TSEITIN);
 * converter.set_backbone_simplification(true);
 * auto result = converter.convert("model.uvl", "output.dimacs");
 * // Result: Simplified 3-CNF with backbone reduction
 * @endcode
 *
 * ## Output Format
 *
 * The generated DIMACS file contains:
 * - Header: `p cnf [variables] [clauses]`
 * - Variable mappings: `c [var_num] [feature_name]`
 * - Clauses: Space-separated literals ending with 0
 *
 * ## Limitations
 *
 * - Feature cardinality `[1..*]` not fully supported (requires indexed feature generation)
 * - Arithmetic constraints are filtered out (requires SMT solver, not pure SAT)
 * - No clause minimization or subsumption
 *
 * @see UVL2Dimacs Main API class
 * @see ConversionMode Enum for conversion strategies
 * @see ConversionResult Structure containing conversion statistics
 */

#ifndef UVL2DIMACS_API_H
#define UVL2DIMACS_API_H

#include <string>
#include <memory>

namespace uvl2dimacs {

/**
 * @enum ConversionMode
 * @ingroup UVL2Dimacs
 * @brief Conversion mode for CNF generation
 *
 * Choose the appropriate mode based on your requirements:
 * - STRAIGHTFORWARD: Fewer variables, may have longer clauses for complex constraints
 * - TSEITIN: Auxiliary variables for cross-tree constraints prevent exponential clause growth
 */
enum class ConversionMode {
    STRAIGHTFORWARD,  ///< Direct NNF conversion without auxiliary variables (compact, fewer variables, variable clause length)
    TSEITIN           ///< Tseitin transformation with auxiliary variables for cross-tree constraint sub-expressions
};

/**
 * @struct ConversionResult
 * @ingroup UVL2Dimacs
 * @brief Result of a conversion operation
 */
struct ConversionResult {
    bool success;                   ///< Whether the conversion was successful
    std::string error_message;      ///< Error message if conversion failed

    // Statistics from the input feature model
    int num_features;               ///< Number of features in the input model
    int num_relations;              ///< Number of parent-child relations
    int num_constraints;            ///< Number of cross-tree constraints

    // Statistics from the output CNF
    int num_variables;              ///< Number of variables in the CNF
    int num_clauses;                ///< Number of clauses in the CNF

    // Skipped constraints
    int num_skipped_constraints;    ///< Constraints skipped due to arithmetic / non-Boolean operators

    /**
     * @brief Default constructor for failed conversion
     */
    ConversionResult()
        : success(false)
        , error_message("")
        , num_features(0)
        , num_relations(0)
        , num_constraints(0)
        , num_variables(0)
        , num_clauses(0)
        , num_skipped_constraints(0) {}
};

/**
 * @class UVL2Dimacs
 * @ingroup UVL2Dimacs
 * @brief Main class for UVL to DIMACS conversion
 *
 * This class provides a high-level interface for converting UVL files
 * to DIMACS CNF format. It handles all the complexity of parsing,
 * transformation, and writing.
 *
 * Example usage:
 * @code
 * uvl2dimacs::UVL2Dimacs converter;
 * converter.set_verbose(true);
 * auto result = converter.convert("input.uvl", "output.dimacs");
 * if (result.success) {
 *     std::cout << "Converted " << result.num_features << " features to "
 *               << result.num_clauses << " clauses" << std::endl;
 * } else {
 *     std::cerr << "Error: " << result.error_message << std::endl;
 * }
 * @endcode
 */
class UVL2Dimacs {
private:
    bool verbose_;
    ConversionMode mode_;
    bool use_backbone_;

public:
    /**
     * @brief Constructor
     * @param verbose Whether to print progress messages (default: false)
     */
    explicit UVL2Dimacs(bool verbose = false);

    /**
     * @brief Destructor
     */
    ~UVL2Dimacs();

    /**
     * @brief Set verbose output mode
     * @param verbose If true, print progress messages during conversion
     */
    void set_verbose(bool verbose);

    /**
     * @brief Set conversion mode
     * @param mode The CNF conversion mode to use
     */
    void set_mode(ConversionMode mode);

    /**
     * @brief Get current conversion mode
     * @return The current conversion mode
     */
    ConversionMode get_mode() const;

    /**
     * @brief Enable or disable backbone simplification
     * @param use_backbone True to apply backbone simplification, false to disable
     *
     * When enabled, the output DIMACS file will be simplified using backbone analysis.
     * Backbone simplification:
     * - Removes clauses that are always satisfied
     * - Shortens clauses by removing backbone literals
     * - Preserves the number of satisfying assignments (solution count)
     * - Typically reduces formula size by 30-50%
     *
     * Note: Requires backbone_solver executable in PATH or in project directory
     */
    void set_backbone_simplification(bool use_backbone);

    /**
     * @brief Check if backbone simplification is enabled
     * @return True if backbone simplification is enabled
     */
    bool get_backbone_simplification() const;

    /**
     * @brief Convert a UVL file to DIMACS format
     * @param input_file Path to input UVL file
     * @param output_file Path to output DIMACS file
     * @return ConversionResult with success status and statistics
     */
    ConversionResult convert(const std::string& input_file,
                            const std::string& output_file);

    /**
     * @brief Convert a UVL file to DIMACS format with specified mode
     * @param input_file Path to input UVL file
     * @param output_file Path to output DIMACS file
     * @param mode Conversion mode to use for this conversion
     * @return ConversionResult with success status and statistics
     */
    ConversionResult convert(const std::string& input_file,
                            const std::string& output_file,
                            ConversionMode mode);

    /**
     * @brief Convert a UVL file to DIMACS string
     * @param input_file Path to input UVL file
     * @param result Output parameter for conversion result
     * @return DIMACS string if successful, empty string if failed
     */
    std::string convert_to_string(const std::string& input_file,
                                  ConversionResult& result);

    /**
     * @brief Convert a UVL file to DIMACS string with specified mode
     * @param input_file Path to input UVL file
     * @param mode Conversion mode to use for this conversion
     * @param result Output parameter for conversion result
     * @return DIMACS string if successful, empty string if failed
     */
    std::string convert_to_string(const std::string& input_file,
                                  ConversionMode mode,
                                  ConversionResult& result);
};

} // namespace uvl2dimacs

#endif // UVL2DIMACS_API_H
