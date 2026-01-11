/**
 * @file Dimacs2GraphsAPI.cc
 * @brief Implementation of the Dimacs2GraphsAPI class for generating dependency graphs from DIMACS CNF formulas
 *
 * This file implements the API for converting DIMACS CNF formulas into strong transitive
 * dependency and conflict graphs. The implementation uses SAT-based backbone detection to
 * identify feature relationships in variability models.
 *
 * The graph generation algorithm operates in three phases:
 *
 * **Phase 1: Global Backbone Computation**
 * - Computes the backbone of the entire formula (no assumptions)
 * - Identifies core features (positive backbone - always selected)
 * - Identifies dead features (negative backbone - never selected)
 * - These features are excluded from edge detection as they have no conditional dependencies
 *
 * **Phase 2: Parallel Variable Analysis**
 * - For each variable v (excluding core/dead features):
 *   - Compute backbone assuming v=true
 *   - Extract requires edges: if assuming v forces i (and i is not core), then v requires i
 *   - Extract excludes edges: if assuming v forbids i (and neither v nor i are dead), then v excludes i
 * - Uses multi-threaded processing with range-based static work partitioning
 * - Each thread processes an independent subset of variables
 *
 * **Phase 3: Output Generation**
 * - Generates Pajek .net format graph files:
 *   - `[basename]__requires.net`: Directed graph (v -> i means selecting v requires i)
 *   - `[basename]__excludes.net`: Undirected graph (v -- i means mutual exclusion)
 * - Generates feature list files:
 *   - `[basename]__core.txt`: Positive backbone features (always selected)
 *   - `[basename]__dead.txt`: Negative backbone features (never selected)
 *
 * **Threading Architecture (CRITICAL)**
 *
 * The implementation uses a pre-initialization pattern to ensure thread safety:
 * 1. Main thread creates and initializes all BackboneSolverAPI instances sequentially
 * 2. Each thread receives a pre-initialized solver instance (no initialization in worker threads)
 * 3. Worker threads only perform variable processing using their assigned solver
 * 4. Results are collected in thread-local buffers and merged in the main thread
 *
 * This pattern is required because BackboneSolverAPI is NOT thread-safe during initialization.
 * Violating this pattern (e.g., creating solvers inside worker threads) will cause data races
 * and undefined behavior.
 *
 * **Performance Characteristics**
 * - Thread count limited to CPU cores (fail-fast validation)
 * - Memory requirement: approximately 60-70 MB per thread
 * - Static work partitioning ensures balanced load distribution
 * - Progress monitoring with atomic counters (low overhead)
 *
 * @warning BackboneSolverAPI is NOT thread-safe. Each thread must use a separate solver
 *          instance, and all instances must be created and initialized in the main thread
 *          BEFORE spawning worker threads.
 *
 * @see Dimacs2GraphsAPI.hh for the public API interface
 * @see BackboneSolverAPI for the underlying SAT-based backbone detection
 *
 * @author Dimacs2Graphs Team
 * @date 2024
 */

#include "Dimacs2GraphsAPI.hh"
#include "../backbone_solver/src/api/BackboneSolverAPI.hh"
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <algorithm>
#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>
#include <memory>
#include <filesystem>

using namespace std;
using namespace dimacs2graphs;
using namespace backbone_solver;

/**
 * @class Dimacs2GraphsAPI::Impl
 * @brief PIMPL (Pointer to Implementation) idiom for ABI stability
 *
 * This implementation class encapsulates all internal state and implementation details
 * of the Dimacs2GraphsAPI. Using the PIMPL idiom provides:
 * - **ABI Stability**: Changes to internal implementation do not affect binary compatibility
 * - **Compilation Firewall**: Reduces compilation dependencies and improves build times
 * - **Information Hiding**: Keeps implementation details private
 *
 * Encapsulated components:
 * - BackboneSolverAPI instance for SAT-based backbone detection
 * - Formula statistics (number of variables and clauses)
 * - Global backbone vector (core and dead features)
 * - File path utilities (basename extraction, directory handling, path normalization)
 * - Thread coordination structures and worker management
 * - Error message storage for diagnostic reporting
 */
class Dimacs2GraphsAPI::Impl {
public:
    BackboneSolverAPI bone_api;
    int num_variables;
    int num_clauses;
    vector<int> global_backbone;
    string error_message;
    bool filter_auxiliary;

