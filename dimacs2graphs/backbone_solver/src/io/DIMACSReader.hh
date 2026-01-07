/**
 * @file DIMACSReader.hh
 * @brief DIMACS CNF format parser and related data structures
 *
 * Provides classes for reading and parsing Boolean formulas in DIMACS CNF format,
 * along with reference-counted immutable literal sets for efficient clause storage.
 */

#ifndef DIMACSREADER_HH
#define DIMACSREADER_HH

#include <stdio.h>
#include <zlib.h>

#include <exception>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

#include "minisat_interface/minisat_aux.hh"
#include "parse_utils.hh"
using std::cerr;
using std::endl;
using std::exception;
using std::istream;
using std::iterator;
using std::pair;
using std::string;
using std::vector;

// Debug macro for LitSet
#ifndef DBG_LITSET
#define DBG_LITSET(x)
#endif

class const_LitIterator;

/**
 * @class LitSet
 * @brief Reference-counted immutable set of literals
 *
 * A memory-efficient data structure for storing clauses (sets of literals).
 * Uses reference counting to allow sharing of literal data between multiple
 * LitSet instances without copying. The first element of the internal array
 * stores the reference count.
 *
 * This is different from LiteralSet (in LiteralSet.hh) which is mutable
 * and optimized for dynamic membership testing. LitSet is immutable and
 * optimized for memory efficiency when storing many clauses.
 *
 * Example usage:
 * @code
 * LiteralVector lits = {mkLit(1, false), mkLit(2, true)};
 * LitSet clause(lits);  // Create clause {x1, ~x2}
 * LitSet copy = clause; // Share data, increment ref count
 * @endcode
 */
class LitSet {
 public:
  /// Standard const iterator type for range-based for loops
  typedef const_LitIterator const_iterator;

  /**
   * @brief Construct an empty LitSet
   */
  inline LitSet() {
    _literals = NULL;
    _size = 0;
  }

  /**
   * @brief Copy constructor with reference counting
   *
   * Creates a new LitSet sharing the same underlying literal array.
   * Increments the reference count.
   *
   * @param ls LitSet to copy from
   */
  inline LitSet(const LitSet& ls) {
    _size = ls._size;
    _literals = ls._literals;
    if (_literals != NULL) ++(_literals[0].x);
  }

  /**
   * @brief Copy assignment operator with reference counting
   *
   * @param ls LitSet to assign from
   * @return Reference to this LitSet
   */
  LitSet& operator=(const LitSet& ls) {
    decrease();
    _size = ls._size;
    _literals = ls._literals;
    if (_literals != NULL) ++(_literals[0].x);
    return *this;
  }

  /**
   * @brief Construct LitSet from a vector of literals
   *
   * Allocates new storage and copies literals from the vector.
   * Initializes reference count to 1.
   *
   * @param lits Vector of literals to store
   */
  LitSet(const LiteralVector& lits);

  /**
   * @brief Destructor with reference counting
   *
   * Decrements reference count and deallocates memory if count reaches zero.
   */
  virtual ~LitSet();

  /**
   * @brief Get the number of literals in the set
   * @return Size of the literal set
   */
  inline size_t size() const;

  /**
   * @brief Get iterator to first literal
   * @return const_iterator to beginning
   */
  inline const_iterator begin() const;

  /**
   * @brief Get iterator past last literal
   * @return const_iterator to end
   */
  inline const_iterator end() const;

  /**
   * @brief Access literal by index
   *
   * @param index Position in the set (0-based)
   * @return The literal at the specified index
   */
  inline const Lit operator[](size_t index) const;

 private:
  size_t _size;      ///< Number of literals in the set
  Lit* _literals;    ///< Array: [0]=reference counter, [1..size]=literals

  /**
   * @brief Decrease reference counter and deallocate if zero
   * @return New reference count value
   */
  inline int decrease();
};

/**
 * @class const_LitIterator
 * @brief Forward iterator for LitSet
 *
 * Standard C++ forward iterator for iterating through literals in a LitSet.
 */
class const_LitIterator : public iterator<std::forward_iterator_tag, Lit> {
 public:
  /**
   * @brief Construct an iterator
   * @param ls LitSet to iterate over
   * @param x Starting position
   */
  const_LitIterator(const LitSet& ls, size_t x) : ls(ls), i(x) {}

  /**
   * @brief Copy constructor
   * @param mit Iterator to copy from
   */
  const_LitIterator(const const_LitIterator& mit) : ls(mit.ls), i(mit.i) {}

  /**
   * @brief Advance to next literal
   * @return Reference to this iterator
   */
  const_LitIterator& operator++() {
    ++i;
    return *this;
  }

  /**
   * @brief Equality comparison
   * @param rhs Iterator to compare with
   * @return true if iterators point to same position
   */
  bool operator==(const const_LitIterator& rhs) {
    assert(&ls == &(rhs.ls));
    return i == rhs.i;
  }

  /**
   * @brief Inequality comparison
   * @param rhs Iterator to compare with
   * @return true if iterators point to different positions
   */
  bool operator!=(const const_LitIterator& rhs) {
    assert(&ls == &(rhs.ls));
    return i != rhs.i;
  }

  /**
   * @brief Dereference to get current literal
   * @return The current literal
   */
  const Lit operator*() const { return ls[i]; }

