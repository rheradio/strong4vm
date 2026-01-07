/**
 * @file DIMACSReader.cc
 * @brief Implementation file for DIMACSReader
 */

#include <assert.h>

#include <string>
#include <vector>

#include "DIMACSReader.hh"
#include "minisat/mtl/Sort.h"
#include "minisat_interface/minisat_aux.hh"
using Minisat::LessThan_default;
using Minisat::lit_Undef;
using Minisat::sort;

// LitSet implementation (merged from LitSet.cc)
LitSet::LitSet(const LiteralVector& lits) {
  const size_t vsz = lits.size();
  if (vsz == 0) {
    _literals = NULL;
    _size = 0;
    return;
  }
  _literals = new Lit[vsz + 1];
  _literals[0].x = 1;
  for (size_t i = 0; i < vsz; ++i) _literals[i + 1] = lits[i];
  sort(_literals + 1, vsz, LessThan_default<Lit>());
  _size = vsz;
  size_t j = 2;
  Lit last = _literals[1];
  for (size_t i = 2; i <= vsz; ++i) {
    if (_literals[i] == last) continue;
    _literals[j] = _literals[i];
    last = _literals[i];
    ++j;
  }
  _size = j - 1;
}

LitSet::~LitSet() { decrease(); }

DIMACSReaderException::DIMACSReaderException(const string& message) {
  s = new char[message.size() + 1];
  message.copy(s, message.size(), 0);
}

const char* DIMACSReaderException::what() const throw() { return s; }

Reader::Reader(gzFile& zf) : zip(new StreamBuffer(zf)), s(NULL) {}
Reader::Reader(StreamBuffer& zipStream) : zip(&zipStream), s(NULL) {}
Reader::Reader(istream& stream) : zip(NULL), s(&stream) { c = s->get(); }
Reader::Reader(const Reader& orig) : zip(orig.zip), s(orig.s), c(orig.c) {}
Reader::~Reader() {}

int Reader::operator*() {
  const int r = s == NULL ? **zip : c;
  return r;
}

void Reader::operator++() {
  if (s == NULL) {
    ++(*zip);
  } else {
    if (s->eof())
      c = EOF;
    else
      c = s->get();
  }
}
void Reader::skip_whitespace() {
  while (((**this) >= 9 && (**this) <= 13) || (**this) == 32) ++(*this);
}

DIMACSReader::DIMACSReader(Reader& input_file) : in(input_file), mxid(0) {}

DIMACSReader::DIMACSReader(const DIMACSReader& orig)
    : in(orig.in), mxid(orig.mxid) {
  assert(false);
}

DIMACSReader::~DIMACSReader() {}

void DIMACSReader::read_cnf_clause(vector<Lit>& lits) {
  Lit parsed_lit;
  lits.clear();
  for (;;) {
    in.skip_whitespace();
    parsed_lit = parse_lit(in);
    if (parsed_lit == lit_Undef) break;
    const Var v = var(parsed_lit);
    if (v > mxid) {
      mxid = v;
    }
    lits.push_back(parsed_lit);
  }
}

void DIMACSReader::read() {
  vector<Lit> literals;
  for (;;) {
    in.skip_whitespace();
    if (*in == EOF)
      break;
    else if (*in == 'c' || *in == 'p')
      skipLine(in);
    else {
      literals.clear();
      read_cnf_clause(literals);
      clause_vector.push_back(LitSet(literals));
    }
  }
}

Lit DIMACSReader::parse_lit(Reader& in) {
  Var v = 0;
  bool neg = false;
  if (*in == '-') {
    neg = true;
    ++in;
  } else if (*in == '+')
    ++in;
  if ((*in < '0') || (*in > '9')) {
    string s("unexpected char in place of a literal: ");
    s += *in;
    throw DIMACSReaderException(s);
  }
  while (*in >= '0' && *in <= '9') {
    v = v * 10 + (*in - '0');
    ++in;
  }
  return v ? mkLit(v, neg) : lit_Undef;
}
