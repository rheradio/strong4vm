/**
 * @file FeatureModel.hh
 * @brief Complete feature model representation
 *
 * This file defines the FeatureModel class which represents a complete UVL feature model
 * including the feature tree hierarchy, relations, and cross-tree constraints.
 *
 * @author UVL2Dimacs Team
 * @date 2024
 */

#ifndef FEATUREMODEL_H
#define FEATUREMODEL_H

#include "Feature.hh"
#include "Constraint.hh"
#include <string>
#include <vector>
#include <memory>
#include <map>

/**
 * @class FeatureModel
 * @brief Represents a complete UVL feature model
 *
 * A FeatureModel contains all elements of a UVL variability model:
 *
 * - **Feature Tree**: Hierarchical tree of features starting from a root
 * - **Relations**: Parent-child relationships with cardinality constraints
 * - **Cross-tree Constraints**: Propositional constraints spanning multiple features
 *
 * The feature model represents the variability space of a software product line,
 * where each valid configuration corresponds to a selection of features that
 * satisfies all relations and constraints.
 *
 * **Key Components**:
 * - Root feature: The top-level feature (always selected)
 * - Feature tree: Hierarchical structure of features
 * - Constraints: Additional logical constraints between features
 * - Feature map: Fast lookup cache for finding features by name
 *
 * @see Feature for individual feature nodes
 * @see Relation for parent-child relationships
 * @see Constraint for cross-tree constraints
 * @see FMToCNF for converting to CNF
 *
 * Example:
 * @code
 * auto root = std::make_shared<Feature>("Car");
 * auto model = std::make_shared<FeatureModel>(root);
 * auto engine = std::make_shared<Feature>("Engine");
 * root->add_relation({engine}, 1, 1);  // Mandatory
 * model->build_feature_map();
 * @endcode
 */
class FeatureModel {
private:
    std::shared_ptr<Feature> root;                            ///< Root feature of the tree
    std::vector<std::shared_ptr<Constraint>> constraints;     ///< Cross-tree constraints

    std::map<std::string, std::shared_ptr<Feature>> feature_map;  ///< Feature name lookup cache

public:
    /**
     * @brief Constructs a feature model with the given root feature
     *
     * @param root_feature The root feature of the feature tree
     */
    explicit FeatureModel(std::shared_ptr<Feature> root_feature);

    /**
     * @brief Destructor
     */
    ~FeatureModel() = default;

    /**
     * @brief Gets the root feature
     * @return Shared pointer to the root feature
     */
    std::shared_ptr<Feature> get_root() const { return root; }

    /**
     * @brief Gets all cross-tree constraints
     * @return Constant reference to the constraints vector
     */
    const std::vector<std::shared_ptr<Constraint>>& get_constraints() const { return constraints; }

    /**
     * @brief Adds a cross-tree constraint to the model
     *
     * @param constraint Shared pointer to the constraint to add
     */
    void add_constraint(std::shared_ptr<Constraint> constraint);

    /**
     * @brief Gets all features in the model
     *
     * Performs a depth-first traversal of the feature tree to collect
     * all features.
     *
     * @return Vector of all features in the tree
     */
    std::vector<std::shared_ptr<Feature>> get_features() const;

    /**
     * @brief Gets all relations in the model
     *
     * Collects all parent-child relations from all features in the tree.
     *
     * @return Vector of all relations
     */
    std::vector<std::shared_ptr<Relation>> get_relations() const;

    /**
     * @brief Finds a feature by name
     *
     * Uses the feature map cache for fast lookup. If the feature map hasn't
     * been built, this method will build it first.
     *
     * @param name The name of the feature to find
     * @return Shared pointer to the feature, or nullptr if not found
     *
     * @see build_feature_map()
     */
    std::shared_ptr<Feature> find_feature(const std::string& name);

    /**
     * @brief Builds the feature name lookup cache
     *
     * Traverses the feature tree and creates a map from feature names to
     * feature objects for fast lookup. Should be called after the model
     * construction is complete.
     *
     * @note This is automatically called by find_feature() if needed
     */
    void build_feature_map();

    /**
     * @brief Creates a string representation of the feature model
     *
     * Includes the feature tree structure and all constraints.
     *
     * @return String representation for debugging
     */
    std::string to_string() const;

private:
    /**
     * @brief Helper method to recursively collect features
     *
     * Performs depth-first traversal to collect all features in the tree.
     *
     * @param feature Current feature being processed
     * @param features Output vector to append features to
     */
    void collect_features(std::shared_ptr<Feature> feature,
                         std::vector<std::shared_ptr<Feature>>& features) const;

    /**
     * @brief Helper method to recursively build feature map
     *
     * Traverses the tree and adds each feature to the feature map.
     *
     * @param feature Current feature being processed
     */
    void build_feature_map_recursive(std::shared_ptr<Feature> feature);
};

#endif // FEATUREMODEL_H
