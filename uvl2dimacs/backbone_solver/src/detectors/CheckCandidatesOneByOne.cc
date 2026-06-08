/**
 * @file CheckCandidatesOneByOne.cc
 * @brief Implementation file for CheckCandidatesOneByOne
 */

#include <iostream>

#include "CheckCandidatesOneByOne.hh"
using namespace bonedigger;
using Minisat::lit_Undef;

// Initialization
CheckCandidatesOneByOne::CheckCandidatesOneByOne(Var _max_id,
                                                 const CNF& _clauses,
                                                 double _attention_weight)
    : max_id(_max_id),
      clauses(_clauses),
      attention_weight(_attention_weight),
      candidates_iterator(candidates.infinite_iterator()) {}

CheckCandidatesOneByOne::~CheckCandidatesOneByOne() {}

bool CheckCandidatesOneByOne::initialize() {
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
void CheckCandidatesOneByOne::run() {
  while (candidates.size()) {
    ++candidates_iterator;
    const Lit candidate = *candidates_iterator;
    vec<Lit> assumptions(1);
    assumptions[0] = ~candidate;
    const bool is_sat = bump_and_solve(assumptions);
    if (!is_sat) {
      backbone.add(candidate);
      discard_one_candidate();
    } else {
      discard_candidates();
    }
  }
}

bool CheckCandidatesOneByOne::bump_and_solve(const vec<Lit>& assumptions) {
  // Update the solver's detector activities and polarities
  for (auto li = candidates.begin(); li != candidates.end(); ++li) {
    const Lit l = *li;
    const Var v = var(l);
    solver.bump(v);
    solver.set_detector_polarity(v, sign(l) ? l_False : l_True);
  }
  return solver.solve(assumptions);
}

void CheckCandidatesOneByOne::discard_one_candidate() {
  candidates.remove(*candidates_iterator);
  const Var v = var(*candidates_iterator);
  solver.reset_activity_for_var(v);
  solver.reset_detector_polarity(v);
}

void CheckCandidatesOneByOne::discard_candidates() {
  candidates.discard_from_model(solver.model, max_id, discarded_candidates);
  for (int i = 0; i < discarded_candidates.size(); ++i) {
    const Var v = var(discarded_candidates[i]);
    solver.reset_activity_for_var(v);
    solver.reset_detector_polarity(v);
  }
}

// Getters
bool CheckCandidatesOneByOne::is_backbone(const Lit& literal) const {
  return backbone.get(literal);
}

bool CheckCandidatesOneByOne::is_backbone(Var var) const {
  return is_backbone(~mkLit(var)) || is_backbone(mkLit(var));
}

bool CheckCandidatesOneByOne::backbone_sign(Var var) const {
  assert(is_backbone(var));
  return is_backbone(mkLit(var));
}