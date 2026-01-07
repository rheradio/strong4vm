/**
 * @file CheckCandidatesOneByOne.hh
 * @brief Fast upper-bound backbone detection algorithm
 *
 * Implements a backbone detection algorithm based on iterative candidate
 * elimination using activity bumping to guide the SAT solver.
 */

#ifndef CHECKCANDIDATESONEBYONE_HH
#define CHECKCANDIDATESONEBYONE_HH
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
 * @class CheckCandidatesOneByOne
 * @ingroup BackboneDetectors
 * @brief Fast backbone detector using upper-bound iterative elimination
 *
 * This algorithm efficiently computes backbones by:
 * 1. Starting with all literals from an initial satisfying assignment as candidates
 * 2. Using activity bumping to guide the solver toward alternative solutions
 * 3. Discarding candidates that appear with different values in new solutions
 * 4. Repeating until no more candidates can be discarded
 *
 * The algorithm is called "upper-bound" because it only discovers backbones
 * that appear in the initial solution. It may miss backbone literals that
 * don't appear in the first satisfying assignment found.
 *
 * Key technique: By bumping the activity of candidate literals, we encourage
 * the solver to try to flip them. If successful, we know they're not backbone.
 *
 * This is typically the fastest backbone detection algorithm for most instances.
 */
class CheckCandidatesOneByOne : public BackBone {
 public:
  /**
   * @brief Construct detector for a CNF formula
   *
   * @param max_id Maximum variable ID in the formula
   * @param clauses Vector of clauses (CNF formula)
   */
  CheckCandidatesOneByOne(Var max_id, const CNF& clauses);

  /**
   * @brief Destructor
   */
  virtual ~CheckCandidatesOneByOne() override;

  /**
   * @brief Initialize detector and check satisfiability
   *
   * Finds an initial satisfying assignment and marks all its literals
   * as backbone candidates.
   *
   * @return true if formula is satisfiable
   * @return false if formula is unsatisfiable
   */
  virtual bool initialize() override;

  /**
   * @brief Run the backbone detection algorithm
   *
   * Iteratively eliminates non-backbone candidates by finding alternative
   * satisfying assignments. Uses activity bumping to guide the search.
   */
  virtual void run() override;

  /**
   * @brief Run backbone detection with assumptions
   *
   * Computes the backbone under given assumptions. Similar to initialize() + run(),
   * but applies assumptions in all SAT solver calls.
   *
   * @param assumptions Literals that must be satisfied
   * @return true if satisfiable under assumptions, false otherwise
   */
  virtual bool run_with_assumptions(const vec<Lit>& assumptions) override;

  /**
   * @brief Check if a literal is in the backbone
   *
   * @param literal The literal to check
   * @return true if literal is confirmed to be in the backbone
   * @return false otherwise
   */
  virtual bool is_backbone(const Lit& literal) const override;

  /**
   * @brief Check if a variable is in the backbone
   *
   * @param var The variable to check
   * @return true if variable is in the backbone
   * @return false otherwise
   */
  virtual bool is_backbone(Var var) const override;

  /**
   * @brief Get the sign of a backbone variable
   *
   * @param var The backbone variable
   * @return true if variable must be positive
   * @return false if variable must be negative
   */
  virtual bool backbone_sign(Var var) const override;

 private:
  // Formula data
  const Var max_id;        ///< Maximum variable ID
  const CNF& clauses;      ///< CNF formula to analyze

  // Algorithm state
  LiteralSet candidates;              ///< Literals that might be backbones
  LiteralSet backbone;                ///< Confirmed backbone literals
  vec<Lit> discarded_candidates;      ///< Recently discarded candidates
  vec<Lit> persistent_assumptions;    ///< Assumptions maintained across solve calls

  // SAT solver
  MiniSatExt solver;  ///< Extended MiniSat solver with activity bumping

  // Iteration state
  const_infinite_LiteralSetIterator candidates_iterator;  ///< Cycles through candidates

  /**
   * @brief Bump candidate activities and solve with assumptions
   *
   * Increases solver activity for candidate literals to encourage
   * the solver to try flipping them, then solves with given assumptions.
   *
   * @param assumptions Literals to assume during solving
   * @return true if satisfiable under assumptions
   * @return false if unsatisfiable
   */
  bool bump_and_solve(const vec<Lit>& assumptions);

  /**
   * @brief Try to discard a single candidate by finding alternative assignment
   */
  void discard_one_candidate();

  /**
   * @brief Discard all candidates contradicted by current solver model
   *
   * After finding a new solution, this method identifies and removes
   * all candidates that have different values in the new solution.
   */
  void discard_candidates();
};

}  // end of namespace backbone_solver

#endif  // CHECKCANDIDATESONEBYONE_HH