    Impl() : num_variables(0), num_clauses(0), filter_auxiliary(false) {}

    /**
     * @brief Normalizes a file path by removing trailing slashes
     *
     * Ensures consistent path representation across platforms by removing
     * trailing directory separators (both forward and backward slashes).
     *
     * @param path The path to normalize
     * @return Normalized path without trailing slashes
     */
    string normalize_path(const string& path) {
        if (path.empty()) return path;
        if (path.back() == '/' || path.back() == '\\') {
            return path.substr(0, path.length() - 1);
        }
        return path;
    }

    /**
     * @brief Extracts the basename from a file path
     *
     * Removes the directory path and .dimacs extension (if present) from a file path.
     * This is used to generate consistent output file names.
     *
     * @param filepath The full file path (e.g., "/path/to/model.dimacs")
     * @return Basename without directory or extension (e.g., "model")
     */
    string get_basename(const string& filepath) {
        // Remove directory path
        size_t last_slash = filepath.find_last_of("/\\");
        string filename = (last_slash == string::npos) ? filepath : filepath.substr(last_slash + 1);

        // Remove .dimacs extension if present
        if (filename.size() >= 7 && filename.substr(filename.size() - 7) == ".dimacs") {
            return filename.substr(0, filename.size() - 7);
        }
        return filename;
    }

    /**
     * @brief Extracts the directory path from a file path
     *
     * Returns the directory component of a file path, or "." if no directory
     * component is present.
     *
     * @param filepath The full file path
     * @return Directory path, or "." for current directory
     */
    string get_directory(const string& filepath) {
        size_t last_slash = filepath.find_last_of("/\\");
        if (last_slash == string::npos) return ".";
        return filepath.substr(0, last_slash);
    }

    /**
     * @brief Reads the clause count from a DIMACS file header
     *
     * Parses the DIMACS problem line (p cnf [vars] [clauses]) to extract
     * the number of clauses. This information is stored separately as it's
     * not provided by the BackboneSolverAPI.
     *
     * @param dimacs_path Path to the DIMACS file
     * @param clauses Output parameter to store the clause count
     * @return true if successful, false on error
     */
    bool read_clause_count(const string& dimacs_path, int& clauses) {
        ifstream dimacs_file(dimacs_path);
        if (!dimacs_file.is_open()) {
            error_message = "Could not open file: " + dimacs_path;
            return false;
        }

        string line_str;
        while (getline(dimacs_file, line_str)) {
            if (line_str[0] == 'p') {
                stringstream ss(line_str);
                char p;
                string cnf;
                int vars;
                ss >> p >> cnf >> vars >> clauses;
                dimacs_file.close();
                return true;
            }
        }
        dimacs_file.close();
        error_message = "No problem line found in DIMACS file";
        return false;
    }

    /**
     * @brief Reads feature names from DIMACS comments and identifies auxiliary variables
     *
     * Parses comment lines in the format "c [var_num] [feature_name]" to build
     * a map of variable numbers to feature names. Also identifies variables
     * whose names start with "aux_" (Tseitin auxiliary variables).
     *
     * @param dimacs_path Path to the DIMACS file
     * @param feature_map Output map of variable numbers to feature names
     * @param aux_vars Output set of auxiliary variable indices
     * @return true if successful, false on error
     */
    bool read_feature_names(const string& dimacs_path,
                           map<int, string>& feature_map,
                           vector<bool>& aux_vars) {
        ifstream dimacs_file(dimacs_path);
        if (!dimacs_file.is_open()) {
            error_message = "Could not open file: " + dimacs_path;
            return false;
        }

        string line_str;
        while (getline(dimacs_file, line_str)) {
            if (line_str.empty()) continue;
            if (line_str[0] == 'c') {
                stringstream ss(line_str);
                char c;
                int var_number;
                string word;

                if (ss >> c >> var_number) {
                    stringstream full_name;
                    bool has_words = false;

                    while (ss >> word) {
                        has_words = true;
                        if (full_name.tellp() > 0) full_name << " ";
                        full_name << word;
                    }

                    if (has_words) {
                        string name = full_name.str();
                        feature_map[var_number] = name;

                        // Check if this is an auxiliary variable (starts with "aux_")
                        if (name.size() >= 4 && name.substr(0, 4) == "aux_") {
                            // Ensure aux_vars vector is large enough
                            if (var_number >= static_cast<int>(aux_vars.size())) {
                                aux_vars.resize(var_number + 1, false);
                            }
                            aux_vars[var_number] = true;
                        }
                    }
                }
            } else if (line_str[0] == 'p') {
                // Problem line - we can stop after reading all comments
                // (comments typically come before the problem line)
                // But some formats have comments throughout, so continue
            }
        }
        dimacs_file.close();
        return true;
    }

