/**
 * @file RushAndPray.cc
 * @brief Implementation file for RushAndPray
 */

#include <iostream>

#include "RushAndPray.hh"
using namespace bonedigger;
using Minisat::lit_Undef;

// Initialization
RushAndPray::RushAndPray(Var _max_id, const CNF& _clauses,
                         double _attention_weight)
    : max_id(_max_id),
      clauses(_clauses),
      attention_weight(_attention_weight),
      candidates_iterator(candidates.infinite_iterator()) {}

RushAndPray::~RushAndPray() {}

bool RushAndPray::initialize() {
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
void RushAndPray::run() {
  int number_of_previous_candidates;
  int number_of_current_candidates;
  do {
    number_of_previous_candidates = (int)candidates.size();

    // Early termination if no candidates left
    if (number_of_previous_candidates == 0) break;

    bump_and_solve();
    discard_candidates();
    number_of_current_candidates = (int)candidates.size();
  } while (number_of_previous_candidates != number_of_current_candidates);

  // Only verify if we still have candidates
  if (candidates.size() > 0) {
    verify_candidates();
  }
}

bool RushAndPray::bump_and_solve(const vec<Lit>& assumptions) {
  // Update the solver's detector activities and polarities
  for (auto li = candidates.begin(); li != candidates.end(); ++li) {
    const Lit l = *li;
    const Var v = var(l);
    solver.bump(v);
    solver.set_detector_polarity(v, sign(l) ? l_False : l_True);
  }
  return solver.solve(assumptions);
}

bool RushAndPray::bump_and_solve() {
  const vec<Lit> assumptions;
  return bump_and_solve(assumptions);
}

void RushAndPray::discard_candidates() {
  candidates.discard_from_model(solver.model, max_id, discarded_candidates);
  for (int i = 0; i < discarded_candidates.size(); ++i) {
    const Var v = var(discarded_candidates[i]);
    solver.reset_activity_for_var(v);
    solver.reset_detector_polarity(v);
  }
}

void RushAndPray::verify_candidates() {
  vec<Lit> literals;
  vec<Lit> assumptions(1);
  Lit relaxation_literal = lit_Undef;

  while (candidates.size()) {
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

    // Run the solver enabling the clause (assuming that the relaxation_literal
    // is false)
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
}

// Getters
bool RushAndPray::is_backbone(const Lit& literal) const {
  return backbone.get(literal);
}

bool RushAndPray::is_backbone(Var var) const {
  return is_backbone(~mkLit(var)) || is_backbone(mkLit(var));
}

bool RushAndPray::backbone_sign(Var var) const {
  assert(is_backbone(var));
  return is_backbone(mkLit(var));
}
