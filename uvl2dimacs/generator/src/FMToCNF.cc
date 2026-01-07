/**
 * @file FMToCNF.cc
 * @brief Implementation of Feature Model to CNF transformation
 *
 * This file implements the FMToCNF class which orchestrates the complete
 * transformation of a feature model to CNF format. The transformation includes:
 *
 * 1. **Feature variables**: Each feature becomes a boolean variable
 * 2. **Root constraint**: Root feature must be selected
 * 3. **Relation constraints**: Parent-child relationships encoded to CNF
 * 4. **Cross-tree constraints**: Boolean expressions converted to CNF
 *
 * The transformation supports two CNF conversion modes:
 * - STRAIGHTFORWARD: Direct conversion (may produce long clauses)
 * - TSEITIN: Uses auxiliary variables (shorter clauses, more variables)
 *
 * @author UVL2Dimacs Team
 * @date 2024
 */

#include "FMToCNF.hh"
#include "RelationEncoder.hh"
#include <stdexcept>

/**
 * @brief Constructs a transformer for the given feature model
 *
 * @param model The feature model to transform
 */
FMToCNF::FMToCNF(std::shared_ptr<FeatureModel> model)
    : source_model(model), mode(CNFMode::STRAIGHTFORWARD) {
}

/**
 * @brief Transforms the feature model to CNF
 *
 * Performs the complete transformation in four steps:
 * 1. Add all features as CNF variables
 * 2. Add root constraint (root must be true)
 * 3. Encode all parent-child relations
 * 4. Convert cross-tree constraints to CNF
 *
 * @param conversion_mode CNF conversion mode (STRAIGHTFORWARD or TSEITIN)
 * @return CNF model ready for DIMACS output
 */
CNFModel FMToCNF::transform(CNFMode conversion_mode) {
    // Store the conversion mode
    mode = conversion_mode;

    // Step 1: Add all features as variables
    add_features();

    // Step 2: Add root constraint (root must be true)
    add_root();

    // Step 3: Add relation constraints
    add_relations();

    // Step 4: Add cross-tree constraints
    add_constraints();

    return cnf_model;
}

/**
 * @brief Adds all features as CNF variables
 *
 * Step 1 of transformation: Creates a boolean variable for each feature
 * in the feature model. Variable IDs are assigned sequentially starting from 1.
 */
void FMToCNF::add_features() {
    auto features = source_model->get_features();

    for (const auto& feature : features) {
        cnf_model.add_feature(feature->get_name());
    }
}

/**
 * @brief Adds the root constraint
 *
 * Step 2 of transformation: Adds a unit clause asserting that the root
 * feature must be selected. This is a fundamental requirement of feature models.
 *
 * @throws std::runtime_error if feature model has no root
 */
void FMToCNF::add_root() {
    auto root = source_model->get_root();
    if (!root) {
        throw std::runtime_error("Feature model has no root");
    }

    int root_var = cnf_model.get_variable(root->get_name());
    cnf_model.add_clause({root_var});
}

/**
 * @brief Encodes all parent-child relations to CNF
 *
 * Step 3 of transformation: Uses RelationEncoder to convert each relation
 * (MANDATORY, OPTIONAL, OR, ALTERNATIVE, CARDINALITY) into CNF clauses.
 */
void FMToCNF::add_relations() {
    RelationEncoder encoder(cnf_model);

    auto relations = source_model->get_relations();
    for (const auto& relation : relations) {
        encoder.encode_relation(relation);
    }
}

/**
 * @brief Converts cross-tree constraints to CNF
 *
 * Step 4 of transformation: Processes each cross-tree constraint and converts
 * it to CNF clauses. Non-boolean constraints (arithmetic, comparison) are
 * silently skipped as they cannot be represented in pure CNF.
 *
 * The conversion mode (STRAIGHTFORWARD or TSEITIN) is passed to each constraint
 * to determine how boolean operations are encoded.
 */
void FMToCNF::add_constraints() {
    const auto& constraints = source_model->get_constraints();

    int total_constraints = constraints.size();
    int skipped_constraints = 0;

    for (const auto& constraint : constraints) {
        // Skip non-boolean constraints (comparison, arithmetic)
        // These cannot be represented in CNF for SAT solvers
        if (!constraint->is_pure_boolean()) {
            skipped_constraints++;
            continue;
        }

        // Create lambda functions for variable lookup and auxiliary variable creation
        auto get_variable = [this](const std::string& name) -> int {
            if (!cnf_model.has_variable(name)) {
                throw std::runtime_error("Constraint references undefined feature: " + name);
            }
            return cnf_model.get_variable(name);
        };

        auto create_aux_var = [this]() -> int {
            return cnf_model.create_auxiliary_variable();
        };

        // Get clauses from constraint and add to CNF model
        auto clauses = constraint->get_clauses(get_variable, create_aux_var, mode);
        for (const auto& clause : clauses) {
            cnf_model.add_clause(clause);
        }
    }

    // Report skipped constraints if any
    if (skipped_constraints > 0) {
        // Note: In a production system, this would use a proper logging mechanism
        // For now, we silently skip (error reporting happens at a higher level)
    }
}
