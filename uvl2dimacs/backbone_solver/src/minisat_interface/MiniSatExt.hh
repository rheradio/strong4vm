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
   * @brief Bump the detector activity of a variable
   *
   * Increases the detector's activity score for a variable,
   * making it more likely to be selected for branching. This is used
   * in backbone detection to guide the solver toward assignments that
   * contradict candidate backbone literals.
   *
   * The detector activity is kept separate from the VSIDS activity
   * and combined during variable ordering using the detector weight.
   *
   * @param var The variable whose detector activity should be increased
   */
  inline void bump(const Var var) { detectorBumpActivity(var); }

  /**
   * @brief Reset the detector activity of a variable to zero
   *
   * Sets the detector's activity score for a variable back to zero.
   *
   * @param v The variable whose detector activity should be reset
   */
  inline void reset_activity_for_var(Var v) { resetDetectorActivity(v); }

  /**
   * @brief Set the weight for combining detector activity with VSIDS activity
   *
   * The combined activity used for variable ordering is:
   *   vsids_activity + weight * detector_activity
   *
   * @param w The weight (must be >= 0, default is 1.0)
   */
  inline void set_detector_weight(double w) { setDetectorWeight(w); }

  /**
   * @brief Set the detector polarity for a variable
   *
   * Detector polarity takes precedence over user polarity when choosing
   * the branch direction for a variable.
   *
   * @param v The variable
   * @param b The desired polarity (l_True for positive, l_False for negative)
   */
  inline void set_detector_polarity(Var v, lbool b) { setDetectorPolarity(v, b); }

  /**
   * @brief Reset the detector polarity for a variable
   *
   * Clears the detector polarity so that user polarity or default heuristics
   * are used instead.
   *
   * @param v The variable whose detector polarity should be reset
   */
  inline void reset_detector_polarity(Var v) { setDetectorPolarity(v, l_Undef); }
};

}  // namespace Minisat

#endif  // MINISATEXT_HH
