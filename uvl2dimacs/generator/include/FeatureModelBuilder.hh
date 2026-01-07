/**
 * @file FeatureModelBuilder.hh
 * @brief Builder for constructing FeatureModel from UVL parse tree
 *
 * This file defines the FeatureModelBuilder class which implements an ANTLR listener
 * to construct a FeatureModel object from a UVL file parse tree.
 *
 * @author UVL2Dimacs Team
 * @date 2024
 */

#ifndef FEATUREMODELBUILDER_H
#define FEATUREMODELBUILDER_H

#include "FeatureModel.hh"
#include "ASTNode.hh"
#include "UVLCppParserBaseListener.h"
#include "UVLCppParser.h"
#include <stack>
#include <map>

/**
 * @class FeatureModelBuilder
 * @brief Builds a FeatureModel from ANTLR UVL parse tree
 *
 * FeatureModelBuilder is an ANTLR listener that walks through the parse tree generated
 * from a UVL file and constructs the corresponding FeatureModel object. It handles:
 *
 * - **Feature tree construction**: Creates Feature objects and establishes parent-child relationships
 * - **Relation parsing**: Interprets group types (mandatory, optional, OR, alternative, cardinality)
 * - **Constraint parsing**: Builds AST nodes for cross-tree constraints
 * - **Expression parsing**: Handles arithmetic and boolean expressions in constraints
 *
 * The builder uses the visitor/listener pattern to process parse tree nodes:
 * - `enter*` methods: Called when entering a parse tree node
 * - `exit*` methods: Called when exiting a parse tree node (after children processed)
 *
 * **Internal State**:
 * - Feature stack: Tracks the current feature hierarchy during tree construction
 * - AST stack: Builds abstract syntax trees for constraint expressions
 * - Constraint counter: Generates unique names for constraints
 *
 * @see FeatureModel for the output model structure
 * @see UVLCppParser for the ANTLR-generated parser
 * @see UVLCppParserBaseListener for the base listener interface
 *
 * Example usage:
 * @code
 * // Parse UVL file with ANTLR
 * UVLCppParser parser = ...;
 * FeatureModelBuilder builder;
 * antlr4::tree::ParseTreeWalker::DEFAULT.walk(&builder, parser.featureModel());
 * auto model = builder.get_feature_model();
 * @endcode
 */
class FeatureModelBuilder : public UVLCppParserBaseListener {
private:
    std::shared_ptr<FeatureModel> feature_model;              ///< The feature model being built
    std::shared_ptr<Feature> current_feature;                 ///< Current feature being processed
    std::stack<std::shared_ptr<Feature>> feature_stack;       ///< Stack tracking feature hierarchy

    std::stack<std::shared_ptr<ASTNode>> ast_stack;           ///< Stack for building constraint ASTs
    int constraint_counter;                                   ///< Counter for auto-naming constraints

public:
    /**
     * @brief Default constructor
     *
     * Initializes the builder with an empty state ready to process a UVL parse tree.
     */
    FeatureModelBuilder();

    /**
     * @brief Destructor
     */
    ~FeatureModelBuilder() = default;

    /**
     * @brief Gets the constructed feature model
     *
     * Should be called after the parse tree has been walked.
     *
     * @return Shared pointer to the built feature model
     */
    std::shared_ptr<FeatureModel> get_feature_model() const { return feature_model; }

    // Feature tree construction callbacks

    /**
     * @brief Called when exiting the features section
     * @param ctx Parse tree context for the features node
     */
    void exitFeatures(UVLCppParser::FeaturesContext *ctx) override;

    /**
     * @brief Called when entering a feature node
     * @param ctx Parse tree context for the feature node
     */
    void enterFeature(UVLCppParser::FeatureContext *ctx) override;

    /**
     * @brief Called when exiting a feature node
     * @param ctx Parse tree context for the feature node
     */
    void exitFeature(UVLCppParser::FeatureContext *ctx) override;

    // Relation group type callbacks

    /**
     * @brief Called when exiting an OR group (at least one child must be selected)
     * @param ctx Parse tree context for the OR group
     */
    void exitOrGroup(UVLCppParser::OrGroupContext *ctx) override;

    /**
     * @brief Called when exiting an alternative group (exactly one child must be selected)
     * @param ctx Parse tree context for the alternative group
     */
    void exitAlternativeGroup(UVLCppParser::AlternativeGroupContext *ctx) override;

    /**
     * @brief Called when exiting an optional group (zero or one child selected)
     * @param ctx Parse tree context for the optional group
     */
    void exitOptionalGroup(UVLCppParser::OptionalGroupContext *ctx) override;

    /**
     * @brief Called when exiting a mandatory group (exactly one child must be selected)
     * @param ctx Parse tree context for the mandatory group
     */
    void exitMandatoryGroup(UVLCppParser::MandatoryGroupContext *ctx) override;

    /**
     * @brief Called when exiting a cardinality group (min..max children selected)
     * @param ctx Parse tree context for the cardinality group
     */
    void exitCardinalityGroup(UVLCppParser::CardinalityGroupContext *ctx) override;

    // Constraint parsing callbacks

    /**
     * @brief Called when exiting a constraint line
     * @param ctx Parse tree context for the constraint line
     */
    void exitConstraintLine(UVLCppParser::ConstraintLineContext *ctx) override;

    // Boolean constraint operation callbacks

    /**
     * @brief Called when exiting a literal constraint (feature name)
     * @param ctx Parse tree context for the literal constraint
     */
    void exitLiteralConstraint(UVLCppParser::LiteralConstraintContext *ctx) override;

