/**
 * @file BackBone.hh
 * @brief Abstract base class for backbone detection algorithms
 *
 * Defines the interface that all backbone detection algorithms must implement.
 */

#ifndef BACKBONE_HH
#define BACKBONE_HH

#include "minisat_interface/MiniSatExt.hh"

/**
 * @defgroup BackboneDetectors Backbone Detection Algorithms
 * @brief Algorithms for computing the backbone of Boolean formulas
 *
 * ## Overview
 *
 * The backbone of a Boolean formula is the set of literals that must have the
 * same value in all satisfying assignments. BoneDigger provides several algorithms
 * for computing backbones, each with different performance characteristics.
 *
 * ## Available Detectors
 *
 * BoneDigger includes four backbone detection algorithms:
 *
 * ### 1. RushAndPray (Default)
 * - **Class:** bonedigger::RushAndPray
 * - **Strategy:** Rush-and-verify approach with iterative refinement
 * - **Best for:** General-purpose backbone detection with good performance on most formulas
 * - **Description:** Starts with literals from an initial solution, uses activity bumping
 *   to find alternative solutions, and iteratively refines candidates until convergence
 *
 * ### 2. CheckCandidatesOneByOne
 * - **Class:** bonedigger::CheckCandidatesOneByOne
 * - **Strategy:** Upper-bound iterative elimination with activity bumping
 * - **Best for:** Formulas where solver guidance significantly improves performance
 * - **Description:** Systematically checks each candidate using activity bumping to
 *   guide the SAT solver toward relevant alternative solutions
 *
 * ### 3. FastOnCliffsSlowOnPlains
 * - **Class:** bonedigger::FastOnCliffsSlowOnPlains
 * - **Strategy:** Adaptive strategy with cliff/plain detection
 * - **Best for:** Formulas with varying difficulty regions (rapid progress followed by plateaus)
 * - **Description:** Automatically switches between rapid elimination (cliffs) and
 *   careful verification (plains) based on convergence rate
 *
 * ## Implementing a New Detector
 *
 * To add a new backbone detection algorithm, create a class that inherits from
 * bonedigger::BackBone and implements all pure virtual methods:
 *
 * @code
 * #include "BackBone.hh"
 *
 * namespace bonedigger {
 *
 * class MyDetector : public BackBone {
 * public:
 *     MyDetector(Var max_id, const CNF& clauses);
 *     ~MyDetector();
 *
 *     // Required: Check satisfiability and initialize data structures
 *     bool initialize() override;
 *
 *     // Required: Execute the backbone detection algorithm
 *     void run() override;
 *
 *     // Required: Query if a literal is in the backbone
 *     bool is_backbone(const Lit& literal) const override;
 *
 *     // Required: Query if a variable is in the backbone
 *     bool is_backbone(Var var) const override;
 *
 *     // Required: Get the sign of a backbone variable
 *     bool backbone_sign(Var var) const override;
 *
 * private:
 *     MiniSatExt solver;
 *     LiteralSet backbone;
 *     // ... other data structures
 * };
 *
 * } // namespace bonedigger
 * @endcode
 *
 * ### Implementation Guidelines
 *
 * 1. **initialize()**: Must check satisfiability using a SAT solver and return the result.
 *    If satisfiable, set up initial data structures for backbone detection.
 *
 * 2. **run()**: Should only be called after initialize() returns true. This method
 *    performs the actual backbone computation. After completion, is_backbone() queries
 *    must return correct results.
 *
 * 3. **is_backbone() methods**: Should return true only for literals/variables that
 *    are proven to be in the backbone (must have the same value in ALL satisfying
 *    assignments).
 *
 * 4. **backbone_sign()**: Should only be called for variables known to be in the
 *    backbone. Returns true if the variable must be positive, false if negative.
 *
 * ### Common Patterns
 *
 * Most detectors follow this pattern:
 * - Use a MiniSatExt solver for SAT queries
 * - Store backbone literals in a LiteralSet for efficient membership testing
 * - Use activity bumping (solver.bump()) to guide solver toward relevant solutions
 * - Maintain a set of candidate literals and iteratively eliminate non-backbone candidates
 *
 * ### Integration
 *
 * After implementing your detector:
 * 1. Add it to the factory function in cli/main.cc
 * 2. Add it to BoneDiggerAPI.cc for API access
 * 3. Update documentation and examples
 * 4. Add command-line option for your detector
 */

namespace bonedigger {
using Minisat::Lit;
using Minisat::Var;

/**
 * @class BackBone
 * @ingroup BackboneDetectors
 * @brief Abstract base class for backbone detection algorithms
 *
 * This class defines the interface that all backbone detection algorithms
 * must implement. A backbone is the set of literals that must have the same
 * value in all satisfying assignments of a Boolean formula.
 *
 * All derived classes must implement:
 * - initialize(): Check satisfiability and set up data structures
 * - run(): Execute the backbone detection algorithm
 * - is_backbone(): Query whether a literal/variable is in the backbone
 * - backbone_sign(): Get the sign of a backbone variable
 *
 * Typical usage pattern:
 * @code
 * BackBone* detector = new ConcreteDetector(...);
 * if (detector->initialize()) {
 *     detector->run();
 *     // Query results with is_backbone() and backbone_sign()
 * }
 * @endcode
 */
class BackBone {
 public:
  /**
   * @brief Virtual destructor for proper cleanup of derived classes
   */
  virtual ~BackBone() {}

  /**
   * @brief Initialize the detector and check satisfiability
   *
   * This method must:
   * 1. Check if the formula is satisfiable
   * 2. Set up initial data structures for backbone detection
   * 3. Return the SAT status
   *
   * Must be called before run().
   *
   * @return true if the instance is satisfiable
   * @return false if the instance is unsatisfiable
   */
  virtual bool initialize() = 0;

  /**
   * @brief Run the backbone detection algorithm
   *
   * Execute the main backbone detection algorithm.
   * This method assumes initialize() has been called and returned true.
   * After this method completes, is_backbone() queries will return
   * correct results.
   */
  virtual void run() = 0;

  /**
   * @brief Check if a literal is in the backbone
   *
   * @param literal The literal to check
   * @return true if the literal must appear in all satisfying assignments
   * @return false otherwise
   */
  virtual bool is_backbone(const Lit& literal) const = 0;

  /**
   * @brief Check if a variable is in the backbone
   *
   * A variable is in the backbone if either its positive or negative
   * literal is in the backbone.
   *
   * @param var The variable to check
   * @return true if the variable has a fixed value in all satisfying assignments
   * @return false otherwise
   */
  virtual bool is_backbone(Var var) const = 0;

  /**
   * @brief Get the sign of a backbone variable
   *
   * This method should only be called for variables that are in the backbone.
   *
   * @param var The backbone variable
   * @return true if the variable must be positive/true in all satisfying assignments
   * @return false if the variable must be negative/false in all satisfying assignments
   */
  virtual bool backbone_sign(Var var) const = 0;
};

}  // end of namespace bonedigger

#endif  // BACKBONE_HH