 private:
  const LitSet& ls;  ///< Reference to the LitSet being iterated
  size_t i;          ///< Current position
};

// Inline implementations for LitSet

inline int LitSet::decrease() {
  if (_literals == NULL) return 0;
  assert(_literals[0].x);
  DBG_LITSET(const int ov = _literals[0].x;)
  const int nv = --(_literals[0].x);
  DBG_LITSET(cerr << "dec " << ov << " to " << nv << endl;)
  if ((_literals[0].x) == 0) {
    DBG_LITSET(cerr << "mem rel" << endl;)
    delete[] _literals;
  }
  _literals = NULL;
  return nv;
}

inline size_t LitSet::size() const { return _size; }

inline const Lit LitSet::operator[](size_t index) const {
  assert(index < _size);
  return _literals[index + 1];
}

inline LitSet::const_iterator LitSet::begin() const {
  return const_LitIterator(*this, 0);
}

inline LitSet::const_iterator LitSet::end() const {
  return const_LitIterator(*this, _size);
}

// Type definitions

/// CNF formula represented as a vector of clauses (LitSets)
typedef vector<LitSet> CNF;

/// Range of variables [min, max]
typedef pair<Var, Var> Range;

/**
 * @class Reader
 * @brief Low-level character reader for DIMACS files
 *
 * Provides character-by-character reading from various input sources
 * (gzipped files, streams, buffers). Handles whitespace skipping.
 */
class Reader {
 public:
  /**
   * @brief Construct reader from gzipped file
   * @param zf Open gzFile handle
   */
  Reader(gzFile& zf);

  /**
   * @brief Construct reader from stream buffer
   * @param zipStream Stream buffer to read from
   */
  Reader(StreamBuffer& zipStream);

  /**
   * @brief Construct reader from input stream
   * @param stream Input stream to read from
   */
  Reader(istream& stream);

  /**
   * @brief Copy constructor
   * @param orig Reader to copy from
   */
  Reader(const Reader& orig);

  /**
   * @brief Destructor
   */
  virtual ~Reader();

  /**
   * @brief Dereference to get current character
   * @return Current character as int
   */
  int operator*();

  /**
   * @brief Advance to next character
   */
  void operator++();

  /**
   * @brief Skip whitespace characters
   */
  void skip_whitespace();

 private:
  StreamBuffer* zip;  ///< Buffer for compressed input
  istream* s;         ///< Input stream
  int c;              ///< Current character
};

/**
 * @class DIMACSReaderException
 * @brief Exception thrown on DIMACS parsing errors
 *
 * Used to report errors encountered while parsing DIMACS CNF files.
 */
class DIMACSReaderException : public exception {
 public:
  /**
   * @brief Construct exception with error message
   * @param message Description of the error
   */
  DIMACSReaderException(const string& message);

  /**
   * @brief Get error message
   * @return C-string describing the error
   */
  const char* what() const throw();

  /**
   * @brief Destructor
   */
  ~DIMACSReaderException() throw() { delete[] s; }

 private:
  char* s;  ///< Error message string
};

/**
 * @class DIMACSReader
 * @brief Parser for DIMACS CNF format files
 *
 * Parses Boolean formulas in the standard DIMACS CNF format.
 * The format consists of:
 * - Comment lines starting with 'c'
 * - Problem line: p cnf [num_vars] [num_clauses]
 * - Clause lines: space-separated literals terminated by 0
 *
 * Example DIMACS file:
 * @code
 * c This is a comment
 * p cnf 3 2
 * 1 -2 0
 * 2 3 0
 * @endcode
 *
 * Usage:
 * @code
 * Reader reader(input_stream);
 * DIMACSReader dimacs(reader);
 * dimacs.read();
 * const CNF& clauses = dimacs.get_clause_vector();
 * Var max_var = dimacs.get_max_id();
 * @endcode
 */
class DIMACSReader {
 public:
  /**
   * @brief Construct DIMACS reader from a Reader
   * @param input_file Reader providing character input
   */
  DIMACSReader(Reader& input_file);

  /**
   * @brief Copy constructor
   * @param orig DIMACSReader to copy from
   */
  DIMACSReader(const DIMACSReader& orig);

  /**
   * @brief Destructor
   */
  virtual ~DIMACSReader();

  /**
   * @brief Parse the DIMACS file
   *
   * Reads and parses the entire DIMACS CNF file, storing clauses internally.
   * Throws DIMACSReaderException on parse errors.
   */
  void read();

  /**
   * @brief Get maximum variable ID in the formula
   * @return Highest variable number used
   */
  Var get_max_id() const { return mxid; }

  /**
   * @brief Get the parsed CNF formula
   * @return Vector of clauses (each clause is a LitSet)
   */
  const CNF& get_clause_vector() const { return clause_vector; }

 private:
  Reader& in;              ///< Input reader
  Var mxid;                ///< Maximum variable ID seen
  CNF clause_vector;       ///< Parsed clauses

  /**
   * @brief Parse a single literal from input
   * @param in Input reader
   * @return Parsed literal
   */
  Lit parse_lit(Reader& in);

  /**
   * @brief Read a CNF clause from input
   * @param lits Output vector to store parsed literals
   */
  void read_cnf_clause(vector<Lit>& lits);
};

#endif  // DIMACSREADER_HH
