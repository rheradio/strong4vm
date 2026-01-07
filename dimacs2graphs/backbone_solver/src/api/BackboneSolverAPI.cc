/**
 * @file BackboneSolverAPI.cc
 * @brief Implementation file for BackboneSolverAPI - SAT formula backbone detection
 *
 * This file implements the BackboneSolverAPI class, providing a high-level interface
 * for detecting backbone literals in propositional logic formulas encoded in DIMACS CNF format.
 *
 * @section backbone_definition Backbone Definition
 *
 * A **backbone** is the set of literals that must be assigned the same truth value in all
 * satisfying assignments of a SAT formula:
 * - **Positive backbone** (core features): Variables that are TRUE in all models (e.g., variable v)
 * - **Negative backbone** (dead features): Variables that are FALSE in all models (e.g., negation -v)
 *
 * For feature models, backbone literals correspond to:
 * - Core features (positive backbone): Features present in all valid configurations
 * - Dead features (negative backbone): Features excluded from all valid configurations
 *
 * @section detection_algorithms Detection Algorithms
 *
 * The API supports two backbone detection algorithms with different performance characteristics:
 *
 * 1. **CheckCandidatesOneByOne** (detector type: "one" or "simple")
 *    - Uses SAT solver activity bumping to guide search
 *    - Bumps variables during backbone testing to prioritize related variables
 *    - Generally faster due to improved solver heuristics
 *    - Default and recommended algorithm
 *
 * 2. **CheckCandidatesOneByOneWithoutAttention** (detector type: "without" or "withoutattention")
 *    - No activity bumping during backbone detection
 *    - Baseline algorithm for comparison
 *    - May be slower but uses standard SAT solver behavior
 *
 * Both algorithms:
 * - Test each variable by attempting to find a solution where it's forced to the opposite value
 * - If no such solution exists, the variable is part of the backbone
 * - Use MiniSat 2.2.0 as the underlying SAT solver via IPASIR interface
 *
 * @section pimpl_architecture Implementation Architecture
 *
 * This implementation uses the **PIMPL (Pointer to Implementation)** idiom:
 * - Public API (BackboneSolverAPI) exposes only the interface
 * - Private implementation class (Impl) contains all data and algorithm logic
 * - Benefits:
 *   - ABI stability: Internal changes don't break binary compatibility
 *   - Compilation firewall: Reduces header dependencies and compilation times
 *   - Encapsulation: Hides MiniSat and implementation details from API users
 *
 * @section typical_workflow Typical Workflow
 *
 * Standard usage pattern:
 * @code
 * // Create API instance
 * BackboneSolverAPI solver;
 *
 * // Load DIMACS CNF file (supports .cnf and .cnf.gz)
 * if (!solver.read_dimacs("formula.cnf")) {
 *     cerr << "Failed to read DIMACS file" << endl;
 *     return 1;
 * }
 *
 * // Select detection algorithm
 * if (!solver.create_backbone_detector("one")) {
 *     cerr << "Failed to create detector" << endl;
 *     return 1;
 * }
 *
 * // Compute backbone
 * vector<int> backbone = solver.compute_backbone();
 *
 * // Check satisfiability
 * if (!solver.is_satisfiable()) {
 *     cerr << "Formula is UNSATISFIABLE" << endl;
 *     return 1;
 * }
 *
 * // Process backbone literals
 * for (int lit : backbone) {
 *     if (lit > 0) {
 *         cout << "Variable " << lit << " is in positive backbone (core)" << endl;
 *     } else {
 *         cout << "Variable " << -lit << " is in negative backbone (dead)" << endl;
 *     }
 * }
 *
 * // Alternative: print formatted summary
 * solver.print_backbone();
 * @endcode
 *
 * Usage with assumptions:
 * @code
 * // Compute backbone under assumptions (e.g., feature A selected, feature B excluded)
 * vector<int> assumptions = {5, -12};  // Assume variable 5=true, 12=false
 * vector<int> conditional_backbone = solver.compute_backbone_with_assumptions(assumptions);
 * @endcode
 *
 * @section thread_safety Thread Safety
 *
 * @warning **NOT THREAD-SAFE**: BackboneSolverAPI instances are not thread-safe.
 *
 * For parallel processing:
 * - Create separate BackboneSolverAPI instances for each thread
 * - Use pre-initialization pattern: instantiate all solver instances sequentially in main thread
 * - Only variable processing loops should run in parallel
 * - Each thread must have its own independent solver instance
 * - NEVER initialize BackboneSolverAPI inside parallel thread workers
 *
 * Example for parallel processing:
 * @code
 * // CORRECT: Pre-initialize in main thread
 * vector<BackboneSolverAPI*> solvers(num_threads);
 * for (int i = 0; i < num_threads; ++i) {
 *     solvers[i] = new BackboneSolverAPI();
 *     solvers[i]->read_dimacs("formula.cnf");
 *     solvers[i]->create_backbone_detector("one");
 * }
 *
 * // Now safe to use solvers in parallel
 * #pragma omp parallel for
 * for (int i = 0; i < num_threads; ++i) {
 *     auto backbone = solvers[i]->compute_backbone();
 *     // Process backbone...
 * }
 *
 * // WRONG: Initialization inside parallel region
 * #pragma omp parallel for
 * for (int i = 0; i < num_threads; ++i) {
 *     BackboneSolverAPI solver;  // DANGEROUS!
 *     solver.read_dimacs("formula.cnf");
 *     // ...
 * }
 * @endcode
 *
 * @see BackboneSolverAPI.hh for public API documentation
 * @see DIMACSReader for DIMACS CNF file parsing
 * @see CheckCandidatesOneByOne for default detection algorithm
 * @see CheckCandidatesOneByOneWithoutAttention for baseline algorithm
 */