    /**
     * @struct ThreadWorker
     * @brief Thread worker structure for parallel variable processing
     *
     * Each ThreadWorker instance represents a worker thread that processes a subset
     * of variables to generate requires and excludes edges. The worker maintains
     * thread-local buffers for edge accumulation and uses a pre-initialized
     * BackboneSolverAPI instance.
     *
     * **CRITICAL REQUIREMENT**: The BackboneSolverAPI instance must be created and
     * initialized in the main thread before being passed to the worker. Workers must
     * NEVER initialize solver instances themselves, as BackboneSolverAPI is not
     * thread-safe during initialization.
     *
     * @note Pre-initialization pattern: Solvers are created sequentially in the main
     *       thread, then passed to workers for parallel variable processing only.
     */
    struct ThreadWorker {
        int thread_id;
        int start_idx;
        int end_idx;

        // Thread-local resources (pre-initialized API passed from main thread)
        BackboneSolverAPI* bone_api;
        stringstream requires_list;
        stringstream excludes_list;

        // Shared read-only data
        const vector<int>& global_bb;
        const vector<bool>& aux_vars;
        const vector<int>& vars_to_process;
        int num_variables;

        // Error handling
        bool success;
        string error_msg;

        // Progress tracking
        atomic<int>* progress_counter;

        /**
         * @brief Constructs a thread worker for a range of indices in vars_to_process
         *
         * @param tid Thread identifier
         * @param start First index in vars_to_process (inclusive)
         * @param end Last index in vars_to_process (inclusive)
         * @param api Pre-initialized BackboneSolverAPI instance (created in main thread)
         * @param bb Global backbone vector (indexed array for O(1) lookup)
         * @param aux Auxiliary variables flags (true if variable is aux_)
         * @param vars Reference to vars_to_process vector
         * @param num_vars Total number of variables in the formula
         * @param progress Atomic counter for progress tracking
         */
        ThreadWorker(int tid, int start, int end,
                    BackboneSolverAPI* api,
                    const vector<int>& bb, const vector<bool>& aux,
                    const vector<int>& vars, int num_vars,
                    atomic<int>* progress)
            : thread_id(tid), start_idx(start), end_idx(end),
              bone_api(api), global_bb(bb), aux_vars(aux), vars_to_process(vars),
              num_variables(num_vars), success(true), progress_counter(progress) {}

        /**
         * @brief Checks if a variable is an auxiliary variable
         */
        bool is_aux(int v) const {
            return v < static_cast<int>(aux_vars.size()) && aux_vars[v];
        }

        /**
         * @brief Main worker thread execution function
         *
         * Processes variables at indices [start_idx, end_idx] in vars_to_process,
         * computing backbone with assumptions for each variable and extracting
         * requires/excludes edges. Updates the shared progress counter atomically.
         *
         * Exception safety: Catches all exceptions and stores error messages
         * for reporting in the main thread.
         */
        void run() {
            try {
                // Process assigned index range in vars_to_process
                for (int idx = start_idx; idx <= end_idx; idx++) {
                    int v = vars_to_process[idx];
                    process_variable(v);
                    (*progress_counter)++;
                }

                success = true;
            } catch (const exception& e) {
                success = false;
                error_msg = "Thread " + to_string(thread_id) +
                           " exception: " + e.what();
            } catch (...) {
                success = false;
                error_msg = "Thread " + to_string(thread_id) +
                           ": Unknown exception";
            }
        }

