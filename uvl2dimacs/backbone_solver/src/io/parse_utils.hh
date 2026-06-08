/**
 * @file parse_utils.hh
 * @brief Buffered stream and parsing utilities for DIMACS files
 *
 * Provides low-level parsing functions adapted from MiniSat for reading
 * and parsing DIMACS format files. Includes buffered stream reading from
 * gzipped files and template-based parsing utilities.
 */

/************************************************************************************[ParseUtils.h]
Copyright (c) 2003-2006, Niklas Een, Niklas Sorensson
Copyright (c) 2007-2010, Niklas Sorensson

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************/

#ifndef Minisat_ParseUtils_hh
#define Minisat_ParseUtils_hh

#include <stdlib.h>
#include <stdio.h>

#include <zlib.h>


/**
 * @defgroup ParseUtils Parsing Utilities
 * @brief Low-level parsing functions for DIMACS format
 * @{
 */

/// Buffer size for StreamBuffer (1MB)
static const int buffer_size = 1048576;

/**
 * @class StreamBuffer
 * @brief Buffered character stream for reading gzipped files
 *
 * Provides efficient buffered reading from gzipped files using zlib.
 * Automatically refills buffer when needed and provides simple
 * iterator-like interface for character-by-character access.
 */
class StreamBuffer {
    gzFile        in;             ///< Gzipped file handle
    unsigned char buf[buffer_size]; ///< Internal buffer
    int           pos;            ///< Current position in buffer
    int           size;           ///< Number of valid bytes in buffer

    /**
     * @brief Ensure buffer has data available
     *
     * Refills buffer from file if empty.
     */
    void assureLookahead() {
        if (pos >= size) {
            pos  = 0;
            size = gzread(in, buf, sizeof(buf)); } }

public:
    /**
     * @brief Construct stream buffer from gzipped file
     * @param i Gzipped file handle
     */
    explicit StreamBuffer(gzFile i) : in(i), pos(0), size(0) { assureLookahead(); }

    /**
     * @brief Get current character without advancing
     * @return Current character or EOF
     */
    int  operator *  () const { return (pos >= size) ? EOF : buf[pos]; }

    /**
     * @brief Advance to next character
     */
    void operator ++ ()       { pos++; assureLookahead(); }

    /**
     * @brief Get current buffer position
     * @return Current position in buffer
     */
    int  position    () const { return pos; }
};


/**
 * @brief Check if StreamBuffer reached end of file
 * @param in Stream buffer to check
 * @return true if at EOF
 */
static inline bool isEof(StreamBuffer& in) { return *in == EOF;  }

/**
 * @brief Check if C-string reached null terminator
 * @param in C-string pointer to check
 * @return true if at null terminator
 */
static inline bool isEof(const char*   in) { return *in == '\0'; }

/**
 * @brief Generic EOF check for any input stream type
 * @tparam B Input stream type
 * @param in Input stream to check
 * @return true if at EOF
 */
template<class B>
static inline bool isEof(B& in) { return *in == EOF;  }

/**
 * @brief Skip true whitespace (space and tab, not newline)
 * @tparam B Input stream type
 * @param in Input stream to advance
 */
template<class B>
static void skipTrueWhitespace(B& in) {     // not including newline
    while (*in == ' ' || *in == '\t')
        ++in; }

/**
 * @brief Skip all whitespace including newlines
 * @tparam B Input stream type
 * @param in Input stream to advance
 */
template<class B>
static void skipWhitespace(B& in) {
    while ((*in >= 9 && *in <= 13) || *in == 32)
        ++in; }

/**
 * @brief Skip to end of current line
 * @tparam B Input stream type
 * @param in Input stream to advance
 */
template<class B>
static void skipLine(B& in) {
    for (;;){
        if (isEof(in)) return;
        if (*in == '\n') { ++in; return; }
        ++in; } }

/**
 * @brief Parse an integer from input stream
 *
 * Skips leading whitespace, handles optional sign, and parses digits.
 * Exits with error if non-digit encountered.
 *
 * @tparam B Input stream type
 * @param in Input stream to parse from
 * @return Parsed integer value
 */
template<class B>
static int parseInt(B& in) {
    int     val = 0;
    bool    neg = false;
    skipWhitespace(in);
    if      (*in == '-') neg = true, ++in;
    else if (*in == '+') ++in;
    if (*in < '0' || *in > '9') fprintf(stderr, "PARSE ERROR! Unexpected char: %c\n", *in), exit(3);
    while (*in >= '0' && *in <= '9')
        val = val*10 + (*in - '0'),
        ++in;
    return neg ? -val : val; }

/**
 * @brief Match a string at current position (requires random access)
 *
 * Checks if the input matches the given string without consuming characters
 * unless there's a complete match. Requires random access iterator.
 *
 * @tparam B Input stream type (must support indexing)
 * @param in Input stream to check
 * @param str String to match
 * @return true if string matches (and input is advanced)
 * @return false if no match (input unchanged)
 */
template<class B>
static bool match(B& in, const char* str) {
    int i;
    for (i = 0; str[i] != '\0'; i++)
        if (in[i] != str[i])
            return false;

    in += i;

    return true;
}

/**
 * @brief Match a string eagerly (for forward-only iterators)
 *
 * Consumes characters from input while matching. If match fails,
 * characters have already been consumed. Use for forward-only iterators.
 *
 * @tparam B Input stream type
 * @param in Input stream to consume from
 * @param str String to match
 * @return true if string matches completely
 * @return false if match fails (partial input consumed)
 */
template<class B>
static bool eagerMatch(B& in, const char* str) {
    for (; *str != '\0'; ++str, ++in)
        if (*str != *in)
            return false;
    return true; }

/** @} */ // end of ParseUtils group

#endif
