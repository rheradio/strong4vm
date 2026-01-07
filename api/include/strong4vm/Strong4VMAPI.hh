/**
 * @file Strong4VMAPI.hh
 * @brief High-level C++ API for Strong4VM - Feature Model to Strong Transitive Graphs
 *
 * This header provides a simple interface for the complete Strong4VM pipeline:
 * 1. Convert UVL feature models to DIMACS CNF (or accept existing DIMACS)
 * 2. Generate strong transitive dependency and conflict graphs
 * 3. Extract core and dead features
 */

#ifndef STRONG4VM_API_HH
#define STRONG4VM_API_HH

#include <string>
#include <vector>
#include <memory>

namespace strong4vm {

/**
 * @brief Input file type for Strong4VM
 */
enum class InputType {
    UVL,        ///< Universal Variability Language feature model
    DIMACS,     ///< DIMACS CNF formula
    AUTO        ///< Automatically detect based on file extension (default)
};

/**
 * @brief CNF conversion mode (for UVL input)
 */
enum class ConversionMode {
    STRAIGHTFORWARD,  ///< Direct conversion without auxiliary variables (default)
    TSEITIN           ///< Tseitin transformation with auxiliary variables
};

/**
 * @brief Backbone detector algorithm
 */
enum class BackboneDetector {
    ONE,        ///< CheckCandidatesOneByOne with activity bumping (default, recommended)
    WITHOUT     ///< CheckCandidatesOneByOneWithoutAttention (baseline)
};

/**
 * @brief Configuration options for Strong4VM analysis
 */
struct AnalysisConfig {
    // Input/Output settings
    std::string input_file;           ///< Path to input file (.uvl or .dimacs)
    std::string output_dir;           ///< Output directory (default: same as input file)
    InputType input_type;             ///< Input file type (default: AUTO)

    // UVL conversion settings (only used for UVL input)
    ConversionMode conversion_mode;   ///< CNF conversion mode (default: STRAIGHTFORWARD)
    bool keep_dimacs;                 ///< Keep intermediate DIMACS file (default: false)

    // Graph generation settings
    BackboneDetector detector;        ///< Backbone detector algorithm (default: ONE)
    int num_threads;                  ///< Number of threads for parallel processing (default: 1)

    // Verbosity
    bool verbose;                     ///< Print progress messages (default: false)

    /**
     * @brief Default constructor with sensible defaults
     */
    AnalysisConfig()
        : input_file("")
        , output_dir("")
        , input_type(InputType::AUTO)
        , conversion_mode(ConversionMode::STRAIGHTFORWARD)
        , keep_dimacs(false)
        , detector(BackboneDetector::ONE)
        , num_threads(1)
        , verbose(false) {}
};

/**
 * @brief Result of a Strong4VM analysis
 */
struct AnalysisResult {
    bool success;                     ///< Whether the analysis was successful
    std::string error_message;        ///< Error message if analysis failed

    // Input file information
    InputType input_type;             ///< Type of input file processed
    std::string input_file;           ///< Path to input file

    // Feature model statistics (only for UVL input)
    int num_features;                 ///< Number of features in the model
    int num_relations;                ///< Number of parent-child relations
    int num_constraints;              ///< Number of cross-tree constraints

    // CNF formula statistics
    int num_variables;                ///< Number of variables in the CNF
    int num_clauses;                  ///< Number of clauses in the CNF

    // Graph analysis results
    std::vector<int> global_backbone; ///< Global backbone literals
    std::vector<int> core_features;   ///< Core features (always selected)
    std::vector<int> dead_features;   ///< Dead features (never selected)

    // Output files
    std::string requires_graph_file;  ///< Path to requires graph (.net)
    std::string excludes_graph_file;  ///< Path to excludes graph (.net)
    std::string core_features_file;   ///< Path to core features (.txt)
    std::string dead_features_file;   ///< Path to dead features (.txt)
    std::string dimacs_file;          ///< Path to DIMACS file (if kept)

