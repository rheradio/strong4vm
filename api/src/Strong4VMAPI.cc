/**
 * @file Strong4VMAPI.cc
 * @brief Implementation of Strong4VM unified API
 *
 * This file implements the Strong4VMAPI class which provides a high-level
 * interface for the complete Strong4VM analysis pipeline, orchestrating the
 * transformation from variability models to strong transitive dependency and
 * conflict graphs.
 *
 * ## Pipeline Architecture
 *
 * The Strong4VM pipeline consists of two major stages:
 *
 * 1. **UVL to DIMACS Conversion** (conditional):
 *    - Parses UVL feature model files
 *    - Transforms feature model structure to CNF clauses
 *    - Supports two conversion modes: STRAIGHTFORWARD and TSEITIN
 *    - Outputs DIMACS CNF format
 *
 * 2. **DIMACS to Graphs Generation**:
 *    - Computes global backbone using SAT-based detection
 *    - Generates strong transitive dependency graph (requires)
 *    - Generates strong transitive conflict graph (excludes)
 *    - Identifies core (mandatory) and dead (forbidden) features
 *    - Outputs Pajek .net graph files and feature lists
 *
 * ## PIMPL Pattern
 *
 * This implementation uses the PIMPL (Pointer to Implementation) idiom to:
 * - **Maintain ABI stability**: Changes to internal data structures don't affect binary compatibility
 * - **Hide implementation details**: Users don't need to include uvl2dimacs/dimacs2graphs headers
 * - **Improve compilation times**: Reduces header dependencies
 * - **Encapsulate pipeline logic**: Clean separation between public API and internal orchestration
 *
 * ## Key Design Decisions
 *
 * - **Automatic file type detection**: Uses file extensions (.uvl, .dimacs, .cnf) when input type is AUTO
 * - **Temporary file management**: DIMACS files are automatically cleaned up unless keep_dimacs is set
 * - **Error propagation**: Detailed error messages from component APIs are preserved and returned
 * - **Pipeline coordination**: Sequential execution ensures proper data flow between stages
 * - **Configuration validation**: Early validation prevents pipeline execution with invalid parameters
 *
 * @author Strong4VM Team
 * @date 2024
 */

#include "strong4vm/Strong4VMAPI.hh"
#include "../../uvl2dimacs/api/include/uvl2dimacs/UVL2Dimacs.hh"
#include "../../dimacs2graphs/api/Dimacs2GraphsAPI.hh"

#include <filesystem>
#include <iostream>
#include <algorithm>
#include <cctype>

namespace fs = std::filesystem;

namespace strong4vm {

/**
 * @brief Internal implementation class (PIMPL idiom)
 *
 * This class encapsulates all implementation details of the Strong4VM API:
 * - **Configuration state**: Default settings for conversion mode, detector, threads, verbosity
 * - **Pipeline orchestration**: Coordinates uvl2dimacs and dimacs2graphs execution
 * - **File management**: Handles paths, extensions, temporary files
 * - **Validation logic**: Ensures configuration parameters are valid before execution
 * - **Result aggregation**: Combines statistics from both pipeline stages
 *
 * The PIMPL pattern ensures that changes to these internal details do not
 * affect the binary interface of the public Strong4VMAPI class.
 */
class Strong4VMAPI::Impl {
public:
    // Default settings
    bool verbose;
    ConversionMode default_conversion_mode;
    BackboneDetector default_detector;
    int default_num_threads;

    // Last result for debugging
    AnalysisResult last_result;

    Impl()
        : verbose(false)
        , default_conversion_mode(ConversionMode::STRAIGHTFORWARD)
        , default_detector(BackboneDetector::ONE)
        , default_num_threads(1) {}

    /**
     * @brief Detect file type based on file extension
     *
     * Performs case-insensitive extension matching to determine input type:
     * - `.uvl` → InputType::UVL (feature model file)
     * - `.dimacs` or `.cnf` → InputType::DIMACS (CNF formula file)
     * - Other extensions → InputType::AUTO (unknown, requires manual specification)
     *
     * @param filename Path to the input file (can be relative or absolute)
     * @return Detected InputType enum value
     */
    InputType detect_file_type(const std::string& filename) const {
        fs::path path(filename);
        std::string ext = path.extension().string();

        // Convert to lowercase for case-insensitive comparison
        std::transform(ext.begin(), ext.end(), ext.begin(),
                      [](unsigned char c) { return std::tolower(c); });

        if (ext == ".uvl") {
            return InputType::UVL;
        } else if (ext == ".dimacs" || ext == ".cnf") {
            return InputType::DIMACS;
        }

        return InputType::AUTO;
    }

