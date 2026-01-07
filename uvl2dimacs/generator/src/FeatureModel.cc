/**
 * @file FeatureModel.cc
 * @brief Implementation of Feature Model representation
 *
 * This file implements the FeatureModel class which represents a complete
 * UVL feature model. It includes:
 * - Feature tree (hierarchical structure of features and relations)
 * - Cross-tree constraints (boolean expressions over features)
 * - Feature lookup map for efficient feature name resolution
 *
 * The feature model is the central data structure that is converted to CNF
 * for SAT solver analysis.
 *
 * @author UVL2Dimacs Team
 * @date 2024
 */

#include "FeatureModel.hh"
#include <sstream>
#include <queue>

/**
 * @brief Constructs a feature model with the given root feature
 *
 * Initializes the feature model and builds an internal lookup map
 * for efficient feature name resolution.
 *
 * @param root_feature Root feature of the feature tree
 */
FeatureModel::FeatureModel(std::shared_ptr<Feature> root_feature)
    : root(root_feature) {
    // Build the feature map for efficient lookup
    build_feature_map();
}

/**
 * @brief Adds a cross-tree constraint to the feature model
 *
 * Cross-tree constraints are boolean expressions that span across
 * different branches of the feature tree (e.g., "A implies B").
 *
 * @param constraint The constraint to add
 */
void FeatureModel::add_constraint(std::shared_ptr<Constraint> constraint) {
    constraints.push_back(constraint);
}

/**
 * @brief Collects all features in the model
 *
 * Traverses the feature tree in depth-first order and returns
 * all features as a flat list.
 *
 * @return Vector containing all features in the model
 */
std::vector<std::shared_ptr<Feature>> FeatureModel::get_features() const {
    std::vector<std::shared_ptr<Feature>> features;
    if (root) {
        collect_features(root, features);
    }
    return features;
}

/**
 * @brief Recursively collects features from the tree
 *
 * Helper method for get_features() that performs depth-first traversal
 * of the feature tree, adding each feature to the output vector.
 *
 * @param feature Current feature to process
 * @param features Output vector to append features to
 */
void FeatureModel::collect_features(std::shared_ptr<Feature> feature,
                                    std::vector<std::shared_ptr<Feature>>& features) const {
    if (!feature) {
        return;
    }

    features.push_back(feature);

    // Recursively collect children
    for (const auto& relation : feature->get_relations()) {
        for (const auto& child : relation->get_children()) {
            collect_features(child, features);
        }
    }
}

/**
 * @brief Collects all relations in the model
 *
 * Traverses all features and gathers their relations into a single vector.
 * Each relation defines the parent-child relationship and cardinality
 * constraints.
 *
 * @return Vector containing all relations in the model
 */
std::vector<std::shared_ptr<Relation>> FeatureModel::get_relations() const {
    std::vector<std::shared_ptr<Relation>> relations;

    auto features = get_features();
    for (const auto& feature : features) {
        const auto& feature_relations = feature->get_relations();
        relations.insert(relations.end(), feature_relations.begin(), feature_relations.end());
    }

    return relations;
}

/**
 * @brief Finds a feature by name
 *
 * Uses the internal feature map for O(1) lookup by feature name.
 *
 * @param name Feature name to search for
 * @return Pointer to feature if found, nullptr otherwise
 */
std::shared_ptr<Feature> FeatureModel::find_feature(const std::string& name) {
    auto it = feature_map.find(name);
    if (it != feature_map.end()) {
        return it->second;
    }
    return nullptr;
}

/**
 * @brief Builds the internal feature name lookup map
 *
 * Clears and rebuilds the feature map by traversing the entire tree.
 * Called automatically by the constructor.
 */
void FeatureModel::build_feature_map() {
    feature_map.clear();
    if (root) {
        build_feature_map_recursive(root);
    }
}

/**
 * @brief Recursively builds the feature map
 *
 * Helper method that traverses the feature tree and populates
 * the feature_map with name-to-feature mappings.
 *
 * @param feature Current feature to add and process
 */
void FeatureModel::build_feature_map_recursive(std::shared_ptr<Feature> feature) {
    if (!feature) {
        return;
    }

    feature_map[feature->get_name()] = feature;

    // Recursively process children
    for (const auto& relation : feature->get_relations()) {
        for (const auto& child : relation->get_children()) {
            build_feature_map_recursive(child);
        }
    }
}

/**
 * @brief Creates a human-readable string representation of the model
 *
 * Generates a multi-line string showing:
 * - Statistics (number of features, relations, constraints)
 * - Feature tree structure with indentation
 * - List of cross-tree constraints
 *
 * @return String representation for debugging and visualization
 */
std::string FeatureModel::to_string() const {
    std::ostringstream oss;

    oss << "FeatureModel:\n";
    oss << "  Features: " << get_features().size() << "\n";
    oss << "  Relations: " << get_relations().size() << "\n";
    oss << "  Constraints: " << constraints.size() << "\n";

    if (root) {
        oss << "\nFeature Tree:\n";
        oss << root->tree_to_string();
    }

    if (!constraints.empty()) {
        oss << "\nConstraints:\n";
        for (const auto& constraint : constraints) {
            oss << "  " << constraint->to_string() << "\n";
        }
    }

    return oss.str();
}