        /**
         * @brief Processes a single variable to extract dependency edges
         *
         * Computes the backbone assuming variable v is true, then extracts:
         * - **Requires edges**: If assuming v=true forces i=true (and i is not core)
         * - **Excludes edges**: If assuming v=true forces i=false (and neither v nor i are dead)
         *
         * The algorithm uses indexed arrays for O(1) backbone lookups to maximize performance.
         * Edges involving auxiliary variables are excluded.
         *
         * @param v The variable to process (1-indexed)
         */
        void process_variable(int v) {
            // Compute backbone assuming v=true
            vector<int> assumptions = {v};
            vector<int> line_vector =
                bone_api->compute_backbone_with_assumptions(assumptions);

            // Convert to indexed array for O(1) lookup
            vector<int> line(num_variables + 1, 0);
            for (int lit : line_vector) {
                int var = abs(lit);
                line[var] = lit;
            }

            // Extract requires edges (skip edges to auxiliary variables)
            for (int i = 1; i <= num_variables; i++) {
                if ((i != v) && (line[i] == i) && (global_bb[i] == 0) && !is_aux(i)) {
                    requires_list << v << " " << i << "\n";
                }
            }

            // Extract excludes edges (skip edges to auxiliary variables)
            for (int i = v; i <= num_variables; i++) {
                if ((line[i] == -i) && (global_bb[i] != -i) &&
                    (global_bb[v] != -v) && !is_aux(i)) {
                    excludes_list << v << " " << i << "\n";
                }
            }
        }
    };