// File:   BackboneSolverAPI.cc
// Implementation of BackboneSolver API

#include "BackboneSolverAPI.hh"
#include "DIMACSReader.hh"
#include "CheckCandidatesOneByOne.hh"
#include "CheckCandidatesOneByOneWithoutAttention.hh"
#include "minisat_interface/MiniSatExt.hh"
#include <iostream>
#include <iomanip>
#include <memory>
#include <set>

using std::cout;
using std::cerr;
using std::endl;

namespace backbone_solver {

using Minisat::Lit;
using Minisat::Var;
using Minisat::mkLit;
using Minisat::vec;

/**
 * @class BackboneSolverAPI::Impl
 * @brief Private implementation class for BackboneSolverAPI (PIMPL idiom)
 *
 * This class encapsulates all implementation details of the backbone solver API,
 * providing ABI stability and compilation firewall benefits. It manages:
 *
 * **Components:**
 * - **DIMACSReader**: Parses DIMACS CNF files (including gzip-compressed .cnf.gz)
 * - **Detector instance**: Polymorphic pointer to selected algorithm (one of two types)
 * - **Formula metadata**: Variable count (max_id), clause vector, file path
 * - **State flags**: has_file (file loaded), is_sat (satisfiability result)
 * - **Cached results**: Detector instance preserves backbone computation for reuse
 *
 * **Lifecycle Management:**
 * - Constructor initializes all members to safe default states
 * - Destructor ensures proper cleanup of detector and reader resources
 * - Cleanup methods handle selective resource deallocation
 *
 * **Design Rationale:**
 * - Separates interface (BackboneSolverAPI) from implementation
 * - Hides MiniSat types and detector implementations from public headers
 * - Enables binary compatibility when internal algorithms change
 * - Reduces compilation dependencies for API users
 *
 * @note This class is never exposed in public headers, ensuring complete encapsulation
 */
class BackboneSolverAPI::Impl {
public:
    /**
     * @brief Default constructor - initializes all members to safe defaults
     */
    Impl() : max_id(0), detector(nullptr), has_file(false), is_sat(false) {}

    /**
     * @brief Destructor - ensures proper cleanup of all resources
     */
    ~Impl() {
        cleanup_detector();
        cleanup_reader();
    }

