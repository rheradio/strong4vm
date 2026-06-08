/**
 * @file RushAndPray.hh
 * @brief Experimental backbone detection algorithm
 *
 * Combines fast elimination with verification phase.
 * Named "rush and pray" because it rushes through elimination
 * and then verifies remaining candidates.
 */

#ifndef RUSHANDPRAY_HH
#define RUSHANDPRAY_HH
#include <functional>
#include <vector>

#include "BackBone.hh"
#include "DIMACSReader.hh"
#include "LiteralSet.hh"
#include "minisat_interface/MiniSatExt.hh"

namespace bonedigger {
using Minisat::Lit;
using Minisat::MiniSatExt;
using Minisat::Var;
using Minisat::vec;

/**
 * @class RushAndPray
 * @ingroup BackboneDetectors
 * @brief Experimental backbone detector with rush and verify strategy
 *
 * This algorithm uses a two-phase approach:
 *
 * 1. **Rush phase**:
 *    - Quickly eliminates easy non-backbone candidates using activity bumping
 *    - Similar to CheckCandidatesOneByOne but may use different stopping criteria
 *
 * 2. **Verify phase**:
 *    - Explicitly verifies remaining candidates
 *    - Uses additional SAT calls to confirm or refute candidates
 *
 * The "pray" part refers to hoping that the rush phase eliminates most
 * non-backbone candidates, leaving only a small number to verify.
 *
 * This is an experimental algorithm that may work well on certain
 * instance types but is generally less reliable than FastOnCliffsSlowOnPlains.
 */
class RushAndPray : public BackBone {
 public:
  /**
   * @brief Construct detector for a CNF formula
   * @param max_id Maximum variable ID
   * @param clauses CNF formula
   * @param attention_weight Weight for detector activity in variable ordering (default 1.0)
   */
  RushAndPray(Var max_id, const CNF& clauses, double attention_weight = 1.0);

  /**
   * @brief Destructor
   */
  virtual ~RushAndPray() override;

  /**
   * @brief Initialize and check satisfiability
   * @return true if satisfiable
   */
  virtual bool initialize() override;

  /**
   * @brief Run rush-and-verify backbone detection
   *
   * Performs quick elimination followed by explicit verification
   * of remaining candidates.
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

  /**
   * @brief Explicitly verify all remaining candidates
   *
   * Tests each remaining candidate to confirm it's truly in the backbone.
   * This is the "pray" phase where we verify what remains after the rush.
   */
  void verify_candidates();
};

}  // end of namespace bonedigger

#endif  // RUSHANDPRAY_HH
