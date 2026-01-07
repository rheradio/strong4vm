/**
 * @file FMToCNF.hh
 * @brief Transformer from Feature Model to CNF
 *
 * This file defines the FMToCNF class which converts a feature model (tree structure
 * with relations and constraints) into a CNF (Conjunctive Normal Form) formula suitable
 * for SAT solving.
 *
 * @author UVL2Dimacs Team
 * @date 2024
 */

#ifndef FMTOCNF_H
#define FMTOCNF_H

#include "FeatureModel.hh"
#include "CNFModel.hh"
#include "CNFMode.hh"
#include <memory>

/**
 * @class FMToCNF
 * @brief Transforms a feature model into a CNF formula
 *
 * FMToCNF converts a hierarchical feature model with relations and cross-tree constraints
 * into an equivalent CNF (Conjunctive Normal Form) formula. The transformation process:
 *
 * 1. **Feature Variables**: Each feature becomes a boolean variable
 * 2. **Root Constraint**: Root feature must be selected (unit clause)
 * 3. **Relation Encoding**: Parent-child relations become CNF clauses
 *    - Mandatory: parent <=> child
 *    - Optional: child => parent
 *    - OR: parent => (at least one child)
 *    - Alternative: parent => (exactly one child)
 *    - Cardinality: parent => (min..max children)
 * 4. **Cross-tree Constraints**: Constraint expressions converted to CNF clauses
 *
 * The transformation supports two CNF conversion modes:
 * - **STRAIGHTFORWARD**: Direct conversion without auxiliary variables
 * - **TSEITIN**: Tseitin transformation with auxiliary variables (3-CNF)
 *
 * @see FeatureModel for the input feature model structure
 * @see CNFModel for the output CNF representation
 * @see CNFMode for conversion mode options
 * @see RelationEncoder for relation-to-CNF encoding
 *
 * Example usage:
 * @code
 * auto feature_model = std::make_shared<FeatureModel>(root_feature);
 * FMToCNF transformer(feature_model);
 * CNFModel cnf = transformer.transform(CNFMode::STRAIGHTFORWARD);
 * @endcode
 */
class FMToCNF {
private:
    std::shared_ptr<FeatureModel> source_model;  ///< The feature model to convert
    CNFModel cnf_model;                          ///< The resulting CNF model
    CNFMode mode;                                ///< Conversion mode for constraints

public:
    /**
     * @brief Constructs a transformer for the given feature model
     *
     * @param model Shared pointer to the feature model to transform
     */
    explicit FMToCNF(std::shared_ptr<FeatureModel> model);

    /**
     * @brief Destructor
     */
    ~FMToCNF() = default;

    /**
     * @brief Transforms the feature model to CNF
     *
     * Performs the complete transformation from feature model to CNF formula:
     * 1. Adds all features as variables
     * 2. Adds root constraint (root must be true)
     * 3. Encodes all parent-child relations
     * 4. Converts all cross-tree constraints
     *
     * @param conversion_mode CNF conversion mode (STRAIGHTFORWARD or TSEITIN)
     * @return The resulting CNF model
     *
     * @see CNFMode for mode descriptions
     */
    CNFModel transform(CNFMode conversion_mode = CNFMode::STRAIGHTFORWARD);

private:
    /**
     * @brief Adds all features as variables to the CNF model
     *
     * Traverses the feature tree and creates a variable for each feature.
     */
    void add_features();

    /**
     * @brief Adds the root constraint
     *
     * Adds a unit clause asserting that the root feature must be selected.
     * This ensures at least one valid configuration exists.
     */
    void add_root();

    /**
     * @brief Encodes all parent-child relations as CNF clauses
     *
     * Converts each relation (mandatory, optional, OR, alternative, cardinality)
     * into the corresponding CNF clauses.
     *
     * @see RelationEncoder for encoding details
     */
    void add_relations();

    /**
     * @brief Converts all cross-tree constraints to CNF clauses
     *
     * Converts each constraint expression to CNF using the specified mode.
     */
    void add_constraints();
};

#endif // FMTOCNF_H
