/**
 * @file Feature.hh
 * @brief Feature node in the UVL feature tree
 *
 * This file defines the Feature class which represents a node in the feature model tree.
 * Each feature can have a parent and multiple child relations defining the tree structure.
 *
 * @author UVL2Dimacs Team
 * @date 2024
 */

#ifndef FEATURE_H
#define FEATURE_H

#include "Relation.hh"
#include <string>
#include <vector>
#include <memory>

/**
 * @class Feature
 * @brief Represents a feature node in the UVL feature tree
 *
 * A Feature is a node in the hierarchical feature model tree. Each feature has:
 * - A unique name
 * - An optional parent feature
 * - Zero or more child relations (defining how children are related)
 *
 * Features are the basic building blocks of variability models, representing
 * configurable aspects of a software product line.
 *
 * @see Relation for parent-child relationship types
 * @see FeatureModel for the complete feature model
 *
 * Example:
 * @code
 * auto root = std::make_shared<Feature>("Car");
 * auto engine = std::make_shared<Feature>("Engine");
 * root->add_relation({engine}, 1, 1);  // Mandatory child
 * @endcode
 */
class Feature : public std::enable_shared_from_this<Feature> {
private:
    std::string name;                                          ///< Unique name of this feature
    std::shared_ptr<Feature> parent;                           ///< Parent feature (nullptr for root)
    std::vector<std::shared_ptr<Relation>> relations;          ///< Child relations

public:
    /**
     * @brief Constructs a new feature with the given name
     * @param feature_name The name of this feature
     */
    explicit Feature(const std::string& feature_name);

    /**
     * @brief Destructor
     */
    ~Feature() = default;

    /**
     * @brief Gets the name of this feature
     * @return The feature name
     */
    const std::string& get_name() const { return name; }

    /**
     * @brief Gets the parent feature
     * @return Shared pointer to parent feature (nullptr if this is root)
     */
    std::shared_ptr<Feature> get_parent() const { return parent; }

    /**
     * @brief Gets all child relations of this feature
     * @return Vector of relations where this feature is the parent
     */
    const std::vector<std::shared_ptr<Relation>>& get_relations() const { return relations; }

    /**
     * @brief Sets the parent feature
     * @param parent_feature The new parent feature
     */
    void set_parent(std::shared_ptr<Feature> parent_feature) { parent = parent_feature; }

    /**
     * @brief Adds an existing relation to this feature
     * @param relation The relation to add (must have this feature as parent)
     */
    void add_relation(std::shared_ptr<Relation> relation);

    /**
     * @brief Creates and adds a new relation with specified children
     *
     * Creates a new Relation object with this feature as parent and the
     * specified children and cardinality constraints.
     *
     * @param children Vector of child features
     * @param card_min Minimum number of children that must be selected
     * @param card_max Maximum number of children that can be selected
     *
     * @see Relation for cardinality semantics
     */
    void add_relation(const std::vector<std::shared_ptr<Feature>>& children,
                     int card_min,
                     int card_max);

    /**
     * @brief Gets all child features across all relations
     *
     * Collects all children from all relations of this feature into a single vector.
     *
     * @return Vector containing all child features
     */
    std::vector<std::shared_ptr<Feature>> get_all_children() const;

    /**
     * @brief Checks if this is a leaf feature
     * @return true if this feature has no child relations (leaf node)
     */
    bool is_leaf() const { return relations.empty(); }

    /**
     * @brief Creates a string representation of this feature
     * @return String representation for debugging (format: "Feature(name)")
     */
    std::string to_string() const;

    /**
     * @brief Creates a string representation of the feature tree
     *
     * Recursively builds a string showing this feature and all descendants
     * with proper indentation.
     *
     * @param indent Current indentation level (number of spaces)
     * @return Multi-line string showing the feature tree
     */
    std::string tree_to_string(int indent = 0) const;
};

#endif // FEATURE_H
