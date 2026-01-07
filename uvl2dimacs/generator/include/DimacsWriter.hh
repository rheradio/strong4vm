/**
 * @file DimacsWriter.hh
 * @brief Writer for DIMACS CNF file format
 *
 * This file defines the DimacsWriter class which converts a CNFModel to the
 * standard DIMACS CNF file format used by SAT solvers.
 *
 * @author UVL2Dimacs Team
 * @date 2024
 */

#ifndef DIMACSWRITER_H
#define DIMACSWRITER_H

#include "CNFModel.hh"
#include <string>
#include <ostream>

/**
 * @class DimacsWriter
 * @brief Writes CNF models to DIMACS format
 *
 * DimacsWriter converts a CNFModel to the standard DIMACS CNF format, which is
 * widely used by SAT solvers. The DIMACS format consists of:
 *
 * **Header Line**:
 * - Format: `p cnf <num_variables> <num_clauses>`
 * - Example: `p cnf 5 10` (5 variables, 10 clauses)
 *
 * **Comment Lines** (optional):
 * - Start with 'c' character
 * - Used for variable mappings and auxiliary variable descriptions
 * - Example: `c 1 GPS`, `c 2 Display`
 *
 * **Clause Lines**:
 * - Each clause on a separate line
 * - Literals separated by spaces
 * - Terminated with 0
 * - Positive integers for variables, negative for negations
 * - Example: `1 -2 0` represents (v1 | ~v2)
 *
 * @see CNFModel for the input CNF representation
 * @see FMToCNF for converting feature models to CNF
 *
 * Example output:
 * @code
 * c Variable mapping
 * c 1 GPS
 * c 2 Display
 * p cnf 2 2
 * 1 0
 * -1 2 0
 * @endcode
 */
class DimacsWriter {
private:
    const CNFModel& cnf_model;  ///< Reference to the CNF model to write

public:
    /**
     * @brief Constructs a DimacsWriter for the given CNF model
     *
     * @param model The CNF model to write (passed by const reference)
     */
    explicit DimacsWriter(const CNFModel& model);

    /**
     * @brief Destructor
     */
    ~DimacsWriter() = default;

    /**
     * @brief Writes the CNF model to a file in DIMACS format
     *
     * Creates or overwrites the file at the specified path with the DIMACS
     * representation of the CNF model.
     *
     * @param filepath Path to the output file
     * @throws std::runtime_error if the file cannot be opened for writing
     */
    void write_to_file(const std::string& filepath);

    /**
     * @brief Writes the CNF model to an output stream in DIMACS format
     *
     * @param out The output stream to write to (e.g., std::cout, file stream)
     */
    void write_to_stream(std::ostream& out);

    /**
     * @brief Converts the CNF model to a DIMACS format string
     *
     * @return String containing the complete DIMACS representation
     */
    std::string to_dimacs_string();
};

#endif // DIMACSWRITER_H
