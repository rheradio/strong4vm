/**
 * @file Relation.cc
 * @brief Implementation of Relation class
 *
 * This file implements the Relation class which represents parent-child
 * relationships in the feature tree with cardinality constraints.
 *
 * The type of relation (MANDATORY, OPTIONAL, OR, ALTERNATIVE, CARDINALITY)
 * is automatically determined from the cardinality and number of children.
 *
 * @author UVL2Dimacs Team
 * @date 2024
 */

#include "Relation.hh"
#include "Feature.hh"
#include <sstream>

/**
 * @brief Constructs a new relation with cardinality constraints
 *
 * Creates a relation between a parent and multiple children with specified
 * cardinality. The relation type is automatically determined based on the
 * cardinality values and number of children.
 *
 * @param parent_feature The parent feature
 * @param child_features Vector of child features
 * @param min_cardinality Minimum number of children to select (0 to N)
 * @param max_cardinality Maximum number of children to select (1 to N)
 */
Relation::Relation(std::shared_ptr<Feature> parent_feature,
                   const std::vector<std::shared_ptr<Feature>>& child_features,
                   int min_cardinality,
                   int max_cardinality)
    : parent(parent_feature),
      children(child_features),
      card_min(min_cardinality),
      card_max(max_cardinality) {
    type = determine_type();
}

/**
 * @brief Copy constructor
 *
 * Creates a shallow copy of the relation. Feature pointers are copied,
 * not the features themselves.
 *
 * @param other The relation to copy from
 */
Relation::Relation(const Relation& other)
    : parent(other.parent),
      children(other.children),
      card_min(other.card_min),
      card_max(other.card_max),
      type(other.type) {
}

/**
 * @brief Assignment operator
 *
 * Performs shallow copy of relation data. Includes self-assignment check.
 *
 * @param other The relation to assign from
 * @return Reference to this relation
 */
Relation& Relation::operator=(const Relation& other) {
    if (this != &other) {
        parent = other.parent;
        children = other.children;
        card_min = other.card_min;
        card_max = other.card_max;
        type = other.type;
    }
    return *this;
}

/**
 * @brief Determines the relation type from cardinality and children count
 *
 * Classification rules:
 * - MANDATORY: Single child, [1..1] - child must be selected
 * - OPTIONAL: Single child, [0..1] - child may be selected
 * - ALTERNATIVE: Multiple children, [1..1] - exactly one must be selected
 * - OR: Multiple children, [1..N] - at least one must be selected
 * - CARDINALITY: Any other combination (e.g., [2..5])
 *
 * @return The determined relation type
 */
Relation::Type Relation::determine_type() const {
    size_t num_children = children.size();

    // Mandatory: 1 child, [1..1]
    if (num_children == 1 && card_min == 1 && card_max == 1) {
        return Type::MANDATORY;
    }

    // Optional: 1 child, [0..1]
    if (num_children == 1 && card_min == 0 && card_max == 1) {
        return Type::OPTIONAL;
    }

    // Alternative: multiple children, [1..1]
    if (num_children > 1 && card_min == 1 && card_max == 1) {
        return Type::ALTERNATIVE;
    }

    // Or: multiple children, [1..N] where N is number of children
    if (num_children > 1 && card_min == 1 && card_max == static_cast<int>(num_children)) {
        return Type::OR;
    }

    // Everything else is a general cardinality constraint
    return Type::CARDINALITY;
}

std::string Relation::to_string() const {
    std::ostringstream oss;

    // Get parent name
    std::string parent_name = parent ? parent->get_name() : "NULL";

    oss << "Relation(" << parent_name << " -> [";

    // Add children names
    for (size_t i = 0; i < children.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << (children[i] ? children[i]->get_name() : "NULL");
    }

    oss << "], [" << card_min << ".." << card_max << "], type=";

    // Add type
    switch (type) {
        case Type::MANDATORY: oss << "MANDATORY"; break;
        case Type::OPTIONAL: oss << "OPTIONAL"; break;
        case Type::OR: oss << "OR"; break;
        case Type::ALTERNATIVE: oss << "ALTERNATIVE"; break;
        case Type::CARDINALITY: oss << "CARDINALITY"; break;
    }

    oss << ")";

    return oss.str();
}
