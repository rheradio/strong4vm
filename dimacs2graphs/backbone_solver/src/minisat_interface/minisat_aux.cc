/**
 * @file minisat_aux.cc
 * @brief Implementation file for minisat_aux
 */

#include <iostream>

#include "minisat_aux.hh"
using std::cerr;

ostream& print(ostream& out, Lit l) {
  return out << (sign(l) ? "-" : "") << var(l);
}

ostream& print(ostream& out, const vec<Lit>& lv) {
  for (int i = 0; i < lv.size(); ++i) out << lv[i] << " ";
  return out;
}

ostream& operator<<(ostream& outs, Lit lit) { return print(outs, lit); }
