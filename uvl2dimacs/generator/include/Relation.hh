/**
 * @file Relation.hh
 * @brief Parent-child relationship in the feature tree
 *
 * This file defines the Relation class which represents the relationship between
 * a parent feature and its children, including cardinality constraints.
 *
 * @author UVL2Dimacs Team
 * @date 2024
 */

#ifndef RELATION_H
#define RELATION_H

#include <memory>
#include <vector>
#include <string>

// Forward declaration
class Feature;

/**
 * @class Relation
 * @brief Represents a parent-child relationship with cardinality constraints
 *
 * A Relation defines how a parent feature relates to a group of child features.
 * The relationship is characterized by cardinality constraints [min..max] that
 * specify how many children must/can be selected when the parent is selected.
 *
 * Standard UVL relation types:
 * - **MANDATORY**: Parent requires exactly one specific child (1..1, single child)
 * - **OPTIONAL**: Parent may have one specific child (0..1, single child)
 * - **OR**: Parent requires at least one child from group (1..N, multiple children)
 * - **ALTERNATIVE**: Parent requires exactly one child from group (1..1, multiple children)
 * - **CARDINALITY**: Custom min/max constraints (e.g., [2..5])
 *
 * @see Feature for the feature nodes
 * @see FeatureModel for the complete model
 *
 * Example:
 * @code
 * // Create an OR group: Car requires Engine OR ElectricMotor OR Both
 * auto car_to_propulsion = std::make_shared<Relation>(
 *     car,
 *     std::vector{engine, electric_motor},
 *     1,  // min: at least one
 *     2   // max: at most two (could select both)
 * );
 * // Type will be automatically determined as OR
 * @endcode
 */
class Relation {
public:
    /**
     * @enum Type
     * @brief Type of parent-child relation based on cardinality
     */
    enum class Type {
        MANDATORY,      ///< Single child, must be selected (1..1, n=1)
        OPTIONAL,       ///< Single child, may be selected (0..1, n=1)
        OR,             ///< Multiple children, at least one must be selected (1..n, n>1)
        ALTERNATIVE,    ///< Multiple children, exactly one must be selected (1..1, n>1)
        CARDINALITY     ///< Custom cardinality constraints (min..max)
    };

private:
    std::shared_ptr<Feature> parent;                          ///< Parent feature
    std::vector<std::shared_ptr<Feature>> children;           ///< Child features
    int card_min;                                             ///< Minimum children to select
    int card_max;                                             ///< Maximum children to select
    Type type;                                                ///< Relation type

public:
    /**
     * @brief Constructs a new relation
     *
     * The relation type is automatically determined from the cardinality
     * and number of children.
     *
     * @param parent_feature The parent feature
     * @param child_features Vector of child features
     * @param min_cardinality Minimum number of children that must be selected
     * @param max_cardinality Maximum number of children that can be selected
     */
    Relation(std::shared_ptr<Feature> parent_feature,
             const std::vector<std::shared_ptr<Feature>>& child_features,
             int min_cardinality,
             int max_cardinality);

    /**
     * @brief Copy constructor
     * @param other The relation to copy from
     */
    Relation(const Relation& other);

    /**
     * @brief Assignment operator
     * @param other The relation to assign from
     * @return Reference to this relation
     */
    Relation& operator=(const Relation& other);

    /**
     * @brief Destructor
     */
    ~Relation() = default;

    /**
     * @brief Gets the parent feature
     * @return Shared pointer to parent feature
     */
    std::shared_ptr<Feature> get_parent() const { return parent; }

    /**
     * @brief Gets all child features
     * @return Vector of child features
     */
    const std::vector<std::shared_ptr<Feature>>& get_children() const { return children; }

    /**
     * @brief Gets the minimum cardinality
     * @return Minimum number of children that must be selected
     */
    int get_card_min() const { return card_min; }

    /**
     * @brief Gets the maximum cardinality
     * @return Maximum number of children that can be selected
     */
    int get_card_max() const { return card_max; }

    /**
     * @brief Gets the relation type
     * @return The type of this relation
     */
    Type get_type() const { return type; }

    /**
     * @brief Checks if this is a mandatory relation
     * @return true if type is MANDATORY
     */
    bool is_mandatory() const { return type == Type::MANDATORY; }

    /**
     * @brief Checks if this is an optional relation
     * @return true if type is OPTIONAL
     */
    bool is_optional() const { return type == Type::OPTIONAL; }

    /**
     * @brief Checks if this is an OR relation
     * @return true if type is OR
     */
    bool is_or() const { return type == Type::OR; }

    /**
     * @brief Checks if this is an alternative relation
     * @return true if type is ALTERNATIVE
     */
    bool is_alternative() const { return type == Type::ALTERNATIVE; }

    /**
     * @brief Checks if this is a cardinality relation
     * @return true if type is CARDINALITY
     */
    bool is_cardinality() const { return type == Type::CARDINALITY; }

    /**
     * @brief Creates a string representation of this relation
     * @return String representation for debugging
     */
    std::string to_string() const;

private:
    /**
     * @brief Determines the relation type from cardinality and children count
     *
     * Type determination rules:
     * - MANDATORY: min=1, max=1, single child
     * - OPTIONAL: min=0, max=1, single child
     * - OR: min=1, max=N (N = child count), multiple children
     * - ALTERNATIVE: min=1, max=1, multiple children
     * - CARDINALITY: any other combination
     *
     * @return The determined relation type
     */
    Type determine_type() const;
};

#endif // RELATION_H
