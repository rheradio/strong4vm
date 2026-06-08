#ifndef BACKBONE_SIMPLIFIER_HH
#define BACKBONE_SIMPLIFIER_HH

#include <string>
#include <vector>
#include <unordered_map>

namespace uvl2dimacs {

/**
 * Backbone state for each variable
 */
enum class BackboneState {
    NONE,       // Variable not in backbone
    TRUE,       // Variable must be true
    FALSE       // Variable must be false
};

/**
 * BackboneSimplifier - Simplifies a DIMACS CNF formula using its backbone
 *
 * The backbone of a formula consists of literals that must be true/false in all
 * satisfying assignments. Using the backbone, we can simplify the formula by:
 * 1. Removing clauses that contain a backbone literal (they are satisfied)
 * 2. Removing negated backbone literals from clauses (they are false)
 */
class BackboneSimplifier {
public:
    BackboneSimplifier();
    ~BackboneSimplifier();

    /**
     * Simplify a DIMACS file using its backbone
     *
     * @param input_dimacs_path Path to input DIMACS file
     * @param output_dimacs_path Path to output simplified DIMACS file
     * @param backbone_solver_path Path to backbone_solver executable
     * @param verbose Enable verbose output
     * @return true if simplification succeeded, false otherwise
     */
    bool simplify(const std::string& input_dimacs_path,
                  const std::string& output_dimacs_path,
                  const std::string& backbone_solver_path,
                  bool verbose = false);

    // Statistics
    int get_backbone_size() const { return backbone_size_; }
    int get_removed_clauses() const { return removed_clauses_; }
    int get_shortened_clauses() const { return shortened_clauses_; }

private:
    /**
     * Execute backbone_solver and parse the output
     *
     * @param dimacs_path Path to DIMACS file
     * @param backbone_solver_path Path to backbone_solver executable
     * @param num_vars Number of variables in the formula
     * @return true if successful, false otherwise
     */
    bool compute_backbone(const std::string& dimacs_path,
                         const std::string& backbone_solver_path,
                         int num_vars);

    /**
     * Parse backbone output from backbone_solver
     *
     * @param output Raw output from backbone_solver
     * @param num_vars Number of variables
     * @return true if parsing succeeded
     */
    bool parse_backbone_output(const std::string& output, int num_vars);

    /**
     * Simplify clauses using the computed backbone
     *
     * @param input_dimacs_path Input DIMACS file
     * @param output_dimacs_path Output DIMACS file
     * @return true if successful
     */
    bool simplify_clauses(const std::string& input_dimacs_path,
                         const std::string& output_dimacs_path);

    /**
     * Process a single clause using the backbone
     * Returns true if clause should be kept, false if it should be removed
     */
    bool process_clause(std::vector<int>& clause);

    // Backbone storage: backbone_[var_id] = state
    std::vector<BackboneState> backbone_;

    // Statistics
    int backbone_size_;
    int removed_clauses_;
    int shortened_clauses_;
    int num_vars_;
    int num_clauses_;

    bool verbose_;
};

} // namespace uvl2dimacs

#endif // BACKBONE_SIMPLIFIER_HH
