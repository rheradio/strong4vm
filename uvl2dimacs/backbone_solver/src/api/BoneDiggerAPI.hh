/**
 * @file BoneDiggerAPI.hh
 * @author BoneDigger Team
 * @brief Public C++ API for computing backbones of Boolean formulas
 *
 * This file provides the main API interface for the BoneDigger library,
 * which computes the backbone of Boolean formulas in CNF format.
 * The backbone is the set of literals that must have the same value
 * in all satisfying assignments.
 */

#ifndef BONEDIGGERAPI_HH
#define BONEDIGGERAPI_HH

#include <string>
#include <vector>

using std::string;
using std::vector;

/**
 * @defgroup API BoneDigger API
 * @brief Public API for programmatic backbone computation
 *
 * ## Overview
 *
 * The BoneDigger API provides a clean, high-level interface for computing backbones
 * of Boolean formulas programmatically. It uses the PIMPL (Pointer to Implementation)
 * pattern to hide internal details and provide a stable ABI.
 *
 * ## Quick Start
 *
 * @code
 * #include "BoneDiggerAPI.hh"
 * using namespace bonedigger;
 *
 * int main() {
 *     BoneDiggerAPI api;
 *
 *     // Load a DIMACS CNF file
 *     if (!api.read_dimacs("formula.cnf")) {
 *         return 1;
 *     }
 *
 *     // Create a detector (options: "one", "flatland", "rush")
 *     if (!api.create_backbone_detector("one")) {
 *         return 1;
 *     }
 *
 *     // Compute the backbone
 *     vector<int> backbone = api.compute_backbone();
 *
 *     // Display results
 *     api.print_backbone();
 *
 *     return 0;
 * }
 * @endcode
 *
 * ## Workflow
 *
 * The typical workflow is:
 * 1. **Create API instance**: `BoneDiggerAPI api;`
 * 2. **Read formula**: `api.read_dimacs("file.cnf")`
 * 3. **Create detector**: `api.create_backbone_detector("detector_type")`
 * 4. **Compute backbone**: `vector<int> backbone = api.compute_backbone()`
 * 5. **Query results**: Use `get_max_variable()`, `is_satisfiable()`, etc.
 *
 * ## Detector Types
 *
 * Available detector types (pass as string to create_backbone_detector()):
 *
 * - **"rush"** - RushAndPray detector (default, good general purpose)
 * - **"one"** - CheckCandidatesOneByOne (systematic with activity bumping)
 * - **"flatland"** - FastOnCliffsSlowOnPlains (adaptive cliff/plain strategy)
 *
 * ## Building with the API
 *
 * To use the API in your project, compile and link with:
 *
 * @code{.sh}
 * g++ -std=c++17 -O3 -I/path/to/bonedigger/src -I/path/to/bonedigger/src/minisat \
 *     your_program.cc \
 *     /path/to/bonedigger/src/api/BoneDiggerAPI.cc \
 *     /path/to/bonedigger/src/io/DIMACSReader.cc \
 *     /path/to/bonedigger/src/detectors/CheckCandidatesOneByOne.cc \
 *     /path/to/bonedigger/src/detectors/FastOnCliffsSlowOnPlains.cc \
 *     /path/to/bonedigger/src/detectors/RushAndPray.cc \
 *     /path/to/bonedigger/src/data_structures/LiteralSet.cc \
 *     /path/to/bonedigger/src/minisat_interface/minisat_aux.cc \
 *     -L/path/to/bonedigger/src/minisat/build/release/lib -lminisat -lz \
 *     -o your_program
 * @endcode
 *
 * ## Thread Safety
 *
 * The BoneDiggerAPI class is NOT thread-safe. Each thread should use its own
 * BoneDiggerAPI instance.
 *
 * ## Example Program
 *
 * See `src/api/api_example.cc` for a complete working example.
 */

