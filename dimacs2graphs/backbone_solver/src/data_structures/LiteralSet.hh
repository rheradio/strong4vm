/**
 * @file LiteralSet.hh
 * @brief Efficient bit-set implementation for storing literal sets
 *
 * Provides a high-performance data structure for storing and manipulating
 * sets of MiniSat literals, optimized for backbone detection algorithms.
 */

#ifndef LITERALSET_HH
#define LITERALSET_HH

#include <vector>

#include "minisat_interface/minisat_aux.hh"
using std::vector;

class LiteralSet;

/**
 * @class const_infinite_LiteralSetIterator
 * @brief Infinite iterator that cycles through literals in a LiteralSet
 *
 * This iterator wraps around when it reaches the end of the set,
 * making it useful for round-robin processing of backbone candidates.
 */
class const_infinite_LiteralSetIterator {
 public:
  /**
   * @brief Construct an infinite iterator
   * @param ls The LiteralSet to iterate over
   * @param x Starting position (index in internal storage)
   */
  const_infinite_LiteralSetIterator(const LiteralSet& ls, size_t x);

  /**
   * @brief Copy constructor
   * @param mit Iterator to copy from
   */
  const_infinite_LiteralSetIterator(
      const const_infinite_LiteralSetIterator& mit)
      : ls(mit.ls), i(mit.i) {}

  /**
   * @brief Advance to next literal (wraps around at end)
   * @return Reference to this iterator
   */
  inline const_infinite_LiteralSetIterator& operator++();

  /**
   * @brief Dereference to get current literal
   * @return The current literal
   */
  inline const Lit operator*() const;

  /**
   * @brief Equality comparison
   * @param rhs Iterator to compare with
   * @return true if iterators point to same position
   */
  bool operator==(const const_infinite_LiteralSetIterator& rhs) {
    assert(&ls == &(rhs.ls));
    return i == rhs.i;
  }

  /**
   * @brief Inequality comparison
   * @param rhs Iterator to compare with
   * @return true if iterators point to different positions
   */
  bool operator!=(const const_infinite_LiteralSetIterator& rhs) {
    assert(&ls == &(rhs.ls));
    return i != rhs.i;
  }

 private:
  const LiteralSet& ls;  ///< Reference to the LiteralSet being iterated
  size_t i;              ///< Current position in internal storage
};

/**
 * @class const_LiteralSetIterator
 * @brief Standard forward iterator for LiteralSet
 *
 * Provides standard C++ forward iterator semantics for iterating
 * through all literals in a LiteralSet exactly once.
 */
class const_LiteralSetIterator
    : public std::iterator<std::forward_iterator_tag, Lit> {
 public:
  /**
   * @brief Construct an iterator
   * @param ls The LiteralSet to iterate over
   * @param x Starting position (index in internal storage)
   */
  const_LiteralSetIterator(const LiteralSet& ls, size_t x);

  /**
   * @brief Copy constructor
   * @param mit Iterator to copy from
   */
  const_LiteralSetIterator(const const_LiteralSetIterator& mit)
      : ls(mit.ls), i(mit.i) {}

  /**
   * @brief Advance to next literal
   * @return Reference to this iterator
   */
  inline const_LiteralSetIterator& operator++();

  /**
   * @brief Dereference to get current literal
   * @return The current literal
   */
  inline const Lit operator*() const;

  /**
   * @brief Equality comparison
   * @param rhs Iterator to compare with
   * @return true if iterators point to same position
   */
  bool operator==(const const_LiteralSetIterator& rhs) {
    assert(&ls == &(rhs.ls));
    return i == rhs.i;
  }

  /**
   * @brief Inequality comparison
   * @param rhs Iterator to compare with
   * @return true if iterators point to different positions
   */
  bool operator!=(const const_LiteralSetIterator& rhs) {
    assert(&ls == &(rhs.ls));
    return i != rhs.i;
  }

 private:
  const LiteralSet& ls;  ///< Reference to the LiteralSet being iterated
  size_t i;              ///< Current position in internal storage
};

/**
 * @class LiteralSet
 * @brief Efficient bit-set for storing MiniSat literals
 *
 * A high-performance data structure for storing sets of literals.
 * Uses vector<uint8_t> instead of vector<bool> for much better performance.
 * Provides O(1) add, remove, and membership testing operations.
 *
 * The internal representation uses one byte per possible literal index,
 * where each byte is either 0 (not present) or 1 (present).
 *
 * Example usage:
 * @code
 * LiteralSet candidates;
 * candidates.add(mkLit(5, false));  // Add positive literal for variable 5
 * candidates.add(mkLit(7, true));   // Add negative literal for variable 7
 * if (candidates.get(mkLit(5, false))) {
 *     // Literal is in the set
 * }
 * candidates.remove(mkLit(5, false));
 * @endcode
 */
class LiteralSet {
 public:
  /// Standard const iterator type for range-based for loops
  typedef const_LiteralSetIterator const_iterator;

  /**
   * @brief Construct an empty LiteralSet
   */
  LiteralSet() : _size(0) {}

  /**
   * @brief Add a literal to the set
   * @param l Literal to add
   * @return true if literal was added (wasn't already present)
   * @return false if literal was already in the set
   */
  inline bool add(Lit l);