    /**
     * @brief Extract basename from file path without extension
     *
     * Uses filesystem::path::stem() to extract the filename without directory
     * components or file extension. Used for generating output filenames.
     *
     * Examples:
     * - "/path/to/model.uvl" → "model"
     * - "example.dimacs" → "example"
     * - "../test/file.cnf" → "file"
     *
     * @param filepath Full path to file
     * @return Basename without directory path or extension
     */
    std::string get_basename(const std::string& filepath) const {
        fs::path path(filepath);
        return path.stem().string();
    }

    /**
     * @brief Extract directory path from file path
     *
     * Returns the parent directory of the file. If the file path has no
     * directory component, returns "." (current directory).
     *
     * @param filepath Full path to file
     * @return Parent directory path, or "." if no parent
     */
    std::string get_directory(const std::string& filepath) const {
        fs::path path(filepath);
        if (path.has_parent_path()) {
            return path.parent_path().string();
        }
        return ".";
    }

    /**
     * @brief Validate analysis configuration before execution
     *
     * Performs comprehensive validation of configuration parameters:
     * - **Input file checks**: Exists, non-empty path
     * - **File type detection**: Can determine UVL or DIMACS if AUTO
     * - **Thread count validation**: At least 1 thread specified
     * - **Output directory**: Exists or can be created
     *
     * This validation happens before any pipeline execution to ensure
     * fast-fail behavior and clear error messages.
     *
     * @param config Configuration to validate
     * @return Empty string if valid, descriptive error message if invalid
     */
    std::string validate_configuration(const AnalysisConfig& config) const {
        // Check input file
        if (config.input_file.empty()) {
            return "Input file not specified";
        }

        if (!fs::exists(config.input_file)) {
            return "Input file not found: " + config.input_file;
        }

        // Detect file type if AUTO
        InputType type = config.input_type;
        if (type == InputType::AUTO) {
            type = detect_file_type(config.input_file);
        }

        if (type == InputType::AUTO) {
            return "Cannot determine file type. Expected .uvl or .dimacs extension";
        }

        // Validate thread count
        if (config.num_threads < 1) {
            return "Thread count must be at least 1";
        }

        // Validate output directory if specified
        if (!config.output_dir.empty() && !fs::exists(config.output_dir)) {
            // Try to create it
            try {
                fs::create_directories(config.output_dir);
            } catch (const std::exception& e) {
                return "Cannot create output directory: " + std::string(e.what());
            }
        }

        return ""; // Valid
    }

    /**
     * @brief Convert detector enum to string identifier
     *
     * Translates BackboneDetector enum values to string identifiers used
     * by the dimacs2graphs API:
     * - BackboneDetector::ONE → "one" (with activity bumping, faster)
     * - BackboneDetector::WITHOUT → "without" (without activity bumping, baseline)
     *
     * @param detector Detector enum value
     * @return String identifier for dimacs2graphs API
     */
    std::string detector_to_string(BackboneDetector detector) const {
        return (detector == BackboneDetector::ONE) ? "one" : "without";
    }

