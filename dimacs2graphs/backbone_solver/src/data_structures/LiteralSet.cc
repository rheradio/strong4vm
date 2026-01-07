/**
 * @file LiteralSet.cc
 * @brief Implementation file for LiteralSet
 */

#include "LiteralSet.hh"

const_infinite_LiteralSetIterator::const_infinite_LiteralSetIterator(
    const LiteralSet& ls, size_t x)
    : ls(ls), i(x) {
  if (!ls.size()) return;
  while (i < 2 || !ls.get(index2literal(i))) i = (i + 1) % ls.physical_size();
  assert(ls.get(index2literal(i)));
}

const_LiteralSetIterator::const_LiteralSetIterator(const LiteralSet& ls,
                                                   size_t x)
    : ls(ls), i(x) {
  while ((i < ls.physical_size()) && !ls.get(index2literal(i))) ++i;
}
