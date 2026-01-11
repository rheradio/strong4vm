/*
 *  Dimacs2GraphsAPI.hh
 *
 *  Public C++ API for generating graph representations from DIMACS CNF formulas
 *  using Backbone Solver backbone detection.
 */

/**
 * @defgroup ParallelGraphs Parallel Graph Generation
 * @brief Multi-threaded generation of dependency graphs using parallel backbone solving
 *
 * ## Overview
 *
 * The Parallel Graph Generation component analyzes Boolean formulas to extract
 * feature dependencies and conflicts using SAT-based backbone detection. The analysis
 * is parallelized across multiple threads for performance on large formulas.
 *
 * ## Algorithm: Parallel Backbone Solving
 *
 * The graph generation algorithm systematically tests each variable in the formula
 * to determine its relationships with other variables:
 *
 * **For each variable v:**
 * 1. **Assume v=true**, compute backbone under this assumption
 * 2. **Assume v=false**, compute backbone under this assumption
 * 3. **Extract relationships:**
 *    - If assuming v forces variable i to be true → v **requires** i
 *    - If assuming v forces variable i to be false → v **excludes** i
 * 4. **Identify core/dead features:**
 *    - If v is in the global backbone (true in all solutions) → v is **core**
 *    - If ¬v is in the global backbone (false in all solutions) → v is **dead**
 *
 * ## Parallel Execution Strategy
 *
 * To accelerate processing of large formulas with hundreds or thousands of variables,
 * the algorithm uses **multi-threaded parallel processing** with a critical thread safety pattern:
 *
 * **Pre-Initialization Pattern (CRITICAL):**
 * ```
 * Main Thread:
 *   1. Create N BackboneSolverAPI instances (one per thread)
 *   2. Initialize each solver sequentially:
 *      - Read DIMACS file
 *      - Create backbone detector
 *   3. Launch N worker threads, passing pre-initialized solvers
 *
 * Worker Threads (parallel):
 *   4. Process assigned variable range [start, end]
 *   5. For each variable:
 *      - Compute backbone with assumptions
 *      - Extract requires/excludes relationships
 *      - Store results in thread-local buffers
 *   6. Return results to main thread
 *
 * Main Thread:
 *   7. Merge results from all threads
 *   8. Write output files
 * ```
 *
 * **Why Pre-Initialization?**
 * - BackboneSolverAPI is NOT thread-safe during initialization
 * - File I/O and solver setup must occur in the main thread
 * - Only the backbone computation itself is parallelized
 * - Each thread operates on an independent solver instance
 *
 * **Work Partitioning:**
 * - Variables are divided into N equal-sized ranges
 * - Each thread processes variables [start, end] independently
 * - Static partitioning (no work stealing) for predictable performance
 * - Example with 1000 variables, 4 threads:
 *   - Thread 1: variables 1-250
 *   - Thread 2: variables 251-500
 *   - Thread 3: variables 501-750
 *   - Thread 4: variables 751-1000
 *
 * ## Performance Characteristics
 *
 * **Scalability:**
 * - Near-linear speedup for formulas with >100 variables
 * - Diminishing returns beyond CPU core count
 * - Recommended thread count: min(num_cores, num_variables / 50)
 *
 * **Memory Usage:**
 * - Each thread requires ~60-70 MB for solver instance
 * - Example: 8 threads ≈ 500 MB total memory
 *
 * **Thread Count Selection:**
 * - Small formulas (<100 vars): 1-2 threads (overhead not worth it)
 * - Medium formulas (100-500 vars): 4-8 threads
 * - Large formulas (>500 vars): up to CPU core count
 * - API validates thread count ≤ available CPU cores (fail-fast if exceeded)
 *
 * ## Output Files
 *
 * The algorithm generates four output files:
 *
 * **Graph Files (Pajek .net format):**
 * - `[basename]__requires.net`: Directed graph where edge v→i means "selecting v requires i"
 * - `[basename]__excludes.net`: Undirected graph where edge v--i means "v and i are mutually exclusive"
 *
 * **Feature Lists (text files):**
 * - `[basename]__core.txt`: Variables that are always true (positive backbone)
 * - `[basename]__dead.txt`: Variables that are always false (negative backbone)
 *
 * ## API Usage
 *
 * Basic parallel graph generation:
 * @code
 * dimacs2graphs::Dimacs2GraphsAPI api;
 * bool success = api.generate_graphs(
 *     "formula.dimacs",     // Input DIMACS file
 *     "output/",            // Output directory
 *     "one",                // Backbone detector ("one" = with activity bumping)
 *     8                     // Number of threads
 * );
 * if (success) {
 *     int num_vars = api.get_num_variables();
 *     std::cout << "Processed " << num_vars << " variables in parallel" << std::endl;
 * }
 * @endcode
 *
 * ## Thread Safety
 *
 * **CRITICAL REQUIREMENT:**
 * - BackboneSolverAPI instances MUST be created and initialized in the main thread
 * - NEVER initialize solvers inside worker threads
 * - Each thread must receive a fully initialized solver instance
 * - Violating this causes race conditions and undefined behavior
 *
 * @see Dimacs2GraphsAPI Main API class for parallel graph generation
 * @see BackboneSolverAPI Underlying backbone detection engine (NOT thread-safe during init)
 */