    /**
     * @brief Called when exiting a NOT constraint (~A)
     * @param ctx Parse tree context for the NOT constraint
     */
    void exitNotConstraint(UVLCppParser::NotConstraintContext *ctx) override;

    /**
     * @brief Called when exiting an AND constraint (A & B)
     * @param ctx Parse tree context for the AND constraint
     */
    void exitAndConstraint(UVLCppParser::AndConstraintContext *ctx) override;

    /**
     * @brief Called when exiting an OR constraint (A | B)
     * @param ctx Parse tree context for the OR constraint
     */
    void exitOrConstraint(UVLCppParser::OrConstraintContext *ctx) override;

    /**
     * @brief Called when exiting an implication constraint (A => B)
     * @param ctx Parse tree context for the implication constraint
     */
    void exitImplicationConstraint(UVLCppParser::ImplicationConstraintContext *ctx) override;

    /**
     * @brief Called when exiting an equivalence constraint (A <=> B)
     * @param ctx Parse tree context for the equivalence constraint
     */
    void exitEquivalenceConstraint(UVLCppParser::EquivalenceConstraintContext *ctx) override;

    /**
     * @brief Called when exiting a parenthesized constraint (A)
     * @param ctx Parse tree context for the parenthesis constraint
     */
    void exitParenthesisConstraint(UVLCppParser::ParenthesisConstraintContext *ctx) override;

    // Comparison operation callbacks

    /**
     * @brief Called when exiting an equation constraint
     * @param ctx Parse tree context for the equation constraint
     */
    void exitEquationConstraint(UVLCppParser::EquationConstraintContext *ctx) override;

    /**
     * @brief Called when exiting an equality equation (A == B)
     * @param ctx Parse tree context for the equal equation
     */
    void exitEqualEquation(UVLCppParser::EqualEquationContext *ctx) override;

    /**
     * @brief Called when exiting a less-than equation (A < B)
     * @param ctx Parse tree context for the lower equation
     */
    void exitLowerEquation(UVLCppParser::LowerEquationContext *ctx) override;

    /**
     * @brief Called when exiting a greater-than equation (A > B)
     * @param ctx Parse tree context for the greater equation
     */
    void exitGreaterEquation(UVLCppParser::GreaterEquationContext *ctx) override;

    /**
     * @brief Called when exiting a less-than-or-equal equation (A <= B)
     * @param ctx Parse tree context for the lower-equals equation
     */
    void exitLowerEqualsEquation(UVLCppParser::LowerEqualsEquationContext *ctx) override;

    /**
     * @brief Called when exiting a greater-than-or-equal equation (A >= B)
     * @param ctx Parse tree context for the greater-equals equation
     */
    void exitGreaterEqualsEquation(UVLCppParser::GreaterEqualsEquationContext *ctx) override;

    /**
     * @brief Called when exiting a not-equals equation (A != B)
     * @param ctx Parse tree context for the not-equals equation
     */
    void exitNotEqualsEquation(UVLCppParser::NotEqualsEquationContext *ctx) override;

    // Arithmetic expression callbacks

    /**
     * @brief Called when exiting an addition expression (A + B)
     * @param ctx Parse tree context for the add expression
     */
    void exitAddExpression(UVLCppParser::AddExpressionContext *ctx) override;

    /**
     * @brief Called when exiting a subtraction expression (A - B)
     * @param ctx Parse tree context for the sub expression
     */
    void exitSubExpression(UVLCppParser::SubExpressionContext *ctx) override;

    /**
     * @brief Called when exiting a multiplication expression (A * B)
     * @param ctx Parse tree context for the mul expression
     */
    void exitMulExpression(UVLCppParser::MulExpressionContext *ctx) override;

    /**
     * @brief Called when exiting a division expression (A / B)
     * @param ctx Parse tree context for the div expression
     */
    void exitDivExpression(UVLCppParser::DivExpressionContext *ctx) override;

    /**
     * @brief Called when exiting a float literal expression
     * @param ctx Parse tree context for the float literal
     */
    void exitFloatLiteralExpression(UVLCppParser::FloatLiteralExpressionContext *ctx) override;

    /**
     * @brief Called when exiting an integer literal expression
     * @param ctx Parse tree context for the integer literal
     */
    void exitIntegerLiteralExpression(UVLCppParser::IntegerLiteralExpressionContext *ctx) override;

    /**
     * @brief Called when exiting a string literal expression
     * @param ctx Parse tree context for the string literal
     */
    void exitStringLiteralExpression(UVLCppParser::StringLiteralExpressionContext *ctx) override;

    /**
     * @brief Called when exiting a literal expression (feature reference)
     * @param ctx Parse tree context for the literal expression
     */
    void exitLiteralExpression(UVLCppParser::LiteralExpressionContext *ctx) override;

private:
    /**
     * @brief Parses cardinality string from UVL syntax
     *
     * Parses strings like "[1..3]", "[2..*]" (unbounded max), or "[*..5]" (unbounded min).
     *
     * @param cardinality_text The cardinality string to parse
     * @return Pair of (min, max) cardinality values
     */
    std::pair<int, int> parse_cardinality(const std::string& cardinality_text);

    /**
     * @brief Extracts feature name from reference context
     *
     * Removes surrounding quotes if present.
     *
     * @param ctx Parse tree context for the reference
     * @return The feature name as a string
     */
    std::string get_reference_name(UVLCppParser::ReferenceContext *ctx);

    /**
     * @brief Collects child features from a group specification
     *
     * @param ctx Parse tree context for the group spec
     * @return Vector of child features in the group
     */
    std::vector<std::shared_ptr<Feature>> collect_children(UVLCppParser::GroupSpecContext *ctx);
};

#endif // FEATUREMODELBUILDER_H