    /**
     * @brief Internal implementation of graph generation
     *
     * This is the main algorithm that orchestrates the entire graph generation pipeline:
     *
     * **Pipeline Overview:**
     * 1. Load DIMACS file and create backbone detector
     * 2. Compute global backbone (core and dead features)
     * 3. Validate thread count against CPU cores
     * 4. Process variables (single-threaded or multi-threaded):
     *    - For each non-backbone variable v, compute backbone assuming v=true
     *    - Extract requires and excludes edges based on forced assignments
     * 5. Extract feature names from DIMACS comments
     * 6. Generate output files (Pajek graphs and feature lists)
     *
     * **Threading Strategy:**
     * - Single-threaded mode: Sequential processing with immediate output
     * - Multi-threaded mode:
     *   1. Pre-create all BackboneSolverAPI instances in main thread (CRITICAL)
     *   2. Create ThreadWorker instances with pre-initialized solvers
     *   3. Launch worker threads with static range partitioning
     *   4. Monitor progress with atomic counter
     *   5. Merge results from thread-local buffers after join
     *
     * **Thread Count Validation:**
     * - Minimum: 1 thread
     * - Maximum: Number of CPU cores (fail-fast if exceeded)
     * - Effective: min(requested_threads, num_variables)
     *
     * @param dimacs_file Path to input DIMACS file (with or without .dimacs extension)
     * @param output_folder Output directory (empty string uses input file directory)
     * @param detector Backbone detector algorithm name (e.g., "CheckCandidatesOneByOne")
     * @param num_of_threads Number of threads for parallel processing (1 for sequential)
     * @return true if successful, false on error (error message in error_message field)
     *
     * @note The formula must be satisfiable. UNSAT formulas are not supported.
     * @note Thread count exceeding CPU cores will fail with an error message.
     *
     * @see BackboneSolverAPI::create_backbone_detector() for available detector algorithms
     * @see ThreadWorker for parallel processing implementation
     */
    bool generate_graphs_impl(
        const string& dimacs_file,
        const string& output_folder,
        const string& detector,
        int num_of_threads
    ) {
        // Reset state
        num_variables = 0;
        num_clauses = 0;
        global_backbone.clear();
        error_message.clear();

        // Construct DIMACS file path
        string dimacs_path = dimacs_file;
        if (dimacs_path.size() < 7 || dimacs_path.substr(dimacs_path.size() - 7) != ".dimacs") {
            dimacs_path += ".dimacs";
        }

        // Determine output location
        string output_dir = output_folder.empty() ? get_directory(dimacs_path) : normalize_path(output_folder);
        string basename = get_basename(dimacs_file);
        string output_base = output_dir + "/" + basename;

        // Load DIMACS file
        if (!bone_api.read_dimacs(dimacs_path)) {
            error_message = "The input formula " + dimacs_path + " could not be loaded";
            cerr << error_message << endl;
            error_message = "Please check that " + dimacs_path + " conforms to the DIMACS CNF format and is accessible.";
            cerr << error_message << endl;
            return false;
        }
        cout << "Loaded formula: " << dimacs_path << endl;

        // Create backbone detector
        if (!bone_api.create_backbone_detector(detector)) {
            error_message = "Failed to create backbone detector: " + detector;
            cerr << error_message << endl;
            return false;
        }

        num_variables = bone_api.get_max_variable();

        // Read clause count from file
        if (!read_clause_count(dimacs_path, num_clauses)) {
            return false;
        }

        cout << "Detected " << num_variables << " variables and " << num_clauses << " clauses..." << endl;

        // Read feature names early to identify auxiliary variables (when filtering is enabled)
        map<int, string> feature_map;
        vector<bool> aux_vars(num_variables + 1, false);
        if (filter_auxiliary) {
            cout << "Filtering auxiliary (aux_*) variables from output..." << endl;
            if (!read_feature_names(dimacs_path, feature_map, aux_vars)) {
                return false;
            }
            // Count auxiliary variables for informational purposes
            int aux_count = 0;
            for (int v = 1; v <= num_variables; v++) {
                if (v < static_cast<int>(aux_vars.size()) && aux_vars[v]) {
                    aux_count++;
                }
            }
            if (aux_count > 0) {
                cout << "Found " << aux_count << " auxiliary variables to filter" << endl;
            }
        }

        // Helper lambda to check if a variable is auxiliary
        auto is_aux = [&aux_vars](int v) -> bool {
            return v < static_cast<int>(aux_vars.size()) && aux_vars[v];
        };

        // Build list of variables to process (excluding auxiliary variables)
        vector<int> vars_to_process;
        if (filter_auxiliary) {
            for (int v = 1; v <= num_variables; v++) {
                if (!is_aux(v)) {
                    vars_to_process.push_back(v);
                }
            }
            cout << "Processing " << vars_to_process.size() << " non-auxiliary variables" << endl;
        } else {
            // Process all variables
            vars_to_process.reserve(num_variables);
            for (int v = 1; v <= num_variables; v++) {
                vars_to_process.push_back(v);
            }
        }

        // Compute global backbone
        cout << "Computing core and dead features..." << endl;
        vector<int> bb_vector = bone_api.compute_backbone();
        global_backbone = bb_vector;

        // Convert backbone vector to indexed array for O(1) lookup
        vector<int> bb(num_variables + 1, 0);
        for (int lit : bb_vector) {
            int var = abs(lit);
            bb[var] = lit;
        }

        // Validate thread count
        unsigned int max_threads = thread::hardware_concurrency();
        if (max_threads == 0) max_threads = 4; // Fallback if detection fails

        if (num_of_threads < 1) {
            error_message = "num_of_threads must be at least 1";
            cerr << error_message << endl;
            return false;
        }

        if (num_of_threads > static_cast<int>(max_threads)) {
            error_message = "Requested " + to_string(num_of_threads) +
                           " threads but only " + to_string(max_threads) +
                           " cores available. Reduce thread count.";
            cerr << error_message << endl;
            return false;
        }

        // Calculate total variables to process (excluding aux vars when filtering)
        int total_to_process = static_cast<int>(vars_to_process.size());

        // Cap effective threads at number of variables to process
        int effective_threads = min(num_of_threads, total_to_process);

        if (effective_threads > 1) {
            cout << "Using " << effective_threads << " threads for parallel processing..." << endl;
        }

        stringstream excludes_list;
        stringstream requires_list;

        if (effective_threads == 1) {
            // Single-threaded mode - iterate only over vars_to_process
            for (int idx = 0; idx < total_to_process; idx++) {
                int v = vars_to_process[idx];
                cout << "\rProgress: " << (idx + 1) << " of " << total_to_process << " variables" << flush;

                // Compute backbone assuming v=true
                vector<int> assumptions = {v};
                vector<int> line_vector = bone_api.compute_backbone_with_assumptions(assumptions);

                // Convert to indexed array for O(1) lookup
                vector<int> line(num_variables + 1, 0);
                for (int lit : line_vector) {
                    int var = abs(lit);
                    line[var] = lit;
                }

                // Extract requires edges (skip edges to auxiliary variables)
                for (int i = 1; i <= num_variables; i++) {
                    if ((i != v) && (line[i] == i) && (bb[i] == 0) && !is_aux(i)) {
                        requires_list << v << " " << i << endl;
                    }
                }

                // Extract excludes edges (skip edges to auxiliary variables)
                for (int i = v; i <= num_variables; i++) {
                    if ((line[i] == -i) && (bb[i] != -i) && (bb[v] != -v) && !is_aux(i)) {
                        excludes_list << v << " " << i << endl;
                    }
                }
            }
            cout << endl;
        } else {
            // Multi-threaded mode
            atomic<int> progress_counter(0);

            // Pre-create and initialize BackboneSolverAPI instances (single-threaded)
            cout << "Initializing " << effective_threads << " backbone solver instances..." << endl;
            vector<unique_ptr<BackboneSolverAPI>> apis;
            for (int t = 0; t < effective_threads; t++) {
                apis.emplace_back(make_unique<BackboneSolverAPI>());
                if (!apis[t]->read_dimacs(dimacs_path)) {
                    error_message = "Failed to load DIMACS for thread " + to_string(t);
                    cerr << error_message << endl;
                    return false;
                }
                if (!apis[t]->create_backbone_detector(detector)) {
                    error_message = "Failed to create detector for thread " + to_string(t);
                    cerr << error_message << endl;
                    return false;
                }
            }

            // Create thread workers with pre-initialized APIs
            vector<ThreadWorker> workers;
            workers.reserve(effective_threads); // Prevent reallocation
            vector<thread> threads;
            threads.reserve(effective_threads);

            // Distribute vars_to_process indices across threads
            int vars_per_thread = total_to_process / effective_threads;
            int remainder = total_to_process % effective_threads;

            int current_idx = 0;
            for (int t = 0; t < effective_threads; t++) {
                int start_idx = current_idx;
                int count = vars_per_thread + (t < remainder ? 1 : 0);
                int end_idx = start_idx + count - 1;

                workers.emplace_back(ThreadWorker(
                    t, start_idx, end_idx,
                    apis[t].get(), bb, aux_vars, vars_to_process, num_variables,
                    &progress_counter
                ));

                current_idx = end_idx + 1;
            }

            // Launch threads
            for (auto& worker : workers) {
                threads.emplace_back([&worker]() { worker.run(); });
            }

            // Progress monitoring
            while (progress_counter < total_to_process) {
                int completed = progress_counter.load();
                cout << "\rProgress: " << completed << " of "
                     << total_to_process << " variables" << flush;
                this_thread::sleep_for(chrono::milliseconds(100));
            }

            // Wait for all workers
            for (auto& thread : threads) {
                thread.join();
            }

            cout << "\rProgress: " << total_to_process << " of "
                 << total_to_process << " variables" << endl;

            // Check for errors (fail-fast)
            for (const auto& worker : workers) {
                if (!worker.success) {
                    error_message = worker.error_msg;
                    cerr << error_message << endl;
                    return false;
                }
            }

            // Merge results from all threads
            for (const auto& worker : workers) {
                requires_list << worker.requires_list.str();
                excludes_list << worker.excludes_list.str();
            }
        }

        // Extract feature names from DIMACS comments (if not already read for filtering)
        // Note: feature_map and aux_vars were declared earlier
        stringstream feat_stream;
        stringstream core_stream;
        stringstream dead_stream;

        ifstream dimacs_input(dimacs_path);
        string line_str;
        while (getline(dimacs_input, line_str)) {
            if (line_str.empty()) continue;
            if (line_str[0] == 'c') {
                stringstream ss(line_str);
                char c;
                int var_number;
                string word;

                if (ss >> c >> var_number) {
                    // Skip auxiliary variables from output
                    if (is_aux(var_number)) {
                        continue;
                    }

                    // Process each word separately
                    feat_stream << var_number;
                    bool has_words = false;
                    stringstream full_name;

                    while (ss >> word) {
                        has_words = true;
                        // Add to feature map as full name
                        if (full_name.tellp() > 0) full_name << " ";
                        full_name << word;

                        // Each word gets quoted separately
                        feat_stream << " \"" << word << "\"";

                        if (bb[var_number] > 0) {
                            core_stream << var_number << " \"" << word << "\"";
                        } else if (bb[var_number] < 0) {
                            dead_stream << var_number << " \"" << word << "\"";
                        }
                    }

                    if (has_words) {
                        feat_stream << endl;
                        feature_map[var_number] = full_name.str();

                        if (bb[var_number] > 0) {
                            core_stream << endl;
                        } else if (bb[var_number] < 0) {
                            dead_stream << endl;
                        }
                    }
                }
            }
        }
        dimacs_input.close();

        // Create output directory if it doesn't exist
        if (!output_dir.empty() && !filesystem::exists(output_dir)) {
            try {
                filesystem::create_directories(output_dir);
            } catch (const exception& e) {
                error_message = "Could not create output directory: " + output_dir + " - " + e.what();
                cerr << error_message << endl;
                return false;
            }
        }

        // Write output files
        ofstream outFile;

        cout << "Saving to " << output_base << "__core.txt" << endl;
        outFile.open(output_base + "__core.txt");
        if (!outFile.is_open()) {
            error_message = "Could not create output file: " + output_base + "__core.txt";
            return false;
        }
        outFile << core_stream.str();
        outFile.close();

        cout << "Saving to " << output_base << "__dead.txt" << endl;
        outFile.open(output_base + "__dead.txt");
        if (!outFile.is_open()) {
            error_message = "Could not create output file: " + output_base + "__dead.txt";
            return false;
        }
        outFile << dead_stream.str();
        outFile.close();

        cout << "Saving to " << output_base << "__requires.net" << endl;
        outFile.open(output_base + "__requires.net");
        if (!outFile.is_open()) {
            error_message = "Could not create output file: " + output_base + "__requires.net";
            return false;
        }
        outFile << "*Vertices " << num_variables << endl;
        outFile << feat_stream.str();
        outFile << "*Arcs" << endl;
        outFile << requires_list.str();
        outFile << endl;
        outFile.close();

        cout << "Saving to " << output_base << "__excludes.net" << endl;
        outFile.open(output_base + "__excludes.net");
        if (!outFile.is_open()) {
            error_message = "Could not create output file: " + output_base + "__excludes.net";
            return false;
        }
        outFile << "*Vertices " << num_variables << endl;
        outFile << feat_stream.str();
        outFile << "*Edges" << endl;
        outFile << excludes_list.str();
        outFile << endl;
        outFile.close();

        cout << "Done!" << endl;
        return true;
    }
};

