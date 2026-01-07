/**
 * @file FeatureModelBuilder.cc
 * @brief Implementation of ANTLR listener for building feature models from UVL
 *
 * This file implements the FeatureModelBuilder class which is an ANTLR4 parse
 * tree listener that constructs a FeatureModel from UVL source code during parsing.
 *
 * The builder uses two primary data structures:
 * - **feature_stack**: Stack of features being constructed (for hierarchy)
 * - **ast_stack**: Stack of AST nodes for constraint expressions
 *
 * The listener pattern works as follows:
 * 1. Parser invokes enter/exit methods for each grammar rule
 * 2. Builder accumulates features and relations during tree traversal
 * 3. After parsing completes, get_feature_model() returns the complete model
 *
 * Key responsibilities:
 * - Building feature tree hierarchy with parent-child relations
 * - Parsing cardinality specifications ([min..max])
 * - Converting constraint expressions to AST nodes
 * - Handling all relation types (MANDATORY, OPTIONAL, OR, ALTERNATIVE, CARDINALITY)
 *
 * @author UVL2Dimacs Team
 * @date 2024
 */

#include "FeatureModelBuilder.hh"
#include <stdexcept>
#include <algorithm>

/**
 * @brief Constructs a new feature model builder
 *
 * Initializes the builder with empty stacks and counters.
 * The feature model will be built during ANTLR parsing.
 */
FeatureModelBuilder::FeatureModelBuilder()
    : feature_model(nullptr), current_feature(nullptr), constraint_counter(0) {
}

/**
 * @brief Called when exiting the features section
 *
 * Finalizes the feature tree by popping the root feature from the stack
 * and creating the FeatureModel object.
 *
 * @param ctx Parse tree context for the features section
 */
void FeatureModelBuilder::exitFeatures(UVLCppParser::FeaturesContext *ctx) {
    // The root feature should be on the stack
    if (!feature_stack.empty()) {
        auto root = feature_stack.top();
        feature_model = std::make_shared<FeatureModel>(root);
        feature_stack.pop();
    }
}

/**
 * @brief Called when entering a feature definition
 *
 * Creates a new Feature object and pushes it onto the stack for processing.
 * The feature will be linked to its parent when the parent's relation is processed.
 *
 * @param ctx Parse tree context containing feature name
 */
void FeatureModelBuilder::enterFeature(UVLCppParser::FeatureContext *ctx) {
    // Get feature name
    std::string feature_name = get_reference_name(ctx->reference());

    // Create new feature
    auto feature = std::make_shared<Feature>(feature_name);

    // Push to stack for processing
    feature_stack.push(feature);
    current_feature = feature;
}

/**
 * @brief Called when exiting a feature definition
 *
 * Marks the current feature as complete. The feature remains on the stack
 * until its parent's relation processing collects it as a child.
 *
 * @param ctx Parse tree context for the feature
 */
void FeatureModelBuilder::exitFeature(UVLCppParser::FeatureContext *ctx) {
    // Current feature is complete
    // Keep it on stack until parent processes it
    current_feature = nullptr;
}

/**
 * @brief Processes OR group: parent → (at least one child)
 * @param ctx Parse tree context for the OR group
 */
void FeatureModelBuilder::exitOrGroup(UVLCppParser::OrGroupContext *ctx) {
    auto children = collect_children(ctx->groupSpec());

    if (!children.empty() && !feature_stack.empty()) {
        // Get the parent feature (should be on top of stack)
        auto parent = feature_stack.top();

        // Create Or relation: [1..N]
        int num_children = children.size();
        parent->add_relation(children, 1, num_children);
    }
}

/**
 * @brief Processes ALTERNATIVE group: parent → (exactly one child)
 * @param ctx Parse tree context for the alternative group
 */
void FeatureModelBuilder::exitAlternativeGroup(UVLCppParser::AlternativeGroupContext *ctx) {
    auto children = collect_children(ctx->groupSpec());

    if (!children.empty() && !feature_stack.empty()) {
        auto parent = feature_stack.top();

        // Create Alternative relation: [1..1]
        parent->add_relation(children, 1, 1);
    }
}

/**
 * @brief Processes OPTIONAL group: each child → parent (optional selection)
 * @param ctx Parse tree context for the optional group
 */
void FeatureModelBuilder::exitOptionalGroup(UVLCppParser::OptionalGroupContext *ctx) {
    auto children = collect_children(ctx->groupSpec());

    if (!feature_stack.empty()) {
        auto parent = feature_stack.top();

        // Create Optional relations: [0..1] for each child
        for (auto& child : children) {
            parent->add_relation({child}, 0, 1);
        }
    }
}

