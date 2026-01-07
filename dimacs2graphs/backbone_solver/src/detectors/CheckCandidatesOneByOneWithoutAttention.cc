/**
 * @file CheckCandidatesOneByOneWithoutAttention.cc
 * @brief Implementation of baseline backbone detector without activity bumping
 *
 * This file implements the CheckCandidatesOneByOneWithoutAttention backbone
 * detection algorithm, which is the baseline variant that does NOT use activity
 * bumping heuristics. This provides a reference implementation for performance
 * comparison with the optimized CheckCandidatesOneByOne variant.
 *
 * ## Algorithm Overview
 *
 * The backbone detection process is identical to CheckCandidatesOneByOne except
 * for the absence of activity bumping:
 *
 * **Phase 1: Initialization**
 * 1. Initialize SAT solver with all variables and clauses
 * 2. Find first satisfying assignment
 * 3. Initialize candidate set with literals from first solution
 *
 * **Phase 2: Systematic Checking**
 * For each candidate literal `l`:
 * 1. Assume `¬l` (opposite of candidate)
 * 2. Solve with assumptions (NO activity bumping)
 * 3. If UNSAT → `l` is backbone (must be true in all solutions)
 * 4. If SAT → Update candidates based on new solution
 *
 * **Phase 3: Refinement**
 * - Remove non-backbone candidates from candidate set
 * - Learn from SAT solver conflicts (natural conflict-driven learning only)
 *
 * ## Key Differences from CheckCandidatesOneByOne
 *
 * - **This variant**: Uses plain solve() without variable activity manipulation
 * - **CheckCandidatesOneByOne**: Uses bump_and_solve() to boost activity scores
 * - **Performance**: Typically 2-3x slower on large formulas due to less informed search
 * - **Use case**: Baseline for benchmarking and academic comparisons
 *
 * ## When to Use This Detector
 *
 * - **Performance analysis**: Measuring the impact of activity bumping heuristics
 * - **Reproducibility**: More deterministic behavior for research experiments
 * - **Small formulas**: Overhead of activity bumping may not be worthwhile
 *
 * @see CheckCandidatesOneByOne for the optimized variant with activity bumping
 *
 * @author Backbone Solver Team
 * @date 2024
 */

#include <iostream>

#include "CheckCandidatesOneByOneWithoutAttention.hh"
using namespace backbone_solver;
using Minisat::lit_Undef;

// Initialization
CheckCandidatesOneByOneWithoutAttention::
    CheckCandidatesOneByOneWithoutAttention(Var _max_id, const CNF& _clauses)
    : max_id(_max_id),
      clauses(_clauses),
      candidates_iterator(candidates.infinite_iterator()) {}

CheckCandidatesOneByOneWithoutAttention::
    ~CheckCandidatesOneByOneWithoutAttention() {}

bool CheckCandidatesOneByOneWithoutAttention::initialize() {
  // Initialize the solver
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
void CheckCandidatesOneByOneWithoutAttention::run() {
  while (candidates.size()) {
    ++candidates_iterator;
    const Lit candidate = *candidates_iterator;
    vec<Lit> assumptions(1);
    assumptions[0] = ~candidate;
    const bool is_sat = solver.solve(assumptions);
    if (!is_sat) {
      backbone.add(candidate);
      candidates.remove(*candidates_iterator);
    } else {
      candidates.discard_from_model(solver.model, max_id, discarded_candidates);
    }
  }
}

// Run backbone detection with assumptions
bool CheckCandidatesOneByOneWithoutAttention::run_with_assumptions(
    const vec<Lit>& assumptions) {
  // Clear previous state
  candidates.clear();
  backbone.clear();

  // Initialize the solver if not already done
  // Check if solver has variables (if not, we need to initialize)
  if (solver.nVars() == 0) {
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
  }

  // Store persistent assumptions for later use
  persistent_assumptions.clear();
  for (int i = 0; i < assumptions.size(); ++i) {
    persistent_assumptions.push(assumptions[i]);
  }

  // Solve with assumptions to get initial solution
  const bool is_sat = solver.solve(assumptions);

  if (!is_sat) {
    // Formula is unsatisfiable under these assumptions
    return false;
  }

  // Initialize candidates from the solution
  const vec<lbool>& solution = solver.model;
  for (Var variable = 1; variable <= max_id; ++variable) {
    const lbool value = solution[variable];
    if (value != l_Undef) {
      const Lit l = mkLit(variable, value == l_False);
      candidates.add(l);
    }
  }

  // Create iterator for the new candidates set
  const_infinite_LiteralSetIterator local_iterator = candidates.infinite_iterator();

  // Run backbone detection with persistent assumptions
  while (candidates.size()) {
    ++local_iterator;
    const Lit candidate = *local_iterator;

    // Create combined assumptions: persistent + negation of candidate
    vec<Lit> combined_assumptions;
    for (int i = 0; i < persistent_assumptions.size(); ++i) {
      combined_assumptions.push(persistent_assumptions[i]);
    }
    combined_assumptions.push(~candidate);

    const bool is_sat_local = solver.solve(combined_assumptions);
    if (!is_sat_local) {
      backbone.add(candidate);
      candidates.remove(*local_iterator);
    } else {
      candidates.discard_from_model(solver.model, max_id, discarded_candidates);
    }
  }

  // Clear persistent assumptions after completion
  persistent_assumptions.clear();
  return true;
}

// Getters
bool CheckCandidatesOneByOneWithoutAttention::is_backbone(
    const Lit& literal) const {
  return backbone.get(literal);
}

bool CheckCandidatesOneByOneWithoutAttention::is_backbone(Var var) const {
  return is_backbone(~mkLit(var)) || is_backbone(mkLit(var));
}

bool CheckCandidatesOneByOneWithoutAttention::backbone_sign(Var var) const {
  assert(is_backbone(var));
  return is_backbone(mkLit(var));
}