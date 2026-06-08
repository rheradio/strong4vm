/**
 * @file FastOnCliffsSlowOnPlains.hh
 * @brief Adaptive backbone detection with flatland detection
 *
 * Advanced algorithm that adapts its strategy based on progress.
 * Fast elimination when making good progress ("cliffs"), switches to
 * verification when progress slows ("flatlands" or "plains").
 */

#ifndef FASTONCLIFFSSLOWONPLAINS_HH
#define FASTONCLIFFSSLOWONPLAINS_HH
#include <functional>
#include <vector>

#include "BackBone.hh"
#include "DIMACSReader.hh"
#include "LiteralSet.hh"
#include "minisat_interface/MiniSatExt.hh"

/**
 * @class FlatlandTester
 * @brief Detects when backbone discovery progress has plateaued
 *
 * Monitors the rate of progress in backbone detection and determines
 * when the algorithm has entered a "flatland" (plateau) where fast
 * elimination is no longer effective.
 *
 * Uses relative and absolute tolerance checks with consecutive
 * measurement requirements to avoid false positives from noise.
 */
class FlatlandTester {
 private:
  double previous_value;            ///< Previous measurement
  double relative_tolerance;        ///< Relative change threshold
  double absolute_tolerance;        ///< Absolute change threshold
  int consecutive_stable_count;     ///< Count of consecutive stable measurements
  int required_consecutive_checks;  ///< Required consecutive stable checks
  bool initialized;                 ///< Whether first value has been recorded

 public:
  /**
   * @brief Construct flatland tester with specified tolerances
   *
   * @param relative_tolerance Relative change threshold (default 1e-4)
   * @param consecutive_checks Required consecutive stable measurements (default 3)
   * @param absolute_tolerance Absolute change threshold (default 1e-6)
   */
  FlatlandTester(double relative_tolerance = 1e-4, int consecutive_checks = 3,
                 double absolute_tolerance = 1e-6);

  /**
   * @brief Test if current value indicates flatland
   *
   * Checks if the current value is sufficiently close to previous value
   * for the required number of consecutive checks.
   *
   * @param current_value New measurement value
   * @return true if flatland detected (progress has plateaued)
   * @return false if still making progress
   */
  bool is_flatland(double current_value);
};

namespace bonedigger {
using Minisat::Lit;
using Minisat::MiniSatExt;
using Minisat::Var;
using Minisat::vec;

/**
 * @class FastOnCliffsSlowOnPlains
 * @ingroup BackboneDetectors
 * @brief Adaptive backbone detector with cliff/plain strategy
 *
 * Advanced algorithm that uses two phases:
 *
 * 1. **Cliff phase** (fast elimination):
 *    - Uses activity bumping like CheckCandidatesOneByOne
 *    - Quickly eliminates non-backbone candidates
 *    - Effective when making good progress
 *
 * 2. **Plain phase** (verification):
 *    - Activated when FlatlandTester detects progress plateau
 *    - Uses relaxation clauses to verify remaining candidates
 *    - More thorough but slower
 *
 * This adaptive strategy combines speed and completeness:
 * - Fast when easy candidates can be eliminated
 * - Thorough when dealing with difficult remaining candidates
 *
 * Generally the most robust algorithm across different instance types.
 */
class FastOnCliffsSlowOnPlains : public BackBone {
 public:
  /**
   * @brief Construct detector for a CNF formula
   * @param max_id Maximum variable ID
   * @param clauses CNF formula
   * @param attention_weight Weight for detector activity in variable ordering (default 1.0)
   */
  FastOnCliffsSlowOnPlains(Var max_id, const CNF& clauses, double attention_weight = 1.0);

  /**
   * @brief Destructor
   */
  virtual ~FastOnCliffsSlowOnPlains() override;

  /**
   * @brief Initialize and check satisfiability
   * @return true if satisfiable
   */
  virtual bool initialize() override;

  /**
   * @brief Run adaptive backbone detection
   *
   * Starts with fast cliff phase, switches to plain phase when
   * flatland is detected.
   */
  virtual void run() override;

  /**
   * @brief Check if literal is in backbone
   * @param literal Literal to check
   * @return true if in backbone
   */
  virtual bool is_backbone(const Lit& literal) const override;

  /**
   * @brief Check if variable is in backbone
   * @param var Variable to check
   * @return true if in backbone
   */
  virtual bool is_backbone(Var var) const override;

  /**
   * @brief Get sign of backbone variable
   * @param var Backbone variable
   * @return true if positive, false if negative
   */
  virtual bool backbone_sign(Var var) const override;

 private:
  const Var max_id;                   ///< Maximum variable ID
  const CNF& clauses;                 ///< CNF formula
  const double attention_weight;      ///< Weight for detector activity
  LiteralSet candidates;              ///< Candidate backbone literals
  LiteralSet backbone;                ///< Confirmed backbone literals
  vec<Lit> discarded_candidates;      ///< Recently discarded candidates
  MiniSatExt solver;                  ///< SAT solver
  FlatlandTester flatland_tester;     ///< Detects progress plateaus
  const_infinite_LiteralSetIterator candidates_iterator;  ///< Iterates through candidates

  /**
   * @brief Bump activities and solve with assumptions
   * @param assumptions Literals to assume
   * @return true if satisfiable
   */
  bool bump_and_solve(const vec<Lit>& assumptions);

  /**
   * @brief Bump activities and solve without assumptions
   * @return true if satisfiable
   */
  bool bump_and_solve();

  /**
   * @brief Discard candidates contradicted by current model
   */
  void discard_candidates();
};

}  // end of namespace bonedigger

#endif  // FASTONCLIFFSSLOWONPLAINS_HH