/**
 * @brief Processes MANDATORY group: each child ⟺ parent (required selection)
 * @param ctx Parse tree context for the mandatory group
 */
void FeatureModelBuilder::exitMandatoryGroup(UVLCppParser::MandatoryGroupContext *ctx) {
    auto children = collect_children(ctx->groupSpec());

    if (!feature_stack.empty()) {
        auto parent = feature_stack.top();

        // Create Mandatory relations: [1..1] for each child
        for (auto& child : children) {
            parent->add_relation({child}, 1, 1);
        }
    }
}

/**
 * @brief Processes CARDINALITY group: parent → (min..max children selected)
 * @param ctx Parse tree context containing cardinality specification
 */
void FeatureModelBuilder::exitCardinalityGroup(UVLCppParser::CardinalityGroupContext *ctx) {
    auto children = collect_children(ctx->groupSpec());

    if (!children.empty() && !feature_stack.empty()) {
        auto parent = feature_stack.top();

        // Parse cardinality
        std::string cardinality_text = ctx->CARDINALITY()->getText();
        auto [card_min, card_max] = parse_cardinality(cardinality_text);

        // Create Cardinality relation
        parent->add_relation(children, card_min, card_max);
    }
}

/**
 * @brief Finalizes a constraint line by creating a Constraint object
 *
 * Pops the completed AST from ast_stack and creates a Constraint with a
 * unique name. The constraint is added to the feature model.
 *
 * @param ctx Parse tree context for the constraint line
 */
void FeatureModelBuilder::exitConstraintLine(UVLCppParser::ConstraintLineContext *ctx) {
    // The AST for the constraint should be on the ast_stack
    if (!ast_stack.empty() && feature_model) {
        auto ast = ast_stack.top();
        ast_stack.pop();

        // Create constraint with unique name
        std::string constraint_name = "Constraint_" + std::to_string(constraint_counter++);
        auto constraint = std::make_shared<Constraint>(constraint_name, ast);

        feature_model->add_constraint(constraint);
    }
}

/**
 * @brief Handles literal (feature name) in constraints
 * @param ctx Parse tree context containing the feature reference
 */
void FeatureModelBuilder::exitLiteralConstraint(UVLCppParser::LiteralConstraintContext *ctx) {
    std::string literal = get_reference_name(ctx->reference());
    ast_stack.push(std::make_shared<ASTNode>(literal));
}

/**
 * @brief Handles NOT operation in constraints
 * @param ctx Parse tree context for NOT operation
 * @throws std::runtime_error if AST stack is empty
 */
void FeatureModelBuilder::exitNotConstraint(UVLCppParser::NotConstraintContext *ctx) {
    if (ast_stack.empty()) {
        throw std::runtime_error("AST stack underflow in NOT constraint");
    }

    auto operand = ast_stack.top();
    ast_stack.pop();

    ast_stack.push(std::make_shared<ASTNode>(ASTOperation::NOT, operand));
}

/**
 * @brief Handles AND operation in constraints
 * @param ctx Parse tree context for AND operation
 * @throws std::runtime_error if AST stack has fewer than 2 operands
 */
void FeatureModelBuilder::exitAndConstraint(UVLCppParser::AndConstraintContext *ctx) {
    if (ast_stack.size() < 2) {
        throw std::runtime_error("AST stack underflow in AND constraint");
    }

    auto right = ast_stack.top();
    ast_stack.pop();
    auto left = ast_stack.top();
    ast_stack.pop();

    ast_stack.push(std::make_shared<ASTNode>(ASTOperation::AND, left, right));
}

/**
 * @brief Handles OR operation in constraints
 * @param ctx Parse tree context for OR operation
 * @throws std::runtime_error if AST stack has fewer than 2 operands
 */
void FeatureModelBuilder::exitOrConstraint(UVLCppParser::OrConstraintContext *ctx) {
    if (ast_stack.size() < 2) {
        throw std::runtime_error("AST stack underflow in OR constraint");
    }

    auto right = ast_stack.top();
    ast_stack.pop();
    auto left = ast_stack.top();
    ast_stack.pop();

    ast_stack.push(std::make_shared<ASTNode>(ASTOperation::OR, left, right));
}

void FeatureModelBuilder::exitImplicationConstraint(UVLCppParser::ImplicationConstraintContext *ctx) {
    if (ast_stack.size() < 2) {
        throw std::runtime_error("AST stack underflow in IMPLIES constraint");
    }

    auto right = ast_stack.top();
    ast_stack.pop();
    auto left = ast_stack.top();
    ast_stack.pop();

    ast_stack.push(std::make_shared<ASTNode>(ASTOperation::IMPLIES, left, right));
}

