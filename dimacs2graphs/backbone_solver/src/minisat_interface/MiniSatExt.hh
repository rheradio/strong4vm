/**
 * @file MiniSatExt.hh
 * @brief Extended MiniSat solver with additional methods
 *
 * Provides extensions to the standard MiniSat solver interface,
 * particularly for activity bumping which is crucial for backbone detection.
 */

#ifndef MINISATEXT_HH
#define MINISATEXT_HH
#include "minisat/core/Solver.h"

namespace Minisat {

/**
 * @class MiniSatExt
 * @brief Extended MiniSat solver with activity manipulation methods
 *
 * This class extends the standard MiniSat Solver to expose additional
 * functionality needed for backbone detection algorithms. The main addition
 * is the ability to manually bump variable activities, which guides the
 * solver toward specific variable assignments.
 *
 * Activity bumping is a key technique in backbone detection: by bumping
 * the activity of candidate backbone literals, we guide the solver to
 * prefer alternative assignments that can disprove non-backbone candidates.
 */
class MiniSatExt : public Solver {
 public:
  /**
   * @brief Bump the activity of a variable
   *
   * Increases the solver's internal activity score for a variable,
   * making it more likely to be selected for branching. This is used
   * in backbone detection to guide the solver toward assignments that
   * contradict candidate backbone literals.
   *
   * @param var The variable whose activity should be increased
   */
  inline void bump(const Var var) { varBumpActivity(var); }

  /**
   * @brief Reset the activity of a variable to zero
   *
   * Sets the solver's internal activity score for a variable back to zero.
   *
   * @param v The variable whose activity should be reset
   */
  inline void reset_activity_for_var(Var v) { activity[v] = 0.0; }
};

}  // namespace Minisat

#endif  // MINISATEXT_HH
