/**
 * @file Feature.cc
 * @brief Implementation of Feature class
 *
 * This file contains the implementation of the Feature class which represents
 * nodes in the UVL feature tree. Features form a hierarchical tree structure
 * with parent-child relationships defined through Relation objects.
 *
 * @author UVL2Dimacs Team
 * @date 2024
 */

#include "Feature.hh"
#include <sstream>

/**
 * @brief Constructs a new feature with the given name
 *
 * Creates a feature with no parent (nullptr). The parent will be set
 * when this feature is added as a child to another feature's relation.
 *
 * @param feature_name The unique name identifying this feature
 */
Feature::Feature(const std::string& feature_name)
    : name(feature_name), parent(nullptr) {
}

/**
 * @brief Adds a relation to this feature
 *
 * Adds an existing relation object to this feature's relations and
 * automatically updates the parent pointers of all child features
 * in the relation to point to this feature.
 *
 * Implementation note: Uses shared_from_this() which requires the
 * Feature object to be managed by a shared_ptr.
 *
 * @param relation The relation to add (must have this feature as parent)
 */
void Feature::add_relation(std::shared_ptr<Relation> relation) {
    relations.push_back(relation);

    // Set this feature as parent for all children in the relation
    for (auto& child : relation->get_children()) {
        if (child) {
            child->set_parent(shared_from_this());
        }
    }
}

/**
 * @brief Creates and adds a new relation with specified children
 *
 * Convenience method that creates a new Relation object with this feature
 * as the parent and adds it to this feature's relations.
 *
 * @param children Vector of child features for the relation
 * @param card_min Minimum number of children that must be selected
 * @param card_max Maximum number of children that can be selected
 */
void Feature::add_relation(const std::vector<std::shared_ptr<Feature>>& children,
                          int card_min,
                          int card_max) {
    auto relation = std::make_shared<Relation>(shared_from_this(), children, card_min, card_max);
    add_relation(relation);
}

/**
 * @brief Collects all child features from all relations
 *
 * Flattens the children from all relations into a single vector.
 * This is useful for tree traversal operations.
 *
 * @return Vector containing all child features (may include duplicates if
 *         a child appears in multiple relations, though this is rare)
 */
std::vector<std::shared_ptr<Feature>> Feature::get_all_children() const {
    std::vector<std::shared_ptr<Feature>> all_children;

    for (const auto& relation : relations) {
        const auto& children = relation->get_children();
        all_children.insert(all_children.end(), children.begin(), children.end());
    }

    return all_children;
}

std::string Feature::to_string() const {
    std::ostringstream oss;
    oss << "Feature(" << name;

    if (parent) {
        oss << ", parent=" << parent->get_name();
    }

    if (!relations.empty()) {
        oss << ", " << relations.size() << " relation(s)";
    }

    oss << ")";

    return oss.str();
}

std::string Feature::tree_to_string(int indent) const {
    std::ostringstream oss;

    // Print indentation
    for (int i = 0; i < indent; ++i) {
        oss << "  ";
    }

    // Print feature name
    oss << name;

    // Print relation info if any
    if (!relations.empty()) {
        for (const auto& relation : relations) {
            oss << " [";
            switch (relation->get_type()) {
                case Relation::Type::MANDATORY: oss << "mandatory"; break;
                case Relation::Type::OPTIONAL: oss << "optional"; break;
                case Relation::Type::OR: oss << "or"; break;
                case Relation::Type::ALTERNATIVE: oss << "alternative"; break;
                case Relation::Type::CARDINALITY:
                    oss << relation->get_card_min() << ".." << relation->get_card_max();
                    break;
            }
            oss << "]";
        }
    }

    oss << "\n";

    // Recursively print children
    for (const auto& relation : relations) {
        for (const auto& child : relation->get_children()) {
            if (child) {
                oss << child->tree_to_string(indent + 1);
            }
        }
    }

    return oss.str();
}