void FeatureModelBuilder::exitEquivalenceConstraint(UVLCppParser::EquivalenceConstraintContext *ctx) {
    if (ast_stack.size() < 2) {
        throw std::runtime_error("AST stack underflow in EQUIVALENCE constraint");
    }

    auto right = ast_stack.top();
    ast_stack.pop();
    auto left = ast_stack.top();
    ast_stack.pop();

    ast_stack.push(std::make_shared<ASTNode>(ASTOperation::EQUIVALENCE, left, right));
}

void FeatureModelBuilder::exitParenthesisConstraint(UVLCppParser::ParenthesisConstraintContext *ctx) {
    // Parenthesis doesn't change the AST structure, just grouping
    // The child constraint is already on the stack
}

void FeatureModelBuilder::exitEquationConstraint(UVLCppParser::EquationConstraintContext *ctx) {
    // Equation is already on the stack from exitXxxEquation handler
    // Nothing to do here
}

void FeatureModelBuilder::exitEqualEquation(UVLCppParser::EqualEquationContext *ctx) {
    if (ast_stack.size() < 2) {
        throw std::runtime_error("AST stack underflow in EQUAL equation");
    }

    auto right = ast_stack.top();
    ast_stack.pop();
    auto left = ast_stack.top();
    ast_stack.pop();

    ast_stack.push(std::make_shared<ASTNode>(ASTOperation::EQUALS, left, right));
}

void FeatureModelBuilder::exitLowerEquation(UVLCppParser::LowerEquationContext *ctx) {
    if (ast_stack.size() < 2) {
        throw std::runtime_error("AST stack underflow in LOWER equation");
    }

    auto right = ast_stack.top();
    ast_stack.pop();
    auto left = ast_stack.top();
    ast_stack.pop();

    ast_stack.push(std::make_shared<ASTNode>(ASTOperation::LOWER, left, right));
}

void FeatureModelBuilder::exitGreaterEquation(UVLCppParser::GreaterEquationContext *ctx) {
    if (ast_stack.size() < 2) {
        throw std::runtime_error("AST stack underflow in GREATER equation");
    }

    auto right = ast_stack.top();
    ast_stack.pop();
    auto left = ast_stack.top();
    ast_stack.pop();

    ast_stack.push(std::make_shared<ASTNode>(ASTOperation::GREATER, left, right));
}

void FeatureModelBuilder::exitLowerEqualsEquation(UVLCppParser::LowerEqualsEquationContext *ctx) {
    if (ast_stack.size() < 2) {
        throw std::runtime_error("AST stack underflow in LOWER_EQUALS equation");
    }

    auto right = ast_stack.top();
    ast_stack.pop();
    auto left = ast_stack.top();
    ast_stack.pop();

    ast_stack.push(std::make_shared<ASTNode>(ASTOperation::LOWER_EQUALS, left, right));
}

void FeatureModelBuilder::exitGreaterEqualsEquation(UVLCppParser::GreaterEqualsEquationContext *ctx) {
    if (ast_stack.size() < 2) {
        throw std::runtime_error("AST stack underflow in GREATER_EQUALS equation");
    }

    auto right = ast_stack.top();
    ast_stack.pop();
    auto left = ast_stack.top();
    ast_stack.pop();

    ast_stack.push(std::make_shared<ASTNode>(ASTOperation::GREATER_EQUALS, left, right));
}

void FeatureModelBuilder::exitNotEqualsEquation(UVLCppParser::NotEqualsEquationContext *ctx) {
    if (ast_stack.size() < 2) {
        throw std::runtime_error("AST stack underflow in NOT_EQUALS equation");
    }

    auto right = ast_stack.top();
    ast_stack.pop();
    auto left = ast_stack.top();
    ast_stack.pop();

    ast_stack.push(std::make_shared<ASTNode>(ASTOperation::NOT_EQUALS, left, right));
}

void FeatureModelBuilder::exitAddExpression(UVLCppParser::AddExpressionContext *ctx) {
    if (ast_stack.size() < 2) {
        throw std::runtime_error("AST stack underflow in ADD expression");
    }

    auto right = ast_stack.top();
    ast_stack.pop();
    auto left = ast_stack.top();
    ast_stack.pop();

    ast_stack.push(std::make_shared<ASTNode>(ASTOperation::ADD, left, right));
}

