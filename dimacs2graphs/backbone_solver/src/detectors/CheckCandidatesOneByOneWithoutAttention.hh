/**
 * @file CheckCandidatesOneByOneWithoutAttention.hh
 * @brief Backbone detection without activity bumping
 *
 * Variant of CheckCandidatesOneByOne that doesn't use activity bumping,
 * relying solely on the solver's default heuristics.
 */

#ifndef CHECKCANDIDATESONEBYONEWITHOUTATTENTION_HH
#define CHECKCANDIDATESONEBYONEWITHOUTATTENTION_HH
#include <functional>
#include <vector>

#include "BackBone.hh"
#include "DIMACSReader.hh"
#include "LiteralSet.hh"
#include "minisat_interface/MiniSatExt.hh"

namespace backbone_solver {
using Minisat::Lit;
using Minisat::MiniSatExt;
using Minisat::Var;
using Minisat::vec;

/**
 * @class CheckCandidatesOneByOneWithoutAttention
 * @ingroup BackboneDetectors
 * @brief Upper-bound backbone detector without activity bumping
 *
 * Similar to CheckCandidatesOneByOne but without activity bumping.
 * This variant relies entirely on the SAT solver's default variable
 * selection heuristics rather than guiding it toward specific assignments.
 *
 * Generally slower than the attention-based version since the solver
 * is not guided toward finding alternative assignments that contradict
 * backbone candidates.
 *
 * Useful for comparison and baseline measurements.
 */
class CheckCandidatesOneByOneWithoutAttention : public BackBone {
 public:
  /**
   * @brief Construct detector for a CNF formula
   * @param max_id Maximum variable ID
   * @param clauses CNF formula
   */
  CheckCandidatesOneByOneWithoutAttention(Var max_id, const CNF& clauses);

  /**
   * @brief Destructor
   */
  virtual ~CheckCandidatesOneByOneWithoutAttention() override;

  /**
   * @brief Initialize and check satisfiability
   * @return true if satisfiable
   */
  virtual bool initialize() override;

  /**
   * @brief Run backbone detection without activity bumping
   */
  virtual void run() override;

  /**
   * @brief Run backbone detection with assumptions
   * @param assumptions Literals that must be satisfied
   * @return true if satisfiable under assumptions
   */
  virtual bool run_with_assumptions(const vec<Lit>& assumptions) override;

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
  const Var max_id;        ///< Maximum variable ID
  const CNF& clauses;      ///< CNF formula
  LiteralSet candidates;   ///< Candidate backbone literals
  LiteralSet backbone;     ///< Confirmed backbone literals
  vec<Lit> discarded_candidates;  ///< Recently discarded candidates
  vec<Lit> persistent_assumptions;  ///< Assumptions maintained across solve calls
  MiniSatExt solver;       ///< SAT solver
  const_infinite_LiteralSetIterator candidates_iterator;  ///< Iterates through candidates
};

}  // end of namespace backbone_solver

#endif  // CHECKCANDIDATESONEBYONEWITHOUTATTENTION_HH