    /**
     * @brief Reads and parses a DIMACS CNF file
     *
     * Loads a SAT formula from a DIMACS CNF file, supporting both plain text (.cnf)
     * and gzip-compressed (.cnf.gz) formats. This method:
     * 1. Cleans up any existing detector and reader instances
     * 2. Opens the file using zlib (transparent gzip decompression)
     * 3. Parses CNF using DIMACSReader
     * 4. Stores formula metadata (variables, clauses)
     * 5. Resets satisfiability state
     *
     * **File Format:**
     * Standard DIMACS CNF format:
     * @code
     * p cnf <num_variables> <num_clauses>
     * <clause1>
     * <clause2>
     * ...
     * @endcode
     *
     * Example:
     * @code
     * p cnf 4 3
     * 1 2 0
     * -1 3 0
     * -2 -3 4 0
     * @endcode
     *
     * **Gzip Support:**
     * Automatically detects and decompresses .gz files via zlib's gzopen.
     * No special handling needed - just pass the .cnf.gz filename.
     *
     * @param file_name Path to DIMACS CNF file (.cnf or .cnf.gz)
     * @return true if file was successfully read and parsed, false otherwise
     *
     * @note Clears any previously loaded formula and computed backbone
     * @note Exception-safe: catches all exceptions and returns false
     * @note Sets has_file=true and is_sat=false on success
     *
     * @see DIMACSReader for parsing details
     */
    bool read_file(const string& file_name) {
        cleanup_detector();
        cleanup_reader();

        try {
            // Open the file
            gzFile ff = gzopen(file_name.c_str(), "rb");
            if (ff == Z_NULL) {
                return false;
            }

            // Create reader and read the file
            reader = new Reader(ff);
            DIMACSReader dimacs_reader(*reader);
            dimacs_reader.read();

            // Store the data
            max_id = dimacs_reader.get_max_id();
            clauses = dimacs_reader.get_clause_vector();
            has_file = true;
            is_sat = false;

            return true;
        } catch (...) {
            cleanup_reader();
            return false;
        }
    }

    /**
     * @brief Creates a backbone detector instance of the specified type
     *
     * Selects and configures the backbone detection algorithm to use for subsequent
     * compute() calls. This method only stores the detector type - the actual detector
     * instance is lazily created during the first compute() call.
     *
     * **Supported Detector Types:**
     *
     * 1. **"one"** or **"simple"**: CheckCandidatesOneByOne
     *    - Uses SAT solver activity bumping for guided search
     *    - Bumps variables during testing to improve solver heuristics
     *    - Generally faster and recommended for most use cases
     *    - Default algorithm used by dimacs2graphs pipeline
     *
     * 2. **"without"** or **"withoutattention"**: CheckCandidatesOneByOneWithoutAttention
     *    - No activity bumping during backbone detection
     *    - Standard SAT solver behavior without search guidance
     *    - Useful as baseline for performance comparisons
     *
     * **Algorithm Selection Guidelines:**
     * - For production use: "one" (default, best performance)
     * - For benchmarking: "without" (baseline comparison)
     * - Both algorithms produce identical backbone results
     * - Only difference is search strategy and performance
     *
     * @param type Detector type string ("one", "simple", "without", or "withoutattention")
     * @return true if detector type is valid and file was previously loaded, false otherwise
     *
     * @note Requires prior call to read_file() - returns false if no file loaded
     * @note Cleans up any existing detector instance before setting new type
     * @note Type string matching is case-sensitive
     * @note Actual detector instantiation is deferred until compute() call (lazy initialization)
     *
     * @see CheckCandidatesOneByOne for "one"/"simple" implementation
     * @see CheckCandidatesOneByOneWithoutAttention for "without"/"withoutattention" implementation
     */
    bool create_detector(const string& type) {
        if (!has_file) {
            return false;
        }

        cleanup_detector();

        if (type == "one" || type == "simple") {
            detector_type = ONE;
        } else if (type == "without" || type == "withoutattention") {
            detector_type = WITHOUT_ATTENTION;
        } else {
            return false;
        }

        return true;
    }