namespace bonedigger {

/**
 * @class BoneDiggerAPI
 * @ingroup API
 * @brief Main API class for backbone computation using PIMPL pattern
 *
 * This class provides a high-level interface for computing backbones
 * of Boolean formulas. It uses the PIMPL (Pointer to Implementation)
 * pattern to hide implementation details and provide a stable ABI.
 *
 * Typical workflow:
 * 1. Create a BoneDiggerAPI instance
 * 2. Call read_dimacs() to load a CNF formula
 * 3. Call create_backbone_detector() to choose an algorithm
 * 4. Call compute_backbone() to get the result
 *
 * Example:
 * @code
 * BoneDiggerAPI api;
 * api.read_dimacs("formula.cnf");
 * api.create_backbone_detector("one");
 * vector<int> backbone = api.compute_backbone();
 * @endcode
 */
class BoneDiggerAPI {
public:
    /**
     * @brief Construct a new BoneDiggerAPI object
     */
    BoneDiggerAPI();

    /**
     * @brief Destroy the BoneDiggerAPI object
     */
    ~BoneDiggerAPI();

    /**
     * @brief Read a DIMACS CNF file
     *
     * Parses a CNF formula from a file in DIMACS format.
     * The file must follow the standard DIMACS CNF specification.
     *
     * @param file_name Path to the DIMACS CNF file
     * @return true if file is valid and successfully read
     * @return false if file cannot be opened or has invalid format
     */
    bool read_dimacs(const string& file_name);

    /**
     * @brief Create a backbone detector for the last read DIMACS file
     *
     * Creates and initializes a backbone detection algorithm.
     * Must be called after read_dimacs().
     *
     * @param bb_detector Algorithm to use:
     *                    - "one" or "simple": CheckCandidatesOneByOne (fast upper-bound)
     *                    - "flatland": FastOnCliffsSlowOnPlains (adaptive strategy)
     *                    - "rush": RushAndPray (experimental)
     * @return true if detector was successfully created
     * @return false if no DIMACS file has been read or bb_detector is invalid
     */
    bool create_backbone_detector(const string& bb_detector = "simple");

    /**
     * @brief Compute the backbone using the created detector
     *
     * Runs the backbone detection algorithm and returns the result.
     * Must be called after create_backbone_detector().
     *
     * @return vector<int> Vector of backbone literals:
     *                     - Positive values indicate variables that must be true
     *                     - Negative values indicate variables that must be false
     *                     - Empty vector if no file read, no detector created,
     *                       or formula is unsatisfiable
     */
    vector<int> compute_backbone();

    /**
     * @brief Get the maximum variable ID from the last read file
     *
     * @return int The highest variable number in the CNF formula
     */
    int get_max_variable() const;

    /**
     * @brief Check if the last read formula is satisfiable
     *
     * This can only be called after compute_backbone() has been executed.
     *
     * @return true if the formula has at least one satisfying assignment
     * @return false if the formula is unsatisfiable
     */
    bool is_satisfiable() const;

    /**
     * @brief Set the detector attention weight
     *
     * Controls how much influence the detector's activity bumping has
     * on variable ordering relative to the VSIDS activity.
     * Combined activity = vsids_activity + weight * detector_activity
     *
     * Must be called before compute_backbone().
     *
     * @param weight The attention weight (must be >= 0, default is 1.0)
     */
    void set_attention_weight(double weight);

    /**
     * @brief Compute the backbone under given assumptions
     *
     * Adds the assumptions as unit clauses to the formula and computes the backbone
     * of the resulting formula. A fresh detector instance is created for each call.
     * Must be called after read_dimacs() and create_backbone_detector().
     *
     * @param assumptions Literals to assume: positive int forces var=true,
     *                    negative int forces var=false (1-indexed DIMACS convention)
     * @return vector<int> Backbone literals under the assumptions, or empty if UNSAT
     */
    vector<int> compute_backbone_with_assumptions(const vector<int>& assumptions);

    /**
     * @brief Print the backbone to standard output
     *
     * Outputs the computed backbone in human-readable format.
     * Prints "UNSATISFIABLE" if formula is not satisfiable.
     * Otherwise prints backbone size and the list of backbone literals.
     */
    void print_backbone() const;

private:
    /**
     * @brief Forward declaration of implementation class
     */
    class Impl;

    /**
     * @brief Pointer to implementation (PIMPL pattern)
     */
    Impl* pimpl;
};

} // namespace bonedigger

#endif // BONEDIGGERAPI_HH