// API Implementation

/**
 * @brief Constructor - creates PIMPL instance
 */
Dimacs2GraphsAPI::Dimacs2GraphsAPI() : pimpl(new Impl()) {}

/**
 * @brief Destructor - cleans up PIMPL instance
 */
Dimacs2GraphsAPI::~Dimacs2GraphsAPI() {
    delete pimpl;
}

/**
 * @brief Generates dependency graphs from a DIMACS CNF formula
 *
 * Public API entry point that delegates to the PIMPL implementation.
 * See Dimacs2GraphsAPI.hh for detailed parameter documentation.
 *
 * @see generate_graphs_impl() for detailed implementation documentation
 */
bool Dimacs2GraphsAPI::generate_graphs(
    const string& dimacs_file,
    const string& output_folder,
    const string& detector,
    int num_of_threads
) {
    return pimpl->generate_graphs_impl(dimacs_file, output_folder, detector, num_of_threads);
}

/**
 * @brief Gets the number of variables in the loaded formula
 * @return Number of variables
 */
int Dimacs2GraphsAPI::get_num_variables() const {
    return pimpl->num_variables;
}

/**
 * @brief Gets the number of clauses in the loaded formula
 * @return Number of clauses
 */
int Dimacs2GraphsAPI::get_num_clauses() const {
    return pimpl->num_clauses;
}

/**
 * @brief Gets the global backbone of the formula
 * @return Vector of literals in the backbone (positive for core, negative for dead)
 */
vector<int> Dimacs2GraphsAPI::get_global_backbone() const {
    return pimpl->global_backbone;
}

/**
 * @brief Gets the last error message
 * @return Error message string (empty if no error)
 */
string Dimacs2GraphsAPI::get_error_message() const {
    return pimpl->error_message;
}

/**
 * @brief Sets whether to filter auxiliary variables
 * @param filter If true, variables starting with "aux_" are excluded
 */
void Dimacs2GraphsAPI::set_filter_auxiliary(bool filter) {
    pimpl->filter_auxiliary = filter;
}