void FeatureModelBuilder::exitSubExpression(UVLCppParser::SubExpressionContext *ctx) {
    if (ast_stack.size() < 2) {
        throw std::runtime_error("AST stack underflow in SUB expression");
    }

    auto right = ast_stack.top();
    ast_stack.pop();
    auto left = ast_stack.top();
    ast_stack.pop();

    ast_stack.push(std::make_shared<ASTNode>(ASTOperation::SUB, left, right));
}

void FeatureModelBuilder::exitMulExpression(UVLCppParser::MulExpressionContext *ctx) {
    if (ast_stack.size() < 2) {
        throw std::runtime_error("AST stack underflow in MUL expression");
    }

    auto right = ast_stack.top();
    ast_stack.pop();
    auto left = ast_stack.top();
    ast_stack.pop();

    ast_stack.push(std::make_shared<ASTNode>(ASTOperation::MUL, left, right));
}

void FeatureModelBuilder::exitDivExpression(UVLCppParser::DivExpressionContext *ctx) {
    if (ast_stack.size() < 2) {
        throw std::runtime_error("AST stack underflow in DIV expression");
    }

    auto right = ast_stack.top();
    ast_stack.pop();
    auto left = ast_stack.top();
    ast_stack.pop();

    ast_stack.push(std::make_shared<ASTNode>(ASTOperation::DIV, left, right));
}

void FeatureModelBuilder::exitFloatLiteralExpression(UVLCppParser::FloatLiteralExpressionContext *ctx) {
    double value = std::stod(ctx->FLOAT()->getText());
    ast_stack.push(std::make_shared<ASTNode>(value));
}

void FeatureModelBuilder::exitIntegerLiteralExpression(UVLCppParser::IntegerLiteralExpressionContext *ctx) {
    int value = std::stoi(ctx->INTEGER()->getText());
    ast_stack.push(std::make_shared<ASTNode>(value));
}

void FeatureModelBuilder::exitStringLiteralExpression(UVLCppParser::StringLiteralExpressionContext *ctx) {
    std::string value = ctx->STRING()->getText();
    // Remove quotes
    if (value.length() >= 2 && value.front() == '\'' && value.back() == '\'') {
        value = value.substr(1, value.length() - 2);
    }
    ast_stack.push(std::make_shared<ASTNode>(value));
}

void FeatureModelBuilder::exitLiteralExpression(UVLCppParser::LiteralExpressionContext *ctx) {
    std::string literal = get_reference_name(ctx->reference());
    ast_stack.push(std::make_shared<ASTNode>(literal));
}

std::pair<int, int> FeatureModelBuilder::parse_cardinality(const std::string& cardinality_text) {
    // Remove brackets: "[1..3]" -> "1..3"
    std::string text = cardinality_text.substr(1, cardinality_text.length() - 2);

    int min_val, max_val;

    // Find ".."
    size_t dot_pos = text.find("..");
    if (dot_pos != std::string::npos) {
        // Range format: "1..3" or "1..*"
        std::string min_str = text.substr(0, dot_pos);
        std::string max_str = text.substr(dot_pos + 2);

        min_val = std::stoi(min_str);

        if (max_str == "*") {
            max_val = -1;  // Unbounded
        } else {
            max_val = std::stoi(max_str);
        }
    } else {
        // Single value: "[3]" means exactly 3
        min_val = std::stoi(text);
        max_val = min_val;
    }

    return {min_val, max_val};
}

std::string FeatureModelBuilder::get_reference_name(UVLCppParser::ReferenceContext *ctx) {
    std::string name = ctx->getText();

    // Remove quotes if present
    if (name.length() >= 2 && name.front() == '"' && name.back() == '"') {
        name = name.substr(1, name.length() - 2);
    }

    return name;
}

std::vector<std::shared_ptr<Feature>> FeatureModelBuilder::collect_children(
    UVLCppParser::GroupSpecContext *ctx) {

    std::vector<std::shared_ptr<Feature>> children;

    // Get all feature contexts from groupSpec
    auto feature_contexts = ctx->feature();

    // Pop children from stack (they were pushed in reverse order)
    std::vector<std::shared_ptr<Feature>> temp;
    for (size_t i = 0; i < feature_contexts.size(); ++i) {
        if (!feature_stack.empty()) {
            // Peek at top to see if it matches
            auto top = feature_stack.top();
            temp.push_back(top);
            feature_stack.pop();
        }
    }

    // Reverse to get correct order
    std::reverse(temp.begin(), temp.end());
    children = temp;

    return children;
}
