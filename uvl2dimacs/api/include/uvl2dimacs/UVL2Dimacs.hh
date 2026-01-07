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
 * The API supports two CNF transformation strategies:
 *
 * **Straightforward (Default):**
 * - Direct transformation using NNF (Negation Normal Form) and distribution law
 * - Fewer variables (1 variable per feature)
 * - May produce longer clauses for complex constraints
 * - Recommended for most use cases
 *
 * **Tseitin:**
 * - Introduces auxiliary variables for subexpressions
 * - Produces 3-CNF (all clauses have ≤3 literals)
 * - More clauses but shorter, potentially faster for some solvers
 * - Recommended for formulas with deeply nested boolean expressions
 *
 * ## API Usage
 *
 * Basic usage pattern:
 * @code
 * uvl2dimacs::UVL2Dimacs converter;
 * converter.set_mode(ConversionMode::STRAIGHTFORWARD);
 * auto result = converter.convert("model.uvl", "output.dimacs");
 * if (result.success) {
 *     std::cout << "Features: " << result.num_features << std::endl;
 *     std::cout << "CNF Variables: " << result.num_variables << std::endl;
 *     std::cout << "CNF Clauses: " << result.num_clauses << std::endl;
 * }
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
 */
enum class ConversionMode {
    STRAIGHTFORWARD,  ///< Direct conversion without auxiliary variables (smaller CNF)
    TSEITIN           ///< Tseitin transformation with auxiliary variables (may be larger but more efficient for some solvers)
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
        , num_clauses(0) {}
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