    /**
     * @brief Computes the backbone of the loaded formula
     *
     * Performs backbone detection on the SAT formula loaded via read_file() using
     * the detector type selected via create_detector(). This is the core computation
     * method of the API.
     *
     * **Computation Strategy:**
     *
     * 1. **Lazy Initialization**: If detector instance doesn't exist, creates it on first call
     * 2. **Satisfiability Check**: First verifies formula is SAT (returns empty if UNSAT)
     * 3. **Backbone Detection**: Tests each variable v ∈ {1..max_id}:
     *    - For positive backbone: Can all models satisfy ¬v? If no, v is in backbone
     *    - For negative backbone: Can all models satisfy v? If no, ¬v is in backbone
     * 4. **Result Caching**: Detector instance preserved for subsequent calls (idempotent)
     *
     * **Algorithm Details:**
     *
     * For each variable v:
     * - Solve formula ∧ {¬v} to test if v can be false
     * - Solve formula ∧ {v} to test if v can be true
     * - If v cannot be false: v is positive backbone (always true)
     * - If v cannot be true: ¬v is negative backbone (always false)
     *
     * **Return Format:**
     * Vector of signed integers representing backbone literals:
     * - Positive integers (e.g., 5): Variable is in positive backbone (always true)
     * - Negative integers (e.g., -12): Variable is in negative backbone (always false)
     * - Empty vector: Either UNSAT formula, no backbone, or error occurred
     *
     * **Performance Considerations:**
     * - First call: O(n × SAT) where n = number of variables
     * - Subsequent calls: O(n) - just extracts cached results
     * - Activity bumping ("one" detector) significantly improves SAT solve times
     * - For large formulas (>1000 vars), consider parallel approach in calling code
     *
     * **Example Output:**
     * @code
     * vector<int> bb = compute();
     * // bb = {1, -3, 5, -7}
     * // Interpretation:
     * //   Variable 1: always true (positive backbone)
     * //   Variable 3: always false (negative backbone)
     * //   Variable 5: always true (positive backbone)
     * //   Variable 7: always false (negative backbone)
     * //   Variables 2,4,6,8,...: not in backbone (can be true or false)
     * @endcode
     *
     * @return Vector of backbone literals (positive/negative integers), empty if UNSAT or error
     *
     * @note Returns empty vector if no file loaded or no detector type selected
     * @note First call creates detector instance (expensive), subsequent calls reuse it
     * @note Sets is_sat flag based on satisfiability check result
     * @note Exception-safe: catches all exceptions and returns empty vector
     * @note Detector instance remains allocated for reuse - call cleanup_detector() to free
     *
     * @see compute_with_assumps() for conditional backbone under assumptions
     * @see is_satisfiable() to check SAT/UNSAT status after computation
     * @see print_backbone() for formatted output of results
     */
    vector<int> compute() {
        vector<int> result;

        if (!has_file || detector_type == NONE) {
            return result;
        }

        // Only cleanup if detector hasn't been created yet
        if (detector == nullptr) {
            try {
                if (detector_type == ONE) {
                    CheckCandidatesOneByOne* one_detector = new CheckCandidatesOneByOne(max_id, clauses);
                    detector = one_detector;

                    if (!one_detector->initialize()) {
                        is_sat = false;
                        cleanup_detector();
                        return result;
                    }

                    is_sat = true;
                    one_detector->run();

                    // Extract backbone
                    for (Var v = 1; v <= max_id; ++v) {
                        if (one_detector->is_backbone(v)) {
                            int lit = one_detector->backbone_sign(v) ? v : -v;
                            result.push_back(lit);
                        }
                    }

                } else if (detector_type == WITHOUT_ATTENTION) {
                    CheckCandidatesOneByOneWithoutAttention* without_detector =
                        new CheckCandidatesOneByOneWithoutAttention(max_id, clauses);
                    detector = without_detector;

                    if (!without_detector->initialize()) {
                        is_sat = false;
                        cleanup_detector();
                        return result;
                    }

                    is_sat = true;
                    without_detector->run();

                    // Extract backbone
                    for (Var v = 1; v <= max_id; ++v) {
                        if (without_detector->is_backbone(v)) {
                            int lit = without_detector->backbone_sign(v) ? v : -v;
                            result.push_back(lit);
                        }
                    }
                }
            } catch (...) {
                cleanup_detector();
                return vector<int>();
            }
        } else {
            // Detector already exists, just extract backbone
            try {
                if (detector_type == ONE) {
                    CheckCandidatesOneByOne* one_detector = static_cast<CheckCandidatesOneByOne*>(detector);
                    for (Var v = 1; v <= max_id; ++v) {
                        if (one_detector->is_backbone(v)) {
                            int lit = one_detector->backbone_sign(v) ? v : -v;
                            result.push_back(lit);
                        }
                    }
                } else if (detector_type == WITHOUT_ATTENTION) {
                    CheckCandidatesOneByOneWithoutAttention* without_detector =
                        static_cast<CheckCandidatesOneByOneWithoutAttention*>(detector);
                    for (Var v = 1; v <= max_id; ++v) {
                        if (without_detector->is_backbone(v)) {
                            int lit = without_detector->backbone_sign(v) ? v : -v;
                            result.push_back(lit);
                        }
                    }
                }
            } catch (...) {
                return vector<int>();
            }
        }

        return result;
    }