    /**
     * @brief Execute the complete Strong4VM analysis pipeline
     *
     * This is the core implementation method that orchestrates the entire pipeline:
     *
     * ## Pipeline Stages
     *
     * **Stage 0: Validation**
     * - Validate all configuration parameters
     * - Detect input file type if AUTO
     * - Ensure output directory exists or can be created
     *
     * **Stage 1: UVL to DIMACS Conversion** (if input is UVL)
     * - Use uvl2dimacs API to parse and convert feature model
     * - Apply selected CNF transformation mode
     * - Generate DIMACS file in output directory
     * - Mark as temporary if keep_dimacs is false
     *
     * **Stage 2: DIMACS to Graphs**
     * - Use dimacs2graphs API for backbone detection
     * - Generate requires/excludes graphs using parallel processing
     * - Identify core and dead features from global backbone
     * - Write Pajek .net files and feature lists
     *
     * **Stage 3: Cleanup**
     * - Remove temporary DIMACS file if needed
     * - Aggregate statistics from both stages
     * - Return comprehensive analysis result
     *
     * ## Error Handling
     *
     * Errors at any stage cause immediate pipeline termination with:
     * - success = false
     * - Descriptive error message indicating which stage failed
     * - Automatic cleanup of temporary files
     *
     * @param config Analysis configuration with all pipeline parameters
     * @return AnalysisResult containing success status, statistics, file paths, and errors
     */
    AnalysisResult perform_analysis(const AnalysisConfig& config) {
        AnalysisResult result;
        result.input_file = config.input_file;

        // Validate configuration
        std::string validation_error = validate_configuration(config);
        if (!validation_error.empty()) {
            result.success = false;
            result.error_message = validation_error;
            return result;
        }

        // Determine input type
        InputType input_type = config.input_type;
        if (input_type == InputType::AUTO) {
            input_type = detect_file_type(config.input_file);
        }
        result.input_type = input_type;

        // Determine output directory
        std::string output_dir = config.output_dir;
        if (output_dir.empty()) {
            output_dir = get_directory(config.input_file);
        }

        // Ensure output directory exists
        if (!fs::exists(output_dir)) {
            try {
                fs::create_directories(output_dir);
            } catch (const std::exception& e) {
                result.success = false;
                result.error_message = "Failed to create output directory: " +
                                      std::string(e.what());
                return result;
            }
        }

        std::string dimacs_file;
        std::string basename = get_basename(config.input_file);
        bool temp_dimacs = false;

        // Step 1: Ensure we have a DIMACS file
        if (input_type == InputType::UVL) {
            if (verbose) {
                std::cout << "=================================================\n";
                std::cout << "Step 1: Converting UVL to DIMACS\n";
                std::cout << "=================================================\n";
            }

            // Determine DIMACS output path
            dimacs_file = output_dir + "/" + basename + ".dimacs";
            temp_dimacs = !config.keep_dimacs;

            // Convert UVL to DIMACS
            uvl2dimacs::UVL2Dimacs converter;
            converter.set_verbose(verbose);

            // Set conversion mode
            uvl2dimacs::ConversionMode uvl_mode =
                (config.conversion_mode == ConversionMode::TSEITIN)
                ? uvl2dimacs::ConversionMode::TSEITIN
                : uvl2dimacs::ConversionMode::STRAIGHTFORWARD;

            converter.set_mode(uvl_mode);

            auto uvl_result = converter.convert(config.input_file, dimacs_file);

            if (!uvl_result.success) {
                result.success = false;
                result.error_message = "UVL to DIMACS conversion failed: " +
                                      uvl_result.error_message;
                return result;
            }

            // Copy UVL statistics to result
            result.num_features = uvl_result.num_features;
            result.num_relations = uvl_result.num_relations;
            result.num_constraints = uvl_result.num_constraints;
            result.num_variables = uvl_result.num_variables;
            result.num_clauses = uvl_result.num_clauses;

            if (verbose) {
                std::cout << "\nConversion successful!\n";
                std::cout << "  Features:   " << uvl_result.num_features << "\n";
                std::cout << "  Variables:  " << uvl_result.num_variables << "\n";
                std::cout << "  Clauses:    " << uvl_result.num_clauses << "\n";
                if (config.keep_dimacs) {
                    std::cout << "  DIMACS file: " << dimacs_file << "\n";
                }
                std::cout << "\n";
            }

            if (config.keep_dimacs) {
                result.dimacs_file = dimacs_file;
            }

        } else {
            // Input is already DIMACS
            dimacs_file = config.input_file;
            basename = get_basename(dimacs_file);
        }

        // Step 2: Generate graphs from DIMACS
        if (verbose) {
            std::cout << "=================================================\n";
            std::cout << "Step 2: Generating Strong Transitive Graphs\n";
            std::cout << "=================================================\n";
        }

        dimacs2graphs::Dimacs2GraphsAPI graph_api;

        std::string detector_str = detector_to_string(config.detector);
        bool graph_success = graph_api.generate_graphs(
            dimacs_file,
            output_dir,
            detector_str,
            config.num_threads
        );

        if (!graph_success) {
            result.success = false;
            result.error_message = "Graph generation failed: " +
                                  graph_api.get_error_message();

            // Clean up temporary DIMACS file if created
            if (temp_dimacs && fs::exists(dimacs_file)) {
                fs::remove(dimacs_file);
            }

            return result;
        }

        // Update statistics if we didn't have them from UVL conversion
        if (input_type == InputType::DIMACS) {
            result.num_variables = graph_api.get_num_variables();
            result.num_clauses = graph_api.get_num_clauses();
        }

        // Get global backbone
        result.global_backbone = graph_api.get_global_backbone();

        // Separate core and dead features from backbone
        for (int lit : result.global_backbone) {
            if (lit > 0) {
                result.core_features.push_back(lit);
            } else {
                result.dead_features.push_back(-lit);  // Store as positive variable
            }
        }

        // Set output file paths
        result.requires_graph_file = output_dir + "/" + basename + "__requires.net";
        result.excludes_graph_file = output_dir + "/" + basename + "__excludes.net";
        result.core_features_file = output_dir + "/" + basename + "__core.txt";
        result.dead_features_file = output_dir + "/" + basename + "__dead.txt";

        if (verbose) {
            std::cout << "\nGraph generation successful!\n";
            std::cout << "  Variables: " << result.num_variables << "\n";
            std::cout << "  Clauses:   " << result.num_clauses << "\n";
            std::cout << "  Core features: " << result.core_features.size() << "\n";
            std::cout << "  Dead features: " << result.dead_features.size() << "\n";
            std::cout << "\nOutput files:\n";
            std::cout << "  " << result.requires_graph_file << "\n";
            std::cout << "  " << result.excludes_graph_file << "\n";
            std::cout << "  " << result.core_features_file << "\n";
            std::cout << "  " << result.dead_features_file << "\n";

            if (verbose) {
                std::cout << "\n=================================================\n";
                std::cout << "Analysis Complete!\n";
                std::cout << "=================================================\n";
            }
        }

        // Clean up temporary DIMACS file if needed
        if (temp_dimacs && fs::exists(dimacs_file)) {
            fs::remove(dimacs_file);
        }

        result.success = true;
        return result;
    }
};

// ============================================================================
// Strong4VMAPI Public Interface Implementation
// ============================================================================

Strong4VMAPI::Strong4VMAPI()
    : pimpl_(new Impl()) {}

Strong4VMAPI::~Strong4VMAPI() = default;

AnalysisResult Strong4VMAPI::analyze(
    const std::string& input_file,
    const std::string& output_dir
) {
    AnalysisConfig config;
    config.input_file = input_file;
    config.output_dir = output_dir;
    config.input_type = InputType::AUTO;
    config.conversion_mode = pimpl_->default_conversion_mode;
    config.keep_dimacs = false;
    config.detector = pimpl_->default_detector;
    config.num_threads = pimpl_->default_num_threads;
    config.verbose = pimpl_->verbose;

    return analyze(config);
}

AnalysisResult Strong4VMAPI::analyze(const AnalysisConfig& config) {
    AnalysisResult result = pimpl_->perform_analysis(config);
    pimpl_->last_result = result;
    return result;
}

void Strong4VMAPI::set_verbose(bool verbose) {
    pimpl_->verbose = verbose;
}

bool Strong4VMAPI::get_verbose() const {
    return pimpl_->verbose;
}

void Strong4VMAPI::set_default_conversion_mode(ConversionMode mode) {
    pimpl_->default_conversion_mode = mode;
}

ConversionMode Strong4VMAPI::get_default_conversion_mode() const {
    return pimpl_->default_conversion_mode;
}

void Strong4VMAPI::set_default_detector(BackboneDetector detector) {
    pimpl_->default_detector = detector;
}

BackboneDetector Strong4VMAPI::get_default_detector() const {
    return pimpl_->default_detector;
}

void Strong4VMAPI::set_default_threads(int num_threads) {
    pimpl_->default_num_threads = num_threads;
}

int Strong4VMAPI::get_default_threads() const {
    return pimpl_->default_num_threads;
}

std::string Strong4VMAPI::validate_config(const AnalysisConfig& config) const {
    return pimpl_->validate_configuration(config);
}

AnalysisResult Strong4VMAPI::get_last_result() const {
    return pimpl_->last_result;
}

} // namespace strong4vm