  /**
   * @brief Remove a literal from the set
   * @param l Literal to remove
   * @return true if literal was removed (was present)
   * @return false if literal was not in the set
   */
  inline bool remove(Lit l);

  /**
   * @brief Check if a literal is in the set
   * @param l Literal to check
   * @return true if literal is present in the set
   * @return false otherwise
   */
  inline bool get(Lit l) const;

  /**
   * @brief Clear all literals from the set
   */
  inline void clear();

  /**
   * @brief Ultra-fast bulk removal based on solver model
   *
   * Removes all candidate literals that are contradicted by the given
   * satisfying assignment. Uses manual loop unrolling and branch hints
   * for maximum performance.
   *
   * @tparam ModelVec Type of model vector (must support operator[] and size())
   * @param model Satisfying assignment from solver
   * @param max_var Maximum variable ID to process
   * @param discarded Output vector for removed literals
   */
  template <typename ModelVec>
  inline void discard_from_model(const ModelVec& model, int max_var,
                                 vec<Lit>& discarded);

  /**
   * @brief Get the number of literals in the set
   * @return Number of literals currently stored
   */
  size_t size() const { return _size; }

  /**
   * @brief Create an infinite iterator that cycles through the set
   * @return Infinite iterator starting at the beginning
   */
  const_infinite_LiteralSetIterator infinite_iterator() const {
    return const_infinite_LiteralSetIterator(*this, 0);
  }

  /**
   * @brief Get the physical size of internal storage
   * @return Size of internal vector (>= logical size)
   */
  size_t physical_size() const { return set.size(); }

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

 private:
  size_t _size;          ///< Number of literals in the set
  vector<uint8_t> set;   ///< Bit-set storage (uint8_t is much faster than vector<bool>)
};

// Inline implementations

inline bool LiteralSet::add(Lit l) {
  const size_t li = literal_index(l);
  if (li >= set.size()) set.resize(li + 1, 0);
  if (set[li]) return false;
  set[li] = 1;
  ++_size;
  return true;
}

inline bool LiteralSet::remove(Lit l) {
  const size_t li = literal_index(l);
  if (li >= set.size()) return false;
  if (!set[li]) return false;
  set[li] = 0;
  --_size;
  return true;
}

inline bool LiteralSet::get(Lit l) const {
  const size_t li = literal_index(l);
  if (li >= set.size()) return false;
  return set[li];
}

inline void LiteralSet::clear() {
  set.clear();
  _size = 0;
}

inline const_infinite_LiteralSetIterator&
const_infinite_LiteralSetIterator::operator++() {
  if (!ls.size()) return *this;
  assert(i < ls.physical_size());
  assert(0 <= i);
  do {
    i = (i + 1) % ls.physical_size();
  } while (i < 2 || !ls.get(index2literal(i)));
  assert(ls.get(index2literal(i)));
  return *this;
}

inline const Lit const_infinite_LiteralSetIterator::operator*() const {
  const Lit return_value = index2literal(i);
  assert(ls.get(return_value));
  return return_value;
}

inline const_LiteralSetIterator& const_LiteralSetIterator::operator++() {
  assert(i < ls.physical_size());
  ++i;
  while ((i < ls.physical_size()) && !ls.get(index2literal(i))) ++i;
  return *this;
}

inline const Lit const_LiteralSetIterator::operator*() const {
  const Lit r = index2literal(i);
  assert(ls.get(r));
  return r;
}

inline LiteralSet::const_iterator LiteralSet::end() const {
  return const_LiteralSetIterator(*this, physical_size());
}
inline LiteralSet::const_iterator LiteralSet::begin() const {
  return const_LiteralSetIterator(*this, 2);
}

// Implementation of discard_from_model (documentation in declaration above)
template <typename ModelVec>
inline void LiteralSet::discard_from_model(const ModelVec& model, int max_var,
                                           vec<Lit>& discarded) {
  discarded.clear();
  if (_size == 0) return;

  const int model_size = model.size();
  const int limit = (max_var < model_size - 1) ? max_var : model_size - 1;

  size_t removed = 0;

  // Process in chunks for better cache utilization
  for (int var = 1; var <= limit; var += 4) {
    // Unroll loop manually for better performance
    const int end = (var + 3 <= limit) ? var + 3 : limit;

    for (int v = var; v <= end; ++v) {
      const auto value = model[v];

      // Branchless selection: compute both, use based on value
      const size_t li_pos = literal_index(mkLit(v, false));
      const size_t li_neg = literal_index(mkLit(v, true));

      if (__builtin_expect(value == l_True, 1)) {
        // Remove negative literal
        if (__builtin_expect(li_neg < set.size() && set[li_neg], 1)) {
          set[li_neg] = 0;
          discarded.push(mkLit(v, true));
          ++removed;
        }
      } else if (__builtin_expect(value == l_False, 1)) {
        // Remove positive literal
        if (__builtin_expect(li_pos < set.size() && set[li_pos], 1)) {
          set[li_pos] = 0;
          discarded.push(mkLit(v, false));
          ++removed;
        }
      }
    }

    // Early exit if we've removed everything
    if (__builtin_expect(removed >= _size, 0)) break;
  }

  _size -= removed;
}

#endif  // LITERALSET_HH