    /**
     * @brief Computes conditional backbone under user-specified assumptions
     *
     * Computes the backbone of the formula when certain variables are fixed to
     * specific values. This enables analysis of conditional dependencies:
     * "If feature X is selected and feature Y is excluded, which features become mandatory/forbidden?"
     *
     * **Assumptions as Constraints:**
     * Assumptions are treated as additional unit clauses added to the formula:
     * - Assumption {5} means: Add clause (5) to formula → variable 5 must be true
     * - Assumption {-12} means: Add clause (-12) to formula → variable 12 must be false
     * - Formula becomes: Original_CNF ∧ assumptions
     *
     * **Use Cases:**
     * 1. Feature model configuration: "If I select feature A, what else must be selected?"
     * 2. What-if analysis: "What are the consequences of enabling this option?"
     * 3. Partial configuration: "Given user choices X, Y, what is determined?"
     * 4. Incremental refinement: Iteratively add assumptions to explore configuration space
     *
     * **Algorithm Behavior:**
     * - Creates fresh detector instance (does NOT reuse cached detector)
     * - Initializes SAT solver with formula + assumptions
     * - Performs full backbone detection under constrained formula
     * - Cleans up detector after extraction (no caching for assumption-based calls)
     *
     * **Return Values:**
     * - Non-empty vector: Backbone literals under assumptions (formula ∧ assumptions is SAT)
     * - Empty vector: Either (1) UNSAT under assumptions, or (2) error occurred
     *
     * **UNSAT Handling:**
     * If assumptions make the formula unsatisfiable, returns empty vector.
     * Check return value size to distinguish:
     * @code
     * vector<int> bb = compute_with_assumps({5, -12});
     * if (bb.empty()) {
     *     // Either UNSAT or no backbone under assumptions
     *     // Cannot determine satisfiability from this method alone
     * }
     * @endcode
     *
     * **Example Usage:**
     * @code
     * // Feature model: variables 1-10 represent features
     * BackboneSolverAPI solver;
     * solver.read_dimacs("feature_model.cnf");
     * solver.create_backbone_detector("one");
     *
     * // What if feature 3 is selected and feature 7 is excluded?
     * vector<int> assumptions = {3, -7};
     * vector<int> conditional_bb = solver.compute_backbone_with_assumptions(assumptions);
     *
     * // conditional_bb might be {1, 3, -5, -7, 9}
     * // Interpretation: Under assumptions {3, -7}:
     * //   Features 1, 3, 9 are mandatory (positive backbone)
     * //   Features 5, 7 are forbidden (negative backbone)
     * //   Other features are optional
     * @endcode
     *
     * **Comparison with compute():**
     * | Aspect | compute() | compute_with_assumps() |
     * |--------|-----------|------------------------|
     * | Assumptions | None (base formula) | User-specified constraints |
     * | Detector caching | Cached after first call | Fresh detector each call |
     * | Performance | Fast (reuses detector) | Slower (recreates detector) |
     * | Use case | Global backbone | Conditional backbone |
     *
     * @param assumptions Vector of assumption literals (e.g., {5, -12, 3})
     * @return Vector of backbone literals under assumptions, empty if UNSAT or error
     *
     * @note Does NOT reuse cached detector - creates fresh instance each call
     * @note Does NOT update is_sat flag (unlike compute())
     * @note Detector is deallocated after extraction (no caching)
     * @note Assumptions are in DIMACS format: positive=true, negative=false
     * @note Exception-safe: catches all exceptions and returns empty vector
     * @note Cannot distinguish between UNSAT and empty backbone from return value alone
     *
     * @see compute() for unconditional backbone computation
     */
    vector<int> compute_with_assumps(const vector<int>& assumptions) {
        vector<int> result;

        if (!has_file || detector_type == NONE) {
            return result;
        }

        // Convert int assumptions to MiniSat Lit assumptions
        vec<Lit> minisat_assumptions;
        for (int lit_int : assumptions) {
            Var v = abs(lit_int);
            bool sign = (lit_int < 0);
            Lit l = mkLit(v, sign);
            minisat_assumptions.push(l);
        }

        try {
            if (detector_type == ONE) {
                CheckCandidatesOneByOne* one_detector = new CheckCandidatesOneByOne(max_id, clauses);
                bool is_sat_local = one_detector->run_with_assumptions(minisat_assumptions);

                if (!is_sat_local) {
                    // Unsatisfiable under assumptions
                    delete one_detector;
                    return result;
                }

                // Extract backbone
                for (Var v = 1; v <= max_id; ++v) {
                    if (one_detector->is_backbone(v)) {
                        int lit = one_detector->backbone_sign(v) ? v : -v;
                        result.push_back(lit);
                    }
                }

                delete one_detector;

            } else if (detector_type == WITHOUT_ATTENTION) {
                CheckCandidatesOneByOneWithoutAttention* without_detector =
                    new CheckCandidatesOneByOneWithoutAttention(max_id, clauses);
                bool is_sat_local = without_detector->run_with_assumptions(minisat_assumptions);

                if (!is_sat_local) {
                    // Unsatisfiable under assumptions
                    delete without_detector;
                    return result;
                }

                // Extract backbone
                for (Var v = 1; v <= max_id; ++v) {
                    if (without_detector->is_backbone(v)) {
                        int lit = without_detector->backbone_sign(v) ? v : -v;
                        result.push_back(lit);
                    }
                }

                delete without_detector;
            }
        } catch (...) {
            return vector<int>();
        }

        return result;
    }

