/**
 * @file minisat_aux.hh
 * @brief Helper functions and type aliases for MiniSat integration
 *
 * Provides utility functions for working with MiniSat types (Lit, Var, lbool)
 * and conversions between different literal representations.
 */

#ifndef MINISAT_AUX_HH
#define MINISAT_AUX_HH
#include <ostream>
#include <vector>

#include "minisat/core/Solver.h"
using Minisat::l_False;
using Minisat::l_True;
using Minisat::l_Undef;
using Minisat::lbool;
using Minisat::Lit;
using Minisat::mkLit;
using Minisat::sign;
using Minisat::Solver;
using Minisat::var;
using Minisat::Var;
using Minisat::vec;
using std::ostream;
using std::vector;

/// Vector of MiniSat literals using std::vector
typedef std::vector<Lit> LiteralVector;

/**
 * @brief Check if a literal is true in a model
 *
 * Evaluates whether a literal has the value true in a given satisfying assignment.
 * Takes into account the sign of the literal.
 *
 * @param l The literal to check
 * @param model The satisfying assignment (variable values)
 * @return true if the literal evaluates to true in the model
 * @return false otherwise
 */
inline bool is_true(Lit l, const vec<lbool>& model) {
  const Var v = var(l);
  assert(v < model.size());
  return (model[v] == l_False) == sign(l);
}

/**
 * @brief Output operator for MiniSat literals
 *
 * Prints a literal in human-readable format (e.g., "5" for positive, "-5" for negative).
 *
 * @param outs Output stream
 * @param lit Literal to print
 * @return Reference to the output stream
 */
ostream& operator<<(ostream& outs, Lit lit);

/**
 * @brief Print a vector of literals
 *
 * Outputs a vector of literals in readable format.
 *
 * @param out Output stream
 * @param lv Vector of literals
 * @return Reference to the output stream
 */
ostream& print(ostream& out, const vec<Lit>& lv);

/**
 * @brief Output operator for MiniSat literal vectors
 *
 * @param out Output stream
 * @param lv Vector of literals
 * @return Reference to the output stream
 */
inline ostream& operator<<(ostream& out, const vec<Lit>& lv) {
  return print(out, lv);
}

/**
 * @brief Output operator for MiniSat literal sets
 *
 * @param out Output stream
 * @param lv Literal set
 * @return Reference to the output stream
 */
inline ostream& operator<<(ostream& out, const Minisat::LSet& lv) {
  return print(out, lv.toVec());
}

/**
 * @brief Convert a MiniSat literal to an index
 *
 * Maps a literal to a unique integer index that can be used for array indexing.
 * Positive and negative literals of the same variable map to different indices.
 * The encoding is: index = (var * 2) + (sign ? 0 : 1)
 *
 * @param l The literal to convert
 * @return Unique index for the literal (>= 2)
 */
inline size_t literal_index(Lit l) {
  assert(var(l) > 0);
  const size_t v = (size_t)var(l);
  return sign(l) ? v << 1 : ((v << 1) + 1);
}

/**
 * @brief Convert an index back to a MiniSat literal
 *
 * Inverse operation of literal_index(). Reconstructs a literal from its index.
 *
 * @param l The index to convert
 * @return The corresponding MiniSat literal
 */
inline Lit index2literal(size_t l) {
  const bool positive = (l & 1);
  const Var variable = l >> 1;
  return positive ? mkLit(variable) : ~mkLit(variable);
}

#endif  // MINISAT_AUX_HH