#ifndef DIMACS2GRAPHS_API_HH
#define DIMACS2GRAPHS_API_HH

#include <string>
#include <vector>

namespace dimacs2graphs {

/**
 * @class Dimacs2GraphsAPI
 * @ingroup ParallelGraphs
 * @brief Main API class for generating transitive graphs from DIMACS CNF formulas
 *
 * This class provides a simple interface to:
 * 1. Load a DIMACS CNF file
 * 2. Compute backbone relationships using Backbone Solver
 * 3. Generate graph output files (requires.net, excludes.net, core.txt, dead.txt)
 */
class Dimacs2GraphsAPI {
public:
    /**
     * @brief Constructor
     */
    Dimacs2GraphsAPI();

    /**
     * @brief Destructor
     */
    ~Dimacs2GraphsAPI();

    /**
     * @brief Generate graph files from a DIMACS CNF formula
     *
     * @param dimacs_file Path to the input DIMACS file (with or without .dimacs extension)
     * @param output_folder Optional output folder path (default: same as input file location)
     * @param detector Backbone detector to use: "one" (default) or "without"
     *                 - "one": CheckCandidatesOneByOne with activity bumping (recommended)
     *                 - "without": CheckCandidatesOneByOneWithoutAttention (baseline)
     * @param num_of_threads Number of threads to use for parallel processing (default: 1)
     *                       Must not exceed available CPU cores
     * @return true if successful, false otherwise
     *
     * Output files created:
     * - `[basename]__requires.net`  : Pajek format directed graph (requires relationships)
     * - `[basename]__excludes.net`  : Pajek format undirected graph (excludes relationships)
     * - `[basename]__core.txt`      : Core features (positive backbone literals)
     * - `[basename]__dead.txt`      : Dead features (negative backbone literals)
     */
    bool generate_graphs(
        const std::string& dimacs_file,
        const std::string& output_folder = "",
        const std::string& detector = "one",
        int num_of_threads = 1
    );

    /**
     * @brief Get the number of variables in the last processed formula
     * @return Number of variables, or 0 if no formula has been processed
     */
    int get_num_variables() const;

    /**
     * @brief Get the number of clauses in the last processed formula
     * @return Number of clauses, or 0 if no formula has been processed
     */
    int get_num_clauses() const;

    /**
     * @brief Get the global backbone computed for the last processed formula
     * @return Vector of backbone literals (empty if no formula has been processed)
     */
    std::vector<int> get_global_backbone() const;

    /**
     * @brief Get the last error message
     * @return Error message string (empty if no error)
     */
    std::string get_error_message() const;

    /**
     * @brief Set whether to filter auxiliary variables from output
     *
     * When enabled, variables whose names start with "aux_" (Tseitin auxiliary
     * variables) will be excluded from:
     * - The main iteration loop (for efficiency)
     * - Core and dead feature lists
     * - Requires and excludes graph edges
     *
     * @param filter If true, filter auxiliary variables (default: false)
     */
    void set_filter_auxiliary(bool filter);

private:
    class Impl;
    Impl* pimpl;
};

} // namespace dimacs2graphs

#endif // DIMACS2GRAPHS_API_HH