    /**
     * @brief Gets the maximum variable ID in the loaded formula
     *
     * @return Maximum variable ID (number of variables), or 0 if no file loaded
     */
    int get_max_var() const { return max_id; }

    /**
     * @brief Checks if the loaded formula is satisfiable
     *
     * @return true if formula is SAT (determined during compute()), false otherwise
     * @note Only valid after calling compute() - returns false before computation
     */
    bool get_is_sat() const { return is_sat; }

    /**
     * @brief Prints formatted backbone summary to stdout
     *
     * Outputs a human-readable summary of the backbone computation including:
     * - Formula statistics (number of variables and clauses)
     * - Satisfiability status (SAT/UNSAT)
     * - Backbone size and percentage of determined variables
     * - List of all backbone literals
     *
     * **Output Format Example:**
     * @code
     * Formula statistics:
     *   Variables: 100
     *   Clauses: 450
     * Formula is SATISFIABLE
     * Backbone size: 23 literals (23.00% of variables)
     * Backbone literals: 1 -3 5 7 -9 11 -15 18 ...
     * @endcode
     *
     * **Special Cases:**
     * - UNSAT formula: Prints "Formula is UNSATISFIABLE"
     * - No computation yet: Prints "No backbone computed yet"
     * - Empty backbone: Shows size 0 with no literals listed
     *
     * @note Requires prior successful compute() call to show results
     * @note Output written to std::cout
     * @note Positive literals indicate positive backbone, negative indicate negative backbone
     */
    void print_bb() const {
        if (!is_sat) {
            cout << "Formula is UNSATISFIABLE" << endl;
            return;
        }

        if (!has_file || detector_type == NONE) {
            cout << "No backbone computed yet" << endl;
            return;
        }

        // Print formula statistics
        cout << "Formula statistics:" << endl;
        cout << "  Variables: " << max_id << endl;
        cout << "  Clauses: " << clauses.size() << endl;
        cout << "Formula is SATISFIABLE" << endl;

        // Count and collect backbone literals
        vector<int> backbone_lits;

        try {
            if (detector_type == ONE) {
                CheckCandidatesOneByOne* one_detector = static_cast<CheckCandidatesOneByOne*>(detector);
                if (one_detector) {
                    for (Var v = 1; v <= max_id; ++v) {
                        if (one_detector->is_backbone(v)) {
                            int lit = one_detector->backbone_sign(v) ? v : -v;
                            backbone_lits.push_back(lit);
                        }
                    }
                }
            } else if (detector_type == WITHOUT_ATTENTION) {
                CheckCandidatesOneByOneWithoutAttention* without_detector =
                    static_cast<CheckCandidatesOneByOneWithoutAttention*>(detector);
                if (without_detector) {
                    for (Var v = 1; v <= max_id; ++v) {
                        if (without_detector->is_backbone(v)) {
                            int lit = without_detector->backbone_sign(v) ? v : -v;
                            backbone_lits.push_back(lit);
                        }
                    }
                }
            }
        } catch (...) {
            cout << "Error retrieving backbone" << endl;
            return;
        }

        // Calculate percentage
        double percentage = (max_id > 0) ? (100.0 * backbone_lits.size() / max_id) : 0.0;

        cout << "Backbone size: " << backbone_lits.size() << " literals";
        cout << " (" << std::fixed << std::setprecision(2) << percentage << "% of variables)" << endl;

        if (backbone_lits.size() > 0) {
            cout << "Backbone literals: ";
            for (int lit : backbone_lits) {
                cout << lit << " ";
            }
            cout << endl;
        }
    }

private:
    /**
     * @enum DetectorType
     * @brief Internal enumeration for tracking selected detector algorithm
     */
    enum DetectorType { NONE, ONE, WITHOUT_ATTENTION };

