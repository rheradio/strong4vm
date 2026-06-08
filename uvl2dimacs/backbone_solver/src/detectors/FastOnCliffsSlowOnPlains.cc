/**
 * @file FastOnCliffsSlowOnPlains.cc
 * @brief Implementation file for FastOnCliffsSlowOnPlains
 */

#include <cmath>
#include <iostream>
#include <stdexcept>

#include "FastOnCliffsSlowOnPlains.hh"
using namespace bonedigger;
using Minisat::lit_Undef;
using std::cout;
using std::endl;

// FlatlandTester parameters
[[maybe_unused]] const double RELATIVE_TOLERANCE = 1e-6;
[[maybe_unused]] const double CONSECUTIVE_CHECKS = 4;
[[maybe_unused]] const double ABSOLUTE_TOLERANCE = 1e-7;

// ============================================================================
// FlatlandTester Implementation
// ============================================================================

FlatlandTester::FlatlandTester(double relative_tolerance,
                               int consecutive_checks,
                               double absolute_tolerance)
    : previous_value(0.0),
      relative_tolerance(relative_tolerance),
      absolute_tolerance(absolute_tolerance),
      consecutive_stable_count(0),
      required_consecutive_checks(consecutive_checks),
      initialized(false) {
  if (relative_tolerance <= 0.0 || absolute_tolerance <= 0.0) {
    throw std::invalid_argument("Tolerances must be positive");
  }
  if (consecutive_checks < 1) {
    throw std::invalid_argument("Consecutive checks must be at least 1");
  }
}

bool FlatlandTester::is_flatland(double current_value) {
  // First iteration: just store the value
  if (!initialized) {
    previous_value = current_value;
    initialized = true;
    return false;
  }

  // Calculate changes
  double absolute_change = std::abs(current_value - previous_value);
  double relative_change = absolute_change / std::abs(previous_value);

  // Check if stable (either relative or absolute criterion met)
  bool is_stable = (relative_change < relative_tolerance) ||
                   (absolute_change < absolute_tolerance);

  if (is_stable) {
    consecutive_stable_count++;
  } else {
    consecutive_stable_count = 0;
  }

  previous_value = current_value;

  return consecutive_stable_count >= required_consecutive_checks;
}

// ============================================================================
// FastOnCliffsSlowOnPlains Implementation
// ============================================================================

// Initialization
FastOnCliffsSlowOnPlains::FastOnCliffsSlowOnPlains(Var _max_id,
                                                   const CNF& _clauses,
                                                   double _attention_weight)
    : max_id(_max_id),
      clauses(_clauses),
      attention_weight(_attention_weight),
      candidates_iterator(candidates.infinite_iterator()) {}

FastOnCliffsSlowOnPlains::~FastOnCliffsSlowOnPlains() {}

bool FastOnCliffsSlowOnPlains::initialize() {
  // Initialize the solver
  solver.set_detector_weight(attention_weight);
  for (Var i = 0; i <= max_id; ++i) {
    solver.newVar();
  }
  vec<Lit> ls;
  for (auto ci = clauses.begin(); ci != clauses.end(); ++ci) {
    ls.clear();
    for (auto li = (*ci).begin(); li != (*ci).end(); ++li) {
      const Lit l = *li;
      assert(var(l) <= max_id);
      ls.push(l);
    }
    solver.addClause(ls);
  }

  // Get the first solution of the formula
  const bool is_sat = solver.solve();

  if (!is_sat) {
    // Finish if the formula is unsatisfiable
    return false;
  }

  // Initialize the candidates set with the literals of the first solution
  const vec<lbool>& solution = solver.model;

  for (Var variable = 1; variable <= max_id; ++variable) {
    const lbool value = solution[variable];
    if (value != l_Undef) {
      const Lit l = mkLit(variable, value == l_False);
      candidates.add(l);
    }
  }

  // The formula is satisfiable. Let's go for the backbone!
  return true;
}

// Run of the worker
void FastOnCliffsSlowOnPlains::run() {
  int number_of_previous_candidates;
  int number_of_current_candidates;
  bool is_flatland = false;
  vec<Lit> literals;
  Lit relaxation_literal = lit_Undef;

  while (candidates.size()) {
    // Store the number of candidates before discarding
    number_of_previous_candidates = (int)candidates.size();
    // Rush down the ravine!
    if (!is_flatland) {
      bump_and_solve();
      discard_candidates();
      // Move carefully on the plateau
    } else {
      // Relax the clause added in the previous iteration
      if (relaxation_literal != lit_Undef) {
        solver.addClause(relaxation_literal);
      }

      literals.clear();

      // Create a new relaxation literal
      const Lit relaxation_literal = mkLit(solver.newVar());
      literals.push(relaxation_literal);

      // Create the clause ~lit1 or ~lit2 or ... for all candidates
      int candidates_size = (int)candidates.size();
      while (literals.size() <= candidates_size) {
        ++candidates_iterator;
        const Lit lit = *candidates_iterator;
        literals.push(~lit);
      }

      // Add the new clause
      solver.addClause(literals);

      // Run the solver enabling the clause (assuming that the
      // relaxation_literal is false)
      vec<Lit> assumptions(1);
      assumptions[0] = ~relaxation_literal;
      const bool is_sat = bump_and_solve(assumptions);

      // Analyze solver's output
      if (!is_sat) {
        backbone = candidates;
        break;
      } else {
        discard_candidates();
      }
    }

    // Update the number of candidates after discarding
    number_of_current_candidates = (int)candidates.size();

    // Check if we are in a flatland
    is_flatland = flatland_tester.is_flatland(number_of_previous_candidates -
                                              number_of_current_candidates);
  }
}

bool FastOnCliffsSlowOnPlains::bump_and_solve(const vec<Lit>& assumptions) {
  // Update the solver's detector activities and polarities
  for (auto li = candidates.begin(); li != candidates.end(); ++li) {
    const Lit l = *li;
    const Var v = var(l);
    solver.bump(v);
    solver.set_detector_polarity(v, sign(l) ? l_False : l_True);
  }
  return solver.solve(assumptions);
}

bool FastOnCliffsSlowOnPlains::bump_and_solve() {
  const vec<Lit> assumptions;
  return bump_and_solve(assumptions);
}

void FastOnCliffsSlowOnPlains::discard_candidates() {
  candidates.discard_from_model(solver.model, max_id, discarded_candidates);
  for (int i = 0; i < discarded_candidates.size(); ++i) {
    const Var v = var(discarded_candidates[i]);
    solver.reset_activity_for_var(v);
    solver.reset_detector_polarity(v);
  }
}

// Getters
bool FastOnCliffsSlowOnPlains::is_backbone(const Lit& literal) const {
  return backbone.get(literal);
}

bool FastOnCliffsSlowOnPlains::is_backbone(Var var) const {
  return is_backbone(~mkLit(var)) || is_backbone(mkLit(var));
}

bool FastOnCliffsSlowOnPlains::backbone_sign(Var var) const {
  assert(is_backbone(var));
  return is_backbone(mkLit(var));
}