    /**
     * @brief Default constructor for failed analysis
     */
    AnalysisResult()
        : success(false)
        , error_message("")
        , input_type(InputType::AUTO)
        , input_file("")
        , num_features(0)
        , num_relations(0)
        , num_constraints(0)
        , num_variables(0)
        , num_clauses(0)
        , requires_graph_file("")
        , excludes_graph_file("")
        , core_features_file("")
        , dead_features_file("")
        , dimacs_file("") {}
};

/**
 * @brief Main API class for Strong4VM analysis
 *
 * This class provides a high-level interface for analyzing feature models
 * and generating strong transitive dependency and conflict graphs.
 *
 * Example usage:
 * @code
 * strong4vm::Strong4VMAPI api;
 *
 * // Simple analysis with defaults
 * auto result = api.analyze("model.uvl");
 * if (result.success) {
 *     std::cout << "Analysis successful!\n";
 *     std::cout << "Core features: " << result.core_features.size() << "\n";
 *     std::cout << "Dead features: " << result.dead_features.size() << "\n";
 * }
 *
 * // Advanced analysis with custom configuration
 * strong4vm::AnalysisConfig config;
 * config.input_file = "model.uvl";
 * config.output_dir = "./output";
 * config.conversion_mode = strong4vm::ConversionMode::TSEITIN;
 * config.num_threads = 4;
 * config.keep_dimacs = true;
 * config.verbose = true;
 *
 * auto result2 = api.analyze(config);
 * @endcode
 */
class Strong4VMAPI {
public:
    /**
     * @brief Constructor
     */
    Strong4VMAPI();

    /**
     * @brief Destructor
     */
    ~Strong4VMAPI();

    /**
     * @brief Analyze a feature model or CNF formula (simple interface)
     *
     * Uses default configuration with auto-detection of file type.
     *
     * @param input_file Path to input file (.uvl or .dimacs)
     * @param output_dir Optional output directory (default: same as input file)
     * @return AnalysisResult with success status, statistics, and output file paths
     */
    AnalysisResult analyze(
        const std::string& input_file,
        const std::string& output_dir = ""
    );

    /**
     * @brief Analyze a feature model or CNF formula (full configuration)
     *
     * Provides complete control over all analysis parameters.
     *
     * @param config Analysis configuration with all options
     * @return AnalysisResult with success status, statistics, and output file paths
     */
    AnalysisResult analyze(const AnalysisConfig& config);

    /**
     * @brief Set verbose output mode
     * @param verbose If true, print progress messages during analysis
     */
    void set_verbose(bool verbose);

    /**
     * @brief Get verbose output mode
     * @return Current verbose setting
     */
    bool get_verbose() const;

    /**
     * @brief Set default conversion mode for UVL input
     * @param mode Default CNF conversion mode
     */
    void set_default_conversion_mode(ConversionMode mode);

    /**
     * @brief Get default conversion mode
     * @return Current default conversion mode
     */
    ConversionMode get_default_conversion_mode() const;

    /**
     * @brief Set default backbone detector
     * @param detector Default backbone detector algorithm
     */
    void set_default_detector(BackboneDetector detector);

    /**
     * @brief Get default backbone detector
     * @return Current default detector
     */
    BackboneDetector get_default_detector() const;

    /**
     * @brief Set default number of threads
     * @param num_threads Default thread count for parallel processing
     */
    void set_default_threads(int num_threads);

    /**
     * @brief Get default number of threads
     * @return Current default thread count
     */
    int get_default_threads() const;

    /**
     * @brief Validate configuration before analysis
     * @param config Configuration to validate
     * @return Error message if invalid, empty string if valid
     */
    std::string validate_config(const AnalysisConfig& config) const;

    /**
     * @brief Get the last analysis result
     * @return Last AnalysisResult (useful for debugging)
     */
    AnalysisResult get_last_result() const;

private:
    class Impl;
    std::unique_ptr<Impl> pimpl_;  ///< Pointer to implementation (PIMPL idiom)
};

} // namespace strong4vm

#endif // STRONG4VM_API_HH