    /**
     * @brief Cleans up detector instance and resets detector type
     *
     * Properly deallocates the detector object using the correct type-specific delete.
     * Since detector is stored as void*, must cast to correct type before deletion.
     *
     * @note Sets detector to nullptr and detector_type to NONE after cleanup
     * @note Safe to call multiple times (checks for nullptr)
     */
    void cleanup_detector() {
        if (detector) {
            if (detector_type == ONE) {
                delete static_cast<CheckCandidatesOneByOne*>(detector);
            } else if (detector_type == WITHOUT_ATTENTION) {
                delete static_cast<CheckCandidatesOneByOneWithoutAttention*>(detector);
            }
            detector = nullptr;
        }
        detector_type = NONE;
    }

    /**
     * @brief Cleans up DIMACS reader and formula data
     *
     * Deallocates the Reader instance and clears all formula data including
     * clauses, variable count, and status flags. Resets the instance to
     * initial state as if no file was ever loaded.
     *
     * @note Clears clauses vector, resets max_id to 0, sets has_file to false
     * @note Safe to call multiple times (checks for nullptr)
     */
    void cleanup_reader() {
        if (reader) {
            delete reader;
            reader = nullptr;
        }
        clauses.clear();
        max_id = 0;
        has_file = false;
    }

    Reader* reader = nullptr;               ///< DIMACS file reader (zlib wrapper)
    Var max_id;                             ///< Maximum variable ID (number of variables)
    CNF clauses;                            ///< Vector of clauses from DIMACS file
    void* detector;                         ///< Polymorphic detector instance (void* for type erasure)
    DetectorType detector_type = NONE;      ///< Currently selected detector algorithm type
    bool has_file;                          ///< Flag: has DIMACS file been successfully loaded
    bool is_sat;                            ///< Flag: is formula satisfiable (determined by compute)
};

// BackboneSolverAPI implementation

/**
 * @brief Constructs a new BackboneSolverAPI instance
 *
 * Initializes the PIMPL implementation object with default state.
 */
BackboneSolverAPI::BackboneSolverAPI() : pimpl(new Impl()) {}

/**
 * @brief Destructor - cleans up PIMPL implementation
 *
 * Deallocates the implementation object, which triggers cleanup of
 * detector and reader resources.
 */
BackboneSolverAPI::~BackboneSolverAPI() {
    delete pimpl;
}

/**
 * @brief Public API wrapper for read_file()
 *
 * Delegates to internal PIMPL implementation.
 * See BackboneSolverAPI.hh for detailed parameter documentation.
 */
bool BackboneSolverAPI::read_dimacs(const string& file_name) {
    return pimpl->read_file(file_name);
}

/**
 * @brief Public API wrapper for create_detector()
 *
 * Delegates to internal PIMPL implementation.
 * See BackboneSolverAPI.hh for detailed parameter documentation.
 */
bool BackboneSolverAPI::create_backbone_detector(const string& bb_detector) {
    return pimpl->create_detector(bb_detector);
}

/**
 * @brief Public API wrapper for compute()
 *
 * @return Vector of backbone literals
 */
vector<int> BackboneSolverAPI::compute_backbone() {
    return pimpl->compute();
}

/**
 * @brief Public API wrapper for compute_with_assumps()
 *
 * Delegates to internal PIMPL implementation.
 * See BackboneSolverAPI.hh for detailed parameter documentation.
 */
vector<int> BackboneSolverAPI::compute_backbone_with_assumptions(const vector<int>& assumptions) {
    return pimpl->compute_with_assumps(assumptions);
}

/**
 * @brief Public API wrapper for get_max_var()
 *
 * @return Maximum variable ID
 */
int BackboneSolverAPI::get_max_variable() const {
    return pimpl->get_max_var();
}

/**
 * @brief Public API wrapper for get_is_sat()
 *
 * @return true if formula is SAT, false otherwise
 */
bool BackboneSolverAPI::is_satisfiable() const {
    return pimpl->get_is_sat();
}

/**
 * @brief Public API wrapper for print_bb()
 */
void BackboneSolverAPI::print_backbone() const {
    pimpl->print_bb();
}

} // namespace backbone_solver
